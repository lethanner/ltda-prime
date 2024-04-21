#pragma once

#include <Arduino.h>
#include <GyverOLED.h>
#include <GyverIO.h>
#include "config.h"
#include "bitmaps.h"
#include "localization.h"
#include "../Hardware/DSP.h"
#include "freertos/timers.h"

class LTDAUI
{
  public:
    void printStatus(const char *text, byte y_coord);

    // ==== СЛУЖЕБКА ====
    void prepare();
    void processCtrl();
    void refresh();
    void reload();

  private:
    // ==== ПЕРЕМЕННЫЕ ====
    // общие переменные
    byte screenID = 0;
    byte screenState = 0;
    byte statusbarState = 0;
    bool turnStarted = false;

    // для экрана виртуального микшера
    int8_t selectedGroup = 0, onScreenChSelect = 0;
    const unsigned char *onScreenChannels;
    byte gap_border, gap_block;
    byte _chan_count;
    // sends on fader
    byte SOF_dest = 0;
    bool is_SOF_active = false;

    // для работы меню со скроллингом
    void (LTDAUI::*_handler)(byte) = NULL;
    byte menuVisibleSelId, menuEntryRendererStartId, menuChooseId;
    const char *const *_entries;
    byte _entryCount, _title_x_coord;
    bool _handlerAutoCall;
    int *_menuBooleans;

    // для внешнего индикатора
    byte monitorChannel = 0;
    //int *monitorDataFeed;

    // ==== АКТИВНОСТИ ====
    // вспомогательные функции
    byte getCenterCoordinate(const char *text);
    void menuRotate(int8_t dir);
    void callMenuHandler();
    void brightDisplay();
    // отрисовщики
    void streamMonitorData();
    void renderMixingConsole();
    void renderMenu();
    // инициализаторы
    void setMonitorDataFeed(byte ch);
    void createMixingConsole(byte groupNo, int8_t sof = -1);
    void createMenu(const char *const *entries,
                    byte entryCount,
                    void (LTDAUI::*handler)(byte),
                    bool handlerAutoCall = false,
                    int *menuBooleans = NULL);

    // ==== ОБРАБОТЧИКИ МЕНЮ ====
    void _menu_channel_h(byte sel);
    void _menu_group_h(byte sel);
    void _menu_SOFdest_h(byte sel);

    // ==== ОТРИСОВЩИКИ ПО МЕЛОЧИ ====
    void printXY(const char *text, byte y_coord, int8_t x_coord = -1);
    void printValue(int8_t value, const char *label, int8_t x_coord,
                    byte y_coord, bool center = false);
    void printRightAlign(const char *text, byte y_coord);
    void printDecibelsRight();

    // ==== FREERTOS ====
    static void vUITimerCallback(TimerHandle_t pxTimer);
};

extern LTDAUI UI;