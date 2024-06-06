
#include "mac_low_level.h"

#include "Main.h"


uint8 uMain_TaskID; 			
devStates_t xMain_NWKSatte;
uint8 uPacketIDSent; 				//Counter send msg
afAddrType_t xAddrNode;

uint16_t uCounterNWK = 0;



/*******************************************************************************
* Function: void vMain_Init( byte task_id )
* Description: main task init
* Param: task_id - task id
* Return: none
*******************************************************************************/
void vMain_Init( byte task_id )
{
	uMain_TaskID = task_id;
	xMain_NWKSatte = DEV_INIT;
	uPacketIDSent = 0;

	xAddrNode.addrMode = (afAddrMode_t)Addr16Bit;
	xAddrNode.endPoint = 8;
	xAddrNode.addr.shortAddr = 0;

	// Register the endpoint description with the AF
	afRegister( &xDataEPDesc );

	ZDO_RegisterForZDOMsg( uMain_TaskID, Device_annce );
	ZDO_RegisterForZDOMsg( uMain_TaskID, NWK_addr_rsp );
	
	//vUART_Init(macroUART_APP_BASE, 115200);
	vUART_Init(macroUART_DEBUG_BASE, 115200);
	APP_DEBUG("\r\n------------------------------ MAIN TASK ------------------------------\r\n");

	if(macRadioSetTxPower(0xFE) == 0xFE)
		APP_DEBUG("--- Main: Set Tx Power with CC2592 Successfully!\r\n");
	else
		APP_DEBUG("--- Main: Set Tx Power with CC2592 failture!\r\n");
}
//End main init





/*******************************************************************************
* Function: UINT16 uiMain_ProcessEvent( byte task_id, UINT16 events )
* Description: main task event process
* Param: 
*		+ task_id: task id
*		+ events: event task
* Return: none
*******************************************************************************/
UINT16 uiMain_ProcessEvent( byte task_id, UINT16 events )
{
  afIncomingMSGPacket_t *MSGpkt;
  // Data Confirmation message fields
  (void)task_id;          //Intentionally unreferenced parameter

	if ( events & SYS_EVENT_MSG )
	{
		MSGpkt = (afIncomingMSGPacket_t *)osal_msg_receive( uMain_TaskID );
		while ( MSGpkt )
		{
			switch ( MSGpkt->hdr.event )
			{
				case ZDO_CB_MSG: {
					APP_DEBUG("--- MAIN: ZDO_CB_MSG Stat\r\n");
					vZigbeeIO_ProcessZDOMsgs((zdoIncomingMsg_t *)MSGpkt);
					break;
				}
				case AF_DATA_CONFIRM_CMD: {
					if((ZStatus_t)(MSGpkt->hdr.status) == afStatus_SUCCESS)	
					{
						APP_DEBUG("--- MAIN: Send Data OK!. TX Counter = %d\r\n", uPacketIDSent);
					}
					else 
					{
						APP_DEBUG("--- MAIN: Send Data FAIL! Status = 0x%x\r\n", (ZStatus_t)(MSGpkt->hdr.status));
					}
					break;
				}
				case AF_INCOMING_MSG_CMD: {
					uZigbeeIO_RecvMessage(MSGpkt);
					break;
				}
				case ZDO_STATE_CHANGE: {
					//zAddrType_t dstAddr;
					xMain_NWKSatte = (devStates_t)(MSGpkt->hdr.status);
					uCounterNWK++;
					if(uCounterNWK >= 90)
					{
						uCounterNWK = 0;
						SysCtrlReset();
					}

					APP_DEBUG("--- MAIN: ZDO_STATE. NWK State = 0x%x\r\n", xMain_NWKSatte);

					if((xMain_NWKSatte == DEV_ZB_COORD) || (xMain_NWKSatte == DEV_ROUTER) || (xMain_NWKSatte == DEV_END_DEVICE))
					{
						uCounterNWK = 0;
						APP_DEBUG("--- MAIN: NLME_GetShortAddr = 0x%x\r\n", NLME_GetShortAddr());
					}
				}
				break;
			}
			//Release the memory
			osal_msg_deallocate( (uint8 *)MSGpkt );
			MSGpkt = (afIncomingMSGPacket_t *)osal_msg_receive( uMain_TaskID );
		}
		//return unprocessed events
		return (events ^ SYS_EVENT_MSG);
	}
	return 0;
}

						








														
														
														




