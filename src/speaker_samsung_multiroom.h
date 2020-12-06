#pragma once

#include "speaker.h"
#include <asyncHTTPrequest.h>

/**
 * Controls an IP-accessible audio speaker.
 */
class SamsungMultiroomSpeaker : public Speaker {
public:
    void setup();
    void loop();
    void setAddress(String address);

    int getVolume();
    bool setVolume(int newVolume);

    bool setTvInput();

    bool toggleMute();

private:
    bool getQueryUrl(String &output, String command);
    bool getSingleParamCommandUrl(String &output, String command, String paramType, String paramName, String paramValue);

    int getVolumeFromHttp();
    bool checkSuccess();
    bool isInputSourceAux(bool &isAux);
    bool getMute(bool &muteStatus);
    bool setMute(bool muteStatus);
    bool isSpeakerAddressValid();
    void checkSpeakerIpAddress();

    String speakerIpAddress;
    unsigned long lastSpeakerIpAddressCheck;
    unsigned int speakerAddressCheckRetry;

    asyncHTTPrequest request;
};

