
#include <stdlib.h>
#include <stdio.h>
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
#include "PacketHandle.h"
#include "ProcessTask.h"
#include "SensorTask.h"
#include "SDCardTask.h"

#define macroTIME_DELAY_LOOP				50

extern taskHandle_t xTask;

//For uart
extern uint8_t uUART_ZigB_Buffer[macroUART_MAX_LENGHT + 1];
extern uint16_t uiUART_ZigB_Cnt;

//Uart  sensor
extern uint8_t uUART_EC_Buffer[macroUART_MAX_LENGHT + 1];
extern uint16_t uiUART_EC_Cnt;
extern uint8_t uUART_PH_Buffer[macroUART_MAX_LENGHT + 1];
extern uint16_t uiUART_PH_Cnt;

extern Flags xFlags;
extern PacketIO xPacketSend;
extern SS_Value xSS_Value;
extern SS_Value xTemp_Val;
extern uint8_t uLedStatus;


/********************************* Function ***********************************/
void vProcessTask_LPTMR_Init(void);
/******************************************************************************/


/******************************************************************************
 * Function		: void vProcessTask_Task( void *pvParameters )
 * Description	: Task xu ly kiem tra cac co de thuc hien cac action tuong ung
 * Param		: N/A
 * Return		: none
*******************************************************************************/
void vProcessTask_Task( void *pvParameters )
{	
	uint8_t uReSendCnt = 0;
	uint8_t uSendLoopCnt = 0;
	
	APP_DEBUG("*********************** ProcessTask *************************\r\n");
	APP_DEBUG("--- ProcessTask: Device init finish\r\n");
	macroTASK_DELAY_MS( 1500 );
	
    while( 1 )
    {
		macroTASK_DELAY_MS( macroTIME_DELAY_LOOP );
		macroNANO_TIMER_DONE();
		
		//blink led status	
		if(uLedStatus < 1)
		{
			macroGPIO_ACTIVE_HIGH(macroLED_STATUS_GPIO, macroLED_STATUS_GPIO_PIN);
		}
		else if(uLedStatus >= 1)
		{
			macroGPIO_ACTIVE_LOW(macroLED_STATUS_GPIO, macroLED_STATUS_GPIO_PIN);
			uLedStatus = 0;
		}
		
		//Kiem tra co nhan qua UART ZigB
		if(xFlags.eData_Recv == eUART_ZigB)
		{
			vPacketHandle_HandleMessage( (char *)uUART_ZigB_Buffer );
			memset((void *)uUART_ZigB_Buffer, 0, uiUART_ZigB_Cnt + 1);
			uiUART_ZigB_Cnt = 0;
			xFlags.eData_Recv = eUART_None;
		}
		//From EC sensor atlats
//#ifdef EC_SENSOR
//		else if(xFlags.eData_Recv == eUART_EC)
//		{
//			xTemp_Val.fEC = atof((char*)uUART_EC_Buffer);
//			memset((void *)uUART_EC_Buffer, 0, uiUART_EC_Cnt + 1);
//			uiUART_EC_Cnt = 0;
//			xFlags.eData_Recv = eUART_None;
//		}
//#endif
		
#ifdef PH_SENSOR
		//From pH sensor
		else if(xFlags.eData_Recv == eUART_PH)
		{
			// process if need
			//save data tam thoi
			xTemp_Val.fpH = atof((char*)uUART_PH_Buffer);
			memset((void *)uUART_PH_Buffer, 0, uiUART_PH_Cnt + 1);
			uiUART_PH_Cnt = 0;
			xFlags.eData_Recv = eUART_None;
		}
#endif
		//kiem tra ket noi Zigb trc khi send
		if(xFlags.bSendReady == false)
		{
			xTask.uiWhoAmITask_Finish = 1;
			vTaskResume(xTask.xTaskHandle_WhoAmI);
		}

		//Kiem tra co truyen du lieu va den thoi gian send msg
		if( ((xFlags.bData_Send == true)&&(xFlags.bSendReady == true) && (xFlags.bTimeSend == true) ) ||\
            ((xFlags.bData_Send == true)&&(xFlags.bSendReady == true) && (xFlags.bSendReqs == true) ) ) 
		{
			APP_DEBUG("--- ProcessTask: Sending Data...\r\n");
			vPacketHandle_SendMessage( &xPacketSend );
			xFlags.bData_Send = false;	//=true when set package done
			xFlags.eSentIsOK = eSend_NOK;	// doi ban tin response
		}
		
//		//kiem tra send da oke chua??
		if(xFlags.eSentIsOK == eSend_NOK)
		{
			uReSendCnt++;
		}
		else if(xFlags.eSentIsOK == eSend_OK)
		{
			//memset xPacket send
			memset(&xPacketSend,0, sizeof(xPacketSend));
			xFlags.eSentIsOK = eSend_None;	//reset flag send
			uReSendCnt =0;
			uSendLoopCnt =0;
			xFlags.bTimeSend = false;
            xFlags.bSendReqs = false;
		}
		else 
		{
			uReSendCnt =0;
			uSendLoopCnt =0;
		}
		
//		//resend
		if(uReSendCnt >= (10 * (1000 / macroTIME_DELAY_LOOP)))
		{
			uReSendCnt = 0;
			uSendLoopCnt++;
		}
		if(uSendLoopCnt > 3)	//Gui 3 lan ma khong phan hoi -> thoat
		{
			APP_DEBUG("--- ProcessTask: Can't send data to Zigbee.\r\n");
			xFlags.bData_Send = false;	// k gui resend nua
			xFlags.eSentIsOK = eSend_OK;	// coi nhu send OK
			uSendLoopCnt = 0;
		}
		
		
		//Kiem tra doc cam bien
		if(xFlags.eReadSensor != eNone)
		{
			xTask.uiSensorTask_Finish = 1;
			vTaskResume( xTask.xTaskHandle_Sensor );
		}
		
        //send who am i dinh ky
		if(xFlags.bWAMItoZigB == true)
		{
			xFlags.bWAMItoZigB = false;
			xTask.uiWhoAmITask_Finish = 1;
			vTaskResume(xTask.xTaskHandle_WhoAmI);
		}
		
		//Kiem tra trang thai cac Task. Neu task nao bi loop hoac die thi restart lai
		if( xTask.uiSensorTask_Finish >= 1 )
			xTask.uiSensorTask_Finish++;
		if (xTask.uiWhoAmITask_Finish >=1)
			xTask.uiWhoAmITask_Finish++;
		if (xTask.uiSDCardTask_Finish >=1 )
			xTask.uiSDCardTask_Finish++;
		if( ( xTask.uiSensorTask_Finish >= (90 * (1000 / macroTIME_DELAY_LOOP)) ) || 	\
			( xTask.uiWhoAmITask_Finish >= (90 * (1000 / macroTIME_DELAY_LOOP)) ) ||	\
			( xTask.uiSDCardTask_Finish >= (90 * (1000 / macroTIME_DELAY_LOOP)) )  )
		{
			APP_DEBUG("--- ProcessTask: Task loop or die....\r\n");
			macroTASK_DELAY_MS( 1000 );
			NVIC_SystemReset();
		}
		//Refresh wdt
		vMain_RefreshWDT();
    }
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
    
//	uLedStatus++;
    /*
     * Workaround for TWR-KV58: because write buffer is enabled, adding
     * memory barrier instructions to make sure clearing interrupt flag completed
     * before go out ISR
     */
    __DSB();
    __ISB();
}





/******************************************************************************
 * Function		: void vMain_LPTMR_Init(void)
 * Description	: khoi tao low power timer
 * Param		: none
 * Return		: none
*******************************************************************************/
void vProcessTask_LPTMR_Init(void)
{
	lptmr_config_t lptmrConfig;
	
	/* Configure LPTMR */
    /*
     * lptmrConfig.timerMode = kLPTMR_TimerModeTimeCounter;
     * lptmrConfig.pinSelect = kLPTMR_PinSelectInput_0;
     * lptmrConfig.pinPolarity = kLPTMR_PinPolarityActiveHigh;
     * lptmrConfig.enableFreeRunning = false;
     * lptmrConfig.bypassPrescaler = true;
     * lptmrConfig.prescalerClockSource = kLPTMR_PrescalerClock_1;
     * lptmrConfig.value = kLPTMR_Prescale_Glitch_0;
     */
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



















