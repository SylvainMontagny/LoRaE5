#include <Arduino.h>
#include "lorae5.h"
#include "config_application.h"
#include "config_board.h"

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
  lorae5.setup_hardware(&Debug_Serial, &LoRa_Serial);
  lorae5.setup_lorawan(REGION, ACTIVATION_MODE, CLASS, SPREADING_FACTOR, ADAPTIVE_DR, CONFIRMED, PORT_UP, SEND_BY_PUSH_BUTTON, FRAME_DELAY);
  lorae5.printInfo();

  if(ACTIVATION_MODE == OTAA){
    Debug_Serial.println("Join Procedure in progress...");
    while(lorae5.join() == false);
    delay(2000);
   }
}

void loop(){
  // Send data
  lorae5.sendData(payloadUp, sizePayloadUp);

  // Check downlink and call processDownlink()
  if ( lorae5.awaitForDownlinkClass_A(payloadDown, &sizePayloadDown) == RET_DOWNLINK ){
    payloadUp[0]=payloadDown[0];
    processDownlink();
  }

  (up==true)? payloadUp[1]+=1 : payloadUp[1]-=1;
  if ((payloadUp[1] == 15) or (payloadUp[1] == 25)) up = !up;

  // Wait for the next transmission
  lorae5.sleep();
}

void processDownlink() {
  // You have received "sizePayloadDown" bytes stored in the table "payloadDown"
}