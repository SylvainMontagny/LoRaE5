#include <arduino.h>
#include <stdio.h>
#include <string.h>
#include "lorae5.h"


// Constants
const uint16_t TIMEOUT_1s = 1000; 
const uint16_t TIMEOUT_10s = 10000; 

struct COMMANDES CMD[22] = {
    { AT,             "AT",               "+AT: OK",            "NO_ENDING",                  TIMEOUT_1s  },
    { RESET,          "AT+RESET",         "+RESET: OK",         "NO_ENDING",                  TIMEOUT_1s  },
    { ADR_ON,         "AT+ADR=ON",        "+ADR: ON",           "NO_ENDING",                  TIMEOUT_1s  },   
    { ADR_OFF,        "AT+ADR=OFF",       "+ADR: OFF",          "NO_ENDING",                  TIMEOUT_1s  },
    { JOIN,           "AT+JOIN",          "+JOIN: Start",       "+JOIN: Network joined",      TIMEOUT_10s },
    { DR,             "AT+DR=",           "+DR: DR*",           "+DR: ***** DR1 SF** BW***",  TIMEOUT_1s },
    { MSG,            "AT+MSG=",          "+MSG: Start",        "+MSG: Done",                 TIMEOUT_10s },
    { CMSG,           "AT+CMSG=",         "+CMSG: Start",       "+CMSG: Done",                TIMEOUT_10s },
    { ABP,            "AT+MODE=ABP",      "+MODE: LWABP",       "NO_ENDING",                  TIMEOUT_1s  },
    { OTAA,           "AT+MODE=LWOTAA",   "+MODE: LWOTAA",      "NO_ENDING",                  TIMEOUT_1s  },
    { PORT,           "AT+PORT=",         "+PORT:",             "SPE",                        TIMEOUT_1s  },
    { DEVEUI,         "AT+ID=DEVEUI,",    "+ID: DevEui",        "SPE",                         TIMEOUT_1s  },
    { APPKEY,         "AT+KEY=APPKEY,",   "+KEY: APPKEY",       "SPE",                         TIMEOUT_1s  },
    { APPEUI,         "AT+ID=APPEUI,",    "+ID: AppEui",       "SPE",                         TIMEOUT_1s  },
    { DEVADDR,        "AT+ID=DEVADDR,",   "+ID: DevAddr",      "SPE",                         TIMEOUT_1s  },
    { NWKSKEY,        "AT+KEY=NWKSKEY,",  "+KEY: NWKSKEY",     "SPE",                         TIMEOUT_1s  }, 
    { APPSKEY,        "AT+KEY=APPSKEY,",  "+KEY: APPSKEY",     "SPE",                         TIMEOUT_1s  }
};

/// 
///  CONSTRUCTOR
///

LoraE5::LoraE5(
    const uint8_t sf, 
    const bool    adr,
    bool          confirmed,     
    uint8_t       nbPort,          
    bool          button,
    uint64_t      delay, 
    bool          hello,
    bool          isOtaa,
    char *        devEUI, 
    char *        devAddr, 
    char *        nwkSKey, 
    char *        appSKey, 
    char *        appKey, 
    char *        appEUI
){
  this->sf=sf;
  this->adr=adr;
  this->isConfirmed=confirmed;
  this->port=nbPort;
  this->pushButton=button;
  this->frameDelay=delay;
  this->hello=hello;
  this->isOtaa=isOtaa;
  this->devEUI=devEUI;
  this->devAddr=devAddr;
  this->nwkSKey=nwkSKey;
  this->appSKey=appSKey;
  this->appKey=appKey;
  this->appEUI=appEUI;

  this->sendOne=false;
}

// Information are printed via Serial
void LoraE5::debugFunction(char* info, uint8_t lvl){
  switch(__LEVEL_DEBUG){
    case 0:
      break;
    case 1:
      if(lvl==1){
        Serial.println(info);
      }
      break;
    case 2 :
      if((lvl==1)|(lvl==2)){
        Serial.println(info);
      }
      break;
    case 3 : 
      Serial.println(info);
      break;
    default : 
      Serial.println("wrong lvl debug");
      break;
  }
}

void LoraE5::sendMsg(){
    sendMsg(this->isConfirmed,"HELLO");
}

void LoraE5::sendMsg(bool confirmed, char* payload){
  char msg[30];
  if(confirmed){
    sprintf(msg, "\nUplink CONFIRMED on port %d ", getPort());
    debugFunction(msg, 1);
    Serial.print("Payload: ");
    debugFunction(payload, 1);
    process(CMD[CMSG], payload);
  }
  else{
    sprintf(msg, "\nUplink UNCONFIRMED on port %d ", getPort());
    debugFunction(msg, 1);
    Serial.print("Payload: ");
    debugFunction(payload, 1);
    process(CMD[MSG],payload);
  }
}

