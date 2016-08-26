/****************************************************************************\
**
** hwdep\led.c
**
** LED functions - TM4C1294XL version
**
** Copyright (C) 2016, SafeNet, Inc. All rights reserved.
**
\****************************************************************************/

//*****************************************************************************

#include <stdint.h>
#include <stdbool.h>

#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"

#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"


void fit_led_on(void)
{
    GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, GPIO_PIN_0);
}

void fit_led_off(void)
{
    GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, 0);
}

void fit_led_init(void)
{
    // Enable the GPIO port that is used for the on-board LED.
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);

    // Enable the GPIO pins for the LED (PN0).
    ROM_GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_0);

    // Enable the peripheral
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    fit_led_off();
}

