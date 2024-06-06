/*
 * MsgCheck.cpp
 *
 *  Created on: Jul 3, 2018
 *      Author: ThaoHuyen
 */

#include "MsgCheck.h"
#include "Parse.h"
// Hàm này được sử dụng để xác định loại gói tin và xác nhận rằng gói tin là hợp lệ

extern String IDGw;




/******************************************************************************
 * Function		: void vMsgCheck_TypePacket(PacketIO *xPacketIO)
 * Description	: check type packet
 * Param		: None
 * Return		: None
******************************************************************************/
static void vMsgCheck_TypePacket(PacketIO *xPacketIO)
{
	xPacketIO->isTypePacket = eNone;

	if(xPacketIO->TypePacket == macroTYPE_PACKET_DATA)
		xPacketIO->isTypePacket = eData;
	else if(xPacketIO->TypePacket == macroTYPE_PACKET_CONF)
		xPacketIO->isTypePacket = eConf;
	else if(xPacketIO->TypePacket == macroTYPE_PACKET_REQS)
		xPacketIO->isTypePacket = eReqs;
	else if(xPacketIO->TypePacket == macroTYPE_PACKET_CTRL)
		xPacketIO->isTypePacket = eCtrl;
	else if(xPacketIO->TypePacket == macroTYPE_PACKET_RESP)
		xPacketIO->isTypePacket = eResp;
	else if(xPacketIO->TypePacket == macroTYPE_PACKET_WAMI)
		xPacketIO->isTypePacket = eWami;

	if(xPacketIO->isTypePacket == eNone)
	{
		//APP_DEBUG("--- MsgCheck: Type packet is not found\r\n");
		xPacketIO->PacketIsOK = false;
	}
	else
		xPacketIO->PacketIsOK = true;
}




/******************************************************************************
 * Function		: void vMsgCheck_TypeDevice(PacketIO *xPacketIO)
 * Description	: check type device
 * Param		: None
 * Return		: None
******************************************************************************/
static void vMsgCheck_TypeDevice(PacketIO *xPacketIO)
{
	xPacketIO->isOf_GwDevice = false;

	if(xPacketIO->PacketIsOK == true)
	{
		if(xPacketIO->isTypePacket != eWami)
		{
			if(xPacketIO->TypeDevice == macroTYPE_DEVICE_GW)
				xPacketIO->isOf_GwDevice = true;
			else if( (xPacketIO->TypeDevice != macroTYPE_DEVICE_SENSOR_NODE) && (xPacketIO->TypeDevice != macroTYPE_DEVICE_CONTROL_NODE) )
			{
				xPacketIO->PacketIsOK = false;
				//APP_DEBUG("--- MsgCheck: type device is not found\r\n");
			}
		}
	}
}




/******************************************************************************
 * Function		: void vMsgCheck_IDGw(PacketIO *xPacketIO)
 * Description	: check id gw
 * Param		: None
 * Return		: None
******************************************************************************/
static void vMsgCheck_IDGw(PacketIO *xPacketIO)
{
	if(xPacketIO->PacketIsOK == true)
	{
		if(xPacketIO->IDGw == IDGw)
			xPacketIO->PacketIsOK = true;
		else
		{
			//APP_DEBUG("--- MsgCheck: ID Gw is not found\r\n");
			xPacketIO->PacketIsOK = false;
		}
	}
}




/******************************************************************************
 * Function		: void vMsgCheck_JsonMessage( String pDataIn, PacketIO *xPacketIO )
 * Description	: check json message
 * Param		: None
 * Return		: None
******************************************************************************/
void vMsgCheck_JsonMessage( String pDataIn, PacketIO *xPacketIO )
{
	vParse_MessageIsJson(pDataIn, xPacketIO);

	vMsgCheck_TypePacket(xPacketIO);
	vMsgCheck_TypeDevice(xPacketIO);
	vMsgCheck_IDGw(xPacketIO);
}




/******************************************************************************
 * Function		: void vMsgCheck_StringMessage( String pDataIn, PacketIO *xPacketIO )
 * Description	: check string message
 * Param		: None
 * Return		: None
******************************************************************************/
void vMsgCheck_StringMessage( String pDataIn, PacketIO *xPacketIO )
{
	vParse_MessageIsString(pDataIn, xPacketIO);

	vMsgCheck_TypePacket(xPacketIO);
	vMsgCheck_TypeDevice(xPacketIO);
	//vMsgCheck_IDGw(xPacketIO);
}
















