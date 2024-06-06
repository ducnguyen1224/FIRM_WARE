
#ifndef __SSCOMMON_H
#define __SSCOMMON_H

#include "main.h"
#include "fsl_gpio.h"  


#define macroI2C_ADRESS_MCP4146      		0x2A
#define macroI2C_ADRESS_E2PROM       		0x50


//! EEPROM memory address
//Common
#define VER_BOARD_REG		        0x00	// 1 Byte
#define SENSOR_TYPE_REG		        0x01	// 1 Byte
#define SENSOR_CHECKSUM		        0x06	// 1 Byte
#define SENSOR_NO_REG		        0x70	// 16 Bytes

//3 electrode board
#define SENSITIVITY_REG		        0x02	// 4 Bytes
#define OFFSET_REG					0x08	// 4 Bytes
#define CAL_REG						0x10	// 64 Bytes

//4 electrode board
#define SENS_WE_REG					0x02	// 4 Bytes
#define WE_OFFSET_REG		        0x08	// 4 Bytes
#define SENS_AE_REG					0x0C	// 4 Bytes
#define CAL_WE_REG					0x10	// 16 Bytes
#define CAL_AE_REG					0x18	// 16 Bytes
#define CAL_WE_EXT_REG		        0x20	// 16 Bytes
#define CAL_AE_EXT_REG		        0x28	// 16 Bytes
#define AE_OFFSET_REG		        0x30	// 4 Bytes

// pellistor/CO2 board
#define CAL_1_REG					0x10	// 4 Bytes
#define CAL_2_REG					0x18	// 4 Bytes


//!***************************************************************************
//!	MCP4146 defines
//!***************************************************************************
#define VOL_WIPER_0_REG				0x00
#define VOL_WIPER_1_REG				0x10
#define NVOL_WIPER_0_REG			0x20
#define NVOL_WIPER_1_REG			0x30
#define VOL_TCON_REG				0x40
#define MCP_STATUS_REG				0x50
#define EEPROM_0_REG				0x60
#define EEPROM_1_REG				0x70
#define EEPROM_2_REG				0x80
#define EEPROM_3_REG				0x90
#define EEPROM_4_REG				0xA0
#define EEPROM_5_REG				0xB0
#define EEPROM_6_REG				0xC0
#define EEPROM_7_REG				0xD0
#define EEPROM_8_REG				0xE0
#define EEPROM_9_REG				0xF0

#define WRITE_COMMAND				0x00
#define INC_COMMAND					0x04
#define DEC_COMMAND					0x08
#define READ_COMMAND				0x0C

#define MCP_GAIN_0					0
#define MCP_GAIN_1					20000
#define MCP_GAIN_2					40000
#define MCP_GAIN_3					60000
#define MCP_GAIN_4					80000
#define MCP_GAIN_5					100000
#define MCP_GAIN_6					1000000

//!***************************************************************************
//!	Sensor defines
//!***************************************************************************
// SOLIDSENSE gas sensors
#define CL2_SS						0	// OK
#define CO_SS_SEC					1	// OK	Vbias: 0V
#define ETO_SS						2	// OK
#define H2_SS_SEC					3	// OK	Vbias: 0V
#define H2S_SS_SEC					4	// OK	Vbias: 0V
#define HCL_SS						5	// OK
#define HCN_SS						6	// OK
#define NH3_SS						7	// OK
#define NO_SS						8	// OK
#define NO2_SS_CLE					9	// OK
#define O2_SS						10	// OK	Vbias: -600mV
#define PH3_SS						11	// OK
#define SO2_SS						12	// OK

// ALPHASENSE gas sensors
#define LEL_AS						13	// Pellistor
#define O3_AS						14	// 4 electrode Vbias: 0V
#define NO2_AS						16	// 4 electrode Vbias: 0V
#define NO_AS						17	// 4 electrode Vbias: +200mV
#define SO2_AS						18	// 4 electrode Vbias: 0V
#define CO_AS						19	// 4 electrode Vbias: 0V

