/*
 * UserFile.c
 *
 *  Created on: Jul 20, 2018
 *      Author: ThaoHuyen
 */
#include <stdio.h>

#include "UserFile.h"
#include "main.h"
	 
	 





/*****************************************************************************
 * Function	: bool bUserFile_Read_MQTTConfig( uint8_t *Host, uint16_t *Port, uint8_t *User, uint8_t *Pwd )
 * Desc		: read mqtt config
 * Param	: None
 * return	: 
 *			+ true - ok
 *			+ false - nok

* time update[0]: 500\n
* xpH[1]: 			0-14\n
* xEC[2]			0-2
	.....
* xLight			0-3000\n
 *****************************************************************************/
//bool bUserFile_Read( DataLocal* xDataLocalIO)
//{
//	char Buffer[256] = {0};
//	char Array[14][48] = {0};
//	uint8_t ui = 0, uj = 0, uk = 0;
//
//	if(bFileAccess_Read_All(macroUSER_FILE, Buffer) == false)
//	{
//		APP_DEBUG("--- UserFile: Read mqtt config failture\r\n");
//		return false;
//	}
//
//	for(ui = 0; Buffer[ui] > 0; ui++)
//	{
//		if( (Buffer[ui] == '\n') || (Buffer[ui] == '-') )
//		{
//			uj++;
//			uk = 0;
//		}
//		else if(Buffer[ui] == '\r')
//			continue;
//		else
//		{
//			Array[uj][uk++] = Buffer[ui];
//		}
//	}
//	APP_DEBUG("--- UserFile: Reading Thresh sensor......\r\n");
//	APP_DEBUG("--- UserFile: Time Read Sensor = %s\r\n", Array[0]);
//	APP_DEBUG("--- UserFile: pH = %s-%s\r\n", Array[1], Array[2]);
//	APP_DEBUG("--- UserFile: EC = %s-%s\r\n", Array[3], Array[4]);
//	APP_DEBUG("--- UserFile: Temp Air = %s-%s\r\n", Array[5], Array[6]);
//	APP_DEBUG("--- UserFile: Humi Air = %s-%s\r\n", Array[7], Array[8]);
//	APP_DEBUG("--- UserFile: Temp Soid = %s-%s\r\n", Array[9], Array[10]);
//	APP_DEBUG("--- UserFile: Humi Soid = %s-%s\r\n", Array[11], Array[12]);
//	APP_DEBUG("--- UserFile: Light = %s-%s\r\n", Array[13], Array[14]);
//
//	xDataLocalIO->uiTimeUpdate = atoi(Array[0]);
//	xDataLocalIO->xPH.ui16LowThresh = atoi(Array[1]);
//	xDataLocalIO->xPH.ui16HighThresh = atoi(Array[2]);
//	
//	xDataLocalIO->xEC.ui16LowThresh = atoi(Array[3]);
//	xDataLocalIO->xEC.ui16HighThresh = atoi(Array[4]);
//	
//	xDataLocalIO->xTempA.ui16LowThresh = atoi(Array[5]);
//	xDataLocalIO->xTempA.ui16HighThresh = atoi(Array[6]);
//	
//	xDataLocalIO->xHumiA.ui16LowThresh = atoi(Array[7]);
//	xDataLocalIO->xHumiA.ui16HighThresh = atoi(Array[8]);
//	
//	xDataLocalIO->xTempG.ui16LowThresh = atoi(Array[9]);
//	xDataLocalIO->xTempG.ui16HighThresh = atoi(Array[10]);
//	
//	xDataLocalIO->xHumiG.ui16LowThresh = atoi(Array[11]);
//	xDataLocalIO->xHumiG.ui16HighThresh = atoi(Array[12]);
//	
//	xDataLocalIO->xLight.ui16LowThresh = atoi(Array[13]);
//	xDataLocalIO->xLight.ui16HighThresh = atoi(Array[14]);
//	
//	return true;
//}




/*****************************************************************************
 * Function	: void vUserFile_Write_MQTTConfig( uint8_t *Host, uint16_t *Port, uint8_t *User, uint8_t *Pwd )
 * Desc		: write mqtt config
 * Param	: None
 * return	: None
 *****************************************************************************/
//bool bUserFile_Write( DataLocal* xDataLocalIO )
//{
//	char cStr[256] = {0};
//
//	sprintf(cStr, "%d\r\n%d-%d\r\n%d-%d\r\n%d-%d\r\n%d-%d\r\n%d-%d\r\n%d-%d\r\n%d-%d\r\n", \
//					xDataLocalIO->uiTimeUpdate,\
//					xDataLocalIO->xPH.ui16LowThresh,xDataLocalIO->xPH.ui16HighThresh,\
//					xDataLocalIO->xEC.ui16LowThresh,xDataLocalIO->xEC.ui16HighThresh,\
//					xDataLocalIO->xTempA.ui16LowThresh,xDataLocalIO->xTempA.ui16HighThresh,\
//					xDataLocalIO->xHumiA.ui16LowThresh,xDataLocalIO->xHumiA.ui16HighThresh,\
//					xDataLocalIO->xTempG.ui16LowThresh,xDataLocalIO->xTempG.ui16HighThresh,\
//					xDataLocalIO->xHumiG.ui16LowThresh,xDataLocalIO->xHumiG.ui16HighThresh,\
//					xDataLocalIO->xLight.ui16LowThresh,xDataLocalIO->xLight.ui16HighThresh
//			);
//
//	if(bFileAccess_Write_All(macroUSER_FILE, cStr) == false)
//	{
//		APP_DEBUG("--- UserFile: Write parameter config error\r\n");
//		return false;
//	}
//	
//	return true;
//}


