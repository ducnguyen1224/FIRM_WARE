
#include <stdlib.h>
#include <stdio.h>

#include "PacketHandle.h"
#include "UART.h"
#include "SensorTask.h"
#include "main.h"
#include "DeviceControl.h"
#include "UserFile.h"


extern Flags xFlags;
extern SS_Value xSS_Value;

extern DataLocal xDataLocal;


/******************************************************************************
 * Function		: void vPacketHandle_SendMessage( PacketIO *xPacketIO )
 * Description	: Ham gui ban tin toi connectivity
 * Param		: xPacketIO - cau truc chua du lieu de dong goi ban tin
 * Return		: none
*******************************************************************************/
void vPacketHandle_SendMessage( PacketIO *xPacketIO )
{
	uint8_t uBuffer[macroPACKET_STRING_MAX_LENGHT + 1] = {0};
	uint16_t uiCnt = 0;
	uint8_t ui = 0;

	macroTASK_DELAY_MS(20);
	
	for(ui = 0; xPacketIO->cTypePacket[ui] > 0; ui++)
		uBuffer[uiCnt++] = xPacketIO->cTypePacket[ui];
	
	if(xPacketIO->cTypeDevice[0] > 0)
	{
		uBuffer[uiCnt++] = ',';
		for(ui = 0; xPacketIO->cTypeDevice[ui] > 0; ui++)
			uBuffer[uiCnt++] = xPacketIO->cTypeDevice[ui];
	}
		
	uBuffer[uiCnt++] = ',';
	for(ui = 0; xPacketIO->cID[ui] > 0; ui++)
		uBuffer[uiCnt++] = xPacketIO->cID[ui];
	
    //for package Data
	for(uint8_t uj = 0; uj < xPacketIO->uDataNumber; uj++)
	{
		uBuffer[uiCnt++] = ',';
		
		for(ui = 0; xPacketIO->Data[uj].cName[ui] > 0; ui++)
			uBuffer[uiCnt++] = xPacketIO->Data[uj].cName[ui];
		if(xPacketIO->Data[uj].cInfo[0] > 0)
		{
			uBuffer[uiCnt++] = ':';
			for(ui = 0; xPacketIO->Data[uj].cInfo[ui] > 0; ui++)
				uBuffer[uiCnt++] = xPacketIO->Data[uj].cInfo[ui];
		}
	}
	
	uBuffer[uiCnt++] = macroPACKET_STRING_ENDCHAR;

    //for destination
	if(xPacketIO->eOutput == eZigB)
	{
		APP_DEBUG("--- PacketHandle: Send data: \"%s\" to ZigBee\r\n", uBuffer);
		vUART_Write( macroUART_ZIGB_BASE, (char *)uBuffer, uiCnt );
	}
	return;
}




