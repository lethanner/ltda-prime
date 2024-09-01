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

namespace LEDUI {
    class MenuScreen;
    class MixerScreen;
    class AdjustScreen;

    class Screen
    {
      public:
        Screen() {};
        virtual void init(void *params) = 0;
        virtual void render() = 0;
        virtual void onClick() = 0;
        virtual void onHold() = 0;
        virtual void onTurn(int8_t dir) = 0;
    };

    void init();
    void reset();
    void render();
    void pollCtrl();

    void open(Screen *scr, void *params = NULL);

    byte getCenterCoordinate(const char *text);
    void printValue(int8_t value, const char *label, int8_t x_coord, byte y_coord, bool center = false);
    void printYX(const char *text, byte y_coord, int8_t x_coord = -1);
    void printRightAlign(const char *text, byte y_coord);
    void bootStatus(const char *text, byte y_coord);

    void vUITimerCallback(TimerHandle_t pxTimer);
    void brightDisplay();

    extern byte monitor_ch;
    inline void setMonitorDataFeed(byte ch) { monitor_ch = ch; }
    void streamMonitorData();

    extern byte title_xCoord;
    extern byte screen_state, statusbar;
    extern Screen *active;

    extern GyverOLED<SSD1306_128x64, OLED_BUFFER> display;
};  //namespace LEDUI

class LEDUI::MenuScreen : public LEDUI::Screen
{
  public:
    MenuScreen(const char *const *entries, byte e_count, bool autoclick, int *booleans)
      : _entries(entries), _e_count(e_count - 1), _autoclick(autoclick), _booleans(booleans) {};
    static MenuScreen *active;

  private:
    void init(void *params = NULL) override;
    void render() override;
    // void onClick() const override;
    void onHold() override;
    void onTurn(int8_t dir) override;

    const char *const *_entries;
    const byte _e_count;
    const bool _autoclick;
    const int *_booleans;

    static byte visibleSel, entryRendererStart;

  protected:
    static byte selected;
    void calculateTitleCenter() { title_xCoord = getCenterCoordinate(_entries[0]); }
    //void return_to_mixer() const { open(MixerScreen::active); }
};

class LEDUI::MixerScreen : public LEDUI::Screen
{
  public:
    static MixerScreen &it()
    {
        static MixerScreen ins;
        return ins;
    }

    enum SoFMode { NO_SOF, FX_SOF, ALL_SOF };
    struct ChannelGroup {
        const char *name;
        const byte *onScreenChannels;
        const byte count;
        const SoFMode sof;
    };

    static const ChannelGroup groups[GROUPS_COUNT];
    static const char ch_labels[][7];
    static const char *sendto_labels[];

    void setGroup(int8_t num);
    SoFMode isSoFAllowed() const { return _group->sof; }
    byte getSelectedChannel() const { return _group->onScreenChannels[selected]; }

  private:
    MixerScreen() : _group(&groups[0]) {};
    void statusbarDecibels() const;

    void init(void *params = NULL) override;
    void render() override;
    void onClick() override;
    void onHold() override;
    void onTurn(int8_t dir) override;

    const ChannelGroup *_group;
    byte gap_block = 0, selected = 0, SoFdest = 0, selectedGroup = 0;
    bool turn_started = false, usingSoF = false;
};

class LEDUI::AdjustScreen : public LEDUI::Screen
{
  public:
    AdjustScreen(const char *title, const char *unit, int8_t min, int8_t max, int8_t *value)
      : _title(title), _unit(unit), __min(min), __max(max), _value(value) {};

  private:
    void init(void *params = NULL) override;
    void render() override;
    void onClick() override;
    void onHold() override;
    // void onTurn(int8_t dir) const override;

    const char *_title, *_unit;
    const int8_t __min, __max, *_value;
};