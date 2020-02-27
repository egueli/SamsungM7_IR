#include <Arduino.h>
#include <ESP8266WiFi.h>

#include "wifi.h"
#include "wifi_credentials.h"
#include "serial.h"
#include "pins.h"

const unsigned long kWifiConnectTimeout = 20000;

unsigned long wifiConnectStart;

boolean isWifiConnected() {
  return WiFi.status() == WL_CONNECTED;
}

void setupWifi() {
  pinMode(kWifiLedPin, OUTPUT);
  
  for (uint8_t t = 4; t > 0; t--) {
    USE_SERIAL.printf("[SETUP] WAIT %d...\n", t);
    USE_SERIAL.flush();
    delay(1000);
  }

  wifiConnectStart = millis();
  WiFi.setAutoConnect(true);
  // WiFi credentials must not be part of git repo.
  // Git will ignore any file starting with WiFi_credentials.
  // Create a tab called "WiFiCredentials.h" with the following:
  //const char* kWifiSsid = "your-wifi-name";
  //const char* kWifiPassword = "your-wifi-password";
  WiFi.begin(kWifiSsid, kWifiPassword);
}

void loopWifi() {
  digitalWrite(kWifiLedPin, isWifiConnected() ? HIGH : LOW);

  if (!isWifiConnected() && millis() > wifiConnectStart + kWifiConnectTimeout) {
    USE_SERIAL.println("wifi disconnected or connect timeout; restarting");
    ESP.restart();
  }
}
