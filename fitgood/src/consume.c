/****************************************************************************\
**
** consumelic.c
**
** Defines functionality for consuming licenses for embedded devices.
** 
** Copyright (C) 2016, SafeNet, Inc. All rights reserved.
**
\****************************************************************************/

#include "parser.h"
#include "internal.h"
#include "hwdep.h"
#include "fit_debug.h"


fit_cb_time_get_t fit_time_get_callback = FIT_TIME_GET;

/**
 *
 * fit_consume_license
 *
 * This function will check whether license contains the feature_id that needs to be
 * consume. If license contains the requested feature_id value then this function will
 * sends FIT_STOP_PARSING status.
 *
 * @param   pdata --> Pointer to data that contains feature id value.
 * @param   level --> level/depth of license schema.
 * @param   index --> structure index in license schema.
 * @param   length --> Length of the requested information in bytes.
 * @param   context <--> Core Fit context data.
 *
 */
fit_status_t fit_consume_license(fit_pointer_t *pdata,
                                   uint8_t level,
                                   uint8_t index,
                                   uint16_t length,
                                   void *context)
{
    uint32_t integer            = 0;
    fitcontextdata *pcontext  = (fitcontextdata *) NULL;
    // Get the field type corresponding to level and index.
    wire_type_t type            = get_field_type(level, index);

    DBG(FIT_TRACE_INFO, "[fit_consume_license]: level=%d, index=%d, type=%d, pdata=%08p # \n",
        level, index, type, pdata->data);

    if (type != FIT_INTEGER)
        return FIT_CONTINUE_PARSE;
    if (pdata == NULL)
        return FIT_INVALID_PARAM_1;
    if (context == NULL)
        return FIT_INVALID_PARAM_5;

    pcontext = (fitcontextdata *)context;
    DBG(FIT_TRACE_INFO, "Looking for Feature ID: %u\n", pcontext->mparserdata.m_id);

    // Check we are at correct level and index.
    if (level == STRUCT_FEATURE_LEVEL && index == ID_FEATURE_FIELD)
    {
        // Get integer value. Integer value can be 16 bit value or 32 bit value.
        if (length == sizeof(uint16_t))
            integer = (read_word(pdata->data, pdata->read_byte)/2)-1;
        else if (length == sizeof(uint32_t))
            integer = read_dword(pdata->data, pdata->read_byte);

        // Check if this feature id is what we are looking for.
        if (((fitcontextdata *)pcontext)->mparserdata.m_id == integer)
        {
            DBG(FIT_TRACE_INFO, "Feature id %u is present.\n", integer);
            ((fitcontextdata *)pcontext)->mstatus = FIT_FEATURE_ID_FOUND;
            return FIT_STOP_PARSE;
        }
    }

    return FIT_STATUS_OK;
}

/**
 *
 * fit_getunixtime
 *
 * This function is used for calling hardware dependent callback fn which will
 * return the current time in unix. If callback function is NULL or not defined
 * then return "license expiration not supported" error.
 *
 * @param   unixtime <--> Pointer to integer that will contain the current time.
 *
 */
fit_status_t fit_getunixtime(uint32_t *unixtime)
{
    uint32_t time = 0;

    if (fit_time_get_callback == NULL )
        return FIT_LIC_EXPIRATION_NOT_SUPP;

    time = fit_time_get_callback();
    *unixtime = time;

    return FIT_STATUS_OK;
}

/**
 *
 * fit_get_lic_prop_model
 *
 * This function is used for getting license model present in license property
 * structure data passed in.
 *
 * @param   pdata --> Pointer to license property structure data.
 * @param   licmodel <-- Pointer to structure that will contain license model
 *                       against data passed in.
 *
 */
