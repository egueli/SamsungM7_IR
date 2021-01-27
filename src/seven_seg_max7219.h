#pragma once
#include "seven_seg_display.h"
#include "LedMatrix.h"

class Max7219Display : public SevenSegmentDisplay {
public:
    Max7219Display(const uint8_t loadPin) 
    : ledMatrix(LedMatrix(1, loadPin)) {}

    void begin();
    void setBrightness(const uint8_t brightness);
    void setSegments(const uint8_t digits[]);

private:
    LedMatrix ledMatrix;
};