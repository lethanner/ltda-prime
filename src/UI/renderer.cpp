#include "UI.h"
#include "channelmap.h"
#include "decibels.h"

extern GyverOLED<SSD1306_128x64, OLED_BUFFER> screen;

/*
 * =========== Отрисовщики активностей ===========
*/

// отрисовщик виртуального микшерного пульта
void LTDAUI::renderMixingConsole()
{
    byte label_offset = gap_block / 2;
    for (byte ch = 0; ch < _chan_count; ch++) {
        byte block_width = 18;  // TODO: разделение каналов на моно и стерео
        byte block_safe_zone = block_width + gap_block;
        byte x_coord = gap_block + (ch * block_safe_zone);
        byte fader_pos = map(DSP.faderPositionDB[onScreenChannels[ch]], -97, 0, 52, 11);
        byte levelL = 52 - DSP.getRelativeSignalLevel(db_calibr_onscreen, 42, onScreenChannels[ch], false);
        byte levelR = 52 - DSP.getRelativeSignalLevel(db_calibr_onscreen, 42, onScreenChannels[ch], true);
        //byte levelL = map(DSP.getDecibelSignalLevel(onScreenChannels[ch], false), -97, 0, 52, 11);
        //byte levelR = map(DSP.getDecibelSignalLevel(onScreenChannels[ch], true), -97, 0, 52, 11);

        screen.line(x_coord + 2, 52, x_coord + 2, 11);                                                           // полоска фейдера
        screen.rect(x_coord, fader_pos, x_coord + 4, fader_pos + static_cast<byte>(onScreenChSelect == ch), 2);  // ручка фейдера
        screen.rect(x_coord + 8, 52, x_coord + 11, levelL, OLED_FILL);                                           // столбик уровня левого канала
        screen.rect(x_coord + 13, 52, x_coord + 16, levelR, OLED_FILL);                                          // столбик уровня правого канала

        // подписи каналов
        if (screenState == 0) {
            byte label_len = strlen(chan_labels[onScreenChannels[ch]]);
            byte max_label_chars = (block_safe_zone / 6 < label_len) ? block_safe_zone / 6 : label_len;
            screen.setCursorXY(label_offset + ((block_safe_zone - (6 * max_label_chars)) / 2), 56);
            for (byte i = 0; i < max_label_chars; i++) {
                screen.write(chan_labels[onScreenChannels[ch]][i]);
            }
            label_offset += block_safe_zone;
        }
    }

    switch (screenState) {
    case 1:
        if (onScreenChSelect > 0)
            printXY("<", 56, 0);
        if (onScreenChSelect < _chan_count - 1)
            printXY(">", 56, 122);
        printXY(chan_labels[onScreenChannels[onScreenChSelect]], 56);
        break;
    case 2:
        if (selectedGroup > 0)
            printXY("<<<", 56, 0);
        if (selectedGroup < CH_GROUP_COUNT - 1)
            printXY(">>>", 56, 110);
        printXY(grp_labels[selectedGroup], 56);
        break;
    }
}

// отрисовщик меню
void LTDAUI::renderMenu()
{
    // заголовок
    screen.invertText(1);
    screen.rect(0, 0, 127, 7, OLED_FILL);
    screen.setCursor(_title_x_coord, 0);
    screen.print(_entries[0]);

    // сами пункты
    uint8_t currentLine = 1;
    byte endLine = ((_entryCount < 7) ? _entryCount + 1 : 7) + menuEntryRendererStartId;
    for (byte line = menuEntryRendererStartId; line < endLine; line++) {
        const char *ptr = _entries[line + 1];

        byte flagId = 0;
        bool isBoolEntry = false;
        if (ptr[0] == '?') {
            flagId = ptr[1] - 65;
            isBoolEntry = true;
            ptr += 2;
        }

        screen.setCursor(0, currentLine);
        screen.invertText(line == menuChooseId);
        screen.print(ptr);

        // отображение плюсиков и минусиков возле boolean-пунктов меню
        if (isBoolEntry) {
            screen.setCursor(120, currentLine);
            if ((*_menuBooleans >> flagId) & 0x01)
                screen.write('+');
            else
                screen.write('-');
        }
        currentLine++;
    }
    screen.invertText(0);
}

// отправка данных на сдвиговики на внешнем индикаторе уровня
void LTDAUI::streamMonitorData()
{
    // не кидайтесь помидорами! я знаю, что это можно реализовать через
    // возведение двойки в степень! с технической точки зрения такое решение
    // однозначно быстрее и удобнее, а с 520 килобайтами оперативки
    // вообще не жалко ради оптимизации лишний раз занять 26 байт.
    // это не DA50X всего с двумя килобайтами.
    const DRAM_ATTR static short ledbitmap[13] = {
        0b000000000000, 0b100000000000, 0b110000000000, 0b111000000000,
        0b111100000000, 0b111110000000, 0b111111000000, 0b111111100000,
        0b111111110000, 0b111111111000, 0b111111111100, 0b111111111110,
        0b111111111111
    };
    // Максим, поменяй 7 на 12 после допайки светодиодиков!
    short dataL = ledbitmap[DSP.getRelativeSignalLevel(db_calibr_ledmonitor, 12, monitorChannel, false)];
    short dataR = ledbitmap[DSP.getRelativeSignalLevel(db_calibr_ledmonitor, 12, monitorChannel, true)];
    int shiftDispCache = dataL << 12 | dataR;

    gio::write(4, false);
    gio::shift::send_byte(15, 16, LSBFIRST, (shiftDispCache & 0xFF), 1);
    gio::shift::send_byte(15, 16, LSBFIRST, ((shiftDispCache >> 8) & 0xFF), 1);
    gio::shift::send_byte(15, 16, LSBFIRST, ((shiftDispCache >> 16) & 0xFF), 1);
    gio::write(4, true);
}

/*
 * =========== Вспомогательные функции для дисплея ===========
*/

// вывод текста с выравниванием посередине и очисткой строки
void LTDAUI::printStatus(const char *text, byte y_coord)
{
    screen.clear(0, y_coord, 127, y_coord + 7);
    screen.setCursorXY(getCenterCoordinate(text), y_coord);
    screen.print(text);
    screen.update();
}

// вывод текста сразу по координатам (с возможностью выравнивания в центре)
void LTDAUI::printXY(const char *text, byte y_coord, int8_t x_coord)
{
    screen.setCursorXY(x_coord < 0 ? getCenterCoordinate(text) : x_coord, y_coord);
    screen.print(text);
}

void LTDAUI::printValue(int8_t value, const char *label, int8_t x_coord, byte y_coord)
{
    if (x_coord < 0) {
        byte length = strlen(label) + 1;
        if (value < -99) length += 3;
        else if (value < -9 || value > 99) length += 2;
        else if (value < 0 || value > 9) length += 1;

        x_coord = 128 - (length * 6);
    }

	screen.setCursorXY(x_coord, y_coord);
	screen.print(value);
	screen.print(label);
}

// получение координаты для выравнивания текста посередине
byte LTDAUI::getCenterCoordinate(const char *text)
{
    // юникод, шож поделать
    byte i = 0, j = 0;
    do {
        if (text[i] < 0)
            continue;
        j++;
    } while (text[++i] != '\0');

    return (64 - ((j * 6) / 2));
}