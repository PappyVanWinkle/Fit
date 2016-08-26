/****************************************************************************\
**
** demo_getinfo.c
**
** Defines functionality for get info API on sentinel fit based licenses for 
** embedded devices.
** 
** Copyright (C) 2016, SafeNet, Inc. All rights reserved.
**
\****************************************************************************/

#include <stdio.h>
#include "parser.h"
#include "internal.h"

#define TEMP_BUF_LEN 41
#define DYNAMIC_MEMORY_SIZE 0x400
uint8_t dynamic_memory[DYNAMIC_MEMORY_SIZE];
uint8_t *head_dynamic_memory = dynamic_memory;
uint8_t *tail_dynamic_memory = dynamic_memory + DYNAMIC_MEMORY_SIZE;
uint16_t current_memory_stock = DYNAMIC_MEMORY_SIZE;
uint16_t current_memory_offset = 0;

uint8_t *packet_malloc(uint16_t length)
{
    uint8_t *temp = NULL;

    if((length + 1) < current_memory_stock)
    {
        dynamic_memory[current_memory_offset] = length;
        temp = &dynamic_memory[current_memory_offset + 1];
        current_memory_offset += (length + 1);
        current_memory_stock -= (length + 1);
    }

    return temp;
}


uint16_t packet_free(uint8_t *memory)
{
    uint16_t length = 0;

    if(memory < tail_dynamic_memory && memory > head_dynamic_memory)
    {
        length = (uint16_t) *(memory - 1);
        current_memory_offset -= (uint16_t) (length + 1);
        current_memory_stock += (uint16_t) (length + 1);
    }

    return length;
}

/**
 *
 * fit_get_vendor_id
 *
 * This function will get the vendor information from the license data passed in.
 *
 * @param   pdata --> Pointer to data that contains vendor license information.
 * @param   level --> level/depth of license schema.
 * @param   index --> structure index whose value is to be fetched.
 * @param   length --> Length of the information requested in bytes.
 * @param   context <--> Core sentinel fit context data.
 *
 */
fit_status_t fit_get_vendor_id( fit_pointer_t *pdata,
                                  uint8_t level,
                                  uint8_t index,
                                  uint16_t length,
                                  void *context)
{
    // Get the field type corresponding to level and index.
    wire_type_t type            = get_field_type(level, index);
    fitcontextdata *pcontext  = (fitcontextdata *)context;

    DBG(FIT_TRACE_INFO, "fit_get_vendor_id() Level=%d, Index=%d, pdata=0x%p \n",
        level, index, pdata->data);
    if (pdata == NULL)
        return FIT_INVALID_PARAM_1;
    if (type != (uint8_t)FIT_INTEGER)
        return FIT_CONTINUE_PARSE;
    if (context == NULL)
        return FIT_INVALID_PARAM_5;
    if (pcontext->m_operation != (uint8_t)FIT_GET_VENDORID)
        return FIT_INVALID_PARAM_5;

    // Check if we are at correct level and index.
    if (level == STRUCT_VENDOR_LEVEL && index == ID_VENDOR_FIELD)
    {
        // Get the vendor information.
        if (type == (uint8_t)FIT_INTEGER && length == sizeof(uint32_t))
        {
            pcontext->mparserdata.m_vendorid = read_dword(pdata->data, pdata->read_byte);

            DBG(FIT_TRACE_INFO, "Got requested vendor information.\n");
            ((fitcontextdata *)pcontext)->mstatus = FIT_STATUS_OK;
            return FIT_STOP_PARSE;
        }
    }

    return FIT_STATUS_OK;
}


/**
 *
 * fit_get_license_uid
 *
 * This function will get the license uid information.
 *
 * @param   pdata --> Pointer to license data.
 * @param   level --> level/depth of license schema.
 * @param   index --> structure index of license schema.
 * @param   length --> Length of the information requested in bytes.
 * @param   context <--> Core sentinel fit context data.
 *
 */
