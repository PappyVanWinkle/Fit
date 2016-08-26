/****************************************************************************\
**
** internal.c
**
** Defines functionality for common function use across the project.
** 
** Copyright (C) 2016, SafeNet, Inc. All rights reserved.
**
\****************************************************************************/

#include "parser.h"
#include "internal.h"
#include "hwdep.h"
#include "fit_debug.h"
#include "mem_read.h"
#ifdef FIT_USE_NODE_LOCKING
#include "dm_hash.h"
#endif // ifdef FIT_USE_NODE_LOCKING

extern fit_cache_data fit_cache;

/**
 *
 * fit_verify_license
 *
 * This function is used to validate following:
 *      1. RSA signature of new license.
 *      2. New license node lock varification.
 * On return it will either return validation success or failure.
 *
 * @param   license --> Start address of the license of type fit_pointer_t.
 *                      fit_pointer_t will describe, from what type of
 *                      memory to read the license through function pointer.
 *
 * @param   key --> Start address of the key of type fit_pointer_t.
 *                  fit_pointer_t will describe, from what type of
 *                  memory to read the key through function pointer.
 *
 */
fit_status_t fit_verify_license(fit_pointer_t *license,
                                fit_pointer_t *key,
                                uint8_t check_cache)
{
    fit_status_t status                 = FIT_STATUS_OK;
    uint8_t dmhash[FIT_DM_HASH_SIZE]     = {0};
    fitcontextdata context              = {0};
    fit_pointer_t fitptr                = {0};

#ifdef FIT_USE_NODE_LOCKING
    uint8_t valid_fp_present  = FALSE;
    fit_fingerprint_t licensefp = {0};
    fit_fingerprint_t devicefp = {0};
    fit_fp_callback callback_fn = fit_deviceid_get;
#endif // #ifdef FIT_USE_NODE_LOCKING

    DBG(FIT_TRACE_INFO, "[fit_verify_license]: license=0x%p length=%hd\n", license->data, license->length);

    fitptr.read_byte = license->read_byte;

        // Check validity of license data by RSA signature check.
    if (fit_cache.m_rsa_check_done == TRUE && check_cache == TRUE)
    {
        // Calculate Davies-Meyer-hash on the license. Write that hash into the hash table.
        context.m_level = STRUCT_V2C_LEVEL;
        context.m_index = LICENSE_FIELD;
        context.m_operation = (uint8_t)FIT_PARSE_LICENSE;
        // Parse license data.
        status = fit_parse_object(STRUCT_V2C_LEVEL, LICENSE_FIELD, license, &context);
        if (!(status == FIT_STATUS_OK || status == FIT_STOP_PARSE))
        {
            DBG(FIT_TRACE_ERROR, "Error in license parsing %d\n", status);
            goto bail;
        }

        fitptr.data = (uint8_t *) license->data;
        fitptr.length = context.m_length;
        // Get the hash of data.
        status = fit_davies_meyer_hash(&fitptr, (uint8_t *)&dmhash);
        if (status != FIT_STATUS_OK)
        {
            DBG(FIT_TRACE_ERROR, "Error in getting Davies Meyer hash with status %d\n", status);
            goto bail;
        }
        if(fit_memcmp(fit_cache.m_dm_hash, dmhash, FIT_DM_HASH_SIZE) != 0 )
        {
            status = fit_check_license_validation(license, key);
        }
    }
    else
    {
        status = fit_check_license_validation(license, key);
    }

    if (status != FIT_STATUS_OK)
    {
        DBG(FIT_TRACE_CRITICAL, "fit_check_license_validation failed with error code %d\n", status);
        goto bail;
    }
    else
    {
        DBG(FIT_TRACE_INFO, "fit_check_license_validation successfully passed \n");
    }

    // Check the presence of fingerprint in the license data.
    context.m_level = STRUCT_HEADER_LEVEL;
    context.m_index = FINGERPRINT_FIELD;
    context.m_operation = (uint8_t)FIT_GET_DATA_ADDRESS;
    context.mstatus = FIT_STATUS_OK;
    status = fit_parse_object(STRUCT_V2C_LEVEL, LICENSE_FIELD, license, &context);
    if (status == FIT_STOP_PARSE && context.mstatus == FIT_LIC_FIELD_PRESENT)
    {
#ifndef FIT_USE_NODE_LOCKING
        DBG(FIT_TRACE_ERROR, "Fit core was not compiled with node locking macro \n");
        return FIT_NODE_LOCKING_NOT_SUPP;
#else
        DBG(FIT_TRACE_ERROR, "Fingerprint information is found in license string.\n");
        // get the fingerprint data.
        fitptr.data = context.mparserdata.m_addr;
        getfingerprintdata(&fitptr, &licensefp);

        // License string contains the fingerprint data. Check the magic value.
        if (licensefp.magic == FP_MAGIC)
        {
            DBG(FIT_TRACE_INFO, "Magic number found in license string.\n");
            valid_fp_present = TRUE;
            status = FIT_STATUS_OK;
        }
        else
        {
            DBG(FIT_TRACE_ERROR, "Invalid Magic number in license string.\n");
            status = FIT_INVALID_V2C;
            goto bail;
        }
        if (licensefp.algid != AES_ALGID)
        {
            status = FIT_UNKNOWN_FP_ALG;
            goto bail;
        }
#endif // #ifndef FIT_USE_NODE_LOCKING
   }

#ifdef FIT_USE_NODE_LOCKING
    if (valid_fp_present)
    {
        DBG(FIT_TRACE_INFO, "Get fingerprint information from respective hardware.\n");
        // get fingerprint data of the device and then compare it data present in the license.
        status = fit_get_device_fpblob(&devicefp, callback_fn);
        if (status != FIT_STATUS_OK)
        {
            DBG(FIT_TRACE_INFO, "Error in getting fingerprint data with status %d \n", status);
            goto bail;
        }
        if (devicefp.algid != AES_ALGID)
        {
            status = FIT_UNKNOWN_FP_ALG;
            goto bail;
        }

        if(fit_memcmp(licensefp.hash, devicefp.hash, FIT_DM_HASH_SIZE) != 0 )
        {
            DBG(FIT_TRACE_ERROR, "Fingerprint hash does not match with stored hash in license \n");
            status = FIT_FP_MISMATCH_ERROR;
            goto bail;
        }
        else
        {
            DBG(FIT_TRACE_INFO, "Device fingerprint match with stored fingerprint data in license string\n");
        }
    }
#endif // #ifdef FIT_USE_NODE_LOCKING

bail:
    if (status != FIT_STATUS_OK)
    {
        fit_cache.m_rsa_check_done = FALSE;
        fit_memset(fit_cache.m_dm_hash, 0, sizeof(fit_cache.m_dm_hash));
    }

    return status;
}

