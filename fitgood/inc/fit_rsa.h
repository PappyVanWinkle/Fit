/****************************************************************************\
**
** rsa.h
**
** Contains declaration for macros, constants and functions used in implementation
** for RSA algorithm
** 
** Copyright (C) 2016, SafeNet, Inc. All rights reserved.
**
\****************************************************************************/

#ifndef __FIT_RSA_H__
#define __FIT_RSA_H__

/* Required Includes ********************************************************/
#include "fit_types.h"
#include "mem_read.h"

/* Constants ****************************************************************/
#define RSA_SIG_SIZE            256

/* Types ********************************************************************/

/* Function Prototypes ******************************************************/

fit_status_t fit_validate_rsa_signature(fit_pointer_t *signature,
                                        uint8_t       *hash,
                                        fit_pointer_t *key);

#endif // __FIT_RSA_H__

