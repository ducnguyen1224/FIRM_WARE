
#ifndef __USERFILE_H
#define __USERFILE_H



/*****************************************************************************
 * Function	: void vFileAccess_ReadWifiConfig(String SSID, String PWD)
 * Desc		: used to read wifi config from file.
 * Param	:
 * 			+ SSID: name of wifi
 * 			+ PWD: password of wifi
 * return	: None
 ****************************************************************************/
void vUserFile_Read_WifiConfig(String *SSID, String *PWD);




/*****************************************************************************
 * Function	: void vUserFile_Write_WifiConfig(String SSID, String PWD)
 * Desc		: used to write wifi config to file.
 * Param	:
 * 			+ SSID: name of wifi
 * 			+ PWD: password of wifi
 * return	: None
 ****************************************************************************/
void vUserFile_Write_WifiConfig(String SSID, String PWD);




/*****************************************************************************
 * Function	: void vUserFile_Read_MqttConfig(String *Host, unsigned int *Port, String *User, String *Pwd)
 * Desc		: used to read mqtt config from file.
 * Param	:
 * 			+ Host: host address
 * 			+ Port: port mqtt
 * return	: None
 ****************************************************************************/
void vUserFile_Read_MqttConfig(String *Host, unsigned int *Port, String *User, String *Pwd);




/*****************************************************************************
 * Function	: void vUserFile_Write_MqttConfig(String Host, unsigned int Port, String User, String Pwd)
 * Desc		: used to write mqtt config to file.
 * Param	:
 * 			+ Host: host address
 * 			+ Port: port mqtt
 * return	: None
 ****************************************************************************/
void vUserFile_Write_MqttConfig(String Host, unsigned int Port, String User, String Pwd);


#endif













