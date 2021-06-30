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

static const Sbl_ModuleClockConfig SBLModuleClockCfgTbl_PkgTypeLop_postApllSwitch[] =
{
    {
        .peripheralId = Rcm_PeripheralId_MSS_MCANA,
        .clkSource    = Rcm_PeripheralClockSource_DPLL_DSP_HSDIV0_CLKOUT2,
        .freqHz       = SBL_MCANA_FREQ_HZ,
    },
    {
        .peripheralId = Rcm_PeripheralId_MSS_MCANB,
        .clkSource    = Rcm_PeripheralClockSource_DPLL_DSP_HSDIV0_CLKOUT2,
        .freqHz       = SBL_MCANB_FREQ_HZ,
    },
    {
        .peripheralId = Rcm_PeripheralId_MSS_RTIA,
        .clkSource    = Rcm_PeripheralClockSource_SYS_CLK,
        .freqHz       = SBL_MSS_RTIA_FREQ_HZ,
    },
    {
        .peripheralId = Rcm_PeripheralId_MSS_RTIB,
        .clkSource    = Rcm_PeripheralClockSource_SYS_CLK,
        .freqHz       = SBL_MSS_RTIB_FREQ_HZ,
    },
    {
        .peripheralId = Rcm_PeripheralId_MSS_RTIC,
        .clkSource    = Rcm_PeripheralClockSource_SYS_CLK,
        .freqHz       = SBL_MSS_RTIC_FREQ_HZ,
    },
    {
        .peripheralId = Rcm_PeripheralId_MSS_WDT,
        .clkSource    = Rcm_PeripheralClockSource_SYS_CLK,
        .freqHz       = SBL_MSS_WDT_FREQ_HZ,
    },
    {
        .peripheralId = Rcm_PeripheralId_MSS_QSPI,
        .clkSource    = Rcm_PeripheralClockSource_DPLL_DSP_HSDIV0_CLKOUT2,
        .freqHz       = SBL_QSPI_FREQ_HZ,
    },
    {
        .peripheralId = Rcm_PeripheralId_MSS_SPIA,
        .clkSource    = Rcm_PeripheralClockSource_SYS_CLK,
        .freqHz       = SBL_MSS_SPIA_FREQ_HZ,
    },
    {
        .peripheralId = Rcm_PeripheralId_MSS_SPIB,
        .clkSource    = Rcm_PeripheralClockSource_SYS_CLK,
        .freqHz       = SBL_MSS_SPIB_FREQ_HZ,
    },
    {
        .peripheralId = Rcm_PeripheralId_MSS_I2C,
        .clkSource    = Rcm_PeripheralClockSource_SYS_CLK,
        .freqHz       = SBL_MSS_I2C_FREQ_HZ,
    },
    {
        .peripheralId = Rcm_PeripheralId_MSS_SCIA,
        .clkSource    = Rcm_PeripheralClockSource_SYS_CLK,
        .freqHz       = SBL_MSS_SCIA_FREQ_HZ,
    },
    {
        .peripheralId = Rcm_PeripheralId_MSS_SCIB,
        .clkSource    = Rcm_PeripheralClockSource_SYS_CLK,
        .freqHz       = SBL_MSS_SCIB_FREQ_HZ,
    },
    {
        .peripheralId = Rcm_PeripheralId_MSS_CPSW,
        .clkSource    = Rcm_PeripheralClockSource_DPLL_PER_HSDIV0_CLKOUT1,
        .freqHz       = SBL_CPSW_FREQ_HZ,
    },
    {
        .peripheralId = Rcm_PeripheralId_DSS_RTIA,
        .clkSource    = Rcm_PeripheralClockSource_SYS_CLK,
        .freqHz       = SBL_DSS_RTIA_FREQ_HZ,
    },
    {
        .peripheralId = Rcm_PeripheralId_DSS_RTIB,
        .clkSource    = Rcm_PeripheralClockSource_SYS_CLK,
        .freqHz       = SBL_DSS_RTIB_FREQ_HZ,
    },
    {
        .peripheralId = Rcm_PeripheralId_DSS_WDT,
        .clkSource    = Rcm_PeripheralClockSource_SYS_CLK,
        .freqHz       = SBL_DSS_WDT_FREQ_HZ,
    },
    {
        .peripheralId = Rcm_PeripheralId_DSS_SCIA,
        .clkSource    = Rcm_PeripheralClockSource_SYS_CLK,
        .freqHz       = SBL_DSS_SCIA_FREQ_HZ,
    },
};

