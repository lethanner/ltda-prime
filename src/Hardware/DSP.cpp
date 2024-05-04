#include "DSP.h"
#include "tables.h"
#include "shiftreg.h"

const byte _dsp_addr = DSP_I2C_ADDRESS;
const float _smooth_mlt = RTA_SMOOTH_MULTIPLIER;

ADAU1452::ADAU1452()
{
    // инициализировать значения всех фейдеров
    memset(&faderPosition_dB, 0, DSP_FADER_COUNT);
    memset(&sendFaders_dB, 0, DSP_BUS_COUNT * DSP_BUS_CHANNELS);
    // инициализировать все флаги MUTE
    memset(&muteFlags, 0, DSP_FADER_COUNT);
    memset(&sendMuteFlags, 0, DSP_BUS_COUNT * DSP_BUS_CHANNELS);

    // вкл синхронизации громкости блютуза с громкостью на DSP
    avrcp_volume_sync = new A2DPExternalVolumeControl(this);
    bluetooth.set_volume_control(avrcp_volume_sync);
}

int* ADAU1452::getFlagRegisterPtr()
{
    return &flagRegister;
}

// запуск и инициализация аудиопроцессора
void ADAU1452::init()
{
    // сброс
    shifters.setOnBoardBit(SHIFT_DSP_RESET, true);
    vTaskDelay(100 / portTICK_PERIOD_MS);
    shifters.setOnBoardBit(SHIFT_DSP_RESET, false);

    // ожидание запуска
    while (getCoreState() == 0x00) {
        // на деле этот цикл скорее всего нифига не выполняется.
        // ибо SigmaDSP возвращает 0x03 как при работе, так и
        // когда ещё не полностью завёлся.
        vTaskDelay(20 / portTICK_PERIOD_MS);
    }
    vTaskDelay(900 / portTICK_PERIOD_MS);  // контрольная задержка

    // https://ez.analog.com/dsp/sigmadsp/f/q-a/108225/spdif-src-dc-offset/320804
    // костыль с переключением ASRC, который был предложен лично одним из представителей Analog Devices.
    // для того чтобы избавиться от постоянки при отсутствии клоков ¯\_(ツ)_/¯.
    // по дефолту все три ASRC натравлены на I2S-выход с ESP32, который работает всегда так же, как магнит в МРТ (шучу).
    // это помогает обнулить ASRC, после чего можно с помощью команд переключить их туда, куда надо

    // ASRC 2 (на нём висит USB), тупо переключаем номер serial input
    gotoRegister(0xF102);
    Wire.write(0x00);
    Wire.write(0xA1);
    Wire.endTransmission();

    // ASRC 0, переключаем род работы с serial input на S/PDIF receiver
    gotoRegister(0xF100);
    Wire.write(0x00);
    Wire.write(0x83);
    Wire.endTransmission();

    // далее расставляем MUTE на то, что изначально должно быть в MUTE
    toggleMute(FADER_REVERB_ST);
    for (byte i = 0; i < DSP_IN_TO_BUS; i++) {
        toggleMute(i, SEND_TO_REVERB);
    }

    // контрольная задержка
    vTaskDelay(100 / portTICK_PERIOD_MS);
    // и только после всего включаем звук на Master
    setDecibelFaderPosition(FADER_MASTER_ST, 0);

    // кэширование всех значений громкости из DSP
    // для их восстановления в случае перезагрузки контроллера
    // ПС: оно пока что нихрена нормально не работает.
    // int32_t val = 0;
    // for (byte i = 0; i < DSP_FADER_COUNT; i++) {
    //     gotoRegister(dsp_fader_address[i], 4);
    //     for (byte j = 0; j < 4; j++) {
    //         val += Wire.read() << (24 - (j * 8));
    //     }
    //     faderPosition[i] = val;
    //     faderPosition_dB[i] = findValue(db_calibration_24bit, 107, val) - 97;
    // }
}

// функция выполнения запроса к 16-бит регистру аудиопроцессора
void ADAU1452::gotoRegister(short reg, byte requestSize)
{
    Wire.beginTransmission(_dsp_addr);
    Wire.write(highByte(reg));
    Wire.write(lowByte(reg));

    if (requestSize > 0) {
        Wire.endTransmission(false);
        Wire.requestFrom(_dsp_addr, requestSize);
    }
}

void ADAU1452::writeAsFloat(short reg, byte value)
{
    int ig = value / 10;
    int fr = value % 10;

    gotoRegister(reg);
    Wire.write(ig & 0xFF);
    for (byte i = 0; i < 3; i++) {
        Wire.write(sigmastudio_fractions[fr][i]);
    }
    Wire.endTransmission();
}

// получение состояния ядра аудиопроцессора
byte ADAU1452::getCoreState()
{
    gotoRegister(DSP_CORE_STATUS_REG, 2);
    Wire.read();  // пропустить старший байт регистра
    return Wire.read();
}

