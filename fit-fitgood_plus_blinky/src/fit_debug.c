/****************************************************************************\
**
** debug.c
**
** Defines functionality for printing debug messages or fit core logging.
** 
** Copyright (C) 2016, SafeNet, Inc. All rights reserved.
**
\****************************************************************************/

#include <stdlib.h>
#include <stdio.h>

#include "fit_debug.h"
#include "hwdep.h"

uint16_t fit_trace_flags = 0;

void fit_putc(char c)
{
    if (c == '\n') 
        FIT_UART_WRITECHAR('\r');
        
    FIT_UART_WRITECHAR(c);
}

EXTERNC void fit_printf(uint16_t trace_flags, const char *format, ...)
{
    char write_buffer[256] = { 0 };
    char *s = write_buffer;
    uint16_t len = 0;
    va_list arg;
    
    if(fit_trace_flags & trace_flags)
    {
        va_start (arg, format);
#ifdef USE_VSPRINTF_P
        len = vsprintf_P (write_buffer, format, arg);
#else
        len = vsprintf(write_buffer, format, arg);
#endif
        va_end (arg);

        if(len)
        {
#ifdef USE_COMX
            comx_packet_transaction(LOGGER, 0, (uint8_t *) write_buffer, len, NULL);
#else
            while (*s)
                fit_putc(*s++);
#endif
        }
    }
}



/*
 * get descriptive string for a fit status code
 */
const char *fit_get_error_str (fit_status_t st)
{
    switch (st)
    {
        case FIT_STATUS_OK:                     return "FIT_STATUS_OK";
        case FIT_INSUFFICIENT_MEMORY:           return "FIT_INSUFFICIENT_MEMORY";
        case FIT_INVALID_FEATURE_ID:            return "FIT_INVALID_FEATURE_ID";
        case FIT_INVALID_V2C:                   return "FIT_INVALID_V2C";
        case FIT_ACCESS_DENIED:                 return "FIT_ACCESS_DENIED";
        case FIT_STATUS_ERROR:                  return "FIT_STATUS_ERROR";
        case FIT_REQ_NOT_SUPPORTED:             return "FIT_REQ_NOT_SUPPORTED";
        case FIT_UNKNOWN_ALG:                   return "FIT_UNKNOWN_ALG";
        case FIT_INVALID_SIGNATURE:             return "FIT_INVALID_SIGNATURE";
        case FIT_FEATURE_NOT_FOUND:             return "FIT_FEATURE_NOT_FOUND";
        case FIT_FEATURE_ID_FOUND:              return "FIT_FEATURE_ID_FOUND";
        case FIT_STOP_PARSE:                    return "FIT_STOP_PARSE";
        case FIT_CONTINUE_PARSE:                return "FIT_CONTINUE_PARSE";
        case FIT_INVALID_LICGEN_VERSION:            return "FIT_INVALID_LICGEN_VERSION";
        case FIT_INVALID_SIG_ID:                return "FIT_INVALID_SIG_ID";
        case FIT_FEATURE_EXPIRED:               return "FIT_FEATURE_EXPIRED";
        case FIT_LIC_CACHING_ERROR:             return "FIT_LIC_CACHING_ERROR";
        case FIT_INVALID_PRODUCT:               return "FIT_INVALID_PRODUCT";
        case FIT_INVALID_PARAM:                 return "FIT_INVALID_PARAM";
        case FIT_INVALID_PARAM_1:               return "FIT_INVALID_PARAM_1";
        case FIT_INVALID_PARAM_2:               return "FIT_INVALID_PARAM_2";
        case FIT_INVALID_PARAM_3:               return "FIT_INVALID_PARAM_3";
        case FIT_INVALID_PARAM_4:               return "FIT_INVALID_PARAM_4";
        case FIT_INVALID_PARAM_5:               return "FIT_INVALID_PARAM_5";
        case FIT_UNIT_TEST_PASSED:              return "FIT_UNIT_TEST_PASSED";
        case FIT_UNIT_TEST_FAILED:              return "FIT_UNIT_TEST_FAILED";
        case FIT_INVALID_WIRE_TYPE:             return "FIT_INVALID_WIRE_TYPE";
        case FIT_INTERNAL_ERROR:                return "FIT_INTERNAL_ERROR";
        case FIT_INVALID_KEYSIZE:               return "FIT_INVALID_KEYSIZE";
        case FIT_INVALID_VENDOR_ID:             return "FIT_INVALID_VENDOR_ID";
        case FIT_INVALID_PRODUCT_ID:            return "FIT_INVALID_PRODUCT_ID";
        case FIT_INVALID_CONT_ID:               return "FIT_INVALID_CONT_ID";
        case FIT_LIC_FIELD_PRESENT:             return "FIT_LIC_FIELD_PRESENT";
        case FIT_INVALID_LIC_TYPE:          return "FIT_INVALID_LIC_TYPE";
        case FIT_LIC_EXPIRATION_NOT_SUPP:              return "FIT_LIC_EXPIRATION_NOT_SUPP";
        case FIT_INVALID_START_DATE:            return "FIT_INVALID_START_DATE";
        case FIT_INVALID_END_DATE:              return "FIT_INVALID_END_DATE";
        case FIT_INACTIVE_LICENSE:              return "FIT_INACTIVE_LICENSE";
        case FIT_RTC_NOT_PRESENT:               return "FIT_RTC_NOT_PRESENT";
        case FIT_NO_CLOCK_SUPPORT:              return "FIT_NO_CLOCK_SUPPORT";
        case FIT_INVALID_FIELD_LEN:             return "FIT_INVALID_FIELD_LEN";
        case FIT_DATA_MISMATCH_ERROR:           return "FIT_DATA_MISMATCH_ERROR";
        case FIT_NODE_LOCKING_NOT_SUPP:         return "FIT_NODE_LOCKING_NOT_SUPP";
        case FIT_FP_MAGIC_NOT_VALID:            return "FIT_FP_MAGIC_NOT_VALID";
        case FIT_UNKNOWN_FP_ALG:                return "FIT_UNKNOWN_FP_ALG";
        case FIT_FP_MISMATCH_ERROR:             return "FIT_FP_MISMATCH_ERROR";
        case FIT_INVALID_DEVICE_LEN:            return "FIT_INVALID_DEVICE_LEN";
        case FIT_RSA_VERIFY_FAILED:             return "FIT_RSA_VERIFY_FAILED";
        default:;
    }
    return "UNKNOWN ERROR";
}

