#include "UI.h"
#include "ArduinoJson.h"
#include "../Hardware/communications.h"
#include "../Hardware/lut.h"

const size_t LEDUI::liveSize = 6 + (DSP_CHANNEL_COUNT * 7);
char LEDUI::liveBuf[liveSize] = { 'L', 'T', 'D', 'A', 0x01, 0x00 };

void LEDUI::transferLiveData()
{
    byte inBuffer = 6;
    for (byte i = 0; i < DSP_CHANNEL_COUNT; i++) {
        channel ch = static_cast<channel>(i);

        liveBuf[inBuffer++] = 0x43;
        liveBuf[inBuffer++] = 0x48;
        liveBuf[inBuffer++] = DSP.getFaderPosition(ch);
        liveBuf[inBuffer++] = DSP.getRelativeSignalLevel(LUT::db_24bit, 97, ch, false);
        liveBuf[inBuffer++] = DSP.getRelativeSignalLevel(LUT::db_24bit, 97, ch, true);
        liveBuf[inBuffer++] = DSP.getStereoBalance(ch);
        liveBuf[inBuffer++] = static_cast<char>(DSP.getMute(ch));
    }

    comm.transferLiveData();
}

const char* LEDUI::processRemoteCommand(char* command)
{
    static char response[2048];
    static DRAM_ATTR const char* const ok = "{\"status\":\"ok\"}";
    static DRAM_ATTR const char* const syntaxError = "{\"status\":\"error\",\"why\":\"syntax\"}";
    static DRAM_ATTR const char* const overflowError = "{\"status\":\"error\",\"why\":\"overflow\"}";

    StaticJsonDocument<64> jRequest;
    static DynamicJsonDocument jResponse(2048);

    jResponse.clear();
    response[0] = '\0';
    DeserializationError error = deserializeJson(jRequest, command);

    log_i("After deserialization: %s\n", command);
    if (error) {
        jResponse["status"] = "error";
        const char* why;
        if (error == DeserializationError::NoMemory || error == DeserializationError::TooDeep)
            why = "tl;dr";
        else if (strlen(command) == 1)  // empty string
            why = "because";
        else if (error == DeserializationError::IncompleteInput || error == DeserializationError::InvalidInput)
            why = "corrupted";
        else why = "teapot";

        jResponse["why"] = why;
        serializeJson(jResponse, response);

        return response;
    }

    const char* cmd = jRequest["cmd"] | "";
    if (strcmp(cmd, "ping") == 0) return ok;
    else if (strcmp(cmd, "list") == 0) {
        JsonArray channels = jResponse.createNestedArray("channels");
        for (byte i = 0; i < DSP_CHANNEL_COUNT; i++) {
            JsonObject ch = channels.createNestedObject();
            ch["n"] = LEDUI::MixerScreen::ch_labels[i];
            ch["st"] = !DSP.isMonoChannel(static_cast<channel>(i));
            ch["t"] = DSPChannels::list[i]->type;
        }

        JsonArray groups = jResponse.createNestedArray("groups");
        for (byte i = 0; i < GROUPS_COUNT; i++) {
            const LEDUI::MixerScreen::ChannelGroup* realGroup = &LEDUI::MixerScreen::groups[i];
            JsonObject group = groups.createNestedObject();

            group["n"] = realGroup->name;
            for (byte j = 0; j < realGroup->count; j++) {
                group["c"].add(realGroup->onScreenChannels[j]);
            }
        }

        serializeJson(jResponse, response);
    } else if (strcmp(cmd, "info") == 0) {
        jResponse["i"] = "LTDA";
        jResponse["v"] = STR_DEV_INFO;

        uint8_t multicastIp[4] = { MULTICAST_IP };
        jResponse["mca"] = static_cast<uint32_t>(
            multicastIp[0] << 24 | multicastIp[1] << 16 | multicastIp[2] << 8 | multicastIp[3]
        );
        jResponse["mcp"] = MULTICAST_PORT;
        jResponse["lsize"] = liveSize;

        serializeJson(jResponse, response);
    }
    else if (strcmp(cmd, "wifi-sta") == 0) {
        if (!jRequest.containsKey("ssid") || !jRequest.containsKey("pass")) 
            return syntaxError;
        else {
            const char* ssid = jRequest["ssid"];
            const char* password = jRequest["pass"];

            if (strlen(ssid) > 32 || strlen(password) > 32)
                return overflowError;

            comm.setWiFiCretendials(ssid, password);
            return ok;
        }
    }
    /* Управление каналами */
    else if (jRequest.containsKey("ch")) {
        channel ch = jRequest["ch"];
        if (!(ch < DSP_CHANNEL_COUNT)) return overflowError;

        /* Mute */
        if (strcmp(cmd, "mute") == 0) {
            DSP.toggleMute(ch);
            return ok;
        }
        /* Fader */
        else if (strcmp(cmd, "fader") == 0) {
            if (jRequest.containsKey("val")) {
                int8_t val = jRequest["val"];
                DSP.setDecibelFaderPosition(ch, val);
                return ok;
            } else return syntaxError;
        }
    } else return syntaxError;

    return response;
}

void LEDUI::remoteUpdated(byte count) { statusbarMessage(" --- ))) --- "); }