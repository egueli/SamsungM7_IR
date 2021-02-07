#include <Arduino.h>

#include "config.h"
#include "ir.h"
#include "serial.h"
#include "wifi.h"
#include "discovery.h"
#include "speaker.h"
#include "speaker_watchdog.h"
#include "http_response.h"
#include "display.h"
#include "notify.h"
#include "use_cases.h"
#include "clock.h"

#ifdef SPEAKER_MULTIROOM
#include "speaker_samsung_multiroom.h"
SamsungMultiroomSpeaker multiroom;
Speaker &speaker = multiroom;
#else
#include "speaker_musiccast.h"
MusicCastSpeaker musicCast;
Speaker &speaker = musicCast;
#endif

SpeakerWatchdog watchdog(speaker);

void setup() {

  USE_SERIAL.begin(115200);
  // USE_SERIAL.setDebugOutput(true);

  USE_SERIAL.println();
  USE_SERIAL.println();
  USE_SERIAL.println();

  setupDisplay();
  setupWifi();
  setupDiscovery(speaker);
  setupIR();
  setupClock();
}

void loop() {
  loopIR();
  loopWifi();
  loopDisplay();
  loopDiscovery();
  loopClock();
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
  setTvInput(speaker);
}

void onMute() {
  toggleMute(speaker);
}