#include <Arduino.h>
#include "lorae5.h"
#include "config_application.h"
#include "config_board.h"

uint8_t sizePayloadUp = 4;
uint8_t sizePayloadDown = 0;

uint8_t ret;

uint8_t payloadUp[20] = {0x00, 0x01, 0x02, 0x03};
uint8_t payloadDown[20]  ={0};

LORAE5 lorae5(devEUI, appEUI, appKey, devAddr, nwkSKey, appSKey);

enum class State { SEND_DATA, AWAIT_FOR_DOWNLINK_CLASS_A, AWAIT_FOR_DOWNLINK_CLASS_C, SLEEP, PROCESS_DOWNLINK};

State currentState = State::SEND_DATA;

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

void loop() {
  switch (currentState) {
    case State::SEND_DATA:
      lorae5.sendData(payloadUp, sizePayloadUp);
      currentState = State::AWAIT_FOR_DOWNLINK_CLASS_A;
      break;

    case State::AWAIT_FOR_DOWNLINK_CLASS_A: {

      ret = lorae5.awaitForDownlinkClass_A(payloadDown, &sizePayloadDown);

      switch (ret) {
          case RET_DOWNLINK:
            currentState = State::PROCESS_DOWNLINK;
            break;
          
          case RET_NO_DOWNLINK:
            currentState = State::PROCESS_DOWNLINK;
            break;

          case RET_NO_ACK:
            currentState = State::SEND_DATA;
            break;
        }
      break;
    }

    case State::AWAIT_FOR_DOWNLINK_CLASS_C: {

      ret = lorae5.awaitForDownlinkClass_C(payloadDown, &sizePayloadDown);

      switch (ret) {
          case RET_DOWNLINK:
            currentState = State::PROCESS_DOWNLINK;
            break;
          
          case RET_T:
            currentState = State::SEND_DATA;
            break;

          case RET_TIMEOUT:
            currentState = State::SEND_DATA;
            break;
        }
      break;
    }

    case State::PROCESS_DOWNLINK:
        processDownlink();
        currentState = State::SLEEP;
        break;

    case State::SLEEP:
        lorae5.sleep();
        currentState = State::SEND_DATA;
        break;

  }
}

void processDownlink() {
  // You have received "sizePayloadDown" bytes stored in the table "payloadDown"
}