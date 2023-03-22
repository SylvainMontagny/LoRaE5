#include <stdint.h>

#ifndef __LORAE5_H__
#define __LORAE5_H__

#define __ATRESP_BUFF_SIZE         128         // max size to store a response from an AT command

#define __LEVEL_DEBUG              1           // from 0 to 3 with 3 the maximum level of debugging
                                               // 0: print nothing / 3: print everything

// ==========================================================
// LoRaE5

enum Commande {
    AT,
    RESET,
    ADR_ON,
    ADR_OFF,
    JOIN,
    DR,
    MSG,
    CMSG,
    ABP,
    OTAA,
    PORT,         // value added later
    DEVEUI,
    APPKEY,       // value added later
    APPEUI,       // value added later
    DEVADDR,      // value added later
    NWKSKEY,      // value added later
    APPSKEY       // value added later
};

struct COMMANDES {
    enum Commande cmd;
    char*         buf_cmd;
    char*         buf_ok;
    char*         buf_ending;
    uint16_t      time_out;
    bool          needMore;
};

class LoraE5{
private: 
    uint8_t       sf;             // Spreading Factor
    bool          adr;            // Adaptive Data Rate
    bool          isConfirmed;    // 1: confirmed trame 2: unconfirmed
    uint8_t       port;           
    bool          pushButton;     // 1 : pushButton / 0 : automatically each frame delay chosed
    uint64_t      frameDelay;
    bool          hello;
    bool          isOtaa;       // OTAA = 1, ABP = 0
    char *        devEUI;         // for both modes
    char *        devAddr;        // only in ABP mode
    char *        nwkSKey;        // only in ABP mode
    char *        appSKey;        // only in ABP mode
    char *        appKey;         // only in OTAA mode
    char *        appEUI;         // only in OTAA mode


    bool          sendOne;
    bool          isJoined;
    char          bufResponse[__ATRESP_BUFF_SIZE]; 
    uint16_t      respIndex;
    uint16_t      timeOut;

    void debugFunction(char* info, uint8_t lvl);
    // SendAT & ReadAT
    bool          process(COMMANDES cmd, char* cmd_to_add);
    void          sendATcmd(COMMANDES sendCmd, char* cmd_to_add); // new one
    bool          readResponse(COMMANDES cmd);
    bool          checkResponse(COMMANDES cmd);
    bool          startsWith(const char * str, const char * ref);
    int           indexOf(const char * str, const char * ref);

    
    uint64_t      delay(uint64_t previousTime);
    
    uint8_t       getSF(void);
    bool          getADR(void);
    uint8_t       getPort(void);

public:
    // CONSTRUCTOR
    LoraE5(
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
    );

    // SETUP 
    void          setATConfig(void);
    void          testProgram(void);
    // MORE
    void          sendMsg(void);
    void          sendMsg(bool confirmed, char* payload);
    void          printInfo(void);


    // SET CONFIG
    bool          setDR(uint8_t dr);
    void          setADR(bool adr);
    void          setPORT(uint8_t port);
    bool          setMode(bool otaa);
    void          setDevEUI(char* deveui);
    void          setAppKey(char* appkey);
    void          setAppEUI(char* appeui);
    bool          join(void);
    void          setDevAddr(char* devaddr);
    void          setNwkSKey(char* nwkskey);
    void          setAppSKey(char* appskey);
    
};

#endif          // __LORAE5_H__ 
