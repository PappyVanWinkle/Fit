/****************************************************************************\
**
** mem_read.h
**
** Contains declaration for memory related functions used in sentinel fit project
**
** Copyright (C) 2016, SafeNet, Inc. All rights reserved.
**
\****************************************************************************/

#ifndef __FIT_MEM_READ_H__
#define __FIT_MEM_READ_H__

#ifdef __cplusplus
#define EXTERNC extern "C"
#else
#define EXTERNC
#endif

/* Required Includes ********************************************************/
#include "stddef.h"
#include "fit_types.h"

/* Constants ****************************************************************/
#define FIT_READ_BYTE_RAM          fit_read_ram_u8

/* Forward Declarations *****************************************************/
EXTERNC uint8_t  FIT_READ_BYTE_RAM  (const uint8_t *p);

/* Types ********************************************************************/

/* Macro Functions **********************************************************/

/* Function Prototypes ******************************************************/

/**
 *
 * read_byte
 *
 * Reads 1 byte data from data pointer passed in.
 *
 * @param   address --> pointer to data.
 *
 * @param   clbk_read_byte --> function pointer to read byte.
 *
 */
uint8_t read_byte(const uint8_t *address,
                  fit_read_byte_callback_t clbk_read_byte);

/**
 *
 * read_word
 *
 * Reads 2 byte data from data pointer passed in.
 *
 * @param   address --> pointer to data.
 *
 * @param   clbk_read_byte --> function pointer to read byte.
 *
 */
uint16_t read_word(const uint8_t* address,
                   fit_read_byte_callback_t clbk_read_byte);

/**
 *
 * read_dword
 *
 * Reads 4 byte data (1 word) from data pointer passed in.
 *
 * @param   address --> pointer to data.
 *
 * @param   clbk_read_byte --> function pointer to read byte.
 *
 */
uint32_t read_dword(const uint8_t* address,
                    fit_read_byte_callback_t clbk_read_byte);

#endif /* __FIT_MEM_READ_H__ */
