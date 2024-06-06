/*
 * Parse.h
 *
 *  Created on: Jul 14, 2018
 *      Author: ThaoHuyen
 */

#ifndef SOURCE_PARSE_H_
#define SOURCE_PARSE_H_

#include "AppData.h"



/******************************************************************************
 * Function: PacketIO xParseMsg_MessageIsJson(uint8_t *pDataIn)
 * Description: convert message to struct data
 * Param: pDataIn - a message received
 * Return: struct data
******************************************************************************/
PacketIO xParseMsg_MessageIsJson(uint8_t *pDataIn);





/******************************************************************************
 * Function: PacketIO xParseMsg_MessageIsString(uint8_t *pDataIn, bool fromConnectivity)
 * Description: convert message to struct data
 * Param: pDataIn - a message received
 * Return: struct data
******************************************************************************/
PacketIO xParseMsg_MessageIsString(uint8_t *pDataIn, bool fromConnectivity);




/******************************************************************************
 * Function: void vParseMsg_Packing_JsonMessage(PacketIO *xPacketIO, uint8_t *pDataOut)
 * Description: packing json message
 * Param:
 * 			+ xPacketIO - struct data
 * 			+ pDataOut - buffer output
 * Return: None
******************************************************************************/
void vParseMsg_Packing_JsonMessage(PacketIO *xPacketIO, uint8_t *pDataOut);





/******************************************************************************
 * Function: void vParseMsg_Packing_StringMessage(PacketIO *xPacketIO, uint8_t *pDataOut, bool toConnectivity)
 * Description: packing string message
 * Param:
 * 			+ xPacketIO - struct data
 * 			+ pDataOut - buffer output
 * Return: None
******************************************************************************/
void vParseMsg_Packing_StringMessage(PacketIO *xPacketIO, uint8_t *pDataOut, bool toConnectivity);



#endif /* SOURCE_PARSE_H_ */
