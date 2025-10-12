#include "communications.h"
#include "../config.h"

bool Communications::setRadio(RadioMode mode) {
    if (mode == current)
        return true;

    bool state = true;

    bluetooth.end();
    state = WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);

    if (mode == WIFI) {
        WiFi.mode(WIFI_AP);
        state = WiFi.softAP(WIFI_DEFAULT_NAME, WIFI_DEFAULT_PASS);
    }
    else if (mode == BT) {
        bluetooth.init();
    }
    
    current = mode;
    return state;
}

Communications comm;