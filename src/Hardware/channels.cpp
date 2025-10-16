#include "channels.h"
using namespace DSPChannels;

#define NO_SEND { { 0, 0 } }
// clang-format off
Channel DSPChannels::spdif = {
    .fader = {0x017F, 0x0182},
    .readback = {0x0178, 0x0179},
    .stereoMode = 0x033B,
    .sends = { { { 0x0225, 0x023C }, 0, false },
               { { 0x0251, 0x0268 }, 0, false },
               { { 0x0190, 0x01A3 }, 0, true },
               { { 0x01B4, 0x01B5 }, 0, true },
               { { 0x027D, 0x0294 }, 0, false } },
    .type = CH_DIGITAL,
    .curStereoMode = STEREO,
    .faderPosition = 0,
    .balpan = 0,
    .mute = false,
    .usingInternalADC = false
};

Channel DSPChannels::bluetooth = {
    .fader = {0x0185, 0x0188},
    .readback = {0x017A, 0x017B},
    .stereoMode = 0x03F0,
    .sends = { { { 0x0227, 0x023E }, 0, false },
               { { 0x0253, 0x026A }, 0, false },
               { { 0x0192, 0x01A5 }, 0, true },
               { { 0x01B6, 0x01B7 }, 0, true },
               { { 0x027F, 0x0296 }, 0, false } },
    .type = CH_DIGITAL,
    .curStereoMode = STEREO,
    .faderPosition = 0,
    .balpan = 0,
    .mute = false,
    .usingInternalADC = false
};

Channel DSPChannels::usb = {
    .fader = {0x018B, 0x018E},
    .readback = {0x017C, 0x017D},
    .stereoMode = 0x03F1,
    .sends = { { { 0x0229, 0x0240 }, 0, false },
               { { 0x0255, 0x026C }, 0, false },
               { { 0x0194, 0x01A7 }, 0, true },
               { { 0x01B8, 0x01B9 }, 0, true },
               { { 0x0281, 0x0298 }, 0, false } },
    .type = CH_DIGITAL,
    .curStereoMode = STEREO,
    .faderPosition = 0,
    .balpan = 0,
    .mute = false,
    .usingInternalADC = false
};

Channel DSPChannels::reverb = {
    .fader = {0x021A, 0x021D},
    .readback = {0x0216, 0x0217},
    .stereoMode = 0,
    .sends = { { { 0x022B, 0x0242 }, 0, false },
               { { 0x0257, 0x026E }, 0, false },
               NO_SEND, NO_SEND,
               { { 0x0283, 0x029A }, 0, false } },
    .type = CH_FX,
    .faderPosition = 0,
    .balpan = 0,
    .mute = true,
    .usingInternalADC = false
};

Channel DSPChannels::pitch = {
    .fader = {0x0220, 0x0223},
    .readback = {0x0218, 0},
    .stereoMode = 0,
    .sends = { { { 0x022D, 0x0244 }, 0, false },
               { { 0x0259, 0x0270 }, 0, false },
               NO_SEND, NO_SEND,
               { { 0x0285, 0x029C }, 0, false } },
    .type = CH_FX,
    .faderPosition = 0,
    .balpan = 0,
    .mute = true,
    .usingInternalADC = false
};

Channel DSPChannels::master = {
    .fader = {0x0334, 0x0337},
    .readback = {0x0339, 0x033A},
    .stereoMode = 0x03FE,
    .sends = { NO_SEND, NO_SEND, NO_SEND, NO_SEND, NO_SEND },

    .type = CH_OUTPUT,
    .curStereoMode = STEREO,
    .faderPosition = 0,
    .balpan = 0,
    .mute = true,
    .usingInternalADC = false
};

Channel DSPChannels::submix = {
    .fader = {0x0324, 0x0327},
    .readback = {0x032F, 0x0330},
    .stereoMode = 0x03FC,
    .sends = { NO_SEND, NO_SEND, NO_SEND, NO_SEND, NO_SEND },

    .type = CH_OUTPUT,
    .curStereoMode = STEREO,
    .faderPosition = 0,
    .balpan = 0,
    .mute = true,
    .usingInternalADC = false
};

