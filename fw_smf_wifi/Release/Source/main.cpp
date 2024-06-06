
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "main.h"

#include <UserUART.h>
#include "Network/WifiStation.h"
#include "Network/MQTTClient.h"
#include "Interface/UserFile.h"
#include "MsgCheck.h"
#include "MsgProcess.h"


#define macroMAIN_RUN_TIME_LOOP					30

#define macroUSE_BUTTON_SMART_CONFIG			5		//GPIO5
//#define BUTTON_STATUS()							digitalRead(GPIO_PIN_BUTTON)

uint16_t uButtonCnt = 0;


extern WifiConfig __WifiConfig;
extern MqttConfig __MqttConfig;
extern String MQTT_RX_Buffer;
extern String UART_RX_Buffer;
extern String UART_TX_Buffer;
extern String MQTT_TX_Buffer;
extern bool bIsDataSending;


String IDGw = "4E4543801003001E";
Timer T_EventProcess;
uint16 Event_IO = EVENT_IDLE;
uint16 uGetIDCounter = 0, uResendCounter = 0;

//main run
static void vMain_Run( void );



//wifi connection call back
static void vMain_WifiConnection_Callback( bool isConnected )
{
	if(isConnected)
		vMQTTClient_Connect();
	else
		vMQTTClient_Stop();
}


//user button smart config
static void IRAM_ATTR interruptHandler( void )
{
//	APP_DEBUG("--- Main: button sc\r\n");
//	vMain_setEvent(EVENT_SMART_CONFIG);
	vWifiStation_SmartConfig();
}



/*************************************************************************
 * Function	: uint16 vMain_getEvent( void )
 * Desc		: get event
 * Param	: None
 * Return	: None
 *************************************************************************/
//static uint16 vMain_getEvent( void );




/*************************************************************************
 * Function	: void init(void)
 * Desc		: Main
 * Param	: None
 * Return	: None
 *************************************************************************/
void init(void)
{
	spiffs_mount();
	vUART_Init();

	//pinMode(GPIO_PIN_BUTTON, INPUT_PULLUP);
	//APP_DEBUG("\n\n********------------------ ESP Start ------------------********\n");

	vUserFile_Read_WifiConfig(&__WifiConfig.UserName, &__WifiConfig.PassWord);
	vUserFile_Read_MqttConfig(&__MqttConfig.Host, &__MqttConfig.Port, &__MqttConfig.UserName, &__MqttConfig.Password);

	attachInterrupt(macroUSE_BUTTON_SMART_CONFIG, interruptHandler, RISING);

	WifiAccessPoint.enable(false);
	vWifiStation_Init(__WifiConfig.UserName, __WifiConfig.PassWord, &vMain_WifiConnection_Callback);
	T_EventProcess.initializeMs(macroMAIN_RUN_TIME_LOOP, vMain_Run).start();
	//vMain_setEvent(EVENT_GET_IDGW);
}




/*************************************************************************
 * Function	: void vMain_Run(void)
 * Desc		: Main run
 * Param	: None
 * Return	: None
 *************************************************************************/
