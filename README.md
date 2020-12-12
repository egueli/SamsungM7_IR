[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)

# Control a network-enabled speaker with a TV remote control

You may be interested in this project if:

* You have a speaker or Hi-Fi set using the Yamaha MusicCast or Samsung Multiroom protocols
* You have one of its inputs connected to a TV digital output
* You want to control the speaker via the same remote control of your TV
* Your TV disables volume control when the audio goes to the digital output
* Your TV remote control is based on infrared (IR) signals

## Description

Modern TVs can output their audio signal to digital output (optical/TOSLINK or RCA) instead of their internal speakers, so a user can use bigger speakers or e.g. a Dolby surround system. When doing so, a TV may ignore the volume controls because it expects the audio device to have its own volume control. The user has then to use two remote controls, one for the TV, one for the other device. This can be undesirable: it's annoying enough to find the TV remote in the couch while watching TV on a dark room. The user might just want to use the same traditional volume controls (up/down/mute) and use the other remote control for other tasks.

In the case of a Samsung Multiroom device, there's no physical remote control but a companion app that controls it via Wi-Fi. To change the volume it's then necessary to have a smartphone nearby and use the phone's volume controls. This may require you to unlock the phone, open the app and swipe on the screen. That's too many steps if you want to quickly turn the volume down.

This project is my attempt to improve the user experience by allowing the same TV remote control to also control a network-enabled speaker. It intercepts the IR signals from the TV remote control, and controls the speaker via its REST API while the TV is ignoring them.

The speaker may accept multiple inputs (Bluetooth, tuner, phono etc.), and it may be necessary to set the right input before using it for the TV. This device can listen to the press of an unused button on the TV remote (e.g. TV/radio select) to select the TV input.

## Features

* It listens to IR signals and sends HTTP requests.
* It supports IR remote controls using the NEC IR protocol.
* It supports speakers or amplifiers using Samsung Multiroom and Yamaha MusicCast protocols.
* WiFi watchdog: resets itself if local wi-fi connection is lost;
* MDNS/Bonjour: auto-discovery of the speaker IP address on the network; optionally a name can be provided to connect to a specific speaker;
* Speaker watchdog: checks periodically that the speaker is still available at that address, and reset itself otherwise;
* Volume and status output to a 7-segment display.

## How to set up

### Hardware

You'll need the following:
* Wemos D1 Mini or an equivalent ESP8266-based board;
* A TSOP1738 IR receiver
* A level shifter like [Adafruit](https://www.adafruit.com/product/757)'s (because the TSOP works at 5V but the ESP officially doesn't)
* (optional) A MAX7219 and 4-digit seven-segment display
* A few passive components, see schematic:

![Schematic](hardware/board.png)

Also check out the `schematic` folder for the EAGLE schematic. I'm not providing a `.brd` file as I only made it for a perfboard and you might want/need a different technology.

### Building

The firmware is based on PlatformIO. A little configuration (see below) and a `pio run -t upload` should be enough to download the required libraries and build the firmware.

### Configuration

Create a file `src/wifi_credentials.h` to enter your Wi-Fi crendentials. See details in `src/wifi.cpp`.

In `config.h` file you can configure the IR commands and speaker protocol to use.

### IR codes

The firmware is compatible with my LG smart TV remote control. The following buttons are recognized:

* Volume up
* Volume down
* Mute
* TV/Radio

To use a different remote control, open the serial monitor and look at the IR codes that are sent when you press the desired buttons. Then change the constants in `config.h` accordingly.

### Display

This firmware can also provide user feedback via four 7-segment displays, controlled by a MAX7219, if provided (i.e. the firmware should just work even if there is no display).

The displays have common cathode; the anodes should be connected to JP2 and the cathodes to JP1. Since I was using an old board I made for the displays, the pin mapping between the MAX7219 and the displays looks somewhat scrambled in the schematic. Feel free to edit it for your setup.
