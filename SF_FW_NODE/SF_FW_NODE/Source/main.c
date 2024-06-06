

#include "pin_mux.h"
#include "fsl_debug_console.h"
#include "fsl_wdog.h"
#include "fsl_uart.h"
#include "fsl_dspi_freertos.h"
#include "fsl_sim.h"
#include "fsl_ftm.h"
#include "fsl_sysmpu.h"		// phai dinh nghia cai nay

#include "main.h"
#include "board.h"

#include  "DeviceControl.h"
/* Task */
#include "ProcessTask.h"
#include "SensorTask.h"
#include "WhoAmITask.h"
#include "SDCardTask.h"
#include "I2C.h"
#include "SPI.h"
#include "UART.h"


#ifdef PH_SENSOR
	#include "PH_Sensor.h"
#endif

#ifdef LIGHT_SENSOR
	#include "BH1750.h"
#endif

#ifdef SHTA_SENSOR
	#include "SHT11.h"
#endif

#ifdef SHTG_SENSOR
	#include "SHT11Soil.h"
#endif

//#ifdef EC_SENSOR
//	#include "EC_Sensor.h"
//#endif

#ifdef EC_SENSOR
	#include "EC_Murata.h"
#endif

/*******************************************************************************
 * Variables
 ******************************************************************************/

//I2C
i2c_rtos_handle_t xMaster_Handle_BH1750;

//Uart Zigb
uint8_t uUART_ZigB_Buffer[macroUART_MAX_LENGHT + 1] = {0};
uint16_t uiUART_ZigB_Cnt = 0;

//Uart sensor EC
uint8_t uUART_EC_Buffer[macroUART_MAX_LENGHT + 1] = {0};
uint16_t uiUART_EC_Cnt = 0;

//Uart sensor pH
uint8_t uUART_PH_Buffer[macroUART_MAX_LENGHT + 1] = {0};
uint16_t uiUART_PH_Cnt = 0;


//Data local
DataLocal xDataLocal = 
{
	.uiTimeUpdate 			= macroTIME_SEND_INTERVAL,
#ifdef PH_SENSOR
    .xPH = {macroPH_THRESH_HIGH,macroPH_THRESH_LOW},
#endif
	
#ifdef EC_SENSOR
    .xEC = {macroEC_THRESH_HIGH,macroEC_THRESH_LOW},
#endif
	
#ifdef SHTA_SENSOR
    .xTempA = {macroTEMPA_THRESH_HIGH,macroTEMPA_THRESH_LOW},
	.xHumiA = {macroHUMIA_THRESH_HIGH,macroHUMIA_THRESH_LOW},
#endif
	
#ifdef SHTG_SENSOR
    .xTempG = {macroTEMPG_THRESH_HIGH,macroTEMPG_THRESH_LOW},
    .xHumiG = {macroHUMIG_THRESH_HIGH,macroHUMIG_THRESH_LOW},
#endif
	
#ifdef LIGHT_SENSOR
    .xLight = {macroLIGHT_THRESH_HIGH,macroLIGHT_THRESH_LOW},
#endif
};

//Flags
Flags xFlags =
{
	.eData_Recv		 	= eUART_None,
	.bData_Send 		= false,
	.eSentIsOK			= eSend_None,
	.eReadSensor 		= eNone,
	.bWAMItoZigB		= false,
	.bSendReady			= false,
	.bZigbIsConnected	= false,
	.eSDCard_Read		= eSDCard_None,
	.eSDCard_Write		= eSDCard_None,
	.bTimeSend			= false,
    .bSendReqs          = false,
};



//Task
taskHandle_t xTask =
{
	.uiSensorTask_Finish = 1,
	.uiWhoAmITask_Finish	= 1,
	.uiSDCardTask_Finish = 1,
};

//WDT
static WDOG_Type *xWdog_Base = WDOG;
//for timer
uint16_t ui16TimerCounter = 0;
uint16_t ui16SecCounter = 0;
uint16_t ui16WAMICounter = 0;
uint16_t ui16SendCounter=0;
uint8_t uLedStatus =0;

