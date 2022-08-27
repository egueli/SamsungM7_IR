#include <Arduino.h>
#include <asyncHTTPrequest.h>

#include "http_response.h"
#include "http_wait.h"
#include "serial.h"

const unsigned long kHttpTimeout = 5000;

Result parseValueInXML(String document, String &output, String openTag, String closeTag) {
  int openIndex = document.indexOf(openTag);
  if (openIndex == -1) {
    return Result::ERROR_PARSE_FAILED;
  }

  int closeIndex = document.indexOf(closeTag, openIndex);
  if (closeIndex == -1) {
    return Result::ERROR_PARSE_FAILED;
  }

  output = document.substring(openIndex + openTag.length(), closeIndex);
  return Result::OK;
}

Result waitForHttpOkResponse(asyncHTTPrequest &request) {
  unsigned long startAt = millis();

  while (request.readyState() != 4) {
    yield();
    onHttpWait();
    if (millis() - startAt > kHttpTimeout) {
      USE_SERIAL.println("[HTTP] timeout");
      request.abort();
      return Result::ERROR_HTTP_TIMEOUT;
    }
  }
  
  int httpCode = request.responseHTTPcode();
  return handleHttpOkResponse(httpCode);
}

Result handleHttpOkResponse(const int httpCode) {
  if (httpCode != 200 /* OK */) {
    USE_SERIAL.printf("[HTTP] GET not OK, code: %d\n", httpCode);
    return Result::ERROR_HTTP_NON_OK_RESPONSE;
  }

  return Result::OK;
}

Result getValueFromHttp(asyncHTTPrequest &request, String &output, String openTag, String closeTag) {
  RETURN_IF_ERROR(waitForHttpOkResponse(request))
  
  String body = request.responseText();
  return parseValueInXML(body, output, openTag, closeTag);
}