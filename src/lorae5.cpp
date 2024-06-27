#include <Arduino.h>
#include <HardwareSerial.h>
#include "lorae5.h"

LORAE5::LORAE5(String devEUI, String appEUI, String appKey, String devAddr, String nwkSKey, String appSKey){
  this->devEUI = devEUI;
  this->appEUI = appEUI;
  this->appKey = appKey;
  this->devAddr = devAddr;
  this->nwkSKey = nwkSKey;
  this->appSKey = appSKey;
}

#if defined(ARDUINO_AVR_LEONARDO)
/// @brief Allow to set the hardware setup for our transmission
/// @param myDebugSerial is the debug serial link
/// @param myLoRaSerial is the LoRaE5 serial link
void LORAE5::setup_hardware(Serial_* myDebugSerial, HardwareSerial* myLoRaSerial){ 
  unsigned long startTime = millis();
  
  this->serialD = myDebugSerial;
  this->serialL = myLoRaSerial;

  (this->serialD)->begin(115200);
  (this->serialL)->begin(9600);

  while (millis() - startTime < 2000);
}
#elif defined(ARDUINO_WIO_TERMINAL)
/// @brief Allow to set the hardware setup for our transmission
/// @param myDebugSerial is the debug serial link
/// @param myLoRaSerial is the LoRaE5 serial link
void LORAE5::setup_hardware(Serial_* myDebugSerial, Uart* myLoRaSerial){ 
  unsigned long startTime = millis();
  
  this->serialD = myDebugSerial;
  this->serialL = myLoRaSerial;

  (this->serialD)->begin(115200);
  (this->serialL)->begin(9600);

  while (millis() - startTime < 2000);
}
#else
/// @brief Allow to set the hardware setup for our transmission
/// @param myDebugSerial is the debug serial link
/// @param myLoRaSerial is the LoRaE5 serial link
void LORAE5::setup_hardware(HardwareSerial* myDebugSerial, HardwareSerial* myLoRaSerial){ 
  unsigned long startTime = millis();

  this->serialD = myDebugSerial;
  this->serialL = myLoRaSerial;

  (this->serialD)->begin(115200);
  (this->serialL)->begin(9600);

  while (millis() - startTime < 2000);
}
#endif

/// @brief Allow to set the LoRaWAN setup for our transmission
/// @param region is the region selected by the user
/// @param mode is the mode selected by the user
/// @param devClass is the class of the device selected by the user
/// @param sf is the Spreading Factor selected by the user
/// @param adr is to enable or disable the Adaptive Data Rate
/// @param confirmed is to enable or disable the confirmed message
/// @param portUp is the uplink port selected by the user
/// @param SEND_BY_PUSH_BUTTON is the push button condition selected by the user
/// @param FRAME_DELAY is the frame delay selected by the user
void LORAE5::setup_lorawan(uint8_t region, bool mode, uint8_t devClass, uint8_t sf, bool adr, bool confirmed, uint8_t portUp, bool SEND_BY_PUSH_BUTTON, uint32_t FRAME_DELAY){

  while(!checkBoard());

  setDevEUI(devEUI);
  setAppEUI(appEUI);
  setAppKey(appKey);

  setDevAddr(devAddr);
  setNwkSKey(nwkSKey);
  setAppSKey(appSKey);

  this->region = region;

  setRXDelay();
  setFrequencyBand();
  setMode(mode);
  setClass(devClass);
  setSF(sf);
  setADR(adr);
  setPortUp(portUp);

  this->confirmed = confirmed;
  this->SEND_BY_PUSH_BUTTON = SEND_BY_PUSH_BUTTON;
  this->FRAME_DELAY = FRAME_DELAY;
}

/// @brief Allow to set the RX delay
void LORAE5::setRXDelay(){
  readResponse(1000,NO_DEBUG);
  SERIAL_L("AT+DELAY=RX1,1000");
  readResponse(400,NO_DEBUG);
  SERIAL_L("AT+DELAY=RX2,2000");
  readResponse(400,NO_DEBUG);
}

/// @brief Allow to set the mode of the device
/// @param mode is the mode selected by the user
void LORAE5::setMode(bool mode){
  String strMode;
  
  this->mode = mode;
  if (mode == OTAA)   strMode = "LWOTAA";
  if (mode == ABP)    strMode = "LWABP";

  SERIAL_L("AT+MODE=" + strMode);
  readResponse(400,NO_DEBUG);
}

