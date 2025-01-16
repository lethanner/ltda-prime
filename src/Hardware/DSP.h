#pragma once

#include <Arduino.h>
#include <Wire.h>
#include "registers.h"
#include "channels.h"
#include "../config.h"
#include "bluetooth.h"
#include <GyverIO.h>
#include <A2DPVolumeControl.h>

#define SEMITONE_INCREMENT 0x1894
#define DSPSETS_IS_BASSBOOSTED 0

typedef DSPChannels::channel channel;
typedef DSPChannels::bus bus;
class A2DPExternalVolumeControl;

class ADAU1452
{
  private:
    A2DPExternalVolumeControl* avrcp_volume_sync = NULL;
    void gotoRegister(__register reg, byte requestSize = 0);
    void writeAsFloat(__register reg, byte value);
    byte findValue(const unsigned int* tab, byte max, int value);
    int flagRegister = 0x00000000;  // импровизированный регистр настроек

  public:
    ADAU1452();
    void init();

    byte getCoreState();
    void retrieveRTAValues();

    bool isMonoChannel(channel id) { return DSPChannels::list[id]->readback[1] == 0; }
    bool getMute(channel id) { return DSPChannels::list[id]->mute; }
    bool getMute(channel id, bus to) { return DSPChannels::list[id]->sends[to].mute; }
    decibel getFaderPosition(channel id) { return DSPChannels::list[id]->faderPosition; }
    decibel getFaderPosition(channel id, bus to) { return DSPChannels::list[id]->sends[to].faderPosition; }
    byte getRelativeSignalLevel(const unsigned int* tab, byte range, channel id, bool right);
    int8_t getStereoBalance(channel id) { return DSPChannels::list[id]->balpan; }
    DSPChannels::Channel *const getChannelPointer(channel id) { return DSPChannels::list[id]; }
    //int8_t getDecibelSignalLevel(channel id, bool right);

    void setDecibelFaderPosition(channel id, decibel val, bool sync = true);
    void setDecibelSendLevel(channel id, bus to, decibel val);
    void setStereoBalance(channel id, int8_t val);
    void setStereoMode(channel id, DSPChannels::StereoMode mode);

    // Mute
    void toggleMute(channel id);
    void toggleMute(channel id, bus to);

    // TODO: возможно, поместить все переменные ниже под inline-функции get()
    // в то же время затруднительно из-за того, что к ним надо обращаться через указатель
    
    // TODO2: взять эти переменные и вместе с flagRegister поместить в одну структуру

    // бассбуст
    void toggleBassBoost();
    void setBBIntensity(byte value);
    void setBBGain(byte value);
    int8_t bassboostIntensity = 10,
           bassboostGain = 20;  // min 0.1, max 3 (* 10)

    // реверб
    void setReverbTime(byte value);
    void setReverbHFDamping(byte value);
    void setReverbBassGain(byte value);
    int8_t reverbTime = 2, reverbHFDamp = 1, reverbBassGain = 1;

    // питч
    void setPitchBusShift(int8_t value);
    int8_t pitch_shift = 0;

    // прочие костыли
    int* getFlagRegisterPtr() { return &flagRegister; };
};

class A2DPExternalVolumeControl : public A2DPVolumeControl
{
  private:
    ADAU1452* _dspptr;

  public:
    virtual void update_audio_data(Frame* data, uint16_t frameCount) override
    {
        // пустая функция перебивает встроенные в библиотеку блютуза
        // средства управления громкостью
    }
    virtual void set_volume(uint8_t volume) override
    {
        _dspptr->setDecibelFaderPosition(DSPChannels::BLUETOOTH, map(volume, 0, 127, -97, 10), false);
    }

    A2DPExternalVolumeControl(ADAU1452* dspptr) { _dspptr = dspptr; }
};

extern ADAU1452 DSP;