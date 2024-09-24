#pragma once
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
    extern const Strings* active;
    void setLanguage(const Strings* n);

    // АААААААААААААААААААААААААААААААААААААААААААААААААА
    inline const char *const *chmenu_generic;
    inline const char *const *chmenu_master;
    inline const char *const *chmenu_reverb;
    inline const char *const *chmenu_pitch;
    inline const char *const *chmenu_blue;
    inline const char *const *groupmenu;
    inline const char *const *sofdestmenu;
    inline const char *const *bassboostmenu;
    inline const char *const *languagemenu;
    inline const char *const *prefsmenu;
    inline const char *const *stereomodemenu;
};  //namespace Localization