/******************************************************************************
 * Function		: PacketIO xPacketHandle_RecvMessage( char *cMessage )
 * Description	: Ham nhan va tach cac truong cua ban tin
 * Param		: 
 *				+ cMessage - ban tin nhan duoc
 * Return		: Cau truc chua thong tin cac truong tach duoc
*******************************************************************************/
PacketIO xPacketHandle_RecvMessage( char *pcMessage )
{
	PacketIO xPacketIO;
	char cContent[macroPACKET_STRING_MAX_FIELD_LENGHT] = {0};
	uint8_t uFieldNext = 0, uCharNext = 0;
	bool bCheck = true;
	bool isNextOK = false;
	
	APP_DEBUG("\r\n--- PacketHandle: Received packet = %s\r\n", pcMessage);

	memset(&xPacketIO, 0, sizeof(xPacketIO));
	memset(cContent, 0, strlen(cContent));
	while( (*pcMessage > 0) && (uFieldNext < macroPACKET_STRING_MAX_FIELD) )
	{
		if( (*pcMessage == ',') || (*pcMessage == '!') )
		{
			if(uFieldNext == 0) // Type Packet
				vPacketHandle_Coppy(xPacketIO.cTypePacket, cContent, 0);
			else if(uFieldNext == 1)    // type device
                vPacketHandle_Coppy(xPacketIO.cTypeDevice, cContent, 0);
			else if(uFieldNext == 2)    // ID end device
				vPacketHandle_Coppy(xPacketIO.cID, cContent, 0);
			else if(uFieldNext > 2)     //data
			{
				if( bCheck == true )
                    vPacketHandle_Coppy(xPacketIO.Data[xPacketIO.uDataNumber].cInfo, cContent, 0);
				else 
					vPacketHandle_Coppy(xPacketIO.Data[xPacketIO.uDataNumber].cName, cContent, 0);
				
				xPacketIO.uDataNumber++;
			}
			uFieldNext++;
			if(*pcMessage == '!')
				break;
			bCheck = false;
			isNextOK = true;
		}
		else if( (*pcMessage == ':') && (uFieldNext > 2) )  //data
		{
			vPacketHandle_Coppy(xPacketIO.Data[xPacketIO.uDataNumber].cName, cContent, 0);
			bCheck = true;
			isNextOK = true;
		}
		else
		{
			cContent[uCharNext++] = *pcMessage;
		}
		
		if(isNextOK == true)
		{
			memset(cContent, 0, strlen(cContent));
			uCharNext = 0;
			isNextOK = false;
		}
		
		if(uCharNext >= macroPACKET_STRING_MAX_FIELD_LENGHT)
		{
			APP_DEBUG("--- PacketHandle: Field Lenght of string Packet too long!\r\n");
			memset(&xPacketIO, 0, sizeof(xPacketIO));
			return xPacketIO;
		}
		pcMessage++;
	}

	APP_DEBUG("--- PacketHandle: String Type Packet    = %s\r\n", xPacketIO.cTypePacket);
	
	for(uint8_t uj = 0; uj < xPacketIO.uDataNumber; uj++)
	{
		APP_DEBUG("--- PacketHandle: String Data name [%d]  = %s\r\n", uj, xPacketIO.Data[uj].cName);
		if(xPacketIO.Data[uj].cInfo[0] > 0)
			APP_DEBUG("--- PacketHandle: String Data info [%d]  = %s\r\n", uj, xPacketIO.Data[uj].cInfo);			
	}

	return xPacketIO;
}



/******************************************************************************
 * Function		: void vPacketHandle_HandleMessage( char *cMessage )
 * Description	: Ham xu ly ban tin sau khi tach cac truong
 * Param		:
 *				+ cMessage - ban tin nhan duoc
 * Return		: none
*******************************************************************************/
void vPacketHandle_HandleMessage( char *pcMessage )
{
	PacketIO xPacketIO;
	
	memset(&xPacketIO, 0, sizeof(xPacketIO));	
    
       /*add function check ban tin */
    /*
    coding here.......
    */
    //nhan va tach cac truong cua ban tin
		
	xPacketIO = xPacketHandle_RecvMessage( pcMessage );
	
    //Ban tin WhoAmI
	if(strcmp(xPacketIO.cTypePacket, macroTYPEPACKET_WAMI) == 0)
	{
		APP_DEBUG("--- PacketHandle: Type packet is \"%s\"\r\n", macroTYPEPACKET_WAMI);
		vPacketHandle_HandleMessageWhoAmI(&xPacketIO);
		return;
	}
	//Ban tin Response
	if(strcmp(xPacketIO.cTypePacket, macroTYPEPACKET_RESP) == 0)
	{
		APP_DEBUG("--- PacketHandle: Type packet is \"%s\"\r\n", macroTYPEPACKET_RESP);
		vPacketHandle_HandleMessageResponse( &xPacketIO );
		return;
	}
	//Kiem tra cID
//	if(strcmp(xPacketIO.cID, macroCONNECTIVITY_SSB) != 0)
//	{
//		APP_DEBUG("--- PacketHandle: Error - ID Sensor Board not found!\r\n");
//		return;
//	}
	
	//Kiem tra type device
//	if(strcmp(xPacketIO.cTypeDevice, macroTYPEDEVICE_SS) != 0)
//	{
//		APP_DEBUG("--- PacketHandle: Error - Type device not found\r\n");
//		return;
//	}
	
	//Kiem tra Type packet
	//Ban tin data
	if(strcmp(xPacketIO.cTypePacket, macroTYPEPACKET_DATA) == 0)
	{
		APP_DEBUG("--- PacketHandle: Type packet is \"%s\"\r\n", macroTYPEPACKET_DATA);
//		vPacketHandle_HandleMessageData( &xPacketIO );
		return;
	}
	//Ban tin config
	else if(strcmp(xPacketIO.cTypePacket, macroTYPEPACKET_CONF) == 0)
	{
		APP_DEBUG("--- PacketHandle: Type packet is \"%s\"\r\n", macroTYPEPACKET_CONF);
		vPacketHandle_HandleMessageConfig( &xPacketIO );
		return;
	}
	//Ban tin Request
	else if(strcmp(xPacketIO.cTypePacket, macroTYPEPACKET_REQS) == 0)
	{
		APP_DEBUG("--- PacketHandle: Type packet is \"%s\"\r\n", macroTYPEPACKET_REQS);
		vPacketHandle_HandleMessageRequest( &xPacketIO );
		return;
	}
	
//	vPacketHandle_SendResponse( macroRESPONSE_NOK('s'), eZigB );
	
	APP_DEBUG("--- PacketHandle: Type packet not found\r\n");
	return;
}



