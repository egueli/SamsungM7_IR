#include "speaker_watchdog.h"
#include "wifi.h"
#include "notify.h"

const unsigned long kSpeakerAddressIpCheckInterval = 30000;
const unsigned int kSpeakerAddressIpCheckRetries = 3;

void SpeakerWatchdog::loop()
{
    checkIpAddress();

    if (isWifiConnected() && ipAddress.isEmpty())
    {
        notifyNoSpeaker();
    }
}

void SpeakerWatchdog::setAddress(const String &address)
{
    ipAddress = address;
    lastSpeakerIpAddressCheck = millis();
}

void SpeakerWatchdog::checkIpAddress()
{
    if (!isWifiConnected())
    {
        speakerAddressCheckRetry = 0;
    }

    if (millis() < lastSpeakerIpAddressCheck + kSpeakerAddressIpCheckInterval)
    {
        return;
    }
    lastSpeakerIpAddressCheck = millis();

    Serial.print("speaker ip valid? ");
    bool valid = speaker.isAddressValid();
    Serial.println(valid ? "yes" : "no");
    if (valid)
    {
        speakerAddressCheckRetry = 0;
    }
    else
    {
        speakerAddressCheckRetry++;
        if (speakerAddressCheckRetry >= kSpeakerAddressIpCheckRetries)
        {
            Serial.println("too many speaker check retries, restarting");
            ESP.restart();
        }
    }
}