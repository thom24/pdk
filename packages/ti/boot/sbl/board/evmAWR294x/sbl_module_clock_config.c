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

#include <ti/osal/DebugP.h>
#include "sbl_soc.h"
#include "sbl_soc_cfg.h"
#include "sbl_rcm.h"


/**
 * @brief
 *  Peripheral Clock config
 *
 */
typedef struct Sbl_ModuleClockConfig_s
{
    Rcm_PeripheralId peripheralId;
    Rcm_PeripheralClockSource clkSource;
    uint32_t freqHz;
} Sbl_ModuleClockConfig;

static const Sbl_ModuleClockConfig SBLModuleClockCfgTbl[] =
{
    //HW_WR_REG32(CSL_MSS_RCM_U_BASE+MSS_MCANA_CLK_DIV_VAL, 0x444);  //MSS_MCANA_CLK_DIV_VAL_CLKDIVR   = 0x444; 400/5
    //HW_WR_REG32(CSL_MSS_RCM_U_BASE+MSS_MCANA_CLK_SRC_SEL, 0x444);  //MSS_MCANA_CLK_SRC_SEL_CLKSRCSEL = 0x222;
    {
        .peripheralId = Rcm_PeripheralId_MSS_MCANA,
        .clkSource    = Rcm_PeripheralClockSource_DPLL_CORE_HSDIV0_CLKOUT2,
        .freqHz       = SBL_MCANA_FREQ_HZ,
    },
    //HW_WR_REG32(CSL_MSS_RCM_U_BASE+MSS_MCANB_CLK_DIV_VAL, 0x444);  //MSS_MCANB_CLK_DIV_VAL_CLKDIVR   = 0x444;
    //HW_WR_REG32(CSL_MSS_RCM_U_BASE+MSS_MCANB_CLK_SRC_SEL, 0x444);  //MSS_MCANB_CLK_SRC_SEL_CLKSRCSEL = 0x222;
    {
        .peripheralId = Rcm_PeripheralId_MSS_MCANB,
        .clkSource    = Rcm_PeripheralClockSource_DPLL_CORE_HSDIV0_CLKOUT2,
        .freqHz       = SBL_MCANB_FREQ_HZ,
    },
    //HW_WR_REG32(CSL_MSS_RCM_U_BASE+MSS_RTIA_CLK_DIV_VAL, 0x000);  //MSS_RTIA_CLK_DIV_VAL_CLKDIVR   = 0x444; 200/1
    //HW_WR_REG32(CSL_MSS_RCM_U_BASE+MSS_RTIA_CLK_SRC_SEL, 0x222);  //MSS_RTIA_CLK_SRC_SEL_CLKSRCSEL = 0x222;
    {
        .peripheralId = Rcm_PeripheralId_MSS_RTIA,
        .clkSource    = Rcm_PeripheralClockSource_SYS_CLK,
        .freqHz       = SBL_MSS_RTIA_FREQ_HZ,
    },
    //HW_WR_REG32(CSL_MSS_RCM_U_BASE+MSS_RTIB_CLK_DIV_VAL, 0x000);  //MSS_RTIB_CLK_DIV_VAL_CLKDIVR   = 0x444;
    //HW_WR_REG32(CSL_MSS_RCM_U_BASE+MSS_RTIB_CLK_SRC_SEL, 0x222);  //MSS_RTIB_CLK_SRC_SEL_CLKSRCSEL = 0x222;
    {
        .peripheralId = Rcm_PeripheralId_MSS_RTIB,
        .clkSource    = Rcm_PeripheralClockSource_SYS_CLK,
        .freqHz       = SBL_MSS_RTIB_FREQ_HZ,
    },
    //HW_WR_REG32(CSL_MSS_RCM_U_BASE+MSS_RTIC_CLK_DIV_VAL, 0x000);  //MSS_RTIC_CLK_DIV_VAL_CLKDIVR   = 0x444;
    //HW_WR_REG32(CSL_MSS_RCM_U_BASE+MSS_RTIC_CLK_SRC_SEL, 0x222);  //MSS_RTIC_CLK_SRC_SEL_CLKSRCSEL = 0x222;
    {
        .peripheralId = Rcm_PeripheralId_MSS_RTIC,
        .clkSource    = Rcm_PeripheralClockSource_SYS_CLK,
        .freqHz       = SBL_MSS_RTIC_FREQ_HZ,
    },
    //HW_WR_REG32(CSL_MSS_RCM_U_BASE+MSS_WDT_CLK_DIV_VAL, 0x000);  //MSS_WDT_CLK_DIV_VAL_CLKDIVR   = 0x444;
    //HW_WR_REG32(CSL_MSS_RCM_U_BASE+MSS_WDT_CLK_SRC_SEL, 0x222);  //MSS_WDT_CLK_SRC_SEL_CLKSRCSEL = 0x222;
    {
        .peripheralId = Rcm_PeripheralId_MSS_WDT,
        .clkSource    = Rcm_PeripheralClockSource_SYS_CLK,
        .freqHz       = SBL_MSS_WDT_FREQ_HZ,
    },
    //HW_WR_REG32(CSL_MSS_RCM_U_BASE+MSS_QSPI_CLK_DIV_VAL, 0x444);  //MSS_QSPI_CLK_DIV_VAL_CLKDIVR   = 0x444; 400/5
    //HW_WR_REG32(CSL_MSS_RCM_U_BASE+MSS_QSPI_CLK_SRC_SEL, 0x444);  //MSS_QSPI_CLK_SRC_SEL_CLKSRCSEL = 0x444;
    {
        .peripheralId = Rcm_PeripheralId_MSS_QSPI,
        .clkSource    = Rcm_PeripheralClockSource_DPLL_CORE_HSDIV0_CLKOUT2,
        .freqHz       = SBL_QSPI_FREQ_HZ,
    },
    //HW_WR_REG32(CSL_MSS_RCM_U_BASE+MSS_SPIA_CLK_DIV_VAL, 0x000);  //MSS_SPIA_CLK_DIV_VAL_CLKDIVR = 0x000;   200/1
    //HW_WR_REG32(CSL_MSS_RCM_U_BASE+MSS_SPIA_CLK_SRC_SEL, 0x222);  //MSS_SPIA_CLK_SRC_SEL_CLKSRCSEL = 0x222;
    {
        .peripheralId = Rcm_PeripheralId_MSS_SPIA,
        .clkSource    = Rcm_PeripheralClockSource_SYS_CLK,
        .freqHz       = SBL_MSS_SPIA_FREQ_HZ,
    },
    //HW_WR_REG32(CSL_MSS_RCM_U_BASE+MSS_SPIB_CLK_SRC_SEL, 0x222);  //MSS_SPIB_CLK_SRC_SEL_CLKSRCSEL = 0x222;
    //HW_WR_REG32(CSL_MSS_RCM_U_BASE+MSS_SPIB_CLK_DIV_VAL, 0x000);  //MSS_SPIB_CLK_DIV_VAL_CLKDIVR = 0x000;
    {
        .peripheralId = Rcm_PeripheralId_MSS_SPIB,
        .clkSource    = Rcm_PeripheralClockSource_SYS_CLK,
        .freqHz       = SBL_MSS_SPIB_FREQ_HZ,
    },
    //HW_WR_REG32(CSL_MSS_RCM_U_BASE+MSS_I2C_CLK_DIV_VAL, 0x000);  //MSS_I2C_CLK_DIV_VAL_CLKDIVR   = 0x000; 200/1
    //HW_WR_REG32(CSL_MSS_RCM_U_BASE+MSS_I2C_CLK_SRC_SEL, 0x222);  //MSS_I2C_CLK_SRC_SEL_CLKSRCSEL = 0x222;
    {
        .peripheralId = Rcm_PeripheralId_MSS_I2C,
        .clkSource    = Rcm_PeripheralClockSource_SYS_CLK,
        .freqHz       = SBL_MSS_I2C_FREQ_HZ,
    },
    //HW_WR_REG32(CSL_MSS_RCM_U_BASE+MSS_SCIA_CLK_DIV_VAL, 0x000);  //MSS_SCIA_CLK_DIV_VAL_CLKDIVR = 0x000; 200/1
    //HW_WR_REG32(CSL_MSS_RCM_U_BASE+MSS_SCIA_CLK_SRC_SEL, 0x222);  //MSS_SCIA_CLK_SRC_SEL_CLKSRCSEL = 0x222;
    {
        .peripheralId = Rcm_PeripheralId_MSS_SCIA,
        .clkSource    = Rcm_PeripheralClockSource_SYS_CLK,
        .freqHz       = SBL_MSS_SCIA_FREQ_HZ,
    },
    //HW_WR_REG32(CSL_MSS_RCM_U_BASE+MSS_SCIB_CLK_DIV_VAL, 0x000);  //MSS_SCIB_CLK_DIV_VAL_CLKDIVR = 0x000;
    //HW_WR_REG32(CSL_MSS_RCM_U_BASE+MSS_SCIB_CLK_SRC_SEL, 0x222);  //MSS_SCIB_CLK_SRC_SEL_CLKSRCSEL = 0x222;
    {
        .peripheralId = Rcm_PeripheralId_MSS_SCIB,
        .clkSource    = Rcm_PeripheralClockSource_SYS_CLK,
        .freqHz       = SBL_MSS_SCIB_FREQ_HZ,
    },
    //HW_WR_REG32(CSL_MSS_RCM_U_BASE+MSS_CPSW_CLK_DIV_VAL, 0x111);  //MSS_CPSW_CLK_DIV_VAL_CLKDIVR   = 0x000; 400/2
    //HW_WR_REG32(CSL_MSS_RCM_U_BASE+MSS_CPSW_CLK_SRC_SEL, 0x444);  //MSS_CPSW_CLK_SRC_SEL_CLKSRCSEL = 0x222;
    {
        .peripheralId = Rcm_PeripheralId_MSS_CPSW,
        .clkSource    = Rcm_PeripheralClockSource_DPLL_CORE_HSDIV0_CLKOUT2,
        .freqHz       = SBL_CPSW_FREQ_HZ,
    },
    //HW_WR_REG32(CSL_MSS_RCM_U_BASE+MSS_CPTS_CLK_DIV_VAL, 0x111);  //MSS_CPTS_CLK_DIV_VAL_CLKDIVR   = 0x000; 400/2
    //HW_WR_REG32(CSL_MSS_RCM_U_BASE+MSS_CPTS_CLK_SRC_SEL, 0x444);  //MSS_CPTS_CLK_SRC_SEL_CLKSRCSEL = 0x222;
    {
        .peripheralId = Rcm_PeripheralId_MSS_CPTS,
        .clkSource    = Rcm_PeripheralClockSource_DPLL_CORE_HSDIV0_CLKOUT2,
        .freqHz       = SBL_CPTS_FREQ_HZ,
    },
    //HW_WR_REG32(CSL_DSS_RCM_U_BASE+DSS_RTIA_CLK_DIV_VAL, 0x000);  //DSS_RTIA_CLK_DIV_VAL_CLKDIVR   = 0x444; 200/1
    //HW_WR_REG32(CSL_DSS_RCM_U_BASE+DSS_RTIA_CLK_SRC_SEL, 0x222);  //DSS_RTIA_CLK_SRC_SEL_CLKSRCSEL = 0x222;
    {
        .peripheralId = Rcm_PeripheralId_DSS_RTIA,
        .clkSource    = Rcm_PeripheralClockSource_SYS_CLK,
        .freqHz       = SBL_DSS_RTIA_FREQ_HZ,
    },
    //HW_WR_REG32(CSL_DSS_RCM_U_BASE+DSS_RTIB_CLK_DIV_VAL, 0x000);  //DSS_RTIB_CLK_DIV_VAL_CLKDIVR   = 0x444;
    //HW_WR_REG32(CSL_DSS_RCM_U_BASE+DSS_RTIB_CLK_SRC_SEL, 0x222);  //DSS_RTIB_CLK_SRC_SEL_CLKSRCSEL = 0x222;
    {
        .peripheralId = Rcm_PeripheralId_DSS_RTIB,
        .clkSource    = Rcm_PeripheralClockSource_SYS_CLK,
        .freqHz       = SBL_DSS_RTIB_FREQ_HZ,
    },
    //HW_WR_REG32(CSL_DSS_RCM_U_BASE+DSS_WDT_CLK_DIV_VAL, 0x000);  //DSS_WDT_CLK_DIV_VAL_CLKDIVR   = 0x444;
    //HW_WR_REG32(CSL_DSS_RCM_U_BASE+DSS_WDT_CLK_SRC_SEL, 0x222);  //DSS_WDT_CLK_SRC_SEL_CLKSRCSEL = 0x222;
    {
        .peripheralId = Rcm_PeripheralId_DSS_WDT,
        .clkSource    = Rcm_PeripheralClockSource_SYS_CLK,
        .freqHz       = SBL_DSS_WDT_FREQ_HZ,
    },
    //HW_WR_REG32(CSL_DSS_RCM_U_BASE+DSS_SCIA_CLK_DIV_VAL, 0x000);  //DSS_SCIA_CLK_DIV_VAL_CLKDIVR = 0x000; 200/1
    //HW_WR_REG32(CSL_DSS_RCM_U_BASE+DSS_SCIA_CLK_SRC_SEL, 0x222);  //DSS_SCIA_CLK_SRC_SEL_CLKSRCSEL = 0x222;
    {
        .peripheralId = Rcm_PeripheralId_DSS_SCIA,
        .clkSource    = Rcm_PeripheralClockSource_SYS_CLK,
        .freqHz       = SBL_DSS_SCIA_FREQ_HZ,
    },
};


/**
 * \brief clock Initialization function
 *
 * Enables different power domains and peripheral clocks of the SoC.
 * Some of the power domains and peripherals will be OFF by default.
 * Enabling the power domains is mandatory before accessing using
 * board interfaces connected to those peripherals.
 *
 * \return  BOARD_SOK              - Clock initialization sucessful.
 *          BOARD_INIT_CLOCK_FAIL  - Clock initialization failed.
 *
 */
void SBL_moduleClockInit(void)
{
    Rcm_Return retVal;
    uint32_t i;

    for (i = 0;
         i < sizeof(SBLModuleClockCfgTbl)/sizeof(SBLModuleClockCfgTbl[0]);
         i++)
    {
        retVal = SBL_RcmSetPeripheralClock(SBLModuleClockCfgTbl[i].peripheralId,
                                           SBLModuleClockCfgTbl[i].clkSource,
                                           SBLModuleClockCfgTbl[i].freqHz);
        DebugP_assert(retVal == Rcm_Return_SUCCESS);
    }
}
