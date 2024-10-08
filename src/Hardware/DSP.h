#pragma once

#include <Arduino.h>
#include <Wire.h>
#include "registers.h"
#include "../config.h"
#include "bluetooth.h"
#include <GyverIO.h>
#include <A2DPVolumeControl.h>

#define SEMITONE_INCREMENT 0x1894
#define DSPSETS_IS_BASSBOOSTED 0

class A2DPExternalVolumeControl;

class ADAU1452
{
  private:
    A2DPExternalVolumeControl* avrcp_volume_sync = NULL;
    void gotoRegister(short reg, byte requestSize = 0);
    void writeAsFloat(short reg, byte value);
    byte findValue(const unsigned int* tab, byte max, int value);
    int readbackVal_old[DSP_READBACK_COUNT];  // буфер предыдущих значений (для сглаживания)
    int flagRegister = 0x00000000;  // импровизированный регистр настроек

  public:
    enum StereoMode { STEREO = 0, FORCEMONO = 1, VOICESUPPRESS = 2 };

    ADAU1452();
    void init();

    byte getCoreState();
    void retrieveRTAValues();

    byte getRelativeSignalLevel(const unsigned int* tab, byte range, byte id, bool right);
    int8_t getDecibelSignalLevel(byte id, bool right);

    bool isMonoChannel(byte id) { return id > DSP_STEREO_BEFORE - 2; }
    void setDecibelFaderPosition(byte id, int8_t val, bool sync = true);
    void setDecibelSendLevel(byte id, byte to, int8_t val);
    void setStereoBalance(byte id, int8_t val);
    void setStereoMode(byte id, StereoMode mode);
    int8_t faderPosition_dB[DSP_FADER_COUNT];  // буфер положений фейдеров внутри DSP
    int8_t sendFaders_dB[DSP_BUS_COUNT][DSP_BUS_CHANNELS];  // буфер уровней посылов внутри DSP
    int8_t balpan[DSP_FADER_COUNT];  // буфер положений регуляторов стереобаланса/панорамы

    // Mute
    void toggleMute(byte id);
    void toggleMute(byte id, byte to);
    bool muteFlags[DSP_FADER_COUNT];  // флаги MUTE для каналов
    bool sendMuteFlags[DSP_BUS_COUNT][DSP_BUS_CHANNELS];  // флаги MUTE для посылов
    //int8_t getDecibelFaderPosition(byte id);

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

    // буфер значений уровней сигнала
    int readbackVal[DSP_READBACK_COUNT];

    // прочие костыли
    int* getFlagRegisterPtr();
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
        _dspptr->setDecibelFaderPosition(FADER_BLUETOOTH_ST, map(volume, 0, 127, -97, 10), false);
    }

    A2DPExternalVolumeControl(ADAU1452* dspptr) { _dspptr = dspptr; }
};

extern ADAU1452 DSP;