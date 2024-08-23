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
        void onClick() const override; \
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
        void onTurn(int8_t dir) const override; \
      private: \
        _name() : AdjustScreen(_title, _unit, __min, __max, _val) {}; \
    }

// clang-format on
namespace Menus
{
DECLARE_MENU(GenericChannel, Localization::chmenu_generic, 1, false, NULL);
DECLARE_MENU(MasterChannel, Localization::chmenu_master, 2, false, NULL);
DECLARE_MENU(BluetoothChannel, Localization::chmenu_blue, 2, false, NULL);
DECLARE_MENU(ReverbChannel, Localization::chmenu_reverb, 3, false, NULL);
DECLARE_MENU(ChannelGroup, Localization::groupmenu, 1, false, NULL);
DECLARE_MENU(Bassboost, Localization::bassboostmenu, 3, false, DSP.getFlagRegisterPtr());
};

namespace Adjusters
{
DECLARE_ADJUST(ReverbTime, Localization::active->reverb_time,
               Localization::active->parrots, 0, 3, &DSP.reverbTime);
DECLARE_ADJUST(ReverbBGain, Localization::active->bassreverbgain,
               Localization::active->parrots, 0, 2, &DSP.reverbBassGain);
DECLARE_ADJUST(ReverbHFDamp, Localization::active->hf_damping,
               Localization::active->parrots, 0, 2, &DSP.reverbHFDamp);
DECLARE_ADJUST(BassboostGain, Localization::active->gain,
               Localization::active->parrots, 1, 30, &DSP.bassboostGain);
DECLARE_ADJUST(BassboostIntens, Localization::active->intensity,
               Localization::active->parrots, 1, 30, &DSP.bassboostIntensity);
};

namespace Mixers
{
extern LEDUI::MixerScreen mix_inputs;
extern LEDUI::MixerScreen mix_fx;
extern LEDUI::MixerScreen mix_outputs;
extern LEDUI::MixerScreen mix_all;
};