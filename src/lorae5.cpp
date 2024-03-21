#include <Arduino.h>
#include "lorae5.h"

LORAE5::LORAE5(String devEUI, String appEUI, String appKey, String devAddr, String nwkSKey, String appSKey){
  this->devEUI = devEUI;
  this->appEUI = appEUI;
  this->appKey = appKey;
  this->devAddr = devAddr;
  this->nwkSKey = nwkSKey;
  this->appSKey = appSKey;
}

void LORAE5::setup(bool mode, uint8_t devClass, uint8_t sf, bool adr, bool confirmed, uint8_t portUp, bool SEND_BY_PUSH_BUTTON, uint32_t FRAME_DELAY){
  unsigned long startTime = millis();
  LoRa_Serial.begin(9600);
  USB_Serial.begin(115200);
  while (!USB_Serial && (millis() - startTime < 5000));

  while(!checkBoard());

  setDevEUI(devEUI);
  setAppEUI(appEUI);
  setAppKey(appKey);

  setDevAddr(devAddr);
  setNwkSKey(nwkSKey);
  setAppSKey(appSKey);

  setRXDelay();
  setMode(mode);
  setClass(devClass);
  setSF(sf);
  setADR(adr);
  setPortUp(portUp);
  this->confirmed = confirmed;
  this->SEND_BY_PUSH_BUTTON = SEND_BY_PUSH_BUTTON;
  this->FRAME_DELAY = FRAME_DELAY;
}

void LORAE5::setRXDelay(){
  readResponse(1000,NO_DEBUG);
  LoRa_Serial.println("AT+DELAY=RX1,1000");
  readResponse(400,NO_DEBUG);
  LoRa_Serial.println("AT+DELAY=RX2,2000");
  readResponse(400,NO_DEBUG);

}
void LORAE5::setMode(bool mode){
  String strMode;
  
  this->mode = mode;
  if (mode == OTAA)   strMode = "LWOTAA";
  if (mode == ABP)    strMode = "LWABP";

  LoRa_Serial.println("AT+MODE=" + strMode);
  readResponse(400,NO_DEBUG);
}

void LORAE5::setClass(uint8_t devClass){
  String strClass;

  this->devClass = devClass;
  if (devClass == CLASS_A)    strClass = "A";
  if (devClass == CLASS_C)    strClass = "C";
  LoRa_Serial.println("AT+CLASS=" + strClass);
  readResponse(400,NO_DEBUG);
}

void LORAE5::setSF(uint8_t sf){
  this->sf = sf;
  LoRa_Serial.println("AT+DR=DR" + String(-sf+12));
  readResponse(400,NO_DEBUG);   
}

void LORAE5::getSetSF(){
  String response;
  LoRa_Serial.println("AT+DR");
  LoRa_Serial.setTimeout(500);
  response = LoRa_Serial.readStringUntil('\n');
  #if DEBUG_LEVEL == 2 
  USB_Serial.println(response);  // DEBUG
  #endif
  if (this->adr == true){
    response = LoRa_Serial.readStringUntil('\n');
    #if DEBUG_LEVEL == 2 
    USB_Serial.println(response);  // DEBUG
    #endif
  }
        if ( response.indexOf("DR5") != -1 )   this->sf = 7;
  else  if ( response.indexOf("DR4") != -1 )   this->sf = 8;
  else  if ( response.indexOf("DR3") != -1 )   this->sf = 9;
  else  if ( response.indexOf("DR2") != -1 )   this->sf = 10;
  else  if ( response.indexOf("DR1") != -1 )   this->sf = 11;
  else  if ( response.indexOf("DR0") != -1 )   this->sf = 12;
  readResponse(100,NO_DEBUG);
}

void LORAE5::setADR(bool adr){
  this->adr = adr;
  LoRa_Serial.println("AT+ADR=" + ((adr == true)? String("ON") : String("OFF")));
  readResponse(400,NO_DEBUG);
}

void LORAE5::setDevEUI(String deveui){
  LoRa_Serial.println("AT+ID=DEVEUI," + devEUI);
  readResponse(400,NO_DEBUG); 
}

void LORAE5::setAppEUI(String appeui){
  LoRa_Serial.println("AT+ID=APPEUI," + appeui);
  readResponse(400,NO_DEBUG); 
}

void LORAE5::setAppKey(String appkey){
  LoRa_Serial.println("AT+KEY=APPKEY," + appkey);
  readResponse(400,NO_DEBUG); 
}

void LORAE5::setDevAddr(String devaddr){
  LoRa_Serial.println("AT+ID=DEVADDR," + devaddr);
  readResponse(400,NO_DEBUG); 
}

void LORAE5::setNwkSKey(String nwkskey){
  LoRa_Serial.println("AT+KEY=NWKSKEY," + nwkskey);
  readResponse(400,NO_DEBUG);   
}

