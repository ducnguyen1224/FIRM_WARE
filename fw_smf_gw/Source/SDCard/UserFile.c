/*
 * UserFile.c
 *
 *  Created on: Jul 20, 2018
 *      Author: ThaoHuyen
 */
#include <stdio.h>

#include "AppData.h"

#ifdef macroUSE_SDCARD

#include "UserFile.h"


//#define macroUSER_FILE_MQTT_CONFIG				"mqttcloud.txt"
//#define macroUSER_FILE_MQTT_CONFIG				"mqttthing.txt"
#define macroUSER_FILE_MQTT_CONFIG				"mqtthosco.txt"
#define macroUSER_FILE_RTC_CONFIG				"RTC.txt"




/*****************************************************************************
 * Function	: bool bUserFile_Read_MQTTConfig( uint8_t *Host, uint16_t *Port, uint8_t *User, uint8_t *Pwd )
 * Desc		: read mqtt config
 * Param	: None
 * return	: 
 *			+ true - ok
 *			+ false - nok
 *****************************************************************************/
bool bUserFile_Read_MQTTConfig( uint8_t *Host, uint16_t *Port, uint8_t *User, uint8_t *Pwd )
{
	char Buffer[128] = {0};
	char Array[8][48] = {0};
	uint8_t ui = 0, uj = 0, uk = 0;

	if(bFileAccess_Read_All(macroUSER_FILE_MQTT_CONFIG, Buffer) == false)
	{
		APP_DEBUG("--- UserFile: Read mqtt config failture\r\n");
		return false;
	}

	for(ui = 0; Buffer[ui] > 0; ui++)
	{
		if( (Buffer[ui] == '\n') || (Buffer[ui] == '.') )
		{
			uj++;
			uk = 0;
		}
		else if(Buffer[ui] == '\r')
			continue;
		else
			Array[uj][uk++] = Buffer[ui];
	}

	APP_DEBUG("--- UserFile: Host = %s.%s.%s.%s\r\n", Array[0], Array[1], Array[2], Array[3]);
	APP_DEBUG("--- UserFile: Port = %s\r\n", Array[4]);
	APP_DEBUG("--- UserFile: User = %s\r\n", Array[5]);
	APP_DEBUG("--- UserFile: Pwd = %s\r\n", Array[6]);

	Host[0] = atoi(Array[0]);
	Host[1] = atoi(Array[1]);
	Host[2] = atoi(Array[2]);
	Host[3] = atoi(Array[3]);
	*Port = atoi(Array[4]);
	strcpy((char *)User, Array[5]);
	strcpy((char *)Pwd, Array[6]);
	return true;
}




/*****************************************************************************
 * Function	: void vUserFile_Write_MQTTConfig( uint8_t *Host, uint16_t Port, uint8_t *User, uint8_t *Pwd )
 * Desc		: write mqtt config
 * Param	: None
 * return	: None
 *****************************************************************************/
void vUserFile_Write_MQTTConfig( uint8_t *Host, uint16_t Port, uint8_t *User, uint8_t *Pwd )
{
	char cStr[256] = {0};

	sprintf(cStr, "%d.%d.%d.%d\r\n%d\r\n%s\r\n%s\r\n", \
					Host[0], Host[1], Host[2], Host[3], \
					Port, \
					(char *)User, \
					(char *)Pwd \
			);

	if(bFileAccess_Write_All(macroUSER_FILE_MQTT_CONFIG, cStr) == false)
	{
		APP_DEBUG("--- UserFile: Write mqtt config error\r\n");
	}
}




/*****************************************************************************
 * Function	: bool bUserFile_Read_RTC( uint8_t *Hour, uint8_t *Minute, uint8_t *Second, uint8_t *Day, uint8_t *Month, uint16_t *Year )
 * Desc		: read mqtt config
 * Param	: None
 * return	:
 * 			+ true - ok
 * 			+ false - nok
 *****************************************************************************/
