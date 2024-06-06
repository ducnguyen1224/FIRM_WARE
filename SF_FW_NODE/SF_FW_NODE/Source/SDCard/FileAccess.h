/*
 * FileAccess.h
 *
 *  Created on: Jul 19, 2018
 *      Author: ThaoHuyen
 */

#ifndef SOURCE_SDCARD_FILEACCESS_H_
#define SOURCE_SDCARD_FILEACCESS_H_

#include "ff.h"

#include "main.h"



/*****************************************************************************
 * Function	: bool bFileAccess_Open(FIL *fp, char *FileName, BYTE Mode)
 * Desc		: use to Open file
 * Param	:
 * 			+ FileName: Name of file
 * 			+ Mode: flag open
 * return	:
 * 			+ Successfully: true
 * 			+ ERROR		  : false
 *****************************************************************************/
bool bFileAccess_Open(FIL *fp, char *FileName, BYTE Mode);




/*****************************************************************************
 * Function	: bool bFileAccess_Read_All(char *FileName, char *pDataOut)
 * Desc		: used to Read all file
 * Param	:
 * 			+ FileName: Name of file
 * 			+ pDataOut: Data read output
 * return	:
 * 			+ false: Failture
 * 			+ true: successfully
 ****************************************************************************/
bool bFileAccess_Read_All(char *FileName, char *pDataOut);




/*****************************************************************************
 * Function	: bool bFileAccess_Write_All(char *FileName, char *pDataIn)
 * Desc		: used to clear old data and write data new to file
 * Param	:
 * 			+ FileName: Name of file
 * 			+ pDataIn: Data writen
 * return	:
 *			+ true: Successfully
 *			+ false: error
 ****************************************************************************/
bool bFileAccess_Write_All(char *FileName, char *pDataIn);




/*****************************************************************************
 * Function	: bool bFileAccess_CheckFile_Exist(char *FileName)
 * Desc		: check file exist
 * Param	:
 * 			+ FileName: Name of file
 * return	:
 *			+ true: existed
 *			+ false: not exist
 ****************************************************************************/
bool bFileAccess_CheckFile_Exist(char *FileName);




#endif /* SOURCE_SDCARD_FILEACCESS_H_ */










