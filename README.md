[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)

# Control a Samsung Multiroom speaker with a TV remote control

You may be interested in this repo if you:

* Have a Samsung Multiroom M7 speaker (WAM250) or similar
* You have its AUX input connected to a TV optical output (via an optical-to-analog converter)
* Your TV doesn't allow you to control the volume when its sound goes to the optical output
* Want to control it via the same remote control of your TV

or you just wanna look around some C++ for ESP8266 code :)

## Description

Modern TVs can output their audio signal to optical/digital output instead of their internal speakers, so a user can use bigger speakers or e.g. a Dolby surround system. Because the output is digital and the volume is handled by another device, the TV cannot control it, assuming the other device will have its own volume controls. This means that the user will have to use two remote controls, one for the TV, one for the other device. This can be undesirable: it's annoying enough to find the TV remote in the couch while watching TV on a dark room. Unless the other device has better controls, the user might just want to use the same traditional volume controls (up/down/mute).

In the case of a Samsung Multiroom device, there's no physical remote control but a companion app that controls it via Wi-Fi. To change the volume it's then necessary to have the smartphone and use the phone's volume controls. This may require you to unlock the phone, open the app and swipe on the screen. That's too many steps if you want to quickly turn the volume down.

This project is my attempt to improve the user experience by allowing the same TV remote control to also control the Samsung speaker. It intercepts the IR signals from the TV remote control, and controls the Samsung speaker via its REST API while the TV is ignoring them.

The Samsung Multiroom speaker can accept multiple inputs: Bluetooth, various streaming services, Samsung TVs and analog AUX. Alas its control panel doesn't allow to select the AUX input; this can only be done by its companion app. To select AUX, an unused button on the LG TV remote (TV/RAD select) is repurposed to select the AUX input in case the speaker has been previously used for Bluetooth.

## How to use

### Hardware

You'll need the following:
* Wemos D1 Mini or an equivalent ESP8266-based board;
* A TSOP1738 IR receiver
* A level shifter like [Adafruit](https://www.adafruit.com/product/757)'s (because the TSOP works at 5V but the ESP officially doesn't)
* (optional) A MAX7219 and 4-digit seven-segment display
* A few passive components, see schematic:

![Schematic](hardware/board.png)

Also check out the `schematic` folder for the EAGLE schematic. I'm not providing a `.brd` file as I only made it for a perfboard and you might want/need a different technology.

### Firmware

The firmware mainly takes care of decoding IR signals and translating them to REST requests. Other features:
* Managing the WiFi connection, and will reset if connection is lost;
* Finding out the speaker on the network, and finding its IP address, using MDNS;
* Controlling a display for user feedback.

The firmware needs PlatformIO to be built.

#### Firmware configuration

Create a file `src/wifi_credentials.h` to enter your Wi-Fi crendentials. See details in `src/wifi.cpp`.

#### IR codes

The firmware is compatible with my LG smart TV remote control. It uses the NEC protocol. The following buttons are recognized:

* Volume up
* Volume down
* Mute
* TV/RAD


### Display

This firmware can also provide user feedback via four 7-segment displays, controlled by a MAX7219, if provided (i.e. the firmware should just work even if there is no display).

The displays have common cathode; the anodes should be connected to JP2 and the cathodes to JP1. Since I was using an old board I made for the displays, the pin mapping between the MAX7219 and the displays looks somewhat scrambled in the schematic. Feel free to edit it for your setup.
