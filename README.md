# 1. Learn more about LoRaWAN
Savoie Mont Blanc University provides many educationnal ressources about LoRaWAN:

:notebook: One free ebook available here: [LoRaWAN for beginers books](https://www.univ-smb.fr/lorawan/en/free-book/)

:tv: E-learning platform available here: [LoRaWAN for beginers videos](https://www.udemy.com/course/lora-lorawan-internet-of-things/?referralCode=21DED0F1021F4E261955)

:bulb: 2 days training sessions available here: [LoRaWAN and IoT Training](https://www.univ-smb.fr/lorawan/avada_portfolio/formation-distanciel/)

# 2. About this Arduino library
This Arduino Library communicates with the [Grove Wio E5 LoRa-E5](https://wiki.seeedstudio.com/Grove_LoRa_E5_New_Version/). The configuration is made thanks to a unique file called config_application. Here are the possible configuration :

| #define           	| Possible values      	|  Behavior                                                    	|
|----------------------	|--------------------	|---------------------------------------------------------------|
| ACTIVATION_MODE      	| ABP or OTAA        	|  Activation Method                              	            |
| CLASS                	| CLASS_A or CLASS_C 	|  Class                                             	        |
| SPREADING_FACTOR     	| Number [7;12]      	|  7 = SF7, 8 = SF8, ..., 12 = SF12                             |
| ADAPTIVE_DR          	| true or false      	|  Enable Adaptive Data Rate (if true)                         	|
| CONFIRMED            	| true or false      	|  Frame Confirmed (if true) OR Frame Unconfirmed (if false)   	|
| PORT                 	| Number [0;255]     	|  Application Port number                                     	|
| SEND_BY_PUSH_BUTTON  	| true or false      	|  Sending method (Time or Push Button)                        	|
| FRAME_DELAY          	| Time in ms         	|  Time between 2 frames (Minimum 7000)                        	|


This library has only been tested with an **Aduino Leonardo board**.


## 2.1. Arduino IDE Installation
- Install Arduino IDE from the [Arduino Website](https://www.arduino.cc/)
- Download the code of this library as a Zip file: [Link here](https://github.com/SylvainMontagny/LoRaE5/archive/refs/heads/main.zip) 
- In Arduino IDE, go to **sketch > include library > Add ZIP library**. Select the downloaded Zip file.
- Open the example project: **File > Example > LoRa-E5 Library > LoRaWAN**

## 2.2. Arduino Hardware
This library works with any Arduino boards with two serial ports (or one USB + one Serial). It has been tested on a Leonardo board with: 
- USB_Serial: Connection to computer: 115200 baud. 
- LoRa_Serial: Serial link for the communication between the Arduino MCU and the LoRa-E5 LoRaWAN module. 

Your LoRa-E5 module need to be connected to the RX-TX arduino header pin.

## 2.3. How to use this library
- Set up the parameters in **config_application.h** file. If you use ABP, you need to configure devAdddr, nwkSKey and appSKey. If you use OTAA you need to configure devEUI, appEUI and appKey.
- Set up your Gateway or use a public coverage.
- Register your Device on a Network Server (TTN, Actility, LORIOT, ...)
- Open the Serial Monitor in the arduino IDE to see the logs. /!\ 115200 baud.


## 2.4. How to add sensors
- Any sensors can be added to the free I2C bus, analog pins and GPIOs.


 