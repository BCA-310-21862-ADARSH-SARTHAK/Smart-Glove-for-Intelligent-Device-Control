\# Smart Glove for Intelligent Device Control

\## Overview

This project presents a Smart Glove capable of controlling intelligent devices using hand gestures. The developed wearable system uses flex sensors mounted on a glove to recognize finger movements and convert them into control commands.

The generated commands are transmitted wirelessly using ESP-NOW between two ESP32 microcontrollers. A Python communication program transfers these commands to an Industrial PC through SSH, where a C++ robot controller running inside Docker executes the corresponding movements on the Syncro5 Collaborative Robot.

The project demonstrates real-time human-machine interaction for industrial automation.

\---

\## Features

\- Flex sensor based gesture recognition

\- Wireless communication using ESP-NOW

\- Real-time command transmission

\- SSH based communication

\- Docker based robot control

\- Syncro5 collaborative robot integration

\- Feetech gripper control

\- Low latency communication

\- Safe emergency stop mechanism

\---

\## Hardware Used

\- Syncro5 Collaborative Robot

\- Feetech Gripper

\- 2 × ESP32 Development Boards

\- 5 × Flex Sensors (2.2 inch)

\- Leather Glove

\- Lithium-ion Batteries

\- Charging Module

\- 10kΩ Resistors

\- USB Cable

\---

\## Software Used

\- Ubuntu 22.04 LTS

\- Arduino IDE

\- Python 3.10

\- C++

\- Docker

\- SSH

\- CMake

\- GCC/G++

\- ESP-NOW

\---

\## Communication Flow

Flex Sensors

↓

ESP32 Transmitter

↓

ESP-NOW

↓

ESP32 Receiver

↓

USB Serial

↓

Python

↓

SSH

↓

Industrial PC

↓

Docker

↓

C++ Robot Controller

↓

Syncro5 Cobot

\---

\## Folder Structure

```text

Documentation/

Hardware/

Images/

Screenshots/

Software/

Source\_Code/

Videos/

```

\---

\## Author

\*\*Adarsh Sarthak\*\*

BCA Final Year Project

Aryabhatta Knowledge University

CIMAGE Professional College

\---

\## License

Educational Use Only.
