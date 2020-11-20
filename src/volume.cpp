#include "volume.h"
#include "speaker.h"

#include "serial.h"

bool setVolumeDelta(int delta) {
  USE_SERIAL.print("... ");
  int volume = getVolume();
  if (volume == kGetVolumeError) {
    notifyVolumeGetFail();
    return false;
  }
  notifyVolumeGetSuccess(volume);
  int newVolume = volume + delta;
  // prevent going out of bounds
  newVolume = constrain(newVolume, 0, 30);
  USE_SERIAL.print(volume);
  USE_SERIAL.print(" => ");
  USE_SERIAL.print(newVolume);
  USE_SERIAL.print(" ");
  bool success;
  if (newVolume == volume) {
    // skip an HTTP request just to set the same volume
    USE_SERIAL.print("(skip) ");
    success = true;
  } else {
    USE_SERIAL.print(" ... ");
    success = setVolume(newVolume);
  }
  if (success) {
    notifyVolumeSetSuccess(newVolume);
  } else {
    notifyVolumeSetFail();
  }
  return success;
}

void increaseVolume() {
  USE_SERIAL.print("VOL+ ");
  bool success = setVolumeDelta(1);
  USE_SERIAL.println(success ? "OK" : "fail :(");
}

void decreaseVolume() {
  USE_SERIAL.print("VOL- ");
  bool success = setVolumeDelta(-3);
  USE_SERIAL.println(success ? "OK" : "fail :(");
}