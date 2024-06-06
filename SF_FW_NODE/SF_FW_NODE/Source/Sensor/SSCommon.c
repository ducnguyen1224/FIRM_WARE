

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

#include "SSCommon.h"
#include "I2C.h"
#include "LMP91000.h"
#include "MCP3421.h"


//I2C
extern i2c_rtos_handle_t xMaster_Handle_SensorGas;


//!Compensation values for baseline
//-20 ºC	20ºC	40ºC	50ºC
const float fTable_Baseline_TempComp[][4]  =
{   
	{0,0,-0.05,-0.3},			// CL2_SS
	{-0.3,0,0,-0.3},			// CO_SS_CLE
	{-1,0,2.66,5},				// ETO_SS
	{-2,0,0,-2.5},				// H2_SS_SEC
	{0,0,0,-0.04},				// H2S_SS_SEC
	{0,0,4,15},					// HCL_SS
	{0,0,-0.1,0.1},				// HCN_SS
	{-0.25,0,1.66,3},			// NH3_SS
	{-0.6,0,4,9},				// NO_SS
	{0.1,0,-0.5,-1.7},			// NO2_SS_CLE
	{0,0,0,0},					// O2_SS
	{0,0,0.15,0.4},				// PH3_SS
	{0,0,0.2,0.6},				// SO2_SS
	{0,0,0,0},					// LEL_AS
	{0.01,0.025,0.12,0.25},		// O3_AS
	{0,0,0,0},					// NDIR_CO2_SS
	{0.013,0.016,0.047,0.130},	// NO2_AS
	{0.015,0.040,0.150,0.325},	// NO_AS
	{0.,0.01,0.1,0.225},		// SO2_AS
	{0,-0.025,-0.075,-0.150}	// CO_AS
};


//!Compensation values for sensitivity
//-20 ºC	0ºC		20ºC	40ºC	50ºC
const float fTable_Sensitivity_TempComp[][5]  = 	  
{   
	{0.92,0.96,1,0.97,0.95},	// CL2_SS
	{0.7,0.9,1,1.09,1},			// CO_SS_CLE
	{0.4,0.7,1,1.15,1.25},		// ETO_SS
	{0.2,0.5,1,2.1,2.8},		// H2_SS_SEC
	{0.87,0.95,1,1,1},			// H2S_SS_SEC
	{0.85,0.92,1,1,1},			// HCL_SS
	{0.8,1,1,1.04,1.04},		// HCN_SS
	{0.9,1,1,1,1},				// NH3_SS
	{0.7,0.8,1,1.06,1.12},		// NO_SS
	{0.8,0.8,1,1.1,1.25},		// NO2_SS_CLE
	{0.9,0.96,1,1.07,1.07},		// O2_SS
	{0.8,0.9,1,1.05,1.05},		// PH3_SS
	{0.9,1,1,1.05,1},			// SO2_SS
	{1.055,1.035,1,1.015,1.0},	// LEL_AS
	{0.7,0.92,1,1,0.92},		// O3_AS
	{1,1,1,1,1},				// NDIR_CO2_SS
	{0.7,0.77,1,1.1,1.13},		// NO2_AS
	{0.4,0.8,1,1.2,1.3},		// NO_AS	
	{0.83,0.95,1,0.99,0.96},	// SO2_AS
	{0.52,0.8,1,1.12,1.18}		// CO_AS
};


//!Compensation values for sensitivity
//-20 ºC	0ºC		20ºC	40ºC	50ºC
const float fTable_PPM2ugm3[]  = 	  
{   
	2900,	// CL2_SS
	1230,	// CO_SS_CLE
	1984,	// ETO_SS
	88.9,	// H2_SS_SEC
	1500,	// H2S_SS_SEC
	1605,	// HCL_SS
	1189,	// HCN_SS
	750,	// NH3_SS
	1320,	// NO_SS
	2030,	// NO2_SS_CLE
	1410,	// O2_SS
	772,	// PH3_SS
	2820,	// SO2_SS
	1,		// LEL_AS
	2115,	// O3_AS
	1940,	// NDIR_CO2_SS
	2030,	// NO2_AS
	1320,	// NO_AS
	2820,	// SO2_AS
	1230	// CO_AS
};


/******************************************************************************
 * Function		: float fSSCommon_GetBaseLineTempComp( sensor_config *xSensor_Config, float fTemperature )
 * Description	: This function generates the baseline compensation for the temperature in ppm
 * Param		: 
 *				+ xSensor_Config: Cau truc chua cac thong tin cau hinh Sensor
 *				+ fTemperature: ambient temperature for sensor compensation (-1000 if doesn't needed)
 * Return		: ppm value for compensation
*******************************************************************************/
float fSSCommon_GetBaseLineTempComp( Sensor_Config *xSensor_Config, float fTemperature )
{
    float val_high, val_low;
    float comp_ppm = 0;
    
    if(fTemperature == -1000)
        return 0;
    
    if(fTemperature < 20)
    {	
        val_low = fTable_Baseline_TempComp[xSensor_Config->sensor_type][0];
        val_high = fTable_Baseline_TempComp[xSensor_Config->sensor_type][1];
        comp_ppm = val_low + (((val_high - val_low) / 40) * (fTemperature - (-20)));
        
    }
    else if((fTemperature >= 20) && (fTemperature < 40))
    {
        val_low = fTable_Baseline_TempComp[xSensor_Config->sensor_type][1];
        val_high = fTable_Baseline_TempComp[xSensor_Config->sensor_type][2];
        comp_ppm = val_low + (((val_high - val_low) / 20) * (fTemperature - 20));	 
    }
    else if((fTemperature >= 40))
    {
        val_low = fTable_Baseline_TempComp[xSensor_Config->sensor_type][2];
        val_high = fTable_Baseline_TempComp[xSensor_Config->sensor_type][3];
        comp_ppm = val_low + (((val_high - val_low) / 10) * (fTemperature - 40));
    }
    else comp_ppm = 0;
    
    return comp_ppm;
}




/******************************************************************************
 * Function		: float fSSCommon_GetSensitivityTempComp( sensor_config *xSensor_Config, float fTemperature )
 * Description	: This function generates the output current compensation for the temperaturem
 * Param		: 
 *				+ xSensor_Config: Cau truc chua cac thong tin cau hinh Sensor
 *				+ fTemperature: ambient temperature for sensor compensation (-1000 if doesn't needed)
 * Return		: compensation factor
*******************************************************************************/
float fSSCommon_GetSensitivityTempComp( Sensor_Config *xSensor_Config, float fTemperature )
{
    float val_high, val_low;
    float comp_sens = 0;
    
    if(fTemperature == -1000) return 1;
    
    if(fTemperature < 0)
    {		
        val_low = fTable_Sensitivity_TempComp[xSensor_Config->sensor_type][0];
        val_high = fTable_Sensitivity_TempComp[xSensor_Config->sensor_type][1];
        comp_sens = val_low + (((val_high - val_low) / 40) * (fTemperature - -20));
    }
    else if((fTemperature >= 0) && (fTemperature < 20))
    {
        val_low = fTable_Sensitivity_TempComp[xSensor_Config->sensor_type][1];
        val_high = fTable_Sensitivity_TempComp[xSensor_Config->sensor_type][2];
        comp_sens = val_low + (((val_high - val_low) / 20) * (fTemperature - 20));		
    }
    else if((fTemperature >= 20) && (fTemperature < 40))
    {
        val_low = fTable_Sensitivity_TempComp[xSensor_Config->sensor_type][2];
        val_high = fTable_Sensitivity_TempComp[xSensor_Config->sensor_type][3];
        comp_sens = val_low + (((val_high - val_low) / 20) * (fTemperature - 20));
    }
    else if((fTemperature >= 40))
    {
        val_low = fTable_Sensitivity_TempComp[xSensor_Config->sensor_type][3];
        val_high = fTable_Sensitivity_TempComp[xSensor_Config->sensor_type][4];
        comp_sens = val_low + (((val_high - val_low) / 10) * (fTemperature - 40));
    }
    else comp_sens = 1;
    comp_sens = 1/comp_sens;
    
    return comp_sens;
}



