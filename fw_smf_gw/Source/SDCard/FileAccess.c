/*
 * FileAccess.c
 *
 *  Created on: Jul 19, 2018
 *      Author: ThaoHuyen
 */

#include "FileAccess.h"



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
bool bFileAccess_Open(FIL *fp, char *FileName, BYTE Mode)
{
	FRESULT res = f_open(fp, _T(FileName), Mode);

	if(res != FR_OK)
	{
		APP_DEBUG("\n--- FileAccess: Open File \"%s\" ERROR. res = %d\r\n", FileName, res);
		return false;
	}

	return true;
}




/*****************************************************************************
 * Function	: FSIZE_t  xFileAccess_GetSizeByID(FIL *fp)
 * Desc		: used to get size of file
 * Param	:
 * 			+ fp: ID of file
 * return	: size of file
 ****************************************************************************/
static FSIZE_t xFileAccess_GetSizeByID(FIL *fp)
{
	FSIZE_t size = f_size(fp);
	return size;
}





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
bool bFileAccess_Read_All(char *FileName, char *pDataOut)
{
	FIL fp;
	FRESULT Res;
	UINT br;

	if(bFileAccess_Open(&fp, FileName, FA_READ) == false)
		return false;

	// Get size
	FSIZE_t size = xFileAccess_GetSizeByID(&fp);
	if(size <= 0)
	{
		f_close(&fp);
		APP_DEBUG("--- FileAccess: File \"%s\" Empty.\r\n", FileName);
		return false;
	}

	memset(pDataOut, 0, strlen(pDataOut));

	Res = f_read(&fp, pDataOut, size, &br);
	f_close(&fp);
	if( (Res != FR_OK) || (br == 0) )
	{
		APP_DEBUG("--- FileAccess: Read file error. Res = %d, Lenght read = %d\r\n", Res, br);
		return false;
	}

	return true;
}




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
bool bFileAccess_Write_All(char *FileName, char *pDataIn)
{
	FIL fp;
	FRESULT Res;
	UINT bw;

	if(bFileAccess_Open(&fp, FileName, FA_CREATE_ALWAYS | FA_WRITE) == false)
		return false;

	Res = f_write(&fp, pDataIn, strlen(pDataIn), &bw);
	f_close(&fp);
	if( (Res != FR_OK) || (bw == 0) )
	{
		APP_DEBUG("--- FileAccess: write file error. Res = %d, Lenght write = %d\r\n", Res, bw);
		return false;
	}

	return true;
}





/*****************************************************************************
 * Function	: bool bFileAccess_CheckFile_Exist(char *FileName)
 * Desc		: check file exist
 * Param	:
 * 			+ FileName: Name of file
 * return	:
 *			+ true: existed
 *			+ false: not exist
 ****************************************************************************/
bool bFileAccess_CheckFile_Exist(char *FileName)
{
	FRESULT fr;
    FILINFO fno;

    fr = f_stat(FileName, &fno);

    if(fr == FR_NO_FILE)
		return false;
    else
    	return true;
}



















