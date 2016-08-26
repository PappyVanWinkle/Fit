/****************************************************************************\
**
** abreast_dm.h
**
** Contains declaration for strctures, enum, constants and functions used in
** abreast dm hash implementation. Abreast DM hash is performed over license data
** and internally uses AES 256 for encryption.
**
** Copyright (C) 2016, SafeNet, Inc. All rights reserved.
**
\****************************************************************************/

#ifndef __FIT_ABREAST_DM_H__
#define __FIT_ABREAST_DM_H__

/* Required Includes ********************************************************/

#include "fit_types.h"
#include "internal.h"
#include "mem_read.h"

/* Constants ****************************************************************/
#define ABREAST_DM_CIPHER_BLOCK_SIZE        16
#define ABREAST_DM_HASH_SIZE                32

/* Types ********************************************************************/

/* Function Prototypes ******************************************************/

void AES256_AbreastDmHash_Finalize(uint8_t* Hash);
void AES256_AbreastDmHash_UpdateBlk(uint8_t* pDataIn, uint8_t* Hash);
void AES256_AbreastDmHash_Update(uint8_t* pDataIn, uint16_t NbOfBlocks, uint8_t* Hash);
void AES256_AbreastDmHash_Init(uint8_t* Hash);
fit_status_t fit_get_AbreastDM_Hash(fit_pointer_t *msg, uint8_t * hash);


#endif // __FIT_ABREAST_DM_H__
