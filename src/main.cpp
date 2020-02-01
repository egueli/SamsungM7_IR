#include <Arduino.h>

#include "ir.h"
#include "serial.h"

void setup() {

  USE_SERIAL.begin(115200);
  // USE_SERIAL.setDebugOutput(true);

  USE_SERIAL.println();
  USE_SERIAL.println();
  USE_SERIAL.println();

//   setupWifi();
  
  setupIR();
//   setupSpeaker();
//   setupDisplay();
}

void loop() {
  loopIR();
//   loopWifi();
//   loopDisplay();
}

void onVolumeUp() {
//   notifyIR();
//   increaseVolume();
}

void onVolumeDown() {
//   notifyIR();
//   decreaseVolume();
}

void onTvRad() {
//   notifyIR();
//   setAux();
}

void onMute() {
//   notifyIR();
//   toggleMute();
}