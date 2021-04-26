/**
 *  \file   sbl_slave_core_boot.c
 *
 *  \brief  This file contain functions related to slave core boot-up.
 *
 */

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

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdint.h>
#include <string.h>
#include <ti/csl/csl_types.h>
#include <ti/csl/cslr_device.h>
#include <ti/csl/hw_types.h>
#include <ti/csl/arch/csl_arch.h>
#include <ti/csl/soc.h>
#include <ti/drv/uart/UART_stdio.h>

#include "sbl_soc.h"
#include "sbl_log.h"
#include "sbl_soc_cfg.h"
#include "sbl_profile.h"
#include "sbl_err_trap.h"
#include "sbl_slave_core_boot.h"
#include "sbl_rcm.h"
#include "sbl_utils_addrxlate.h"

#if defined(BOOT_UART)
#include "sbl_uart.h"
#endif

#if defined(BOOT_QSPI)
#include "sbl_qspi.h"
#endif

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */
#define SBL_DISABLE_MCU_LOCKSTEP    (0)
#define SBL_ENABLE_MCU_LOCKSTEP     (1)

/* Entry address for DSP1 applications in the memory region where trampolene is located */
#define DSP_ENTRY_ADDR         (CSL_DSP_L2_U_BASE)

/*
 * DSP instructions to be copied to the Trampolene to avoid the alignment
 * requirement.
 */
uint32_t dspInstruction[10] =
{
    0x0500002a, /* MVK.S2  destAddr, B10      */
    0x0500006a, /* MVKH.S2 destAddr, B10      */
    0x00280362, /* B.S2    B10                */
    0x00006000, /* NOP     4                  */
    0x00000000, /* NOP                        */
    0x00000000  /* NOP                        */
};

/* ========================================================================== */
/*                           Internal Functions                               */
/* ========================================================================== */
static void SBL_c66xWakeup(void)
{
    CSL_dss_rcmRegs *dssRcmRegs = (CSL_dss_rcmRegs *)CSL_DSS_RCM_U_BASE;
    Rcm_Return retVal;

    /* GEL reset RST CTRL before DSS PLL config. Changing order here. Revert if not functional */
    /* WR_MEM_32(CSL_DSS_RCM_U_BASE+CSL_DSS_RCM_DSS_DSP_RST_CTRL, 0x0); //DSS_DSP_RST_CTRL */
    //HW_WR_REG32(CSL_DSS_RCM_U_BASE+DSS_DSP_CLK_DIV_VAL, 0x000);  //DSS_WDT_CLK_DIV_VAL_CLKDIVR   = 0x000;  450/1
    //HW_WR_REG32(CSL_DSS_RCM_U_BASE+DSS_DSP_CLK_SRC_SEL, 0x222);  //DSS_WDT_CLK_SRC_SEL_CLKSRCSEL = 0x222;
    retVal = SBL_RcmSetDspCoreClock(Rcm_DSPClockSource_DPLL_DSP_HSDIV0_CLKOUT1, SBL_DSP1_C66X_FREQ_HZ);
    DebugP_assert(retVal == Rcm_Return_SUCCESS);

    CSL_FINS(dssRcmRegs->DSS_DSP_RST_CTRL, DSS_RCM_DSS_DSP_RST_CTRL_DSS_DSP_RST_CTRL_ASSERT_POR, 0x0);
    CSL_FINS(dssRcmRegs->DSS_DSP_RST_CTRL, DSS_RCM_DSS_DSP_RST_CTRL_DSS_DSP_RST_CTRL_ASSERT_GLOBAL, 0x0);
    CSL_FINS(dssRcmRegs->DSS_DSP_RST_CTRL, DSS_RCM_DSS_DSP_RST_CTRL_DSS_DSP_RST_CTRL_ASSERT_LOCAL, 0x0);

    CSL_FINS(dssRcmRegs->DSS_DSP_CLK_GATE, DSS_RCM_DSS_DSP_CLK_GATE_DSS_DSP_CLK_GATE_GATED, 0x0);

    CSL_FINS(dssRcmRegs->DSP_PD_WAKEUP_MASK0, DSS_RCM_DSP_PD_WAKEUP_MASK0_DSP_PD_WAKEUP_MASK0_WAKEUP_MASK0, 0xFFFEFFFF);
    CSL_FINS(dssRcmRegs->DSP_PD_TRIGGER_WAKUP,DSS_RCM_DSP_PD_TRIGGER_WAKUP_DSP_PD_TRIGGER_WAKUP_WAKEUP_TRIGGER, 0x1);

    while((CSL_FEXT(dssRcmRegs->DSP_PD_STATUS, DSS_RCM_DSP_PD_STATUS_DSP_PD_STATUS_PROC_HALTED) & 0x1) != 1U);
}

