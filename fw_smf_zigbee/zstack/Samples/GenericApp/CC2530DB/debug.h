#ifndef DEBUG_H
#define DEBUG_H

#include "ZComDef.h"
#include "std_header.h"

#define UART_DEBUG	HAL_UART_PORT_0
#define DBG_PRINTF		uart_put_string
#define DBG_NPRINTF		uart_put_byte_array
#define DBG_PRINTF_VALUE	uart_put_string_value
void uart_put_string(uint8 *data);
void uart_put_byte_array(uint8 *data, uint16 len);
void uart_put_string_value( char *title, uint16 value, uint8 format);

#endif