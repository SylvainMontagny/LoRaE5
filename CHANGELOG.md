# Changelog
#### File: LoRaE5
#### Author: Sylvain Montagny & Dorian Desserey


## Knows limitation
- Only tested on EU868.
- Tested compatibility only with "Leonardo" and "Zero" Arduino boards.
- "LoRaWAN modem is busy" message may occurs for Class C/ABP.


## Version: V1.X.X | 2024-03-XX

### Added
- Implement CLASS_C mode.
- Implement "setClass" method.
- Implement "getClass" method.
- Display the downlink C frame to the user.

### Modified
- Update of "printInfo".


## Version: V1.1.1 | 2024-03-08

### Added
- Implemention of a method keyboard Input "t" for Uplink Initiation.
- Improved program stability by fixing "Length error 0" error.

### Modified
- Update the access specifier of the displayPayloadUp function from public to private.
- Update of all blank function (func() by func(void)).
- Error correction of uplink payload value sent to server.

### Removed
- Unused “sendMsg” function.


## Version: V1.1.0 | 2024-03-07

### Added
- Implement a new method named “getPortDown” .
- Implement an overloaded version of the “sendData” method.

### Modified
- Replace all occurrences of "port" with "portUp”.
- Replace all occurrences of "getPort" with "getPortUp".
- Replace all occurrences of "setPort" with "setPortUp".
- Update both versions of the sendData methods to use the displayPayloadUp function for displaying the "Up" payload.


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
- Optimize energy consumption and reliability

#### Spreading Factor (SF):
- Controls how wide you "spread" your signal
