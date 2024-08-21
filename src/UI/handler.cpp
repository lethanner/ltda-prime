#include "UI.h"

LTDAUI::LTDAUI()
{
    // ================ МЕНЮ КАНАЛА ОБЩЕЕ ================
    stdChannelMenu = {
        [this](byte sel) {
            switch (sel) {
            case 0:  // send to monitor
                setMonitorDataFeed(onScreenChannels[onScreenChSelect]);
                break;
            }
            // выход
            createMixingConsole(selectedGroup);
        },
        chmenu_generic, 0, false, NULL
    };

    // ================ МЕНЮ КАНАЛА MASTER ================
    masterChannelMenu = {
        [this](byte sel) {
            switch (sel) {
            case 0:  // send to monitor
                setMonitorDataFeed(FADER_MASTER_ST);
                createMixingConsole(selectedGroup);  // выход
                break;
            case 1:  // bassboost
                createMenu(&bassParamMenu);
                break;
            }
        },
        chmenu_master, 1, false, DSP.getFlagRegisterPtr()
    };

    btChannelMenu = {
        [this](byte sel) {
            switch (sel) {
            case 0:  // send to monitor
                setMonitorDataFeed(FADER_BLUETOOTH_ST);
                break;
            case 1:  // disconnect
                bluetooth.disconnect();
                break;
            }
            createMixingConsole(selectedGroup);  // выход
        },
        btmenu, 1, false, NULL
    };

    // ================ МЕНЮ КАНАЛА REVERB ================
    reverbChannelMenu = {
        [this](byte sel) {
            switch (sel) {
            case 0:  // time
                createAdjustScreen(&reverbTimeAdj);
                break;
            case 1:  // hf damping
                createAdjustScreen(&reverbHFDampAdj);
                break;
            case 2:  // bass gain
                createAdjustScreen(&reverbBGainAdj);
                break;
            }
        },
        reverbmenu, 2, false, NULL
    };
    // -----------------------------------------------------

    // ================ МЕНЮ ГРУППЫ КАНАЛОВ ================
    chGroupMenu = {
        [this](byte sel) {
            switch (sel) {
            case 0:  // пункт sends on fader
                if (selectedGroup == 0)
                    createMenu(&sofSelectMenu);
                else
                    createMenu(&sofSelectMenu_FX);
                break;
            }
        },
        groupmenu, 0, false, NULL
    };

    // ================ МЕНЮ SENDS ON FADER ================
    sofSelectMenu = {
        [this](byte sel) {
            createMixingConsole(selectedGroup, sel);
        },
        sofdestmenu, 1, false, NULL
    };

    sofSelectMenu_FX = {
        [this](byte sel) {
            createMixingConsole(selectedGroup, sel);
        },
        sofdestmenu, 0, false, NULL
    };

    bassParamMenu = {
        [this](byte sel) {
            switch (sel) {
            case 0:
                DSP.toggleBassBoost();
                break;
            case 1:
                createAdjustScreen(&bassIntensAdj);
                break;
            case 2:
                createAdjustScreen(&bassGainAdj);
                break;
            }
        },
        bassmenu, 2, false, DSP.getFlagRegisterPtr()
    };

    reverbTimeAdj = {
        [this](int8_t dir) {
            DSP.setReverbTime(DSP.reverbTime + dir);
        },
        reverbmenu_time, parrots, 0, 3, &DSP.reverbTime
    };
    reverbBGainAdj = {
        [this](int8_t dir) {
            DSP.setReverbBassGain(DSP.reverbBassGain + dir);
        },
        reverbmenu_bgain, parrots, 0, 2, &DSP.reverbBassGain
    };
    reverbHFDampAdj = {
        [this](int8_t dir) {
            DSP.setReverbHFDamping(DSP.reverbHFDamp + dir);
        },
        reverbmenu_hfdmp, parrots, 0, 2, &DSP.reverbHFDamp
    };

    bassGainAdj = {
        [this](int8_t dir) {
            DSP.setBBGain(DSP.bassboostGain + dir);
        },
        bassmenu_gain, parrots, 1, 30, &DSP.bassboostGain
    };
    bassIntensAdj = {
        [this](int8_t dir) {
            DSP.setBBIntensity(DSP.bassboostIntensity + dir);
        },
        bassmenu_intens, parrots, 1, 30, &DSP.bassboostIntensity
    };
}