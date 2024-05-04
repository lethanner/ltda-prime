#include "UI.h"
#include "channelmap.h"
#include "decibels.h"
#include "../Hardware/shiftreg.h"

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
        byte levelL = 52 - DSP.getRelativeSignalLevel(db_calibr_onscreen, 42, onScreenChannels[ch], false);
        byte levelR = 52 - DSP.getRelativeSignalLevel(db_calibr_onscreen, 42, onScreenChannels[ch], true);
        byte fader_pos = map(is_SOF_active
                               ? DSP.sendFaders_dB[SOF_dest][onScreenChannels[ch]]
                               : DSP.faderPosition_dB[onScreenChannels[ch]],
                             -97, 10, 52, 11);
        bool is_muted = is_SOF_active
                          ? DSP.sendMuteFlags[SOF_dest][onScreenChannels[ch]]
                          : DSP.muteFlags[onScreenChannels[ch]];

        screen.line(x_coord + 2, 52, x_coord + 2, 11);  // полоска фейдера
        if (is_muted) {
            // если MUTED, то рисуем (какую-то фигню) вместо ручки фейдера
            screen.fastLineH(fader_pos + 1, x_coord + 1, x_coord + 3);
            screen.fastLineH(fader_pos - 1, x_coord + 1, x_coord + 3);
            // если замьюченный канал выбран, то дорисовываем (фигню) до квадратика
            if (onScreenChSelect == ch) screen.fastLineH(fader_pos, x_coord + 1, x_coord + 3);
        } else
            // если не MUTED, то рисуем обычную ручку фейдера
            screen.rect(x_coord, fader_pos, x_coord + 4, fader_pos + static_cast<byte>(onScreenChSelect == ch), 2);

        screen.rect(x_coord + 8, 52, x_coord + 11, levelL, OLED_FILL);   // столбик уровня левого канала
        screen.rect(x_coord + 13, 52, x_coord + 16, levelR, OLED_FILL);  // столбик уровня правого канала

        // подписи каналов с выравниванием
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

    // селектор вместо подписей каналов (по необходимости)
    switch (screenState) {
    case 1:  // выбор канала на дисплее
        if (onScreenChSelect > 0)
            printXY("<", 56, 0);
        if (onScreenChSelect < _chan_count - 1)
            printXY(">", 56, 122);
        //printXY(chan_labels[onScreenChannels[onScreenChSelect]], 56);
        printXY(SELECTION, 56);
        break;
    case 2:  // выбор группы каналов
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

void LTDAUI::renderAdjustScreen()
{
    screen.invertText(1);
    screen.rect(0, 0, 127, 7, OLED_FILL);
    screen.setCursor(_title_x_coord, 0);
    screen.print(adj_title);

    screen.invertText(0);
    printValue(*adj_value, adj_unit, 0, 24, true);
    // TODO: сделать так, чтобы эта дичь могла рисовать эту полоску и для отрицательных значений
    byte x_end = map(*adj_value, adj_borders[0], adj_borders[1], 10, 116);
    screen.rect(10, 44, x_end, 49, OLED_FILL);
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
    int combined = dataL << 12 | dataR;

    // передать собранный битовый пакет на паровозик из сдвиговых регистров
    shifters.sendToIndicators(combined);
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

// вывод значения с подписью - в центр или с выравниванием справа
void LTDAUI::printValue(int8_t value, const char *label, int8_t x_coord, byte y_coord, bool center)
{
    byte length = strlen(label) + 1;
    if (value < -99) length += 3;
    else if (value < -9 || value > 99) length += 2;
    else if (value < 0 || value > 9) length += 1;

    if (x_coord < 0)
        x_coord = 129 - (length * 6) + x_coord;
    else if (center)
        x_coord = (64 - ((length * 6) / 2));

    screen.setCursorXY(x_coord, y_coord);
    screen.print(value);
    screen.print(label);
}

// вывод текста с выравниванием справа
void LTDAUI::printRightAlign(const char *text, byte y_coord)
{
    screen.setCursorXY(128 - (strlen(text) * 6), y_coord);
    screen.print(text);
}

// вывод уровня дБ для статусной строки
void LTDAUI::printDecibelsRight()
{
    int8_t value = is_SOF_active
                     ? DSP.sendFaders_dB[SOF_dest][onScreenChannels[onScreenChSelect]]
                     : DSP.faderPosition_dB[onScreenChannels[onScreenChSelect]];
    bool is_muted = is_SOF_active
                      ? DSP.sendMuteFlags[SOF_dest][onScreenChannels[onScreenChSelect]]
                      : DSP.muteFlags[onScreenChannels[onScreenChSelect]];
    if (value == -97 || is_muted)
        printRightAlign("muted", 0);
    else
        printValue(value, "dB", -1, 0);
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