#pragma once

#include "speaker.h"
#include <asyncHTTPrequest.h>

/**
 * Controls a Samsung Multiroom compatible speaker like Samsung M7 (WAM750).
 */
class SamsungMultiroomSpeaker : public Speaker {
public:
    Configuration getConfiguration();

    void setAddress(const String &address);
    bool isAddressValid();

    Result getVolume(int &outVolume);
    Result setVolume(int newVolume);

    Result setTvInput();

    Result getMuteStatus(bool &muteStatus);
    Result setMuteStatus(const bool muteStatus);

private:
    Result getQueryUrl(String &output, String command);
    Result getSingleParamCommandUrl(String &output, String command, String paramType, String paramName, String paramValue);

    Result getVolumeFromHttp(int &outVolume);
    Result checkSuccess();
    Result isInputSourceAux(bool &isAux);
    void checkSpeakerIpAddress();

    String speakerIpAddress;

    asyncHTTPrequest request;
};

