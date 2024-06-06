

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "fsl_gpio.h"  
#include "fsl_port.h"

#include "main.h"
#include "AppData.h"

#include "I2C.h"
#include "UART.h"
#include "SensorTask.h"
#include "DeviceControl.h"

#include "PH_Sensor.h"
#include "SHT11.h"
#include "SHT11Soil.h"
#include "BH1750.h"
//#include "EC_Sensor.h"
#include "EC_Murata.h"


PacketIO xPacketSend;

SS_Value xSS_Value = {0};

SS_Value xTemp_Val = {0};		//luu tru gia tri sensor tuc thoi

//Task
extern taskHandle_t xTask;

//Flags
extern Flags xFlags;

extern uint16_t uiUART_EC_Cnt;
extern uint8_t uUART_EC_Buffer[macroUART_MAX_LENGHT + 1];


extern char cID_EndDevice[17];

extern DataLocal xDataLocal;

/******************************************************************************
 * Function		: void vSensorTask_Task( void *pvParameters)
 * Description	: Task xu ly doc du lieu tu sensor
 * Param		: N/A
 * Return		: none
*******************************************************************************/
void vSensorTask_Task( void *pvParameters)
{
	
	APP_DEBUG("************************* SensorTask *************************\r\n");
	macroTASK_DELAY_MS(30000);
    while( 1 )
    {
		uint8_t ui8ReadEC=0;
		uint8_t ui8CountReadEC =0;
		char cDataEC[7]={0};
		if(xFlags.eReadSensor != eNone)
		{
			APP_DEBUG("--- SensorTask: Reading sensor...\r\n");
#ifdef PH_SENSOR
            /*pH sensor, data pH already store xss_value.fpH after this funtion*/
			PH_Sensor_vRead();
			macroTASK_DELAY_MS(1000);
			//check val pH vua doc dc
			if((xTemp_Val.fpH >xDataLocal.xPH.ui16LowThresh)&&(xTemp_Val.fpH < xDataLocal.xPH.ui16HighThresh))
				xSS_Value.fpH = xTemp_Val.fpH;
#endif

//#ifdef EC_SENSOR
//			/*EC sensor, data ec already store xss_value.fEC after this funtion*/
//			EC_Sensor_vRead();
//			macroTASK_DELAY_MS(500);
            //check val vua doc dc
//			if((xTemp_Val.fEC > xDataLocal.xEC.ui16LowThresh)&&(xTemp_Val.fEC < xDataLocal.xEC.ui16HighThresh))
//				xSS_Value.fEC = xTemp_Val.fEC;
//#endif

#ifdef SHTA_SENSOR
			/*SHT11: TempA & HumiA*/
			SHT11(&xTemp_Val.fTempA, &xTemp_Val.fHumiA);
			macroTASK_DELAY_MS(500);
			//check val vua doc dc
			if((xTemp_Val.fTempA >xDataLocal.xTempA.ui16LowThresh)&&(xTemp_Val.fTempA < xDataLocal.xTempA.ui16HighThresh))
				xSS_Value.fTempA = xTemp_Val.fTempA;
			if((xTemp_Val.fHumiA >xDataLocal.xHumiA.ui16LowThresh)&&(xTemp_Val.fHumiA < xDataLocal.xHumiA.ui16HighThresh))
				xSS_Value.fHumiA = xTemp_Val.fHumiA;
#endif
			
#ifdef SHTG_SENSOR
            /*SHT11: TempG & HumiG*/
          	SHT11Ground(&xTemp_Val.fTempG, &xTemp_Val.fHumiG);
			macroTASK_DELAY_MS(500);
			//check val vua doc dc
			if((xTemp_Val.fTempG > xDataLocal.xTempG.ui16LowThresh)&&(xTemp_Val.fTempG < xDataLocal.xTempG.ui16HighThresh))
				xSS_Value.fTempG = xTemp_Val.fTempG;
			if((xTemp_Val.fHumiG > xDataLocal.xHumiG.ui16LowThresh)&&(xTemp_Val.fHumiG < xDataLocal.xHumiG.ui16HighThresh))
				xSS_Value.fHumiG = xTemp_Val.fHumiG;
#endif

#ifdef LIGHT_SENSOR
			/*Light sensor*/
			xTemp_Val.fLight = BH1750_vGetLux();
            macroTASK_DELAY_MS(500);
			//check val vua doc dc
			if((xTemp_Val.fLight > xDataLocal.xLight.ui16LowThresh)&&(xTemp_Val.fLight < xDataLocal.xLight.ui16HighThresh))
				xSS_Value.fLight = xTemp_Val.fLight;
#endif

#ifdef EC_SENSOR
			/*read sensor EC murata*/
			/*format buffer EC and process data*/
			while((ui8ReadEC == 0)&&(ui8CountReadEC <5))
			{
				EC_Sensor_vStart();
				macroTASK_DELAY_MS(1000);
				memset(uUART_EC_Buffer, 0, macroUART_MAX_LENGHT);
				memset(cDataEC, 0, 7);
				uiUART_EC_Cnt = 0;
				macroTASK_DELAY_MS(500);
				EC_Sensor_vRead();
				macroTASK_DELAY_MS(1000);
				/*process data rev*/
				if(uUART_EC_Buffer[0]== macroEC_READ_CMD_MRT)
				{
					for (uint8_t ui =0; ui < 7; ui++  )
						cDataEC[ui]= uUART_EC_Buffer[ui];
					ui8ReadEC=1;
				}
				else
					ui8CountReadEC++;
				macroTASK_DELAY_MS(100);
				
				/*Process data EC*/
				if(ui8ReadEC==1)
					xTemp_Val.fEC = EC_Sensor_fCalcEC(cDataEC);
			}
			
			//check val vua doc dc
			if((xTemp_Val.fEC > xDataLocal.xEC.ui16LowThresh)&&(xTemp_Val.fEC < xDataLocal.xEC.ui16HighThresh))
				xSS_Value.fEC = xTemp_Val.fEC;
#endif
#ifdef PH_SENSOR
			APP_DEBUG("--- SensorTask: pH = %f\r\n",xSS_Value.fpH);
#endif

#ifdef EC_SENSOR			
			APP_DEBUG("--- SensorTask: EC = %f\r\n",xSS_Value.fEC);
#endif           
			
#ifdef SHTA_SENSOR           
			APP_DEBUG("--- SensorTask: TempA = %f, HumiA = %f\r\n",xSS_Value.fTempA,xSS_Value.fHumiA);
#endif            
            
#ifdef SHTG_SENSOR            
            APP_DEBUG("--- SensorTask: TempG = %f, HumiG = %f\r\n",xSS_Value.fTempG,xSS_Value.fHumiG);
#endif            
			
#ifdef LIGHT_SENSOR            
			APP_DEBUG("--- SensorTask: Light Lux = %d\r\n",xSS_Value.fLight);
#endif			
			vSensorTask_SetDataSensor( &xPacketSend, &xSS_Value, macroTYPEPACKET_DATA, xFlags.eReadSensor);
			
			xFlags.bData_Send = true;
			xFlags.bSendReady = false;	//turn on when rev who am I response
		}
		
		xFlags.eReadSensor = eNone;
		
		//Task finish
		xTask.uiSensorTask_Finish = 0;
        vTaskSuspend( NULL );
    }
}


