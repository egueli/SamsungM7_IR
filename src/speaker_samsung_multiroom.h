#pragma once

#include "speaker.h"
#include <asyncHTTPrequest.h>

/**
 * Controls a Samsung Multiroom compatible speaker like Samsung M7 (WAM750).
 */
class SamsungMultiroomSpeaker : public Speaker {
public:
    void setAddress(const String &address);
    bool isAddressValid();

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
    void checkSpeakerIpAddress();

    String speakerIpAddress;

    asyncHTTPrequest request;
};

