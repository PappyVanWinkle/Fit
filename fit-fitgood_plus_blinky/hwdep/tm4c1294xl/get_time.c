/****************************************************************************\
**
** gettime.c
**
** Contains function declaration related with expiration based licenses.
** 
** Copyright (C) 2016, SafeNet, Inc. All rights reserved.
**
\****************************************************************************/

#include "get_time.h"
#include "stddef.h"
#include "fit_debug.h"


#include <stdint.h>
#include <stdbool.h>

#include "fit_api.h"

#include "hwdep.h"

#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"

#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"

uint32_t unixtime = 0;

/**
 *
 * fit_time_get
 *
 * This function returns the current time for hardware board.
 *
 */
uint32_t fit_time_get (void)
{
    uint32_t t = unixtime;
    DBG(FIT_TRACE_INFO, "Get unix time %lu\n", unixtime);

    return t;
}


/**
 *
 * fit_settime
 *
 * This function set the current unix time for hardware board.
 *
 */
void fit_time_set (uint32_t settime)
{
    DBG(FIT_TRACE_INFO, "Set unix time %lu\n", settime);

    unixtime = settime;
}

/**
 *
 * Interrupt routine for "unixtime" RTC implementation
 *
 */
void Timer0IntHandler(void)
{
    ++unixtime;
    ROM_TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
}


/**
 *
 * fit_time_init
 *
 * This function initializes an RTC
 * returns 0 for failure,
 *         1 for success
 *
 */
 extern uint32_t g_ui32SysClock;

 uint32_t fit_time_init (void)
 {
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);

    // Enable processor interrupts.
    ROM_IntMasterEnable();

    ROM_TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
    ROM_TimerLoadSet(TIMER0_BASE, TIMER_A, g_ui32SysClock);   // 1 Hz
    ROM_IntEnable(INT_TIMER0A);
    ROM_TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    ROM_TimerEnable(TIMER0_BASE, TIMER_A);

    return 1; /* OK */
 }

