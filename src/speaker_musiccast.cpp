#include "speaker_musiccast.h"
#include "http_response.h"

void MusicCastSpeaker::setAddress(const String &address) {
    ipAddress = address;
}

bool MusicCastSpeaker::isAddressValid() {
    if (ipAddress.isEmpty()) {
        return false;
    }

    String url;
    url = "http://" + ipAddress + "/YamahaExtendedControl/v1/main/getStatus";

    request.open("GET", url.c_str());
    request.send();

    return waitForHttpOkResponse(request);
}

int MusicCastSpeaker::getVolume() {
    return 0;
}

bool MusicCastSpeaker::setVolume(int newVolume) {
    return true;
}

bool MusicCastSpeaker::setTvInput() {
    return true;
}

bool MusicCastSpeaker::toggleMute() {
    return true;
}