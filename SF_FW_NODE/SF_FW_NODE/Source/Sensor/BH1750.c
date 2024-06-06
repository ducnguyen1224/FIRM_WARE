
#include "BH1750.h"
#include "main.h"
#include "UART.h"
#include "I2C.h"


//I2C
extern i2c_rtos_handle_t xMaster_Handle_BH1750;


/******************************************************************************

*******************************************************************************/
void BH1750_vInit(void)
{
	macroGPIO_ACTIVE_LOW( macroBH1750_GPIO, macroBH1750_PIN );
	//init I2C
	vI2C_InitModule( &xMaster_Handle_BH1750, macroI2C_MASTER_BASE_BH1750, macroI2C_BAUDRATE_BH1750 );
	//pull down addr pin
	
}

/******************************************************************************

*******************************************************************************/
void BH1750_vBegin(void)
{
	BH1750_vWriteCmd(BH1750_POWER_ON);
}
/******************************************************************************

*******************************************************************************/
void BH1750_vSetMode(uint8_t Mode)
{
	BH1750_vWriteCmd(Mode);
}
/******************************************************************************

*******************************************************************************/
void BH1750_vSetAddr(uint8_t Addr)
{
	BH1750_vWriteCmd(Addr);
}
/******************************************************************************

*******************************************************************************/
uint16_t BH1750_vGetLux(void)
{
	uint8_t puData[2] = {0};
	uint16_t ui16Lux_val = 0;
	BH1750_vBegin();
	macroTASK_DELAY_MS( 50 );
	BH1750_vSetMode(BH1750_ONETIME_1LX_HRES);
	macroTASK_DELAY_MS( 180 );
	//read value
	uI2C_Read( &xMaster_Handle_BH1750,BH1750_Address_L,0xFF, puData, 2);
	
//	APP_DEBUG("--- BH1750_vGetLux = 0x%x\r\n",puData[0]);
//	APP_DEBUG("--- BH1750_vGetLux = 0x%x\r\n",puData[1]);
	
	ui16Lux_val = (puData[0] <<8) | puData[1];
	
	ui16Lux_val = (uint16_t)(ui16Lux_val/1.2);
	
//	APP_DEBUG("--- ui16Lux_val = %d\r\n",ui16Lux_val);
	
	return ui16Lux_val;
}

/******************************************************************************

*******************************************************************************/

void BH1750_vWriteCmd(uint8_t cmd)
{
	uint8_t puData[2] = {0};
	uI2C_Write( &xMaster_Handle_BH1750, BH1750_Address_L, cmd, puData, 0);
}

/******************************************************************************

*******************************************************************************/



