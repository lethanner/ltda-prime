#include "UI.h"
#include <EncButton.h>
// по неизвестной причине библиотека EncButton не рассчитана на множественные #include.
// поэтому при её появлении в заголовочном файле возникают ошибки multiple definition.
// её использование вряд ли будет где-то дальше этого файла, поэтому оставлю её здесь
// и не буду пихать инстанс её класса в общий namespace.
#include "screens.h"
#include "decibels.h"
#include "../Hardware/shiftreg.h"

/* Объявление энкодера и дисплея */
GyverOLED<SSD1306_128x64, OLED_BUFFER> LEDUI::display(OLED_I2C_ADDRESS);
EncButton control(CTRL_S1, CTRL_S2, CTRL_KEY);

/* Таймеры FreeRTOS */
TimerHandle_t xBacklightTimer = NULL;
TimerHandle_t xActivityTimer = NULL;

byte LEDUI::monitor_ch = FADER_MASTER_ST;
byte LEDUI::title_xCoord = 0;
byte LEDUI::screen_state = 0, LEDUI::statusbar = 0;
LEDUI::Screen *LEDUI::active = nullptr;

void LEDUI::init()
{
    // TODO: переключение языка из меню
    //Localization::set(&Localization::english);

    // инициализация дисплея
    display.init();                                             // инициализация дисплея
    display.clear();                                            // очистка кадра
    display.setContrast(BRIGHTNESS_DEFAULT);                    // установка яркости
    display.drawBitmap(0, 0, Bitmaps::splash_128x64, 128, 64);  // вывод логотипа
    display.update();                                           // обновление кадра

    // инициализация таймеров
    xBacklightTimer = xTimerCreate("BacklightTimer",
                                   DISPLAY_AUTO_DIMM_TIMEOUT / portTICK_PERIOD_MS,
                                   pdFALSE, NULL, &vUITimerCallback);
    xActivityTimer = xTimerCreate("UIActivityTimer",
                                  UI_ACTIVITY_TIMEOUT / portTICK_PERIOD_MS,
                                  pdFALSE, NULL, &vUITimerCallback);
}

void LEDUI::reset()
{
    MixerScreen::it().setGroup(0);
    open(&MixerScreen::it());
    setMonitorDataFeed(FADER_MASTER_ST);
    brightDisplay();
}

void LEDUI::render()
{
    active->render();
    display.update();
}

void LEDUI::pollCtrl()
{
    control.tick();
    if (control.action() == 0)
        return;

    brightDisplay();
    if (control.click())
        active->onClick();
    if (control.hold())
        active->onHold();
    if (control.turn())
        active->onTurn(control.dir());
}

void LEDUI::open(Screen *scr, void *params)
{
    scr->init(params);
    active = scr;
}

byte LEDUI::getCenterCoordinate(const char *text)
{
    // спасибо ChatGPT за эту реализацию strlen() для юникода
    byte length = 0;
    while (*text != '\0') {
        if ((*text & 0xC0) != 0x80)  // Check if it's the beginning of a UTF-8 character
            length++;
        text++;
    }

    return (64 - ((length * 6) / 2));
}

void LEDUI::printValue(int8_t value, const char *label, int8_t x_coord, byte y_coord, bool center)
{
    byte length = strlen(label) + 1;
    if (value < -99) length += 3;
    else if (value < -9 || value > 99) length += 2;
    else if (value < 0 || value > 9) length += 1;

    if (x_coord < 0)
        x_coord = 129 - (length * 6) + x_coord;
    else if (center)
        x_coord = (64 - ((length * 6) / 2));

    display.setCursorXY(x_coord, y_coord);
    display.print(value);
    display.print(label);
}

void LEDUI::bootStatus(const char *text, byte y_coord)
{
    display.clear(0, y_coord, 127, y_coord + 7);
    display.setCursorXY(getCenterCoordinate(text), y_coord);
    display.print(text);
    display.update();
}

// вывод текста сразу по координатам (с возможностью выравнивания в центре)
void LEDUI::printYX(const char *text, byte y_coord, int8_t x_coord)
{
    display.setCursorXY(x_coord < 0 ? getCenterCoordinate(text) : x_coord, y_coord);
    display.print(text);
}

// вывод текста с выравниванием справа
void LEDUI::printRightAlign(const char *text, byte y_coord)
{
    display.setCursorXY(128 - (strlen(text) * 6), y_coord);
    display.print(text);
}

// подсветка дисплея на время
void LEDUI::brightDisplay()
{
    if (xTimerIsTimerActive(xBacklightTimer) == pdFALSE)
        display.setContrast(BRIGHTNESS_DEFAULT);

    xTimerReset(xBacklightTimer, 1);
}

// отправка данных на сдвиговики на внешнем индикаторе уровня
void LEDUI::streamMonitorData()
{
    // не кидайтесь помидорами! я знаю, что это можно реализовать через
    // возведение двойки в степень! с технической точки зрения такое решение
    // однозначно быстрее и удобнее, а с 520 килобайтами оперативки
    // вообще не жалко ради оптимизации лишний раз занять около 26 байт.
    // это не DA50X всего с двумя килобайтами.
    const DRAM_ATTR static short ledbitmap[13] = {
        0b000000000000, 0b100000000000, 0b110000000000, 0b111000000000,
        0b111100000000, 0b111110000000, 0b111111000000, 0b111111100000,
        0b111111110000, 0b111111111000, 0b111111111100, 0b111111111110,
        0b111111111111
    };
    // Максим, поменяй 7 на 12 после допайки светодиодиков!
    short dataL = ledbitmap[DSP.getRelativeSignalLevel(db_calibr_ledmonitor, 12, monitor_ch, false)];
    short dataR = ledbitmap[DSP.getRelativeSignalLevel(db_calibr_ledmonitor, 12, monitor_ch, true)];
    int combined = dataL << 12 | dataR;

    // передать собранный битовый пакет на паровозик из сдвиговых регистров
    shifters.sendToIndicators(combined);
}

void LEDUI::vUITimerCallback(TimerHandle_t pxTimer)
{
    if (pxTimer == xBacklightTimer) // таймер снижения яркости подсветки
        display.setContrast(BRIGHTNESS_MINIMUM);
    else if (pxTimer == xActivityTimer) // таймер возврата на главный экран
        screen_state = statusbar = 0;
}