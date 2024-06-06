/******************************************************************************
  Filename:       GenericApp.c
  Revised:        $Date: 2014-09-07 13:36:30 -0700 (Sun, 07 Sep 2014) $
  Revision:       $Revision: 40046 $

  Description:    Generic Application (no Profile).


  Copyright 2004-2014 Texas Instruments Incorporated. All rights reserved.

  IMPORTANT: Your use of this Software is limited to those specific rights
  granted under the terms of a software license agreement between the user
  who downloaded the software, his/her employer (which must be your employer)
  and Texas Instruments Incorporated (the "License"). You may not use this
  Software unless you agree to abide by the terms of the License. The License
  limits your use, and you acknowledge, that the Software may not be modified,
  copied or distributed unless embedded on a Texas Instruments microcontroller
  or used solely and exclusively in conjunction with a Texas Instruments radio
  frequency transceiver, which is integrated into your product. Other than for
  the foregoing purpose, you may not use, reproduce, copy, prepare derivative
  works of, modify, distribute, perform, display or sell this Software and/or
  its documentation for any purpose.

  YOU FURTHER ACKNOWLEDGE AND AGREE THAT THE SOFTWARE AND DOCUMENTATION ARE
  PROVIDED “AS IS” WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED,
  INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY, TITLE,
  NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT SHALL
  TEXAS INSTRUMENTS OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER CONTRACT,
  NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR OTHER
  LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
  INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE
  OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT
  OF SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
  (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.

  Should you have any questions regarding your right to use this Software,
  contact Texas Instruments Incorporated at www.TI.com.
******************************************************************************/

/*********************************************************************
  This application isn't intended to do anything useful - it is
  intended to be a simple example of an application's structure.

  This application periodically sends a "Hello World" message to
  another "Generic" application (see 'txMsgDelay'). The application
  will also receive "Hello World" packets.

  This application doesn't have a profile, so it handles everything
  directly - by itself.

  Key control:
    SW1:  changes the delay between TX packets
    SW2:  initiates end device binding
    SW3:
    SW4:  initiates a match description request
*********************************************************************/

/*********************************************************************
 * INCLUDES
 */
#include "OSAL.h"
#include "AF.h"
#include "ZDApp.h"
#include "ZDObject.h"
#include "ZDProfile.h"

#include "GenericApp.h"
#include "DebugTrace.h"
#include "gpio.h"
#include "OnBoard.h"

#include "ZMAC.h"

/* HAL */
#include "hal_lcd.h"
#include "hal_led.h"
#include "hal_key.h"
#include "hal_uart.h"
#include "debug.h"
#include "common.h"
#include "stdio.h"
#include "hal_adc.h"

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * CONSTANTS
 */

/*********************************************************************
 * TYPEDEFS
 */
 extern data_struct my_data;
/*********************************************************************
 * GLOBAL VARIABLES
 */
// This list should be filled with Application specific Cluster IDs.
const cId_t GenericApp_InClusterList[GENERICAPP_MAX_CLUSTERS] =
{
  APPDATA_CLUSTER_CONI_NORMAL
};
const cId_t GenericApp_Out_ClusterList[GENERICAPP_MAX_CLUSTERS] =
{
  APPDATA_CLUSTER_CINO_NORMAL
};

const SimpleDescriptionFormat_t GenericApp_SimpleDesc =
{
  GENERICAPP_ENDPOINT,              	//  int Endpoint;
  GENERICAPP_PROFID,                	//  uint16 AppProfId[2];
  GENERICAPP_DEVICEID,              	//  uint16 AppDeviceId[2];
  GENERICAPP_DEVICE_VERSION,        	//  int   AppDevVer:4;
  GENERICAPP_FLAGS,                 	//  int   AppFlags:4;
  GENERICAPP_MAX_CLUSTERS,          	//  byte  AppNumInClusters;
  (cId_t *)GenericApp_InClusterList,  	//  byte *pAppInClusterList;
  GENERICAPP_MAX_CLUSTERS,          	//  byte  AppNumInClusters;
  (cId_t *)GenericApp_Out_ClusterList   //  byte *pAppInClusterList;
};

// This is the Endpoint/Interface description.  It is defined here, but
// filled-in in GenericApp_Init().  Another way to go would be to fill
// in the structure here and make it a "const" (in code space).  The
// way it's defined in this sample app it is define in RAM.
endPointDesc_t GenericApp_epDesc;

