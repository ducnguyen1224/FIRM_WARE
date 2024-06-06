
#include "PH_Sensor.h"
#include "main.h"
#include "UART.h"

#define macroPH_SEND_CMD(uart_base,cmd, len_cmd)		vUART_Write(macroUART_PH_SENSOR_BASE, (char*)cmd, len_cmd)
/******************************************************************************

*******************************************************************************/
void PH_Sensor_vInit(void)
{
	//disable response
	PH_Sensor_vSendCMD(macroPH_DISABLE_RESP_CMD);
//	//disable read continous mode
	PH_Sensor_vSendCMD(macroPH_DISABLE_CONT_MODE_CMD);
}
/******************************************************************************

*******************************************************************************/
void PH_Sensor_vSendCMD(char* cmd)
{
    vUART_Write(macroUART_PH_SENSOR_BASE, (char*)cmd, strlen(cmd));
}
/******************************************************************************

*******************************************************************************/
void PH_Sensor_vRead(void)
{
	PH_Sensor_vSendCMD(macroPH_SINGLE_READ_CMD);
}

/******************************************************************************

*******************************************************************************/