
#include "SHT11Soil.h"
#include "main.h"

int _readRawGround(unsigned char cmd)
{
  
    //config pin for I2c sw
	gpio_pin_config_t xGPIO_ConfigInput = { kGPIO_DigitalInput, 0, };
	gpio_pin_config_t xGPIO_ConfigOutput = { kGPIO_DigitalOutput, 0, };
	
	int _val = 0;
	unsigned char i;
	unsigned long cnt = 0;

	// Conversion coefficients from SHT15 datasheet
//	const float D1 = -39.7;
//	const float D2 =   0.01;

	// Transmission Start
	SHTG_DATA_DIR_OUT(xGPIO_ConfigOutput);
	SHTG_CLK_DIR_OUT(xGPIO_ConfigOutput);
	SHTG_DATA_OUT_HIGH;
	SHTG_CLK_OUT_HIGH;
	SHTG_DATA_OUT_LOW;
	SHTG_CLK_OUT_LOW;
	SHTG_CLK_OUT_HIGH;
	SHTG_DATA_OUT_HIGH;
	SHTG_CLK_OUT_LOW;

	// The command (3 msb are address and must be 000, and last 5 bits are command)

	for (i = 0; i < 8; i++)
	{
		if (!(cmd & (1 << (7 - i)))) SHTG_DATA_OUT_LOW;
		else    SHTG_DATA_OUT_HIGH;
		SHTG_CLK_OUT_HIGH;
		SHTG_CLK_OUT_LOW;
	}

	SHTG_CLK_OUT_HIGH;
	SHTG_CLK_OUT_LOW;

	SHTG_DATA_DIR_IN(xGPIO_ConfigInput);
	while((SHTG_DATA_IN) && (cnt < 2000000))
	{
		cnt++;
	};
	cnt = 0;
	
	// get the MSB (most significant bits)
	SHTG_DATA_DIR_IN(xGPIO_ConfigInput);
	for (i=0; i<8; ++i)
	{
		_val <<= 1;
		SHTG_CLK_OUT_HIGH;
		macroTASK_DELAY_MS(1);
		if (SHTG_DATA_IN)
		_val |= SHTG_DATA_IN;
		SHTG_CLK_OUT_LOW;
	}

	// send the required ACK
	SHTG_DATA_DIR_OUT(xGPIO_ConfigOutput);
	SHTG_DATA_OUT_HIGH;
	SHTG_DATA_OUT_LOW;
	SHTG_CLK_OUT_HIGH;
	SHTG_CLK_OUT_LOW;

	// get the LSB (less significant bits)
	SHTG_DATA_DIR_IN(xGPIO_ConfigInput);
	for (i=0; i<8; ++i)
	{
		_val <<= 1;
		SHTG_CLK_OUT_HIGH;
		macroTASK_DELAY_MS(1);
		if (SHTG_DATA_IN)
		_val |= SHTG_DATA_IN;
		SHTG_CLK_OUT_LOW;
	}

	//  skip CRC
	SHTG_DATA_DIR_OUT(xGPIO_ConfigOutput);
	SHTG_DATA_OUT_HIGH;
	SHTG_CLK_OUT_HIGH;
	SHTG_CLK_OUT_LOW;

	// Convert raw value to degrees Celsius
	return _val;
}
float _readTempGround(void)
{
	int raw = _readRawGround(tempG_cmd);
	float _temp = GD2*raw + GD1;
	return _temp;
}
float _readHumiGround(void)
{
	float _linear_humi, temp, _correct_humi;
	int raw;
	raw =_readRawGround(humiG_cmd);
	_linear_humi = GC1 + GC2 * raw + (GC3 * raw)* raw;
	temp = _readTempGround();
	_correct_humi = (temp - 25.0 ) * (GT1 + GT2 * raw) + _linear_humi;
	return _correct_humi;
}
void SHT11Ground(float *_temp, float *humi)
{
	int raw;
	*_temp = _readTempGround();
	raw = _readRawGround(humiG_cmd);
	*humi = (*_temp - 25.0) * (GT1*GT2*raw) + GC1 + GC2 * raw + (GC3 * raw)* raw;
}
