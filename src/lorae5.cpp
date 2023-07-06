#include <Arduino.h>
#include "lorae5.h"


LoraE5::LoraE5(String devEUI, String appEUI, String appKey, String devAddr, String nwkSKey, String appSKey){
  this->devEUI = devEUI;
  this->appEUI = appEUI;
  this->appKey = appKey;
  this->devAddr = devAddr;
  this->nwkSKey = nwkSKey;
  this->appSKey = appSKey;
}

void LoraE5::setup(bool mode, uint8_t devClass, uint8_t sf, bool adr, bool confirmed, uint8_t port){
  setMode(mode);
  setDR(sf);
  setADR(adr);
  setPort(port);
  this->confirmed = confirmed;
}

void LoraE5::setMode(bool mode){
  String strMode;
  
  this->mode = mode;
  if (mode == OTAA)   strMode = "LWOTAA";
  if (mode == ABP)    strMode = "LWABP";

  //USB_Serial.println("\r\n  AT+MODE=" + strMode);
  LoRa_Serial.println("AT+MODE=" + strMode);
  readResponse(400,NO_DEBUG);
}

void LoraE5::setDR(uint8_t sf){
  //USB_Serial.println("\r\n  AT+DR=" + String(sf));
  this->sf = sf;
  LoRa_Serial.println("AT+DR=DR" + String(-sf+12));
  readResponse(400,NO_DEBUG);   
}

void LoraE5::setADR(bool adr){
    //USB_Serial.println("\r\n  AT+DR=" + String(sf));
  this->adr = adr;
  LoRa_Serial.println("AT+ADR=" + ((adr == true)? String("ON") : String("OFF")));
  readResponse(400,NO_DEBUG);
}

void LoraE5::setDevEUI(String deveui){
  //USB_Serial.println("\r\n  AT+ID=DEVEUI," + devEUI);
  LoRa_Serial.println("AT+ID=DEVEUI," + devEUI);
  readResponse(400,NO_DEBUG); 
}

void LoraE5::setAppEUI(String appeui){
  this->appEUI = appeui;
  //USB_Serial.println("\r\n  AT+ID=APPEUI," + appeui);
  LoRa_Serial.println("AT+ID=APPEUI," + appeui);
  readResponse(400,NO_DEBUG); 
}

void LoraE5::setAppKey(String appkey){
  //USB_Serial.println("\r\n  AT+KEY=APPKEY," + appkey);
  LoRa_Serial.println("AT+KEY=APPKEY," + appkey);
  readResponse(400,NO_DEBUG); 
}

void LoraE5::setDevAddr(String devaddr){
  //USB_Serial.println("\r\n  AT+ID=DEVADDR," + devaddr);
  LoRa_Serial.println("AT+ID=DEVADDR," + devaddr);
  readResponse(400,NO_DEBUG); 
}

void LoraE5::setNwkSKey(String nwkskey){
  //USB_Serial.println("\r\n  AT+KEY=NWKSKEY," + nwkskey);
  LoRa_Serial.println("AT+KEY=NWKSKEY," + nwkskey);
  readResponse(400,NO_DEBUG);   
}

void LoraE5::setAppSKey(String appskey){
  //USB_Serial.println("\r\n  AT+KEY=APPSKEY," + appskey);
  LoRa_Serial.println("AT+KEY=APPSKEY," + appskey);
  readResponse(400,NO_DEBUG);   
}

void LoraE5::setPort(uint8_t port){
  this->port = port;
   //USB_Serial.println("\r\n  AT+PORT=" + port);
  LoRa_Serial.println("AT+PORT=" + String(port));
  readResponse(400,NO_DEBUG); 
}

bool LoraE5::checkBoard(){
  bool success;
  delay(1000);
  LoRa_Serial.println("AT");
   if ( (success =  checkResponse(400,"+AT: OK\r\n",NO_DEBUG)) == false){
    USB_Serial.println("> LoRa-E5 board not detected ...");
  }
  else{
      USB_Serial.println("> LoRa-E5 board detected ...\r\n");
  }
  return success;
}


bool LoraE5::join(){
  bool success;
  delay(2000);
  
  LoRa_Serial.println("AT+JOIN");
  if ( (success =  checkResponse(10000,"+JOIN: Network joined\r\n",NO_DEBUG)) == true){
     
    USB_Serial.println("> JOINED = OTAA !\r\n");
  } 
  else{
    USB_Serial.println("> JOIN FAILED ...");
  }
  return success;
}


void LoraE5::sendMsg(bool msgType, String payload){
  USB_Serial.println("");
  if(msgType == _HEXA)    USB_Serial.println("Send hexadecimal message : " + payload );
  if(msgType == _STRING) USB_Serial.println("Send string message : \"" + payload + String("\"") );

LoRa_Serial.println("AT+" + String(this->confirmed? "C":"") + String("MSG") + String((msgType==_HEXA)?"HEX":"") + String("=") + payload);
readResponse(10000,DEBUG);
}

void LoraE5::readResponse(uint32_t timeOut, bool debug){
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

bool LoraE5::checkResponse(uint32_t timeOut, char *strCheck, bool debug){
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


void LoraE5::getPort(){
  
}


void LoraE5::getDR(){
 
}

void LoraE5::getADR(){
 
}



void LoraE5::printInfo(bool sendByPushButton, uint32_t frameDelay, bool lowPower){ 

  if(this->mode == OTAA)     {USB_Serial.println("> Activation Mode :       OTAA");}
  else                       {USB_Serial.println("> Activation Mode :       ABP");}

  if(sendByPushButton == true)     {USB_Serial.println("> Send frame              On push button event");}
  else                             {USB_Serial.println("> Send frame every        " + String(frameDelay) +  "ms");}

  USB_Serial.println("> Spreading Factor        " + String(this->sf));
  
  if(this->adr == true)           {USB_Serial.println("> Adaptive Data Rate      ON");}
  else                            {USB_Serial.println("> Adaptive Data Rate      OFF");}

  USB_Serial.println("> Frame type              " + ((this->confirmed == CONF)? String("Confirmed") : String("Unconfirmed")));
  USB_Serial.println("> App Port number         " + String(this->port));
  USB_Serial.println();

  USB_Serial.println(" * Device EUI :              0x " + devEUI);

   if (this->mode == ABP) {
    USB_Serial.println(" * Device Address :          0x " + devAddr);
    USB_Serial.println(" * Network Session Key :     0x " + nwkSKey);
    USB_Serial.println(" * Application Session Key : 0x " + appSKey);
  }

  if (this->mode == OTAA){
   USB_Serial.println(" * Application key : 0x " + appKey);
   USB_Serial.println(" * Application EUI : 0x " + appEUI);
 }

  if(sendByPushButton == false){
    USB_Serial.println("\r\n> Message will be sent every " + String(frameDelay) + String("ms"));
  }
  else {
    USB_Serial.println(" Send letter 'p' on your keyboard to send a message");
  }
   USB_Serial.println();
  
}

 

