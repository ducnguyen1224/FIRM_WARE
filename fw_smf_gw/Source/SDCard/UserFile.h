/*
 * UserFile.h
 *
 *  Created on: Jul 20, 2018
 *      Author: ThaoHuyen
 */

#ifndef SOURCE_SDCARD_USERFILE_H_
#define SOURCE_SDCARD_USERFILE_H_

#include "FileAccess.h"


/*****************************************************************************
 * Function	: bool bUserFile_Read_MQTTConfig( uint8_t *Host, uint16_t *Port, uint8_t *User, uint8_t *Pwd )
 * Desc		: read mqtt config
 * Param	: None
 * return	: 
 *			+ true - ok
 *			+ false - nok
 *****************************************************************************/
bool bUserFile_Read_MQTTConfig( uint8_t *Host, uint16_t *Port, uint8_t *User, uint8_t *Pwd );




/*****************************************************************************
 * Function	: void vUserFile_Write_MQTTConfig( uint8_t *Host, uint16_t Port, uint8_t *User, uint8_t *Pwd )
 * Desc		: write mqtt config
 * Param	: None
 * return	: None
 *****************************************************************************/
void vUserFile_Write_MQTTConfig( uint8_t *Host, uint16_t Port, uint8_t *User, uint8_t *Pwd );




/*****************************************************************************
 * Function	: bool bUserFile_Read_RTC( uint8_t *Hour, uint8_t *Minute, uint8_t *Second, uint8_t *Day, uint8_t *Month, uint16_t *Year )
 * Desc		: read mqtt config
 * Param	: None
 * return	:
 * 			+ true - ok
 * 			+ false - nok
 *****************************************************************************/
bool bUserFile_Read_RTC( uint8_t *Hour, uint8_t *Minute, uint8_t *Second, uint8_t *Day, uint8_t *Month, uint16_t *Year );




/*****************************************************************************
 * Function	: void vUserFile_Write_RTC( uint8_t Hour, uint8_t Minute, uint8_t Second, uint8_t Day, uint8_t Month, uint16_t Year )
 * Desc		: write mqtt config
 * Param	: None
 * return	: None
 *****************************************************************************/
void vUserFile_Write_RTC( uint8_t Hour, uint8_t Minute, uint8_t Second, uint8_t Day, uint8_t Month, uint16_t Year );





/*****************************************************************************
 * Function	: bool bUserFile_Read_Connectivity_Long_Addr( char *EndDevice_File, uint8_t *LongAddr )
 * Desc		: read long address of connectivity
 * Param	: None
 * return	: true - ok, false - nok
 *****************************************************************************/
bool bUserFile_Read_Connectivity_Long_Addr( char *EndDevice_File, uint8_t *LongAddr );




/*****************************************************************************
 * Function	: void vUserFile_Write_Connectivity_Long_Addr( char *EndDevice_File, uint8_t *LongAddr )
 * Desc		: write long address of connectivity
 * Param	: None
 * return	: None
 *****************************************************************************/
void vUserFile_Write_Connectivity_Long_Addr( char *EndDevice_File, uint8_t *LongAddr );


#endif /* SOURCE_SDCARD_USERFILE_H_ */












