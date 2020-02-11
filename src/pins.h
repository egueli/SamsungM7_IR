#pragma once

#include <stdint.h>

const uint8_t kIrRecvPin = 5; // D1
const uint8_t kMax7219LoadPin = 16; // D0
const uint8_t kWifiLedPin = LED_BUILTIN;

// SPI uses pin 12 (D6), 13 (D7), 14 (D5), maybe 15 (D8) for CS