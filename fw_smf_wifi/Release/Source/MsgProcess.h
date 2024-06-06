/*
 * MsgProcess.h
 *
 *  Created on: Jul 5, 2018
 *      Author: ThaoHuyen
 */

#ifndef SOURCE_MSGPROCESS_H_
#define SOURCE_MSGPROCESS_H_



/******************************************************************************
 * Function		: void vMsgProcess_All(PacketIO *xPacketIO)
 * Description	: process all message
 * Param		:
 * 				+ xPacketIO - struct data
 * Return		: None
******************************************************************************/
void vMsgProcess_All(PacketIO *xPacketIO);




/******************************************************************************
 * Function		: void vMsgProcess_Request_Out( String Request, bool toGw )
 * Description	: request out
 * Param		: None
 * Return		: None
******************************************************************************/
void vMsgProcess_Request_Out( String Request, bool toGw );




/******************************************************************************
 * Function		: void vMsgProcess_Request_Out( String Request)
 * Description	: response out
 * Param		: None
 * Return		: None
******************************************************************************/
void vMsgProcess_Response_Out( String Response );


#endif /* SOURCE_MSGPROCESS_H_ */














