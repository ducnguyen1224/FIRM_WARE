

#include "I2C.h"

#define macroI2C_MASTER_MODULE0              ((I2C_Type *)I2C0_BASE)
#define macroI2C_MASTER_MODULE1              ((I2C_Type *)I2C1_BASE)
#define macroI2C_MASTER_MODULE2              ((I2C_Type *)I2C2_BASE)

#define macroI2C_MASTER_CLK_SRC0             (I2C0_CLK_SRC)
#define macroI2C_MASTER_CLK_SRC1             (I2C1_CLK_SRC)
#define macroI2C_MASTER_CLK_SRC2             (I2C2_CLK_SRC)


/*******************************************************************************
 * Function     : void vI2C_InitModule( i2c_rtos_handle_t *xI2C_Rtos_Handle )
 * Description  : Khoi tao module I2C 
 * Param        : none
 * Return       : none
*******************************************************************************/
void vI2C_InitModule( i2c_rtos_handle_t *xI2C_Rtos_Handle, uint32_t ulI2C_Base, uint32_t ulBaudrate )
{
	uint32_t ulSourceClock;
	i2c_master_config_t xMasterConfig;
	status_t eStatus = kStatus_Fail;
	
	I2C_MasterGetDefaultConfig(&xMasterConfig);
	xMasterConfig.baudRate_Bps = ulBaudrate;
	
	if(ulI2C_Base == I2C0_BASE)
	{
		if (__CORTEX_M >= 0x03)
		  NVIC_SetPriority(I2C0_IRQn, 6);
		else
		  NVIC_SetPriority(I2C0_IRQn, 3);
		ulSourceClock = CLOCK_GetFreq(macroI2C_MASTER_CLK_SRC0);
		eStatus = I2C_RTOS_Init(xI2C_Rtos_Handle, macroI2C_MASTER_MODULE0, &xMasterConfig, ulSourceClock);
	}
	else if(ulI2C_Base == I2C1_BASE)
	{
		if (__CORTEX_M >= 0x03)
		  NVIC_SetPriority(I2C1_IRQn, 6);
		else
		  NVIC_SetPriority(I2C1_IRQn, 3);
		ulSourceClock = CLOCK_GetFreq(macroI2C_MASTER_CLK_SRC1);
		eStatus = I2C_RTOS_Init(xI2C_Rtos_Handle, macroI2C_MASTER_MODULE1, &xMasterConfig, ulSourceClock);
	}
	else if(ulI2C_Base == I2C2_BASE)
	{
		if (__CORTEX_M >= 0x03)
		  NVIC_SetPriority(I2C2_IRQn, 6);
		else
		  NVIC_SetPriority(I2C2_IRQn, 3);
		ulSourceClock = CLOCK_GetFreq(macroI2C_MASTER_CLK_SRC2);
		eStatus = I2C_RTOS_Init(xI2C_Rtos_Handle, macroI2C_MASTER_MODULE2, &xMasterConfig, ulSourceClock);
	}

	if (eStatus != kStatus_Success)
		APP_DEBUG("--- I2C: I2C master error during init, status = 0x%x\r\n", eStatus);
}




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
uint8_t uI2C_WriteBits( i2c_rtos_handle_t *xI2C_Rtos_Handle, uint8_t uI2C_Device_Address, uint8_t uReg, uint8_t uValue,uint8_t uPosition, uint8_t uLength )
{
    uint8_t uMask;
    uint8_t uBuffer;
    
    uI2C_Read( xI2C_Rtos_Handle, uI2C_Device_Address, uReg, (uint8_t *)&uBuffer, 1 );
    
    // Mask to the read data and stores the value
    uMask = (((1 << uLength) - 1) << uPosition);
    uValue = (uValue << uPosition) & uMask;
    uBuffer &= ~uMask;
    uBuffer |= uValue;

    return uI2C_Write( xI2C_Rtos_Handle, uI2C_Device_Address, uReg, (uint8_t *)&uBuffer, 1 );
}





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
uint8_t uI2C_Read( i2c_rtos_handle_t *xI2C_Master_Handle, uint8_t uAddress, uint8_t uReg, uint8_t *puData, uint8_t uLenght )
{
	status_t eStatus = 0;
	uint8_t uMaster_Buff[2] = {0};
    i2c_master_transfer_t xMasterXfer;
	
	memset(puData, 0, sizeof(puData));
	
	if(uLenght < 1) 
	{
		APP_DEBUG("--- I2C: Read data from I2C fail, lenght < 1\r\n");
		return 0;
	}
    
	if(uReg != 0xFF)
	{
		uMaster_Buff[0] = uReg;
		
		xMasterXfer.slaveAddress = uAddress;
		xMasterXfer.direction = kI2C_Write;
		xMasterXfer.subaddress = 0;
		xMasterXfer.subaddressSize = 0;
		xMasterXfer.data = uMaster_Buff;
		xMasterXfer.dataSize = 1;
		xMasterXfer.flags = kI2C_TransferDefaultFlag;
		
		eStatus = I2C_RTOS_Transfer( xI2C_Master_Handle, &xMasterXfer );
		if(eStatus != kStatus_Success)
		{
			APP_DEBUG("--- I2C: Read fail. Send address of slave error = %d\r\n", eStatus);
			return 0;
		}
	}

    xMasterXfer.slaveAddress = uAddress;
    xMasterXfer.direction = kI2C_Read;
    xMasterXfer.subaddress = 0;
    xMasterXfer.subaddressSize = 0;
    xMasterXfer.data = puData;
    xMasterXfer.dataSize = uLenght;
    xMasterXfer.flags = kI2C_TransferDefaultFlag;
    
    eStatus = I2C_RTOS_Transfer( xI2C_Master_Handle, &xMasterXfer );
    
    if(eStatus != kStatus_Success)
	{
        APP_DEBUG("--- I2C: Read data from I2C error = %d\r\n", eStatus);
		return 0;
	}
	return 1;
}




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
uint8_t uI2C_Write( i2c_rtos_handle_t *xI2C_Master_Handle, uint8_t uAddress, uint8_t uReg, uint8_t *puData, uint8_t uLenght )
{
	status_t eStatus = 0;
	uint8_t uMaster_Buff[macroI2C_DATA_MAX_LENGTH + 1] = {0};
    i2c_master_transfer_t xMasterXfer;
    
    uMaster_Buff[0] = uReg;
	for(uint8_t ui = 0; ui < uLenght; ui++)
		uMaster_Buff[ui + 1] = puData[ui];
    
    xMasterXfer.slaveAddress = uAddress;
    xMasterXfer.direction = kI2C_Write;
    xMasterXfer.subaddress = 0;
    xMasterXfer.subaddressSize = 0;
    xMasterXfer.data = uMaster_Buff;
    xMasterXfer.dataSize = uLenght + 1;
    xMasterXfer.flags = kI2C_TransferDefaultFlag;
    
    eStatus = I2C_RTOS_Transfer( xI2C_Master_Handle, &xMasterXfer );
	if(eStatus != kStatus_Success)
	{
        APP_DEBUG("--- I2C: Write data to I2C fail, error = %d\r\n", eStatus);
		return 0;
	}
	return 1;
}




