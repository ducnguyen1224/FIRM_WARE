
#include "ZigbeeIO.h"


extern uint8 uPacketIDSent;
extern uint8 uMain_TaskID;



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
							AF_SUPRESS_ROUTE_DISC_NETWORK,						//Default
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
					APP_DEBUG("--- ZigbeeIO: NWK_addr_respone. Node address = 0x%x\r\n", pRspNew->nwkAddr);
				}
			}
			break;
		}
  	}//endswitch
}


