/*
 * SDCardTask.c
 *
 *  Created on: Jul 18, 2018
 *      Author: ThaoHuyen
 */
#include <stdio.h>
#include <string.h>
	 
#include "SDCardTask.h"

#ifdef macroUSE_SDCARD
	 
#include "FreeRTOS.h"
#include "semphr.h"
	 
#include "UserFile.h"
#include "RTC.h"
	 
#ifdef macroCONNECTIVITY_ETH
	#include "MQTTClient.h"
	extern mqttConfig _mqttConfig;
#endif


enum Card_Event
{
	eCARD_EVENT_HOST_INIT = 0,
	eCARD_EVENT_INSERT,
	eCARD_EVENT_INIT,
	eCARD_EVENT_DEINIT,
	eCARD_EVENT_ACCESS,
	eCARD_EVENT_READ_CONFIG,
	eCARD_EVENT_READ,
	eCARD_EVENT_WRITE,
	eCARD_EVENT_IDLE,
}Enum_CARD_EVENT = eCARD_EVENT_HOST_INIT;


//Task
extern taskHandle_t xTask;

//RTC
extern rtc_datetime_t _RTC;

bool bRTCisRead = false;


/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void SDCARD_DetectCallBack(bool isInserted, void *userData);
static bool bSDCardTask_MMC_Access( void );
/******************************************************************************/

/*! @brief Data written to the card */
SDK_ALIGN(uint8_t g_dataWrite[SDK_SIZEALIGN(DATA_BUFFER_SIZE, SDMMC_DATA_BUFFER_ALIGN_CACHE)], // @suppress("Symbol is not resolved")
          MAX(SDMMC_DATA_BUFFER_ALIGN_CACHE, SDMMCHOST_DMA_BUFFER_ADDR_ALIGN));
/*! @brief Data read from the card */
SDK_ALIGN(uint8_t g_dataRead[SDK_SIZEALIGN(DATA_BUFFER_SIZE, SDMMC_DATA_BUFFER_ALIGN_CACHE)], // @suppress("Symbol is not resolved")
          MAX(SDMMC_DATA_BUFFER_ALIGN_CACHE, SDMMCHOST_DMA_BUFFER_ADDR_ALIGN));

/*! @brief SDMMC host detect card configuration */
static const sdmmchost_detect_card_t s_sdCardDetect = 
{
#ifndef BOARD_SD_DETECT_TYPE
    .cdType = kSDMMCHOST_DetectCardByGpioCD,
#else
    .cdType = BOARD_SD_DETECT_TYPE,
#endif
    .cdTimeOut_ms = (~0U),
    .cardInserted = SDCARD_DetectCallBack,
    .cardRemoved = SDCARD_DetectCallBack,
};

/*! @brief SDMMC card power control configuration */
#if defined DEMO_SDCARD_POWER_CTRL_FUNCTION_EXIST
static const sdmmchost_pwr_card_t s_sdCardPwrCtrl = 
{
    .powerOn = BOARD_PowerOnSDCARD, .powerOnDelay_ms = 500U, .powerOff = BOARD_PowerOffSDCARD, .powerOffDelay_ms = 0U,
};
#endif

/*! @brief SD card detect flag  */
static bool s_cardInserted = false;

static FATFS g_fileSystem; /* File system object */
const TCHAR driverNumberBuffer[3U] = {SDDISK + '0', ':', '/'}; // @suppress("Symbol is not resolved")

uint8_t uError_Counter = 0;




