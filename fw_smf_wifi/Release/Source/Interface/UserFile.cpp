
#include <string.h>

#include "FileAccess.h"
#include "UserFile.h"


//define data file for esp and application
#define macroUSER_FILE_WIFI_CONFIG					"WifiConfig.conf"
#define macroUSER_FILE_MQTT_CONFIG					"MqttConfig.conf"

//wifi
#define macroUSER_WIFI_USER_KEY						"user"
#define macroUSER_WIFI_PWD_KEY						"password"

//mqtt
#define macroUSER_MQTT_HOST_KEY						"host"
#define macroUSER_MQTT_PORT_KEY						"port"
#define macroUSER_MQTT_USER_KEY						"user"
#define macroUSER_MQTT_PWD_KEY						"password"



/*****************************************************************************
 * Function	: void vFileAccess_ReadWifiConfig(String SSID, String PWD)
 * Desc		: used to read wifi config from file.
 * Param	:
 * 			+ SSID: name of wifi
 * 			+ PWD: password of wifi
 * return	: None
 ****************************************************************************/
void vUserFile_Read_WifiConfig(String *SSID, String *PWD)
{
	char Buffer[90] = {0};
	String Array[2];
	uint8 ui = 0, uj = 0, uk = 0, uCnt = 0;

	if(eFileAccess_Read_All(macroUSER_FILE_WIFI_CONFIG, Buffer) == eFileFailture)
	{
		//APP_DEBUG("--- UserFile: Read wifi config failture. Can't read file\r\n");
		return;
	}

	for(ui = 0; Buffer[ui] > 0; ui++)
	{
		if( (Buffer[ui] == '=') || (Buffer[ui] == '\n') )
		{
			uj++;
			uCnt++;
			uk = 0;
		}
		else if(Buffer[ui] == '\r')
			continue;
		else
		{
			if((uCnt % 2) == 1)
				Array[uj / 2] += Buffer[ui];
		}

		if(uCnt >= 4)
			break;
	}

//	APP_DEBUG("--- UserFile: SSID = %s\r\n", Array[0].c_str());
//	APP_DEBUG("--- UserFile: PWD = %s\r\n", Array[1].c_str());

	//if(Array[0] == (char *)macroUSER_WIFI_USER_KEY)
		*SSID = Array[0];
	//if(Array[2] == (char *)macroUSER_WIFI_PWD_KEY)
		*PWD = Array[1];
}




/*****************************************************************************
 * Function	: void vUserFile_Write_WifiConfig(String SSID, String PWD)
 * Desc		: used to write wifi config to file.
 * Param	:
 * 			+ SSID: name of wifi
 * 			+ PWD: password of wifi
 * return	: None
 ****************************************************************************/
void vUserFile_Write_WifiConfig(String SSID, String PWD)
{
	char cStr[90] = {0};

	sprintf(cStr, "%s=%s\r\n%s=%s\r\n", macroUSER_WIFI_USER_KEY, SSID.c_str(), macroUSER_WIFI_PWD_KEY, PWD.c_str());
	eFileAccess_Write_All(macroUSER_FILE_WIFI_CONFIG, cStr);
//	if(eFileAccess_Write_All(macroUSER_FILE_WIFI_CONFIG, cStr) != eFileSuccess);
//		APP_DEBUG("--- UserFile: write wifi config is successfully\r\n");
//	else
		//APP_DEBUG("--- UserFile: write wifi config is failture\r\n");
}




/*****************************************************************************
 * Function	: void vUserFile_Read_MqttConfig(String *Host, unsigned int *Port, String *User, String *Pwd)
 * Desc		: used to read mqtt config from file.
 * Param	:
 * 			+ Host: host address
 * 			+ Port: port mqtt
 * return	: None
 ****************************************************************************/
void vUserFile_Read_MqttConfig(String *Host, unsigned int *Port, String *User, String *Pwd)
{
	char Buffer[128] = {0};
	String Array[4];
	uint8 ui = 0, uj = 0, uCnt = 0;

	if(eFileAccess_Read_All((char *)macroUSER_FILE_MQTT_CONFIG, Buffer) == eFileFailture)
	{
		//APP_DEBUG("--- UserFile: Read mqtt config failture. Can't read file\r\n");
		return;
	}

	for(ui = 0; Buffer[ui] > 0; ui++)
	{
		if( (Buffer[ui] == '=') || (Buffer[ui] == '\n') )
		{
			uj++;
			uCnt++;
		}
		else if(Buffer[ui] == '\r')
			continue;
		else
		{
			if((uCnt % 2) == 1)
				Array[uj / 2] += Buffer[ui];
		}

		if(uCnt >= 8)
			break;
	}

//	APP_DEBUG("--- UserFile: Host = %s\r\n", Array[0].c_str());
//	APP_DEBUG("--- UserFile: Port = %s\r\n", Array[1].c_str());
//	APP_DEBUG("--- UserFile: User = %s\r\n", Array[2].c_str());
//	APP_DEBUG("--- UserFile: Pwd = %s\r\n", Array[3].c_str());

	//if(Array[0] == macroUSER_MQTT_HOST_KEY)
		*Host = Array[0];
	//if(Array[2] == macroUSER_MQTT_PORT_KEY)
		*Port = atoi(Array[1].c_str());
	//if(Array[4] == macroUSER_MQTT_USER_KEY)
		*User = Array[2];
	//if(Array[6] == macroUSER_MQTT_PWD_KEY)
		*Pwd = Array[3];
}




/*****************************************************************************
 * Function	: void vUserFile_Write_MqttConfig(String Host, unsigned int Port, String User, String Pwd)
 * Desc		: used to write mqtt config to file.
 * Param	:
 * 			+ Host: host address
 * 			+ Port: port mqtt
 * return	: None
 ****************************************************************************/
void vUserFile_Write_MqttConfig(String Host, unsigned int Port, String User, String Pwd)
{
	char cStr[128] = {0};

	sprintf(cStr, "%s=%s\r\n%s=%d\r\n%s=%s\r\n%s=%s\r\n", \
					macroUSER_MQTT_HOST_KEY, Host.c_str(), macroUSER_MQTT_PORT_KEY, Port, \
					macroUSER_MQTT_USER_KEY, User.c_str(), macroUSER_MQTT_PWD_KEY, Pwd.c_str() \
				);
	eFileAccess_Write_All(macroUSER_FILE_MQTT_CONFIG, cStr);
//	if(eFileAccess_Write_All(macroUSER_FILE_MQTT_CONFIG, cStr) != eFileSuccess)
//		APP_DEBUG("--- UserFile: write mqtt config is successfully\r\n");
//	else
//		APP_DEBUG("--- UserFile: write mqtt config is failture\r\n");
}

















