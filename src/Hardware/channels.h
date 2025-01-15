#pragma once
#include <stdint.h>
#include "registers.h"
typedef signed char decibel;
//typedef unsigned char channel;

#define DSP_CHANNEL_COUNT 7
#define DSP_BUS_COUNT 4
namespace DSPChannels {
    enum StereoMode { STEREO = 0, FORCEMONO = 1, VOICESUPPRESS = 2 };

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

        // ---- переменные ----
        StereoMode curStereoMode;
        uint32_t readbackVal[2];
        uint32_t readbackVal_old[2];
        decibel faderPosition;
        int8_t balpan;
        bool mute;
    };
    
    extern Channel spdif;
    extern Channel bluetooth;
    extern Channel usb;
    extern Channel reverb;
    extern Channel pitch;
    extern Channel master;
    extern Channel submix;

    enum channel { SPDIF = 0, BLUETOOTH = 1, USB = 2, REVERB = 3, PITCH = 4, MASTER = 5, SUBMIX = 6 };
    enum bus { BUS_MASTER = 0, BUS_SUBMIX = 1, BUS_REVERB = 2, BUS_PITCH = 3 };
    Channel *const list[DSP_CHANNEL_COUNT] = {
        &spdif, &bluetooth, &usb, &reverb, &pitch, &master, &submix
    };
};  //namespace DSPChannels

// #define CHANNEL_SPDIF 0
// #define CHANNEL_BLUETOOTH 1
// #define CHANNEL_USB 2
// #define CHANNEL_REVERB 3
// #define CHANNEL_PITCH 4
// #define CHANNEL_MASTER 5
// #define CHANNEL_SUBMIX 6