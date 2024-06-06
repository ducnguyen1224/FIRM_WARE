

#include "pin_mux.h"
#include "fsl_debug_console.h"
#include "fsl_wdog.h"
#include "fsl_sim.h"
#include "fsl_sysmpu.h"

#include "main.h"
#include "board.h"

/* Task */
#ifdef macroCONNECTIVITY_ETH
	#include "EthTask.h"
#endif

#include "ProcessTask.h"
#include "WhoAmITask.h"

#ifdef macroUSE_SDCARD
	#include "SDCardTask.h"
#endif

#include "IOControl.h"
#include "UART.h"
#include "RTC.h"
#include "MQTTClient.h"


/*******************************************************************************
 * Variables
 ******************************************************************************/
//RTC
extern rtc_datetime_t _RTC;

//Uart to Connectivity
uint8_t uUART_CONN_RX_Buffer[macroUART_RX_BUFFER_LENGHT] = {0};
uint8_t uUART_CONN_TX_Buffer[macroUART_TX_BUFFER_LENGHT] = {0};

//UART to network
uint8_t uUART_NWK_RX_Buffer[macroUART_RX_BUFFER_LENGHT] = {0};
uint8_t uUART_NWK_TX_Buffer[macroUART_TX_BUFFER_LENGHT] = {0};

//Buffer MQTT (Ethernet)
uint8_t uETH_MQTT_RX_Buffer[macroMQTT_RX_BUFFER_LENGHT] = {0};
uint8_t uETH_MQTT_TX_Buffer[macroMQTT_TX_BUFFER_LENGHT] = {0};

//event
uint16_t Event_IO = EVENT_IDLE;

//Task
taskHandle_t xTask =
{
	.uiProcessTask_Finish	= 1,

#ifdef macroCONNECTIVITY_ETH
	.uiEthTask_Finish		= 1,
#endif

	.uiWamiTask_Finish		= 1,

#ifdef macroUSE_SDCARD
	.uiSDCardTask_Finish	= 1,
#endif
};

//WDT
static WDOG_Type *xWdog_Base = WDOG;

//ID Gw
uint8_t uIDGw[17] = {0};


//Who am i
WhoAmI _WhoAmI =
{
	.Network =
	{
		.Ethernet		= false,
		.Wifi			= false,
		._3G			= false,
		._2G			= false,
		.ConnectionOK	= false,
	},
	.Connectivity =
	{
		.Zigbee			= false,
		.Sub1Ghz		= false,
	}
};

//response
bool isResponse = false;

//sync RTC
bool bRTC_Sync = false;


/******************************** Function ************************************/
void vMain_InitWatchdog( void );
void vMain_GetUniqueID(uint8_t *pID);
/******************************************************************************/



/*******************************************************************************
 * Function		: int main(void)
 * Description	: main
 * Param		: none
 * Return		: none
*******************************************************************************/
int main(void)
{
    /* Init board hardware. */
    BOARD_InitPins();

#if defined(macroCONNECTIVITY_ETH) || defined(macroUSE_SDCARD)
    macroETH_POWER_ON();
	BOARD_BootClockRUN();
#else
	BOARD_BootClockVLPR();
#endif
	SYSMPU_Enable(SYSMPU, false);
	
	vMain_Peripheral_Init();
    
	APP_DEBUG("\r\n");
    APP_DEBUG("*******************************************************************\r\n");
    APP_DEBUG("******************->> SMF Project is Welcome <<-*******************\r\n");
    APP_DEBUG("*******************************************************************\r\n");
    APP_DEBUG("\r\n");
	
	vMain_GetUniqueID( uIDGw );
	APP_DEBUG("--- Main: ID Gw = %s\r\n", uIDGw);
	
	APP_DEBUG("--- Main: Init Peripheral successfully\r\n");
	
	//Process task
	if(xTaskCreate(vProcessTask_Run	, "vProcessTask_Run"	, configMINIMAL_STACK_SIZE + 1024	, NULL, macroPRIORITY_TASK_PROCESS	, &xTask.xTaskHandle_Process) != pdPASS)
		APP_DEBUG("--- Main: Failed to create process task\r\n");

	//Ethernet task
#ifdef macroCONNECTIVITY_ETH
	if(xTaskCreate(vEthTask_Run		, "vEthTask_Run"		, configMINIMAL_STACK_SIZE + 1024	, NULL, macroPRIORITY_TASK_ETH		, &xTask.xTaskHandle_Eth) != pdPASS)
		APP_DEBUG("--- Main: Failed to create ethernet task\r\n");
#endif

	//Who am i task
	if(xTaskCreate(vWhoAmITask_Run	, "vWhoAmITask_Run"		, configMINIMAL_STACK_SIZE + 256	, NULL, macroPRIORITY_TASK_WhoAmI	, &xTask.xTaskHandle_WhoAmI) != pdPASS)
		APP_DEBUG("--- Main: Failed to create Who Am I task\r\n");

	//SD Card task
#ifdef macroUSE_SDCARD
	if(xTaskCreate(vSDCardTask_Run	, "vSDCardTask_Run"		, configMINIMAL_STACK_SIZE + 1024*3	, NULL, macroPRIORITY_TASK_SDCard	, &xTask.xTaskHandle_SDCard) != pdPASS)
		APP_DEBUG("--- Main: Failed to create SD Card task\r\n");
#endif
   	
    vTaskStartScheduler();
    
    for (;;);
}




