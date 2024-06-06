
#include <stdarg.h>
#include <hw_ioc.h>

#include "Main.h"
#include "UserUART.h"

//for debug
#define macroUART_DEBUG_SYS_CTRL_PERIH			SYS_CTRL_PERIPH_UART0
#define macroUART_DEBUG_CLOCK					UART_CLOCK_SYSTEM
#define macroUART_DEBUG_IOC_RX					IOC_UARTRXD_UART0
#define macroUART_DEBUG_IOC_MUX_TX				IOC_MUX_OUT_SEL_UART0_TXD
#define macroUART_DEBUG_IOC_PORT				GPIO_D_BASE
#define macroUART_DEBUG_RX_PIN					GPIO_PIN_3
#define macroUART_DEBUG_TX_PIN					GPIO_PIN_4

//UART App
#define macroUART_APP_SYS_CTRL_PERIH			SYS_CTRL_PERIPH_UART1
#define macroUART_APP_CLOCK						UART_CLOCK_SYSTEM
#define macroUART_APP_IOC_RX					IOC_UARTRXD_UART1
#define macroUART_APP_IOC_MUX_TX				IOC_MUX_OUT_SEL_UART1_TXD
#define macroUART_APP_INT						INT_UART1
#define macroUART_APP_IOC_PORT					GPIO_B_BASE
#define macroUART_APP_RX_PIN					GPIO_PIN_1
#define macroUART_APP_TX_PIN					GPIO_PIN_2


extern uint8 uMain_TaskID; 



char UART_APP_RX_Buffer[macroUART_RX_BUF_SIZE] = {0};
char UART_APP_TX_Buffer[macroUART_TX_BUF_SIZE] = {0};




static void vUART_App_Recv_Callback( void )
{
	uint16 UART_APP_Lenght = 0;// = strlen(UART_APP_RX_Buffer);
	memset(UART_APP_RX_Buffer, 0, strlen(UART_APP_RX_Buffer));
	
	uint32_t ui32Status = UARTIntStatus(macroUART_APP_BASE, 1);
	UARTIntClear(macroUART_APP_BASE, ui32Status);//(ui32IntBm & BSP_UART_INT_BM));
	
	while (1)
	{
		uint32 uChar = UARTCharGet(macroUART_APP_BASE);
		if((char)uChar == NULL)
		{
			memset(UART_APP_RX_Buffer, 0, strlen(UART_APP_RX_Buffer));
			break;
		}
		else
			UART_APP_RX_Buffer[UART_APP_Lenght++] = (char)uChar;
		
		if(UART_APP_Lenght >= macroUART_RX_BUF_SIZE)
		{
			memset(UART_APP_RX_Buffer, 0, strlen(UART_APP_RX_Buffer));
			UART_APP_Lenght = 0;
		}
		else if((char)uChar == '!')
		{
			APP_DEBUG("\r\n--- UserUART: Recv = %s\r\n", UART_APP_RX_Buffer);
			break;
		}
	}
	
	UARTIntClear(macroUART_APP_BASE, UART_INT_RX | UART_INT_RT | UART_INT_TX | UART_INT_CTS);
}




/*******************************************************************************
* Function		: void vUART_Init(uint32 UART_Base, uint32 ulBaudrate)
* Description	: initialize uart
* Param			: None
* Return		: None
*******************************************************************************/
void vUART_Init(uint32 UART_Base, uint32 ulBaudrate)
{
	

	if(UART_Base == UART0_BASE)
	{
		SysCtrlPeripheralEnable(macroUART_DEBUG_SYS_CTRL_PERIH);	
		// Disable UART function
		UARTDisable(macroUART_DEBUG_BASE);
		// Disable all UART module interrupts
		UARTIntDisable(macroUART_DEBUG_BASE, 0x1FFF);
		// Set IO clock as UART clock source
    	UARTClockSourceSet(macroUART_DEBUG_BASE, macroUART_DEBUG_CLOCK);
		
		IOCPinConfigPeriphInput(macroUART_DEBUG_IOC_PORT, macroUART_DEBUG_RX_PIN, macroUART_DEBUG_IOC_RX);
		IOCPinConfigPeriphOutput(macroUART_DEBUG_IOC_PORT, macroUART_DEBUG_TX_PIN, macroUART_DEBUG_IOC_MUX_TX);
		GPIOPinTypeUARTOutput(macroUART_DEBUG_IOC_PORT, macroUART_DEBUG_TX_PIN);
		GPIOPinTypeUARTInput(macroUART_DEBUG_IOC_PORT, macroUART_DEBUG_RX_PIN);
	}
	else if(UART_Base == UART1_BASE)
	{
		SysCtrlPeripheralEnable(macroUART_APP_SYS_CTRL_PERIH);
		// Disable UART function
		UARTDisable(macroUART_APP_BASE);
		// Disable all UART module interrupts
		UARTIntDisable(macroUART_APP_BASE, 0x1FFF);
		// Set IO clock as UART clock source
   		UARTClockSourceSet(macroUART_APP_BASE, macroUART_APP_CLOCK);
		
		IOCPinConfigPeriphInput(macroUART_APP_IOC_PORT, macroUART_APP_RX_PIN, macroUART_APP_IOC_RX);
		IOCPinConfigPeriphOutput(macroUART_APP_IOC_PORT, macroUART_APP_TX_PIN, macroUART_APP_IOC_MUX_TX);
		GPIOPinTypeUARTOutput(macroUART_APP_IOC_PORT, macroUART_APP_TX_PIN);
		GPIOPinTypeUARTInput(macroUART_APP_IOC_PORT, macroUART_APP_RX_PIN);
	}
	
	// Configure the UART for 115,200, 8-N-1 operation.
    // This function uses SysCtrlClockGet() to get the system clock
    // frequency.  This could be also be a variable or hard coded value
    // instead of a function call.
    UARTConfigSetExpClk(UART_Base, SysCtrlClockGet(), ulBaudrate, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
	
	if(UART_Base == UART1_BASE)
	{
		UARTIntClear(macroUART_APP_BASE, 0x17FF);	
		UARTIntRegister(macroUART_APP_BASE, vUART_App_Recv_Callback);
		UARTIntEnable(macroUART_APP_BASE, UART_INT_RX | UART_INT_RT);
		IntEnable(macroUART_APP_INT);
	}
	UARTEnable(UART_Base);
}




/*******************************************************************************
* Function		: void vUART_Debug( const char *Format, ... )
* Description	: UART app write data
* Param			: None
* Return		: None
*******************************************************************************/
static void vUART_Debug_Write(char *Buffer)
{
	uint16 ui = 0;
	while(ui <= strlen(Buffer))
	{
		UARTCharPut(macroUART_DEBUG_BASE, Buffer[ui]);
		ui++;
	}
}





/*******************************************************************************
* Function		: void vUART_Debug( const char *Format, ... )
* Description	: UART debug write data
* Param			: None
* Return		: None
*******************************************************************************/
void vUART_App_Write(char *Buffer)
{
	uint16 ui = 0;
	while(ui <= strlen(Buffer))
	{
		UARTCharPut(macroUART_APP_BASE, Buffer[ui]);
		ui++;
	}
}




/*******************************************************************************
* Function		: void vUART_Debug( const char *Format, ... )
* Description	: UART debug write data
* Param			: None
* Return		: None
*******************************************************************************/
void vUART_Debug( const char *Format, ... )
{
	va_list args;
	va_start(args, Format);
	vPrintf_To_Host(vUART_Debug_Write, Format, args);
	va_end(args);
}




