#include "seven_seg_max7219.h"

#include <Arduino.h>
#include <SPI.h>
#include "board.h"

void Max7219Display::begin()
{
    ledMatrix.init();
    ledMatrix.sendByte(MAX7219_REG_SCANLIMIT, kDisplayScanLimit);
    clear();
}

void Max7219Display::setBrightness(const uint8_t brightness) {
    ledMatrix.setIntensity(brightness >> 4);
}

void Max7219Display::setSegments(const uint8_t digits[]) {
    for (int digitIndex = 0; digitIndex < 4; digitIndex++) {
        ledMatrix.setColumn(digitIndex, digits[digitIndex]);
    }
}