char cID_EndDevice[17]={0};
/******************************** Function ************************************/
void vMain_InitWatchdog( void );
void vMain_TimerInit(void);
void vMain_GetUniqueID(char *pcDeviceID);
/******************************************************************************/



/*******************************************************************************
 * Function		: int main(void)
 * Description	: Ham chinh cua chuong trinh
 * Param		: none
 * Return		: none
*******************************************************************************/
int main(void)
{
    /* Init board hardware. */
    BOARD_InitPins();
	BOARD_BootClockVLPR();
	SYSMPU_Enable(SYSMPU, false);		//khong duoc bo
	vMain_Peripheral_Init();
	
	APP_DEBUG("\r\n");
    APP_DEBUG("*******************************************************************\r\n");
    APP_DEBUG("******************->> SmartFarm Project is Welcome <<-*******************\r\n");
    APP_DEBUG("*******************************************************************\r\n");
    APP_DEBUG("\r\n");
	
//	vDeviceControl_Init();
	
	vMain_GetUniqueID(cID_EndDevice);
	APP_DEBUG("---- cID_EndDevice = %s\r\n",cID_EndDevice);
	
    if(xTaskCreate(vSensorTask_Task, "vSensorTask_Task", configMINIMAL_STACK_SIZE + 512, NULL, macroPRIORITY_TASK_SENSOR, &xTask.xTaskHandle_Sensor) != pdPASS)
    {
        APP_DEBUG("--- Main: Failed to create sensor task\r\n");
    }
	
	if(xTaskCreate(vProcessTask_Task, "vProcessTask_Task", configMINIMAL_STACK_SIZE + 1024, NULL, macroPRIORITY_TASK_PROCESS, &xTask.xTaskHandle_Process) != pdPASS)
	{
		APP_DEBUG("--- Main: Failed to create process task\r\n");
	}
   	
	if(xTaskCreate(vWhoAmITask_Task, "vWhoAmITask_Task", configMINIMAL_STACK_SIZE + 512, NULL, macroPRIORITY_TASK_WhoAmI, &xTask.xTaskHandle_WhoAmI) != pdPASS)
	{
		APP_DEBUG("--- Main: Failed to create Who Am I task\r\n");
	}
	
	#ifdef macroUSE_SDCARD
	if(xTaskCreate(vSDCardTask_Run	, "vSDCardTask_Run"		, configMINIMAL_STACK_SIZE + 1024*4, NULL, macroPRIORITY_TASK_SDCard	, &xTask.xTaskHandle_SDCard) != pdPASS)
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
	BOARD_InitDebugConsole();

	vMain_InitWatchdog();
	
	vMain_TimerInit();

	/*Zigb UART */
    macroPOWER_ON(macroZIGB_POWER_GPIO,macroZIGB_POWER_PIN);
	vUART_Init(macroUART_ZIGB_BASE, macroUART_ZIGB_CLKSRC, macroUART_ZIGB_BAUDRATE, true, macroUART_ZIGB_IRQn);
	
#ifdef PH_SENSOR
	/* pH sensor Init */
    macroPOWER_ON(macroPH_POWER_GPIO,macroPH_POWER_PIN);
	vUART_Init(macroUART_PH_SENSOR_BASE, macroUART_PH_SENSOR_CLKSRC, macroUART_PH_SENSOR_BAUDRATE, true, macroUART_PH_SENSOR_IRQn);
	PH_Sensor_vInit();
#endif
	
#ifdef EC_SENSOR
	/* EC sensor UART */
    macroPOWER_ON(macroEC_POWER_GPIO, macroEC_POWER_PIN);
	vUART_Init(macroUART_EC_SENSOR_BASE, macroUART_EC_SENSOR_CLKSRC, macroUART_EC_SENSOR_BAUDRATE, true, macroUART_EC_SENSOR_IRQn);
#endif
	
#ifdef LIGHT_SENSOR	
	/* Khoi tao I2C1 for BH1750 */
	BH1750_vInit();
#endif
}