/// @brief Allow to set the class of the device
/// @param devClass is the the class of the device selected by the user
void LORAE5::setClass(uint8_t devClass){
  String strClass;

  this->devClass = devClass;
  if (devClass == CLASS_A)    strClass = "A";
  if (devClass == CLASS_C)    strClass = "C";
  SERIAL_L("AT+CLASS=" + strClass);
  readResponse(400,NO_DEBUG);
}

/// @brief Allow to set the Spreading Factor (SF) of the device
/// @param sf is the Spreading factor selected by the user
void LORAE5::setSF(uint8_t sf){
  this->sf = sf;
  if ((this->region) == EU868 || (this->region) == CN779 || (this->region) == EU433 || (this->region) == AU915 || (this->region) == CN470 || (this->region) == AS923 || (this->region) == KR920 || (this->region) == IN865 || (this->region) == RU864 || (this->region) == CN470PREQUEL || (this->region) == STE920){ 
  SERIAL_L("AT+DR=DR" + String(-sf+12));
  }

  if ((this->region) == US915 || (this->region) == US915HYBRID || (this->region) == AU915OLD){
  SERIAL_L("AT+DR=DR" + String(-sf+10));
  }

  String response = (this->serialL)->readStringUntil('\n');
  readResponse(400,NO_DEBUG);   
}

/// @brief Allow to process and set the Spreading Factor (SF) received from the downlink
void LORAE5::getSetSF(){
  String response;
  (this->serialL)->setTimeout(500);
  SERIAL_L("AT+DR");
  response = (this->serialL)->readStringUntil('\n');
  #if DEBUG_LEVEL == 2
  SERIAL_D(response);  // DEBUG
  #endif
  if (this->adr == true){
    response = (this->serialL)->readStringUntil('\n');
    #if DEBUG_LEVEL == 2 
    SERIAL_D(response);  // DEBUG
    #endif
  }

  if ((this->region) == EU868 || (this->region) == CN779 || (this->region) == EU433 || (this->region) == AU915 || (this->region) == CN470 || (this->region) == AS923 || (this->region) == KR920 || (this->region) == IN865 || (this->region) == RU864 || (this->region) == CN470PREQUEL || (this->region) == STE920){
        if ( response.indexOf("DR0") != -1 )   this->sf = 12;
  else  if ( response.indexOf("DR1") != -1 )   this->sf = 11;  
  else  if ( response.indexOf("DR2") != -1 )   this->sf = 10;    
  else  if ( response.indexOf("DR3") != -1 )   this->sf = 9;
  else  if ( response.indexOf("DR4") != -1 )   this->sf = 8;
  else  if ( response.indexOf("DR5") != -1 )   this->sf = 7;
  }

  if ((this->region) == US915 || (this->region) == US915HYBRID || (this->region) == AU915OLD){
        if ( response.indexOf("DR0") != -1 )   this->sf = 10;
  else  if ( response.indexOf("DR1") != -1 )   this->sf = 9;
  else  if ( response.indexOf("DR2") != -1 )   this->sf = 8;
  else  if ( response.indexOf("DR3") != -1 )   this->sf = 7;
  }

  readResponse(100,NO_DEBUG);
}

/// @brief Allow to set Adaptative Data Rate (ADR)
/// @param adr is to enable or disable the Adaptive Data Rate
void LORAE5::setADR(bool adr){
  this->adr = adr;
  SERIAL_L("AT+ADR=" + ((adr == true)? String("ON") : String("OFF")));
  readResponse(400,NO_DEBUG);
}

/// @brief Allow to set Device EUI
/// @param deveui is the Device EUI
void LORAE5::setDevEUI(String deveui){
  SERIAL_L("AT+ID=DEVEUI," + devEUI);
  readResponse(400,NO_DEBUG); 
}

/// @brief Allow to set Application EUI
/// @param appeui is the Application EUI
void LORAE5::setAppEUI(String appeui){
  SERIAL_L("AT+ID=APPEUI," + appeui);
  readResponse(400,NO_DEBUG); 
}

/// @brief Allow to set Application Key
/// @param appkey is the Application Key
void LORAE5::setAppKey(String appkey){
  SERIAL_L("AT+KEY=APPKEY," + appkey);
  readResponse(400,NO_DEBUG); 
}

