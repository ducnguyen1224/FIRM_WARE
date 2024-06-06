#ifndef STD_HEADER_H
#define STD_HEADER_H

#include <string.h>
#include <stdio.h>
#include "ZComDef.h"
#include "OSAL.h"
#include "AF.h"
#include "ZDApp.h"
#include "ZDObject.h"
#include "ZDProfile.h"

#include "GenericApp.h"
#include "DebugTrace.h"

#if !defined( WIN32 ) || defined( ZBIT )
  #include "OnBoard.h"
#endif

/* HAL */
#include "hal_lcd.h"
#include "hal_led.h"
#include "hal_key.h"
#include "hal_uart.h"
#include "debug.h"
/* RTOS */
#if defined( IAR_ARMCM3_LM )


#define 
#include "RTOS_App.h"
#endif
#endif //STD_HEADER