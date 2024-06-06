/*
 * IOControl.h
 *
 *  Created on: Jul 14, 2018
 *      Author: ThaoHuyen
 */

#ifndef SOURCE_IOCONTROL_H_
#define SOURCE_IOCONTROL_H_

#include <fsl_gpio.h>
#include "pin_mux.h"


/****************** Led status ***********************/
#define macroLED_STATUS_ON()					GPIO_WritePinOutput(macroLED_STATUS_GPIO, macroLED_STATUS_GPIO_PIN, 0)
#define macroLED_STATUS_OFF()					GPIO_WritePinOutput(macroLED_STATUS_GPIO, macroLED_STATUS_GPIO_PIN, 1)

/****************** network module ***********************/
//For network module power
#define macroNWK_POWER_ON()        				GPIO_WritePinOutput(macroNWK_POWER_GPIO, macroNWK_POWER_PIN, 1)
#define macroNWK_POWER_OFF()          			GPIO_WritePinOutput(macroNWK_POWER_GPIO, macroNWK_POWER_PIN, 0)

//For network module power
#define macroNWK_POWER_WIFI_ON()        		GPIO_WritePinOutput(macroNWK_POWER_GPIO, macroNWK_POWER_PIN, 1)
#define macroNWK_POWER_WIFI_OFF()      			GPIO_WritePinOutput(macroNWK_POWER_GPIO, macroNWK_POWER_PIN, 0)

//reset nwk
#define macroNWK_RESET_WIFI_ON()        		GPIO_WritePinOutput(macroNWK_RESET_WIFI_GPIO, macroNWK_RESET_WIFI_PIN, 0)
#define macroNWK_RESET_WIFI_OFF()      			GPIO_WritePinOutput(macroNWK_RESET_WIFI_GPIO, macroNWK_RESET_WIFI_PIN, 1)

//For network module power saving
#define macroNWK_POWER_SAVING_ON()        		GPIO_WritePinOutput(macroNWK_POWER_SAVE_GPIO, macroNWK_POWER_SAVE_PIN, 1)
#define macroNWK_POWER_SAVING_OFF()          	GPIO_WritePinOutput(macroNWK_POWER_SAVE_GPIO, macroNWK_POWER_SAVE_PIN, 0)

//For GPS power
#define macroGNSS_POWER_ON()     				GPIO_WritePinOutput(macroGNSS_POWER_GPIO, macroGNSS_POWER_GPIO_PIN, 1)
#define macroGNSS_POWER_OFF()          			GPIO_WritePinOutput(macroGNSS_POWER_GPIO, macroGNSS_POWER_GPIO_PIN, 0)

//network module reset
#define macroNWK_RESET_ACTIVE()					GPIO_WritePinOutput(macroNWK_RESET_GPIO, macroNWK_RESET_PIN, 0)
#define macroNWK_RESET_DEACTIVE()				GPIO_WritePinOutput(macroNWK_RESET_GPIO, macroNWK_RESET_PIN, 1)


/****************** connectivity module ***********************/
//control power
#define macroCONN_POWER_ON()					GPIO_WritePinOutput(macroCONN_POWER_GPIO, macroCONN_POWER_PIN, 0)
#define macroCONN_POWER_OFF()					GPIO_WritePinOutput(macroCONN_POWER_GPIO, macroCONN_POWER_PIN, 1)

//reset conn
#define macroCONN_RESET_ON()					GPIO_WritePinOutput(macroRESET_CONN_GPIO, macroRESET_CONN_PIN, 0)
#define macroCONN_RESET_OFF()					GPIO_WritePinOutput(macroRESET_CONN_GPIO, macroRESET_CONN_PIN, 1)

//reset connectivity
#define macroRESET_CONN_ACTIVE()				GPIO_WritePinOutput(macroRESET_CONN_GPIO, macroRESET_CONN_PIN, 0)
#define macroRESET_CONN_DEACTIVE()				GPIO_WritePinOutput(macroRESET_CONN_GPIO, macroRESET_CONN_PIN, 1)


/****************** ethernet module ***********************/
//ether control power
#define macroETH_POWER_ON()						GPIO_WritePinOutput(macroETH_POWER_GPIO, macroETH_POWER_PIN, 1)
#define macroETH_POWER_OFF()					GPIO_WritePinOutput(macroETH_POWER_GPIO, macroETH_POWER_PIN, 0)


/****************** nano timer module ***********************/
//nano timer done
#define macroNANO_TIMER_DONE_HIGH()				GPIO_WritePinOutput(macroNANO_TIMER_DONE_GPIO, macroNANO_TIMER_DONE_PIN, 1)
#define macroNANO_TIMER_DONE_LOW()				GPIO_WritePinOutput(macroNANO_TIMER_DONE_GPIO, macroNANO_TIMER_DONE_PIN, 0)
#define macroNANO_TIME_DONE()					GPIO_TogglePinsOutput(macroNANO_TIMER_DONE_GPIO, 1 << macroNANO_TIMER_DONE_PIN)



/******************************************************************************
 * Function		: void vIDControl_Init( void )
 * Description	: Ham khoi IO
 * Param		: none
 * Return		: none
*******************************************************************************/
void vIDControl_Init( void );



#endif /* SOURCE_IOCONTROL_H_ */





