/******************************************************************************/
bool bUserFile_ReadFile(char* filename, float* fThresh_Low, float* fThresh_High)
{
	//read file
	char Buffer[256]={0};
	char Array[3][48] = {0};
	uint8_t ui = 0, uj = 0, uk = 0;
	
	if(bFileAccess_Read_All(filename, Buffer) == false)
	{
		APP_DEBUG("--- UserFile: Read config failture\r\n");
		return false;
	}
	
	for(ui = 0; Buffer[ui] > 0; ui++)
	{
		if(Buffer[ui] == '\n')
		{
			uj++;
			uk = 0;
		}
		else if(Buffer[ui] == '\r')
			continue;
		else
		{
			Array[uj][uk++] = Buffer[ui];
		}
	}
	
	APP_DEBUG("--- UserFile: Reading DataOut......\r\n");
	APP_DEBUG("--- UserFile: Data = %s-%s\r\n", Array[0], Array[1]);
    
    if(*fThresh_High != -1)
      *fThresh_High= atof(Array[1]);
    
    *fThresh_Low = atof(Array[0]);
    

	return true;
}


/******************************************************************************/
bool bUserFile_WriteFile( char* filename, float fThresh_Low, float fThresh_High )
{
	char cStr[256] = {0};

    if(fThresh_High == -1)
      
        sprintf(cStr, "%f\r\n", fThresh_Low);
    else
        sprintf(cStr, "%f\r\n%f\r\n", fThresh_Low, fThresh_High);

	if(bFileAccess_Write_All(filename, cStr) == false)
	{
		APP_DEBUG("--- UserFile: Write parameter config error\r\n");
		return false;
	}
	return true;
}


/******************************************************************************/
bool bUserFile_ReadAll(DataLocal* xDataLocalIO)
{

  float iX = -1;
  float fDataUT = 0;
if(bUserFile_ReadFile(macroUSER_FILE_TIMESEND, &fDataUT, &iX) == false)
{
    APP_DEBUG("--- UserFile: Read config failture\r\n");
    return false;
}

xDataLocalIO->uiTimeUpdate = (int16_t)fDataUT;
  
#ifdef PH_SENSOR
    if(bUserFile_ReadFile(macroUSER_FILE_PH, &xDataLocalIO->xPH.ui16LowThresh, (float*)&xDataLocalIO->xPH.ui16HighThresh) == false)
    {
        APP_DEBUG("--- UserFile: Read config failture\r\n");
        return false;
    }
#endif
    
#ifdef EC_SENSOR
    if(bUserFile_ReadFile(macroUSER_FILE_EC, &xDataLocalIO->xEC.ui16LowThresh, &xDataLocalIO->xEC.ui16HighThresh) == false)
    {
        APP_DEBUG("--- UserFile: Read config failture\r\n");
        return false;
    }
#endif
    
#ifdef SHTA_SENSOR
    if(bUserFile_ReadFile(macroUSER_FILE_TEMPA, &xDataLocalIO->xTempA.ui16LowThresh, &xDataLocalIO->xTempA.ui16HighThresh) == false)
    {
        APP_DEBUG("--- UserFile: Read config failture\r\n");
        return false;
    }
    
    if(bUserFile_ReadFile(macroUSER_FILE_HUMIA, &xDataLocalIO->xHumiA.ui16LowThresh, &xDataLocalIO->xHumiA.ui16HighThresh) == false)
    {
        APP_DEBUG("--- UserFile: Read config failture\r\n");
        return false;
    }
#endif
    
#ifdef SHTG_SENSOR
    if(bUserFile_ReadFile(macroUSER_FILE_TEMPG, &xDataLocalIO->xTempG.ui16LowThresh, &xDataLocalIO->xTempG.ui16HighThresh) == false)
    {
        APP_DEBUG("--- UserFile: Read config failture\r\n");
        return false;
    }
    
    if(bUserFile_ReadFile(macroUSER_FILE_HUMIG, &xDataLocalIO->xHumiG.ui16LowThresh, &xDataLocalIO->xHumiG.ui16HighThresh) == false)
    {
        APP_DEBUG("--- UserFile: Read config failture\r\n");
        return false;
    }
#endif

#ifdef LIGHT_SENSOR
    if(bUserFile_ReadFile(macroUSER_FILE_LIGHT, &xDataLocalIO->xLight.ui16LowThresh, &xDataLocalIO->xLight.ui16HighThresh) == false)
    {
        APP_DEBUG("--- UserFile: Read config failture\r\n");
        return false;
    }
#endif

    
return true;
}










