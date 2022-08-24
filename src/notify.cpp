// #include <Arduino.h>
// #include "speaker.h"
// #include "display.h"
// #include "config.h"

// void animate(char ch) {
//   String text = "    ";
//   int dashPos = (millis() / 600) % 4;
//   text[dashPos] = ch;
//   displayText(text);
// }

// void notifyNoWifi() {
//   animate('-');
// }

// void notifyNoSpeaker() {
//   animate('o');
// }

// void notifySpeakerAddress(String address) {
//   int lastDotPosition = address.lastIndexOf(".");
//   if (lastDotPosition == -1) {
//     displayText("S....");
//     return;
//   }

//   String lastOctet = address.substring(lastDotPosition + 1);
//   displayText("S." + lastOctet);
// }

// void notifyIR() {
//   displayText(" .");
// }

// void notifyVolume(Speaker &speaker, int volume, bool wasSet) {
//   char text[6]; // 4 digits + 1 decimal point + 1 set confirmation dot
//   const char *setDot = wasSet ? "." : "";
// #ifdef SPEAKER_MULTIROOM
//   snprintf(text, 6, " %2d%s", volume, setDot);
// #else
//   /*
//    * MusicCast speakers may show the volume in dB, with 0dB beng the max and
//    * every step is a 0.5dB increment. Not sure if all speakers follow the
//    * same convention; the Yamaha R-N602 does.
//    */
//   float decibel = ((float)volume - speaker.getConfiguration().maxVolume) / 2;
//   snprintf(text, 6, "% 4.1f%s", decibel, setDot);
// #endif
//   Serial.printf("notifyVolume: '%s'", text);
//   displayText(text);
// }

// void notifyFail(Result result) {
//   switch(result) {
//   case Result::OK:
//     displayText("  OH"); // error: no error?
//     break;
//   case Result::ERROR_NO_SPEAKER_ADDRESS:
//     displayText("  EA");
//     break;
//   case Result::ERROR_HTTP_TIMEOUT:
//     displayText("  Et");
//     break;
//   case Result::ERROR_HTTP_NON_OK_RESPONSE:
//     displayText("  Eh");
//     break;
//   case Result::ERROR_PARSE_FAILED:
//     displayText("  EP");
//     break;
//   default:
//     displayText("  E_");
//   }
// }

// void notifyVolumeGetSuccess(Speaker &speaker, int volume) {
//   notifyVolume(speaker, volume, false);
// }
// void notifyVolumeSetSuccess(Speaker &speaker, int volume) {
//   notifyVolume(speaker, volume, true);
// }


// void notifyTv(bool wasSet) {
//   displayText(wasSet ? " tv." : " tv");
// }

// void notifyAuxGetSuccess(bool isAux) {
//   notifyTv(false);
// }
// void notifyAuxSetSuccess(bool isAux) {
//   notifyTv(true);
// }

// void notifyMuteGetSuccess() {
//   displayText("mu");
// }
// void notifyMuteSetSuccess(bool isMute) {
//   displayText(isMute ? "mute." : "soun.");
// }
