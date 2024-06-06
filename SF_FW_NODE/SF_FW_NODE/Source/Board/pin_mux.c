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



/*******************************************************************************
 * Code
 ******************************************************************************/
void BOARD_InitPins(void)
{
    /* Declare and initialise for pull up configuration */
    port_pin_config_t pinConfig = {0};
    pinConfig.pullSelect = kPORT_PullUp;
#if defined(FSL_FEATURE_PORT_HAS_OPEN_DRAIN) && FSL_FEATURE_PORT_HAS_OPEN_DRAIN
    pinConfig.openDrainEnable = kPORT_OpenDrainEnable;
#endif /* FSL_FEATURE_PORT_HAS_OPEN_DRAIN */
	
	//Pin config
	gpio_pin_config_t xGPIO_ConfigInput = { kGPIO_DigitalInput, 0, };
	gpio_pin_config_t xGPIO_ConfigOutput = { kGPIO_DigitalOutput, 0, };

    /* Ungate the port clock */
    CLOCK_EnableClock(kCLOCK_PortA);
    CLOCK_EnableClock(kCLOCK_PortB);
    CLOCK_EnableClock(kCLOCK_PortC);
    CLOCK_EnableClock(kCLOCK_PortD);
    CLOCK_EnableClock(kCLOCK_PortE);
	
	/************************** Led status & POWER Sensor**********************/
	/* For led status */
    PORT_SetPinMux( macroLED_STATUS_PORT, macroLED_STATUS_GPIO_PIN, kPORT_MuxAsGpio );
    GPIO_PinInit( macroLED_STATUS_GPIO, macroLED_STATUS_GPIO_PIN, &xGPIO_ConfigOutput );
	
	/* for Power sensor */
	PORT_SetPinMux( macroPH_POWER_PORT, macroPH_POWER_PIN, kPORT_MuxAsGpio );
    GPIO_PinInit( macroPH_POWER_GPIO, macroPH_POWER_PIN, &xGPIO_ConfigOutput );
	
	PORT_SetPinMux( macroEC_POWER_PORT, macroEC_POWER_PIN, kPORT_MuxAsGpio );
    GPIO_PinInit( macroEC_POWER_GPIO, macroEC_POWER_PIN, &xGPIO_ConfigOutput );
	
	PORT_SetPinMux( macroZIGB_POWER_PORT, macroZIGB_POWER_PIN, kPORT_MuxAsGpio );
    GPIO_PinInit( macroZIGB_POWER_GPIO, macroZIGB_POWER_PIN, &xGPIO_ConfigOutput );
	
	PORT_SetPinMux( macroPH2_POWER_PORT, macroPH2_POWER_PIN, kPORT_MuxAsGpio );
    GPIO_PinInit( macroPH2_POWER_GPIO, macroPH2_POWER_PIN, &xGPIO_ConfigOutput );
	
	PORT_SetPinMux( macroEC2_POWER_PORT, macroEC2_POWER_PIN, kPORT_MuxAsGpio );
    GPIO_PinInit( macroEC2_POWER_GPIO, macroEC2_POWER_PIN, &xGPIO_ConfigOutput );

	/*for BH1750 light sensor*/
	PORT_SetPinMux( macroBH1750_PORT, macroBH1750_PIN, kPORT_MuxAsGpio );
    GPIO_PinInit( macroBH1750_GPIO, macroBH1750_PIN, &xGPIO_ConfigOutput );
	/**************************************************************************/
	
	/********************************** UART **********************************/
	/* Initialize UART1 for PH*/
    PORT_SetPinMux(PORTC, 3U, kPORT_MuxAlt3);
    PORT_SetPinMux(PORTC, 4U, kPORT_MuxAlt3);
	
	/* Initialize UART2 for pH 2 */
    PORT_SetPinMux(PORTD, 3U, kPORT_MuxAlt3);
    PORT_SetPinMux(PORTD, 2U, kPORT_MuxAlt3);
	
	
	/*Initialize UART3 for EC sensor*/
    PORT_SetPinMux(PORTC, 16U, kPORT_MuxAlt3);
    PORT_SetPinMux(PORTC, 17U, kPORT_MuxAlt3);
	
	
	/*Initialize UART0 for EC 2 sensor*/
    PORT_SetPinMux(PORTB, 16U, kPORT_MuxAlt3);
    PORT_SetPinMux(PORTB, 17U, kPORT_MuxAlt3);
	
	/* Initialize UART4 for ZigB */
    PORT_SetPinMux(PORTC, 14U, kPORT_MuxAlt3);
    PORT_SetPinMux(PORTC, 15U, kPORT_MuxAlt3);
	/**************************************************************************/

	/************************************ I2C *********************************/
	/* I2C0 pull up resistor setting */
    PORT_SetPinConfig(PORTE, 24U, &pinConfig);
    PORT_SetPinConfig(PORTE, 25U, &pinConfig);
    /* I2C0 PIN_MUX Configuration */
    PORT_SetPinMux(PORTE, 24U, kPORT_MuxAlt5);
    PORT_SetPinMux(PORTE, 25U, kPORT_MuxAlt5);
	
	/************************for i2c sw****************************************/
	PORT_SetPinMux( macroSHT_SDA_PORT, macroSHT_SDA_PIN, kPORT_MuxAsGpio );
	PORT_SetPinMux( macroSHT_SCL_PORT, macroSHT_SCL_PIN, kPORT_MuxAsGpio );
    
    PORT_SetPinMux( macroSHTG_SDA_PORT, macroSHTG_SDA_PIN, kPORT_MuxAsGpio );
	PORT_SetPinMux( macroSHTG_SCL_PORT, macroSHTG_SCL_PIN, kPORT_MuxAsGpio );
	
	/******************************* Nano timer *******************************/
	/* Done */
	PORT_SetPinMux( macroNANO_TIMER_DONE_PORT, macroNANO_TIMER_DONE_PIN, kPORT_MuxAsGpio);
	GPIO_PinInit( macroNANO_TIMER_DONE_GPIO, macroNANO_TIMER_DONE_PIN, &xGPIO_ConfigOutput );
    
	
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
		kPORT_PullUp,                                          /* Internal pull-down resistor is enabled */ // @suppress("Symbol is not resolved")
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
