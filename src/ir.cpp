#include <IRrecv.h>
#include <IRremoteESP8266.h>
#include <IRutils.h>

#include "ir.h"
#include "serial.h"

const uint16_t kIrRecvPin = 5; // D1
const uint16_t kIrCaptureBufferSize = 1024;
const uint8_t kIrTimeout = 15;
const uint8_t kIrTolerance = 30;
const uint8_t kIrMaxSkip = 5;
const uint16_t kIrNoiseFloor = 300;

const uint64_t kIrVolumeUpCode = 0x20DF40BF;
const uint64_t kIrVolumeDownCode = 0x20DFC03F;
const uint64_t kIrMuteCode = 0x20DF906F;
const uint64_t kIrTvRadCode = 0x20DF0FF0;

IRrecv irrecv(kIrRecvPin, kIrCaptureBufferSize, kIrTimeout, true);
decode_results results; // can't pass it as parameter to function?

// Holds the last non-repeat IR code. So when IR repeat messages are received, we know
// which code has been repeated. UINT64_MAX means no code has been received yet.
uint64_t currentIrCode = UINT64_MAX;

// Holds the next IR code to call callbacks for. Set by peekIR() and consumed by consumeIR().
// If peekIR() decodes multiple messages before consumeIR() is called, only the last
// message should be kept. Once consumerIR() is done with this code, it sets it to UINT64_MAX.
uint64_t irCodeToProcess = UINT64_MAX;

void peekIR() {
  bool haveNewMessage = irrecv.decode(&results, NULL, kIrMaxSkip, kIrNoiseFloor);
  if (!haveNewMessage) {
    return;
  }

  if (results.decode_type != NEC) {
    return;
  }

  if (results.repeat) {
    irCodeToProcess = currentIrCode;
  } else {
    irCodeToProcess = results.value;
    currentIrCode = results.value;
  }

  USE_SERIAL.printf("[ir:%llx]", irCodeToProcess);
}

void consumeIR() {
  if (irCodeToProcess == UINT64_MAX) {
    return;
  }

  switch (irCodeToProcess) {
    case kIrVolumeUpCode:
      onVolumeUp();
      break;
    case kIrVolumeDownCode:
      onVolumeDown();
      break;
    case kIrTvRadCode:
      onTvRad();
      break;
    case kIrMuteCode:
      onMute();
      break;
    default:
      serialPrintUint64(irCodeToProcess, HEX);
      Serial.println();
  }

  irCodeToProcess = UINT64_MAX;
}

void setupIR() {
  irrecv.enableIRIn();  // Start the receiver
  irrecv.setTolerance(kIrTolerance);
}

void loopIR() {
  peekIR();
  consumeIR();
}
