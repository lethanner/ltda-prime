#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <AsyncTCP.h>
#include "bluetooth.h"
#include "../config.h"

class Communications {
public:
    Communications();
    void tick();

    enum RadioMode { OFF = 0, _WIFI_AP, _WIFI_STA, BT };
    bool setRadio(RadioMode mode);

    void transferLiveData();

    void setConnectionCallback(void (*callback)(byte)) { connectionCallback = callback; }
    void setDataCallback(const char* (*callback)(char*)) { dataCallback = callback; }
    void setWiFiStatusCallback(void (*callback)(wl_status_t)) { wifiStatusCallback = callback; }
    void setLiveDataSource(uint8_t* source, size_t size) { liveData = source, liveDataSize = size; }

    void setWiFiCretendials(const char* ssid, const char* pass);

private:
    RadioMode current = OFF;
    wl_status_t conn = WL_DISCONNECTED;

    AsyncServer* tcp;
    WiFiUDP udp;
    static byte tcpClients;

    static void onTCPConnected(void *arg, AsyncClient *client);
    static void onTCPDisconnected(void *arg, AsyncClient *client);
    static void onTCPData(void *arg, AsyncClient *client, void *data, size_t len);
    static void onTCPError(void *arg, AsyncClient *client, int8_t error);
    static void onTCPTimeout(void *arg, AsyncClient *client, uint32_t time);

    static void (*connectionCallback)(byte);
    static const char* (*dataCallback)(char*);
    static void (*wifiStatusCallback)(wl_status_t);

    char uartBuffer[128];

    uint8_t* liveData = NULL;
    size_t liveDataSize;
};

extern Communications comm;