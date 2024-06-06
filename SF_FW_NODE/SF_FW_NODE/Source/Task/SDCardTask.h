/*
 * SDCardTask.h
 *
 *  Created on: Jul 18, 2018
 *      Author: ThaoHuyen
 */

#ifndef SOURCE_TASK_SDCARDTASK_H_
#define SOURCE_TASK_SDCARDTASK_H_

#include "fsl_sd.h"
#include "ff.h"
#include "diskio.h"
#include "fsl_sd_disk.h"

#include "main.h"


/*! @brief Data block count accessed in card */
#define DATA_BLOCK_COUNT 						(5U)
/*! @brief Start data block number accessed in card */
#define DATA_BLOCK_START 						(2U)
/*! @brief Data buffer size. */
#define DATA_BUFFER_SIZE 						(FSL_SDMMC_DEFAULT_BLOCK_SIZE * DATA_BLOCK_COUNT)

/*! @brief Task stack size. */
#define ACCESSCARD_TASK_STACK_SIZE 				(DATA_BUFFER_SIZE + 1000U)
/*! @brief Task stack priority. */
#define ACCESSCARD_TASK_PRIORITY 				(configMAX_PRIORITIES - 2U)

/*! @brief Task stack size. */
#define CARDDETECT_TASK_STACK_SIZE 				(DATA_BUFFER_SIZE + 1000U)




/******************************************************************************
 * Function		: void vSDCardTask_Run( void *pvParameters )
 * Description	: SDCard task
 * Param		: N/A
 * Return		: none
*******************************************************************************/
void vSDCardTask_Run( void *pvParameters );


#endif /* SOURCE_TASK_SDCARDTASK_H_ */
