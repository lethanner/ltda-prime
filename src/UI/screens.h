#pragma once
#include "UI.h"

// clang-format off
#define DECLARE_MENU(_name, _entr, _count, _aclick, _bools) \
    class _name : public LEDUI::MenuScreen { \
      public: \
        static _name& it() \
        { \
            static _name ins; \
            return ins; \
        } \
        void onClick() override; \
      private: \
        _name() : MenuScreen(_entr, _count, _aclick, _bools) {}; \
    }

#define DECLARE_ADJUST(_name, _title, _unit, __min, __max, _val) \
    class _name : public LEDUI::AdjustScreen { \
      public: \
        static _name& it() \
        { \
            static _name ins; \
            return ins; \
        } \
        void onTurn(int8_t dir) override; \
        void onClick() override {}; \
      private: \
        _name() : AdjustScreen(_title, _unit, __min, __max, _val) {}; \
    }

#define DECLARE_ADJUST_CLICKABLE(_name, _title, _unit, __min, __max, _val) \
    class _name : public LEDUI::AdjustScreen { \
      public: \
        static _name& it() \
        { \
            static _name ins; \
            return ins; \
        } \
        void onTurn(int8_t dir) override; \
        void onClick() override; \
      private: \
        _name() : AdjustScreen(_title, _unit, __min, __max, _val) {}; \
    }

#define DECLARE_CHOOSE(_name, _title, _text, _pos, _neg) \
    class _name : public LEDUI::ChooseScreen { \
      public: \
        static _name& it() \
        { \
            static _name ins; \
            return ins; \
        } \
        void onClick() override; \
      private: \
        _name() : ChooseScreen(_title, _text, _pos, _neg) {}; \
    }

// clang-format on
namespace Menus {
    DECLARE_MENU(LanguageSelect, Localization::languagemenu, 2, false, NULL);
    DECLARE_MENU(Preferences, Localization::prefsmenu, 1, false, NULL);
    DECLARE_MENU(GenericChannel, Localization::chmenu_generic, 4, false, NULL);
    DECLARE_MENU(MasterChannel, Localization::chmenu_master, 5, false, NULL);
    DECLARE_MENU(BluetoothChannel, Localization::chmenu_blue, 5, false, NULL);
    DECLARE_MENU(ReverbChannel, Localization::chmenu_reverb, 5, false, NULL);
    DECLARE_MENU(PitchChannel, Localization::chmenu_pitch, 4, false, NULL);
    DECLARE_MENU(ChannelGroup, Localization::groupmenu, 1, false, NULL);
    DECLARE_MENU(SendsOnFader, Localization::sofdestmenu, DSP_BUS_COUNT, false, NULL);
    DECLARE_MENU(Bassboost, Localization::bassboostmenu, 3, false, DSP.getFlagRegisterPtr());
    DECLARE_MENU(MStereoMode, Localization::stereomodemenu, 3, true, NULL);
};  //namespace Menus

// clang-format off
namespace Adjusters {
    DECLARE_ADJUST(ReverbTime, Localization::act->reverb_time,
                   Localization::act->parrots, 0, 3, &DSP.reverbTime);
    DECLARE_ADJUST(ReverbBGain, Localization::act->bassreverbgain,
                   Localization::act->parrots, 0, 2, &DSP.reverbBassGain);
    DECLARE_ADJUST(ReverbHFDamp, Localization::act->hf_damping,
                   Localization::act->parrots, 0, 2, &DSP.reverbHFDamp);
    DECLARE_ADJUST(BassboostGain, Localization::act->gain,
                   Localization::act->parrots, 1, 30, &DSP.bassboostGain);
    DECLARE_ADJUST(BassboostIntens, Localization::act->intensity,
                   Localization::act->parrots, 1, 30, &DSP.bassboostIntensity);
    DECLARE_ADJUST(Pitch, Localization::act->pitch,
                   Localization::act->semitones, -16, 16, &DSP.pitch_shift);
    DECLARE_ADJUST_CLICKABLE(Balance, Localization::act->stereobalance,
                             Localization::act->parrots, -50, 50, NULL);
};  //namespace Adjusters

// clang-format on
namespace Choosers {
    DECLARE_CHOOSE(LangSelectReboot, Localization::act->warning, Localization::act->reboot_warning,
                   Localization::act->ok, Localization::act->cancel);
};  //namespace Choosers