/// @brief Allow to set Device Address
/// @param devaddr is the Device Address
void LORAE5::setDevAddr(String devaddr){
  SERIAL_L("AT+ID=DEVADDR," + devaddr);
  readResponse(400,NO_DEBUG); 
}

/// @brief Allow to set Network Session Key
/// @param nwkskey is the Network Session Key
void LORAE5::setNwkSKey(String nwkskey){
  SERIAL_L("AT+KEY=NWKSKEY," + nwkskey);
  readResponse(400,NO_DEBUG);   
}

/// @brief Allow to set Application Session Key
/// @param appskey is the Application Session Key
void LORAE5::setAppSKey(String appskey){
  SERIAL_L("AT+KEY=APPSKEY," + appskey);
  readResponse(400,NO_DEBUG);   
}

/// @brief Allow to set the uplink port
/// @param portUp is the uplink port selected by the user
void LORAE5::setPortUp(uint8_t portUp){
  this->portUp = portUp;
  SERIAL_L("AT+PORT=" + String(portUp));
  readResponse(400,NO_DEBUG); 
}

/// @brief Allow to check if a board is connected
/// @return Board status
bool LORAE5::checkBoard(){
  bool success;
  delay(500);
  SERIAL_L("AT");
  SERIAL_D("\n\n\n\n");
  if ( (success =  checkResponse(400,"+AT: OK\r\n",NO_DEBUG)) == false){
    SERIAL_D("> LoRa-E5 board not detected ...");
  }
  else{
    SERIAL_D("> LoRa-E5 board detected ...\n");
  }
  return success;
}

/// @brief Allow to check if join procedure is successful
/// @return Joined status
bool LORAE5::join(){
  bool joined = false, failed = false, done = false;
  String response, strDevAddr;
  uint32_t index;

  delay(2000);
  (this->serialL)->setTimeout(10000);
  SERIAL_L("AT+JOIN");

  do{
    response = (this->serialL)->readStringUntil('\n');
    #if DEBUG_LEVEL == 1 
    SERIAL_D(response);  // DEBUG
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
      SERIAL_D("> SUCCESS JOINED OTAA !!!");
      SERIAL_D("> DevAddr = " + strDevAddr);
      joined = true;
    }
    // Analyse end of Join procedure
    else if ( (index = response.indexOf("Done")) != -1 ){
      done = true;
    }
    // Analyse failed join
    else if ( (index = response.indexOf("Join failed")) != -1 ){
      SERIAL_D("> JOIN FAILED ...");
      failed = true;
    }
  }  while((!joined || !done) && !failed );
  SERIAL_D("");
  return joined;
}

/// @brief Allow to send uplink data
/// @param payloadUp is the data to send
/// @param sizePayloadUp is the size of the data to send
void LORAE5::sendPayloadUp(uint8_t* payloadUp, uint8_t sizePayloadUp) {
  SERIAL_D("Sending " + String(this->confirmed ? "Confirmed " : "Unconfirmed ") + String("Data Up"));
  (this->serialD)->print("- Payload UP   ");

  String stringPayloadUp = "";
  for (uint8_t i = 0; i < sizePayloadUp; i++) {
    (this->serialD)->print(" 0x");
    if (payloadUp[i] < 0x10) {
      (this->serialD)->print('0');
      stringPayloadUp += 0;
    }
    (this->serialD)->print(payloadUp[i], HEX);
    (this->serialD)->print(" ");
    stringPayloadUp += String(payloadUp[i], HEX);
  }

  SERIAL_D("");
  SERIAL_D("- PortUp\t" + String(this->portUp));
  SERIAL_D("- SF\t\t" + String(this->sf));
  SERIAL_D("");

  SERIAL_L("AT+" + String(this->confirmed ? "C" : "") + String("MSGHEX") + String("=") + stringPayloadUp);
}

