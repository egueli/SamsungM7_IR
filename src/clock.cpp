#include "clock.h"
#include "wifi.h"
#include <WiFiUdp.h>
#include <TimeLib.h>
#include <ESP8266WiFi.h>
#include "config.h"
#include "display.h"

/** Local port to listen for UDP packets */
unsigned const int kLocalPort = 8888; 

/** NTP Server hostname */
static const char *kNtpServerName = "pool.ntp.org";

const int kNtpPacketSize = 48; // NTP time is in the first 48 bytes of message
byte packetBuffer[kNtpPacketSize]; //buffer to hold incoming & outgoing packets


WiFiUDP Udp;
bool clockInitialized;

void initializeClock();
time_t getNtpTime();
void sendNTPpacket(IPAddress &address);

void setupClock() {
  // do nothing; we must wait for WiFi and that only happens during sketch loop
}

void loopClock() {
  if (!isWifiConnected()) {
    return;
  }

  if (!clockInitialized) {
    initializeClock();
    clockInitialized = true;
  }

  if (timeStatus() == timeNotSet) {
    setDisplayBackground(String("--.--"));
  } else {
    char text[5];
    snprintf(text, 6, "%02d.%02d", hour(), minute());
    setDisplayBackground(String(text));
  }
}

void initializeClock() {
  Udp.begin(kLocalPort);
  setSyncProvider(getNtpTime);
}

time_t getNtpTime()
{
  IPAddress ntpServerIP; // NTP server's ip address

  while (Udp.parsePacket() > 0) ; // discard any previously received packets
  Serial.println("Transmit NTP Request");
  // get a random server from the pool
  WiFi.hostByName(kNtpServerName, ntpServerIP);
  Serial.print(kNtpServerName);
  Serial.print(": ");
  Serial.println(ntpServerIP);
  sendNTPpacket(ntpServerIP);
  uint32_t beginWait = millis();
  while (millis() - beginWait < 5000) {
    int size = Udp.parsePacket();
    if (size >= kNtpPacketSize) {
      Serial.println("Receive NTP Response");
      Udp.read(packetBuffer, kNtpPacketSize);  // read packet into the buffer
      unsigned long secsSince1900;
      // convert four bytes starting at location 40 to a long integer
      secsSince1900 =  (unsigned long)packetBuffer[40] << 24;
      secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
      secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
      secsSince1900 |= (unsigned long)packetBuffer[43];
      return secsSince1900 - 2208988800UL + kTimeZone * SECS_PER_HOUR;
    }
  }
  Serial.println("No NTP Response :-(");
  return 0; // return 0 if unable to get the time
}

// send an NTP request to the time server at the given address
void sendNTPpacket(IPAddress &address)
{
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, kNtpPacketSize);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12] = 49;
  packetBuffer[13] = 0x4E;
  packetBuffer[14] = 49;
  packetBuffer[15] = 52;
  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  Udp.beginPacket(address, 123); //NTP requests are to port 123
  Udp.write(packetBuffer, kNtpPacketSize);
  Udp.endPacket();
}
