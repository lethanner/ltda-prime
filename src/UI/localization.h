#pragma once
#define SOF_FX_MAXIMUM_SENDS 2
#define SOF_DESTINATION_LABELS "Master", "Submix", "Reverb", "Pitch", "SP-Out"

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
        const char* warning;
        const char* reboot_warning;
        const char* ok;
        const char* cancel;
        const char* sof_fail;
        const char* rtasmooth;
        const char* aux;
        const char* auxgain;
        const char* radio;
        const char* off;
        const char* wifiOK;
        const char* btOK;
        const char* radioOff;
        const char* error;
    };

    extern const Strings russian;
    extern const Strings english;
    const Strings* const locales[] = { &english, &russian };

    // оставить это как NULLPTR нельзя - получаем SIOF
    static const Strings* act = &Localization::english;
    inline void setLanguage(unsigned char id) { act = locales[id]; }

    const char* const chmenu_generic[] = { act->channel, act->preferences, act->send_to_monitor,
                                           act->stereobalance, act->stereomode };
    const char* const chmenu_master[] = { act->master,          act->preferences,
                                          act->send_to_monitor, act->stereobalance,
                                          act->stereomode,      act->bassboost };
    const char* const chmenu_reverb[] = { act->reverb,        act->preferences,
                                          act->stereobalance, act->reverb_time,
                                          act->hf_damping,    act->bassreverbgain };
    const char* const chmenu_pitch[] = { act->pitch, act->preferences, act->send_to_monitor,
                                         act->stereobalance, act->pitch_change };
    const char* const chmenu_blue[] = { act->bluetooth,       act->preferences,
                                        act->send_to_monitor, act->stereobalance,
                                        act->stereomode,      act->disconnect };
    const char* const chmenu_aux[] = { act->aux, act->preferences, act->send_to_monitor,
                                       act->stereobalance, act->auxgain };

    const char* const groupmenu[] = { act->group, act->sends_on_fader };
    const char* const sofdestmenu[] = { act->to, SOF_DESTINATION_LABELS };
    const char* const bassboostmenu[] = { act->bassboost, act->enable, act->intensity, act->gain };
    const char* const languagemenu[] = { act->language, "English", "Русский" };
    const char* const prefsmenu[] = { act->preferences, act->language, act->rtasmooth, act->radio };
    const char* const stereomodemenu[] = { act->stereomode, act->stereo, act->forcemono,
                                           act->vocalfilter };
    const char* const radioselectmenu[] = { act->radio, act->off, "Wi-Fi", "Bluetooth" };
};  //namespace Localization