/******************************************************************************
 * Function		: void vMain_Peripheral_Init( void )
 * Description	: Ham khoi tao ngoai vi
 * Param		: none
 * Return		: none
*******************************************************************************/
void vMain_Peripheral_Init( void )
{
	vMain_InitWatchdog();
	
	BOARD_InitDebugConsole();

	//network UART
	vUART_Init(macroUART_NETWORK_BASE, macroUART_NETWORK_CLKSRC, macroUART_NETWORK_BAUDRATE, true, macroUART_NETWORK_IRQn);
	
	//connectivity UART
	vUART_Init(macroUART_CONNECTIVITY_BASE, macroUART_CONNECTIVITY_CLKSRC, macroUART_CONNECTIVITY_BAUDRATE, true, macroUART_CONNECTIVITY_IRQn);
	
	//Set RTC
	vRTC_Init(&_RTC);
}




/******************************************************************************
 * Function		: void vMain_Peripheral_DeInit( void )
 * Description	: Ham stop ngoai vi
 * Param		: none
 * Return		: none
*******************************************************************************/
void vMain_Peripheral_DeInit( void )
{
	WDOG_Deinit( xWdog_Base );
	
	//Deinit uart
	vUART_DeInit( macroUART_NETWORK_BASE );
	vUART_DeInit( macroUART_CONNECTIVITY_BASE );
	
	//Deinit rtc
	RTC_Deinit( RTC );
	
	DbgConsole_Deinit();
}	




/******************************************************************************
 * Function		: void macroUART_NETWORK_IRQHandler( void )
 * Description	: Network uart rx interrupt
 * Param		: none
 * Return		: none
*******************************************************************************/
void macroUART_NETWORK_IRQHandler( void )
{
	if ((kUART_RxDataRegFullFlag | kUART_RxOverrunFlag) & UART_GetStatusFlags(macroUART_NETWORK_BASE))
	//if (UART_GetStatusFlags(macroUART_NETWORK_BASE))
    {
		uint16_t uiLenght = strlen((char *)uUART_NWK_RX_Buffer);
		uUART_NWK_RX_Buffer[uiLenght] = UART_ReadByte( macroUART_NETWORK_BASE );
		
        if(uiLenght >= macroUART_RX_BUFFER_LENGHT)
		{
            uiLenght = 0;
			memset(uUART_NWK_RX_Buffer, 0, strlen((char *)uUART_NWK_RX_Buffer));
		}
        else if(uUART_NWK_RX_Buffer[uiLenght] == macroPACKET_STRING_ENDCHAR)
        {
        	vMain_setEvent(EVENT_UART_NWK_RECEIVED);
        }
    }
#if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
#endif
}




/******************************************************************************
 * Function		: void macroUART_CONNECTIVITY_IRQHandler( void )
 * Description	: UART connectivity interrupt
 * Param		: none
 * Return		: none
*******************************************************************************/
void macroUART_CONNECTIVITY_IRQHandler( void )
{
	if ((kUART_RxDataRegFullFlag | kUART_RxOverrunFlag) & UART_GetStatusFlags(macroUART_CONNECTIVITY_BASE))
    {
		uint16_t uiLenght = strlen((char *)uUART_CONN_RX_Buffer);
        uUART_CONN_RX_Buffer[uiLenght] = UART_ReadByte( macroUART_CONNECTIVITY_BASE );
		
        if(uiLenght >= macroUART_RX_BUFFER_LENGHT)
		{
            uiLenght = 0;
			memset(uUART_CONN_RX_Buffer, 0, strlen((char *)uUART_CONN_RX_Buffer));
		}
        else if(uUART_CONN_RX_Buffer[uiLenght] == macroPACKET_STRING_ENDCHAR)
        {
        	vMain_setEvent(EVENT_UART_CONN_RECEIVED);
        }
    }
#if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
#endif
}





/******************************************************************************
 * Function		: void vMain_InitWatchdog( void )
 * Description	: Ham khoi tao WDT
 * Param		: none
 * Return		: none
*******************************************************************************/
void vMain_InitWatchdog( void )
{
    wdog_config_t xWDTConfig;

    WDOG_GetDefaultConfig( &xWDTConfig );
    xWDTConfig.timeoutValue = macroTIME_WATCHDOG_DELAY;
    
    /* wdog refresh test in window mode */
    WDOG_Init(xWdog_Base, &xWDTConfig);
	
	for (uint32_t i = 0; i < WDOG_WCT_INSTRUCITON_COUNT; i++)
        (void)xWdog_Base->RSTCNT;
    
    /* Delay watchdog 1 second */
    while (((uint32_t)((((uint32_t)xWdog_Base->TMROUTH) << 16U) | (xWdog_Base->TMROUTL))) < (1000));
    APP_DEBUG("--- Main: Init watchdog time delay: %d ms complete\r\n", macroTIME_WATCHDOG_DELAY);
}




