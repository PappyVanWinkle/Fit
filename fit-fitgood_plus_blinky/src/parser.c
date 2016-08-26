/****************************************************************************\
**
** parser.c
**
** Defines functionality for parsing licenses for embedded devices.
** 
** Copyright (C) 2016, SafeNet, Inc. All rights reserved.
**
\****************************************************************************/
#include "internal.h"
#include "parser.h"
#include <string.h> /* Required for removing unnecessary warnings */
#include "hwdep.h"
#include "fit_debug.h"
#include "dm_hash.h"
#include "fit_rsa.h"
#include "abreast_dm.h"


/* Global Data **************************************************************/
// Initialize all field types for {level, index} to FIT_INVALID_VALUE
wire_type_t lic_field_type[MAX_LEVEL][MAX_INDEX] = {{0xFF}};
uint8_t lic_tag_id[MAX_LEVEL][MAX_INDEX] = {{0xFF}};
fit_cache_data fit_cache = {0};
#ifdef FIT_USE_UNIT_TESTS
extern unsigned char licensebin[];
#endif

/* Constants ****************************************************************/
struct callbacks fct[] = {{FIT_CONSUME_LICENSE, fit_consume_license},
                          {FIT_PARSE_LICENSE, fit_parse_field_data},
                          {FIT_GET_DATA_ADDRESS, fit_get_data_address}
#ifdef FIT_USE_UNIT_TESTS
              ,
                          {FIT_GET_VENDORID, fit_get_vendor_id},
                          {FIT_GET_LICENSE_UID, fit_get_license_uid}
#endif

};

#ifdef FIT_USE_UNIT_TESTS
struct testcallbacks testfct[] =
    {{STRUCT_LICENSE,
        LICENSE_CONTAINER_FIELD,
        FIT_TEST_LIC_CONTAINER_DATA,
        fit_test_lic_container_data},

    {STRUCT_LICENSE,
        HEADER_FIELD,
        FIT_TEST_LIC_HEADER_DATA,
        fit_test_header_data},

    {STRUCT_LICENSE_CONTAINER_LEVEL,
        VENDOR_FIELD,
        FIT_TEST_VENDOR_DATA,
        fit_test_vendor_data},

    {STRUCT_VENDOR_LEVEL,
        PRODUCT_FIELD,
        FIT_TEST_LIC_PRODUCT_DATA,
        fit_test_lic_product_data},

    {STRUCT_PRODUCT_LEVEL,
        PRODUCT_PART_FIELD,
        FIT_TEST_LICENSE_PROPERTY_DATA,
        fit_test_lic_property_data},

    {STRUCT_LIC_PROP_LEVEL,
        FEATURE_FIELD,
        FIT_TEST_FEATURE_DATA,
        fit_test_feature_info}};

#endif //#ifdef FIT_USE_UNIT_TESTS

/* Function Prototypes ******************************************************/
// This function will be called in case value of field is 00 00 and data is encoded
// in data part.

static void register_field_types(void);
static void register_tag_id(void);

static fit_status_t fit_parse_data(uint8_t level,
                                   uint8_t index,
                                   fit_pointer_t *pdata,
                                   void *context);
// This function will call the callback function register for each operation type.
static fit_status_t parsercallbacks(uint8_t level,
                                    uint8_t index,
                                    fit_pointer_t *pdata,
                                    uint16_t length,
                                    void *context);
#ifdef FIT_USE_UNIT_TESTS
static fit_status_t fieldcallbackfn(uint8_t level, uint8_t index, fit_pointer_t *pdata, void *context);
#endif // #ifdef FIT_USE_UNIT_TESTS

/* Functions ****************************************************************/
/**
 *
 * fit_parse_object
 *
 * fit_parse_object will parse the license data passed to it. If data contains
 * the sub array or object then it calls appropriate routines/functions and passes the
 * address of corresponding array or object data.
 *
 * @param   level --> level/depth of license schema to be parse by fit_parse_object function.
 * @param   index --> structure index. All fields will have unique index at each level.
 *                    So all fields at level 0 will have index value from 0..n, fields at
 *                    level 1 will have index value from 0..n and so on.
 * @param   pdata --> Pointer to data that to be parsed by fit_parse_object.
 * @param   context --> Pointer to fit context structure.
 *
 */
