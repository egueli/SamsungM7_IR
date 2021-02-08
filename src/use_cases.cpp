#include "use_cases.h"
#include "speaker.h"
#include "notify.h"
#include "serial.h"
#include "config.h"
#include "display.h"

Result setVolumeDelta(Speaker& speaker, int delta) {
  USE_SERIAL.print("... ");
  int volume;
  RETURN_IF_ERROR(speaker.getVolume(volume))
  notifyVolumeGetSuccess(speaker, volume);
  int newVolume = volume + delta;
  // prevent going out of bounds
  auto configuration = speaker.getConfiguration();
  newVolume = constrain(newVolume, configuration.minVolume, configuration.maxVolume);
  USE_SERIAL.print(volume);
  USE_SERIAL.print(" => ");
  USE_SERIAL.print(newVolume);
  USE_SERIAL.print(" ");
  if (newVolume == volume) {
    // skip an HTTP request just to set the same volume
    USE_SERIAL.print("(skip) ");
  } else {
    USE_SERIAL.print(" ... ");
    RETURN_IF_ERROR(speaker.setVolume(newVolume))
  }
  notifyVolumeSetSuccess(speaker, newVolume);
  return Result::OK;
}

Result doIncreaseVolume(Speaker& speaker) {
  USE_SERIAL.print("VOL+ ");
  RETURN_IF_ERROR(setVolumeDelta(speaker, speaker.getConfiguration().volumeUpStep))
  return Result::OK;
}

void increaseVolume(Speaker& speaker) {
  Result result = doIncreaseVolume(speaker);
  if (result != Result::OK) {
    notifyFail(result);
  }
}

Result doDecreaseVolume(Speaker& speaker) {
  USE_SERIAL.print("VOL- ");
  RETURN_IF_ERROR(setVolumeDelta(speaker, speaker.getConfiguration().volumeDownStep))
  return Result::OK;
}

void decreaseVolume(Speaker& speaker) {
  Result result = doDecreaseVolume(speaker);
  if (result != Result::OK) {
    notifyFail(result);
  }
}

Result doToggleMute(Speaker &speaker) {
  bool isMuted;
  USE_SERIAL.print("Mute: ");
  RETURN_IF_ERROR(speaker.getMuteStatus(isMuted))
  notifyMuteGetSuccess();
  USE_SERIAL.print(isMuted ? "on" : "off");
  USE_SERIAL.print(" => ");

  RETURN_IF_ERROR(speaker.setMuteStatus(!isMuted))

  USE_SERIAL.println(!isMuted ? "on" : "off");
  notifyMuteSetSuccess(!isMuted);
  return Result::OK;
}

void toggleMute(Speaker &speaker) {
  Result result = doToggleMute(speaker);
  if (result != Result::OK) {
    notifyFail(result);
  }
}

void setTvInput(Speaker &speaker) {
  Result result = speaker.setTvInput();
  if (result != Result::OK) {
    notifyFail(result);
  }
}

void toggleClock() {
  toggleDisplayBackground();
}