/******************************************************************************
 * Function		: void vPacketHandle_HandleMessageData( PacketIO *xPacketIO )
 * Description	: Ham xu ly ban tin data
 * Param		: xPacketIO - cau truc chua cac truong du lieu tach duoc
 * Return		: none
*******************************************************************************/
void vPacketHandle_HandleMessageData( PacketIO *xPacketIO )
{
//	for(uint8_t ui = 0; ui < xPacketIO->uDataNumber; ui++)
//	{
//		//Dust sensor
//		if(strcmp(xPacketIO->Data[ui].cName, macroSENSOR_PM2_5('s')) == 0)
//		{
//			APP_DEBUG("--- PacketHandle: Data is \"%s\"\r\n", xPacketIO->Data[ui].cName);
//			
//			int32_t PM2_5_Data = atoi(xPacketIO->Data[ui].cInfo);
//			
//			if(PM2_5_Data < 0)
//				xSS_Value.SS_Dust.fPM2_5 = 0;
//			else
//				xSS_Value.SS_Dust.fPM2_5 = *(float *)&PM2_5_Data;
//			xFlags.bDustSensor = true;
//		}
//		else if(strcmp(xPacketIO->Data[ui].cName, macroSENSOR_PM10('s')) == 0)
//		{
//			APP_DEBUG("--- PacketHandle: Data is \"%s\"\r\n", xPacketIO->Data[ui].cName);
//			
//			int32_t PM10_Data = atoi(xPacketIO->Data[ui].cInfo);
//			
//			if(PM10_Data < 0)
//				xSS_Value.SS_Dust.fPM10 = 0;
//			else
//				xSS_Value.SS_Dust.fPM10 = *(float *)&PM10_Data;
//			xFlags.bDustSensor = true;
//		}
//	}
//
//	free(xPacketIO);
}



