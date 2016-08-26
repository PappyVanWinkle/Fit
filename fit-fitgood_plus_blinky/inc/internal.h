/****************************************************************************\
**
** internal.h
**
** Contains declaration for strctures, enum, constants and functions used in Sentinel fit
** project and not exposed outside.
** 
** Copyright (C) 2016, SafeNet, Inc. All rights reserved.
**
\****************************************************************************/

#ifndef __FIT_INTERNAL_H__
#define __FIT_INTERNAL_H__

/* Required Includes ********************************************************/
#include "fit.h"
#include "consume.h"
#include "get_info.h"
#include "mem_read.h"

#ifdef __cplusplus
#define EXTERNC extern "C"
#else
#define EXTERNC
#endif

#ifdef FIT_USE_UNIT_TESTS
#include "unittest/test_parser.h"
#endif // #ifdef FIT_USE_UNIT_TESTS

/* Constants ****************************************************************/

//Maximum key size and fingerprint magic
#define FP_MAGIC            0x666D7446          //'fitF';
#define POBJECT_SIZE        sizeof(uint32_t)
#define PARRAY_SIZE         sizeof(uint32_t)
#define PSTRING_SIZE        sizeof(uint32_t)
#define PFIELD_SIZE         sizeof(uint16_t)
#define MAX_LEVEL           16
#define MAX_INDEX           16

// Algorithms used in sentinel fit core.
#define AES_ALGID          1

// Sentinel fit license schema data types.
enum wire_type {
    FIT_INTEGER         = 1,
    FIT_STRING          = 2,
    FIT_OBJECT          = 3,
    FIT_ARRAY           = 4,

    FIT_INVALID_VALUE   = 0x0FF,
};

// Type of licensing model supported.
typedef struct licensemodel
{
    uint8_t perpetual;
    uint8_t startdate;
    uint8_t enddate;
} fitlicensemodel;

/* Forward Declarations *****************************************************/

// This function will be used to get the field value at particular level and index.
fit_status_t fit_parse_field_data(fit_pointer_t *pdata,
                                  uint8_t level,
                                  uint8_t index,
                                  uint16_t length,
                                  void *context);

// This function will be used to get address at particular level and index of license binary.
fit_status_t fit_get_data_address(fit_pointer_t *pdata,
                                  uint8_t level,
                                  uint8_t index,
                                  uint16_t length,
                                  void *context);

/* Types ********************************************************************/

// Global structure for caching RSA validation data. It caches the hash of license
// string using Davies Meyer hash function.
typedef struct {
    uint8_t m_rsa_check_done;
    uint8_t m_dm_hash[FIT_DM_HASH_SIZE];
} fit_cache_data;

// Hard coded level and index values for sentinel fit licenses (as per sproto schema)
#define STRUCT_V2C_LEVEL                0
#define LICENSE_FIELD                   0
#define SIGNATURE_FIELD                 1

// License data at level 1
#define STRUCT_LICENSE                  1
#define HEADER_FIELD                    0
#define LICENSE_CONTAINER_FIELD         1
// Signature data at level 1
#define STRUCT_SIGNATURE_LEVEL          1
#define ALGORITHM_ID_FIELD              2
#define RSA_SIGNATURE_FIELD             3

// Header data at level 2
#define STRUCT_HEADER_LEVEL             2
#define LICGEN_VERSION_FIELD            0
#define LM_VERSION_FIELD                1
#define UID_FIELD                       2
#define FINGERPRINT_FIELD               3
// License container data at level 2
#define STRUCT_LICENSE_CONTAINER_LEVEL  2
#define ID_LC_FIELD                     4
#define VENDOR_FIELD                    5

// Vendor data at level 3
#define STRUCT_VENDOR_LEVEL             3
#define ID_VENDOR_FIELD                 0
#define PRODUCT_FIELD                   1
#define VENDOR_NAME_FIELD               2

// Product information at level 4
#define STRUCT_PRODUCT_LEVEL            4
#define ID_PRODUCT_FIELD                0
#define VERSION_REGEX_FIELD             1
#define PRODUCT_PART_FIELD              2