fit_status_t fit_get_lic_prop_model(fit_pointer_t *pdata, fitlicensemodel *licmodel)
{
    uint16_t cntr       = 0;
    // skip_fields represents number of fields to skip or number of fields that
    // does not have any data in license binary.
    uint8_t skip_fields = 0;
    uint8_t cur_index   = 0;
    uint8_t index       = 0;
    uint8_t *temp       = pdata->data;
    uint16_t num_fields = read_word(pdata->data, pdata->read_byte);
    uint16_t field_data = 0;

    DBG(FIT_TRACE_INFO, "[fit_get_lic_prop]: pdata=%08p # \n", pdata);

    if (licmodel == NULL)
        return FIT_INVALID_PARAM_2;

    // Move data pointer to next field.
    pdata->data   = pdata->data + PFIELD_SIZE;

    // Parse all fields data in a structure.
    for( cntr = 0; cntr < num_fields; cntr++)
    {
        field_data = read_word(pdata->data, pdata->read_byte);
        if( field_data == 0 )
        {
            index = cur_index;
            // Go to next index value.
            cur_index++;
        }
        else if( field_data & 1)
        {
            index = cur_index;
            skip_fields  = (uint8_t)(field_data+1)/2;
            // skip the fields as it does not contain any data in V2C.
            cur_index    = cur_index + skip_fields;
        }
        else if(field_data%2 == 0)
        {
            index = cur_index;
            // Go to next index value.
            cur_index++;
        }

        if (index == PERPETUAL_FIELD)
        {
            licmodel->perpetual = (read_word(pdata->data, pdata->read_byte)/2) - 1;
        }
        else if (index == START_DATE_FIELD)
            licmodel->startdate = TRUE;
        else if (index == END_DATE_FIELD)
            licmodel->enddate = TRUE;

        // Move data pointer to next field.
        pdata->data = pdata->data + PFIELD_SIZE;
    }

    pdata->data = temp;

    return FIT_STATUS_OK;
}

/**
 *
 * fit_licenf_consume_license
 *
 * This function is used to grant or deny access to different areas of functionality
 * in the software. This feature is similar to login type operation on licenses. It
 * will look for presence of feature id in the license binary.
 *
 * @param   license --> Start address of the license in binary format, depending on
 *                      your READ_LICENSE_BYTE definition e.g. in case of RAM, this
 *                      can just be the memory address of the license variable 
 * @param   feature_id --> feature id which will be consumed/used for login operation.
 * @param   state_buffer <--> Pointer to the buffer that contains the current state
 *                            of the license. Not used for perpetual licenses.
 * @param   rsakey --> start address of the rsa public key in binary format, depending on your
 *                     READ_AES_BYTE definition
 *
 */
