#pragma once

#include <Arduino.h>
#include <asyncHTTPrequest.h>

bool parseValueInXML(String document, String &output, String openTag, String closeTag);
bool getValueFromHttp(asyncHTTPrequest &request, String &output, String openTag, String closeTag);

