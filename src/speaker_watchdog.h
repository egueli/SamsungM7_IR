#pragma once

#include "speaker.h"

/**
 * Checks periodically that the speaker is still reachable via an IP address.
 * The method of checking if the speaker (e.g. doing an HTTP request) is reachable depends on the
 * specific Speaker implementation.
 * It won't do any checks, and calls notifyNoSpeaker(), if the IP address is not set via setAddress() yet. 
 * Otherwise it restarts the system (ESP.reset()) after too many failed attempts to connect.
 * No checks are made while there's no WiFi connection.
 */
class SpeakerWatchdog
{
public:
    SpeakerWatchdog(Speaker &speaker) : speaker(speaker) {};

    void loop();

    void setAddress(const String &address);

private:
    void checkIpAddress();

    Speaker &speaker;
    String ipAddress;
    unsigned long lastSpeakerIpAddressCheck;
    unsigned int speakerAddressCheckRetry;
};