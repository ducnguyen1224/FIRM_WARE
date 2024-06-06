/*
 * MsgCheck.h
 *
 *  Created on: Jul 3, 2018
 *      Author: ThaoHuyen
 */

#ifndef SOURCE_MSGCHECK_H_
#define SOURCE_MSGCHECK_H_

#include "main.h"


/******************************************************************************
 * Function		: void vMsgCheck_JsonMessage( String pDataIn, PacketIO *xPacketIO )
 * Description	: check json message
 * Param		: None
 * Return		: None
******************************************************************************/
void vMsgCheck_JsonMessage( String pDataIn, PacketIO *xPacketIO );




/******************************************************************************
 * Function		: void vMsgCheck_StringMessage( String pDataIn, PacketIO *xPacketIO )
 * Description	: check string message
 * Param		: None
 * Return		: None
******************************************************************************/
void vMsgCheck_StringMessage( String pDataIn, PacketIO *xPacketIO );


#endif /* SOURCE_MSGCHECK_H_ */
