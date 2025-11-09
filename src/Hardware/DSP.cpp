#include "DSP.h"
#include "lut.h"
#include "shiftreg.h"
#include "sigmastudio/prime_IC_1_PARAM.h"
#include "sigmastudio/prime_IC_1_REG.h"

ADAU1452::ADAU1452()
{
    // вкл синхронизации громкости блютуза с громкостью на DSP
    avrcp_volume_sync = new A2DPExternalVolumeControl(this);
    bluetooth.set_volume_control(avrcp_volume_sync);
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
    gotoRegister(REG_ASRC_INPUT2_IC_1_ADDR);
    Wire.write(0x00);
    Wire.write(0xA1);
    Wire.endTransmission();

    // ASRC 0, переключаем род работы с serial input на S/PDIF receiver
    gotoRegister(REG_ASRC_INPUT0_IC_1_ADDR);
    Wire.write(0x00);
    Wire.write(0x83);
    Wire.endTransmission();

    // контрольная задержка
    vTaskDelay(100 / portTICK_PERIOD_MS);
    // и только после всего включаем звук на Master
    setDecibelFaderPosition(DSPChannels::MASTER, 0);

    // TODO: во избежание неожиданно громкого звука при включении
    // следовало бы, конечно, держать Master в MUTE
    // но в то же время необходимость ещё и ручками включать звук
    // после запуска - дело тоже не царское
}

// функция выполнения запроса к 16-бит регистру аудиопроцессора
void ADAU1452::gotoRegister(__register reg, byte requestSize)
{
    // TODO: решить вопрос с возможными ложными вызовами
    // Wire.write() после этой функции
    if (reg == 0) return;

    Wire.beginTransmission(DSP_I2C_ADDRESS);
    Wire.write(highByte(reg));
    Wire.write(lowByte(reg));

    if (requestSize > 0) {
        Wire.endTransmission(false);
        Wire.requestFrom(DSP_I2C_ADDRESS, requestSize);
    }
}

void ADAU1452::writeAsFloat(__register reg, byte value)
{
    int ig = value / 10;
    int fr = value % 10;

    gotoRegister(reg);
    Wire.write(ig & 0xFF);
    for (byte i = 0; i < 3; i++) {
        Wire.write(LUT::fractions[fr][i]);
    }
    Wire.endTransmission();
}

// получение состояния ядра аудиопроцессора
byte ADAU1452::getCoreState()
{
    gotoRegister(REG_CORE_STATUS_IC_1_ADDR, 2);
    Wire.read();  // пропустить старший байт регистра
    return Wire.read();
}

// обновление значений уровня сигнала всех каналов
void ADAU1452::retrieveRTAValues()
{
    for (byte i = 0; i < DSP_CHANNEL_COUNT; i++) {
        for (byte k = 0; k < 2; k++) {
            DSPChannels::Channel *ch = DSPChannels::list[i];
            // если канала не существует (правого для моно) - пропускаем
            if (ch->readback[k] == 0) continue;

            int32_t value = 0;
            // из-за того, что SigmaDSP не всегда делает адреса блоков порядковыми,
            // пришлось убрать burst-чтение за одну передачу адреса регистра и
            // на каждую итерацию сделать передачу нового адреса. :angry:
            if (ch->usingInternalADC) {
                gotoRegister(ch->readback[k], 2);
                value += Wire.read() << 8;
                value += Wire.read();
                ch->readbackVal[k] = value << 14;
            } else {
                gotoRegister(ch->readback[k], 4);
                for (byte j = 0; j < 25; j += 8) {
                    value += Wire.read() << (24 - j);
                }
                ch->readbackVal[k] = (value < 0 ? -value : value);
            }
            
            // запаздывающий фильтр (код нагло украден из проекта спектроанализатора от AlexGyver)
            ch->readbackVal[k] = ch->readbackVal_old[k] * rta_multiplier + ch->readbackVal[k] * (1 - rta_multiplier);
            ch->readbackVal_old[k] = ch->readbackVal[k];
        }
    }
}

