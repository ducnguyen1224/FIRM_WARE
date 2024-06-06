/*
 * UserFile.h
 *
 *  Created on: Jul 20, 2018
 *      Author: ThaoHuyen
 */

#ifndef SOURCE_SDCARD_USERFILE_H_
#define SOURCE_SDCARD_USERFILE_H_

#include "FileAccess.h"

   
#define macroUSER_FILE				"user_config.txt"

#define macroUSER_FILE_PH				"PH.txt"
#define macroUSER_FILE_EC				"EC.txt"
#define macroUSER_FILE_LIGHT			"LIGHT.txt"
#define macroUSER_FILE_TEMPA		    "TEMPA.txt"
#define macroUSER_FILE_HUMIA			"HUMIA.txt"
#define macroUSER_FILE_TEMPG		    "TEMPG.txt"
#define macroUSER_FILE_HUMIG			"HUMIG.txt"
#define macroUSER_FILE_TIMESEND			"TIMESEND.txt"

/*****************************************************************************
 * Function	: bool bUserFile_Read_MQTTConfig( uint8_t *Host, uint16_t *Port, uint8_t *User, uint8_t *Pwd )
 * Desc		: read mqtt config
 * Param	: None
 * return	: 
 *			+ true - ok
 *			+ false - nok
 *****************************************************************************/
//bool bUserFile_Read( DataLocal* xDataLocalIO);




/*****************************************************************************
 * Function	: void vUserFile_Write_MQTTConfig( uint8_t *Host, uint16_t *Port, uint8_t *User, uint8_t *Pwd )
 * Desc		: write mqtt config
 * Param	: None
 * return	: None
 *****************************************************************************/
//bool bUserFile_Write( DataLocal* xDataLocalIO );


#endif /* SOURCE_SDCARD_USERFILE_H_ */


bool bUserFile_WriteFile( char* filename, float fThresh_Low, float fThresh_High );



bool bUserFile_ReadFile(char* filename, float* fThresh_Low, float* fThresh_High);


bool bUserFile_ReadAll(DataLocal* xDataLocalIO);


