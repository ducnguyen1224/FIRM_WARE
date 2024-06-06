
#ifndef __SHT11_H
#define __SHT11_H

#include "pin_mux.h"
#include "fsl_gpio.h"

#define SHT_DATA_DIR_OUT(Config)				GPIO_PinInit( macroSHT_SDA_GPIO, macroSHT_SDA_PIN, &Config )


#define SHT_DATA_DIR_IN(Config)	    			GPIO_PinInit( macroSHT_SDA_GPIO, macroSHT_SDA_PIN, &Config )
//#define SHT_CLK_DIR_OUT	    					gpio_set_pin_output(GPIOB, GPIO_Pin_7)

#define SHT_CLK_DIR_OUT(Config)	    			GPIO_PinInit( macroSHT_SCL_GPIO, macroSHT_SCL_PIN, &Config )

#define SHT_DATA_OUT_HIGH						macroGPIO_ACTIVE_HIGH(macroSHT_SDA_GPIO, macroSHT_SDA_PIN)
#define SHT_DATA_OUT_LOW						macroGPIO_ACTIVE_LOW(macroSHT_SDA_GPIO, macroSHT_SDA_PIN)
#define SHT_CLK_OUT_HIGH						macroGPIO_ACTIVE_HIGH(macroSHT_SCL_GPIO, macroSHT_SCL_PIN)
#define SHT_CLK_OUT_LOW	    					macroGPIO_ACTIVE_LOW(macroSHT_SCL_GPIO, macroSHT_SCL_PIN)

#define SHT_DATA_IN		    					macroGPIO_READ_STATE(macroSHT_SDA_GPIO, macroSHT_SDA_PIN)


/* Define of Sensor */
#define temp_cmd    0x03
#define humi_cmd    0x05

#define D1      -39.7
#define D2      0.01

#define C1       -4.0
#define C2       0.0405
#define C3       -0.0000028
#define T1       0.01
#define T2       0.00008
/////////////////////////

/* Define function */
int _readRaw(unsigned char cmd);
float _readTemp(void);
float _readHumi(void);
void SHT11(float *_temp, float *humi);

#endif
