/****************************************************************************\
**
** aes.c
**
** Defines functionality for implementation for AES algorithm
** 
** Copyright (C) 2016, SafeNet, Inc. All rights reserved.
**
\****************************************************************************/

#include "fit_aes.h"

static const uint8_t sbox[256] = {
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16 };

static const uint8_t Rcon[255] = {
    0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 
    0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 
    0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 
    0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 
    0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 
    0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 
    0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 
    0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 
    0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 
    0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 
    0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 
    0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 
    0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 
    0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 
    0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 
    0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb  };


static uint8_t get_sbox_value(uint8_t num)
{
    return sbox[num];
}

/**
 *
 * aes_setup
 *
 * This function produces Nb(Nr+1) round keys. The round keys are used in each
 * round to decrypt the states
 *
 * @param   aes --> Pointer to structure containing aes state.
 * @param   key --> AES encryption/decryption key
 * @param   keylen --> key length
 * @param   skey <-- On return it will contains the Nb(Nr+1) round keys
 *
 */
fit_status_t aes_setup(aes_state_t* aes, const uint8_t *key, uint16_t keylen, uint8_t *skey)
{
    uint32_t i = 0, j = 0;
    uint8_t k = 0;
    uint8_t tempa[4] = {0}; // Used for the column/row operations

    // As of now we only support 16 byte (128 bit) and 32 byte (256 bits) key length
    if (!(keylen == AES_128_KEY_LENGTH || keylen == AES_256_KEY_LENGTH))
    {
        DBG(FIT_TRACE_ERROR, "aes_setup - Invalid Keysize %d", keylen);
        return FIT_INVALID_KEYSIZE;
    }

    if( keylen == AES_128_KEY_LENGTH ){
        aes->Nk = 4;
        aes->Nr = 10;
        aes->keylen = AES_128_KEY_LENGTH;
    }

    if( keylen == AES_256_KEY_LENGTH ){
        aes->Nk = 8; 
        aes->Nr = 14; 
        aes->keylen = AES_256_KEY_LENGTH;
    }

    // The first round key is the key itself.
    for(i = 0; i < aes->Nk; ++i)
    {
        skey[(i * 4) + 0] = key[(i * 4) + 0];
        skey[(i * 4) + 1] = key[(i * 4) + 1];
        skey[(i * 4) + 2] = key[(i * 4) + 2];
        skey[(i * 4) + 3] = key[(i * 4) + 3];
    }

    // All other round keys are found from the previous round keys.
    for(; (i < (Nb * (aes->Nr + 1))); ++i)
    {
        for(j = 0; j < 4; ++j)
        {
            tempa[j] = skey[(i-1) * 4 + j];
        }
        if (i % aes->Nk == 0)
        {
            // This function rotates the 4 bytes in a word to the left once.
            // [a0,a1,a2,a3] becomes [a1,a2,a3,a0]

            // Function RotWord()
            {
                k = tempa[0];
                tempa[0] = tempa[1];
                tempa[1] = tempa[2];
                tempa[2] = tempa[3];
                tempa[3] = k;
            }

            // SubWord() is a function that takes a four-byte input word and 
            // applies the S-box to each of the four bytes to produce an output word.

            // Function Subword()
            {
                tempa[0] = get_sbox_value(tempa[0]);
                tempa[1] = get_sbox_value(tempa[1]);
                tempa[2] = get_sbox_value(tempa[2]);
                tempa[3] = get_sbox_value(tempa[3]);
            }

            tempa[0] = tempa[0] ^ Rcon[i/aes->Nk];
        }
        else if( aes->Nk > 6 && i % aes->Nk == 4 )
        {
            tempa[0] = get_sbox_value(tempa[0]);
            tempa[1] = get_sbox_value(tempa[1]);
            tempa[2] = get_sbox_value(tempa[2]);
            tempa[3] = get_sbox_value(tempa[3]);
        }

        skey[i * 4 + 0] = skey[(i - aes->Nk) * 4 + 0] ^ tempa[0];
        skey[i * 4 + 1] = skey[(i - aes->Nk) * 4 + 1] ^ tempa[1];
        skey[i * 4 + 2] = skey[(i - aes->Nk) * 4 + 2] ^ tempa[2];
        skey[i * 4 + 3] = skey[(i - aes->Nk) * 4 + 3] ^ tempa[3];
    }

    return FIT_STATUS_OK;
}

/**
 *
 * add_round_key
 *
 * This function adds the round key to state. The round key is added to the state
 * by an XOR function.
 *
 */
void add_round_key(uint8_t *state, const uint8_t *key, uint16_t round)
{
    uint8_t i = 0, j = 0;

    for(i=0;i<4;++i)
    {
        for(j = 0; j < 4; ++j)
        {
            state[4*i+j] ^= key[(round * Nb * 4 + i * Nb + j)];
        }
    }
}