/*********************************************************************
 * EXTERNAL FUNCTIONS
 */

/*********************************************************************
 * LOCAL VARIABLES
 */
uint32 ucMainRXID = 0;
uint8 GenericApp_TaskID;   // Task ID for internal task/event processing
                          // This variable will be received when
                          // GenericApp_Init() is called.

devStates_t GenericApp_NwkState;

uint8 GenericApp_TransID;  // This is the unique message ID (counter)

afAddrType_t GenericApp_DstAddr;

// Number of recieved messages
static uint16 rxMsgCount;
// if sent failt times is 10 => reboot
uint8 count_fail = 0;

// Time interval between sending messages
static uint32 txMsgDelay = GENERICAPP_SEND_MSG_TIMEOUT;

// check set txpower is ok
ZMacStatus_t check;

/*********************************************************************
 * LOCAL FUNCTIONS
 */
static void GenericApp_ProcessZDOMsgs( zdoIncomingMsg_t *inMsg );
static void GenericApp_MessageMSGCB( afIncomingMSGPacket_t *pckt );
static void GenericApp_SendTheMessage( void );
void seriaCommand(uint8 *buff);

#if defined( IAR_ARMCM3_LM )
static void GenericApp_ProcessRtosMessage( void );
#endif

/*********************************************************************
 * NETWORK LAYER CALLBACKS
 */

/*********************************************************************
 * PUBLIC FUNCTIONS
 */
/*********************************************************************/
                        /*USB_CMDfunction*/
 uint8 USB_CMD[100];
 extern bool UART_RX_Flag; 
 void seriaCommand(uint8 *buff)
{
  if(strncmp((const char*)buff,"MQTT_Connected",14)==0)  {/*GenericApp_SendCommandLed(router_Addr,"off");*/DBG_PRINTF("=> LED 1 is OFF\n");}
  if(strncmp((const char*)buff,"on",2)==0)   {/*GenericApp_SendCommandLed(router_Addr,"on");*/DBG_PRINTF("=> LED 1 is ON\n");}
}
//UART Init
static void uart_callback(uint8 port, uint8 event)
{
	
}
static void UartInit(void)
{
  halUARTCfg_t uartConfig;
  
  //Define config
  uartConfig.configured           = true;
  uartConfig.baudRate             = HAL_UART_BR_115200;         // Baudrate
  uartConfig.flowControl          = FALSE;                      // Delete flowcontrol
  uartConfig.flowControlThreshold = NULL;
  uartConfig.rx.maxBufSize        = 512;
  uartConfig.tx.maxBufSize        = 512;
  uartConfig.idleTimeout          = NULL;                       // Time out 6ms/ 1 char
  uartConfig.callBackFunc         = uart_callback;              // Time cback
  uartConfig.intEnable            = FALSE;                      // Delete interrupt
  HalUARTInit();
  HalUARTOpen( HAL_UART_PORT_1, &uartConfig);                   // Open Port 1 
  DBG_PRINTF("Starting UART...\r\n");
  //osal_set_event(GenericApp_TaskID,UART_RX_CHECK_EVT);
 // osal_start_timerEx(GenericApp_TaskID, UART_RX_CHECK_EVT,100);
}

/*
*@fn GpioDoor_Init
*
* @brief Init GPIO Pin to operate input mode.
*
*/
static void GpioDoor_Init(void)
{
      GPIOPinTypeGPIOInput(GPIO_C_BASE, GPIO_PIN_7);
      IOCPadConfigSet(GPIO_C_BASE, GPIO_PIN_7, IOC_OVERRIDE_PUE);
}

/*********************************************************************
 * @fn      GenericApp_Init
 *
 * @brief   Initialization function for the Generic App Task.
 *          This is called during initialization and should contain
 *          any application specific initialization (ie. hardware
 *          initialization/setup, table initialization, power up
 *          notificaiton ... ).
 *
 * @param   task_id - the ID assigned by OSAL.  This ID should be
 *                    used to send messages and set timers.
 *
 * @return  none
 */
