/*
 * SDL TEST
 *
 * SafeTI Diagnostics Library Test
 *
 * Copyright (C) 2018-2019 Texas Instruments Incorporated - http://www.ti.com/
 * ALL RIGHTS RESERVED
 */

#ifndef _INCLUDE_SDTF_CORE_H_
#define _INCLUDE_SDTF_CORE_H_

#include <stdint.h>

#define TIMER_ID 1

extern uint32_t SDTF_EccNumMemCfgEntries;

void SDTF_cpuCacheInv(const void * addr, int32_t size);
void SDTF_cpuCacheWbInv(const void * addr, int32_t size);

#endif /* _INCLUDE_SDTF_CORE_H_ */
