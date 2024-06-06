
#include "ZigbeeIO.h"


extern uint8 uPacketIDSent;
extern afAddrType_t xAddrNode;
extern uint8 uMain_TaskID;


char ZIGBEE_RX_Buffer[macroZIGBEE_RX_BUFF_SIZE] = {0};
char ZIGBEE_TX_Buffer[macroZIGBEE_TX_BUFF_SIZE] = {0};



/*****************************************************************************************
 * Zigbee Data Send
*****************************************************************************************/
uint8 uZigbeeIO_SendMessage(afAddrType_t xAddrDst, uint16 uiClusterID, uint8 *pData)
{
	afStatus_t Status;

	//xAddr_Dst.addrMode = (afAddrMode_t)Addr16Bit;
	// network addr of ZC is 0
	//xAddr_Dst.addr.shortAddr = 0;
	// endpoint of app on zc
	//xAddr_Dst.endPoint = APPDATA_ENDPOINT;
	
	Status = AF_DataRequest(&xAddrDst, 							//Destrination Address
							&xDataEPDesc, 						//EndPoint
							uiClusterID, 						//Cluster
							(byte)(strlen((char *)pData)) + 1,	//Lenght of Data
							(byte *)pData,						//Data
							&uPacketIDSent,						//TranID increased if Success
							AF_DISCV_ROUTE,						//Default
							AF_DEFAULT_RADIUS					//Default
						);
	if(Status == afStatus_SUCCESS)
	{
		APP_DEBUG("--- ZigbeeIO: Data Sent = %s\r\n", (char *)pData);
		return 1;
	}
	else
	{ 
		APP_DEBUG("--- ZigbeeIO: Send Data FAIL. Error = 0x%x\r\n", Status);
		return 0;
	}
}
//End





/*****************************************************************************************
 * Zigbee Data Receive
*****************************************************************************************/
uint8 uZigbeeIO_RecvMessage(afIncomingMSGPacket_t *xDataStructRecv)
{	
	APP_DEBUG("\r\n\r\n--- ZigbeeIO: Recv = %s\r\n", xDataStructRecv->cmd.Data);	
	APP_DEBUG("--- ZigbeeIO: Addr Node = 0x%x\r\n", xDataStructRecv->srcAddr.addr.shortAddr);
	
	if(xDataStructRecv->clusterId == APPDATA_CLUSTER_COOR_OUT_NODE_IN)
	{
		memset(ZIGBEE_RX_Buffer, 0, strlen(ZIGBEE_RX_Buffer));
		strcpy(ZIGBEE_RX_Buffer, (char *)xDataStructRecv->cmd.Data);
		macroSET_EVENT(uMain_TaskID, EVENT_ZIGBEE_RECEIVE);
	}
	
	return 1;
}
//End




/*********************************************************************
 * @fn      vZigbeeIO_ProcessZDOMsgs()
 *
 * @brief   Process response messages
 *
 * @param   none
 *
 * @return  none
 */
void vZigbeeIO_ProcessZDOMsgs(zdoIncomingMsg_t *inMsg)
{
	ZDO_DeviceAnnce_t dev_annce;
	switch(inMsg->clusterID)
	{
		case Device_annce: 
		{
			ZDO_ParseDeviceAnnce(inMsg, &dev_annce);
			APP_DEBUG("--- ZigbeeIO: New ED joined\r\n");
			APP_DEBUG("--- ZigbeeIO: ED nwk addr: 0x%x\r\n", dev_annce.nwkAddr);
			APP_DEBUG("--- ZigbeeIO: IEEE address = 0x");
			for(uint8 uci = 0; uci < Z_EXTADDR_LEN; uci++) 
				APP_DEBUG("%x", dev_annce.extAddr[uci]);
			APP_DEBUG("\r\n");
			break;
		}
		
		case NWK_addr_rsp: 
		{
			ZDO_NwkIEEEAddrResp_t *pRspNew = ZDO_ParseAddrRsp(inMsg);
 
			if(pRspNew)
			{
				if ( pRspNew->status == ZSuccess)
				{
//					APP_DEBUG("--- ZigbeeIO: NWK_addr_respone. Node address = 0x%x\r\n", pRspNew->nwkAddr);
//					xAddrNode.addr.shortAddr = pRspNew->nwkAddr;
//					macroSET_EVENT(uMain_TaskID, EVENT_ZIGBEE_SEND);
				}
			}
			break;
		}
  	}//endswitch
}




/******************************************************************************
 * @fn      vZigbeeIO_GetIEEEAdress
 *
 * @brief   Get the IEEE address (MSB to LSB).
 *
 * @param   none
 *
 * @return  none
*/
void vZigbeeIO_GetIEEEAdress(uint8 *ucAddrIEEE, ZLongAddr_t ucAddrExt)
{
	uint8 uci;
	uint8 *xad;

	//Display the extended address.
	xad = ucAddrExt + Z_EXTADDR_LEN - 1;

	for (uci = 0; uci < Z_EXTADDR_LEN * 2; xad--)
	{
		uint8 ucCh;
		ucCh = (*xad >> 4) & 0x0F;
		ucAddrIEEE[uci++] = ucCh + (( ucCh < 10 ) ? '0' : '7');
		ucCh = *xad & 0x0F;
		ucAddrIEEE[uci++] = ucCh + (( ucCh < 10 ) ? '0' : '7');
	}
	ucAddrIEEE[Z_EXTADDR_LEN*2] = '\0';
}





/*******************************************************************************
* Function		: uint16 uiZigbeeIO_ShortAddr_From_IEEEAddr(uint8 *uAddrIEEE, uint8 *uLongAddr)
* Description	: Get Short address of End Device from IEEE address
* Param			: 
*				+ uAddrIEEE - IEEE address
*				+ uLongAddr - Long address
* Return		: Short Address
*******************************************************************************/
uint16 uiZigbeeIO_ShortAddr_From_IEEEAddr(uint8 *uAddrIEEE, uint8 *uLongAddr)
{
	associated_devices_t *xAss;
	
	uint8 uk = 0;
	for(uint8 uci = 0; uci < Z_EXTADDR_LEN; uci++)
	{
		//Chuyen dia chi IEEE ve 8 Byte dia chi Long address
		uint8 uCnt = 0;
		uCnt = (uAddrIEEE[uk] <= '9')? (uAddrIEEE[uk] ^ 0x30) : (((uAddrIEEE[uk] < 'a')? (uAddrIEEE[uk] - 'A') : (uAddrIEEE[uk] - 'a')) + 10);
		uLongAddr[uci] = uCnt * 16 + ((uAddrIEEE[uk + 1] <= '9')? (uAddrIEEE[uk + 1] ^ 0x30) : (((uAddrIEEE[uk + 1] < 'a')? (uAddrIEEE[uk + 1] - 'A') : (uAddrIEEE[uk + 1] - 'a')) + 10));
		uk += 2;
	}

	xAss = AssocGetWithExt(uLongAddr);		//Lay dia chi cua ED theo dia chi IEEE
	//APP_DEBUG("--- ZigbeeIO: End Device Addr = 0x%x.\r\n", xAss->shortAddr);
	
	return xAss->shortAddr;
}



