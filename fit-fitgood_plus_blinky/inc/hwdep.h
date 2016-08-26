/****************************************************************************\
**
** hwdep.h
**
** Defines hardware dependent functions.
**
** Copyright (C) 2016, SafeNet, Inc. All rights reserved.
**
** $Header:
**
\****************************************************************************/

#ifndef __FIT_HW_DEP_H__
#define __FIT_HW_DEP_H__

/* Required Includes ********************************************************/
#include "fit_status.h"
#ifndef _MSC_VER
#include <stdint.h>
#endif

/* Macro Functions **********************************************************/
#ifdef __cplusplus
#define EXTERNC extern "C"
#else
#define EXTERNC
#endif

#define DEV_ID "abcdefghijklmn"

extern char devid[];
extern uint16_t devid_len;

/*
 * read memory specific defines
 *
 * change values for READ_AESKEY_BYTE and READ_LICENSE_BYTE
 * according to your hardware and the storage type where you 
 * decide to store the license and the aes/rsa key.
 */
#define READ_BYTE_RAM          read_ram_u8
#define READ_BYTE_FLASH        read_ram_u8
#define READ_BYTE_E2           read_ram_u8

EXTERNC uint8_t  READ_BYTE_RAM  (const uint8_t *p);
EXTERNC uint8_t  READ_BYTE_FLASH  (const uint8_t *p);
EXTERNC uint8_t  READ_BYTE_E2  (const uint8_t *p);

/*
 * Time specific defines
 */
#ifdef FIT_USE_CLOCK
#define FIT_TIME_GET           fit_time_get
#define FIT_TIME_SET(x)        fit_time_set(x)
#define FIT_TIME_INIT          fit_time_init

EXTERNC uint32_t FIT_TIME_GET(void);
EXTERNC void FIT_TIME_SET(uint32_t settime);
EXTERNC uint32_t FIT_TIME_INIT(void);

#else
#define FIT_TIME_GET        NULL
#define FIT_TIME_SET(x)
#define FIT_TIME_INIT()
#endif

EXTERNC void FIT_TIMESET (uint32_t settime);

/* Types ********************************************************************/

/* Function Prototypes ******************************************************/

/*
 * Initialize UART (used for debug/info output)
 */

#define FIT_UART_INIT fit_uart_init
EXTERNC void FIT_UART_INIT(unsigned int baudrate);

/*
 * write character to console prototype
 *  void FIT_UART_WRITECHAR(char data)
 */
#define FIT_UART_WRITECHAR fit_uart_putc
EXTERNC void FIT_UART_WRITECHAR(unsigned char data);

#define FIT_UART_GETCHAR fit_uart_getchar
EXTERNC unsigned char FIT_UART_GETCHAR(void);


/*
 * Specific board initialization
 */
EXTERNC void fit_board_setup(void);

/*
 * Fill buffer with board's unique deviceid and its length
 */
EXTERNC fit_status_t fit_deviceid_get(uint8_t *rawdata,
                                      uint16_t *datalen);

/*
 * get unixtime
 */
EXTERNC uint32_t fit_time_get(void);

/*
 * Initialize LED pin
 */
EXTERNC void fit_led_init(void);

/*
 * switch led off/on
 */
EXTERNC void fit_led_off(void);
EXTERNC void fit_led_on(void);
/* Prototype of a hardware dependent callback function. This function is called 
 * to get unix time for time expiration based licenses.
 */
typedef int32_t (*gettime_cb_t) (void);

#endif //__FIT_HW_DEP_H__
