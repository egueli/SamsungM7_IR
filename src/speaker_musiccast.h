#pragma once

#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>
#include "speaker.h"

/**
 * Controls a MusicCast-compatible speaker.
 */
class MusicCastSpeaker : public Speaker {
public:
    MusicCastSpeaker() {
        // httpClient.setReuse(true);
    }

    Configuration getConfiguration();

    void setAddress(const String &address);
    bool isAddressValid();

    Result getVolume(int &outVolume);
    Result setVolume(int newVolume);

    Result setTvInput();

    Result getMuteStatus(bool &outStatus);
    Result setMuteStatus(const bool newMuteStatus);

private:
    String ipAddress;
    WiFiClient wifiClient;
    HTTPClient httpClient;
    Result getZoneUrl(String &output, const String &endPart);
    Result getSystemUrl(String &output, const String &endPart);
    Result checkSuccess(const int httpCode);
    Result getStatus(DynamicJsonDocument &outputDoc);
    Result powerOn();
    Result setABSpeakers();
    Result setABSpeaker(const char &letter, const bool enable);
};