/******************************************************************************
 * Function		: void vMain_GetUniqueID(char *pcDeviceID)
 * Description	: Ham lay Unique ID of mcu
 * Param		: pcDeviceID - ID device of mcu
 * Return		: none
*******************************************************************************/
void vMain_GetUniqueID(uint8_t *pID)
{
	sim_uid_t xSim_UID;
	uint8_t uValue = 0;
	
	SIM_GetUniqueId(&xSim_UID);
	
	pID[0]  = ((uValue = (((xSim_UID.ML >> 24) & 0xFF) / 16)) <= 9)? (uValue | 0x30) : ((uValue - 9) | 0x41);
	pID[1]  = ((uValue = (((xSim_UID.ML >> 24) & 0xFF) % 16)) <= 9)? (uValue | 0x30) : ((uValue - 9) | 0x41);
	pID[2]  = ((uValue = (((xSim_UID.ML >> 16) & 0xFF) / 16)) <= 9)? (uValue | 0x30) : ((uValue - 9) | 0x41);
	pID[3]  = ((uValue = (((xSim_UID.ML >> 16) & 0xFF) % 16)) <= 9)? (uValue | 0x30) : ((uValue - 9) | 0x41);
	pID[4]  = ((uValue = (((xSim_UID.ML >> 8 ) & 0xFF) / 16)) <= 9)? (uValue | 0x30) : ((uValue - 9) | 0x41);
	pID[5]  = ((uValue = (((xSim_UID.ML >> 8 ) & 0xFF) % 16)) <= 9)? (uValue | 0x30) : ((uValue - 9) | 0x41);
	pID[6]  = ((uValue = (((xSim_UID.ML >> 0 ) & 0xFF) / 16)) <= 9)? (uValue | 0x30) : ((uValue - 9) | 0x41);
	pID[7]  = ((uValue = (((xSim_UID.ML >> 0 ) & 0xFF) % 16)) <= 9)? (uValue | 0x30) : ((uValue - 9) | 0x41);
	pID[8]  = ((uValue = (((xSim_UID.L  >> 24) & 0xFF) / 16)) <= 9)? (uValue | 0x30) : ((uValue - 9) | 0x41);
	pID[9]  = ((uValue = (((xSim_UID.L  >> 24) & 0xFF) % 16)) <= 9)? (uValue | 0x30) : ((uValue - 9) | 0x41);
	pID[10] = ((uValue = (((xSim_UID.L  >> 16) & 0xFF) / 16)) <= 9)? (uValue | 0x30) : ((uValue - 9) | 0x41);
	pID[11] = ((uValue = (((xSim_UID.L  >> 16) & 0xFF) % 16)) <= 9)? (uValue | 0x30) : ((uValue - 9) | 0x41);
	pID[12] = ((uValue = (((xSim_UID.L  >> 8 ) & 0xFF) / 16)) <= 9)? (uValue | 0x30) : ((uValue - 9) | 0x41);
	pID[13] = ((uValue = (((xSim_UID.L  >> 8 ) & 0xFF) % 16)) <= 9)? (uValue | 0x30) : ((uValue - 9) | 0x41);
	pID[14] = ((uValue = (((xSim_UID.L  >> 0 ) & 0xFF) / 16)) <= 9)? (uValue | 0x30) : ((uValue - 9) | 0x41);
	pID[15] = ((uValue = (((xSim_UID.L  >> 0 ) & 0xFF) % 16)) <= 9)? (uValue | 0x30) : ((uValue - 9) | 0x41);
}



/******************************************************************************
 * Function		: void vMain_RefreshWDT( void )
 * Description	: refresh wdt
 * Param		: none
 * Return		: none
*******************************************************************************/
void vMain_RefreshWDT( void )
{
	WDOG_Refresh(xWdog_Base);
}




/******************************************************************************
 * Function		: void vMain_setEvent(uint16_t Event)
 * Description	: set event
 * Param		: none
 * Return		: none
*******************************************************************************/
void vMain_setEvent(uint16_t Event)
{
	Event_IO |= Event;
}




/******************************************************************************
 * Function		: uint16_t uiMain_getEvent( void )
 * Description	: get event
 * Param		: none
 * Return		: none
*******************************************************************************/
uint16_t uiMain_getEvent( void )
{
	uint16_t uEvent = Event_IO;
	Event_IO = EVENT_IDLE;
	return uEvent;
}



























