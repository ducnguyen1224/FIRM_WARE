
#ifndef __EC_SENSOR_H
#define __EC_SENSOR_H

#include "pin_mux.h"
#include "fsl_gpio.h"

#define macroEC_DISABLE_RESP_CMD				"*OK,0\r\n"
#define macroEC_ENABLE_RESP_CMD				"*OK,1\r\n"
#define macroEC_DISABLE_CONT_MODE_CMD			"C,0\r\n"
#define macroEC_ENABLE_CONT_MODE_CMD			"C,1\r\n"
#define macroEC_SINGLE_READ_CMD				"R\r\n"
#define macroEC_SET_BAURATE9600_CMD			"Baud,9600\r\n"
#define macroEC_SET_BAURATE38400_CMD			"Baud,38400\r\n"
#define macroEC_SET_BAURATE115200_CMD			"Baud,115200\r\n"
//more define for baurate if need
#define macroEC_RESET_FACTORY_CMD				"Factory\r\n"
#define macroEC_END_CMD							0x0D

void EC_Sensor_vInit(void);

void EC_Sensor_vRead(void);

void EC_Sensor_vSendCMD(char* cmd);

#endif
