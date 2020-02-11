#include <Arduino.h>

#include "ir.h"
#include "serial.h"
#include "wifi.h"
#include "speaker.h"
#include "http_xml.h"
#include "display.h"

void setup() {

  USE_SERIAL.begin(115200);
  // USE_SERIAL.setDebugOutput(true);

  USE_SERIAL.println();
  USE_SERIAL.println();
  USE_SERIAL.println();

  setupWifi();
  
  setupIR();
  setupSpeaker();
  setupDisplay();
}

void loop() {
  loopIR();
  loopWifi();
  loopDisplay();
}

void onHttpWait() {
  peekIR();
}

void onVolumeUp() {
  increaseVolume();
}

void onVolumeDown() {
  decreaseVolume();
}

void onTvRad() {
  setAux();
}

void onMute() {
  toggleMute();
}