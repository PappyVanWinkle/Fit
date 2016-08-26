/****************************************************************************\
**
** getinfo.h
**
** Defines functionality for getting license information for embedded devices.
** 
** Copyright (C) 2016, SafeNet, Inc. All rights reserved.
**
\****************************************************************************/

#ifndef __FIT_GET_INFO_H__
#define __FIT_GET_INFO_H__

/* Required Includes ********************************************************/
#include "mem_read.h"

/* Constants ****************************************************************/

/* Forward Declarations *****************************************************/

/* Types ********************************************************************/

/* Function Prototypes ******************************************************/

// This function will get the vendor information from the license data passed in.
fit_status_t fit_get_vendor_id(fit_pointer_t *pdata,
                               uint8_t level,
                               uint8_t index,
                               uint16_t length,
                               void *contextP);

fit_status_t fit_get_license_uid(fit_pointer_t *pdata,
                                 uint8_t level,
                                 uint8_t index,
                                 uint16_t length,
                                 void *context);

#endif /* __FIT_GET_INFO_H__ */

