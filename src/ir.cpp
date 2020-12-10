#include <IRrecv.h>
#include <IRremoteESP8266.h>
#include <IRutils.h>

#include "ir.h"
#include "serial.h"
#include "board.h"
#include "config.h"

const uint16_t kIrCaptureBufferSize = 1024;
const uint8_t kIrTimeout = 15;
const uint8_t kIrTolerance = 45;
const uint8_t kIrMaxSkip = 5;
const uint16_t kIrNoiseFloor = 300;

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

  if (results.overflow) {
    USE_SERIAL.print("[ir:OVERFLOW]");
    currentIrCode = irCodeToProcess = UINT64_MAX;
    return;
  }

  if (results.decode_type != NEC) {
    USE_SERIAL.printf("[ir:UNKNOWN(%d)]", results.rawlen);
    // USE_SERIAL.println();
    // USE_SERIAL.println(resultToSourceCode(&results));
    currentIrCode = irCodeToProcess = UINT64_MAX;
    return;
  }

  if (results.repeat) {
    irCodeToProcess = currentIrCode;
    USE_SERIAL.print("[ir:RPT]");
  } else {
    irCodeToProcess = results.value;
    currentIrCode = results.value;
    USE_SERIAL.printf("[ir:%llx]", results.value);
  }
}

void consumeIR() {
  if (irCodeToProcess == UINT64_MAX) {
    return;
  }

  switch (irCodeToProcess) {
    case kIrVolumeUpCode:
      notifyIR();
      onVolumeUp();
      break;
    case kIrVolumeDownCode:
      notifyIR();
      onVolumeDown();
      break;
    case kIrTvRadCode:
      notifyIR();
      onTvRad();
      break;
    case kIrMuteCode:
      notifyIR();
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
