#include "UI.h"
#include "channelmap.h"
#include <EncButton.h>

/* Объявление энкодера и дисплея */
EncButton control(CTRL_S1, CTRL_S2, CTRL_KEY);
GyverOLED<SSD1306_128x64, OLED_BUFFER> screen(OLED_I2C_ADDRESS);

/* Таймеры FreeRTOS */
TimerHandle_t xBacklightTimer = NULL;
TimerHandle_t xActivityTimer = NULL;

// временный тестовый костыль
int menuBoolTestStub = 0;

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
        case 1: {                                                      // на экране микшера
            if (screenState == 1 && (turnStarted || is_SOF_active)) {  // если уже начали листать каналы или если режим "sends on fader"
                screenState = 0;                                       // сбрасываем действие
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
        case 1:                                                        // на экране микшера
            if (is_SOF_active) createMixingConsole(selectedGroup);  // удержание на экране sends on fader - возврат
            else if (screenState == 2 && selectedGroup < 2) createMenu(groupmenu, 1, &LTDAUI::_menu_group_h);
            else createMenu(channelmenu, 2, &LTDAUI::_menu_channel_h, false, &menuBoolTestStub);
            break;
        case 2:                                  // на экране меню
            createMixingConsole(selectedGroup);  // возврат взад на главный экран
            break;
        }
    }

    // ======== вращение энкодера ========
    if (control.turn()) {
        switch (screenID) {
        case 1:  // на экране микшера
            switch (screenState) {
            case 0: {  // управление громкостью канала
                if (is_SOF_active) {
                    DSP.setDecibelSendLevel(
                      onScreenChannels[onScreenChSelect], SOF_dest,
                      DSP.sendFaders_dB[SOF_dest][onScreenChannels[onScreenChSelect]] + control.dir());
                } else {
                    DSP.setDecibelFaderPosition(
                      onScreenChannels[onScreenChSelect],
                      DSP.faderPosition_dB[onScreenChannels[onScreenChSelect]] + control.dir());
                    statusbarState = 1;
                }
                break;
            }
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

    // статусбар
    if (screenID == 1) {
        if (is_SOF_active) {  // "sends on fader" вместо статусбара
            printDecibelsRight(DSP.sendFaders_dB[SOF_dest][onScreenChannels[onScreenChSelect]]);
            printXY(chan_labels[onScreenChannels[onScreenChSelect]], 0, 0);
            screen.print(" to ");
            screen.print(sendto_labels[SOF_dest]);
        } else if (statusbarState == 0) {  // непосредственно статусбар
            printXY("Mixer", 0, 0);        // заглушка заголовка
            printValue(0, "'C", -1, 0);    // заглушка датчика температуры
        } else if (statusbarState == 1) {  // в момент изменения громкости канала
            printDecibelsRight(DSP.faderPosition_dB[onScreenChannels[onScreenChSelect]]);
            printXY(chan_labels[onScreenChannels[onScreenChSelect]], 0, 0);
            screen.print(":");
        }
    }

    switch (screenID) {
    case 1: renderMixingConsole(); break;
    case 2: renderMenu(); break;
    }

    screen.update();
    streamMonitorData();
}

// (пере)запуск юзер-интерфейса
void LTDAUI::reload()
{
    createMixingConsole(0);
    setMonitorDataFeed(FADER_MASTER_ST);
    brightDisplay();
}

// инициализация юзер-интерфейса. ВСЕГО!
void LTDAUI::prepare()
{
    // инициализация дисплея
    screen.init();                                    // инициализация дисплея
    screen.clear();                                   // очистка кадра
    screen.setContrast(200);                          // установка яркости
    screen.drawBitmap(0, 0, splash_128x64, 128, 64);  // вывод логотипа
    screen.update();                                  // обновление кадра

    // инициализация таймеров
    xBacklightTimer = xTimerCreate("BacklightTimer",
                                   DISPLAY_AUTO_DIMM_TIMEOUT / portTICK_PERIOD_MS,
                                   pdFALSE, static_cast<void *>(this), &vUITimerCallback);
    xActivityTimer = xTimerCreate("UIActivityTimer",
                                  UI_ACTIVITY_TIMEOUT / portTICK_PERIOD_MS,
                                  pdFALSE, static_cast<void *>(this), &vUITimerCallback);
}

/*
 * =========== Инициализаторы активностей ===========
*/

// запуск виртуального микшерного пульта
void LTDAUI::createMixingConsole(byte groupNo, int8_t sof)
{
    onScreenChannels = ch_groups[groupNo];
    _chan_count = ch_count[groupNo];
    if (groupNo != selectedGroup) {
        onScreenChSelect = 0;
        selectedGroup = groupNo;
    }

    // пересчёт координат для выравнивания каналов на дисплее
    gap_block = (128 - (_chan_count * 18)) / (_chan_count + 1);

    screenID = 1;
    SOF_dest = sof, is_SOF_active = (sof > -1);
    if (is_SOF_active) statusbarState = screenState = 0;
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

// вызов триггера при выборе пункта меню
void LTDAUI::callMenuHandler()
{
    (this->*_handler)(menuChooseId);
}

// подсветка дисплея на время
void LTDAUI::brightDisplay()
{
    if (xTimerIsTimerActive(xBacklightTimer) == pdFALSE)
        screen.setContrast(200);

    xTimerReset(xBacklightTimer, 1);
}

void LTDAUI::vUITimerCallback(TimerHandle_t pxTimer)
{
    // повезло, что у таймеров FreeRTOS есть место, куда пихнуть указатель
    // https://stackoverflow.com/questions/71199868/c-use-a-class-non-static-method-as-a-function-pointer-callback-in-freertos-xti
    LTDAUI *instance = static_cast<LTDAUI *>(pvTimerGetTimerID(pxTimer));

    if (pxTimer == xBacklightTimer)
        screen.setContrast(10);
    else if (pxTimer == xActivityTimer) {
        instance->screenState = 0;
        instance->statusbarState = 0;
    }
}

LTDAUI UI;