fit_status_t fit_licenf_consume_license(fit_pointer_t* license,
                                        uint16_t feature_id,
                                        void* state_buffer,
                                        fit_pointer_t*rsakey )
{
    fit_status_t status             = FIT_STATUS_OK;
    uint32_t startdate              = 0;
    fitlicensemodel licensemodel    = {0};
    fitcontextdata context          = {0};
    uint8_t *lic_addr               = NULL;
    fit_pointer_t fitptr            = {0};

    DBG(FIT_TRACE_INFO, "[fit_licenf_consume_license]: feature_id=%d, pdata=0x%p \n",
        feature_id, license->data);

    fitptr.read_byte = license->read_byte;

    // Validate parameters.
    if (license->read_byte == NULL)
        return FIT_INVALID_PARAM_1;
    if (feature_id > MAX_FEATURE_ID_VALUE)
        return FIT_INVALID_PARAM_2;
    if (rsakey->read_byte == NULL)
        return FIT_INVALID_PARAM_4;

    status = fit_verify_license(license, rsakey, TRUE);
    if (status != FIT_STATUS_OK)
        return status;

    fit_memset((uint8_t *)&context, 0, sizeof(fitcontextdata));

    DBG(FIT_TRACE_INFO, "See the presence of feature id ((%d) in license binary \n",feature_id );
    // fill the requested operation type and its related data.
    context.m_operation = (uint8_t)FIT_CONSUME_LICENSE;
    context.mparserdata.m_id = feature_id;
    context.mstatus = FIT_STATUS_ERROR;

    // Parse the license data to look for Feature id that will be used for
    // login type operation.
    status = fit_parse_object(STRUCT_V2C_LEVEL, LICENSE_FIELD, license, &context);

    if (status == FIT_STATUS_OK || status == FIT_STOP_PARSE || status == FIT_CONTINUE_PARSE)
    {
        // Set the consume license status value.
        status = (fit_status_t)context.mstatus;
    }
    else
    {
        // If there is any error during lookup of feature ID then license string is not valid.
        return FIT_INVALID_V2C;
    }

    if (status != FIT_FEATURE_ID_FOUND)
    {
        DBG(FIT_TRACE_ERROR, "Requested Feature ID NOT found error = %d\n", status);
        return FIT_FEATURE_NOT_FOUND;
    }
    else
    {
        lic_addr = context.mparserdata.m_addr;
        if (lic_addr == NULL)
            return FIT_INVALID_V2C;
    }

    DBG(FIT_TRACE_INFO, "Requested Feature ID found with status = %d\n", status);

    // Get the time when license is generated i.e. start time for license
    context.m_level = STRUCT_LIC_PROP_LEVEL;
    context.m_index = START_DATE_FIELD;
    context.m_operation = (uint8_t)FIT_GET_DATA_ADDRESS;
    context.mstatus = FIT_STATUS_OK;
    // Parse license data.
    fitptr.data = lic_addr;
    
    status = fit_parse_object(STRUCT_LIC_PROP_LEVEL, FEATURE_FIELD, &fitptr, &context);
    if (status == FIT_STOP_PARSE && context.mstatus == FIT_LIC_FIELD_PRESENT)
    {
#ifdef FIT_USE_CLOCK
        if (context.mparserdata.m_addr == NULL)
            return FIT_INVALID_V2C;
        startdate = read_dword(context.mparserdata.m_addr, fitptr.read_byte);
        if (startdate > 0)
        {
            // check the presence of clock on board. If no clock is set then return error.
            uint32_t curtime = 0;

            status = fit_getunixtime(&curtime);
            if (status != FIT_STATUS_OK)
                return status;
            // TODO hard-coded value, need to sync with real clock server.
            if (curtime <= 1449571095)
            {
                DBG(FIT_TRACE_ERROR, "No real time clock is present on board");
                return FIT_RTC_NOT_PRESENT;
            }
        }
#endif //#ifdef FIT_USE_CLOCK
    }

    // Get the license model corresponding to feature ID.
    status = fit_get_lic_prop_model(&fitptr, &licensemodel);
    if (status != FIT_STATUS_OK)
        return FIT_STATUS_ERROR;

    // Behavior of consume license is different for each type of license.
    // See if license is perpertual.
    DBG(FIT_TRACE_INFO, "Check if license is perpetual one, is_perpetual=%d.\n", licensemodel.perpetual);
    if (licensemodel.perpetual == TRUE)
        {
#ifdef FIT_USE_CLOCK
            // Current time should be greater than start date (time)
            if (startdate > 0)
            {
                uint32_t curtime = 0;

                status = fit_getunixtime(&curtime);
                if (status != FIT_STATUS_OK)
                    return status;
                if (curtime < startdate)
                    return FIT_INACTIVE_LICENSE;
            }
#endif // #ifdef FIT_USE_CLOCK
            // For perpetual licenses, return status FIT_STATUS_OK if feature id is found
            // else return FIT_FEATURE_NOT_FOUND.
            DBG(FIT_TRACE_INFO, "Consume License operation completed succesfully.\n");
            return FIT_STATUS_OK;
        }
    else if (licensemodel.enddate == TRUE)
    {
#ifndef FIT_USE_CLOCK
        return FIT_NO_CLOCK_SUPPORT;
#endif // #ifdef FIT_USE_CLOCK
    // See if license is expiration based.
    context.m_level = STRUCT_LIC_PROP_LEVEL;
    context.m_index = END_DATE_FIELD;
    context.m_operation = (uint8_t)FIT_GET_DATA_ADDRESS;
    context.mstatus = FIT_STATUS_OK;
    // Parse license data.
        status = fit_parse_object(STRUCT_LIC_PROP_LEVEL, FEATURE_FIELD, &fitptr, &context);
    if (status == FIT_STOP_PARSE && context.mstatus == FIT_LIC_FIELD_PRESENT)
    {
        uint32_t curtime = 0;
        uint32_t enddate = 0;

        if (context.mparserdata.m_addr == NULL)
            return FIT_INVALID_V2C;

        enddate = read_dword(context.mparserdata.m_addr, fitptr.read_byte);
        status = fit_getunixtime(&curtime);
        if (status != FIT_STATUS_OK)
            return status;
        // Current time should be greater than start date (time)
        if (curtime < startdate)
            return FIT_INACTIVE_LICENSE;

        if (enddate < curtime)
            return FIT_FEATURE_EXPIRED;
        else
            return FIT_STATUS_OK;
        }
    }

    return FIT_INVALID_LIC_TYPE;
}
