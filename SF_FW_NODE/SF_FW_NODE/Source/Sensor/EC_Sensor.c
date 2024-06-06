

#include "main.h"
#include "UART.h"
#include "EC_Sensor.h"
/******************************************************************************

*******************************************************************************/
void EC_Sensor_vInit(void)
{
	macroPOWER_ON(macroEC_POWER_GPIO,macroEC_POWER_PIN);
	//disable response
	EC_Sensor_vSendCMD(macroEC_DISABLE_RESP_CMD);
	//disable read continous mode
	EC_Sensor_vSendCMD(macroEC_DISABLE_CONT_MODE_CMD);
}
/******************************************************************************

*******************************************************************************/
void EC_Sensor_vSendCMD(char* cmd)
{
    vUART_Write(macroUART_EC_SENSOR_BASE, (char*)cmd, strlen(cmd));
}
/******************************************************************************

*******************************************************************************/
void EC_Sensor_vRead(void)
{
	EC_Sensor_vSendCMD(macroEC_SINGLE_READ_CMD);
}

/******************************************************************************

*******************************************************************************/