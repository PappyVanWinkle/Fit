/****************************************************************************\
**
** hwdep\delays.c
**
** UART output functions - TM4C1294XL version
**
** Copyright (C) 2016, SafeNet, Inc. All rights reserved.
**
\****************************************************************************/

#include <stdint.h>
#include <stdbool.h>

#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"

#include "utils/uartstdio.h"


#ifdef DEBUG
void __error__(char *pcFilename, uint32_t ui32Line) { }
#endif

extern uint32_t g_ui32SysClock; /* System clock rate in Hz. */


/*************************************************************************
Function: fit_uart_init()
Purpose:  initialize UART 
Returns:  none
**************************************************************************/
void fit_uart_init(unsigned int baudrate)
{
    (void)baudrate;

    // Enable the GPIO Peripheral used by the UART.
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    // Enable UART0
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

    // Configure GPIO Pins for UART mode.
    ROM_GPIOPinConfigure(GPIO_PA0_U0RX);
    ROM_GPIOPinConfigure(GPIO_PA1_U0TX);
    ROM_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    // Initialize the UART for console I/O.
    UARTEchoSet(0);
    UARTStdioConfig(0, 115200, g_ui32SysClock);
}


/*************************************************************************
Function: uart_putc()
Purpose:  transmit byte to UART
Input:    byte to be transmitted
Returns:  none          
**************************************************************************/

void fit_uart_putc(unsigned char data)
{
    while (UARTTxBytesFree() < 8) { } /* wait for free Tx buffer space */
    UARTwrite((char*)&data, 1);
}

/**
 * fit_uart_getc - read char from UART, non-blocking
 *
 * result:  <= 255  character received
 *          >= 256  NO char available
 */

uint32_t fit_uart_getc(void)
{
    if (UARTRxBytesAvail()) {
        return UARTgetc();
    } else {
        return 0x100;
    }
}

unsigned char fit_uart_getchar(void)
{
    while (UARTRxBytesAvail() == 0) { }
    return UARTgetc();
}

