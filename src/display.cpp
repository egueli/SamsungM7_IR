#include <SPI.h>
#include "LedMatrix.h"
#include "pins.h"

const unsigned long kTextDisplayDuration = 2000;


//                               dp G  F  E  D  C  B  A
const byte kSegmentsWiring[] = { 4, 0, 1, 2, 3, 7, 5, 6 };

//                             left     right
const byte kDigitsWiring[] = { 1, 0, 2, 3 };

const byte kNumChars = 20;
const byte kDisplayFont[kNumChars][2] = {
  // ch    ABCDEFGd
  { ' ', 0b00000000 },
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
  { 'E', 0b10011110 },
  { 'e', 0b10011110 },
  { 'm', 0b00101010 },
  { 'n', 0b00101010 },
  { 'o', 0b00111010 },
  { 's', 0b10110110 },
  { 't', 0b00011110 },
  { 'u', 0b00111000 },
  { 'v', 0b00111000 },
};

LedMatrix ledMatrix = LedMatrix(1, kMax7219LoadPin);

unsigned long lastTextAt;

void setupDisplay() {
  ledMatrix.init();
  ledMatrix.sendByte(MAX7219_REG_SCANLIMIT, 4);   // show only 4 digits

  ledMatrix.clear();
  ledMatrix.commit();
  ledMatrix.setIntensity(15);
}

void loopDisplay() {
  if (lastTextAt != 0 && millis() > lastTextAt + kTextDisplayDuration) {
    ledMatrix.clear();
    ledMatrix.commit();
    lastTextAt = 0;
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

void displayText(String text) {
  ledMatrix.clear();
  int digit = 0;
  for (int p = 0; p < (int)text.length() && digit < 4 + 1; p++) {
    char ch = text[p];
    if (digit > 0 && ch == '.') {
      byte prevDigitIndex = kDigitsWiring[digit - 1];
      byte dotIndex = kSegmentsWiring[0];
      ledMatrix.setPixel(prevDigitIndex, dotIndex);
    } else if (digit < 4) {
      byte digitIndex = kDigitsWiring[digit];
      byte glyph = getSegmentsOfChar(ch);
      for (int ns = 0; ns < 8; ns++) {
        if ((glyph & (1 << ns)) != 0) {
          byte segmentIndex = kSegmentsWiring[ns];
          ledMatrix.setPixel(digitIndex, segmentIndex);
        }
      }
      digit++;
    }
  }
  ledMatrix.commit();
  ledMatrix.setIntensity(15);
  lastTextAt = millis();
}

