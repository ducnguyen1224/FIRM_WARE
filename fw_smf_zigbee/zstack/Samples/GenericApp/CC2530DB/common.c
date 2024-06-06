#include "common.h"

static void delay_1us(void);
static void delay_1ms(void);
static void delay_1us(void) {
  uint32 i = 0;
  for (i = 0; i < 10; i++);
}
static void delay_1ms(void){
  uint32 i = 0;
  for (i = 0; i < 400; i++) {
    delay_1us();
  }
}
void delay_us(uint32 time) {
  uint32 i = 0;
  for (i = 0; i < time; i++) {
    delay_1us();
  }
}

void delay_ms(uint32 time) {
  uint32 i = 0;
  for (i = 0; i < time; i++) {
    delay_1ms();
  }
}
void get_ieee_string(char *lcd_buf) {
  uint8 *xad;
 // uint8 lcd_buf[Z_EXTADDR_LEN*2+1];
  uint8 i = 0;
  // Display the extended address.
  xad = aExtendedAddress + Z_EXTADDR_LEN - 1;

  for (i = 0; i < Z_EXTADDR_LEN*2; xad--)
  {
    uint8 ch;
    ch = (*xad >> 4) & 0x0F;
    lcd_buf[i++] = ch + (( ch < 10 ) ? '0' : '7');
    ch = *xad & 0x0F;
    lcd_buf[i++] = ch + (( ch < 10 ) ? '0' : '7');
  }
  lcd_buf[Z_EXTADDR_LEN*2] = '\0';
}
void get_ieee_source_addr_string(char *buf, ZLongAddr_t ieee_addr) 
{
  byte *xad;
  
 // uint8 lcd_buf[Z_EXTADDR_LEN*2+1];
  uint8 i = 0;
  // Display the extended address.
  xad = ieee_addr; //+ Z_EXTADDR_LEN - 1;

  for (i = 0; i < Z_EXTADDR_LEN*2; xad--)
  {
    uint8 ch;
    ch = (*xad >> 4) & 0x0F;
    buf[i++] = ch + (( ch < 10 ) ? '0' : '7');
    ch = *xad & 0x0F;
    buf[i++] = ch + (( ch < 10 ) ? '0' : '7');
  }
  buf[Z_EXTADDR_LEN*2] = '\0';
}