#ifdef FIT_USE_NODE_LOCKING
void getfingerprintdata(fit_pointer_t *fpdata, fit_fingerprint_t *fpstruct)
{
    fit_pointer_t fitptr = {0};

    // Get first four bytes of fingerprint data. This will represent magic id.
    fpstruct->magic = read_dword(fpdata->data, fpdata->read_byte);
    // Read algorith id value.
    fpstruct->algid = read_dword(fpdata->data + sizeof(uint32_t), fpdata->read_byte);

    // Get device id hash value
    fitptr.data = fpdata->data+sizeof(uint32_t)+sizeof(uint32_t);
    fitptr.length = FIT_DM_HASH_SIZE;
    fitptr.read_byte = fpdata->read_byte;
    fitptr_memcpy(fpstruct->hash, &fitptr);
}
#endif // ifdef FIT_USE_NODE_LOCKING

void fit_memcpy(uint8_t *dst, uint8_t *src, uint16_t srclen)
{
    uint16_t cntr = 0;

    for (cntr = 0; cntr < srclen; ++cntr)
        *dst++ = *src++;
}

void fitptr_memcpy(uint8_t *dst, fit_pointer_t *src)
{
    uint16_t cntr = 0;

    for (cntr = 0; cntr < src->length; ++cntr)
        *dst++ = src->read_byte(src->data + cntr);
}

int16_t fit_memcmp(uint8_t *pdata1, uint8_t *pdata2, uint16_t len)
{
    uint16_t cntr = 0;

    for (cntr = 0; cntr < len; ++cntr)
    {
        if (*pdata1++ == *pdata2++)
            continue;
        else
        {
            return (len - cntr);
        }
    }

    return 0;
}

void fit_memset(uint8_t *pdata, uint8_t value, uint16_t len)
{
    uint16_t cntr = 0;

    for (cntr = 0; cntr < len; ++cntr)
    {
        *pdata = value;
        pdata++;
    }
}

