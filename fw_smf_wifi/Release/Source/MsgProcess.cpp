/*
 * MsgProcess.cpp
 *
 *  Created on: Jul 5, 2018
 *      Author: ThaoHuyen
 */
#include "main.h"

#include "MsgProcess.h"
#include "Parse.h"
#include "AppData.h"
#include "MQTTClient.h"
#include "Network/WifiStation.h"


extern String IDGw;
extern MqttConfig __MqttConfig;
extern WifiConfig __WifiConfig;
extern bool bMQTTisConnected;

String UART_TX_Buffer;
String MQTT_TX_Buffer;
bool bIsDataSending = false;




/******************************************************************************
 * Function		: void vMsgProcess_isDATA( PacketIO *xPacketIO )
 * Description	: process data message
 * Param		:
 * 				+ xPacketIO - struct data
 * Return		: None
******************************************************************************/
static void vMsgProcess_isDATA( PacketIO *xPacketIO )
{
//	APP_DEBUG("--- MsgProcess: message is DATA\r\n");
	if(xPacketIO->IDGw.length() < 1)
	{
		//assert(false);
		return;
	}

	if(xPacketIO->PacketIsString == true)
	{
		vParse_Packing_JsonMessage(xPacketIO, &MQTT_TX_Buffer);
		bIsDataSending = true;
		vMain_setEvent(EVENT_STC_SEND);
	}
}




/******************************************************************************
 * Function		: void vMsgProcess_isCONF( PacketIO *xPacketIO )
 * Description	: process config message
 * Param		:
 * 				+ xPacketIO - struct data
 * Return		: None
******************************************************************************/
static void vMsgProcess_isCONF( PacketIO *xPacketIO )
{
	//APP_DEBUG("--- MsgProcess: message is CONF\r\n");

	if(xPacketIO->PacketIsString == true)
	{
		if(xPacketIO->isOf_GwDevice)
		{
			if(xPacketIO->Data[0].Name == macroID_CONF_ID_GW)
			{
				//APP_DEBUG("--- MsgProcess: config is id gw\r\n");
				IDGw = xPacketIO->Data[0].Info;
				APP_DEBUG("--- MsgProcess: ID Gw = %s\r\n", IDGw.c_str());
			}
			else
			{
				//APP_DEBUG("--- MsgProcess: config is not found\r\n");
			}
		}
	}
	else
	{
		if(xPacketIO->Data[0].Name == macroID_CONF_RTC)
		{
			//APP_DEBUG("--- MsgProcess: config is rtc\r\n");
			//APP_DEBUG("--- MsgProcess: rtc = %s\r\n", xPacketIO->Data[0].Info.c_str());
			if(xPacketIO->TypeDevice != macroTYPE_DEVICE_GW)
			{
				//APP_DEBUG("--- MsgProcess: type device config haven't gw\r\n");
				return;
			}
			xPacketIO->IDEd = 0;
			vParse_Packing_StringMessage(xPacketIO, &UART_TX_Buffer);
			vMain_setEvent(EVENT_CTS_SEND);
		}//
		else if( (xPacketIO->Data[0].Name == macroID_CONF_THRES_PH) || (xPacketIO->Data[0].Name == macroID_CONF_THRES_EC) || \
				 (xPacketIO->Data[0].Name == macroID_CONF_THRES_TEMP_G) || (xPacketIO->Data[0].Name == macroID_CONF_THRES_HUMI_G) || \
				 (xPacketIO->Data[0].Name == macroID_CONF_THRES_TEMP_A) || (xPacketIO->Data[0].Name == macroID_CONF_THRES_HUMI_A) || \
				 (xPacketIO->Data[0].Name == macroID_CONF_THRES_LIGHT) \
				 )
		{
			//APP_DEBUG("--- MsgProcess: config is sensor threshold\r\n");
			if( (xPacketIO->TypeDevice == macroTYPE_DEVICE_GW) || (xPacketIO->TypeDevice == macroTYPE_DEVICE_CONTROL_NODE) )
			{
				//APP_DEBUG("--- MsgProcess: type device config haven't sensor node\r\n");
				return;
			}
			vParse_Packing_StringMessage(xPacketIO, &UART_TX_Buffer);
			vMain_setEvent(EVENT_CTS_SEND);
		}//
		else if( (xPacketIO->Data[0].Name == macroID_CONF_TIME_AUTO_VAC_1) || (xPacketIO->Data[0].Name == macroID_CONF_TIME_AUTO_VAC_2) || \
				 (xPacketIO->Data[0].Name == macroID_CONF_TIME_AUTO_VAC_3) || (xPacketIO->Data[0].Name == macroID_CONF_TIME_AUTO_VAC_4) \
				 )
		{
			//APP_DEBUG("--- MsgProcess: config is time VAC auto on/off\r\n");
			if( (xPacketIO->TypeDevice == macroTYPE_DEVICE_GW) || (xPacketIO->TypeDevice == macroTYPE_DEVICE_SENSOR_NODE) )
			{
				//APP_DEBUG("--- MsgProcess: type device config haven't control node\r\n");
				return;
			}
			vParse_Packing_StringMessage(xPacketIO, &UART_TX_Buffer);
			vMain_setEvent(EVENT_CTS_SEND);
		}//
		else if( (xPacketIO->Data[0].Name == macroID_CONF_DATA_UPDATE_TIME) )
		{
			//APP_DEBUG("--- MsgProcess: config is data update time\r\n");
			if(xPacketIO->TypeDevice == macroTYPE_DEVICE_GW)
			{
				//APP_DEBUG("--- MsgProcess: type device config haven't end device\r\n");
				return;
			}
			vParse_Packing_StringMessage(xPacketIO, &UART_TX_Buffer);
			vMain_setEvent(EVENT_CTS_SEND);
		}//
		else if( (xPacketIO->Data[0].Name == macroID_CONF_MQTT) )
		{
			//APP_DEBUG("--- MsgProcess: config is mqtt info\r\n");
			if(xPacketIO->TypeDevice != macroTYPE_DEVICE_GW)
			{
				//APP_DEBUG("--- MsgProcess: type device config haven't gw\r\n");
				return;
			}
			char Array[4][32] = {0};
			uint8 uCnt = 0, ui = 0, uj = 0;
			while(ui <= xPacketIO->Data[0].Info.length())
			{
				if(xPacketIO->Data[0].Info[ui] == '/')
				{
					uCnt++;
					uj = 0;
				}
				else
					Array[uCnt][uj++] += xPacketIO->Data[0].Info[ui];
				if(uCnt >= 4)
					break;
				ui++;
			}
			__MqttConfig.Host = Array[0];
			__MqttConfig.Port = atoi(Array[1]);
			__MqttConfig.UserName = Array[2];
			__MqttConfig.Password = Array[3];
			vMain_setEvent(EVENT_MQTT_UPDATE);
		}//
		else if( (xPacketIO->Data[0].Name == macroID_CONF_WIFI) )
		{
			//APP_DEBUG("--- MsgProcess: config is wifi\r\n");
			if(xPacketIO->TypeDevice != macroTYPE_DEVICE_GW)
			{
				//APP_DEBUG("--- MsgProcess: type device config haven't gw\r\n");
				return;
			}
			char Array[4][32] = {0};
			uint8 uCnt = 0, ui = 0, uj = 0;
			while(ui <= xPacketIO->Data[0].Info.length())
			{
				if(xPacketIO->Data[0].Info[ui] == '/')
				{
					uCnt++;
					uj = 0;
				}
				else
					Array[uCnt][uj++] += xPacketIO->Data[0].Info[ui];
				if(uCnt >= 4)
					break;
				ui++;
			}
			__WifiConfig.UserName = Array[1];
			__WifiConfig.PassWord = Array[3];
			vMain_setEvent(EVENT_WIFI_UPDATE);
		}//
		else
		{
			//APP_DEBUG("--- MsgProcess: config is not found\r\n");
		}
	}
}





