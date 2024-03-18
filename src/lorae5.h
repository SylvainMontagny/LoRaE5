#define USB_Serial    Serial
#define LoRa_Serial   Serial1

#define ABP                     0 
#define OTAA     		            1

#define CLASS_A                 0
#define CLASS_B                 1
#define CLASS_C                 2

#define DEBUG                   true
#define NO_DEBUG                false

#define CONF                    true
#define UNCONF                  false

#define _HEXA                   0
#define _STRING                 1                                       


class LORAE5{

public:
   // uint8_t       downlink[50];

private: 
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
         
    void          readResponse(uint32_t timeOut, bool debug);
    bool          checkResponse(uint32_t timeOut, char *strCheck, bool debug);
    void          displayPayloadUp(uint8_t* payloadUp, uint8_t sizePayloadUp);
    
public:
                  LORAE5(String devEUI, String appEUI, String appKey, String devAddr, String nwkSKey, String appSKey);
    void          setup(bool mode, uint8_t devClass, uint8_t sf, bool adr, bool messageType, uint8_t portUp);
    void          printInfo(bool sendByPushButton, uint32_t frameDelay);
    bool          checkBoard(void);

    bool          sendData(uint8_t* payloadUp, uint8_t sizePayloadUp, uint8_t* payloadDown, uint8_t* sizeDownlink);
    void          sendData(uint8_t* payloadUp, uint8_t sizePayloadUp);
    void          awaitNextTransmission(uint32_t delay, bool pushButton);
    void          setDevEUI(String deveui);
    void          setAppKey(String appkey);
    void          setAppEUI(String appeui);
    void          setDevAddr(String devaddr);
    void          setNwkSKey(String nwkskey);
    void          setAppSKey(String appskey);
    bool          join(void);

    void          setMode(bool mode);
    void          getMode(void);
    void          setClass(uint8_t devClass);
    uint8_t       getClass(void);
    void          setRXDelay();
    void          setSF(uint8_t sf);
    void          getSetSF(void);
    void          setADR(bool adr);
    uint8_t       getADR(void);
    void          setPortUp(uint8_t portUp);
    uint8_t       getPortUp(void);
    void          getPortDown(uint8_t portDown);   
};
