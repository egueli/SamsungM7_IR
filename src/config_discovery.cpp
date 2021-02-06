#include "config.h"

#ifdef SPEAKER_MULTIROOM
const char *kSpeakerMdnsServiceQuestion = "_spotify-connect._tcp.local";
const char *kSpeakerServiceName = "Tramezzino";
#else
const char *kSpeakerMdnsServiceQuestion = "_http._tcp.local";
const char *kSpeakerServiceName = "Living Room";
#endif
