
#include <string.h>
#include <stdio.h>

#include "main.h"

#ifdef macroCONNECTIVITY_ETH

#include "api.h"
#include "MQTTClient.h"


extern uint8_t uIDGw[17];
extern uint8_t uETH_MQTT_RX_Buffer[macroMQTT_RX_BUFFER_LENGHT];

mqttConfig _mqttConfig =
{
	.getMMC		= false,
	.Host		= {171, 244, 27, 194},
	.Port		= 1883,
	.User		= "admin",
	.Pwd		= "20051926",
	.TopicPub	= "test1",
	.TopicSub	= "test2"
};


//Mqtt client
mqtt_client_t xMQTT_Client;
static int siInPub_ID;
bool isMQTTSend_OK = false;


void (*vPublish_Callback)( bool isSendOK );



/********************************** Function **********************************/
static void MQTTClient_IncomingSubcribe_Callback( void *vArg, const char *cTopic, u32_t ulTot_Len );
static void vMQTTClient_IncomingRecvData_Callback( void *vArg, const uint8_t *uData, u16_t uiLen, u8_t uFlags );
static void vMQTTClient_SubcribeRequest_Callback(void *vArg, err_t eResult);
static void vMQTTClient_Connection_Callback(mqtt_client_t *xClient, void *vArg, mqtt_connection_status_t eStatus);
static void vMQTTClient_PublishRequest_Callback(void *vArg, err_t eResult);
/******************************************************************************/




/******************************************************************************
 * Function		: void MQTTClient_IncomingSubcribe_Callback( void *vArg, const char *cTopic, u32_t ulTot_Len )
 * Description	: N/A
 * Param		: 
 *				+ cTopic: topic publish
 *				+ ulTot_len: do dai payload publish
 * Return		: none
*******************************************************************************/
static void MQTTClient_IncomingSubcribe_Callback( void *vArg, const char *cTopic, u32_t ulTot_Len )
{
	APP_DEBUG("\r\n--- MQTTClient: subcribed at topic \"%s\" with total length %d\r\n", cTopic, (uint16_t)ulTot_Len);

	/* Decode topic string into a user defined reference */
	if( strcmp( cTopic, "print_payload" ) == 0 ) 
	{
		siInPub_ID = 0;
	} 
	else if(cTopic[0] == 'A') 
	{
		/* All topics starting with 'A' might be handled at the same way */
		siInPub_ID = 1;
	} 
	else 
	{
		/* For all other topics */
		siInPub_ID = 2;
	}
}




/******************************************************************************
 * Function		: void vMQTTClient_IncomingRecvData_Callback( void *vArg, const uint8_t *uData, u16_t uiLen, u8_t uFlags )
 * Description	: callback when received packet from broker
 * Param		: 
 *				+ uData: data publish
 *				+ uiLen: do dai payload publish
 *				+ uFlags: co trang thai publish
 * Return		: none
*******************************************************************************/
static void vMQTTClient_IncomingRecvData_Callback( void *vArg, const uint8_t *uData, u16_t uiLen, u8_t uFlags )
{
	memset(uETH_MQTT_RX_Buffer, 0, macroMQTT_RX_BUFFER_LENGHT);
//	strcpy((char *)uETH_MQTT_RX_Buffer, (char *)uData);

	for(uint16_t uii = 0; uii < uiLen; uii++)
		uETH_MQTT_RX_Buffer[uii] = uData[uii];

	if( uFlags & MQTT_DATA_FLAG_LAST ) 
	{
		if(siInPub_ID == 0)
		{
			/* Don't trust the publisher, check zero termination */
//			if(uData[uiLen - 1] == 0) 
//			{
//				APP_DEBUG("--- MQTTClient: vMQTTClient_IncomingData_Callback - Data = %s\r\n", (const char *)uData);
//			}
		} 
		else if(siInPub_ID == 1) 
		{
			/* Call an 'A' function... */
		}
		else 
		{
//			APP_DEBUG("--- MQTTClient: vMQTTClient_IncomingData_Callback - Ignoring payload...\r\n");
		}
		APP_DEBUG("--- MQTTClient: Received data = \"%s\" with length = %d, flags = %d\r\n", (char *)uData, uiLen, (uint16_t)uFlags);
	
		memset((void *)uData, 0, uiLen);
		vMain_setEvent(EVENT_ETH_MQTT_RECEIVED); // @suppress("Symbol is not resolved")
	} 
	else 
	{
		/* Handle fragmented payload, store in buffer, write to file or whatever */
	}
}




