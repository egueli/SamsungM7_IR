#pragma once

#include <Arduino.h>

void notifyNoSpeaker();
void notifySpeakerAddress(String address);

void notifyVolumeGetSuccess(int volume);
void notifyVolumeGetFail();
void notifyVolumeSetSuccess(int volume);
void notifyVolumeSetFail();

void notifyAuxGetSuccess(bool isAux);
void notifyAuxGetFail();
void notifyAuxSetSuccess(bool isAux);
void notifyAuxSetFail();

void notifyTv(bool wasSet);
void notifyTvFail();

void notifyMuteGetSuccess();
void notifyMuteGetFail();
void notifyMuteSetSuccess(bool isMuted);
void notifyMuteSetFail();
