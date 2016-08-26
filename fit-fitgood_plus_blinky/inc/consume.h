/****************************************************************************\
**
** consumelic.h
**
** Defines functionality for consuming licenses for embedded devices.
**
** Copyright (C) 2016, SafeNet, Inc. All rights reserved.
**
\****************************************************************************/

#ifndef __FIT_CONSUME_LIC_H__
#define __FIT_CONSUME_LIC_H__

/* Required Includes ********************************************************/
#include "mem_read.h"

/* Constants ****************************************************************/

/* Forward Declarations *****************************************************/

/* Types ********************************************************************/

/* Function Prototypes ******************************************************/

// This function will check whether pdata contains the feature_id that needs to be consume.
fit_status_t fit_consume_license(fit_pointer_t *pdata,
                                 uint8_t level,
                                 uint8_t index,
                                 uint16_t length,
                                 void *context);

#endif /* __FIT_CONSUME_LIC_H__ */