/******************************************************************************
 * Function		: void vSDCardTask_Run( void *pvParameters )
 * Description	: SDCard task
 * Param		: none
 * Return		: none
*******************************************************************************/
void vSDCardTask_Run( void *pvParameters )
{
	APP_DEBUG("*********************** SDCardTask *************************\r\n");

	g_sd.host.base = SD_HOST_BASEADDR; // @suppress("Symbol is not resolved") // @suppress("Field cannot be resolved")
	g_sd.host.sourceClock_Hz = SD_HOST_CLK_FREQ; // @suppress("Field cannot be resolved") // @suppress("Symbol is not resolved")
	g_sd.usrParam.cd = &s_sdCardDetect;
#if defined DEMO_SDCARD_POWER_CTRL_FUNCTION_EXIST
	g_sd.usrParam.pwr = &s_sdCardPwrCtrl;
#endif

#if defined(__CORTEX_M)
	NVIC_SetPriority(SD_HOST_IRQ, 5U); // @suppress("Symbol is not resolved")
#else
	GIC_SetPriority(SD_HOST_IRQ, 25U);
#endif

	while(1)
	{
		xTask.uiSDCardTask_Finish = 1;

		switch(Enum_CARD_EVENT)
		{
			case eCARD_EVENT_HOST_INIT:
				APP_DEBUG("\r\n--- SDCardTask: eCARD_EVENT_HOST_INIT\r\n");
				if (SD_HostInit(&g_sd) != kStatus_Success)
				{
					APP_DEBUG("--- SDCardTask: SD host init fail\r\n");
					uError_Counter++;
				}
				else
				{
					uError_Counter = 0;
					Enum_CARD_EVENT = eCARD_EVENT_INSERT;
				}
				break;

			case eCARD_EVENT_INSERT:
				APP_DEBUG("\r\n--- SDCardTask: eCARD_EVENT_INSERT\r\n");

				if (!s_cardInserted)
				{
					APP_DEBUG("\r\n--- SDCardTask: sd card removed\r\n");
					SD_PowerOffCard(g_sd.host.base, g_sd.usrParam.pwr);	/* power off card */ // @suppress("Field cannot be resolved")
					uError_Counter++;
				}
				else
				{
					/* power on the card */
					SD_PowerOnCard(g_sd.host.base, g_sd.usrParam.pwr); // @suppress("Field cannot be resolved")
					Enum_CARD_EVENT = eCARD_EVENT_INIT;
					uError_Counter = 0;
				}
				break;

			case eCARD_EVENT_INIT:
				APP_DEBUG("\r\n--- SDCardTask: eCARD_EVENT_INIT\r\n");
				if (SD_CardInit(&g_sd))
				{
					uError_Counter++;
					PRINTF("--- SDCardTask: SD card init failed.\r\n");
				}
				else
				{
					uError_Counter = 0;
					Enum_CARD_EVENT = eCARD_EVENT_ACCESS;
				}
				break;

			case eCARD_EVENT_DEINIT:
				APP_DEBUG("\r\n--- SDCardTask: eCARD_EVENT_DEINIT\r\n");
				SD_Deinit(&g_sd);
				Enum_CARD_EVENT = eCARD_EVENT_HOST_INIT;
				break;

			case eCARD_EVENT_ACCESS:
				APP_DEBUG("\r\n--- SDCardTask: eCARD_EVENT_ACCESS\r\n");

				if (bSDCardTask_MMC_Access() == true)
				{
					uError_Counter = 0;
					Enum_CARD_EVENT = eCARD_EVENT_READ_CONFIG;
				}
				else
				{
					uError_Counter++;
				}
				break;

			case eCARD_EVENT_READ_CONFIG:
				APP_DEBUG("\r\n--- SDCardTask: eCARD_EVENT_READ_CONFIG\r\n");
//				f_unlink(_T("12345678.txt"));
//				f_unlink(_T("System Volume Information"));
//				f_unlink(_T("4E4543801003001E.txt"));
				if(bRTCisRead == false)
				{
					vMain_setEvent(EVENT_SET_RTC);
					if(bUserFile_Read_RTC( &_RTC.hour, &_RTC.minute, &_RTC.second, &_RTC.day, &_RTC.month, &_RTC.year ) == false)
						vUserFile_Write_RTC( _RTC.hour, _RTC.minute, _RTC.second, _RTC.day, _RTC.month, _RTC.year );
					bRTCisRead = true;
				}
				
			#ifdef macroCONNECTIVITY_ETH
				macroTASK_DELAY_MS( 1000 );
				if(bUserFile_Read_MQTTConfig(_mqttConfig.Host, &_mqttConfig.Port, _mqttConfig.User, _mqttConfig.Pwd) == false)
				{
					uError_Counter++;
					APP_DEBUG("--- SDCardTask: Read mqtt config fail\r\n");
				}
				else
				{
					_mqttConfig.getMMC = true;
					Enum_CARD_EVENT = eCARD_EVENT_IDLE;
				}
			#else
				Enum_CARD_EVENT = eCARD_EVENT_IDLE;
			#endif
				break;

			case eCARD_EVENT_READ:
				Enum_CARD_EVENT = eCARD_EVENT_IDLE;
				break;

			case eCARD_EVENT_WRITE:
				Enum_CARD_EVENT = eCARD_EVENT_IDLE;
				break;

			case eCARD_EVENT_IDLE:
				break;
		}

		if(uError_Counter >= 10)
		{
			uError_Counter = 0;
			Enum_CARD_EVENT = eCARD_EVENT_DEINIT;
		}
		xTask.uiSDCardTask_Finish = 0;
		macroTASK_DELAY_MS( 1000 );
	}
}




/******************************************************************************
 * Function		: void vSDCardTask_Run( void *pvParameters )
 * Description	: call back function for SD card detect.
 * Param		:
 * 				isInserted  true,  indicate the card is insert.
*                    		false, indicate the card is remove.
 * Return		: none
*******************************************************************************/
static void SDCARD_DetectCallBack(bool isInserted, void *userData)
{
    s_cardInserted = isInserted;
    Enum_CARD_EVENT = eCARD_EVENT_INSERT;
}




/******************************************************************************
 * Function		: bool bSDCardTask_MMC_Access( void )
 * Description	: sd card access
 * Param		: none
 * Return		: none
*******************************************************************************/
static bool bSDCardTask_MMC_Access( void )
{
	if (f_mount(&g_fileSystem, driverNumberBuffer, 0U))
	{
		APP_DEBUG("--- SDCardTask: Mount volume failed.\r\n");
		return false;
	}
#if (FF_FS_RPATH >= 2U)
	if (f_chdrive((char const *)&driverNumberBuffer[0U]))
	{
		APP_DEBUG("--- SDCardTask: Change drive failed.\r\n");
		return false;
	}
#endif
	
	DIR directory;
	FILINFO fileInformation;
	APP_DEBUG("\r\n--- SDCardTask: List the file in that directory......\r\n");
    if (f_opendir(&directory, "/"))
    {
    	APP_DEBUG("--- SDCardTask: Open directory failed.\r\n");
        return false;
    }
	for (;;)
    {
        FRESULT error = f_readdir(&directory, &fileInformation);

        /* To the end. */
        if ((error != FR_OK) || (fileInformation.fname[0U] == 0U))
            break;
        if (fileInformation.fname[0] == '.')
            continue;
        if (fileInformation.fattrib & AM_DIR)
        	APP_DEBUG("--- SDCardTask: Directory file : %s.\r\n", fileInformation.fname);
        else
        	APP_DEBUG("--- SDCardTask: General file : %s.\r\n", fileInformation.fname);
    }
	
	return true;
}



#endif