fit_status_t fit_get_license_uid(fit_pointer_t *pdata,
                                   uint8_t level,
                                   uint8_t index,
                                   uint16_t length,
                                   void *context)
{
    // Get the field type corresponding to level and index.
    wire_type_t type        = get_field_type(level, index);
    fitcontextdata *pcontext = (fitcontextdata *)context;
    fit_pointer_t fitptr = {0};

    DBG(FIT_TRACE_INFO, "fit_get_vendor_id() Level=%d, Index=%d, pdata=0x%p \n",
        level, index, pdata->data);
    if (type != (uint8_t)FIT_STRING)
        return FIT_CONTINUE_PARSE;
    if (pdata == NULL)
        return FIT_INVALID_PARAM_1;
    if (context == NULL)
        return FIT_INVALID_PARAM_5;
    if (pcontext->m_operation != (uint8_t)FIT_GET_LICENSE_UID)
        return FIT_INVALID_PARAM_5;

    // Check if we are at correct level and index.
    if (level == STRUCT_HEADER_LEVEL && index == UID_FIELD)
    {
        fitptr.data = pdata->data;
        fitptr.length = FIT_UID_LEN;
        fitptr.read_byte = pdata->read_byte;

        fitptr_memcpy(pcontext->mparserdata.m_uid, &fitptr);
        DBG(FIT_TRACE_INFO, "Got requested uid information.\n");
        ((fitcontextdata *)pcontext)->mstatus = FIT_STATUS_OK;
        return FIT_STOP_PARSE;
    }

    return FIT_STATUS_OK;
}

/**
 *
 * fit_getlicensedata_cb
 *
 * This function will get complete license information for embedded devices like
 * license header information, license signature data, vendor information, and like
 * license property information i.e. license is perpetual or not, start date, end date,
 * counter information etc.
 *
 * @param   tagid --> define unique field in sentinel fit license.
 * @param   pdata --> Pointer to data that gives tagid information.
 * @param   length --> Length of the data requested in bytes.
 * @param   context <--> Pointer to structure for requested information.
 *
 */
