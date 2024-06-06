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

/*!
 * @brief configure all pins for this demo/example
 *
 */
	
//LED Status
#define macroLED_STATUS_PORT          				PORTD
#define macroLED_STATUS_GPIO          				GPIOD
#define macroLED_STATUS_GPIO_PIN      				5U

////for GPIO power
#define macroPH_POWER_PORT							PORTC
#define macroPH_POWER_GPIO							GPIOC
#define macroPH_POWER_PIN							2U

#define macroPH2_POWER_PORT							PORTD
#define macroPH2_POWER_GPIO							GPIOD
#define macroPH2_POWER_PIN							1U

#define macroEC_POWER_PORT							PORTC
#define macroEC_POWER_GPIO							GPIOC
#define macroEC_POWER_PIN							18U

#define macroEC2_POWER_PORT							PORTB
#define macroEC2_POWER_GPIO							GPIOB
#define macroEC2_POWER_PIN							18U
//
#define macroZIGB_POWER_PORT						PORTC
#define macroZIGB_POWER_GPIO						GPIOC
#define macroZIGB_POWER_PIN							13U
//
	
/**********************for i2C sw for Temp&Humi Air****************************/
#define macroSHT_SDA_PORT							PORTC				
#define macroSHT_SDA_GPIO							GPIOC
#define macroSHT_SDA_PIN							11U
	
#define macroSHT_SCL_PORT							PORTC
#define macroSHT_SCL_GPIO							GPIOC
#define macroSHT_SCL_PIN							10U
/**********************for i2C sw for Temp&Humi Soil(Ground)*******************/
#define macroSHTG_SDA_PORT							PORTA				
#define macroSHTG_SDA_GPIO							GPIOA
#define macroSHTG_SDA_PIN							13U
	
#define macroSHTG_SCL_PORT							PORTA
#define macroSHTG_SCL_GPIO							GPIOA
#define macroSHTG_SCL_PIN							12U
	
/**************************Nano timer done*************************************/
#define macroNANO_TIMER_DONE_PORT						PORTD
#define macroNANO_TIMER_DONE_GPIO						GPIOD
#define macroNANO_TIMER_DONE_PIN						7U
	
#define macroNANO_TIMER_WAKE_PORT						PORTD
#define macroNANO_TIMER_WAKE_GPIO						GPIOD
#define macroNANO_TIMER_WAKE_PIN						6U

/***************config BH1750:  pin addr ***************************************/
#define macroBH1750_PORT					PORTE
#define macroBH1750_GPIO					GPIOE
#define macroBH1750_PIN						26U	 

//sdcard
#define SDCARD_DATA_0                 	1u   /*!< Pin number for pin 1 in E port */
#define SDCARD_DATA_1	                0u   /*!< Pin number for pin 0 in E port */
#define SDCARD_DATA_2	                5u   /*!< Pin number for pin 5 in E port */
#define SDCARD_DATA_3                   4u   /*!< Pin number for pin 4 in E port */
#define SDCARD_CMD	 	                3u   /*!< Pin number for pin 3 in E port */
#define SDCARD_CLK		                2u   /*!< Pin number for pin 2 in E port */
#define SDCARD_DETECT	                6u   /*!< Pin number for pin 6 in E port */	

//for gpio_interrupt
													
#define macroGPIO_IE_PORT						PORTD
#define macroGPIO_IE_GPIO						GPIOD
#define macroGPIO_IE_PIN						4U

void BOARD_InitPins(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* _PIN_MUX_H_ */


