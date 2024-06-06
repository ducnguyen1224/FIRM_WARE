
#ifndef __UART_H
#define __UART_H

#include "fsl_uart.h"


#define macroUART_RX_BUFFER_LENGHT					512
#define macroUART_TX_BUFFER_LENGHT					512

// UART for Network
#define macroUART_NETWORK_BAUDRATE		          	115200
#define macroUART_NETWORK_BASE             			UART4
#define macroUART_NETWORK_CLKSRC           			UART4_CLK_SRC
#define macroUART_NETWORK_IRQn             			UART4_RX_TX_IRQn
#define macroUART_NETWORK_IRQHandler       			UART4_RX_TX_IRQHandler


// UART for connectivity
#define macroUART_CONNECTIVITY_BAUDRATE          	38400
#define macroUART_CONNECTIVITY_BASE               	UART1
#define macroUART_CONNECTIVITY_CLKSRC             	UART1_CLK_SRC
#define macroUART_CONNECTIVITY_IRQn               	UART1_RX_TX_IRQn
#define macroUART_CONNECTIVITY_IRQHandler         	UART1_RX_TX_IRQHandler



#define UART_NWK_WRITE_DATA(pData)					vUART_Write(macroUART_NETWORK_BASE, pData)
#define UART_CONN_WRITE_DATA(pData)					vUART_Write(macroUART_CONNECTIVITY_BASE, pData)




/******************************************************************************
 * Function		: void vUART_Init(UART_Type *xBase, clock_name_t xClk, uint32_t Baudrate, bool bInterrupt, IRQn_Type xUART_IRQn)
 * Description	: Ham khoi tao UART
 * Param		: 
 *				+ xBase: Port uart khoi tao
 *				+ ulClk: clock nguon cho uart
 *				+ Baudrate: toc do truyen nhan tren uart
 *				+ bInterrupt: true - enable ngat, false - disable ngat
 *				+ ulUART_IRQn: con tro ngat
 * Return		: none
*******************************************************************************/
void vUART_Init(UART_Type *xBase, clock_name_t xClk, uint32_t Baudrate, bool bInterrupt, IRQn_Type xUART_IRQn);




/******************************************************************************
 * Function		: void vUART_DeInit( UART_Type *xBase )
 * Description	: stop UART
 * Param		: xBase - uart address
 * Return		: none
*******************************************************************************/
void vUART_DeInit( UART_Type *xBase );



/******************************************************************************
 * Function		: void vUART_Write(UART_Type * UartBase, uint8_t *Buffer)
 * Description	: write data to UART
 * Param		: UartBase - uart address
 * Return		: none
*******************************************************************************/
void vUART_Write(UART_Type * UartBase, uint8_t *Buffer);


#endif



