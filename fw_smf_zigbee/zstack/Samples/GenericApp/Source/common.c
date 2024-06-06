#include "common.h" 

extern uint8 GenericApp_TaskID; 
/*************************************************************************
*
* Active data
*/
data_struct my_data=
             {
               {0},
#ifdef GAS_SENSOR_NODE
                FALSE,
                0,
#endif
#ifdef TEMP_HUMI_SENSOR_NODE
                0,
                0
#endif
#ifdef LIGHT_NODE
                0
#endif
#ifdef SMART_PLUG_NODE
                0,
                0
#endif
              }; 
static void delay_1us(void);
static void delay_1ms(void);
void PreaperData(uint8 CmdType,uint8* buf);
void get_ieee_string(uint8 *lcd_buf);
void get_ieee_source_addr_string(uint8 *buf, ZLongAddr_t ieee_addr);
void Alarm_gas_high_level(void);

char** str_split(char* a_str, const char a_delim);




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

void Alarm_gas_high_level(void)
{
 //my_data.AlarmHighLevel=TRUE;
 //osal_start_timerEx(GenericApp_TaskID,ALARM_EVT,100);
}

void get_ieee_string(uint8 *buf) {
  uint8 *xad;
 // uint8 lcd_buf[Z_EXTADDR_LEN*2+1];
  uint8 i = 0;
  // Display the extended address.
  xad = aExtendedAddress + Z_EXTADDR_LEN - 1;

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
void get_ieee_source_addr_string(uint8*buf, ZLongAddr_t ieee_addr) 
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
/*********************************************************************
 * @fn      PreaperData
 *
 * @brief   Preapering the data befor sennd to gateway.
 *
 * @param   CmdType - command type.
 * @param   buf     - The Buffer stored data after preapering.
 *
 * @return  none
 */
void PreaperData(uint8 CmdType,uint8* buf)
{
  uint8 AppType;
  uint8 NumberFiled;
  uint8 ieee_string[17]; /* 16 char MAC + 1 for NUL*/
  get_ieee_string(ieee_string);
#ifdef HOME_APP
  AppType=HOME_APPLICATION;
#endif
#ifdef AGRI_APP
  AppType=AGRI_APPLICATION;
#endif
#ifdef TEMP_HUMI_SENSOR_NODE
  NumberFiled= 0x02;
  sprintf((char*)buf,"%d,%s,%d,%d,%d%d,%d%d!",CmdType,ieee_string,AppType,NumberFiled,TEMP_FILED,my_data.Humi,TEMP_FILED,my_data.Temp);
#else
  NumberFiled= 0x01;
#endif
#ifdef GAS_SENSOR_NODE
  sprintf((char*)buf,"%d,%s,%d,%d,%d%d!",CmdType,ieee_string,AppType,NumberFiled,GAS_FILED,my_data.Gas);
#endif 
#ifdef SMART_PLUG_NODE
    sprintf((char*)buf,"%d,%s,%d,%d,%d%d,%d%d!",CmdType,ieee_string,AppType,NumberFiled,PLUG1_FILED,my_data.Plug1,PLUG2_FILED,my_data.Plug1);
#endif
}

/* 
 * @fn      str_split
 *
 * @brief   Split String to sub string with any char
 *
 * @param    a_str - String which need split
 * @param   a_delim     - char
 *
 * @return  char **
*/
char** str_split(char* a_str, const char a_delim)
{
      char** result    = 0;
    size_t count     = 0;
    char* tmp        = a_str;
    char* last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;

    /* Count how many elements will be extracted. */
    while (*tmp)
    {
        if (a_delim == *tmp)
        {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }

    /* Add space for trailing token. */
    count += last_comma < (a_str + strlen(a_str) - 1);

    /* Add space for terminating null string so caller
       knows where the list of returned strings ends. */
    count++;

    result = malloc(sizeof(char*) * count);

    if (result)
    {
        size_t idx  = 0;
        char* token = strtok(a_str, delim);

        while (token)
        {
            assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
        assert(idx == count - 1);
        *(result + idx) = 0;
    }

    return result;
}