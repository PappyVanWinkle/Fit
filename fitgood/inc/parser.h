/****************************************************************************\
**
** parser.h
**
** Contains declaration for structures, enum, constants and functions used in
** parsing licenses.
**
** Copyright (C) 2016, SafeNet, Inc. All rights reserved.
**
** $Header:
**
\****************************************************************************/

#ifndef __FIT_PARSER_H__
#define __FIT_PARSER_H__

/* Required Includes ********************************************************/
#include "fit.h"
#include "stddef.h"
#include "mem_read.h"

/* Constants ****************************************************************/

/* Forward Declarations *****************************************************/
typedef unsigned char wire_type_t;

/* Types ********************************************************************/

/* Macro Functions **********************************************************/

/* Function Prototypes ******************************************************/

// This function will register all wire types corresponding to.particular level and
// index i.e. hard-coded the wire types corresponding to level and index.

// This function will parse the license data passed to it
fit_status_t fit_parse_object(uint8_t level,
                              uint8_t index,
                              fit_pointer_t *pdata,
                              void *context);
// Return wire type corresponding to index and level passed in.
wire_type_t get_field_type(uint8_t level,
                           uint8_t index);
// This functoin will traverse each object of an array and call appropriate
// functions to parse individual objects of an array.
fit_status_t fit_parse_array(uint8_t level,
                             uint8_t index,
                             fit_pointer_t *pdata,
                             void *context);

// This function will be used to validate rsa signature value present in license binary
fit_status_t fit_check_license_validation(fit_pointer_t* license,
                                          fit_pointer_t* rsakey);


#endif /* __FIT_PARSER_H__ */