/// @brief Allow to manage the sending of uplink data
/// @param payloadUp is the data to send
/// @param sizePayloadUp is the size of the data to send
void LORAE5::sendData(uint8_t* payloadUp, uint8_t sizePayloadUp){
  bool transmissionDone = false;
  String response;
  uint32_t index;
  (this->serialL)->setTimeout(500);

  getSetSF();
  sendPayloadUp(payloadUp, sizePayloadUp);
  do{
    response = (this->serialL)->readStringUntil('\n');
    #if DEBUG_LEVEL == 3 
    SERIAL_D(response);  // DEBUG
    #endif

    if ( (index = response.indexOf("Start")) != -1 ){
      SERIAL_D("Transmission Done\nWaiting for Downlink...");
      transmissionDone = true;
    }

    if ( (index = response.indexOf("Length error 0")) != -1 ){
      SERIAL_L("AT+MSG");
    }

    if ( (index = response.indexOf("LoRaWAN modem is busy")) != -1 ){
      SERIAL_D("LoRaWAN modem is busy");
    }

  } while (!transmissionDone);
  
  if(confirmed == true){
    response = (this->serialL)->readStringUntil('\n');
    #if DEBUG_LEVEL == 3 
    SERIAL_D(response);  // DEBUG
    #endif
    if ( (index = response.indexOf("Wait ACK")) != -1 ){
      SERIAL_D("Waiting for ACK...");
    }
  }
  SERIAL_D("");
}

/// @brief Allow to debug or consume characters from the LoRaE5 module
/// @param timeOut is the maximum allocated time for the function
/// @param debug is to enable or disable the display
void LORAE5::readResponse(uint32_t timeOut, bool debug){
  uint32_t tstart = millis();

  while (millis()-tstart < timeOut){
    if((this->serialL)->available() > 0) {
      if (debug == DEBUG){
        (this->serialD)->print((char)(this->serialL)->read());
      }
      else{
        (this->serialL)->read();
      } 
    }       
  }
}

/// @brief Allow to check if a string is present in the log
/// @param timeOut is the maximum waiting time
/// @param strCheck is the String we are looking for
/// @param debug is to enable or disable the display
/// @return if the requested string is present or not
bool LORAE5::checkResponse(uint32_t timeOut, char *strCheck, bool debug){
  uint32_t tstart = millis();
  uint8_t i=0, checkSize = strlen(strCheck);
  char c;
  bool success = false;

  while (millis()-tstart < timeOut){
    if((this->serialL)->available() > 0) {
        c = (this->serialL)->read();
        
        if (debug == DEBUG){
          (this->serialD)->print(c);
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

/// @brief Displays the port up
/// @param string is the information received from the device and stored in a buffer memory
/// @return port up
uint8_t LORAE5::getPortUp(){
  return this->portUp;
}

/// @brief Displays the port down
/// @param string is the information received from the device and stored in a buffer memory
/// @return port down
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

  SERIAL_D("- Port " + String(portDown));
  this->portDown = portDown;
}

/// @brief Allow to process and display the payload down
/// @param payloadDown is downlink message
/// @param sizePayloadDown is size of the downlink message
/// @param response is the information received from the device and stored in a buffer memory
/// @return Payload down
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
    (this->serialD)->print("- Payload DOWN ");
    for (uint8_t i = 0 ; i < *sizePayloadDown ; i++){
      (this->serialD)->print(" 0x");
      if (payloadDown[i] < 0x10){
        (this->serialD)->print('0');
      }
      (this->serialD)->print(payloadDown[i], HEX);
      (this->serialD)->print(" ");
    }
  SERIAL_D(""); 
}

/// @brief Displays downlink channel
/// @param response is the information received from the device and stored in a buffer memory
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
    SERIAL_D("- Slot RXC");
  } else {
    SERIAL_D("- Slot RX" + String(channelDown));
  }
  SERIAL_D("");
}

/// @brief Updates the Device Class
/// @return Device Class
uint8_t LORAE5::getClass(){
  return this->devClass;
}

/// @brief Updates the Data Rate
/// @return Data rate
uint8_t LORAE5::getADR(){
  return this->adr;
}

/// @brief Summarizes device parameters
void LORAE5::printInfo(){ 
#ifndef ARDUINO_AVR_LEONARDO
  SERIAL_D("# For more information visit : https://github.com/SylvainMontagny/LoRaE5");

  if(this->mode == OTAA)     {SERIAL_D("> OTAA");}
  else                       {SERIAL_D("> ABP");}

  if(this->devClass == CLASS_A) {SERIAL_D("> CLASS_A");}
  if(this->devClass == CLASS_C) {SERIAL_D("> CLASS_C");}

  SERIAL_D("> SF " + String(this->sf));
  
  if(this->adr == true)           {SERIAL_D("> ADR  ON");}
  else                            {SERIAL_D("> ADR  OFF");}

  SERIAL_D(((this->confirmed == CONF)? String("> Confirmed") : String("> Unconfirmed")));
  SERIAL_D("> Port  " + String(this->portUp));
  SERIAL_D();
  SERIAL_D(  "* DevEUI   0x " + this->devEUI);

  if (this->mode == ABP) {
    SERIAL_D("* DevAddr  0x " + this->devAddr);
    SERIAL_D("* NwkSKey  0x " + this->nwkSKey);
    SERIAL_D("* AppSKey  0x " + this->appSKey);
  }

  if (this->mode == OTAA){
    SERIAL_D("* AppKey  0x " + this->appKey);
    SERIAL_D("* AppEUI-JoinEUI  0x " + this->appEUI);
  }
    SERIAL_D();
#endif
}

