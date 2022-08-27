#include "speaker_musiccast.h"
#include "http_response.h"
#include "notify.h"

Speaker::Configuration MusicCastSpeaker::getConfiguration()
{
    return {
        "_spotify-connect._tcp.local",
        1,
        161,
        2,
        -2
    };
}

void MusicCastSpeaker::setAddress(const String &address)
{
    ipAddress = address;
}

bool MusicCastSpeaker::isAddressValid()
{
    String url;
    if (getZoneUrl(url, "getStatus") != Result::OK)
    {
        return false;
    }

    httpClient.begin(wifiClient, url);
    int httpCode = httpClient.GET();

    return handleHttpOkResponse(httpCode) == Result::OK;
}

Result MusicCastSpeaker::getVolume(int &outVolume)
{
    DynamicJsonDocument doc(384);
    RETURN_IF_ERROR(getStatus(doc))

    outVolume = doc["volume"];
    return Result::OK;
}

Result MusicCastSpeaker::setVolume(int newVolume)
{
    String url;
    RETURN_IF_ERROR(getZoneUrl(url, "setVolume?volume=" + String(newVolume)))

    httpClient.begin(wifiClient, url);
    return checkSuccess(httpClient.GET());
}

/**
 * In addition to setting the input to the optical input (where TV audio comes from),
 * this also configures the A/B speakers so to use the speakers near to the TV.
 * It also turns the power on from standby.
 */
Result MusicCastSpeaker::setTvInput()
{
    Serial.print("Powering on (if on standby)... ");
    notifyTv(false);
    RETURN_IF_ERROR(powerOn())

    String url;
    RETURN_IF_ERROR(getZoneUrl(url, "setInput?input=optical1"))

    Serial.print("OK. Setting input to Optical1... ");
    httpClient.begin(wifiClient, url);
    int httpCode = httpClient.GET();
    RETURN_IF_ERROR(checkSuccess(httpCode));

    Serial.println("OK. Setting speakers A/B... ");
    Result result = setABSpeakers();
    if (result == Result::OK)
    {
        notifyTv(true);
    }
    return result;
}

Result MusicCastSpeaker::powerOn()
{
    String url;
    RETURN_IF_ERROR(getZoneUrl(url, "setPower?power=on"))
    httpClient.begin(wifiClient, url);
    return checkSuccess(httpClient.GET());
}

Result MusicCastSpeaker::setABSpeakers() 
{
    RETURN_IF_ERROR(setABSpeaker('A', false))
    RETURN_IF_ERROR(setABSpeaker('B', true))
    return Result::OK;
}

Result MusicCastSpeaker::setABSpeaker(const char &letter, const bool enable) {
    String command = "setSpeaker";
    command += letter;
    command += "?enable=";
    command += enable ? "true" : "false";
    String url;
    RETURN_IF_ERROR(getSystemUrl(url, command))

    Serial.println(url);
    httpClient.begin(wifiClient, url);
    return checkSuccess(httpClient.GET());
}

Result MusicCastSpeaker::getMuteStatus(bool &outStatus)
{
    DynamicJsonDocument doc(384);
    RETURN_IF_ERROR(getStatus(doc))

    outStatus = doc["mute"];
    return Result::OK;
}

Result MusicCastSpeaker::setMuteStatus(const bool newMuteStatus)
{
    String url;
    RETURN_IF_ERROR(getZoneUrl(url, newMuteStatus ? "setMute?enable=true" : "setMute?enable=false"))

    httpClient.begin(wifiClient, url);
    return checkSuccess(httpClient.GET());
}

/**
 * Puts the base URL to send zone-based requests to the speaker
 * \return true if the IP address is known and a URL can be constructed, false otherwise.
 */
Result MusicCastSpeaker::getZoneUrl(String &output, const String &endPart)
{
    if (ipAddress.isEmpty())
    {
        return Result::ERROR_NO_SPEAKER_ADDRESS;
    }

    output = "http://" + ipAddress + "/YamahaExtendedControl/v1/main/" + endPart;
    return Result::OK;
}

/**
 * Puts the base URL to send system-based requests to the speaker
 * \return true if the IP address is known and a URL can be constructed, false otherwise.
 */
Result MusicCastSpeaker::getSystemUrl(String &output, const String &endPart)
{
    if (ipAddress.isEmpty())
    {
        return Result::ERROR_NO_SPEAKER_ADDRESS;
    }

    output = "http://" + ipAddress + "/YamahaExtendedControl/v1/system/" + endPart;
    return Result::OK;
}

Result MusicCastSpeaker::checkSuccess(const int httpCode)
{
    RETURN_IF_ERROR(handleHttpOkResponse(httpCode))

    String body = httpClient.getString();
    DynamicJsonDocument doc(384);
    DeserializationError jsonError = deserializeJson(doc, body);
    if (jsonError)
    {
        Serial.printf("Response JSON deserialization error: %s\n", jsonError.c_str());
        return Result::ERROR_PARSE_FAILED;
    }

    int responseCode = doc["response_code"];
    if (responseCode != 0)
    {
        Serial.printf("non-zero YXC response code %d\n", responseCode);
        return Result::ERROR_PARSE_FAILED;
    }

    return Result::OK;
}

Result MusicCastSpeaker::getStatus(DynamicJsonDocument &outputDoc)
{
    String url;
    RETURN_IF_ERROR(getZoneUrl(url, "getStatus"))

    httpClient.begin(wifiClient, url);
    int httpCode = httpClient.GET();
    RETURN_IF_ERROR(handleHttpOkResponse(httpCode));

    String body = httpClient.getString();
    DeserializationError jsonError = deserializeJson(outputDoc, body);
    if (jsonError)
    {
        Serial.printf("JSON deserialization error: %s\n", jsonError.c_str());
        return Result::ERROR_PARSE_FAILED;
    }

    return Result::OK;
}