#include <Arduino.h>
#include "lorae5.h"
#include "config_application.h"

LoraE5 LoRaE5(devEUI, appEUI, appKey, devAddr, nwkSKey, appSKey); 

void setup() {
  LoRa_Serial.begin(9600);
  USB_Serial.begin(9600);
  while (!USB_Serial);
  USB_Serial.println("\r\n\n\n\n\n\n\n\n");
  USB_Serial.println("#######################################");
  USB_Serial.println("#### LoRaWAN Training Session #########");
  USB_Serial.println("#### Savoie Mont Blanc University #####\r\n");
  
  while(!LoRaE5.checkBoard());

  LoRaE5.setup(ACTIVATION_MODE, CLASS, SPREADING_FACTOR, ADAPTIVE_DR, CONFIRMED, PORT);
  LoRaE5.printInfo(SEND_BY_PUSH_BUTTON, FRAME_DELAY, LOW_POWER);


  if(ACTIVATION_MODE == OTAA){
    LoRaE5.setDevEUI(devEUI);
    LoRaE5.setAppEUI(appEUI);
    LoRaE5.setAppKey(appKey);
    USB_Serial.println("\r\nJoin Procedure in progress...");
    while(LoRaE5.join() == false);
    delay(3000);
   }
  
 if(ACTIVATION_MODE == ABP){
   LoRaE5.setDevAddr(devAddr);
   LoRaE5.setNwkSKey(nwkSKey);
   LoRaE5.setAppSKey(appSKey);
 }
}

void loop() {
  LoRaE5.sendMsg(STRING, "HELLO");
  //LoRaE5.sendMsg(HEX, "AA22BB33");
  delay(FRAME_DELAY-10000);
}
