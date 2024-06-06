
#ifndef __MAIN_H_
#define __MAIN_H_
#ifdef __cplusplus
extern "C"
{
#endif

/* FreeRTOS kernel includes. */
#include "MK64F12.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "semphr.h"

#include "fsl_debug_console.h"
#include "fsl_rtc.h"

#include "AppData.h"

/*********************************************************************
* CONSTANTS
*/
#define MAIN_DEBUG
#ifdef MAIN_DEBUG
	#define APP_DEBUG 				DbgConsole_Printf
	#define ERROR()					APP_DEBUG("Error: %s, %s", __FILE__, __LINE__)
#else
	#define APP_DEBUG(...)
	#define ERROR(...)
#endif


#define macroBASE_TIME                       		(1)
#define macroVLPR_CLOCK								4000000U
#define macroDELAY_TIME_1S							(uint32_t)((float)(1000 / macroBASE_TIME) * (float)(SystemCoreClock / macroVLPR_CLOCK) / (float)portTICK_PERIOD_MS)
#define macroTASK_DELAY_MS( msec )					vTaskDelay( (uint32_t)(msec / portTICK_PERIOD_MS) )
//#define macroTASK_DELAY_MS( msec )					vTaskDelay( (uint32_t)((msec / macroBASE_TIME) * (SystemCoreClock / macroVLPR_CLOCK) / portTICK_PERIOD_MS) )

// Define time watchdog
#define macroTIME_WATCHDOG_DELAY     				(120000) 	//ms
#define WDOG_WCT_INSTRUCITON_COUNT 					(256U)

//GPIO io
#define macroGPIO_ACTIVE_HIGH(GPIO, PIN)   			GPIO_WritePinOutput(GPIO, PIN, 1)
#define macroGPIO_ACTIVE_LOW(GPIO, PIN)  			GPIO_WritePinOutput(GPIO, PIN, 0)
    
//Event
#define EVENT_IDLE								0x0000
#define EVENT_WAMI								0x0001
#define EVENT_SET_RTC							0x0002
#define EVENT_UART_CONN_RECEIVED				0x0004
#define EVENT_UART_CONN_SEND					0x0008
#define EVENT_UART_NWK_RECEIVED					0x0010
#define EVENT_UART_NWK_SEND						0x0020

#ifdef macroCONNECTIVITY_ETH
	#define EVENT_ETH_MQTT_RECEIVED				0x0040
	#define EVENT_ETH_MQTT_SEND					0x0080
#endif
	
#define EVENT_WAIT_NWK_CONNECTED				0x0100


//For Task
/* Task priorities. */
#define macroPRIORITY_TASK_PROCESS					(configMAX_PRIORITIES - 0)
#ifdef macroCONNECTIVITY_ETH
	#define macroPRIORITY_TASK_ETH					(configMAX_PRIORITIES - 1)
#endif
#define macroPRIORITY_TASK_WhoAmI					(configMAX_PRIORITIES - 2)

#ifdef macroUSE_SDCARD
	#define macroPRIORITY_TASK_SDCard				(configMAX_PRIORITIES - 3)
#endif


//task handle
typedef struct
{
	TaskHandle_t 	xTaskHandle_Process;
#ifdef macroCONNECTIVITY_ETH
	TaskHandle_t 	xTaskHandle_Eth;
#endif
	TaskHandle_t 	xTaskHandle_WhoAmI;

#ifdef macroUSE_SDCARD
	TaskHandle_t 	xTaskHandle_SDCard;
#endif
	
	struct
	{
		uint16_t	uiProcessTask_Finish;
	#ifdef macroCONNECTIVITY_ETH
		uint16_t	uiEthTask_Finish;
	#endif
		uint16_t	uiWamiTask_Finish;
	#ifdef macroUSE_SDCARD
		uint16_t	uiSDCardTask_Finish;
	#endif
	};
}taskHandle_t;




/******************************************************************************
 * Function		: void vMain_Peripheral_Init( void )
 * Description	: Ham khoi tao ngoai vi
 * Param		: none
 * Return		: none
*******************************************************************************/
void vMain_Peripheral_Init( void );




/******************************************************************************
 * Function		: void vMain_Peripheral_DeInit( void )
 * Description	: Ham stop ngoai vi
 * Param		: none
 * Return		: none
*******************************************************************************/
void vMain_Peripheral_DeInit( void );




/******************************************************************************
 * Function		: void vMain_RefreshWDT( void )
 * Description	: refresh wdt
 * Param		: none
 * Return		: none
*******************************************************************************/
void vMain_RefreshWDT( void );




/******************************************************************************
 * Function		: void vMain_setEvent(uint16_t Event)
 * Description	: set event
 * Param		: none
 * Return		: none
*******************************************************************************/
void vMain_setEvent(uint16_t Event);




/******************************************************************************
 * Function		: uint16_t uiMain_getEvent( void )
 * Description	: get event
 * Param		: none
 * Return		: none
*******************************************************************************/
uint16_t uiMain_getEvent( void );



    
#ifdef __cplusplus
}
#endif
#endif



