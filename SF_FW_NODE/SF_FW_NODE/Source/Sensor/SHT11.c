
#include "SHT11.h"
#include "main.h"

int _readRaw(unsigned char cmd)
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
	SHT_DATA_DIR_OUT(xGPIO_ConfigOutput);
	SHT_CLK_DIR_OUT(xGPIO_ConfigOutput);
	SHT_DATA_OUT_HIGH;
	SHT_CLK_OUT_HIGH;
	SHT_DATA_OUT_LOW;
	SHT_CLK_OUT_LOW;
	SHT_CLK_OUT_HIGH;
	SHT_DATA_OUT_HIGH;
	SHT_CLK_OUT_LOW;

	// The command (3 msb are address and must be 000, and last 5 bits are command)

	for (i = 0; i < 8; i++)
	{
		if (!(cmd & (1 << (7 - i)))) SHT_DATA_OUT_LOW;
		else    SHT_DATA_OUT_HIGH;
		SHT_CLK_OUT_HIGH;
		SHT_CLK_OUT_LOW;
	}

	SHT_CLK_OUT_HIGH;
	SHT_CLK_OUT_LOW;

	SHT_DATA_DIR_IN(xGPIO_ConfigInput);
	while((SHT_DATA_IN) && (cnt < 2000000))
	{
		cnt++;
	};
	cnt = 0;
	
	// get the MSB (most significant bits)
	SHT_DATA_DIR_IN(xGPIO_ConfigInput);
	for (i=0; i<8; ++i)
	{
		_val <<= 1;
		SHT_CLK_OUT_HIGH;
		macroTASK_DELAY_MS(1);
		if (SHT_DATA_IN)
		_val
		|= SHT_DATA_IN;
		SHT_CLK_OUT_LOW;
	}

	// send the required ACK
	SHT_DATA_DIR_OUT(xGPIO_ConfigOutput);
	SHT_DATA_OUT_HIGH;
	SHT_DATA_OUT_LOW;
	SHT_CLK_OUT_HIGH;
	SHT_CLK_OUT_LOW;

	// get the LSB (less significant bits)
	SHT_DATA_DIR_IN(xGPIO_ConfigInput);
	for (i=0; i<8; ++i)
	{
		_val <<= 1;
		SHT_CLK_OUT_HIGH;
		macroTASK_DELAY_MS(1);
		if (SHT_DATA_IN)
		_val |= SHT_DATA_IN;
		SHT_CLK_OUT_LOW;
	}

	//  skip CRC
	SHT_DATA_DIR_OUT(xGPIO_ConfigOutput);
	SHT_DATA_OUT_HIGH;
	SHT_CLK_OUT_HIGH;
	SHT_CLK_OUT_LOW;

	// Convert raw value to degrees Celsius
	return _val;
}
float _readTemp(void)
{
	int raw = _readRaw(temp_cmd);
	float _temp = D2*raw + D1;
	return _temp;
}
float _readHumi(void)
{
	float _linear_humi, temp, _correct_humi;
	int raw;
	raw =_readRaw(humi_cmd);
	_linear_humi = C1 + C2 * raw + (C3 * raw)* raw;
	temp = _readTemp();
	_correct_humi = (temp - 25.0 ) * (T1 + T2 * raw) + _linear_humi;
	return _correct_humi;
}
void SHT11(float *_temp, float *humi)
{
	int raw;
	*_temp = _readTemp();
	raw = _readRaw(humi_cmd);
	*humi = (*_temp - 25.0) * (T1*T2*raw) + C1 + C2 * raw + (C3 * raw)* raw;
}
