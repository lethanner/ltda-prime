#pragma once

#include "UI.h"
using namespace LEDUI;

class : public MenuScreen
{
    using MenuScreen::MenuScreen;
    void onClick() const override
    {
        switch (selected) {
        case 0:  // send to monitor
            setMonitorDataFeed(MixerScreen::active->getSelectedChannel());
            break;
        }
        // выход
        __exit();
    }
} menu_genericChannel(Localization::chmenu_generic, 1, false, NULL);

class : public MenuScreen
{
    using MenuScreen::MenuScreen;
    void onClick() const override
    {
        switch (selected) {
        case 0:  // send to monitor
            setMonitorDataFeed(FADER_MASTER_ST);
            __exit();  // выход
            break;
        case 1:  // bassboost
            open(&menu_bassboost);
            break;
        }
    }
} menu_masterChannel(Localization::chmenu_master, 2, false, NULL);

class : public MenuScreen
{
    using MenuScreen::MenuScreen;
    void onClick() const override
    {
        switch (selected) {
        case 0:  // send to monitor
            setMonitorDataFeed(FADER_BLUETOOTH_ST);
            __exit();  // выход
            break;
        case 1:  // disconnect
            bluetooth.disconnect();
            break;
        }
    }
} menu_bluetoothChannel(Localization::chmenu_blue, 2, false, NULL);

class : public MenuScreen
{
    using MenuScreen::MenuScreen;
    void onClick() const override
    {
        // TODO: переделать на использование массива adjustscreen'ов
        switch (selected) {
        case 0:  // time
            open(&adj_reverbTime);
            break;
        case 1:  // hf damping
            open(&adj_reverbHFDamp);
            break;
        case 2:  // bass gain
            open(&adj_reverbBGain);
            break;
        }
    }
} menu_reverbChannel(Localization::chmenu_reverb, 3, false, NULL);

class : public MenuScreen
{
    using MenuScreen::MenuScreen;
    void onClick() const override
    {
        // TODO: sends on fader
    }
} menu_channelGroup(Localization::groupmenu, 1, false, NULL);

class : public MenuScreen
{
    using MenuScreen::MenuScreen;
    void onClick() const override
    {
        // TODO: sends on fader
    }
} menu_bassboost(Localization::bassboostmenu, 3, false, DSP.getFlagRegisterPtr());


class : public AdjustScreen
{
    using AdjustScreen::AdjustScreen;
    void onTurn(int8_t dir) const override
    {
        DSP.setReverbTime(DSP.reverbTime + dir);
    }
} adj_reverbTime(Localization::active->reverb_time,
                 Localization::active->parrots, 0, 3, &DSP.reverbTime);

class : public AdjustScreen
{
    using AdjustScreen::AdjustScreen;
    void onTurn(int8_t dir) const override
    {
        DSP.setReverbBassGain(DSP.reverbBassGain + dir);
    }
} adj_reverbBGain(Localization::active->bassreverbgain,
                  Localization::active->parrots, 0, 2, &DSP.reverbBassGain);

class : public AdjustScreen
{
    using AdjustScreen::AdjustScreen;
    void onTurn(int8_t dir) const override
    {
        DSP.setReverbHFDamping(DSP.reverbHFDamp + dir);
    }
} adj_reverbHFDamp(Localization::active->hf_damping,
                   Localization::active->parrots, 0, 2, &DSP.reverbHFDamp);

class : public AdjustScreen
{
    using AdjustScreen::AdjustScreen;
    void onTurn(int8_t dir) const override
    {
        DSP.setBBGain(DSP.bassboostGain + dir);
    }
} adj_bassboostGain(Localization::active->gain,
                    Localization::active->parrots, 1, 30, &DSP.bassboostGain);

class : public AdjustScreen
{
    using AdjustScreen::AdjustScreen;
    void onTurn(int8_t dir) const override
    {
        DSP.setBBIntensity(DSP.bassboostIntensity + dir);
    }
} adj_bassboostIntens(Localization::active->intensity,
                      Localization::active->parrots, 1, 30, &DSP.bassboostIntensity);

MixerScreen mix_inputs  (&MixerScreen::groups[0]);
MixerScreen mix_fx      (&MixerScreen::groups[1]);
MixerScreen mix_outputs (&MixerScreen::groups[2]);
MixerScreen mix_all     (&MixerScreen::groups[3]);
