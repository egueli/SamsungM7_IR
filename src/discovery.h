#pragma once

void setupDiscovery();

void loopDiscovery();

// The functions below are callbacks. Their definition must be in a file other than ir.cpp.
void onDiscoveryFinished(String address);