/// @brief Waiting for Class_A downlink
/// @param payloadDown is downlink message
/// @param sizePayloadDown is size of the downlink message
/// @return Downlink data information (RET_T/RET_TIMEOUT/RET_DOWNLINK/RET_NO_DOWNLINK)
uint8_t LORAE5::awaitForDownlinkClass_A(uint8_t* payloadDown, uint8_t* sizePayloadDown){
  uint32_t index;  
  String response;
  bool isDoneReceived = false;
  bool isAckReceived = false;
 
  (this->serialL)->setTimeout(500);

  do{
    response += (this->serialL)->readStringUntil('\n');
    #if DEBUG_LEVEL == 4
    SERIAL_D(response);  // DEBUG
    #endif
    
    /******        Test Done     *******/
    if ( (index = response.indexOf("Done")) != -1 ){
      isDoneReceived = true;
    }

  }while(isDoneReceived == false);

    //Confirmed part
  if ((confirmed == true)){
    if (index = response.indexOf("ACK Received") != -1 ){
      SERIAL_D("ACK received");
      isAckReceived = true;
    }
    else {
      SERIAL_D("No ACK received\n");
      return RET_NO_ACK;
    }
  }

  //Processing data received
  if ((index = response.indexOf("PORT: ")) != -1) {
    SERIAL_D("Receiving Data Down:");
    getPortDown(response);
    getPayloadDown(payloadDown, sizePayloadDown, response);
    getChannelDown(response);
    return RET_DOWNLINK;
  }

  else if ( ((index = response.indexOf("RXWIN")) != -1) && (isAckReceived == false) ) {
    SERIAL_D("MAC command received :");
    getChannelDown(response);
    return RET_NO_DOWNLINK;
  }
  
  else {
    if(isAckReceived == false){
    SERIAL_D("No Class_A Data Received.");
    }
    SERIAL_D();
    return RET_NO_DOWNLINK;
  }

}

/// @brief Waiting for Class_C downlink
/// @param payloadDown is downlink message
/// @param sizePayloadDown is size of the downlink message
/// @return Downlink data information (RET_T/RET_TIMEOUT/RET_DOWNLINK)
uint8_t LORAE5::awaitForDownlinkClass_C(uint8_t* payloadDown, uint8_t* sizePayloadDown){
  uint32_t index;
  String response;
  bool isDoneReceived = false;
  uint32_t startTime = millis();

  (this->serialL)->setTimeout(500);

#ifndef ARDUINO_AVR_LEONARDO 
  if( this->SEND_BY_PUSH_BUTTON == true ){
    SERIAL_D("> Press 't' or receive a downlink.\n");
  }else if( this->SEND_BY_PUSH_BUTTON == false ){
     SERIAL_D("> Press 't', receive a downlink or wait " + String(this->FRAME_DELAY) + String(" ms.\n"));
  }
#endif

  do{
    response += (this->serialL)->readStringUntil('\n');
    #if DEBUG_LEVEL == 4
    SERIAL_D(response);  // DEBUG
    #endif
    
    /******        Test Done     *******/
    if ( (index = response.indexOf("Done")) != -1 ){
      isDoneReceived = true;
    }

    /******    Test 't' : transmission request  *******/
    if ( (this->serialD)->available() > 0 ) {
      char userInput = (this->serialD)->read(); 
      if (userInput == 't' || userInput == 'T') {
        SERIAL_D("# User input 't'.\n");
        return RET_T;
      }
    }

    /******    Test Timeout  *******/
    if ((this->SEND_BY_PUSH_BUTTON == false) && ((millis() - startTime) >= (this->FRAME_DELAY))){
        SERIAL_D("No Class_C Data Received.\n");
        SERIAL_D("# Timeout reached.\n");
        return RET_TIMEOUT;
    }

  }while(isDoneReceived == false);

    //Processing data received
  if ((index = response.indexOf("PORT: ")) != -1) {
    SERIAL_D("Receiving Data Down:");
    getPortDown(response);
    getPayloadDown(payloadDown, sizePayloadDown, response);
    getChannelDown(response);
    return RET_DOWNLINK;
  }
}