void LORAE5::setAppSKey(String appskey){
  LoRa_Serial.println("AT+KEY=APPSKEY," + appskey);
  readResponse(400,NO_DEBUG);   
}

void LORAE5::setPortUp(uint8_t portUp){
  this->portUp = portUp;
  LoRa_Serial.println("AT+PORT=" + String(portUp));
  readResponse(400,NO_DEBUG); 
}

bool LORAE5::checkBoard(){
  bool success;
  delay(500);
  LoRa_Serial.println("AT");
  USB_Serial.println("\n\n\n\n");
  if ( (success =  checkResponse(400,"+AT: OK\r\n",NO_DEBUG)) == false){
    USB_Serial.println("> LoRa-E5 board not detected ...");
  }
  else{
    USB_Serial.println("> LoRa-E5 board detected ...\n");
  }
  return success;
}

bool LORAE5::join(){
  bool joined = false, failed = false, done = false;
  String response, strDevAddr;
  uint32_t index;

  delay(2000);
  LoRa_Serial.setTimeout(10000);
  LoRa_Serial.println("AT+JOIN");

  do{
    response = LoRa_Serial.readStringUntil('\n');
    #if DEBUG_LEVEL == 1 
    USB_Serial.println(response);  // DEBUG
    #endif
    
    // Analyse DevAddr response
    if ( (index = response.indexOf("DevAddr")) != -1 ){
      index += strlen("DevAddr ");
      strDevAddr += response.charAt(index + 0);
      strDevAddr += response.charAt(index + 1);
      strDevAddr += ":";
      strDevAddr += response.charAt(index + 3);
      strDevAddr += response.charAt(index + 4);
      strDevAddr += ":";
      strDevAddr += response.charAt(index + 6);
      strDevAddr += response.charAt(index + 7);
      strDevAddr += ":";
      strDevAddr += response.charAt(index + 9);
      strDevAddr += response.charAt(index + 10);
      USB_Serial.println("> SUCCESS JOINED OTAA !!!");
      USB_Serial.println("> DevAddr = " + strDevAddr);
      joined = true;
    }
    // Analyse end of Join procedure
    else if ( (index = response.indexOf("Done")) != -1 ){
      done = true;
    }
    // Analyse failed join
    else if ( (index = response.indexOf("Join failed")) != -1 ){
      USB_Serial.println("> JOIN FAILED ...");
      failed = true;
    }
  }  while((!joined || !done) && !failed );
  USB_Serial.println("");
  return joined;
}

void LORAE5::sendPayloadUp(uint8_t* payloadUp, uint8_t sizePayloadUp) {
  USB_Serial.println("Sending " + String(this->confirmed ? "Confirmed " : "Unconfirmed ") + String("Data Up"));
  USB_Serial.print("- Payload UP   ");

  String stringPayloadUp = "";
  for (uint8_t i = 0; i < sizePayloadUp; i++) {
    USB_Serial.print(" 0x");
    if (payloadUp[i] < 0x10) {
      USB_Serial.print('0');
      stringPayloadUp += 0;
    }
    USB_Serial.print(payloadUp[i], HEX);
    USB_Serial.print(" ");
    stringPayloadUp += String(payloadUp[i], HEX);
  }

  USB_Serial.println("");
  USB_Serial.println("- PortUp\t" + String(this->portUp));
  USB_Serial.println("- SF\t\t" + String(this->sf));
  USB_Serial.println("");

  LoRa_Serial.println("AT+" + String(this->confirmed ? "C" : "") + String("MSGHEX") + String("=") + stringPayloadUp);
}

void LORAE5::sendData(uint8_t* payloadUp, uint8_t sizePayloadUp){
  bool transmissionDone = false;
  String response;
  uint32_t index;
  LoRa_Serial.setTimeout(500);

  getSetSF();
  sendPayloadUp(payloadUp, sizePayloadUp);
  do{
    response = LoRa_Serial.readStringUntil('\n');
    #if DEBUG_LEVEL == 3 
    USB_Serial.println(response);  // DEBUG
    #endif

    if ( (index = response.indexOf("Start")) != -1 ){
      USB_Serial.println("Transmission Done\nWaiting for Downlink...");
      transmissionDone = true;
    }

    if ( (index = response.indexOf("Length error 0")) != -1 ){
      LoRa_Serial.println("AT+MSG");
    }

  } while (!transmissionDone);
  
  if(confirmed == true){
    response = LoRa_Serial.readStringUntil('\n');
    #if DEBUG_LEVEL == 3 
    USB_Serial.println(response);  // DEBUG
    #endif
    if ( (index = response.indexOf("Wait ACK")) != -1 ){
      USB_Serial.println("Waiting for ACK...");
    }
  }
  USB_Serial.println("");
}

