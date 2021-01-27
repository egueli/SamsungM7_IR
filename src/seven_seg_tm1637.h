#pragma once

#include "seven_seg_display.h"
#include <TM1637Display.h>

class Tm1637Display : public SevenSegmentDisplay {
public:
    Tm1637Display(const uint8_t clockPin, const uint8_t dataPin) :
        display(clockPin, dataPin) {}

    void begin();
    void setBrightness(const uint8_t brightness);
    void setSegments(const uint8_t digits[]);

private:
    TM1637Display display;
};