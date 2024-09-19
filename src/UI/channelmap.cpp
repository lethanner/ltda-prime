#include "UI.h"

const LEDUI::MixerScreen::ChannelGroup LEDUI::MixerScreen::groups[GROUPS_COUNT] = {
    { "Inputs",   (byte[]){ 0, 1, 2 },       3, MixerScreen::ALL_SOF },
    { "FX",       (byte[]){ 3, 6 },          2, MixerScreen::FX_SOF  },
    { "Outputs",  (byte[]){ 4, 5 },          2, MixerScreen::NO_SOF  },
    { "In + Out", (byte[]){ 0, 1, 2, 4, 5 }, 5, MixerScreen::NO_SOF  },
    { "In + FX",  (byte[]){ 0, 1, 2, 3, 6 }, 5, MixerScreen::NO_SOF  },
    { "Out + FX", (byte[]){ 3, 6, 4, 5 },    4, MixerScreen::NO_SOF  }
};

const char LEDUI::MixerScreen::ch_labels[][7] = { "SPDIF",  "BT",     "USB",  "Reverb",
                                                  "Master", "Submix", "Pitch" };
const char* LEDUI::MixerScreen::sendto_labels[] = { SOF_DESTINATION_LABELS };
// да что я, чёрт побери, такое делаю... почему содержимое SOF_DESTINATION_LABELS у меня дублируется в двух массивах...