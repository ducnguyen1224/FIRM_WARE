
#ifndef __EC_MURATA_H
#define __EC_MURATA_H

#include "main.h"
#include "pin_mux.h"
#include "fsl_gpio.h"

#define macroEC_READ_CMD_MRT		0x01

void EC_Sensor_vInit(void);

void EC_Sensor_vWriteCmd(char* uCommand);

void EC_Sensor_vRead(void);

unsigned short CRC16(int size, char* data);

void EC_Sensor_vStart(void);


float EC_Sensor_fCalcEC(char* data);
#endif