// установка уровня громкости канала (в децибелах от -97 до 10, где -97 = MUTE)
void ADAU1452::setDecibelFaderPosition(channel id, decibel val, bool sync)
{
    DSPChannels::Channel *const ch = DSPChannels::list[id];
    val = constrain(val, -97, 10);

    // если MUTE, то значение принудительно 0
    uint32_t _val = ch->mute ? 0 : LUT::db_24bit[97 + val];

    // пересчитываем значения фейдеров L-R в соответствии со значением стереобаланса
    // если balpan == -50, значение уровня левого канала в разах увеличится в 2 раза (+6 дБ),
    // а значение уровня правого канала станет равно 0. для balpan == 50 всё ровно наоборот
    uint32_t values[2];
    if (ch->balpan == 0) values[0] = values[1] = _val;
    else {
        float coeff = 0.02 * ch->balpan;
        values[0] = _val * (1.0 - coeff);
        values[1] = _val * (1.0 + coeff);
    }

    // отправить новый уровень на DSP (сразу левый+правый)
    for (byte j = 0; j < 2; j++) {
        gotoRegister(ch->fader[j]);
        for (byte i = 0; i < 4; i++) {
            Wire.write((values[j] >> (24 - (i * 8))) & 0xFF);
        }
        Wire.endTransmission();
    }

    if (id == DSPChannels::MASTER && bitRead(flagRegister, DSPSETS_MASTER_SUB_SYNC)) {
        decibel submixLevel = getFaderPosition(DSPChannels::SUBMIX);
        decibel delta = val - ch->faderPosition;
        setDecibelFaderPosition(DSPChannels::SUBMIX, submixLevel + delta, false);
    }

    ch->faderPosition = val;

    // синхронизация с блютуз устройством, если регулировали его
    if (sync && id == DSPChannels::BLUETOOTH) bluetooth.sendAVRCPVolume(val);
}

// установка уровня посыла канала на шину (в децибелах от -97 до 10, где -97 = MUTE)
void ADAU1452::setDecibelSendLevel(channel id, bus to, decibel val)
{
    DSPChannels::SendTo *const send = &DSPChannels::list[id]->sends[to];
    val = constrain(val, -97, 10);

    // если MUTE, то значение принудительно 0
    uint32_t _val = send->mute ? 0 : LUT::db_24bit[97 + val];

    // TODO: быть может, прикрутить управление панорамой и сюда тоже?
    for (byte j = 0; j < 2; j++) {
        gotoRegister(send->fader[j]);
        for (byte i = 0; i < 4; i++) {
            Wire.write((_val >> (24 - (i * 8))) & 0xFF);
        }
        Wire.endTransmission();
    }

    if (to == DSPChannels::BUS_MASTER && bitRead(flagRegister, DSPSETS_MASTER_SUB_SYNC)) {
        decibel submixLevel = getFaderPosition(id, DSPChannels::BUS_SUBMIX);
        decibel delta = val - send->faderPosition;
        setDecibelSendLevel(id, DSPChannels::BUS_SUBMIX, submixLevel + delta);
    }

    send->faderPosition = val;
}

void ADAU1452::toggleMute(channel id)
{
    DSPChannels::Channel *const ch = DSPChannels::list[id];

    ch->mute = !ch->mute;
    setDecibelFaderPosition(id, ch->faderPosition, false);
}

void ADAU1452::toggleMute(channel id, bus to)
{
    DSPChannels::SendTo *const send = &DSPChannels::list[id]->sends[to];

    send->mute = !send->mute;
    setDecibelSendLevel(id, to, send->faderPosition);
}

// поиск ID ближайшего значения в массиве (для конвертации значения уровня в децибелы)
byte ADAU1452::findValue(const unsigned int *tab, byte max, int value)
{
    for (byte i = 0; i < max; i++) {
        if (value >= tab[i] && value < tab[i + 1]) return i;
    }
    return max;
}

// получение децибельного уровня сигнала на канале (согласно подаваемой калибровочной таблице)
byte ADAU1452::getRelativeSignalLevel(const unsigned int *tab, byte max, channel id, bool right)
{
    byte lr = isMonoChannel(id) ? 0 : static_cast<byte>(right);
    return findValue(tab, max, DSPChannels::list[id]->readbackVal[lr]);
}

// переключение режима bassboost
void ADAU1452::toggleBassBoost()
{
    bitToggle(flagRegister, DSPSETS_IS_BASSBOOSTED);
    bool state = !bitRead(flagRegister, DSPSETS_IS_BASSBOOSTED);

    gotoRegister(MOD_MASTER_BASSBOOST_ALG0_SUPERBASSALGSTEREOS3001BYPASS_ADDR);
    Wire.write(state);
    for (byte i = 0; i < 3; i++) {
        Wire.write(0x00);
    }
    Wire.endTransmission();
}

void ADAU1452::setBBGain(byte value)
{
    value = constrain(value, 1, 30);

    writeAsFloat(MOD_MASTER_BASSBOOST_ALG0_SUPERBASSALGSTEREOS3001POSTGAIN_ADDR, value);
    bassboostGain = value;
}

