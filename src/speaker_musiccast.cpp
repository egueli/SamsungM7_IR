#include "speaker_musiccast.h"
#include "http_response.h"

void MusicCastSpeaker::setAddress(const String &address)
{
    ipAddress = address;
}

bool MusicCastSpeaker::isAddressValid()
{
    String url;
    if (!getZoneUrl(url, "getStatus"))
    {
        return false;
    }

    request.open("GET", url.c_str());
    request.send();

    return waitForHttpOkResponse(request);
}

bool MusicCastSpeaker::getVolume(int &outVolume)
{
    DynamicJsonDocument doc(384);
    if (!getStatus(doc)) {
        return false;
    }

    outVolume = doc["volume"];
    return true;
}


bool MusicCastSpeaker::setVolume(int newVolume)
{
    String url;
    if (!getZoneUrl(url, "setVolume?volume=" + String(newVolume)))
    {
        return false;
    }

    request.open("GET", url.c_str());
    request.send();
    return checkSuccess();
}

/**
 * In addition to setting the input to the optical input (where TV audio comes from),
 * this also configures the A/B speakers so to use the speakers near to the TV.
 * It also turns the power on from standby.
 */
bool MusicCastSpeaker::setTvInput()
{
    Serial.print("Powering on (if on standby)... ");
    notifyTv(false);
    bool success = powerOn();
    if (!success) {
        return false;        
    }

    String url;
    if (!getZoneUrl(url, "setInput?input=optical1"))
    {
        return false;
    }

    Serial.print("OK. Setting input to Optical1... ");
    request.open("GET", url.c_str());
    request.send();
    if (!checkSuccess()) 
    {
        return false;
    }

    Serial.println("OK. Setting speakers A/B... ");
    success = setABSpeakers();
    if (success)
    {
        notifyTv(true);
    }
    return success;
}

bool MusicCastSpeaker::powerOn()
{
    String url;
    if (!getZoneUrl(url, "setPower?power=on"))
    {
        return false;
    }
    request.open("GET", url.c_str());
    request.send();

    return checkSuccess();
}

bool MusicCastSpeaker::setABSpeakers() 
{
    return setABSpeaker('A', false) && setABSpeaker('B', true);
}

bool MusicCastSpeaker::setABSpeaker(const char &letter, const bool enable) {
    String command = "setSpeaker";
    command += letter;
    command += "?enable=";
    command += enable ? "true" : "false";
    String url;
    if (!getSystemUrl(url, command))
    {
        return false;
    }

    Serial.println(url);
    request.open("GET", url.c_str());
    request.send();
    return checkSuccess();
}

bool MusicCastSpeaker::getMuteStatus(bool &outStatus)
{
    DynamicJsonDocument doc(384);
    if (!getStatus(doc)) {
        return false;
    }

    outStatus = doc["mute"];
    return true;
}

bool MusicCastSpeaker::setMuteStatus(const bool newMuteStatus)
{
    String url;
    if (!getZoneUrl(url, newMuteStatus ? "setMute?enable=true" : "setMute?enable=false"))
    {
        return false;
    }

    request.open("GET", url.c_str());
    request.send();
    return checkSuccess();
}

/**
 * Puts the base URL to send zone-based requests to the speaker
 * \return true if the IP address is known and a URL can be constructed, false otherwise.
 */
bool MusicCastSpeaker::getZoneUrl(String &output, const String &endPart)
{
    if (ipAddress.isEmpty())
    {
        return false;
    }

    output = "http://" + ipAddress + "/YamahaExtendedControl/v1/main/" + endPart;
    return true;
}

/**
 * Puts the base URL to send system-based requests to the speaker
 * \return true if the IP address is known and a URL can be constructed, false otherwise.
 */
bool MusicCastSpeaker::getSystemUrl(String &output, const String &endPart)
{
    if (ipAddress.isEmpty())
    {
        return false;
    }

    output = "http://" + ipAddress + "/YamahaExtendedControl/v1/system/" + endPart;
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
    if (!getZoneUrl(url, "getStatus"))
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