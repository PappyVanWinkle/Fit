/****************************************************************************\
**
** fit_status.h
**
** This file contains possible error codes used in Sentinel FIT.
** 
** Copyright (C) 2016, SafeNet, Inc. All rights reserved.
**
\****************************************************************************/

#ifndef __FIT_STATUS_H__
#define __FIT_STATUS_H__

/* Required Includes ********************************************************/

/* Constants ****************************************************************/

/**
 * @defgroup fit_error_codes sentinel fit core Status Codes
 *
 * @{
 */

enum fit_error_codes
{
    /** Request successfully completed */
    FIT_STATUS_OK = 0,

    /** Sentinel FIT core is out of memory */
    FIT_INSUFFICIENT_MEMORY,

    /** Specified Feature ID not available */
    FIT_INVALID_FEATURE_ID,

    /** Invalid V2C/Binary data format */
    FIT_INVALID_V2C,

    /** Access to Feature or functionality denied */
    FIT_ACCESS_DENIED,

    /** Invalid value for Sentinel fit license string. */
    FIT_STATUS_ERROR = 5,

    /** Unable to execute function in this context; the requested
     * functionality is not implemented */
    FIT_REQ_NOT_SUPPORTED,

    /** Unknown algorithm used in V2C file */
    FIT_UNKNOWN_ALG,

    /** Signature verification operation failed */
    FIT_INVALID_SIGNATURE,

    /** Requested Feature not available */
    FIT_FEATURE_NOT_FOUND,

    /** Requested Feature found */
    FIT_FEATURE_ID_FOUND = 10,

    /** Stop further parsing of Sentinel fit Licenses */
    FIT_STOP_PARSE,

    /** Continue parsing of Sentinel fit Licenses */
    FIT_CONTINUE_PARSE,

    /** licgen version used for generate license is not valid */
    FIT_INVALID_LICGEN_VERSION,

    /** signature id is not valid */
    FIT_INVALID_SIG_ID,

    /** Feature expired */
    FIT_FEATURE_EXPIRED = 15,

    /** Error occurred during caching of sentinel fit licenses */
    FIT_LIC_CACHING_ERROR,

    /** Invalid Product information */
    FIT_INVALID_PRODUCT,

    /** Invalid function parameter */
    FIT_INVALID_PARAM,

    /** Invalid function first parameter */
    FIT_INVALID_PARAM_1,

    /** Invalid function second parameter */
    FIT_INVALID_PARAM_2 = 20,

    /** Invalid function third parameter */
    FIT_INVALID_PARAM_3,

    /** Invalid function fourth parameter */
    FIT_INVALID_PARAM_4,

    /** Invalid function fifth parameter */
    FIT_INVALID_PARAM_5,

    /** Unit test passes status. */
    FIT_UNIT_TEST_PASSED,

    /** Unit test failed error. */
    FIT_UNIT_TEST_FAILED = 25,

    /** Invalid wire type */
    FIT_INVALID_WIRE_TYPE,

    /** Internal error occurred in Sentinel fit core */
    FIT_INTERNAL_ERROR,

    /** OMAC validate erorr */
    FIT_INVALID_KEYSIZE,

    /** invalid vendor id */
    FIT_INVALID_VENDOR_ID,

    /** invalid product id */
    FIT_INVALID_PRODUCT_ID = 30,

    /** invalid license container id */
    FIT_INVALID_CONT_ID,

    /** Field data is present in license */
    FIT_LIC_FIELD_PRESENT,

    /** Invlaid license type */
    FIT_INVALID_LIC_TYPE,

    /** Time expiration not supported */
    FIT_LIC_EXPIRATION_NOT_SUPP,

    /** Invalid start date value */
    FIT_INVALID_START_DATE = 35,

     /** Invalid end date value */
    FIT_INVALID_END_DATE,

    /** License not active */
    FIT_INACTIVE_LICENSE,

    /** No real time clock is present on board */
    FIT_RTC_NOT_PRESENT,

    /** Clock support not present */
    FIT_NO_CLOCK_SUPPORT,

    /** length not valid */
    FIT_INVALID_FIELD_LEN = 40,

    /* Data comparison gets failed */
    FIT_DATA_MISMATCH_ERROR,

    /* Code not compiled with node locking */
    FIT_NODE_LOCKING_NOT_SUPP,

    /** fingerprint magic value not correct */
    FIT_FP_MAGIC_NOT_VALID,

    /** Unknown fingerprint algorithm */
    FIT_UNKNOWN_FP_ALG,

    /* Fingerprint data comparison gets failed */
    FIT_FP_MISMATCH_ERROR = 45,

    /* Invalid device id length */
    FIT_INVALID_DEVICE_LEN,

    /* RSA Verification failed error */
    FIT_RSA_VERIFY_FAILED,

};

/**
 * @}
 */

/* Types ********************************************************************/

typedef enum fit_error_codes fit_status_t;

/* Macro Functions **********************************************************/

#endif /* __FIT_STATUS_H__ */