/******************************************************************************
 * Function		: void vMain_Peripheral_DeInit( void )
 * Description	: Ham stop ngoai vi
 * Param		: none
 * Return		: none
*******************************************************************************/
void vMain_Peripheral_DeInit( void )
{
//	WDOG_Deinit( xWdog_Base );
////	
//	//Deinit uart
//	vUART_DeInit( macroUART_ZIGB_BASE );
//	
//	//Deinit uart
//	vUART_DeInit( macroUART_EC_SENSOR_BASE );
//	
////	//Deinit i2c
//	I2C_RTOS_Deinit( &xMaster_Handle_SensorGas );
//
//	DbgConsole_Deinit();
}




/******************************************************************************
 * Function		: void macroUART_ZIGB_IRQHandler( void )
 * Description	: This function get data receive from UART to array data_receive
 * Param		: none
 * Return		: none
*******************************************************************************/
void macroUART_ZIGB_IRQHandler( void )
{
	if ((kUART_RxDataRegFullFlag | kUART_RxOverrunFlag) & UART_GetStatusFlags(macroUART_ZIGB_BASE))
    {
		
        uUART_ZigB_Buffer[uiUART_ZigB_Cnt] = UART_ReadByte( macroUART_ZIGB_BASE );
        if(uiUART_ZigB_Cnt >= macroUART_MAX_LENGHT - 1)
		{
            uiUART_ZigB_Cnt = 0;
			memset(uUART_ZigB_Buffer, 0, macroUART_MAX_LENGHT);
		}
        else if(uUART_ZigB_Buffer[uiUART_ZigB_Cnt] == macroPACKET_STRING_ENDCHAR)
        {
            xFlags.eData_Recv = eUART_ZigB;
        }
		uiUART_ZigB_Cnt++;
    }
#if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
#endif
}



/******************************************************************************
 * Function		: void macroUART_EC_SENSOR_IRQHandler( void )
 * Description	: This function get data receive from UART to array data_receive
 * Param		: none
 * Return		: none
*******************************************************************************/
void macroUART_EC_SENSOR_IRQHandler( void )
{
//#ifdef EC_SENSOR
//	if ((kUART_RxDataRegFullFlag | kUART_RxOverrunFlag) & UART_GetStatusFlags(macroUART_EC_SENSOR_BASE))
//    {
//        uUART_EC_Buffer[uiUART_EC_Cnt] = UART_ReadByte( macroUART_EC_SENSOR_BASE );
//		
//        if(uiUART_EC_Cnt >= macroUART_MAX_LENGHT - 1)
//		{
//            uiUART_EC_Cnt = 0;
//			memset(uUART_EC_Buffer, 0, macroUART_MAX_LENGHT);
//		}
//        else if(uUART_EC_Buffer[uiUART_EC_Cnt] == macroEC_END_CMD)
//        {
//            xFlags.eData_Recv = eUART_EC;
//        }
//		uiUART_EC_Cnt++;
//    }
//#endif
	
#ifdef EC_SENSOR
	/**************************************************************************
    for sensor murata
    ***************************************************************************/
    if ((kUART_RxDataRegFullFlag | kUART_RxOverrunFlag) & UART_GetStatusFlags(macroUART_EC_SENSOR_BASE))
    {
        uUART_EC_Buffer[uiUART_EC_Cnt] = UART_ReadByte( macroUART_EC_SENSOR_BASE );
        if(uiUART_EC_Cnt >= macroUART_MAX_LENGHT - 1)
		{
            uiUART_EC_Cnt = 0;
		}
        uiUART_EC_Cnt++;
    }
#endif
    
#if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
#endif
    
}


