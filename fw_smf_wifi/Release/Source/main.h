
#ifndef __MAIN_H
#define __MAIN_H

#include <user_config.h>
#include <SmingCore.h>
#include <UserUART.h>

#include "AppData.h"


//#define UART_DEBUG

#ifdef UART_DEBUG
#define APP_DEBUG					Serial.printf
#else
#define APP_DEBUG(FORMAT, ...)
#endif


//event
#define EVENT_IDLE						0x0000
#define EVENT_GET_IDGW					0x0001
#define EVENT_CTS_RECEIVED				0x0002
#define EVENT_CTS_SEND					0x0004
#define EVENT_STC_RECEIVED				0x0008
#define EVENT_STC_SEND					0x0010
#define EVENT_MQTT_UPDATE				0x0020
#define EVENT_WIFI_UPDATE				0x0040
#define EVENT_DATA_RESP					0x0080
#define EVENT_STC_RESEND				0x0100
//#define EVENT_SMART_CONFIG				0x0200



/*************************************************************************
 * Function	: void vMain_setEvent(Enum_EVENT_IO Event)
 * Desc		: set event
 * Param	: Event - user event
 * Return	: None
 *************************************************************************/
void vMain_setEvent(uint16 Event);



#endif




