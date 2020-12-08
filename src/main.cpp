#include <Arduino.h>

#include "ir.h"
#include "serial.h"
#include "wifi.h"
#include "discovery.h"
#include "speaker.h"
#include "speaker_samsung_multiroom.h"
#include "speaker_watchdog.h"
#include "http_xml.h"
#include "display.h"
#include "volume.h"

SamsungMultiroomSpeaker multiroom;
Speaker &speaker = multiroom;
SpeakerWatchdog watchdog(speaker);

void setup() {

  USE_SERIAL.begin(115200);
  // USE_SERIAL.setDebugOutput(true);

  USE_SERIAL.println();
  USE_SERIAL.println();
  USE_SERIAL.println();

  setupDisplay();
  setupWifi();
  setupDiscovery();
  setupIR();
  speaker.setup();
}

void loop() {
  loopIR();
  loopWifi();
  loopDisplay();
  loopDiscovery();
  speaker.loop();
  watchdog.loop();
}

void onDiscoveryFinished(String address) {
  notifySpeakerAddress(address);
  speaker.setAddress(address);
  watchdog.setAddress(address);
}

void onHttpWait() {
  peekIR();
}

void onVolumeUp() {
  increaseVolume(speaker);
}

void onVolumeDown() {
  decreaseVolume(speaker);
}

void onTvRad() {
  speaker.setTvInput();
}

void onMute() {
  speaker.toggleMute();
}