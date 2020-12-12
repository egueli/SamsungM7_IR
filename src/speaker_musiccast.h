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

    int getVolume();
    bool setVolume(int newVolume);

    bool setTvInput();

    bool getMuteStatus(bool &outStatus);
    bool setMuteStatus(const bool newMuteStatus);

private:
    String ipAddress;
    asyncHTTPrequest request;
    bool getZoneUrl(String &output, const String &endPart);
    bool getSystemUrl(String &output, const String &endPart);
    bool checkSuccess();
    bool getStatus(DynamicJsonDocument &outputDoc);
    bool setABSpeakers();
    bool setABSpeaker(const char &letter, const bool enable);
};

