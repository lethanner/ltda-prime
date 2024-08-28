#include "screens.h"

void Menus::GenericChannel::onClick()
{
    switch (selected) {
    case 0:  // send to monitor
        LEDUI::setMonitorDataFeed(LEDUI::MixerScreen::it().getSelectedChannel());
        break;
    }
    open(&LEDUI::MixerScreen::it());  // выход
}

void Menus::MasterChannel::onClick()
{
    switch (selected) {
    case 0:  // send to monitor
        LEDUI::setMonitorDataFeed(FADER_MASTER_ST);
        open(&LEDUI::MixerScreen::it());  // выход
        break;
    case 1:  // bassboost
        open(&Menus::Bassboost::it());
        break;
    }
}

void Menus::BluetoothChannel::onClick()
{
    switch (selected) {
    case 0:  // send to monitor
        LEDUI::setMonitorDataFeed(FADER_BLUETOOTH_ST);
        open(&LEDUI::MixerScreen::it());  // выход
        break;
    case 1:  // disconnect
        bluetooth.disconnect();
        break;
    }
}

void Menus::ReverbChannel::onClick()
{
    // TODO: переделать на использование массива adjustscreen'ов
    switch (selected) {
    case 0:  // time
        open(&Adjusters::ReverbTime::it());
        break;
    case 1:  // hf damping
        open(&Adjusters::ReverbHFDamp::it());
        break;
    case 2:  // bass gain
        open(&Adjusters::ReverbBGain::it());
        break;
    }
}

void Menus::ChannelGroup::onClick()
{
    switch (selected) {
    case 0:  // sends on fader
        if (LEDUI::MixerScreen::it().isSoFAllowed() == LEDUI::MixerScreen::FX_SOF)
            open(&Menus::SendsOnFaderFX::it());
        else if (LEDUI::MixerScreen::it().isSoFAllowed() == LEDUI::MixerScreen::ALL_SOF)
            open(&Menus::SendsOnFaderAll::it());
        break;
    }
}

void Menus::SendsOnFaderAll::onClick()
{
    open(&LEDUI::MixerScreen::it(), &selected);
}

void Menus::SendsOnFaderFX::onClick()
{
    open(&LEDUI::MixerScreen::it(), &selected);
}

void Menus::Bassboost::onClick()
{
    switch (selected) {
    case 0:
        DSP.toggleBassBoost();
        break;
    case 1:
        open(&Adjusters::BassboostIntens::it());
        break;
    case 2:
        open(&Adjusters::BassboostGain::it());
        break;
    }
}

void Adjusters::ReverbTime::onTurn(int8_t dir)
{
    DSP.setReverbTime(DSP.reverbTime + dir);
}

void Adjusters::ReverbBGain::onTurn(int8_t dir)
{
    DSP.setReverbBassGain(DSP.reverbBassGain + dir);
}

void Adjusters::ReverbHFDamp::onTurn(int8_t dir)
{
    DSP.setReverbHFDamping(DSP.reverbHFDamp + dir);
}

void Adjusters::BassboostGain::onTurn(int8_t dir)
{
    DSP.setBBGain(DSP.bassboostGain + dir);
}

void Adjusters::BassboostIntens::onTurn(int8_t dir)
{
    DSP.setBBIntensity(DSP.bassboostIntensity + dir);
}