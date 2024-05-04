#include "UI.h"

// обработчик меню канала (стандартного)
void LTDAUI::_menu_channel_h(byte sel)
{
    switch (sel) {
    case 0: // send to monitor
        setMonitorDataFeed(onScreenChannels[onScreenChSelect]);
        break;
    }

    // выход
    createMixingConsole(selectedGroup);
}

// обработчик меню канала master
void LTDAUI::_menu_master_h(byte sel)
{
    switch (sel) {
    case 0: // send to monitor
        setMonitorDataFeed(onScreenChannels[onScreenChSelect]);
        createMixingConsole(selectedGroup); // выход
        break;
    case 1: // bassboost
        createMenu(bassmenu, 3, &LTDAUI::_menu_bassboost_h, false, DSP.getFlagRegisterPtr());
        break;
    }
}

// обработчик меню группы каналов
void LTDAUI::_menu_group_h(byte sel)
{
    switch (sel) {
    case 0:  // пункт sends on fader
        if (selectedGroup == 0)
            createMenu(sofdestmenu, 2, &LTDAUI::_menu_SOFdest_h);
        else
            createMenu(sofdestmenu, 1, &LTDAUI::_menu_SOFdest_h);
        break;
    }
}

// обработчик меню sends on fader (выбор пункта назначения посылов)
void LTDAUI::_menu_SOFdest_h(byte sel)
{
    createMixingConsole(selectedGroup, sel);
}

void LTDAUI::_menu_bassboost_h(byte sel)
{
    switch (sel) {
        case 0:
            DSP.toggleBassBoost();
            break;
        case 1:
            createAdjustScreen(bassmenu_intens, " parrots", DSP_BASSBOOST_INTENSITY, &DSP.bassboostIntensity, 1, 30);
            break;
        case 2:
            createAdjustScreen(bassmenu_gain, " parrots", DSP_BASSBOOST_GAIN, &DSP.bassboostGain, 1, 30);
            break;
    }
}

// обработчик всего, что связано с подстройками чего бы то ни было
void LTDAUI::adjustHandler(int8_t dir)
{
    switch (adj_current) {
        case DSP_BASSBOOST_FREQ:
            // TODO
            break;
        case DSP_BASSBOOST_INTENSITY:
            DSP.setBBIntensity(DSP.bassboostIntensity + dir);
            break;
        case DSP_BASSBOOST_GAIN:
            DSP.setBBGain(DSP.bassboostGain + dir);
            break;
    }
}