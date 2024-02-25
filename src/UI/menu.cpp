#include "UI.h"

void LTDAUI::chMenuHandler(byte sel)
{
    switch (sel) {
    case 0:
        setMonitorDataFeed(onScreenChannels[onScreenChSelect]);
        break;
    case 1:
        break;
    }

    // выход
    createMixingConsole(selectedGroup);
}