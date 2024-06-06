
#ifndef __DEVICECONTROL_H
#define __DEVICECONTROL_H


#include "main.h"
#include "pin_mux.h"
#include "fsl_gpio.h"


//Dust sensor power control
#define macroDUST_SENSOR_POWER_ON()					macroGPIO_ACTIVE_HIGH(macroPOWER_SS_DUST_CONTROL_GPIO, macroPOWER_SS_DUST_CONTROL_PIN)
#define macroDUST_SENSOR_POWER_OFF()				macroGPIO_ACTIVE_LOW(macroPOWER_SS_DUST_CONTROL_GPIO, macroPOWER_SS_DUST_CONTROL_PIN)

//Dust sensor reset control
#define macroDUST_SENSOR_RESET_ACTIVE()				macroGPIO_ACTIVE_LOW(macroSS_DUST_RESET_GPIO, macroSS_DUST_RESET_PIN)
#define macroDUST_SENSOR_RESET_DEACTIVE()			macroGPIO_ACTIVE_HIGH(macroSS_DUST_RESET_GPIO, macroSS_DUST_RESET_PIN)

//Dust sensor detect (active low)
#define macroDUST_SENSOR_DETECT_INPUT()				macroGPIO_READ_STATE(macroSS_DUST_DETECT_GPIO, macroSS_DUST_DETECT_PIN)

//Dust sensor buckboost power saving
#define macroDUST_SENSOR_POWER_SAVING()				macroGPIO_ACTIVE_LOW(macroSS_DUST_BUCKBOOST_POWER_SAVING_GPIO, macroSS_DUST_BUCKBOOST_POWER_SAVING_PIN)
#define macroDUST_SENSOR_POWER_NOT_SAVING()			macroGPIO_ACTIVE_HIGH(macroSS_DUST_BUCKBOOST_POWER_SAVING_GPIO, macroSS_DUST_BUCKBOOST_POWER_SAVING_PIN)




/******************************************************************************
 * Function		: void vDeviceControl_Init(void)
 * Description	: Device control init
 * Param		: None
 * Return		: None
*******************************************************************************/
void vDeviceControl_Init(void);




/******************************************************************************
 * Function		: bool bDeviceControl_DustSS_Detect( void )
 * Description	: Dust sensor detect
 * Param		: None
 * Return		: None
*******************************************************************************/
bool bDeviceControl_DustSS_Detect( void );



#endif








