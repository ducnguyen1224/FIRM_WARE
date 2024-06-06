/*
 * Parse.c
 *
 *  Created on: Jul 14, 2018
 *      Author: ThaoHuyen
 */
#include <stdio.h>

#include "Parse.h"
#include "main.h"


extern uint8_t uIDGw[17];


typedef struct
{
	uint8_t Array[macroPACKET_JSON_MAX_FIELD][macroPACKET_JSON_MAX_FIELD_LENGHT];
}Array;



/******************************************************************************
 * Function: Array xParseMsg_JsonSplit(uint8_t *pDataIn)
 * Description: split json message
 * Param: pDataIn - a message received
 * Return: Array data
******************************************************************************/
Array xParseMsg_JsonSplit(uint8_t *pDataIn)
{
	Array _Array = {0};
	uint8_t ui = 0, uj = 0;
	bool bMask = false;

	while( (*pDataIn > 0) && (*pDataIn != '}') )
	{
		if( (*pDataIn == '{') || (*pDataIn == '}') || (*pDataIn == ':') || (*pDataIn == ',') )
		{
			goto Next;
		}
		else if(*pDataIn == '"')
		{
			if(bMask == false)
			{
				bMask = true;
			}
			else
			{
				ui++;
				uj = 0;
				bMask = false;
			}
		}
		else
		{
			_Array.Array[ui][uj++] = *pDataIn;
		}

		if( (uj >= macroPACKET_JSON_MAX_FIELD_LENGHT) || (ui >= macroPACKET_JSON_MAX_FIELD) )
		{
			APP_DEBUG("--- ParseMsg: json max field or field lenght too long\r\n");
			memset(&_Array, 0, sizeof(_Array));
			return _Array;
		}

		Next:;
		pDataIn++;
	}

	return _Array;
}





/******************************************************************************
 * Function: Array xParseMsg_StringSplit(uint8_t *pDataIn)
 * Description: split string message
 * Param: pDataIn - a message received
 * Return: Array data
******************************************************************************/
Array xParseMsg_StringSplit(uint8_t *pDataIn)
{
	Array _Array = {0};
	uint8_t ui = 0, uj = 0;

	while( (*pDataIn > 0) && (*pDataIn != '!') )
	{
		if( (*pDataIn == ',') || (*pDataIn == ':') )
		{
			ui++;
			uj = 0;
		}
		else
		{
			_Array.Array[ui][uj++] = *pDataIn;
		}

		if( (ui >= macroPACKET_STRING_MAX_FIELD) || (uj >= macroPACKET_STRING_MAX_FIELD_LENGHT) )
		{
			APP_DEBUG("--- ParseMsg: string max field or field lenght too long\r\n");
			memset(&_Array, 0, sizeof(_Array));
			return _Array;
		}

		pDataIn++;
	}

	return _Array;
}





/******************************************************************************
 * Function: PacketIO xParseMsg_MessageIsJson(uint8_t *pDataIn)
 * Description: convert message to struct data
 * Param: pDataIn - a message received
 * Return: struct data
******************************************************************************/
PacketIO xParseMsg_MessageIsJson(uint8_t *pDataIn)
{
	PacketIO xPacketIO = {0};
	uint8_t uCounter = 0;

	Array _Array = xParseMsg_JsonSplit(pDataIn);

	if(_Array.Array[0][0] == 0)
		return xPacketIO;

	xPacketIO.bPacketIsString = false;

	strcpy((char *)xPacketIO.TypePacket,	(char *)_Array.Array[uCounter + 1]);
	strcpy((char *)xPacketIO.TypeDevice,	(char *)_Array.Array[uCounter + 3]);
	strcpy((char *)xPacketIO.IDEd, 			(char *)_Array.Array[uCounter + 5]);
	strcpy((char *)xPacketIO.IDGw,	 		(char *)_Array.Array[uCounter + 7]);

	uint8_t uCnt = 0, ui = 0;
	for(ui = uCounter + 9; _Array.Array[ui][0] > 0; ui+=2)
	{
		strcpy((char *)xPacketIO.Data[uCnt].Name, (char *)_Array.Array[ui]);
		if(_Array.Array[ui + 1][0] > 0)
			strcpy((char *)xPacketIO.Data[uCnt].Info, (char *)_Array.Array[ui + 1]);

		uCnt++;
	}

//	APP_DEBUG("--- ParseMsg: Type Packet = %s\r\n", xPacketIO.TypePacket);
//	APP_DEBUG("--- ParseMsg: Type Device = %s\r\n", xPacketIO.TypeDevice);
//	APP_DEBUG("--- ParseMsg: ID ED	= %s\r\n", xPacketIO.IDED);
//	APP_DEBUG("--- ParseMsg: ID GW	= %s\r\n", xPacketIO.IDGW);
//	for(uint8_t uk = 0; xPacketIO.Data_IO[uk].Name[0] > 0; uk++)
//	{
//		APP_DEBUG("--- ParseMsg: Data Name[%d] = %s\r\n", uk, (char*)xPacketIO.Data_IO[uk].Name);
//		if(xPacketIO.Data_IO[uk].Info[0] > 0)
//			APP_DEBUG("--- ParseMsg: Data Info[%d] = %s\r\n", uk, (char*)xPacketIO.Data_IO[uk].Info);
//	}

	return xPacketIO;
}





