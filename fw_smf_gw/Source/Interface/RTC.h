/******************************************************************************
Filename:       rtc_task.h
Designed:       tuanna3
Revised:        $Date: 2016-10-25 8:36:30 -0700 (Tu, 25 Oct 2016) $
Decription: library read value pH through IC LMP91200 of TI.
            equation Nernst in 25oC 
            ->>anpha Nernst is 59.16mV/pH

Copyright 2015 VNPT-Technology . All rights reserved.


******************************************************************************/
#ifndef __RTC_H_
#define __RTC_H_
#ifdef __cplusplus
extern "C"
{
#endif

/******************************************************************************
 * Function		: void vRTC_Init( rtc_datetime_t *xRTC_DateTime ))
 * Description	: ham khoi tao RTC
 * Param		: none
 * Return		: none
*******************************************************************************/
void vRTC_Init( rtc_datetime_t *xRTC_DateTime );

	
	
/******************************************************************************
 * Function		: void vRTC_SetDateTime(rtc_datetime_t *xRTC)
 * Description	: ham thiet lap thoi gian cho RTC
 * Param		: 
 * Return		: none
*******************************************************************************/
void vRTC_SetDateTime(rtc_datetime_t *xRTC);



/******************************************************************************
 * Function		: void RTC_GetDateTime(rtc_datetime_t *xRTC_DateTime)
 * Description	: ham lay thoi gian tu bo RTC
 * Param		: none
 * Return		: none
*******************************************************************************/
void vRTC_GetDateTime(rtc_datetime_t *xRTC_DateTime);

#ifdef __cplusplus
}
#endif
#endif

