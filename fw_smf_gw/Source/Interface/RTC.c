
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include <fsl_rtc.h>

#include "RTC.h"
#include "main.h"


rtc_datetime_t _RTC =
{
	.year	= 2018,
	.month	= 7,
	.day	= 15,
	.hour	= 12,
	.minute	= 12,
	.second = 0,
};




/******************************************************************************
 * Function		: void vRTC_Init( rtc_datetime_t *xRTC_DateTime ))
 * Description	: ham khoi tao RTC
 * Param		: none
 * Return		: none
*******************************************************************************/
void vRTC_Init( rtc_datetime_t *xRTC_DateTime )
{
    rtc_config_t xRTCConfig;
 
    RTC_GetDefaultConfig(&xRTCConfig);
    RTC_Init(RTC, &xRTCConfig);
	
    /* Select RTC clock source */
    RTC_SetClockSource(RTC);
	
	/* Set a start date time and start RT default */
    /* Read flash and init RTC here */
	vRTC_SetDateTime(xRTC_DateTime);
}



/******************************************************************************
 * Function		: void vRTC_SetDateTime(rtc_datetime_t *xRTC)
 * Description	: ham thiet lap thoi gian cho RTC
 * Param		: 
 * Return		: none
*******************************************************************************/
void vRTC_SetDateTime(rtc_datetime_t *xRTC)
{
	RTC_StopTimer(RTC);

	if(RTC_SetDatetime(RTC, xRTC) != kStatus_Success)
		APP_DEBUG("--- RTC: RTC config error\r\n");

	/* Start the RTC time counter */
	RTC_StartTimer(RTC);
}




/******************************************************************************
 * Function		: void RTC_GetDateTime(rtc_datetime_t *xRTC_DateTime)
 * Description	: ham lay thoi gian tu bo RTC
 * Param		: none
 * Return		: none
*******************************************************************************/
void vRTC_GetDateTime(rtc_datetime_t *xRTC_DateTime)
{	
	RTC_GetDatetime( RTC, xRTC_DateTime);
}






