#pragma once

/*
 * This file collects all user-related configuration.
 * Other configuration is in
 * - wifi_credentials.h for WiFi credentials (not checked in source code) 
 * - board.h for board-specific configuration (pins, display wiring etc.)
 */

#include <stdint.h>

/*
Uncomment this to use the Samsung Multiroom (legacy) instead of
Yamaha MusicCast protocol.
*/
//#define SPEAKER_MULTIROOM



#ifdef SAMSUNG_MULTIROOM
const int kMinVolume = 0;
const int kMaxVolume = 30;
const int kVolumeUpStep = 1;
const int kVolumeDownStep = -3;
#else
const int kMinVolume = 1;
const int kMaxVolume = 161;
const int kVolumeUpStep = 2;
const int kVolumeDownStep = -2;
#endif

// IR codes for the remote control being used.
#if 1
// My LG TV remote control
const uint64_t kIrVolumeUpCode = 0x20DF40BF;
const uint64_t kIrVolumeDownCode = 0x20DFC03F;
const uint64_t kIrMuteCode = 0x20DF906F;
const uint64_t kIrTvRadCode = 0x20DF0FF0;
#else
// My Acer TV remote control
const uint64_t kIrVolumeUpCode = 0x86C620DF;
const uint64_t kIrVolumeDownCode = 0x86C6A05F;
const uint64_t kIrMuteCode = 0x86C67887;
const uint64_t kIrTvRadCode = 0x86C630CF;
#endif