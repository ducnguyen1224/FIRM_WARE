
 /******************************************************************************
Filename:       SensorGas_task.h
Designed:       tuanna3
Revised:        $Date: 2016-10-25 8:36:30 -0700 (Tu, 25 Oct 2016) $
Decription: library read value Sensor through IC LMP91000 of TI.
            equation Nernst in 25oC 
            ->>anpha Nernst is 59.16mV/pH

Copyright 2015 VNPT-Technology . All rights reserved.


******************************************************************************/
#ifndef __SENSORGAS_TASK_H_
#define __SENSORGAS_TASK_H_
#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
* INCLUDES
*/

//#include "SSCommon.h"
#include "PacketHandle.h"

/*********************************************************************
* CONSTANTS
*/
//!***************************************************************************
//!	SOCKET defines
//!***************************************************************************
#define macroSENSOR_SOCKET_1	        			1
#define macroSENSOR_SOCKET_2	        			2
#define macroSENSOR_SOCKET_3	        			3
#define macroSENSOR_SOCKET_4	        			4
#define macroSENSOR_SOCKET_5	        			5
    
// inver
#define macroPOWER_PIN_SOCKET1       				(21U)
#define macroPOWER_PIN_SOCKET2       				(23U)
#define macroPOWER_PIN_SOCKET3       				(1U)
#define macroPOWER_PIN_SOCKET4       				(3U)
#define macroPOWER_PIN_SOCKET5       				(13U)
#define macroI2C_CHIP_SELECT_PIN_SOCKET1         	(20U)
#define macroI2C_CHIP_SELECT_PIN_SOCKET2         	(22U)
#define macroI2C_CHIP_SELECT_PIN_SOCKET3         	(0U)
#define macroI2C_CHIP_SELECT_PIN_SOCKET4         	(2U)
#define macroI2C_CHIP_SELECT_PIN_SOCKET5         	(12U)
	
	


/******************************************************************************
 * Function		: void vSensorTask_Task( void *pvParameters)
 * Description	: Task xu ly doc du lieu tu sensor
 * Param		: N/A
 * Return		: none
*******************************************************************************/
void vSensorTask_Task( void *pvParameters);    




/******************************************************************************
 * Function		: void vSensorTask_CheckAllSocketConnect( void )
 * Description	: Ham kiem tra socket co duoc ket noi voi sensor khong
 * Param		: none
 * Return		: none
*******************************************************************************/
void vSensorTask_CheckAllSocketConnect( void );




/******************************************************************************
 * Function		: void vSensorTask_GetDataFromSocket( SS_Value *xSS_Value, sensor_config *xSensor_Config )
 * Description	: Ham lay du lieu tu Socket
 * Param		: 
 * 				+ xSS_Value - Cau truc luu du lieu
 *				+ xSensor_Config - cau truc chua cau hinh cua moi socket
 * Return		: none
*******************************************************************************/
//void vSensorTask_GetDataFromSocket( SS_Value *xSS_Value, Sensor_Config *xSensor_Config, float fData );





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
void vSensorTask_SetDataSensor( PacketIO *xPacketIO, SS_Value *xSS_Value, char *cTypePacket, Enum_IDSensor eSendTypeSS );

    
    
    
#ifdef __cplusplus
}
#endif
#endif