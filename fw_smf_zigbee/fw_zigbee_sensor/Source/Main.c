
#include "mac_low_level.h"

#include "Main.h"
#include "Parse.h"


extern char UART_APP_RX_Buffer[macroUART_RX_BUF_SIZE];
extern char UART_APP_TX_Buffer[macroUART_TX_BUF_SIZE];
extern char ZIGBEE_RX_Buffer[macroZIGBEE_RX_BUFF_SIZE];
extern char ZIGBEE_TX_Buffer[macroZIGBEE_TX_BUFF_SIZE];


uint8 uMain_TaskID; 			
devStates_t xMain_NWKSatte;
uint8 uPacketIDSent; 				//Counter send msg
afAddrType_t xAddrCoordinator;

bool bNetworkIsOK = false;

char ED_IEEEAddr[17] = {0};

uint8_t uNWK_Counter = 0;


void vMain_GetIDLocal( char *IDLocal );




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

	xAddrCoordinator.addrMode = (afAddrMode_t)Addr16Bit;
	xAddrCoordinator.endPoint = 8;
	xAddrCoordinator.addr.shortAddr = 0;

	// Register the endpoint description with the AF
	afRegister( &xDataEPDesc );

	//ZDO_RegisterForZDOMsg( uMain_TaskID, Device_annce );
	//ZDO_RegisterForZDOMsg( uMain_TaskID, NWK_addr_rsp );
	
	vUART_Init(macroUART_APP_BASE, 38400);
	vUART_Init(macroUART_DEBUG_BASE, 115200);
	APP_DEBUG("\r\n------------------------------ MAIN TASK ------------------------------\r\n");

	if(macRadioSetTxPower(0xFF) == 0xFF)
		APP_DEBUG("--- Main: Set Tx Power with CC2592 Successfully!\r\n");
	else
		APP_DEBUG("--- Main: Set Tx Power with CC2592 failture!\r\n");
	
	vMain_GetIDLocal(ED_IEEEAddr);
	APP_DEBUG("--- Main: ID local = %s\r\n", ED_IEEEAddr);
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
						UART_APP_SEND_MESSAGE("RESP,SMF_GW,OK!");
					}
					else 
					{
						macroLOOP_EVENT(uMain_TaskID, EVENT_ZIGBEE_SEND, macroTIME_LOOP_EVENT_RESEND);
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

					APP_DEBUG("--- MAIN: ZDO_STATE. NWK State = 0x%x\r\n", xMain_NWKSatte);
					bNetworkIsOK = false;
					uNWK_Counter++;
					if(uNWK_Counter >= 120)
					{
						uNWK_Counter = 0;
						SysCtrlReset();
					}
					if((xMain_NWKSatte == DEV_ZB_COORD) || (xMain_NWKSatte == DEV_ROUTER) || (xMain_NWKSatte == DEV_END_DEVICE))
					{
						uNWK_Counter = 0;
						bNetworkIsOK = true;
						APP_DEBUG("--- MAIN: NLME_GetShortAddr = 0x%x\r\n", NLME_GetShortAddr());
						//macroLOOP_EVENT(uMain_TaskID, EVENT_UART_RECEIVE, 20000);
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
	
//	if(events & EVENT_ZIGBEE_LOOP)
//	{
//		APP_DEBUG("\r\n\r\n--- Main: EVENT_ZIGBEE_LOOP\r\n");
//		
//		sprintf((char *)UART_APP_RX_Buffer, "DATA,SMF_SENSN,12345678,PH:12,EC:2.1,TempA:31,HumiA:87!");
//
//		macroSET_EVENT(uMain_TaskID, EVENT_UART_RECEIVE);
//		macroLOOP_EVENT(uMain_TaskID, EVENT_ZIGBEE_LOOP, 20000);
//		
//		return (events ^ EVENT_ZIGBEE_LOOP);
//	}
	
	//Serial receive
	if(events & EVENT_UART_RECEIVE)
	{
		APP_DEBUG("--- Main: EVENT_UART_RECEIVE\r\n");
		
		//sprintf((char *)UART_APP_RX_Buffer, "DATA,SMF_SENSN,12345678,PH:5,EC:0.4,TempG:25,HumiG:87!");
		//Kiem tra ban tin who am i
		if(strstr(UART_APP_RX_Buffer, "AMI") != NULL)
		{
			UART_APP_SEND_MESSAGE("WAMI,Zigb!");
		}
		else
		{
			PacketIO _PacketIO = {0};
			vParse_MsgSplit(UART_APP_RX_Buffer, &_PacketIO, true);
			vParse_PackingMsg(&_PacketIO, ZIGBEE_TX_Buffer, false);
			macroSET_EVENT(uMain_TaskID, EVENT_ZIGBEE_SEND);
		}
		//macroLOOP_EVENT(uMain_TaskID, EVENT_UART_RECEIVE, 20000);
		
		return (events ^ EVENT_UART_RECEIVE);
	}

	//Serial send
	if(events & EVENT_UART_SEND)
	{
		APP_DEBUG("--- Main: EVENT_UART_SEND\r\n");
		UART_APP_SEND_MESSAGE(UART_APP_TX_Buffer);
		
		return (events ^ EVENT_UART_SEND);
	}

	//zigbee receive
	if(events & EVENT_ZIGBEE_RECEIVE)
	{
		APP_DEBUG("--- Main: EVENT_ZIGBEE_RECEIVE\r\n");
		PacketIO _PacketIO = {0};
		vParse_MsgSplit(ZIGBEE_RX_Buffer, &_PacketIO, false);
		vParse_PackingMsg(&_PacketIO, UART_APP_TX_Buffer, true);
		macroSET_EVENT(uMain_TaskID, EVENT_UART_SEND);
		
		return (events ^ EVENT_ZIGBEE_RECEIVE);
	}
	
	//zigbee send
	if(events & EVENT_ZIGBEE_SEND)
	{
		APP_DEBUG("--- Main: EVENT_ZIGBEE_SEND\r\n");
	
		if(  (bNetworkIsOK == false) || (uZigbeeIO_SendMessage(xAddrCoordinator, APPDATA_CLUSTER_COOR_IN_NODE_OUT, (uint8 *)ZIGBEE_TX_Buffer) == 0) )
			macroLOOP_EVENT(uMain_TaskID, EVENT_ZIGBEE_SEND, macroTIME_LOOP_EVENT_RESEND);
		
		return (events ^ EVENT_ZIGBEE_SEND);
	}
	
	return 0;
}

						



/*******************************************************************************
* Function: void vMain_GetIDLocal( char *IDLocal )
* Description: get ID device local
* Param: None
* Return: None
*******************************************************************************/
void vMain_GetIDLocal( char *IDLocal )
{
	uint8 *IDExtLocal = 0;
	uint8 cHex[17] = "0123456789ABCDEF";
	uint8 uCnt = 0;
	
	IDExtLocal = NLME_GetExtAddr();
	
	for(uint8 ui = 0; ui < Z_EXTADDR_LEN; ui++)
	{
		IDLocal[uCnt++] = cHex[IDExtLocal[ui] / 0x10];
		IDLocal[uCnt++] = cHex[IDExtLocal[ui] % 0x10];
	}
}




														
														
														