static const Sbl_ModuleClockConfig SBLModuleClockCfgTbl_PkgTypeEts_postApllSwitch[] =
{
    {
        .peripheralId = Rcm_PeripheralId_MSS_MCANA,
        .clkSource    = Rcm_PeripheralClockSource_DPLL_DSP_HSDIV0_CLKOUT2,
        .freqHz       = SBL_MCANA_FREQ_HZ,
    },
    {
        .peripheralId = Rcm_PeripheralId_MSS_MCANB,
        .clkSource    = Rcm_PeripheralClockSource_DPLL_DSP_HSDIV0_CLKOUT2,
        .freqHz       = SBL_MCANB_FREQ_HZ,
    },
    {
        .peripheralId = Rcm_PeripheralId_MSS_RTIA,
        .clkSource    = Rcm_PeripheralClockSource_SYS_CLK,
        .freqHz       = SBL_MSS_RTIA_SYS150_FREQ_HZ,
    },
    {
        .peripheralId = Rcm_PeripheralId_MSS_RTIB,
        .clkSource    = Rcm_PeripheralClockSource_SYS_CLK,
        .freqHz       = SBL_MSS_RTIB_SYS150_FREQ_HZ,
    },
    {
        .peripheralId = Rcm_PeripheralId_MSS_RTIC,
        .clkSource    = Rcm_PeripheralClockSource_SYS_CLK,
        .freqHz       = SBL_MSS_RTIC_SYS150_FREQ_HZ,
    },
    {
        .peripheralId = Rcm_PeripheralId_MSS_WDT,
        .clkSource    = Rcm_PeripheralClockSource_SYS_CLK,
        .freqHz       = SBL_MSS_WDT_SYS150_FREQ_HZ,
    },
    {
        .peripheralId = Rcm_PeripheralId_MSS_QSPI,
        .clkSource    = Rcm_PeripheralClockSource_DPLL_DSP_HSDIV0_CLKOUT2,
        .freqHz       = SBL_QSPI_FREQ_HZ,
    },
    {
        .peripheralId = Rcm_PeripheralId_MSS_SPIA,
        .clkSource    = Rcm_PeripheralClockSource_SYS_CLK,
        .freqHz       = SBL_MSS_SPIA_SYS150_FREQ_HZ,
    },
    {
        .peripheralId = Rcm_PeripheralId_MSS_SPIB,
        .clkSource    = Rcm_PeripheralClockSource_SYS_CLK,
        .freqHz       = SBL_MSS_SPIB_SYS150_FREQ_HZ,
    },
    {
        .peripheralId = Rcm_PeripheralId_MSS_I2C,
        .clkSource    = Rcm_PeripheralClockSource_SYS_CLK,
        .freqHz       = SBL_MSS_I2C_SYS150_FREQ_HZ,
    },
    {
        .peripheralId = Rcm_PeripheralId_MSS_SCIA,
        .clkSource    = Rcm_PeripheralClockSource_SYS_CLK,
        .freqHz       = SBL_MSS_SCIA_SYS150_FREQ_HZ,
    },
    {
        .peripheralId = Rcm_PeripheralId_MSS_SCIB,
        .clkSource    = Rcm_PeripheralClockSource_SYS_CLK,
        .freqHz       = SBL_MSS_SCIB_SYS150_FREQ_HZ,
    },
    {
        .peripheralId = Rcm_PeripheralId_MSS_CPSW,
        .clkSource    = Rcm_PeripheralClockSource_DPLL_PER_HSDIV0_CLKOUT1,
        .freqHz       = SBL_CPSW_FREQ_HZ,
    },
    {
        .peripheralId = Rcm_PeripheralId_DSS_RTIA,
        .clkSource    = Rcm_PeripheralClockSource_SYS_CLK,
        .freqHz       = SBL_DSS_RTIA_SYS150_FREQ_HZ,
    },
    {
        .peripheralId = Rcm_PeripheralId_DSS_RTIB,
        .clkSource    = Rcm_PeripheralClockSource_SYS_CLK,
        .freqHz       = SBL_DSS_RTIB_SYS150_FREQ_HZ,
    },
    {
        .peripheralId = Rcm_PeripheralId_DSS_WDT,
        .clkSource    = Rcm_PeripheralClockSource_SYS_CLK,
        .freqHz       = SBL_DSS_WDT_SYS150_FREQ_HZ,
    },
    {
        .peripheralId = Rcm_PeripheralId_DSS_SCIA,
        .clkSource    = Rcm_PeripheralClockSource_SYS_CLK,
        .freqHz       = SBL_DSS_SCIA_SYS150_FREQ_HZ,
    },
};

