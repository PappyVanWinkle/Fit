/****************************************************************************\
**
** get_info.c
**
** Defines functionality for getting license information for embedded devices.
** 
** Copyright (C) 2016, SafeNet, Inc. All rights reserved.
**
\****************************************************************************/
#include "parser.h"
#include "internal.h"

/**
 *
 * fit_licenf_get_info
 *
 * This function will parse the license binary passed to it and call the user provided
 * callback function for every field data. User can take any action on receiving
 * license field data like storing values in some structure or can take some action
 * like consume license etc.
 *
 * @param   license --> Start address of the license in binary format, depending on
 *                      your READ_LICENSE_BYTE definition e.g. in case of RAM, this
 *                      can just be the memory address of the license variable 
 * @param   callback_fn --> User provided callback function to be called by fit core.
 * @param   context <--> Pointer to user provided data structure.
 *
 */
fit_status_t fit_licenf_get_info(fit_pointer_t* license,
                                 fit_get_info_callback callback_fn,
                                 void *context)
{
    fit_status_t status         = FIT_STATUS_OK;
    fitcontextdata getinfo    = {0UL};

    DBG(FIT_TRACE_INFO, "[fit_licenf_get_info]: pdata=0x%p \n", license);

    /* Validate parameters */
    if (callback_fn == NULL) {
        return FIT_INVALID_PARAM_2;
    }
    if (context == NULL) {
        return FIT_INVALID_PARAM_3;
    }

    /* Initialize context for get info operation. */
    getinfo.m_operation = (uint8_t)FIT_GET_LICENSE_INFO_DATA;
    getinfo.mparserdata.m_getinfodata.m_callback_fn = callback_fn;
    getinfo.mparserdata.m_getinfodata.m_get_info_data = context;

    /* Parse license data and call the user provided callback fn for each field. */
    status = fit_parse_object(STRUCT_V2C_LEVEL, LICENSE_FIELD, license, (void *)&getinfo);

    if (!(status == FIT_STATUS_OK || status == FIT_CONTINUE_PARSE)) {
        DBG(FIT_TRACE_ERROR, "[fit_licenf_get_info]: return with error code %d \n", status);
        return status;
    }

    return FIT_STATUS_OK;
}