/**
 *
 * sub_bytes
 *
 * This function Substitutes the values in the state matrix with values in an S-box.
 *
 */
void sub_bytes(uint8_t *state)
{
    uint8_t i = 0, j = 0;

    for(i = 0; i < 4; ++i)
    {
        for(j = 0; j < 4; ++j)
        {
            state[4*j+i] = get_sbox_value(state[4*j+i]);
        }
    }
}

/**
 *
 * shift_rows
 *
 * This function shifts the rows in the state to the left. Each row is shifted
 * with  different offset.
 * Offset = Row number. So the first row is not shifted.
 *
 */
void shift_rows(uint8_t *state)
{
    uint8_t temp = 0;

    // Rotate first row 1 columns to left  
    temp         = state[(4*0+1)];
    state[4*0+1] = state[(4*1+1)];
    state[4*1+1] = state[(4*2+1)];
    state[4*2+1] = state[(4*3+1)];
    state[4*3+1] = temp;

    // Rotate second row 2 columns to left  
    temp         = state[(4*0+2)];
    state[4*0+2] = state[(4*2+2)];
    state[4*2+2] = temp;

    temp         = state[(4*1+2)];
    state[4*1+2] = state[(4*3+2)];
    state[4*3+2] = temp;

    // Rotate third row 3 columns to left
    temp         = state[(4*0+3)];
    state[4*0+3] = state[(4*3+3)];
    state[4*3+3] = state[(4*2+3)];
    state[4*2+3] = state[(4*1+3)];
    state[4*1+3] = temp;
}

static uint8_t xtime(uint8_t x)
{
    return ((x<<1) ^ (((x>>7) & 1) * 0x1b));
}

/**
 *
 * mix_columns
 *
 * This function mixes the columns of the state matrix
 *
 */
void mix_columns(uint8_t *state)
{
    uint8_t i = 0;
    uint8_t Tmp = 0, Tm = 0, t = 0;

    for(i = 0; i < 4; ++i)
    {  
        t   = state[(4*i+0)];
        Tmp = state[(4*i+0)] ^ state[(4*i+1)] ^ state[(4*i+2)] ^ state[(4*i+3)] ;
        Tm  = state[(4*i+0)] ^ state[(4*i+1)] ; Tm = xtime(Tm);  state[4*i+0] ^= Tm ^ Tmp ;
        Tm  = state[(4*i+1)] ^ state[(4*i+2)] ; Tm = xtime(Tm);  state[4*i+1] ^= Tm ^ Tmp ;
        Tm  = state[(4*i+2)] ^ state[(4*i+3)] ; Tm = xtime(Tm);  state[4*i+2] ^= Tm ^ Tmp ;
        Tm  = state[(4*i+3)] ^ t ;        Tm = xtime(Tm);  state[4*i+3] ^= Tm ^ Tmp ;
    }
}

// Cipher is the main function that encrypts the PlainText.
static void encrypt(aes_state_t* aes, const uint8_t* key, uint8_t *state)
{
    uint8_t round = 0;

    // Add the First round key to the state before starting the rounds.
    add_round_key(state, key, 0); 

    // There will be Nr rounds.
    // The first Nr-1 rounds are identical.
    // These Nr-1 rounds are executed in the loop below.
    for(round = 1; round < aes->Nr; ++round)
    {
        sub_bytes(state);
        shift_rows(state);
        mix_columns(state);
        add_round_key(state, key, round);
    }

    // The last round is given below.
    // The MixColumns function is not here in the last round.
    sub_bytes(state);
    shift_rows(state);
    add_round_key(state, key, aes->Nr);
}

void block_copy(uint8_t* output, uint8_t* input)
{
    uint8_t i = 0;

    for (i=0; i<AES_OUTPUT_DATA_SIZE; ++i)
    {
        output[i] = input[i];
    }
}

/**
 *
 * aes_encrypt
 *
 * Performs encryption on given input data and key and produce encrypted data.
 *
 * @param   aes --> Pointer to structure containing aes state.
 * @param   input --> Plain data i.e. data to be encrypted.
 * @param   output <-- Encrypted data.
 * @param   key --> Encryption key used in AES.
 *
 */
void aes_encrypt(aes_state_t* aes,
                 uint8_t* input,
                 uint8_t* output,
                 const uint8_t* key,
                 uint8_t *state)
{
    // Copy input to output, and work in-memory on output
    block_copy(output, input);
    state = (uint8_t*)output;

    // The next function call encrypts the PlainText with the Key using AES algorithm.
    encrypt(aes, key, state);
}