fit_status_t fit_getlicensedata_cb (uint8_t tagid,
                                    fit_pointer_t *pdata,
                                      uint16_t length,
                                      void *context)
{
    fit_status_t status             = FIT_STATUS_OK;
    fitv2cdata *v2c               = (fitv2cdata *)context;
    // cur_prod will be pointer to current product ID.
    static fitproductdata *cur_prod = NULL;
    // There can be multiple features per product in V2C. cur_feat will be pointer to
    // current feature ID.
    static fitfeaturedata *cur_feat = NULL;
    // There can be multiple license model define in V2C. cur_prod_part will be pointer to
    // current license model
    static fitprodpartdata *cur_prod_part = NULL;
    fit_pointer_t fitptr = {0};

    fitptr.read_byte = pdata->read_byte;

    DBG(FIT_TRACE_INFO, "User provided callback function\n");
    // Validate parameters.
    if (pdata == NULL)
        return FIT_INVALID_PARAM_2;
    if (v2c == NULL)
        return FIT_INVALID_PARAM_4;

    // Get the information based on tag id.
    switch(tagid) {

    // Start of license data. Here we can re-initialized static variables.
    case FIT_LICENSE_TAG_ID:
        DBG(FIT_TRACE_INFO, "FIT_LICENSE_TAG_ID\n");
        cur_prod = NULL;
        cur_feat = NULL;
        cur_prod_part = NULL;
        fit_memset(dynamic_memory, 0, DYNAMIC_MEMORY_SIZE);
        break;

    // tag for signature data.
    case FIT_SIGNATURE_TAG_ID:
        DBG(FIT_TRACE_INFO, "FIT_SIGNATURE_TAG_ID\n");
        // Allocate memory for license signature information.
        v2c->signature = (fitsignaturedata *)packet_malloc(sizeof(fitsignaturedata));
        if (v2c->signature == NULL)
            // Return FIT_INSUFFICIENT_MEMORY if memory was not sufficient
            status = FIT_INSUFFICIENT_MEMORY;
        else
            status = FIT_CONTINUE_PARSE;
        break;

    // tag for header data.
    case FIT_HEADER_TAG_ID:
        break;

    // tag for license container data.
    case FIT_LIC_CONTAINER_TAG_ID:
        DBG(FIT_TRACE_INFO, "FIT_LIC_CONTAINER_TAG_ID\n");
        // Allocate memory for license container information.
        v2c->lic.cont = (fitcontainerdata *)packet_malloc(sizeof(fitcontainerdata));
        if (v2c->lic.cont == NULL)
            // Return FIT_INSUFFICIENT_MEMORY if memory was not sufficient
            status = FIT_INSUFFICIENT_MEMORY;
        else
        {
            v2c->lic.cont->id = 0;
            v2c->lic.cont->vendor = NULL;
            status = FIT_CONTINUE_PARSE;
        }

        break;

    // tag for algorithm used.
    case FIT_ALGORITHM_TAG_ID:
        DBG(FIT_TRACE_INFO, "FIT_ALGORITHM_TAG_ID\n");
        if (v2c->signature != NULL)
        {
            // Get the algorithm used for signing sentinel fit based licenses.
            v2c->signature->algid = read_word(pdata->data, pdata->read_byte)/2 - 1;
            status = FIT_CONTINUE_PARSE;
        }
        else
            status = FIT_STATUS_ERROR;

        break;

    // tag for licgen version.
    case FIT_LICGEN_VERSION_TAG_ID:
        DBG(FIT_TRACE_INFO, "FIT_LICGEN_VERSION_TAG_ID\n");
        // Get the licgen version used for creating licenses.
        v2c->lic.header.licgen_version = read_word(pdata->data, pdata->read_byte)/2 - 1;
        DBG(FIT_TRACE_INFO, "v2c->lic.header.licgen_version=%d\n", v2c->lic.header.licgen_version);
        status = FIT_CONTINUE_PARSE;
        break;

    // tag for LM version.
    case FIT_LM_VERSION_TAG_ID:
        DBG(FIT_TRACE_INFO, "FIT_LM_VERSION_TAG_ID\n");
        // Get the LM version value.
        v2c->lic.header.lm_version = read_word(pdata->data, pdata->read_byte)/2 - 1;
        DBG(FIT_TRACE_INFO, "v2c->lic.header.lm_version=%d\n", v2c->lic.header.lm_version);
        status = FIT_CONTINUE_PARSE;
        break;

    // tag for license unique UID value.
    case FIT_UID_TAG_ID:
        DBG(FIT_TRACE_INFO, "FIT_UID_TAG_ID\n");
        // Get the license unique UID value.
        fitptr.data = pdata->data;
        fitptr.length = FIT_UID_LEN;
        
        fitptr_memcpy(v2c->lic.header.uid, &fitptr);
        status = FIT_CONTINUE_PARSE;
        break;

#ifdef FIT_USE_NODE_LOCKING
    // tag for fingerprint data.
    case FIT_FP_TAG_ID:
        DBG(FIT_TRACE_INFO, "FINGERPRINT_FIELD\n");
        // Get the fingerprint information.
        getfingerprintdata(pdata, &(v2c->lic.header.licensefp));
        status = FIT_CONTINUE_PARSE;
        break;
#endif // ifdef FIT_USE_NODE_LOCKING

    // tag for license container ID.
    case FIT_ID_LC_TAG_ID:
        DBG(FIT_TRACE_INFO, "FIT_ID_LC_TAG_ID\n");
        if (v2c->lic.cont == NULL)
            status = FIT_STATUS_ERROR;
        else
        {
            // Get the license container ID value.
            v2c->lic.cont->id = read_dword(pdata->data, pdata->read_byte)/2 - 1;
            status = FIT_CONTINUE_PARSE;
        }
        break;

    // Tag for vendor information.
    case FIT_VENDOR_ARRAY_TAG_ID:
        DBG(FIT_TRACE_INFO, "FIT_VENDOR_ARRAY_TAG_ID\n");
        if (v2c->lic.cont == NULL)
            status = FIT_STATUS_ERROR;
        else
        {
            v2c->lic.cont->vendor = (fitvendordata *)packet_malloc(sizeof(fitvendordata));
            if (v2c->lic.cont->vendor == NULL)
                // Return FIT_INSUFFICIENT_MEMORY if memory was not sufficient
                status = FIT_INSUFFICIENT_MEMORY;
            else
                status = FIT_CONTINUE_PARSE;
        }
        break;

    // Tag for vendor ID.
    case FIT_VENDOR_ID_TAG_ID:
        DBG(FIT_TRACE_INFO, "FIT_VENDOR_ID_TAG_ID\n");
        if (v2c->lic.cont == NULL || v2c->lic.cont->vendor == NULL)
            status = FIT_STATUS_ERROR;
        else
        {
            // Get the vendor ID value.
            if (length == PFIELD_SIZE)
                v2c->lic.cont->vendor->vendorid = (uint16_t)read_word(pdata->data, pdata->read_byte)/2 -1;
            else if (length == PARRAY_SIZE)
                v2c->lic.cont->vendor->vendorid = read_dword(pdata->data, pdata->read_byte);
            status = FIT_CONTINUE_PARSE;
        }
        break;

    case FIT_PRODUCT_TAG_ID:
        break;

    // Tag for product id.
    case FIT_PRODUCT_ID_TAG_ID:
        DBG(FIT_TRACE_INFO, "FIT_PRODUCT_TAG_ID\n");
        if (v2c->lic.cont == NULL || v2c->lic.cont->vendor == NULL)
            status = FIT_STATUS_ERROR;
        else
        {
            cur_prod = (fitproductdata *)&(v2c->lic.cont->vendor->prod);
            // get the product id.
            if (length == PFIELD_SIZE)
                cur_prod->prodid = read_word(pdata->data, pdata->read_byte)/2 -1;
            else if (length == PARRAY_SIZE)
                cur_prod->prodid = read_dword(pdata->data, pdata->read_byte);
            status = FIT_CONTINUE_PARSE;
        }
        break;

    // tag for version regex.
    case FIT_VERSION_REGEX_TAG_ID:
        DBG(FIT_TRACE_INFO, "FIT_VERSION_REGEX_TAG_ID\n");
        if (v2c->lic.cont == NULL || v2c->lic.cont->vendor == NULL || cur_prod == NULL)
            status = FIT_STATUS_ERROR;
        else
        {
            fitptr.data = pdata->data;
            fitptr.length = FIT_UID_LEN;	
            // to change FIT_UID_LEN
            fitptr_memcpy((uint8_t *)cur_prod->verregex, &fitptr);
            status = FIT_CONTINUE_PARSE;
        }
        break;

    // Tag for product part information.
    case FIT_PRODUCT_PART_ARRAY_TAG_ID:
        DBG(FIT_TRACE_INFO, "FIT_PRODUCT_PART_ARRAY_TAG_ID\n");
        break;

    case FIT_PRODUCT_PART_ID_TAG_ID:
        DBG(FIT_TRACE_INFO, "FIT_PRODUCT_PART_ID_TAG_ID\n");
        if (cur_prod == NULL)
            status = FIT_STATUS_ERROR;
        else
        {
            // Get pointer to first product part id. If NULL, means this is first
            // product part in the license.
            // If not NULL, create a linked list of product part id's (first product 
            // part at head of linked list)
            fitprodpartdata *prodpart = cur_prod->prodpart;
            fitprodpartdata *lastprodpart = NULL;
            if (prodpart == NULL)
            {
                prodpart = (fitprodpartdata *)packet_malloc(sizeof(fitprodpartdata));
                if (prodpart == NULL)
                    // Return FIT_INSUFFICIENT_MEMORY if memory was not sufficient
                    return FIT_INSUFFICIENT_MEMORY;
                else
                    cur_prod->prodpart = prodpart;
            }
            else
            {
                // Create a linked list of product id's.
                while(prodpart != NULL)
                {
                    lastprodpart = prodpart;
                    prodpart = prodpart->next;
                }

                prodpart = (fitprodpartdata *)packet_malloc(sizeof(fitprodpartdata));
                if (prodpart == NULL)
                    // Return FIT_INSUFFICIENT_MEMORY if memory was not sufficient
                    return FIT_INSUFFICIENT_MEMORY;
                else
                    lastprodpart->next = prodpart;
            }

            if (status == FIT_STATUS_OK)
            {
                prodpart->next = NULL;
                // cur_prod will contain pointer to current product id in license data.
                cur_prod_part = prodpart;

                // get the product id.
                if (length == PFIELD_SIZE)
                    cur_prod_part->partid = read_word(pdata->data, pdata->read_byte)/2 -1;
                else if (length == PARRAY_SIZE)
                    cur_prod_part->partid = read_dword(pdata->data, pdata->read_byte);

                status = FIT_CONTINUE_PARSE;
            }

        }
        break;

    // tag for license properties
    case FIT_LIC_PROP_TAG_ID:
        DBG(FIT_TRACE_INFO, "FIT_LIC_PROP_TAG_ID\n");
        if (cur_prod_part == NULL)
            status = FIT_STATUS_ERROR;
        else
        {
            // Initialize the license properties members (to avoid garbage values)
            cur_prod_part->properties.feat = NULL;
            cur_prod_part->properties.enddate = 0;
            cur_prod_part->properties.startdate = 0;
            cur_prod_part->properties.perpetual = 0;

            status = FIT_CONTINUE_PARSE;
        }
        break;

    // tag for feature array
    case FIT_FEATURE_ARRAY_TAG_ID:
        break;

    // tag for feature id.
    case FIT_FEATURE_TAG_ID:
    {
        if (cur_prod_part == NULL)
            status = FIT_STATUS_ERROR;
        else
        {
            // Get pointer to first feature id in product. If NULL, means this is first feature in the product.
            // If not NULL, create a linked list of features id's (first feature at head of linked list)
            fitfeaturedata *features = cur_prod_part->properties.feat;
            fitfeaturedata *lastfeat = NULL;
            if (features == NULL)
            {
                features = (fitfeaturedata *)packet_malloc(sizeof(fitfeaturedata));
                if (features == NULL)
                    // Return FIT_INSUFFICIENT_MEMORY if memory was not sufficient
                    return FIT_INSUFFICIENT_MEMORY;
                else
                    cur_prod_part->properties.feat = features;
            }
            else
            {
                // create a linked list of feature's ID.
                while(features != NULL)
                {
                    lastfeat = features;
                    features = features->next;
                }

                features = (fitfeaturedata *)packet_malloc(sizeof(fitfeaturedata));
                if (features == NULL)
                    // Return FIT_INSUFFICIENT_MEMORY if memory was not sufficient
                    return FIT_INSUFFICIENT_MEMORY;
                else
                    lastfeat->next = features;
            }

            // cur_feat will contain pointer to current feature id in license data.
            if (status == FIT_STATUS_OK)
            {
                features->next = NULL;
                cur_feat = features;
                // get the feature id.
                if (length == PFIELD_SIZE)
                    cur_feat->featid = read_word(pdata->data, pdata->read_byte)/2 - 1;
                else if (length == PARRAY_SIZE)
                    cur_feat->featid = read_dword(pdata->data, pdata->read_byte);
                status = FIT_CONTINUE_PARSE;
            }
        }
    }
        break;

    // tag for perpatual license
    case FIT_PERPETUAL_TAG_ID:
        if (cur_prod_part == NULL)
            status = FIT_STATUS_ERROR;
        else
        {
            // check whether license is perpetual or not.
            cur_prod_part->properties.perpetual = pdata->read_byte(pdata->data)/2 - 1;
            cur_prod_part->lictype = FIT_LIC_PERPETUAL;
            status = FIT_CONTINUE_PARSE;
        }
        break;

    // tag for start date.
    case FIT_START_DATE_TAG_ID:
        if (cur_prod_part == NULL)
            status = FIT_STATUS_ERROR;
        else
        {
            // Get the start date value.
            cur_prod_part->properties.startdate = read_dword(pdata->data, pdata->read_byte);
            status = FIT_CONTINUE_PARSE;
        }
        break;

    // tag for end date.
    case FIT_END_DATE_TAG_ID:
        if (cur_prod_part == NULL)
            status = FIT_STATUS_ERROR;
        else
        {
            // Get the end date value.
            cur_prod_part->properties.enddate = read_dword(pdata->data, pdata->read_byte);
            cur_prod_part->lictype = FIT_LIC_EXPIRATION_BASED;
            status = FIT_CONTINUE_PARSE;
        }
        break;

    case FIT_DURATION_FROM_FIRST_USE_TAG_ID:
        if (cur_prod_part == NULL)
            status = FIT_STATUS_ERROR;
        else
        {
            // Get the end date value.
            cur_prod_part->lictype = FIT_LIC_TIME_BASED;
            status = FIT_CONTINUE_PARSE;
        }
        break;

    default:
        break;

    }

    return status;
}

