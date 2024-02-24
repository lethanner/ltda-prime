#include "UI.h"
#include "gio/gio_esp32.h"
#include "channelmap.h"
#include <EncButton.h>

/* Объявление энкодера и дисплея */
EncButton control(19, 18, 5);
GyverOLED<SSD1306_128x64, OLED_BUFFER> screen(OLED_I2C_ADDRESS);

/* Таймеры FreeRTOS */
TimerHandle_t xBacklightTimer = NULL;
TimerHandle_t xActivityTimer = NULL;

// обработка сигналов управления
void LTDAUI::processCtrl()
{
    control.tick();
    if (control.action() == 0)
        return;

    brightDisplay();
    xTimerReset(xActivityTimer, 1);
    // ======== клик на энкодер ========
    if (control.click()) {
        switch (screenID) {
        case 1: {                                   // на экране микшера
            if (screenState == 1 && turnStarted) {  // если уже начали листать каналы
                setMonitorDataFeed(onScreenChannels[onScreenChSelect]);
                screenState = 0;  // сбрасываем действие
                turnStarted = false;
            }
            // если на экране только один канал, переход сразу к переключению страниц
            else if (screenState == 0 && _chan_count == 1)
                screenState = 2;
            else if (++screenState > 2)  // а так просто по кругу переключаем действия
                screenState = 0;

            break;
        }
        case 2:  // на экране меню
            callMenuHandler();
            break;
        }
    }

    // ======== удержание энкодера ========
    if (control.hold()) {
        switch (screenID) {
        case 1:  // на экране микшера
            createMenu(testmenu, 4, &LTDAUI::testHandler);
            break;
        case 2:                      // на экране меню
            createMixingConsole(0);  // возврат взад на главный экран
            break;
        }
    }

    // ======== вращение энкодера ========
    if (control.turn()) {
        switch (screenID) {
        case 1:  // на экране микшера
            switch (screenState) {
            case 0:  // управление громкостью канала
                DSP.setDecibelFaderPosition(
                  onScreenChannels[onScreenChSelect],
                  DSP.faderPositionDB[onScreenChannels[onScreenChSelect]] + control.dir());
                break;
            case 1: {  // переход между каналами на странице
                turnStarted = true;
                onScreenChSelect = constrain(onScreenChSelect + control.dir(), 0, _chan_count - 1);
                break;
            }
            case 2:  // переход между страницами каналов
                createMixingConsole(constrain(selectedGroup + control.dir(), 0, CH_GROUP_COUNT - 1));
                break;
            }
            break;
        case 2:  // на экране меню
            menuRotate(control.dir());
            break;
        }
    }
}

// обновление содержимого интерфейса (вызов отрисовщиков в зависимости от активности)
void LTDAUI::refresh()
{
    screen.clear();

    if (screenID == 1) {
		printXY(grp_labels[selectedGroup], 0, 0);
        printValue(DSP.faderPositionDB[onScreenChannels[onScreenChSelect]],
                   "dB", -1, 0);
    }

    switch (screenID) {
    case 1: renderMixingConsole(); break;
    case 2: renderMenu(); break;
    }

    screen.update();
    streamMonitorData();
}

void LTDAUI::reload()
{
    createMixingConsole(0);
    brightDisplay();
}

// инициализация юзер-интерфейса. ВСЕГО!
void LTDAUI::prepare()
{
    // настройка выводов передачи данных на индикатор (сдвиговые регистры там)
    pinMode(15, OUTPUT);  // сигнал данных
    pinMode(4, OUTPUT);   // сигнал защелки
    pinMode(16, OUTPUT);  // сигнал тактирования

    // инициализация дисплея
    screen.init();                                    // инициализация дисплея
    screen.clear();                                   // очистка кадра
    screen.setContrast(200);                          // установка яркости
    screen.drawBitmap(0, 0, splash_128x64, 128, 64);  // вывод логотипа
    screen.update();                                  // обновление кадра

    // инициализация таймеров
    xBacklightTimer = xTimerCreate("BacklightTimer",
                                   DISPLAY_AUTO_DIMM_TIMEOUT / portTICK_PERIOD_MS,
                                   pdFALSE, 0, &vUITimerCallback);
    xActivityTimer = xTimerCreate("UIActivityTimer",
                                  UI_ACTIVITY_TIMEOUT / portTICK_PERIOD_MS,
                                  pdFALSE, 0, &vUITimerCallback);
}

/*
 * =========== Инициализаторы активностей ===========
*/

// запуск виртуального микшерного пульта
void LTDAUI::createMixingConsole(byte groupNo)
{
    onScreenChSelect = 0;
    selectedGroup = groupNo;
    onScreenChannels = ch_groups[groupNo];
    _chan_count = ch_count[groupNo];
    // пересчёт координат для выравнивания каналов на дисплее
    gap_block = (128 - (_chan_count * 18)) / (_chan_count + 1);

    screenID = 1;
    //renderMixingConsole();
}

// вывод меню
void LTDAUI::createMenu(const char *const *entries, byte entryCount, void (LTDAUI::*handler)(byte), bool handlerAutoCall, int *menuBooleans)
{
    // сижу я такой, переписываю код, скопипастенный из исходника DA50X.
    // и думаю - а что если убрать аргумент с заголовком и оставить
    // в качестве заголовка первую строку в entries?
    menuChooseId = menuEntryRendererStartId = menuVisibleSelId = 0;
    _title_x_coord = getCenterCoordinate(entries[0]);
    _handler = handler;
    _handlerAutoCall = handlerAutoCall;
    _menuBooleans = menuBooleans;
    _entryCount = entryCount - 1;
    _entries = entries;

    screenID = 2;
}

/*
 * =========== Прочее добро ===========
*/

// выбор источника сигнала для внешнего индикатора
void LTDAUI::setMonitorDataFeed(byte ch)
{
    monitorChannel = ch;
}

// обработчик взаимодействия с меню со скроллингом
void LTDAUI::menuRotate(int8_t dir)
{
    if (dir > 0) {  // вправо
        if (menuVisibleSelId < 6) {
            if (menuVisibleSelId < _entryCount)
                menuVisibleSelId++;
            else return;
        } else {
            if (menuEntryRendererStartId + 6 < _entryCount)
                menuEntryRendererStartId++;
            else return;
        }
    } else {  // влево
        if (menuVisibleSelId > 0)
            menuVisibleSelId--;
        else {
            if (menuEntryRendererStartId > 0)
                menuEntryRendererStartId--;
            else return;
        }
    }

    menuChooseId = menuEntryRendererStartId + menuVisibleSelId;
    if (_handlerAutoCall)
        callMenuHandler();
}

void LTDAUI::callMenuHandler()
{
    (this->*_handler)(menuChooseId);
}

void LTDAUI::brightDisplay()
{
    if (xTimerIsTimerActive(xBacklightTimer) == pdFALSE)
        screen.setContrast(200);

    xTimerReset(xBacklightTimer, 1);
}

void LTDAUI::vUITimerCallback(TimerHandle_t pxTimer)
{
    if (pxTimer == xBacklightTimer)
        screen.setContrast(10);
    //else if (pxTimer == xActivityTimer)
    //    screenState = 0;
}

LTDAUI UI;