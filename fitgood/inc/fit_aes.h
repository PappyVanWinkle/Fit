/****************************************************************************\
**
** aes.h
**
** Contains declaration for macros, constants and functions used in implementation
** for AES algorithm. Currently we support AES 128 and AES 256.
** 
** Copyright (C) 2016, SafeNet, Inc. All rights reserved.
**
\****************************************************************************/

#ifndef __FIT_AES_H__
#define __FIT_AES_H__

/* Required Includes ********************************************************/
#include "fit.h"

/* Constants ****************************************************************/
#define AES_128_KEY_LENGTH      16  // Key length in bytes [128 bits]
#define AES_256_KEY_LENGTH      32  // Key length in bytes [256 bits]
#define AES_OUTPUT_DATA_SIZE    16  // encrypted output size from AES algorithm
// The number of columns comprising a state in AES. This is a constant in AES. Value=4
#define Nb                      4

/* Types ********************************************************************/

typedef struct aes {
    uint16_t Nk;        // The number of 32 bit words in a key.
    uint16_t Nr;        // The number of rounds in AES Cipher.
    uint16_t keylen;    // Key size (128 or 256 bits)
} aes_state_t;

/* Function Prototypes ******************************************************/

void aes_encrypt(aes_state_t* aes, uint8_t* input, uint8_t* output, const uint8_t* key, uint8_t *state);
fit_status_t aes_setup(aes_state_t* aes, const uint8_t *key, uint16_t keylen, uint8_t *skey);
void add_round_key(uint8_t *state, const uint8_t *key, uint16_t round);
void sub_bytes(uint8_t *state);
void shift_rows(uint8_t *state);
void mix_columns(uint8_t *state);
void block_copy(uint8_t* output, uint8_t* input);

#endif // __FIT_AES_H__