Channel DSPChannels::spdifo = {
    .fader = {0x032A, 0x032D},
    .readback = {0x0331, 0x0332},
    .stereoMode = 0x03FD,
    .sends = { NO_SEND, NO_SEND, NO_SEND, NO_SEND, NO_SEND },
    
    .type = CH_OUTPUT,
    .curStereoMode = STEREO,
    .faderPosition = 0,
    .balpan = 0,
    .mute = true,
    .usingInternalADC = false
};

Channel DSPChannels::aux1 = {
    .fader = {0x0119, 0x011C},
    .readback = {0x0078, 0},
    .stereoMode = 0,
    .sends = { { { 0x022F, 0x0246 }, 0, false },
               { { 0x025B, 0x0272 }, 0, false },
               { { 0x0196, 0x01A9 }, 0, true },
               { { 0x01BA, 0x01BB }, 0, true },
               { { 0x0287, 0x029E }, 0, false } },
    
    .type = CH_ANALOG,
    .faderPosition = 0,
    .balpan = 0,
    .mute = true,
    .usingInternalADC = false
};

Channel DSPChannels::aux2 = {
    .fader = {0x011F, 0x0122},
    .readback = {0x0079, 0},
    .stereoMode = 0,
    .sends = { { { 0x0231, 0x0248 }, 0, false },
               { { 0x025D, 0x0274 }, 0, false },
               { { 0x0198, 0x01AB }, 0, true },
               { { 0x01BC, 0x01BD }, 0, true },
               { { 0x0289, 0x02A0 }, 0, false } },

    .type = CH_ANALOG,
    .faderPosition = 0,
    .balpan = 0,
    .mute = true,
    .usingInternalADC = false
};

Channel DSPChannels::aux3 = {
    .fader = {0x0125, 0x0128},
    .readback = {0xF5A0, 0},
    .stereoMode = 0,
    .sends = { { { 0x0233, 0x024A }, 0, false },
               { { 0x025F, 0x0276 }, 0, false },
               { { 0x019A, 0x01AD }, 0, true },
               { { 0x01BE, 0x01BF }, 0, true },
               { { 0x028B, 0x02A2 }, 0, false } },

    .type = CH_ANALOG,
    .faderPosition = 0,
    .balpan = 0,
    .mute = true,
    .usingInternalADC = true
};

Channel DSPChannels::aux4 = {
    .fader = {0x012B, 0x012E},
    .readback = {0xF5A1, 0},
    .stereoMode = 0,
    .sends = { { { 0x0235, 0x024C }, 0, false },
               { { 0x0261, 0x0278 }, 0, false },
               { { 0x019C, 0x01AF }, 0, true },
               { { 0x01C0, 0x01C1 }, 0, true },
               { { 0x028D, 0x02A4 }, 0, false } },

    .type = CH_ANALOG,
    .faderPosition = 0,
    .balpan = 0,
    .mute = true,
    .usingInternalADC = true
};

Channel DSPChannels::aux5 = {
    .fader = {0x0131, 0x0134},
    .readback = {0xF5A2, 0},
    .stereoMode = 0,
    .sends = { { { 0x0237, 0x024E }, 0, false },
               { { 0x0263, 0x027A }, 0, false },
               { { 0x019E, 0x01B1 }, 0, true },
               { { 0x01C2, 0x01C3 }, 0, true },
               { { 0x028F, 0x02A6 }, 0, false } },

    .type = CH_ANALOG,
    .faderPosition = 0,
    .balpan = 0,
    .mute = true,
    .usingInternalADC = true
};

Channel DSPChannels::aux6 = {
    .fader = {0x0137, 0x013A},
    .readback = {0xF5A3, 0},
    .stereoMode = 0,
    .sends = { { { 0x0239, 0x0250 }, 0, false },
               { { 0x0265, 0x027C }, 0, false },
               { { 0x01A0, 0x01B3 }, 0, true },
               { { 0x01C4, 0x01C5 }, 0, true },
               { { 0x0291, 0x02A8 }, 0, false } },

    .type = CH_ANALOG,
    .faderPosition = 0,
    .balpan = 0,
    .mute = true,
    .usingInternalADC = true
};
