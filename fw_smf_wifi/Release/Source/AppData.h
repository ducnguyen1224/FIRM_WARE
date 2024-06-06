
#ifndef __APPDATA_H
#define __APPDATA_H

#include <SmingCore.h>

//define String packet
#define PACKET_STRING_MAX_FIELD				17
#define PACKET_STRING_MAX_FIELDLENGHT		32
//#define PACKET_STRING_MIN_FIELD             4
//#define PACKET_STRING_MAX_LENGHT			(PACKET_STRING_MAX_FIELD * PACKET_STRING_MAX_FIELDLENGHT + PACKET_STRING_MAX_FIELD)
/*(Packet lenght = so truong * do dai moi truong + so ky tu ngan cach , va !)*/


//define Json Packet
#define PACKET_JSON_MAX_FIELD				23
#define PACKET_JSON_MAX_FIELDLENGHT			32
#define PACKET_JSON_MIN_FIELD               10
//#define PACKET_JSON_MAX_LENGHT				(PACKET_JSON_MAX_FIELD * PACKET_JSON_MAX_FIELDLENGHT + PACKET_JSON_MAX_FIELD * 3 + 4 - 1)
/*(Packet lenght = so truong * do dai moi truong + so truong * so ky tu ngan cach cua moi truong + 4 dau {} - 1 dau phay cuoi ban tin)*/


//Dinh nghia Keywork in Json Packet
//#define JSON_TYPE_PACKET                 	"TypePacket"
//#define JSON_DEVICE_IDEP                 	"DeviceidEp"
//#define JSON_DEVICE_IDGW                 	"DeviceidGw"
//#define JSON_TYPE_DEVICE                 	"TypeDevice"
//#define JSON_DATA                       	"Data"

//Type device gw
#define macroTYPE_DEVICE_GW					"SMF_GW"
//type device sensor node
#define macroTYPE_DEVICE_SENSOR_NODE		"SMF_SENSN"
//type device control node
#define macroTYPE_DEVICE_CONTROL_NODE		"SMF_CTRLN"

//Type packet
#define macroTYPE_PACKET_DATA				"DATA"
#define macroTYPE_PACKET_CONF				"CONF"
#define macroTYPE_PACKET_REQS				"REQS"
#define macroTYPE_PACKET_CTRL				"CTRL"
#define macroTYPE_PACKET_WAMI				"WAMI"
#define macroTYPE_PACKET_RESP				"RESP"

//ID Data
//#define macroID_DATA_SENSOR_PH				"PH"
//#define macroID_DATA_SENSOR_EC				"EC"
//#define macroID_DATA_SENSOR_TEMP_GROUND		"TempG"
//#define macroID_DATA_SENSOR_HUMI_GROUND		"HumiG"
//#define macroID_DATA_SENSOR_TEMP_AIR		"TempA"
//#define macroID_DATA_SENSOR_HUMI_AIR		"HumiA"
//#define macroID_DATA_SENSOR_LIGHT			"Light"
//#define macroID_DATA_CTRL_STATUS			"CtrlStat"
//#define macroID_DATA_DEVICE_STATE			"D_State"
#define macroID_DATA_TIME_SEND				"Time"

//ID Config
#define macroID_CONF_DATA_UPDATE_TIME		"DataUT"
#define macroID_CONF_MQTT					"MQTT"
#define macroID_CONF_WIFI					"Wifi"
#define macroID_CONF_TIME_AUTO_VAC_1		"OutVAC_1"
#define macroID_CONF_TIME_AUTO_VAC_2		"OutVAC_2"
#define macroID_CONF_TIME_AUTO_VAC_3		"OutVAC_3"
#define macroID_CONF_TIME_AUTO_VAC_4		"OutVAC_4"
#define macroID_CONF_RTC					"RTC"
#define macroID_CONF_ID_GW					"IDGW"
#define macroID_CONF_THRES_PH				"PH"
#define macroID_CONF_THRES_EC				"EC"
#define macroID_CONF_THRES_TEMP_G			"TempG"
#define macroID_CONF_THRES_HUMI_G			"HumiG"
#define macroID_CONF_THRES_TEMP_A			"TempA"
#define macroID_CONF_THRES_HUMI_A			"HumiA"
#define macroID_CONF_THRES_LIGHT			"Light"

//ID Request
#define macroID_REQS_ALL					"ALL"
#define macroID_REQS_RTC					"RTC"
#define macroID_REQS_IDGW					"IDGW"

//ID Control
#define macroID_CTRL_OUT_VAC				"OutVAC"

//ID Connectivity
#define macroID_CONNECTIVITY_WIFI			"Wifi"

//ID Response
#define macroID_RESP_OK						"OK"
#define macroID_RESP_NOK					"NOK"


typedef enum
{
	eNone = 0,
	eData,
	eConf,
	eReqs,
	eCtrl,
	eResp,
	eWami,
}Enum_TypePacket;



typedef struct
{
	bool			PacketIsOK;
	bool			PacketIsString;
	Enum_TypePacket isTypePacket;
	bool			isOf_GwDevice;

	String			TypePacket;
	String			TypeDevice;
	String			IDEd;
	String			IDGw;

	struct
	{
		String		Name;
		String		Info;
	}Data[((PACKET_JSON_MAX_FIELD - PACKET_JSON_MIN_FIELD) / 2) + 1];
}PacketIO;




#endif
