fit_status_t fit_parse_object(uint8_t level,
                              uint8_t index,
                              fit_pointer_t *pdata,
                              void *context)
{
    uint16_t cntr           = 0;
    // skip_fields represents number of fields to skip or number of fields that
    // does not have any data in license binary.
    uint8_t skip_fields     = 0;
    uint8_t cur_index       = index;
    uint8_t *parserdata     = pdata->data;
    fit_pointer_t fitptr    = {0}; 
    // Header is a 16bit integer. It represents number of fields.
    uint16_t num_fields     = read_word(pdata->data, pdata->read_byte);
    // struct_offset contains value that represents start of field data(all except integer data)
    // i.e. number of bytes after which field data will start. If field value
    // is 00 00 that means data corresponding to that filed will be encoded in data part.
    uint16_t struct_offset  = (num_fields+1)*PFIELD_SIZE;
    // Contains success or error code.
    fit_status_t status     = FIT_STATUS_OK;
    uint16_t field_data     = 0;
    static uint8_t fit_initialized = 0;

    if( fit_initialized == 0 ){
        fit_initialized ++;
        register_tag_id();
        register_field_types();
    }

    DBG(FIT_TRACE_INFO, "[parse_object start]: for Level=%d, Index=%d, pdata=0x%X \n",
        level, index, pdata->data);

    fitptr.length = 0;
    fitptr.read_byte = pdata->read_byte;

    // First field represents no. of fields for object. Move data pointer to next
    // field to get first field data.
    pdata->data = pdata->data + PFIELD_SIZE;

    // Parse all fields data in a structure.
    for( cntr = 0; cntr < num_fields; cntr++)
    {
        // If there is any error then stop further parsing and return the error.
        if (!(status == FIT_STATUS_OK || status == FIT_CONTINUE_PARSE) || status == FIT_STOP_PARSE)
            break;
        // Each field in field part is a 16bit integer  Value of this field will
        // tell what type of data it contains.
        field_data = read_word(pdata->data, pdata->read_byte);
        // If field_data is zero, that means the field data is encoded in data part.
        // This field data can be in form of string or array or an object itself.
        if( field_data == 0 )
        {
#ifdef FIT_USE_UNIT_TESTS
            // This code is used for unit tests. This will call the callback fn registered
            // for passed in level and index.
            if (((fitcontextdata *)context)->m_testop == TRUE)
            {
                fitptr.data = parserdata+struct_offset;
                status = fieldcallbackfn(level, cur_index, &fitptr, context);
                if (status != FIT_STATUS_OK || status == FIT_STOP_PARSE || status == FIT_CONTINUE_PARSE)
                    break;
            }
#endif // #ifdef FIT_USE_UNIT_TESTS

            // Get to data pointer where data corresponding to cur_index is present.
            // This data can be an array or an object or string or integer in form of string.
            // This will then call the appropriate functions based on type of data.
            fitptr.data = parserdata+struct_offset;

            status = fit_parse_data (level, cur_index, &fitptr, context);
            struct_offset   = (uint16_t)(struct_offset + (uint16_t)read_dword(parserdata+struct_offset, pdata->read_byte) + sizeof(uint32_t));
            // Move data pointer to next field.
            pdata->data     = pdata->data + PFIELD_SIZE;
            // Go to next index value.
            cur_index++;
        }

        // If value of field_data is odd, that means the tags is not continuous i.e.
        // we need to skip struct member fields by (field_data+1)/2 .
        else if( field_data & 1)
        {
           skip_fields  = (uint8_t)(field_data+1)/2;
           // Move data pointer to next field.
           pdata->data  = pdata->data + PFIELD_SIZE;
           // skip the fields as it does not contain any data in V2C.
           cur_index    = cur_index + skip_fields;
        } 

        // if field_data is even (and not zero), then the field contains integer value
        // and the value of this field is field_data/2-1 
        else if(field_data%2 == 0)
        {
#ifdef FIT_USE_UNIT_TESTS
            // This code is used for unit tests. This will call the callback fn registered
            // at particular level and index.
            if (((fitcontextdata *)context)->m_testop == TRUE)
            {
                status = fieldcallbackfn(level, cur_index, pdata, context);
            }
            else
#endif // #ifdef FIT_USE_UNIT_TESTS

            // Get the value. Also if there is any callback function registered at
            // passed in level and index or operation requested by Fit context then call the function.
            status = parsercallbacks(level, cur_index, pdata, sizeof(uint16_t), context);

            // Move data pointer to next field.
            pdata->data = pdata->data + PFIELD_SIZE;
            // Go to next index value.
            cur_index++;
        }
        else
        {
            // Wrong data.
            DBG(FIT_TRACE_CRITICAL, "[parse_object]: Invalid license data. \n");
            status = FIT_INVALID_PARAM_3;
            break;
        }
    }
    ((fitcontextdata *)context)->m_length = struct_offset;
    // Get license property address corresponding to featute id found.
    if(((fitcontextdata *)context)->m_operation == FIT_CONSUME_LICENSE &&
        ((fitcontextdata *)context)->mstatus == FIT_FEATURE_ID_FOUND &&
        level == STRUCT_LIC_PROP_LEVEL && index == FEATURE_FIELD)
    {
        ((fitcontextdata *)context)->mparserdata.m_addr = parserdata;
    }

    // Get license property address corresponding to featute id found.
    if(((fitcontextdata *)context)->m_operation == FIT_CONSUME_LICENSE &&
        ((fitcontextdata *)context)->mstatus == FIT_FEATURE_ID_FOUND &&
        level == STRUCT_LIC_PROP_LEVEL && index == FEATURE_FIELD)
    {
        ((fitcontextdata *)context)->mparserdata.m_addr = parserdata;
    }

    DBG(FIT_TRACE_INFO, "[parse_object end]: for Level=%d, Index=%d \n\n", level, index);
    pdata->data = parserdata;
    return status;
}

/**
 *
 * fit_parse_data
 *
 * This function will be called in case value of field is 00 00 and data is encoded
 * in data part. Passed data can be in form of Array, object, string or integer(in form of string)
 * Calls the appropriate routines/functions based on type of data and passes the
 * corresponding data pointer.
 *
 * @param   level --> level/depth of license schema to be parse by fit_parse_data function.
 * @param   index --> structure index. All fields will have unique index at each level.
 *                    So all fields at level 0 will have index value from 0..n, fields at
 *                    level 1 will have index value from 0..n and so on.
 * @param   pdata --> Pointer to data that to be parsed by fit_parse_data.
 * @param   context --> Pointer to fit context structure.
 *
 */
