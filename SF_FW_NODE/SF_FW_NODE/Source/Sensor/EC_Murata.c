

#include "main.h"
#include "UART.h"
#include "EC_Murata.h"

/******************************************************************************

*******************************************************************************/
void EC_Sensor_vInit(void)
{

}

/******************************************************************************

*******************************************************************************/
void EC_Sensor_vStart(void)
{
    char uStartCmd[6] = { 0x02, 0x07, 0x01, 0x01, 0x0D, 0x70 };
    EC_Sensor_vWriteCmd(uStartCmd);
}


/******************************************************************************

*******************************************************************************/
void EC_Sensor_vWriteCmd(char* uCommand)
{
     vUART_Write(macroUART_EC_SENSOR_BASE, (char*)uCommand, strlen(uCommand));
}

/******************************************************************************

*******************************************************************************/

void EC_Sensor_vRead()
{
    char uReadCmd[5] = { 0x01, 0x0B, 0x02, 0xF1, 0xA6 };
    EC_Sensor_vWriteCmd(uReadCmd);
}


/******************************************************************************/
unsigned short CRC16(int size, char* data)
{
    int i=0,j=0;
    unsigned short cr = 0xFFFF;
    for( i = 0; i < size; i++)
    {
        cr = cr ^ data[i];
        for( j = 0; j < 8; j++)
        {
            if((cr & 0x0001) == 0x0001)
            {
                cr >>= 1;
                cr ^= 0xA001;
            }
            else
            {
                cr >>= 1;
            }
        }
    }
    return cr;
}

/******************************************************************************
data[7]={0x01, 0x0B, 0x02, data_lower, data_upper, CRC16_lower, CRC16_upper}
*******************************************************************************/
float EC_Sensor_fCalcEC(char* data)
{
	APP_DEBUG("--- EC_Sensor_CalcEC: Start calculator..\r\n");
	
	char cBuff[7] = {0};
	char cCheck[5] = {0};
	
	float EC_Val = -1;
//	uint16_t EC_Val = 1;
	//copy sang mang khac de xu ly
	strcpy((char*)cBuff, (char*)data);
	
//	for (uint8_t ui =0; ui < 7; ui++  )
//		APP_DEBUG("--- EC_Sensor_CalcEC: hex : %x\r\n",cBuff[ui]);
	//tach lay phan data+header de check CRC
	for (uint8_t ui =0; ui <5; ui++)
		cCheck[ui]= cBuff[ui];
	
	//CRC of EC sensor
	unsigned short EC_CRC16 = ( (cBuff[5]<<8)| cBuff[6] );
	
	//check sum
	unsigned short CRC16_Cal = CRC16(sizeof(cCheck), (char*) cCheck);
	
	if(EC_CRC16 == CRC16_Cal)
	{
		
		EC_Val = (float)((   (float) ((cBuff[4]<<8)|cBuff[3] ) *2.0)/4095.0);
//		EC_Val = (cBuff[4]<<8)|cBuff[3];
		
		APP_DEBUG("--- EC_Sensor_CalcEC: EC_VAL = %f\r\n",EC_Val);
	}
	else
	{
		APP_DEBUG("--- EC_Sensor_CalcEC: check sum failed, read again\r\n");
	}

	return EC_Val;	
}