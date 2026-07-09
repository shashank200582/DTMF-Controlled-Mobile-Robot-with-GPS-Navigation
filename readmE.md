# GSM DTMF Controlled Robot with GPS

## Overview

This project is a GSM-based mobile robot controlled using DTMF (Dual Tone Multi Frequency) signals from a phone call. The robot receives keypad commands through a SIM800L GSM module to move in different directions. It also includes a GPS module to send its current location via SMS when requested.

## Features

* GSM-based wireless robot control
* DTMF keypad command detection
* Forward, reverse, left, right, and stop movement
* GPS location tracking using NEO-6M
* Sends Google Maps location via SMS
* 16×2 I2C LCD for status display
* Automatic call answering

## Components Used

* Arduino Uno
* SIM800L GSM Module
* NEO-6M GPS Module
* L293D/L298N Motor Driver
* 16×2 I2C LCD Display
* DC Gear Motors
* Robot Chassis
* Battery Pack
* Jumper Wires

## DTMF Commands

| Key | Action                    |
| --- | ------------------------- |
| 2   | Forward                   |
| 8   | Reverse                   |
| 4   | Left                      |
| 6   | Right                     |
| 5   | Stop                      |
| 9   | Send GPS Location via SMS |

## Working

1. The SIM800L waits for an incoming call.
2. The robot automatically answers the call.
3. Pressing keypad buttons sends DTMF tones.
4. The Arduino decodes the received DTMF command.
5. Based on the command, the robot moves accordingly.
6. Pressing **9** reads the GPS coordinates and sends a Google Maps link through SMS.

## Connections

### SIM800L

* TX → Arduino D3
* RX → Arduino D2
* VCC → External 4V–4.2V Supply
* GND → GND

### GPS (NEO-6M)

* TX → Arduino D12
* RX → Arduino D11
* VCC → 5V
* GND → GND

### Motor Driver

* EN1 → D9
* EN2 → D6
* IN1 → D5
* IN2 → D4
* IN3 → D7
* IN4 → D8

### I2C LCD

* SDA → A4
* SCL → A5
* VCC → 5V
* GND → GND

## Applications

* Surveillance Robot
* Remote Vehicle Control
* Rescue and Emergency Assistance
* Educational Robotics Projects

## Future Improvements

* Obstacle Avoidance
* Live GPS Tracking
* Camera Integration
* Android Mobile Application
* Voice Command Support

## Author

**Shashank H**
