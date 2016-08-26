/****************************************************************************\
**
** dm_hash.c
**
** Defines functionality for implementation for davies meyer hash function.
** 
** Copyright (C) 2016, SafeNet, Inc. All rights reserved.
**
\****************************************************************************/

#include "dm_hash.h"
#include "internal.h"
#include "parser.h"
#include "fit_aes.h"

#define ROUNDS_KEY_LENGTH       176

/**
 *
 * fit_dm_hash_init
 *
 * This function will be used to pad the data to make it’s length be an even multiple
 * of the block size and include a length encoding. This is done by padding with zeros
 * to the next size which is an odd multiple of 64 bits and then appending a 64-bit
 * big-endian encoding of the number of bits of license data length.
 *
 * @param   pdata --> Pointer to data that needs to be hashed.
 * @param   datalen <--> Length of final data after data is padded and encoded.
 * @param   msgfulllen --> Message length for which hash needs to be calculated.
 *                         This is different than pdatalen as this function is called
 *                         for only last block of data (to avoid overuse of stack size
 *                         for long messages)
 *
 */
void fit_dm_hash_init(uint8_t *pdata, uint16_t *pdatalen, uint16_t msgfulllen)
{
    uint16_t length         = 0;
    uint16_t sizeinbits     = 0;
    uint16_t cntr           = 0;
    uint8_t zeropads        = 0;

    DBG(FIT_TRACE_INFO, "\nfit_dm_hash_init..\n");

    length = *pdatalen;
    sizeinbits= msgfulllen*8;
    zeropads = ((DM_CIPHER_BLOCK_SIZE/2)-(length%(DM_CIPHER_BLOCK_SIZE/2)));

    // Pad with zeros to the next size which is an odd multiple of 64 bits
    for(cntr=0; cntr < zeropads; cntr++)
    {
        pdata[length++] = 0x00;
    }
    if ((length%DM_CIPHER_BLOCK_SIZE) == 0)
    {
        for(cntr=0; cntr < DM_CIPHER_BLOCK_SIZE/2; cntr++)
        {
            pdata[length++] = 0x00;
        }
    }

    // Append a 64-bit big-endian encoding of the number of bits to the license data
    pdata[length++] = 0x00;
    pdata[length++] = 0x00;
    pdata[length++] = 0x00;
    pdata[length++] = 0x00;
    pdata[length++] = 0x00;
    pdata[length++] = 0x00;
    pdata[length++] = sizeinbits >> 8;
    pdata[length++] = sizeinbits >> 0;

    *pdatalen = length;
}

/**
 *
 * fit_davies_meyer_hash
 *
 * This function will be used to get the davies meyer hash of the data passed in.
 * This is performed by first splitting the data (message m) into 128 bits (m1 … mn)
 * For each of the 128 bit sub-block, calculate
 *      Hi = AES (Hi-1, mi)  XOR Hi-1
 * The final Hash is calculated as:
 *      H = AES (Hn, Hn) XOR Hn
 *
 * @param   pdata --> Pointer to data that needs to be hashed.
 * @param   datalen <--> Length of above data.
 * @param   dmhash <-- On return this will contain the davies mayer hash of data
 *                     passed in.
 *
 */
fit_status_t fit_davies_meyer_hash(fit_pointer_t *pdata, uint8_t *dmhash)
{
    fit_status_t  status            = FIT_STATUS_OK;
    uint8_t skey[ROUNDS_KEY_LENGTH]   = {0};
    uint8_t aes_state[4][4]         = {{0}};
    uint16_t cntr                   = 0;
    uint16_t cntr2                  = 0;
    uint8_t output[16]              = {0};
    uint8_t prev_hash[16]           = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                                       0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    aes_state_t aes = {0};
    uint8_t tempmsg[32]     = {0};
    uint16_t msglen         = 0;
    fit_pointer_t fitptr    = {0};

    // Initialize the read pointer.
    fitptr.read_byte = pdata->read_byte;

    // For each of the 128 bit sub-block, calculate
    //      Hi = AES (Hi-1, mi)  XOR Hi-1
    for (cntr = 0; cntr < pdata->length; cntr+=16)
    {
        if ((cntr+16) < pdata->length)
        {
            // Initialize the aes context
            status = aes_setup(&aes, (pdata->data)+cntr, AES_128_KEY_LENGTH, skey);
            if (status != FIT_STATUS_OK)
            {
                DBG(FIT_TRACE_ERROR, "failed to initialize aes setup error =%d\n", status);
                return status;
            }

            fit_memset((uint8_t*)aes_state, 0, sizeof(aes_state));
            fit_memset((uint8_t*)output, 0, sizeof(output));
            // Encrypt data (AES 128)
            aes_encrypt(&aes, prev_hash, output, skey, (uint8_t*)aes_state);
            for (cntr2 = 0; cntr2 < 16; cntr2++)
            {
                dmhash[cntr2] = output[cntr2] ^ prev_hash[cntr2];
            }
            fit_memcpy(prev_hash, dmhash, 16);
        }
    }
    cntr -= 16;

    // Pad the last block of data (last block will always be less than 16 bytes)
    // and calculate Hi = AES (Hi-1, mi)  XOR Hi-1 
    fitptr.data = pdata->data+cntr;
    fitptr.length = pdata->length-cntr;
    msglen = fitptr.length;
    fitptr_memcpy(tempmsg, &fitptr);

    // Do padding for the last block of data.
    fit_dm_hash_init(tempmsg, &msglen, pdata->length);
    // For each of the 128 bit sub-block, calculate
    //      Hi = AES (Hi-1, mi)  XOR Hi-1
    for (cntr = 0; cntr < msglen; cntr+=16)
    {
        // Initialize the aes context
        status = aes_setup(&aes, tempmsg+cntr, AES_128_KEY_LENGTH, skey);
        if (status != FIT_STATUS_OK)
        {
            DBG(FIT_TRACE_ERROR, "failed to initialize aes setup error =%d\n", status);
            return status;
        }

        fit_memset((uint8_t*)aes_state, 0, sizeof(aes_state));
        fit_memset((uint8_t*)output, 0, sizeof(output));
        // Encrypt data (AES 128)
        aes_encrypt(&aes, prev_hash, output, skey, (uint8_t*)aes_state);
        for (cntr2 = 0; cntr2 < 16; cntr2++)
        {
            dmhash[cntr2] = output[cntr2] ^ prev_hash[cntr2];
        }
        fit_memcpy(prev_hash, dmhash, 16);
    }

    // The final Hash is calculated as:
    //      H = AES (Hn, Hn) XOR Hn
    // Initialize the aes context
    status = aes_setup(&aes, prev_hash, AES_128_KEY_LENGTH, skey);
    if (status != FIT_STATUS_OK)
    {
        DBG(FIT_TRACE_ERROR, "failed to initialize aes setup error =%d\n", status);
        return status;
    }

    fit_memset((uint8_t*)aes_state, 0, sizeof(aes_state));
    fit_memset((uint8_t*)output, 0, sizeof(output));
    // Encrypt data (AES 128)
    aes_encrypt(&aes, prev_hash, output, skey, (uint8_t*)aes_state);
    for (cntr2 = 0; cntr2 < 16; cntr2++)
    {
        dmhash[cntr2] = output[cntr2] ^ prev_hash[cntr2];
    }

    return status;
}

