
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "pin_mux.h"
#include "fsl_gpio.h"  
#include "fsl_port.h"
#include "fsl_lptmr.h"

#include "board.h"
#include "AppData.h"
#include "main.h"
#include "UART.h"
#include "RTC.h"
#include "IOControl.h"
#include "ProcessMsg.h"

#ifdef macroCONNECTIVITY_ETH
	#include "EthTask.h"
	#include "MQTTClient.h"
#endif

#include "WhoAmITask.h"
#include "ProcessTask.h"


#define macroTIME_DELAY_LOOP				20
#define macroTIME_LED_STATUS_ON				3 //s


extern taskHandle_t xTask;
//extern rtc_datetime_t _RTC;
extern uint16_t Event_IO;

//Uart to Connectivity
extern uint8_t uUART_CONN_RX_Buffer[macroUART_RX_BUFFER_LENGHT];
extern uint8_t uUART_CONN_TX_Buffer[macroUART_TX_BUFFER_LENGHT];

//UART to network
extern uint8_t uUART_NWK_RX_Buffer[macroUART_RX_BUFFER_LENGHT];
extern uint8_t uUART_NWK_TX_Buffer[macroUART_TX_BUFFER_LENGHT];

//Buffer MQTT (Ethernet)
#ifdef macroCONNECTIVITY_ETH
	extern uint8_t uETH_MQTT_RX_Buffer[macroMQTT_RX_BUFFER_LENGHT];
	extern uint8_t uETH_MQTT_TX_Buffer[macroMQTT_TX_BUFFER_LENGHT];
#endif

//response
extern bool isResponse;

extern WhoAmI _WhoAmI;

//RTC
extern rtc_datetime_t _RTC;

//sync RTC
extern bool bRTC_Sync;
uint16_t uMinuteCounter = 0;
uint8_t uMinuteOld = 0;

uint8_t uLedStatus = 0;

uint16_t uiNWK_Send_Counter = 0;
uint16_t uiMQTT_Send_Counter = 0;
uint16_t uiWaitNWK_Connected_Counter = 0;
uint16_t uMessageNWK_Send_Counter = 0;
bool bSendState = false;


/********************************* Function ***********************************/
static void vProcessTask_LPTMR_Init(void);
static void vProcessTask_Event( void );
/******************************************************************************/


/******************************************************************************
 * Function		: void vProcessTask_Run( void *pvParameters )
 * Description	: Task xu ly kiem tra cac co de thuc hien cac action tuong ung
 * Param		: N/A
 * Return		: none
*******************************************************************************/
void vProcessTask_Run( void *pvParameters )
{	
	APP_DEBUG("*********************** ProcessTask *************************\r\n");
	APP_DEBUG("--- ProcessTask: Device init finish\r\n");
	
	macroTASK_DELAY_MS( 1000 );
	uMinuteOld = _RTC.minute;
	vIDControl_Init();
	vProcessTask_LPTMR_Init();

    while( 1 )
    {
    	xTask.uiProcessTask_Finish = 1;
		macroTASK_DELAY_MS( macroTIME_DELAY_LOOP );
		
		//blink led status	
		if( uLedStatus >= macroTIME_LED_STATUS_ON )
		{
			macroLED_STATUS_ON();
			uLedStatus++;
			if(uLedStatus > (macroTIME_LED_STATUS_ON + 50 / macroTIME_DELAY_LOOP))
			{
				uLedStatus = 0;
				macroLED_STATUS_OFF();
			}
		}

		//process event
		vProcessTask_Event();

		//Refresh wdt
		vMain_RefreshWDT();
		macroNANO_TIME_DONE();
		xTask.uiProcessTask_Finish = 0;
    }
}