/******************************************************************************
 * Function		: void vSSCommon_SetAmplifier( sensor_config *xSensor_Config, bool bElectrode, float fResister_ )
 * Description	: This function generates the output current compensation for the temperaturem
 * Param		: 
 *				+ xSensor_Config: Cau truc chua cac thong tin cau hinh Sensor
 *				+ bElectrode: electrode asociated to each digipot
 *					WORKING_ELECTRODE
 *					AUXILIARY_ELECTRODE
 *				+ fResister_: resistor value to set in Ohms
 * Return		: none
*******************************************************************************/
void vSSCommon_SetAmplifier( Sensor_Config *xSensor_Config, bool bElectrode, float fResister_ )
{
    uint8_t uValue = 0;
	uint8_t uAux[3] = {0};
    
    uI2C_Read( &xMaster_Handle_SensorGas,  macroI2C_ADRESS_MCP4146, VOL_TCON_REG | READ_COMMAND, uAux, 2);
    macroTASK_DELAY_MS( 10 );
    
    if(fResister_ <= 100000)
    {
        // Connect the resistor
        uValue = (uint8_t)(128.0 * (fResister_ / 100000.0));
        if (bElectrode == AUXILIARY_ELECTRODE)
        {
            uAux[0] =  uAux[1] | 0x04;
			uI2C_Write( &xMaster_Handle_SensorGas, macroI2C_ADRESS_MCP4146, VOL_TCON_REG | WRITE_COMMAND | 0x01, uAux, 1 );
            uValue = 128 - uValue;
			macroTASK_DELAY_MS( 10 );
			uI2C_Write( &xMaster_Handle_SensorGas, macroI2C_ADRESS_MCP4146, VOL_WIPER_0_REG | WRITE_COMMAND, (uint8_t *)&uValue, 1 );
        }
        else
        {
            uAux[0] =  uAux[1] | 0x10;
			uI2C_Write( &xMaster_Handle_SensorGas, macroI2C_ADRESS_MCP4146, VOL_TCON_REG | WRITE_COMMAND | 0x01, uAux, 1 );
			macroTASK_DELAY_MS( 10 );
			uI2C_Write( &xMaster_Handle_SensorGas, macroI2C_ADRESS_MCP4146, VOL_WIPER_1_REG | WRITE_COMMAND, (uint8_t *)&uValue, 1 );
        }	
    }
    else
    {	
        // Disconnect the resistor
        if (bElectrode == AUXILIARY_ELECTRODE)
        {
            uValue = uAux[1] & 0xFB;
			uI2C_Write( &xMaster_Handle_SensorGas, macroI2C_ADRESS_MCP4146, VOL_TCON_REG | WRITE_COMMAND | 0x01, (uint8_t *)&uValue, 1 );
        }
        else
        {
            uValue = uAux[1] & 0xEF;
			uI2C_Write( &xMaster_Handle_SensorGas, macroI2C_ADRESS_MCP4146, VOL_TCON_REG | WRITE_COMMAND | 0x01, (uint8_t *)&uValue, 1 );
        }
    }
    MYTRACE("--- SSCommon: Setting R: %d to %f Ohms||wiper\r\n", bElectrode, fResister_);
}




