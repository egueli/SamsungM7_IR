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
#include "volume.h"
#include "notify.h"

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
}

void loop() {
  loopIR();
  loopWifi();
  loopDisplay();
  loopDiscovery();
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
  Result result = increaseVolume(speaker);
  if (result != Result::OK) {
    notifyFail(result);
  }
}

void onVolumeDown() {
  Result result = decreaseVolume(speaker);
  if (result != Result::OK) {
    notifyFail(result);
  }
}

void onTvRad() {
  Result result = speaker.setTvInput();
  if (result != Result::OK) {
    notifyFail(result);
  }
}

void onMute() {
  Result result = toggleMute(speaker);
  if (result != Result::OK) {
    notifyFail(result);
  }
}