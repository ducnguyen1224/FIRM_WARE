/*
 * IOControl.c
 *
 *  Created on: Jul 14, 2018
 *      Author: ThaoHuyen
 */

#include "IOControl.h"



/******************************************************************************
 * Function		: void vIDControl_Init( void )
 * Description	: Ham khoi IO
 * Param		: none
 * Return		: none
*******************************************************************************/
void vIDControl_Init( void )
{
	//turn off led status
	macroLED_STATUS_OFF();

	//Enable network module
	macroNWK_POWER_ON();
	macroNWK_POWER_WIFI_ON();
	macroNWK_POWER_SAVING_ON();
	macroGNSS_POWER_OFF();
	macroNWK_RESET_DEACTIVE();

	//enable connectivity module
	macroCONN_POWER_ON();
	macroRESET_CONN_DEACTIVE();

	//enable ethernet
	macroETH_POWER_ON();

	//refesh nano timer
	macroNANO_TIMER_DONE_HIGH();
}


