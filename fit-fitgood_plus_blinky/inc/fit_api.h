/****************************************************************************\
**
** fit_api.h
**
** Sentinel FIT Licensing interface header file. File contains exposed interface for
** C/C++ language.
**
** Copyright (C) 2016, SafeNet, Inc. All rights reserved.
**
** $Header:
**
\****************************************************************************/

#ifndef __FIT_API_H__
#define __FIT_API_H__

/* Required Includes ********************************************************/
#include "fit_types.h"
#include "fit_debug.h"
#include "mem_read.h"

/* Constants ****************************************************************/

// Define tag id for each field (as per sproto schema). So each tagid will represent
// member/field used in sentinel fit licenses.
#define FIT_BASE_TAG_ID_VALUE               0

#define FIT_LICENSE_TAG_ID                  FIT_BASE_TAG_ID_VALUE + 1
#define FIT_SIGNATURE_TAG_ID                FIT_LICENSE_TAG_ID + 1
#define FIT_HEADER_TAG_ID                   FIT_SIGNATURE_TAG_ID + 1
#define FIT_LIC_CONTAINER_TAG_ID            FIT_HEADER_TAG_ID + 1
#define FIT_ALGORITHM_TAG_ID                FIT_LIC_CONTAINER_TAG_ID + 1
#define FIT_RSA_SIG_TAG_ID                  FIT_ALGORITHM_TAG_ID + 1
#define FIT_LICGEN_VERSION_TAG_ID           FIT_RSA_SIG_TAG_ID + 1
#define FIT_LM_VERSION_TAG_ID               FIT_LICGEN_VERSION_TAG_ID + 1
#define FIT_UID_TAG_ID                      FIT_LM_VERSION_TAG_ID + 1
#define FIT_FP_TAG_ID                       FIT_UID_TAG_ID + 1
#define FIT_ID_LC_TAG_ID                    FIT_FP_TAG_ID + 1
#define FIT_VENDOR_ARRAY_TAG_ID             FIT_ID_LC_TAG_ID + 1
#define FIT_VENDOR_ID_TAG_ID                FIT_VENDOR_ARRAY_TAG_ID + 1
#define FIT_PRODUCT_TAG_ID                  FIT_VENDOR_ID_TAG_ID + 1
#define FIT_PRODUCT_ID_TAG_ID               FIT_PRODUCT_TAG_ID + 1
#define FIT_VERSION_REGEX_TAG_ID            FIT_PRODUCT_ID_TAG_ID + 1
#define FIT_PRODUCT_PART_ARRAY_TAG_ID       FIT_VERSION_REGEX_TAG_ID + 1
#define FIT_PRODUCT_PART_ID_TAG_ID          FIT_PRODUCT_PART_ARRAY_TAG_ID + 1
#define FIT_LIC_PROP_TAG_ID                 FIT_PRODUCT_PART_ID_TAG_ID + 1
#define FIT_FEATURE_ARRAY_TAG_ID            FIT_LIC_PROP_TAG_ID + 1
#define FIT_PERPETUAL_TAG_ID                FIT_FEATURE_ARRAY_TAG_ID + 1
#define FIT_START_DATE_TAG_ID               FIT_PERPETUAL_TAG_ID + 1
#define FIT_END_DATE_TAG_ID                 FIT_START_DATE_TAG_ID + 1
#define FIT_COUNTER_ARRAY_TAG_ID            FIT_END_DATE_TAG_ID + 1
#define FIT_DURATION_FROM_FIRST_USE_TAG_ID  FIT_COUNTER_ARRAY_TAG_ID + 1
#define FIT_FEATURE_TAG_ID                  FIT_DURATION_FROM_FIRST_USE_TAG_ID + 1
#define FIT_COUNTER_TAG_ID                  FIT_FEATURE_TAG_ID + 1
#define FIT_LIMIT_TAG_ID                    FIT_COUNTER_TAG_ID + 1
#define FIT_SOFT_LIMIT_TAG_ID               FIT_LIMIT_TAG_ID + 1
#define FIT_IS_FIELD_TAG_ID                 FIT_SOFT_LIMIT_TAG_ID + 1

// Please Update FIT_END_TAG_ID when adding new tag id's at bottom of list.
#define FIT_END_TAG_ID                      FIT_IS_FIELD_TAG_ID + 1

/* Forward Declarations *****************************************************/

/* Types ********************************************************************/

/* Function Prototypes ******************************************************/

#ifdef __cplusplus
extern "C" {
#endif

// This function is used to grant or deny access to different areas of functionality
// in the software. This feature is similar to login type operation on licenses. It
// will look for presence of feature id in the license binary.
fit_status_t fit_licenf_consume_license(fit_pointer_t* license,
                                        uint16_t feature_id,
                                        void* state_buffer,
                                        fit_pointer_t* key);

// This function will parse the license binary passed to it and call the user provided
// callback function for every field data. User can take any action on receiving
// license field data like storing values in some structure or can take some action
// like consume license etc.
fit_status_t fit_licenf_get_info(fit_pointer_t* license,
                                 fit_get_info_callback callback_fn,
                                 void *context);

// This function is used to validate following:
//      1. RSA signature of new license.
//      2. New license node lock verification.
fit_status_t fit_licenf_validate_license(fit_pointer_t *license,
                                         fit_pointer_t *key);

// This function used for getting information about sentinel fit core versioning information
fit_status_t fit_licenf_get_version(uint8_t* major_version,
                                    uint8_t* minor_version,
                                    uint8_t* revision);
#ifdef __cplusplus
}
#endif
#endif /* __FIT_API_H__ */

