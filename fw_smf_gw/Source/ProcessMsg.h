/*
 * ProcessMsg.h
 *
 *  Created on: Jul 16, 2018
 *      Author: ThaoHuyen
 */

#ifndef SOURCE_PROCESSMSG_H_
#define SOURCE_PROCESSMSG_H_

#include "AppData.h"



/******************************************************************************
 * Function		: void vProcessMsg_AllMessage( uint8_t *pDataIn, bool isStringMessage, bool fromConnectivity )
 * Description	: process all message
 * Param		: None
 * Return		: None
******************************************************************************/
void vProcessMsg_AllMessage( uint8_t *pDataIn, bool isStringMessage, bool fromConnectivity );




/******************************************************************************
 * Function		: void vProcessMsg_Send_Data( char *IDEp, char *ID_Data, bool State )
 * Description	: send data
 * Param		: None
 * Return		: None
******************************************************************************/
void vProcessMsg_Send_Data( char *IDEp, char *ID_Data, char *State );




/******************************************************************************
 * Function		: void vProcessMsg_Send_Request( bool toConnectivity )
 * Description	: send request
 * Param		: None
 * Return		: None
******************************************************************************/
void vProcessMsg_Send_Request( char *Request, bool toConnectivity );




/******************************************************************************
 * Function		: void vProcessMsg_Send_Response( char *Response, bool toConnectivity )
 * Description	: send response
 * Param		: None
 * Return		: None
******************************************************************************/
void vProcessMsg_Send_Response( char *Response, bool toConnectivity );




/******************************************************************************
 * Function		: void vProcessMsg_Send_Wami( void )
 * Description	: send who am i
 * Param		: None
 * Return		: None
******************************************************************************/
void vProcessMsg_Send_Wami( void );



#endif /* SOURCE_PROCESSMSG_H_ */