/******************************************************************************
 * Function		: float fSSCommon_GetAmplifier( sensor_config *xSensor_Config, bool bElectrode )
 * Description	: This function gets the resistance from an specific digipot
 * Param		: 
 *				+ xSensor_Config: Cau truc chua cac thong tin cau hinh Sensor
 *				+ bElectrode: electrode asociated to each digipot
 *					WORKING_ELECTRODE
 *					AUXILIARY_ELECTRODE
 * Return		: resistor value in Ohms
*******************************************************************************/
float fSSCommon_GetAmplifier( Sensor_Config *xSensor_Config, bool bElectrode )
{
    uint8_t uAux[3];
    uint16_t uiWiper;
    float fResistor;
    
    uI2C_Read( &xMaster_Handle_SensorGas, macroI2C_ADRESS_MCP4146, VOL_TCON_REG | READ_COMMAND, uAux, 2);	
    if ((uAux[1] == 0xEF) || (uAux[1] == 0xFB) || (uAux[1] == 0xEB))
    {	
        MYTRACE("--- SSCommon: Reading wiper %d external resistors\r\n", bElectrode);
        fResistor = 1000000;
        return fResistor;
    }
    
    //macroTASK_DELAY_MS( 10 );
    
    if (bElectrode == AUXILIARY_ELECTRODE)
        uI2C_Write( &xMaster_Handle_SensorGas, macroI2C_ADRESS_MCP4146, VOL_WIPER_0_REG | READ_COMMAND, uAux, 2);
    else
        uI2C_Write( &xMaster_Handle_SensorGas, macroI2C_ADRESS_MCP4146, VOL_WIPER_1_REG | READ_COMMAND, uAux, 2);
    
    uiWiper = (uAux[0] << 8) | uAux[1];
    
    if (bElectrode == AUXILIARY_ELECTRODE)
        uiWiper = 128 - uiWiper;
    
    fResistor = ((float)uiWiper / 128.0) * 100000.0;
    MYTRACE("--- SSCommon: Reading wiper %d: %d|| Registor value: %f\r\n", bElectrode, uiWiper, fResistor);
    
    return fResistor;
}



																			 
/******************************************************************************
 * Function		: Enum_SensorStatus eSSCommon_ReadSensorInfo( sensor_config *xSensor_Config )
 * Description	: This function reads the sensor information from EEPROM
 * Param		: 
 *				+ xSensor_Config: Cau truc chua cac thong tin cau hinh Sensor
 * Return		: 
 *              + Successfully: eSS_OK
 *              + Failure: eSS_Fail
*******************************************************************************/
Enum_SensorStatus eSSCommon_ReadSensorInfo( Sensor_Config *xSensor_Config )
{
    uint8_t uBuffer[5] = "";
    uint8_t EEPROM_checksum, generated_checksum;
    
    // First read the board version and sensor type
    // This parameters are common for all sensors
	uI2C_Read( &xMaster_Handle_SensorGas, macroI2C_ADRESS_E2PROM, VER_BOARD_REG, (uint8_t *)&xSensor_Config->AFE_ver, 1 );
	uI2C_Read( &xMaster_Handle_SensorGas, macroI2C_ADRESS_E2PROM, SENSOR_TYPE_REG, (uint8_t *)&xSensor_Config->sensor_type, 1 );
    
    // Then read the calibration values according the sensor type
    // Each kind of sensor has different calibration patterns
    switch(xSensor_Config->sensor_type)
    {
        // Pellistors PCB
		case LEL_AS:
		case CALIBRATION_PEL:
		case NDIR_CO2_SS:
		case CALIBRATION_NDIR:	
			uI2C_Read( &xMaster_Handle_SensorGas,  macroI2C_ADRESS_E2PROM, SENSITIVITY_REG, uBuffer, 4);
			memcpy(&xSensor_Config->m_conc, uBuffer, 4);
			uI2C_Read( &xMaster_Handle_SensorGas,  macroI2C_ADRESS_E2PROM, OFFSET_REG, uBuffer, 4);
			memcpy(&xSensor_Config->baseline, uBuffer, 4);
			// Stores calibration value 1
			uI2C_Read( &xMaster_Handle_SensorGas,  macroI2C_ADRESS_E2PROM, CAL_1_REG, uBuffer, 4);
			memcpy(&xSensor_Config->calibration[0][0], uBuffer, 4);
			// Stores calibration value 2
			uI2C_Read( &xMaster_Handle_SensorGas,  macroI2C_ADRESS_E2PROM, CAL_2_REG, uBuffer, 4);
			memcpy(&xSensor_Config->calibration[0][1], uBuffer, 4);
			break;
        
        // 3 electrodes PCB
		case CL2_SS:
		case CO_SS_SEC:
		case ETO_SS:
		case H2_SS_SEC:
		case H2S_SS_SEC:
		case HCL_SS:
		case HCN_SS:
		case NH3_SS:
		case NO_SS:
		case NO2_SS_CLE:
		case O2_SS:
		case PH3_SS:
		case SO2_SS:
		case CALIBRATION_3E:
			uI2C_Read( &xMaster_Handle_SensorGas,  macroI2C_ADRESS_E2PROM, SENSITIVITY_REG, uBuffer, 4);
			memcpy(&xSensor_Config->m_conc, uBuffer, 4);
			uI2C_Read( &xMaster_Handle_SensorGas,  macroI2C_ADRESS_E2PROM, OFFSET_REG, uBuffer, 4);
			memcpy(&xSensor_Config->baseline, uBuffer, 4);
			
			// Reads calibration values
			for (int x = 0; x < 7; x++)
			{
				uI2C_Read( &xMaster_Handle_SensorGas,  macroI2C_ADRESS_E2PROM, CAL_REG + (x * 8), uBuffer, 4);
				memcpy(&xSensor_Config->calibration[x][0], uBuffer, 4);
				uI2C_Read( &xMaster_Handle_SensorGas,  macroI2C_ADRESS_E2PROM, CAL_REG + (x * 8) + 4, uBuffer, 4);
				memcpy(&xSensor_Config->calibration[x][1], uBuffer, 4);
			}
			
			if( xSensor_Config->AFE_ver >= 3)
			{
				uI2C_Read( &xMaster_Handle_SensorGas,  macroI2C_ADRESS_E2PROM, CAL_REG + 56, uBuffer, 4);
				memcpy(&xSensor_Config->calibration[7][0], uBuffer, 4);
				uI2C_Read( &xMaster_Handle_SensorGas,  macroI2C_ADRESS_E2PROM, CAL_REG + 60, uBuffer, 4);
				memcpy(&xSensor_Config->calibration[7][1], uBuffer, 4);
			}
			break;
        
		// 4 electrodes PCB
		case O3_AS:
		case NO2_AS:
		case NO_AS:
		case SO2_AS:
		case CO_AS:
		case CALIBRATION_4E:
			uI2C_Read( &xMaster_Handle_SensorGas, macroI2C_ADRESS_E2PROM, SENSITIVITY_REG, uBuffer, 4);
			memcpy(&xSensor_Config->m_conc, uBuffer, 4);
			uI2C_Read( &xMaster_Handle_SensorGas,  macroI2C_ADRESS_E2PROM, OFFSET_REG, uBuffer, 4);
			memcpy(&xSensor_Config->baseline, uBuffer, 4);
			uI2C_Read( &xMaster_Handle_SensorGas,  macroI2C_ADRESS_E2PROM, SENS_AE_REG, uBuffer, 4);
			memcpy(&xSensor_Config->OX_NO2_sens, uBuffer, 4);
			
			// Reads calibration values for working electrode
			uI2C_Read( &xMaster_Handle_SensorGas,  macroI2C_ADRESS_E2PROM, CAL_WE_REG, uBuffer, 4);
			memcpy(&xSensor_Config->calibration[0][0], uBuffer, 4);
			uI2C_Read( &xMaster_Handle_SensorGas,  macroI2C_ADRESS_E2PROM, CAL_WE_REG + 4, uBuffer, 4);
			memcpy(&xSensor_Config->calibration[0][1], uBuffer, 4);
			
			// Reads calibration values for working electrode
			uI2C_Read( &xMaster_Handle_SensorGas,  macroI2C_ADRESS_E2PROM, CAL_AE_REG, uBuffer, 4);
			memcpy(&xSensor_Config->calibration[1][0], uBuffer, 4);
			uI2C_Read( &xMaster_Handle_SensorGas,  macroI2C_ADRESS_E2PROM, CAL_AE_REG + 4, uBuffer, 4);
			memcpy(&xSensor_Config->calibration[1][1], uBuffer, 4);
			
			xSensor_Config->aux_baseline = 0;
			
			switch(xSensor_Config->AFE_ver)
			{
				case 3:
					uI2C_Read( &xMaster_Handle_SensorGas,  macroI2C_ADRESS_E2PROM, AE_OFFSET_REG, uBuffer, 4);
					memcpy(&xSensor_Config->aux_baseline, uBuffer, 4);
					break;
			}
			break;
    }
    
    if(xSensor_Config->AFE_ver >= 3)
    {
        uI2C_Read( &xMaster_Handle_SensorGas,  macroI2C_ADRESS_E2PROM, SENSOR_CHECKSUM, &EEPROM_checksum, 1);
        memcpy(uBuffer, &xSensor_Config->m_conc, 4);
        generated_checksum = xSensor_Config->AFE_ver ^ xSensor_Config->sensor_type ^ uBuffer[0] ^ uBuffer[1] ^ uBuffer[2] ^ uBuffer[3];
        if (EEPROM_checksum != generated_checksum)
        {
            APP_DEBUG("--- SSCommon: Error with parameter checksum: %d read from EEPROM: %d\r\n", generated_checksum, EEPROM_checksum);
            return eSS_Fail;
        }
    }
	
//	APP_DEBUG("\r\n--- SSCommon: Read info sensor socket%d\r\n", xSensor_Config->socket);
//	APP_DEBUG("--- SSCommon: Sensor type 	= %d\r\n", xSensor_Config->sensor_type);
//	APP_DEBUG("--- SSCommon: m_conc 		= %f [nA/ppm]\r\n", xSensor_Config->m_conc);
//	APP_DEBUG("--- SSCommon: baseline 		= %f [nA or mV]\r\n", xSensor_Config->baseline);
//	APP_DEBUG("--- SSCommon: aux_baseline	= %f [nA]\r\n", xSensor_Config->aux_baseline);
//	APP_DEBUG("--- SSCommon: OX_NO2_sens	= %f [nA/ppm]\r\n", xSensor_Config->OX_NO2_sens);
//	for(uint8_t ui = 0; ui < 8; ui++)
//	{
//		APP_DEBUG("--- SSCommon: calibration	= [%f][%f]\r\n", xSensor_Config->calibration[ui][0], xSensor_Config->calibration[ui][1]);
//	}
//	APP_DEBUG("--- SSCommon: AFE_ver 		= %d \r\n", xSensor_Config->AFE_ver);
    
    return eSS_OK;
}


 

