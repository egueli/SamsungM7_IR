#include <Arduino.h>
#include <asyncHTTPrequest.h>

#include "http_xml.h"
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

bool getValueFromHttp(asyncHTTPrequest &request, String &output, String openTag, String closeTag) {
  while (request.readyState() != 4) {
    yield();
    onHttpWait();
  }
  
  int httpCode = request.responseHTTPcode();

  if (httpCode <= 0) {
    USE_SERIAL.printf("[HTTP] GET... failed, error: %d\n", httpCode);
    return false;
  }

  if (httpCode != 200 /* OK */) {
    USE_SERIAL.printf("[HTTP] GET not OK, code: %d\n", httpCode);
    return false;
  }

  String body = request.responseText();
  bool success = parseValueInXML(body, output, openTag, closeTag);
  if (!success) {
    USE_SERIAL.println("[HTTP] unable to read volume");
    return false;
  }

  return true;
}