/**
 *
 * freelicensedata
 *
 * This function will free dynamic memory allocated in get info callback function.
 *
 * @param   context <--> Pointer to structure that contains license data.
 *
 */
void freelicensedata (void *context)
{
    fitv2cdata *v2c           = (fitv2cdata *)context;

    DBG(FIT_TRACE_INFO, "*** Free License Data ***************************************************\n");
    if (v2c != NULL)
    {
        if (v2c->signature != NULL)
        {
            // free memory associated with signature data
            packet_free((uint8_t *)v2c->signature);
            v2c->signature = NULL;
        }
        if (v2c->lic.cont != NULL)
        {
            if (v2c->lic.cont->vendor != NULL)
            {
                if (v2c->lic.cont->vendor->prod.prodpart != NULL)
                {
                    // Get pointer to first product in the license
                    fitprodpartdata *prodpart = v2c->lic.cont->vendor->prod.prodpart;

                    while(prodpart != NULL)
                    {
                        fitprodpartdata *delprod = prodpart;
                        // Get pointer to first feature in the product
                        fitfeaturedata *featarray = prodpart->properties.feat;
                        while(featarray != NULL)
                        {
                            fitfeaturedata *delfeat = featarray;

                            featarray = featarray->next;
                            // Free feature information for one product ID.
                            packet_free((uint8_t *)delfeat);
                        }
                        prodpart = prodpart->next;
                        // Free memory associated with product data.
                        packet_free((uint8_t *)delprod);
                    }
                }
                // Free memory associated with vendor data.
                packet_free((uint8_t *)(v2c->lic.cont->vendor));
            }
            // Free memory associated with license container data.
            packet_free((uint8_t *)(v2c->lic.cont));
        }
    }
}

