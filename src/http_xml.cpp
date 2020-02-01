#include <Arduino.h>
#include <ESP8266HTTPClient.h>

#include "serial.h"

bool parseValueInXML(String document, String &output, String openTag, String closeTag) {
  int openIndex = document.indexOf(openTag);
  if (openIndex == -1) {
    return false;
  }

  int closeIndex = document.indexOf(closeTag, openIndex);
  if (closeIndex == -1) {
    return false;
  }

  output = document.substring(openIndex + openTag.length(), closeIndex);
  return true;
}

bool getValueFromHttp(HTTPClient &http, String &output, String openTag, String closeTag) {
  int httpCode = http.GET();

  if (httpCode <= 0) {
    USE_SERIAL.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    return false;
  }

  if (httpCode != HTTP_CODE_OK) {
    USE_SERIAL.printf("[HTTP] GET not OK, code: %d\n", httpCode);
    return false;
  }

  String body = http.getString();
  bool success = parseValueInXML(body, output, openTag, closeTag);
  if (!success) {
    USE_SERIAL.println("[HTTP] unable to read volume");
    return false;
  }

  return true;
}