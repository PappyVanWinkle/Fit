/****************************************************************************\
**
** validate.c
**
** Defines functionality for validate license for embedded devices.
** 
** Copyright (C) 2016, SafeNet, Inc. All rights reserved.
**
\****************************************************************************/

#include "parser.h"
#include "fit_debug.h"
#include "internal.h"

/**
 *
 * fit_validate_license
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
fit_status_t fit_licenf_validate_license(fit_pointer_t *license,
                                         fit_pointer_t *key)
{
    fit_status_t status = FIT_STATUS_OK;

    DBG(FIT_TRACE_INFO, "[fit_validate_license]: pdata=0x%p \n", license->data);

    if (license->read_byte == NULL)
        return FIT_INVALID_PARAM_1;

    if (key->read_byte == NULL)
        return FIT_INVALID_PARAM_2;

    status = fit_verify_license(license, key, FALSE);

    return status;
}

