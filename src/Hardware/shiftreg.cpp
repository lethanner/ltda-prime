#include "shiftreg.h"

SemaphoreHandle_t _sspi_lock = NULL;

ShiftRegisters::ShiftRegisters()
{
    _sspi_lock = xSemaphoreCreateMutex();
}

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
    _buffer[1] = (buf >> 16) & 0xFF;
    _buffer[2] = (buf >> 8) & 0xFF;
    _buffer[3] = (buf) & 0xFF;
    refresh();
}

void ShiftRegisters::setOnBoardBit(uint8_t bit, bool state)
{
    bitWrite(_buffer[0], bit, state);
    refresh();
}

ShiftRegisters shifters;