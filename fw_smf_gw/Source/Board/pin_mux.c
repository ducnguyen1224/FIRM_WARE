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

#include "fsl_port.h"
#include "fsl_gpio.h"
#include "pin_mux.h"


#define PIN0_IDX                         0u   /*!< Pin number for pin 0 in a port */

#define PIN1_IDX                         1u   /*!< Pin number for pin 1 in a port */

#define PIN5_IDX                         5u   /*!< Pin number for pin 5 in a port */

#define PIN12_IDX                       12u   /*!< Pin number for pin 12 in a port */

#define PIN13_IDX                       13u   /*!< Pin number for pin 13 in a port */

#define PIN14_IDX                       14u   /*!< Pin number for pin 14 in a port */

#define PIN15_IDX                       15u   /*!< Pin number for pin 15 in a port */

#define PIN16_IDX                       16u   /*!< Pin number for pin 16 in a port */

#define PIN17_IDX                       17u   /*!< Pin number for pin 17 in a port */

#define PIN18_IDX                       18u   /*!< Pin number for pin 18 in a port */

//sdcard
#define SDCARD_DATA_0                 	1u   /*!< Pin number for pin 1 in E port */
#define SDCARD_DATA_1	                0u   /*!< Pin number for pin 0 in E port */
#define SDCARD_DATA_2	                5u   /*!< Pin number for pin 5 in E port */
#define SDCARD_DATA_3                   4u   /*!< Pin number for pin 4 in E port */
#define SDCARD_CMD	 	                3u   /*!< Pin number for pin 3 in E port */
#define SDCARD_CLK		                2u   /*!< Pin number for pin 2 in E port */
#define SDCARD_DETECT	                6u   /*!< Pin number for pin 6 in E port */


/*******************************************************************************
 * Code
 ******************************************************************************/
