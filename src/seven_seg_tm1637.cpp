#include "seven_seg_tm1637.h"

#include <Arduino.h>
#include "board.h"

void Tm1637Display::begin()
{
    clear();
}

void Tm1637Display::setBrightness(const uint8_t brightness) {
    display.setBrightness(brightness >> 5);
}

void Tm1637Display::setSegments(const uint8_t digits[]) {
    display.setSegments(digits);
}
