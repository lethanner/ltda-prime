#pragma once

#include <Arduino.h>
#include <BluetoothA2DPSink.h>

class LTDABT : public BluetoothA2DPSink
{
  public:
    void init();
	void sendAVRCPVolume(int8_t volume);
};

extern LTDABT bluetooth;