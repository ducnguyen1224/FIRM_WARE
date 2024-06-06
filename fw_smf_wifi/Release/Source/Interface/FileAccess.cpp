
#include "FileAccess.h"



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
file_t xFileAccess_Open(String FileName, enum FileOpenFlags OpenFlags)
{
	file_t file = fileOpen(FileName, OpenFlags | eFO_CreateIfNotExist);

	if(file < 0)
	{
		//APP_DEBUG("\n--- FileAccess: Open File \"%s\" ERROR. File Description = %d\r\n", FileName.c_str(), file);
		fileClose(file);
		return eFileFailture;
	}
	//APP_DEBUG("\n--- FileAccess: Open File \"%s\" Successfully. File Description = %d\r\n", FileName.c_str(), file);

	return file;
}



/*****************************************************************************
 * Function	: size_t xFileAccess_GetSizeByName(String FileName)
 * Desc		: used to get size of file
 * Param	:
 * 			+ FileName: Name of file
 * return	:
 * 			+ Size file	: Successfully
 * 			+ eFileFailture	: Failture
 ****************************************************************************/
size_t xFileAccess_GetSizeByName(String FileName)
{
	file_t file;
	if((file = xFileAccess_Open(FileName, eFO_ReadOnly)) == eFileFailture)
		return eFileFailture;
	fileSeek(file, 0, eSO_FileEnd);

	return fileTell(file);
}




/*****************************************************************************
 * Function	: size_t xFileAccess_GetSizeByID(file_t file)
 * Desc		: used to get size of file
 * Param	:
 * 			+ file: ID of file
 * return	: size of file
 ****************************************************************************/
size_t xFileAccess_GetSizeByID(file_t file)
{
	fileSeek(file, 0, eSO_FileEnd);
	return fileTell(file);
}



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
Enum_FILEState eFileAccess_Read_All(String FileName, char *pDataOut)
{
	file_t file;
	if((file = xFileAccess_Open(FileName, eFO_ReadOnly)) == eFileFailture)
		return eFileFailture;

	// Get size
	size_t size = xFileAccess_GetSizeByID(file);
	if(size <= 0)
	{
		fileClose(file);
		//APP_DEBUG("--- FileAccess: File \"%s\" Empty. size = %d\r\n", FileName.c_str(), size);
		return eFileFailture;
	}
	else
		//APP_DEBUG("--- FileAccess: Read all file \"%s\" Successfully. Size = %d byte\r\n", FileName.c_str(), size);

	fileSeek(file, 0, eSO_FileStart);
	//memset(pDataOut, 0, strlen(pDataOut));
	fileRead(file, pDataOut, size);
	fileClose(file);

	return eFileSuccess;
}




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
Enum_FILEState eFileAccess_Write_All(String FileName, char *pDataIn)
{
	//APP_DEBUG("--- FileAccess: Write all file. Size = %d byte\r\n", strlen(pDataIn));
	fileDelete(FileName);

	file_t file = xFileAccess_Open(FileName, eFO_WriteOnly);
	fileSeek(file, 0, eSO_FileStart);
	size_t size = fileWrite(file, pDataIn, strlen(pDataIn));
	fileClose(file);

	if(size == strlen(pDataIn))
		return eFileSuccess;
	else
		return eFileFailture;
}



/*****************************************************************************
 * Function	: cFileAccess_ReadChar(char *FileName, unsigned long PointerPos)
 * Desc		: use to Read character of file
 * Param	:
 * 			+ FileName: Name of file
 * 			+ PointerPos: Pointer position in file
 * return	:
 * 			+ Successfully: character output
 ****************************************************************************/
