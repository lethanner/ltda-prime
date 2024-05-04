#include "UI.h"
#include "channelmap.h"
#include <EncButton.h>

/* Объявление энкодера и дисплея */
EncButton control(CTRL_S1, CTRL_S2, CTRL_KEY);
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

            statusbarState = 1;
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
        case 1:                                  // на экране микшера
            if (screenState == 1) is_SOF_active  // удержание на выборе канала - MUTE
                                    ? DSP.toggleMute(onScreenChannels[onScreenChSelect], SOF_dest)
                                    : DSP.toggleMute(onScreenChannels[onScreenChSelect]);
            else if (is_SOF_active) createMixingConsole(selectedGroup);  // удержание на экране sends on fader - возврат
            else if (screenState == 2 && selectedGroup < 2) createMenu(groupmenu, 1, &LTDAUI::_menu_group_h);
            else {
                if (onScreenChannels[onScreenChSelect] == FADER_MASTER_ST)  // если выбрали канал Master, открываем меню для него
                    createMenu(chmenu_master, 2, &LTDAUI::_menu_master_h, false, DSP.getFlagRegisterPtr());
                else if (onScreenChannels[onScreenChSelect] == FADER_REVERB_ST)  // если выбрали канал reverb, то для него меню
                    createMenu(reverbmenu, 3, &LTDAUI::_menu_reverb_h);
                else  // иначе меню для всех остальных
                    createMenu(chmenu_generic, 1, &LTDAUI::_menu_channel_h, false);
            }
            break;
        case 2:                                  // на экране меню
            createMixingConsole(selectedGroup);  // возврат взад на главный экран
            break;
        case 3:                                                                                        // на экране подстройки чего-то...
            if (adj_current == DSP_BASSBOOST_GAIN || adj_current == DSP_BASSBOOST_INTENSITY)           // бассбуста
                createMenu(bassmenu, 3, &LTDAUI::_menu_bassboost_h, false, DSP.getFlagRegisterPtr());  // назад в меню (потом переделать!!!)
            else if (adj_current >= DSP_REVERB_TIME && adj_current <= DSP_REVERB_BASSGAIN)             // реверба
                createMenu(reverbmenu, 3, &LTDAUI::_menu_reverb_h);                                    // то же самое - переделать как-нибудь!!!
            else                                                                                       // любом другом
                createMixingConsole(selectedGroup);                                                    // выход
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
        case 3:  // на экране подстройки
            adjustHandler(control.dir());
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
            printDecibelsRight();
            printXY(chan_labels[onScreenChannels[onScreenChSelect]], 0, 0);
            screen.print(" to ");
            screen.print(sendto_labels[SOF_dest]);
        } else if (statusbarState == 0) {  // непосредственно статусбар
            printXY("Mixer", 0, 0);        // заглушка заголовка
            printValue(0, "'C", -1, 0);    // заглушка датчика температуры
        } else if (statusbarState == 1) {  // в момент изменения громкости канала
            printDecibelsRight();
            printXY(chan_labels[onScreenChannels[onScreenChSelect]], 0, 0);
            screen.print(":");
        }
    }

    switch (screenID) {
    case 1: renderMixingConsole(); break;
    case 2: renderMenu(); break;
    case 3: renderAdjustScreen(); break;
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

// вывод фиговины для подстройки чего бы то ни было
void LTDAUI::createAdjustScreen(const char *title, const char *unit, AdjustParameter param, int8_t *value, int8_t min, int8_t max)
{
    _title_x_coord = getCenterCoordinate(title);
    adj_title = title;
    adj_unit = unit;
    adj_current = param;
    adj_borders[0] = min, adj_borders[1] = max;
    adj_value = value;

    screenID = 3;
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