void BOARD_InitPins(void)
{
    /* Declare and initialise for pull up configuration */
//    port_pin_config_t pinConfig = {0}; // @suppress("Type cannot be resolved")
//    pinConfig.pullSelect = kPORT_PullUp; // @suppress("Field cannot be resolved") // @suppress("Symbol is not resolved")
//#if defined(FSL_FEATURE_PORT_HAS_OPEN_DRAIN) && FSL_FEATURE_PORT_HAS_OPEN_DRAIN
//    pinConfig.openDrainEnable = kPORT_OpenDrainEnable;
//#endif /* FSL_FEATURE_PORT_HAS_OPEN_DRAIN */
	
	//Pin config
	gpio_pin_config_t xGPIO_ConfigOutput = { kGPIO_DigitalOutput, 0, };
	gpio_pin_config_t xGPIO_ConfigInput = { kGPIO_DigitalInput, 0, };

    /* Ungate the port clock */
    CLOCK_EnableClock(kCLOCK_PortA);
    CLOCK_EnableClock(kCLOCK_PortB);
    CLOCK_EnableClock(kCLOCK_PortC);
    CLOCK_EnableClock(kCLOCK_PortD);
    CLOCK_EnableClock(kCLOCK_PortE);
	
    /******************************** Led status ******************************/
    PORT_SetPinMux( macroLED_STATUS_PORT, macroLED_STATUS_GPIO_PIN, kPORT_MuxAsGpio ); // @suppress("Symbol is not resolved")
    GPIO_PinInit( macroLED_STATUS_GPIO, macroLED_STATUS_GPIO_PIN, &xGPIO_ConfigOutput );
	
    /******************************* network module ***************************/
    //For network module power key
	PORT_SetPinMux( macroNWK_POWER_KEY_PORT, macroNWK_POWER_KEY_PIN, kPORT_MuxAsGpio); // @suppress("Symbol is not resolved")
	GPIO_PinInit( macroNWK_POWER_KEY_GPIO, macroNWK_POWER_KEY_PIN, &xGPIO_ConfigOutput );

	//For network module power saving
	PORT_SetPinMux( macroNWK_POWER_SAVE_PORT, macroNWK_POWER_SAVE_PIN, kPORT_MuxAsGpio); // @suppress("Symbol is not resolved")
	GPIO_PinInit( macroNWK_POWER_SAVE_GPIO, macroNWK_POWER_SAVE_PIN, &xGPIO_ConfigOutput );

    //For network module power
	PORT_SetPinMux( macroNWK_POWER_PORT, macroNWK_POWER_PIN, kPORT_MuxAsGpio); // @suppress("Symbol is not resolved")
	GPIO_PinInit( macroNWK_POWER_GPIO, macroNWK_POWER_PIN, &xGPIO_ConfigOutput );
	
	//For network wifi module power
	PORT_SetPinMux( macroNWK_POWER_WIFI_PORT, macroNWK_POWER_WIFI_PIN, kPORT_MuxAsGpio); // @suppress("Symbol is not resolved")
	GPIO_PinInit( macroNWK_POWER_WIFI_GPIO, macroNWK_POWER_WIFI_PIN, &xGPIO_ConfigOutput );

	//For network wifi module power
	PORT_SetPinMux( macroNWK_RESET_WIFI_PORT, macroNWK_RESET_WIFI_PIN, kPORT_MuxAsGpio); // @suppress("Symbol is not resolved")
	GPIO_PinInit( macroNWK_RESET_WIFI_GPIO, macroNWK_RESET_WIFI_PIN, &xGPIO_ConfigOutput );

	//For GPS power
	PORT_SetPinMux( macroGNSS_POWER_PORT, macroGNSS_POWER_GPIO_PIN, kPORT_MuxAsGpio); // @suppress("Symbol is not resolved")
	GPIO_PinInit( macroGNSS_POWER_GPIO, macroGNSS_POWER_GPIO_PIN, &xGPIO_ConfigOutput );

	//network module reset
	PORT_SetPinMux( macroNWK_RESET_PORT, macroNWK_RESET_PIN, kPORT_MuxAsGpio); // @suppress("Symbol is not resolved")
	GPIO_PinInit( macroNWK_RESET_GPIO, macroNWK_RESET_PIN, &xGPIO_ConfigOutput );
	/**************************************************************************/
	
	/*************************** connectivity module **************************/
	//control power
	PORT_SetPinMux( macroCONN_POWER_PORT, macroCONN_POWER_PIN, kPORT_MuxAsGpio); // @suppress("Symbol is not resolved")
	GPIO_PinInit( macroCONN_POWER_GPIO, macroCONN_POWER_PIN, &xGPIO_ConfigOutput );

	//reset connectivity
	PORT_SetPinMux( macroRESET_CONN_PORT, macroRESET_CONN_PIN, kPORT_MuxAsGpio); // @suppress("Symbol is not resolved")
	GPIO_PinInit( macroRESET_CONN_GPIO, macroRESET_CONN_PIN, &xGPIO_ConfigOutput );
	/**************************************************************************/
	
	/**************************** nano timer module ***************************/
	//nano timer wake
	PORT_SetPinMux( macroNANO_TIMER_WAKE_PORT, macroNANO_TIMER_WAKE_PIN, kPORT_MuxAsGpio); // @suppress("Symbol is not resolved")
	GPIO_PinInit( macroNANO_TIMER_WAKE_GPIO, macroNANO_TIMER_WAKE_PIN, &xGPIO_ConfigInput );

	//nano timer done
	PORT_SetPinMux( macroNANO_TIMER_DONE_PORT, macroNANO_TIMER_DONE_PIN, kPORT_MuxAsGpio); // @suppress("Symbol is not resolved")
	GPIO_PinInit( macroNANO_TIMER_DONE_GPIO, macroNANO_TIMER_DONE_PIN, &xGPIO_ConfigOutput );
	/**************************************************************************/

	/********************************** button ********************************/
	PORT_SetPinMux( macroBUTTON_USER_PORT, macroBUTTON_USER_PIN, kPORT_MuxAsGpio); // @suppress("Symbol is not resolved")
	GPIO_PinInit( macroBUTTON_USER_GPIO, macroBUTTON_USER_PIN, &xGPIO_ConfigInput );
	/**************************************************************************/
    
	/******************************** UART ************************************/
	/* Initialize UART1 for network module */
    PORT_SetPinMux(PORTC, 3U, kPORT_MuxAlt3); // @suppress("Symbol is not resolved")
    PORT_SetPinMux(PORTC, 4U, kPORT_MuxAlt3); // @suppress("Symbol is not resolved")

    /*Initialize UART4 for connectivity module*/
    PORT_SetPinMux(PORTC, 14U, kPORT_MuxAlt3); // @suppress("Symbol is not resolved")
    PORT_SetPinMux(PORTC, 15U, kPORT_MuxAlt3); // @suppress("Symbol is not resolved")
    
    /*Initialize UART0 for debugger*/
    PORT_SetPinMux(PORTB, 16U, kPORT_MuxAlt3); // @suppress("Symbol is not resolved")
    PORT_SetPinMux(PORTB, 17U, kPORT_MuxAlt3); // @suppress("Symbol is not resolved")
	/**************************************************************************/
	
	/****************************** Ethernet **********************************/
    //ether control power
	PORT_SetPinMux( macroETH_POWER_PORT, macroETH_POWER_PIN, kPORT_MuxAsGpio); // @suppress("Symbol is not resolved")
	GPIO_PinInit( macroETH_POWER_GPIO, macroETH_POWER_PIN, &xGPIO_ConfigOutput );
	/**************************************************************************/
	
	PORT_SetPinMux(PORTA, PIN12_IDX, kPORT_MuxAlt4);           /* PORTA12 (pin 42) is configured as RMII0_RXD1 */ // @suppress("Symbol is not resolved")
	PORT_SetPinMux(PORTA, PIN13_IDX, kPORT_MuxAlt4);           /* PORTA13 (pin 43) is configured as RMII0_RXD0 */ // @suppress("Symbol is not resolved")
	PORT_SetPinMux(PORTA, PIN14_IDX, kPORT_MuxAlt4);           /* PORTA14 (pin 44) is configured as RMII0_CRS_DV */ // @suppress("Symbol is not resolved")
	PORT_SetPinMux(PORTA, PIN15_IDX, kPORT_MuxAlt4);           /* PORTA15 (pin 45) is configured as RMII0_TXEN */ // @suppress("Symbol is not resolved")
	PORT_SetPinMux(PORTA, PIN16_IDX, kPORT_MuxAlt4);           /* PORTA16 (pin 46) is configured as RMII0_TXD0 */ // @suppress("Symbol is not resolved")
	PORT_SetPinMux(PORTA, PIN17_IDX, kPORT_MuxAlt4);           /* PORTA17 (pin 47) is configured as RMII0_TXD1 */ // @suppress("Symbol is not resolved")
	PORT_SetPinMux(PORTA, PIN5_IDX, kPORT_MuxAlt4);            /* PORTA5 (pin 39) is configured as RMII0_RXER */ // @suppress("Symbol is not resolved")
	const port_pin_config_t portb0_pin53_config = { // @suppress("Type cannot be resolved")
		kPORT_PullUp,                                            /* Internal pull-up resistor is enabled */ // @suppress("Symbol is not resolved")
		kPORT_FastSlewRate,                                      /* Fast slew rate is configured */ // @suppress("Symbol is not resolved")
		kPORT_PassiveFilterDisable,                              /* Passive filter is disabled */ // @suppress("Symbol is not resolved")
		kPORT_OpenDrainEnable,                                   /* Open drain is enabled */ // @suppress("Symbol is not resolved")
		kPORT_LowDriveStrength,                                  /* Low drive strength is configured */ // @suppress("Symbol is not resolved")
		kPORT_MuxAlt4,                                           /* Pin is configured as RMII0_MDIO */ // @suppress("Symbol is not resolved")
		kPORT_UnlockRegister                                     /* Pin Control Register fields [15:0] are not locked */ // @suppress("Symbol is not resolved")
	};
	PORT_SetPinConfig(PORTB, PIN0_IDX, &portb0_pin53_config);  /* PORTB0 (pin 53) is configured as RMII0_MDIO */
	PORT_SetPinMux(PORTB, PIN1_IDX, kPORT_MuxAlt4);            /* PORTB1 (pin 54) is configured as RMII0_MDC */ // @suppress("Symbol is not resolved")
	//PORT_SetPinMux(PORTC, PIN16_IDX, kPORT_MuxAlt4);           /* PORTC16 (pin 90) is configured as ENET0_1588_TMR0 */
	//PORT_SetPinMux(PORTC, PIN17_IDX, kPORT_MuxAlt4);           /* PORTC17 (pin 91) is configured as ENET0_1588_TMR1 */
	//PORT_SetPinMux(PORTC, PIN18_IDX, kPORT_MuxAlt4);           /* PORTC18 (pin 92) is configured as ENET0_1588_TMR2 */
	/**************************************************************************/

	/******************************* SDCard ***********************************/
	const port_pin_config_t SDCard_Data_1_config = { // @suppress("Type cannot be resolved")
		kPORT_PullUp,                                            /* Internal pull-up resistor is enabled */ // @suppress("Symbol is not resolved")
		kPORT_FastSlewRate,                                      /* Fast slew rate is configured */ // @suppress("Symbol is not resolved")
		kPORT_PassiveFilterDisable,                              /* Passive filter is disabled */ // @suppress("Symbol is not resolved")
		kPORT_OpenDrainDisable,                                  /* Open drain is disabled */ // @suppress("Symbol is not resolved")
		kPORT_HighDriveStrength,                                 /* High drive strength is configured */ // @suppress("Symbol is not resolved")
		kPORT_MuxAlt4,                                           /* Pin is configured as SDHC0_D1 */ // @suppress("Symbol is not resolved")
		kPORT_UnlockRegister                                     /* Pin Control Register fields [15:0] are not locked */ // @suppress("Symbol is not resolved")
	};
	PORT_SetPinConfig(PORTE, SDCARD_DATA_1, &SDCard_Data_1_config);   /* PORTE0 (pin 1) is configured as SDHC0_D1 */

	const port_pin_config_t SDCard_Data_0_config = { // @suppress("Type cannot be resolved")
		kPORT_PullUp,                                            /* Internal pull-up resistor is enabled */ // @suppress("Symbol is not resolved")
		kPORT_FastSlewRate,                                      /* Fast slew rate is configured */ // @suppress("Symbol is not resolved")
		kPORT_PassiveFilterDisable,                              /* Passive filter is disabled */ // @suppress("Symbol is not resolved")
		kPORT_OpenDrainDisable,                                  /* Open drain is disabled */ // @suppress("Symbol is not resolved")
		kPORT_HighDriveStrength,                                 /* High drive strength is configured */ // @suppress("Symbol is not resolved")
		kPORT_MuxAlt4,                                           /* Pin is configured as SDHC0_D0 */ // @suppress("Symbol is not resolved")
		kPORT_UnlockRegister                                     /* Pin Control Register fields [15:0] are not locked */ // @suppress("Symbol is not resolved")
	};
	PORT_SetPinConfig(PORTE, SDCARD_DATA_0, &SDCard_Data_0_config);   /* PORTE1 (pin 2) is configured as SDHC0_D0 */

	const port_pin_config_t SDCard_CLK_config = { // @suppress("Type cannot be resolved")
		kPORT_PullUp,                                            /* Internal pull-up resistor is enabled */ // @suppress("Symbol is not resolved")
		kPORT_FastSlewRate,                                      /* Fast slew rate is configured */ // @suppress("Symbol is not resolved")
		kPORT_PassiveFilterDisable,                              /* Passive filter is disabled */ // @suppress("Symbol is not resolved")
		kPORT_OpenDrainDisable,                                  /* Open drain is disabled */ // @suppress("Symbol is not resolved")
		kPORT_HighDriveStrength,                                 /* High drive strength is configured */ // @suppress("Symbol is not resolved")
		kPORT_MuxAlt4,                                           /* Pin is configured as SDHC0_DCLK */ // @suppress("Symbol is not resolved")
		kPORT_UnlockRegister                                     /* Pin Control Register fields [15:0] are not locked */ // @suppress("Symbol is not resolved")
	};
	PORT_SetPinConfig(PORTE, SDCARD_CLK, &SDCard_CLK_config);   /* PORTE2 (pin 3) is configured as SDHC0_DCLK */

	const port_pin_config_t SDCard_CMD_config = { // @suppress("Type cannot be resolved")
		kPORT_PullUp,                                            /* Internal pull-up resistor is enabled */ // @suppress("Symbol is not resolved")
		kPORT_FastSlewRate,                                      /* Fast slew rate is configured */ // @suppress("Symbol is not resolved")
		kPORT_PassiveFilterDisable,                              /* Passive filter is disabled */ // @suppress("Symbol is not resolved")
		kPORT_OpenDrainDisable,                                  /* Open drain is disabled */ // @suppress("Symbol is not resolved")
		kPORT_HighDriveStrength,                                 /* High drive strength is configured */ // @suppress("Symbol is not resolved")
		kPORT_MuxAlt4,                                           /* Pin is configured as SDHC0_CMD */ // @suppress("Symbol is not resolved")
		kPORT_UnlockRegister                                     /* Pin Control Register fields [15:0] are not locked */ // @suppress("Symbol is not resolved")
	};
	PORT_SetPinConfig(PORTE, SDCARD_CMD, &SDCard_CMD_config);   /* PORTE3 (pin 4) is configured as SDHC0_CMD */

	const port_pin_config_t SDCard_Data_3_config = { // @suppress("Type cannot be resolved")
		kPORT_PullUp,                                            /* Internal pull-up resistor is enabled */ // @suppress("Symbol is not resolved")
		kPORT_FastSlewRate,                                      /* Fast slew rate is configured */ // @suppress("Symbol is not resolved")
		kPORT_PassiveFilterDisable,                              /* Passive filter is disabled */ // @suppress("Symbol is not resolved")
		kPORT_OpenDrainDisable,                                  /* Open drain is disabled */ // @suppress("Symbol is not resolved")
		kPORT_HighDriveStrength,                                 /* High drive strength is configured */ // @suppress("Symbol is not resolved")
		kPORT_MuxAlt4,                                           /* Pin is configured as SDHC0_D3 */ // @suppress("Symbol is not resolved")
		kPORT_UnlockRegister                                     /* Pin Control Register fields [15:0] are not locked */ // @suppress("Symbol is not resolved")
	};
	PORT_SetPinConfig(PORTE, SDCARD_DATA_3, &SDCard_Data_3_config);   /* PORTE4 (pin 5) is configured as SDHC0_D3 */

	const port_pin_config_t SDCard_Data_2_config = { // @suppress("Type cannot be resolved")
		kPORT_PullUp,                                            /* Internal pull-up resistor is enabled */ // @suppress("Symbol is not resolved")
		kPORT_FastSlewRate,                                      /* Fast slew rate is configured */ // @suppress("Symbol is not resolved")
		kPORT_PassiveFilterDisable,                              /* Passive filter is disabled */ // @suppress("Symbol is not resolved")
		kPORT_OpenDrainDisable,                                  /* Open drain is disabled */ // @suppress("Symbol is not resolved")
		kPORT_HighDriveStrength,                                 /* High drive strength is configured */ // @suppress("Symbol is not resolved")
		kPORT_MuxAlt4,                                           /* Pin is configured as SDHC0_D2 */ // @suppress("Symbol is not resolved")
		kPORT_UnlockRegister                                     /* Pin Control Register fields [15:0] are not locked */ // @suppress("Symbol is not resolved")
	};
	PORT_SetPinConfig(PORTE, SDCARD_DATA_2, &SDCard_Data_2_config);   /* PORTE5 (pin 6) is configured as SDHC0_D2 */

	const port_pin_config_t porte6_pin7_config = { // @suppress("Type cannot be resolved")
		kPORT_PullDown,                                          /* Internal pull-down resistor is enabled */ // @suppress("Symbol is not resolved")
		kPORT_FastSlewRate,                                      /* Fast slew rate is configured */ // @suppress("Symbol is not resolved")
		kPORT_PassiveFilterDisable,                              /* Passive filter is disabled */ // @suppress("Symbol is not resolved")
		kPORT_OpenDrainDisable,                                  /* Open drain is disabled */ // @suppress("Symbol is not resolved")
		kPORT_LowDriveStrength,                                  /* Low drive strength is configured */ // @suppress("Symbol is not resolved")
		kPORT_MuxAsGpio,                                         /* Pin is configured as PTE6 */ // @suppress("Symbol is not resolved")
		kPORT_UnlockRegister                                     /* Pin Control Register fields [15:0] are not locked */ // @suppress("Symbol is not resolved")
	};
	PORT_SetPinConfig(PORTE, SDCARD_DETECT, &porte6_pin7_config);   /* PORTE6 (pin 7) is configured as PTE6 */
	//GPIO_PinInit( GPIOE, SDCARD_DETECT, &xGPIO_ConfigInput );
}