char cFileAccess_ReadChar(file_t file)
{
	char c;

	//fileSeek(file, 0, eSO_CurrentPos);
	fileRead(file, (char *)&c, 1);
	return c;
}



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
Enum_FILEState eFileAccess_Read_Line(file_t FileID, char *pDataOut, int PointerPos)
{
	char c;
	unsigned int uiLenght = 0;

	size_t size = xFileAccess_GetSizeByID(FileID);
	fileSeek(FileID, PointerPos, eSO_FileStart);

	for(uiLenght = 0; uiLenght <= size; uiLenght++)
	{
		c = cFileAccess_ReadChar(FileID);
		if(c == '\r')
			continue;
		if((c != '\n') && (c != '\0'))
			pDataOut[uiLenght] = c;
		else
			break;
	}

	//APP_DEBUG("--- FileAccess: Read line file Successfully. Size = %d byte\r\n", size);
	PointerPos = fileTell(FileID);

	if(uiLenght > 0)
		return eFileSuccess;
	else
		return eFileFailture;
}





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
Enum_FILEState eFileAccess_Write_AppendFile(String FileName, char *pDataIn)
{
	size_t uiSize;

	file_t file = xFileAccess_Open(FileName, eFO_WriteOnly);
	if(file >= 0)
	{
		fileSeek(file, 0, eSO_FileEnd);
		uiSize = fileWrite(file, pDataIn, strlen(pDataIn));
	}
	else
		return eFileFailture;
	fileClose(file);
	if(uiSize !=  strlen(pDataIn))
	{
		//APP_DEBUG("--- FileAccess: Write expand file Fail. Writen %d byte of %d byte\n", uiSize,  strlen(pDataIn));
		return eFileFailture;
	}
	//APP_DEBUG("--- FileAccess: Write expand file \"%s\" OK. Lenght = %d\n", FileName, uiSize);
	return eFileSuccess;
}




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
Enum_FILEState eFileAccess_Write_ReplaceLineFile(String FileName, char *pDataIn, int PosLine)
{
	char filesTranfer[24] = "FileTransfer.txt";
	int Pos = 0, isOK = 0;
	int size;
	char c;

	if(fileExist(filesTranfer) == true)
	{
		fileDelete(filesTranfer);
		//APP_DEBUG("--- FileAccess: File \"%s\" existed. Delete file!\r\n", filesTranfer);
	}

	fileRename(FileName, filesTranfer);
	file_t fileTrans = xFileAccess_Open(filesTranfer, eFO_ReadOnly);
	file_t fileOrigin = xFileAccess_Open(FileName, eFO_WriteOnly);

	if((size = xFileAccess_GetSizeByID(fileTrans)) <= 0)
	{
		fileClose(fileOrigin);
		fileClose(fileTrans);
		fileDelete(filesTranfer);
		return eFileFailture;
	}

	fileSeek(fileTrans, 0, eSO_FileStart);
	for(;size >= 0;)
	{
		c = cFileAccess_ReadChar(fileTrans);
		fileSeek(fileOrigin, 0, eSO_FileEnd);
		if(c == '\n') Pos++;
		if(Pos != PosLine) fileWrite(fileOrigin, (char *)&c, 1);
		else if(!isOK)
		{
			if(PosLine != 0) fileWrite(fileOrigin, "\n", 1);
			fileWrite(fileOrigin, pDataIn, strlen(pDataIn));
			fileWrite(fileOrigin, "\n", 1);
			isOK = 1;
		}
		size--;
	}

	//APP_DEBUG("--- FileAccess: Write replace line in file \"%s\" Successfully.\r\n", FileName);
	fileClose(fileOrigin);
	fileClose(fileTrans);
	fileDelete(filesTranfer);
	return eFileSuccess;
}




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
int iFileAccess_Find_LinebyString(String FileName, char *pDataFind)
{
	char c;
	unsigned int Pos = -1, uii = 0;
	file_t file;
	int size;
	unsigned int StringLenght = strlen(pDataFind);

	if(((file = xFileAccess_Open(FileName, eFO_ReadOnly)) < 0) || (size = xFileAccess_GetSizeByID(file)) <= 0)
	{
		fileClose(file);
		return -1;
	}

	fileSeek(file, 0, eSO_FileStart);
	for( ; size >= 0; )
	{
		c = cFileAccess_ReadChar(file);
		if(pDataFind[uii] == c)
			uii++;
		else if(c == '\n')
		{
			Pos++;
			uii = 0;
		}
		else if(uii == StringLenght)
		{
			fileClose(file);
			return (Pos + 1);
		}
		else uii = 0;

		size--;
	}

	fileClose(file);
	return -1;
}



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
void vFileAccess_DeleteLine(String FileName, int PosLine)
{
	char c;
	unsigned int Pos = 0;
	char filesTranfer[32] = "FileTransfer.txt";

	if(fileExist(filesTranfer) == true)
	{
		fileDelete(filesTranfer);
		//APP_DEBUG("--- FileAccess: File \"%s\" existed. Delete file\r\n", filesTranfer);
	}

	fileRename(FileName, filesTranfer);

	file_t fileTrans = xFileAccess_Open(filesTranfer, eFO_ReadOnly);
	file_t fileOrigin = xFileAccess_Open(FileName, eFO_WriteOnly);

	int size = xFileAccess_GetSizeByID(fileTrans);
	//Read data from Transfer file and write to Origin file
	fileSeek(fileTrans, 0, eSO_FileStart);
	for( ; size >= 0; )
	{
		c = cFileAccess_ReadChar(fileTrans);
		if(c == '\n') Pos++;
		if(Pos != PosLine)
		{
			fileSeek(fileOrigin, 0, eSO_FileEnd);
			fileWrite(fileOrigin, (char *)&c, 1);
		}
		size--;
	}

	fileClose(fileOrigin);
	fileClose(fileTrans);
	fileDelete(filesTranfer);
}
















