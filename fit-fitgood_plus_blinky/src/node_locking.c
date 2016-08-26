/****************************************************************************\
**
** node_locking.c
**
** Defines functionality for consuming licenses for embedded devices.
** 
** Copyright (C) 2016, SafeNet, Inc. All rights reserved.
**
\****************************************************************************/

#include "dm_hash.h"
#include "internal.h"

/**
 *
 * fit_get_device_fpblob
 *
 * This function will fetch fingerprint/deviceid for the respective board. This will
 * call the hardware implemented callback function which will give raw data that would
 * be unique to each device. Raw data would be then hash with Daview Meyer hash function.
 *
 * @param   fp <-- Pointer to fingerprint data that need to be filled in.
 * @param   callback_fn --> hardware implemented callback function that will return
 *                          raw fingerprint data and its length.
 *
 */
fit_status_t fit_get_device_fpblob(fit_fingerprint_t* fp, fit_fp_callback callback_fn)
{
    fit_status_t status = FIT_STATUS_OK;
    uint8_t rawdata[64]    = {0}; // Maximum length of device id is 64 bytes.
    uint16_t datalen    = 0;
    uint8_t dmhash[16]  = {0};
    uint16_t cntr       = 0;
    fit_pointer_t fitptr = {0};

    // TODO assign read pointer.
    fitptr.read_byte = (fit_read_byte_callback_t )FIT_READ_BYTE_RAM;

    // Get the hardware fingerprint data.
    status = callback_fn(rawdata, &datalen);
    if (status != FIT_STATUS_OK)
    {
        DBG(FIT_TRACE_ERROR, "Error in getting fingerprint data with status %d\n", status);
        return status;
    }
    if (datalen < 4 || datalen > 64)
        return FIT_INVALID_DEVICE_LEN;

    // Print fingerprint raw data 
    for (cntr=0; cntr<datalen; cntr++) DBG(FIT_TRACE_INFO, "%X ", rawdata[cntr]);
    DBG(FIT_TRACE_INFO, "\n");

    fitptr.length = datalen;
    fitptr.data = (uint8_t *)rawdata;
    // Get the Davies Meyer hash of fingerprint data.
    status = fit_davies_meyer_hash(&fitptr, (uint8_t *)&dmhash);
    if (status != FIT_STATUS_OK)
    {
        DBG(FIT_TRACE_ERROR, "Error in getting Davies Meyer hash with status %d\n", status);
        return status;
    }

    // Print fingerprint hash data (Davies Meyer Hash)
    DBG(FIT_TRACE_INFO, "\nDavoes Meyer hash of fingerprint data: ");
    for (cntr=0; cntr<FIT_DM_HASH_SIZE; cntr++) DBG(FIT_TRACE_INFO, "%X ", dmhash[cntr]);
    DBG(FIT_TRACE_INFO, "\n");

    // Fill fingerprint data.
    fp->algid = AES_ALGID; // AES algorithm used for davies meyer hash function.
    fp->magic = FP_MAGIC; //'fitF' Magic no.
    fit_memcpy(fp->hash, dmhash, FIT_DM_HASH_SIZE); // copy fingerprint hash data.

    return status;
}