static const Sbl_ModuleClockConfig SBLModuleClockCfgTbl_SysClk200_preApllSwitch[] =
{
    {
        .peripheralId = Rcm_PeripheralId_MSS_MCANA,
        .clkSource    = Rcm_PeripheralClockSource_DPLL_CORE_HSDIV0_CLKOUT2,
        .freqHz       = SBL_MCANA_FREQ_HZ,
    },
    {
        .peripheralId = Rcm_PeripheralId_MSS_MCANB,
        .clkSource    = Rcm_PeripheralClockSource_DPLL_CORE_HSDIV0_CLKOUT2,
        .freqHz       = SBL_MCANB_FREQ_HZ,
    },
    {
        .peripheralId = Rcm_PeripheralId_MSS_RTIA,
        .clkSource    = Rcm_PeripheralClockSource_SYS_CLK,
        .freqHz       = SBL_MSS_RTIA_FREQ_HZ,
    },
    {
        .peripheralId = Rcm_PeripheralId_MSS_RTIB,
        .clkSource    = Rcm_PeripheralClockSource_SYS_CLK,
        .freqHz       = SBL_MSS_RTIB_FREQ_HZ,
    },
    {
        .peripheralId = Rcm_PeripheralId_MSS_RTIC,
        .clkSource    = Rcm_PeripheralClockSource_SYS_CLK,
        .freqHz       = SBL_MSS_RTIC_FREQ_HZ,
    },
    {
        .peripheralId = Rcm_PeripheralId_MSS_WDT,
        .clkSource    = Rcm_PeripheralClockSource_SYS_CLK,
        .freqHz       = SBL_MSS_WDT_FREQ_HZ,
    },
    {
        .peripheralId = Rcm_PeripheralId_MSS_QSPI,
        .clkSource    = Rcm_PeripheralClockSource_DPLL_CORE_HSDIV0_CLKOUT2,
        .freqHz       = SBL_QSPI_FREQ_HZ,
    },
    {
        .peripheralId = Rcm_PeripheralId_MSS_SPIA,
        .clkSource    = Rcm_PeripheralClockSource_SYS_CLK,
        .freqHz       = SBL_MSS_SPIA_FREQ_HZ,
    },
    {
        .peripheralId = Rcm_PeripheralId_MSS_SPIB,
        .clkSource    = Rcm_PeripheralClockSource_SYS_CLK,
        .freqHz       = SBL_MSS_SPIB_FREQ_HZ,
    },
    {
        .peripheralId = Rcm_PeripheralId_MSS_I2C,
        .clkSource    = Rcm_PeripheralClockSource_SYS_CLK,
        .freqHz       = SBL_MSS_I2C_FREQ_HZ,
    },
    {
        .peripheralId = Rcm_PeripheralId_MSS_SCIA,
        .clkSource    = Rcm_PeripheralClockSource_SYS_CLK,
        .freqHz       = SBL_MSS_SCIA_FREQ_HZ,
    },
    {
        .peripheralId = Rcm_PeripheralId_MSS_SCIB,
        .clkSource    = Rcm_PeripheralClockSource_SYS_CLK,
        .freqHz       = SBL_MSS_SCIB_FREQ_HZ,
    },
    {
        .peripheralId = Rcm_PeripheralId_MSS_CPSW,
        .clkSource    = Rcm_PeripheralClockSource_DPLL_CORE_HSDIV0_CLKOUT2,
        .freqHz       = SBL_CPSW_FREQ_HZ,
    },
    {
        .peripheralId = Rcm_PeripheralId_DSS_RTIA,
        .clkSource    = Rcm_PeripheralClockSource_SYS_CLK,
        .freqHz       = SBL_DSS_RTIA_FREQ_HZ,
    },
    {
        .peripheralId = Rcm_PeripheralId_DSS_RTIB,
        .clkSource    = Rcm_PeripheralClockSource_SYS_CLK,
        .freqHz       = SBL_DSS_RTIB_FREQ_HZ,
    },
    {
        .peripheralId = Rcm_PeripheralId_DSS_WDT,
        .clkSource    = Rcm_PeripheralClockSource_SYS_CLK,
        .freqHz       = SBL_DSS_WDT_FREQ_HZ,
    },
    {
        .peripheralId = Rcm_PeripheralId_DSS_SCIA,
        .clkSource    = Rcm_PeripheralClockSource_SYS_CLK,
        .freqHz       = SBL_DSS_SCIA_FREQ_HZ,
    },
};