/******************************************************************************
 * Function: PacketIO xParseMsg_MessageIsString(uint8_t *pDataIn, bool fromConnectivity)
 * Description: convert message to struct data
 * Param: pDataIn - a message received
 * Return: struct data
******************************************************************************/
PacketIO xParseMsg_MessageIsString(uint8_t *pDataIn, bool fromConnectivity)
{
	PacketIO xPacketIO = {0};
	uint8_t uCounter = 0;

	Array _Array = xParseMsg_StringSplit(pDataIn);

	if(_Array.Array[0][0] == 0)
		return xPacketIO;

	xPacketIO.bPacketIsString = true;

	strcpy((char *)xPacketIO.TypePacket, (char *)_Array.Array[uCounter++]);

	if( (strcmp((char *)xPacketIO.TypePacket, macroTYPE_PACKET_WAMI) == 0) )
	{
		goto Next;
	}
	strcpy((char *)xPacketIO.TypeDevice, (char *)_Array.Array[uCounter++]);

	if( (strcmp((char *)xPacketIO.TypePacket, macroTYPE_PACKET_CONF) == 0) )
	{
		if( (strcmp((char *)_Array.Array[uCounter], macroID_CONF_RTC) == 0) || (strcmp((char *)_Array.Array[uCounter], macroID_CONF_SMC_WIFI) == 0) )
			goto Next;
	}
	else if( (strcmp((char *)xPacketIO.TypePacket, macroTYPE_PACKET_REQS) == 0) )
	{
		if(strcmp((char *)_Array.Array[uCounter], macroID_REQS_IDGW) == 0)
			goto Next;
	}
	else if(strcmp((char *)xPacketIO.TypePacket, macroTYPE_PACKET_RESP) == 0)
	{
		goto Next;
	}

	strcpy((char *)xPacketIO.IDEd, (char *)_Array.Array[uCounter++]);
	strcpy((char *)xPacketIO.IDGw, (char *)uIDGw);

	if( fromConnectivity == true )
		strcpy((char *)xPacketIO.IDZb, (char *)_Array.Array[uCounter++]);

	Next:;

	uint8_t uCnt = 0, ui = 0;
	for(ui = uCounter; _Array.Array[ui][0] > 0; ui+=2)
	{
		strcpy((char *)xPacketIO.Data[uCnt].Name, (char *)_Array.Array[ui]);
		if(_Array.Array[ui + 1][0] > 0)
			strcpy((char *)xPacketIO.Data[uCnt].Info, (char *)_Array.Array[ui + 1]);

		uCnt++;
	}

//	uint8_t uk = 0;
//	APP_DEBUG("--- ParseMsg: Type Packet = %s\r\n", xPacketIO.TypePacket);
//	APP_DEBUG("--- ParseMsg: ID ED	= %s\r\n", xPacketIO.IDED);
//	APP_DEBUG("--- ParseMsg: Type Device = %s\r\n", xPacketIO.TypeDevice);
//	for(uk = 0; xPacketIO.Data_IO[uk].Name[0] > 0; uk++)
//	{
//		APP_DEBUG("--- ParseMsg: Data Name[%d] = %s\r\n", uk, (char*)xPacketIO.Data_IO[uk].Name);
//		if(xPacketIO.Data_IO[uk].Info[0] > 0)
//			APP_DEBUG("--- ParseMsg: Data Info[%d] = %s\r\n", uk, (char*)xPacketIO.Data_IO[uk].Info);
//	}

	return xPacketIO;
}




