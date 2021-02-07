#include <Arduino.h>
#include "board.h"
#include "config.h"
#include "seven_seg_display.h"
#include "display.h"

#ifdef DISPLAY_MAX7219
#include "seven_seg_max7219.h"
#else
#include "seven_seg_tm1637.h"
#endif

const unsigned long kTextDisplayDuration = 1500;

const byte kNumChars = 28;
const byte kDisplayFont[kNumChars][2] = {
  // ch    ABCDEFGd
  { ' ', 0b00000000 },
  { '-', 0b00000010 },
  { '0', 0b11111100 },
  { '1', 0b01100000 },
  { '2', 0b11011010 },
  { '3', 0b11110010 },
  { '4', 0b01100110 },
  { '5', 0b10110110 },
  { '6', 0b10111110 },
  { '7', 0b11100000 },
  { '8', 0b11111110 },
  { '9', 0b11110110 },
  { 'A', 0b11101110 },
  { 'E', 0b10011110 },
  { 'H', 0b01101110 },
  { 'h', 0b00101110 },
  { 'O', 0b11111100 },
  { 'S', 0b10110110 },
  { 'e', 0b10011110 },
  { 'i', 0b00001000 },
  { 'm', 0b00101010 },
  { 'n', 0b00101010 },
  { 'o', 0b00111010 },
  { 'P', 0b11001110 },
  { 's', 0b10110110 },
  { 't', 0b00011110 },
  { 'u', 0b00111000 },
  { 'v', 0b00111000 },
};

#ifdef DISPLAY_MAX7219
Max7219Display max7219Display(kMax7219LoadPin);
SevenSegmentDisplay &display = max7219Display;
#else
Tm1637Display tm1637Display(kTm1637ClockPin, kTm1637DataPin);
SevenSegmentDisplay &display = tm1637Display;
#endif

unsigned long lastTextAt;

byte getSegmentsOfChar(char c);
void renderText(uint8_t outDigits[4], String text);

void setupDisplay() {
  display.begin();
  display.setBrightness(kDisplayBrightness);
  displayText(" Hi");
} 

void loopDisplay() {
  if (lastTextAt != 0 && millis() > lastTextAt + kTextDisplayDuration) {
    display.clear();
    lastTextAt = 0;
  }
}

void displayText(String text) {
  uint8_t digits[4] = {0, 0, 0, 0};
  renderText(digits, text);
  display.setSegments(digits);
  display.setBrightness(kDisplayBrightness);
  lastTextAt = millis();
}

void renderText(uint8_t outDigits[4], String text) {
  int digit = 0;
  for (int p = 0; p < (int)text.length() && digit < 4 + 1; p++) {
    char ch = text[p];
    if (digit > 0 && ch == '.') {
      byte prevDigitIndex = kDigitsWiring[digit - 1];
      byte dotIndex = kSegmentsWiring[0];
      bitSet(outDigits[prevDigitIndex], dotIndex);
    } else if (digit < 4) {
      byte digitIndex = kDigitsWiring[digit];
      byte glyph = getSegmentsOfChar(ch);
      for (int ns = 0; ns < 8; ns++) {
        if ((glyph & (1 << ns)) != 0) {
          byte segmentIndex = kSegmentsWiring[ns];
          bitSet(outDigits[digitIndex], segmentIndex);
        }
      }
      digit++;
    }
  }
}

byte getSegmentsOfChar(char c) {
  for (int i = 0; i < kNumChars; i++) {
    auto possibleCharacter = kDisplayFont[i];
    if (possibleCharacter[0] == c) {
      return possibleCharacter[1];
    }
  }
  return 0b00010000;
}
