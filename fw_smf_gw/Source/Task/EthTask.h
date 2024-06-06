
#ifndef __LED_TASK_H_
#define __LED_TASK_H_
#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
* INCLUDES
*/
#define ETH_MQTT_WRITE_DATA(Buffer)					vEthTask_WriteData(Buffer)
    
/*********************************************************************
* CONSTANTS
*/

/******************************************************************************
 * Function		: void vEthTask_Run( void *pvParameters)
 * Description	: Task xu ly ket noi ethernet
 * Param		: None
 * Return		: None
*******************************************************************************/
void vEthTask_Run( void *pvParameters );




/******************************************************************************
 * Function		: void vEthTask_WriteData( uint8_t *pData )
 * Description	: ham gui du lieu qua ethernet mqtt
 * Param		: None
 * Return		: None
*******************************************************************************/
void vEthTask_WriteData( uint8_t *pData );




    
#ifdef __cplusplus
}
#endif
#endif




