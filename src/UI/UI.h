#pragma once
#include <Arduino.h>
#include <GyverOLED.h>
#include <GyverIO.h>
#include "config.h"
#include "bitmaps.h"
#include "localization.h"
#include "../Hardware/DSP.h"
#include "../Hardware/shiftreg.h"
#include "../Hardware/communications.h"
#include "freertos/timers.h"

#define GROUPS_COUNT 7

namespace LEDUI {
    class MenuScreen;
    class MixerScreen;
    class AdjustScreen;
    class ChooseScreen;
    class NotificationScreen;

    class Screen
    {
      public:
        Screen() {};
        virtual bool init(void *params) = 0;
        virtual void render() = 0;
        virtual void onClick() = 0;
        virtual void onHold() = 0;
        virtual void onTurn(int8_t dir) = 0;
    };

    void init();
    void reset();
    void render();
    void pollCtrl();

    bool open(Screen *scr, void *params = NULL);

    byte getCenterCoordinate(const char *text);
    void printValue(int8_t value, const char *label, int8_t x_coord, byte y_coord, bool center = false);
    void printYX(const char *text, byte y_coord, int8_t x_coord = -1);
    void printRightAlign(const char *text, byte y_coord);
    void bootStatus(const char *text, byte y_coord);

    void vUITimerCallback(TimerHandle_t pxTimer);
    void brightDisplay();

    extern channel monitor_ch;
    inline void setMonitorDataFeed(channel ch) { monitor_ch = ch; }
    void streamMonitorData();

    void statusbarMessage(const char* message);
    void wifiStatusUpdated(wl_status_t status);

    void transferLiveData();
    const char* processRemoteCommand(char* command);
    void remoteUpdated(byte count);

    extern byte title_xCoord;
    extern byte screen_state, statusbar;
    extern const char* sbMessage;
    extern Screen *active;

    extern GyverOLED<SSD1306_128x64, OLED_BUFFER> display;
};  //namespace LEDUI

class LEDUI::MenuScreen : public LEDUI::Screen
{
  public:
    MenuScreen(const char *const *entries, byte e_count, bool autoclick, int *booleans)
      : _entries(entries), _e_count_static(e_count - 1), _autoclick(autoclick), _booleans(booleans) {};
    void overrideEntryCount(byte newcount) { _e_count = newcount - 1; }
    void overrideSelection(byte newselect)
    {
        selected = newselect, visibleSel = newselect;
    }
    // TODO: пока что overrideSelection() не рассчитан на многостраничные меню, переделать
    static MenuScreen *active;

  private:
    bool init(void *params = NULL) override;
    void render() override;
    // void onClick() const override;
    void onHold() override;
    void onTurn(int8_t dir) override;

    const char *const *_entries;
    const byte _e_count_static;
    const bool _autoclick;
    const int *_booleans;

    static byte visibleSel, entryRendererStart;
    static byte _e_count;

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
  
    struct ChannelGroup {
        const char *name;
        const channel *onScreenChannels;
        const byte count;
    };

    static const ChannelGroup groups[GROUPS_COUNT];
    static const char ch_labels[][7];
    static const char *sendto_labels[];

    void setGroup(int8_t num);
    channel getSelectedChannel() const { return _group->onScreenChannels[selected]; }

  private:
    MixerScreen() : _group(&groups[0]) {};
    void statusbarDecibels() const;

    bool init(void *params = NULL) override;
    void render() override;
    void onClick() override;
    void onHold() override;
    void onTurn(int8_t dir) override;

    const ChannelGroup *_group;
    byte _static[3][10];
    byte selected = 0, selectedGroup = 0;
    byte count;
    bus SoFdest;
    bool turn_started = false, usingSoF = false;
};

class LEDUI::AdjustScreen : public LEDUI::Screen
{
  public:
    AdjustScreen(const char *title, const char *unit, int8_t min, int8_t max, int8_t *value, const char *const *aliases)
      : _title(title), _unit(unit), __min(min), __max(max), _value(value), _aliases(aliases) {};
    void overrideValuePtr(int8_t *newptr);

  private:
    bool init(void *params = NULL) override;
    void render() override;
    // void onClick() override;
    void onHold() override;
    // void onTurn(int8_t dir) const override;

    const char *const *_aliases;
    const char *_title, *_unit;
    const int8_t __min, __max;
    int8_t *_value;
};

class LEDUI::ChooseScreen : public LEDUI::Screen
{
  public:
    ChooseScreen(const char *title, const char *text, const char *positive, const char *negative = NULL)
      : _title(title), _text(text), _neg(negative), _pos(positive) {};

  private:
    bool init(void *params = NULL) override;
    void render() override;
    //void onClick() override;
    void onHold() override;
    void onTurn(int8_t dir) override;

    const char *_title, *_text, *_neg, *_pos;

  protected:
    static bool confirmation;
    static void *_params;
};

/*class LEDUI::NotificationScreen : public LEDUI::Screen
{
  public:
    NotificationScreen(const char* ) {};

  private:
    bool init(void *params = NULL) override;
    void render() override;
    void onClick() override;
    void onHold() override;
    void onTurn(int8_t dir) override;
};*/