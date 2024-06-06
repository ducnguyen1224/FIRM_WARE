
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
afAddrType_t xAddrNode;

uint8 uZigbee_ResendCounter = 0;
bool bUARTResponseOK = false;
uint8_t uSerialResend_Counter = 0;



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
	
	//UartInit();
	vUART_Init(macroUART_APP_BASE, 38400);
	vUART_Init(macroUART_DEBUG_BASE, 115200);
	APP_DEBUG("\r\n------------------------------ MAIN TASK ------------------------------\r\n");

	if(macRadioSetTxPower(0xFF) == 0xFF)
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
						uZigbee_ResendCounter = 0;
						APP_DEBUG("--- MAIN: Send Data OK!. TX Counter = %d\r\n", uPacketIDSent);
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
					if((xMain_NWKSatte == DEV_ZB_COORD) || (xMain_NWKSatte == DEV_ROUTER) || (xMain_NWKSatte == DEV_END_DEVICE))
					{
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
	
	//Serial receive
	if(events & EVENT_UART_RECEIVE)
	{
		APP_DEBUG("--- Main: EVENT_UART_RECEIVE\r\n");
		
		//Kiem tra ban tin who am i
		if(strstr(UART_APP_RX_Buffer, "WAMI") != NULL)
		{
			APP_DEBUG("--- Main: uart message is Who am i\r\n");
			UART_APP_SEND_MESSAGE("WAMI,Zigb!");
		}
		else if(strstr(UART_APP_RX_Buffer, "RESP,") != NULL)
		{
			APP_DEBUG("--- Main: uart message is response\r\n");
			bUARTResponseOK = true;
		}
		else
		{
			PacketIO _PacketIO = {0};
			vParse_MsgSplit(UART_APP_RX_Buffer, &_PacketIO, true);
			
			associated_devices_t *xAss;
			uint8 Addr16[Z_EXTADDR_LEN] = {0};
			
			uint8_t ucj = 0;
			for(uint8_t uci = 0; uci < Z_EXTADDR_LEN; uci++)
			{
				//Chuyen dia chi IEEE ve 8 Byte dia chi
				uint8 xCnt = 0;
				xCnt = (_PacketIO.IDZB[ucj] <= '9')? (_PacketIO.IDZB[ucj] ^ 0x30) : (((_PacketIO.IDZB[ucj] < 'a')? (_PacketIO.IDZB[ucj] - 'A') : (_PacketIO.IDZB[ucj] - 'a')) + 10);
				Addr16[uci] = xCnt * 16 + ((_PacketIO.IDZB[ucj + 1] <= '9')? (_PacketIO.IDZB[ucj + 1] ^ 0x30) : (((_PacketIO.IDZB[ucj + 1] < 'a')? (_PacketIO.IDZB[ucj + 1] - 'A') : (_PacketIO.IDZB[ucj + 1] - 'a')) + 10));
				ucj += 2;
			}
			
			xAss = AssocGetWithExt(Addr16);		//Lay dia chi cua ED theo dia chi IEEE
			//APP_DEBUG("--- Main: request end device short address. IEEE = %s.\r\n", (char *)_PacketIO.IDZB);
//			for(uint8_t uCnt = 0; uCnt < Z_EXTADDR_LEN; uCnt++)
//				APP_DEBUG("%x", uLongAddr[uCnt]);
//			APP_DEBUG("\r\n");
			
			if(((xAss->shortAddr) == 0xFFFF) || ((xAss->shortAddr) == 0x8000))
			{
				APP_DEBUG("--- Main: End Device don't Connected to Coordinator!. Request network ED\r\n");
				ZDP_NwkAddrReq( Addr16, ZDP_ADDR_REQTYPE_SINGLE, 0, 0 );
			}
			else
			{
				APP_DEBUG("--- Main: ED Addr = 0x%x.\r\n", xAss->shortAddr);
				xAddrNode.addr.shortAddr = xAss->shortAddr;
				macroSET_EVENT(uMain_TaskID, EVENT_ZIGBEE_SEND);
			}
			
			vParse_PackingMsg(&_PacketIO, ZIGBEE_TX_Buffer, false);
		}
		
		return (events ^ EVENT_UART_RECEIVE);
	}

	//Serial send
	if(events & EVENT_UART_SEND)
	{
		//APP_DEBUG("--- Main: EVENT_UART_SEND\r\n");
		if(bUARTResponseOK == false)
		{
			uSerialResend_Counter++;
			if(uSerialResend_Counter >= 10)
			{
				UART_APP_SEND_MESSAGE(UART_APP_TX_Buffer);
				uSerialResend_Counter = 0;
			}
			macroLOOP_EVENT(uMain_TaskID, EVENT_UART_SEND, macroTIME_LOOP_EVENT_RESEND);
		}
		else
		{
			bUARTResponseOK = false;
			uSerialResend_Counter = 0;
			APP_DEBUG("--- Main: uart send message is successfully\r\n");
		}
		
		return (events ^ EVENT_UART_SEND);
	}

	//zigbee receive
	if(events & EVENT_ZIGBEE_RECEIVE)
	{
		APP_DEBUG("--- Main: EVENT_ZIGBEE_RECEIVE\r\n");
		
		PacketIO _PacketIO = {0};
		vParse_MsgSplit(ZIGBEE_RX_Buffer, &_PacketIO, false);
		
		if( (strcmp(_PacketIO.TypePacket, "DATA") == 0) || (strcmp(_PacketIO.TypePacket, "CONF") == 0) || (strcmp(_PacketIO.TypePacket, "REQS") == 0) || \
			(strcmp(_PacketIO.TypePacket, "CTRL") == 0) || (strcmp(_PacketIO.TypePacket, "RESP") == 0) || (strcmp(_PacketIO.TypePacket, "WAMI") == 0) )
		{
			vParse_PackingMsg(&_PacketIO, UART_APP_TX_Buffer, true);
			if(UART_APP_TX_Buffer[0] > 0)
			{
				bUARTResponseOK = false;
				macroSET_EVENT(uMain_TaskID, EVENT_UART_SEND);
			}
			else
			{
				APP_DEBUG("--- Main: zigbee message not found\r\n");
			}
		}
	
		
		return (events ^ EVENT_ZIGBEE_RECEIVE);
	}
	
	//zigbee send
	if(events & EVENT_ZIGBEE_SEND)
	{
		APP_DEBUG("--- Main: EVENT_ZIGBEE_SEND\r\n");
		
		uZigbee_ResendCounter++;
		if(uZigbee_ResendCounter <= 10)
		{
			if(uZigbeeIO_SendMessage(xAddrNode, APPDATA_CLUSTER_COOR_OUT_NODE_IN, (uint8 *)ZIGBEE_TX_Buffer) == 0)
				macroLOOP_EVENT(uMain_TaskID, EVENT_ZIGBEE_SEND, macroTIME_LOOP_EVENT_RESEND);
		}
		else
		{
			uZigbee_ResendCounter = 0;
			APP_DEBUG("--- Main: Can't send message\r\n");
		}
		
		return (events ^ EVENT_ZIGBEE_SEND);
	}
	
	return 0;
}

						








														
														
														




