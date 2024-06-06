
#ifndef __MYMQTTCLIENT_H
#define __MYMQTTCLIENT_H

#include "mqtt.h"


#define macroMQTT_RX_BUFFER_LENGHT					512
#define macroMQTT_TX_BUFFER_LENGHT					512



typedef struct
{
	bool		getMMC;		//get from sdcard
	uint8_t		Host[4];
	uint16_t	Port;
	uint8_t		User[32];
	uint8_t		Pwd[32];
	uint8_t		TopicPub[64];
	uint8_t		TopicSub[64];
}mqttConfig;




/******************************************************************************
 * Function		: void vMQTTClient_Connect( void )
 * Description	: Ham khoi tao ket noi Mqtt
 * Param		: None
 * Return		: none
*******************************************************************************/
void vMQTTClient_Connect( void );



/******************************************************************************
 * Function		: void vMQTTClient_Disconnect( void )
 * Description	: Ham huy ket noi MQTT
 * Param		: none
 * Return		: none
*******************************************************************************/
void vMQTTClient_Disconnect( void );




/******************************************************************************
 * Function		: void vMQTTClient_Publish(uint8_t *Payload, void (*PubCallback)( bool ) )
 * Description	: publish data to broker
 * Param		: 
 *				+ xClient - mqtt client
 *				+ Payload - data to publish
 * Return		: none
*******************************************************************************/
void vMQTTClient_Publish(uint8_t *Payload, void (*PubCallback)( bool ) );



/******************************************************************************
 * Function		: bool bMQTTClient_IsConnected(void)
 * Description	: Ham kiem tra ket noi MQTT
 * Param		: none
 * Return		: true - ok, false - nok
*******************************************************************************/
bool bMQTTClient_IsConnected(void);



#endif



