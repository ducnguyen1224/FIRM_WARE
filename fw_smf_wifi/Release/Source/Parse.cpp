/*
 * Parse.cpp
 *
 *  Created on: Jul 3, 2018
 *      Author: ThaoHuyen
 */

#include <stdlib.h>
#include <string.h>

#include "Parse.h"
#include "main.h"


extern String IDGw;




/******************************************************************************
 * Function		: void vParse_StringMessage( String pDataIn, String *Array )
 * Description	: split string message
 * Param		: pDataIn - a message received
 * Return		: Array data
******************************************************************************/
static void vParse_StringMessage( String pDataIn, String *Array )
{
	unsigned char uCnt = 0;
	unsigned char ui = 0;

	while( (pDataIn[uCnt] > 0) && (pDataIn[uCnt] != '!') )
	{
		if( (pDataIn[uCnt] == ',') || (pDataIn[uCnt] == ':') )
		{
			if(pDataIn[uCnt] == ':')
			{
				if(Array[ui - 1] != macroID_DATA_TIME_SEND)
					ui++;
				else
					Array[ui] += pDataIn[uCnt];
			}
			else
				ui++;
		}
		else
			Array[ui] += pDataIn[uCnt];

		if( (ui >= PACKET_STRING_MAX_FIELD) || (Array[ui].length() >= PACKET_STRING_MAX_FIELDLENGHT) )
		{
			//APP_DEBUG("--- Parse: string max field or field lenght too long\r\n");
			//assert(false);
			memset(Array, 0, sizeof(Array));
			break;
		}

		uCnt++;
	}
}




/******************************************************************************
 * Function		: void vParse_JsonMessage( char *pDataIn, ParseArray *xParseArrayOut )
 * Description	: split json message
 * Param		: pDataIn - a message received
 * Return		: Array data
******************************************************************************/
static void vParse_JsonMessage( String pDataIn, String *Array )
{
	unsigned char uCnt = 0;
	unsigned char ui = 0;
	bool bMask = false;

	while( (pDataIn[uCnt] > 0) && (pDataIn[uCnt] != '}') )
	{
		if( (pDataIn[uCnt] == '{') || (pDataIn[uCnt] == '}') || (pDataIn[uCnt] == ':') || (pDataIn[uCnt] == ',') )
		{
			goto Next;
		}
		else if(pDataIn[uCnt] == '"')
		{
			if(bMask == false)
			{
				bMask = true;
			}
			else
			{
				ui++;
				bMask = false;
			}
		}
		else if(pDataIn[uCnt] == ' ')
			goto Next;
		else
		{
			Array[ui] += pDataIn[uCnt];
		}

		if( (Array[ui].length() >= PACKET_JSON_MAX_FIELDLENGHT) || (ui >= PACKET_JSON_MAX_FIELD) )
		{
			//APP_DEBUG("--- Parse: json max field or field lenght too long\r\n");
			memset(Array, 0, sizeof(Array));
			break;
		}

		Next:;
		uCnt++;
	}
}




/******************************************************************************
 * Function		: void vParse_MessageIsString(String pDataIn, PacketIO *xPacketIO)
 * Description	: convert message to struct data
 * Param		: pDataIn - a message received
 * Return		: struct data
******************************************************************************/
void vParse_MessageIsString(String pDataIn, PacketIO *xPacketIO)
{
	unsigned char uCounter = 0;
	String Array[PACKET_STRING_MAX_FIELD];

	vParse_StringMessage(pDataIn, Array);

	if(Array[0].length() == 0)
		return;

	xPacketIO->PacketIsString = true;
	xPacketIO->TypePacket = Array[uCounter++];

	if(xPacketIO->TypePacket ==  macroTYPE_PACKET_WAMI)
		goto Label;

	xPacketIO->TypeDevice = Array[uCounter++];
	if(xPacketIO->TypePacket == macroTYPE_PACKET_RESP)
		goto Label;
	else if(xPacketIO->TypePacket == macroTYPE_PACKET_DATA)
		xPacketIO->IDEd = Array[uCounter++];

	Label:;
	xPacketIO->IDGw = IDGw;

	uint8 uCnt = 0, ui = 0;
	for(ui = uCounter; Array[ui].length() > 0; ui+=2)
	{
		xPacketIO->Data[uCnt].Name = Array[ui];
		if(Array[ui + 1].length() > 0)
			xPacketIO->Data[uCnt].Info = Array[ui + 1];
		uCnt++;
	}

//	unsigned char uk = 0;
//	APP_DEBUG("--- ParseMsg: Type Packet = %s\r\n", xPacketIO->TypePacket.c_str());
//	APP_DEBUG("--- ParseMsg: Type Device = %s\r\n", xPacketIO->TypeDevice.c_str());
//	APP_DEBUG("--- ParseMsg: ID ED = %s\r\n", xPacketIO->IDEd.c_str());
//	APP_DEBUG("--- ParseMsg: ID Gw = %s\r\n", xPacketIO->IDGw.c_str());
//	for(uk = 0; xPacketIO->Data[uk].Name.length() > 0; uk++)
//	{
//		APP_DEBUG("--- ParseMsg: Data Name[%d] = %s\r\n", uk, xPacketIO->Data[uk].Name.c_str());
//		if(xPacketIO->Data[uk].Info.length() > 0)
//			APP_DEBUG("--- ParseMsg: Data Info[%d] = %s\r\n", uk, xPacketIO->Data[uk].Info.c_str());
//	}
}




