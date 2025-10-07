#include "shiftreg.h"

SemaphoreHandle_t _sspi_lock = NULL;
const char* const ShiftRegisters::gainValuesAliases[] = {
    "0 dB", "+6 dB", "+12 dB", "+20 dB", "+30 dB", "+40 dB", "+50 dB", "+60 dB"
};

ShiftRegisters::ShiftRegisters() { _sspi_lock = xSemaphoreCreateMutex(); }

void ShiftRegisters::quickInit()
{
    pinMode(SHIFT_DAT, OUTPUT);  // сигнал данных
    pinMode(SHIFT_LAT, OUTPUT);  // сигнал защелки
    pinMode(SHIFT_CLK, OUTPUT);  // сигнал тактирования

    // БЫСТРО сбрасываем первый регистр, пока у него не поднялся Output Enable
    // на остальных ранний запуск не такой уж и страшный
    gio::write(SHIFT_LAT, false);
    gio::shift::send_byte(SHIFT_DAT, SHIFT_CLK, LSBFIRST, SHIFT_MAIN_DEFAULT, 1);
    gio::write(SHIFT_LAT, true);

    _buffer[0] = SHIFT_MAIN_DEFAULT;
}

void ShiftRegisters::refresh()
{
    // в этом участке кода неверная работа с семафором приводила к лагам при УПРАВЛЕНИИ ГРОМКОСТЬЮ ПО I2C.
    // чудеса, не иначе
    if (xSemaphoreTake(_sspi_lock, portMAX_DELAY) == pdTRUE) {
        // отправить данные на сдвиговые регистры
        // под семафором во избежание конфликтов при вызове из нескольких потоков
        gio::write(SHIFT_LAT, false);
        gio::shift::send(SHIFT_DAT, SHIFT_CLK, LSB_REVERSE, _buffer, SHIFT_COUNT, 1);
        gio::write(SHIFT_LAT, true);

        xSemaphoreGive(_sspi_lock);
    }
}

void ShiftRegisters::sendToIndicators(int buf)
{
    // цикл for вообще есть смысл делать?
    _buffer[4] = (buf >> 16) & 0xFF;
    _buffer[5] = (buf >> 8) & 0xFF;
    _buffer[6] = (buf) & 0xFF;
    refresh();
}

void ShiftRegisters::setAuxGain(DSPChannels::channel ch, int8_t value)
{
    if (ch >= DSPChannels::AUX1 && ch <= DSPChannels::AUX6) {
        value = constrain(value, 0, 7);
        // reverse bit order
        byte value_rev = ((value & 0x01) << 2) | ((value & 0x02) << 0) | ((value & 0x04) >> 2);

        uint8_t auxChannelId = ch - DSPChannels::AUX1;
        //byte *arrayStart = &_buffer[1];
        uint8_t byteNo = 1 + (auxChannelId / 2);

        //byte bitmaskOffset = auxChannelId * 4;
        //*arrayStart &= ~(0x70 >> bitmaskOffset);
        //*arrayStart |= ((value_rev << 4) & 0x70) >> bitmaskOffset;
        byte bitmaskOffset = (auxChannelId % 2 == 0) ? 0 : 4;
        _buffer[byteNo] &= ~(0x70 >> bitmaskOffset);
        _buffer[byteNo] |= ((value_rev << 4) & 0x70) >> bitmaskOffset;

        _auxGains[auxChannelId] = value;
        refresh();
    }
}

void ShiftRegisters::setClippingIndicator(DSPChannels::channel ch, bool value)
{
    if (ch >= DSPChannels::AUX1 && ch <= DSPChannels::AUX6) {}
}

int8_t* ShiftRegisters::getAuxGainPtr(DSPChannels::channel ch)
{
    if (ch >= DSPChannels::AUX1 && ch <= DSPChannels::AUX6) {
        return &_auxGains[ch - DSPChannels::AUX1];
    }

    return NULL;
}

void ShiftRegisters::setOnBoardBit(uint8_t bit, bool state)
{
    bitWrite(_buffer[0], bit, state);
    refresh();
}

ShiftRegisters shifters;