/******************************************************************************
 * Function		: Enum_SensorStatus eSSCommon_ConfigureAFE( sensor_config *xSensor_Config, float fR_gain )
 * Description	: This function configure the AFE module (LMP91000 and/or MCP4146)
 * Param		: 
 *				+ xSensor_Config: Cau truc chua cac thong tin cau hinh Sensor
 *				+ fR_gain: resistor value for transimpendance stage
 * Return		: 
 *              + Successfully: eSS_OK
 *              + Failure: eSS_Fail
*******************************************************************************/
Enum_SensorStatus eSSCommon_ConfigureAFE( Sensor_Config *xSensor_Config, float fR_gain )
{
    uint8_t gain_3E = 0;
    float gain_4E = -1;
    
    // NDIR and pellistor sensors don't need to configure the AFE	
    if ((xSensor_Config->sensor_type == LEL_AS) || (xSensor_Config->sensor_type == NDIR_CO2_SS) || \
		(xSensor_Config->sensor_type == CALIBRATION_NDIR) || (xSensor_Config->sensor_type == CALIBRATION_PEL))
    {
		DISABLE_I2C(xSensor_Config->I2C_gpio, xSensor_Config->I2C_pin);
        return eSS_OK;
    }
	
	//Enable communication with the AFE
    ENABLE_I2C(xSensor_Config->I2C_gpio, xSensor_Config->I2C_pin);
	macroTASK_DELAY_MS( 100 );
    
    if(eLMP91000_IsReady() == eSS_Fail)
    {
        APP_DEBUG("--- SSCommon: LMP91000 communication error\r\n");
        DISABLE_I2C(xSensor_Config->I2C_gpio, xSensor_Config->I2C_pin);
        return eSS_Fail;
    }
    
    if( fR_gain == -1000)
    {
        if ((xSensor_Config->sensor_type == O3_AS) || (xSensor_Config->sensor_type == NO2_AS) || (xSensor_Config->sensor_type == NO_AS) ||	\
            (xSensor_Config->sensor_type == SO2_AS) || (xSensor_Config->sensor_type == CO_AS))
        {
            gain_4E = MCP_GAIN_2;
        }
        else if (xSensor_Config->sensor_type == O2_SS)	// Basic gain for O2 sensor
        {
            gain_3E = LMP91000_TIAC_REG_REF_R_GAIN_3K5;
        }
        else											// Basic gain for others
        {
            gain_3E = LMP91000_TIAC_REG_REF_R_GAIN_35K;
        }
    }
    else if ((fR_gain > 7) && (xSensor_Config->sensor_type != O3_AS) && (xSensor_Config->sensor_type != NO2_AS)&& \
             (xSensor_Config->sensor_type != NO_AS) && (xSensor_Config->sensor_type != SO2_AS)&& (xSensor_Config->sensor_type != CO_AS))
    {
        gain_3E = LMP91000_TIAC_REG_REF_R_GAIN_35K;
    }
    else
    {
        gain_3E = (uint8_t)fR_gain;
        gain_4E = fR_gain;
    }
    
    // Configure LMP91000 to use an specific sensor
    // Pellistors don't use the LMP91000
    switch( xSensor_Config->sensor_type)
    {
		case CL2_SS:
			eLMP91000_SetTIACNReg(gain_3E, LMP91000_TIAC_REG_REF_R_LOAD_50R);
			eLMP91000_SetREFCNReg(	LMP91000_REFC_REG_REF_SOURCE_EXTERNAL_REF, 
									LMP91000_REFC_REG_REF_INT_Z_50,
									LMP91000_REFC_REG_REF_BIAS_PERC_0, 
									LMP91000_REFC_REG_REF_POLARITY_POSITIVE);
			break;
			
		case CO_SS_SEC:
			eLMP91000_SetTIACNReg(gain_3E, LMP91000_TIAC_REG_REF_R_LOAD_50R);
			eLMP91000_SetREFCNReg(	LMP91000_REFC_REG_REF_SOURCE_EXTERNAL_REF, 
									LMP91000_REFC_REG_REF_INT_Z_50,
									LMP91000_REFC_REG_REF_BIAS_PERC_0, 
									LMP91000_REFC_REG_REF_POLARITY_POSITIVE);
			break;
			
		case ETO_SS:
			eLMP91000_SetTIACNReg(gain_3E, LMP91000_TIAC_REG_REF_R_LOAD_50R);
			eLMP91000_SetREFCNReg(	LMP91000_REFC_REG_REF_SOURCE_EXTERNAL_REF, 
									LMP91000_REFC_REG_REF_INT_Z_50,
									LMP91000_REFC_REG_REF_BIAS_PERC_14,
									LMP91000_REFC_REG_REF_POLARITY_POSITIVE);
			break;
			
		case H2_SS_SEC:
			eLMP91000_SetTIACNReg(gain_3E, LMP91000_TIAC_REG_REF_R_LOAD_50R);
			eLMP91000_SetREFCNReg(	LMP91000_REFC_REG_REF_SOURCE_EXTERNAL_REF, 
									LMP91000_REFC_REG_REF_INT_Z_50,
									LMP91000_REFC_REG_REF_BIAS_PERC_0,
									LMP91000_REFC_REG_REF_POLARITY_POSITIVE);
			break;
			
		case H2S_SS_SEC:
			eLMP91000_SetTIACNReg(gain_3E, LMP91000_TIAC_REG_REF_R_LOAD_50R);
			eLMP91000_SetREFCNReg(	LMP91000_REFC_REG_REF_SOURCE_EXTERNAL_REF, 
									LMP91000_REFC_REG_REF_INT_Z_50,
									LMP91000_REFC_REG_REF_BIAS_PERC_10,
									LMP91000_REFC_REG_REF_POLARITY_POSITIVE);
			break;
			
		case HCL_SS:
			eLMP91000_SetTIACNReg(gain_3E, LMP91000_TIAC_REG_REF_R_LOAD_50R);
			eLMP91000_SetREFCNReg(	LMP91000_REFC_REG_REF_SOURCE_EXTERNAL_REF, 
									LMP91000_REFC_REG_REF_INT_Z_50,
									LMP91000_REFC_REG_REF_BIAS_PERC_10,
									LMP91000_REFC_REG_REF_POLARITY_POSITIVE);
			break;
			
		case HCN_SS:
			eLMP91000_SetTIACNReg(gain_3E, LMP91000_TIAC_REG_REF_R_LOAD_50R);
			eLMP91000_SetREFCNReg(	LMP91000_REFC_REG_REF_SOURCE_EXTERNAL_REF, 
									LMP91000_REFC_REG_REF_INT_Z_50,
									LMP91000_REFC_REG_REF_BIAS_PERC_0,
									LMP91000_REFC_REG_REF_POLARITY_POSITIVE);
			break;
			
		case NH3_SS:
			eLMP91000_SetTIACNReg(gain_3E, LMP91000_TIAC_REG_REF_R_LOAD_50R);
			eLMP91000_SetREFCNReg(	LMP91000_REFC_REG_REF_SOURCE_EXTERNAL_REF, 
									LMP91000_REFC_REG_REF_INT_Z_50,
									LMP91000_REFC_REG_REF_BIAS_PERC_0,
									LMP91000_REFC_REG_REF_POLARITY_POSITIVE);
			break;
			
		case NO_SS:
			eLMP91000_SetTIACNReg(gain_3E, LMP91000_TIAC_REG_REF_R_LOAD_50R);
			eLMP91000_SetREFCNReg(	LMP91000_REFC_REG_REF_SOURCE_EXTERNAL_REF, 
									LMP91000_REFC_REG_REF_INT_Z_50,
									LMP91000_REFC_REG_REF_BIAS_PERC_14,
									LMP91000_REFC_REG_REF_POLARITY_POSITIVE);
			break;
			
		case NO2_SS_CLE:
			eLMP91000_SetTIACNReg(gain_3E, LMP91000_TIAC_REG_REF_R_LOAD_50R);
			eLMP91000_SetREFCNReg(	LMP91000_REFC_REG_REF_SOURCE_EXTERNAL_REF, 
									LMP91000_REFC_REG_REF_INT_Z_50,
									LMP91000_REFC_REG_REF_BIAS_PERC_0,
									LMP91000_REFC_REG_REF_POLARITY_POSITIVE);
			break;
			
		case O2_SS:
			eLMP91000_SetTIACNReg(gain_3E, LMP91000_TIAC_REG_REF_R_LOAD_50R);
			eLMP91000_SetREFCNReg(	LMP91000_REFC_REG_REF_SOURCE_INTERNAL_REF,
									LMP91000_REFC_REG_REF_INT_Z_50,
									LMP91000_REFC_REG_REF_BIAS_PERC_18,
									LMP91000_REFC_REG_REF_POLARITY_NEGATIVE);
			break;
			
		case PH3_SS:
			eLMP91000_SetTIACNReg(gain_3E, LMP91000_TIAC_REG_REF_R_LOAD_50R);
			eLMP91000_SetREFCNReg(	LMP91000_REFC_REG_REF_SOURCE_EXTERNAL_REF, 
									LMP91000_REFC_REG_REF_INT_Z_50,
									LMP91000_REFC_REG_REF_BIAS_PERC_0,
									LMP91000_REFC_REG_REF_POLARITY_POSITIVE);
			break;
					
		case SO2_SS:
			eLMP91000_SetTIACNReg(gain_3E, LMP91000_TIAC_REG_REF_R_LOAD_50R);
			eLMP91000_SetREFCNReg(	LMP91000_REFC_REG_REF_SOURCE_EXTERNAL_REF, 
									LMP91000_REFC_REG_REF_INT_Z_50,
									LMP91000_REFC_REG_REF_BIAS_PERC_0,
									LMP91000_REFC_REG_REF_POLARITY_POSITIVE);
			break;
						
		case O3_AS:
			eLMP91000_SetTIACNReg(LMP91000_TIAC_REG_REF_R_GAIN_EXT, LMP91000_TIAC_REG_REF_R_LOAD_33R);
			eLMP91000_SetREFCNReg(	LMP91000_REFC_REG_REF_SOURCE_EXTERNAL_REF, 
									LMP91000_REFC_REG_REF_INT_Z_50,
									LMP91000_REFC_REG_REF_BIAS_PERC_0,
									LMP91000_REFC_REG_REF_POLARITY_POSITIVE);
			if (xSensor_Config->AFE_ver != 3)
			{
				vSSCommon_SetAmplifier(xSensor_Config, WORKING_ELECTRODE, gain_4E);
				vSSCommon_SetAmplifier(xSensor_Config, AUXILIARY_ELECTRODE, gain_4E);
			}
			break;
	
		case NO2_AS:
			eLMP91000_SetTIACNReg(LMP91000_TIAC_REG_REF_R_GAIN_EXT, LMP91000_TIAC_REG_REF_R_LOAD_33R);
			eLMP91000_SetREFCNReg(LMP91000_REFC_REG_REF_SOURCE_EXTERNAL_REF, 
								 LMP91000_REFC_REG_REF_INT_Z_50,
								 LMP91000_REFC_REG_REF_BIAS_PERC_0,
								 LMP91000_REFC_REG_REF_POLARITY_POSITIVE);
			if (xSensor_Config->AFE_ver != 3)
			{
				vSSCommon_SetAmplifier(xSensor_Config, WORKING_ELECTRODE, gain_4E);
				vSSCommon_SetAmplifier(xSensor_Config, AUXILIARY_ELECTRODE, gain_4E);				
			}
			break;
            
		case NO_AS:
			eLMP91000_SetTIACNReg(LMP91000_TIAC_REG_REF_R_GAIN_EXT, LMP91000_TIAC_REG_REF_R_LOAD_33R);
			eLMP91000_SetREFCNReg(LMP91000_REFC_REG_REF_SOURCE_EXTERNAL_REF, 
								 LMP91000_REFC_REG_REF_INT_Z_50,
								 LMP91000_REFC_REG_REF_BIAS_PERC_10,
								 LMP91000_REFC_REG_REF_POLARITY_POSITIVE);
			if (xSensor_Config->AFE_ver != 3)
			{
				vSSCommon_SetAmplifier(xSensor_Config, WORKING_ELECTRODE, gain_4E);
				vSSCommon_SetAmplifier(xSensor_Config, AUXILIARY_ELECTRODE, gain_4E);
			}
			break;
			
		case SO2_AS:
			eLMP91000_SetTIACNReg(LMP91000_TIAC_REG_REF_R_GAIN_EXT, LMP91000_TIAC_REG_REF_R_LOAD_33R);
			eLMP91000_SetREFCNReg(LMP91000_REFC_REG_REF_SOURCE_EXTERNAL_REF, 
								 LMP91000_REFC_REG_REF_INT_Z_50,
								 LMP91000_REFC_REG_REF_BIAS_PERC_0,
								 LMP91000_REFC_REG_REF_POLARITY_POSITIVE);
			if (xSensor_Config->AFE_ver != 3)
			{
				vSSCommon_SetAmplifier(xSensor_Config, WORKING_ELECTRODE, gain_4E);
				vSSCommon_SetAmplifier(xSensor_Config, AUXILIARY_ELECTRODE, gain_4E);				
			}
			break;
        
		case CO_AS:
			eLMP91000_SetTIACNReg(LMP91000_TIAC_REG_REF_R_GAIN_EXT, LMP91000_TIAC_REG_REF_R_LOAD_33R);
			eLMP91000_SetREFCNReg(LMP91000_REFC_REG_REF_SOURCE_EXTERNAL_REF, 
								 LMP91000_REFC_REG_REF_INT_Z_50,
								 LMP91000_REFC_REG_REF_BIAS_PERC_0,
								 LMP91000_REFC_REG_REF_POLARITY_POSITIVE);
			if (xSensor_Config->AFE_ver != 3)
			{
				vSSCommon_SetAmplifier(xSensor_Config, WORKING_ELECTRODE, gain_4E);
				vSSCommon_SetAmplifier(xSensor_Config, AUXILIARY_ELECTRODE, gain_4E);				
			}
			break;
        
		case CALIBRATION_3E:
			eLMP91000_SetTIACNReg(gain_3E, LMP91000_TIAC_REG_REF_R_LOAD_50R);
			eLMP91000_SetREFCNReg(	LMP91000_REFC_REG_REF_SOURCE_EXTERNAL_REF, 
									LMP91000_REFC_REG_REF_INT_Z_50,
									LMP91000_REFC_REG_REF_BIAS_PERC_14,
									LMP91000_REFC_REG_REF_POLARITY_POSITIVE);	
			break;
				
		case CALIBRATION_4E:
			eLMP91000_SetTIACNReg(LMP91000_TIAC_REG_REF_R_GAIN_EXT, LMP91000_TIAC_REG_REF_R_LOAD_50R);
			eLMP91000_SetREFCNReg(	LMP91000_REFC_REG_REF_SOURCE_EXTERNAL_REF, 
									LMP91000_REFC_REG_REF_INT_Z_50,
									LMP91000_REFC_REG_REF_BIAS_PERC_14,
									LMP91000_REFC_REG_REF_POLARITY_POSITIVE);			
			break;
		default:
			break;
    }
    eLMP91000_SetMODEReg(LMP91000_MODEC_REG_FET_NOT_SHORTED, LMP91000_MODEC_REG_PWR_MODE_3_LEAD_AMPERIOMETRIC);
    // Disable communication
    DISABLE_I2C(xSensor_Config->I2C_gpio, xSensor_Config->I2C_pin);
    
    return eSS_OK;
}




