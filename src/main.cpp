#include <Arduino.h>

#include "ir.h"
#include "serial.h"
#include "board.h"

void setup() {

  USE_SERIAL.begin(115200);
  // USE_SERIAL.setDebugOutput(true);

  USE_SERIAL.println();
  USE_SERIAL.println();
  USE_SERIAL.println();

  pinMode(kOnboardLedPin, OUTPUT);
  setupIR();
}

void loop() {
  loopIR();
}

void onVolumeUp() {
}

void onVolumeDown() {
}

void onTvRad() {
}

void onMute() {
}

void onToggleClock() {
}

void notifyIR() {
  digitalWrite(kOnboardLedPin, HIGH);
  delay(15);
  digitalWrite(kOnboardLedPin, LOW);
}