/// @brief Allow to wait and put the LoRaE5 module in Low Power mode ON/OFF
/// @return Downlink data information (RET_T/RET_TIMEOUT)
uint8_t LORAE5::sleep(){
  uint32_t startTime = millis();
  SERIAL_L("AT+LOWPOWER");
  String response = (this->serialL)->readStringUntil('\n');
  readResponse(5, NO_DEBUG);

  if (this->SEND_BY_PUSH_BUTTON == false) {
    while((millis() - startTime) < (this->FRAME_DELAY)){
      if ((this->serialD)->available() > 0) {
        char userInput = (this->serialD)->read(); 
        if (userInput == 't' || userInput == 'T') {
          SERIAL_L("WAKE-UP");
          response = (this->serialL)->readStringUntil('\n');
          readResponse(5,NO_DEBUG);
          SERIAL_D("# User input 't'.\n");
          return RET_T;
        }
      }
    }
    SERIAL_L("WAKE-UP");
    response = (this->serialL)->readStringUntil('\n');
    readResponse(5,NO_DEBUG);
    SERIAL_D("# Timeout reached.\n");
    return RET_TIMEOUT;
  } else {
    bool userTest = false;
    while (userTest == false){
    if ((this->serialD)->available() > 0) {
        char userInput = (this->serialD)->read(); 
        if (userInput == 't' || userInput == 'T') {
          SERIAL_L("WAKE-UP");
          response = (this->serialL)->readStringUntil('\n');
          readResponse(5,NO_DEBUG);
          userTest = true;
          SERIAL_D("# User input 't'.\n");
          return RET_T;
        }
      }
    }
  }
}

/// @brief Allow to change frequency band
void LORAE5::setFrequencyBand(){
  String response;

  if ((this->region) == EU868){
    SERIAL_L("AT+DR=EU868");
    response = (this->serialL)->readStringUntil('\n');
  }

  if ((this->region) == US915){
    SERIAL_L("AT+DR=US915");
    response = (this->serialL)->readStringUntil('\n');
  }
  
  if ((this->region) == US915HYBRID){
    SERIAL_L("AT+DR=US915HYBRID");
    response = (this->serialL)->readStringUntil('\n');
  }

  if ((this->region) == CN779){
    SERIAL_L("AT+DR=CN779");
    response = (this->serialL)->readStringUntil('\n');
  }

  if ((this->region) == EU433){
    SERIAL_L("AT+DR=EU433");
    response = (this->serialL)->readStringUntil('\n');
  }

  if ((this->region) == AU915){
    SERIAL_L("AT+DR=AU915");
    response = (this->serialL)->readStringUntil('\n');
  }

  if ((this->region) == AU915OLD){
    SERIAL_L("AT+DR=AU915OLD");
    response = (this->serialL)->readStringUntil('\n');
  }

  if ((this->region) == CN470){
    SERIAL_L("AT+DR=CN470");
    response = (this->serialL)->readStringUntil('\n');
  }

  if ((this->region) == CN470PREQUEL){
    SERIAL_L("AT+DR=CN470PREQUEL");
    response = (this->serialL)->readStringUntil('\n');
  }

  if ((this->region) == AS923){
    SERIAL_L("AT+DR=AS923");
    response = (this->serialL)->readStringUntil('\n');
  }

  if ((this->region) == KR920){
    SERIAL_L("AT+DR=KR920");
    response = (this->serialL)->readStringUntil('\n');
  }

  if ((this->region) == IN865){
    SERIAL_L("AT+DR=IN865");
    response = (this->serialL)->readStringUntil('\n');
  }

  if ((this->region) == RU864){
    SERIAL_L("AT+DR=RU864");
    response = (this->serialL)->readStringUntil('\n');
  }

  if ((this->region) == STE920){
    SERIAL_L("AT+DR=STE920");
    response = (this->serialL)->readStringUntil('\n');
  }

  #if DEBUG_LEVEL == 5
  SERIAL_L("AT+CH");
  response = (this->serialL)->readStringUntil('\n');
  SERIAL_D(response);
  #endif
}