void GenericApp_Init( uint8 task_id )
{
  GenericApp_TaskID = task_id;
  GenericApp_NwkState = DEV_INIT;
  GenericApp_TransID = 0;

  // Device hardware initialization can be added here or in main() (Zmain.c).
  // If the hardware is application specific - add it here.
  // If the hardware is other parts of the device add it in main().

  GenericApp_DstAddr.addrMode = (afAddrMode_t)Addr16Bit;
  GenericApp_DstAddr.endPoint = 8;
  GenericApp_DstAddr.addr.shortAddr = 0;

  // Fill out the endpoint description.
  GenericApp_epDesc.endPoint = GENERICAPP_ENDPOINT;
  GenericApp_epDesc.task_id = &GenericApp_TaskID;
  GenericApp_epDesc.simpleDesc= (SimpleDescriptionFormat_t *)&GenericApp_SimpleDesc;
  GenericApp_epDesc.latencyReq = noLatencyReqs;

  // Register the endpoint description with the AF
  afRegister( &GenericApp_epDesc );

  // Register for all key events - This app will handle all key events
  RegisterForKeys( GenericApp_TaskID );
  ZDO_RegisterForZDOMsg( GenericApp_TaskID, End_Device_Bind_rsp );
  ZDO_RegisterForZDOMsg( GenericApp_TaskID, Match_Desc_rsp );
  ZDO_RegisterForZDOMsg( GenericApp_TaskID, NWK_addr_req );
#if HAL_UART==TRUE
  UartInit();
DBG_PRINTF("UART INIT COMPLETED!\r\n");
#endif
  HalLedInit();
  HAL_TURN_ON_LED1();
  HAL_TURN_ON_LED2(); 
  HalAdcInit();
DBG_PRINTF("ADC INIT COMPLETED!\r\n");
 // Set Tx power following ZMAC.h 
  check = ZMacSetTransmitPower(TX_PWR_PLUS_7);
  if(check == ZMacSuccess) DBG_PRINTF("\r\n Set TX power is OK \r\n");
  // Configure GPIO as a input pin
  GpioDoor_Init();
  DBG_PRINTF("GPIO INPUT INIT COMPLETED!\r\n");
  DBG_PRINTF("TEST FUNCTION COMPLETED!\r\n");
}

/*********************************************************************
 * @fn      GenericApp_HandleKeys
 *
 * @brief   Handles all key events for this device.
 *
 * @param   shift - true if in shift/alt.
 * @param   keys - bit field for key events. Valid entries:
 *                 HAL_KEY_SW_4
 *                 HAL_KEY_SW_3
 *                 HAL_KEY_SW_2
 *                 HAL_KEY_SW_1
 *
 * @return  none
 */
static void GenericApp_HandleKeys( uint8 shift, uint8 keys )
{
  //zAddrType_t dstAddr;

  // Shift is used to make each button/switch dual purpose.
  if ( shift )
  {
    if ( keys & HAL_KEY_SW_1 )
    {
    }
    if ( keys & HAL_KEY_SW_2 )
    {

    }
    if ( keys & HAL_KEY_SW_3 )
    {
    }
    if ( keys & HAL_KEY_SW_4 )
    {
    }
  }
  else
  {
    if ( keys & HAL_KEY_SW_1 )
    {
#if defined( SWITCH1_BIND )
      // We can use SW1 to simulate SW2 for devices that only have one switch,
      keys |= HAL_KEY_SW_2;
#elif defined( SWITCH1_MATCH )
      // or use SW1 to simulate SW4 for devices that only have one switch
      keys |= HAL_KEY_SW_4;
#else
      // Normally, SW1 changes the rate that messages are sent
      if ( txMsgDelay > 100 )
      {
        // Cut the message TX delay in half
        txMsgDelay /= 2;
      }
      else
      {
        // Reset to the default
        txMsgDelay = GENERICAPP_SEND_MSG_TIMEOUT;
      }
#endif
    }

    if ( keys & HAL_KEY_SW_2 )
    {
      HAL_TURN_OFF_LED1();
      DBG_PRINTF("KEY 2 pressed \r\n");
    }

    if ( keys & HAL_KEY_SW_3 )
    {
      HAL_TURN_OFF_LED1();
      DBG_PRINTF("KEY 3 pressed \r\n");
    }

    if ( keys & HAL_KEY_SW_4 )
    {
//      HalLedSet ( HAL_LED_4, HAL_LED_MODE_OFF );
      HAL_TURN_OFF_LED1();
      DBG_PRINTF("KEY 4 pressed \r\n");
/*
      // Initiate a Match Description Request (Service Discovery)
      dstAddr.addrMode = AddrBroadcast;
      dstAddr.addr.shortAddr = NWK_BROADCAST_SHORTADDR;
      ZDP_MatchDescReq( &dstAddr, NWK_BROADCAST_SHORTADDR,
                        GENERICAPP_PROFID,
                        GENERICAPP_MAX_CLUSTERS, (cId_t *)GenericApp_ClusterList,
                        GENERICAPP_MAX_CLUSTERS, (cId_t *)GenericApp_ClusterList,
                        FALSE );
*/
    }
  }
}
/*********************************************************************
 * @fn      GenericApp_ProcessEvent
 *
 * @brief   Generic Application Task event processor.  This function
 *          is called to process all events for the task.  Events
 *          include timers, messages and any other user defined events.
 *
 * @param   task_id  - The OSAL assigned task ID.
 * @param   events - events to process.  This is a bit map and can
 *                   contain more than one event.
 *
 * @return  none
 */
