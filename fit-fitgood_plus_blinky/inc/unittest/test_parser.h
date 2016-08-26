/****************************************************************************\
**
** test_parser.h
**
** Defines functionality for consuming licenses for embedded devices.
** 
** Copyright (C) 2016, SafeNet, Inc. All rights reserved.
**
\****************************************************************************/

#ifndef __FIT_TEST_PARSER_H__
#define __FIT_TEST_PARSER_H__

#ifdef FIT_USE_UNIT_TESTS
/* Required Includes ********************************************************/

/* Constants ****************************************************************/
#define MIN_ARRAY_LEN       8
#define MIN_OBJECT_LEN      6

/* Forward Declarations *****************************************************/

/* Types ********************************************************************/

/* Function Prototypes ******************************************************/

// This function will test the presence and validity of license container data in
// the license string.
fit_status_t fit_test_lic_container_data(fit_pointer_t *pdata,
                                         uint8_t level,
                                         uint8_t index,
                                         void *context);

// This function will test the presence and validity of vendor data in the license string.
fit_status_t fit_test_vendor_data(fit_pointer_t *pdata,
                                  uint8_t level,
                                  uint8_t index,
                                  void *context);

// This function will test the presence and validity of product information in
// the license string
fit_status_t fit_test_lic_product_data(fit_pointer_t *pdata,
                                       uint8_t level,
                                       uint8_t index,
                                       void *context);

// This function will test the presence and validity of license property information
// in the license string
fit_status_t fit_test_lic_property_data(fit_pointer_t *pdata,
                                        uint8_t level,
                                        uint8_t index,
                                        void *context);

// This function will test the presence and validity of feature information in
// in the license string
fit_status_t fit_test_feature_info(fit_pointer_t *pdata,
                                   uint8_t level,
                                   uint8_t index,
                                   void *context);

fit_status_t fit_test_header_data(fit_pointer_t *pdata,
                                  uint8_t level,
                                  uint8_t index,
                                  void *context);

fit_status_t fit_test_licence_binary(fit_pointer_t *pdata,
                                     uint8_t level,
                                     uint8_t index,
                                     void *context);

fit_status_t fit_test_wire_protocol(fit_pointer_t *pdata,
                                    uint8_t level,
                                    uint8_t index,
                                    void *context);

#endif // #ifdef FIT_USE_UNIT_TESTS
#endif /* __FIT_TEST_PARSER_H__ */