/**
 *
 * printlicensedata
 *
 * This function will print license information.
 *
 * @param   context <--> Pointer to structure that contains license data.
 *
 */
void printlicensedata (void *context)
{
    fitv2cdata *v2c       = (fitv2cdata *)context;
    uint8_t cntr            = 0;

    DBG(FIT_TRACE_INFO, "*** License Data ********************************************************\n");
    if (v2c != NULL)
    {
        if (v2c->signature != NULL)
        {
            DBG(FIT_TRACE_INFO, "Algorithm used for signing license data = %d\n",
                v2c->signature->algid);
        }
        DBG(FIT_TRACE_INFO, "\nLicgen version = %d\n", v2c->lic.header.licgen_version);
        DBG(FIT_TRACE_INFO, "LM version = %d\n", v2c->lic.header.lm_version);
        if (v2c->lic.header.uid[0] != 0)
        {
            DBG(FIT_TRACE_INFO, "License UID : ");
            for (cntr=0; cntr<FIT_UID_LEN; cntr++) DBG(FIT_TRACE_INFO, "%X ", v2c->lic.header.uid[cntr]);
        }
#ifdef FIT_USE_NODE_LOCKING
        if (v2c->lic.header.licensefp.magic == 0x666D7446) // 'fitF'
        {
            DBG(FIT_TRACE_INFO, "Fingerprint information :");
            DBG(FIT_TRACE_INFO, " Algorithm = %X\n", v2c->lic.header.licensefp.algid);
            DBG(FIT_TRACE_INFO, "Fingerprint Hash :");
            for (cntr=0; cntr<FIT_DM_HASH_SIZE; cntr++) DBG(FIT_TRACE_INFO, "%X ", v2c->lic.header.licensefp.hash[cntr]);
        }
#endif // ifdef FIT_USE_NODE_LOCKING
        if (v2c->lic.cont != NULL)
        {
            DBG(FIT_TRACE_INFO, "\nLicense container ID = %ld\n", v2c->lic.cont->id);
            if (v2c->lic.cont->vendor != NULL)
            {
                DBG(FIT_TRACE_INFO, "\tVendor ID = %ld\n", v2c->lic.cont->vendor->vendorid);
                DBG(FIT_TRACE_INFO, "\tProduct ID = %d\n", v2c->lic.cont->vendor->prod.prodid);
                if (v2c->lic.cont->vendor->prod.prodpart != NULL)
                {
                    // Get pointer to first product in the license
                    fitprodpartdata *prodpart = v2c->lic.cont->vendor->prod.prodpart;

                    while(prodpart != NULL)
                    {
                        // Get pointer to first feature in the product
                        fitfeaturedata *featarray = prodpart->properties.feat;
                        DBG(FIT_TRACE_INFO, "\t\tProduct Part Information = %d\n", prodpart->partid);

                        if (prodpart->properties.perpetual == 0)
                            DBG(FIT_TRACE_INFO, "\t\tIs License Perpetual = FALSE\n");
                        if (prodpart->properties.perpetual == 1)
                            DBG(FIT_TRACE_INFO, "\t\tIs License Perpetual = TRUE\n");
                        if (prodpart->properties.startdate != 0)
                            DBG(FIT_TRACE_INFO, "\t\tLicense Start Date = %lu\n", prodpart->properties.startdate);
                        if (prodpart->properties.enddate != 0)
                            DBG(FIT_TRACE_INFO, "\t\tLicense End Date = %lu\n", prodpart->properties.enddate);

                        while(featarray != NULL)
                        {
                            DBG(FIT_TRACE_INFO, "\t\t\tFeature ID = %d\n", featarray->featid);
                            featarray = featarray->next;
                        }
                        prodpart = prodpart->next;
                    }
                }
            }
        }
    }
    DBG(FIT_TRACE_INFO, "*** End License Data ****************************************************\n");
}


