
#ifndef __SHT11SOIL_H
#define __SHT11SOIL_H

#include "pin_mux.h"
#include "fsl_gpio.h"

#define SHTG_DATA_DIR_OUT(Config)				GPIO_PinInit( macroSHTG_SDA_GPIO, macroSHTG_SDA_PIN, &Config )


#define SHTG_DATA_DIR_IN(Config)	    			GPIO_PinInit( macroSHTG_SDA_GPIO, macroSHTG_SDA_PIN, &Config )
//#define SHT_CLK_DIR_OUT	    					gpio_set_pin_output(GPIOB, GPIO_Pin_7)

#define SHTG_CLK_DIR_OUT(Config)	    			GPIO_PinInit( macroSHTG_SCL_GPIO, macroSHTG_SCL_PIN, &Config )

#define SHTG_DATA_OUT_HIGH						macroGPIO_ACTIVE_HIGH(macroSHTG_SDA_GPIO, macroSHTG_SDA_PIN)
#define SHTG_DATA_OUT_LOW						macroGPIO_ACTIVE_LOW(macroSHTG_SDA_GPIO, macroSHTG_SDA_PIN)
#define SHTG_CLK_OUT_HIGH						macroGPIO_ACTIVE_HIGH(macroSHTG_SCL_GPIO, macroSHTG_SCL_PIN)
#define SHTG_CLK_OUT_LOW	    					macroGPIO_ACTIVE_LOW(macroSHTG_SCL_GPIO, macroSHTG_SCL_PIN)

#define SHTG_DATA_IN		    					macroGPIO_READ_STATE(macroSHTG_SDA_GPIO, macroSHTG_SDA_PIN)


/* Define of Sensor */
#define tempG_cmd    0x03
#define humiG_cmd    0x05

#define GD1      -39.7
#define GD2      0.01

#define GC1       -4.0
#define GC2       0.0405
#define GC3       -0.0000028
#define GT1       0.01
#define GT2       0.00008
/////////////////////////

/* Define function */
int _readRawGround(unsigned char cmd);
float _readTempGround(void);
float _readHumiGround(void);
void SHT11Ground(float *_temp, float *humi);

#endif
