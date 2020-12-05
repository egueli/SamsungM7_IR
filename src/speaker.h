#pragma once

#include <Arduino.h>
#include "http_xml.h"

/**
 * Controls an IP-accessible audio speaker.
 */
class Speaker {
public:
    /**
     * Initializes the speaker.
     */
    void setup();

    /**
     * Checks that the speaker is still reachable via the current IP address.
     * Restarts the system after too many failed attempts to connect.
     * To be called in the idle loop.
     */
    void loop();

    /**
     * Sets the IP address for this speaker.
     */
    void setAddress(String address);

    /**
     * Special value returned by getVolume() when a connection error occurred.
     */
    static const int kGetVolumeError = -1;

    /** 
     * Queries the speaker for the current volume level.
     * This function will make an HTTP request and will call onHttpWait() while waiting for the response.
     * \return the volume level from 0 to 30, or kGetVolumeError if an error occurred.
     */
    int getVolume();

    /**
     * Sets the speaker volume.
     * This function will make an HTTP request and will call onHttpWait() while waiting for the response.
     * \param newVolume the desired volume from 0 to 30. Note that no bounds check is made.
     * \return true if the set was successful, false otherwise.
     */
    bool setVolume(int newVolume);

    /**
     * Sets the speaker's audio input to the TV corresponding to the IR remote being used.
     * This function will make an HTTP request and will call onHttpWait() while waiting for the response.
     * \return true if the set was successful, false otherwise.
     */
    bool setTvInput();

    /**
     * Toggles the mute status of the speaker.
     * This function will make an HTTP request and will call onHttpWait() while waiting for the response.
     * \return true if the toggle was successful, false otherwise.
     */
    bool toggleMute();

private:
    bool getQueryUrl(String &output, String command);
    bool getSingleParamCommandUrl(String &output, String command, String paramType, String paramName, String paramValue);

    int getVolumeFromHttp();
    bool checkSuccess();
    bool isInputSourceAux(bool &isAux);
    bool getMute(bool &muteStatus);
    bool setMute(bool muteStatus);
    bool isSpeakerAddressValid();
    void checkSpeakerIpAddress();
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
