/****************************************************************************\
**
** version.c
**
** Defines sentinel fit core versioning functionality
** 
** Copyright (C) 2016, SafeNet, Inc. All rights reserved.
**
\****************************************************************************/

#include "fit_types.h"
#include "fit_version.h"

#include "stddef.h"

/**
 *
 * fit_licenf_get_version
 *
 * This function used for getting information about sentinel fit core versioning
 * information.
 *
 * @param   major_version <-- On return it will contain the sentinel fit core major
 *                            version data.
 * @param   minor_version <-- On return it will contain the sentinel fit core minor
 *                            version data.
 * @param   revision <-- On return it will contain the sentinel fit core revision data.
 *
 */
fit_status_t fit_licenf_get_version(uint8_t* major_version,
                                    uint8_t* minor_version,
                                    uint8_t* revision)
{
    if (major_version == NULL)
        return FIT_INVALID_PARAM_1;
    if (minor_version == NULL)
        return FIT_INVALID_PARAM_2;
    if (revision == NULL)
        return FIT_INVALID_PARAM_3;

    *major_version = FIT_MAJOR_VERSION;
    *minor_version = FIT_MINOR_VERSION;
    *revision = FIT_REVISION_VERSION;

    return FIT_STATUS_OK;
}
