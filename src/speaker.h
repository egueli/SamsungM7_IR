#pragma once

#include <Arduino.h>

void setupSpeaker();
void loopSpeaker();
void setSpeakerAddress(String address);

void increaseVolume();
void decreaseVolume(); 
bool setAux();
bool toggleMute();
void checkSpeakerIpAddress();

// The functions below are callbacks. Their definition must be in a file other than speaker.cpp.
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

void notifyMuteGetSuccess();
void notifyMuteGetFail();
void notifyMuteSetSuccess(bool isMuted);
void notifyMuteSetFail();
