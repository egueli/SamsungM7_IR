#include <Arduino.h>

#include "ir.h"
#include "serial.h"
#include "wifi.h"
#include "discovery.h"
#include "speaker.h"
#include "speaker_samsung_multiroom.h"
#include "http_xml.h"
#include "display.h"
#include "volume.h"

SamsungMultiroomSpeaker multiroom = SamsungMultiroomSpeaker{};
Speaker &speaker = multiroom;

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
}

void onDiscoveryFinished(String address) {
  speaker.setAddress(address);
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