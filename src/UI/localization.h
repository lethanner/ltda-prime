#pragma once
#define SOF_FX_MAXIMUM_SENDS 2
#define SOF_DESTINATION_LABELS "Master", "Submix", "Reverb", "Pitch"

namespace Localization {
    struct Strings {
        const char* channel;
        const char* send_to_monitor;
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
        const char* language;
        const char* preferences;
        const char* pitch;
        const char* pitch_change;
        const char* semitones;
        const char* stereobalance;
        const char* stereomode;
        const char* stereo;
        const char* forcemono;
        const char* vocalfilter;
    };

    extern const Strings russian;
    extern const Strings english;

    static const Strings* act = &Localization::english;
    inline const Strings* active() { return act; }
    inline void setLanguage(const Strings* n) { act = n; }

    const char* const chmenu_generic[] = { active()->channel, active()->preferences,
                                           active()->send_to_monitor,
                                           active()->stereobalance, active()->stereomode };
    const char* const chmenu_master[] = {
        active()->master,        active()->preferences, active()->send_to_monitor,
        active()->stereobalance, active()->stereomode,  active()->bassboost
    };
    const char* const chmenu_reverb[] = {
        active()->reverb,      active()->preferences, active()->stereobalance,
        active()->reverb_time, active()->hf_damping,  active()->bassreverbgain
    };
    const char* const chmenu_pitch[] = { active()->pitch, active()->preferences, active()->send_to_monitor,
                                         active()->stereobalance, active()->pitch_change };
    const char* const chmenu_blue[] = {
        active()->bluetooth,     active()->preferences, active()->send_to_monitor,
        active()->stereobalance, active()->stereomode,  active()->disconnect
    };

    const char* const groupmenu[] = { active()->group, active()->sends_on_fader };
    const char* const sofdestmenu[] = { active()->to, SOF_DESTINATION_LABELS };
    const char* const bassboostmenu[] = { active()->bassboost, active()->enable,
                                          active()->intensity, active()->gain };
    const char* const languagemenu[] = { active()->language, "English", "Русский" };
    const char* const prefsmenu[] = { active()->preferences, active()->language };
    const char* const stereomodemenu[] = { active()->stereomode, active()->stereo,
                                           active()->forcemono, active()->vocalfilter };
};  //namespace Localization