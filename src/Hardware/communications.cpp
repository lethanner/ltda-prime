#include "communications.h"
#include "../System/memory.h"
#include "../config.h"

const IPAddress mcastAddr(MULTICAST_IP);

byte Communications::tcpClients = 0;
void (*Communications::connectionCallback)(byte) = NULL;
const char *(*Communications::dataCallback)(char *) = NULL;
void (*Communications::wifiStatusCallback)(wl_status_t) = NULL;

Communications::Communications()
{
    tcp = new AsyncServer(TCP_PORT);
    tcp->onClient(&Communications::onTCPConnected, tcp);
}

void Communications::tick()
{
    wl_status_t status = WiFi.status();
    if (status != conn) {
        conn = status;
        if (wifiStatusCallback) wifiStatusCallback(conn);
    }

    while (Serial.available()) {
        static uint16_t bufferPos = 0;
        static bool signatureValid = false;

        if (bufferPos >= 128) {
            signatureValid = false;
            bufferPos = 0;
        }
        uartBuffer[bufferPos++] = Serial.read();

        if (bufferPos > 7) {
            if (!signatureValid && memcmp("LTDA", uartBuffer, 4) == 0) {
                signatureValid = true;
            }

            if (signatureValid && uartBuffer[4] == 0x01) {
                uint16_t packetLength = uartBuffer[6] << 8 | uartBuffer[7];
                if (packetLength == 0) {
                    if (liveData != NULL) Serial.write(liveData, liveDataSize);
                } 
                else if (bufferPos < packetLength + 8)
                    continue;
                else {
                    const char *response = NULL;
                    response = dataCallback(&uartBuffer[8]);

                    uint16_t respLen = strlen(response);
                    Serial.write("LTDA\x01\x80", 6);
                    Serial.write((respLen >> 8) & 0xFF);
                    Serial.write((respLen) & 0xFF);
                    Serial.print(response);
                }
            }
            // сброс буфера
            uartBuffer[0] = uartBuffer[8] = '\0';
            signatureValid = false;
            bufferPos = 0;
        }
        
    }
}

bool Communications::setRadio(RadioMode mode)
{
    if (mode == current)
        return true;

    //udp.stop();
    bluetooth.end();
    bluetooth.set_connectable(ESP_BT_NON_CONNECTABLE);
    bluetooth.set_discoverability(ESP_BT_NON_DISCOVERABLE);
    WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);

    bool state = true;
    if (mode == _WIFI_STA) {
        char ssid[32], password[32];
        // если данные для подключения не настроены
        if (!memory.getBool("w_set", false)) 
            return false;
        memory.getString("w_ssid", ssid, 32);
        memory.getString("w_pass", password, 32);
        WiFi.mode(WIFI_STA);
        state = WiFi.begin(ssid, password);

        udp.beginMulticast(mcastAddr, MULTICAST_PORT);
        tcp->begin();
    } else if (mode == _WIFI_AP) {
        WiFi.mode(WIFI_AP);
        state = WiFi.softAP(WIFI_DEFAULT_NAME, WIFI_DEFAULT_PASS);

        udp.beginMulticast(mcastAddr, MULTICAST_PORT);
        tcp->begin();
    } else if (mode == BT) {
        bluetooth.init();
        bluetooth.set_connectable(ESP_BT_CONNECTABLE);
        bluetooth.set_discoverability(ESP_BT_GENERAL_DISCOVERABLE);
    }

    current = mode;
    return state;
}

void Communications::transferLiveData()
{
    if (liveData == NULL || tcpClients == 0 || conn != WL_CONNECTED) return;

    udp.beginMulticastPacket();
    udp.write(liveData, liveDataSize);
    udp.endPacket();
}

void Communications::setWiFiCretendials(const char* ssid, const char* pass)
{
    memory.putBool("w_set", strlen(ssid) > 1 ? true : false);
    memory.putString("w_ssid", ssid);
    memory.putString("w_pass", pass);
}

void Communications::onTCPConnected(void *arg, AsyncClient *client)
{
    tcpClients += 1;

    client->onData(&onTCPData, NULL);
    client->onError(&onTCPError, NULL);
    client->onDisconnect(&onTCPDisconnected, NULL);
    client->onTimeout(&onTCPTimeout, NULL);

    client->setRxTimeout(20);

    if (connectionCallback)
        connectionCallback(tcpClients);
}

void Communications::onTCPDisconnected(void *arg, AsyncClient *client)
{
    tcpClients -= 1;

    if (connectionCallback)
        connectionCallback(tcpClients);
}

void Communications::onTCPData(void *arg, AsyncClient *client, void *data, size_t len)
{
    if (!dataCallback)
        return;

    log_i("Incoming: %s\n", static_cast<char*>(data));

    const char *response = NULL;
    response = dataCallback(static_cast<char*>(data));

    log_i("Outgoing: %s\n", response);

    if (!response)
        return;

    size_t size = strlen(response);
    if (client->space() > size && client->canSend()) {
        client->add(response, size);
        client->send();
    }
}

void Communications::onTCPError(void *arg, AsyncClient *client, int8_t error) {}
void Communications::onTCPTimeout(void *arg, AsyncClient *client, uint32_t time) {}

Communications comm;