
#ifndef __FILEACCESS_H
#define __FILEACCESS_H

#include "../main.h"



typedef enum
{
	eFileFailture = -1,
	eFileSuccess,
}Enum_FILEState;




/*****************************************************************************
 * Function	: file_t xFileAccess_Open(String FileName, enum FileOpenFlags OpenFlags)
 * Desc		: use to Open file
 * Param	:
 * 			+ FileName: Name of file
 * 			+ OpenFlags: flag open
 * return	:
 * 			+ Successfully: file description (>= 0)
 * 			+ ERROR		  : eFileFailture
 *****************************************************************************/
file_t xFileAccess_Open(String FileName, enum FileOpenFlags OpenFlags);




/*****************************************************************************
 * Function	: size_t xFileAccess_GetSizeByName(String FileName)
 * Desc		: used to get size of file
 * Param	:
 * 			+ FileName: Name of file
 * return	:
 * 			+ Size file	: Successfully
 * 			+ eFileFailture	: Failture
 ****************************************************************************/
size_t xFileAccess_GetSizeByName(String FileName);





/*****************************************************************************
 * Function	: size_t xFileAccess_GetSizeByID(file_t file)
 * Desc		: used to get size of file
 * Param	:
 * 			+ file: ID of file
 * return	: size of file
 ****************************************************************************/
size_t xFileAccess_GetSizeByID(file_t file);




/*****************************************************************************
 * Function	: Enum_FILEState eFileAccess_Read_All(String FileName, String *pDataOut)
 * Desc		: used to Read all file
 * Param	:
 * 			+ FileName: Name of file
 * 			+ pDataOut: Data read output
 * return	:
 * 			+ eFileFailture: Failture
 * 			+ eFileSuccess: successfully
 ****************************************************************************/
Enum_FILEState eFileAccess_Read_All(String FileName, char *pDataOut);




/*****************************************************************************
 * Function	: Enum_FILEState eFileAccess_Write_All(String FileName, char *pDataIn)
 * Desc		: used to clear old data and write data new to file
 * Param	:
 * 			+ FileName: Name of file
 * 			+ pDataIn: Data writen
 * return	:
 *			+ eFileSuccess: Successfully
 *			+ eFileFailture: error
 ****************************************************************************/
Enum_FILEState eFileAccess_Write_All(String FileName, char *pDataIn);




/*****************************************************************************
 * Function	: cFileAccess_ReadChar(char *FileName, unsigned long PointerPos)
 * Desc		: use to Read character of file
 * Param	:
 * 			+ FileName: Name of file
 * 			+ PointerPos: Pointer position in file
 * return	:
 * 			+ Successfully: character output
 ****************************************************************************/
char cFileAccess_ReadChar(file_t file);




/*****************************************************************************
 * Function	: Enum_FILEState eFileAccess_Read_Line(file_t FileID, char *pDataOut, int PointerPos)
 * Desc		: use to Read line of file
 * Param	:
 * 			+ FileID: ID of file
 * 			+ pDataOut: Data read output
 * 			+ PointerPos: Pointer position in file
 * return	:
 * 			+ eFileSuccess: Successfully
 *			+ eFileFailture: error
 ****************************************************************************/
Enum_FILEState eFileAccess_Read_Line(file_t FileID, char *pDataOut, int PointerPos);





/*****************************************************************************
 * Function	: Enum_FILEState eFileAccess_Write_AppendFile(String FileName, char *pDataIn)
 * Desc		: use to Write expand file
 * Param	:
 * 			+ FileName: Name of file
 * 			+ pDataIn: Data write input
 * return	:
 * 			+ eFileSuccess: Successfully
 *			+ eFileFailture: error
 ****************************************************************************/
Enum_FILEState eFileAccess_Write_AppendFile(String FileName, char *pDataIn);





/*****************************************************************************
 * Function	: Enum_FILEState eFileAccess_Write_ReplaceLineFile(String FileName, char *pDataIn, int PosLine)
 * Desc		: used to replace a line in file.
 * Param	:
 * 			+ FileName: Name of file
 * 			+ pDataIn: Data write input
 * 			+ PosLine: Position a line to replace
 * return	:
 * 			+ eFileSuccess: Successfully
 *			+ eFileFailture: error
 ****************************************************************************/
Enum_FILEState eFileAccess_Write_ReplaceLineFile(String FileName, char *pDataIn, int PosLine);





/*****************************************************************************
 * Function	: int iFileAccess_Find_LinebyString(String FileName, char *pDataFind)
 * Desc		: used to find line in file by the char *exist in this line.
 * Param	:
 * 			+ FileName: Name of file
 * 			+ pDataFind: char *in line
 * return	:
 * 			+ >= 0: position of line. start is 0
 * 			+ -1: find false
 ****************************************************************************/
int iFileAccess_Find_LinebyString(String FileName, char *pDataFind);




/*****************************************************************************
 * Function	: void vFileAccess_DeleteLine(Strinng FileName, int PosLine)
 * Desc		: used to delete line in file.
 * 			  The begin point of Line has position that it's value is Posbegin.
 * 			  The end point of Line has position that it's value is Posend.
 * Param	:
 * 			+ FileName: Name of file
 * 			+ PosLine: Position Line to delete
 * return	: none
 ****************************************************************************/
void vFileAccess_DeleteLine(String FileName, int PosLine);


#endif


