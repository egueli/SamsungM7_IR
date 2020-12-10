#include "speaker_musiccast.h"
#include "http_response.h"

void MusicCastSpeaker::setAddress(const String &address)
{
    ipAddress = address;
}

bool MusicCastSpeaker::isAddressValid()
{
    String url;
    if (!getBaseUrl(url, "getStatus"))
    {
        return false;
    }

    request.open("GET", url.c_str());
    request.send();

    return waitForHttpOkResponse(request);
}

int MusicCastSpeaker::getVolume()
{
    return 0;
}

bool MusicCastSpeaker::setVolume(int newVolume)
{
    return true;
}

bool MusicCastSpeaker::setTvInput()
{
    return true;
}

bool MusicCastSpeaker::toggleMute()
{
    return true;
}

/**
 * Puts the base URL to send requests to the speaker
 * \return true if the IP address is known and a URL can be constructed, false otherwise.
 */
bool MusicCastSpeaker::getBaseUrl(String &output, const String &endPart)
{
    if (ipAddress.isEmpty())
    {
        return false;
    }

    output = "http://" + ipAddress + "/YamahaExtendedControl/v1/main/" + endPart;
    return true;
}