static fit_status_t fit_parse_data(uint8_t level,
                                   uint8_t index,
                                   fit_pointer_t *pdata,
                                   void *context)
{
    uint8_t startindex  = 0;
    fit_status_t status = FIT_STATUS_OK;
    fit_pointer_t fitptr = {0}; 
    // Get the field type corresponding to level and index.
    wire_type_t type    = get_field_type(level, index);

    DBG(FIT_TRACE_INFO, "[parse_data start]: for Level=%d, Index=%d, Type=%d \n",
        level, index, type);

    fitptr.length = 0;
    fitptr.read_byte = pdata->read_byte;

    switch(type)
    {
        case(FIT_ARRAY):
        {
            // Check if there is any operation or some checks that need to be performed on object.
            status = parsercallbacks(level, index, pdata, POBJECT_SIZE, context);
            if (status != FIT_STATUS_OK && status != FIT_CONTINUE_PARSE)
                break;

            // Field value in data part represents an array.
            status = fit_parse_array(level+1, startindex, pdata, context);
            if (status != FIT_STATUS_OK && status != FIT_CONTINUE_PARSE)
                break;
        }
        break;

        case (FIT_OBJECT):
        {
            // Check if there is any operation or some checks that need to be performed on object.
            status = parsercallbacks(level, index, pdata, POBJECT_SIZE, context);
            if (status != FIT_STATUS_OK && status != FIT_CONTINUE_PARSE)
                break;

            // Field value in data part represents an object.
            fitptr.data = pdata->data+POBJECT_SIZE;
            status = fit_parse_object(level+1, startindex, &fitptr, context);
            if (status != FIT_STATUS_OK)
                break;
        }
        break;

        case (FIT_STRING):
        case (FIT_INTEGER):
        {
#ifdef FIT_USE_UNIT_TESTS
            // This code is used for unit tests. This will call the callback fn registered
            // at particular level and index.
            if (((fitcontextdata *)context)->m_testop == TRUE)
            {
                fitptr.data = pdata->data+PSTRING_SIZE;
                status = fieldcallbackfn(level, index, &fitptr, context);
            }
            else
            {
#endif // #ifdef FIT_USE_UNIT_TESTS
                // Field value in data part contains string value or integer value in
                // form of string like vendor id = "37515"
                fitptr.data = pdata->data+PSTRING_SIZE;
                status = parsercallbacks(level, index, &fitptr,
                    (uint16_t)read_dword(pdata->data, pdata->read_byte), context);
#ifdef FIT_USE_UNIT_TESTS
            }
#endif // #ifdef FIT_USE_UNIT_TESTS

            if (status != FIT_STATUS_OK && status != FIT_CONTINUE_PARSE)
                break;
        }
        break;

        default:
        {
            DBG(FIT_TRACE_CRITICAL, "[parse_data]: Invalid wire type \n");
            // Invalid wire type or not supported.
            status = FIT_INVALID_WIRE_TYPE;
        }
            break;
    }

    DBG(FIT_TRACE_INFO, "[parse_data end]: for Level=%d, Index=%d\n", level, index);
    return status;
}

/**
 *
 * fit_parse_array
 *
 * Fit license can have array of data like array of features in one product
 * or array of products per vendor. fit_parse_array function will traverse each
 * object of an array and call appropriate functions to parse individual objects
 * of an array.
 *
 * @param   level --> level/depth of license schema to be parse by fit_parse_array function.
 * @param   index --> structure index. All fields will have unique index at each level.
 *                    So all fields at level 0 will have index value from 0..n, fields at
 *                    level 1 will have index value from 0..n and so on.
 * @param   pdata --> Pointer to data that to be parsed by fit_parse_array.
 * @param   context --> Pointer to fit context structure.
 *
 */
fit_status_t fit_parse_array(uint8_t level, uint8_t index, fit_pointer_t *pdata, void *context)
{
    // Get the total size of array in bytes.
    uint32_t arraysize  = read_dword(pdata->data, pdata->read_byte);
    // Get the size of first structure in that array.
    uint8_t *dataoffset = pdata->data + PARRAY_SIZE;
    uint16_t cntr       = 0;
    fit_pointer_t fitptr = {0}; 
    // contains success or error code.
    fit_status_t status = FIT_STATUS_OK;

    fitptr.length = 0;
    fitptr.read_byte = pdata->read_byte;

    for(cntr = 0; cntr < arraysize;)
    {
        // Call the fit_parse_object function that will parse the structure component
        // of the array. (dataoffset+POBJECT_SIZE) will contain the size of each
        // structure in array.
        fitptr.data = dataoffset+POBJECT_SIZE;

        status = fit_parse_object(level, index, &fitptr, context);
        if (status != FIT_STATUS_OK && status != FIT_CONTINUE_PARSE)
            break;
        cntr += (uint16_t)(POBJECT_SIZE + read_dword(dataoffset, pdata->read_byte));
        // Get to the next structure data in the array.
        dataoffset += POBJECT_SIZE + read_dword(dataoffset, pdata->read_byte);
    }

    return status;
}

/**
 *
 * add_field_type
 *
 * This function will add/register the field type(see enum wire_type) corresponding
 * to particular level and index.
 *
 * @param   level --> level/depth of license schema.
 * @param   index --> structure index. All fields will have unique index at each level.
 *                    So all fields at level 0 will have index value from 0..n, fields at
 *                    level 1 will have index value from 0..n and so on.
 * @param   type --> wire_type to be registered at particular level and index. See enum wire_type.
 *
 */
fit_status_t add_field_type(uint8_t level, uint8_t index, wire_type_t type)
{
    // Validate Parameters.
    if (level >= MAX_LEVEL)
        return FIT_INVALID_PARAM_1;
    if (index >= MAX_INDEX)
        return FIT_INVALID_PARAM_2;
    if (!(type >= (uint8_t)FIT_INTEGER && type <= (uint8_t)FIT_ARRAY))
        return FIT_INVALID_PARAM_3;

    // Add field type at particular level and index. So all fields will be hard-coded
    // at each level and index.
    lic_field_type[level][index] = type;

    return FIT_STATUS_OK;
}