/******************************************************************************
 * Function		: void vMQTTClient_SubcribeRequest_Callback(void *vArg, err_t eResult)
 * Description	: subcribe request callback
 * Param		: N/A
 * Return		: none
*******************************************************************************/
static void vMQTTClient_SubcribeRequest_Callback(void *vArg, err_t eResult)
{
	/* Just print the result code here for simplicity,
	   normal behaviour would be to take some action if subscribe fails like
	   notifying user, retry subscribe or disconnect from server */
	if(eResult == ERR_OK)
		APP_DEBUG("--- MQTTClient: Subscribe successfully\n");
	else
		APP_DEBUG("--- MQTTClient: Subscribe failture\n");
}




/******************************************************************************
 * Function		: void vMQTTClient_SubcribeRequest_Callback(void *vArg, err_t eResult)
 * Description	: subcribe request callback
 * Param		: N/A
 * Return		: none
*******************************************************************************/
static void vMQTTClient_Connection_Callback(mqtt_client_t *xClient, void *vArg, mqtt_connection_status_t eStatus)
{
	err_t eErr;
	
	if(eStatus == MQTT_CONNECT_ACCEPTED)
	{
		APP_DEBUG("--- MQTTClient: vMQTTClient_Connection_Callback - connected\r\n");

		/* Setup callback for incoming publish requests */
		mqtt_set_inpub_callback(xClient, MQTTClient_IncomingSubcribe_Callback, vMQTTClient_IncomingRecvData_Callback, vArg);
		/* Subscribe to a topic with QoS level 1, call mqtt_sub_request_cb with result */
		eErr = mqtt_subscribe(xClient, (const char *)_mqttConfig.TopicSub, 1, vMQTTClient_SubcribeRequest_Callback, vArg);

		if(eErr != ERR_OK)
			APP_DEBUG("--- MQTTClient: Subcribe topic \"%s\" error. Return: %d\r\n", (const char *)_mqttConfig.TopicSub, eErr);
		else
			APP_DEBUG("--- MQTTClient: Subcribe topic \"%s\" successfully\r\n", (const char *)_mqttConfig.TopicSub);
	}
	else 
	{
		APP_DEBUG("--- MQTTClient: vMQTTClient_Connection_Callback - Disconnected, reason: %d\r\n", eStatus);

		/* Its more nice to be connected, so try to reconnect */
		vMQTTClient_Connect();
	}
}




