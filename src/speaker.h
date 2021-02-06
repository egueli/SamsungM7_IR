#pragma once

#include <Arduino.h>
#include "error.h"

/**
 * Controls an IP-accessible audio speaker.
 */
struct Speaker {
    struct Configuration;

    /**
     * Returns a configuration that is specific to this speaker type.
     */
    virtual Configuration getConfiguration() = 0;

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
     * Queries the speaker for the current volume level.
     * This function will make an HTTP request and will call onHttpWait() while waiting for the response.
     * \param outVolume the current speaker volume. The magnitude is device-specific.
     * \return the result of the query
     */
    virtual Result getVolume(int &outVolume) = 0;

    /**
     * Sets the speaker volume.
     * This function will make an HTTP request and will call onHttpWait() while waiting for the response.
     * \param newVolume the desired volume as a non-negative number. The bounds are device-specific, and no checks are made.
     * \return the result of the operation
     */
    virtual Result setVolume(int newVolume) = 0;

    /**
     * Sets the speaker's audio input to the TV corresponding to the IR remote being used.
     * Will call notify* functions to indicate the progress.
     * This function will make an HTTP request and will call onHttpWait() while waiting for the response.
     * \return the result of the operation
     */
    virtual Result setTvInput() = 0;

    /**
     * Queries the speaker for the current mute status.
     * This function will make an HTTP request and will call onHttpWait() while waiting for the response.
     * \param outStatus the variable that will contain the mute status. True means that the spaker is muted.
     * \return the result of the query
     */
    virtual Result getMuteStatus(bool &outStatus) = 0;

    /**
     * Sets the speaker mute status.
     * This function will make an HTTP request and will call onHttpWait() while waiting for the response.
     * \param newMuteStatus the desired mute status. True means that the speaker is muted.
     * \return the result of the operation
     */
    virtual Result setMuteStatus(const bool newMuteStatus) = 0;

    struct Configuration {
        const char *mdnsServiceQuestion;
        const int minVolume;
        const int maxVolume;
        const int volumeUpStep;
        const int volumeDownStep;
    };
};
