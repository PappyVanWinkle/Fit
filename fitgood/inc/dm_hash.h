/****************************************************************************\
**
** dm_hash.h
**
** Contains declaration for macros, constants and functions used in implementation
** for Davies Meyer hash function.
** 
** Copyright (C) 2016, SafeNet, Inc. All rights reserved.
**
\****************************************************************************/

#ifndef __FIT_DM_HASH_H__
#define __FIT_DM_HASH_H__

/* Required Includes ********************************************************/
#include "fit.h"

/* Constants ****************************************************************/

#define DM_CIPHER_BLOCK_SIZE                16

/* Types ********************************************************************/

/* Function Prototypes ******************************************************/

// This function will be used to get the davies meyer hash of the data passed in.
fit_status_t fit_davies_meyer_hash(fit_pointer_t *pdata, uint8_t *dmhash);
// This function will be used to pad the data to make it�s length be an even multiple
// of the block size and include a length encoding
void fit_dm_hash_init(uint8_t *pdata, uint16_t *pdatalen, uint16_t msgfulllen);

#endif // __FIT_DM_HASH_H__