void LORAE5::readResponse(uint32_t timeOut, bool debug){
  uint32_t tstart = millis();

  while (millis()-tstart < timeOut){
    if(LoRa_Serial.available() > 0) {
      if (debug == DEBUG){
        USB_Serial.print((char)LoRa_Serial.read());
      }
      else{
        LoRa_Serial.read();
      } 
    }       
  }
}

bool LORAE5::checkResponse(uint32_t timeOut, char *strCheck, bool debug){
  uint32_t tstart = millis();
  uint8_t i=0, checkSize = strlen(strCheck);
  char c;
  bool success = false;

  while (millis()-tstart < timeOut){
    if(LoRa_Serial.available() > 0) {
        c = LoRa_Serial.read();
        
        if (debug == DEBUG){
          USB_Serial.print(c);
        }

        if( c == strCheck[i] ){
           if (i == (checkSize-1)){
              success = true;
          }
          else{
            i++;
          }
        }
        else{
          i=0;
        }
    }       
  }
return success;
}

uint8_t LORAE5::getPortUp(){
  return this->portUp;
}

void LORAE5::getPortDown(String response){

  String strPortDown;
  uint32_t index;
  uint32_t indexSemicolon = response.indexOf(';');
  index = response.indexOf("PORT: ");
  index += strlen ("PORT: ");

  for (uint8_t i = 0 ; i < (indexSemicolon - index ) ; i++){
    strPortDown +=response.charAt(index + i);
  }
  uint8_t portDown = strPortDown.toInt();

  USB_Serial.println("- Port " + String(portDown));
  this->portDown = portDown;
}

void LORAE5::getPayloadDown(uint8_t* payloadDown, uint8_t* sizePayloadDown, String response){

  // Get Downlink Payload
    String strPayloadDown;
    uint32_t index;
    uint32_t indexQuote = response.lastIndexOf("\"");
    index = response.indexOf("RX: \"");
    index+= strlen("RX: \"");
    for (uint8_t i = 0 ; i < (indexQuote - index ) ; i++){
      strPayloadDown +=response.charAt(index + i);
    }
    *sizePayloadDown = strPayloadDown.length() / 2 ;
    for (uint8_t i = 0 ; i < *sizePayloadDown ; i++){
      String strByte =strPayloadDown.substring(i * 2, i * 2 + 2);
      payloadDown[i] = strtoul(strByte.c_str(), nullptr, 16);     
    }
    USB_Serial.print("- Payload DOWN ");
    for (uint8_t i = 0 ; i < *sizePayloadDown ; i++){
      USB_Serial.print(" 0x");
      if (payloadDown[i] < 0x10){
        USB_Serial.print('0');
      }
      USB_Serial.print(payloadDown[i], HEX);
      USB_Serial.print(" ");
    }
  USB_Serial.println(""); 
}

void LORAE5::getChannelDown(String response){

  String strChannelDown;
  uint32_t index;
  uint32_t indexSemicolon = response.indexOf(", RSSI ");
  index = response.indexOf(" RXWIN");
  index += strlen (" RXWIN");

  for (uint8_t i = 0 ; i < (indexSemicolon - index ) ; i++){
    strChannelDown +=response.charAt(index + i);
  }
  uint8_t channelDown = strChannelDown.toInt();

  if (channelDown == 0){
    USB_Serial.println("- Slot RXC");
  } else {
    USB_Serial.println("- Slot RX" + String(channelDown));
  }
  USB_Serial.println("");
}

uint8_t LORAE5::getClass(){
  return this->devClass;
}

uint8_t LORAE5::getADR(){
  return this->adr;
}

void LORAE5::printInfo(){ 
#ifndef ARDUINO_AVR_LEONARDO
  USB_Serial.println("# For more information visit : https://github.com/SylvainMontagny/LoRaE5");

  if(this->mode == OTAA)     {USB_Serial.println("> OTAA");}
  else                       {USB_Serial.println("> ABP");}

  if(this->devClass == CLASS_A) {USB_Serial.println("> CLASS_A");}
  if(this->devClass == CLASS_C) {USB_Serial.println("> CLASS_C");}

  USB_Serial.println("> SF " + String(this->sf));
  
  if(this->adr == true)           {USB_Serial.println("> ADR  ON");}
  else                            {USB_Serial.println("> ADR  OFF");}

  USB_Serial.println(((this->confirmed == CONF)? String("> Confirmed") : String("> Unconfirmed")));
  USB_Serial.println("> Port  " + String(this->portUp));
  USB_Serial.println();
  USB_Serial.println(  "* DevEUI   0x " + this->devEUI);

  if (this->mode == ABP) {
    USB_Serial.println("* DevAddr  0x " + this->devAddr);
    USB_Serial.println("* NwkSKey  0x " + this->nwkSKey);
    USB_Serial.println("* AppSKey  0x " + this->appSKey);
  }

  if (this->mode == OTAA){
    USB_Serial.println("* AppKey  0x " + this->appKey);
    USB_Serial.println("* AppEUI-JoinEUI  0x " + this->appEUI);
  }
    USB_Serial.println();
#endif
}

