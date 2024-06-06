
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "UART.h"


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
void vUART_Init(UART_Type *xBase, clock_name_t xClk, uint32_t Baudrate, bool bInterrupt, IRQn_Type xUART_IRQn)
{
	uart_config_t xUartConfig;
	
	UART_GetDefaultConfig(&xUartConfig);
    xUartConfig.baudRate_Bps = Baudrate;
    xUartConfig.enableTx = true;
    xUartConfig.enableRx = true;
    
    UART_Init(xBase, &xUartConfig, CLOCK_GetFreq(xClk));
	
	if(bInterrupt == true)
	{
		/* Enable RX interrupt. */
		NVIC_SetPriority(xUART_IRQn, 2);
		UART_EnableInterrupts(xBase, kUART_RxDataRegFullInterruptEnable | kUART_RxOverrunInterruptEnable);
		EnableIRQ(xUART_IRQn);
	}
}




/******************************************************************************
 * Function		: void vUART_DeInit( UART_Type *xBase )
 * Description	: stop UART
 * Param		: xBase - uart address
 * Return		: none
*******************************************************************************/
void vUART_DeInit( UART_Type *xBase )
{
	UART_Deinit( xBase );
}
	 

	
/******************************************************************************
 * Function		: void vUART_Write( UART_Type *xBase, char *uBuffer, uint16_t uiLenght )
 * Description	: ham gui du lieu qua UART
 * Param		: 
 *				+ xBase - uart address
 *				+ uBuffer - Data gui di
 *				+ uiLenght - do dai du lieu gui
 * Return		: none
*******************************************************************************/
void vUART_Write( UART_Type *xBase, char *uBuffer, uint16_t uiLenght )
{
	UART_WriteBlocking( xBase, (uint8_t const *)uBuffer, uiLenght );
}









