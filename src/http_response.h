#pragma once

#include <Arduino.h>
#include <asyncHTTPrequest.h>
#include "error.h"

Result waitForHttpOkResponse(asyncHTTPrequest &request);
Result parseValueInXML(String document, String &output, String openTag, String closeTag);
Result getValueFromHttp(asyncHTTPrequest &request, String &output, String openTag, String closeTag);