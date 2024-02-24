#pragma once

#include <Arduino.h>
#include <GyverOLED.h>
#include <GyverIO.h>
#include "bitmaps.h"
#include "localization.h"
#include "../Hardware/DSP.h"

class LTDAUI
{
  public:
    // ==== ОТРИСОВЩИКИ ПО МЕЛОЧИ ====
    void printStatus(const char *text, byte y_coord);
    void printXY(const char *text, byte y_coord, int8_t x_coord = -1);

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
    bool turnStarted = false;

    // для экрана виртуального микшера
    int8_t selectedGroup = 0, onScreenChSelect = 0;
    const unsigned char *onScreenChannels;
    byte gap_border, gap_block;
    byte _chan_count;

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
    // отрисовщики
    void streamMonitorData();
    void renderMixingConsole();
    void renderMenu();
    // инициализаторы
    void setMonitorDataFeed(byte ch);
    void createMixingConsole(byte groupNo);
    void createMenu(const char *const *entries,
                    byte entryCount,
                    void (LTDAUI::*handler)(byte),
                    bool handlerAutoCall = false,
                    int *menuBooleans = NULL);

	// ==== ОБРАБОТЧИКИ МЕНЮ ====
	void testHandler(byte sel);
};

extern LTDAUI UI;