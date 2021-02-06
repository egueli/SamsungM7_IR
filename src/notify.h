#pragma once

#include <Arduino.h>
#include "error.h"
#include "speaker.h"

void notifyNoSpeaker();
void notifySpeakerAddress(String address);

void notifyVolumeGetSuccess(Speaker &speaker, int volume);
void notifyVolumeSetSuccess(Speaker &speaker, int volume);

void notifyAuxGetSuccess(bool isAux);
void notifyAuxSetSuccess(bool isAux);

void notifyTv(bool wasSet);

void notifyMuteGetSuccess();
void notifyMuteSetSuccess(bool isMuted);

void notifyFail(Result result);