#include <Arduino.h>
#include <asyncHTTPrequest.h>

#include "speaker.h"
#include "http_xml.h"
#include "serial.h"
#include "wifi.h"

extern const int kGetVolumeError = -1;

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

const unsigned long kSpeakerAddressIpCheckInterval = 30000;
const unsigned int kSpeakerAddressIpCheckRetries = 3;

String speakerIpAddress;
unsigned long lastSpeakerIpAddressCheck;
unsigned int speakerAddressCheckRetry;

asyncHTTPrequest request;

void setupSpeaker() {
}

void loopSpeaker() {
  if (!isWifiConnected()) {
    return;
  }

  checkSpeakerIpAddress();

  if (speakerIpAddress.isEmpty()) {
    notifyNoSpeaker();
  }
}

void setSpeakerAddress(String address) {
  notifySpeakerAddress(address);
  speakerIpAddress = address;
  lastSpeakerIpAddressCheck = millis();
}

bool getQueryUrl(String &output, String command) {
  if (speakerIpAddress.isEmpty()) {
    return false;
  }

  output = "http://" + speakerIpAddress + ":55001/UIC?cmd=%3Cname%3E" + command + "%3C/name%3E";
  
  return true;
}

bool getSingleParamCommandUrl(String &output, String command, String paramType, String paramName, String paramValue) {
  if (speakerIpAddress.isEmpty()) {
    return false;
  }

  output = "http://" + speakerIpAddress + ":55001/UIC?cmd="
    "%3Cpwron%3Eon%3C/pwron%3E"
    "%3Cname%3E" + command + "%3C/name%3E"
    "%3Cp"
    "%20type%3D%22" + paramType + "%22"
    "%20name%3D%22" + paramName + "%22"
    "%20val%3D%22" + paramValue + "%22"
    "/%3E";
  return true;
}

int getVolumeFromHttp() {
  String valueString;
  bool success = getValueFromHttp(request, valueString, kVolumeOpenTag, kVolumeCloseTag);
  if (!success) {
    return kGetVolumeError;
  }
  return valueString.toInt();
}

int getVolume() {
  String url;
  if (!getQueryUrl(url, "GetVolume")) {
    return kGetVolumeError;
  }

  request.open("GET", url.c_str());
  request.send();
  int volume = getVolumeFromHttp();
  return volume;
}

bool checkSuccess() {
  while (request.readyState() != 4) {
    yield();
    onHttpWait();
  }

  int httpCode = request.responseHTTPcode();
  if (httpCode != 200 /* OK */) {
    USE_SERIAL.printf("[HTTP] GET not OK, code: %d\n", httpCode);
    return false;
  }

  return true;
}

bool setVolume(int newVolume) {
  String url;
  if (!getSingleParamCommandUrl(url, "SetVolume", "dec", "volume", String(newVolume))) {
    return false;
  }

  request.open("GET", url.c_str());
  request.send();
  return checkSuccess();
}

// return value is success/failure; actual output is in param reference
bool isInputSourceAux(bool &isAux) {
  String url;
  if (!getQueryUrl(url, "GetFunc")) {
    return false;
  }
  request.open("GET", url.c_str());
  request.send();
  String inputSource;
  bool success = getValueFromHttp(request, inputSource, kInputSourceOpenTag, kInputSourceCloseTag);
  if (!success) {
    return false;
  }
  USE_SERIAL.print("[");
  USE_SERIAL.print(inputSource);
  USE_SERIAL.print("]");
  isAux = inputSource == kInputSourceAux;
  return true;
}

bool setAux() {
  bool isAux;
  {
    bool success = isInputSourceAux(isAux);
    if (!success) {
      USE_SERIAL.println("unable to get input source");
      notifyAuxGetFail();
      return false;
    }
  }
  notifyAuxGetSuccess(isAux);

  if (isAux) {
    USE_SERIAL.println("already AUX; not setting again");
    notifyAuxSetSuccess(isAux);
    return true;
  }

  USE_SERIAL.print("set AUX... ");
  String url;
  if (!getSingleParamCommandUrl(url, "SetFunc", "str", "function", "aux")) {
    return false;
  }

  request.open("GET", url.c_str());
  request.send();
  bool success = checkSuccess();

  USE_SERIAL.println(success ? "OK" : "fail :(");
  if (success) {
    notifyAuxSetSuccess(isAux);
  } else {
    notifyAuxSetFail();
  }
  return success;
}

bool getMute(bool &muteStatus) {
  String url;
  if (!getQueryUrl(url, "GetMute")) {
    return false;
  }
  request.open("GET", url.c_str());
  request.send();
  String muteString;
  bool success = getValueFromHttp(request, muteString, kMuteOpenTag, kMuteCloseTag);
  if (!success) {
    return false;
  }
  USE_SERIAL.print("[");
  USE_SERIAL.print(muteString);
  USE_SERIAL.print("]");
  muteStatus = muteString == kMuteOn;
  return true;
}


bool setMute(bool muteStatus) {
  String url;
  if (!getSingleParamCommandUrl(url, "SetMute", "str", "mute", muteStatus ? "on" : "off")) {
    return false;
  }
  request.open("GET", url.c_str());
  request.send();
  return checkSuccess();
}

bool toggleMute() {
  bool isMuted;
  USE_SERIAL.print("Mute: ");
  bool getSuccess = getMute(isMuted);
  if (!getSuccess) {
    USE_SERIAL.println("unable to get mute state");
    notifyMuteGetFail();
    return false;
  }
  notifyMuteGetSuccess();
  USE_SERIAL.print(isMuted ? "on" : "off");
  USE_SERIAL.print(" => ");

  bool setSuccess = setMute(!isMuted);
  if (!setSuccess) {
    USE_SERIAL.println("fail :(");
    notifyMuteSetFail();
    return false;
  }

  USE_SERIAL.println(!isMuted ? "on" : "off");
  notifyMuteSetSuccess(!isMuted);
  return true;
}

bool isSpeakerAddressValid() {
  USE_SERIAL.print("speaker ip valid? ");
  String url;
  if (!getQueryUrl(url, "GetVolume")) {
    USE_SERIAL.println("no: unknown address");
    return false;
  }

  request.open("GET", url.c_str());
  request.send();

  String valueString;
  bool success = getValueFromHttp(request, valueString, kVolumeOpenTag, kVolumeCloseTag);
  USE_SERIAL.println(success ? "yes" : "no");
  if (!success) {
    return false;
  }

  return true;
}

void checkSpeakerIpAddress() {
  if (!isWifiConnected()) {
      speakerAddressCheckRetry = 0;
  }

  if (millis() < lastSpeakerIpAddressCheck + kSpeakerAddressIpCheckInterval) {
    return;
  }
  lastSpeakerIpAddressCheck = millis();

  bool valid = isSpeakerAddressValid();
  if (valid) {
    speakerAddressCheckRetry = 0;
  } else {
    speakerAddressCheckRetry++;
    if (speakerAddressCheckRetry >= kSpeakerAddressIpCheckRetries) {
      USE_SERIAL.println("too many speaker check retries, restarting");
      ESP.restart();
    }
  }
}