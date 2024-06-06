
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "fsl_gpio.h"  
#include "fsl_port.h"

#include "SPI.h"


#define macroSPI_BUFFER					128
  

#define DSPI_MASTER_BASEADDR0 			((SPI_Type *)SPI0_BASE)
#define DSPI_MASTER_BASEADDR1 			((SPI_Type *)SPI1_BASE)



/******************************************************************************
 * Function		: Enum_Result eSPI_InitModule0(dspi_rtos_handle_t *xSPI_Handle, uint32_t ulBaudrate)
 * Description	: Ham khoi tao SPI0
 * Param		: Baudrate - toc do truyen nhan du lieu
 * Return		: 
 *              + Successfully: eSUCCESS
 *              + Failure: eFAILURE
*******************************************************************************/
Enum_Result eSPI_InitModule0(dspi_rtos_handle_t *xSPI_Handle, uint32_t ulBaudrate)
{
	uint32_t ulSourceClock;
    status_t eStatus;
    dspi_master_config_t xMasterConfig;
	
	// Init module SPI
    /* Master config */
    xMasterConfig.whichCtar = kDSPI_Ctar0;
    xMasterConfig.ctarConfig.baudRate = ulBaudrate;
    xMasterConfig.ctarConfig.bitsPerFrame = 8;
    xMasterConfig.ctarConfig.cpol = kDSPI_ClockPolarityActiveHigh;
    xMasterConfig.ctarConfig.cpha = kDSPI_ClockPhaseSecondEdge;
    //xMasterConfig.ctarConfig.cpha = kDSPI_ClockPhaseFirstEdge;
    xMasterConfig.ctarConfig.direction = kDSPI_MsbFirst;
    xMasterConfig.ctarConfig.pcsToSckDelayInNanoSec = 0;
    xMasterConfig.ctarConfig.lastSckToPcsDelayInNanoSec = 0;
    xMasterConfig.ctarConfig.betweenTransferDelayInNanoSec = 0;
    
    xMasterConfig.whichPcs = kDSPI_Pcs0;
    xMasterConfig.pcsActiveHighOrLow = kDSPI_PcsActiveLow;
    
    xMasterConfig.samplePoint = kDSPI_SckToSin0Clock;
    xMasterConfig.enableContinuousSCK = false;
    xMasterConfig.enableRxFifoOverWrite = false;
    xMasterConfig.enableModifiedTimingFormat = false;
    
	NVIC_SetPriority(SPI0_IRQn, 6);
	ulSourceClock = CLOCK_GetFreq(DSPI0_CLK_SRC);
	eStatus = DSPI_RTOS_Init(xSPI_Handle, DSPI_MASTER_BASEADDR0, &xMasterConfig, ulSourceClock);

    if (eStatus != kStatus_Success)
    {
        APP_DEBUG("->> DSPI master: error during initialization0. \r\n");
		return eFAILURE;
    }
	return eSUCCESS;
}




/******************************************************************************
 * Function		: Enum_Result eSPI_InitModule1(dspi_rtos_handle_t *xSPI_Handle, uint32_t ulBaudrate)
 * Description	: Ham khoi tao SPI1
 * Param		: Baudrate - toc do truyen nhan du lieu
 * Return		: 
 *              + Successfully: eSUCCESS
 *              + Failure: eFAILURE
*******************************************************************************/
Enum_Result eSPI_InitModule1(dspi_rtos_handle_t *xSPI_Handle, uint32_t ulBaudrate)
{
	uint32_t ulSourceClock;
    status_t eStatus;
    dspi_master_config_t xMasterConfig;
	
	// Init module SPI
    /* Master config */
    xMasterConfig.whichCtar = kDSPI_Ctar0;
    xMasterConfig.ctarConfig.baudRate = ulBaudrate;
    xMasterConfig.ctarConfig.bitsPerFrame = 8;
    xMasterConfig.ctarConfig.cpol = kDSPI_ClockPolarityActiveHigh;
    //xMasterConfig.ctarConfig.cpha = kDSPI_ClockPhaseSecondEdge;
    xMasterConfig.ctarConfig.cpha = kDSPI_ClockPhaseFirstEdge;
    xMasterConfig.ctarConfig.direction = kDSPI_MsbFirst;
    xMasterConfig.ctarConfig.pcsToSckDelayInNanoSec = 0;
    xMasterConfig.ctarConfig.lastSckToPcsDelayInNanoSec = 0;
    xMasterConfig.ctarConfig.betweenTransferDelayInNanoSec = 0;
    
    xMasterConfig.whichPcs = kDSPI_Pcs0;
    xMasterConfig.pcsActiveHighOrLow = kDSPI_PcsActiveLow;
    
    xMasterConfig.samplePoint = kDSPI_SckToSin0Clock;
    xMasterConfig.enableContinuousSCK = false;
    xMasterConfig.enableRxFifoOverWrite = false;
    xMasterConfig.enableModifiedTimingFormat = false;
    
	NVIC_SetPriority(SPI1_IRQn, 6);
	ulSourceClock = CLOCK_GetFreq(DSPI1_CLK_SRC);
	eStatus = DSPI_RTOS_Init(xSPI_Handle, DSPI_MASTER_BASEADDR1, &xMasterConfig, ulSourceClock);

    if (eStatus != kStatus_Success)
    {
        APP_DEBUG("->> DSPI master: error during initialization0. \r\n");
		return eFAILURE;
    }
	return eSUCCESS;
}