// SOLIDSENSE NDIR
#define NDIR_CO2_SS	        		15	// CO2 NDIR

#define CALIBRATION_NDIR			251
#define CALIBRATION_3E				252
#define CALIBRATION_4E				253
#define CALIBRATION_PEL				254
#define UNDEFINED_SENSOR			255


//!***************************************************************************
//!	Miscelaneus defines
//!***************************************************************************
#define WORKING_ELECTRODE			0
#define AUXILIARY_ELECTRODE			1
#define COMPENSATED					2


//!***************************************************************************
//!	AutoGain function defines
//!***************************************************************************
#define UHV 				7	//Upper High Voltage
#define UMV 				6	//Upper Medium Voltage
#define ULV 				5	//Upper Low Voltage
#define LLV 				4	//Lower Low Voltage
#define LMV 				3	//Lower Medium Voltage
#define LHV 				2	//Lower High Voltage
#define URG 				1	//Upper Resisitor Gain
#define LRG 				0	//Lower Resisitor Gain

#define MAX_RANGE_FACTOR	0.85
#define MIN_RANGE_FACTOR	0.15


#define ENABLE_I2C(PORT, PIN) 					GPIO_WritePinOutput(PORT, PIN, 1)
#define DISABLE_I2C(PORT, PIN) 					GPIO_WritePinOutput(PORT, PIN, 0)
#define ENABLE_POWER(PORT, PIN) 				GPIO_WritePinOutput(PORT, PIN, 0)
#define DISABLE_POWER(PORT, PIN) 				GPIO_WritePinOutput(PORT, PIN, 1)


typedef enum
{
	eSS_OK,
	eSS_Fail,
}Enum_SensorStatus;


typedef struct
{
	bool isConnected;
	bool isConfigLoaded;
	
	char cName[8];
	
    uint8_t socket;             // Board socket
    uint8_t sensor_type;        // Gas sensor number
    uint8_t power_pin;          // GPIO of power pin
    PORT_Type *power_port;
    GPIO_Type *power_gpio;
    uint8_t I2C_pin;            // GPIO of I2C pin
    PORT_Type *I2C_port;
    GPIO_Type *I2C_gpio;
    float m_conc;               // extra variable. nA/ppm, only for 4 electrode sensors (O3)
    float baseline;             // nA or mV
    float aux_baseline;         // nA, offset for auxiliary electrode
    float OX_NO2_sens;			// nA/ppm
    float calibration[8][2];    // compensation value for AFE modules
    uint8_t AFE_ver;			// AFE board version
}Sensor_Config;


/******************************************************************************
 * Function		: float fSSCommon_GetBaseLineTempComp( sensor_config *xSensor_Config, float fTemperature )
 * Description	: This function generates the baseline compensation for the temperature in ppm
 * Param		: 
 *				+ xSensor_Config: Cau truc chua cac thong tin cau hinh Sensor
 *				+ fTemperature: ambient temperature for sensor compensation (-1000 if doesn't needed)
 * Return		: ppm value for compensation
*******************************************************************************/
float fSSCommon_GetBaseLineTempComp( Sensor_Config *xSensor_Config, float fTemperature );




/******************************************************************************
 * Function		: float fSSCommon_GetSensitivityTempComp( sensor_config *xSensor_Config, float fTemperature )
 * Description	: This function generates the output current compensation for the temperaturem
 * Param		: 
 *				+ xSensor_Config: Cau truc chua cac thong tin cau hinh Sensor
 *				+ fTemperature: ambient temperature for sensor compensation (-1000 if doesn't needed)
 * Return		: compensation factor
*******************************************************************************/
float fSSCommon_GetSensitivityTempComp( Sensor_Config *xSensor_Config, float fTemperature );



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
void vSSCommon_SetAmplifier( Sensor_Config *xSensor_Config, bool bElectrode, float fResister_ );



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
float fSSCommon_GetAmplifier( Sensor_Config *xSensor_Config, bool bElectrode );



