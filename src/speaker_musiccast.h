#pragma once

#include <asyncHTTPrequest.h>
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

    bool toggleMute();

private:
    String ipAddress;
    asyncHTTPrequest request;
};

