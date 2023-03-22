# LoRaE5 Arduino library
This Arduino Library communicates with the LoRa_E5 LoRaWAN module(Seed Studio).
This library has only been tested with an Aduino Leonardo board.

## Installation on the Arduino 2 IDE
- Download the Zip file from this github
- In Arduino IDE, go to sketch > include library > Add ZIP library. Select the downloaded Zip file.
- Open the example project: File > Example > LoRa-E5 Library > LoRaWAN

## How to use it
- Set up the parameters in "config_application.h" file
- Open the Serial Monitor to see the logs 

Serial: Serial link for Logs to the PC. 
Serial1: Serial link for the communication between the AVR MCU and the LoRa E5 LoRaWAN module. 

## How to add sensors
- Any sensors can be added to the free I2C bus, analog pins and GPIOs.


 