/**
 *
 * get_field_type
 *
 * Return wire type corresponding to index and level passed in.
 *
 * @param   level --> level/depth of license schema to be parsed.
 * @param   index --> structure index. All fields will have unique index at each level.
 *                    So all fields at level 0 will have index value from 0..n, fields at
 *                    level 1 will have index value from 0..n and so on.
 *
 */
wire_type_t get_field_type(uint8_t level, uint8_t index)
{
    // Validate Parameters.
    if (level >= MAX_LEVEL)
        return FIT_INVALID_VALUE;
    if (index >= MAX_INDEX)
        return FIT_INVALID_VALUE;

    /* Field type is hard-coded based on level and Index of the structure in question */
    return lic_field_type[level][index];
}

/**
 *
 * add_tag_id
 *
 * This function will add/register the tag id (as per sproto schema) corresponding
 * to particular level and index.
 *
 * @param   level --> level/depth of license schema.
 * @param   index --> structure index. All fields will have unique index at each level.
 *                    So all fields at level 0 will have index value from 0..n, fields at
 *                    level 1 will have index value from 0..n and so on.
 * @param   tag_id --> tag id to be registered at particular level and index.
 *
 */
fit_status_t add_tag_id(uint8_t level, uint8_t index, uint8_t tag_id)
{
    // Validate Parameters.
    if (level >= MAX_LEVEL)
        return FIT_INVALID_PARAM_1;
    if (index >= MAX_INDEX)
        return FIT_INVALID_PARAM_2;
    if (!(tag_id > FIT_BASE_TAG_ID_VALUE && tag_id <= FIT_END_TAG_ID))
        return FIT_INVALID_PARAM_3;

    // Add tag id at particular level and index. So all tags will be hardc-oded
    // at each level and index.
    lic_tag_id[level][index] = tag_id;

    return FIT_STATUS_OK;
}

/**
 *
 * get_tag_id
 *
 * Return tag id corresponding to index and level passed in.
 *
 * @param   level --> level/depth of license schema to be parsed.
 * @param   index --> structure index. All fields will have unique index at each level.
 *                    So all fields at level 0 will have index value from 0..n, fields at
 *                    level 1 will have index value from 0..n and so on.
 *
 */
uint8_t get_tag_id(uint8_t level, uint8_t index)
{
    // Validate Parameters.
    if (level >= MAX_LEVEL)
        return FIT_INVALID_VALUE;
    if (index >= MAX_INDEX)
        return FIT_INVALID_VALUE;

    /* tag id is hard-coded based on level and Index of the structure in question */
    return lic_tag_id[level][index];
}

/**
 *
 * fit_parse_field_data
 *
 * This function will be used to parse the license data passed in at given level
 * and index.
 *
 * @param   pdata --> Pointer to data at passed in level and index.
 * @param   level --> level/depth of license schema.
 * @param   index --> structure index whose value is to be fetched.
 * @param   length --> Length of the data to be get.
 * @param   context --> Pointer to fit context structure.
 *
 */
fit_status_t fit_parse_field_data(fit_pointer_t *pdata,
                                    uint8_t level,
                                    uint8_t index,
                                    uint16_t length,
                                    void *context)
{
    fit_status_t status     = FIT_STATUS_OK;
    uint32_t integer        = 0;
    uint8_t string[64]      = {0};
    fit_pointer_t fitptr    = {0}; 
    // Get the field type corresponding to level and index.
    wire_type_t type        = get_field_type(level, index);

    DBG(FIT_TRACE_INFO, "[fit_parse_field_data]: for Level=%d, Index=%d, length=%d bytes, type=%d, pdata=0x%X \n",
        level, index, length, type, pdata->data);

    fitptr.read_byte = pdata->read_byte;
    // Check if field length is greater than maximum allowed.
    if (level == STRUCT_SIGNATURE_LEVEL && index == RSA_SIGNATURE_FIELD)
    {
        if (length != RSA_SIG_SIZE)
            return FIT_INVALID_FIELD_LEN;
    }
    else if (length > FIT_MAX_FIELD_SIZE)
        return FIT_INVALID_FIELD_LEN;

    // Field type should be either FIT_INTEGER or FIT_STRING; otherwise return FIT_CONTINUE_PARSE.
    if (!(type == (uint8_t)FIT_INTEGER || type == (uint8_t)FIT_STRING))
        return FIT_CONTINUE_PARSE;

    // Get integer value. Integer value can be 16 bit value or 32 bit value.
    if (type == (uint8_t)FIT_INTEGER)
    {
        // Get the 16 bit field value.
        if (length == sizeof(uint16_t))
        {
            integer = (read_word(pdata->data, pdata->read_byte)/2)-1;
            DBG(FIT_TRACE_INFO, "Integer Value = %ld\n", integer);
        }
        // Get the 32 bit field value.
        else if (length == sizeof(uint32_t))
        {
            // This represents integer data in form of string, so need to do calculations.
            integer = read_dword(pdata->data, pdata->read_byte);
            DBG(FIT_TRACE_INFO, "Integer Value = %ld\n", integer);
        }
    }
    else if (type == (uint8_t)FIT_STRING) // Get string value.
    {
        if (level != STRUCT_SIGNATURE_LEVEL && index != RSA_SIGNATURE_FIELD)
        {
        // Get the string value.
        if (length < sizeof(string))
        {
            fitptr.data = pdata->data;
            fitptr.length = length;
            fitptr_memcpy(string, &fitptr);
            DBG(FIT_TRACE_INFO, "String Data [length=%d] =\"%X\" \n", length, string);
        }
        else
        {
            DBG(FIT_TRACE_CRITICAL, "[fit_parse_field_data]: Requested string data is of longer size.\n");
            status = FIT_INSUFFICIENT_MEMORY;
        }
    }
    }

    // Validate license genaration value. It should be >=100
    if (level == STRUCT_HEADER_LEVEL && index == LICGEN_VERSION_FIELD)
        if (integer < 100)
            status = FIT_INVALID_LICGEN_VERSION;

    // Validate Algorithm used for signing license data.
    if (level == STRUCT_SIGNATURE_LEVEL && index == ALGORITHM_ID_FIELD)
        if (integer != AES_ALGID)
            status = FIT_INVALID_SIG_ID;

    // Validate vendor ID.
    if (level == STRUCT_LICENSE_CONTAINER_LEVEL && index == VENDOR_FIELD)
        if (integer > MAX_VENDOR_ID_VALUE)
            status = FIT_INVALID_VENDOR_ID;

    // If fingerprint is present then fit core should compiled with FIT_USE_NODE_LOCKING
    // macro. If not return error.
    if ((level == STRUCT_HEADER_LEVEL && index == FINGERPRINT_FIELD))
    {
#ifndef FIT_USE_NODE_LOCKING
            status = FIT_NODE_LOCKING_NOT_SUPP;
#endif
    }

    // Validate license container ID.
    if (level == STRUCT_LICENSE_CONTAINER_LEVEL && index == ID_LC_FIELD)
        if (integer > MAX_LC_ID_VALUE)
            status = FIT_INVALID_CONT_ID;

    // Validate product ID.
    if (level == STRUCT_PRODUCT_LEVEL && index == ID_PRODUCT_FIELD)
        if (integer > MAX_PRODUCT_ID_VALUE)
            status = FIT_INVALID_PRODUCT_ID;

    // Validate Feature ID.
    if (level == STRUCT_FEATURE_LEVEL && index == ID_FEATURE_FIELD)
        if (integer > MAX_FEATURE_ID_VALUE)
            status = FIT_INVALID_FEATURE_ID;

    // Validate start date
    if (level == STRUCT_LIC_PROP_LEVEL && index == START_DATE_FIELD)
        if (!(integer > 0 && integer <= MAX_START_DATE_VALUE))
            status = FIT_INVALID_START_DATE;

    // Validate end date.
    if (level == STRUCT_LIC_PROP_LEVEL && index == START_DATE_FIELD)
        if (!(integer > 0 && integer <= MAX_END_DATE_VALUE))
            status = FIT_INVALID_END_DATE;

    DBG(FIT_TRACE_INFO, "\n");
    return status;
}

