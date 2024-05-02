#pragma once
#include <Arduino.h>
#include <GyverIO.h>
#include "../config.h"
#include "freertos/semphr.h"

#define SHIFT_AMP_RESET 0
#define SHIFT_DSP_RESET 7

#define SHIFT_MAIN_DEFAULT 0b10000001
#define SHIFT_COUNT 4

class ShiftRegisters
{
  public:
    ShiftRegisters();
    void quickInit(bool reset);
    void refresh();

    void modifyBit(uint8_t reg, uint8_t bit, bool state);
    void setOnBoardBit(uint8_t bit, bool state);
    void sendToIndicators(int buf);
  private:
    byte _buffer[SHIFT_COUNT];
};

extern ShiftRegisters shifters;