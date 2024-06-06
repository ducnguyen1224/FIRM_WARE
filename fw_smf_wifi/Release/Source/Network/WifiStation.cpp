
#include "WifiStation.h"
#include "UserFile.h"


#define macroTIME_CHECK_CONNECT_WIFI					10


String MAC;

WifiConfig __WifiConfig =
{
	.UserName	= ":D",
	.PassWord	= "12345678@@",
};

bool bWifiConnected = false;
bool bSmartConfigIsStarted = false;
Timer T_Check_WifiStatus;

//Connection call back
void (*__ConnectionCallBack)( bool isConnected );




static void smartConfigCallback(sc_status status, void *pdata)
{
	switch (status)
	{
		case SC_STATUS_WAIT:
//			APP_DEBUG("--- Main: Smart config STATUS_WAIT\n");
			break;

		case SC_STATUS_FIND_CHANNEL:
//			APP_DEBUG("--- WifiStation: Smart config STATUS_FIND_CHANNEL\n");
			break;

		case SC_STATUS_GETTING_SSID_PSWD:
//			APP_DEBUG("--- Main: Smart config STATUS_GETTING_SSID_PSWD\n");
			break;

		case SC_STATUS_LINK:
			{
//				APP_DEBUG("--- Main: Smart config STATUS_LINK\n");
				station_config *sta_conf = (station_config *)pdata;
				__WifiConfig.UserName = (char*)sta_conf->ssid;
				__WifiConfig.PassWord = (char*)sta_conf->password;
				vUserFile_Write_WifiConfig(__WifiConfig.UserName, __WifiConfig.PassWord);
				WifiStation.config(__WifiConfig.UserName, __WifiConfig.PassWord);
				WifiStation.connect();
				T_Check_WifiStatus.start();
			}
			break;

		case SC_STATUS_LINK_OVER:
			APP_DEBUG("--- WifiStation: Smart config STATUS_LINK_OVER\n");
			WifiStation.smartConfigStop();
			bSmartConfigIsStarted = false;
			break;
	}
}




/*************************************************************************
 * Function	: vWifiStation_DeviceReady(void)
 * Desc		: Notice device wifi is Ready
 * Param	: N/A
 * return	: none
 *************************************************************************/
static void vWifiStation_DeviceReady(void)
{
	MAC = WifiStation.getMAC();
//	APP_DEBUG("\r\n--- WifiStation: READY! MAC = %s\r\n", MAC.c_str());
}




/*************************************************************************
 * Function	: vWifiStation_ConnectOK(void)
 * Desc		: Will be called when WiFi station was connected to AP
 * Param	: None
 * Return	: None
 *************************************************************************/
static void vWifiStation_ConnectOK(void)
{
	if(bWifiConnected == false)
	{
//		APP_DEBUG("--- WifiStation: CONNECTED. IP = %s\r\n", WifiStation.getIP().toString().c_str());
		__ConnectionCallBack(true);
	}
	bWifiConnected = true;
}




/*************************************************************************
 * Function	: vWifiStation_ConnectFAIL(void)
 * Desc		: Will be called when WiFi station timeout was reached
 * Param	: N/A
 * Return	: none
 *************************************************************************/
static void vWifiStation_ConnectFAIL(void)
{
	APP_DEBUG("--- WifiStation: NOT CONNECTED!\r\n");

	if(bWifiConnected == true)
		__ConnectionCallBack(false);
	bWifiConnected = false;

	WifiStation.connect();
}




/*************************************************************************
 * Function	: void vWifiStation_CheckStatus( void )
 * Desc		: check status wifi
 * Param	: None
 * Return	: None
 *************************************************************************/
static void vWifiStation_CheckStatus( void )
{
	if(WifiStation.isConnected() == true)
		vWifiStation_ConnectOK();
	else
		vWifiStation_ConnectFAIL();
}




/*************************************************************************
 * Function	: vWifiStation_Init(String WifiSSID, String WifiPwd, void (*ConnectedCallBack)( bool ))
 * Desc		: Init wifi station mode
 * Param	:
 * 			+ WifiSSID: SSID of wifi
 * 			+ WifiPwd: Password of wifi
 * Return	: none
 *************************************************************************/
void vWifiStation_Init(String WifiSSID, String WifiPwd, void (*ConnectedCallBack)( bool ))
{
//	APP_DEBUG("\r\n--- WifiStation: Wifi station Initialize\r\n");
	// Set system ready callback method
	System.onReady(vWifiStation_DeviceReady);
	
	//Enable Wifi
	WifiStation.enable(true);		//Enable Wifi Station
	//WifiStation.isEnabled();

	//Config Wifi
	WifiStation.config(WifiSSID, WifiPwd, true, true);

//	APP_DEBUG("--- WifiStation: Connecting SSID: \"%s\" pass: \"%s\"\r\n", WifiSSID.c_str(), WifiPwd.c_str());
	WifiStation.connect();

	// We recommend 20+ seconds at start
	//WifiStation.waitConnection(vWifiStation_ConnectOK, macroTIME_CHECK_CONNECT_WIFI, vWifiStation_ConnectFAIL);
	__ConnectionCallBack = ConnectedCallBack;
	T_Check_WifiStatus.initializeMs(macroTIME_CHECK_CONNECT_WIFI * 1000, vWifiStation_CheckStatus).start();
}




/*************************************************************************
 * Function	: void vWifiStation_SmartConfig( void )
 * Desc		: Smart config action
 * Param	: none
 * Return	: none
 *************************************************************************/
void vWifiStation_SmartConfig( void )
{
	if(bSmartConfigIsStarted == true)
	{
		WifiStation.smartConfigStop();
		T_Check_WifiStatus.start();
	}
	else
	{
		UART_APP_SEND("CONF,SMF_GW,SMConfig!");
		T_Check_WifiStatus.stop();
		__ConnectionCallBack(false);
		WifiStation.smartConfigStart(SCT_EspTouch, smartConfigCallback);
	}
	bSmartConfigIsStarted = ~bSmartConfigIsStarted;
}