/******************************************************************************
 * Function		: Enum_Status eSSCommon_ReadSensorInfo( sensor_config *xSensor_Config )
 * Description	: This function reads the sensor information from EEPROM
 * Param		: 
 *				+ xSensor_Config: Cau truc chua cac thong tin cau hinh Sensor
 * Return		: 
 *              + Successfully: eSS_OK
 *              + Failure: eSS_Fail
*******************************************************************************/
Enum_SensorStatus eSSCommon_ReadSensorInfo( Sensor_Config *xSensor_Config );



/******************************************************************************
 * Function		: Enum_Status eSSCommon_ConfigureAFE( sensor_config *xSensor_Config, float fR_gain )
 * Description	: This function configure the AFE module (LMP91000 and/or MCP4146)
 * Param		: 
 *				+ xSensor_Config: Cau truc chua cac thong tin cau hinh Sensor
 *				+ fR_gain: resistor value for transimpendance stage
 * Return		: 
 *              + Successfully: eSS_OK
 *              + Failure: eSS_Fail
*******************************************************************************/
Enum_SensorStatus eSSCommon_ConfigureAFE( Sensor_Config *xSensor_Config, float fR_gain );



/******************************************************************************
 * Function		: void SSCommon_ShowSensorInfo( sensor_config *xSensor_Config ))
 * Description	: This function shows the information about the sensor
 * Param		: 
 *				+ xSensor_Config: Cau truc chua cac thong tin cau hinh Sensor
 * Return		: none
*******************************************************************************/
void SSCommon_ShowSensorInfo( Sensor_Config *xSensor_Config );



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
uint8_t uSSCommon_AutoGain( Sensor_Config *xSensor_Config );
	 


/******************************************************************************
 * Function		: float fSSCommon_PPM2perc( float fPPM_Conc )
 * Description	: This function converts concentration in ppm to %
 * Param		: 
 *				+ ppm_conc: concentration in ppm
 * Return		: The concetration value in %
*******************************************************************************/
float fSSCommon_PPM2perc( float fPPM_Conc );


/******************************************************************************
 * Function		: float fSSCommon_PPM2ugm3(sensor_config *sensor_config, float fPPM_Conc)
 * Description	: This function converts concentration in ppm to ugm3
 * Param		: 
 *				+ fPPM_Conc: concentration in ppm
 * Return		: The concetration value in %
*******************************************************************************/
float fSSCommon_PPM2ugm3(Sensor_Config *xSensor_Config, float fPPM_Conc);



/******************************************************************************
 * Function		: float fSSCommon_Celsius2Fahrenheit(float fTemp)
 * Description	: This function converts temperature in Celsius degrees to Fahrenheit degrees
 * Param		: 
 *				+ fTemp: concentration in Celsius degrees
 * Return		: The concetration value in Fahrenheit degrees
*******************************************************************************/
float fSSCommon_Celsius2Fahrenheit(float fTemp);



/******************************************************************************
 * Function		: Enum_SensorStatus eSSCommon_SetPowerMode( uint8_t uPower_Mode )
 * Description	: This function sets the power mode of LMP91000
 * Param		: 
*				+ uPower_Mode: working mode for LMP91000
 * Return		: 
 *				+ eSS_OK: Successfully
 *				+ eSS_Fail: fail
*******************************************************************************/
Enum_SensorStatus eSSCommon_SetPowerMode( uint8_t uPower_Mode );



/******************************************************************************
 * Function		: Enum_SensorStatus eSSCommon_PowerOn( sensor_config *xSensor_Config )
 * Description	: Ham on power tren sensor
 * Param		: 
 *				+ xSensor_Config: Cau truc chua cac thong tin cau hinh Sensor
 * Return		: 
 *				+ eSS_OK: Successfully
 *				+ eSS_Fail: fail
*******************************************************************************/
Enum_SensorStatus eSSCommon_PowerOn( Sensor_Config *xSensor_Config );



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
Enum_SensorStatus eSSCommon_PowerOff( Sensor_Config *xSensor_Config, uint8_t uEnable_FET );


#endif