/******************************************************************************
 * Function		: void vMsgProcess_isREQS( PacketIO *xPacketIO )
 * Description	: process request message
 * Param		:
 * 				+ xPacketIO - struct data
 * Return		: None
******************************************************************************/
static void vMsgProcess_isREQS( PacketIO *xPacketIO )
{
	//APP_DEBUG("--- MsgProcess: message is REQS\r\n");
	if(xPacketIO->IDGw.length() < 1)
	{
		//assert(false);
		return;
	}

	if(xPacketIO->PacketIsString == true)
	{
		if(xPacketIO->Data[0].Name == macroID_REQS_RTC)
		{
			//APP_DEBUG("--- MsgProcess: request is RTC\r\n");
			//vMain_setEvent(EVENT_GET_TIME);
			vParse_Packing_JsonMessage(xPacketIO, &MQTT_TX_Buffer);
			vMain_setEvent(EVENT_STC_SEND);
		}
		else
		{
			//APP_DEBUG("--- MsgProcess: request is not found\r\n");
		}
	}
	else
	{
		//APP_DEBUG("--- MsgProcess: request is ALL\r\n");
		vParse_Packing_StringMessage(xPacketIO, &UART_TX_Buffer);
		vMain_setEvent(EVENT_CTS_SEND);
	}
}




