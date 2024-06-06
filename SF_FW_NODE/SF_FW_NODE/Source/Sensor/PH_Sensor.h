
#ifndef __PH_SENSOR_H
#define __PH_SENSOR_H

#include "pin_mux.h"
#include "fsl_gpio.h"

#define macroPH_DISABLE_RESP_CMD				"RESPONSE,0\r\n"
#define macroPH_ENABLE_RESP_CMD				"RESPONSE,1\r\n"
#define macroPH_DISABLE_CONT_MODE_CMD			"C,0\r\n"
#define macroPH_ENABLE_CONT_MODE_CMD			"C,1\r\n"
#define macroPH_SINGLE_READ_CMD				"R\r\n"
#define macroPH_SET_BAURATE9600_CMD			"SERIAL,9600\r\n"
#define macroPH_SET_BAURATE38400_CMD			"SERIAL,38400\r\n"
#define macroPH_SET_BAURATE115200_CMD			"SERIAL,115200\r\n"
//more define for baurate if need
#define macroPH_RESET_FACTORY_CMD				"X\r\n"
#define macroPH_END_CMD							0x0D

void PH_Sensor_vInit(void);

void PH_Sensor_vRead(void);

void PH_Sensor_vSendCMD(char* cmd);
#endif