/******************************************************************************
 * Function		: void macroUART_PH_SENSOR_IRQHandler( void )
 * Description	: This function get data receive from UART to array data_receive
 * Param		: none
 * Return		: none
*******************************************************************************/
void macroUART_PH_SENSOR_IRQHandler( void )
{
	if ((kUART_RxDataRegFullFlag | kUART_RxOverrunFlag) & UART_GetStatusFlags(macroUART_PH_SENSOR_BASE))
    {
        uUART_PH_Buffer[uiUART_PH_Cnt] = UART_ReadByte( macroUART_PH_SENSOR_BASE );
		
        if(uiUART_PH_Cnt >= macroUART_MAX_LENGHT - 1)
		{
            uiUART_PH_Cnt = 0;
			memset(uUART_PH_Buffer, 0, macroUART_MAX_LENGHT);
		}
#ifdef PH_SENSOR        
        else if(uUART_PH_Buffer[uiUART_PH_Cnt] == macroPH_END_CMD)
        {
            xFlags.eData_Recv = eUART_PH;
        }
#endif        
		uiUART_PH_Cnt++;
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
	
	for (uint32_t i = 0; i < macroWDOG_WCT_INSTRUCITON_COUNT; i++)
    {
        (void)xWdog_Base->RSTCNT;
    }
    
    /* Delay watchdog 1 second */
    while (((uint32_t)((((uint32_t)xWdog_Base->TMROUTH) << 16U) | (xWdog_Base->TMROUTL))) < (1000));
    APP_DEBUG("--- Main: Init watchdog time delay: %d ms complete\r\n", macroTIME_WATCHDOG_DELAY);
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
 * Function		: void vMain_TimerInit( void )
 * Description	: Init Timer 
 * Param		: none
 * Return		: none
*******************************************************************************/
void vMain_TimerInit(void)
{
	ftm_config_t ftmInfo;
	
    FTM_GetDefaultConfig(&ftmInfo);

    /* Divide FTM clock by 4 */
    ftmInfo.prescale = kFTM_Prescale_Divide_4;

    /* Initialize FTM module */
    FTM_Init(BOARD_FTM_BASEADDR, &ftmInfo);

    /*
     * Set timer period.
    */
    FTM_SetTimerPeriod(BOARD_FTM_BASEADDR, USEC_TO_COUNT(1000U, FTM_SOURCE_CLOCK));

    FTM_EnableInterrupts(BOARD_FTM_BASEADDR, kFTM_TimeOverflowInterruptEnable);

    EnableIRQ(BOARD_FTM_IRQ_NUM);

    FTM_StartTimer(BOARD_FTM_BASEADDR, kFTM_SystemClock);
}


/******************************************************************************
 * Function		: void vMain_TimerInit( void )
 * Description	: Init Timer 
 * Param		: none
 * Return		: none
*******************************************************************************/
void BOARD_FTM_HANDLER(void)
{
	ui16TimerCounter++ ;
    /* Clear interrupt flag.*/
    FTM_ClearStatusFlags(BOARD_FTM_BASEADDR, kFTM_TimeOverflowFlag);
	if(ui16TimerCounter == 1000)   //1s
    {
        ui16TimerCounter = 0;
        ui16SecCounter++;
        ui16WAMICounter++;
        uLedStatus++;
		ui16SendCounter++;
        if(ui16SecCounter >= macroTIME_READ_SENSOR)
        {
            xFlags.eReadSensor = eALL;
            ui16SecCounter = 0;
        }
        if(ui16WAMICounter >= macroTIME_SEND_WAMI)
        {
            xFlags.bWAMItoZigB = true;
            ui16WAMICounter = 0;
        }
        
		if(ui16SendCounter >= xDataLocal.uiTimeUpdate)	//time send to Zigb           
		{
			//send to Zigbee
			xFlags.bTimeSend = true;
			ui16SendCounter=0;
		}
    }
	
}

/******************************************************************************
 * Function		: void vMain_GPIO_Interrupt( void )
 * Description	: Init Timer 
 * Param		: none
 * Return		: none
*******************************************************************************/
//void vMain_GPIO_Interrupt(void)
//{
//	/* Define the init structure for the input switch pin */
//    gpio_pin_config_t sw_config = {
//        kGPIO_DigitalInput, 0,
//    };
//    /* Init input switch GPIO. */
//    PORT_SetPinInterruptConfig(macroGPIO_IE_PORT, macroGPIO_IE_PIN, kPORT_InterruptRisingEdge);
//    EnableIRQ(macro_GPIO_IE_IRQ);
//    GPIO_PinInit(macroGPIO_IE_GPIO, macroGPIO_IE_PIN, &sw_config);
//}
//
//void macro_GPIO_IE_IRQHandler(void)
//{
//	 /* Clear external interrupt flag. */
//    GPIO_PortClearInterruptFlags(macroGPIO_IE_GPIO, 1U << macroGPIO_IE_PIN);
// 	xFlags.bResp_Done_NanoTimer = true;
//    /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
//      exception return operation might vector to incorrect interrupt */
//#if defined __CORTEX_M && (__CORTEX_M == 4U)
//    __DSB();
//#endif
//	
//}

/******************************************************************************
 * Function		: void vMain_GetUniqueID(char *pcDeviceID)
 * Description	: Ham lay Unique ID of mcu
 * Param		: pcDeviceID - ID device of mcu
 * Return		: none
*******************************************************************************/
void vMain_GetUniqueID(char *pcDeviceID)
{
	sim_uid_t xSim_UID;
	uint8_t uValue = 0;
	
	SIM_GetUniqueId(&xSim_UID);
	
	pcDeviceID[0]  = ((uValue = (((xSim_UID.ML >> 24) & 0xFF) / 16)) <= 9)? (uValue | 0x30) : ((uValue - 9) | 0x41);
	pcDeviceID[1]  = ((uValue = (((xSim_UID.ML >> 24) & 0xFF) % 16)) <= 9)? (uValue | 0x30) : ((uValue - 9) | 0x41);
	pcDeviceID[2]  = ((uValue = (((xSim_UID.ML >> 16) & 0xFF) / 16)) <= 9)? (uValue | 0x30) : ((uValue - 9) | 0x41);
	pcDeviceID[3]  = ((uValue = (((xSim_UID.ML >> 16) & 0xFF) % 16)) <= 9)? (uValue | 0x30) : ((uValue - 9) | 0x41);
	pcDeviceID[4]  = ((uValue = (((xSim_UID.ML >> 8 ) & 0xFF) / 16)) <= 9)? (uValue | 0x30) : ((uValue - 9) | 0x41);
	pcDeviceID[5]  = ((uValue = (((xSim_UID.ML >> 8 ) & 0xFF) % 16)) <= 9)? (uValue | 0x30) : ((uValue - 9) | 0x41);
	pcDeviceID[6]  = ((uValue = (((xSim_UID.ML >> 0 ) & 0xFF) / 16)) <= 9)? (uValue | 0x30) : ((uValue - 9) | 0x41);
	pcDeviceID[7]  = ((uValue = (((xSim_UID.ML >> 0 ) & 0xFF) % 16)) <= 9)? (uValue | 0x30) : ((uValue - 9) | 0x41);
	pcDeviceID[8]  = ((uValue = (((xSim_UID.L  >> 24) & 0xFF) / 16)) <= 9)? (uValue | 0x30) : ((uValue - 9) | 0x41);
	pcDeviceID[9]  = ((uValue = (((xSim_UID.L  >> 24) & 0xFF) % 16)) <= 9)? (uValue | 0x30) : ((uValue - 9) | 0x41);
	pcDeviceID[10] = ((uValue = (((xSim_UID.L  >> 16) & 0xFF) / 16)) <= 9)? (uValue | 0x30) : ((uValue - 9) | 0x41);
	pcDeviceID[11] = ((uValue = (((xSim_UID.L  >> 16) & 0xFF) % 16)) <= 9)? (uValue | 0x30) : ((uValue - 9) | 0x41);
	pcDeviceID[12] = ((uValue = (((xSim_UID.L  >> 8 ) & 0xFF) / 16)) <= 9)? (uValue | 0x30) : ((uValue - 9) | 0x41);
	pcDeviceID[13] = ((uValue = (((xSim_UID.L  >> 8 ) & 0xFF) % 16)) <= 9)? (uValue | 0x30) : ((uValue - 9) | 0x41);
	pcDeviceID[14] = ((uValue = (((xSim_UID.L  >> 0 ) & 0xFF) / 16)) <= 9)? (uValue | 0x30) : ((uValue - 9) | 0x41);
	pcDeviceID[15] = ((uValue = (((xSim_UID.L  >> 0 ) & 0xFF) % 16)) <= 9)? (uValue | 0x30) : ((uValue - 9) | 0x41);
//    if(pcDeviceID[15]=='E')
//        pcDeviceID[15] = 'D';
}
