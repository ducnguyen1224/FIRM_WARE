
#ifndef __ZIGBEEIO_H
#define __ZIGBEEIO_H

#include "Main.h"


#define macroZIGBEE_TX_BUFF_SIZE				256
#define macroZIGBEE_RX_BUFF_SIZE				256



/*****************************************************************************************
 * Zigbee Data Send
*****************************************************************************************/
uint8 uZigbeeIO_SendMessage(afAddrType_t xAddrDst, uint16 uiClusterID, uint8 *pData);





/*****************************************************************************************
 * Zigbee Data Receive
*****************************************************************************************/
uint8 uZigbeeIO_RecvMessage(afIncomingMSGPacket_t *xDataStructRecv);




/*********************************************************************
 * @fn      vZigbeeIO_ProcessZDOMsgs()
 *
 * @brief   Process response messages
 *
 * @param   none
 *
 * @return  none
 */
void vZigbeeIO_ProcessZDOMsgs(zdoIncomingMsg_t *inMsg);




/******************************************************************************
 * @fn      vZigbeeIO_GetIEEEAdress
 *
 * @brief   Get the IEEE address (MSB to LSB).
 *
 * @param   none
 *
 * @return  none
*/
void vZigbeeIO_GetIEEEAdress(uint8 *ucAddrIEEE, ZLongAddr_t ucAddrExt);




/*******************************************************************************
* Function		: uint16 uiZigbeeIO_ShortAddr_From_IEEEAddr(uint8 *uAddrIEEE, uint8 *uLongAddr)
* Description	: Get Short address of End Device from IEEE address
* Param			: 
*				+ uAddrIEEE - IEEE address
*				+ uLongAddr - Long address
* Return		: Short Address
*******************************************************************************/
uint16 uiZigbeeIO_ShortAddr_From_IEEEAddr(uint8 *uAddrIEEE, uint8 *uLongAddr);

#endif












