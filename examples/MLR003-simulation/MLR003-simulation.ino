#include <Arduino.h>
#include "lorae5.h"
#include "config_application.h"

uint8_t sizeUplink = 2;
uint8_t sizeDownlink = 0;

// uplink[0] = setpoint
// uplink[1] = temperature
uint8_t uplink[2] = {20, 20};
uint8_t downlink[50]  ={0};
bool up = true;

LORAE5 lorae5(devEUI, appEUI, appKey, devAddr, nwkSKey, appSKey); 

void setup() {


  lorae5.setup(ACTIVATION_MODE, CLASS, SPREADING_FACTOR, ADAPTIVE_DR, CONFIRMED, PORT);
  lorae5.printInfo(SEND_BY_PUSH_BUTTON, FRAME_DELAY);

  if(ACTIVATION_MODE == OTAA){
    USB_Serial.println("Join Procedure in progress...");
    while(lorae5.join() == false);
    delay(2000);
   }
}

void loop() {

  // Send "sizeUplink" bytes from "uplink" table
  if( lorae5.sendData(uplink, sizeUplink, downlink, &sizeDownlink) == true){
    uplink[0]=downlink[0];
  }

  (up==true)? uplink[1]+=1 : uplink[1]-=1;
  if ((uplink[1] == 15) or (uplink[1] == 25)) up = !up;
  
  delay(FRAME_DELAY-6000);  // Wait
}
