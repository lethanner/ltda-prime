#include "UI.h"

void LTDAUI::_menu_channel_h(byte sel)
{
    switch (sel) {
    case 0:
        setMonitorDataFeed(onScreenChannels[onScreenChSelect]);
        break;
    }

    // выход
    createMixingConsole(selectedGroup);
}

void LTDAUI::_menu_master_h(byte sel)
{
    switch (sel) {
    case 0:
        setMonitorDataFeed(onScreenChannels[onScreenChSelect]);
        createMixingConsole(selectedGroup); // выход
        break;
    case 1:
        DSP.toggleBassBoost();
        break;
    }
}

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

void LTDAUI::_menu_SOFdest_h(byte sel)
{
    createMixingConsole(selectedGroup, sel);
}