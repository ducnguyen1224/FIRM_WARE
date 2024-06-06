
#ifndef __PROCESSTASK_H_
#define __PROCESSTASK_H_
#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
* INCLUDES
*/

/*********************************************************************
* CONSTANTS
*/   
//for low power timer
#define macroLPTMR_COUNTER_HANDLER 					LPTMR0_IRQHandler
/* Get source clock for LPTMR driver */
#define macroLPTMR_SOURCE_CLOCK 					CLOCK_GetFreq(kCLOCK_LpoClk)
/* Define LPTMR microseconds counts value */
#define macroLPTMR_USEC_COUNT 						1000000U

	
/******************************************************************************
 * Function		: void vProcessTask_Run( void *pvParameters )
 * Description	: Task xu ly kiem tra cac co de thuc hien cac action tuong ung
 * Param		: N/A
 * Return		: none
*******************************************************************************/
void vProcessTask_Run( void *pvParameters );

#ifdef __cplusplus
}
#endif
#endif



