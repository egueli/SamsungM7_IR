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
    DynamicJsonDocument doc(384);
    if (!getStatus(doc)) {
        return kGetVolumeError;
    }

    return doc["volume"];
}


bool MusicCastSpeaker::setVolume(int newVolume)
{
    String url;
    if (!getBaseUrl(url, "setVolume?volume=" + String(newVolume)))
    {
        return false;
    }

    request.open("GET", url.c_str());
    request.send();
    return checkSuccess();
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

bool MusicCastSpeaker::checkSuccess()
{
    if (!waitForHttpOkResponse(request))
    {
        return false;
    }

    String body = request.responseText();
    DynamicJsonDocument doc(384);
    DeserializationError jsonError = deserializeJson(doc, body);
    if (jsonError)
    {
        Serial.printf("Response JSON deserialization error: %s\n", jsonError.c_str());
        return false;
    }

    int responseCode = doc["response_code"];
    if (responseCode != 0)
    {
        Serial.printf("non-zero YXC response code %d\n", responseCode);
        return false;
    }

    return true;
}

bool MusicCastSpeaker::getStatus(DynamicJsonDocument &outputDoc)
{
    String url;
    if (!getBaseUrl(url, "getStatus"))
    {
        return false;
    }

    request.open("GET", url.c_str());
    request.send();

    if (!waitForHttpOkResponse(request))
    {
        return false;
    }

    String body = request.responseText();
    DeserializationError jsonError = deserializeJson(outputDoc, body);
    if (jsonError)
    {
        Serial.printf("JSON deserialization error: %s\n", jsonError.c_str());
        return false;
    }

    return true;
}