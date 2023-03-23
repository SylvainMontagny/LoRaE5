#define ABP                     0 
#define OTAA     		            1

#define ACTIVATION_MODE     		OTAA
#define CLASS						        CLASS_A
#define SPREADING_FACTOR    		7
#define ADAPTIVE_DR         		false
#define CONFIRMED           		false
#define PORT                		15

#define SEND_BY_PUSH_BUTTON 		false
#define FRAME_DELAY         		10000
#define PAYLOAD_HELLO				    true
#define PAYLOAD_TEMPERATURE    	false
#define CAYENNE_LPP_         		false
#define LOW_POWER           		false

const char devEUI[] = "0000000000000000";

// Configuration for ABP Activation Mode
const char devAddr[] = "00000000";
const char nwkSKey[] = "00000000000000000000000000000000";
const char appSKey[] = "00000000000000000000000000000000";

// Configuration for OTAA Activation Mode
const char appKey[] = "00000000000000000000000000000000"; 
const char appEUI[] = "0000000000000000";