/******************************************************************************
 * Function		: uint8_t uSPI_ReceiveByte( dspi_rtos_handle_t xSPI_Handle, uint8_t uReg )
 * Description	: Ham doc 1 byte qua SPI
 * Param		: 
 *				+ uReg - thanh ghi doc du lieu
 *				+ xSPI_Handle - Spi direct
 * Return		: gia tri thanh ghi tra ve
*******************************************************************************/
uint8_t uSPI_ReceiveByte( dspi_rtos_handle_t *xSPI_Handle, uint8_t uReg )
{
    uint8_t uCharSend[2];
    uint8_t uCharReceive[2];
    dspi_transfer_t xMasterXfer0;
    
    memset( uCharSend, 0x00, sizeof(uCharSend) );
    memset( uCharReceive, 0x00, sizeof(uCharReceive) );
    
    uCharSend[0] = uReg;
    uCharSend[1] = 0x00;
    
    xMasterXfer0.txData = uCharSend;
    xMasterXfer0.rxData = uCharReceive;
    xMasterXfer0.dataSize = 1;
    xMasterXfer0.configFlags = kDSPI_MasterCtar0 | kDSPI_MasterPcs0 | kDSPI_MasterPcsContinuous;
    
    DSPI_RTOS_Transfer(xSPI_Handle, &xMasterXfer0);
    
    return uCharReceive[0];
}




/******************************************************************************
 * Function		: void vSPI_ReceiveBytes( dspi_rtos_handle_t *xSPI_Handle, uint8_t uReg, uint8_t *uBuffer, uint8_t uLenght)
 * Description	: Ham doc n byte qua SPI
 * Param		: 
 *				+ uReg - thanh ghi doc du lieu
 *				+ xSPI_Handle - Spi direct
 *				+ uBuffer: buffer doc du lieu
 *				+ uLenght: do dai du lieu duoc doc
 * Return		: none
*******************************************************************************/
void vSPI_ReceiveBytes( dspi_rtos_handle_t *xSPI_Handle, uint8_t uReg, uint8_t *uBuffer, uint8_t uLenght)
{
	uint8_t uCharSend[2] = {0};
    dspi_transfer_t xMasterXfer0;
    
    memset( uBuffer, 0x00, sizeof(uBuffer) );
    
    uCharSend[0] = uReg;
    xMasterXfer0.txData = uCharSend;
    xMasterXfer0.rxData = uBuffer;
    xMasterXfer0.dataSize = 1;
    xMasterXfer0.configFlags = kDSPI_MasterCtar0 | kDSPI_MasterPcs0 | kDSPI_MasterPcsContinuous;
    
    status_t eStatus = DSPI_RTOS_Transfer(xSPI_Handle, &xMasterXfer0);
	
	uCharSend[0] = 0x00;
	xMasterXfer0.txData = uCharSend;
    xMasterXfer0.rxData = uBuffer;
    xMasterXfer0.dataSize = uLenght;
    xMasterXfer0.configFlags = kDSPI_MasterCtar0 | kDSPI_MasterPcs0 | kDSPI_MasterPcsContinuous;
    
    eStatus = DSPI_RTOS_Transfer(xSPI_Handle, &xMasterXfer0);
	
	if(eStatus != kStatus_Success)
	{
		APP_DEBUG("--- SPI: Transfer error\r\n");
	}
}