uint16 GenericApp_ProcessEvent( uint8 task_id, uint16 events )
{
  afIncomingMSGPacket_t *MSGpkt;
  // Data Confirmation message fields
  ZStatus_t sentStatus;
  (void)task_id;          // Intentionally unreferenced parameter

  if ( events & SYS_EVENT_MSG )
  {
    MSGpkt = (afIncomingMSGPacket_t *)osal_msg_receive( GenericApp_TaskID );
#ifdef DOOR_NODE
                //osal_start_timerEx(GenericApp_TaskID,PIN_READ_EVT,100);
#endif
    while ( MSGpkt )
    {
      switch ( MSGpkt->hdr.event )
      {
        case ZDO_CB_MSG:
			DBG_PRINTF("ZDO_CB_MSG");
          GenericApp_ProcessZDOMsgs( (zdoIncomingMsg_t *)MSGpkt );
          break;
          // process key change:
         case KEY_CHANGE:
          GenericApp_HandleKeys( ((keyChange_t *)MSGpkt)->state, ((keyChange_t *)MSGpkt)->keys );
          uint8 presskeys;
          char skey[20];
          presskeys = ((keyChange_t *)MSGpkt)->keys;
          sprintf(skey,"Press Key:%x\n",presskeys);
          DBG_PRINTF((uint8 *)skey);
          break;
        case AF_DATA_CONFIRM_CMD:
          // This message is received as a confirmation of a data packet sent.
          // The status is of ZStatus_t type [defined in ZComDef.h]
          // The message fields are defined in AF.h
          sentStatus = MSGpkt->hdr.status;
          // Action taken when confirmation is received.
          if ( sentStatus != ZSuccess )
          {
			  DBG_PRINTF_VALUE("\r\nSend msg fail:", sentStatus, 16);
                          count_fail++;
                          if (count_fail > 4) 
                          {
                          //Onboard_soft_reset();
                          count_fail =0;
                          }
		break;	 
            // The data wasn't delivered -- Do something
          }
          else
		  DBG_PRINTF("\r\nSend msg success\r\n");
          break;
        case AF_INCOMING_MSG_CMD:
			DBG_PRINTF("\r\nRecv msg success\r\n");
          GenericApp_MessageMSGCB( MSGpkt );
          break;

		  case ZDO_STATE_CHANGE: {
			char cStr[128] = {""};
          GenericApp_NwkState = (devStates_t)(MSGpkt->hdr.status);
		  
		  sprintf(cStr, "NWK State = 0x%x\n", GenericApp_NwkState);
		  DBG_PRINTF((uint8 *)cStr);
          if ( (GenericApp_NwkState == DEV_ZB_COORD) ||
               (GenericApp_NwkState == DEV_ROUTER) ||
               (GenericApp_NwkState == DEV_END_DEVICE) )
          {
			  
			  
			  sprintf(cStr, "Addr = 0x%x", MSGpkt->srcAddr.addr.shortAddr);
			  //for(uint8 uci = 0; uci < Z_EXTADDR_LEN; uci++)
			  //	sprintf(cStr, "%s%x-", cStr, (byte)(xMsg_Main->srcAddr.addr.extAddr[uci]));
			 DBG_PRINTF("\r\n");
			 DBG_PRINTF((uint8 *)cStr);
			 DBG_PRINTF("\r\n");
                HAL_TURN_OFF_LED2(); 
	        DBG_PRINTF("\r\n ZDO change state\r\n");
                //Start sending "the" message in a regular interval.
		osal_start_timerEx(GenericApp_TaskID,GENERICAPP_SEND_MSG_EVT,txMsgDelay);
#ifdef GAS_SENSOR_NODE
                osal_start_timerEx(GenericApp_TaskID,ADC_READ_EVT,50000);
#endif
// LIGHT Sensor node                
#ifdef LIGHT_NODE
                osal_start_timerEx(GenericApp_TaskID,ADC_READ_LIGHT_EVT,50000);
#endif         
          }
		  }
          break;
      }

      // Release the memory
      osal_msg_deallocate( (uint8 *)MSGpkt );

      // Next
      MSGpkt = (afIncomingMSGPacket_t *)osal_msg_receive( GenericApp_TaskID );
    }

    // return unprocessed events
    return (events ^ SYS_EVENT_MSG);
  }

  // Send a message out - This event is generated by a timer
  //  (setup in GenericApp_Init()).
  if ( events & GENERICAPP_SEND_MSG_EVT )
  {
    // Send "the" message
    GenericApp_SendTheMessage();
        
	// if last msg is sent success, Setup to send message again
    osal_start_timerEx(GenericApp_TaskID,GENERICAPP_SEND_MSG_EVT,txMsgDelay);
    //HAL_TURN_ON_LED1(); 
    HalLedBlink (HAL_LED_1,1,50,100);
    // return unprocessed events
    return (events ^ GENERICAPP_SEND_MSG_EVT);
  }
  /*
  if ( events & UART_RX_CHECK_EVT )
  {
    // if last msg is sent success, Setup to send message again
    if(UART_RX_Flag==TRUE)
    {
     HalUARTRead(NULL,USB_CMD, 100);
     DBG_PRINTF((uint8 *)USB_CMD);
     DBG_PRINTF("\n");
     seriaCommand(USB_CMD);
     memset(USB_CMD, 0, sizeof(USB_CMD));
     UART_RX_Flag=FALSE;
    }
    // if last msg is sent success, Setup to send message again
    osal_start_timerEx(GenericApp_TaskID,UART_RX_CHECK_EVT,1000);
    // return unprocessed events
    return (events ^ UART_RX_CHECK_EVT);
  }
  */
#ifdef GAS_SENSOR_NODE
  if ( events & ADC_READ_EVT)
  {
    uint8 i=0;
    uint16 GasBuf[5],GasMedium;
    char buf[50];
    for(i=0; i<5 ;i++)
    {
      GasBuf[i]=(uint16)(HalAdcRead(HAL_ADC_CHN_AIN7,HAL_ADC_RESOLUTION_12));
    } 
    GasMedium=(uint16)((GasBuf[0]+GasBuf[1]+GasBuf[2]+GasBuf[3]+GasBuf[4])*0.2);
    my_data.Gas=(uint16)(GasMedium*0.806);
    if(  my_data.Gas >=400 && my_data.AlarmHighLevel==FALSE) 
    {
      DBG_PRINTF("Alarm Gas too high\n");
      Alarm_gas_high_level();
    }
    if(my_data.Gas <400 && my_data.AlarmHighLevel==TRUE) my_data.AlarmHighLevel=FALSE;
    sprintf(buf,"<-- SENSOR VOLT:%d-->\n",my_data.Gas);
    DBG_PRINTF((uint8 *)buf);
    osal_start_timerEx(GenericApp_TaskID,ADC_READ_EVT,50000);
    // return unprocessed events
    return (events ^ ADC_READ_EVT);
   }
   if ( events & ALARM_EVT)
  {
    HalLedBlink (HAL_LED_4,4,30,200);
    if(my_data.AlarmHighLevel)
    {
    osal_start_timerEx(GenericApp_TaskID,ALARM_EVT,50000);
    }
    // return unprocessed events
    return (events ^ ALARM_EVT);
   } 
#endif
 /* define light node process event */
#ifdef LIGHT_NODE
   if ( events & ADC_READ_LIGHT_EVT)
   {
     uint16 LightBuf[1];//,LightMedium;
     char buflight[50];
     DBG_PRINTF("Reading Light Sensor\r\n");
     LightBuf[0]=HalAdcRead(HAL_ADC_CHN_AIN2,HAL_ADC_RESOLUTION_12);
     sprintf(buflight,"<-- SENSOR Light:%d-->\n",LightBuf[0]);
     DBG_PRINTF((uint8 *)buflight);
     osal_start_timerEx(GenericApp_TaskID,ADC_READ_LIGHT_EVT,50000);
    // return unprocessed events
     return (events ^ ADC_READ_LIGHT_EVT);
   }
#endif
// end def Light node

#ifdef DOOR_NODE
  if ( events & PIN_READ_EVT)
  {
    uint8 statusPin;
     // read status PIN 
     DBG_PRINTF("STATUS DOOR\r\n");
     statusPin = (uint8)GPIOPinRead(GPIO_C_BASE, GPIO_PIN_7);
     if (statusPin == 0) DBG_PRINTF("DOOR CLOSED\r\n");
     else DBG_PRINTF("DOOR OPENED\r\n");
   //  teststring();
     osal_start_timerEx(GenericApp_TaskID,PIN_READ_EVT,50000);
    // return unprocessed events
     return (events ^ PIN_READ_EVT);
  }
#endif
  // Discard unknown events
  return 0;
}

