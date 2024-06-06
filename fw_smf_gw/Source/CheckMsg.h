/*
 * CheckMsg.h
 *
 *  Created on: Jul 15, 2018
 *      Author: ThaoHuyen
 */

#ifndef SOURCE_CHECKMSG_H_
#define SOURCE_CHECKMSG_H_

#include "AppData.h"



/******************************************************************************
 * Function: void vCheckMsg_JsonMessage(PacketIO *xPacketIO)
 * Description: check json message
 * Param:
 * 			+ xPacketIO - struct data input
 * Return: None
******************************************************************************/
void vCheckMsg_JsonMessage(PacketIO *xPacketIO);




/******************************************************************************
 * Function: void vCheckMsg_StringMessage(PacketIO *xPacketIO)
 * Description: check string message
 * Param:
 * 			+ xPacketIO - struct data input
 * Return: None
******************************************************************************/
void vCheckMsg_StringMessage(PacketIO *xPacketIO);


#endif /* SOURCE_CHECKMSG_H_ */
