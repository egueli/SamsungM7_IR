#pragma once

#include <stdint.h>

struct SevenSegmentDisplay {
    /** Initialize the display */
    virtual void begin() = 0;

    /**
     * Set the brightness from 0 to 255. 0 means off.
     * The display may use less precision.
     */
    virtual void setBrightness(const uint8_t brightness) = 0;

    /**
     * Set the segment data.
     * 
     * The array must be 4 bytes long for 4 digits.
     */
    virtual void setSegments(const uint8_t digits[]) = 0;

    void clear() {
        const uint8_t empty[] = {0, 0, 0, 0}; 
        setSegments(empty);
    }
};