static const Sbl_ModuleClockConfig SBLModuleClockCfgTbl_SysClk150_preApllSwitch[] =
{
    {
        .peripheralId = Rcm_PeripheralId_MSS_MCANA,
        .clkSource    = Rcm_PeripheralClockSource_DPLL_CORE_HSDIV0_CLKOUT2,
        .freqHz       = SBL_MCANA_SYS150_FREQ_HZ,
    },
    {
        .peripheralId = Rcm_PeripheralId_MSS_MCANB,
        .clkSource    = Rcm_PeripheralClockSource_DPLL_CORE_HSDIV0_CLKOUT2,
        .freqHz       = SBL_MCANB_SYS150_FREQ_HZ,
    },
    {
        .peripheralId = Rcm_PeripheralId_MSS_RTIA,
        .clkSource    = Rcm_PeripheralClockSource_SYS_CLK,
        .freqHz       = SBL_MSS_RTIA_SYS150_FREQ_HZ,
    },
    {
        .peripheralId = Rcm_PeripheralId_MSS_RTIB,
        .clkSource    = Rcm_PeripheralClockSource_SYS_CLK,
        .freqHz       = SBL_MSS_RTIB_SYS150_FREQ_HZ,
    },
    {
        .peripheralId = Rcm_PeripheralId_MSS_RTIC,
        .clkSource    = Rcm_PeripheralClockSource_SYS_CLK,
        .freqHz       = SBL_MSS_RTIC_SYS150_FREQ_HZ,
    },
    {
        .peripheralId = Rcm_PeripheralId_MSS_WDT,
        .clkSource    = Rcm_PeripheralClockSource_SYS_CLK,
        .freqHz       = SBL_MSS_WDT_SYS150_FREQ_HZ,
    },
    {
        .peripheralId = Rcm_PeripheralId_MSS_QSPI,
        .clkSource    = Rcm_PeripheralClockSource_DPLL_CORE_HSDIV0_CLKOUT2,
        .freqHz       = SBL_QSPI_SYS150_FREQ_HZ,
    },
    {
        .peripheralId = Rcm_PeripheralId_MSS_SPIA,
        .clkSource    = Rcm_PeripheralClockSource_SYS_CLK,
        .freqHz       = SBL_MSS_SPIA_SYS150_FREQ_HZ,
    },
    {
        .peripheralId = Rcm_PeripheralId_MSS_SPIB,
        .clkSource    = Rcm_PeripheralClockSource_SYS_CLK,
        .freqHz       = SBL_MSS_SPIB_SYS150_FREQ_HZ,
    },
    {
        .peripheralId = Rcm_PeripheralId_MSS_I2C,
        .clkSource    = Rcm_PeripheralClockSource_SYS_CLK,
        .freqHz       = SBL_MSS_I2C_SYS150_FREQ_HZ,
    },
    {
        .peripheralId = Rcm_PeripheralId_MSS_SCIA,
        .clkSource    = Rcm_PeripheralClockSource_SYS_CLK,
        .freqHz       = SBL_MSS_SCIA_SYS150_FREQ_HZ,
    },
    {
        .peripheralId = Rcm_PeripheralId_MSS_SCIB,
        .clkSource    = Rcm_PeripheralClockSource_SYS_CLK,
        .freqHz       = SBL_MSS_SCIB_SYS150_FREQ_HZ,
    },
    {
        .peripheralId = Rcm_PeripheralId_MSS_CPSW,
        .clkSource    = Rcm_PeripheralClockSource_DPLL_CORE_HSDIV0_CLKOUT2,
        .freqHz       = SBL_CPSW_SYS150_FREQ_HZ,
    },
    {
        .peripheralId = Rcm_PeripheralId_DSS_RTIA,
        .clkSource    = Rcm_PeripheralClockSource_SYS_CLK,
        .freqHz       = SBL_DSS_RTIA_SYS150_FREQ_HZ,
    },
    {
        .peripheralId = Rcm_PeripheralId_DSS_RTIB,
        .clkSource    = Rcm_PeripheralClockSource_SYS_CLK,
        .freqHz       = SBL_DSS_RTIB_SYS150_FREQ_HZ,
    },
    {
        .peripheralId = Rcm_PeripheralId_DSS_WDT,
        .clkSource    = Rcm_PeripheralClockSource_SYS_CLK,
        .freqHz       = SBL_DSS_WDT_SYS150_FREQ_HZ,
    },
    {
        .peripheralId = Rcm_PeripheralId_DSS_SCIA,
        .clkSource    = Rcm_PeripheralClockSource_SYS_CLK,
        .freqHz       = SBL_DSS_SCIA_SYS150_FREQ_HZ,
    },
};

