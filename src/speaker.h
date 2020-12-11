#pragma once

#include <Arduino.h>

/**
 * Controls an IP-accessible audio speaker.
 */
struct Speaker {
    /**
     * Sets the IP address for this speaker.
     */
    virtual void setAddress(const String &address) = 0;

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
     * Will call notify* functions to indicate the progress.
     * This function will make an HTTP request and will call onHttpWait() while waiting for the response.
     * \return true if the set was successful, false otherwise.
     */
    virtual bool setTvInput() = 0;

    /**
     * Queries the speaker for the current mute status.
     * This function will make an HTTP request and will call onHttpWait() while waiting for the response.
     * \param outStatus the variable that will contain the mute status. True means that the spaker is muted.
     * \return true if the query was successful, false otherwise.
     */
    virtual bool getMuteStatus(bool &outStatus) = 0;

    /**
     * Sets the speaker mute status.
     * This function will make an HTTP request and will call onHttpWait() while waiting for the response.
     * \param newMuteStatus the desired mute status. True means that the speaker is muted.
     * \return true if the set was successful, false otherwise.
     */
    virtual bool setMuteStatus(const bool newMuteStatus) = 0;
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

void notifyTv(bool wasSet);
void notifyTvFail();

void notifyMuteGetSuccess();
void notifyMuteGetFail();
void notifyMuteSetSuccess(bool isMuted);
void notifyMuteSetFail();
