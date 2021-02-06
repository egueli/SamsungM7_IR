#include <Arduino.h>
#include <asyncHTTPrequest.h>

#include "speaker_samsung_multiroom.h"
#include "http_response.h"
#include "http_wait.h"
#include "serial.h"
#include "wifi.h"
#include "notify.h"

const String kVolumeOpenTag = "<volume>";
const String kVolumeCloseTag = "</volume>";

const int kMinVolume = 0;
const int kMaxVolume = 30;

const String kInputSourceOpenTag = "<function>";
const String kInputSourceCloseTag = "</function>";
const String kInputSourceAux = "aux";

const String kMuteOpenTag = "<mute>";
const String kMuteCloseTag = "</mute>";
const String kMuteOn = "on";

Speaker::Configuration SamsungMultiroomSpeaker::getConfiguration() {
  return {
    "_spotify-connect._tcp.local",
    0,
    30,
    1,
    -3
  };
}

void SamsungMultiroomSpeaker::setAddress(const String &address) {
  speakerIpAddress = address;
}

Result SamsungMultiroomSpeaker::getQueryUrl(String &output, String command) {
  if (speakerIpAddress.isEmpty()) {
    return Result::ERROR_HTTP_TIMEOUT;
  }

  output = "http://" + speakerIpAddress + ":55001/UIC?cmd=%3Cname%3E" + command + "%3C/name%3E";
  
  return Result::OK;
}

Result SamsungMultiroomSpeaker::getSingleParamCommandUrl(String &output, String command, String paramType, String paramName, String paramValue) {
  if (speakerIpAddress.isEmpty()) {
    return Result::ERROR_NO_SPEAKER_ADDRESS;
  }

  output = "http://" + speakerIpAddress + ":55001/UIC?cmd="
    "%3Cpwron%3Eon%3C/pwron%3E"
    "%3Cname%3E" + command + "%3C/name%3E"
    "%3Cp"
    "%20type%3D%22" + paramType + "%22"
    "%20name%3D%22" + paramName + "%22"
    "%20val%3D%22" + paramValue + "%22"
    "/%3E";
  return Result::OK;
}

Result SamsungMultiroomSpeaker::getVolumeFromHttp(int &outVolume) {
  String valueString;
  RETURN_IF_ERROR(getValueFromHttp(request, valueString, kVolumeOpenTag, kVolumeCloseTag))
  outVolume = valueString.toInt();
  return Result::OK;
}

Result SamsungMultiroomSpeaker::getVolume(int &outVolume) {
  String url;
  RETURN_IF_ERROR(getQueryUrl(url, "GetVolume"))

  request.open("GET", url.c_str());
  request.send();
  return getVolumeFromHttp(outVolume);
}

Result SamsungMultiroomSpeaker::checkSuccess() {
  while (request.readyState() != 4) {
    yield();
    onHttpWait();
  }

  int httpCode = request.responseHTTPcode();
  if (httpCode != 200 /* OK */) {
    USE_SERIAL.printf("[HTTP] GET not OK, code: %d\n", httpCode);
    return Result::ERROR_HTTP_NON_OK_RESPONSE;
  }

  return Result::OK;
}

Result SamsungMultiroomSpeaker::setVolume(int newVolume) {
  String url;
  RETURN_IF_ERROR(getSingleParamCommandUrl(url, "SetVolume", "dec", "volume", String(newVolume)))

  request.open("GET", url.c_str());
  request.send();
  return checkSuccess();
}

// return value is success/failure; actual output is in param reference
Result SamsungMultiroomSpeaker::isInputSourceAux(bool &isAux) {
  String url;
  RETURN_IF_ERROR(getQueryUrl(url, "GetFunc"))
  request.open("GET", url.c_str());
  request.send();
  String inputSource;
  RETURN_IF_ERROR(getValueFromHttp(request, inputSource, kInputSourceOpenTag, kInputSourceCloseTag))
  USE_SERIAL.print("[");
  USE_SERIAL.print(inputSource);
  USE_SERIAL.print("]");
  isAux = inputSource == kInputSourceAux;
  return Result::OK;
}

Result SamsungMultiroomSpeaker::setTvInput() {
  bool isAux;
  RETURN_IF_ERROR(isInputSourceAux(isAux))
  notifyAuxGetSuccess(isAux);

  if (isAux) {
    USE_SERIAL.println("already AUX; not setting again");
    notifyAuxSetSuccess(isAux);
    return Result::OK;
  }

  USE_SERIAL.print("set AUX... ");
  String url;
  RETURN_IF_ERROR(getSingleParamCommandUrl(url, "SetFunc", "str", "function", "aux"))

  request.open("GET", url.c_str());
  request.send();
  RETURN_IF_ERROR(checkSuccess())

  notifyAuxSetSuccess(isAux);
  return Result::OK;
}

Result SamsungMultiroomSpeaker::getMuteStatus(bool &muteStatus) {
  String url;
  RETURN_IF_ERROR(getQueryUrl(url, "GetMute"))
  request.open("GET", url.c_str());
  request.send();
  String muteString;
  RETURN_IF_ERROR(getValueFromHttp(request, muteString, kMuteOpenTag, kMuteCloseTag))
  USE_SERIAL.print("[");
  USE_SERIAL.print(muteString);
  USE_SERIAL.print("]");
  muteStatus = muteString == kMuteOn;
  return Result::OK;
}


Result SamsungMultiroomSpeaker::setMuteStatus(const bool muteStatus) {
  String url;
  RETURN_IF_ERROR(getSingleParamCommandUrl(url, "SetMute", "str", "mute", muteStatus ? "on" : "off"))
  request.open("GET", url.c_str());
  request.send();
  return checkSuccess();
}

bool SamsungMultiroomSpeaker::isAddressValid() {
  String url;
  if (getQueryUrl(url, "GetVolume") != Result::OK) {
    return false;
  }

  request.open("GET", url.c_str());
  request.send();

  String valueString;
  return getValueFromHttp(request, valueString, kVolumeOpenTag, kVolumeCloseTag) == Result::OK;
}
