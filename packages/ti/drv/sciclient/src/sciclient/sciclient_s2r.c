/*
 *  Copyright (c) Texas Instruments Incorporated 2023
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * \file sciclient_s2r.c
 *
 * \brief This file contains the code which load DDR retention and PMIC S2R code
 * into SRAM
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <string.h>
#include <ti/csl/arch/csl_arch.h>
#include <ti/board/board.h>
#if defined(SOC_J7200)
#include <ti/drv/lpm/src/s2r/j7200_evm/lpm_s2r.h>
#include <ti/board/src/j7200_evm/include/board_internal.h>
#include <ti/csl/soc/j7200/src/cslr_soc.h>
#endif
#include <ti/drv/sciclient/src/sciclient/sciclient_s2r.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/**
 * This address belongs to the space reserved from the DM firmware for
 * the point of view of Linux, U-boot SPL must use the same address
 * to restore BL31 ATF and resume entry point address.
 */
#define SCLICLIENT_ATF_BL31_DDR_RESTORE_ADDRESS 0xA5000000

/**
 * This is actually the biggest possible size of BL31 ATF. This value is defined
 * in ATF.
 */
#define SCICLIENT_ATF_BL31_CODE_SIZE 0x20000

/**
 * Address in BT SRAM where to load the code that will take care of the
 * DDR retention and PMIC S2R configuration.
 */
#define SCICLIENT_S2R_SRAM_CODE_ADDRESS 0x41011000

/* Address to point the sp register in SRAM */
#define SCICLIENT_S2R_SRAM_STACK_POINTER 0x41014000

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

uint32_t lpm_sram_s2r[] = LPM_SRAM_S2R;

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

void S2R_goRetention(uint32_t coreResumeAddr)
{
    CSL_REG32_WR(SCLICLIENT_ATF_BL31_DDR_RESTORE_ADDRESS + SCICLIENT_ATF_BL31_CODE_SIZE,
                 coreResumeAddr);
    S2R_debugPrintf("Resume address stored = 0x%x\n", coreResumeAddr);

    /* load DDR retention code and PMIC S2R into SRAM */
    memcpy((void*)SCICLIENT_S2R_SRAM_CODE_ADDRESS,
           (void*)&lpm_sram_s2r[0],
           LPM_SRAM_S2R_SIZE_IN_BYTES);

    S2R_debugPrintf("Suspending\n");

    asm volatile("mov sp, %0" : : "r" (SCICLIENT_S2R_SRAM_STACK_POINTER));
    asm volatile("blx %0" : : "r" (SCICLIENT_S2R_SRAM_CODE_ADDRESS));

    /* Never reach this point */
}