/**
 *
 * fit_get_data_address
 *
 * This function will be used to get address at particular level and index of
 * license binary.
 *
 * @param   pdata --> Pointer to data at passed in level and index.
 * @param   level --> level/depth of license schema.
 * @param   index --> structure index whose value is to be fetched.
 * @param   length --> Length of the data to be get.
 * @param   context --> Pointer to fit context structure.
 *
 */
fit_status_t fit_get_data_address(fit_pointer_t *pdata,
                                    uint8_t level,
                                    uint8_t index,
                                    uint16_t length,
                                    void *context)
{
    fit_status_t status         = FIT_STATUS_OK;
    fitcontextdata *pcontext  = (fitcontextdata *)context;

    if (level == pcontext->m_level && index == pcontext->m_index)
    {
        DBG(FIT_TRACE_INFO, "[fit_get_data_address]: for Level=%d, Index=%d, pdata=0x%X \n",
            level, index, pdata->data);

        pcontext->mparserdata.m_addr = (uint8_t *)pdata->data;
        status = FIT_STOP_PARSE;
        pcontext->mstatus = FIT_LIC_FIELD_PRESENT;
    }

    return status;
}

#ifdef FIT_USE_UNIT_TESTS

/**
 *
 * fieldcallbackfn
 *
 * This function will call the callback function register at particular level and
 * index of the license schema.
 *
 * @param   level --> level/depth of license schema.
 * @param   index --> structure index. All fields will have unique index at each level.
 *                    So all fields at level 0 will have index value from 0..n, fields at
 *                    level 1 will have index value from 0..n and so on.
 * @param   pdata --> Pointer to license binary
 * @param   context --> Pointer to fit context structure.
 *
 */
fit_status_t fieldcallbackfn(uint8_t level, uint8_t index, fit_pointer_t *pdata, void *context)
{
    uint16_t cntr               = 0;
    fit_status_t status         = FIT_STATUS_OK;
    fitcontextdata *pcontext  = (fitcontextdata *)context;
    // Get the field type corresponding to level and index.
    if (level >= MAX_LEVEL)
        return FIT_INVALID_PARAM_1;
    if (index >= MAX_INDEX)
        return FIT_INVALID_PARAM_2;

    DBG(FIT_TRACE_INFO, "[fieldcallbackfn start]: for Level=%d, Index=%d, pdata=0x%X \n",
        level, index, pdata->data);

    // Call callback function that is registered against passed in level and index.
    for(cntr = 0; cntr < (sizeof(testfct)/sizeof(struct testcallbacks)); cntr++)
    {
        if( (testfct[cntr].level == pcontext->m_level && testfct[cntr].index == pcontext->m_index) &&
            (testfct[cntr].level == level && testfct[cntr].index == index))
        {
            status = testfct[cntr].m_callback_fn(pdata, pcontext->m_level, pcontext->m_index, context);
            break;
        }
    }

    return status;
}
#endif // #ifdef FIT_USE_UNIT_TESTS

