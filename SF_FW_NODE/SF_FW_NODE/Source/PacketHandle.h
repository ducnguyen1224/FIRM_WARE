
#ifndef __PACKETHANDLE_H
#define __PACKETHANDLE_H

#include "AppData.h"

												
typedef struct
{
	char 		cTypePacket[macroPACKET_STRING_MAX_FIELD_LENGHT];

	char		cTypeDevice[macroPACKET_STRING_MAX_FIELD_LENGHT];
    
    char		cID[macroPACKET_STRING_MAX_FIELD_LENGHT];

	uint8_t		uDataNumber;
	
	Enum_Output eOutput;

	struct
	{
		char	cName[macroPACKET_STRING_MAX_FIELD_LENGHT];
		char	cInfo[macroPACKET_STRING_MAX_FIELD_LENGHT*2];
	}Data[macroPACKET_STRING_MAX_FIELD - macroPACKET_STRING_MIN_FIELD];

}PacketIO;



/******************************************************************************
 * Function		: void vPacketHandle_SendMessage( PacketIO *xPacketIO )
 * Description	: Ham gui ban tin toi connectivity
 * Param		: xPacketIO - cau truc chua du lieu de dong goi ban tin
 * Return		: none
*******************************************************************************/
void vPacketHandle_SendMessage( PacketIO *xPacketIO );




/******************************************************************************
 * Function		: PacketIO xPacketHandle_RecvMessage( char *cMessage )
 * Description	: Ham nhan va tach cac truong cua ban tin
 * Param		: 
 *				+ cMessage - ban tin nhan duoc
 * Return		: Cau truc chua thong tin cac truong tach duoc
*******************************************************************************/
PacketIO xPacketHandle_RecvMessage( char *pcMessage );



/******************************************************************************
 * Function		: void vPacketHandle_HandleMessageData( PacketIO *xPacketIO )
 * Description	: Ham xu ly ban tin data
 * Param		: xPacketIO - cau truc chua cac truong du lieu tach duoc
 * Return		: none
*******************************************************************************/
void vPacketHandle_HandleMessageData( PacketIO *xPacketIO );



/******************************************************************************
 * Function		: void vPacketHandle_HandleMessage( char *cMessage )
 * Description	: Ham xu ly ban tin sau khi tach cac truong
 * Param		:
 *				+ cMessage - ban tin nhan duoc
 * Return		: none
*******************************************************************************/
void vPacketHandle_HandleMessage( char *pcMessage );




/******************************************************************************
 * Function		: void vPacketHandle_HandleMessageConfig( PacketIO *xPacketIO )
 * Description	: Ham xu ly ban tin config
 * Param		: xPacketIO - cau truc chua cac truong du lieu tach duoc
 * Return		: none
*******************************************************************************/
void vPacketHandle_HandleMessageConfig( PacketIO *xPacketIO );




/******************************************************************************
 * Function		: void vPacketHandle_HandleMessageRequest( PacketIO *xPacketIO )
 * Description	: Ham xu ly ban tin request
 * Param		: xPacketIO - cau truc chua cac truong du lieu tach duoc
 * Return		: none
*******************************************************************************/
void vPacketHandle_HandleMessageRequest( PacketIO *xPacketIO );



/******************************************************************************
 * Function		: void vPacketHandle_HandleMessageRequest(PacketIO *xPacketIO)
 * Description	: Ham xu ly ban tin request
 * Param		: xPacketIO - cau truc chua cac truong du lieu tach duoc
 * Return		: none
*******************************************************************************/
void vPacketHandle_HandleMessageResponse( PacketIO *xPacketIO );




/******************************************************************************
 * Function		: void vPacketHandle_HandleMessageWhoAmI( void )
 * Description	: Ham xu ly ban tin Who Am I
 * Param		: 
 * Return		: none
*******************************************************************************/
void vPacketHandle_HandleMessageWhoAmI( PacketIO *xPacketIO );




/******************************************************************************
 * Function		: void vPacketHandle_HandleMessageCheck( PacketIO *xPacketIO )
 * Description	: Ham xu ly ban tin check
 * Param		: none
 * Return		: none
*******************************************************************************/
void vPacketHandle_HandleMessageCheck( PacketIO *xPacketIO );




/******************************************************************************
 * Function		: void vPacketHandle_SendResponse( char *cResponse, Enum_Output eOutput )
 * Description	: Ham gui ban tin Response
 * Param		: 
 *				+ cResponse - loai response - OK or NOK
 *				+ eOutput - toi main board hoac dust sensor
 * Return		: none
*******************************************************************************/
void vPacketHandle_SendResponse( char *cResponse, Enum_Output eOutput );





/******************************************************************************
 * Function		: void vPacketHandle_Coppy(char *Dst, char *Src, uint8_t NBCharacterBeginSplit)
 * Description	: Ham coppy 1 chuoi
 * Param		: 
*				+ Dst: chuoi dich
*				+ Src: chuoi nguon
*				+ NBCharacterBeginSplit: so ky tu bo qua tu dau
 * Return		: none
*******************************************************************************/
void vPacketHandle_Coppy(char *Dst, char *Src, uint8_t NBCharacterBeginSplit);



void vPacketHandle_SendWhoAmI(void);

#endif




