/****************************************************************************\
**
** fit_rsa.c
**
** Defines functionality for rsa verification process
** 
** Copyright (C) 2016, SafeNet, Inc. All rights reserved.
**
\****************************************************************************/

#include <string.h>

#include "fit_rsa.h"
#include "hwdep.h"
#include "fit_debug.h"
#include "mbedtls\pk.h"

/**
 *
 * fit_validate_rsa_signature
 *
 * This function is to validate rsa signature and hash against rsa public key.
 * Returns FIT_INVALID_V2C or FIT_STATUS_OK
 *
 * @param   signature   --> fit_pointer to the signature (part of license)
 * @param   hash        --> RAM pointer to hash to be verified
 * @param   key         --> fit_pointer to RSA public key
 *
 */
fit_status_t fit_validate_rsa_signature(fit_pointer_t *signature,
                                        uint8_t       *hash,
                                        fit_pointer_t *key)
{
    mbedtls_pk_context pk;
    uint8_t pubkey_or_sig[512] = {0}; /* share memory */
    int i, ret;

    /* read pubkey into RAM */
    for (i = 0; i < key->length; i++) 
        pubkey_or_sig[i] = key->read_byte(key->data + i);

    mbedtls_pk_init( &pk );

    ret = mbedtls_pk_parse_public_key( &pk, (const unsigned char *)pubkey_or_sig, key->length + 1);
    if (ret) {
        DBG(FIT_TRACE_ERROR, "[fit_validate_rsa_signature] parsing public key FAILED -0x%04x\n", -ret);
        goto exit;
    }
    DBG(FIT_TRACE_INFO, "[fit_validate_rsa_signature] public key is accepted\n" );

    /* read signature from license memory */
    for (i = 0; i < 256; i++)
        pubkey_or_sig[i] = signature->read_byte(signature->data + i);

    ret = mbedtls_pk_verify(&pk, MBEDTLS_MD_SHA256, hash, 32, pubkey_or_sig, 256);
    if (ret) {
        DBG(FIT_TRACE_ERROR, "[fit_validate_rsa_signature] verify FAILED -0x%04x\n", -ret);
        goto exit;
    }

    DBG(FIT_TRACE_INFO, "[fit_validate_rsa_signature] verify OK\n" );
    ret = 0;

 exit:
    mbedtls_pk_free( &pk );

    if (ret) return FIT_RSA_VERIFY_FAILED;
    return FIT_STATUS_OK;

}
