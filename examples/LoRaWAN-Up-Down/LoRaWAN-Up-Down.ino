#include <Arduino.h>
#include "lorae5.h"
#include "config_application.h"

uint8_t sizePayloadUp = 4;
uint8_t sizePayloadDown = 0;

uint8_t payloadUp[20] = {0x00, 0x01, 0x02, 0x03};
uint8_t payloadDown[20]  ={0};

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
    // You have received "sizePayloadDown" bytes stored in "payloadDown" table
  } 
  lorae5.awaitNextTransmission(FRAME_DELAY, SEND_BY_PUSH_BUTTON);
}