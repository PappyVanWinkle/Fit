/****************************************************************************\
**
** hwdep\tm4c1294xl\fingerprint.c
**
** get board fingerprint - TM4C1294XL version
**
** Copyright (C) 2016, SafeNet, Inc. All rights reserved.
**
\****************************************************************************/

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "fit_debug.h"
#include <driverlib/rom.h>
#include "hwdep.h"


char devid[64] = DEV_ID;
uint16_t devid_len = sizeof(DEV_ID) - 1;

//static void get_mac_address (char *mac, uint16_t *maclen)
//{
//  uint32_t ui32User0 = 0, ui32User1 = 0;
//  uint8_t m1, m2, m3, m4, m5, m6;
//
//  ROM_FlashUserGet(&ui32User0, &ui32User1);
//  m1 = (ui32User0 >>  0) & 0xff;
//  m2 = (ui32User0 >>  8) & 0xff;
//  m3 = (ui32User0 >> 16) & 0xff;
//  m4 = (ui32User1 >>  0) & 0xff;
//  m5 = (ui32User1 >>  8) & 0xff;
//  m6 = (ui32User1 >> 16) & 0xff;
//
//  *maclen = sprintf(mac, "%02X%02X%02X%02X%02X%02X", m1, m2, m3, m4, m5, m6);
//}

/*fit_status_t fit_deviceid_get(uint8_t *rawdata,
                              uint16_t *datalen)
{
	//00 1A B6 02 D9 71
    DBG(FIT_TRACE_INFO, "Fetching deviceid for tiva board: ");
	
    strncpy(rawdata, "TM4C1294XL_AABBCCDDEEFF", 10);
    get_mac_address(rawdata+10);
    *datalen = 22;
   
    return FIT_STATUS_OK;
}*/


fit_status_t fit_deviceid_get(uint8_t *rawdata,
                              uint16_t *datalen)
{
    DBG(FIT_TRACE_INFO, "Fetching deviceid for tiva board: ");

    memcpy(rawdata, devid, devid_len);
    *datalen = devid_len;

    return FIT_STATUS_OK;
}

