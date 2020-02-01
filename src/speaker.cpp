#include <Arduino.h>
#include <ESP8266HTTPClient.h>

#include "speaker.h"
#include "http_xml.h"
#include "serial.h"

HTTPClient http;

const int kGetVolumeError = -1;
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

void setupSpeaker() {
  http.setReuse(true);
}

String getSingleParamCommandUrl(String command, String paramType, String paramName, String paramValue) {
  return String("http://192.168.0.101:55001/UIC?cmd="
    "%3Cpwron%3Eon%3C/pwron%3E"
    "%3Cname%3E" + command + "%3C/name%3E"
    "%3Cp"
    "%20type%3D%22" + paramType + "%22"
    "%20name%3D%22" + paramName + "%22"
    "%20val%3D%22" + paramValue + "%22"
    "/%3E");
}

int getVolumeFromHttp() {
  String valueString;
  bool success = getValueFromHttp(http, valueString, kVolumeOpenTag, kVolumeCloseTag);
  if (!success) {
    return kGetVolumeError;
  }
  return valueString.toInt();
}

int getVolume() {
  http.begin("http://192.168.0.101:55001/UIC?cmd=%3Cname%3EGetVolume%3C/name%3E");
  int volume = getVolumeFromHttp();
  http.end();
  return volume;
}

bool checkSuccess() {
  int httpCode = http.GET();
  if (httpCode <= 0) {
    USE_SERIAL.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    return false;
  }

  if (httpCode != HTTP_CODE_OK) {
    USE_SERIAL.printf("[HTTP] GET not OK, code: %d\n", httpCode);
    return false;
  }

  return true;
}

bool setVolume(int newVolume) {
  String url = getSingleParamCommandUrl("SetVolume", 
    "dec", "volume", String(newVolume));

  http.begin(url);
  bool success = checkSuccess();
  http.end();
  return success;
}

bool setVolumeDelta(int delta) {
  int volume = getVolume();
  if (volume == kGetVolumeError) {
    notifyVolumeGetFail();
    return false;
  }
  notifyVolumeGetSuccess(volume);
  int newVolume = volume + delta;
  USE_SERIAL.print(volume);
  USE_SERIAL.print(" => ");
  USE_SERIAL.print(newVolume);
  USE_SERIAL.print(" ");
  bool success;
  if (newVolume != volume) {
    success = setVolume(newVolume);
  } else {
    // skip an HTTP round-trip just to set the same volume
    success = true;
  }
  if (success) {
    notifyVolumeSetSuccess(newVolume);
  } else {
    notifyVolumeSetFail();
  }
  return success;
}

void increaseVolume() {
  USE_SERIAL.print("VOL+... ");
  bool success = setVolumeDelta(1);
  USE_SERIAL.println(success ? "OK" : "fail :(");
}

void decreaseVolume() {
  USE_SERIAL.print("VOL-... ");
  bool success = setVolumeDelta(-1);
  USE_SERIAL.println(success ? "OK" : "fail :(");
}

// return value is success/failure; actual output is in param reference
bool isInputSourceAux(bool &isAux) {
  String inputSource;
  http.begin("http://192.168.0.101:55001/UIC?cmd=%3Cname%3EGetFunc%3C/name%3E");
  bool success = getValueFromHttp(http, inputSource, kInputSourceOpenTag, kInputSourceCloseTag);
  http.end();
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
  String url = getSingleParamCommandUrl("SetFunc", "str", "function", "aux");

  http.begin(url);
  bool success = checkSuccess();
  http.end();

  USE_SERIAL.println(success ? "OK" : "fail :(");
  if (success) {
    notifyAuxSetSuccess(isAux);
  } else {
    notifyAuxSetFail();
  }
  return success;
}

bool getMute(bool &muteStatus) {
  String muteString;
  http.begin("http://192.168.0.101:55001/UIC?cmd=%3Cname%3EGetMute%3C/name%3E");
  bool success = getValueFromHttp(http, muteString, kMuteOpenTag, kMuteCloseTag);
  http.end();
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
  String url = getSingleParamCommandUrl("SetMute", 
    "str", "mute", muteStatus ? "on" : "off");
  http.begin(url);
  bool success = checkSuccess();
  http.end();
  return success;
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
