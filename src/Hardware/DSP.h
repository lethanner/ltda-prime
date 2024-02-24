#pragma once

#include <Arduino.h>
#include <Wire.h>
#include "slots.h"
#include "../config.h"
#include "bluetooth.h"
#include <A2DPVolumeControl.h>

class ADAU1452
{
  private:
    void gotoRegister(short reg, byte requestSize = 0);
    void setFaderPosition(byte id, int val);
    byte findValue(const unsigned int* tab, byte max, int value);
    int readbackVal_old[DSP_READBACK_COUNT];  // буфер предыдущих значений (для сглаживания)

  public:
    ADAU1452();

    byte getCoreState();
    void retrieveRTAValues();

    byte getRelativeSignalLevel(const unsigned int* tab, byte range, byte id, bool right);
    void setDecibelFaderPosition(byte id, int8_t val, bool sync = true);
    int8_t getDecibelSignalLevel(byte id, bool right);
    //int8_t getDecibelFaderPosition(byte id);

    int readbackVal[DSP_READBACK_COUNT];      // буфер значений уровней сигнала
    int faderPosition[DSP_FADER_COUNT];       // буфер положений фейдеров внутри DSP
    int8_t faderPositionDB[DSP_FADER_COUNT];  // тот же буфер, только в децибелах
};

extern ADAU1452 DSP;

class A2DPSyncedVolumeControl : public A2DPVolumeControl
{
  public:
    virtual void update_audio_data(Frame* data, uint16_t frameCount) override
    {
        // пустая функция перебивает встроенные в библиотеку блютуза
        // средства управления громкостью
    }
    virtual void set_volume(uint8_t volume) override
    {
        DSP.setDecibelFaderPosition(FADER_BLUETOOTH_ST, map(volume, 0, 127, -97, 0), false);
    }
};