// обновление значений уровня сигнала всех каналов
void ADAU1452::retrieveRTAValues()
{
    for (byte i = 0; i < DSP_READBACK_COUNT; i++) {
        int32_t value = 0;
        // из-за того, что SigmaDSP не всегда делает адреса блоков порядковыми,
        // пришлось убрать burst-чтение за одну передачу адреса регистра и
        // на каждую итерацию сделать передачу нового адреса. :angry:
        gotoRegister(dsp_readback_addr[i], 4);
        for (byte j = 0; j < 4; j++) {
            value += Wire.read() << (24 - (j * 8));
        }

        readbackVal[i] = (value < 0 ? -value : value);
        // запаздывающий фильтр (код нагло украден из проекта спектроанализатора от AlexGyver)
        readbackVal[i] = readbackVal_old[i] * _smooth_mlt + readbackVal[i] * (1 - _smooth_mlt);
        readbackVal_old[i] = readbackVal[i];
    }
}

// установка уровня громкости канала (в децибелах от -97 до 10, где -97 = MUTE)
void ADAU1452::setDecibelFaderPosition(byte id, int8_t val, bool sync)
{
    val = constrain(val, -97, 10);
    uint32_t _val = muteFlags[id] ? 0 : db_calibration_24bit[97 + val];
    
    if (sync && id == FADER_BLUETOOTH_ST)
        bluetooth.sendAVRCPVolume(val);
    
    gotoRegister(dsp_fader_address[id]);
    for (byte i = 0; i < 4; i++) {
        Wire.write((_val >> (24 - (i * 8))) & 0xFF);
    }
    Wire.endTransmission();

    faderPosition_dB[id] = val;
}

// установка уровня посыла канала на шину (в децибелах от -97 до 10, где -97 = MUTE)
void ADAU1452::setDecibelSendLevel(byte id, byte to, int8_t val)
{
    val = constrain(val, -97, 10);
    uint32_t _val = sendMuteFlags[to][id] ? 0 : db_calibration_24bit[97 + val];

    for (byte j = 0; j < 2; j++) {
        gotoRegister(dsp_bus_send_addr[to][(id * 2) + j]);
        for (byte i = 0; i < 4; i++) {
            Wire.write((_val >> (24 - (i * 8))) & 0xFF);
        }
        Wire.endTransmission();
    }

    sendFaders_dB[to][id] = val;
}

void ADAU1452::toggleMute(byte id)
{
    muteFlags[id] = !muteFlags[id];
    setDecibelFaderPosition(id, faderPosition_dB[id], false);
}

void ADAU1452::toggleMute(byte id, byte to)
{
    sendMuteFlags[to][id] = !sendMuteFlags[to][id];
    setDecibelSendLevel(id, to, sendFaders_dB[to][id]);
}

// поиск ID ближайшего значения в массиве (для конвертации значения уровня в децибелы)
byte ADAU1452::findValue(const unsigned int* tab, byte max, int value)
{
    for (byte i = 0; i < max; i++) {
        if (value >= tab[i] && value < tab[i + 1])
            return i;
    }
    return max;
}

// получение децибельного уровня сигнала на канале (согласно подаваемой калибровочной таблице)
byte ADAU1452::getRelativeSignalLevel(const unsigned int* tab, byte max, byte id, bool right)
{
    return findValue(tab, max, readbackVal[(id * 2) + static_cast<byte>(right)]);
}

// переключение режима bassboost
void ADAU1452::toggleBassBoost()
{
    bitToggle(flagRegister, DSPSETS_IS_BASSBOOSTED);
    bool state = !bitRead(flagRegister, DSPSETS_IS_BASSBOOSTED);

    gotoRegister(DSP_BASSBOOST_REG);
    Wire.write(state);
    for (byte i = 0; i < 3; i++) {
        Wire.write(0x00);
    }
    Wire.endTransmission();
}

void ADAU1452::setBBGain(byte value)
{
    value = constrain(value, 1, 30);
    
    writeAsFloat(DSP_BB_GAIN_REG, value);
    bassboostGain = value;
}

void ADAU1452::setBBIntensity(byte value)
{
    value = constrain(value, 1, 30);

    writeAsFloat(DSP_BB_INTENSITY_REG, value);
    bassboostIntensity = value;
}

void ADAU1452::setReverbTime(byte value)
{
    value = constrain(value, 0, 3);

    gotoRegister(DSP_REVERB_TIME_REG);
    for (byte i = 0; i < 8; i++) {
        Wire.write(ss_reverbtime[value][i]);
    }
    Wire.endTransmission();

    reverbTime = value;
}

void ADAU1452::setReverbHFDamping(byte value)
{
    value = constrain(value, 0, 2);

    gotoRegister(DSP_REVERB_HFDAMP_REG);
    for (byte i = 0; i < 4; i++) {
        Wire.write(ss_rev_hf_damping[value][i]);
    }
    Wire.endTransmission();

    reverbHFDamp = value;
}

void ADAU1452::setReverbBassGain(byte value)
{
    value = constrain(value, 0, 2);
    
    gotoRegister(DSP_REVERB_BGAIN_REG);
    for (byte i = 0; i < 4; i++) {
        Wire.write(ss_rev_bass_gain[value][i]);
    }
    Wire.endTransmission();

    reverbBassGain = value;
}

ADAU1452 DSP;