bool LoraE5::process(COMMANDES cmd, char* cmd_to_add){
  bool ret = false;
  sendATcmd(cmd, cmd_to_add);
  readResponse(cmd);
  //ret=checkResponse(cmd);
  memset(this->bufResponse,0, this->respIndex);
  this->respIndex= 0;
  ret=true;
  return ret;
}

void LoraE5::sendATcmd(COMMANDES sendCmd, char* cmd_to_add){
  char commande[60];
  strcpy(commande, sendCmd.buf_cmd);
  strcat(commande, cmd_to_add);
  Serial1.println(commande);
  this->timeOut = sendCmd.time_out;
}

bool LoraE5::readResponse(COMMANDES cmd){
  uint32_t tdelay = this->timeOut;
  uint32_t tstart = millis();
  this->respIndex = 0;

  while (millis()-tstart < tdelay){
    if(Serial1.available() > 0) {
      char c = Serial1.read(); 
      //this->bufResponse[respIndex] = c;
      Serial.print(c);
      //this->respIndex++;
    }       
  }
  return false;
}

bool LoraE5::checkResponse(COMMANDES cmd){
  bool ret = false;
  if(startsWith(this->bufResponse, "+JOIN: Joined already")){
    isJoined = true;
    return isJoined;
  }
  else if(indexOf(this->bufResponse, "+JOIN: Join failed")>0){
    isJoined = false;
    return isJoined;
  }

  if(startsWith(this->bufResponse, cmd.buf_ok)){
    if(startsWith(cmd.buf_ending, "NO_ENDING")){
      debugFunction("\n No ending", 2);
      ret = true;
    }
    else if(startsWith(cmd.buf_ending,"+JOIN: Network joined")){
      debugFunction(this->bufResponse, 1); 
      isJoined = true; 
      ret=true;
    }
    else if(indexOf(this->bufResponse, cmd.buf_ending)>0){ // for MSG and CMSG
      //if(!this->sendOne){
        //debugFunction(this->bufResponse, 1); // No space available for more than 1 message in console
        sendOne=true;
      //}
      ret=true;
    }
    
    else{ 
      ret = true;
    }
  }
  else if(startsWith(cmd.buf_ending, "+MSG: Done")||startsWith(cmd.buf_ending, "+CMSG: Done")){ // no reception = no error for MSG
    ret=true;
  }
  else if(startsWith(cmd.buf_ending, "SPE")){
    ret = true;
  }
  else{
    debugFunction("ERROR RESPONSE", 1);
    debugFunction(this->bufResponse,1);
    ret = false;
  }
  return ret;
}

int LoraE5::indexOf(const char * str, const char * ref) {
  int sStr = strlen(str);
  int sRef = strlen(ref);
  int e;
  for ( int d = 0 ; d < (sStr - sRef) ; d++ ) {
    if ( str[d] == ref[0] ) {
      for ( e = 1 ; e < sRef ; e++ ) {
        if ( str[d+e] != ref[e] ) {
          break;
        }
      }
      if ( e == sRef ) {
        return d+e;
      }
    }
  }
  return -1;
}

bool LoraE5::startsWith(const char * str, const char * ref) {
  if ( strlen(str) >= strlen(ref) ) {
    // possible 
    int i;
    for ( i = 0 ; i < strlen(ref) ; i++ ) {
        if ( ref[i] != '*' && str[i] != ref[i] ) {
                break;
        }
    }
    return ( i == strlen(ref) );
  }
  return false;
}

void LoraE5::setATConfig(){
  setDR(this->sf);
  setADR(this->adr);
  setPORT(this->port);
  setDevEUI(this->devEUI);
  if(setMode(this->isOtaa)){
    setAppKey(this->appKey);  
    setAppEUI(this->appEUI);
  }
  else{
    setDevAddr(this->devAddr);  
    setNwkSKey(this->nwkSKey);
    setAppSKey(this->appSKey);  
  }
}

bool LoraE5::setDR(uint8_t sf){
  bool ret = true;
  uint8_t dr = 0;
  char cmd[30];
  this->sf = sf;
  switch(sf){
    case 7: 
      dr=5;
      break;
    case 8: 
      dr=4;
      break;
    case 9: 
      dr=3;
      break;    
    case 10: 
      dr=2;
      break;
    case 11: 
      dr=1;
      break;
    case 12: 
      dr=0;
      break;
    default:
      ret = false;
      break;
  }
  sprintf(cmd,"DR%d", dr);
  process(CMD[DR],cmd);
}

void LoraE5::setADR(bool adr){
  this->adr = adr;
  if(adr){
    process(CMD[ADR_ON], "");
  }
  else{
    process(CMD[ADR_OFF], "");
  }
}