/******************************************************************************
 * Function		: void SSCommon_ShowSensorInfo( sensor_config *xSensor_Config ))
 * Description	: This function shows the information about the sensor
 * Param		: 
 *				+ xSensor_Config: Cau truc chua cac thong tin cau hinh Sensor
 * Return		: none
*******************************************************************************/
void SSCommon_ShowSensorInfo( Sensor_Config *xSensor_Config )
{
	char uSensor_NO[17] = {0};
    
    // Enable communication with the AFE
    ENABLE_I2C(xSensor_Config->I2C_gpio, xSensor_Config->I2C_pin);
    memset(uSensor_NO, 0x00, sizeof(uSensor_NO));
    uI2C_Read( &xMaster_Handle_SensorGas,  macroI2C_ADRESS_E2PROM, SENSOR_NO_REG, (uint8_t*)uSensor_NO, 16);
    
    //Disable communication with the AFE
    DISABLE_I2C(xSensor_Config->I2C_gpio, xSensor_Config->I2C_pin);
    
#ifdef DEBUG
    MYTRACE("--- SSCommon: ************ Sensor data ************\r\n");
    MYTRACE("--- SSCommon: Serial no: ");
    for(uint8_t i = 0; i < 17; i++)
        MYTRACE("%d", uSensor_NO[i]);
    MYTRACE("\r\n");
    MYTRACE("--- SSCommon: Sensor type:");
    switch(xSensor_Config->sensor_type)
    {
		case CL2_SS:
			MYTRACE("--- SSCommon: Cl2\r\n");
			break;
		case CO_SS_SEC:
			MYTRACE("--- SSCommon: CO\r\n");
			break;
		case ETO_SS:
			MYTRACE("--- SSCommon: ETC\r\n");
			break;
		case H2_SS_SEC:
			MYTRACE("--- SSCommon: H2\r\n");
			break;
		case H2S_SS_SEC:
			MYTRACE("--- SSCommon: H2S\r\n");
			break;
		case HCL_SS:
			MYTRACE("--- SSCommon: HCl\r\n");
			break;
		case HCN_SS:
			MYTRACE("--- SSCommon: HCN\r\n");
		case NH3_SS:
			MYTRACE("--- SSCommon: NH3\r\n");
			break;
		case NO_SS:
			MYTRACE("--- SSCommon: NO\r\n");
			break;
		case NO2_SS_CLE:
			MYTRACE("--- SSCommon: NO2\r\n");
			break;
		case O2_SS:
			MYTRACE("--- SSCommon: O2\r\n");
			break;
		case PH3_SS:
			MYTRACE("--- SSCommon: PH3\r\n");
			break;
		case SO2_SS:
			MYTRACE("--- SSCommon: SO2\r\n");
			break;
		case LEL_AS:
			MYTRACE("--- SSCommon: Combustible gas pellistor\r\n");
		case O3_AS:
			MYTRACE("--- SSCommon: O3\r\n");
			break;
		case NDIR_CO2_SS:
			MYTRACE("--- SSCommon: CO2\r\n");
			break;
		default:
			MYTRACE("--- SSCommon: Sensor_type no default, number: %d\r\n", xSensor_Config->sensor_type);
			break;
    }
    MYTRACE("--- SSCommon: power_pin: %d\r\n", xSensor_Config->power_pin);
    MYTRACE("--- SSCommon: I2C_pin: %d\r\n", xSensor_Config->I2C_pin);
    MYTRACE("--- SSCommon: m_conc: %f", xSensor_Config->m_conc);
    
    if((xSensor_Config->sensor_type == LEL_AS) || (xSensor_Config->sensor_type == NDIR_CO2_SS) \
       || (xSensor_Config->sensor_type == CALIBRATION_NDIR) || (xSensor_Config->sensor_type == CALIBRATION_PEL))
    {
        MYTRACE("  mV/% LEL\r\n");
    }
    else MYTRACE(" nA/ppm\r\n");
    
    /* baseline */
    MYTRACE("--- SSCommon: Baseline: %f", xSensor_Config->baseline);
    if((xSensor_Config->sensor_type == LEL_AS) || (xSensor_Config->sensor_type == NDIR_CO2_SS)	\
       || (xSensor_Config->sensor_type == CALIBRATION_NDIR) || (xSensor_Config->sensor_type == CALIBRATION_PEL))
    {
        MYTRACE("  mV\r\n");
    }
    else MYTRACE(" nA\r\n");
    
    /* For O3 sensor */
    if ((xSensor_Config->sensor_type == O3_AS) || (xSensor_Config->sensor_type == NO2_AS) || (xSensor_Config->sensor_type == NO_AS) || \
		(xSensor_Config->sensor_type == SO2_AS) || (xSensor_Config->sensor_type == CO_AS) || (xSensor_Config->sensor_type == CALIBRATION_4E))
    {
        MYTRACE("--- SSCommon: OX_NO2_sens: %f nA/ppm \r\n", xSensor_Config->OX_NO2_sens);
        if(xSensor_Config->AFE_ver == 3)
            MYTRACE("--- SSCommon: aux_baseline: %f nA\r\n", xSensor_Config->aux_baseline);
    }
    else if((xSensor_Config->sensor_type == LEL_AS) || (xSensor_Config->sensor_type == NDIR_CO2_SS) || \
			(xSensor_Config->sensor_type == CALIBRATION_NDIR) || (xSensor_Config->sensor_type == CALIBRATION_PEL))
    {
        MYTRACE("--- SSCommon: Adjust values: \r\n");
        MYTRACE("--- SSCommon: %d; %d\r\n", xSensor_Config->calibration[0][0], xSensor_Config->calibration[0][1]);
    }
    else 
    {
        MYTRACE("--- SSCommon: Calibration table\r\n");
        for(uint8_t i = 0; i < 7; i++)
        {       
            MYTRACE("--- SSCommon: i: \r\n");
            MYTRACE("--- SSCommon: %f; %f\r\n", xSensor_Config->calibration[0][0], xSensor_Config->calibration[0][1]);
        }
    }
#endif
}