/*********************************************************************
 * Event Generation Functions
 */

/*********************************************************************
 * @fn      GenericApp_ProcessZDOMsgs()
 *
 * @brief   Process response messages
 *
 * @param   none
 *
 * @return  none
 */
static void GenericApp_ProcessZDOMsgs( zdoIncomingMsg_t *inMsg )
{
  ZDO_DeviceAnnce_t dev_annce;
  switch ( inMsg->clusterID )
  {
      case Device_annce:
          ZDO_ParseDeviceAnnce(inMsg, &dev_annce);
	  DBG_PRINTF("\r\nNew node joined\r\n");
	  DBG_PRINTF_VALUE("\r\nnwk addr:", dev_annce.nwkAddr, 16);
	  DBG_PRINTF("\r\nIEEE address:");
	  DBG_NPRINTF(dev_annce.extAddr, Z_EXTADDR_LEN);
          break;
      case NWK_addr_req:
	  DBG_PRINTF("\r\nrecv nwk addr req\r\n");
	  break;
    case End_Device_Bind_rsp:
      if ( ZDO_ParseBindRsp( inMsg ) == ZSuccess )
      {
        // Light LED
        //HalLedSet( HAL_LED_4, HAL_LED_MODE_ON );
        DBG_PRINTF("End Device Bind done\r\n");
      }
#if defined( BLINK_LEDS )
      else
      {
        // Flash LED to show failure
        //HalLedSet ( HAL_LED_4, HAL_LED_MODE_FLASH );
      }
#endif
      break;

    case Match_Desc_rsp:
      {
        ZDO_ActiveEndpointRsp_t *pRsp = ZDO_ParseEPListRsp( inMsg );
		DBG_PRINTF("Match_Desc_rsp\r\n");
        if ( pRsp )
        {
          if ( pRsp->status == ZSuccess && pRsp->cnt )
          {
            GenericApp_DstAddr.addrMode = (afAddrMode_t)Addr16Bit;
            GenericApp_DstAddr.addr.shortAddr = pRsp->nwkAddr;
            // Take the first endpoint, Can be changed to search through endpoints
            //GenericApp_DstAddr.endPoint = pRsp->epList[0];

            // Light LED
            //HalLedSet( HAL_LED_4, HAL_LED_MODE_ON );
          }
          osal_mem_free( pRsp );
        }
      }
      break;
  }
}
/*********************************************************************
 * LOCAL FUNCTIONS
 */

