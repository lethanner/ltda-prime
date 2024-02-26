#include "DSP.h"
#include "decibels.h"

const byte _dsp_addr = DSP_I2C_ADDRESS;
const float _smooth_mlt = RTA_SMOOTH_MULTIPLIER;

A2DPSyncedVolumeControl avrcp_volume_sync;

ADAU1452::ADAU1452()
{
    memset(&faderPosition, 0xFFFFFF, DSP_FADER_COUNT * 4);
    memset(&faderPositionDB, 0, DSP_FADER_COUNT);

    bluetooth.set_volume_control(&avrcp_volume_sync);
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

void ADAU1452::setFaderPosition(byte id, int val)
{
    gotoRegister(dsp_fader_address[id]);
    for (byte i = 0; i < 4; i++) {
        Wire.write((val >> (24 - (i * 8))) & 0xFF);
    }
    Wire.endTransmission();

    faderPosition[id] = val;
    faderPositionDB[id] = findValue(db_calibration_24bit, 107, val) - 97;
}

void ADAU1452::setDecibelFaderPosition(byte id, int8_t val, bool sync)
{
    val = constrain(val, -97, 10);
    if (sync && id == FADER_BLUETOOTH_ST)
        bluetooth.sendAVRCPVolume(val);
    setFaderPosition(id, db_calibration_24bit[97 + val]);
}

// int8_t ADAU1452::getDecibelSignalLevel(byte id, bool right)
// {
//     return getRelativeSignalLevel(db_calibration_24bit, 97, id, right) - 97;
// }

// int8_t ADAU1452::getDecibelFaderPosition(byte id)
// {
//     return convertToDecibels(faderPosition[id]);
// }

byte ADAU1452::findValue(const unsigned int* tab, byte max, int value)
{
    for (byte i = 0; i < max; i++) {
        if (value >= tab[i] && value < tab[i + 1])
            return i;
    }
    return max;
}

byte ADAU1452::getRelativeSignalLevel(const unsigned int* tab, byte max, byte id, bool right)
{
    return findValue(tab, max, readbackVal[(id * 2) + static_cast<byte>(right)]);
}

ADAU1452 DSP;