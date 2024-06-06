
#include "UserUART.h"


#define macroUART_RX_BUFFER_LENGHT				512


String UART_RX_Buffer;



/*static void UserUART_ConfigUART1( void )
{
	UartDevice uart;

	uart.baut_rate = BIT_RATE_115200;
	uart.parity = NONE_BITS;
	uart.exist_parity = STICK_PARITY_DIS;
	uart.stop_bits = ONE_STOP_BIT;
	uart.data_bits = EIGHT_BITS;

    PIN_PULLUP_DIS(PERIPHS_IO_MUX_GPIO2_U);
    PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO2_U, FUNC_U1TXD_BK);

    uart_div_modify(UART_ID_1, UART_CLK_FREQ / (uart.baut_rate));

    WRITE_PERI_REG(UART_CONF0(UART_ID_1), uart.exist_parity
                   | uart.parity
                   | (uart.stop_bits << UART_STOP_BIT_NUM_S)
                   | (uart.data_bits << UART_BIT_NUM_S));

    //clear rx and tx fifo,not ready
    SET_PERI_REG_MASK(UART_CONF0(UART_ID_1), UART_RXFIFO_RST | UART_TXFIFO_RST);
    CLEAR_PERI_REG_MASK(UART_CONF0(UART_ID_1), UART_RXFIFO_RST | UART_TXFIFO_RST);

    //clear all interrupt
    WRITE_PERI_REG(UART_INT_CLR(UART_ID_1), 0xffff);
}



static void uart1_tx_one_char(char TxChar)
{
    while (true)
    {
        uint32 fifo_cnt = READ_PERI_REG(UART_STATUS(UART_ID_1)) & (UART_TXFIFO_CNT << UART_TXFIFO_CNT_S);

        if ((fifo_cnt >> UART_TXFIFO_CNT_S & UART_TXFIFO_CNT) < 126)
        {
            break;
        }
    }

    WRITE_PERI_REG(UART_FIFO(UART_ID_1) , TxChar);
}*/





/*******************************************************************************
 * Function		: void vUART_OnData(Stream& stream, char cArrivedChar, uint16_t availableCharsCount)
 * Description	: On Data receive
 * Param		:
 * 				+ stream: uart data stream
 * 				+ cArrivedChar: character received
 * 				+ availableCharsCount: byte number of data
 * Return		: None
 ******************************************************************************/
static void vUART_OnData(Stream& stream, char cArrivedChar, uint16_t availableCharsCount)
{
	if (cArrivedChar == '!') // Lets show data!
	{
		UART_RX_Buffer = 0;

		while (stream.available())
		{
			char c = stream.read();
			UART_RX_Buffer += c;

			if(UART_RX_Buffer.length() >= macroUART_RX_BUFFER_LENGHT)
				UART_RX_Buffer = 0;
		}

		if(UART_RX_Buffer.length() > 0)
		{
			APP_DEBUG("\r\n--- UART: UART Recv data = \"%s\"\r\n", UART_RX_Buffer.c_str());
			vMain_setEvent(EVENT_STC_RECEIVED);
		}
	}
}




/*******************************************************************************
 * Function		: void vUART_Init(void)
 * Description	: Init uart
 * Param		: None
 * Return		: None
 ******************************************************************************/
void vUART_Init(void)
{
	//Initialize Serial
	Serial.begin(BIT_RATE_115200);
//	PIN_PULLUP_DIS(PERIPHS_IO_MUX_U0TXD_U);
//	PIN_PULLUP_DIS(PERIPHS_IO_MUX_U0RXD_U);
//	PIN_PULLUP_DIS(PERIPHS_IO_MUX_MTCK_U);
//	PIN_PULLUP_DIS(PERIPHS_IO_MUX_MTDO_U);
//	PIN_FUNC_SELECT(PERIPHS_IO_MUX_U0RXD_U, FUNC_GPIO13);
//	PIN_FUNC_SELECT(PERIPHS_IO_MUX_U0TXD_U, FUNC_GPIO15);
//	SET_PERI_REG_MASK(0x3ff00028 , BIT2);//SWAP PIN : U0TXD<==>U0RTS(MTDO) , U0RXD<==>U0CTS(MTCK)


#ifdef UART_DEBUG
	system_set_os_print(1);
	Serial.systemDebugOutput(true); // Allow debug print to serial
#else
	system_set_os_print(0);
	Serial.systemDebugOutput(false); // Allow debug print to serial
#endif

	Serial.setCallback(vUART_OnData);
}





