/******************************************************************************
 * Function		: void vParse_MessageIsJson(String pDataIn, PacketIO *xPacketIO)
 * Description	: convert message to struct data
 * Param		: pDataIn - a message received
 * Return		: struct data
******************************************************************************/
void vParse_MessageIsJson(String pDataIn, PacketIO *xPacketIO)
{
	unsigned char uCounter = 0;
	String Array[PACKET_JSON_MAX_FIELD];

	vParse_JsonMessage(pDataIn, Array);

	if(Array[0].length() == 0)
		return;

	xPacketIO->PacketIsString = false;

//	if( (Array[0] != JSON_TYPE_PACKET) || (Array[2] != JSON_TYPE_DEVICE) || (Array[4] != JSON_DEVICE_IDEP) || \
//		(Array[6] != JSON_DEVICE_IDGW) || (Array[8] != JSON_DATA) \
//		)
//	{
//		//APP_DEBUG("--- Parse: Json static string not found\r\n");
//		return;
//	}

	xPacketIO->TypePacket = Array[uCounter + 1];
	xPacketIO->TypeDevice =	Array[uCounter + 3];
	xPacketIO->IDEd = Array[uCounter + 5];
	xPacketIO->IDGw = Array[uCounter + 7];

	unsigned char uCnt = 0, ui = 0;
	for(ui = uCounter + 9; Array[ui].length() > 0; ui+=2)
	{
		xPacketIO->Data[uCnt].Name = Array[ui];
		if(Array[ui + 1].length() > 0)
			xPacketIO->Data[uCnt].Info = Array[ui + 1];

		uCnt++;
	}

//	APP_DEBUG("--- ParseMsg: Type Packet = %s\r\n", xPacketIO->TypePacket.c_str());
//	APP_DEBUG("--- ParseMsg: Type Device = %s\r\n", xPacketIO->TypeDevice.c_str());
//	APP_DEBUG("--- ParseMsg: ID ED = %s\r\n", xPacketIO->IDEd.c_str());
//	APP_DEBUG("--- ParseMsg: ID GW = %s\r\n", xPacketIO->IDGw.c_str());
//	for(unsigned char uk = 0; xPacketIO->Data[uk].Name.length() > 0; uk++)
//	{
//		APP_DEBUG("--- ParseMsg: Data Name[%d] = %s\r\n", uk, xPacketIO->Data[uk].Name.c_str());
//		if(xPacketIO->Data[uk].Info.length() > 0)
//			APP_DEBUG("--- ParseMsg: Data Info[%d] = %s\r\n", uk, xPacketIO->Data[uk].Info.c_str());
//	}
}




/******************************************************************************
 * Function		: void vParse_Packing_JsonMessage(PacketIO *xPacketIO, String *pDataOut)
 * Description	: packing json message
 * Param		:
 * 				+ xPacketIO - struct data
 * 				+ pDataOut - buffer output
 * Return		: None
******************************************************************************/
void vParse_Packing_JsonMessage(PacketIO *xPacketIO, String *pDataOut)
{
	uint8 ui = 0;

	//type packet
	*pDataOut = "{\"TypePacket\":\"" + xPacketIO->TypePacket + "\"," + \
				"\"TypeDevice\":\"" + xPacketIO->TypeDevice + "\"," + \
				"\"DeviceidEp\":\"" + xPacketIO->IDEd + "\"," + \
				"\"DeviceidGw\":\"" + xPacketIO->IDGw + "\"," + \
				"\"Data\":{";

	for( ui = 0; xPacketIO->Data[ui].Name.length() > 0; ui++)
	{
		if(ui > 0)
			*pDataOut += ',';
		*pDataOut += "\"" + xPacketIO->Data[ui].Name + "\"";
//		if(xPacketIO->Data[ui].Info.length() > 0)
			*pDataOut += ":\"" + xPacketIO->Data[ui].Info + "\"";
	}

	*pDataOut += "}}";
}




/******************************************************************************
 * Function		: void vParse_Packing_StringMessage(PacketIO *xPacketIO, String *pDataOut)
 * Description	: packing string message
 * Param		:
 * 				+ xPacketIO - struct data
 * 				+ pDataOut - buffer output
 * Return		: None
******************************************************************************/
void vParse_Packing_StringMessage(PacketIO *xPacketIO, String *pDataOut)
{
	unsigned char ui = 0;

	*pDataOut = 0;

	//type packet
	*pDataOut = xPacketIO->TypePacket + "," + xPacketIO->TypeDevice;

	if(xPacketIO->IDEd.length() > 0)
		*pDataOut += "," + xPacketIO->IDEd;

	for(ui = 0; xPacketIO->Data[ui].Name.length() > 0; ui++)
	{
		*pDataOut += "," + xPacketIO->Data[ui].Name;
		if(xPacketIO->Data[ui].Info.length() > 0)
			*pDataOut += ":" + xPacketIO->Data[ui].Info;
	}
	*pDataOut += '!';
}



















