#include "UI.h"

using namespace DSPChannels;
const LEDUI::MixerScreen::ChannelGroup LEDUI::MixerScreen::groups[GROUPS_COUNT] = {
    { "Inputs",   (channel[]){ SPDIF, BLUETOOTH, USB },                 3 },
    { "Analog",   (channel[]){ AUX1, AUX2, AUX3, AUX4, AUX5, AUX6 },    6 },
    { "FX",       (channel[]){ REVERB, PITCH },                         2 },
    { "Outputs",  (channel[]){ MASTER, SUBMIX, SPDIFO },                3 },
    { "In + Out", (channel[]){ SPDIF, BLUETOOTH, USB, MASTER, SUBMIX }, 5 },
    { "In + FX",  (channel[]){ SPDIF, BLUETOOTH, USB, REVERB, PITCH },  5 },
    { "Out + FX", (channel[]){ REVERB, PITCH, MASTER, SUBMIX, SPDIFO }, 5 }
};

const char LEDUI::MixerScreen::ch_labels[][7] = {
    "SPDIF",  "BT", "USB", "Reverb", "Pitch", "Master", "Submix",
    "SP-Out", "1",  "2",   "3",      "4",     "5",      "6"
};
const char* LEDUI::MixerScreen::sendto_labels[] = { SOF_DESTINATION_LABELS };
// да что я, чёрт побери, такое делаю... почему содержимое SOF_DESTINATION_LABELS у меня дублируется в двух массивах...