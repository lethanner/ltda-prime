#include "screens.h"

void Menus::GenericChannel::onClick() const
{
    switch (selected) {
    case 0:  // send to monitor
        LEDUI::setMonitorDataFeed(LEDUI::MixerScreen::active->getSelectedChannel());
        break;
    }
    open(LEDUI::MixerScreen::active);  // выход
}

void Menus::MasterChannel::onClick() const
{
    switch (selected) {
    case 0:  // send to monitor
        LEDUI::setMonitorDataFeed(FADER_MASTER_ST);
        break;
    case 1:  // bassboost
        open(&Menus::Bassboost::it());
        break;
    }
    open(LEDUI::MixerScreen::active);  // выход
}

void Menus::BluetoothChannel::onClick() const
{
    switch (selected) {
    case 0:  // send to monitor
        LEDUI::setMonitorDataFeed(FADER_BLUETOOTH_ST);
        open(LEDUI::MixerScreen::active);  // выход
        break;
    case 1:  // disconnect
        bluetooth.disconnect();
        break;
    }
}

void Menus::ReverbChannel::onClick() const
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

void Menus::ChannelGroup::onClick() const
{
    // TODO: sends on fader
}

void Menus::Bassboost::onClick() const
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

void Adjusters::ReverbTime::onTurn(int8_t dir) const
{
    DSP.setReverbTime(DSP.reverbTime + dir);
}

void Adjusters::ReverbBGain::onTurn(int8_t dir) const
{
    DSP.setReverbBassGain(DSP.reverbBassGain + dir);
}

void Adjusters::ReverbHFDamp::onTurn(int8_t dir) const
{
    DSP.setReverbHFDamping(DSP.reverbHFDamp + dir);
}

void Adjusters::BassboostGain::onTurn(int8_t dir) const
{
    DSP.setBBGain(DSP.bassboostGain + dir);
}

void Adjusters::BassboostIntens::onTurn(int8_t dir) const
{
    DSP.setBBIntensity(DSP.bassboostIntensity + dir);
}

LEDUI::MixerScreen Mixers::mix_inputs(&LEDUI::MixerScreen::groups[0]);
LEDUI::MixerScreen Mixers::mix_fx(&LEDUI::MixerScreen::groups[1]);
LEDUI::MixerScreen Mixers::mix_outputs(&LEDUI::MixerScreen::groups[2]);
LEDUI::MixerScreen Mixers::mix_all(&LEDUI::MixerScreen::groups[3]);