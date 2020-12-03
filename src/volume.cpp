#include "volume.h"
#include "speaker.h"

#include "serial.h"

bool setVolumeDelta(Speaker& speaker, int delta) {
  USE_SERIAL.print("... ");
  int volume = speaker.getVolume();
  if (volume == Speaker::kGetVolumeError) {
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
    success = speaker.setVolume(newVolume);
  }
  if (success) {
    notifyVolumeSetSuccess(newVolume);
  } else {
    notifyVolumeSetFail();
  }
  return success;
}

void increaseVolume(Speaker& speaker) {
  USE_SERIAL.print("VOL+ ");
  bool success = setVolumeDelta(speaker, 1);
  USE_SERIAL.println(success ? "OK" : "fail :(");
}

void decreaseVolume(Speaker& speaker) {
  USE_SERIAL.print("VOL- ");
  bool success = setVolumeDelta(speaker, -3);
  USE_SERIAL.println(success ? "OK" : "fail :(");
}