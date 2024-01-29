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

#define _HEXA                     0
#define _STRING                  1                                       


class LORAE5{

public:
   // uint8_t       downlink[50];

private: 
    bool          mode;
    uint8_t       sf;
    bool          adr;
    bool          confirmed;
    uint8_t       port;
    uint8_t       portDown;

    String        devEUI;         
    String        devAddr;        
    String        nwkSKey;        
    String        appSKey;       
    String        appKey;         
    String        appEUI;         
         
    void          readResponse(uint32_t timeOut, bool debug);
    bool          checkResponse(uint32_t timeOut, char *strCheck, bool debug);
    
public:
                  LORAE5(String devEUI, String appEUI, String appKey, String devAddr, String nwkSKey, String appSKey);
    void          setup(bool mode, uint8_t devClass, uint8_t sf, bool adr, bool messageType, uint8_t port);
    void          printInfo(bool sendByPushButton, uint32_t frameDelay);
    bool          checkBoard();

    void          sendMsg();
    void          sendMsg(bool msgType, String payload);
    bool          sendData(uint8_t* payload, uint8_t sizeUplink, uint8_t* downlink, uint8_t* sizeDownlink);
    void          setDevEUI(String deveui);
    void          setAppKey(String appkey);
    void          setAppEUI(String appeui);
    void          setDevAddr(String devaddr);
    void          setNwkSKey(String nwkskey);
    void          setAppSKey(String appskey);
    bool          join(void);

    void          setMode(bool mode);
    void          getMode();
    void          setRXDelay();
    void          setSF(uint8_t sf);
    void          getSF();
    void          setADR(bool adr);
    void          getADR();
    void          setPort(uint8_t port);
    void          getPort();   
};