static void SBL_c66xStart(void)
{
    CSL_dss_rcmRegs *dssRcmRegs = (CSL_dss_rcmRegs *)CSL_DSS_RCM_U_BASE;

    CSL_FINS(dssRcmRegs->DSP_PD_CTRL, DSS_RCM_DSP_PD_CTRL_DSP_PD_CTRL_INTERRUPT_MASK, 0x1);
    CSL_FINS(dssRcmRegs->DSP_PD_CTRL, DSS_RCM_DSP_PD_CTRL_DSP_PD_CTRL_INTERRUPT_MASK, 0x0);
    CSL_FINS(dssRcmRegs->DSP_PD_CTRL, DSS_RCM_DSP_PD_CTRL_DSP_PD_CTRL_PROC_HALT, 0x0);
}

static void SBL_memInitc66x(void)
{
    SBL_RcmStartMeminitDSSL2(RCM_MEMINIT_DSSL2_MEMBANK_ALL);
    SBL_RcmStartMeminitDSSL3(RCM_MEMINIT_DSSL3_MEMBANK_ALL);
    SBL_RcmWaitMeminitDSSL2(RCM_MEMINIT_DSSL2_MEMBANK_ALL);
    SBL_RcmWaitMeminitDSSL3(RCM_MEMINIT_DSSL3_MEMBANK_ALL);
}

static void SBL_memInitTCMACR5(void)
{
    SBL_RcmStartMeminitTCMA();
    SBL_RcmWaitMeminitTCMA();
}

static void SBL_memInitTCMBCR5(void)
{
    SBL_RcmStartMeminitTCMB();
    SBL_RcmWaitMeminitTCMB();
}

#if defined (SOC_AWR294X)
static void SBL_BSSControl(void)
{
    SBL_RcmBSSControl();
}

static void SBL_PopulateBSSBootInfo(void)
{
    SBL_RcmPopulateBSSControl();
}

static void SBL_memInitTCMBSS(void)
{
    SBL_RcmStartMeminitTCMBSS();
    SBL_RcmWaitMeminitTCMBSS();
}

static void SBL_memInitStaticBSS(void)
{
    SBL_RcmStartMeminitStaticBSS();
    SBL_RcmWaitMeminitStaticBSS();
}

static void SBL_memInitSharedBSS(void)
{
    SBL_RcmStartMeminitSharedBSS();
    SBL_RcmWaitMeminitSharedBSS();
}
#endif /* defined (SOC_AWR294X) */

void SBL_cr5AUnhalt(CSL_mss_ctrlRegs *mssCtrl)
{
    //Core A unhalt
    CSL_FINS(mssCtrl->R5_COREA_HALT, MSS_CTRL_R5_COREA_HALT_R5_COREA_HALT_HALT, 0x0U);
}

void SBL_cr5BUnhalt(CSL_mss_ctrlRegs *mssCtrl)
{
    //Core B unhalt
    CSL_FINS(mssCtrl->R5_COREB_HALT, MSS_CTRL_R5_COREB_HALT_R5_COREB_HALT_HALT, 0x0);
}

void SBL_localR5CoreTriggerReset(void)
{
    CSL_mss_ctrlRegs *mssCtrl = (CSL_mss_ctrlRegs *)CSL_MSS_CTRL_U_BASE;
    CSL_FINS(mssCtrl->R5_CONTROL, MSS_CTRL_R5_CONTROL_R5_CONTROL_RESET_FSM_TRIGGER, 0x7);
    asm(" wfi ");
}