uint16_t write_get_info_buffer(uint16_t *current_offset, uint16_t buffer_length,
                               uint8_t *buffer, const char *format, ...)
{
    va_list arg;

    if((buffer_length - *current_offset) < TEMP_BUF_LEN)
        return 0;

    va_start (arg, format);
    *current_offset += (uint16_t)vsprintf ((char*)buffer + *current_offset, format, arg);
    va_end (arg);

    return *current_offset;
}

/**
 *
 * fit_testgetinfodata
 *
 * This function will test get license info API. It will try to fetch license information
 * like licgen version, list of product ID's and each product license property information.
 * Then with license information it will create a string of license info and passed
 * this string to calling function.
 *
 */
fit_status_t fit_testgetinfodata(fit_pointer_t *licenseData, uint8_t *pgetinfo, uint16_t *getinfolen)
{
    fit_status_t status = FIT_STATUS_OK;
    uint16_t featcnt    = 0;
    uint16_t cntr       = 0;
    fitv2cdata V2C    = {0};
    fitproductdata *products = NULL;
    fitfeaturedata *features = NULL;
    fitprodpartdata *prodpart = NULL;
    uint16_t offset     = 0;

    DBG(FIT_TRACE_INFO, "\nTest case:Get Info ---------\n");
    fit_memset((uint8_t*)&V2C,0x0,sizeof(fitv2cdata));

    if(pgetinfo == NULL || getinfolen <= 0)
    return FIT_INSUFFICIENT_MEMORY;
    
    // Parse license data and get requested license data
    status = fit_licenf_get_info(licenseData, fit_getlicensedata_cb, &V2C);
    if (!(status == FIT_STATUS_OK || status == FIT_CONTINUE_PARSE))
    {
        *getinfolen = 0;
        freelicensedata(&V2C);
        return status;
    }

    if (status == FIT_STATUS_OK || status == FIT_CONTINUE_PARSE)
    {
        status = FIT_STATUS_OK;
        /* check the output against hard coded hard coded values */
        if(!write_get_info_buffer(&offset, *getinfolen, pgetinfo, "Licver=%hd\n", V2C.lic.header.licgen_version))
            goto end;

        if(!write_get_info_buffer(&offset, *getinfolen, pgetinfo, "LMver=%hd\n", V2C.lic.header.lm_version))
            goto end;

        if (V2C.lic.header.uid[0] != 0 && V2C.lic.header.uid[31] != 0)
        {
            if(!write_get_info_buffer(&offset, *getinfolen, pgetinfo, "UID="))
                goto end;
            for (cntr = 0; cntr < 32; cntr++)
                if(!write_get_info_buffer(&offset, *getinfolen, pgetinfo, "%X", V2C.lic.header.uid[cntr]))
                    goto end;
            if(!write_get_info_buffer(&offset, *getinfolen, pgetinfo, "\n"))
                goto end;
        }
#ifdef FIT_USE_NODE_LOCKING
        if (V2C.lic.header.licensefp.magic == 0x666D7446) // 'fitF'
        {
            if(!write_get_info_buffer(&offset, *getinfolen, pgetinfo, "HID=%X\n", V2C.lic.header.licensefp.algid))
                goto end;

            if(!write_get_info_buffer(&offset, *getinfolen, pgetinfo, "FPHash="))
                goto end;

            for (cntr=0; cntr<FIT_DM_HASH_SIZE; cntr++)
                if(!write_get_info_buffer(&offset, *getinfolen, pgetinfo, "%X ", V2C.lic.header.licensefp.hash[cntr]))
                    goto end;

            if(!write_get_info_buffer(&offset, *getinfolen, pgetinfo, "\n"))
                goto end;
        }
#endif // ifdef FIT_USE_NODE_LOCKING

        if (V2C.lic.cont == NULL)
            goto end;

        if(!write_get_info_buffer(&offset, *getinfolen, pgetinfo, "CID=%ld\n", V2C.lic.cont->id))
            goto end;
        if (V2C.lic.cont->vendor == NULL)
            goto end;
        if(!write_get_info_buffer(&offset, *getinfolen, pgetinfo, "VID=%ld\n", V2C.lic.cont->vendor->vendorid))
            goto end;

        // Products information.
        products = (fitproductdata *)&(V2C.lic.cont->vendor->prod);
        if (products == NULL)
            goto end;
        if(!write_get_info_buffer(&offset, *getinfolen, pgetinfo, "PID=%ld\n", products->prodid))
            goto end;
        if (products->verregex[0] != 0 && products->verregex[1] != 0)
        {
            if(!write_get_info_buffer(&offset, *getinfolen, pgetinfo, "Ver_regex=%s\n", products->verregex))
                goto end;
        }

        prodpart = products->prodpart;
        while (prodpart != NULL)
        {

            featcnt=0;
            if(!write_get_info_buffer(&offset, *getinfolen, pgetinfo, "PPID=%ld\n", prodpart->partid))
                goto end;

            features = prodpart->properties.feat;
            while(features != NULL)
            {
                features = features->next;
                featcnt++;
            }
            
            if(!write_get_info_buffer(&offset, *getinfolen, pgetinfo, "FC=%d\n", featcnt))
                goto end;

            features = prodpart->properties.feat;
            if(!write_get_info_buffer(&offset, *getinfolen, pgetinfo, "FID \n"))
                goto end;
            while(features != NULL)
            {
                if(!write_get_info_buffer(&offset, *getinfolen, pgetinfo, "%ld,", features->featid))
                    goto end;
                features= features->next;
            }


            if(!write_get_info_buffer(&offset, *getinfolen, pgetinfo, "\n"))
                goto end;
            if(!write_get_info_buffer(&offset, *getinfolen, pgetinfo, "Perpetual=%d\n", prodpart->properties.perpetual))
                goto end;
            if(!write_get_info_buffer(&offset, *getinfolen, pgetinfo, "Start date=%lu\n", prodpart->properties.startdate))
                goto end;
            if(!write_get_info_buffer(&offset, *getinfolen, pgetinfo, "End date=%lu\n", prodpart->properties.enddate))
                goto end;

            prodpart = prodpart->next;
        }

        if (V2C.signature == NULL)
            goto end;
        if(!write_get_info_buffer(&offset, *getinfolen, pgetinfo, "AlgID=%hd\n", V2C.signature->algid))
            goto end;
    }

    end:
    // Print the license information
    printlicensedata(&V2C);

    // Free memory allocated in getting license information.
    freelicensedata(&V2C);
    *getinfolen = offset;

    return status;
}

