/****************************************************************************\
**
** board_setup.c
**
** Contains function definitions for setting board for testing fit core code.
**
** Copyright (C) 2016, SafeNet, Inc. All rights reserved.
**
\****************************************************************************/

#include "hwdep.h"
#include "get_time.h"

#include <stdint.h>
#include <stdbool.h>

#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"

uint32_t g_ui32SysClock; /* System clock rate in Hz. */

extern void fit_uart_init(unsigned int baudrate);

void fit_board_setup(void)
{
    // Set the clocking to run directly from the crystal at 120MHz.
    g_ui32SysClock = MAP_SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |
                                             SYSCTL_OSC_MAIN |
                                             SYSCTL_USE_PLL |
                                             SYSCTL_CFG_VCO_480), 120000000);

    // Enable EEPROM
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_EEPROM0);
    ROM_EEPROMInit();

    fit_led_init();
    fit_uart_init(0);
    FIT_TIME_INIT();
}