/******************************************************************************
 * Function		: void vPacketHandle_HandleMessageConfig( PacketIO *xPacketIO )
 * Description	: Ham xu ly ban tin config
 * Param		: xPacketIO - cau truc chua cac truong du lieu tach duoc
 * Return		: none
*******************************************************************************/
void vPacketHandle_HandleMessageConfig( PacketIO *xPacketIO )
{
    
  char cHighThresh[20]={0};
  char cLowThresh[20]={0};
	for(uint8_t ui = 0; ui < xPacketIO->uDataNumber; ui++)
	{
        if(strcmp(xPacketIO->Data[ui].cName, macroCFG_DataUT) == 0)
		{
			APP_DEBUG("--- PacketHandle: Config is \"%s\"\r\n", xPacketIO->Data[ui].cName);
            uint16_t ui16DataUT = atoi(xPacketIO->Data[ui].cInfo);
            if(ui16DataUT >=macroCFG_TIMESEND)
            {
              xDataLocal.uiTimeUpdate = ui16DataUT;
              //write sd card
              bUserFile_WriteFile(macroUSER_FILE_TIMESEND ,(float)xDataLocal.uiTimeUpdate, (float)-1);
              APP_DEBUG("--- PacketHandle: Time update data config  = %d\r\n",  xDataLocal.uiTimeUpdate);
            }
            else
			{
				APP_DEBUG("--- PacketHandle: Time update data config error = %s\r\n", xPacketIO->Data[ui].cInfo);
				free(xPacketIO);
				return;
			}
        }
#ifdef PH_SENSOR
        //cau hinh nguong cho senssor
		else if(strcmp(xPacketIO->Data[ui].cName, macroCFG_SENSOR_PH) == 0)
		{
            bool bLow = false;  //low thresh
            uint8_t uj=0;
			APP_DEBUG("--- PacketHandle: Config is \"%s\"\r\n", xPacketIO->Data[ui].cName);
            for(uint8_t uk = 0; xPacketIO->Data[ui].cInfo[uk] > 0; uk++)
            {
              if((xPacketIO->Data[ui].cInfo[uk]) != '/')
              {
                if(bLow == false)
                    cHighThresh[uk] = xPacketIO->Data[ui].cInfo[uk];
                else
                    cLowThresh[uj++] = xPacketIO->Data[ui].cInfo[uk];
              }
              else
                bLow = true;
            }
            if(strcmp(cHighThresh,macroCFG_NO_CHANGE_THRESH)!=0)
			{	
				//coding write to sd card
				xDataLocal.xPH.ui16HighThresh = atof(cHighThresh);
			}
            
            if(strcmp(cLowThresh,macroCFG_NO_CHANGE_THRESH)!=0)
                xDataLocal.xPH.ui16LowThresh = atof(cLowThresh);
            
            bUserFile_WriteFile(macroUSER_FILE_PH ,xDataLocal.xPH.ui16LowThresh, xDataLocal.xPH.ui16HighThresh);
            APP_DEBUG("--- PacketHandle: Config Thresh: High = %f, Low = %f \r\n", xDataLocal.xPH.ui16HighThresh, xDataLocal.xPH.ui16LowThresh);
		}
#endif

#ifdef EC_SENSOR
        else if(strcmp(xPacketIO->Data[ui].cName, macroCFG_SENSOR_EC) == 0)
		{
			bool bLow = false;
            uint8_t uj=0;
			APP_DEBUG("--- PacketHandle: Config is \"%s\"\r\n", xPacketIO->Data[ui].cName);
            for(uint8_t uk = 0; xPacketIO->Data[ui].cInfo[uk] > 0; uk++)
            {
              if((xPacketIO->Data[ui].cInfo[uk]) != '/')
              {
                if(bLow == false)
                    cHighThresh[uk] = xPacketIO->Data[ui].cInfo[uk];
                else
                    cLowThresh[uj++] = xPacketIO->Data[ui].cInfo[uk];
              }
              else
                bLow = true;
            }
            
           if(strcmp(cHighThresh,macroCFG_NO_CHANGE_THRESH)!=0)
                xDataLocal.xEC.ui16HighThresh = atof(cHighThresh);
            
            if(strcmp(cLowThresh,macroCFG_NO_CHANGE_THRESH)!=0)
                xDataLocal.xEC.ui16LowThresh = atof(cLowThresh);
            //write sd card
            bUserFile_WriteFile(macroUSER_FILE_EC ,xDataLocal.xEC.ui16LowThresh, xDataLocal.xEC.ui16HighThresh);
            APP_DEBUG("--- PacketHandle: Config Thresh: High = %f, Low = %f \r\n", xDataLocal.xEC.ui16HighThresh, xDataLocal.xEC.ui16LowThresh);
		}
#endif
		
#ifdef SHTG_SENSOR
        else if(strcmp(xPacketIO->Data[ui].cName, macroCFG_SENSOR_TEMPG) == 0)
		{
			bool bLow = false;
            uint8_t uj=0;
			APP_DEBUG("--- PacketHandle: Config is \"%s\"\r\n", xPacketIO->Data[ui].cName);
            for(uint8_t uk = 0; xPacketIO->Data[ui].cInfo[uk] > 0; uk++)
            {
              if((xPacketIO->Data[ui].cInfo[uk]) != '/')
              {
                if(bLow == false)
                    cHighThresh[uk] = xPacketIO->Data[ui].cInfo[uk];
                else
                    cLowThresh[uj++] = xPacketIO->Data[ui].cInfo[uk];
              }
              else
                bLow = true;
            }
            
            if(strcmp(cHighThresh,macroCFG_NO_CHANGE_THRESH)!=0)
                xDataLocal.xTempG.ui16HighThresh = atof(cHighThresh);
            
            if(strcmp(cLowThresh,macroCFG_NO_CHANGE_THRESH)!=0)
                xDataLocal.xTempG.ui16LowThresh = atof(cLowThresh);
            
            //write sd card
            bUserFile_WriteFile(macroUSER_FILE_TEMPG ,xDataLocal.xTempG.ui16LowThresh, xDataLocal.xTempG.ui16HighThresh);
            APP_DEBUG("--- PacketHandle: Config Thresh: High = %f, Low = %f \r\n", xDataLocal.xTempG.ui16HighThresh, xDataLocal.xTempG.ui16LowThresh);
		}
        else if(strcmp(xPacketIO->Data[ui].cName, macroCFG_SENSOR_HUMIG) == 0)
		{
			bool bLow = false;
            uint8_t uj=0;
			APP_DEBUG("--- PacketHandle: Config is \"%s\"\r\n", xPacketIO->Data[ui].cName);
            for(uint8_t uk = 0; xPacketIO->Data[ui].cInfo[uk] > 0; uk++)
            {
              if((xPacketIO->Data[ui].cInfo[uk]) != '/')
              {
                if(bLow == false)
                    cHighThresh[uk] = xPacketIO->Data[ui].cInfo[uk];
                else
                    cLowThresh[uj++] = xPacketIO->Data[ui].cInfo[uk];
              }
              else
                bLow = true;
            }
            
          if(strcmp(cHighThresh,macroCFG_NO_CHANGE_THRESH)!=0)
                xDataLocal.xHumiG.ui16HighThresh = atof(cHighThresh);
            
          if(strcmp(cLowThresh,macroCFG_NO_CHANGE_THRESH)!=0)
                xDataLocal.xHumiG.ui16LowThresh = atof(cLowThresh);
           //write sd card
          bUserFile_WriteFile(macroUSER_FILE_HUMIG ,xDataLocal.xHumiG.ui16LowThresh, xDataLocal.xHumiG.ui16HighThresh);
         
          APP_DEBUG("--- PacketHandle: Config Thresh: High = %f, Low = %f \r\n", xDataLocal.xHumiG.ui16HighThresh, xDataLocal.xHumiG.ui16LowThresh);
		}
#endif
		
#ifdef SHTA_SENSOR
        else if(strcmp(xPacketIO->Data[ui].cName, macroCFG_SENSOR_TEMPA) == 0)
		{
			bool bLow = false;
            uint8_t uj=0;
			APP_DEBUG("--- PacketHandle: Config is \"%s\"\r\n", xPacketIO->Data[ui].cName);
            for(uint8_t uk = 0; xPacketIO->Data[ui].cInfo[uk] > 0; uk++)
            {
              if((xPacketIO->Data[ui].cInfo[uk]) != '/')
              {
                if(bLow == false)
                    cHighThresh[uk] = xPacketIO->Data[ui].cInfo[uk];
                else
                    cLowThresh[uj++] = xPacketIO->Data[ui].cInfo[uk];
              }
              else
                bLow = true;
            }
            
            if(strcmp(cHighThresh,macroCFG_NO_CHANGE_THRESH)!=0)
                xDataLocal.xTempA.ui16HighThresh = atof(cHighThresh);
            
            if(strcmp(cLowThresh,macroCFG_NO_CHANGE_THRESH)!=0)
                xDataLocal.xTempA.ui16LowThresh = atof(cLowThresh);
               //write sd card
          bUserFile_WriteFile(macroUSER_FILE_TEMPA ,xDataLocal.xTempA.ui16LowThresh, xDataLocal.xTempA.ui16HighThresh);
          
            APP_DEBUG("--- PacketHandle: Config Thresh: High = %f, Low = %f \r\n", xDataLocal.xTempA.ui16HighThresh, xDataLocal.xTempA.ui16LowThresh);
		}
        else if(strcmp(xPacketIO->Data[ui].cName, macroCFG_SENSOR_HUMIA) == 0)
		{
			bool bLow = false;
            uint8_t uj=0;
			APP_DEBUG("--- PacketHandle: Config is \"%s\"\r\n", xPacketIO->Data[ui].cName);
            for(uint8_t uk = 0; xPacketIO->Data[ui].cInfo[uk] > 0; uk++)
            {
              if((xPacketIO->Data[ui].cInfo[uk]) != '/')
              {
                if(bLow == false)
                    cHighThresh[uk] = xPacketIO->Data[ui].cInfo[uk];
                else
                    cLowThresh[uj++] = xPacketIO->Data[ui].cInfo[uk];
              }
              else
                bLow = true;
            }
            
            if(strcmp(cHighThresh,macroCFG_NO_CHANGE_THRESH)!=0)
                xDataLocal.xHumiA.ui16HighThresh = atof(cHighThresh);
            
            if(strcmp(cLowThresh,macroCFG_NO_CHANGE_THRESH)!=0)
                xDataLocal.xHumiA.ui16LowThresh = atof(cLowThresh);
                  //write sd card
            bUserFile_WriteFile(macroUSER_FILE_HUMIA ,xDataLocal.xHumiA.ui16LowThresh, xDataLocal.xHumiA.ui16HighThresh);
            
            APP_DEBUG("--- PacketHandle: Config Thresh: High = %f, Low = %f \r\n", xDataLocal.xHumiA.ui16HighThresh, xDataLocal.xHumiA.ui16LowThresh);
		}
#endif
		
#ifdef LIGHT_SENSOR
        else if(strcmp(xPacketIO->Data[ui].cName, macroCFG_SENSOR_LIGHT) == 0)
		{
			bool bLow = false;
            uint8_t uj=0;
			APP_DEBUG("--- PacketHandle: Config is \"%s\"\r\n", xPacketIO->Data[ui].cName);
            for(uint8_t uk = 0; xPacketIO->Data[ui].cInfo[uk] > 0; uk++)
            {
              if((xPacketIO->Data[ui].cInfo[uk]) != '/')
              {
                if(bLow == false)
                    cHighThresh[uk] = xPacketIO->Data[ui].cInfo[uk];
                else
                    cLowThresh[uj++] = xPacketIO->Data[ui].cInfo[uk];
              }
              else
                bLow = true;
            }
            
            if(strcmp(cHighThresh,macroCFG_NO_CHANGE_THRESH)!=0)
                xDataLocal.xLight.ui16HighThresh = atof(cHighThresh);
            
            if(strcmp(cLowThresh,macroCFG_NO_CHANGE_THRESH)!=0)
                xDataLocal.xLight.ui16LowThresh = atof(cLowThresh);
                  //write sd card
            bUserFile_WriteFile(macroUSER_FILE_LIGHT ,xDataLocal.xLight.ui16LowThresh, xDataLocal.xLight.ui16HighThresh);
            
            APP_DEBUG("--- PacketHandle: Config Thresh: High = %f, Low = %f \r\n", xDataLocal.xLight.ui16HighThresh, xDataLocal.xLight.ui16LowThresh);
		}
#endif
        else
        {
            APP_DEBUG("--- PacketHandle: Config \"%s\" not found\r\n", xPacketIO->Data[ui].cName);
        }
	}
    
	free(xPacketIO);
}




