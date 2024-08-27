#pragma once

namespace Localization {
    struct Strings {
        const char* channel;
        const char* send_to_monitor;
        const char* vocal_filter;
        const char* bassboost;
        const char* disconnect;
        const char* group;
        const char* sends_on_fader;
        const char* to;
        const char* enable;
        const char* intensity;
        const char* gain;
        const char* reverb;
        const char* reverb_time;
        const char* hf_damping;
        const char* bassreverbgain;
        const char* parrots;
        const char* init_dsp;
        const char* init_rtos;
        const char* select;
        const char* master;
        const char* bluetooth;
    };
    extern const Strings *active;
    inline void set(const Strings *loc) { active = loc; }

    extern const Strings russian;
    extern const Strings english;

    const char *const chmenu_generic[] = { active->channel, active->send_to_monitor };
    const char *const chmenu_master[] = { active->master, active->send_to_monitor, active->bassboost };
    const char *const chmenu_reverb[] = { active->reverb, active->reverb_time, active->hf_damping, active->bassreverbgain };
    const char *const chmenu_blue[] = { active->bluetooth, active->send_to_monitor, active->disconnect };

    const char *const groupmenu[] = { active->group, active->sends_on_fader };
    const char *const sofdestmenu[] = { active->to, active->master, active->reverb };
    
    const char *const bassboostmenu[] = { active->bassboost, active->enable, active->intensity, active->gain };
};