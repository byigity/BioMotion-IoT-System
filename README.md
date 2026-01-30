# BioMotion
Work in Progress ESP32 IoT Project 

BioMotion: Wearable IoT Health Monitoring System
BioMotion is an integrated IoT solution designed to provide real-time biometric data tracking through a wearable form factor. This project focuses on the intersection of Embedded Systems, IoT architecture, and Object-Oriented Programming (OOP) to deliver a modular and scalable health monitoring platform.




Key Features
Real-time Biometric Tracking: Continuous monitoring of pulse rate and body temperature using high-precision sensors.

Object-Oriented Firmware: Developed in C++ with a focus on modularity, allowing easy integration of additional sensors in the future.

Robust Hardware Design: Transitioned from initial breadboard prototyping to a permanent, soldered perfboard circuit for enhanced durability and signal integrity.


Low Power Consumption: Optimized for the ESP32 microcontroller to ensure efficient energy usage for wearable applications.


🛠️ Tech Stack & Components

Microcontroller: ESP32 (Wi-Fi & Bluetooth enabled).



Programming Language: C++ (with OOP principles).



Sensors: Pulse Sensor, Temperature Sensor (NTC/DS18B20), Motion (Gyro) Sensor MPU6050.


Tools: Arduino IDE / PlatformIO, Proteus (for circuit simulation).


Project Structure
/src: Contains the firmware source code (.cpp, .h files).


/hardware: Circuit schematics and component datasheets.


/docs: Project documentation and performance analysis reports.

Installation & Usage
Clone the Repository:

Bash

git clone https://github.com/byigity/BioMotion-IoT-System.git

Hardware Setup: Follow the schematic provided in the /hardware folder to connect the sensors to the ESP32.


Upload Firmware: Open the /src folder in your preferred IDE and upload it to your ESP32 board.

Future Improvements
Integration of a mobile application for real-time data visualization via MQTT.

Integration of an Artificial Intelligince API. Let that AI determine what is wrong according to the user's health data.

Implementation of an MPU6050 accelerometer for fall detection and activity tracking.