/******************************************************************************
 * Function		: void vMQTTClient_Connect( void )
 * Description	: Ham khoi tao ket noi Mqtt
 * Param		: 
 *				+ xClient - Mqtt client
 * Return		: none
*******************************************************************************/
void vMQTTClient_Connect( void )
{
	struct mqtt_connect_client_info_t xClientConnect_Info;
	uint8_t uClientID[32] = {0};
	err_t eErr;
	ip4_addr_t IP_Server;

	IP4_ADDR(&IP_Server, _mqttConfig.Host[0], _mqttConfig.Host[1], _mqttConfig.Host[2], _mqttConfig.Host[3]);
	/* Setup an empty client info structure */
	memset(&xClientConnect_Info, 0, sizeof(xClientConnect_Info));

	sprintf((char *)uClientID, "smf-%s-eth", (char *)uIDGw);
	APP_DEBUG("\r\n--- MQTTClient: Client ID = %s\r\n", uClientID);
	
	sprintf((char *)_mqttConfig.TopicPub, "smartfarm/data/%s", (char *)uIDGw);
	sprintf((char *)_mqttConfig.TopicSub, "smartfarm/command/%s", (char *)uIDGw);

	xClientConnect_Info.client_id = (const char *)uClientID;
	xClientConnect_Info.client_user = (const char *)_mqttConfig.User;
	xClientConnect_Info.client_pass = (const char *)_mqttConfig.Pwd;
	xClientConnect_Info.keep_alive = 60;
	
	APP_DEBUG("--- MQTTClient: mqtt connect to \"%d.%d.%d.%d\" port %d\r\n", IP_Server.addr & 0xff, (IP_Server.addr >> 8) & 0xff, (IP_Server.addr >> 16) & 0xff, (IP_Server.addr >> 24) & 0xff, _mqttConfig.Port);

	eErr = mqtt_client_connect(&xMQTT_Client, &IP_Server, _mqttConfig.Port, vMQTTClient_Connection_Callback, 0, &xClientConnect_Info);
	
	if(eErr != ERR_OK)
	{
		APP_DEBUG("--- MQTTClient: Connect to broker error, return %d\r\n", eErr);
		return;
	}
}




/******************************************************************************
 * Function		: void vMQTTClient_PublishRequest_Callback(void *vArg, err_t eResult)
 * Description	: Called when publish is complete either with sucess or failure
 * Param		: xClient - mqtt client
 * Return		: none
*******************************************************************************/
static void vMQTTClient_PublishRequest_Callback(void *vArg, err_t eResult)
{
	if(eResult != ERR_OK) 
	{
		APP_DEBUG("--- MQTTClient: Publish error. result: %d\r\n", eResult);
		vPublish_Callback(false);
		return;
	}
	vPublish_Callback(true);
}




/******************************************************************************
 * Function		: void vMQTTClient_Publish(uint8_t *Payload, void (*PubCallback)( bool ) )
 * Description	: publish data to broker
 * Param		: 
 *				+ xClient - mqtt client
 *				+ Payload - data to publish
 * Return		: none
*******************************************************************************/
void vMQTTClient_Publish(uint8_t *Payload, void (*PubCallback)( bool ) )
{
	if(PubCallback != NULL)
		vPublish_Callback = PubCallback;

	if(bMQTTClient_IsConnected() == true)
	{
		err_t eErr;
		u8_t uQos = 0; /* 0 1 or 2, see MQTT specification */
		u8_t uRetain = 0; /* No don't retain such crappy payload... */

		eErr = mqtt_publish(&xMQTT_Client, (const char *)_mqttConfig.TopicPub, (char *)Payload, strlen((char *)Payload), uQos, uRetain, vMQTTClient_PublishRequest_Callback, NULL);
		if(eErr != ERR_OK)
		{
			APP_DEBUG("--- MQTTClient: Publish err: %d\n", eErr);
			vPublish_Callback(false);
		}
		else
			APP_DEBUG("--- MQTTClient: Publish at topic = \"%s\" with Data = \"%s\" successfully!\r\n\r\n", (const char *)_mqttConfig.TopicPub, (char *)Payload);
	}
	else
		vPublish_Callback(false);
}




/******************************************************************************
 * Function		: void vMQTTClient_Disconnect( void )
 * Description	: Ham huy ket noi MQTT
 * Param		: none
 * Return		: none
*******************************************************************************/
void vMQTTClient_Disconnect( void )
{
	mqtt_disconnect( &xMQTT_Client );
}




/******************************************************************************
 * Function		: bool bMQTTClient_IsConnected(void)
 * Description	: Ham kiem tra ket noi MQTT
 * Param		: none
 * Return		: true - ok, false - nok
*******************************************************************************/
bool bMQTTClient_IsConnected(void)
{
	return mqtt_client_is_connected(&xMQTT_Client);
}

#endif