/******************************************************************************
 * Function		: void vPacketHandle_HandleMessageRequest( PacketIO *xPacketIO )
 * Description	: Ham xu ly ban tin request
 * Param		: xPacketIO - cau truc chua cac truong du lieu tach duoc
 * Return		: none
*******************************************************************************/
void vPacketHandle_HandleMessageRequest( PacketIO *xPacketIO )
{
	for(uint8_t ui = 0; ui < xPacketIO->uDataNumber; ui++)
	{
		APP_DEBUG("--- PacketHandle: Request is \"%s\"\r\n", xPacketIO->Data[ui].cName);
		
		//Yeu cau du lieu cua tat ca Sensor
		if(strcmp(xPacketIO->Data[ui].cName, macroREQUEST_ALL) == 0)
		{
			xFlags.eReadSensor = eALL;
            xFlags.bSendReqs = true;
			break;
		}
//#ifdef PH_SENSOR
//		//Yeu cau du lieu cua pH
//		else if(strcmp(xPacketIO->Data[ui].cName, macroSENSOR_PH) == 0)
//		{
//			xFlags.eReadSensor = ePH;
//			break;
//		}
//#endif
//
//#ifdef EC_SENSOR
//		//Yeu cau du lieu cua EC Sensor
//		else if(strcmp(xPacketIO->Data[ui].cName, macroSENSOR_EC) == 0)
//		{
//			xFlags.eReadSensor = eEC;
//			break;
//		}
//#endif
//
//#ifdef SHTG_SENSOR
//		//Yeu cau du lieu cua Pressure Sensor
//		else if(strcmp(xPacketIO->Data[ui].cName, macroSENSOR_TEMPG) == 0)
//		{
//			xFlags.eReadSensor = eTempG;
//			break;
//		}
//		//Yeu cau du lieu cua CO Sensor
//		else if(strcmp(xPacketIO->Data[ui].cName, macroSENSOR_HUMIG) == 0)
//		{
//			xFlags.eReadSensor = eHumiG;
//			break;
//		}
//#endif
//		
//#ifdef SHTA_SENSOR
//		//Yeu cau du lieu cua NO Sensor
//		else if(strcmp(xPacketIO->Data[ui].cName, macroSENSOR_TEMPA) == 0)
//		{
//			xFlags.eReadSensor = eTempA;
//			break;
//		}
//		//Yeu cau du lieu cua NO2 Sensor
//		else if(strcmp(xPacketIO->Data[ui].cName, macroSENSOR_HUMIA) == 0)
//		{
//			xFlags.eReadSensor = eHumiA;
//			break;
//		}
//#endif
//		
//#ifdef LIGHT_SENSOR
//		//Yeu cau du lieu cua SO2 Sensor
//		else if(strcmp(xPacketIO->Data[ui].cName, macroSENSOR_LIGHT) == 0)
//		{
//			xFlags.eReadSensor = eLight;
//			break;
//		}
//#endif
		else 
		{
			APP_DEBUG("--- PacketHandle: Request \"%s\" not found\r\n", xPacketIO->Data[ui].cName);
		}
	}
	
    //for msg response
//	if(xFlags.eReadSensor != eNone)
//	{
//		vPacketHandle_SendResponse( macroRESPONSE_OK('s'), eZigB );
//	}
//	else
//	{
//		vPacketHandle_SendResponse( macroRESPONSE_NOK('s'), eZigB );
//	}
	free(xPacketIO);
}




