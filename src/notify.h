#pragma once

#include <Arduino.h>
#include "error.h"

void notifyNoSpeaker();
void notifySpeakerAddress(String address);

void notifyVolumeGetSuccess(int volume);
void notifyVolumeSetSuccess(int volume);

void notifyAuxGetSuccess(bool isAux);
void notifyAuxSetSuccess(bool isAux);

void notifyTv(bool wasSet);

void notifyMuteGetSuccess();
void notifyMuteSetSuccess(bool isMuted);

void notifyFail(Result result);