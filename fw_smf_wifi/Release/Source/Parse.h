/*
 * Parse.h
 *
 *  Created on: Jul 3, 2018
 *      Author: ThaoHuyen
 */

#ifndef SOURCE_PARSE_H_
#define SOURCE_PARSE_H_

#include "AppData.h"




/******************************************************************************
 * Function		: void vParse_MessageIsString(String pDataIn, PacketIO *xPacketIO)
 * Description	: convert message to struct data
 * Param		: pDataIn - a message received
 * Return		: struct data
******************************************************************************/
void vParse_MessageIsString(String pDataIn, PacketIO *xPacketIO);




/******************************************************************************
 * Function		: void vParse_MessageIsJson(String pDataIn, PacketIO *xPacketIO)
 * Description	: convert message to struct data
 * Param		: pDataIn - a message received
 * Return		: struct data
******************************************************************************/
void vParse_MessageIsJson(String pDataIn, PacketIO *xPacketIO);




/******************************************************************************
 * Function		: void vParse_Packing_JsonMessage(PacketIO *xPacketIO, String *pDataOut)
 * Description	: packing json message
 * Param		:
 * 				+ xPacketIO - struct data
 * 				+ pDataOut - buffer output
 * Return		: None
******************************************************************************/
void vParse_Packing_JsonMessage(PacketIO *xPacketIO, String *pDataOut);




/******************************************************************************
 * Function		: void vParse_Packing_StringMessage(PacketIO *xPacketIO, String *pDataOut)
 * Description	: packing string message
 * Param		:
 * 				+ xPacketIO - struct data
 * 				+ pDataOut - buffer output
 * Return		: None
******************************************************************************/
void vParse_Packing_StringMessage(PacketIO *xPacketIO, String *pDataOut);



#endif /* SOURCE_PARSE_H_ */