/******************************************************************************
 * Function		: void vProcessTask_Event( void )
 * Description	: Process event
 * Param		: None
 * Return		: None
*******************************************************************************/
static void vProcessTask_Event( void )
{
	//event
	uint16_t uEvent = uiMain_getEvent();
	
	if(uEvent & EVENT_SET_RTC)		//uart conn received
	{
		APP_DEBUG("\r\n--- ProcessTask: EVENT_SET_RTC\r\n");
		vRTC_SetDateTime(&_RTC);
		macroTASK_DELAY_MS( 1000 );

		uEvent ^= EVENT_SET_RTC;
	}
	else
	{
		vRTC_GetDateTime(&_RTC);
		if(uMinuteOld != _RTC.minute)
		{
			if(bRTC_Sync == false)
				vProcessMsg_Send_Request(macroID_REQS_RTC, false);

			uMinuteOld = _RTC.minute;
			uMinuteCounter++;
			if( uMinuteCounter >= (60*24) )
			{
				bRTC_Sync = false;
				uMinuteCounter = 0;
			}
			bSendState = false;
		}
		if( (bSendState == false) && ((uMinuteCounter % 15) == 0) )
		{
			vProcessMsg_Send_Data( NULL, macroID_DATA_DEVICE_STATE, macroRESP_OK );
			bSendState = true;
		}
	}

	if(uEvent & EVENT_UART_CONN_RECEIVED)		//uart conn received
	{
		APP_DEBUG("\r\n--- ProcessTask: EVENT_UART_CONN_RECEIVED\r\n");
		APP_DEBUG("--- ProcessTask: uart conn recv = \"%s\"\r\n", uUART_CONN_RX_Buffer);
		vProcessMsg_AllMessage(uUART_CONN_RX_Buffer, true, true);
		uEvent ^= EVENT_UART_CONN_RECEIVED;
	}

	if(uEvent & EVENT_UART_NWK_SEND)		//uart nwk send
	{
		//APP_DEBUG("\r\n--- ProcessTask: EVENT_UART_NWK_SEND\r\n");
		if(isResponse == false)
		{
			if((uiNWK_Send_Counter == 0) || (uiNWK_Send_Counter % (5000 / macroTIME_DELAY_LOOP) == 0))
			{
				isResponse = false;
				UART_NWK_WRITE_DATA(uUART_NWK_TX_Buffer);
			}
			uiNWK_Send_Counter++;
			vMain_setEvent(EVENT_UART_NWK_SEND);
		}
		else
		{
			uMessageNWK_Send_Counter++;
			APP_DEBUG("--- ProcessTask: UART nwk send finish. Data Count = %d\r\n", uMessageNWK_Send_Counter);
			uiNWK_Send_Counter = 0;
			isResponse = false;
		}
		uEvent ^= EVENT_UART_NWK_SEND;
	}

	if(uEvent & EVENT_UART_CONN_SEND)		//uart conn send
	{
		APP_DEBUG("\r\n--- ProcessTask: EVENT_UART_CONN_SEND\r\n");
		UART_CONN_WRITE_DATA(uUART_CONN_TX_Buffer);
		uEvent ^= EVENT_UART_CONN_SEND;
	}

	if(uEvent & EVENT_UART_NWK_RECEIVED)	//uart nwk received
	{
		APP_DEBUG("\r\n--- ProcessTask: EVENT_UART_NWK_RECEIVED\r\n");
		APP_DEBUG("--- ProcessTask: uart nwk recv = \"%s\"\r\n", uUART_NWK_RX_Buffer);
		vProcessMsg_AllMessage(uUART_NWK_RX_Buffer, true, false);
		uEvent ^= EVENT_UART_NWK_RECEIVED;
	}

#ifdef macroCONNECTIVITY_ETH

	if(uEvent & EVENT_ETH_MQTT_RECEIVED)	//mqtt received
	{
		APP_DEBUG("\r\n--- ProcessTask: EVENT_ETH_MQTT_RECEIVED\r\n");
		vProcessMsg_AllMessage(uETH_MQTT_RX_Buffer, false, false);
		uEvent ^= EVENT_ETH_MQTT_RECEIVED;
	}

	if(uEvent & EVENT_ETH_MQTT_SEND)		//mqtt send
	{
		APP_DEBUG("\r\n--- ProcessTask: EVENT_ETH_MQTT_SEND\r\n");
		ETH_MQTT_WRITE_DATA(uETH_MQTT_TX_Buffer);
		uEvent ^= EVENT_ETH_MQTT_SEND;
	}
#endif
	
	if(uEvent & EVENT_WAIT_NWK_CONNECTED)		//uart nwk send
	{
//		uiWaitNWK_Connected_Counter++;
//		if(uiWaitNWK_Connected_Counter >= (1000 / macroTIME_DELAY_LOOP))
//		{
			APP_DEBUG("\r\n--- ProcessTask: EVENT_WAIT_NWK_CONNECTED\r\n");
		#ifdef macroCONNECTIVITY_ETH
			if(_WhoAmI.Network.Ethernet == true)
				vMain_setEvent(EVENT_ETH_MQTT_SEND);
			else
		#endif
			if( (_WhoAmI.Network.Wifi == true) || (_WhoAmI.Network._2G == true) || (_WhoAmI.Network._3G == true) )
				vMain_setEvent(EVENT_UART_NWK_SEND);
//			uiWaitNWK_Connected_Counter = 0;
//		}
//		vMain_setEvent(EVENT_WAIT_NWK_CONNECTED);
		uEvent ^= EVENT_WAIT_NWK_CONNECTED;
	}

	Event_IO |= uEvent;
}





