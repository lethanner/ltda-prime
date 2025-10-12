#include "screens.h"
#include "../System/memory.h"
#include "../Hardware/communications.h"

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
        Adjusters::Balance::it().overrideValuePtr(
         &DSP.getChannelPointer(LEDUI::MixerScreen::it().getSelectedChannel())->balpan);
        break;
    case 3:  // stereo mode
        open(&Menus::MStereoMode::it());
        Menus::MStereoMode::it().overrideSelection(
         DSP.getStereoMode(LEDUI::MixerScreen::it().getSelectedChannel()));
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
        LEDUI::setMonitorDataFeed(DSPChannels::MASTER);
        open(&LEDUI::MixerScreen::it());  // выход
        break;
    case 2:  // stereo balance
        open(&Adjusters::Balance::it());
        Adjusters::Balance::it().overrideValuePtr(
         &DSP.getChannelPointer(DSPChannels::MASTER)->balpan);
        break;
    case 3:  // stereo mode
        open(&Menus::MStereoMode::it());
        Menus::MStereoMode::it().overrideSelection(DSP.getStereoMode(DSPChannels::MASTER));
        // у канала Master нет режима вычитания - отрезаем последний пункт
        Menus::MStereoMode::it().overrideEntryCount(2);
        break;
    case 4:  // bassboost
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
        LEDUI::setMonitorDataFeed(DSPChannels::BLUETOOTH);
        open(&LEDUI::MixerScreen::it());  // выход
        break;
    case 2:  // stereo balance
        open(&Adjusters::Balance::it());
        Adjusters::Balance::it().overrideValuePtr(
         &DSP.getChannelPointer(LEDUI::MixerScreen::it().getSelectedChannel())->balpan);
        break;
    case 3:  // stereo mode
        open(&Menus::MStereoMode::it());
        Menus::MStereoMode::it().overrideSelection(DSP.getStereoMode(DSPChannels::BLUETOOTH));
        break;
    case 4:  // disconnect
        bluetooth.disconnect();
        open(&LEDUI::MixerScreen::it());  // выход
        break;
    }
}

void Menus::PitchChannel::onClick()
{
    switch (selected) {
    case 0:  // preferences
        open(&Menus::Preferences::it());
        break;
    case 1:  // send to monitor
        LEDUI::setMonitorDataFeed(DSPChannels::PITCH);
        open(&LEDUI::MixerScreen::it());  // выход
        break;
    case 2:  // stereo balance
        open(&Adjusters::Balance::it());
        Adjusters::Balance::it().overrideValuePtr(
         &DSP.getChannelPointer(LEDUI::MixerScreen::it().getSelectedChannel())->balpan);
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
    else {
        open(rvr_menus[selected - 1]);
        Adjusters::Balance::it().overrideValuePtr(
         &DSP.getChannelPointer(LEDUI::MixerScreen::it().getSelectedChannel())->balpan);
    }
}

void Menus::AUXChannel::onClick()
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
        Adjusters::Balance::it().overrideValuePtr(
         &DSP.getChannelPointer(LEDUI::MixerScreen::it().getSelectedChannel())->balpan);
        break;
    case 3: // aux gain
        open(&Adjusters::AUXGain::it());
        Adjusters::AUXGain::it().overrideValuePtr(
         shifters.getAuxGainPtr(LEDUI::MixerScreen::it().getSelectedChannel()));
        break;
    }
}

void Menus::ChannelGroup::onClick()
{
    switch (selected) {
    case 0:  // sends on fader
        open(&Menus::SendsOnFader::it());
        break;
    }
}

void Menus::SendsOnFader::onClick()
{
    if (!open(&LEDUI::MixerScreen::it(), &selected))
        open(&Choosers::NoRoutableChannels::it());
}

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
    case 1:  // rta smooth
        open(&Adjusters::RTASmooth::it());
        break;
    case 2: // wireless
        open(&Menus::MRadio::it());
        break;
    }
}

void Menus::LanguageSelect::onClick()
{
    open(&Choosers::LangSelectReboot::it(), &selected);
}

void Menus::MStereoMode::onClick()
{
    DSP.setStereoMode(LEDUI::MixerScreen::it().getSelectedChannel(),
                      static_cast<DSPChannels::StereoMode>(selected));
}

void Menus::MRadio::onClick()
{
    bool status = comm.setRadio(static_cast<Communications::RadioMode>(selected));
    const char* message;
    if (status) {
        if (selected == Communications::RadioMode::OFF)
            message = Localization::act->radioOff;
        else if (selected == Communications::RadioMode::WIFI)
            message = Localization::act->wifiOK;
        else if (selected == Communications::RadioMode::BT)
            message = Localization::act->btOK;
    } else message == Localization::act->error;

    open(&LEDUI::MixerScreen::it());
    LEDUI::statusbarMessage(message);
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
    channel ch = LEDUI::MixerScreen::it().getSelectedChannel();
    DSP.setStereoBalance(ch, DSP.getStereoBalance(ch) + dir);
}

void Adjusters::Balance::onClick()
{
    channel ch = LEDUI::MixerScreen::it().getSelectedChannel();
    int8_t current = DSP.getStereoBalance(ch);

    if (current == 0) DSP.setStereoBalance(ch, -50);
    else if (abs(current) == 50) DSP.setStereoBalance(ch, -current);
    else DSP.setStereoBalance(ch, 0);
}

void Adjusters::RTASmooth::onTurn(int8_t dir)
{
    DSP.setRTASmoothing(DSP.rta_smoothing + dir);
}

void Adjusters::AUXGain::onTurn(int8_t dir)
{
    channel ch = LEDUI::MixerScreen::it().getSelectedChannel();
    int8_t current = *shifters.getAuxGainPtr(ch);

    shifters.setAuxGain(ch, current + dir);
}

void Choosers::LangSelectReboot::onClick()
{
    if (confirmation) {
        byte langParam = *static_cast<byte *>(_params);
        memory.putUInt("lang", langParam);
        ESP.restart();
    } else {
        open(&Menus::Preferences::it());
    }
}

void Choosers::NoRoutableChannels::onClick()
{
    // возврат назад
    open(&Menus::SendsOnFader::it());
}