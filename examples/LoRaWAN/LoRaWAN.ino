#include <arduino.h>
#include <lorae5.h>
#include "config_application.h"


LoraE5 loraE5(SPREADING_FACTOR, ADAPTIVE_DR, CONFIRMED, PORT, SEND_BY_PUSH_BUTTON, FRAME_DELAY, PAYLOAD_HELLO, ACTIVATION_MODE, devEUI, devAddr, nwkSKey, appSKey, appKey, appEUI); // init LoRaE5

void setup() {
  Serial1.begin(9600);
  Serial.begin(9600);
  while (!Serial);        // wait for serial port to connect.
  Serial.println("\n\n\n\n\n\n\n\n\n\n\r");
  Serial.println("# LoRaWAN Training Session #");
  Serial.println("######## LoRa E5 HT ########");
  
  loraE5.setDR(SPREADING_FACTOR);
  loraE5.setADR(ADAPTIVE_DR);

  if(ACTIVATION_MODE == OTAA){
    loraE5.setMode(OTAA);
    loraE5.setDevEUI(devEUI);
    loraE5.setAppEUI(appEUI);
    loraE5.setAppKey(appKey);
    loraE5.join();
  }
  
 if(ACTIVATION_MODE == ABP){
   loraE5.setMode(ABP);
   loraE5.setDevAddr(devAddr);
   loraE5.setNwkSKey(nwkSKey);
   loraE5.setAppSKey(appSKey);
 }
  
}

void loop() {
  loraE5.sendMsg(CONFIRMED,"HELLO");
   delay(10000);
}