void LoraE5::setPORT(uint8_t port){
  this->port = port;
  char cmd_port[10];
  sprintf(cmd_port, "%d", getPort());
  process(CMD[PORT], cmd_port);
}

bool LoraE5::setMode(bool otaa){
  if(otaa){
    process(CMD[OTAA], "");
  }
  else{
    process(CMD[ABP], "");
  }
  return otaa; // 1 if otaa / 0 if ABP
}

void LoraE5::setDevEUI(char* deveui){
  if(strlen(deveui) != 16){
    debugFunction("Invalid devEUI", 1);
  }
  this->devEUI = deveui;
  process(CMD[DEVEUI], deveui); 
}

void LoraE5::setAppKey(char* appkey){
  if(strlen(appkey) != 32){
    debugFunction("Invalid AppKey", 1);
  }
  if(!this->isOtaa){
    debugFunction("appEUI != ABP", 1);
  }
  this->appKey = appkey;
  process(CMD[APPKEY], appkey);
}

void LoraE5::setAppEUI(char* appeui){
  if(strlen(appeui) != 16){
    debugFunction("Invalid AppEUI", 1);
  }
  if(!this->isOtaa){
    debugFunction("appEUI != ABP", 1);
  }
  this->appEUI = appeui;
  process(CMD[APPEUI], appeui);
}

void LoraE5::setDevAddr(char* devaddr){
  if(strlen(devaddr)!= 8){
    debugFunction("Invalid DevAddr", 1);
  }
  if(this->isOtaa){
    debugFunction("devAddr != OTAA", 1);
  }
  this->devAddr = devaddr;
  process(CMD[DEVADDR], devaddr);
}

void LoraE5::setNwkSKey(char* nwkskey){
  if(strlen(nwkskey)!= 32){
    debugFunction("Invalid NwkSKey", 1);
  }
  if(this->isOtaa){
    debugFunction("nwkSKey != OTAA", 1);
  }
  this->nwkSKey = nwkskey;
  process(CMD[NWKSKEY], nwkskey);
}

void LoraE5::setAppSKey(char* appskey){
  if(strlen(appskey) != 32){
    debugFunction("Invalid AppSKey", 1);
  }
  if(this->isOtaa){
    debugFunction("AppSKey != OTAA", 1);
  }
  this->appSKey = appskey;
  process(CMD[APPSKEY], appskey);
}

bool LoraE5::join(){
  debugFunction("\nJoin Request: ...", 1);
  if(this->isOtaa){
    return(process(CMD[JOIN], ""));
  }
}

uint8_t LoraE5::getSF(){
  return this->sf;
}

bool LoraE5::getADR(){
  return this->adr;
}

uint8_t LoraE5::getPort(){
  return this->port;
}


// Info message

void LoraE5::printInfo(){
  
  Serial.println("# LoRaWAN Training Session #");
  Serial.println("######## LoRa E5 HT ########");
  
  if(this->isOtaa){
    Serial.println("\n * Mode: OTAA");
  }else{
    Serial.println("\n * Mode: ABP");
  }
  
  if(this->pushButton){
    Serial.println("\n * Push Button: ON");
  }else{
    char frame[20];
    sprintf(frame, " * Frame Delay: %ds", this->frameDelay/1000);
    Serial.println(frame);
  }
  
  char spreading[10];
  sprintf(spreading," * SF%d", getSF());
  Serial.println(spreading);
  
  if(this->adr){
    Serial.println(" * ADR: ON");
  }
  else{
    Serial.println(" * ADR: OFF");
  }
  
  if (this->isConfirmed){
    Serial.println(" * Confirmed");
  }
  else{
    Serial.println(" * Unconfirmed");
  }
  
  char numPort[10];
  sprintf(numPort, " * PORT: %d", getPort());
  Serial.println(numPort);
  
  //if(this->isOtaa){join()? debugFunction("Join: SUCCESS !", 1):debugFunction("Join: FAILED ! Retry... ", 1);}
}

/*
void LoraE5::printCredentials(){
  // Payload is Text : -100   
  /*
  char dev_EUI[60] = strcat(" * device EUI : ", this->devEUI);
  char app_Key[80] = strcat(" * application key : ", this->appKey);
  char app_EUI[80] = strcat(" * application EUI : ", this->appEUI);
  Serial.println(dev_EUI);
  Serial.println(app_Key);
  Serial.println(app_EUI);
  */
  /*
  if(this->isOtaa){
    Serial.println(strcat(" * application key : ", this->appKey) );
    Serial.println(strcat(" * application EUI : ", this->appEUI) );
  }else{
    Serial.println(strcat(" * device address : ", this->devAddr) );
    Serial.println(strcat(" * network session key : ", this->nwkSKey) );
    Serial.println(strcat(" * application session key : ", this->appSKey) );
  }
  
}
*/
