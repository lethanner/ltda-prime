#pragma once
#include <stdint.h>
#include "registers.h"
typedef signed char decibel;
//typedef unsigned char channel;

#define DSP_CHANNEL_COUNT 14
#define DSP_BUS_COUNT 5
namespace DSPChannels {
    enum StereoMode { STEREO = 0, FORCEMONO = 1, VOICESUPPRESS = 2 };
    enum ChannelType { CH_DIGITAL = 0, CH_ANALOG = 1, CH_FX = 2, CH_OUTPUT = 3 };

    struct SendTo {
        const __register fader[2];

        decibel faderPosition;
        bool mute;
    };

    struct Channel {
        // ---- регистры ----
        // фейдеры канала
        const __register fader[2];
        // регистры считывания уровней с канала
        const __register readback[2];
        // регистр переключения стереорежима
        const __register stereoMode;
        // регистры с посылами
        SendTo sends[DSP_BUS_COUNT];

        // ---- константы ----
        const ChannelType type;

        // ---- переменные ----
        StereoMode curStereoMode;
        uint32_t readbackVal[2];
        uint32_t readbackVal_old[2];
        decibel faderPosition;
        int8_t balpan;
        bool mute;
        bool usingInternalADC;
    };

    extern Channel spdif;
    extern Channel bluetooth;
    extern Channel usb;
    extern Channel reverb;
    extern Channel pitch;
    extern Channel master;
    extern Channel submix;
    extern Channel spdifo;
    extern Channel aux1;
    extern Channel aux2;
    extern Channel aux3;
    extern Channel aux4;
    extern Channel aux5;
    extern Channel aux6;

    enum channel {
        SPDIF = 0,
        BLUETOOTH = 1,
        USB = 2,
        REVERB = 3,
        PITCH = 4,
        MASTER = 5,
        SUBMIX = 6,
        SPDIFO = 7,
        AUX1 = 8,
        AUX2 = 9,
        AUX3 = 10,
        AUX4 = 11,
        AUX5 = 12,
        AUX6 = 13
    };
    enum bus {
        BUS_MASTER = 0,
        BUS_SUBMIX = 1,
        BUS_REVERB = 2,
        BUS_PITCH = 3,
        BUS_SPDIF = 4
    };
    Channel *const list[DSP_CHANNEL_COUNT] = {
        &spdif,  &bluetooth, &usb,  &reverb, &pitch, &master, &submix,
        &spdifo, &aux1,      &aux2, &aux3,   &aux4,  &aux5,   &aux6
    };
};  //namespace DSPChannels