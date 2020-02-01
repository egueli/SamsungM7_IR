#include <IRrecv.h>
#include <IRremoteESP8266.h>
#include <IRutils.h>

#include "ir.h"
#include "serial.h"

const uint16_t kIrRecvPin = 5; // D1
const uint16_t kIrCaptureBufferSize = 1024;
const uint8_t kIrTimeout = 15;

const uint64_t kIrVolumeUpCode = 0x20DF40BF;
const uint64_t kIrVolumeDownCode = 0x20DFC03F;
const uint64_t kIrMuteCode = 0x20DF906F;
const uint64_t kIrTvRadCode = 0x20DF0FF0;

IRrecv irrecv(kIrRecvPin, kIrCaptureBufferSize, kIrTimeout, true);
decode_results results; // can't pass it as parameter to function?

uint64_t last_decoded = UINT64_MAX;

void onIRMessage() {
  if (results.decode_type != NEC) {
    return;
  }

  uint64_t decoded;
  if (results.repeat) {
    decoded = last_decoded;
  } else {
    decoded = results.value;
    last_decoded = decoded;
  }

  switch (decoded) {
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
      serialPrintUint64(decoded, HEX);
      Serial.println();
  }
}

void setupIR() {
  irrecv.enableIRIn();  // Start the receiver
}

void loopIR() {
  if (irrecv.decode(&results)) {
    irrecv.resume();  // get ready to receive the next value while we handle this one
    onIRMessage();
  }
}
