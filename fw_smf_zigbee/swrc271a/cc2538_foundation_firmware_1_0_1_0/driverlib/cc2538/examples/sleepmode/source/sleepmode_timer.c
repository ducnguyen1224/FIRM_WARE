/******************************************************************************
*  Filename:       sleepmode_timer.c
*  Revised:        $Date: 2013-04-08 17:38:20 +0200 (ma, 08 apr 2013) $
*  Revision:       $Revision: 9684 $
*
*  Description:    Example demonstrating how to configure sleep timer
*                  and power modes.
*
*  Copyright (C) 2013 Texas Instruments Incorporated - http://www.ti.com/
*
*
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions
*  are met:
*
*    Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
*
*    Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the
*    documentation and/or other materials provided with the distribution.
*
*    Neither the name of Texas Instruments Incorporated nor the names of
*    its contributors may be used to endorse or promote products derived
*    from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
*  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
*  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
*  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
*  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
*  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
*  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
*  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
*  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
******************************************************************************/

#include <stdbool.h>
#include <stdint.h>
#include "hw_memmap.h"
#include "hw_ioc.h"
#include "hw_ints.h"
#include "gpio.h"
#include "ioc.h"
#include "interrupt.h"
#include "sleepmode.h"
#include "uart.h"
#include "sys_ctrl.h"
#include "uartstdio.h"

// NOTE:
// This example has been run it on a SmartRF06 evaluation board.  
//*****************************************************************************
//
//! \addtogroup sleepmode_examples_list
//! <h1>Sleep Mode Timer (sleepmode)</h1>
//!
//! This example shows how to configure the sleep mode timer and activate power 
//! modes.
//!
//! The following UART signals are configured only for displaying console
//! messages for this example.  These are not required for operation of sleep 
//! mode timer.
//! - UART0 peripheral
//! - GPIO Port A peripheral (for UART0 pins)
//! - UART0RX - PA0
//! - UART0TX - PA1
//!
//! This example uses the following interrupt handlers.  To use this example
//! in your own application you might have to add these interrupt handlers 
//! to your vector table.
//! - SleepModeIntHandler.
//
//*****************************************************************************    
#define EXAMPLE_PIN_UART_RXD            GPIO_PIN_0 
#define EXAMPLE_PIN_UART_TXD            GPIO_PIN_1 
#define EXAMPLE_GPIO_UART_BASE          GPIO_A_BASE

//*****************************************************************************
//
// Sleep mode timer ISR
//
//*****************************************************************************
void 
SleepModeIntHandler(void)
{
   //
   // Do nothing
   //
   return;
}

//*****************************************************************************
//
// This function sets up UART0 to be used for a console to display information
// as the example is running.
//
//*****************************************************************************
void
InitConsole(void)
{
    //
    // Map UART signals to the correct GPIO pins and configure them as
    // hardware controlled.
    //
    IOCPinConfigPeriphOutput(EXAMPLE_GPIO_UART_BASE, EXAMPLE_PIN_UART_TXD, 
                             IOC_MUX_OUT_SEL_UART0_TXD);
    GPIOPinTypeUARTOutput(EXAMPLE_GPIO_UART_BASE, EXAMPLE_PIN_UART_TXD);
    
    IOCPinConfigPeriphInput(EXAMPLE_GPIO_UART_BASE, EXAMPLE_PIN_UART_RXD, 
                            IOC_UARTRXD_UART0);
    GPIOPinTypeUARTInput(EXAMPLE_GPIO_UART_BASE, EXAMPLE_PIN_UART_RXD);
     
    //
    // Initialize the UART (UART0) for console I/O.
    //
    UARTStdioInit(0);
}

//*****************************************************************************
//
// Configure sleep timer, put system into deep sleep and watch sleep timer 
// interrupt wakeup system again
//
//*****************************************************************************
int
main(void)
{
    uint32_t ui32Val;
  
    //
    // Set the clocking to run directly from the external crystal/oscillator.
    // (no ext 32k osc, no internal osc)
    //
    SysCtrlClockSet(false, false, SYS_CTRL_SYSDIV_32MHZ);

    //
    // Set IO clock to the same as system clock
    //
    SysCtrlIOClockSet(SYS_CTRL_SYSDIV_32MHZ);
   
    //
    // Set up the serial console to use for displaying messages.  This is
    // just for this example program and is not needed for SSI operation.
    //
    InitConsole();
    
    //
    // Display the setup on the console.
    //
    UARTprintf("Sleepmode timer\n");

    //
    // Disable UART0 when in deep sleep
    //
    SysCtrlPeripheralDeepSleepDisable(SYS_CTRL_PERIPH_UART0);
    
    //
    // Let system enter powermode 2 when going to deep sleep
    //
    SysCtrlPowerModeSet(SYS_CTRL_PM_2);

    //
    // Enable the Sleep Timer wakeup
    //
    GPIOIntWakeupEnable(GPIO_IWE_SM_TIMER); 

    //
    // Enable sleep mode interrupt
    //
    IntEnable(INT_SMTIM);
    
    //
    // Set timer to 10000 above current value
    //
    ui32Val = SleepModeTimerCountGet();
    SleepModeTimerCompareSet(ui32Val + 10000);

    //
    // Display the timer value on the console.
    //
    UARTprintf("Timer val = %d\n", ui32Val);
    
    //
    // Wait for UART to be flushed
    //
    while(UARTBusy(UART0_BASE))
    {
    }
    
    //
    // Go to sleep
    //
    SysCtrlDeepSleep();

    //
    // Display the timer value on the console.
    //    
    ui32Val = SleepModeTimerCountGet();
    UARTprintf("Timer val = %d (after wakeup)\n", ui32Val);
    
    //
    // Done - enter an infinite loop.
    //
    while(1)
    {
    }
}
