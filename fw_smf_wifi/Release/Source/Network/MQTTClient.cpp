
#include "MQTTClient.h"
#include "AppData.h"

extern String MAC;

extern String IDGw;

String MQTT_RX_Buffer;


MqttConfig __MqttConfig =
{
	.Host		= "m10.cloudmqtt.com",
	.Port		= 10434,
	.UserName	= "thaonm",
	.Password	= "thaonm",
	.TopicPub	= "test_publish",
	.TopicSub	= "test_subcribe",
};

// MQTT client
char ClientID[24] = {0};
MqttClient *mqttClient;
Timer T_Mqtt_CheckConnect;
bool bMQTTisConfigurated = false;
bool bMQTTisConnected = false;
uint8_t uCounterSub = 0;





/*******************************************************************************
 * Function		: void vMQTTClient_OnData(String sTopic, String Message)
 * Description	: mqtt receive message
 * Param		:
 * 				+ sTopic: topic receive
 * 				+ sMsg - message received
 * Return		: None
 ******************************************************************************/
static void vMQTTClient_OnData(String sTopic, String Message)
{
	APP_DEBUG("\r\n--- MQTTClient: Received at Topic \"%s\" data = \"%s\"\r\n", sTopic.c_str(), Message.c_str());

	MQTT_RX_Buffer = Message;
	vMain_setEvent(EVENT_CTS_RECEIVED);
}




//static void vMQTTClient_PublishCallback(uint16_t msgId, int type)
//{
//	//APP_DEBUG("--- MQTTClient: Message with id %d and QoS %d was delivered successfully.", msgId, (type==MQTT_MSG_PUBREC? 2: 1));
//}



/*******************************************************************************
 * Function		: void vMQTTClient_Subcribe(String Topic)
 * Description	: mqtt subcribe topics
 * Param		: Topic - topic subcribe
 * Return		: None
 ******************************************************************************/
static void vMQTTClient_Subcribe(String Topic)
{
	if(mqttClient->subscribe(Topic))
		APP_DEBUG("--- MQTTClient: Subcribe topic \"%s\" is successfully\r\n", Topic.c_str());
	else
	{
		APP_DEBUG("--- MQTTClient: Subcribe topic \"%s\" is failture\r\n", Topic.c_str());
	}
}




/*******************************************************************************
 * Function		: void vMQTTClient_Connected( void )
 * Description	: mqtt is connected
 * Param		: None
 * Return		: None
 ******************************************************************************/
static void vMQTTClient_Connected( void )
{
	uCounterSub++;
	if(uCounterSub == 10)
		mqttClient->unsubscribe(__MqttConfig.TopicSub);
	else if(uCounterSub > 10)
	{
		vMQTTClient_Subcribe(__MqttConfig.TopicSub);
		uCounterSub = 0;
	}
	bMQTTisConnected = true;
	//APP_DEBUG("--- MQTTClient: Client is connected\r\n");
}




/*******************************************************************************
 * Function		: void vMQTTClient_Disconnected( void )
 * Description	: mqtt is disconnected
 * Param		: None
 * Return		: None
 ******************************************************************************/
static void vMQTTClient_Disconnected( void )
{
	bMQTTisConnected = false;
	APP_DEBUG("--- MQTTClient: Client is disconnected\r\n");
	mqttClient->connect(ClientID, __MqttConfig.UserName, __MqttConfig.Password);
	vMQTTClient_Subcribe(__MqttConfig.TopicSub);
}




/*******************************************************************************
 * Function		: void vMQTTClient_CheckStatus( void )
 * Description	: mqtt check status
 * Param		: None
 * Return		: None
 ******************************************************************************/
static void vMQTTClient_CheckStatus(void)
{
	if(mqttClient->getConnectionState() == eTCS_Connected)
		vMQTTClient_Connected();
	else
		vMQTTClient_Disconnected();

}



/*******************************************************************************
 * Function		: void vMQTTClient_Stop(void)
 * Description	: mqtt stop connect to broker
 * Param		: None
 * Return		: None
 ******************************************************************************/
void vMQTTClient_Stop(void)
{
	T_Mqtt_CheckConnect.stop();
	bMQTTisConnected = false;
}




/*******************************************************************************
 * Function		: void vMQTTClient_Connect(void)
 * Description	: mqtt connect to broker
 * Param		: None
 * Return		: None
 ******************************************************************************/
void vMQTTClient_Connect(void)
{
	if(bMQTTisConfigurated == false)
	{
		bMQTTisConfigurated = true;

		mqttClient =  new MqttClient(__MqttConfig.Host, __MqttConfig.Port, vMQTTClient_OnData);
		mqttClient->setKeepAlive(60);

		__MqttConfig.TopicPub = "smartfarm/data/" + IDGw;
		__MqttConfig.TopicSub = "smartfarm/command/" + IDGw;

		sprintf(ClientID, "smf-%s", MAC.c_str());
		//APP_DEBUG("--- MQTTClient: Client ID = %s\r\n", ClientID);
		T_Mqtt_CheckConnect.initializeMs(3 * 1000, vMQTTClient_CheckStatus).start(); // every 10 seconds
	}

	APP_DEBUG("--- MQTTClient: Connect to Host: \"%s\" Port: \"%d\" User: \"%s\" Pwd: \"%s\"\r\n", \
				__MqttConfig.Host.c_str(), __MqttConfig.Port, __MqttConfig.UserName.c_str(), __MqttConfig.Password.c_str());

	mqttClient->connect(ClientID, __MqttConfig.UserName, __MqttConfig.Password);
	vMQTTClient_Subcribe(__MqttConfig.TopicSub);
	//mqttClient->setCompleteDelegate(vMQTTClient_IsDisconnect);

}




/*******************************************************************************
 * Function		: bool bMQTTClient_Publish(String spData)
 * Description	: mqtt publish message
 * Param		: spData - message to publish
 * Return		: true - success, false - failture
 ******************************************************************************/
bool bMQTTClient_Publish(String spData)
{
	if(bMQTTisConnected == false)
		return false;

	//if(mqttClient.publish(MQTT_TOPIC_PUBLISH, spData)) // or publishWithQoS
	if(mqttClient->publishWithQoS(__MqttConfig.TopicPub, spData, 0, false, NULL)) // or publishWithQoS
	{
		APP_DEBUG("--- MQTTClient: Publish to Topic = \"%s\" data = \"%s\" is successffuly\r\n", __MqttConfig.TopicPub.c_str(), spData.c_str());
		return true;
	}

	APP_DEBUG("--- MQTTClient: Publish is failture\r\n");
	return false;
}




/*******************************************************************************
 * Function		: void vMQTTClient_Update(void)
 * Description	: mqtt update broker
 * Param		: None
 * Return		: None
 ******************************************************************************/
void vMQTTClient_Update(void)
{
	mqttClient->~MqttClient();
	mqttClient = new MqttClient(__MqttConfig.Host, __MqttConfig.Port, vMQTTClient_OnData);
}



