// Product part data at level 5
#define STRUCT_PRODUCT_PART_FIELD       5
#define PRODUCT_PART_FIELD_ID           0
#define LIC_PROP_FIELD                  1

// License property data at level 6
#define STRUCT_LIC_PROP_LEVEL           6
#define FEATURE_FIELD                   0
#define PERPETUAL_FIELD                 1
#define START_DATE_FIELD                2
#define END_DATE_FIELD                  3
#define COUNTER_FIELD                   4
#define DURATION_FROM_FIRST_USE_FIELD   5

// Feature information at level 7
#define STRUCT_FEATURE_LEVEL            7
#define ID_FEATURE_FIELD                0

#define STRUCT_COUNTER_LEVEL            7
#define ID_COUNTER_FIELD                2
#define LIMIT_FIELD                     3
#define SOFT_LIMIT_FIELD                4
#define IS_FIELD                        5

// Prototype of a callback function. This function is called during parsing of 
// sentinel fit licenses.
typedef fit_status_t (*fit_callback)(fit_pointer_t *pdata,
                                     uint8_t level,
                                     uint8_t index,
                                     uint16_t length,
                                     void *context);

// This structure is used for registering callbacks for each operation type.
// Each callback fn should have same prototype.
struct callbacks
{
    uint8_t m_operation;
    fit_callback m_callback_fn;
};

#ifdef FIT_USE_UNIT_TESTS

// Prototype of a callback function. This function is called during parsing of sentinel fit
// licenses for testing validity of licenses.
typedef fit_status_t (*fit_test_field_callback)(fit_pointer_t *pdata,
                                                uint8_t level,
                                                uint8_t index,
                                                void *context);

// This structure is used for registering callbacks at particular level and index.
// Callback function can be same at all levels and index or different/unique for
// each level and index, but each callback fn should have same prototype.
struct testcallbacks
{
    uint8_t level;
    uint8_t index;
    uint8_t m_operation;
    fit_test_field_callback m_callback_fn;
};

#endif //#ifdef FIT_USE_UNIT_TESTS

/* Macro Functions **********************************************************/

/* Function Prototypes ******************************************************/

/*
 * Callback function for get_info. called for every item while traversing license data
 * This function will get complete license information for embedded devices
 */
EXTERNC fit_status_t fit_getlicensedata_cb (uint8_t tagid,
                                            fit_pointer_t *pdata,
                                            uint16_t length,
                                            void *context);

/*
 * free memory allocated while traversing license data
 */
EXTERNC void freelicensedata (void *context);

/*
 * print human readable summary of license data
 */
EXTERNC void printlicensedata (void *context);

// This function will fetch fingerprint/deviceid for the respective board. This will
// call the hardware implemented callback function which will give raw data that would
// be unique to each device. Raw data would be then hash with Daview Meyer hash function.
fit_status_t fit_get_device_fpblob(fit_fingerprint_t* fp,
                                   fit_fp_callback callback_fn);

// This function will fetch licensing information present in the data passed in.
fit_status_t fit_testgetinfodata(fit_pointer_t *licenseData, uint8_t *pgetinfo,
                                 uint16_t *getinfolen);
// This function will return the current time in unix.
fit_status_t fit_getunixtime(uint32_t *unixtime);
// This function is used for validating licensing data
fit_status_t fit_verify_license(fit_pointer_t *license,
                                fit_pointer_t *key,
                                uint8_t check_cache);

void getfingerprintdata(fit_pointer_t *fpdata, fit_fingerprint_t *fpstruct);
void fit_memcpy(uint8_t *dst, uint8_t *src, uint16_t srclen);
void fitptr_memcpy(uint8_t *dst, fit_pointer_t *src);
int16_t fit_memcmp(uint8_t *pdata1, uint8_t *pdata2, uint16_t len);
void fit_memset(uint8_t *pdata, uint8_t value, uint16_t len);

#endif  /* __FIT_INTERNAL_H__ */