/******************************************************************************
 * Function		: void vPacketHandle_HandleMessageResponse(PacketIO *xPacketIO)
 * Description	: Ham xu ly ban tin response
 * Param		: xPacketIO - cau truc chua cac truong du lieu tach duoc
 * Return		: none
*******************************************************************************/
void vPacketHandle_HandleMessageResponse( PacketIO *xPacketIO )
{
	/*ban tin response co 3 truong: type packet, type device: GW, ID endevice: OK/NOK*/
	if( ( strcmp(xPacketIO->cTypeDevice, macroTYPEDEVICE_GW) == 0 )&&
		( strcmp(xPacketIO->cID, macroRESPONSE_OK) == 0))
	{
		APP_DEBUG("--- PacketHandle: Response is \"%s\"\r\n", xPacketIO->cID);
		xFlags.eSentIsOK = eSend_OK;
	}
	else
		xFlags.eSentIsOK = eSend_NOK;
	
	free(xPacketIO);
}




/******************************************************************************
 * Function		: void vPacketHandle_HandleMessageWhoAmI( void )
 * Description	: Ham xu ly ban tin Who Am I
 * Param		: none
 * Return		: none
*******************************************************************************/
void vPacketHandle_HandleMessageWhoAmI( PacketIO *xPacketIO )
{
	if( strcmp(xPacketIO->cTypeDevice, macroCONNECTIVITY_SSB) == 0 )
	{
		xFlags.bZigbIsConnected = true;
	}
	else
	{
		APP_DEBUG("--- PacketHandle: ZigB is disconnected\r\n");
		xFlags.bZigbIsConnected = false;
	}
		
	
	free(xPacketIO);
}

