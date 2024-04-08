#include "DSP.h"
#include "decibels.h"

const byte _dsp_addr = DSP_I2C_ADDRESS;
const float _smooth_mlt = RTA_SMOOTH_MULTIPLIER;

ADAU1452::ADAU1452()
{
    memset(&faderPosition_dB, 0, DSP_FADER_COUNT);
    memset(&sendFaders_dB, 0, DSP_BUS_COUNT * (DSP_BUS_CHANNELS / 2));

    // вкл синхронизации громкости блютуза с громкостью на DSP
    avrcp_volume_sync = new A2DPExternalVolumeControl(this);
    bluetooth.set_volume_control(avrcp_volume_sync);
}

// запуск и инициализация аудиопроцессора
void ADAU1452::init()
{
    // сброс
    gio::write(DSP_RESET, false);
    pinMode(DSP_RESET, OUTPUT);
    vTaskDelay(100 / portTICK_PERIOD_MS);
    gio::write(DSP_RESET, true);

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
    uint32_t _val = db_calibration_24bit[97 + val];
    
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
    uint32_t _val = db_calibration_24bit[97 + val];

    for (byte j = 0; j < 2; j++) {
        gotoRegister(dsp_bus_send_addr[to][(id * 2) + j]);
        for (byte i = 0; i < 4; i++) {
            Wire.write((_val >> (24 - (i * 8))) & 0xFF);
        }
        Wire.endTransmission();
    }

    sendFaders_dB[to][id] = val;
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

ADAU1452 DSP;