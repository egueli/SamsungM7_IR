#pragma once

#include <stdint.h>

const uint8_t kIrRecvPin = 5; // D1
const uint8_t kMax7219LoadPin = 16; // D0
const uint8_t kWifiLedPin = LED_BUILTIN;

// SPI uses pin 12 (D6), 13 (D7), 14 (D5), maybe 15 (D8) for CS

/*
Level shifter mapping:
   LV/HV  
D0   4   MAX7219:12
D5   3   MAX7219:13
D7   2   MAX7219:1
D1   1   TSOP1738:3
*/


//                               dp G  F  E  D  C  B  A
const byte kSegmentsWiring[] = { 4, 6, 1, 3, 7, 2, 0, 5 };

//                          10^3  2  1  0
const byte kDigitsWiring[] = { 2, 6, 4, 0 };

/**
 * The MAX7219 will drive up to this number of displays.
 * Must not be lower than the maximum item in kDigitsWiring.
 */
const byte kDisplayScanLimit = 6;