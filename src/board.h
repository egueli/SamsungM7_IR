#pragma once

#include <stdint.h>

const uint8_t kIrRecvPin = 5; // D1
const uint8_t kWifiLedPin = LED_BUILTIN;

// Uncomment this to use the legacy MAX7219 display method.
// Otherwise the firmware assumes there's a TM1637 display driver.
//#define DISPLAY_MAX7219

/**
 * The MAX7219 will drive up to this number of displays.
 * Must not be lower than the maximum item in kDigitsWiring.
 */
const byte kDisplayScanLimit = 6;

// SPI uses pin 12 (D6), 13 (D7), 14 (D5); CS is defined below.
const uint8_t kMax7219LoadPin = 16; // D0

/*
Level shifter mapping:
   LV/HV  
D0   4   MAX7219:12
D5   3   MAX7219:13
D7   2   MAX7219:1
D1   1   TSOP1738:3
*/

const uint8_t kTm1637ClockPin = 14; // D5
const uint8_t kTm1637DataPin = 13; // D7

//                               dp G  F  E  D  C  B  A
#ifdef DISPLAY_MAX7219
const byte kSegmentsWiring[] = { 4, 6, 1, 3, 7, 2, 0, 5 };
#else
const byte kSegmentsWiring[] = { 7, 6, 5, 4, 3, 2, 1, 0 };
#endif

//                          10^3  2  1  0
#ifdef DISPLAY_MAX7219
const byte kDigitsWiring[] = { 2, 6, 4, 0 };
#else
const byte kDigitsWiring[] = { 3, 2, 1, 0 };
#endif
