/****************************************************************************\
**
** fit.h
**
** Sentinel FIT Licensing interface header file. File contains exposed interface for
** C/C++ language.
** 
** Copyright (C) 2016, SafeNet, Inc. All rights reserved.
**
\****************************************************************************/

#ifndef __FIT_H__
#define __FIT_H__

/* Required Includes ********************************************************/
#include "fit_types.h"
#include "fit_api.h"
#include "mem_read.h"

/* Constants ****************************************************************/
#define MAX_FEATURE_ID_VALUE        65471
#define MAX_PRODUCT_ID_VALUE        65471
#define MAX_LC_ID_VALUE             0xFFFFFFFF
#define MAX_VENDOR_ID_VALUE         0xFFFFFF
#define MAX_START_DATE_VALUE        0x7FFFFFFF
#define MAX_END_DATE_VALUE          0x7FFFFFFF
#define FIT_UID_LEN                 32
#define FIT_MAX_FIELD_SIZE          32
#define FIT_DM_HASH_SIZE            16

/**IN parameter*/
#define IN
/**OUT parameter*/
#define OUT
/**IN/OUT parameter*/
#define IO

enum licensetype {
    /** Invalid value */
    FIT_LIC_INVALID_VALUE       = 0,
    /** Perpetual licenses */
    FIT_LIC_PERPETUAL           = 1,
    /** Time based licenses i.e no. of days from its first use */
    FIT_LIC_TIME_BASED,
    /** Execution based licenses */
    FIT_LIC_COUNTER_BASED,
    /** Time expiration based licenses */
    FIT_LIC_EXPIRATION_BASED,
};

// Enum describing types of query to be operate on sentinel fit licenses.
enum fit_operation_type {
    /** Default value. It means no data requested */
    FIT_NONE_OPERATION              = 0,
    /** Parse sentinel fit license */
    FIT_PARSE_LICENSE,
    /** consume one license at each consume license apil call. */
    FIT_CONSUME_LICENSE,
    /** get the field data at particular level and index */
    FIT_GET_FIELD_DATA,
    /** Get vendor information */
    FIT_GET_VENDORID,
    /** Get license UID value */
    FIT_GET_LICENSE_UID,
    /** Get data address at particular level and index */
    FIT_GET_DATA_ADDRESS,
    /** Get licence related info */
    FIT_GET_LICENSE_INFO_DATA,

#ifdef FIT_USE_UNIT_TESTS
    // Describes types of query to be operate on sentinel fit licenses for testing licence string.
    /** test for validate license data i.e. it should parse without any error.*/
    FIT_TEST_PARSE_LICENSE,
    /** test for validate license container data */
    FIT_TEST_LIC_CONTAINER_DATA,
    /** test for validate license vendor information data like vendor id etc. */
    FIT_TEST_VENDOR_DATA,
    /** test for validate license product definition. */
    FIT_TEST_LIC_PRODUCT_DATA,
    /** test for validate license property information. */
    FIT_TEST_LICENSE_PROPERTY_DATA,
    /** test for validate license fetaure definition or data. */
    FIT_TEST_FEATURE_DATA,
    /** test for validate license header information like version information etc. */
    FIT_TEST_LIC_HEADER_DATA,
    /** test for parsing license strung from any level and index. */
    FIT_TEST_PARSE_LIC_FROM_ANY_LEVEL,
    /** test for wire protocol */
    FIT_TEST_WIRE_PROTOCOL,
    /** test for validity of AES algorithm */
    FIT_TEST_AES_ALGORITHM,
#endif // #ifdef FIT_USE_UNIT_TESTS

    FIT_LAST_OPERATION,
};

/* Forward Declarations *****************************************************/


/* Types ********************************************************************/

// Defines context data for sentinel fit. This structure is used when user wants to query
// license data or wants to see current state of sentinel fit licenses.
typedef struct context {
    // Defines operation type. See enum fit_operation_type
    uint8_t m_operation;
    // License schema level/depth info.
    uint8_t m_level;
    // License schema index info.
    uint8_t m_index;
    // If test callback fn to be called.
    uint8_t m_testop;
    // Contains length of license data.
    uint16_t m_length;
    // Contains Return value if required.
    uint8_t mstatus;

    union {
        // License data address. To be used for getting pointer to license data at
        // particular level and index.
        uint8_t *m_addr;
        // can be feature id or product id or any other valid value for sentinel fit based licenses.
        uint32_t m_id;
        // Query for vendor id.
        uint32_t m_vendorid;
        // Contains license unique UID value.
        uint8_t m_uid[FIT_UID_LEN];

        // get info data
        struct {
            // Callback operation
            fit_get_info_callback m_callback_fn;
            // Requested data format.
            void *m_get_info_data;

        } m_getinfodata;

    } mparserdata;

} fitcontextdata;

// Structure describing fingerprint information.
typedef struct fingerprint_t {
    uint32_t    magic;      // fingerprint magic
    uint8_t     algid;      // fingerprint algorithm id
    uint8_t     hash[16];   // hash (Davies Meyer) of fingerprint
} fit_fingerprint_t;

// Structure defining license features information 
typedef struct fitfeature {
    // feature ID
    uint32_t featid;
    // Pointer to next feature ID (one product ID can contain multiple features)
    struct fitfeature *next;
} fitfeaturedata;

// Structure defining product license property information. All features inside
// product share same licensing model.
typedef struct {
    // Pointer to feature data.
    fitfeaturedata  *feat;
    // tell whether feature's are perpetual or not.
    uint8_t         perpetual;
    // Start date information for time based licenses.
    uint32_t        startdate;
    // End date information for time based licenses.
    uint32_t        enddate;
} fitpropertydata;

// Structure defining product part information for multi license model.
typedef struct fitprodpart{
    // Product Part ID.
    uint32_t        partid;
    // Structure containing license property information.
    fitpropertydata properties;
    // License type
    uint8_t         lictype;
    // Pointer to next product part information.
    struct fitprodpart *next;
} fitprodpartdata;

// Structure defining license product information.
typedef struct fitproduct{
    // Product ID.
    uint32_t        prodid;
    // Product version related information.
    char            verregex[32];
    // Pointer to product part information.
    fitprodpartdata *prodpart;
} fitproductdata;

// Structure defining vendor information.
typedef struct {
    // Vendor ID.
    uint32_t        vendorid;
    // Pointer to license product information.
    fitproductdata  prod;
} fitvendordata;

// Structure defining license Container information.
typedef struct {
    // License container ID.
    uint32_t        id;
    // Pointer to Vendor information.
    fitvendordata   *vendor;
} fitcontainerdata;

// Structure defining License Header information.
typedef struct {
    // Licgen version used for creating licenses for embedded devices.
    uint16_t    licgen_version;
    // LM version used.
    uint16_t    lm_version;
    // Unique license identifier. Used in creating license updates.
    uint8_t     uid[32];
    // fingerprint data.
    fit_fingerprint_t licensefp;
} fitheaderdata;

// Structure defining license data for embedded devices.
typedef struct {
    // Structure defining License Header data.
    fitheaderdata       header;
    // Pointer to license container data.
    fitcontainerdata    *cont;
} fitlicensedata;

// Structure defining license signature data.
typedef struct {
    // Algorithm used for signing license data for embedded devices.
    uint16_t    algid;
} fitsignaturedata;

// Structure defining V2C data.
typedef struct {
    // Structure containing license data.
    fitlicensedata      lic;
    // Structure containing license signature data.
    fitsignaturedata    *signature;
} fitv2cdata;

/* Macro Functions **********************************************************/

/* Function Prototypes ******************************************************/


#endif // __FIT_H__

