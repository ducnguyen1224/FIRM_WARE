
#ifndef MAIN_H
#define MAIN_H

#include "string.h"
#include "stdio.h"
#include "stdlib.h"

#include "OSAL.h"
#include "AF.h"
#include "ZDApp.h"
#include "ZDObject.h"
#include "ZDProfile.h"
#include "OnBoard.h"

#include "ZMAC.h"
/* HAL */
#include "ZComDef.h"

#include "AppData.h"
#include "ZigbeeIO.h"
#include "UserUART.h"


 
//event
#define EVENT_UART_RECEIVE									0x0001
#define EVENT_UART_SEND										0x0002
#define EVENT_ZIGBEE_RECEIVE								0x0004
#define EVENT_ZIGBEE_SEND									0x0008
#define EVENT_UART_RESEND									0x0010

//time loop event (ms)
#define macroTIME_LOOP_EVENT_RESEND								100

//event function
#define macroLOOP_EVENT(TASK_ID, EVENT_ID, TIMEOUT)				osal_start_timerEx(TASK_ID, EVENT_ID, TIMEOUT)
#define macroSET_EVENT(TASK_ID, EVENT_ID)						osal_set_event(TASK_ID, EVENT_ID)

 
/*********************************************************************
 * FUNCTIONS
 */
/*******************************************************************************
* Function: void vMain_Init( byte task_id )
* Description: main task init
* Param: task_id - task id
* Return: none
*******************************************************************************/
void vMain_Init( byte task_id );



/*******************************************************************************
* Function: UINT16 uiMain_ProcessEvent( byte task_id, UINT16 events )
* Description: main task event process
* Param: 
*		+ task_id: task id
*		+ events: event task
* Return: none
*******************************************************************************/
UINT16 uiMain_ProcessEvent( byte task_id, UINT16 events );

#endif /* GENERICAPP_H */