/*********************************************************************
 * @fn      GenericApp_MessageMSGCB
 *
 * @brief   Data message processor callback.  This function processes
 *          any incoming data - probably from other devices.  So, based
 *          on cluster ID, perform the intended action.
 *
 * @param   none
 *
 * @return  none
 */
static void GenericApp_MessageMSGCB( afIncomingMSGPacket_t *pkt )
{
 // char* msgrev;
  //char** msgfield;
 // uint8 *datafield[12];
  switch ( pkt->clusterId )
  {
    case APPDATA_CLUSTER_CONI_NORMAL:
	{
			char cStr[128] = {""};
          rxMsgCount += 1;  // Count this message
          //HalLedSet ( HAL_LED_4, HAL_LED_MODE_BLINK );  // Blink an LED
			char ieee_source_addr_string[17];
			get_ieee_source_addr_string((uint8*)ieee_source_addr_string,pkt->srcAddr.addr.extAddr);
			sprintf(cStr, "\r\nAddr Coord = %s\r\n", ieee_source_addr_string);
			DBG_PRINTF((uint8*)cStr);
          DBG_PRINTF((uint8 *)pkt->cmd.Data);
		  ucMainRXID++;
		  sprintf(cStr, "\r\nRX Count = %d\r\n", ucMainRXID);
		  DBG_PRINTF((uint8 *)cStr);
		  
			  
			sprintf(cStr, "Addr Coordinator = 0x%x", pkt->srcAddr.addr.shortAddr);
		   DBG_PRINTF("\r\n");
		   DBG_PRINTF((uint8 *)cStr);
		   DBG_PRINTF("\r\n");
         // msgrev = (char *)pkt->cmd.Data;
//          msgfield =str_split((char *)pkt->cmd.Data,',');
//          if (msgfield)
//          {
//            int i;
//            for (i = 0; *(msgfield + i); i++)
//            {
//              char* msg;
//              msg = *(msgfield +i);
////              DBG_PRINTF(*(msgfield + i));
//              DBG_PRINTF("***\n");
//              DBG_PRINTF((uint8*)msg);
//              free(*(msgfield + i));
//            }
//            DBG_PRINTF("\n");
//            free(msgfield);
//          }
          DBG_PRINTF("\r\n");
//#ifdef DEBUG_UART
 //DBG_PRINTF_VALUE("Rcvd:", rxMsgCount,16) 
 //DBG_PRINTF("\r\n");
//#endif
          break;
	}
  }
}