/******************************************************************************
 * Function		: uint8_t uSSCommon_AutoGain( sensor_config *xSensor_Config )
 * Description	: This function changes the gain and the Vref of the AFE module
 * 				  when the signal is into the limits of the ADC
 * Param		: 
 *				+ xSensor_Config: Cau truc chua cac thong tin cau hinh Sensor
 * Return		: 
 *				+ high nibble: steps changed in Vref
 * 				+ low nibble: steps changed in Rgain
*******************************************************************************/
uint8_t uSSCommon_AutoGain( Sensor_Config *xSensor_Config )
{
    float v_ADC = 0;
    float v_ref = 0;
    float upper_range = 0;
    float lower_range = 0;
    int r_gain;
    int vref_reg;
    int8_t r_steps = 0;
    int8_t v_steps = 0;
    bool end = false;
    uint8_t process_reg = 0;
    uint8_t mask;
    float aux_resistor;
    
    //Enable communication with the AFE
    ENABLE_I2C(xSensor_Config->I2C_gpio, xSensor_Config->I2C_pin);
    
    switch(xSensor_Config->sensor_type)
    {
        /***************************************
		*    3-ELECTRODE AMPERIOMETRIC BOARD	*
		***************************************/
		case CL2_SS:
		case CO_SS_SEC:
		case ETO_SS:
		case H2_SS_SEC:
		case H2S_SS_SEC:
		case HCL_SS:
		case HCN_SS:
		case NH3_SS:
		case NO_SS:
		case NO2_SS_CLE:
		case O2_SS:
		case PH3_SS:
		case SO2_SS:
		case CALIBRATION_3E:
            /*
            *	2048mV	|----------------------------------------	
            *		|					UHV						
            *		|----------------------------------------	(2048 - Vref) * MAX_RANGE_FACTOR
            *		|					UMV						
            *		|----------------------------------------	(2048 - Vref) * MIN_RANGE_FACTOR
            *		|					ULV						
            *	Vref	|----------------------------------------	
            *		|					LLV						
            *		|----------------------------------------	(0 - Vref) * MIN_RANGE_FACTOR
            *		|					LMV						
            *		|----------------------------------------	(0 - Vref) * MAX_RANGE_FACTOR
            *		|					LHV						
            *	0mV	|----------------------------------------	
            */
            r_steps = 0;
            v_steps = 0;
            MYTRACE("--- SSCommon: Starting autogain proccess for 3E\r\n");
            // Gets the Vref
            switch(uLMP91000_GetRefSource())
            {
				case 0:
					v_ref = 3300;
					break;
				case 1:
					v_ref = 2048;
					break;
            }
            
            // Gets the internal zero
            switch(uLMP91000_GetInternalZero())
            {
				case 0:
					v_ref *= 0.2;
					break;
				case 1:
					v_ref *= 0.5;
					break;
				case 2:
					v_ref *= 0.67;
					break;
				case 3:
					v_ref *= 1;
					break;
            }
            
            upper_range = 2048 - v_ref;
            lower_range = 0 - v_ref;
            MYTRACE("--- SSCommon: Upper limits: %f \r\n", upper_range);
            MYTRACE("--- SSCommon: Lower limits: %f \r\n", lower_range);
            
            do
			{
                // Reads from ADC
                v_ADC = fMCP3421_ReadADC(MCP3421_MEDIUM_RES, MCP3421_GAIN_1, MCP3421_VOLTS);
                v_ADC = fMCP3421_ReadADC(MCP3421_MEDIUM_RES, MCP3421_GAIN_1, MCP3421_VOLTS);
                v_ADC /= 2;
                MYTRACE("--- SSCommon: Vref: %f | ADC value: %f | ADC-vref value: %f \r\n", v_ref, v_ADC, v_ADC - v_ref);
                v_ADC = (v_ADC - v_ref); 	// VADC(mV) - Vzero(mV)
                
                if (v_ADC >= MAX_RANGE_FACTOR * upper_range)		// UHV
                {
                    process_reg &= 0x03;
                    mask = 1 << UHV;
                    process_reg |= mask;
                } 
                else if ((v_ADC < (MAX_RANGE_FACTOR * upper_range)) && (v_ADC >= (MIN_RANGE_FACTOR * upper_range)))	//UMV
                {
                    process_reg &= 0x03;
                    mask = 1 << UMV;
                    process_reg |= mask;
                } 
                else if ((v_ADC < (MIN_RANGE_FACTOR * upper_range)) && (v_ADC >= 0))	//ULV
                {
                    process_reg &= 0x03;
                    mask = 1 << ULV;
                    process_reg |= mask;					
                } 
                else if ((v_ADC < 0) && (v_ADC >= (MIN_RANGE_FACTOR * lower_range)))	//LLV
                {	
                    process_reg &= 0x03;
                    mask = 1 << LLV;
                    process_reg |= mask;	
                }
                else if ((v_ADC < (MIN_RANGE_FACTOR * lower_range)) && (v_ADC >= (MAX_RANGE_FACTOR * lower_range)))	//LMV
                {
                    process_reg &= 0x03;
                    mask = 1 << LMV;
                    process_reg |= mask;	
                } 	
                else	// LHV
                {
                    process_reg &= 0x03;
                    mask = 1 << LHV;
                    process_reg |= mask;	
                }
                
                r_gain = uLMP91000_GetRgain();
                if (r_gain == 7)
                {	
                    process_reg &= 0xFC;
                    process_reg |= 0x02;	
                }
                else if (r_gain == 1)
                {					
                    process_reg &= 0xFC;
                    process_reg |= 0x01;	
                }
                else
                {
                    process_reg &= 0xFC;
                }
                
                MYTRACE("--- SSCommon: process_reg: %f \r\n", process_reg);
                
                switch (process_reg)
                {
					case 0x80:	//Decreases gain
					case 0x82:
					case 0x04:
					case 0x06:						
						MYTRACE("--- SSCommon: Decresaing Rgain\r\n");
						r_gain--;
						eLMP91000_SetRgain(r_gain);
						r_steps--;
						end = false;
						break;
                    
					case 0x20:	//Increases gain
					case 0x21:
					case 0x10:
					case 0x11:
						MYTRACE("--- SSCommon: Incresaing Rgain \r\n");
						r_gain++;
						eLMP91000_SetRgain(r_gain);
						r_steps++;
						end = false;
						break;
                    
					case 0x81:	//Decreases Vref
						vref_reg = uLMP91000_GetInternalZero();
						if (vref_reg == 0)
						{
							end = true;
							MYTRACE("--- SSCommon: Vref low limit reached\r\n");
						}
						else
						{
							MYTRACE("--- SSCommon: Decresaing Vref \r\n");
							eLMP91000_SetInternalZero(vref_reg - 1);
							v_steps--;
						}			
						// Gets the Vref
						switch (uLMP91000_GetRefSource())
						{
							case 0:
								v_ref = 3300;
								break;
							case 1:
								v_ref = 2048;
								break;
						}
						
						switch (uLMP91000_GetInternalZero())
						{
							case 0:
								v_ref *= 0.2;
								break;
							case 1:
								v_ref *= 0.5;
								break;
							case 2:
								v_ref *= 0.67;
								break;
							case 3:
								v_ref *= 1;
								break;
						}
						
						upper_range = 2048 - v_ref;
						lower_range = 0 - v_ref;
						MYTRACE("--- SSCommon: Upper limits: %f \r\n", upper_range);
						MYTRACE("--- SSCommon: Lower limits: %f \r\n", lower_range);
						break;
                    
					case 0x05:	//Increases Vref
						vref_reg = uLMP91000_GetInternalZero();
						if (vref_reg == 2)
						{
							end = true;
							MYTRACE("--- SSCommon: Vref high limit reached \r\n");
						}
						else
						{
							MYTRACE("--- SSCommon: Incresaing Vref \r\n");
							eLMP91000_SetInternalZero(vref_reg + 1);
							v_steps++;
						}
						
						switch (uLMP91000_GetRefSource())
						{
							case 0:
								v_ref = 3300;
								break;
							case 1:
								v_ref = 2048;
								break;
						}
						
						switch (uLMP91000_GetInternalZero())
						{
							case 0:
								v_ref *= 0.2;
								break;
							case 1:
								v_ref *= 0.5;
								break;
							case 2:
								v_ref *= 0.67;
								break;
							case 3:
								v_ref *= 1;
								break;
						}
						
						upper_range = 2048 - v_ref;
						lower_range = 0 - v_ref;
						MYTRACE("--- SSCommon: Upper limits: %f \r\n", upper_range);
						MYTRACE("--- SSCommon: Lower limits: %f \r\n", lower_range);
						break;
                    
					case 0x40:	//End
					case 0x41:
					case 0x42:
					case 0x22:
					case 0x12:
					case 0x08:
					case 0x09:
					case 0x0A:
					default:
						end = true;
						break;
                }
            }while(end == false);
            /***************************************
            * 	        PELLISTOR BOARD		   *
            ***************************************/
		case LEL_AS:
		case CALIBRATION_PEL:
			break;
		case NDIR_CO2_SS:
		case CALIBRATION_NDIR:
			break;
        
        /***************************************
		*    4-ELECTRODE AMPERIOMETRIC BOARD	*
		***************************************/
		case O3_AS:
		case CALIBRATION_4E:
			r_steps = 0;
			v_steps = 0;
			MYTRACE("--- SSCommon: Starting autogain proccess for 4E \r\n");
			upper_range = 2048;
			lower_range = -2048;
			MYTRACE("--- SSCommon: Upper limits: %f \r\n", upper_range);
			MYTRACE("--- SSCommon: Lower limits: %f \r\n", lower_range);
			
			do
			{
				aux_resistor = fSSCommon_GetAmplifier(xSensor_Config, AUXILIARY_ELECTRODE);
				vSSCommon_SetAmplifier(xSensor_Config, AUXILIARY_ELECTRODE, 0);
				macroTASK_DELAY_MS( 10 );
				// Reads the ADC
				v_ADC = fMCP3421_ReadADC(MCP3421_MEDIUM_RES, MCP3421_GAIN_1, MCP3421_VOLTS);
				v_ADC = fMCP3421_ReadADC(MCP3421_MEDIUM_RES, MCP3421_GAIN_1, MCP3421_VOLTS);
				v_ADC /=2;
				vSSCommon_SetAmplifier(xSensor_Config, AUXILIARY_ELECTRODE, aux_resistor);
				MYTRACE("--- SSCommon: ADC value: %f \r\n", v_ADC);
				if (v_ADC >= MAX_RANGE_FACTOR * upper_range)		// UHV
				{
					process_reg &= 0x03;
					mask = 1 << UHV;
					process_reg |= mask;
				} 
				else if ((v_ADC < (MAX_RANGE_FACTOR * upper_range)) && (v_ADC >= (MIN_RANGE_FACTOR * upper_range)))	//UMV
				{
					process_reg &= 0x03;
					mask = 1 << UMV;
					process_reg |= mask;
				} 
				else if ((v_ADC < (MIN_RANGE_FACTOR * upper_range)) && (v_ADC >= 0))	//ULV
				{
					process_reg &= 0x03;
					mask = 1 << ULV;
					process_reg |= mask;					
				} 
				else if ((v_ADC < 0) && (v_ADC >= (MIN_RANGE_FACTOR * lower_range)))	//LLV
				{	
					process_reg &= 0x03;
					mask = 1 << LLV;
					process_reg |= mask;	
				}
				else if ((v_ADC < (MIN_RANGE_FACTOR * lower_range)) && (v_ADC >= (MAX_RANGE_FACTOR * lower_range)))	//LMV
				{
					process_reg &= 0x03;
					mask = 1 << LMV;
					process_reg |= mask;	
				} 	
				else	// LHV
				{
					process_reg &= 0x03;
					mask = 1 << LHV;
					process_reg |= mask;	
				}
				
				aux_resistor = fSSCommon_GetAmplifier(xSensor_Config, WORKING_ELECTRODE);
				aux_resistor = round( aux_resistor / 10000) * 10000;
				if (aux_resistor == 100000.0)
				{	
					process_reg &= 0xFC;
					process_reg |= 0x02;	
					aux_resistor = 100000;
				}
				else if (aux_resistor == 80000.0)
				{
					aux_resistor = 80000;
				}
				else if (aux_resistor == 60000.0)
				{
					aux_resistor = 60000;
				}
				else if (aux_resistor == 40000.0)
				{
					aux_resistor = 40000;
				}
				else if (aux_resistor == 20000.0)
				{					
					process_reg &= 0xFC;
					process_reg |= 0x01;	
					aux_resistor = 20000;
				}
				else
				{
					process_reg &= 0xFC;
				}
				
				MYTRACE("--- SSCommon: process_reg: %f \r\n", process_reg);

				switch (process_reg)
				{
					case 0x80:	//Decreases gain
					case 0x82:
					case 0x04:
					case 0x06:	
						MYTRACE("--- SSCommon: Decresaing Rgain \r\n");
						aux_resistor -= 20000;
						vSSCommon_SetAmplifier(xSensor_Config, WORKING_ELECTRODE, aux_resistor);
						vSSCommon_SetAmplifier(xSensor_Config, AUXILIARY_ELECTRODE, aux_resistor);
						r_steps--;
						end = 0;
						break;
						
					case 0x20:	//Increases gain
					case 0x21:
					case 0x10:
					case 0x11:
						MYTRACE("--- SSCommon: Incresaing Rgain \r\n");
						aux_resistor += 20000;
						vSSCommon_SetAmplifier(xSensor_Config, WORKING_ELECTRODE, aux_resistor);
						vSSCommon_SetAmplifier(xSensor_Config, AUXILIARY_ELECTRODE, aux_resistor);
						r_steps++;
						end = 0;
						break;
					
					case 0x81:	//Decreases Vref
						vref_reg = uLMP91000_GetInternalZero();
						if (vref_reg == 0)
						{
							end = 1;
							MYTRACE("--- SSCommon: Vref low limit reached \r\n");
						}
						else
						{
							MYTRACE("--- SSCommon: Decresaing Vref \r\n");
							eLMP91000_SetInternalZero(vref_reg - 1);
							v_steps--;
						}
						break;
					
					case 0x05:	//Increases Vref
						vref_reg = uLMP91000_GetInternalZero();
						if (vref_reg == 2)
						{
							end = 1;
							MYTRACE("--- SSCommon: Vref high limit reached \r\n");
						}
						else
						{
							MYTRACE("--- SSCommon: Incresaing Vref \r\n");
							eLMP91000_SetInternalZero(vref_reg + 1);
							v_steps++;
						}
						break;
					
					case 0x40:	//End
					case 0x41:
					case 0x42:
					case 0x22:
					case 0x12:
					case 0x08:
					case 0x09:
					case 0x0A:
					default:
						end = 1;
						break;
				}
			}while(end == 0);	
			break;
    }
    
    // Disable communication
    DISABLE_I2C(xSensor_Config->I2C_gpio, xSensor_Config->I2C_pin);
    MYTRACE("--- SSCommon: End of autogain proccess \r\n");
    return ((v_steps << 4) & 0xF0) + (r_steps & 0x0F);
}