static void SBL_dualCoreBoot(CSL_mss_ctrlRegs *mssCtrl, CSL_mss_rcmRegs *rcmRegs)
{
    uint32_t regVal;

    regVal = rcmRegs->RST2ASSERTDLY;
    CSL_FINS(regVal, MSS_RCM_RST2ASSERTDLY_RST2ASSERTDLY_R5SSA, 0x0);
    CSL_FINS(regVal, MSS_RCM_RST2ASSERTDLY_RST2ASSERTDLY_R5SSB, 0x0);
    CSL_FINS(regVal, MSS_RCM_RST2ASSERTDLY_RST2ASSERTDLY_R5A, 0x0);
    CSL_FINS(regVal, MSS_RCM_RST2ASSERTDLY_RST2ASSERTDLY_R5B, 0x0);
    rcmRegs->RST2ASSERTDLY = regVal;

    regVal = rcmRegs->RST_WFICHECK;
    CSL_FINS(regVal, MSS_RCM_RST_WFICHECK_RST_WFICHECK_R5SSA, 0x7);
    CSL_FINS(regVal, MSS_RCM_RST_WFICHECK_RST_WFICHECK_R5SSB, 0x7);
    CSL_FINS(regVal, MSS_RCM_RST_WFICHECK_RST_WFICHECK_R5A, 0x7);
    CSL_FINS(regVal, MSS_RCM_RST_WFICHECK_RST_WFICHECK_R5B, 0x7);
    rcmRegs->RST_WFICHECK = regVal;

    regVal = mssCtrl->R5_CONTROL;
    CSL_FINS(regVal, MSS_CTRL_R5_CONTROL_R5_CONTROL_LOCK_STEP, 0x0);
    CSL_FINS(regVal, MSS_CTRL_R5_CONTROL_R5_CONTROL_LOCK_STEP_SWITCH_WAIT, 0x7);
    mssCtrl->R5_CONTROL = regVal;


}

static void SBL_lockStepBoot(CSL_mss_ctrlRegs *mssCtrl, CSL_mss_rcmRegs *rcmRegs)
{
    uint32_t regVal;

    /* WR_MEM_32(MSS_RCM_U_BASE+RST2ASSERTDLY, 0x0);  */ //RST2ASSERTDLY
    regVal = rcmRegs->RST2ASSERTDLY;
    CSL_FINS(regVal, MSS_RCM_RST2ASSERTDLY_RST2ASSERTDLY_R5SSA, 0x0);
    CSL_FINS(regVal, MSS_RCM_RST2ASSERTDLY_RST2ASSERTDLY_R5SSB, 0x0);
    CSL_FINS(regVal, MSS_RCM_RST2ASSERTDLY_RST2ASSERTDLY_R5A, 0x0);
    CSL_FINS(regVal, MSS_RCM_RST2ASSERTDLY_RST2ASSERTDLY_R5B, 0x0);
    rcmRegs->RST2ASSERTDLY = regVal;

    /* WR_MEM_32(MSS_RCM_U_BASE+RST_WFICHECK, 0x00000707); */ //RSTWFI CHECK
    regVal = rcmRegs->RST_WFICHECK;
    CSL_FINS(regVal, MSS_RCM_RST_WFICHECK_RST_WFICHECK_R5SSA, 0x7);
    CSL_FINS(regVal, MSS_RCM_RST_WFICHECK_RST_WFICHECK_R5SSB, 0x7);
    CSL_FINS(regVal, MSS_RCM_RST_WFICHECK_RST_WFICHECK_R5A, 0x7);
    CSL_FINS(regVal, MSS_RCM_RST_WFICHECK_RST_WFICHECK_R5B, 0x7);
    rcmRegs->RST_WFICHECK = regVal;

    regVal = mssCtrl->R5_CONTROL;
    CSL_FINS(regVal, MSS_CTRL_R5_CONTROL_R5_CONTROL_LOCK_STEP, 0x7);
    CSL_FINS(regVal, MSS_CTRL_R5_CONTROL_R5_CONTROL_LOCK_STEP_SWITCH_WAIT, 0x7);
    mssCtrl->R5_CONTROL = regVal;

}

