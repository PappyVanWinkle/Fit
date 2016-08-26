/****************************************************************************\
**
** fit_types.h
**
** Basic types used in Sentinel FIT
** 
** Copyright (C) 2016, SafeNet, Inc. All rights reserved.
**
\****************************************************************************/

#ifndef __FIT_TYPES_H__
#define __FIT_TYPES_H__

/* Required Includes ********************************************************/
#ifndef _MSC_VER
#include <stdint.h>
#endif
#include "fit_status.h"

/* Constants ****************************************************************/
#define ENABLE          1
#define DISABLE         0
#define TRUE            1
#define FALSE           0

/* Types ********************************************************************/

// !!! an int normally is 16bits on 8bit machines, so stuff below doesn't 
//     work on AVR 8bit

// it's safe to rely on stdint types being available, since they are crucial
// for embedded stuff; does not make any sense to introduce own types apart
// from annoying users

#ifdef _MSC_VER

typedef unsigned char           uint8_t;
typedef signed char             int8_t;
typedef unsigned short          uint16_t;
typedef signed short            int16_t;
typedef unsigned long           uint32_t;
typedef signed long             int32_t;
typedef signed long long int    int64_t;
typedef unsigned long long int  uint64_t;

#endif // _MSC_VER

/* Types ********************************************************************/

typedef uint32_t (*fit_cb_time_get_t)(void);

typedef uint8_t (*fit_read_byte_callback_t)(const void *address);

typedef struct fit_pointer_t
{
    uint8_t* data;                      // pointer to license binary
    uint16_t length;                    // length of binary data
    fit_read_byte_callback_t read_byte; // pointer to read byte function for reading data part.

}fit_pointer_t, *pfit_pointer_t;

/* Forward Declarations *****************************************************/

// Prototype of a get_info callback function.
typedef fit_status_t (*fit_get_info_callback)(uint8_t tagid,
                                              fit_pointer_t *pdata,
                                              uint16_t length,
                                              void *context);

// Prototype of a get fingerprint/deviceid data callback function.
typedef fit_status_t (*fit_fp_callback)(uint8_t *rawdata,
                                        uint16_t *datalen);

/* Macro Functions **********************************************************/
#define LITTLE_ENDIAN

/* Extern Data **************************************************************/

/* Function Prototypes ******************************************************/

#endif /* __FIT_TYPES_H__ */

