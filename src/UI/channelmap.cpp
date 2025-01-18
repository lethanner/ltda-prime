#include "UI.h"

using namespace DSPChannels;
const LEDUI::MixerScreen::ChannelGroup LEDUI::MixerScreen::groups[GROUPS_COUNT] = {
    { "Inputs",   (channel[]){ SPDIF, BLUETOOTH, USB },                 3 },
    { "FX",       (channel[]){ REVERB, PITCH },                         2 },
    { "Outputs",  (channel[]){ MASTER, SUBMIX },                        2 },
    { "In + Out", (channel[]){ SPDIF, BLUETOOTH, USB, MASTER, SUBMIX }, 5 },
    { "In + FX",  (channel[]){ SPDIF, BLUETOOTH, USB, REVERB, PITCH },  5 },
    { "Out + FX", (channel[]){ REVERB, PITCH, MASTER, SUBMIX },         4 }
};

const char LEDUI::MixerScreen::ch_labels[][7] = { "SPDIF", "BT",     "USB",   "Reverb",
                                                  "Pitch", "Master", "Submix" };
const char* LEDUI::MixerScreen::sendto_labels[] = { SOF_DESTINATION_LABELS };
// да что я, чёрт побери, такое делаю... почему содержимое SOF_DESTINATION_LABELS у меня дублируется в двух массивах...