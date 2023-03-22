#define ABP                     0 
#define OTAA     		            1

#define ACTIVATION_MODE     		OTAA
#define CLASS						        CLASS_A
#define SPREADING_FACTOR    		12
#define ADAPTIVE_DR         		true
#define CONFIRMED           		true
#define PORT                		15

#define SEND_BY_PUSH_BUTTON 		false
#define FRAME_DELAY         		10000
#define PAYLOAD_HELLO				    true
#define PAYLOAD_TEMPERATURE    	false
#define CAYENNE_LPP_         		false
#define LOW_POWER           		false

const char devEUI[] = "70B3D57ED005ADFE";

// Configuration for ABP Activation Mode
const char devAddr[] = "260BF055";
const char nwkSKey[] = "FD3B51331776AE8374B7764512871E8E";
const char appSKey[] = "FFAE2922DCB659789C896BDD479B5F1A";

// Configuration for OTAA Activation Mode
const char appKey[] = "3477563246F9BAF537FBE195A3931AF3"; 
const char appEUI[] = "0000000000000000";