/******************************************************************************
 * Function		: float fSSCommon_PPM2perc( float fPPM_Conc )
 * Description	: This function converts concentration in ppm to %
 * Param		: 
 *				+ fPPM_Conc: concentration in ppm
 * Return		: The concetration value in %
*******************************************************************************/
float fSSCommon_PPM2perc( float fPPM_Conc )
{
    return (fPPM_Conc / 10000);
}



/******************************************************************************
 * Function		: float fSSCommon_PPM2ugm3(sensor_config *sensor_config, float fPPM_Conc)
 * Description	: This function converts concentration in ppm to ugm3
 * Param		: 
 *				+ fPPM_Conc: concentration in ppm
 * Return		: The concetration value in %
*******************************************************************************/
float fSSCommon_PPM2ugm3(Sensor_Config *xSensor_Config, float fPPM_Conc)
{
	float fAux_Val;
	fAux_Val = fTable_PPM2ugm3[xSensor_Config->sensor_type];
	return (fPPM_Conc * fAux_Val);
	
}



/******************************************************************************
 * Function		: float fSSCommon_Celsius2Fahrenheit(float fTemp)
 * Description	: This function converts temperature in Celsius degrees to Fahrenheit degrees
 * Param		: 
 *				+ fTemp: concentration in Celsius degrees
 * Return		: The concetration value in Fahrenheit degrees
*******************************************************************************/
float fSSCommon_Celsius2Fahrenheit(float fTemp)
{
	return ((1.8 * fTemp) + 32);	
}