/******************************************************************************
 * Function: void vParseMsg_Packing_JsonMessage(PacketIO *xPacketIO, uint8_t *pDataOut)
 * Description: packing json message
 * Param:
 * 			+ xPacketIO - struct data
 * 			+ pDataOut - buffer output
 * Return: None
******************************************************************************/
void vParseMsg_Packing_JsonMessage(PacketIO *xPacketIO, uint8_t *pDataOut)
{
	uint8_t ui = 0;

	memset(pDataOut, 0, strlen((char *)pDataOut));

	sprintf((char *)pDataOut, "{\"%s\":\"%s\",\"%s\":\"%s\",\"%s\":\"%s\",\"%s\":\"%s\",\"%s\":{", \
			//type packet
			(char *)macroJSON_TYPE_PACKET, (char *)xPacketIO->TypePacket, \
			//type device
			(char *)macroJSON_TYPE_DEVICE, (char *)xPacketIO->TypeDevice, \
			//ID End device
			(char *)macroJSON_DEVICE_IDEP, (char *)xPacketIO->IDEd, \
			//ID Gw
			(char *)macroJSON_DEVICE_IDGW, (char *)xPacketIO->IDGw,
			//data
			(char *)macroJSON_DATA \
			);

	for( ui = 0; xPacketIO->Data[ui].Name[0] > 0; ui++)
	{
		if(ui > 0)
			pDataOut[strlen((char *)pDataOut) + 1] = ',';
		sprintf((char *)pDataOut, "%s\"%s\"", (char *)pDataOut, (char *)xPacketIO->Data[ui].Name);
		//if(xPacketIO->Data[ui].Info[0] > 0)
			sprintf((char *)pDataOut, "%s:\"%s\"", (char *)pDataOut, (char *)xPacketIO->Data[ui].Info);
	}

	pDataOut[strlen((char *)pDataOut)] = '}';
	pDataOut[strlen((char *)pDataOut)] = '}';
}





/******************************************************************************
 * Function: void vParseMsg_Packing_StringMessage(PacketIO *xPacketIO, uint8_t *pDataOut, bool toConnectivity)
 * Description: packing string message
 * Param:
 * 			+ xPacketIO - struct data
 * 			+ pDataOut - buffer output
 * Return: None
******************************************************************************/
void vParseMsg_Packing_StringMessage(PacketIO *xPacketIO, uint8_t *pDataOut, bool toConnectivity)
{
	uint8_t ui = 0;

	memset(pDataOut, 0, strlen((char *)pDataOut));

	sprintf((char *)pDataOut, "%s,%s", (char *)xPacketIO->TypePacket, (char *)xPacketIO->TypeDevice);
	if(xPacketIO->IDEd[0] > 0)
		sprintf((char *)pDataOut, "%s,%s", (char *)pDataOut, (char *)xPacketIO->IDEd);

	if( (toConnectivity == true) && (xPacketIO->IDZb[0] > 0) )
		sprintf((char *)pDataOut, "%s,%s", (char *)pDataOut, (char *)xPacketIO->IDZb);

	for(ui = 0; xPacketIO->Data[ui].Name[0] > 0; ui++)
	{
		sprintf((char *)pDataOut, "%s,%s", (char *)pDataOut, (char *)xPacketIO->Data[ui].Name);
		if(xPacketIO->Data[ui].Info[0] > 0)
			sprintf((char *)pDataOut, "%s:%s", (char *)pDataOut, (char *)xPacketIO->Data[ui].Info);
	}
	pDataOut[strlen((char *)pDataOut)] = '!';
}






















