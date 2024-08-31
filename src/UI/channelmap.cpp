#include "UI.h"

const LEDUI::MixerScreen::ChannelGroup LEDUI::MixerScreen::groups[GROUPS_COUNT] = {
    { "Inputs",  (byte[]){ 0, 1, 2 },       3, MixerScreen::ALL_SOF },
    { "FX",      (byte[]){ 3 },             1, MixerScreen::FX_SOF  },
    { "Outputs", (byte[]){ 4 },             1, MixerScreen::NO_SOF  },
    { "All",     (byte[]){ 0, 1, 2, 3, 4 }, 5, MixerScreen::NO_SOF  }
};

const char LEDUI::MixerScreen::ch_labels[][7] = { "SPDIF", "BT", "USB", "Reverb", "Master" };
const char* LEDUI::MixerScreen::sendto_labels[] = { "Master", "Reverb" };