/******************************************************************************
 * Function		: Enum_SensorStatus eSSCommon_SetPowerMode( uint8_t uPower_Mode )
 * Description	: This function sets the power mode of LMP91000
 * Param		: 
*				+ uPower_Mode: working mode for LMP91000
 * Return		: 
 *				+ eSS_OK: Successfully
 *				+ eSS_Fail: fail
*******************************************************************************/
Enum_SensorStatus eSSCommon_SetPowerMode( uint8_t uPower_Mode )
{
    return eLMP91000_SetMODEReg( LMP91000_MODEC_REG_FET_NOT_SHORTED, uPower_Mode );
}




/******************************************************************************
 * Function		: Enum_SensorStatus eSSCommon_PowerOn( sensor_config *xSensor_Config )
 * Description	: Ham on power tren sensor
 * Param		: 
 *				+ xSensor_Config: Cau truc chua cac thong tin cau hinh Sensor
 * Return		: 
 *				+ eSS_OK: Successfully
 *				+ eSS_Fail: fail
*******************************************************************************/
Enum_SensorStatus eSSCommon_PowerOn( Sensor_Config *xSensor_Config )
{
    Enum_SensorStatus eStatus;
	
	if(xSensor_Config->isConnected == false)
		return eSS_Fail;

    /* enable power_pin */
    ENABLE_POWER(xSensor_Config->power_gpio, xSensor_Config->power_pin);
    /* enable i2c_pin */
    ENABLE_I2C(xSensor_Config->I2C_gpio, xSensor_Config->I2C_pin);
    macroTASK_DELAY_MS( 10 );
    /* read infor sensor */
    eStatus = eSSCommon_ReadSensorInfo(xSensor_Config);
    if(eStatus == eSS_Fail) 
	{
		APP_DEBUG("--- SensorTask: Read sensor info fail\r\n");
		return eSS_Fail;
	}
    
    /* show info sensor to UART */
#ifdef DEBUG
    SSCommon_ShowSensorInfo(xSensor_Config);
#endif
    macroTASK_DELAY_MS( 1000 );
    eStatus = eSSCommon_ConfigureAFE( xSensor_Config, -1000 );
    if(eStatus != eSS_OK)
        APP_DEBUG("--- SensorTask: Error config AFE module\r\n");
    
    return eStatus;
}



/******************************************************************************
 * Function		: Enum_SensorStatus eSSCommon_PowerOff( sensor_config *xSensor_Config, uint8_t uEnable_FET )
 * Description	: Ham off power tren sensor
 * Param		: 
 *				+ xSensor_Config: Cau truc chua cac thong tin cau hinh Sensor
 *				+ uEnable_FET: N/A
 * Return		: 
 *				+ eSS_OK: Successfully
 *				+ eSS_Fail: fail
*******************************************************************************/
Enum_SensorStatus eSSCommon_PowerOff( Sensor_Config *xSensor_Config, uint8_t uEnable_FET )
{
    // Enable communication with the AFE
    ENABLE_I2C(xSensor_Config->I2C_gpio, xSensor_Config->I2C_pin);
    
    if( uEnable_FET == 1 )
        eLMP91000_SetMODEReg( LMP91000_MODEC_REG_FET_SHORTED, LMP91000_MODEC_REG_PWR_MODE_DEEP_SLEEP );
    else
        eLMP91000_SetMODEReg( LMP91000_MODEC_REG_FET_NOT_SHORTED, LMP91000_MODEC_REG_PWR_MODE_DEEP_SLEEP);
    
    // Disable communication
    DISABLE_I2C(xSensor_Config->I2C_gpio, xSensor_Config->I2C_pin);
    // Power off the AFE module
    DISABLE_POWER(xSensor_Config->power_gpio, xSensor_Config->power_pin);
    
    return eSS_OK;
}






