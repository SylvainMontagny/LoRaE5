# 1. Learn more about LoRaWAN

Savoie Mont Blanc University provides many educationnal ressources about LoRaWAN:

:notebook: One free ebook available here: [LoRaWAN for beginners books](https://www.univ-smb.fr/lorawan/en/free-book/)

:tv: E-learning platform available here: [LoRaWAN for beginners videos](https://www.udemy.com/course/lora-lorawan-internet-of-things/?referralCode=21DED0F1021F4E261955)

:bulb: 2 days training sessions available here: [LoRaWAN and IoT Training](https://www.univ-smb.fr/lorawan/avada_portfolio/formation-distanciel/)


# 2. About this Arduino library
This library has been tested with the following boards: 
1. Legacy Arduino : **Leonardo**, **Uno**, **Zero**, **Due**.
2. ESP32 Espressif.
3. STM32 Nucleo Boards : **NUCLEO_F446**, **NUCLEO_L073**, **NUCLEO_F411RE**, **NUCLEO_L433RC-P**, **NUCLEO_WL55JC1**, **NUCLEO_G031K8**, **NUCLEO_F746ZG**.
4. Seeed Studio: **Wio Terminal**.

:bulb: If you've tested this library with any other boards than the one listed we would be happy to have your feedback :
- Report an [issue on github](https://github.com/SylvainMontagny/LoRaE5/issues/new) (label **"Test board failure"**) in case of **failure**.
- Report an [issue on github](https://github.com/SylvainMontagny/LoRaE5/issues/new) (label **"Test board sucess"**) in case of **success**.

This Arduino Library communicates with the [Grove Wio E5 LoRa-E5](https://wiki.seeedstudio.com/Grove_LoRa_E5_New_Version/). The application configuration is done thanks to a unique file called **config_application.h**. Here are the possible configuration :

| #define           	| Possible values      	|  Behavior                                                    	|
|----------------------	|--------------------	|---------------------------------------------------------------|
| REGION                | EU868 or US915        |  Region                                                       |
| ACTIVATION_MODE      	| ABP or OTAA        	|  Activation Method                              	            |
| CLASS                	| CLASS_A or CLASS_C 	|  Class                                             	        |
| SPREADING_FACTOR     	| Number [7;12]      	|  7 = SF7, 8 = SF8, ..., 12 = SF12                             |
| ADAPTIVE_DR          	| true or false      	|  Enable Adaptive Data Rate (if true)                         	|
| CONFIRMED            	| true or false      	|  Frame Confirmed (if true) OR Frame Unconfirmed (if false)   	|
| PORT_UP               | Number [0;255]     	|  Application Port number                                     	|
| SEND_BY_PUSH_BUTTON  	| true or false      	|  Sending method (Time or Push Button)                        	|
| FRAME_DELAY          	| Time (ms)         	|  Time between 2 frames (Minimum 7000)                        	|


The board configuration is done thanks to a unique file called **config_board.h**. The default configuration is:
- **Serial** for debug
- **Serial1** for the module connection
 
 
## 2.1. Arduino IDE Installation

- Install Arduino IDE from the [Arduino Website](https://www.arduino.cc/).
- In Arduino IDE, go to **tools > Manage Librairies...**.
- Write in the section **Filter your search...** : "lora e5".
- Choose the last version and then click on **Install**.


## 2.2. Arduino Hardware

This library works with any boards with two serial ports (or one USB + one Serial). 
- Debug_Serial (**Serial** by default): Connection to computer: 115200 baud. 
- LoRa_Serial (**Serial1** by default): Serial link for the communication between the Arduino MCU and the LoRa-E5 LoRaWAN module. 


## 2.3. How to use this library

- Set up the LoRaWAN parameters in **config_application.h** file. If you use ABP, you need to configure devAdddr, nwkSKey and appSKey. If you use OTAA you need to configure devEUI, appEUI and appKey.
- Set up the hardware parameters in **config_board.h** file. Choose the right setting for your board (Serial or Pins).
- Register your Device on a Network Server (TTN, Actility, LORIOT, ...)
- Open the Serial Monitor in the arduino IDE to see the logs. (:warning: 115200 baud).


## 2.4. How to add sensors

- Any sensors can be added to the free I2C bus, analog pins and GPIOs.


## 2.5. Best practices

- It is recommended to opt for OTAA over ABP for the Activation Mode.