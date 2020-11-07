/*
 * Copyright (C) 2020 Texas Instruments Incorporated - http://www.ti.com/
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of Texas Instruments Incorporated nor the names of
 * its contributors may be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include "sbl_soc.h"
#include "sbl_soc_cfg.h"
#include "sbl_log.h"
#include "sbl_profile.h"
#include "sbl_rprc_parse.h"
#include "sbl_err_trap.h"
#include "sbl_rcm.h"
#include <strings.h>
#include <ti/drv/i2c/I2C.h>
#include <ti/drv/i2c/soc/I2C_soc.h>
#include <ti/board/board.h>


sblProfileInfo_t sblProfileLog[MAX_PROFILE_LOG_ENTRIES];
uint32_t sblProfileLogIndx = 0, sblProfileLogOvrFlw = 0;

uint32_t SBL_IsAuthReq(void);

void SBL_init_profile(void)
{
    CSL_armR5PmuEnableAllCntrs(1);    /* Set/clear PMCR E-bit */
    CSL_armR5PmuResetCntrs();         /* Set PMCR P-bit */
    CSL_armR5PmuResetCycleCnt();      /* Set PMCR C-bit */
    CSL_armR5PmuEnableCntr(CSL_ARM_R5_PMU_CYCLE_COUNTER_NUM, 1);     /* Set PMCNTENSET for event */
    CSL_armR5PmuClearCntrOverflowStatus(0x80000007);
}

uint32_t SBL_readPmu(void)
{
    return (CSL_armR5PmuReadCntr(0x1F));
}

#ifdef __cplusplus
#pragma FUNC_EXT_CALLED()
#else
#pragma FUNC_EXT_CALLED(SBL_LogCycleCount)
#endif
uint32_t SBL_LogCycleCount(uint8_t *fxn, uint32_t line)
{
    uint32_t indx = sblProfileLogIndx % MAX_PROFILE_LOG_ENTRIES;
    sblProfileInfo_t *sblProfileLogPtr = &(sblProfileLog[indx]);

    if (sblProfileLogIndx == MAX_PROFILE_LOG_ENTRIES)
    {
        sblProfileLogOvrFlw = 1;
    }

    sblProfileLogPtr->fxn = fxn;
    sblProfileLogPtr->line = line;
    sblProfileLogPtr->cycle_cnt = CSL_armR5PmuReadCntr(0x1F);

    sblProfileLogIndx++;

    return sblProfileLogPtr->cycle_cnt;
}

void SBL_cpuGetFreq(uint32_t coreId, uint64_t *cpu_clk_freq)
{
    *cpu_clk_freq = SBL_RcmGetCR5Freq();
}



#ifdef __cplusplus
#pragma FUNC_EXT_CALLED()
#pragma FUNC_CANNOT_INLINE()
#else
#pragma FUNC_EXT_CALLED(SBL_printProfileLog)
#pragma FUNC_CANNOT_INLINE(SBL_printProfileLog)
#endif

void SBL_printProfileLog(void)
{
#ifdef SBL_DISPLAY_PROFILE_INFO
    uint64_t mcu_clk_freq;
    uint32_t i = 0, prev_cycle_cnt = 0, cycles_per_usec;
    uint32_t lastlogIndx;

    SBL_ADD_PROFILE_POINT;

    SBL_cpuGetFreq(SBL_CLK_ID_MCU1_CPU0, &mcu_clk_freq);
    cycles_per_usec = (mcu_clk_freq / 1000000);

    SBL_log(SBL_LOG_ERR, "MCU @ %dHz.\n", uint64_to_int32(mcu_clk_freq));
    SBL_log(SBL_LOG_ERR, "cycles per usec  = %d\n", cycles_per_usec);

    lastlogIndx = sblProfileLogIndx;

    if (sblProfileLogOvrFlw)
    {
        i = sblProfileLogIndx;
        prev_cycle_cnt = sblProfileLog[i].cycle_cnt;
        lastlogIndx = MAX_PROFILE_LOG_ENTRIES;
        SBL_log(SBL_LOG_ERR, "Detected overflow, some profile entries might be lost.\n");
        SBL_log(SBL_LOG_ERR, "Rebuild with a larger vlaue of MAX_PROFILE_LOG_ENTRIES ??\n");
    }

    while((i % MAX_PROFILE_LOG_ENTRIES) < lastlogIndx)
    {
        uint32_t cycles_to_us;

        if (sblProfileLog[i].cycle_cnt < prev_cycle_cnt)
        {
            SBL_log(SBL_LOG_MAX,"**");
        }
        else
        {
            SBL_log(SBL_LOG_MAX,"  ");
        }
        cycles_to_us = sblProfileLog[i].cycle_cnt/cycles_per_usec;
        SBL_log(SBL_LOG_ERR,"fxn:%32s\t", sblProfileLog[i].fxn);
        SBL_log(SBL_LOG_ERR,"line:%4d\t", sblProfileLog[i].line);
        SBL_log(SBL_LOG_ERR,"cycle:%10d\t", sblProfileLog[i].cycle_cnt);
        SBL_log(SBL_LOG_ERR,"timestamp:%10dus\n", cycles_to_us);
        prev_cycle_cnt = sblProfileLog[i].cycle_cnt;
        i++;
    }
#else
    if (sblProfileLogOvrFlw)
    {
    }
#endif
}

void SBL_DCacheClean(void *addr, uint32_t size)
{
    uint32_t set = 0, way = 0;
    uint32_t cacheLineSize = CSL_armR5CacheGetDcacheLineSize();
    uintptr_t firstAddr = (uintptr_t ) addr & ~(cacheLineSize -1U);
    uintptr_t lastAddr = (uintptr_t) addr + size;
    uintptr_t num_iterations = (lastAddr - firstAddr) / cacheLineSize;

    if (num_iterations < 64 * 4)
    {
        /* Invalidate by MVA */
        CSL_armR5CacheWbInv((const void *)addr, uint32_to_int32(size));
    }
    else
    {
        /* Invalidating full cache by set and way is more efficient */
        for (set = 0; set < 128; set ++)
        {
            for (way = 0; way < 4; way++)
            {
                CSL_armR5CacheCleanInvalidateDcacheSetWay(set, way);
            }
        }
    }

}




/* Keep all boot media read offsets and read sizes */
/* word aligned throughout the function in case    */
/* boot media cannot handle byte accesses          */
int32_t SBL_VerifyMulticoreImage(void **img_handle,
                                 uint32_t *ImageOffsetPtr,
                                 uint8_t *scratch_mem,
                                 uint32_t scratch_sz)
{
    /* X509 Certificate parsing is TBD */
    return E_PASS;
}


/**********************************************************************
 ******************* SoC Specific Initilization ***********************
 **********************************************************************/

void SBL_SocEarlyInit(uint32_t isBuildHs)
{
    //SetupLvCmosDriveStrength();


}

void SBL_SocLateInit(void)
{
    //EnableThermalMaxTempAlert();

    //SBL_ADD_PROFILE_POINT;

    //SetupPmicAvs(SBL_OPP_NOM);

    //SBL_ADD_PROFILE_POINT;

}


#ifdef __cplusplus
#pragma RETAIN
#pragma LOCATION(SBL_SCRATCH_MEM_START);
#pragma DATA_SECTION(".sblScratch");
#else
#pragma RETAIN(gSblScratch);
#pragma LOCATION(gSblScratch,SBL_SCRATCH_MEM_START);
#pragma DATA_SECTION(gSblScratch, ".sblScratch");
#endif
uint8_t gSblScratch[SBL_SCRATCH_MEM_SIZE];

