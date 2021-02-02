#pragma once

#include <Arduino.h>

void notifyNoSpeaker();
void notifySpeakerAddress(String address);

void notifyVolumeGetSuccess(int volume);
void notifyVolumeSetSuccess(int volume);
void notifyVolumeFail();

void notifyAuxGetSuccess(bool isAux);
void notifyAuxSetSuccess(bool isAux);
void notifyAuxFail();

void notifyTv(bool wasSet);
void notifyTvFail();

void notifyMuteGetSuccess();
void notifyMuteSetSuccess(bool isMuted);
void notifyMuteFail();
