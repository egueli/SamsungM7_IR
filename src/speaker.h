#pragma once

#include <Arduino.h>
#include "http_xml.h"

/**
 * Controls an IP-accessible audio speaker.
 */
struct Speaker {
    /**
     * Initializes the speaker.
     */
    virtual void setup() = 0;

    /**
     * Checks that the speaker is still reachable via the current IP address.
     * Restarts the system after too many failed attempts to connect.
     * To be called in the idle loop.
     */
    virtual void loop() = 0;

    /**
     * Sets the IP address for this speaker.
     */
    virtual void setAddress(String address) = 0;

    /**
     * Checks if this speaker is still reachable at the current IP address.
     * This function will make an HTTP request and will call onHttpWait() while waiting for the response.
     * \return true if it is reachable, false otherwise.
     */
    virtual bool isAddressValid() = 0;

    /**
     * Special value returned by getVolume() when a connection error occurred.
     */
    static const int kGetVolumeError = -1;

    /** 
     * Queries the speaker for the current volume level.
     * This function will make an HTTP request and will call onHttpWait() while waiting for the response.
     * \return the volume level from 0 to 30, or kGetVolumeError if an error occurred.
     */
    virtual int getVolume() = 0;

    /**
     * Sets the speaker volume.
     * This function will make an HTTP request and will call onHttpWait() while waiting for the response.
     * \param newVolume the desired volume from 0 to 30. Note that no bounds check is made.
     * \return true if the set was successful, false otherwise.
     */
    virtual bool setVolume(int newVolume) = 0;

    /**
     * Sets the speaker's audio input to the TV corresponding to the IR remote being used.
     * This function will make an HTTP request and will call onHttpWait() while waiting for the response.
     * \return true if the set was successful, false otherwise.
     */
    virtual bool setTvInput() = 0;

    /**
     * Toggles the mute status of the speaker.
     * This function will make an HTTP request and will call onHttpWait() while waiting for the response.
     * \return true if the toggle was successful, false otherwise.
     */
    virtual bool toggleMute() = 0;
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