/**
 *
 * parsercallbacks
 *
 * This function will call the callback function register for each operation type.
 *
 * @param   level --> level/depth of license schema.
 * @param   index --> structure index.
 * @param   pdata --> Pointer to license data at passed in level and index.
 * @param   length --> Length of the data to be get.
 * @param   context --> Pointer to fit context structure.
 *
 */
fit_status_t parsercallbacks(uint8_t level,
                             uint8_t index,
                             fit_pointer_t *pdata,
                             uint16_t length,
                             void *context)
{
    uint16_t cntr               = 0;
    fit_status_t status         = FIT_STATUS_OK;
    fitcontextdata *pcontext  = (fitcontextdata *)NULL;

    if (level >= MAX_LEVEL)
        return FIT_INVALID_PARAM_1;
    if (index >= MAX_INDEX)
        return FIT_INVALID_PARAM_2;
    if (pdata == NULL)
        return FIT_INVALID_PARAM_3;
    if (context == NULL)
        return FIT_INVALID_PARAM_5;

    DBG(FIT_TRACE_INFO, "[parsercallbacks start]: for Level=%d, Index=%d, pdata=0x%X \n",
        level, index, pdata->data);

    pcontext = (fitcontextdata *)context;
    if (pcontext->m_operation > FIT_LAST_OPERATION)
        return FIT_INVALID_PARAM_5;
    if (pcontext->m_operation == FIT_NONE_OPERATION)
        return FIT_STATUS_OK;

    if (pcontext->m_operation == (uint8_t)FIT_GET_LICENSE_INFO_DATA)
    {
        // Get the field type corresponding to level and index.
        uint8_t tagid = get_tag_id(level, index);
        fitv2cdata *v2c = (fitv2cdata *)pcontext->mparserdata.m_getinfodata.m_get_info_data;

        DBG(FIT_TRACE_INFO, "Calling user provided callback function\n");
        status = pcontext->mparserdata.m_getinfodata.m_callback_fn(tagid, pdata, length, v2c);
    }
    else
    {
        // Call the callback function that is registered against operation type.
        for(cntr = 0; cntr < (sizeof(fct)/sizeof(struct callbacks)); cntr++)
        {
            if( fct[cntr].m_operation == pcontext->m_operation )
            {
                status = fct[cntr].m_callback_fn(pdata, level, index, length, pcontext);
                break;
            }
        }
    }

    DBG(FIT_TRACE_INFO, "[parsercallbacks end]: for Level=%d, Index=%d \n", level, index);
    return status;
}

/**
 *
 * fit_check_license_validation
 *
 * This function will be used to validate license string. It will perform following operations
 * Check RSA signature:
 *      Decrypt RSA signature by RSA public key
 *      Calculate Hash of the license by Abreast-DM
 *      Compare calculated Hash and decrypted RSA signature (including sanity check on padding)
•* If the RSA signature has been verified, update the Hash table in RAM:
 *      Calculate Davies-Meyer-hash on the license
 *      Write that hash into the hash table.
 *
 * @param   license --> Pointer to license data that need to be validated for RSA decryption.
 * @param   rsakey --> start address of the rsa public key in binary format, depending on your
 *                     READ_AES_BYTE definition
 * @param   rsakeylen --> Length of above rsa public key.
 *
 */
fit_status_t fit_check_license_validation(fit_pointer_t* license,
                                          fit_pointer_t* rsakey)
{
    fit_status_t status           = FIT_STATUS_OK;
    fitcontextdata context        = {0};
    fit_pointer_t licaddr         = {0};
    fit_pointer_t signature       = {0};
    uint16_t num_fields           = 0;
    uint8_t abreasthash[ABREAST_DM_HASH_SIZE] = {0};
    uint8_t dmhash[FIT_DM_HASH_SIZE]              = {0};

    DBG(FIT_TRACE_INFO, "[fit_check_license_validation]: Entry.\n");

    licaddr.read_byte = license->read_byte;
    signature.read_byte = license->read_byte;

    // Check RSA signature:
    // Step 1:  Decrypt RSA signature by RSA public key
    // Step 2:  Calculate Hash of the license by Abreast-DM
    // Step 3:  Compare calculated Hash and decrypted RSA signature (including sanity check on padding)

    // Step 1: Get RSA signature from license hash. Decrypt RSA signature by RSA public key
    context.m_level = STRUCT_SIGNATURE_LEVEL;
    context.m_index = RSA_SIGNATURE_FIELD;
    context.m_operation = (uint8_t)FIT_GET_DATA_ADDRESS;
    // Parse license data.
    status = fit_parse_object(STRUCT_V2C_LEVEL, LICENSE_FIELD, license, &context);
    if (!(status == FIT_STATUS_OK || status == FIT_STOP_PARSE))
    {
        DBG(FIT_TRACE_ERROR, "Not able to get rsa data %d\n", status);
        goto bail;
    }
    if (context.mparserdata.m_addr == NULL)
    {
        status = FIT_INVALID_V2C;
        goto bail;
    }

    licaddr.data = context.mparserdata.m_addr;
    signature.data = licaddr.data;
    signature.length = RSA_SIG_SIZE;

    // Step 2:  Calculate Hash of the license by Abreast-DM
    // Get address and length of license part in binary.
    // TODO we can get address via parsing of hard coded knowledge of schema
    num_fields  = read_word(license->data, license->read_byte);
    licaddr.length  = (uint16_t)(read_dword(license->data + ((num_fields*PFIELD_SIZE)+PFIELD_SIZE), license->read_byte));
    licaddr.data = (uint8_t *)license->data + ((num_fields*PFIELD_SIZE)+PFIELD_SIZE+PARRAY_SIZE);

    // Get Abreast DM hash of the license
    status = fit_get_AbreastDM_Hash(&licaddr, abreasthash);

    if (status != FIT_STATUS_OK)
    {
        DBG(FIT_TRACE_CRITICAL, "Error in getting AbreastDM hash, status = %d\n", status);
        goto bail;
    }
    else
    {
        DBG(FIT_TRACE_INFO, "Got AbreastDM hash successfully. \n");
    }

    status = fit_validate_rsa_signature(&signature, abreasthash, rsakey);
    if (status != FIT_STATUS_OK)
        goto bail;

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

    licaddr.length = context.m_length;
    licaddr.data = (uint8_t *) license->data;

    // Get the hash of data.
    status = fit_davies_meyer_hash(&licaddr, (uint8_t *)&dmhash);
    if (status != FIT_STATUS_OK)
    {
        DBG(FIT_TRACE_ERROR, "Error in getting Davies Meyer hash with status %d\n", status);
        goto bail;
    }
    fit_cache.m_rsa_check_done = TRUE;
    fit_memcpy(fit_cache.m_dm_hash, dmhash, FIT_DM_HASH_SIZE);

bail:
    DBG(FIT_TRACE_INFO, "[fit_check_license_validation]: Exit.\n");

    return status;
}