#define SBL_UTILS_ARRAYSIZE(x)                  (sizeof(x)/sizeof(x[0]))

static void SBL_getModuleClockInitTbl(Rcm_ModuleClkInitStage initStage,
                                      const Sbl_ModuleClockConfig **tbl,
                                      uint32_t *numEntries)
{
    if (Rcm_ModuleClkInitStage_POST_APLL_SWITCH == initStage)
    {
        RcmEfusePkgType pkgType;

        SBL_RcmGetPackageType(&pkgType);
        if (pkgType ==RCM_EFUSE_DEVICE_PKG_TYPE_LOP)
        {
            *tbl = &SBLModuleClockCfgTbl_PkgTypeLop_postApllSwitch[0];
            *numEntries = SBL_UTILS_ARRAYSIZE(SBLModuleClockCfgTbl_PkgTypeLop_postApllSwitch);
        }
        else
        {
            *tbl = &SBLModuleClockCfgTbl_PkgTypeEts_postApllSwitch[0];
            *numEntries = SBL_UTILS_ARRAYSIZE(SBLModuleClockCfgTbl_PkgTypeEts_postApllSwitch);
        }
    }
    else
    {
        Rcm_EfuseBootFreqConfig bootFreqEfuseCfg;

        SBL_RcmGetEfuseBootFrequency(&bootFreqEfuseCfg);
        if (bootFreqEfuseCfg.sysClkFreqHz == SBL_FREQ_MHZ2HZ(200U))
        {
            *tbl = &SBLModuleClockCfgTbl_SysClk200_preApllSwitch[0];
            *numEntries = SBL_UTILS_ARRAYSIZE(SBLModuleClockCfgTbl_SysClk200_preApllSwitch);
        }
        else
        {
            *tbl = &SBLModuleClockCfgTbl_SysClk150_preApllSwitch[0];
            *numEntries = SBL_UTILS_ARRAYSIZE(SBLModuleClockCfgTbl_SysClk150_preApllSwitch);
        }
    }
}

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
void SBL_moduleClockInit(Rcm_ModuleClkInitStage initStage)
{
    Rcm_Return retVal;
    uint32_t i;
    const Sbl_ModuleClockConfig *tbl;
    uint32_t numEntries;

    SBL_getModuleClockInitTbl(initStage, &tbl, &numEntries);
    for (i = 0;  i < numEntries; i++)
    {
        retVal = SBL_RcmSetPeripheralClock(tbl[i].peripheralId,
                                           tbl[i].clkSource,
                                           tbl[i].freqHz);
        DebugP_assert(retVal == Rcm_Return_SUCCESS);
    }
}
