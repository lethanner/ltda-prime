#include "UI.h"
using namespace LEDUI;

// clang-format off
const MixerScreen::ChannelGroup const MixerScreen::groups[GROUPS_COUNT] = {
    {0, "Inputs",  (byte[]){0, 1, 2}, 3,       MixerScreen::ALL_SOF},
    {1, "FX",      (byte[]){3}, 1,             MixerScreen::FX_SOF},
    {2, "Outputs", (byte[]){4}, 1,             MixerScreen::NO_SOF},
    {3, "All",     (byte[]){0, 1, 2, 3, 4}, 5, MixerScreen::NO_SOF}
};

const char const MixerScreen::ch_labels[][7] = {
    "SPDIF",
    "BT",
    "USB",
    "Reverb",
    "Master"
};