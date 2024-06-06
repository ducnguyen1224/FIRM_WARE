#ifndef COMMON_H
#define COMMON_H


#include "std_header.h"
void delay_us(uint32 time);
void delay_ms(uint32 time);
void get_ieee_string(char *lcd_buf);
void get_ieee_source_addr_string(char *buf, ZLongAddr_t ieee_addr);

#endif