void ADAU1452::setBBIntensity(byte value)
{
    value = constrain(value, 1, 30);

    writeAsFloat(MOD_MASTER_BASSBOOST_ALG0_SUPERBASSALGSTEREOS3001K_ADDR, value);
    bassboostIntensity = value;
}

void ADAU1452::setReverbTime(byte value)
{
    value = constrain(value, 0, 3);

    gotoRegister(MOD_REVERB_PROC_ALG0_REVERBS3001LOOPGAIN_ADDR);
    for (byte i = 0; i < 8; i++) {
        Wire.write(LUT::reverbtime[value][i]);
    }
    Wire.endTransmission();

    reverbTime = value;
}

void ADAU1452::setReverbHFDamping(byte value)
{
    value = constrain(value, 0, 2);

    gotoRegister(MOD_REVERB_PROC_ALG0_REVERBS3001LP_ADDR);
    for (byte i = 0; i < 4; i++) {
        Wire.write(LUT::rev_hf_damping[value][i]);
    }
    Wire.endTransmission();

    reverbHFDamp = value;
}

void ADAU1452::setReverbBassGain(byte value)
{
    value = constrain(value, 0, 2);

    gotoRegister(MOD_REVERB_PROC_ALG0_REVERBS3001BASSGAIN_ADDR);
    for (byte i = 0; i < 4; i++) {
        Wire.write(LUT::rev_bass_gain[value][i]);
    }
    Wire.endTransmission();

    reverbBassGain = value;
}

void ADAU1452::setPitchBusShift(int8_t value)
{
    value = constrain(value, -16, +16);
    int32_t _val = SEMITONE_INCREMENT * value;

    gotoRegister(MOD_PITCH_PROC_ALG0_PITCHSHIFTSADAU145XALG1FREQ_ADDR);
    for (byte i = 0; i < 4; i++) {
        Wire.write((_val >> (24 - (i * 8))) & 0xFF);
    }
    Wire.endTransmission();

    pitch_shift = value;
}

void ADAU1452::setStereoBalance(channel id, int8_t val)
{
    DSPChannels::Channel *const ch = DSPChannels::list[id];

    ch->balpan = constrain(val, -50, 50);
    setDecibelFaderPosition(id, ch->faderPosition, false);
}

// установка стереорежима для стереоканала
// либо "нормальный", либо принудительное моно, либо вычитание стереоканалов
// (вычитание стереоканалов иногда используется для подавления вокала)
void ADAU1452::setStereoMode(channel id, DSPChannels::StereoMode mode)
{
    __register reg = DSPChannels::list[id]->stereoMode;

    // не для всех каналов поддерживается переключение режима
    if (reg == 0) return;

    if (id == DSPChannels::MASTER && biAmpMode == LF_LEFT_HF_RIGHT)
        mode = DSPChannels::FORCEMONO;

    gotoRegister(reg);
    for (byte i = 0; i < 3; i++) {
        Wire.write(0x00);
    }
    Wire.write(mode * 2);
    Wire.endTransmission();

    DSPChannels::list[id]->curStereoMode = mode;
}

void ADAU1452::setRTASmoothing(byte value)
{
    value = constrain(value, 1, 9);

    rta_smoothing = value;
    rta_multiplier = value / 10.0;
}

void ADAU1452::setBiAmpMode(BiAmpMode mode)
{
    biAmpMode = mode;
    setStereoMode(DSPChannels::MASTER);

    bitWrite(flagRegister, DSPSETS_MASTER_SUB_SYNC, mode == LF_MASTER_HF_SUBMIX);

    gotoRegister(MOD_BIAMP_MODE_STEREOMUXSIGMA300NS41INDEX_ADDR);
    for (byte i = 0; i < 3; i++) {
        Wire.write(0x00);
    }
    Wire.write(mode * 4);
    Wire.endTransmission();
}

void ADAU1452::toggleMasterSubSync()
{  
    bitToggle(flagRegister, DSPSETS_MASTER_SUB_SYNC);
    // точно ли нужно принудительно выставлять уровни, если я все равно
    // смогу подстраивать балансировку между Master и Submix?
    /*if (sync) {
        for (byte i = 0; i < DSP_CHANNEL_COUNT; i++) {
            channel ch = static_cast<channel>(i);
            setDecibelSendLevel(ch, DSPChannels::BUS_SUBMIX, getFaderPosition(ch, DSPChannels::BUS_MASTER));
        }
        setDecibelFaderPosition(DSPChannels::SUBMIX, getFaderPosition(DSPChannels::MASTER));
    }*/
}

ADAU1452 DSP;