/*********************************************************************
 * @fn      GenericApp_SendTheMessage
 *
 * @brief   Send "the" message.
 *
 * @param   none
 *
 * @return  none
 */
#ifndef  SHT10
#define  SHT10
#endif

static void GenericApp_SendTheMessage( void )
{

  uint8 buf[200] = "Hello world";
  afStatus_t sentStatus;
  PreaperData(DATA_TYPE,buf);
  DBG_PRINTF((uint8_t*)buf);
  GenericApp_DstAddr.addrMode = (afAddrMode_t)Addr16Bit;
  // network addr of ZC is 0
  GenericApp_DstAddr.addr.shortAddr = 0;
  // endpoint of app on zc
  GenericApp_DstAddr.endPoint = 8;
  DBG_PRINTF_VALUE("\r\n[Send]TransID:", GenericApp_TransID, 10);
  sentStatus = AF_DataRequest( &GenericApp_DstAddr, &GenericApp_epDesc,
                              APPDATA_CLUSTER_CINO_NORMAL,
                              (byte)osal_strlen((char*)buf ) + 1,
                              (byte *)&buf,
                              &GenericApp_TransID,
                              AF_DISCV_ROUTE, AF_DEFAULT_RADIUS );
  if (sentStatus== afStatus_SUCCESS )
  {
  //Successfully requested to be sent.
  }
 // else
  //{
    //DBG_PRINTF_VALUE("\r\nSend msg fail:", sentStatus, 16);
    //count_fail++;
    //if (count_fail >= 10) {
     //Onboard_soft_reset();
     //count_fail =0;
  // }
    // Error occurred in request to send.
 // }
}
#if defined( IAR_ARMCM3_LM )
/*********************************************************************
 * @fn      GenericApp_ProcessRtosMessage
 *
 * @brief   Receive message from RTOS queue, send response back.
 *
 * @param   none
 *
 * @return  none
 */
static void GenericApp_ProcessRtosMessage( void )
{
  osalQueue_t inMsg;

  if ( osal_queue_receive( OsalQueue, &inMsg, 0 ) == pdPASS )
  {
    uint8 cmndId = inMsg.cmnd;
    uint32 counter = osal_build_uint32( inMsg.cbuf, 4 );

    switch ( cmndId )
    {
      case CMD_INCR:
        counter += 1;  /* Increment the incoming counter */
                       /* Intentionally fall through next case */

      case CMD_ECHO:
      {
        userQueue_t outMsg;

        outMsg.resp = RSP_CODE | cmndId;  /* Response ID */
        osal_buffer_uint32( outMsg.rbuf, counter );    /* Increment counter */
        osal_queue_send( UserQueue1, &outMsg, 0 );  /* Send back to UserTask */
        break;
      }

      default:
        break;  /* Ignore unknown command */
    }
  }
}
#endif

/*********************************************************************
 */