/******************************************************************************
 * Function		: void macroLPTMR_COUNTER_HANDLER(void)
 * Description	: low power timer interrup
 * Param		: none
 * Return		: none
*******************************************************************************/
void macroLPTMR_COUNTER_HANDLER(void)
{
    LPTMR_ClearStatusFlags(LPTMR0, kLPTMR_TimerCompareFlag);
	
    uLedStatus++;
    macroLED_STATUS_OFF();
	//Kiem tra trang thai cac Task. Neu task nao bi loop hoac die thi restart lai
#ifdef macroCONNECTIVITY_ETH
	if( xTask.uiEthTask_Finish >= 1 )
		xTask.uiEthTask_Finish++;
#endif
	if( xTask.uiWamiTask_Finish >= 1 )
		xTask.uiWamiTask_Finish++;
	if( xTask.uiProcessTask_Finish >= 1 )
		xTask.uiProcessTask_Finish++;
#ifdef macroUSE_SDCARD
	if( xTask.uiSDCardTask_Finish >= 1 )
		xTask.uiSDCardTask_Finish++;
#endif

	//Kiem tra trang thai cac Task. Neu task nao bi loop hoac die thi restart lai
	if( (xTask.uiWamiTask_Finish >= 90)  || (xTask.uiProcessTask_Finish >= 90)
#ifdef macroUSE_SDCARD
		|| (xTask.uiSDCardTask_Finish >= 90)
#endif
#ifdef macroCONNECTIVITY_ETH
		|| (xTask.uiEthTask_Finish >= 90)
#endif
		)
	{
		NVIC_SystemReset();
	}
    __DSB();
    __ISB();
}




/******************************************************************************
 * Function		: void vProcessTask_LPTMR_Init(void)
 * Description	: khoi tao low power timer
 * Param		: none
 * Return		: none
*******************************************************************************/
static void vProcessTask_LPTMR_Init(void)
{
	lptmr_config_t lptmrConfig;
	
    LPTMR_GetDefaultConfig(&lptmrConfig);
    /* Initialize the LPTMR */
    LPTMR_Init(LPTMR0, &lptmrConfig);
	/*
     * Set timer period.
     * Note : the parameter "ticks" of LPTMR_SetTimerPeriod should be equal or greater than 1.
    */
    LPTMR_SetTimerPeriod(LPTMR0, USEC_TO_COUNT(macroLPTMR_USEC_COUNT, CLOCK_GetFreq(kCLOCK_LpoClk)));
    /* Enable timer interrupt */
    LPTMR_EnableInterrupts(LPTMR0, kLPTMR_TimerInterruptEnable);
    /* Enable at the NVIC */
    EnableIRQ(LPTMR0_IRQn);
	/* Start counting */
    LPTMR_StartTimer(LPTMR0);
}


















