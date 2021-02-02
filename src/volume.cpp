#include "volume.h"
#include "speaker.h"

#include "serial.h"
#include "config.h"

bool setVolumeDelta(Speaker& speaker, int delta) {
  USE_SERIAL.print("... ");
  int volume;
  bool success;
  success = speaker.getVolume(volume);
  if (!success) {
    notifyVolumeGetFail();
    return false;
  }
  notifyVolumeGetSuccess(volume);
  int newVolume = volume + delta;
  // prevent going out of bounds
  newVolume = constrain(newVolume, kMinVolume, kMaxVolume);
  USE_SERIAL.print(volume);
  USE_SERIAL.print(" => ");
  USE_SERIAL.print(newVolume);
  USE_SERIAL.print(" ");
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
  bool success = setVolumeDelta(speaker, kVolumeUpStep);
  USE_SERIAL.println(success ? "OK" : "fail :(");
}

void decreaseVolume(Speaker& speaker) {
  USE_SERIAL.print("VOL- ");
  bool success = setVolumeDelta(speaker, kVolumeDownStep);
  USE_SERIAL.println(success ? "OK" : "fail :(");
}


bool toggleMute(Speaker &speaker) {
  bool isMuted;
  USE_SERIAL.print("Mute: ");
  bool getSuccess = speaker.getMuteStatus(isMuted);
  if (!getSuccess) {
    USE_SERIAL.println("unable to get mute state");
    notifyMuteGetFail();
    return false;
  }
  notifyMuteGetSuccess();
  USE_SERIAL.print(isMuted ? "on" : "off");
  USE_SERIAL.print(" => ");

  bool setSuccess = speaker.setMuteStatus(!isMuted);
  if (!setSuccess) {
    USE_SERIAL.println("fail :(");
    notifyMuteSetFail();
    return false;
  }

  USE_SERIAL.println(!isMuted ? "on" : "off");
  notifyMuteSetSuccess(!isMuted);
  return true;
}