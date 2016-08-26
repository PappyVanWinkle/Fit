/****************************************************************************\
**
** unit_test.h
**
** Contains declaration for unit tests cases for testing Sentinel fit code developed
** for embedded devices.
** 
** Copyright (C) 2016, SafeNet, Inc. All rights reserved.
**
\****************************************************************************/

#ifndef __FIT_UNIT_TEST_H__
#define __FIT_UNIT_TEST_H__

/* Required Includes ********************************************************/
#include "fit_status.h"
#include "fit.h"

/* Constants ****************************************************************/

/* Forward Declarations *****************************************************/

/* Types ********************************************************************/

/* Function Prototypes ******************************************************/

void fit_set_time(uint32_t settime);
fit_status_t fit_unit_test_entry(void);
fit_status_t fit_unit_tests(void);
fit_status_t fit_unit_testparselic(fit_pointer_t *licenseData);
fit_status_t fit_unit_testgetinfodata(fit_pointer_t *licenseData, uint8_t *pgetinfo, uint16_t *getinfolen);
fit_status_t fit_unit_test_lic_container(fit_pointer_t *licenseData);
fit_status_t fit_unit_test_lic_vendor(fit_pointer_t *licenseData);
fit_status_t fit_unit_test_lic_product(fit_pointer_t *licenseData);
fit_status_t fit_unit_test_lic_expiration(void);
fit_status_t fit_unit_test_lic_property(fit_pointer_t *licenseData);
fit_status_t fit_unit_test_lic_feature(fit_pointer_t *licenseData);
fit_status_t fit_unit_test_lic_header(fit_pointer_t *licenseData);
fit_status_t fit_unit_test_parse_lic_level(fit_pointer_t *licenseData);
fit_status_t fit_unit_test_wire_protocol(fit_pointer_t *licenseData);
fit_status_t fit_unit_test_aes_algorithm(void);
fit_status_t fit_unit_test_dm_algorithm(void);


#endif /* __FIT_UNIT_TEST_H__ */

