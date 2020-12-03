#pragma once

#include <Arduino.h>

class Speaker {
public:
    void setup();
    void loop();
    void setAddress(String address);

    static const int kGetVolumeError = -1;

    /** 
     * Queries the speaker for the current volume level.
     * \return the volume level from 0 to 30, or kGetVolumeError if an error occurred.
     */
    int getVolume();

    /**
     * Set the speaker volume.
     * \param newVolume the desired volume from 0 to 30. Note that no bounds check is made.
     * \return true if the set was successful, false otherwise.
     */
    bool setVolume(int newVolume);

    bool setAux();
    bool toggleMute();
    void checkSpeakerIpAddress();

private:
    bool getQueryUrl(String &output, String command);
    bool getSingleParamCommandUrl(String &output, String command, String paramType, String paramName, String paramValue);

    int getVolumeFromHttp();
    bool checkSuccess();
    bool isInputSourceAux(bool &isAux);
    bool getMute(bool &muteStatus);
    bool setMute(bool muteStatus);
    bool isSpeakerAddressValid();
};



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
