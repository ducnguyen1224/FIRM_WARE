
#ifndef __APPDATA_H
#define __APPDATA_H

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

/******************************************************************************
define for sensor user
******************************************************************************/



#define macroTIME_READ_SENSOR							11				//2mins
#define macroTIME_SEND_WAMI								7				//7s
#define macroTIME_SEND_INTERVAL							30				//2mins

//Dinh nghia do dai cua ban tin String
#define macroPACKET_STRING_MAX_FIELD					15
#define macroPACKET_STRING_MIN_FIELD					3
#define macroPACKET_STRING_MAX_FIELD_LENGHT				20
#define macroPACKET_STRING_ENDCHAR						'!'
#define macroPACKET_STRING_MAX_LENGHT					(macroPACKET_STRING_MAX_FIELD * macroPACKET_STRING_MAX_FIELD_LENGHT + macroPACKET_STRING_MAX_FIELD)


#define macroTYPEDEVICE_SS							        "SMF_SENSN"
#define macroTYPEDEVICE_GW									"SMF_GW"

//Type packet
#define macroTYPEPACKET_DATA							 "DATA"
#define macroTYPEPACKET_CONF							 "CONF"
#define macroTYPEPACKET_REQS							 "REQS"
#define macroTYPEPACKET_CTRL							 "CTRL"
#define macroTYPEPACKET_RESP							 "RESP"
#define macroTYPEPACKET_WAMI							 "WAMI"


//ID sensor
#define macroSENSOR_PH								 	"PH"
#define macroSENSOR_EC								 	"EC"
#define macroSENSOR_TEMPG								"TempG"
#define macroSENSOR_HUMIG								"HumiG"
#define macroSENSOR_TEMPA								"TempA"
#define macroSENSOR_HUMIA								"HumiA"
#define macroSENSOR_LIGHT								"Light"

//ID Data control
#define macroCTRL_STATUS								"CtrlStat"
#define macroDEV_STATUS									"D_State" 
#define macroOUT_VAC                                    "OutVAC"

//ID Reqs
#define macroREQUEST_ALL								"ALL"
#define macroREQUSET_RTC                            	"RTC"


//ID name Config
#define macroCFG_SENSOR_PH								 	"PH"
#define macroCFG_SENSOR_EC								 	"EC"
#define macroCFG_SENSOR_TEMPG								"TempG"
#define macroCFG_SENSOR_HUMIG								"HumiG"
#define macroCFG_SENSOR_TEMPA								"TempA"
#define macroCFG_SENSOR_HUMIA								"HumiA"
#define macroCFG_SENSOR_LIGHT								"Light"
#define macroCFG_DataUT                                     "DataUT"

#define macroCFG_TIMESEND                                   300


//define for Thresh
#ifdef PH_SENSOR
	#define macroPH_THRESH_HIGH                                 14
	#define macroPH_THRESH_LOW                                  1
#endif

#ifdef EC_SENSOR
	#define macroEC_THRESH_HIGH                                 2
	#define macroEC_THRESH_LOW                                  1
#endif

#ifdef SHTA_SENSOR
	#define macroTEMPA_THRESH_HIGH                              50
	#define macroTEMPA_THRESH_LOW                               0

	#define macroHUMIA_THRESH_HIGH                              100
	#define macroHUMIA_THRESH_LOW                               5
#endif

#ifdef SHTG_SENSOR
	#define macroTEMPG_THRESH_HIGH                              50
	#define macroTEMPG_THRESH_LOW                               0

	#define macroHUMIG_THRESH_HIGH                              100
	#define macroHUMIG_THRESH_LOW                               0
#endif

#ifdef LIGHT_SENSOR
	#define macroLIGHT_THRESH_HIGH                              65000
	#define macroLIGHT_THRESH_LOW                               0
#endif

#define macroCFG_NO_CHANGE_THRESH                           "x"

//Response
#define macroRESPONSE_OK								"OK"
#define macroRESPONSE_NOK								"NOK"



/******************************* Define connectivity **************************/
#define macroCONNECTIVITY_SSB					        "Zigb"
/******************************************************************************/


typedef enum
{
	eNone		= 0xff,
	eALL		= 0x30,
	eTempG		= 0x31,
	eHumiG		= 0x32,
	eTempA		= 0x33,
	eHumiA		= 0x34,
	ePH			= 0x35,
	eEC		    = 0x36,
	eLight		= 0x37,
}Enum_IDSensor;

typedef enum
{
	eToNone = 0x00,
	eToZigB = 0x01,
}Enum_WhoAmI;

typedef enum
{
	eSend_None = 0x00,
	eSend_OK,
	eSend_NOK,
}Enum_SendIsOK;

typedef enum
{
	eZigB = 1
}Enum_Output;


typedef enum
{
	eUART_None,
	eUART_ZigB,
	eUART_EC,
	eUART_PH
}Enum_UART_IO;


typedef enum
{
	eSDCard_None=0x00,
	eSDCard_True,
	eSDCard_False
}Enum_SDCard;


typedef struct
{
	Enum_UART_IO	eData_Recv;						//Co bao nhan duoc du lieu
	
	bool			bData_Send;						//Co gui du lieu
	
	Enum_SendIsOK	eSentIsOK;
	
	Enum_IDSensor	eReadSensor;					//Co doc cam bien
	
	bool 			bWAMItoZigB;
	
	bool			bSendReady;
	
	bool 			bZigbIsConnected;
	
	Enum_SDCard 	eSDCard_Read;
	
	Enum_SDCard		eSDCard_Write;
	
	bool 			bTimeSend;
    
    bool            bSendReqs;
}Flags;



//store data after read sensor
typedef struct
{
#ifdef PH_SENSOR
	float fpH;
#endif
#ifdef EC_SENSOR
	float fEC;
#endif
#ifdef SHTG_SENSOR
	float fTempG;
	float fHumiG;
#endif
#ifdef SHTA_SENSOR
	float fTempA;
	float fHumiA;
#endif
#ifdef LIGHT_SENSOR
	uint16_t fLight;
#endif
}SS_Value;

#endif







