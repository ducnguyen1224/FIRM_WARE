
#ifndef __MQTTCLIENT_H
#define __MQTTCLIENT_H

#include "../main.h"



typedef struct
{
	String 			Host;
	unsigned int 	Port;
	String 			UserName;
	String 			Password;
	String 			TopicPub;
	String 			TopicSub;
}MqttConfig;




/*******************************************************************************
 * Function		: void vMQTTClient_Stop(void)
 * Description	: mqtt stop connect to broker
 * Param		: None
 * Return		: None
 ******************************************************************************/
void vMQTTClient_Stop(void);




/*******************************************************************************
 * Function		: void vMQTTClient_Connect(void)
 * Description	: mqtt connect to broker
 * Param		: None
 * Return		: None
 ******************************************************************************/
void vMQTTClient_Connect(void);




/*******************************************************************************
 * Function		: bool bMQTTClient_Publish(String spData)
 * Description	: mqtt publish message
 * Param		: spData - message to publish
 * Return		: true - success, false - failture
 ******************************************************************************/
bool bMQTTClient_Publish(String spData);




/*******************************************************************************
 * Function		: void vMQTTClient_Update(void)
 * Description	: mqtt update broker
 * Param		: None
 * Return		: None
 ******************************************************************************/
void vMQTTClient_Update(void);




#endif






