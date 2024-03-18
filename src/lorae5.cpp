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

void LORAE5::setup(bool mode, uint8_t devClass, uint8_t sf, bool adr, bool confirmed, uint8_t portUp){
  unsigned long startTime = millis();
  LoRa_Serial.begin(9600);
  USB_Serial.begin(115200);
  while (!USB_Serial && (millis() - startTime < 5000));
  USB_Serial.println("\r\n\n\n\n");
  USB_Serial.println("# For more information visit : https://github.com/SylvainMontagny/LoRaE5 ");

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
  response = LoRa_Serial.readStringUntil('\n');
  if (this->adr == true){
    response = LoRa_Serial.readStringUntil('\n');
  }
  if ( response.indexOf("DR5") != -1 )   this->sf = 7;
  else  if ( response.indexOf("DR4") != -1 )   this->sf = 8;
  else  if ( response.indexOf("DR3") != -1 )   this->sf = 9;
  else  if ( response.indexOf("DR2") != -1 )   this->sf = 10;
  else  if ( response.indexOf("DR1") != -1 )   this->sf = 11;
  else  if ( response.indexOf("DR0") != -1 )   this->sf = 12;
  else  if ( response.indexOf("") != -1 )   this->sf = 12;
  readResponse(200,NO_DEBUG);
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
  if ( (success =  checkResponse(400,"+AT: OK\r\n",NO_DEBUG)) == false){
    USB_Serial.println("> LoRa-E5 board not detected ...");
  }
  else{
    USB_Serial.println("> LoRa-E5 board detected ...\r\n");
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
    // USB_Serial.println(response);
    
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

  return joined;
}

void LORAE5::displayPayloadUp(uint8_t* payloadUp, uint8_t sizePayloadUp) {
  USB_Serial.println("");
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

  LoRa_Serial.println("AT+" + String(this->confirmed ? "C" : "") + String("MSGHEX") + String("=") + stringPayloadUp);
}

void LORAE5::sendData(uint8_t* payloadUp, uint8_t sizePayloadUp){

  bool done = false;
  String response;
  uint32_t index;

  displayPayloadUp(payloadUp, sizePayloadUp);

  do{
    response = LoRa_Serial.readStringUntil('\n');
    //USB_Serial.println(response);  // DEBUG
    
    // Analyse End of transmission process
    if ( (index = response.indexOf("Done")) != -1 ){
      done = 1;
    }

    // Analyse Start transmission
    else if ( (index = response.indexOf("Start")) != -1 ){
      USB_Serial.println("Transmission Done");
    }

    else if ( (index = response.indexOf("Length error 0")) != -1 ){
      LoRa_Serial.println("AT+MSG");
    }
    
  } while (!done);
  getSetSF();
}

bool LORAE5::sendData(uint8_t* payloadUp, uint8_t sizePayloadUp, uint8_t* payloadDown, uint8_t* sizePayloadDown){

  bool done = false; 
  bool isPayloadDownReceived = false; 
  String response;
  uint32_t index;

  displayPayloadUp(payloadUp, sizePayloadUp);

  do{
    response = LoRa_Serial.readStringUntil('\n');
    //USB_Serial.println(response);  // DEBUG

    // Analyse End of transmission process
    if ( (index = response.indexOf("Done")) != -1 ){
      done = 1;
    }

    // Analyse Start transmission
    else if ( (index = response.indexOf("Start")) != -1 ){
      USB_Serial.println("Transmission Done");
    }

    else if ( (index = response.indexOf("Length error 0")) != -1 ){
      LoRa_Serial.println("AT+MSG");
    }
    
    // Analyse Wait ACK
    else if ( (index = response.indexOf("Wait ACK")) != -1 ){
      USB_Serial.println("Wait ACK...");
    }

    // Analyse ACK
    else if ( (index = response.indexOf("ACK Received")) != -1 ){
      USB_Serial.println("ACK Received");
    }
    
    // Analyse Downlink reception
    else if ( (index = response.indexOf("PORT: ")) != -1 ){
      String strPortDown, strPayloadDown;
      isPayloadDownReceived = true;

      // Get Downlink Port
      uint32_t indexSemicolon = response.indexOf(';');
      index += strlen ("PORT: ");
      for (uint8_t i = 0 ; i < (indexSemicolon - index ) ; i++){
        strPortDown +=response.charAt(index + i);
      }
      uint8_t portDown = strPortDown.toInt();
      getPortDown(portDown);
      
      // Get Downlink Payload
      uint32_t indexQuote = response.lastIndexOf('"');
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
      USB_Serial.println();
    }

  }  while(!done);
  getSetSF();
  if( isPayloadDownReceived == true) return true;
  else                            return false;
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

void LORAE5::getPortDown(uint8_t portDown){
  USB_Serial.println("");
  USB_Serial.println("Downlink Received");
  USB_Serial.println("- Port " + String(portDown));
  this->portDown = portDown;
}

uint8_t LORAE5::getClass(){
  return this->devClass;
}

uint8_t LORAE5::getADR(){
  return this->adr;
}

void LORAE5::printInfo(bool sendByPushButton, uint32_t frameDelay){ 

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
}

void LORAE5::awaitNextTransmission(uint32_t delay, bool pushButton){
  uint32_t startTime = millis();
  bool upRequest = false;

  if ( pushButton == false ){
    USB_Serial.println("\r\n> Press 't' or wait " + String(delay) + String(" ms."));
    while ( ((millis() - startTime) < delay) && (upRequest != true)) {
      if(LoRa_Serial.available()>0){
        USB_Serial.print((char)LoRa_Serial.read());
      }
      if (USB_Serial.available() > 0) {
        char userInput = USB_Serial.read(); 
        if ( userInput == 't' || userInput == 'T' ){
          upRequest =  true;
        }
      }
    }
  } else {
    USB_Serial.println("\r\n> Press 't' to send a message.");
    while (upRequest != true) {
        if(LoRa_Serial.available()>0){
          USB_Serial.print((char)LoRa_Serial.read());
        }
      if (USB_Serial.available() > 0) {
        char userInput = USB_Serial.read(); 
        if ( userInput == 't' || userInput == 'T' ){
          upRequest =  true;
        }
      }
    }
  }
}