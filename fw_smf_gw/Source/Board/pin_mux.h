/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _PIN_MUX_H_
#define _PIN_MUX_H_

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */
#include "MK64F12.h"

/*!
 * @brief configure all pins for this demo/example
 *
 */
	
/****************** Led status ***********************/
#define macroLED_STATUS_PORT          			PORTD
#define macroLED_STATUS_GPIO          			GPIOD
#define macroLED_STATUS_GPIO_PIN      			3U

/****************** network module ***********************/
//For network module power key
#define macroNWK_POWER_KEY_PORT        			PORTE
#define macroNWK_POWER_KEY_GPIO        			GPIOE
#define macroNWK_POWER_KEY_PIN		    		25U

//For network module power
#define macroNWK_POWER_PORT          			PORTC
#define macroNWK_POWER_GPIO          			GPIOC
#define macroNWK_POWER_PIN			    		17U

//Power for wifi
#define macroNWK_POWER_WIFI_PORT          		PORTD
#define macroNWK_POWER_WIFI_GPIO          		GPIOD
#define macroNWK_POWER_WIFI_PIN			    	5U

//nwk reset
#define macroNWK_RESET_WIFI_PORT          		PORTD
#define macroNWK_RESET_WIFI_GPIO          		GPIOD
#define macroNWK_RESET_WIFI_PIN			    	4U

//For network module power saving
#define macroNWK_POWER_SAVE_PORT          		PORTC
#define macroNWK_POWER_SAVE_GPIO          		GPIOC
#define macroNWK_POWER_SAVE_PIN			    	16U

//For GPS power
#define macroGNSS_POWER_PORT      				PORTE
#define macroGNSS_POWER_GPIO          			GPIOE
#define macroGNSS_POWER_GPIO_PIN      			24U

//network module reset
#define macroNWK_RESET_PORT						PORTE
#define macroNWK_RESET_GPIO						GPIOE
#define macroNWK_RESET_PIN						26U
	

/****************** connectivity module ***********************/
//control power
#define macroCONN_POWER_PORT					PORTC
#define macroCONN_POWER_GPIO					GPIOC
#define macroCONN_POWER_PIN						5U
	
//reset connectivity
#define macroRESET_CONN_PORT					PORTC
#define macroRESET_CONN_GPIO					GPIOC
#define macroRESET_CONN_PIN						2U


/****************** ethernet module ***********************/
//ether control power
#define macroETH_POWER_PORT						PORTC
#define macroETH_POWER_GPIO						GPIOC
#define macroETH_POWER_PIN						13U


/****************** nano timer module ***********************/
//nano timer wake
#define macroNANO_TIMER_WAKE_PORT				PORTD
#define macroNANO_TIMER_WAKE_GPIO				GPIOD
#define macroNANO_TIMER_WAKE_PIN				6U
	 
//nano timer done
#define macroNANO_TIMER_DONE_PORT				PORTD
#define macroNANO_TIMER_DONE_GPIO				GPIOD
#define macroNANO_TIMER_DONE_PIN				7U
	

/****************** Button ***********************/
//user button
#define macroBUTTON_USER_PORT					PORTD
#define macroBUTTON_USER_GPIO					GPIOD
#define macroBUTTON_USER_PIN					2U

	
void BOARD_InitPins(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* _PIN_MUX_H_ */
