/*******************************************************************************
 * Function     : uint8_t uI2C_WriteBit( i2c_rtos_handle_t *xI2C_Master_Handle, uint8_t uAddress, uint8_t uReg, uint8_t uData, uint8_t uPos )
 * Description  : Ghi bit cua byte qua I2C
 * Param        :
 *              + uReg: Dia chi cua lenh
 *				+ uData: data ghi vao
 *				+ uPos: vi tri cua bit ghi vao
 * Return       :
 *              + Successfully: 1
 *              + Failure: 0
*******************************************************************************/
uint8_t uI2C_WriteBit( i2c_rtos_handle_t *xI2C_Master_Handle, uint8_t uAddress, uint8_t uReg, uint8_t uData, uint8_t uPos )
{
	uint8_t uMaster_Buff[macroI2C_DATA_MAX_LENGTH + 1] = {0};
	uint8_t uMask = 0;
    //i2c_master_transfer_t xMasterXfer;
    
    if( uI2C_Read( xI2C_Master_Handle, uAddress, uReg, uMaster_Buff, 1 ) == 0)
		return 0;
	
	// Mask to the read data and stores the value
	uMask = ~(1 << uPos);
	uData = (uData << uPos);
	uMaster_Buff[0] &= uMask;	
	uMaster_Buff[0] |= uData;
    
    return uI2C_Write( xI2C_Master_Handle, uAddress, uReg, uMaster_Buff, 1 );
}







