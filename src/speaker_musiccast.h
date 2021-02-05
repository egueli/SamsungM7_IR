#pragma once

#include <asyncHTTPrequest.h>
#include <ArduinoJson.h>
#include "speaker.h"

/**
 * Controls a MusicCast-compatible speaker.
 */
class MusicCastSpeaker : public Speaker {
public:
    void setAddress(const String &address);
    bool isAddressValid();

    Result getVolume(int &outVolume);
    Result setVolume(int newVolume);

    Result setTvInput();

    Result getMuteStatus(bool &outStatus);
    Result setMuteStatus(const bool newMuteStatus);

private:
    String ipAddress;
    asyncHTTPrequest request;
    Result getZoneUrl(String &output, const String &endPart);
    Result getSystemUrl(String &output, const String &endPart);
    Result checkSuccess();
    Result getStatus(DynamicJsonDocument &outputDoc);
    Result powerOn();
    Result setABSpeakers();
    Result setABSpeaker(const char &letter, const bool enable);
};

