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

    bool getVolume(int &outVolume);
    bool setVolume(int newVolume);

    bool setTvInput();

    bool getMuteStatus(bool &muteStatus);
    bool setMuteStatus(const bool muteStatus);

private:
    bool getQueryUrl(String &output, String command);
    bool getSingleParamCommandUrl(String &output, String command, String paramType, String paramName, String paramValue);

    int getVolumeFromHttp();
    bool checkSuccess();
    bool isInputSourceAux(bool &isAux);
    void checkSpeakerIpAddress();

    String speakerIpAddress;

    asyncHTTPrequest request;
};

