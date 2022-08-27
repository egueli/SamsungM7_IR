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

const int kNumSamples = 30;
int count = 0;
unsigned long totalTime;
bool enableFakeIR;

void loop() {
  if (enableFakeIR) {
    auto start_at = millis();
    if (count % 2 == 0) {
      onVolumeUp();
    } else {
      onVolumeDown();
    }
    auto end_at = millis();
    count++;

    auto time = end_at - start_at;
    USE_SERIAL.printf("\n** req time: %3lums **\n", time);
    totalTime += time;

    if (count % kNumSamples == 0) {
      unsigned long average = totalTime / kNumSamples;
      USE_SERIAL.printf("\n**** AVG time: %3lums ****\n", average);
      displayText(String(average));
      totalTime = 0;
      count = 0;
    }
  }

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
  enableFakeIR = true;
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

void onToggleClock() {
  toggleClock();
}