#include "UI.h"

using namespace DSPChannels;
const LEDUI::MixerScreen::ChannelGroup LEDUI::MixerScreen::groups[GROUPS_COUNT] = {
    { "Inputs",   (channel[]){ SPDIF, BLUETOOTH, USB },                 3, MixerScreen::ALL_SOF },
    { "FX",       (channel[]){ REVERB, PITCH },                         2, MixerScreen::FX_SOF  },
    { "Outputs",  (channel[]){ MASTER, SUBMIX },                        2, MixerScreen::NO_SOF  },
    { "In + Out", (channel[]){ SPDIF, BLUETOOTH, USB, MASTER, SUBMIX }, 5, MixerScreen::NO_SOF  },
    { "In + FX",  (channel[]){ SPDIF, BLUETOOTH, USB, REVERB, PITCH },  5, MixerScreen::NO_SOF  },
    { "Out + FX", (channel[]){ REVERB, PITCH, MASTER, SUBMIX },         4, MixerScreen::NO_SOF  }
};

const char LEDUI::MixerScreen::ch_labels[][7] = { "SPDIF",  "BT",     "USB",  "Reverb",
                                                  "Master", "Submix", "Pitch" };
const char* LEDUI::MixerScreen::sendto_labels[] = { SOF_DESTINATION_LABELS };
// да что я, чёрт побери, такое делаю... почему содержимое SOF_DESTINATION_LABELS у меня дублируется в двух массивах...