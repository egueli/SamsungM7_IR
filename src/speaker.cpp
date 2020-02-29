#include <Arduino.h>
#include <asyncHTTPrequest.h>

#include "speaker.h"
#include "http_xml.h"
#include "serial.h"

#define IP_ADDRESS "192.168.0.102"

asyncHTTPrequest request;

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
}

String getSingleParamCommandUrl(String command, String paramType, String paramName, String paramValue) {
  return String("http://" IP_ADDRESS ":55001/UIC?cmd="
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
  bool success = getValueFromHttp(request, valueString, kVolumeOpenTag, kVolumeCloseTag);
  if (!success) {
    return kGetVolumeError;
  }
  return valueString.toInt();
}

int getVolume() {
  request.open("GET", "http://" IP_ADDRESS ":55001/UIC?cmd=%3Cname%3EGetVolume%3C/name%3E");
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
  String url = getSingleParamCommandUrl("SetVolume", "dec", "volume", String(newVolume));

  request.open("GET", url.c_str());
  request.send();
  return checkSuccess();
}

bool setVolumeDelta(int delta) {
  USE_SERIAL.print("... ");
  int volume = getVolume();
  if (volume == kGetVolumeError) {
    notifyVolumeGetFail();
    return false;
  }
  notifyVolumeGetSuccess(volume);
  int newVolume = volume + delta;
  // prevent going out of bounds
  newVolume = constrain(newVolume, 0, 30);
  USE_SERIAL.print(volume);
  USE_SERIAL.print(" => ");
  USE_SERIAL.print(newVolume);
  USE_SERIAL.print(" ");
  bool success;
  if (newVolume == volume) {
    // skip an HTTP request just to set the same volume
    USE_SERIAL.print("(skip) ");
    success = true;
  } else {
    USE_SERIAL.print(" ... ");
    success = setVolume(newVolume);
  }
  if (success) {
    notifyVolumeSetSuccess(newVolume);
  } else {
    notifyVolumeSetFail();
  }
  return success;
}

void increaseVolume() {
  USE_SERIAL.print("VOL+ ");
  bool success = setVolumeDelta(1);
  USE_SERIAL.println(success ? "OK" : "fail :(");
}

void decreaseVolume() {
  USE_SERIAL.print("VOL- ");
  bool success = setVolumeDelta(-3);
  USE_SERIAL.println(success ? "OK" : "fail :(");
}

// return value is success/failure; actual output is in param reference
bool isInputSourceAux(bool &isAux) {
  request.open("GET", "http://" IP_ADDRESS ":55001/UIC?cmd=%3Cname%3EGetFunc%3C/name%3E");
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
  String url = getSingleParamCommandUrl("SetFunc", "str", "function", "aux");

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
  String muteString;
  request.open("GET", "http://" IP_ADDRESS ":55001/UIC?cmd=%3Cname%3EGetMute%3C/name%3E");
  request.send();
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
  String url = getSingleParamCommandUrl("SetMute", "str", "mute", muteStatus ? "on" : "off");
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