/******************************************************************************
 * Function		: void vSensorTask_SetDataSensor( PacketIO *xPacketIO, SS_Value *xSS_Value, char *cTypePacket, Enum_IDSensor eSendTypeSS )
 * Description	: Ham tao cau truc du lieu
 * Param		: 
 * 				+ xPacketIO - Cau truc chua du lieu
 *				+ xSS_Value: cau truc du lieu dau vao
 *				+ cTypePacket - loai ban tin
 *				+ eSendTypeSS - Loai sensor de dua vao cau truc
 * Return		: none
*******************************************************************************/
void vSensorTask_SetDataSensor( PacketIO *xPacketIO, SS_Value *xSS_Value, char *cTypePacket, Enum_IDSensor eSendTypeSS )
{
	uint8_t uCnt = 0;
	vPacketHandle_Coppy(xPacketIO->cTypePacket, cTypePacket, 0);
	vPacketHandle_Coppy(xPacketIO->cTypeDevice, macroTYPEDEVICE_SS, 0);
    vPacketHandle_Coppy(xPacketIO->cID, cID_EndDevice , 0);
	xPacketIO->eOutput = eZigB;

#ifdef PH_SENSOR
    if((eSendTypeSS == eALL) || (eSendTypeSS == ePH))
	{
		//pH
		vPacketHandle_Coppy(xPacketIO->Data[uCnt].cName, macroSENSOR_PH, 0);
		sprintf(xPacketIO->Data[uCnt].cInfo, "%0.2f", xSS_Value->fpH);
		uCnt++;
	}
#endif

#ifdef EC_SENSOR
    if((eSendTypeSS == eALL) || (eSendTypeSS == eEC))
	{
		//EC
		vPacketHandle_Coppy(xPacketIO->Data[uCnt].cName, macroSENSOR_EC, 0);
		sprintf(xPacketIO->Data[uCnt].cInfo, "%0.2f", xSS_Value->fEC);
		uCnt++;
	}
#endif

#ifdef SHTG_SENSOR
	if((eSendTypeSS == eALL) || (eSendTypeSS == eTempG))
	{
		//TempG
		vPacketHandle_Coppy(xPacketIO->Data[uCnt].cName, macroSENSOR_TEMPG, 0);
		sprintf(xPacketIO->Data[uCnt].cInfo, "%0.2f", xSS_Value->fTempG);
		uCnt++;
	}

	if((eSendTypeSS == eALL) || (eSendTypeSS == eHumiG))
	{
		//Humi
		vPacketHandle_Coppy(xPacketIO->Data[uCnt].cName, macroSENSOR_HUMIG, 0);
		sprintf(xPacketIO->Data[uCnt].cInfo, "%0.2f", xSS_Value->fHumiG);
		uCnt++;
	}
#endif
	
#ifdef SHTA_SENSOR
	if((eSendTypeSS == eALL) || (eSendTypeSS == eTempA))
	{
		//TempA
		vPacketHandle_Coppy(xPacketIO->Data[uCnt].cName, macroSENSOR_TEMPA, 0);
		sprintf(xPacketIO->Data[uCnt].cInfo, "%0.2f", xSS_Value->fTempA);
		uCnt++;
	}

	if((eSendTypeSS == eALL) || (eSendTypeSS == eHumiA))
	{
		//HumiA
		vPacketHandle_Coppy(xPacketIO->Data[uCnt].cName, macroSENSOR_HUMIA, 0);
		sprintf(xPacketIO->Data[uCnt].cInfo, "%0.2f", xSS_Value->fHumiA);
		uCnt++;
	}
#endif
	
#ifdef LIGHT_SENSOR
	if((eSendTypeSS == eALL) || (eSendTypeSS == eLight))
	{
		//Light
		vPacketHandle_Coppy(xPacketIO->Data[uCnt].cName, macroSENSOR_LIGHT, 0);
		sprintf(xPacketIO->Data[uCnt].cInfo, "%d", xSS_Value->fLight);
		uCnt++;
	}
#endif
	xPacketIO->uDataNumber = uCnt;
}





    
