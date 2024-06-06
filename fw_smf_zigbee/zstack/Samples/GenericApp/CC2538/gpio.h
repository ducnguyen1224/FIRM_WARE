#ifndef GPIO_H
#define GPIO_H

#include "std_header.h"

/* define hardware */
//#define SHT__PORT                       P0
#define SHT_SCK_PIN                     BV(7)
#define SHT_DATA_PIN                    BV(0)  
#define SHT_SCK_BIT                     P1_7
#define SHT_DATA_BIT                    P2_0
#define SHT_SCK_SEL                     P1SEL
#define SHT_DATA_SEL                    P2SEL
#define SHT_SCK_DIR                     P1DIR
#define SHT_DATA_DIR                    P2DIR
#define SHT_SCK_INP                     P1INP
#define SHT_DATA_INP                    P2INP
#define SHT_DATA_STATE()                SHT_DATA_BIT
#define SHT_SCK_DIR_OUTPUT()            SHT_SCK_DIR |= SHT_SCK_PIN;   
#define SHT_DATA_DIR_OUTPUT()           SHT_DATA_DIR |= SHT_DATA_PIN;   
#define SHT_DATA_DIR_INPUT()            SHT_DATA_DIR &= ~(SHT_DATA_PIN); //SHT_INP |= SHT_DATA_PIN
#define SCK_LOW()       SHT_SCK_BIT=0
#define SCK_HIGH()      SHT_SCK_BIT=1
#define DATA_LOW()      SHT_DATA_BIT=0
#define DATA_HIGH()     SHT_DATA_BIT=1

//#define LED_PORT        P2
//#define LED_BIT         BV(0)
//#define LED_SBIT        P2_3
//#define LED_PORT_SEL    P2SEL
//#define LED_PORT_DIR    P2DIR
//#define LED_ON()        LED_SBIT=1
//#define LED_OFF()       LED_SBIT=0

/* define register of sht1x */
#define SHT_NOACK           0
#define SHT_ACK             1
#define SHT_STATUS_REG_W 0x06   //0000 0110
#define SHT_STATUS_REG_R 0x07   //0000 0111
#define SHT_MEASURE_TEMP 0x03   //0000 0011
#define SHT_MEASURE_HUMI 0x05   //0000 0101
#define SHT_RESET        0x1E   //0001 1110
/* resolution of sht1x */
#define SHT_14_12_BIT   0x00
#define SHT_12_8_BIT   0x01
//Cac hang so voi do phan giai 14 va 12 bit
#define H_C1  -4.0            
#define H_C2  0.0405
#define H_C3  -0.0000028
#define H_D1  -40.00          
#define H_D2  0.01
#define H_T1  0.01            
#define H_T2 0.00008
//Cac hang so voi do phan giai 12 va 8 bit
#define L_C1  -4.0          
#define L_C2  0.648
#define L_C3  -0.00072
#define L_D1  -40.00      
#define L_D2  0.04
#define L_T1  0.01            
#define L_T2 0.00128

void sht_start(void);
void sht_init(uint8 res);
void sht_reset_conn(void);
void sht_reset_chip(void);
uint16 sht_read_cmd(uint8 cmd);
void sht_read(float *temp, float *humi);
#endif //GPIO