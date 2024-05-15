# Library Changelog
#### Author: Sylvain Montagny & Dorian Desserey


## Knows limitation
* Tested on "EU868" & "US915" only.
* "LoRaWAN modem is busy" message may occurs for Class C/ABP.

## Version: V3.0.1 | 2024-05-24

### Added
- Possibility of selecting all Regions supported by the loraE5 module.
- UART pin list for known boards.

## Version: V3.0.0 | 2024-05-14

### Added
- A “config_board.h” header file has been added to to manage the serial configuration (Debug and LoRa Module connection).
- Implement a setup_hardware() method.

### Modified
- "USB_Serial" has been replaced by "Debug_Serial".
- Renamed setup() method to setup_lorawan().


## Version: V2.0.0 | 2024-04-22

### Added
- Implement a awaitForDownlinkClass_A() & awaitForDownlinkClass_C() method.
- Implement a getPortDown() method.
- Implement a getPayloadDown() method.
- Implement a getClassDownlink() method.
- Implement a sleep() method.
- Implement a setFrequencyBand() method.
- Define a new parameter "REGION".
- "LoRaWAN_Class_A.ino", "LoRaWAN_Class_C" & "LoRaWAN_State-Machine.ino".
- Doxygen standard written for all functions.
- Ultra-low power consumption of the LoRaE5 module when sleep() function is called.

### Modified
- Rename "displayPayloadUp()" by "sendPayloadUp()".
- In sendData() functions with 2 parameters, rename "done = 1" by "done = true".
- Move "Send_By_Push_Button" variable from each function parameters to setup() function.
- Arrange functions in header (private or public).
- getSetSF() & setSF() to match with US915 region.
- Add "REGION" parameter in setup.

### Removed
- SendData() function with 4 parameters.


## Version: V1.2.0 | 2024-03-22

### Added
- Implement a CLASS_C mode.
- Implement a setClass() method.
- Implement a getClass() method.
- Display the downlink C frame to the user.

### Modified
- Update of printInfo().


## Version: V1.1.1 | 2024-03-08

### Added
- Implement a method keyboard Input "t" for Uplink Initiation.
- Improved program stability by fixing "Length error 0" error.

### Modified
- Update the access specifier of the displayPayloadUp function from public to private.
- Update of all blank function (func() by func(void)).
- Error correction of uplink payload value sent to server.

### Removed
- Unused sendMsg() function.


## Version: V1.1.0 | 2024-03-07

### Added
- Implement a new method named getPortDown().
- Implement an overloaded version of the sendData() method.

### Modified
- Replace all occurrences of "port" with "portUp”.
- Replace all occurrences of getPort() with getPortUp().
- Replace all occurrences of setPort() with setPortUp().
- Update both versions of the sendData() methods to use the displayPayloadUp() function for displaying the "Up" payload.


## Version: V1.0.0 | 2024-02-19

### First version features:

#### Send and receive frames (Uplink/Downlink):
- Uplink : Send data to the network server.
- Downlink : Receive data from the network server.

#### Choose your transmission mode (ABP/OTAA):
- ABP : Activation by Personalization.
- OTAA : Over-the-Air Activation.

#### Supported class:
- Class A, suitable for battery-powered devices.

#### Differents types of frames (Confirmed/Unconfirmed):
- Confirmed : The network server acknowledges messages.
- Unconfirmed : The network server does not acknowledge messages.

#### Adaptive Data Rate activation choice (ON/OFF):
- Adjusts the data transmission rate.
- Optimize energy consumption and reliability.

#### Spreading Factor (SF):
- Controls how wide you "spread" your signal.
