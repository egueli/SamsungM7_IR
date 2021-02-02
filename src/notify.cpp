#include <Arduino.h>
#include "speaker.h"
#include "display.h"
#include "config.h"

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
  char text[6]; // 4 digits + 1 decimal point + 1 set confirmation dot
  const char *setDot = wasSet ? "." : "";
#ifdef SPEAKER_MULTIROOM
  snprintf(text, 6, " %2d%s", volume, setDot);
#else
  /*
   * MusicCast speakers may show the volume in dB, with 0dB beng the max and
   * every step is a 0.5dB increment. Not sure if all speakers follow the
   * same convention; the Yamaha R-N602 does.
   */
  float decibel = ((float)volume - kMaxVolume) / 2;
  snprintf(text, 6, "% 4.1f%s", decibel, setDot);
#endif
  Serial.printf("notifyVolume: '%s'", text);
  displayText(text);
}

void notifyFail(char code) {
  String text = "  E";
  text += code;
  displayText(text);
}

void notifyVolumeGetSuccess(int volume) {
  notifyVolume(volume, false);
}
void notifyVolumeSetSuccess(int volume) {
  notifyVolume(volume, true);
}
void notifyVolumeFail() {
  notifyFail(kDisplayErrorCodeVolume);
}


void notifyTv(bool wasSet) {
  displayText(wasSet ? " tv." : " tv");
}

void notifyAuxGetSuccess(bool isAux) {
  notifyTv(false);
}
void notifyAuxSetSuccess(bool isAux) {
  notifyTv(true);
}
void notifyAuxFail() {
  notifyFail(kDisplayErrorCodeAux);
}
void notifyTvFail() {
  notifyAuxFail();
}

void notifyMuteGetSuccess() {
  displayText("mu?");
}
void notifyMuteSetSuccess(bool isMute) {
  displayText(isMute ? "mute." : "soun.");
}
void notifyMuteFail() {
  notifyFail(kDisplayErrorCodeMute);
}
