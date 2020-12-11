#include <Arduino.h>
#include "speaker.h"
#include "display.h"

const char kDisplayErrorCodeVolume = 'v';
const char kDisplayErrorCodeAux = 't';
const char kDisplayErrorCodeMute = 'n';

void animate(char ch) {
  String text = "    ";
  int dashPos = (millis() / 600) % 4;
  text[dashPos] = ch;
  displayText(text);
}

void notifyNoWifi() {
  animate('-');
}

void notifyNoSpeaker() {
  animate('o');
}

void notifySpeakerAddress(String address) {
  int lastDotPosition = address.lastIndexOf(".");
  if (lastDotPosition == -1) {
    displayText("S....");
    return;
  }

  String lastOctet = address.substring(lastDotPosition + 1);
  displayText("S." + lastOctet);
}

void notifyIR() {
  displayText(" .");
}

void notifyVolume(int volume, bool wasSet) {
  String text = " ";
  if (volume < 10) {
    text += " ";
  }
  text += volume;
  if (wasSet) {
    text += ".";
  }
  displayText(text);
}

void notifyFail(char code, bool duringSet) {
  String text = "  E";
  text += code;
  if (duringSet) {
    text += ".";
  }
  displayText(text);
}

void notifyVolumeGetSuccess(int volume) {
  notifyVolume(volume, false);
}
void notifyVolumeGetFail() {
  notifyFail(kDisplayErrorCodeVolume, false);
}
void notifyVolumeSetSuccess(int volume) {
  notifyVolume(volume, true);
}
void notifyVolumeSetFail() {
  notifyFail(kDisplayErrorCodeVolume, true);
}


void notifyTv(bool wasSet) {
  displayText(wasSet ? " tv." : " tv");
}

void notifyTvFail() {
  notifyAuxSetFail();
}

void notifyAuxGetSuccess(bool isAux) {
  notifyTv(false);
}
void notifyAuxGetFail() {
  notifyFail(kDisplayErrorCodeAux, false);
}
void notifyAuxSetSuccess(bool isAux) {
  notifyTv(true);
}
void notifyAuxSetFail() {
  notifyFail(kDisplayErrorCodeAux, true);
}

void notifyMuteGetSuccess() {
  displayText("mu?");
}
void notifyMuteGetFail() {
  notifyFail(kDisplayErrorCodeMute, false);
}
void notifyMuteSetSuccess(bool isMute) {
  displayText(isMute ? "mute." : "soun.");
}
void notifyMuteSetFail() {
  notifyFail(kDisplayErrorCodeMute, true);
}
