
#ifndef __SPI_H
#define __SPI_H

#include "main.h"
#include "fsl_dspi_freertos.h"



#define macroSPI_BAUDRATE_SENSOR         			(100000U)
#define macroSPI_BAUDRATE_FLASH         			(500000U)


/******************************************************************************
 * Function		: Enum_Result eSPI_InitModule0(dspi_rtos_handle_t *xSPI_Handle, uint32_t ulBaudrate)
 * Description	: Ham khoi tao SPI
 * Param		: Baudrate - toc do truyen nhan du lieu
 * Return		: 
 *              + Successfully: eSUCCESS
 *              + Failure: eFAILURE
*******************************************************************************/
Enum_Result eSPI_InitModule0(dspi_rtos_handle_t *xSPI_Handle, uint32_t ulBaudrate);



/******************************************************************************
 * Function		: Enum_Result eSPI_InitModule1(dspi_rtos_handle_t *xSPI_Handle, uint32_t ulBaudrate)
 * Description	: Ham khoi tao SPI1
 * Param		: Baudrate - toc do truyen nhan du lieu
 * Return		: 
 *              + Successfully: eSUCCESS
 *              + Failure: eFAILURE
*******************************************************************************/
Enum_Result eSPI_InitModule1(dspi_rtos_handle_t *xSPI_Handle, uint32_t ulBaudrate);



/******************************************************************************
 * Function		: uint8_t uSPI_ReceiveByte( dspi_rtos_handle_t *xSPI_Handle, uint8_t uReg )
 * Description	: Ham doc 1 byte qua SPI
 * Param		: 
 *				+ uReg - thanh ghi doc du lieu
 *				+ xSPI_Handle - Spi direct
 * Return		: gia tri thanh ghi tra ve
*******************************************************************************/
uint8_t uSPI_ReceiveByte( dspi_rtos_handle_t *xSPI_Handle, uint8_t uReg );



/******************************************************************************
 * Function		: void vSPI_ReceiveBytes( dspi_rtos_handle_t *xSPI_Handle, uint8_t uReg, uint8_t *uBuffer, uint8_t uLenght)
 * Description	: Ham doc 1 byte qua SPI
 * Param		: 
 *				+ uReg - thanh ghi doc du lieu
 *				+ xSPI_Handle - Spi direct
 *				+ uBuffer: buffer doc du lieu
 *				+ uLenght: do dai du lieu duoc doc
 * Return		: none
*******************************************************************************/
void vSPI_ReceiveBytes( dspi_rtos_handle_t *xSPI_Handle, uint8_t uReg, uint8_t *uBuffer, uint8_t uLenght);



#endif




