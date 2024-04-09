#include <stdint.h>
#define USB_Serial    Serial
#define LoRa_Serial   Serial1

#define EU868                   0
#define US915                   1

#define ABP                     0 
#define OTAA     		            1

#define CLASS_A                 0
#define CLASS_B                 1
#define CLASS_C                 2

#define DEBUG                   true
#define NO_DEBUG                false

#define DEBUG_LEVEL             0
/* list for DEBUG_LEVEL:
- join : 1;
- getSetSF : 2;
- sendData : 3;
- awaitForDownlink : 4;
- setFrequencyBand : 5;
*/

#define CONF                    true
#define UNCONF                  false

#define _HEXA                   0
#define _STRING                 1                                       

enum RETURN { RET_DOWNLINK, RET_NO_DOWNLINK, RET_NO_ACK, RET_T, RET_TIMEOUT };

class LORAE5{

public:
   // uint8_t       downlink[50];

private: 
    uint8_t       region;
    bool          mode;
    uint8_t       devClass;
    uint8_t       sf;
    bool          adr;
    bool          confirmed;
    uint8_t       portUp;
    uint8_t       portDown;

    String        devEUI;         
    String        devAddr;        
    String        nwkSKey;        
    String        appSKey;       
    String        appKey;         
    String        appEUI;

    bool          SEND_BY_PUSH_BUTTON;      
    uint32_t      FRAME_DELAY;
         
    void          readResponse(uint32_t timeOut, bool debug);
    bool          checkResponse(uint32_t timeOut, char *strCheck, bool debug);
    bool          checkBoard(void);

    void          setDevEUI(String deveui);
    void          setAppKey(String appkey);
    void          setAppEUI(String appeui);
    void          setDevAddr(String devaddr);
    void          setNwkSKey(String nwkskey);
    void          setAppSKey(String appskey);
    void          setMode(bool mode);
    void          setClass(uint8_t devClass);
    void          setRXDelay();
    void          setSF(uint8_t sf);
    void          setADR(bool adr);
    void          setPortUp(uint8_t portUp);
    void          setFrequencyBand(void);

    void          getMode(void);
    void          getSetSF(void);
    void          getPortDown(String response);
    void          getPayloadDown(uint8_t* payloadDown, uint8_t* sizePayloadDown, String response);
    void          getChannelDown(String response);
    uint8_t       getADR(void);
    uint8_t       getPortUp(void);
    uint8_t       getClass(void);

    void          sendPayloadUp(uint8_t* payloadUp, uint8_t sizePayloadUp);
    
public:
                  LORAE5(String devEUI, String appEUI, String appKey, String devAddr, String nwkSKey, String appSKey);
    void          setup(uint8_t band, bool mode, uint8_t devClass, uint8_t sf, bool adr, bool messageType, uint8_t portUp, bool SEND_BY_PUSH_BUTTON, uint32_t FRAME_DELAY);
    void          printInfo();

    bool          join(void);

    void          sendData(uint8_t* payloadUp, uint8_t sizePayloadUp);
    uint8_t       awaitForDownlinkClass_A(uint8_t* payloadDown, uint8_t* sizePayloadDown);
    uint8_t       awaitForDownlinkClass_C(uint8_t* payloadDown, uint8_t* sizePayloadDown);
    uint8_t       sleep(void);
    
};
