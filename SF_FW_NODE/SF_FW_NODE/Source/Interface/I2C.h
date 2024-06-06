
#ifndef __I2C_H
#define __I2C_H

#include "fsl_i2c.h"
#include "fsl_i2c_freertos.h"

#include "main.h"


#define macroI2C_DATA_MAX_LENGTH                 	32

//Define i2c for sensor gas
#define macroI2C_MASTER_BASE_BH1750				I2C0_BASE
#define macroI2C_BAUDRATE_BH1750				(400000) /* 100k */


/*******************************************************************************
 * Function     : void vI2C_InitModule( i2c_rtos_handle_t *xI2C_Rtos_Handle )
 * Description  : Khoi tao module I2C 0
 * Param        : none
 * Return       : none
*******************************************************************************/
void vI2C_InitModule( i2c_rtos_handle_t *xI2C_Rtos_Handle, uint32_t ulI2C_Base, uint32_t ulBaudrate );



/*******************************************************************************
 * Function     : uint8_t uI2C_WriteBits( i2c_rtos_handle_t *xI2C_Rtos_Handle, uint8_t uI2C_Device_Address, uint8_t uReg, uint8_t uValue,uint8_t uPosition, uint8_t uLength )
 * Description  : sends an I2C command to the specified slave
 * Param        :
 *              + uI2C_Device_Address: Dia chi i2c cua cam bien
 *              + uReg: thanh ghi ghi du lieu
 *				+ uValue: gia tri ghi vao thanh ghi
 *				+ uPosition: Vi tri bit duoc ghi
 *				+ uLength: so bit duoc ghi
 * Return       : 
 *              + Successfully: 1
 *              + Failure: 0
*******************************************************************************/
uint8_t uI2C_WriteBits( i2c_rtos_handle_t *xI2C_Rtos_Handle, uint8_t uI2C_Device_Address, uint8_t uReg, uint8_t uValue,uint8_t uPosition, uint8_t uLength );



/*******************************************************************************
 * Function     : uint8_t uI2C_Read( i2c_rtos_handle_t *xI2C_Master_Handle, uint8_t uAddress, uint8_t uReg, uint8_t *puData, uint8_t uLenght )
 * Description  : Ham doc du lieu tu I2C
 * Param        :
 *				+ uAddress: Dia chi cua slave
 *              + uReg: Dia chi cua lenh
 *				+ puData: bo dem luu du lieu doc ra
 *				+ uLenght: do dai du lieu duoc doc
 * Return       :
 *              + Successfully: 1
 *              + Failure: 0
*******************************************************************************/
uint8_t uI2C_Read( i2c_rtos_handle_t *xI2C_Master_Handle, uint8_t uAddress, uint8_t uReg, uint8_t *puData, uint8_t uLenght );




/*******************************************************************************
 * Function     : uint8_t uI2C_Write( i2c_rtos_handle_t *xI2C_Master_Handle, uint8_t uAddress, uint8_t uReg, uint8_t *puData, uint8_t uLenght )
 * Description  : lay trang thai cua cac lenh control trong sensor
 * Param        :
 *              + uReg: Dia chi cua lenh
 *				+ puData: data ghi vao
 *				+ uLenght: do dai du lieu duoc ghi
 * Return       :
 *              + Successfully: 1
 *              + Failure: 0
*******************************************************************************/
uint8_t uI2C_Write( i2c_rtos_handle_t *xI2C_Master_Handle, uint8_t uAddress, uint8_t uReg, uint8_t *puData, uint8_t uLenght );




/*******************************************************************************
 * Function     : uint8_t uI2C_WriteBit( i2c_rtos_handle_t *xI2C_Master_Handle, uint8_t uAddress, uint8_t uReg, uint8_t uData, uint8_t uPos )
 * Description  : lay trang thai cua cac lenh control trong sensor
 * Param        :
 *              + uReg: Dia chi cua lenh
 *				+ uData: data ghi vao
 *				+ uPos: vi tri cua bit ghi vao
 * Return       :
 *              + Successfully: 1
 *              + Failure: 0
*******************************************************************************/
uint8_t uI2C_WriteBit( i2c_rtos_handle_t *xI2C_Master_Handle, uint8_t uAddress, uint8_t uReg, uint8_t uData, uint8_t uPos );
	 


#endif











