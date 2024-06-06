
#ifndef __APPDATA_H
#define __APPDATA_H

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#define macroUSE_SDCARD

/******************************* Define connectivity **************************/
#define macroCONNECTIVITY_ETH 					 		"Ethe"
#define macroCONNECTIVITY_WIFI 					 		"Wifi"
#define macroCONNECTIVITY_3G 					 		"3G"
#define macroCONNECTIVITY_2G 					 		"2G"
#define macroCONNECTIVITY_ZIGB 					 		"Zigb"
#define macroCONNECTIVITY_SUB1G							"Sub1"
/******************************************************************************/


//Dinh nghia do dai cua ban tin String
#define macroPACKET_STRING_MAX_FIELD					17
#define macroPACKET_STRING_MIN_FIELD					4
#define macroPACKET_STRING_MAX_FIELD_LENGHT				32
#define macroPACKET_STRING_ENDCHAR						'!'
#define macroPACKET_STRING_MAX_LENGHT					(macroPACKET_STRING_MAX_FIELD * macroPACKET_STRING_MAX_FIELD_LENGHT + macroPACKET_STRING_MAX_FIELD)


//Dinh nghia do dai cua ban tin Json
#define macroPACKET_JSON_MAX_FIELD						23
#define macroPACKET_JSON_MIN_FIELD						10
#define macroPACKET_JSON_MAX_FIELD_LENGHT				32
#define macroPACKET_JSON_MAX_LENGHT						(macroPACKET_JSON_MAX_FIELD * macroPACKET_JSON_MAX_FIELD_LENGHT + macroPACKET_JSON_MAX_FIELD * 3 + 4)

#define macroJSON_TYPE_PACKET							"TypePacket"
#define macroJSON_DEVICE_IDEP							"DeviceIDEp"
#define macroJSON_DEVICE_IDGW							"DeviceIDGw"
#define macroJSON_TYPE_DEVICE							"TypeDevice"
#define macroJSON_DATA									"Data"


//gw
#define macroAPP_TYPE_DEVICE_GW							"SMF_GW"
//sensor node
#define macroAPP_TYPE_DEVICE_SENSOR_NODE				"SMF_SENSN"
//control node
#define macroAPP_TYPE_DEVICE_CONTROL_NODE				"SMF_CTRLN"

//Type packet
#define macroTYPE_PACKET_DATA		 					"DATA"
#define macroTYPE_PACKET_CONF 							"CONF"
#define macroTYPE_PACKET_REQS 					 		"REQS"
#define macroTYPE_PACKET_CTRL 					 		"CTRL"
#define macroTYPE_PACKET_RESP 					 		"RESP"
#define macroTYPE_PACKET_WAMI 					 		"WAMI"

//ID Data
#define macroID_DATA_SENSOR_PH							"PH"
#define macroID_DATA_SENSOR_EC							"EC"
#define macroID_DATA_SENSOR_TEMP_G						"TempG"
#define macroID_DATA_SENSOR_HUMI_G						"HumiG"
#define macroID_DATA_SENSOR_TEMP_A						"TempA"
#define macroID_DATA_SENSOR_HUMI_A						"HumiA"
#define macroID_DATA_SENSOR_LIGHT						"Light"
#define macroID_DATA_CTRL_STATUS						"CtrlStat"
#define macroID_DATA_DEVICE_STATE						"D_State"
#define macroID_DATA_TIME								"Time"

//ID Config
#define macroID_CONF_DATA_UPDATE_TIME					"DataUT"
#define macroID_CONF_MQTT								"MQTT"
#define macroID_CONF_MQTT_ETH							"MQTT_ETH"
#define macroID_CONF_SMC_WIFI							"SMConfig"
#define macroID_CONF_AUTO_OUTVAC_1						"OutVAC_1"
#define macroID_CONF_AUTO_OUTVAC_2						"OutVAC_2"
#define macroID_CONF_AUTO_OUTVAC_3						"OutVAC_3"
#define macroID_CONF_AUTO_OUTVAC_4						"OutVAC_4"
#define macroID_CONF_RTC								"RTC"
#define macroID_CONF_IDGW								"IDGW"
#define macroID_CONF_THRESHOLD_PH						"PH"
#define macroID_CONF_THRESHOLD_EC						"EC"
#define macroID_CONF_THRESHOLD_TEMP_G					"TempG"
#define macroID_CONF_THRESHOLD_HUMI_G					"HumiG"
#define macroID_CONF_THRESHOLD_TEMP_A					"TempA"
#define macroID_CONF_THRESHOLD_HUMI_A					"HumiA"
#define macroID_CONF_THRESHOLD_LIGHT					"Light"

//ID Request
#define macroID_REQS_ALL								"ALL"
#define macroID_REQS_RTC								"RTC"
#define macroID_REQS_IDGW								"IDGW"

//ID Control
#define macroCTRL_OUTPUT_VAC							"OutVAC"

//ID Response
#define macroRESP_OK 						 			"OK"
#define macroRESP_NOK 						 			"NOK"


typedef struct
{
	bool			bPacketIsOK;
	bool			bPacketIsString;

	uint8_t			TypePacket[8];
	uint8_t			TypeDevice[16];
	uint8_t			IDEd[17];
	uint8_t			IDGw[17];
	uint8_t			IDZb[17];
	struct
	{
		uint8_t		Name[macroPACKET_JSON_MAX_FIELD_LENGHT / 2];
		uint8_t		Info[macroPACKET_JSON_MAX_FIELD_LENGHT];
	}Data[((macroPACKET_JSON_MAX_FIELD - macroPACKET_JSON_MIN_FIELD) / 2) + 1];
}PacketIO;



typedef struct
{
	struct
	{
		bool	Ethernet;
		bool	Wifi;
		bool	_3G;
		bool	_2G;
		bool	ConnectionOK;
	}Network;

	struct
	{
		bool	Zigbee;
		bool	Sub1Ghz;
	}Connectivity;
}WhoAmI;



#endif







