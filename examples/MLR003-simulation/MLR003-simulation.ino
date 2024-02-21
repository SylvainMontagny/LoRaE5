#include <Arduino.h>
#include "lorae5.h"
#include "config_application.h"

uint8_t sizePayloadUp = 2;
uint8_t sizePayloadDown = 0;

// payloadUp[0] = setpoint
// payloadUp[1] = temperature
uint8_t payloadUp[2] = {20, 20};
uint8_t payloadDown[50]  ={0};
bool up = true;

LORAE5 lorae5(devEUI, appEUI, appKey, devAddr, nwkSKey, appSKey); 

/***********************************************************************/
/* Please see README page on https://github.com/SylvainMontagny/LoRaE5 */
/***********************************************************************/

void setup() {
  lorae5.setup(ACTIVATION_MODE, CLASS, SPREADING_FACTOR, ADAPTIVE_DR, CONFIRMED, PORT_UP);
  lorae5.printInfo(SEND_BY_PUSH_BUTTON, FRAME_DELAY);

  if(ACTIVATION_MODE == OTAA){
    USB_Serial.println("Join Procedure in progress...");
    while(lorae5.join() == false);
    delay(2000);
   }
}

void loop() {
  // Send "sizePayloadUp" bytes from "payloadUp" table
  if( lorae5.sendData(payloadUp, sizePayloadUp, payloadDown, &sizePayloadDown) == true){
    payloadUp[0]=payloadDown[0];
  }

  (up==true)? payloadUp[1]+=1 : payloadUp[1]-=1;
  if ((payloadUp[1] == 15) or (payloadUp[1] == 25)) up = !up;
  
  lorae5.awaitNextTransmission(FRAME_DELAY, SEND_BY_PUSH_BUTTON);
}