static void SBL_ConfigMcuLockStep(uint8_t enableLockStep)
{

    SBL_ADD_PROFILE_POINT;


    if (enableLockStep)
    {
        SBL_log(SBL_LOG_MAX, "SBL_procBootSetProcessorCfg, ProcId 0x%x, enabling Lockstep mode...\n");
        SBL_lockStepBoot((CSL_mss_ctrlRegs *) CSL_MSS_CTRL_U_BASE , (CSL_mss_rcmRegs *)CSL_MSS_RCM_U_BASE);

    }
    else
    {
        SBL_log(SBL_LOG_MAX, "SBL_procBootSetProcessorCfg, ProcId 0x%x, enabling split mode...\n");
        SBL_dualCoreBoot((CSL_mss_ctrlRegs *) CSL_MSS_CTRL_U_BASE , (CSL_mss_rcmRegs *)CSL_MSS_RCM_U_BASE);
    }

    SBL_ADD_PROFILE_POINT;

    return;
}

int32_t SBL_ImageCopy(sblEntryPoint_t *pEntry)
{
    int32_t retval = 0;
    cpu_core_id_t core_id;

    SBL_ADD_PROFILE_POINT;

    /* Initialize the entry point array to 0. */
    for (core_id = SBL_FIRST_CORE_ID; core_id < NUM_CORES; core_id ++)
        pEntry->CpuEntryPoint[core_id] = SBL_INVALID_ENTRY_ADDR;

    SBL_ADD_PROFILE_POINT;

#if defined(BOOT_UART)
    if (SBL_UARTBootImage(pEntry) != E_PASS)
#elif defined(BOOT_QSPI)
    if (SBL_QSPIBootImage(pEntry) != E_PASS)
#endif
    {
        retval = E_FAIL;
    }

    SBL_ADD_PROFILE_POINT;

    return retval;
}

/**
 * \brief        SBL_SetupCoreMem function sets up the CPUs internal memory
 *
 * \param[in]    core_id - CPU ID
 * \param[in]    pAppEntry - Core info struct
 *
 * \return   none
 */
void SBL_SetupCoreMem(uint32_t core_id)
{
    uint8_t runLockStep = 0;

    SBL_ADD_PROFILE_POINT;

    /* Remap virtual core-ids if needed */
    switch (core_id)
    {
        case MCU1_SMP_ID:
            runLockStep = 1;
            core_id = MCU1_CPU0_ID;
            break;
        default:
            break;
    }

    if(runLockStep)
    {
        SBL_log(SBL_LOG_MAX, "Detected locktep for core_id %d\n", core_id);
        SBL_ConfigMcuLockStep(SBL_ENABLE_MCU_LOCKSTEP);
    }

    switch (core_id)
    {
        case DSP1_C66X_ID:
            SBL_log(SBL_LOG_MAX, "DSP Wakeup... \n");
            SBL_c66xWakeup();
            SBL_log(SBL_LOG_MAX, "DSP Meminit... \n");
            SBL_memInitc66x();
            break;
#if defined (SOC_AWR294X)
        case RSS1_R4_ID:
            SBL_log(SBL_LOG_MAX, "BSS Control : Allocate DSS L3 and FW development mode of operation... \n");
            SBL_BSSControl();
            SBL_log(SBL_LOG_MAX, "BSS TCM Meminit... \n");
            SBL_memInitTCMBSS();
            SBL_log(SBL_LOG_MAX, "BSS Static Meminit... \n");
            SBL_memInitStaticBSS();
            SBL_log(SBL_LOG_MAX, "BSS Shared memory Meminit... \n");
            SBL_memInitSharedBSS();
            break;
#endif /* defined (SOC_AWR294X) */
        case MCU1_CPU1_ID:
            SBL_log(SBL_LOG_MAX, "Switching core id %d to split mode... \n", core_id-1);
            /* Image for second MCU core present, disable lock step for the cluster */
            SBL_ConfigMcuLockStep(SBL_DISABLE_MCU_LOCKSTEP);
            /* DOnt break, fall through for enabling TCMs */
        case MCU1_CPU0_ID:
            SBL_memInitTCMACR5();
            SBL_memInitTCMBCR5();
            break;
        default:
            /* No special memory setup needed */
            break;
    }

    SBL_ADD_PROFILE_POINT;

    return;
}

/**
 * \brief    SBL_SlaveCoreBoot function sets the entry point, sets up clocks
 *           and enable to core to start executing from entry point.
 *
 * \param    core_id = Selects a core on the SOC, refer to cpu_core_id_t enum
 *           freqHz = Speed of core at boot up, 0 indicates use SBL default freqs.
 *           pAppEntry = SBL entry point struct
 *
 **/