static void vMain_Run( void )
{
	uint16 Event = Event_IO;//vMain_getEvent();
	Event_IO = EVENT_IDLE;

//	if(!BUTTON_STATUS())
//		uButtonCnt++;
//	else
//	{
//		if(uButtonCnt >= (400 / macroMAIN_RUN_TIME_LOOP))
//			vWifiStation_SmartConfig();
//		uButtonCnt = 0;
//	}

//	if(Event & EVENT_SMART_CONFIG)
//	{
//
//		Event ^= EVENT_SMART_CONFIG;
//	}

	if(Event & EVENT_CTS_RECEIVED)
	{
		PacketIO xPacketIO;
		vMsgCheck_JsonMessage(MQTT_RX_Buffer, &xPacketIO);
		if(xPacketIO.PacketIsOK == true)
		{
			//APP_DEBUG("--- Main: Json message is ok\r\n");
			vMsgProcess_All(&xPacketIO);
		}
		else
			APP_DEBUG("--- Main: Json message is nok\r\n");

		Event ^= EVENT_CTS_RECEIVED;
	}

	if(Event & EVENT_CTS_SEND)
	{
		UART_APP_SEND(UART_TX_Buffer.c_str());
		Event ^= EVENT_CTS_SEND;
	}

	if(Event & EVENT_STC_RECEIVED)
	{
		PacketIO xPacketIO;
		vMsgCheck_StringMessage(UART_RX_Buffer, &xPacketIO);
		if(xPacketIO.PacketIsOK == true)
		{
			//APP_DEBUG("--- Main: String message is ok\r\n");
			vMsgProcess_All(&xPacketIO);
		}
		else
			APP_DEBUG("--- Main: String message is nok\r\n");

		Event ^= EVENT_STC_RECEIVED;
	}

	if(Event & EVENT_STC_SEND)
	{
		//APP_DEBUG("--- Main: MQTT TX Buffer = %s\r\n", MQTT_TX_Buffer.c_str());
		bool Result = bMQTTClient_Publish(MQTT_TX_Buffer);
		if( (Result == true) && (bIsDataSending == true) )
//			vMain_setEvent(EVENT_DATA_RESP);
			vMsgProcess_Response_Out(macroID_RESP_OK);
		else if( (Result == false) && (bIsDataSending == true) )
			vMain_setEvent(EVENT_STC_RESEND);

		Event ^= EVENT_STC_SEND;
	}

	if(Event & EVENT_STC_RESEND)
	{
		uResendCounter++;
		if(uResendCounter >= (1000 / macroMAIN_RUN_TIME_LOOP))
		{
			vMain_setEvent(EVENT_STC_SEND);
			uResendCounter = 0;
		}
		else
			vMain_setEvent(EVENT_STC_RESEND);

		Event ^= EVENT_STC_RESEND;
	}

//	if(Event & EVENT_GET_IDGW)
//	{
//		uGetIDCounter++;
//		if(uGetIDCounter >= (3000 / macroMAIN_RUN_TIME_LOOP))
//		{
//			vMsgProcess_Request_Out(macroID_REQS_IDGW, true);
//			uGetIDCounter = 0;
//		}
//		if(IDGw.length() == 0)
//		{
//			vMain_setEvent(EVENT_GET_IDGW);
//		}
//		Event ^= EVENT_GET_IDGW;
//	}

	if(Event & EVENT_MQTT_UPDATE)
	{
		//APP_DEBUG("--- Main: Update mqtt broker config\r\n");
		vUserFile_Write_MqttConfig(__MqttConfig.Host, __MqttConfig.Port, __MqttConfig.UserName, __MqttConfig.Password);
		vMQTTClient_Update();
		Event ^= EVENT_MQTT_UPDATE;
	}

	if(Event & EVENT_WIFI_UPDATE)
	{
		//APP_DEBUG("--- Main: Update mqtt broker config\r\n");
		vUserFile_Write_WifiConfig(__WifiConfig.UserName, __WifiConfig.PassWord);
		WifiStation.config(__WifiConfig.UserName, __WifiConfig.PassWord);
		WifiStation.connect();
		Event ^= EVENT_WIFI_UPDATE;
	}
//	else if(Event & EVENT_DATA_RESP)
//	{
//		vMsgProcess_Response_Out(macroID_RESP_OK);
//		Event ^= EVENT_DATA_RESP;
//	}

	Event_IO |= Event;

//	if(Event_IO != EVENT_IDLE)
//		T_EventProcess.startOnce();
}




/*************************************************************************
 * Function	: void vMain_setEvent(Enum_EVENT_IO Event)
 * Desc		: set event
 * Param	: Event - user event
 * Return	: None
 *************************************************************************/
void vMain_setEvent(uint16 Event)
{
	Event_IO |= Event;
//	T_EventProcess.startOnce();
}



/*************************************************************************
 * Function	: uint16 vMain_getEvent( void )
 * Desc		: get event
 * Param	: None
 * Return	: None
 *************************************************************************/
//static uint16 vMain_getEvent( void )
//{
//	uint16 Event = Event_IO;
//	Event_IO = EVENT_IDLE;
//	return Event;
//}
























