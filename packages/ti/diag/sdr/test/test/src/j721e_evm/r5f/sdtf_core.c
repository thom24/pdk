/*
 * SDL TEST
 *
 * Software Diagnostics Library Test
 *
 * Copyright (C) 2018-2020 Texas Instruments Incorporated - http://www.ti.com/
 * ALL RIGHTS RESERVED
 */

#include <ti/csl/arch/r5/csl_arm_r5_pmu.h>
#include <ti/csl/arch/r5/csl_cache.h>
#include "sdtf_common.h"
#include "sdtf_core.h"

uint32_t SDTF_profileTimerRead(void)
{
    return(CSL_armR5PmuReadCntr(CSL_ARM_R5_PMU_CYCLE_COUNTER_NUM));
}

void SDTF_profileTimerInit(void)
{

}

void SDTF_cpuCacheInv(const void * addr, int32_t size)
{
    CSL_armR5CacheInv(addr, size);
}

void SDTF_cpuCacheWbInv(const void * addr, int32_t size)
{
    CSL_armR5CacheWbInv(addr, size);
}