/**
 *
 * register_tag_id
 *
 * This function will register all tag's corresponding to.particular level and
 * index i.e. hard-coded tag ID for fit licenses (as per sproto schema)
 *
 */
static void register_tag_id(void)
{
    DBG(FIT_TRACE_INFO, "[register_tag_id]: Entry.\n");

    // V2C - level 0
    add_tag_id(STRUCT_V2C_LEVEL, LICENSE_FIELD, FIT_LICENSE_TAG_ID);
    add_tag_id(STRUCT_V2C_LEVEL, SIGNATURE_FIELD, FIT_SIGNATURE_TAG_ID);

    // License - Level 1
    add_tag_id(STRUCT_LICENSE, HEADER_FIELD, FIT_HEADER_TAG_ID);
    add_tag_id(STRUCT_LICENSE, LICENSE_CONTAINER_FIELD, FIT_LIC_CONTAINER_TAG_ID);
    // Signature - level 1
    add_tag_id(STRUCT_SIGNATURE_LEVEL, ALGORITHM_ID_FIELD, FIT_ALGORITHM_TAG_ID);
    add_tag_id(STRUCT_SIGNATURE_LEVEL, RSA_SIGNATURE_FIELD, FIT_RSA_SIG_TAG_ID);

    // Header - level 2
    add_tag_id(STRUCT_HEADER_LEVEL, LICGEN_VERSION_FIELD, FIT_LICGEN_VERSION_TAG_ID);
    add_tag_id(STRUCT_HEADER_LEVEL, LM_VERSION_FIELD, FIT_LM_VERSION_TAG_ID);
    add_tag_id(STRUCT_HEADER_LEVEL, UID_FIELD, FIT_UID_TAG_ID);
    add_tag_id(STRUCT_HEADER_LEVEL, FINGERPRINT_FIELD, FIT_FP_TAG_ID);
    // LicenseContainer - level 2
    add_tag_id(STRUCT_LICENSE_CONTAINER_LEVEL, ID_LC_FIELD, FIT_ID_LC_TAG_ID);
    add_tag_id(STRUCT_LICENSE_CONTAINER_LEVEL, VENDOR_FIELD, FIT_VENDOR_ARRAY_TAG_ID);

    // Vendor - level 3
    add_tag_id(STRUCT_VENDOR_LEVEL, ID_VENDOR_FIELD, FIT_VENDOR_ID_TAG_ID);
    add_tag_id(STRUCT_VENDOR_LEVEL, PRODUCT_FIELD, FIT_PRODUCT_TAG_ID);

    // Product - level 4
    add_tag_id(STRUCT_PRODUCT_LEVEL, ID_PRODUCT_FIELD, FIT_PRODUCT_ID_TAG_ID);
    add_tag_id(STRUCT_PRODUCT_LEVEL, VERSION_REGEX_FIELD, FIT_VERSION_REGEX_TAG_ID);
    add_tag_id(STRUCT_PRODUCT_LEVEL, PRODUCT_PART_FIELD, FIT_PRODUCT_PART_ARRAY_TAG_ID);

    // Product Part- level 5
    add_tag_id(STRUCT_PRODUCT_PART_FIELD, PRODUCT_PART_FIELD_ID, FIT_PRODUCT_PART_ID_TAG_ID);
    add_tag_id(STRUCT_PRODUCT_PART_FIELD, LIC_PROP_FIELD, FIT_LIC_PROP_TAG_ID);

    // LicenseProperties - level 6
    add_tag_id(STRUCT_LIC_PROP_LEVEL, FEATURE_FIELD, FIT_FEATURE_ARRAY_TAG_ID);
    add_tag_id(STRUCT_LIC_PROP_LEVEL, PERPETUAL_FIELD, FIT_PERPETUAL_TAG_ID);
    add_tag_id(STRUCT_LIC_PROP_LEVEL, START_DATE_FIELD, FIT_START_DATE_TAG_ID);
    add_tag_id(STRUCT_LIC_PROP_LEVEL, END_DATE_FIELD, FIT_END_DATE_TAG_ID);
    add_tag_id(STRUCT_LIC_PROP_LEVEL, COUNTER_FIELD, FIT_COUNTER_ARRAY_TAG_ID);
    add_tag_id(STRUCT_LIC_PROP_LEVEL, DURATION_FROM_FIRST_USE_FIELD, FIT_DURATION_FROM_FIRST_USE_TAG_ID);

    // Feature - level 7
    add_tag_id(STRUCT_FEATURE_LEVEL, ID_FEATURE_FIELD, FIT_FEATURE_TAG_ID);

    // Counter - level 7
    add_tag_id(STRUCT_COUNTER_LEVEL, ID_COUNTER_FIELD, FIT_COUNTER_TAG_ID);
    add_tag_id(STRUCT_COUNTER_LEVEL, LIMIT_FIELD, FIT_LIMIT_TAG_ID);
    add_tag_id(STRUCT_COUNTER_LEVEL, SOFT_LIMIT_FIELD, FIT_SOFT_LIMIT_TAG_ID);
    add_tag_id(STRUCT_COUNTER_LEVEL, IS_FIELD, FIT_IS_FIELD_TAG_ID);

    DBG(FIT_TRACE_INFO, "[register_tag_id]: Exit.\n");
}

