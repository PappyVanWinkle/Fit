/****************************************************************************\
**
** debug.h
**
** Contains declarations for printing debug messages or sentinel fit core logging.
** 
** Copyright (C) 2016, SafeNet, Inc. All rights reserved.
**
\****************************************************************************/
#ifndef __FIT_DEBUG_H__
#define __FIT_DEBUG_H__

/* Required Includes ********************************************************/
#include <stdarg.h>
#include "fit_types.h"

#ifdef __cplusplus
#define EXTERNC extern "C"
#else
#define EXTERNC
#endif //ifdef __cplusplus__

/* Forward Declarations *****************************************************/

/* Function Prototypes ******************************************************/
/*
 * get descriptive string for a sentinel fit status code
 */
EXTERNC const char *fit_get_error_str (fit_status_t st);

#ifdef USE_COMX
#include "comx.h"
#include "comx_packet.h"
#endif

extern uint16_t fit_trace_flags;

/*
 * DBG() allows printf-like formatted debug output
 * on AVR8, format string is PROGMEM to save RAM
 */
#ifdef __AVR_8__
#include <avr/pgmspace.h>
#define USE_VSPRINTF_P
#endif

#define FIT_TRACE_INFO			0x0001
#define FIT_TRACE_ERROR			0x0002
#define FIT_TRACE_CRITICAL		0x0004
#define FIT_TRACE_FATAL			0x0008
#define FIT_TRACE_ALL			0x00FF

// Below trace flags are specific to comx communication
// Don't use them in sentinel fit core debug
#define FIT_TRACE_RX_TX			0x0010
#define FIT_TRACE_ECHO			0x0020
#define FIT_TRACE_COMX			0x0040

EXTERNC void fit_printf(uint16_t trace_flags, const char *format, ...);

#ifdef  FIT_DEBUG_MSG //defined (FIT_USE_UNIT_TESTS) || defined (USE_COMX) 

#ifdef USE_VSPRINTF_P
#define DBG(trace, format, args...) fit_printf(trace, PSTR(format), ## args)
#else
#ifdef _MSC_VER
#define FIT_DEBUG_LEVEL FIT_TRACE_ALL
#define DBG(X, ...) { if(FIT_DEBUG_LEVEL == FIT_TRACE_ALL || ((FIT_DEBUG_LEVEL & X) == FIT_DEBUG_LEVEL)) { \
                    unsigned char buf[128] = {0}; \
                    sprintf_s(buf, 128, __VA_ARGS__); \
                    printf("%s", buf); } }

#else
#define DBG(trace, format, args...) fit_printf(trace, format, ## args)
#endif // #ifdef _MSC_VER
#endif // USE_VSPRINTF_P

#else
#define DBG(...)

#endif // FIT_DEBUG_MSG
#endif //__FIT_DEBUG_H__
