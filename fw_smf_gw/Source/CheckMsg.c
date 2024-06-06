/*
 * CheckMsg.c
 *
 *  Created on: Jul 15, 2018
 *      Author: ThaoHuyen
 */
#include <stdio.h>
#include <stdbool.h>

#include "CheckMsg.h"
#include "UserFile.h"
#include "main.h"

extern uint8_t uIDGw[17];

/******************************************************************************
 * Function		: void vCheckMsg_TypePacket(PacketIO *xPacketIO)
 * Description	: check type packet
 * Param		: xPacketIO - struct data input
 * Return		: None
 ******************************************************************************/
void vCheckMsg_TypePacket(PacketIO *xPacketIO) {
	xPacketIO->bPacketIsOK = true;

	if ((strcmp((char*) xPacketIO->TypePacket, (char*) macroTYPE_PACKET_DATA)
			== 0)
			|| (strcmp((char*) xPacketIO->TypePacket,
					(char*) macroTYPE_PACKET_CONF) == 0)
			|| (strcmp((char*) xPacketIO->TypePacket,
					(char*) macroTYPE_PACKET_REQS) == 0)
			|| (strcmp((char*) xPacketIO->TypePacket,
					(char*) macroTYPE_PACKET_CTRL) == 0)
			|| (strcmp((char*) xPacketIO->TypePacket,
					(char*) macroTYPE_PACKET_RESP) == 0)
			|| (strcmp((char*) xPacketIO->TypePacket,
					(char*) macroTYPE_PACKET_WAMI) == 0)) {
		//APP_DEBUG("--- ProcessMsg: Type Packet is OK\r\n");
		xPacketIO->bPacketIsOK = true;
	} else {
		APP_DEBUG("--- CheckMsg: Type Packet is Not OK\r\n");
		xPacketIO->bPacketIsOK = false;
	}
}

/******************************************************************************
 * Function: void vProcessMsg_Check_TypeDevice(PacketIO *xPacketIO)
 * Description: check type device
 * Param:
 * 			+ xPacketIO - struct data input
 * Return: None
 ******************************************************************************/
void vCheckMsg_TypeDevice(PacketIO *xPacketIO) {
	if (xPacketIO->bPacketIsOK == false)
		return;

	if ((strcmp((char*) xPacketIO->TypeDevice, (char*) macroAPP_TYPE_DEVICE_GW)
			== 0)
			|| (strcmp((char*) xPacketIO->TypeDevice,
					(char*) macroAPP_TYPE_DEVICE_SENSOR_NODE) == 0)
			|| (strcmp((char*) xPacketIO->TypeDevice,
					(char*) macroAPP_TYPE_DEVICE_CONTROL_NODE) == 0)
			|| (strcmp((char*) xPacketIO->TypePacket,
					(char*) macroTYPE_PACKET_WAMI) == 0)) {
		//APP_DEBUG("--- ProcessMsg: Type Device is OK\r\n");
		xPacketIO->bPacketIsOK = true;
	} else {
		APP_DEBUG("--- ProcessMsg: Type Device is not OK\r\n");
		xPacketIO->bPacketIsOK = false;
	}
}

/******************************************************************************
 * Function: void vCheckMsg_IDEndDevice(PacketIO *xPacketIO)
 * Description: check id end device
 * Param:
 * 			+ xPacketIO - struct data input
 * Return: None
 ******************************************************************************/
void vCheckMsg_IDEndDevice(PacketIO *xPacketIO) {
	if (xPacketIO->bPacketIsOK == false)
		return;

#ifdef macroUSE_SDCARD
	if (xPacketIO->IDEd[0] > 0) {
		char ED_File[24] = { 0 };
		char IDZb[24] = { 0 };
		sprintf(ED_File, "%s.txt", (char*) xPacketIO->IDEd);
		if (bUserFile_Read_Connectivity_Long_Addr(ED_File,
				(uint8_t*) IDZb) == false) {
			if (xPacketIO->IDZb[0] > 0)
				vUserFile_Write_Connectivity_Long_Addr(ED_File,
						xPacketIO->IDZb);
		} else {
			strcpy((char*) xPacketIO->IDZb, IDZb);
		}
		APP_DEBUG("--- CheckMsg: ID Zigbee = %s\r\n", (char*) xPacketIO->IDZb);
	}
#endif

//	if( (bFileData_CheckEndDevice(xPacketIO->IDED, xPacketIO->TypeDevice) == true) || 	\
//		(strcmp((char *)xPacketIO->IDED, (char *)xPacketIO->IDGW) == 0) ||				\
//		(strcmp((char *)xPacketIO->TypePacket, (char *)macroTYPE_PACKET_PAIR) == 0)		\
//		)
//	{
//		//APP_DEBUG("--- ProcessMsg: ID end device existed. OK\r\n");
//		xPacketIO->bPacketIsOK = true;
//	}
//	else
//	{
//		APP_DEBUG("--- ProcessMsg: ID end device not existed. NOK\r\n");
//		xPacketIO->bPacketIsOK = false;
//	}
}

/******************************************************************************
 * Function: void vCheckMsg_IDGW(PacketIO *xPacketIO)
 * Description: check id gateway
 * Param:
 * 			+ xPacketIO - struct data input
 * Return: None
 ******************************************************************************/
void vCheckMsg_IDGW(PacketIO *xPacketIO) {
	if (xPacketIO->bPacketIsOK == false)
		return;

	if (strcmp((char*) xPacketIO->IDGw, (char*) uIDGw) == 0) {
		//APP_DEBUG("--- ProcessMsg: ID Gw is OK\r\n");
		xPacketIO->bPacketIsOK = true;
	} else {
		APP_DEBUG("--- ProcessMsg: ID Gw is Not OK\r\n");
		xPacketIO->bPacketIsOK = false;
	}
}

/******************************************************************************
 * Function: void vCheckMsg_JsonMessage(PacketIO *xPacketIO)
 * Description: check json message
 * Param:
 * 			+ xPacketIO - struct data input
 * Return: None
 ******************************************************************************/
void vCheckMsg_JsonMessage(PacketIO *xPacketIO) {
	//check type packet
	vCheckMsg_TypePacket(xPacketIO);
	//check type device
	vCheckMsg_TypeDevice(xPacketIO);
	//check ID End Device
	vCheckMsg_IDEndDevice(xPacketIO);
	//check ID Gw
	vCheckMsg_IDGW(xPacketIO);
}

/******************************************************************************
 * Function: void vCheckMsg_StringMessage(PacketIO *xPacketIO)
 * Description: check string message
 * Param:
 * 			+ xPacketIO - struct data input
 * Return: None
 ******************************************************************************/
void vCheckMsg_StringMessage(PacketIO *xPacketIO) {
	//check type packet
	vCheckMsg_TypePacket(xPacketIO);
	//check type device
	vCheckMsg_TypeDevice(xPacketIO);
	//check ID End Device
	vCheckMsg_IDEndDevice(xPacketIO);
	//check ID Gw
	//vCheckMsg_IDGW( xPacketIO );
}