uint8_t LORAE5::awaitForDownlinkClass_A(uint8_t* payloadDown, uint8_t* sizePayloadDown){
  uint32_t index;  
  String response;
  bool isDoneReceived = false;
  bool isAckReceived = false;
 
  LoRa_Serial.setTimeout(500);

  do{
    response += LoRa_Serial.readStringUntil('\n');
    #if DEBUG_LEVEL == 4
    USB_Serial.println(response);  // DEBUG
    #endif
    
    /******        Test Done     *******/
    if ( (index = response.indexOf("Done")) != -1 ){
      isDoneReceived = true;
    }

  }while(isDoneReceived == false);

    //Confirmed part
  if ((confirmed == true)){
    if (index = response.indexOf("ACK Received") != -1 ){
      USB_Serial.println("ACK received");
      isAckReceived = true;
    }
    else {
      USB_Serial.println("No ACK received\n");
      return RET_NO_ACK;
    }
  }

  //Processing data received
  if ((index = response.indexOf("PORT: ")) != -1) {
    USB_Serial.println("Receiving Data Down:");
    getPortDown(response);
    getPayloadDown(payloadDown, sizePayloadDown, response);
    getChannelDown(response);
    return RET_DOWNLINK;
  }

  else if ( ((index = response.indexOf("RXWIN")) != -1) && (isAckReceived == false) ) {
    USB_Serial.println("MAC command received :");
    getChannelDown(response);
    return RET_NO_DOWNLINK;
  }
  
  else {
    if(isAckReceived == false){
    USB_Serial.println("No Class_A Data Received.");
    }
    USB_Serial.println();
    return RET_NO_DOWNLINK;
  }

}

uint8_t LORAE5::awaitForDownlinkClass_C(uint8_t* payloadDown, uint8_t* sizePayloadDown){
  uint32_t index;
  String response;
  bool isDoneReceived = false;
  uint32_t startTime = millis();

  LoRa_Serial.setTimeout(500);

#ifndef ARDUINO_AVR_LEONARDO 
  if( this->SEND_BY_PUSH_BUTTON == true ){
    USB_Serial.println("\n> Press 't' or receive a downlink.");
  }else if( this->SEND_BY_PUSH_BUTTON == false ){
     USB_Serial.println("\n> Press 't', receive a downlink or wait " + String(this->FRAME_DELAY) + String(" ms."));
  }
#endif

  do{
    response += LoRa_Serial.readStringUntil('\n');
    #if DEBUG_LEVEL == 4
    USB_Serial.println(response);  // DEBUG
    #endif
    
    /******        Test Done     *******/
    if ( (index = response.indexOf("Done")) != -1 ){
      isDoneReceived = true;
    }

    /******    Test 't' : transmission request  *******/
    if ( USB_Serial.available() > 0 ) {
      char userInput = USB_Serial.read(); 
      if (userInput == 't' || userInput == 'T') {
        USB_Serial.println("# User input 't'.\n");
        return RET_T;
      }
    }

    /******    Test Timeout  *******/
    if ((this->SEND_BY_PUSH_BUTTON == false) && ((millis() - startTime) >= (this->FRAME_DELAY))){
        USB_Serial.println("No Class_C Data Received.\n");
        USB_Serial.println("# Timeout reached.\n");
        return RET_TIMEOUT;
    }

  }while(isDoneReceived == false);

    //Processing data received
  if ((index = response.indexOf("PORT: ")) != -1) {
    USB_Serial.println("Receiving Data Down:");
    getPortDown(response);
    getPayloadDown(payloadDown, sizePayloadDown, response);
    getChannelDown(response);
    return RET_DOWNLINK;
  }
}

uint8_t LORAE5::await(){
  uint32_t startTime = millis();

  if (this->SEND_BY_PUSH_BUTTON == false) {
    while((millis() - startTime) < (this->FRAME_DELAY)){
      if (USB_Serial.available() > 0) {
        char userInput = USB_Serial.read(); 
        if (userInput == 't' || userInput == 'T') {
          USB_Serial.println("# User input 't'.\n");
          return RET_T;
        }
      }
    }
    USB_Serial.println("# Timeout reached.\n");
    return RET_TIMEOUT;
  } else {
    bool userTest = false;
    while (userTest == false){
    if (USB_Serial.available() > 0) {
        char userInput = USB_Serial.read(); 
        if (userInput == 't' || userInput == 'T') {
          userTest = true;
          USB_Serial.println("# User input 't'.\n");
          return RET_T;
        }
      }
    }
  }
}