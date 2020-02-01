#pragma once

#include <Arduino.h>
#include <ESP8266HTTPClient.h>

bool parseValueInXML(String document, String &output, String openTag, String closeTag);
bool getValueFromHttp(HTTPClient &http, String &output, String openTag, String closeTag);