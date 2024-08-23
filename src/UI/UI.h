#pragma once
#include <Arduino.h>
#include <GyverOLED.h>
#include <GyverIO.h>
#include "config.h"
#include "bitmaps.h"
#include "localization.h"
#include "../Hardware/DSP.h"
#include "freertos/timers.h"

#define GROUPS_COUNT 4

namespace LEDUI
{
    class MenuScreen;
    class MixerScreen;
    class AdjustScreen;

    class Screen
    {
    public:
        Screen() {};
        virtual void init(void* params) const = 0;
        virtual void render() const = 0;
        virtual void onClick() const = 0;
        virtual void onHold() const = 0;
        virtual void onTurn(int8_t dir) const = 0;
    };

    void init();
    void reset();
    void render();
    void pollCtrl();

    void open(const Screen *scr, void *params = NULL);

    byte getCenterCoordinate(const char *text);
    void printValue(int8_t value, const char *label, int8_t x_coord,
                    byte y_coord, bool center = false);
    void printYX(const char *text, byte y_coord, int8_t x_coord = -1);
    void bootStatus(const char *text, byte y_coord);

    void vUITimerCallback(TimerHandle_t pxTimer);
    void brightDisplay();

    static inline byte monitor_ch;
    inline void setMonitorDataFeed(byte ch) { monitor_ch = ch; }
    void streamMonitorData();
    
    static inline byte title_xCoord;
    static inline byte screen_state, statusbar;
    static inline const Screen *active;

    extern GyverOLED<SSD1306_128x64, OLED_BUFFER> display;
};

class LEDUI::MenuScreen : public LEDUI::Screen
{
public:
    MenuScreen(const char *const *entries, byte e_count, bool autoclick, int *booleans)
        : _entries(entries), _e_count(e_count - 1), _autoclick(autoclick), _booleans(booleans) {};
    static inline const MenuScreen *active;

private:
    void init(void* params = NULL) const override;
    void render() const override;
    // void onClick() const override;
    void onHold() const override;
    void onTurn(int8_t dir) const override;

    const char *const *_entries;
    const byte _e_count;
    const bool _autoclick;
    const int *_booleans;

    static inline byte visibleSel, entryRendererStart;

protected:
    static inline byte selected;
    //void return_to_mixer() const { open(MixerScreen::active); }
};

class LEDUI::MixerScreen : public LEDUI::Screen
{
public:
    enum SoFMode {
        NO_SOF,
        FX_SOF,
        ALL_SOF
    };
    struct ChannelGroup {
        const byte num;
        const char *name;
        const byte *onScreenChannels;
        const byte count;
        const SoFMode sof;
    };
    static inline const MixerScreen *active;
    // зачем я это делаю... если все равно один обьект MixerScreen - одна группа каналов...
    // зачем я тогда пихаю все свои группы в сам класс MixerScreen...
    static const ChannelGroup groups[GROUPS_COUNT];
    static const char ch_labels[][7];

    MixerScreen(const ChannelGroup *group) : _group(group) {};
    SoFMode isSoFAllowed() const { return _group->sof; }
    byte getSelectedChannel() const { return _group->onScreenChannels[selected]; }

private:
    void init(void* params = NULL) const override;
    void render() const override;
    void onClick() const override;
    void onHold() const override;
    void onTurn(int8_t dir) const override;

    // const byte *onScreenChannels, _count;
    // const bool _allowSoF;
    const ChannelGroup *_group;

    static inline byte gap_block;
    static inline byte selected;
    static inline byte SoFdest;
    static inline bool turn_started;
    static inline bool usingSoF;
};

class LEDUI::AdjustScreen : public LEDUI::Screen
{
public:
    AdjustScreen(const char *title, const char *unit, int8_t min, int8_t max, int8_t *value)
        : _title(title), _unit(unit), __min(min), __max(max), _value(value) {};

private:
    void init(void* params = NULL) const override;
    void render() const override;
    void onClick() const override;
    void onHold() const override;
    // void onTurn(int8_t dir) const override;

    const char *_title, *_unit;
    const int8_t __min, __max, *_value;
};