void SBL_SlaveCoreBoot(cpu_core_id_t core_id, uint32_t freqHz, sblEntryPoint_t *pAppEntry)
{
    SBL_ADD_PROFILE_POINT;

#if defined(SBL_SKIP_MCU_RESET)
    /* Finished processing images for all cores, start MCU_0 */
    if ((core_id == MCU1_CPU1_ID) &&
        (pAppEntry->CpuEntryPoint[core_id] >=  SBL_INVALID_ENTRY_ADDR))
    {
            /* Display profile logs */
            SBL_printProfileLog();

            SBL_log(SBL_LOG_MAX, "Starting app, branching to 0x0 \n");
            /* Branch to start of ATCM */
            ((void(*)(void))0x0)();
    }
#endif

    /* Power down and then power up each core*/
    switch (core_id)
    {
        case MCU1_CPU1_ID:
            /* Display profile logs */
            SBL_printProfileLog();

#ifdef SBL_SKIP_MCU_RESET
            SBL_log(SBL_LOG_MAX, "Starting app, branching to 0x0 \n");
            /* Branch to start of ATCM */
            ((void(*)(void))0x0)();
#else

#endif
            break;

        case MCU1_CPU0_ID:

            break;
        case DSP1_C66X_ID:
            SBL_log(SBL_LOG_MAX, "SBL_procBootReleaseProcessor, ProcId 0x%x...\n", core_id);
            SBL_c66xStart();
            SBL_ADD_PROFILE_POINT;
            break;
#if defined (SOC_AWR294X)
        case RSS1_R4_ID:
            /* Populate the BSS Boot Info registers */
            /* BSS Unhalt will be done in the MSS Application */
            SBL_log(SBL_LOG_MAX, "Populate BSS Boot info registers\n");
            SBL_PopulateBSSBootInfo();
            SBL_ADD_PROFILE_POINT;
            break;
#endif /* defined (SOC_AWR294X) */
    }
}


cpu_core_id_t SBL_getLocalCoreId(void)
{
#ifndef BUILD_MCU1_0
#error "Self Core address assumes SBL runs on MCU1_0"
#endif
    return MCU1_CPU0_ID;
}

static void SBL_DspEntryPointSet(uint32_t entryPoint, uint32_t *pDspInstr)
{
    uint32_t entryVal = 0U;
    uint32_t dspOpcode = 0U;

    entryVal = (entryPoint & 0x0000FFFF);
    dspOpcode = *pDspInstr;
    /*
    ** Mask and update the lower 16 bits of entry address within the MVK
    ** instruction opcode.
    */
    *pDspInstr = (((dspOpcode) & ~(0x007FFF80)) | (( (entryVal) << 0x7) & 0x007FFF80));

    entryVal = ((entryPoint & 0xFFFF0000) >> 16);
    dspOpcode = *(pDspInstr + 1);
    /*
    ** Mask and update the upper 16 bits of entry address within the MVK
    ** instruction opcode.
    */
    *(pDspInstr + 1) = (((dspOpcode) & ~(0x007FFF80)) | (( (entryVal) << 0x7) & 0x007FFF80));
}

void SBL_SetDSPResetVec(uint8_t core, uint32_t entry)
{
    CSL_dss_ctrlRegs * ptrDSSCTRLRegs = (CSL_dss_ctrlRegs *)CSL_DSS_CTRL_U_BASE;
    /* TODO Local module reset assert */

    SBL_DspEntryPointSet(entry, dspInstruction);

    memcpy((void *) Utils_xlate2LocalAddr(DSP_ENTRY_ADDR, DSP1_C66X_ID), (void *)dspInstruction,
        sizeof(dspInstruction));

    CacheP_wbInv((void *) Utils_xlate2LocalAddr(DSP_ENTRY_ADDR, DSP1_C66X_ID), sizeof(dspInstruction));
    /* Set DSP boot address to trampoline for both Secure and NS mode */
    CSL_FINS(ptrDSSCTRLRegs->DSS_DSP_BOOTCFG , DSS_CTRL_DSS_DSP_BOOTCFG_DSS_DSP_BOOTCFG_ISTP_RST_VAL, (DSP_ENTRY_ADDR >> 10));

    /* TODO Local module reset deassert */
}
