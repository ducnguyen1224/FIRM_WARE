#include "gpio.h"
#include "common.h"

static uint8 sht_resolution = 0;
static int8 sht_write_byte(uint8 data);
static uint8 sht_read_byte(uint8 ack);
static void sht_reset_conn(void);

/*****************************************
 * @fn:  sht_init()
 * @brief: khoi tao module sht
 * @param: none
 * @return: none
 */
void sht_start(void) {
//  SHT_DIR |= SHT_SCK_PIN | SHT_DATA_PIN;    /* Set pin direction to output */
    SHT_SCK_DIR_OUTPUT();
    SHT_DATA_DIR_OUTPUT();
//  LED_PORT_SEL &= ~(LED_BIT);    /* Set pin function to GPIO */
//  LED_PORT_DIR |= LED_BIT;    
  
  DATA_HIGH();
  SCK_LOW();
  delay_us(2);
  SCK_HIGH();
  delay_us(2);
  DATA_LOW();
  delay_us(2);

  SCK_LOW();
  delay_us(2);
  SCK_HIGH();
  delay_us(2);

  DATA_HIGH();
  delay_us(2);
  SCK_LOW();
}

/*****************************************
 * @fn: sht_rese_conn
 * @brief: reset connection
 * @param: 
 * @return: 
 */

static void sht_reset_conn(void) {
  uint8 i = 0;
  SHT_SCK_DIR_OUTPUT();
  SHT_DATA_DIR_OUTPUT();
  DATA_HIGH();
  for(i = 0; i < 9; i++) {
    SCK_HIGH();
    delay_us(2);
    SCK_LOW();
    delay_us(2);
  }
  sht_start();
}
/*****************************************
 * @fn:  sht_write_byte()
 * @brief: write 1 byte to sht sensor
 * @param: data - 1 byte data
 * @return: 1- error occured
 *          0- success    
 */

static int8 sht_write_byte(uint8 data) {
  uint8 i, err = 0;
  SHT_DATA_DIR_OUTPUT();
//  sht_start();
  delay_us(2);
  for (i = 0x80; i > 0; i >>= 1 ) {
    SCK_LOW();
    delay_us(2);
    if (i & data) {
      DATA_HIGH();
    } else {
      DATA_LOW();
    }
    delay_us(2);
    SCK_HIGH();
    delay_us(2);   
  }
  SCK_LOW();
  SHT_DATA_DIR_INPUT();
  delay_us(2);
  SCK_HIGH();
  delay_us(2);
  err = SHT_DATA_STATE();
  SCK_LOW();
  delay_ms(250);
  if (err) {
    return -1;
  } 
  return 0;
} 

/*****************************************
 * @fn:  
 * @brief: 
 * @param: 
 * @return: 
 *              
 */
static uint8 sht_read_byte(uint8 ack) {
  uint8 i, ret = 0;
  SHT_DATA_DIR_INPUT();
  for (i = 0x80; i > 0; i >>= 1) {
    SCK_HIGH();
    delay_us(2);
    if (SHT_DATA_STATE()) {
      ret |= i;
    }
    delay_us(2);
    SCK_LOW();
    delay_us(2);

  }
  SHT_DATA_DIR_OUTPUT();
  delay_us(1);
  if (ack) {
    DATA_LOW();
  } else {
    DATA_HIGH();
  }
  SCK_HIGH();
  delay_us(2);
  SCK_LOW();
  return ret;
}

/*****************************************
 * @fn:  
 * @brief: 
 * @param: 
 * @return: 
 *              
 */
void sht_init(uint8 resolution) {
  int8 test; 
  SHT_SCK_SEL &= ~(SHT_SCK_PIN);
  SHT_DATA_SEL &= ~(SHT_DATA_PIN);    /* Set pin function to GPIO */

  SHT_SCK_DIR_OUTPUT()
  SHT_DATA_DIR_OUTPUT()    /* Set pin direction to output */
  sht_reset_conn();
  test = sht_write_byte(SHT_STATUS_REG_W);
 // if(test == 0)
   // DBG_PRINTF("\r\n ngon \r\n");
  //else
   // DBG_PRINTF("\r\n nhu chim \r\n");
    
  delay_ms(300);
  sht_write_byte(resolution);
  sht_resolution = resolution;
}
/*****************************************
 * @fn:  
 * @brief: 
 * @param: 
 * @return: 
 *              
 */
void sht_reset_chip(void) {
  sht_reset_conn();
  sht_write_byte(SHT_RESET);
  delay_ms(100);
}

/*****************************************
 * @fn:  
 * @brief: 
 * @param: 
 * @return: 
 *              
 */
uint16 sht_read_cmd(uint8 cmd) {
  uint8 msb, lsb, crc;
  int8 test;
  sht_reset_conn();
  test = sht_write_byte(cmd);
  //if(test == 0)
   // DBG_PRINTF("\r\n ngon \r\n");
  //else
    //DBG_PRINTF("\r\n nhu chim \r\n");
  SHT_DATA_DIR_INPUT();
  //while(SHT_DATA_STATE());
  delay_us(500);
  msb = sht_read_byte(SHT_ACK);
  lsb = sht_read_byte(SHT_ACK);
  crc = sht_read_byte(SHT_NOACK);
  return ((uint16)msb << 8 | lsb);
}

void sht_read(float *temp, float *humi) {
 uint16 temper = 0;
 uint16 humility = 0;
 temper = sht_read_cmd(SHT_MEASURE_TEMP);
 humility = sht_read_cmd(SHT_MEASURE_HUMI);
 if (sht_resolution == SHT_14_12_BIT) {
  *temp = (H_D1 + H_D2 * temper);
  *humi = ((H_D1 + H_D2 *temper - 25) * (H_T1 + H_T2 * humility) + H_C1 + H_C2 * humility + H_C3 * humility * humility);
 } else {
   *temp = L_D1 + L_D2 * temper;
   *humi = (L_D1 + L_D2 *temper - 25) * (L_T1 + L_T2 * humility) + L_C1 + L_C2 * humility + L_C3 *humility * humility;
 }
}
