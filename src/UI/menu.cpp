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
        DSP.toggleBassBoost();
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