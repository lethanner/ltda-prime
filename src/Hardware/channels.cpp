#include "channels.h"
using namespace DSPChannels;

#define NO_SEND { { 0, 0 } }
// clang-format off
Channel DSPChannels::spdif = {
    .fader = {0x0037, 0x003A},
    .readback = {0x0030, 0x0031},
    .stereoMode = 0x012D,
    .sends = { { { 0x00B9, 0x00C4 }, 0, false },
               { { 0x00CD, 0x00D8 }, 0, false },
               { { 0x0048, 0x004F }, 0, true },
               { { 0x0054, 0x0055 }, 0, true } },
    .curStereoMode = STEREO,
    .faderPosition = 0,
    .balpan = 0,
    .mute = false
};

Channel DSPChannels::bluetooth = {
    .fader = {0x003D, 0x0040},
    .readback = {0x0032, 0x0033},
    .stereoMode = 0x012E,
    .sends = { { { 0x00BB, 0x00C6 }, 0, false },
               { { 0x00CF, 0x00DA }, 0, false },
               { { 0x004A, 0x0051 }, 0, true },
               { { 0x0056, 0x0057 }, 0, true } },
    .curStereoMode = STEREO,
    .faderPosition = 0,
    .balpan = 0,
    .mute = false
};

Channel DSPChannels::usb = {
    .fader = {0x0043, 0x0046},
    .readback = {0x0034, 0x0035},
    .stereoMode = 0x012F,
    .sends = { { { 0x00BD, 0x00C8 }, 0, false },
               { { 0x00D1, 0x00DC }, 0, false },
               { { 0x004C, 0x0053 }, 0, true },
               { { 0x0058, 0x0059 }, 0, true } },
    .curStereoMode = STEREO,
    .faderPosition = 0,
    .balpan = 0,
    .mute = false
};

Channel DSPChannels::reverb = {
    .fader = {0x00AE, 0x00B1},
    .readback = {0x00AA, 0x00AB},
    .stereoMode = 0,
    .sends = { { { 0x00BF, 0x00CA }, 0, false },
               { { 0x00D3, 0x00DE }, 0, false },
               NO_SEND, NO_SEND },
    .faderPosition = 0,
    .balpan = 0,
    .mute = true
};

Channel DSPChannels::pitch = {
    .fader = {0x00B4, 0x00B7},
    .readback = {0x00AC, 0},
    .stereoMode = 0,
    .sends = { { { 0x00C1, 0x00CC }, 0, false },
              { { 0x00D5, 0x00E0 }, 0, false },
              NO_SEND, NO_SEND },
    .faderPosition = 0,
    .balpan = 0,
    .mute = true
};

Channel DSPChannels::master = {
    .fader = {0x0124, 0x0127},
    .readback = {0x012B, 0x012C},
    .stereoMode = 0x013B,
    .sends = { NO_SEND, NO_SEND, NO_SEND, NO_SEND },

    .curStereoMode = STEREO,
    .faderPosition = 0,
    .balpan = 0,
    .mute = true
};

Channel DSPChannels::submix = {
    .fader = {0x011E, 0x0121},
    .readback = {0x0129, 0x012A},
    .stereoMode = 0x013A,
    .sends = { NO_SEND, NO_SEND, NO_SEND, NO_SEND },

    .curStereoMode = STEREO,
    .faderPosition = 0,
    .balpan = 0,
    .mute = true
};