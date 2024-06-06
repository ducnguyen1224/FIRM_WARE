
#ifndef __WIFISTATION_H
#define __WIFISTATION_H

#include "../main.h"


typedef struct
{
	String UserName;
	String PassWord;
}WifiConfig;


//typedef void (*ConnectionCB)( void );




/*************************************************************************
 * Function	: vWifiStation_Init(String WifiSSID, String WifiPwd, void (*ConnectionCallBack)( bool ))
 * Desc		: Init wifi station mode
 * Param	:
 * 			+ WifiSSID: SSID of wifi
 * 			+ WifiPwd: Password of wifi
 * Return	: none
 *************************************************************************/
void vWifiStation_Init(String WifiSSID, String WifiPw, void (*ConnectionCallBack)( bool ));




/*************************************************************************
 * Function	: void vWifiStation_SmartConfig( void )
 * Desc		: Smart config action
 * Param	: none
 * Return	: none
 *************************************************************************/
void vWifiStation_SmartConfig( void );


#endif








