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

void LORAE5::setup(bool mode, uint8_t devClass, uint8_t sf, bool adr, bool confirmed, uint8_t port){
  LoRa_Serial.begin(9600);
  USB_Serial.begin(115200);
  while (!USB_Serial);
  USB_Serial.println("\r\n\n\n\n\n\n\n\n");
  USB_Serial.println("#######################################");
  USB_Serial.println("#### LoRaWAN Training Session #########");
  USB_Serial.println("#### Savoie Mont Blanc University #####\r\n");
  
  while(!checkBoard());

  setDevEUI(devEUI);
  setAppEUI(appEUI);
  setAppKey(appKey);

  setDevAddr(devAddr);
  setNwkSKey(nwkSKey);
  setAppSKey(appSKey);

  setRXDelay();
  setMode(mode);
  setSF(sf);
  setADR(adr);
  setPort(port);
  this->confirmed = confirmed;
}

void LORAE5::setRXDelay(){
  LoRa_Serial.println("AT+RESET");
  while (!USB_Serial);
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

void LORAE5::setSF(uint8_t sf){
  this->sf = sf;
  LoRa_Serial.println("AT+DR=DR" + String(-sf+12));
  readResponse(400,NO_DEBUG);   
}

void LORAE5::getSF(){
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
  delay(100);
  while (Serial.available() > 0) {
        char c = Serial.read(); 
    }
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
  this->appEUI = appeui;
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

void LORAE5::setPort(uint8_t port){
  this->port = port;
  LoRa_Serial.println("AT+PORT=" + String(port));
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


void LORAE5::sendMsg(bool msgType, String payload){
  USB_Serial.println("");
  if(msgType == _HEXA)    USB_Serial.println("Send hexadecimal message : " + payload );
  if(msgType == _STRING) USB_Serial.println("Send string message : \"" + payload + String("\"") );

  LoRa_Serial.println("AT+" + String(this->confirmed? "C":"") + String("MSG") + String((msgType==_HEXA)?"HEX":"") + String("=") + payload);
  readResponse(10000,DEBUG);
}



bool LORAE5::sendData(uint8_t* payload, uint8_t sizeUplink, uint8_t* downlink, uint8_t* sizeDownlink){

  bool done = false; 
  bool isDownlinkReceived = false; 
  String stringPayload = "";
  String response;
  uint32_t index;
  

  USB_Serial.println("");
  USB_Serial.println("Sending " + String(this->confirmed? "Confirmed ":"Unconfirmed ") + String("Data Up"));
  USB_Serial.print("- Payload UP   ");

  for(uint8_t i=0 ; i < sizeUplink ; i++){
    USB_Serial.print(" 0x");
    if (payload[i] < 0x10){
      stringPayload += "0";
      USB_Serial.print('0');
    }
    USB_Serial.print(payload[i], HEX);
    USB_Serial.print(" ");
    stringPayload += String(payload[i],HEX);
  }
  USB_Serial.println("");
  USB_Serial.println("- Port\t\t" + String(this->port));
  USB_Serial.println("- SF\t\t" + String(this->sf));
  LoRa_Serial.println("AT+" + String(this->confirmed? "C":"") + String("MSGHEX") + String("=") + stringPayload);

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
      String strPortDown, strDownlink;
      isDownlinkReceived = true;

      // Get Downlink Port
      uint32_t indexSemicolon = response.indexOf(';');
      index += strlen ("PORT: ");
      for (uint8_t i = 0 ; i < (indexSemicolon - index ) ; i++){
        strPortDown +=response.charAt(index + i);
      }
      USB_Serial.println("\r\nDownlink Received");
      USB_Serial.println("- Port          " + strPortDown); 
      this->portDown = strPortDown.toInt();
      
      // Get Downlink Payload
      uint32_t indexQuote = response.lastIndexOf('"');
      index = response.indexOf("RX: \"");
      index+= strlen("RX: \"");
      for (uint8_t i = 0 ; i < (indexQuote - index ) ; i++){
        strDownlink +=response.charAt(index + i);
      }
      *sizeDownlink = strDownlink.length() / 2 ;
      for (uint8_t i = 0 ; i < *sizeDownlink ; i++){
        String strByte =strDownlink.substring(i * 2, i * 2 + 2);
        downlink[i] = strtoul(strByte.c_str(), nullptr, 16);     
      }
      USB_Serial.print("- Payload DOWN ");
      for (uint8_t i = 0 ; i < *sizeDownlink ; i++){
        USB_Serial.print(" 0x");
        if (downlink[i] < 0x10){
          USB_Serial.print('0');
        }
        USB_Serial.print(downlink[i], HEX);
        USB_Serial.print(" ");
      }
      USB_Serial.println();
    }

  }  while(!done);
  getSF();
  if( isDownlinkReceived == true) return true;
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


void LORAE5::getPort(){
  
}




void LORAE5::getADR(){
 
}



void LORAE5::printInfo(bool sendByPushButton, uint32_t frameDelay){ 

  if(this->mode == OTAA)     {USB_Serial.println("> Activation Mode :       OTAA");}
  else                       {USB_Serial.println("> Activation Mode :       ABP");}

  if(sendByPushButton == true)     {USB_Serial.println("> Send frame              On push button event");}
  else                             {USB_Serial.println("> Send frame every        " + String(frameDelay) +  " ms");}

  USB_Serial.println("> Spreading Factor        " + String(this->sf));
  
  if(this->adr == true)           {USB_Serial.println("> Adaptive Data Rate      ON");}
  else                            {USB_Serial.println("> Adaptive Data Rate      OFF");}

  USB_Serial.println("> Frame type              " + ((this->confirmed == CONF)? String("Confirmed") : String("Unconfirmed")));
  USB_Serial.println("> App Port number         " + String(this->port));
  USB_Serial.println();

  USB_Serial.println(" * Device EUI :      0x " + devEUI);

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
    USB_Serial.println("\r\n> Message will be sent every " + String(frameDelay) + String(" ms"));
  }
  else {
    USB_Serial.println(" Send letter 'p' on your keyboard to send a message");
  }
   USB_Serial.println();
  
}

 