/**
 *
 * register_field_types
 *
 * This function will register all wire types corresponding to.particular level and
 * index i.e. hard-coded the wire types corresponding to level and index.
 *
 */
static void register_field_types(void)
{
    DBG(FIT_TRACE_INFO, "[register_field_types]: Entry.\n");

    // V2C - level 0
    add_field_type(STRUCT_V2C_LEVEL, LICENSE_FIELD, (uint8_t)FIT_OBJECT);
    add_field_type(STRUCT_V2C_LEVEL, SIGNATURE_FIELD, (uint8_t)FIT_ARRAY);

    // License - Level 1
    add_field_type(STRUCT_LICENSE, HEADER_FIELD, (uint8_t)FIT_OBJECT);
    add_field_type(STRUCT_LICENSE, LICENSE_CONTAINER_FIELD, (uint8_t)FIT_ARRAY);
    // Signature - level 1
    add_field_type(STRUCT_SIGNATURE_LEVEL, ALGORITHM_ID_FIELD, (uint8_t)FIT_INTEGER);
    add_field_type(STRUCT_SIGNATURE_LEVEL, RSA_SIGNATURE_FIELD, (uint8_t)FIT_STRING);

    // Header - level 2
    add_field_type(STRUCT_HEADER_LEVEL, LICGEN_VERSION_FIELD, (uint8_t)FIT_INTEGER);
    add_field_type(STRUCT_HEADER_LEVEL, LM_VERSION_FIELD, (uint8_t)FIT_INTEGER);
    add_field_type(STRUCT_HEADER_LEVEL, UID_FIELD, (uint8_t)FIT_STRING);
    add_field_type(STRUCT_HEADER_LEVEL, FINGERPRINT_FIELD, (uint8_t)FIT_STRING);
    // LicenseContainer - level 2
    add_field_type(STRUCT_LICENSE_CONTAINER_LEVEL, ID_LC_FIELD, (uint8_t)FIT_INTEGER);
    add_field_type(STRUCT_LICENSE_CONTAINER_LEVEL, VENDOR_FIELD, (uint8_t)FIT_ARRAY);

    // Vendor - level 3
    add_field_type(STRUCT_VENDOR_LEVEL, ID_VENDOR_FIELD, (uint8_t)FIT_INTEGER);
    add_field_type(STRUCT_VENDOR_LEVEL, PRODUCT_FIELD, (uint8_t)FIT_OBJECT);

    // Product - level 4
    add_field_type(STRUCT_PRODUCT_LEVEL, ID_PRODUCT_FIELD, (uint8_t)FIT_INTEGER);
    add_field_type(STRUCT_PRODUCT_LEVEL, VERSION_REGEX_FIELD, (uint8_t)FIT_STRING);
    add_field_type(STRUCT_PRODUCT_LEVEL, PRODUCT_PART_FIELD, (uint8_t)FIT_ARRAY);

    // Product Part- level 5
    add_field_type(STRUCT_PRODUCT_PART_FIELD, PRODUCT_PART_FIELD_ID, (uint8_t)FIT_INTEGER);
    add_field_type(STRUCT_PRODUCT_PART_FIELD, LIC_PROP_FIELD, (uint8_t)FIT_OBJECT);

    // LicenseProperties - level 6
    add_field_type(STRUCT_LIC_PROP_LEVEL, FEATURE_FIELD, (uint8_t)FIT_ARRAY);
    add_field_type(STRUCT_LIC_PROP_LEVEL, PERPETUAL_FIELD, (uint8_t)FIT_INTEGER);
    add_field_type(STRUCT_LIC_PROP_LEVEL, START_DATE_FIELD, (uint8_t)FIT_INTEGER);
    add_field_type(STRUCT_LIC_PROP_LEVEL, END_DATE_FIELD, (uint8_t)FIT_INTEGER);
    add_field_type(STRUCT_LIC_PROP_LEVEL, COUNTER_FIELD, (uint8_t)FIT_ARRAY);
    add_field_type(STRUCT_LIC_PROP_LEVEL, DURATION_FROM_FIRST_USE_FIELD, (uint8_t)FIT_INTEGER);

    // Feature - level 7
    add_field_type(STRUCT_FEATURE_LEVEL, ID_FEATURE_FIELD, (uint8_t)FIT_INTEGER);

    // Counter - level 7
    add_field_type(STRUCT_COUNTER_LEVEL, ID_COUNTER_FIELD, (uint8_t)FIT_INTEGER);
    add_field_type(STRUCT_COUNTER_LEVEL, LIMIT_FIELD, (uint8_t)FIT_INTEGER);
    add_field_type(STRUCT_COUNTER_LEVEL, SOFT_LIMIT_FIELD, (uint8_t)FIT_INTEGER);
    add_field_type(STRUCT_COUNTER_LEVEL, IS_FIELD, (uint8_t)FIT_INTEGER);

    DBG(FIT_TRACE_INFO, "[register_field_types]: Exit.\n");
}






