#pragma once
#include <Arduino.h>
#include <GyverIO.h>
#include "../config.h"
#include "channels.h"
#include "freertos/semphr.h"

#define SHIFT_AMP_RESET 0
#define SHIFT_DSP_RESET 7

#define SHIFT_MAIN_DEFAULT 0b10000001
#define SHIFT_COUNT 7

class ShiftRegisters
{
  public:
    ShiftRegisters();
    void quickInit();
    void refresh();

    void setOnBoardBit(uint8_t bit, bool state);
    void setAuxGain(DSPChannels::channel ch, int8_t value);
    void setClippingIndicator(DSPChannels::channel ch, bool value);
    void sendToIndicators(int buf);

    int8_t* getAuxGainPtr(DSPChannels::channel ch);
    static const char* const gainValuesAliases[];
  private:
    byte _buffer[SHIFT_COUNT];
    int8_t _auxGains[6];
};

extern ShiftRegisters shifters;