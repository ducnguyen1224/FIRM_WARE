

#ifndef __CC2538SERIAL_H
#define __CC2538SERIAL_H

#include "Printf.h"


#define macroUART_RX_BUF_SIZE       			512
#define macroUART_TX_BUF_SIZE       			macroPRINTF_BUFFER_LENGHT


//for debug
#define macroUART_DEBUG_BASE					UART0_BASE
//UART App
#define macroUART_APP_BASE						UART1_BASE


//UART App
#define UART_APP_SEND_MESSAGE				vUART_App_Write


/************************************************************************************
 * For Debug
************************************************************************************/
#define macroUSE_UART_DEBUG

#ifdef macroUSE_UART_DEBUG
#define APP_DEBUG								vUART_Debug
#else
#define APP_DEBUG(FORMAT,...)
#endif



/*******************************************************************************
* Function		: void vUART_Init(uint32 UART_Base, uint32 ulBaudrate)
* Description	: initialize uart
* Param			: None
* Return		: None
*******************************************************************************/
void vUART_Init(uint32 UART_Base, uint32 ulBaudrate);



/*******************************************************************************
* Function		: void vUART_Debug( const char *Format, ... )
* Description	: UART debug write data
* Param			: None
* Return		: None
*******************************************************************************/
void vUART_App_Write(char *Buffer);




/*******************************************************************************
* Function		: void vUART_Debug( const char *Format, ... )
* Description	: UART debug write data
* Param			: None
* Return		: None
*******************************************************************************/
void vUART_Debug( const char *Format, ... );



#endif 




