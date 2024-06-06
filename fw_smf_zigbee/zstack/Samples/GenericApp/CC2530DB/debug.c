#include "debug.h"
#define SOT     '*'
#define EOT     '#'
void uart_put_string(uint8 *data) {
  //uint8 c = 0;
  //c = SOT;
  //HalUARTWrite(UART_DEBUG, &c , 1);
  
  HalUARTWrite(UART_DEBUG, data, strlen((char *)data));
  //c = EOT;
  
  //HalUARTWrite(UART_DEBUG, &c, 1);

}
void uart_put_byte_array(uint8 *data, uint16 len) {
	HalUARTWrite(UART_DEBUG, data, len);	

}
void uart_put_string_value( char *title, uint16 value, uint8 format)
{
 uint8 tmpLen;
  uint8 buf[HAL_LCD_MAX_BUFF];
  uint32 err;
  tmpLen = (uint8)osal_strlen( (char*)title );
  osal_memcpy( buf, title, tmpLen );
  buf[tmpLen] = ' ';
  err = (uint32)(value);
  _ltoa( err, &buf[tmpLen+1], format );
  uart_put_string(buf);
}