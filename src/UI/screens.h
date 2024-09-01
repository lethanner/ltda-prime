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
      private: \
        _name() : AdjustScreen(_title, _unit, __min, __max, _val) {}; \
    }

// clang-format on
namespace Menus {
    DECLARE_MENU(LanguageSelect, Localization::languagemenu, 2, true, NULL);
    DECLARE_MENU(Preferences, Localization::prefsmenu, 1, false, NULL);
    DECLARE_MENU(GenericChannel, Localization::chmenu_generic, 2, false, NULL);
    DECLARE_MENU(MasterChannel, Localization::chmenu_master, 3, false, NULL);
    DECLARE_MENU(BluetoothChannel, Localization::chmenu_blue, 3, false, NULL);
    DECLARE_MENU(ReverbChannel, Localization::chmenu_reverb, 4, false, NULL);
    DECLARE_MENU(ChannelGroup, Localization::groupmenu, 1, false, NULL);
    DECLARE_MENU(SendsOnFaderAll, Localization::sofdestmenu, 2, false, NULL);
    DECLARE_MENU(SendsOnFaderFX, Localization::sofdestmenu, 1, false, NULL);
    DECLARE_MENU(Bassboost, Localization::bassboostmenu, 3, false, DSP.getFlagRegisterPtr());
};  //namespace Menus

namespace Adjusters {
    DECLARE_ADJUST(ReverbTime, Localization::active()->reverb_time,
                   Localization::active()->parrots, 0, 3, &DSP.reverbTime);
    DECLARE_ADJUST(ReverbBGain, Localization::active()->bassreverbgain,
                   Localization::active()->parrots, 0, 2, &DSP.reverbBassGain);
    DECLARE_ADJUST(ReverbHFDamp, Localization::active()->hf_damping,
                   Localization::active()->parrots, 0, 2, &DSP.reverbHFDamp);
    DECLARE_ADJUST(BassboostGain, Localization::active()->gain,
                   Localization::active()->parrots, 1, 30, &DSP.bassboostGain);
    DECLARE_ADJUST(BassboostIntens, Localization::active()->intensity,
                   Localization::active()->parrots, 1, 30, &DSP.bassboostIntensity);
};  //namespace Adjusters