
#define ACTIVATION_MODE     		ABP
#define CLASS						        CLASS_A
#define SPREADING_FACTOR    		7
#define ADAPTIVE_DR         		true
#define CONFIRMED           		false
#define PORT                		15

#define SEND_BY_PUSH_BUTTON 		false
#define FRAME_DELAY         		10000
//#define PAYLOAD_HELLO				    true
//#define PAYLOAD_TEMPERATURE    	false
//#define CAYENNE_LPP_         		false
#define LOW_POWER           		false

String devEUI = "0080E115000A9BD2";

// Configuration for ABP Activation Mode
String devAddr = "260BBBD3";
String nwkSKey = "373BA906ED0C9EAB45767AC4F0454809";
String appSKey = "DA20768C7183F2011F41EF637CA70814";

// Configuration for OTAA Activation Mode
String appKey = "BC673B8F4431ACE4A7B8412209AE1C45"; 
String appEUI = "0000000000000000";
