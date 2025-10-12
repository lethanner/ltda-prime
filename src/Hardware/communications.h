#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include "bluetooth.h"

class Communications {
public:
    enum RadioMode { OFF = 0, WIFI, BT };
    bool setRadio(RadioMode mode);
private:
    RadioMode current = OFF;
};

extern Communications comm;