/******************************************************************************
 * Function		: void vPacketHandle_SendWhoAmI( uint8_t uWhoAmI )
 * Description	: Ham gui ban tin who am i de kiem tra connectivity co hoat dong khong
 * Param		: 
 *				+ eWhoAmI - Loai connectivity
 * Return		: none
*******************************************************************************/
void vPacketHandle_SendWhoAmI(void)
{
	uint8_t uBuffer[6] = "WAMI!";
	APP_DEBUG("--- PacketHandle: Send data: \"%s\" to ZigBee\r\n", uBuffer);
	vUART_Write( macroUART_ZIGB_BASE, (char *)uBuffer, strlen((char*)uBuffer) );
}


/******************************************************************************
 * Function		: void vPacketHandle_HandleMessageCheck( PacketIO *xPacketIO )
 * Description	: Ham xu ly ban tin check
 * Param		: none
 * Return		: none
*******************************************************************************/
void vPacketHandle_HandleMessageCheck( PacketIO *xPacketIO )
{		
}




/******************************************************************************
 * Function		: void vPacketHandle_SendResponse( char *cResponse, Enum_Output eOutput )
 * Description	: Ham gui ban tin Response
 * Param		: 
 *				+ cResponse - loai response - OK or NOK
 *				+ eOutput - toi main board hoac dust sensor
 * Return		: none
*******************************************************************************/
void vPacketHandle_SendResponse( char *cResponse, Enum_Output eOutput )
{
//	PacketIO xPacketIO;
//	
//	memset(&xPacketIO, 0, sizeof(xPacketIO));
//	
//	vPacketHandle_Coppy(xPacketIO.cTypePacket, macroTYPEPACKET_RESP('s'), 0);
//	vPacketHandle_Coppy(xPacketIO.cID, macroCONNECTIVITY_SSB('s'), 0);
//	vPacketHandle_Coppy(xPacketIO.cTypeDevice, macroAPPDATA_TYPEDEVICE, 0);
//	xPacketIO.uDataNumber = xPacketIO.uFieldNumber = 1;
//	vPacketHandle_Coppy(xPacketIO.Data[0].cName, cResponse, 0);
//	memset(xPacketIO.Data[0].cInfo, 0, strlen(xPacketIO.Data[0].cInfo));
//	
//	xPacketIO.eOutput = eOutput;
//	
//	vPacketHandle_SendMessage( &xPacketIO );
}




/******************************************************************************
 * Function		: void vPacketHandle_Coppy(char *Dst, char *Src, uint8_t NBCharacterBeginSplit)
 * Description	: Ham coppy 1 chuoi
 * Param		: 
*				+ Dst: chuoi dich
*				+ Src: chuoi nguon
*				+ NBCharacterBeginSplit: so ky tu bo qua tu dau
 * Return		: none
*******************************************************************************/
void vPacketHandle_Coppy(char *Dst, char *Src, uint8_t NBCharacterBeginSplit)
{
	memset(Dst, 0, strlen((char *)Dst));
    Src += NBCharacterBeginSplit;
    while(*Src > 0)
    {
        *Dst++ = *Src++;
    }
}











