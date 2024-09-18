#include "screens.h"

void Menus::GenericChannel::onClick()
{
    switch (selected) {
    case 0:  // preferences
        open(&Menus::Preferences::it());
        break;
    case 1:  // send to monitor
        LEDUI::setMonitorDataFeed(LEDUI::MixerScreen::it().getSelectedChannel());
        open(&LEDUI::MixerScreen::it());  // выход
        break;
    case 2:  // stereo balance
        open(&Adjusters::Balance::it());
        break;
    }
}

void Menus::MasterChannel::onClick()
{
    switch (selected) {
    case 0:  // preferences
        open(&Menus::Preferences::it());
        break;
    case 1:  // send to monitor
        LEDUI::setMonitorDataFeed(FADER_MASTER_ST);
        open(&LEDUI::MixerScreen::it());  // выход
        break;
    case 2:  // stereo balance
        open(&Adjusters::Balance::it());
        break;
    case 3:  // bassboost
        open(&Menus::Bassboost::it());
        break;
    }
}

void Menus::BluetoothChannel::onClick()
{
    switch (selected) {
    case 0:  // preferences
        open(&Menus::Preferences::it());
        break;
    case 1:  // send to monitor
        LEDUI::setMonitorDataFeed(FADER_BLUETOOTH_ST);
        break;
    case 2:  // stereo balance
        open(&Adjusters::Balance::it());
        break;
    case 3:  // disconnect
        bluetooth.disconnect();
        break;
    }
    open(&LEDUI::MixerScreen::it());  // выход
}

void Menus::PitchChannel::onClick()
{
    switch (selected) {
    case 0:  // preferences
        open(&Menus::Preferences::it());
        break;
    case 1:  // send to monitor
        LEDUI::setMonitorDataFeed(FADER_PITCH);
        open(&LEDUI::MixerScreen::it());  // выход
        break;
    case 2:  // stereo balance
        open(&Adjusters::Balance::it());
        break;
    case 3:  // change pitch
        open(&Adjusters::Pitch::it());
        break;
    }
}

void Menus::ReverbChannel::onClick()
{
    static Screen *rvr_menus[] = { &Adjusters::Balance::it(), &Adjusters::ReverbTime::it(),
                                   &Adjusters::ReverbHFDamp::it(), &Adjusters::ReverbBGain::it() };
    if (selected == 0) open(&Menus::Preferences::it());
    else open(rvr_menus[selected - 1]);
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

void Menus::SendsOnFaderAll::onClick() { open(&LEDUI::MixerScreen::it(), &selected); }
void Menus::SendsOnFaderFX::onClick() { open(&LEDUI::MixerScreen::it(), &selected); }

void Menus::Bassboost::onClick()
{
    switch (selected) {
    case 0: DSP.toggleBassBoost(); break;
    case 1: open(&Adjusters::BassboostIntens::it()); break;
    case 2: open(&Adjusters::BassboostGain::it()); break;
    }
}

void Menus::Preferences::onClick()
{
    switch (selected) {
    case 0:  // language
        open(&Menus::LanguageSelect::it());
        break;
    }
}

void Menus::LanguageSelect::onClick()
{
    Localization::setLanguage(selected == 1 ? &Localization::russian : &Localization::english);
    calculateTitleCenter();
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

void Adjusters::Pitch::onTurn(int8_t dir) { DSP.setPitchBusShift(DSP.pitch_shift + dir); }

void Adjusters::Balance::onTurn(int8_t dir)
{
    byte ch = LEDUI::MixerScreen::it().getSelectedChannel();
    DSP.setStereoBalance(ch, DSP.balpan[ch] + dir);
}