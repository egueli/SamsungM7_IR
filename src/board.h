#pragma once

#include <stdint.h>

const uint8_t kIrRecvPin = 5; // D1
const uint8_t kOnboardLedPin = LED_BUILTIN;

/*
Level shifter mapping:
   LV/HV  
D0   4   MAX7219:12
D5   3   MAX7219:13
D7   2   MAX7219:1
D1   1   TSOP1738:3
*/