bool bUserFile_Read_RTC( uint8_t *Hour, uint8_t *Minute, uint8_t *Second, uint8_t *Day, uint8_t *Month, uint16_t *Year )
{
	#define RTC_MAX_FIELD			12
	#define RTC_MAX_FIELD_LENGHT	8
	char Buffer[RTC_MAX_FIELD * RTC_MAX_FIELD_LENGHT + 18] = {0};
	char Array[RTC_MAX_FIELD][RTC_MAX_FIELD_LENGHT] = {0};
	uint8_t ui = 0, uj = 0, uk = 0;

	if(bFileAccess_Read_All(macroUSER_FILE_RTC_CONFIG, Buffer) == false)
	{
		APP_DEBUG("--- UserFile: Read RTC config is failture\r\n");
		return false;
	}

	for(ui = 0; Buffer[ui] > 0; ui++)
	{
		if( (Buffer[ui] == '\n') || (Buffer[ui] == '=') )
		{
			uj++;
			uk = 0;
		}
		else if(Buffer[ui] == '\r')
			continue;
		else
			Array[uj][uk++] = Buffer[ui];
		if(uj > RTC_MAX_FIELD)
			ERROR();
		if(uk > RTC_MAX_FIELD_LENGHT)
			ERROR();

	}

	*Hour 	= atoi(Array[1]);
	*Minute = atoi(Array[3]);
	*Second = atoi(Array[5]);
	*Day 	= atoi(Array[7]);
	*Month 	= atoi(Array[9]);
	*Year 	= atoi(Array[11]);

	APP_DEBUG("--- UserFile: RTC Read = %d:%d:%d - %d/%d/%d\r\n", *Hour, *Minute, *Second, *Day, *Month, *Year);
	return true;
}




/*****************************************************************************
 * Function	: void vUserFile_Write_RTC( uint8_t Hour, uint8_t Minute, uint8_t Second, uint8_t Day, uint8_t Month, uint16_t Year )
 * Desc		: write mqtt config
 * Param	: None
 * return	: None
 *****************************************************************************/
void vUserFile_Write_RTC( uint8_t Hour, uint8_t Minute, uint8_t Second, uint8_t Day, uint8_t Month, uint16_t Year )
{
	char cStr[128] = {0};

	sprintf(cStr, "hour=%d\r\nminute=%d\r\nsecond=%d\r\nday=%d\r\nmonth=%d\r\nyear=%d\r\n", \
					Hour, Minute, Second, Day, Month, Year);

	if(bFileAccess_Write_All(macroUSER_FILE_RTC_CONFIG, cStr) == false)
		APP_DEBUG("--- UserFile: Write RTC config error\r\n");
}





/*****************************************************************************
 * Function	: bool bUserFile_Read_Connectivity_Long_Addr( char *EndDevice_File, uint8_t *LongAddr )
 * Desc		: read long address of connectivity
 * Param	: None
 * return	: true - ok, false - nok
 *****************************************************************************/
bool bUserFile_Read_Connectivity_Long_Addr( char *EndDevice_File, uint8_t *LongAddr )
{
	char Buffer[24] = {0};

	if(bFileAccess_CheckFile_Exist(EndDevice_File) == false)
	{
		APP_DEBUG("--- UserFile: End device not exist\r\n");
		return false;
	}
	if(bFileAccess_Read_All(EndDevice_File, Buffer) == false)
		return false;

	strcpy((char *)LongAddr, Buffer);
	APP_DEBUG("--- UserFile: ZB IEEE = %s\r\n", (char *)LongAddr);
	return true;
}




/*****************************************************************************
 * Function	: void vUserFile_Write_Connectivity_Long_Addr( char *EndDevice_File, uint8_t *LongAddr )
 * Desc		: write long address of connectivity
 * Param	: None
 * return	: None
 *****************************************************************************/
void vUserFile_Write_Connectivity_Long_Addr( char *EndDevice_File, uint8_t *LongAddr )
{
	if(bFileAccess_Write_All(EndDevice_File, (char *)LongAddr) == false)
	{
		APP_DEBUG("--- UserFile: Write long address of connectivity fail\r\n");
		return;
	}
}

#endif





















