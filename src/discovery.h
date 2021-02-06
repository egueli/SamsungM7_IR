#pragma once

#include "speaker.h"

void setupDiscovery(Speaker &speaker);

void loopDiscovery();

// The functions below are callbacks. Their definition must be in a file other than ir.cpp.
void onDiscoveryFinished(String address);