#1. Learn more about LoRaWAN !
* [LoRaWAN for beginers books](https://www.univ-smb.fr/lorawan/en/free-book/)
* [LoRaWAN for beginers videos](https://www.udemy.com/course/lora-lorawan-internet-of-things/?referralCode=21DED0F1021F4E261955)

#2. LoRaE5 Arduino library
This Arduino Library communicates with the LoRa_E5 LoRaWAN module(Seed Studio).
This library has only been tested with an Aduino Leonardo board.

## Installation on the Arduino 2 IDE
- Download the Zip file from this github
- In Arduino IDE, go to sketch > include library > Add ZIP library. Select the downloaded Zip file.
- Open the example project: File > Example > LoRa-E5 Library > LoRaWAN

## How to use it
- Set up the parameters in "config_application.h" file
- Set up your Gateway or use a public coverage.
- Register your Device on a Network Server (TTN, Actility, LORIOT, ...)
- Open the Serial Monitor to see the logs 

USB_Serial: Serial link for Logs to the PC. 
LoRa_Serial: Serial link for the communication between the AVR MCU and the LoRa-E5 LoRaWAN module. 

## How to add sensors
- Any sensors can be added to the free I2C bus, analog pins and GPIOs.


 