/******************************************************************************
 * Function		: void vMsgProcess_isCTRL( PacketIO *xPacketIO )
 * Description	: process control message
 * Param		:
 * 				+ xPacketIO - struct data
 * Return		: None
******************************************************************************/
static void vMsgProcess_isCTRL( PacketIO *xPacketIO )
{
	//APP_DEBUG("--- MsgProcess: message is CTRL\r\n");
	if(xPacketIO->IDGw.length() < 1)
	{
		//assert(false);
		return;
	}

	if(xPacketIO->PacketIsString == false)
	{
		vParse_Packing_StringMessage(xPacketIO, &UART_TX_Buffer);
		vMain_setEvent(EVENT_CTS_SEND);
	}
}




/******************************************************************************
 * Function		: void vMsgProcess_isWAMI( PacketIO *xPacketIO )
 * Description	: process control message
 * Param		:
 * 				+ xPacketIO - struct data
 * Return		: None
******************************************************************************/
static void vMsgProcess_isWAMI( PacketIO *xPacketIO )
{
	//APP_DEBUG("--- MsgProcess: message is WAMI\r\n");

	if(xPacketIO->PacketIsString == true)
	{
//		memset(xPacketIO, 0, sizeof(xPacketIO));
//		xPacketIO->TypePacket = macroTYPE_PACKET_WAMI;
//		xPacketIO->TypeDevice = macroID_CONNECTIVITY_WIFI;
//		vParse_Packing_StringMessage(xPacketIO, &UART_TX_Buffer);
		char cStr[16] = {0};
		if(bMQTTisConnected == true)
			sprintf(cStr, "%s,%s,%s!", macroTYPE_PACKET_WAMI, macroID_CONNECTIVITY_WIFI, "OK");
		else
			sprintf(cStr, "%s,%s,%s!", macroTYPE_PACKET_WAMI, macroID_CONNECTIVITY_WIFI, "NOK");
		UART_APP_SEND(cStr);
//		vMain_setEvent(EVENT_CTS_SEND);
	}
}






/******************************************************************************
 * Function		: void vMsgProcess_All(PacketIO *xPacketIO)
 * Description	: process all message
 * Param		:
 * 				+ xPacketIO - struct data
 * Return		: None
******************************************************************************/
void vMsgProcess_All(PacketIO *xPacketIO)
{
	if(xPacketIO->isTypePacket == eData)
	{
		vMsgProcess_isDATA(xPacketIO);
	}
	else if(xPacketIO->isTypePacket == eConf)
	{
		vMsgProcess_isCONF(xPacketIO);
	}
	else if(xPacketIO->isTypePacket == eReqs)
	{
		vMsgProcess_isREQS(xPacketIO);
	}
	else if(xPacketIO->isTypePacket == eCtrl)
	{
		vMsgProcess_isCTRL(xPacketIO);
	}
	else if(xPacketIO->isTypePacket == eResp)
	{

	}
	else if(xPacketIO->isTypePacket == eWami)
	{
		vMsgProcess_isWAMI(xPacketIO);
	}
}




/******************************************************************************
 * Function		: void vMsgProcess_Request_Out( String Request, bool toGw )
 * Description	: request out
 * Param		: None
 * Return		: None
******************************************************************************/
void vMsgProcess_Request_Out( String Request, bool toGw )
{
	PacketIO xPacketIO;

	xPacketIO.TypePacket = macroTYPE_PACKET_REQS;
	xPacketIO.TypeDevice = macroTYPE_DEVICE_GW;

	xPacketIO.IDEd = 0;
	if(toGw == false)
		xPacketIO.IDGw = IDGw;
	else
		xPacketIO.IDGw = 0;

	xPacketIO.Data[0].Name = Request;

	if(toGw == true)
	{
		vParse_Packing_StringMessage(&xPacketIO, &UART_TX_Buffer);
		vMain_setEvent(EVENT_CTS_SEND);
	}
	else
	{
//		vParse_Packing_JsonMessage(&xPacketIO, &MQTT_TX_Buffer);
//		vMain_setEvent(EVENT_STC_SEND);
	}
}



/******************************************************************************
 * Function		: void vMsgProcess_Request_Out( String Request)
 * Description	: response out
 * Param		: None
 * Return		: None
******************************************************************************/
void vMsgProcess_Response_Out( String Response )
{
//	PacketIO xPacketIO;
//
//	xPacketIO.TypePacket = macroTYPE_PACKET_RESP;
//	xPacketIO.TypeDevice = macroTYPE_DEVICE_GW;
//	xPacketIO.Data[0].Name = Response;

	char cStr[32] = {0};
	sprintf(cStr, "%s,%s,%s!", macroTYPE_PACKET_RESP, macroTYPE_DEVICE_GW, Response.c_str());
	UART_APP_SEND(cStr);
//
//	vParse_Packing_StringMessage(&xPacketIO, &UART_TX_Buffer);
//	vMain_setEvent(EVENT_CTS_SEND);
}






















