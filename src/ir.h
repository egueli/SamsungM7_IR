#pragma once

void setupIR();
void loopIR();

// The functions below are callbacks. Their definition must be in a file other than ir.cpp.
void onVolumeUp();
void onVolumeDown();
void onTvRad();
void onMute();