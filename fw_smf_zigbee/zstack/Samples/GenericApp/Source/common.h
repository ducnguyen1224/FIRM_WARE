#ifndef COMMON_H
#define COMMON_H
#include "std_header.h"

/* C library */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

extern char** str_split(char* a_str, const char a_delim);

extern void delay_us(uint32 time);
extern void delay_ms(uint32 time);
extern void PreaperData(uint8 CmdType,uint8* buf);
extern void get_ieee_string(uint8 *lcd_buf);
extern void get_ieee_source_addr_string(uint8 *buf, ZLongAddr_t ieee_addr);
void Alarm_gas_high_level(void);
/*********************************************************************
 * COMMAND TYPE
  */  
#define DATA_TYPE                               0x01
#define CONFIG_TYPE                             0x02
#define REQUEST_TYPE                            0x03
#define CONTROL_TYPE                            0x04
  
/*********************************************************************
 * FIELD DATA TYPE
  */
#ifdef TEMP_HUMI_SENSOR_NODE 
  #define TEMP_FILED                        0x01
  #define HUMI_FILED                        0x02
#endif
#ifdef LIGHT_NODE
  #define LIGHT_FILED                       0x03
#endif
#ifdef GAS_SENSOR_NODE                          
  #define GAS_FILED                         0x06
#endif
#ifdef SMART_PLUG_NODE 
  #define PLUG1_FILED                       0x82
  #define PLUG2_FILED                       0x83
#endif
  
/*********************************************************************
 * FIELD CONTROL TYPE
  */  
#ifdef SMART_LAMP_NODE 
  #define LAMP_DIMMER_FILED                   0x81
#endif
  
/*********************************************************************
 * APPLICATION TYPE
  */  
#ifdef HOME_APP 
  #define HOME_APPLICATION                    0x01
#endif
#ifdef AGRI_APP 
  #define AGRI_APPLICATION                    0x02
#endif
/*********************************************************************
 * EXTERNAL VARIABLES
 */
typedef struct
{
 uint8 ieee_string[17];
#ifdef GAS_SENSOR_NODE
 uint8  AlarmHighLevel;
 uint16 Gas;
#endif
#ifdef TEMP_HUMI_SENSOR_NODE
 uint8 Temp;
 uint8 Humi;
#endif
#ifdef LIGHT_NODE
 uint8 Ligh;
#endif
#ifdef SMART_PLUG_NODE
 uint8 Plug1;
 uint8 Plug2;
#endif
 }data_struct;
#endif