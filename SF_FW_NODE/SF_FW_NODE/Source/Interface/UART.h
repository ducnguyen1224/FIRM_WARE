
#ifndef __UART_H
#define __UART_H

#include "fsl_uart.h"


#define macroUART_MAX_LENGHT						512

/* UART4 to Zigbee */
#define macroUART_ZIGB_BAUDRATE          	        		38400
#define macroUART_ZIGB_BASE               					UART4
#define macroUART_ZIGB_CLKSRC             					UART4_CLK_SRC
#define macroUART_ZIGB_IRQn               					UART4_RX_TX_IRQn
#define macroUART_ZIGB_IRQHandler         					UART4_RX_TX_IRQHandler

/////* UART0 to EC2 sensor */
#define macroUART_EC2_SENSOR_BAUDRATE          	        		9600
#define macroUART_EC2_SENSOR_BASE               				UART0
#define macroUART_EC2_SENSOR_CLKSRC             				UART0_CLK_SRC
#define macroUART_EC2_SENSOR_IRQn               				UART0_RX_TX_IRQn
#define macroUART_EC2_SENSOR_IRQHandler         				UART0_RX_TX_IRQHandler

///* UART3 to EC sensor */
#define macroUART_EC_SENSOR_BAUDRATE          	        		9600
#define macroUART_EC_SENSOR_BASE               					UART3
#define macroUART_EC_SENSOR_CLKSRC             					UART3_CLK_SRC
#define macroUART_EC_SENSOR_IRQn               					UART3_RX_TX_IRQn
#define macroUART_EC_SENSOR_IRQHandler         					UART3_RX_TX_IRQHandler

/* UART1 to pH sensor */
#define macroUART_PH_SENSOR_BAUDRATE          	        		9600
#define macroUART_PH_SENSOR_BASE               					UART1
#define macroUART_PH_SENSOR_CLKSRC             					UART1_CLK_SRC
#define macroUART_PH_SENSOR_IRQn               					UART1_RX_TX_IRQn
#define macroUART_PH_SENSOR_IRQHandler         					UART1_RX_TX_IRQHandler

/* UART2 to pH 2 sensor */
#define macroUART_PH2_SENSOR_BAUDRATE          	        		9600
#define macroUART_PH2_SENSOR_BASE               				UART2
#define macroUART_PH2_SENSOR_CLKSRC             				UART2_CLK_SRC
#define macroUART_PH2_SENSOR_IRQn               				UART2_RX_TX_IRQn
#define macroUART_PH2_SENSOR_IRQHandler         				UART2_RX_TX_IRQHandler




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
 * Function		: void vUART_Write( UART_Type *xBase, char *uBuffer, uint16_t uiLenght )
 * Description	: ham gui du lieu qua UART
 * Param		: 
 *				+ xBase - uart address
 *				+ uBuffer - Data gui di
 *				+ uiLenght - do dai du lieu gui
 * Return		: none
*******************************************************************************/
void vUART_Write( UART_Type *xBase, char *uBuffer, uint16_t uiLenght );


#endif



