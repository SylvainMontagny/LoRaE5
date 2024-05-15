#define SERIAL_D(x)   this->serialD->println(x)
#define SERIAL_L(x)   this->serialL->println(x)

#define EU868                   0
#define US915                   1
#define CN779                   2
#define EU433                   3
#define AU915                   4
#define CN470                   5
#define AS923                   6
#define KR920                   7
#define IN865                   8
#define RU864                   9
#define CN470PREQUEL            10
#define STE920                  11
#define US915HYBRID             12
#define AU915OLD                13

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

#if defined(ARDUINO_AVR_LEONARDO) || defined(ARDUINO_WIO_TERMINAL)
    Serial_* serialD;
#else
    HardwareSerial* serialD;
#endif

#if defined(ARDUINO_WIO_TERMINAL)
    Uart* serialL;
#else
    HardwareSerial* serialL;
#endif

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

#if defined(ARDUINO_AVR_LEONARDO)
    void          setup_hardware(Serial_* myDebugSerial, HardwareSerial* myLoRaSerial);
#elif defined(ARDUINO_WIO_TERMINAL)
void          setup_hardware(Serial_* myDebugSerial, Uart* myLoRaSerial);
#else
    void          setup_hardware(HardwareSerial* myDebugSerial, HardwareSerial* myLoRaSerial);
#endif

    void          setup_lorawan(uint8_t band, bool mode, uint8_t devClass, uint8_t sf, bool adr, bool messageType, uint8_t portUp, bool SEND_BY_PUSH_BUTTON, uint32_t FRAME_DELAY);
    void          printInfo(void);

    bool          join(void);

    void          sendData(uint8_t* payloadUp, uint8_t sizePayloadUp);
    uint8_t       awaitForDownlinkClass_A(uint8_t* payloadDown, uint8_t* sizePayloadDown);
    uint8_t       awaitForDownlinkClass_C(uint8_t* payloadDown, uint8_t* sizePayloadDown);
    uint8_t       sleep(void);
    
};