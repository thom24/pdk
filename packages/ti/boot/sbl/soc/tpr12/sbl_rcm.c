/*
 * drv_rcm
 *
 * RCM Driver source file
 *
 * This file implements the Reset and Clock control for R5 core and peripherals.
 *
 * Copyright (C) 2020 Texas Instruments Incorporated - http://www.ti.com/
 * ALL RIGHTS RESERVED
 *
 */

/**************************************************************************
 *************************** Include Files ********************************
 **************************************************************************/
#include "sbl_rcm.h"
#include <ti/csl/csl.h>
#include <ti/csl/soc.h>
#include <ti/osal/osal.h>
#include <ti/osal/DebugP.h>
#include "sbl_csl.h"
#include "sbl_soc_cfg.h"


/**************************************************************************
 ************************** Local Definitions *****************************
 **************************************************************************/
#define L3MEMCONFIG_START_BIT               (7U)
#define L3MEMCONFIG_END_BIT                 (10U)
#define FLASHMODECONFIG_START_BIT           (15U)
#define FLASHMODECONFIG_END_BIT             (16U)
#define QSPIFREQCONFIG_START_BIT            (17U)
#define QSPIFREQCONFIG_END_BIT              (18U)
#define BISTENABLECONFIG_START_BIT          (1U)
#define BISTENABLECONFIG_END_BIT            (9U)
#define COREADPLLTRIMCONFIG_START_BIT       (0U)
#define COREADPLLTRIMCONFIG_END_BIT         (11U)
#define DSPADPLLTRIMCONFIG_START_BIT        (12U)
#define DSPADPLLTRIMCONFIG_END_BIT          (23U)
#define PERADPLLTRIMCONFIG_START_BIT        (0U)
#define PERADPLLTRIMCONFIG_END_BIT          (11U)
#define COREADPLLTRIMVALIDCONFIG_START_BIT  (12U)
#define COREADPLLTRIMVALIDCONFIG_END_BIT    (12U)
#define DSPADPLLTRIMVALIDCONFIG_START_BIT   (13U)
#define DSPADPLLTRIMVALIDCONFIG_END_BIT     (13U)
#define PERADPLLTRIMVALIDCONFIG_START_BIT   (14U)
#define PERADPLLTRIMVALIDCONFIG_END_BIT     (14U)
#define PARTID_START_BIT                    (13U)
#define PARTID_END_BIT                      (24U)
#define PARTNUMBER_START_BIT                (0U)
#define PARTNUMBER_END_BIT                  (2U)
#define PGVERSION_START_BIT                 (0U)
#define PGVERSION_END_BIT                   (3U)
#define ROMVERSION_START_BIT                (4U)
#define ROMVERSION_END_BIT                  (6U)
#define METALVERSION_START_BIT              (7U)
#define METALVERSION_END_BIT                (9U)
#define XTAL_FREQ_SCALE_START_BIT           (25U)
#define XTAL_FREQ_SCALE_END_BIT             (25U)
 
#define RCM_CORE_ADPLL_DEFAULT_VALUE            (0x9U)
#define RCM_DSP_ADPLL_DEFAULT_VALUE             (0x9U)
#define RCM_PER_ADPLL_DEFAULT_VALUE             (0x9U)
#define RCM_PLL_SYS_CLK_FREQUENCY_HZ            (200000000U)
#define RCM_PLL_HSDIV_CLKOUT2_FREQUENCY_HZ      (400000000U)
#define RCM_XTAL_CLK_40MHZ                      (40000000U)
#define RCM_XTAL_CLK_50MHZ                      (50000000U)
#define RCM_XTAL_CLK_49p152MHZ                  (49152000U)
#define RCM_XTAL_CLK_45p1584MHZ                 (45158400U)
#define RCM_XTAL_CLK_20MHZ                      (20000000U)
#define RCM_XTAL_CLK_25MHZ                      (25000000U)
#define RCM_XTAL_CLK_24p576MHZ                  (24576000U)
#define RCM_XTAL_CLK_22p5792MHZ                 (22579200U)

#define SBL_UTILS_ARRAYSIZE(x)                  (sizeof(x)/sizeof(x[0]))

typedef enum RcmXtalFreqId_e
{
    RCM_XTAL_FREQID_CLK_40MHZ,
    RCM_XTAL_FREQID_CLK_50MHZ,
    RCM_XTAL_FREQID_CLK_49p152MHZ,
    RCM_XTAL_FREQID_CLK_45p1584MHZ,
    RCM_XTAL_FREQID_CLK_20MHZ,
    RCM_XTAL_FREQID_CLK_25MHZ,
    RCM_XTAL_FREQID_CLK_24p576MHZ,
    RCM_XTAL_FREQID_CLK_22p5792MHZ
} Rcm_XtalFreqId;

typedef enum RcmPLLId_e
{
    RCM_PLLID_CORE,
    RCM_PLLID_DSS,
    RCM_PLLID_PER,
    RCM_PLLID_XTALCLK,
    RCM_PLLID_WUCPUCLK,
    RCM_PLLID_RCCLK32K,
    RCM_PLLID_RCCLK10M,
} Rcm_PllId;

typedef enum RcmPllHSDIVOutId_e
{
    RCM_PLLHSDIV_OUT_0,
    RCM_PLLHSDIV_OUT_1,
    RCM_PLLHSDIV_OUT_2,
    RCM_PLLHSDIV_OUT_3,
    RCM_PLLHSDIV_OUT_NONE,
} Rcm_PllHSDIVOutId;

typedef struct Rcm_ClkSrcInfo_s
{
    Rcm_PllId pllId;
    Rcm_PllHSDIVOutId hsDivOut;
} Rcm_ClkSrcInfo;

typedef struct Rcm_XTALInfo_s
{
    uint32_t Finp;
    bool     div2flag;
} Rcm_XTALInfo;

typedef struct Rcm_ADPLLJConfig_s
{
    uint32_t N; /* Input Clock divider/Pre-divider (N) */
    uint32_t M2; /* Post divider (M2) */
    uint32_t M;  /* Multiplier integer (M) */
    uint32_t FracM; /* Multiplier fractional (M) */
    uint32_t Fout; /* Output frequency of PLL */
    uint32_t Finp; /* Output frequency of PLL */
} Rcm_ADPLLJConfig_t;


/* Table populated from TPR12_ADPLLJ_Settings_1p0.xlsx. 
 * Each entry corresponds to tab in the excel sheet
 */
const Rcm_ADPLLJConfig_t gADPLLJConfigTbl[] = 
{
    /* CORE_DSP_800_40MHz */
    {
        .Finp = 40U,
        .N = 19U,
        .Fout = 800U,
        .M2 = 1U,
        .M = 400U,
        .FracM = 0U,
    },
    /* CORE_DSP_2000_40MHz */
    {
        .Finp = 40U,
        .N = 39U,
        .Fout = 2000U,
        .M2 = 1U,
        .M = 2000U,
        .FracM = 0U,
    },
    /* CORE_DSP_2000_40MHz */
    {
        .Finp = 40U,
        .N = 19U,
        .Fout = 2000U,
        .M2 = 1U,
        .M = 1000U,
        .FracM = 0U,
    },
    /* CORE_PER_1728_40MHz */
    {
        .Finp = 40U,
        .N   = 39U,
        .Fout = 1728U,
        .M2  = 1U,
        .M = 1728U,
        .FracM = 0U,
    },
    /* DSP_900_40MHz  */
    {
        .Finp = 40U,
        .N = 19U,
        .Fout = 900U,
        .M2 = 1U,
        .M  = 450U,
        .FracM = 0U,
    },
    /* DSP_1800_40MHz  */
    {
        .Finp = 40U,
        .N    = 19U,
        .Fout = 1800U,
        .M2   = 1U,
        .M    = 900U,
        .FracM = 0U,
    },
    /* PER_1920_40MHz  */
    {
        .Finp = 40U,
        .N = 19U,
        .Fout = 1920U,
        .M2   = 1U,
        .M    = 960U,
        .FracM = 0U,
    },
    /* CORE_DSP_800_50MHz  */
    {
        .Finp = 50U,
        .N = 24U,
        .Fout = 800U,
        .M2 = 1U,
        .M = 400U,
        .FracM = 0U,
    },
    /* CORE_DSP_2000_50MHz  */
    {
       .Finp = 50U,
       .N = 24U,
       .Fout = 2000U,
       .M2 = 1U,
       .M = 1000U,
       .FracM = 0U,
    },
    /* DSP_1800_50MHz  */
    {
       .Finp = 50U,
       .N = 24U,
       .Fout = 1800U,
       .M2 = 1U,
       .M = 900U,
       .FracM = 0U,
    },
    /* PER_1920_50MHz  */
    {
       .Finp = 50U,
       .N = 24U,
       .Fout = 1920U,
       .M2 = 1U,
       .M = 960U,
       .FracM = 0U,
    },
    /* CORE_800_49152  */
    {
       .Finp = 49U,
       .N = 23U,
       .Fout = 800U,
       .M2 = 1U,
       .M = 390U,
       .FracM = 163840U,
    },
    /* CORE_2000_49152  */
    {
       .Finp = 49U,
       .N = 21U,
       .Fout = 2000U,
       .M2 = 1U,
       .M = 895U,
       .FracM = 47787U,
    },
    /* DSP_1800_49152  */
    {
       .Finp = 49U,
       .N = 20U,
       .Fout = 1800U,
       .M2 = 1U,
       .M = 769U,
       .FracM = 11264U,
    },
    /* DSP_1728_49152  */
    {
       .Finp = 49U,
       .N = 19U,
       .Fout = 1728U,
       .M2 = 1U,
       .M = 703U,
       .FracM = 32768U,
    },
    /* PER_1966p08_49152  */
    {
       .Finp = 49U,
       .N = 19U,
       .Fout = 1966U,
       .M2 = 1U,
       .M = 800U,
       .FracM = 209715200U,
    },
    /* CORE_800_451584  */
    {
       .Finp = 45U,
       .N = 20U,
       .Fout = 800U,
       .M2 = 1U,
       .M = 372U,
       .FracM = 6242U,
    },
    /* CORE_2000_451584  */
    {
       .Finp = 45U,
       .N = 20U,
       .Fout = 2000U,
       .M2 = 1U,
       .M = 930U,
       .FracM = 15604U,
    },
    /* DSP_1800_451584  */
    {
       .Finp = 45U,
       .N = 20U,
       .Fout = 1800U,
       .M2 = 1U,
       .M = 837U,
       .FracM = 14043U,
    },
    /* DSP_1728_451584  */
    {
       .Finp = 45U,
       .N = 18U,
       .Fout = 1728U,
       .M2 = 1U,
       .M = 727U,
       .FracM = 10700U,
    },
    /* PER_1806p336_451584  */
    {
       .Finp = 45U,
       .N = 18U,
       .Fout = 1806U,
       .M2 = 1U,
       .M = 760U,
       .FracM = 0U,
    },
    /* PER_1699_40MHz  */
    {
       .Finp = 40U,
       .N = 16U,
       .Fout = 1699U,
       .M2 = 1U,
       .M = 722U,
       .FracM = 44032U,
    },
};


const Rcm_XTALInfo gXTALInfo[] = 
{
    [RCM_XTAL_FREQID_CLK_40MHZ]      = {.Finp = SBL_FREQ_HZ2MHZ(RCM_XTAL_CLK_40MHZ),      .div2flag = false},
    [RCM_XTAL_FREQID_CLK_50MHZ]      = {.Finp = SBL_FREQ_HZ2MHZ(RCM_XTAL_CLK_50MHZ),      .div2flag = false},
    [RCM_XTAL_FREQID_CLK_49p152MHZ]  = {.Finp = SBL_FREQ_HZ2MHZ(RCM_XTAL_CLK_49p152MHZ),  .div2flag = false},
    [RCM_XTAL_FREQID_CLK_45p1584MHZ] = {.Finp = SBL_FREQ_HZ2MHZ(RCM_XTAL_CLK_45p1584MHZ), .div2flag = false},
    [RCM_XTAL_FREQID_CLK_20MHZ]      = {.Finp = SBL_FREQ_HZ2MHZ(RCM_XTAL_CLK_40MHZ),      .div2flag = true},
    [RCM_XTAL_FREQID_CLK_25MHZ]      = {.Finp = SBL_FREQ_HZ2MHZ(RCM_XTAL_CLK_50MHZ),      .div2flag = true},
    [RCM_XTAL_FREQID_CLK_24p576MHZ]  = {.Finp = SBL_FREQ_HZ2MHZ(RCM_XTAL_CLK_49p152MHZ),  .div2flag = true},
    [RCM_XTAL_FREQID_CLK_22p5792MHZ] = {.Finp = SBL_FREQ_HZ2MHZ(RCM_XTAL_CLK_50MHZ),      .div2flag = true},
};

const uint32_t gPLLFreqId2FOutMap[] = 
{
    [RCM_PLL_FOUT_FREQID_CLK_800MHZ]        = 800U,
    [RCM_PLL_FOUT_FREQID_CLK_900MHZ]        = 900U,
    [RCM_PLL_FOUT_FREQID_CLK_2000MHZ]       = 2000U,
    [RCM_PLL_FOUT_FREQID_CLK_1800MHZ]       = 1800U,
    [RCM_PLL_FOUT_FREQID_CLK_1920MHZ]       = 1920U,
    [RCM_PLL_FOUT_FREQID_CLK_1699p21875MHZ] = 1699U,
    [RCM_PLL_FOUT_FREQID_CLK_1728MHZ]       = 1728U,
    [RCM_PLL_FOUT_FREQID_CLK_1966p08MHZ]    = 1966U,
    [RCM_PLL_FOUT_FREQID_CLK_1806p336MHZ]   = 1806U,
};

const Rcm_ClkSrcInfo gDspClkSrcInfoMap[] =
{
    [Rcm_DSPClockSource_XTAL_CLK] = 
    {
        .pllId = RCM_PLLID_XTALCLK, 
        .hsDivOut = RCM_PLLHSDIV_OUT_NONE,
    },
    [Rcm_DSPClockSource_DPLL_DSP_HSDIV0_CLKOUT1] =
    {
        .pllId = RCM_PLLID_DSS, 
        .hsDivOut = RCM_PLLHSDIV_OUT_1,
    },
    [Rcm_DSPClockSource_DPLL_CORE_HSDIV0_CLKOUT1] =
    {
        .pllId = RCM_PLLID_CORE, 
        .hsDivOut = RCM_PLLHSDIV_OUT_1,
    },
};

const Rcm_ClkSrcInfo gPeripheralClkSrcInfoMap[] = 
{
    [Rcm_PeripheralClockSource_XTAL_CLK] = 
    {
        .pllId = RCM_PLLID_XTALCLK, 
        .hsDivOut = RCM_PLLHSDIV_OUT_NONE,
    },
    [Rcm_PeripheralClockSource_SYS_CLK] = 
    {
        .pllId = RCM_PLLID_CORE, 
        .hsDivOut = RCM_PLLHSDIV_OUT_2,
    },
    [Rcm_PeripheralClockSource_DPLL_CORE_HSDIV0_CLKOUT2] = 
    {
        .pllId = RCM_PLLID_CORE, 
        .hsDivOut = RCM_PLLHSDIV_OUT_2,
    },
    [Rcm_PeripheralClockSource_DPLL_PER_HSDIV0_CLKOUT1] = 
    {
        .pllId = RCM_PLLID_PER, 
        .hsDivOut = RCM_PLLHSDIV_OUT_1,
    },
    [Rcm_PeripheralClockSource_DPLL_PER_HSDIV0_CLKOUT2] = 
    {
        .pllId = RCM_PLLID_PER, 
        .hsDivOut = RCM_PLLHSDIV_OUT_2,
    },
    [Rcm_PeripheralClockSource_DPLL_PER_HSDIV0_CLKOUT3] = 
    {
        .pllId = RCM_PLLID_PER, 
        .hsDivOut = RCM_PLLHSDIV_OUT_3,
    },
};

const Rcm_ClkSrcInfo gCR5ClkSrcInfoMap =
{
    .pllId = RCM_PLLID_CORE, 
    .hsDivOut = RCM_PLLHSDIV_OUT_2,
};


/**
 * @brief
 *  Mapping Array for RTI
 *
 * @details
 *  Mapping Array between Clock mode and Clock Mode Value for RTI and WDG
 */
static const uint16_t gRtiClkSrcValMap[] =
{
    [Rcm_PeripheralClockSource_XTAL_CLK] = 0x000U, /* XTAL_CLK */
    [Rcm_PeripheralClockSource_SYS_CLK]  = 0x222U, /* SYS_CLK */
    [Rcm_PeripheralClockSource_DPLL_CORE_HSDIV0_CLKOUT2] = 0x888U, /* DPLL_CORE_HSDIV0_CLKOUT2 - Not Valid in this case */
    [Rcm_PeripheralClockSource_DPLL_PER_HSDIV0_CLKOUT1] = 0x333U, /* Set unsupported clock source to 0x888 which indicates invalid value */
    [Rcm_PeripheralClockSource_DPLL_PER_HSDIV0_CLKOUT2]  = 0x888U, /* Set unsupported clock source to 0x888 which indicates invalid value */
    [Rcm_PeripheralClockSource_DPLL_PER_HSDIV0_CLKOUT3] = 0x888U, /* Set unsupported clock source to 0x888 which indicates invalid value */
};

/**
 * @brief
 *  Mapping Array for SPI
 *
 * @details
 *  Mapping Array between Clock mode and Clock Mode Value for SPI
 */
static const uint16_t gSpiClkSrcValMap[] =
{
    [Rcm_PeripheralClockSource_XTAL_CLK] = 0x111U, /* XTAL_CLK */
    [Rcm_PeripheralClockSource_SYS_CLK]  = 0x222U, /* SYS_CLK */
    [Rcm_PeripheralClockSource_DPLL_CORE_HSDIV0_CLKOUT2] = 0x333U, /* DPLL_CORE_HSDIV0_CLKOUT2 */
    [Rcm_PeripheralClockSource_DPLL_PER_HSDIV0_CLKOUT1] = 0x444U, /* Set unsupported clock source to 0x888 which indicates invalid value */
    [Rcm_PeripheralClockSource_DPLL_PER_HSDIV0_CLKOUT2]  = 0x888U, /* Set unsupported clock source to 0x888 which indicates invalid value */
    [Rcm_PeripheralClockSource_DPLL_PER_HSDIV0_CLKOUT3] = 0x888U, /* Set unsupported clock source to 0x888 which indicates invalid value */
};

static const uint16_t gI2CClkSrcValMap[] = 
{
    [Rcm_PeripheralClockSource_XTAL_CLK] = 0x111U,
    [Rcm_PeripheralClockSource_SYS_CLK] = 0x222U,
    [Rcm_PeripheralClockSource_DPLL_PER_HSDIV0_CLKOUT1] = 0x333U,
    [Rcm_PeripheralClockSource_DPLL_CORE_HSDIV0_CLKOUT2] = 0x444U,
    [Rcm_PeripheralClockSource_DPLL_PER_HSDIV0_CLKOUT2]  = 0x888U, /* Set unsupported clock source to 0x888 which indicates invalid value */
    [Rcm_PeripheralClockSource_DPLL_PER_HSDIV0_CLKOUT3] = 0x888U, /* Set unsupported clock source to 0x888 which indicates invalid value */

};


/**
 * @brief
 *  Mapping Array for QSPI
 *
 * @details
 *  Mapping Array between Clock mode and Clock Mode Value for QSPI
 */
static const uint16_t gQspiClkSrcValMap[] =
{
    [Rcm_PeripheralClockSource_XTAL_CLK] = 0x000U, /* XTAL_CLK */
    [Rcm_PeripheralClockSource_SYS_CLK]  = 0x222U, /* SYS_CLK */
    [Rcm_PeripheralClockSource_DPLL_CORE_HSDIV0_CLKOUT2] = 0x444U, /* DPLL_CORE_HSDIV0_CLKOUT2 */
    [Rcm_PeripheralClockSource_DPLL_PER_HSDIV0_CLKOUT1] = 0x333,
    [Rcm_PeripheralClockSource_DPLL_PER_HSDIV0_CLKOUT2]  = 0x888U, /* Set unsupported clock source to 0x888 which indicates invalid value */
    [Rcm_PeripheralClockSource_DPLL_PER_HSDIV0_CLKOUT3] = 0x888U, /* Set unsupported clock source to 0x888 which indicates invalid value */

};

/**
 * @brief
 *  Mapping Array for SCI
 *
 * @details
 *  Mapping Array between Clock mode and Clock Mode Value for SCI
 */
static const uint16_t gSciClkSrcValMap[] =
{
    [Rcm_PeripheralClockSource_XTAL_CLK] = 0x111U, /* XTAL_CLK */
    [Rcm_PeripheralClockSource_SYS_CLK] = 0x222U, /* SYS_CLK */
    [Rcm_PeripheralClockSource_DPLL_CORE_HSDIV0_CLKOUT2] = 0x444U, /* DPLL_CORE_HSDIV0_CLKOUT2 */
    [Rcm_PeripheralClockSource_DPLL_PER_HSDIV0_CLKOUT1] = 0x333U, /* Set unsupported clock source to 0x888 which indicates invalid value */
    [Rcm_PeripheralClockSource_DPLL_PER_HSDIV0_CLKOUT2]  = 0x888U, /* Set unsupported clock source to 0x888 which indicates invalid value */
    [Rcm_PeripheralClockSource_DPLL_PER_HSDIV0_CLKOUT3] = 0x888U, /* Set unsupported clock source to 0x888 which indicates invalid value */

};

static const uint16_t gMcanClkSrcValMap[] = 
{
    [Rcm_PeripheralClockSource_XTAL_CLK] = 0x888U, /* XTAL clock source not supported for MCANA */
    [Rcm_PeripheralClockSource_SYS_CLK]  = 0x222U,
    [Rcm_PeripheralClockSource_DPLL_CORE_HSDIV0_CLKOUT2] = 0x444U,
    [Rcm_PeripheralClockSource_DPLL_PER_HSDIV0_CLKOUT1] = 0x888U, /* Set unsupported clock source to 0x888 which indicates invalid value */
    [Rcm_PeripheralClockSource_DPLL_PER_HSDIV0_CLKOUT2]  = 0x888U, /* Set unsupported clock source to 0x888 which indicates invalid value */
    [Rcm_PeripheralClockSource_DPLL_PER_HSDIV0_CLKOUT3] = 0x888U, /* Set unsupported clock source to 0x888 which indicates invalid value */

};

static const uint16_t gCsiRxClkSrcValMap[] = 
{
    [Rcm_PeripheralClockSource_XTAL_CLK] = 0x666U, /* XTAL clock source not supported for MCANA */
    [Rcm_PeripheralClockSource_SYS_CLK]  = 0x888U,
    [Rcm_PeripheralClockSource_DPLL_CORE_HSDIV0_CLKOUT2] = 0x888U,
    [Rcm_PeripheralClockSource_DPLL_PER_HSDIV0_CLKOUT1] = 0x888U, /* Set unsupported clock source to 0x888 which indicates invalid value */
    [Rcm_PeripheralClockSource_DPLL_PER_HSDIV0_CLKOUT2]  = 0x222,
    [Rcm_PeripheralClockSource_DPLL_PER_HSDIV0_CLKOUT3] = 0x888U, /* Set unsupported clock source to 0x888 which indicates invalid value */
};

static const uint16_t gDssRtiClkSrcValMap[] = 
{
    [Rcm_PeripheralClockSource_XTAL_CLK] = 0x111U,
    [Rcm_PeripheralClockSource_SYS_CLK] = 0x222U,
    [Rcm_PeripheralClockSource_DPLL_PER_HSDIV0_CLKOUT1] = 0x333U,
    [Rcm_PeripheralClockSource_DPLL_CORE_HSDIV0_CLKOUT2] = 0x888U,
    [Rcm_PeripheralClockSource_DPLL_PER_HSDIV0_CLKOUT2]  = 0x888U,
    [Rcm_PeripheralClockSource_DPLL_PER_HSDIV0_CLKOUT3] = 0x888U, /* Set unsupported clock source to 0x888 which indicates invalid value */
};

static const uint16_t gDssSciClkSrcValMap[] = 
{
    [Rcm_PeripheralClockSource_XTAL_CLK] = 0x111U,
    [Rcm_PeripheralClockSource_SYS_CLK] = 0x222U,
    [Rcm_PeripheralClockSource_DPLL_PER_HSDIV0_CLKOUT1] = 0x666U,
    [Rcm_PeripheralClockSource_DPLL_CORE_HSDIV0_CLKOUT2] = 0x888U,
    [Rcm_PeripheralClockSource_DPLL_PER_HSDIV0_CLKOUT2]  = 0x888U,
    [Rcm_PeripheralClockSource_DPLL_PER_HSDIV0_CLKOUT3] = 0x888U, /* Set unsupported clock source to 0x888 which indicates invalid value */
};

static const uint16_t gRcssSciClkSrcValMap[] = 
{
    [Rcm_PeripheralClockSource_XTAL_CLK] = 0x111U,
    [Rcm_PeripheralClockSource_SYS_CLK] = 0x222U,
    [Rcm_PeripheralClockSource_DPLL_PER_HSDIV0_CLKOUT1] = 0x333U,
    [Rcm_PeripheralClockSource_DPLL_CORE_HSDIV0_CLKOUT2] = 0x888U,
    [Rcm_PeripheralClockSource_DPLL_PER_HSDIV0_CLKOUT2]  = 0x888U,
    [Rcm_PeripheralClockSource_DPLL_PER_HSDIV0_CLKOUT3] = 0x888U, /* Set unsupported clock source to 0x888 which indicates invalid value */
};

static const uint16_t gRcssSpiClkSrcValMap[] = 
{
    [Rcm_PeripheralClockSource_XTAL_CLK] = 0x111U,
    [Rcm_PeripheralClockSource_SYS_CLK] = 0x222U,
    [Rcm_PeripheralClockSource_DPLL_PER_HSDIV0_CLKOUT1] = 0x333U,
    [Rcm_PeripheralClockSource_DPLL_CORE_HSDIV0_CLKOUT2] = 0x888U,
    [Rcm_PeripheralClockSource_DPLL_PER_HSDIV0_CLKOUT2]  = 0x888U,
    [Rcm_PeripheralClockSource_DPLL_PER_HSDIV0_CLKOUT3] = 0x888U, /* Set unsupported clock source to 0x888 which indicates invalid value */
};

static const uint16_t gRcssI2CClkSrcValMap[] = 
{
    [Rcm_PeripheralClockSource_XTAL_CLK] = 0x111U,
    [Rcm_PeripheralClockSource_SYS_CLK] = 0x222U,
    [Rcm_PeripheralClockSource_DPLL_PER_HSDIV0_CLKOUT1] = 0x333U,
    [Rcm_PeripheralClockSource_DPLL_CORE_HSDIV0_CLKOUT2] = 0x888U,
    [Rcm_PeripheralClockSource_DPLL_PER_HSDIV0_CLKOUT2]  = 0x888U,
    [Rcm_PeripheralClockSource_DPLL_PER_HSDIV0_CLKOUT3] = 0x888U, /* Set unsupported clock source to 0x888 which indicates invalid value */
};

static const uint16_t gRcssATLClkSrcValMap[] = 
{
    [Rcm_PeripheralClockSource_XTAL_CLK] = 0x111U,
    [Rcm_PeripheralClockSource_SYS_CLK] = 0x222U,
    [Rcm_PeripheralClockSource_DPLL_PER_HSDIV0_CLKOUT1] = 0x333U,
    [Rcm_PeripheralClockSource_DPLL_PER_HSDIV0_CLKOUT2] = 0x444U,
    [Rcm_PeripheralClockSource_DPLL_PER_HSDIV0_CLKOUT3] = 0x666U,
    [Rcm_PeripheralClockSource_DPLL_CORE_HSDIV0_CLKOUT2] = 0x888U,
};

static const uint16_t gRcssMCASPAuxClkSrcValMap[] = 
{
    [Rcm_PeripheralClockSource_XTAL_CLK] = 0x888U,
    [Rcm_PeripheralClockSource_SYS_CLK] = 0x888U,
    [Rcm_PeripheralClockSource_DPLL_CORE_HSDIV0_CLKOUT2] = 0x888U,
    [Rcm_PeripheralClockSource_DPLL_PER_HSDIV0_CLKOUT1] = 0x222U,
    [Rcm_PeripheralClockSource_DPLL_PER_HSDIV0_CLKOUT2] = 0x333U,
    [Rcm_PeripheralClockSource_DPLL_PER_HSDIV0_CLKOUT3] = 0x444U,
};

static const uint16_t gCptsClkSrcValMap[] = 
{
    [Rcm_PeripheralClockSource_XTAL_CLK] = 0x111U,
    [Rcm_PeripheralClockSource_SYS_CLK] = 0x222U,
    [Rcm_PeripheralClockSource_DPLL_CORE_HSDIV0_CLKOUT2] = 0x444U,
    [Rcm_PeripheralClockSource_DPLL_PER_HSDIV0_CLKOUT1] = 0x888U, /* Set unsupported clock source to 0x888 which indicates invalid value */
    [Rcm_PeripheralClockSource_DPLL_PER_HSDIV0_CLKOUT2]  = 0x888U, /* Set unsupported clock source to 0x888 which indicates invalid value */
    [Rcm_PeripheralClockSource_DPLL_PER_HSDIV0_CLKOUT3] = 0x888U, /* Set unsupported clock source to 0x888 which indicates invalid value */
};

static const uint16_t gCpswClkSrcValMap[] = 
{
    [Rcm_PeripheralClockSource_XTAL_CLK] = 0x111U,
    [Rcm_PeripheralClockSource_SYS_CLK] = 0x222U,
    [Rcm_PeripheralClockSource_DPLL_CORE_HSDIV0_CLKOUT2] = 0x444U,
    [Rcm_PeripheralClockSource_DPLL_PER_HSDIV0_CLKOUT1] = 0x888U, /* Set unsupported clock source to 0x888 which indicates invalid value */
    [Rcm_PeripheralClockSource_DPLL_PER_HSDIV0_CLKOUT2]  = 0x888U, /* Set unsupported clock source to 0x888 which indicates invalid value */
    [Rcm_PeripheralClockSource_DPLL_PER_HSDIV0_CLKOUT3] = 0x888U, /* Set unsupported clock source to 0x888 which indicates invalid value */
};

static const uint16_t gDspcoreClkSrcValMap[] = 
{
    [Rcm_DSPClockSource_XTAL_CLK] = 0x111,
    [Rcm_DSPClockSource_DPLL_DSP_HSDIV0_CLKOUT1] = 0x222,
    [Rcm_DSPClockSource_DPLL_CORE_HSDIV0_CLKOUT1] = 0x444
};

static const uint16_t gCR5ClkSrcValMap[] = 
{
    [Rcm_CR5ClockSource_DPLL_CORE_HSDIV0_CLKOUT2] = 0x222U,
};

/**
 * @brief
 *  Mapping Array for Reset Cause
 *
 * @details
 *  Mapping Array between Reset Cause Bit and Reset Cause
 */
static const Rcm_ResetCause gResetBitToResetCause[11U] =
{
    Rcm_ResetCause_POWER_ON_RESET,
    Rcm_ResetCause_WARM_RESET,
    Rcm_ResetCause_STC_RESET,
    Rcm_ResetCause_MMR_CPU0_VIM0_RESET,
    Rcm_ResetCause_MMR_CPU1_VIM1_RESET,
    Rcm_ResetCause_MMR_CPU0_RESET,
    Rcm_ResetCause_MMR_CPU1_RESET,
    Rcm_ResetCause_DBG_CPU0_RESET,
    Rcm_ResetCause_DBG_CPU1_RESET,
    Rcm_ResetCause_FSM_TRIGGER_RESET,
    Rcm_ResetCause_RST_CAUSE_UNKNOWN,
};

/**************************************************************************
 ************************ Local Data Structures ***************************
 **************************************************************************/

/**************************************************************************
 **************************** Local Functions *****************************
 **************************************************************************/
static void getClkSrcAndDivReg (Rcm_PeripheralId PeriphID,
                                Rcm_PeripheralClockSource clkSource, uint16_t *clkSrcVal,
                                volatile uint32_t **clkSrcReg,
                                volatile uint32_t **clkdDivReg);
static void progPllCoreDivider (uint8_t inputClockDiv , uint8_t divider,
                                uint16_t multiplier, uint8_t postDivider,
                                uint32_t fracMultiplier);
static void progPllDspDivider (uint8_t inputClockDiv , uint8_t divider,
                               uint16_t multiplier, uint8_t postDivider,
                               uint32_t fracMultiplier);
static void progPllPerDivider (uint8_t inputClockDiv , uint8_t divider,
                               uint16_t multiplier, uint8_t postDivider,
                              uint32_t fracMultiplier);

static void configurePllCore (uint16_t trimVal);
static void configurePllDsp (uint16_t trimVal);
static void configurePllPer (uint16_t trimVal);
static Rcm_XtalFreqId SBL_RcmGetXTALFrequency(void);
static uint16_t SBL_RcmGetCoreTrimVal(void);
static uint16_t SBL_RcmGetDspTrimVal(void);
static uint16_t SBL_RcmGetPerTrimVal(void);
static Rcm_ADPLLJConfig_t const * SBL_getADPLLJConfig(uint32_t Finp, 
                                                      Rcm_PllFoutFreqId foutFreqId);
static uint32_t SBL_RcmADPLLJGetFOut(uint32_t Finp, uint32_t N, uint32_t M, 
                                     uint32_t M2, uint32_t FracM, 
                                     bool div2flag);
static uint32_t SBL_RcmGetDSSFout(uint32_t Finp, bool div2flag);
static uint32_t SBL_RcmGetCoreFout(uint32_t Finp, bool div2flag);
static uint32_t SBL_RcmGetPerFout(uint32_t Finp, bool div2flag);
static uint32_t SBL_RcmGetCoreHSDivOut(uint32_t Finp, bool div2flag, Rcm_PllHSDIVOutId hsDivOut);
static uint32_t SBL_RcmGetDSSHSDivOut(uint32_t Finp, bool div2flag, Rcm_PllHSDIVOutId hsDivOut);
static uint32_t SBL_RcmGetPerHSDivOut(uint32_t Finp, bool div2flag, Rcm_PllHSDIVOutId hsDivOut);

/**
 *  @b Description
 *  @n
 *      The function is used to get the base address to the MSS CTRL register space
 *
 *  @retval
 *      pointer to the MSS CTRL Register space.
 */
CSL_mss_ctrlRegs* CSL_MSS_CTRL_getBaseAddress (void)
{
    return (CSL_mss_ctrlRegs*) CSL_MSS_CTRL_U_BASE;
}

CSL_dss_ctrlRegs* CSL_DSS_CTRL_getBaseAddress (void)
{
    return (CSL_dss_ctrlRegs *) CSL_DSS_CTRL_U_BASE;
}

#if defined (SOC_AWR294X)
/**
 *  @b Description
 *  @n
 *      The function is used to get the base address to the RSS CTRL register space
 *
 *  @retval
 *      pointer to the RSS CTRL Register space.
 */
CSL_rss_ctrlRegs* CSL_RSS_CTRL_getBaseAddress (void)
{
    return (CSL_rss_ctrlRegs *) CSL_RSS_CTRL_U_BASE;
}

/**
 *  @b Description
 *  @n
 *      The function is used to get the base address to the RSS PROC CTRL register space
 *
 *  @retval
 *      pointer to the RSS PROC CTRL Register space.
 */
CSL_rss_proc_ctrlRegs* CSL_RSS_PROC_CTRL_getBaseAddress (void)
{
    return (CSL_rss_proc_ctrlRegs *) CSL_RSS_PROC_CTRL_U_BASE;
}
#endif /* defined (SOC_AWR294X) */

/**
 *  @b Description
 *  @n
 *      The function is used to get the base address of the RCM.
 *
 *  \ingroup CSL_RCM_FUNCTION
 *
 *  @retval
 *      Base Address of the RCM Register
 */
CSL_mss_rcmRegs* CSL_RCM_getBaseAddress (void)
{
    return (CSL_mss_rcmRegs*) CSL_MSS_RCM_U_BASE;
}


CSL_dss_rcmRegs* CSL_DSSRCM_getBaseAddress (void) 
{
    return (CSL_dss_rcmRegs*) CSL_DSS_RCM_U_BASE;
}

CSL_rcss_rcmRegs* CSL_RCSSRCM_getBaseAddress (void)
{
    return (CSL_rcss_rcmRegs*) CSL_RCSS_RCM_U_BASE;
}


/**
 *  @b Description
 *  @n
 *      The function is used to get the base address of the Top-level RCM
 *      configuration registers.
 *
 *  \ingroup CSL_TOPRCM_FUNCTION
 *
 *  @retval
 *      Base Address of the Top-level RCM Register
 */
CSL_mss_toprcmRegs* CSL_TopRCM_getBaseAddress (void)
{
    return (CSL_mss_toprcmRegs*) CSL_MSS_TOPRCM_U_BASE;
}

/**
 *  @b Description
 *  @n
 *      The function is used to get the base address of the Top-level CTRL
 *      configuration registers.
 *
 *  \ingroup CSL_TOP_CTRL_FUNCTION
 *
 *  @retval
 *      Base Address of the Top-level CTRL Register
 */
CSL_top_ctrlRegs* CSL_TopCtrl_getBaseAddress (void)
{
    return (CSL_top_ctrlRegs*)CSL_TOP_CTRL_U_BASE;
}

/**
 *  @b Description
 *  @n
 *      This function reads the Efuse row config for XTAL Freq Scale
 *
 *  @param[in] ptrTopCtrlRegs
 *      Pointer to the MSS TOP CTRL Registers base address
 *
 *  @retval
 *      Efuse Row Value for XTAL Frequency scale.
 */
uint8_t CSL_TopCtrl_readXtalFreqScale (const CSL_top_ctrlRegs* ptrTopCtrlRegs)
{
    return (CSL_extract8 (ptrTopCtrlRegs->EFUSE1_ROW_10, \
                          XTAL_FREQ_SCALE_END_BIT, \
                          XTAL_FREQ_SCALE_START_BIT));
}

/**
 *  @b Description
 *  @n
 *      The function is read the Efuse row config for Core ADPLL Trim Valid Flag
 *
 *  @param[in] ptrTopCtrlRegs
 *      Pointer to the MSS TOP CTRL Registers base address
 *
 *  @retval
 *      Efuse Row Value for Core ADPLL Trim Valid Flag.
 */
uint8_t CSL_TopCtrl_readCoreADPLLTrimValidEfuse (const CSL_top_ctrlRegs* ptrTopCtrlRegs)
{
    return (CSL_extract8 (ptrTopCtrlRegs->EFUSE1_ROW_41, \
                          COREADPLLTRIMVALIDCONFIG_END_BIT, \
                          COREADPLLTRIMVALIDCONFIG_START_BIT));
}

/**
 *  @b Description
 *  @n
 *      The function is read the Efuse row config for Core ADPLL Trim Valid Flag
 *
 *  @param[in] ptrTopCtrlRegs
 *      Pointer to the MSS TOP CTRL Registers base address
 *
 *  @retval
 *      Efuse Row Value for Core ADPLL Trim Valid Flag.
 */
uint8_t CSL_TopCtrl_readDspADPLLTrimValidEfuse (const CSL_top_ctrlRegs* ptrTopCtrlRegs)
{
    return (CSL_extract8 (ptrTopCtrlRegs->EFUSE1_ROW_41, \
                          DSPADPLLTRIMVALIDCONFIG_END_BIT, \
                          DSPADPLLTRIMVALIDCONFIG_START_BIT));
}

/**
 *  @b Description
 *  @n
 *      The function is read the Efuse row config for Core ADPLL Trim Valid Flag
 *
 *  @param[in] ptrTopCtrlRegs
 *      Pointer to the MSS TOP CTRL Registers base address
 *
 *  @retval
 *      Efuse Row Value for Core ADPLL Trim Valid Flag.
 */
uint8_t CSL_TopCtrl_readPerADPLLTrimValidEfuse (const CSL_top_ctrlRegs* ptrTopCtrlRegs)
{
    return (CSL_extract8 (ptrTopCtrlRegs->EFUSE1_ROW_41, \
                          PERADPLLTRIMVALIDCONFIG_END_BIT, \
                          PERADPLLTRIMVALIDCONFIG_START_BIT));
}

/**
 *  @b Description
 *  @n
 *      The function is read the Efuse row config for Core ADPLL Trim Valid Flag
 *
 *  @param[in] ptrTopCtrlRegs
 *      Pointer to the MSS TOP CTRL Registers base address
 *
 *  @retval
 *      Efuse Row Value for Core ADPLL Trim Valid Flag.
 */
uint8_t CSL_TopCtrl_readPERADPLLTrimValidEfuse (const CSL_top_ctrlRegs* ptrTopCtrlRegs)
{
    return (CSL_extract8 (ptrTopCtrlRegs->EFUSE1_ROW_41, \
                          PERADPLLTRIMVALIDCONFIG_END_BIT, \
                          PERADPLLTRIMVALIDCONFIG_START_BIT));
}

/**
 *  @b Description
 *  @n
 *      The function is read the Efuse row config for QSPI Clock Frequency
 *
 *  @param[in] ptrTopCtrlRegs
 *      Pointer to the MSS TOP CTRL Registers base address
 *
 *  @retval
 *      Efuse Row Value for QSPI Clock Frequency.
 */
uint8_t CSL_TopCtrl_readQSPIClkFreqEfuse (const CSL_top_ctrlRegs * ptrTopCtrlRegs)
{
    return (CSL_extract8 (ptrTopCtrlRegs->EFUSE1_ROW_11, \
                          QSPIFREQCONFIG_END_BIT, \
                          QSPIFREQCONFIG_START_BIT));
}

/**
 *  @b Description
 *  @n
 *      The function is read the Efuse row config for Flash Mode
 *
 *  @param[in] ptrTopCtrlRegs
 *      Pointer to the MSS TOP CTRL Registers base address
 *
 *  @retval
 *      Efuse Row Value for FLash Mode.
 */
uint8_t CSL_TopCtrl_readFlashModeEfuse (const CSL_top_ctrlRegs * ptrTopCtrlRegs)
{
    return (CSL_extract8 (ptrTopCtrlRegs->EFUSE1_ROW_11, \
                          FLASHMODECONFIG_END_BIT, \
                          FLASHMODECONFIG_START_BIT));
}

/**
 *  @b Description
 *  @n
 *      The function is used to enable the access to the IO-Mux register space
 *
 *  @param[in] ptrIOMuxRegs
 *      Pointer to the IO-Mux Registers base address
 *
 *  @retval
 *      Not Applicable.
 */
void CSL_DSSRCM_enableAccess(CSL_dss_rcmRegs *ptrDSSRcmRegs)
{
    ptrDSSRcmRegs->LOCK0_KICK0 = 0x01234567;
    ptrDSSRcmRegs->LOCK0_KICK1 =  0xFEDCBA8;
}

/**
 *  @b Description
 *  @n
 *      The function is used to disable the access to the IO-Mux register space
 *
 *  @param[in] ptrIOMuxRegs
 *      Pointer to the IO-Mux Registers base address
 *
 *  @retval
 *      Not Applicable.
 */
void CSL_DSSRCM_disableAccess(CSL_dss_rcmRegs *ptrDSSRcmRegs)
{
    ptrDSSRcmRegs->LOCK0_KICK0 = 0x0;
    ptrDSSRcmRegs->LOCK0_KICK1 =  0x0;
}

void CSL_DSS_CTRL_enableAccess(CSL_dss_ctrlRegs *ptrDSSCtrlRegs)
{
    ptrDSSCtrlRegs->LOCK0_KICK0 = 0x01234567;
    ptrDSSCtrlRegs->LOCK0_KICK1 = 0xFEDCBA8;
}

/**************************************************************************
 ************************** RCM Functions *****************************
 **************************************************************************/

static uint32_t getClkSrcFromClkSelVal(const uint16_t *clkSelTbl, uint32_t numEntries, uint32_t clkSelMatchVal)
{
    uint32_t i;
    uint32_t clkSource;

    for (i = 0; i < numEntries; i++)
    {
        if (clkSelMatchVal == clkSelTbl[i])
        {
            break;
        }
    }
    if (i < numEntries)
    {
        clkSource = i;
    }
    else
    {
        clkSource = ~0U;
    }
    return clkSource;
}


/**
 *  @b Description
 *  @n
 *      This API returns Clk Src Selection register, Clk  Divider Register for
 *      specified peripheral. It also returns the Clk Src Selection value for a
 *      specified clock
 *
 *  @param[in]  PeriphID
 *      Peripheral Id
 *  @param[in]  clkSource
 *      Clock Source
 *  @param[out]  clkSrcVal
 *      Value to be programmed corresponding to the ClkSource
 *  @param[out]  clkSrcReg
 *      Register Address for programming Clock Source Selection
 *  @param[in]  clkdDivReg
 *      PRegister Address for programming Clock Divider Selection
 *
 *  @retval     None
 */
static void getClkSrcAndDivReg (Rcm_PeripheralId PeriphID,
                                Rcm_PeripheralClockSource clkSource, 
                                uint16_t *clkSrcVal,
                                volatile uint32_t **clkSrcReg,
                                volatile uint32_t **clkdDivReg)
{
    CSL_mss_rcmRegs *ptrMSSRCMRegs;
    CSL_dss_rcmRegs *ptrDSSRCMRegs;
    CSL_rcss_rcmRegs *ptrRCSSRCMRegs;
    CSL_mss_toprcmRegs *ptrTOPRCMRegs;

    ptrMSSRCMRegs = CSL_RCM_getBaseAddress ();
    ptrDSSRCMRegs = CSL_DSSRCM_getBaseAddress ();
    ptrTOPRCMRegs = CSL_TopRCM_getBaseAddress ();
    ptrRCSSRCMRegs = CSL_RCSSRCM_getBaseAddress ();

    switch (PeriphID)
    {
        case Rcm_PeripheralId_MSS_RTIA:
        {
            *clkSrcReg  = &(ptrMSSRCMRegs->MSS_RTIA_CLK_SRC_SEL);
            *clkdDivReg = &(ptrMSSRCMRegs->MSS_RTIA_CLK_DIV_VAL);
            *clkSrcVal = gRtiClkSrcValMap[clkSource];
            break;
        }
        case Rcm_PeripheralId_MSS_RTIB:
        {
            *clkSrcReg  = &(ptrMSSRCMRegs->MSS_RTIB_CLK_SRC_SEL);
            *clkdDivReg = &(ptrMSSRCMRegs->MSS_RTIB_CLK_DIV_VAL);
            *clkSrcVal = gRtiClkSrcValMap[clkSource];
            break;
        }
        case Rcm_PeripheralId_MSS_RTIC:
        {
            *clkSrcReg  = &(ptrMSSRCMRegs->MSS_RTIC_CLK_SRC_SEL);
            *clkdDivReg = &(ptrMSSRCMRegs->MSS_RTIC_CLK_DIV_VAL);
            *clkSrcVal = gRtiClkSrcValMap[clkSource];
            break;
        }
        case Rcm_PeripheralId_MSS_WDT:
        {
            *clkSrcReg  = &(ptrMSSRCMRegs->MSS_WDT_CLK_SRC_SEL);
            *clkdDivReg = &(ptrMSSRCMRegs->MSS_WDT_CLK_DIV_VAL);
            *clkSrcVal = gRtiClkSrcValMap[clkSource];
            break;
        }
        case Rcm_PeripheralId_MSS_SCIA:
        {
            *clkSrcReg  = &(ptrMSSRCMRegs->MSS_SCIA_CLK_SRC_SEL);
            *clkdDivReg = &(ptrMSSRCMRegs->MSS_SCIA_CLK_DIV_VAL);
            *clkSrcVal = gSciClkSrcValMap[clkSource];
            break;
        }
        case Rcm_PeripheralId_MSS_SCIB:
        {
            *clkSrcReg  = &(ptrMSSRCMRegs->MSS_SCIB_CLK_SRC_SEL);
            *clkdDivReg = &(ptrMSSRCMRegs->MSS_SCIB_CLK_DIV_VAL);
            *clkSrcVal = gSciClkSrcValMap[clkSource];
            break;
        }
        case Rcm_PeripheralId_MSS_SPIA:
        {
            *clkSrcReg  = &(ptrMSSRCMRegs->MSS_SPIA_CLK_SRC_SEL);
            *clkdDivReg = &(ptrMSSRCMRegs->MSS_SPIA_CLK_DIV_VAL);
            *clkSrcVal = gSpiClkSrcValMap[clkSource];
            break;
        }
        case Rcm_PeripheralId_MSS_SPIB:
        {
            *clkSrcReg  = &(ptrMSSRCMRegs->MSS_SPIB_CLK_SRC_SEL);
            *clkdDivReg = &(ptrMSSRCMRegs->MSS_SPIB_CLK_DIV_VAL);
            *clkSrcVal = gSpiClkSrcValMap[clkSource];
            break;
        }
        case Rcm_PeripheralId_MSS_I2C:
        {
            *clkSrcReg  = &(ptrMSSRCMRegs->MSS_I2C_CLK_SRC_SEL);
            *clkdDivReg = &(ptrMSSRCMRegs->MSS_I2C_CLK_DIV_VAL);
            *clkSrcVal = gI2CClkSrcValMap[clkSource];
            break;
        }
        case Rcm_PeripheralId_MSS_QSPI:
        {
            *clkSrcReg  = &(ptrMSSRCMRegs->MSS_QSPI_CLK_SRC_SEL);
            *clkdDivReg = &(ptrMSSRCMRegs->MSS_QSPI_CLK_DIV_VAL);
            *clkSrcVal = gQspiClkSrcValMap[clkSource];
            break;
        }
        case Rcm_PeripheralId_MSS_MCANA:
        {
            *clkSrcReg  = &(ptrMSSRCMRegs->MSS_MCANA_CLK_SRC_SEL);
            *clkdDivReg = &(ptrMSSRCMRegs->MSS_MCANA_CLK_DIV_VAL);
            *clkSrcVal = gMcanClkSrcValMap[clkSource];
            break;
        }
        case Rcm_PeripheralId_MSS_MCANB:
        {
            *clkSrcReg  = &(ptrMSSRCMRegs->MSS_MCANB_CLK_SRC_SEL);
            *clkdDivReg = &(ptrMSSRCMRegs->MSS_MCANB_CLK_DIV_VAL);
            *clkSrcVal = gMcanClkSrcValMap[clkSource];
            break;
        }
        case Rcm_PeripheralId_MSS_CPSW:
        {
            *clkSrcReg  = &(ptrMSSRCMRegs->MSS_CPSW_CLK_SRC_SEL);
            *clkdDivReg = &(ptrMSSRCMRegs->MSS_CPSW_CLK_DIV_VAL);
            *clkSrcVal = gCpswClkSrcValMap[clkSource];
            break;
        }
        case Rcm_PeripheralId_MSS_CPTS:
        {
            *clkSrcReg  = &(ptrMSSRCMRegs->MSS_CPTS_CLK_SRC_SEL);
            *clkdDivReg = &(ptrMSSRCMRegs->MSS_CPTS_CLK_DIV_VAL);
            *clkSrcVal = gCptsClkSrcValMap[clkSource];
            break;
        }
        case Rcm_PeripheralId_CSIRX:
        {
            *clkSrcReg  = &(ptrTOPRCMRegs->CSIRX_CLK_SRC_SEL);
            *clkdDivReg = &(ptrTOPRCMRegs->CSIRX_DIV_VAL);
            *clkSrcVal = gCsiRxClkSrcValMap[clkSource];
            break;
        }
        case Rcm_PeripheralId_DSS_RTIA:
        {
            *clkSrcReg  = &(ptrDSSRCMRegs->DSS_RTIA_CLK_SRC_SEL);
            *clkdDivReg = &(ptrDSSRCMRegs->DSS_RTIA_CLK_DIV_VAL);
            *clkSrcVal = gDssRtiClkSrcValMap[clkSource];
            break;
        }
        case Rcm_PeripheralId_DSS_RTIB:
        {
            *clkSrcReg  = &(ptrDSSRCMRegs->DSS_RTIB_CLK_SRC_SEL);
            *clkdDivReg = &(ptrDSSRCMRegs->DSS_RTIB_CLK_DIV_VAL);
            *clkSrcVal = gDssRtiClkSrcValMap[clkSource];
            break;
        }
        case Rcm_PeripheralId_DSS_WDT:
        {
            *clkSrcReg  = &(ptrDSSRCMRegs->DSS_WDT_CLK_SRC_SEL);
            *clkdDivReg = &(ptrDSSRCMRegs->DSS_WDT_CLK_DIV_VAL);
            *clkSrcVal = gDssRtiClkSrcValMap[clkSource];
            break;
        }
        case Rcm_PeripheralId_DSS_SCIA:
        {
            *clkSrcReg  = &(ptrDSSRCMRegs->DSS_SCIA_CLK_SRC_SEL);
            *clkdDivReg = &(ptrDSSRCMRegs->DSS_SCIA_CLK_DIV_VAL);
            *clkSrcVal = gDssSciClkSrcValMap[clkSource];
            break;
        }
        case Rcm_PeripheralId_RCSS_SCIA:
        {
            *clkSrcReg  = &(ptrRCSSRCMRegs->RCSS_SCIA_CLK_SRC_SEL);
            *clkdDivReg = &(ptrRCSSRCMRegs->RCSS_SCIA_CLK_DIV_VAL);
            *clkSrcVal = gRcssSciClkSrcValMap[clkSource];
            break;
        }
        case Rcm_PeripheralId_RCSS_SPIA:
        {
            *clkSrcReg  = &(ptrRCSSRCMRegs->RCSS_SPIA_CLK_SRC_SEL);
            *clkdDivReg = &(ptrRCSSRCMRegs->RCSS_SPIA_CLK_DIV_VAL);
            *clkSrcVal = gRcssSpiClkSrcValMap[clkSource];
            break;
        }
        case Rcm_PeripheralId_RCSS_SPIB:
        {
            *clkSrcReg  = &(ptrRCSSRCMRegs->RCSS_SPIB_CLK_SRC_SEL);
            *clkdDivReg = &(ptrRCSSRCMRegs->RCSS_SPIB_CLK_DIV_VAL);
            *clkSrcVal = gRcssSpiClkSrcValMap[clkSource];
            break;
        }
        case Rcm_PeripheralId_RCSS_I2CA:
        {
            *clkSrcReg  = &(ptrRCSSRCMRegs->RCSS_I2CA_CLK_SRC_SEL);
            *clkdDivReg = &(ptrRCSSRCMRegs->RCSS_I2CA_CLK_DIV_VAL);
            *clkSrcVal = gRcssI2CClkSrcValMap[clkSource];
            break;
        }
        case Rcm_PeripheralId_RCSS_I2CB:
        {
            *clkSrcReg  = &(ptrRCSSRCMRegs->RCSS_I2CB_CLK_SRC_SEL);
            *clkdDivReg = &(ptrRCSSRCMRegs->RCSS_I2CB_CLK_DIV_VAL);
            *clkSrcVal = gRcssI2CClkSrcValMap[clkSource];
            break;
        }
        case Rcm_PeripheralId_RCSS_ATL:
        {
            *clkSrcReg  = &(ptrRCSSRCMRegs->RCSS_ATL_CLK_SRC_SEL);
            *clkdDivReg = &(ptrRCSSRCMRegs->RCSS_ATL_CLK_DIV_VAL);
            *clkSrcVal = gRcssATLClkSrcValMap[clkSource];
            break;
        }
        case Rcm_PeripheralId_RCSS_MCASPA_AUX:
        {
            *clkSrcReg  = &(ptrRCSSRCMRegs->RCSS_MCASPA_AUX_CLK_SRC_SEL);
            *clkdDivReg = &(ptrRCSSRCMRegs->RCSS_MCASPA_AUX_CLK_DIV_VAL);
            *clkSrcVal = gRcssMCASPAuxClkSrcValMap[clkSource];
            break;
        }
        case Rcm_PeripheralId_RCSS_MCASPB_AUX:
        {
            *clkSrcReg  = &(ptrRCSSRCMRegs->RCSS_MCASPB_AUX_CLK_SRC_SEL);
            *clkdDivReg = &(ptrRCSSRCMRegs->RCSS_MCASPB_AUX_CLK_DIV_VAL);
            *clkSrcVal = gRcssMCASPAuxClkSrcValMap[clkSource];
            break;
        }
        case Rcm_PeripheralId_RCSS_MCASPC_AUX:
        {
            *clkSrcReg  = &(ptrRCSSRCMRegs->RCSS_MCASPC_AUX_CLK_SRC_SEL);
            *clkdDivReg = &(ptrRCSSRCMRegs->RCSS_MCASPC_AUX_CLK_DIV_VAL);
            *clkSrcVal = gRcssMCASPAuxClkSrcValMap[clkSource];
            break;
        }
        default:
        {
            *clkSrcReg  = NULL;
            *clkdDivReg = NULL;
            *clkSrcVal = 0x888U;
        }
    }
    return;
}

/**
 *  @b Description
 *  @n
 *      This API returns Clk Src Selection register, Clk  Divider Register for
 *      specified peripheral. It also returns the Clk Src Selection value for a
 *      specified clock
 *
 *  @param[in]  PeriphID
 *      Peripheral Id
 *  @param[in]  clkSource
 *      Clock Source
 *  @param[out]  clkSrcVal
 *      Value to be programmed corresponding to the ClkSource
 *  @param[out]  clkSrcReg
 *      Register Address for programming Clock Source Selection
 *  @param[in]  clkdDivReg
 *      PRegister Address for programming Clock Divider Selection
 *
 *  @retval     None
 */
static Rcm_Return getClkSrcAndDivValue (Rcm_PeripheralId PeriphID,
                                        Rcm_PeripheralClockSource *clkSource, 
                                        volatile uint32_t *clkDiv)
{
    CSL_mss_rcmRegs *ptrMSSRCMRegs;
    CSL_dss_rcmRegs *ptrDSSRCMRegs;
    CSL_rcss_rcmRegs *ptrRCSSRCMRegs;
    CSL_mss_toprcmRegs *ptrTOPRCMRegs;
    uint32_t clkSrc;
    uint32_t clkSrcId;
    Rcm_Return retVal = Rcm_Return_SUCCESS;

    ptrMSSRCMRegs = CSL_RCM_getBaseAddress ();
    ptrDSSRCMRegs = CSL_DSSRCM_getBaseAddress ();
    ptrTOPRCMRegs = CSL_TopRCM_getBaseAddress ();
    ptrRCSSRCMRegs = CSL_RCSSRCM_getBaseAddress ();

    switch (PeriphID)
    {
        case Rcm_PeripheralId_MSS_RTIA:
        {
            clkSrc  = ptrMSSRCMRegs->MSS_RTIA_CLK_SRC_SEL;
            *clkDiv = ptrMSSRCMRegs->MSS_RTIA_CLK_DIV_VAL;
            clkSrcId = getClkSrcFromClkSelVal(gRtiClkSrcValMap, SBL_UTILS_ARRAYSIZE(gRtiClkSrcValMap), clkSrc);
            DebugP_assert(clkSrcId != ~0U);
            *clkSource = (Rcm_PeripheralClockSource) clkSrcId;
            break;
        }
        case Rcm_PeripheralId_MSS_RTIB:
        {
            clkSrc  = ptrMSSRCMRegs->MSS_RTIB_CLK_SRC_SEL;
            *clkDiv = ptrMSSRCMRegs->MSS_RTIB_CLK_DIV_VAL;
            clkSrcId = getClkSrcFromClkSelVal(gRtiClkSrcValMap, SBL_UTILS_ARRAYSIZE(gRtiClkSrcValMap), clkSrc);
            DebugP_assert(clkSrcId != ~0U);
            *clkSource = (Rcm_PeripheralClockSource) clkSrcId;
            break;
        }
        case Rcm_PeripheralId_MSS_RTIC:
        {
            clkSrc  = ptrMSSRCMRegs->MSS_RTIC_CLK_SRC_SEL;
            *clkDiv = ptrMSSRCMRegs->MSS_RTIC_CLK_DIV_VAL;
            clkSrcId = getClkSrcFromClkSelVal(gRtiClkSrcValMap, SBL_UTILS_ARRAYSIZE(gRtiClkSrcValMap), clkSrc);
            DebugP_assert(clkSrcId != ~0U);
            *clkSource = (Rcm_PeripheralClockSource) clkSrcId;

            break;
        }
        case Rcm_PeripheralId_MSS_WDT:
        {
            clkSrc  = ptrMSSRCMRegs->MSS_WDT_CLK_SRC_SEL;
            *clkDiv = ptrMSSRCMRegs->MSS_WDT_CLK_DIV_VAL;
            clkSrcId = getClkSrcFromClkSelVal(gRtiClkSrcValMap, SBL_UTILS_ARRAYSIZE(gRtiClkSrcValMap), clkSrc);
            DebugP_assert(clkSrcId != ~0U);
            *clkSource = (Rcm_PeripheralClockSource) clkSrcId;
            break;
        }
        case Rcm_PeripheralId_MSS_SCIA:
        {
            clkSrc  = ptrMSSRCMRegs->MSS_SCIA_CLK_SRC_SEL;
            *clkDiv = ptrMSSRCMRegs->MSS_SCIA_CLK_DIV_VAL;
            clkSrcId = getClkSrcFromClkSelVal(gSciClkSrcValMap, SBL_UTILS_ARRAYSIZE(gSciClkSrcValMap), clkSrc);
            DebugP_assert(clkSrcId != ~0U);
            *clkSource = (Rcm_PeripheralClockSource) clkSrcId;
            break;
        }
        case Rcm_PeripheralId_MSS_SCIB:
        {
            clkSrc  = ptrMSSRCMRegs->MSS_SCIB_CLK_SRC_SEL;
            *clkDiv = ptrMSSRCMRegs->MSS_SCIB_CLK_DIV_VAL;
            clkSrcId = getClkSrcFromClkSelVal(gSciClkSrcValMap, SBL_UTILS_ARRAYSIZE(gSciClkSrcValMap), clkSrc);
            DebugP_assert(clkSrcId != ~0U);
            *clkSource = (Rcm_PeripheralClockSource) clkSrcId;
            break;
        }
        case Rcm_PeripheralId_MSS_SPIA:
        {
            clkSrc  = ptrMSSRCMRegs->MSS_SPIA_CLK_SRC_SEL;
            *clkDiv = ptrMSSRCMRegs->MSS_SPIA_CLK_DIV_VAL;
            clkSrcId = getClkSrcFromClkSelVal(gSpiClkSrcValMap, SBL_UTILS_ARRAYSIZE(gSpiClkSrcValMap), clkSrc);
            DebugP_assert(clkSrcId != ~0U);
            *clkSource = (Rcm_PeripheralClockSource) clkSrcId;
            break;
        }
        case Rcm_PeripheralId_MSS_SPIB:
        {
            clkSrc  = ptrMSSRCMRegs->MSS_SPIB_CLK_SRC_SEL;
            *clkDiv = ptrMSSRCMRegs->MSS_SPIB_CLK_DIV_VAL;
            clkSrcId = getClkSrcFromClkSelVal(gSpiClkSrcValMap, SBL_UTILS_ARRAYSIZE(gSpiClkSrcValMap), clkSrc);
            DebugP_assert(clkSrcId != ~0U);
            *clkSource = (Rcm_PeripheralClockSource) clkSrcId;
            break;
        }
        case Rcm_PeripheralId_MSS_I2C:
        {
            clkSrc  = ptrMSSRCMRegs->MSS_I2C_CLK_SRC_SEL;
            *clkDiv = ptrMSSRCMRegs->MSS_I2C_CLK_DIV_VAL;
            clkSrcId = getClkSrcFromClkSelVal(gI2CClkSrcValMap, SBL_UTILS_ARRAYSIZE(gI2CClkSrcValMap), clkSrc);
            DebugP_assert(clkSrcId != ~0U);
            *clkSource = (Rcm_PeripheralClockSource) clkSrcId;
            break;
        }
        case Rcm_PeripheralId_MSS_QSPI:
        {
            clkSrc  = ptrMSSRCMRegs->MSS_QSPI_CLK_SRC_SEL;
            *clkDiv = ptrMSSRCMRegs->MSS_QSPI_CLK_DIV_VAL;
            clkSrcId = getClkSrcFromClkSelVal(gQspiClkSrcValMap, SBL_UTILS_ARRAYSIZE(gQspiClkSrcValMap), clkSrc);
            DebugP_assert(clkSrcId != ~0U);
            *clkSource = (Rcm_PeripheralClockSource) clkSrcId;
            break;
        }
        case Rcm_PeripheralId_MSS_MCANA:
        {
            clkSrc  = ptrMSSRCMRegs->MSS_MCANA_CLK_SRC_SEL;
            *clkDiv = ptrMSSRCMRegs->MSS_MCANA_CLK_DIV_VAL;
            clkSrcId = getClkSrcFromClkSelVal(gMcanClkSrcValMap, SBL_UTILS_ARRAYSIZE(gMcanClkSrcValMap), clkSrc);
            DebugP_assert(clkSrcId != ~0U);
            *clkSource = (Rcm_PeripheralClockSource) clkSrcId;
            break;
        }
        case Rcm_PeripheralId_MSS_MCANB:
        {
            clkSrc  = ptrMSSRCMRegs->MSS_MCANB_CLK_SRC_SEL;
            *clkDiv = ptrMSSRCMRegs->MSS_MCANB_CLK_DIV_VAL;
            clkSrcId = getClkSrcFromClkSelVal(gMcanClkSrcValMap, SBL_UTILS_ARRAYSIZE(gMcanClkSrcValMap), clkSrc);
            DebugP_assert(clkSrcId != ~0U);
            *clkSource = (Rcm_PeripheralClockSource) clkSrcId;
            break;
        }
        case Rcm_PeripheralId_MSS_CPSW:
        {
            clkSrc  = ptrMSSRCMRegs->MSS_CPSW_CLK_SRC_SEL;
            *clkDiv = ptrMSSRCMRegs->MSS_CPSW_CLK_DIV_VAL;
            clkSrcId = getClkSrcFromClkSelVal(gCpswClkSrcValMap, SBL_UTILS_ARRAYSIZE(gCpswClkSrcValMap), clkSrc);
            DebugP_assert(clkSrcId != ~0U);
            *clkSource = (Rcm_PeripheralClockSource) clkSrcId;
            break;
        }
        case Rcm_PeripheralId_MSS_CPTS:
        {
            clkSrc  = ptrMSSRCMRegs->MSS_CPTS_CLK_SRC_SEL;
            *clkDiv = ptrMSSRCMRegs->MSS_CPTS_CLK_DIV_VAL;
            clkSrcId = getClkSrcFromClkSelVal(gCptsClkSrcValMap, SBL_UTILS_ARRAYSIZE(gCptsClkSrcValMap), clkSrc);
            DebugP_assert(clkSrcId != ~0U);
            *clkSource = (Rcm_PeripheralClockSource) clkSrcId;
            break;
        }
        case Rcm_PeripheralId_CSIRX:
        {
            clkSrc  = ptrTOPRCMRegs->CSIRX_CLK_SRC_SEL;
            *clkDiv = ptrTOPRCMRegs->CSIRX_DIV_VAL;
            clkSrcId = getClkSrcFromClkSelVal(gCsiRxClkSrcValMap, SBL_UTILS_ARRAYSIZE(gCsiRxClkSrcValMap), clkSrc);
            DebugP_assert(clkSrcId != ~0U);
            *clkSource = (Rcm_PeripheralClockSource) clkSrcId;
            break;
        }
        case Rcm_PeripheralId_DSS_RTIA:
        {
            clkSrc  = ptrDSSRCMRegs->DSS_RTIA_CLK_SRC_SEL;
            *clkDiv = ptrDSSRCMRegs->DSS_RTIA_CLK_DIV_VAL;
            clkSrcId = getClkSrcFromClkSelVal(gDssRtiClkSrcValMap, SBL_UTILS_ARRAYSIZE(gDssRtiClkSrcValMap), clkSrc);
            DebugP_assert(clkSrcId != ~0U);
            *clkSource = (Rcm_PeripheralClockSource) clkSrcId;
            break;
        }
        case Rcm_PeripheralId_DSS_RTIB:
        {
            clkSrc  = ptrDSSRCMRegs->DSS_RTIB_CLK_SRC_SEL;
            *clkDiv = ptrDSSRCMRegs->DSS_RTIB_CLK_DIV_VAL;
            clkSrcId = getClkSrcFromClkSelVal(gDssRtiClkSrcValMap, SBL_UTILS_ARRAYSIZE(gDssRtiClkSrcValMap), clkSrc);
            DebugP_assert(clkSrcId != ~0U);
            *clkSource = (Rcm_PeripheralClockSource) clkSrcId;
            break;
        }
        case Rcm_PeripheralId_DSS_WDT:
        {
            clkSrc  = ptrDSSRCMRegs->DSS_WDT_CLK_SRC_SEL;
            *clkDiv = ptrDSSRCMRegs->DSS_WDT_CLK_DIV_VAL;
            clkSrcId = getClkSrcFromClkSelVal(gDssRtiClkSrcValMap, SBL_UTILS_ARRAYSIZE(gDssRtiClkSrcValMap), clkSrc);
            DebugP_assert(clkSrcId != ~0U);
            *clkSource = (Rcm_PeripheralClockSource) clkSrcId;
            break;
        }
        case Rcm_PeripheralId_DSS_SCIA:
        {
            clkSrc  = ptrDSSRCMRegs->DSS_SCIA_CLK_SRC_SEL;
            *clkDiv = ptrDSSRCMRegs->DSS_SCIA_CLK_DIV_VAL;
            clkSrcId = getClkSrcFromClkSelVal(gDssSciClkSrcValMap, SBL_UTILS_ARRAYSIZE(gDssSciClkSrcValMap), clkSrc);
            DebugP_assert(clkSrcId != ~0U);
            *clkSource = (Rcm_PeripheralClockSource) clkSrcId;
            break;
        }
        case Rcm_PeripheralId_RCSS_SCIA:
        {
            clkSrc  = ptrRCSSRCMRegs->RCSS_SCIA_CLK_SRC_SEL;
            *clkDiv = ptrRCSSRCMRegs->RCSS_SCIA_CLK_DIV_VAL;
            clkSrcId = getClkSrcFromClkSelVal(gRcssSciClkSrcValMap, SBL_UTILS_ARRAYSIZE(gRcssSciClkSrcValMap), clkSrc);
            DebugP_assert(clkSrcId != ~0U);
            *clkSource = (Rcm_PeripheralClockSource) clkSrcId;
            break;
        }
        case Rcm_PeripheralId_RCSS_SPIA:
        {
            clkSrc  = ptrRCSSRCMRegs->RCSS_SPIA_CLK_SRC_SEL;
            *clkDiv = ptrRCSSRCMRegs->RCSS_SPIA_CLK_DIV_VAL;
            clkSrcId = getClkSrcFromClkSelVal(gRcssSpiClkSrcValMap, SBL_UTILS_ARRAYSIZE(gRcssSpiClkSrcValMap), clkSrc);
            DebugP_assert(clkSrcId != ~0U);
            *clkSource = (Rcm_PeripheralClockSource) clkSrcId;
            break;
        }
        case Rcm_PeripheralId_RCSS_SPIB:
        {
            clkSrc  = ptrRCSSRCMRegs->RCSS_SPIB_CLK_SRC_SEL;
            *clkDiv = ptrRCSSRCMRegs->RCSS_SPIB_CLK_DIV_VAL;
            clkSrcId = getClkSrcFromClkSelVal(gRcssSpiClkSrcValMap, SBL_UTILS_ARRAYSIZE(gRcssSpiClkSrcValMap), clkSrc);
            DebugP_assert(clkSrcId != ~0U);
            *clkSource = (Rcm_PeripheralClockSource) clkSrcId;
            break;
        }
        case Rcm_PeripheralId_RCSS_I2CA:
        {
            clkSrc  = ptrRCSSRCMRegs->RCSS_I2CA_CLK_SRC_SEL;
            *clkDiv = ptrRCSSRCMRegs->RCSS_I2CA_CLK_DIV_VAL;
            clkSrcId = getClkSrcFromClkSelVal(gRcssI2CClkSrcValMap, SBL_UTILS_ARRAYSIZE(gRcssI2CClkSrcValMap), clkSrc);
            DebugP_assert(clkSrcId != ~0U);
            *clkSource = (Rcm_PeripheralClockSource) clkSrcId;
            break;
        }
        case Rcm_PeripheralId_RCSS_I2CB:
        {
            clkSrc  = ptrRCSSRCMRegs->RCSS_I2CB_CLK_SRC_SEL;
            *clkDiv = ptrRCSSRCMRegs->RCSS_I2CB_CLK_DIV_VAL;
            clkSrcId = getClkSrcFromClkSelVal(gRcssI2CClkSrcValMap, SBL_UTILS_ARRAYSIZE(gRcssI2CClkSrcValMap), clkSrc);
            DebugP_assert(clkSrcId != ~0U);
            *clkSource = (Rcm_PeripheralClockSource) clkSrcId;
            break;
        }
        case Rcm_PeripheralId_RCSS_ATL:
        {
            clkSrc  = ptrRCSSRCMRegs->RCSS_ATL_CLK_SRC_SEL;
            *clkDiv = ptrRCSSRCMRegs->RCSS_ATL_CLK_DIV_VAL;
            clkSrcId = getClkSrcFromClkSelVal(gRcssATLClkSrcValMap, SBL_UTILS_ARRAYSIZE(gRcssATLClkSrcValMap), clkSrc);
            DebugP_assert(clkSrcId != ~0U);
            *clkSource = (Rcm_PeripheralClockSource) clkSrcId;
            break;
        }
        case Rcm_PeripheralId_RCSS_MCASPA_AUX:
        {
            clkSrc  = ptrRCSSRCMRegs->RCSS_MCASPA_AUX_CLK_SRC_SEL;
            *clkDiv = ptrRCSSRCMRegs->RCSS_MCASPA_AUX_CLK_DIV_VAL;
            clkSrcId = getClkSrcFromClkSelVal(gRcssMCASPAuxClkSrcValMap, SBL_UTILS_ARRAYSIZE(gRcssMCASPAuxClkSrcValMap), clkSrc);
            DebugP_assert(clkSrcId != ~0U);
            *clkSource = (Rcm_PeripheralClockSource) clkSrcId;
            break;
        }
        case Rcm_PeripheralId_RCSS_MCASPB_AUX:
        {
            clkSrc  = ptrRCSSRCMRegs->RCSS_MCASPB_AUX_CLK_SRC_SEL;
            *clkDiv = ptrRCSSRCMRegs->RCSS_MCASPB_AUX_CLK_DIV_VAL;
            clkSrcId = getClkSrcFromClkSelVal(gRcssMCASPAuxClkSrcValMap, SBL_UTILS_ARRAYSIZE(gRcssMCASPAuxClkSrcValMap), clkSrc);
            DebugP_assert(clkSrcId != ~0U);
            *clkSource = (Rcm_PeripheralClockSource) clkSrcId;
            break;
        }
        case Rcm_PeripheralId_RCSS_MCASPC_AUX:
        {
            clkSrc  = ptrRCSSRCMRegs->RCSS_MCASPC_AUX_CLK_SRC_SEL;
            *clkDiv = ptrRCSSRCMRegs->RCSS_MCASPC_AUX_CLK_DIV_VAL;
            clkSrcId = getClkSrcFromClkSelVal(gRcssMCASPAuxClkSrcValMap, SBL_UTILS_ARRAYSIZE(gRcssMCASPAuxClkSrcValMap), clkSrc);
            DebugP_assert(clkSrcId != ~0U);
            *clkSource = (Rcm_PeripheralClockSource) clkSrcId;
            break;
        }
        default:
        {
            *clkDiv = 0U;
            retVal = Rcm_Return_ERROR;
        }
    }
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      This API returns Clk Src Selection register, Clk  Divider Register for
 *      specified peripheral. It also returns the Clk Src Selection value for a
 *      specified clock
 *
 *  @param[in]  PeriphID
 *      Peripheral Id
 *  @param[in]  clkSource
 *      Clock Source
 *  @param[out]  clkSrcVal
 *      Value to be programmed corresponding to the ClkSource
 *  @param[out]  clkSrcReg
 *      Register Address for programming Clock Source Selection
 *  @param[in]  clkdDivReg
 *      PRegister Address for programming Clock Divider Selection
 *
 *  @retval     None
 */
static void getDSPClkSrcAndDivReg (Rcm_DSPClockSource clkSource,
                                   uint16_t *clkSrcVal,
                                   volatile uint32_t **clkSrcReg,
                                   volatile uint32_t **clkdDivReg)
{
    CSL_dss_rcmRegs *ptrDSSRCMRegs;

    ptrDSSRCMRegs = CSL_DSSRCM_getBaseAddress ();

    *clkSrcReg  = &(ptrDSSRCMRegs->DSS_DSP_CLK_SRC_SEL);
    *clkdDivReg = &(ptrDSSRCMRegs->DSS_DSP_CLK_DIV_VAL);
    *clkSrcVal = gDspcoreClkSrcValMap[clkSource];

    return;
}


static Rcm_Return getDSPClkSrcAndDivValue(Rcm_DSPClockSource *clkSource,
                                          uint32_t *clkDiv)
{
    uint32_t clkSrc, clkSrcId;
    Rcm_Return retVal = Rcm_Return_SUCCESS;
    CSL_dss_rcmRegs *ptrDSSRCMRegs;

    ptrDSSRCMRegs = CSL_DSSRCM_getBaseAddress ();
    clkSrc  = ptrDSSRCMRegs->DSS_DSP_CLK_SRC_SEL;
    *clkDiv = ptrDSSRCMRegs->DSS_DSP_CLK_DIV_VAL;
    clkSrcId = getClkSrcFromClkSelVal(gDspcoreClkSrcValMap, SBL_UTILS_ARRAYSIZE(gDspcoreClkSrcValMap), clkSrc);
    DebugP_assert(clkSrcId != ~0U);
    if (clkSrcId != ~0U)
    {
        *clkSource = (Rcm_DSPClockSource) clkSrcId;
    }
    else
    {
        retVal = Rcm_Return_ERROR;
    }
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      This API Configure the Dividers and Multipliers for Core/DSP PLL
 *
 *  @param[in]  inputClockDiv
 *      Pre-divider value
 *  @param[in]  divider
 *      Divider value
 *  @param[in]  multiplier
 *      Multiplier Value
 *  @param[in]  postDivider
 *      Post Divider Value
 *  @param[in]  fracMultiplier
 *      Fractional Multiplier programmable value
 *
 *  @retval     None
 */
static void progPllCoreDivider (uint8_t inputClockDiv , uint8_t divider,
                                uint16_t multiplier, uint8_t postDivider,
                                uint32_t fracMultiplier)
{
    volatile uint32_t *ptrM2NReg, *ptrMN2Reg, *ptrFracMReg;
    CSL_mss_toprcmRegs *ptrTopRCMRegs;

    ptrTopRCMRegs = CSL_TopRCM_getBaseAddress ();

    ptrM2NReg   = &(ptrTopRCMRegs->PLL_CORE_M2NDIV);
    ptrMN2Reg   = &(ptrTopRCMRegs->PLL_CORE_MN2DIV);
    ptrFracMReg = &(ptrTopRCMRegs->PLL_CORE_FRACDIV);

    /* Initialization sequence referred from ADPLLLJ_GS70_v0.8-02 */
    /* program M2 (post divider) */
    // GEL file equivalent 
    // APPLJ-1 Setting
    // CLOCKOUT = M/(N+1) * CLKINP * (1/M2)  =  0x7d0/(39+1) * 40 * (1/1) = 2G
    // GEL file equivalent 
    //HW_WR_REG32(CSL_MSS_TOPRCM_U_BASE+PLL_CORE_M2NDIV     , 0x10027);      //M2NDIV_M2[22:16] = 1 , M2NDIV_N[7:0] = 0x27
    //HW_WR_REG32(CSL_MSS_TOPRCM_U_BASE+PLL_CORE_MN2DIV     , 0x107d0);      //MN2DIV_N2[19:16] = 1 , MN2DIV_M[11:0] = 0x7d0

    *ptrM2NReg = CSL_insert8 (*ptrM2NReg, 22U, 16U, postDivider);

    /* program N (input clock divider) */
    *ptrM2NReg = CSL_insert8 (*ptrM2NReg, 7U, 0U, inputClockDiv);

    /* program M (multiplier) */
    *ptrMN2Reg = CSL_insert16 (*ptrMN2Reg, 11U, 0U, multiplier);

    /* program N2 (divider) */
    *ptrMN2Reg = CSL_insert8 (*ptrMN2Reg, 19U, 16U, divider);

    /* program Fractional Multiplier */
    *ptrFracMReg = CSL_insert32 (*ptrFracMReg, 17U, 0U, fracMultiplier);
}

/**
 *  @b Description
 *  @n
 *      This API Configure the Dividers and Multipliers for Core/DSP PLL
 *
 *  @param[in]  inputClockDiv
 *      Pre-divider value
 *  @param[in]  divider
 *      Divider value
 *  @param[in]  multiplier
 *      Multiplier Value
 *  @param[in]  postDivider
 *      Post Divider Value
 *  @param[in]  fracMultiplier
 *      Fractional Multiplier programmable value
 *
 *  @retval     None
 */
static void progPllDspDivider (uint8_t inputClockDiv , uint8_t divider,
                               uint16_t multiplier, uint8_t postDivider,
                               uint32_t fracMultiplier)
{
    volatile uint32_t *ptrM2NReg, *ptrMN2Reg, *ptrFracMReg;
    CSL_mss_toprcmRegs *ptrTopRCMRegs;

    ptrTopRCMRegs = CSL_TopRCM_getBaseAddress ();

    ptrM2NReg   = &(ptrTopRCMRegs->PLL_DSP_M2NDIV);
    ptrMN2Reg   = &(ptrTopRCMRegs->PLL_DSP_MN2DIV);
    ptrFracMReg = &(ptrTopRCMRegs->PLL_DSP_FRACDIV);

    /* Initialization sequence referred from ADPLLLJ_GS70_v0.8-02 */
    /* program M2 (post divider) */
    // GEL file equivalent 
    // APPLJ-2 Setting
    // CLOCKOUT = M/(N+1) * CLKINP * (1/M2)  =  0x708/(39+1) * 40 * (1/1) = 1.8G
    //HW_WR_REG32(CSL_MSS_TOPRCM_U_BASE+PLL_DSP_M2NDIV     , 0x10027);      //M2NDIV_M2[22:16] = 1 , M2NDIV_N[7:0] = 0x27
    //HW_WR_REG32(CSL_MSS_TOPRCM_U_BASE+PLL_DSP_MN2DIV     , 0x10708);      //MN2DIV_N2[19:16] = 1 , MN2DIV_M[11:0] = 0x708
    *ptrM2NReg = CSL_insert8 (*ptrM2NReg, 22U, 16U, postDivider);

    /* program N (input clock divider) */
    *ptrM2NReg = CSL_insert8 (*ptrM2NReg, 7U, 0U, inputClockDiv);

    /* program M (multiplier) */
    *ptrMN2Reg = CSL_insert16 (*ptrMN2Reg, 11U, 0U, multiplier);

    /* program N2 (divider) */
    *ptrMN2Reg = CSL_insert8 (*ptrMN2Reg, 19U, 16U, divider);

    /* program Fractional Multiplier */
    *ptrFracMReg = CSL_insert32 (*ptrFracMReg, 17U, 0U, fracMultiplier);
}

/**
 *  @b Description
 *  @n
 *      This API Configure the Dividers and Multipliers for Core/DSP PLL
 *
 *  @param[in]  inputClockDiv
 *      Pre-divider value
 *  @param[in]  divider
 *      Divider value
 *  @param[in]  multiplier
 *      Multiplier Value
 *  @param[in]  postDivider
 *      Post Divider Value
 *  @param[in]  fracMultiplier
 *      Fractional Multiplier programmable value
 *
 *  @retval     None
 */
static void progPllPerDivider (uint8_t inputClockDiv , uint8_t divider,
                               uint16_t multiplier, uint8_t postDivider,
                               uint32_t fracMultiplier)
{
    volatile uint32_t *ptrM2NReg, *ptrMN2Reg, *ptrFracMReg;
    CSL_mss_toprcmRegs *ptrTopRCMRegs;

    ptrTopRCMRegs = CSL_TopRCM_getBaseAddress ();

    ptrM2NReg   = &(ptrTopRCMRegs->PLL_PER_M2NDIV);
    ptrMN2Reg   = &(ptrTopRCMRegs->PLL_PER_MN2DIV);
    ptrFracMReg = &(ptrTopRCMRegs->PLL_PER_FRACDIV);

    /* Initialization sequence referred from ADPLLLJ_GS70_v0.8-02 */
    /* program M2 (post divider) */
    // GEL file equivalent 
    // APPLJ-3 Setting
    // CLOCKOUT = M/(N+1) * CLKINP * (1/M2)  =  0x6C0/(39+1) * 40 * (1/1) = 1.728G
    //HW_WR_REG32(CSL_MSS_TOPRCM_U_BASE+PLL_PER_M2NDIV     , 0x10027);      //M2NDIV_M2[22:16] = 1 , M2NDIV_N[7:0] = 0x27
    //HW_WR_REG32(CSL_MSS_TOPRCM_U_BASE+PLL_PER_MN2DIV     , 0x106C0);      //MN2DIV_N2[19:16] = 1 , MN2DIV_M[11:0] = 0x6C0
    *ptrM2NReg = CSL_insert8 (*ptrM2NReg, 22U, 16U, postDivider);

    /* program N (input clock divider) */
    *ptrM2NReg = CSL_insert8 (*ptrM2NReg, 7U, 0U, inputClockDiv);

    /* program M (multiplier) */
    *ptrMN2Reg = CSL_insert16 (*ptrMN2Reg, 11U, 0U, multiplier);

    /* program N2 (divider) */
    *ptrMN2Reg = CSL_insert8 (*ptrMN2Reg, 19U, 16U, divider);

    /* program Fractional Multiplier */
    *ptrFracMReg = CSL_insert32 (*ptrFracMReg, 17U, 0U, fracMultiplier);
}


/**
 *  @b Description
 *  @n
 *      This API Configure the Dividers and Multipliers for Core/DSP PLL
 *
 *  @param[in]  trimVal
 *      Trim value for the PLL
 *  @retval     None
 */
static void configurePllCore (uint16_t trimVal)
{
    volatile uint32_t *ptrClkCtrl, *ptrTenable, *ptrTenableDiv, *ptrPllStatus;
    CSL_mss_toprcmRegs *ptrTopRCMRegs;
    uint8_t phaseLockStatus;

    /* Core PLL settings */
    ptrTopRCMRegs = CSL_TopRCM_getBaseAddress ();

    ptrClkCtrl    = &(ptrTopRCMRegs->PLL_CORE_CLKCTRL);
    ptrTenable    = &(ptrTopRCMRegs->PLL_CORE_TENABLE);
    ptrTenableDiv = &(ptrTopRCMRegs->PLL_CORE_TENABLEDIV);
    ptrPllStatus  = &(ptrTopRCMRegs->PLL_CORE_STATUS);

    /* update the Clock control setting */
    // APPLJ-1 Setting
    // CLOCKOUT = M/(N+1) * CLKINP * (1/M2)  =  0x7d0/(39+1) * 40 * (1/1) = 2G
    // GEL file equivalent 
    //HW_WR_REG32(CSL_MSS_TOPRCM_U_BASE+PLL_CORE_M2NDIV     , 0x10027);      //M2NDIV_M2[22:16] = 1 , M2NDIV_N[7:0] = 0x27
    //HW_WR_REG32(CSL_MSS_TOPRCM_U_BASE+PLL_CORE_MN2DIV     , 0x107d0);      //MN2DIV_N2[19:16] = 1 , MN2DIV_M[11:0] = 0x7d0
    //HW_WR_REG32(CSL_MSS_TOPRCM_U_BASE+PLL_CORE_CLKCTRL    , 0x29021000);   //CLKDCOLDOEN[29] = 1,NWELLTRIM[28:24] = 9 IDLE[23] = 0 CLKDCOLDOPWDNZ[17] = 1 SELFREQDCO[12:10] = 4

    //HW_WR_REG32(CSL_MSS_TOPRCM_U_BASE+PLL_CORE_TENABLE    , 0x1);          // TENABLE    = 1
    //HW_WR_REG32(CSL_MSS_TOPRCM_U_BASE+PLL_CORE_CLKCTRL    , 0x29021001);   //+TINTZ[0]   = 1
    //HW_WR_REG32(CSL_MSS_TOPRCM_U_BASE+PLL_CORE_TENABLE    , 0x0);          // TENABLE    = 0
    //HW_WR_REG32(CSL_MSS_TOPRCM_U_BASE+PLL_CORE_TENABLEDIV , 0x1);          // TENABLEDIV = 1
    //HW_WR_REG32(CSL_MSS_TOPRCM_U_BASE+PLL_CORE_TENABLEDIV , 0x0);          // TENABLEDIV = 0

    /* program CLKDCOLDOEN[29] = 1, IDLE[23] = 0, CLKDCOLDOPWDNZ[17] = 1, SELFREQDCO[12:10] = 4 */
    *ptrClkCtrl = CSL_insert8 (*ptrClkCtrl, 29U, 29U, 0x1U);
    *ptrClkCtrl = CSL_insert8 (*ptrClkCtrl, 23U, 23U, 0x0U);
    *ptrClkCtrl = CSL_insert8 (*ptrClkCtrl, 17U, 17U, 0x1U);
    *ptrClkCtrl = CSL_insert8 (*ptrClkCtrl, 12U, 10U, 0x4U);

    /* Write Nwell Trim Value */
    *ptrClkCtrl = CSL_insert8 (*ptrClkCtrl, 28U, 24U, ((uint8_t) trimVal & 0x1FU));

    /* Soft reset Pll */
    *ptrClkCtrl = CSL_insert8 (*ptrClkCtrl, 0U, 0U, 0x0U);

    /* TENABLE High */
    *ptrTenable = CSL_insert8 (*ptrTenable, 0U, 0U, 0x1U);

    /* out of reset Pll */
    *ptrClkCtrl = CSL_insert8 (*ptrClkCtrl, 0U, 0U, 0x1U);

    /* TENABLE Low */
    *ptrTenable = CSL_insert8 (*ptrTenable, 0U, 0U, 0x0U);

    /* TENABLEDIV High */
    *ptrTenableDiv = CSL_insert8 (*ptrTenableDiv, 0U, 0U, 0x1U);

    /* TENABLEDIV Low */
    *ptrTenableDiv = CSL_insert8 (*ptrTenableDiv, 0U, 0U, 0x0U);

    /* wait for the Phase lock for Core/DSP PLL */
    //GEL File equivalent
    // APPLJ-1  :  loop check to PLLLOCK DONE
    //lock_status = HW_RD_REG32(CSL_MSS_TOPRCM_U_BASE+PLL_CORE_STATUS); //PHASELOCK[10]
    //while(0x400 != (lock_status & 0x400)) {
    //   lock_status = HW_RD_REG32(CSL_MSS_TOPRCM_U_BASE+PLL_CORE_STATUS); //PHASELOCK[10]
    //}

    do
    {
        phaseLockStatus = CSL_extract8 (*ptrPllStatus, 10U, 10U);
    }while(phaseLockStatus != 1U);
}

/**
 *  @b Description
 *  @n
 *      This API Configure the Dividers and Multipliers for Core/DSP PLL
 *
 *  @param[in]  trimVal
 *      Trim value for the PLL
 *  @retval     None
 */
static void configurePllDsp (uint16_t trimVal)
{
    volatile uint32_t *ptrClkCtrl, *ptrTenable, *ptrTenableDiv, *ptrPllStatus;
    CSL_mss_toprcmRegs *ptrTopRCMRegs;
    uint8_t phaseLockStatus;

    /* Core PLL settings */
    ptrTopRCMRegs = CSL_TopRCM_getBaseAddress ();

    ptrClkCtrl    = &(ptrTopRCMRegs->PLL_DSP_CLKCTRL);
    ptrTenable    = &(ptrTopRCMRegs->PLL_DSP_TENABLE);
    ptrTenableDiv = &(ptrTopRCMRegs->PLL_DSP_TENABLEDIV);
    ptrPllStatus  = &(ptrTopRCMRegs->PLL_DSP_STATUS);

    /* update the Clock control setting */
    /* program CLKDCOLDOEN[29] = 1, IDLE[23] = 0, CLKDCOLDOPWDNZ[17] = 1, SELFREQDCO[12:10] = 4 */
    /* GEL file equivalent */
    //HW_WR_REG32(CSL_MSS_TOPRCM_U_BASE+PLL_DSP_CLKCTRL    , 0x29021000);   //CLKDCOLDOEN[29] = 1,NWELLTRIM[28:24] = 9 IDLE[23] = 0 CLKDCOLDOPWDNZ[17] = 1 SELFREQDCO[12:10] = 4

    //HW_WR_REG32(CSL_MSS_TOPRCM_U_BASE+PLL_DSP_TENABLE    , 0x1);          // TENABLE    = 1
    //HW_WR_REG32(CSL_MSS_TOPRCM_U_BASE+PLL_DSP_CLKCTRL    , 0x29021001);   //+TINTZ[0]   = 1
    //HW_WR_REG32(CSL_MSS_TOPRCM_U_BASE+PLL_DSP_TENABLE    , 0x0);          // TENABLE    = 0
    //HW_WR_REG32(CSL_MSS_TOPRCM_U_BASE+PLL_DSP_TENABLEDIV , 0x1);          // TENABLEDIV = 1
    //HW_WR_REG32(CSL_MSS_TOPRCM_U_BASE+PLL_DSP_TENABLEDIV , 0x0);          // TENABLEDIV = 0

    *ptrClkCtrl = CSL_insert8 (*ptrClkCtrl, 29U, 29U, 0x1U);
    *ptrClkCtrl = CSL_insert8 (*ptrClkCtrl, 23U, 23U, 0x0U);
    *ptrClkCtrl = CSL_insert8 (*ptrClkCtrl, 17U, 17U, 0x1U);
    *ptrClkCtrl = CSL_insert8 (*ptrClkCtrl, 12U, 10U, 0x4U);

    /* Write Nwell Trim Value */
    *ptrClkCtrl = CSL_insert8 (*ptrClkCtrl, 28U, 24U, ((uint8_t) trimVal & 0x1FU));

    /* Soft reset Pll */
    *ptrClkCtrl = CSL_insert8 (*ptrClkCtrl, 0U, 0U, 0x0U);

    /* TENABLE High */
    *ptrTenable = CSL_insert8 (*ptrTenable, 0U, 0U, 0x1U);

    /* out of reset Pll */
    *ptrClkCtrl = CSL_insert8 (*ptrClkCtrl, 0U, 0U, 0x1U);

    /* TENABLE Low */
    *ptrTenable = CSL_insert8 (*ptrTenable, 0U, 0U, 0x0U);

    /* TENABLEDIV High */
    *ptrTenableDiv = CSL_insert8 (*ptrTenableDiv, 0U, 0U, 0x1U);

    /* TENABLEDIV Low */
    *ptrTenableDiv = CSL_insert8 (*ptrTenableDiv, 0U, 0U, 0x0U);

    /* wait for the Phase lock for Core/DSP PLL */
    //GEL File equivalent
    // APPLJ-2 : loop check to PLLLOCK DONE
    //lock_status = HW_RD_REG32(CSL_MSS_TOPRCM_U_BASE+PLL_DSP_STATUS); //PHASELOCK[10]
    //while(0x400 != (lock_status & 0x400)) {
    //   lock_status = HW_RD_REG32(CSL_MSS_TOPRCM_U_BASE+PLL_DSP_STATUS); //PHASELOCK[10]
    //}

    do
    {
        phaseLockStatus = CSL_extract8 (*ptrPllStatus, 10U, 10U);
    }while(phaseLockStatus != 1U);
}


/**
 *  @b Description
 *  @n
 *      This API Configure the Dividers and Multipliers for Core/DSP PLL
 *
 *  @param[in]  trimVal
 *      Trim value for the PLL
 *  @retval     None
 */
static void configurePllPer (uint16_t trimVal)
{
    volatile uint32_t *ptrClkCtrl, *ptrTenable, *ptrTenableDiv, *ptrPllStatus;
    CSL_mss_toprcmRegs *ptrTopRCMRegs;
    uint8_t phaseLockStatus;

    /* Core PLL settings */
    ptrTopRCMRegs = CSL_TopRCM_getBaseAddress ();

    ptrClkCtrl    = &(ptrTopRCMRegs->PLL_PER_CLKCTRL);
    ptrTenable    = &(ptrTopRCMRegs->PLL_PER_TENABLE);
    ptrTenableDiv = &(ptrTopRCMRegs->PLL_PER_TENABLEDIV);
    ptrPllStatus  = &(ptrTopRCMRegs->PLL_PER_STATUS);

    /* update the Clock control setting */
    /* program CLKDCOLDOEN[29] = 1, IDLE[23] = 0, CLKDCOLDOPWDNZ[17] = 1, SELFREQDCO[12:10] = 4 */
    /* GEL file equivalent */
    //HW_WR_REG32(CSL_MSS_TOPRCM_U_BASE+PLL_PER_CLKCTRL    , 0x29021000);   //CLKDCOLDOEN[29] = 1,NWELLTRIM[28:24] = 9 IDLE[23] = 0 CLKDCOLDOPWDNZ[17] = 1 SELFREQDCO[12:10] = 4

    //HW_WR_REG32(CSL_MSS_TOPRCM_U_BASE+PLL_PER_TENABLE    , 0x1);          // TENABLE    = 1
    //HW_WR_REG32(CSL_MSS_TOPRCM_U_BASE+PLL_PER_CLKCTRL    , 0x29021001);   //+TINTZ[0]   = 1
    //HW_WR_REG32(CSL_MSS_TOPRCM_U_BASE+PLL_PER_TENABLE    , 0x0);          // TENABLE    = 0
    //HW_WR_REG32(CSL_MSS_TOPRCM_U_BASE+PLL_PER_TENABLEDIV , 0x1);          // TENABLEDIV = 1
    //HW_WR_REG32(CSL_MSS_TOPRCM_U_BASE+PLL_PER_TENABLEDIV , 0x0);          // TENABLEDIV = 0

    *ptrClkCtrl = CSL_insert8 (*ptrClkCtrl, 29U, 29U, 0x1U);
    *ptrClkCtrl = CSL_insert8 (*ptrClkCtrl, 23U, 23U, 0x0U);
    *ptrClkCtrl = CSL_insert8 (*ptrClkCtrl, 17U, 17U, 0x1U);
    *ptrClkCtrl = CSL_insert8 (*ptrClkCtrl, 12U, 10U, 0x4U);

    /* Write Nwell Trim Value */
    *ptrClkCtrl = CSL_insert8 (*ptrClkCtrl, 28U, 24U, ((uint8_t) trimVal & 0x1FU));

    /* Soft reset Pll */
    *ptrClkCtrl = CSL_insert8 (*ptrClkCtrl, 0U, 0U, 0x0U);

    /* TENABLE High */
    *ptrTenable = CSL_insert8 (*ptrTenable, 0U, 0U, 0x1U);

    /* out of reset Pll */
    *ptrClkCtrl = CSL_insert8 (*ptrClkCtrl, 0U, 0U, 0x1U);

    /* TENABLE Low */
    *ptrTenable = CSL_insert8 (*ptrTenable, 0U, 0U, 0x0U);

    /* TENABLEDIV High */
    *ptrTenableDiv = CSL_insert8 (*ptrTenableDiv, 0U, 0U, 0x1U);

    /* TENABLEDIV Low */
    *ptrTenableDiv = CSL_insert8 (*ptrTenableDiv, 0U, 0U, 0x0U);

    /* wait for the Phase lock for Core/DSP PLL */
    //GEL File equivalent
    // APPLJ-3 : loop check to PLLLOCK DONE
    //lock_status = HW_RD_REG32(CSL_MSS_TOPRCM_U_BASE+PLL_PER_STATUS); //PHASELOCK[10]
    //while(0x400 != (lock_status & 0x400)) {
    //   lock_status = HW_RD_REG32(CSL_MSS_TOPRCM_U_BASE+PLL_PER_STATUS); //PHASELOCK[10]
    //}

    do
    {
        phaseLockStatus = CSL_extract8 (*ptrPllStatus, 10U, 10U);
    }while(phaseLockStatus != 1U);
}

/**
 *  @b Description
 *  @n
 *      This API returns the XTAL Frequency
 *
 *  @retval     XTAL Frequency
 */
static Rcm_XtalFreqId SBL_RcmGetXTALFrequency(void)
{
    CSL_mss_toprcmRegs *ptrTopRCMRegs;
    CSL_top_ctrlRegs* ptrTopCtrlRegs;
    uint32_t xtalRegVal;
    Rcm_XtalFreqId freq;
    uint8_t xtalFreqScale;

    ptrTopRCMRegs = CSL_TopRCM_getBaseAddress ();
    ptrTopCtrlRegs = CSL_TopCtrl_getBaseAddress ();

    /* read the register bits corresponding to XTAL Frequency */
    xtalRegVal = CSL_extract32 (ptrTopRCMRegs->ANA_REG_WU_MODE_REG_LOWV, 6U, 5U);
    /* read the register bits corresponding to XTAL Frequency Scale */
    xtalFreqScale = CSL_TopCtrl_readXtalFreqScale (ptrTopCtrlRegs);

    if(xtalFreqScale == 1U)
    {
        if (xtalRegVal == 0U)
        {
            freq = RCM_XTAL_FREQID_CLK_20MHZ;
        }
        else if (xtalRegVal == 1U)
        {
            freq = RCM_XTAL_FREQID_CLK_22p5792MHZ;
        }
        else if (xtalRegVal == 2U)
        {
            freq = RCM_XTAL_FREQID_CLK_24p576MHZ;
        }
        else
        {
            freq = RCM_XTAL_FREQID_CLK_25MHZ;
        }
    }
    else
    {
        if (xtalRegVal == 0U)
        {
            freq = RCM_XTAL_FREQID_CLK_40MHZ;
        }
        else if (xtalRegVal == 1U)
        {
            freq = RCM_XTAL_FREQID_CLK_45p1584MHZ;
        }
        else if (xtalRegVal == 2U)
        {
            freq = RCM_XTAL_FREQID_CLK_49p152MHZ;
        }
        else
        {
            freq = RCM_XTAL_FREQID_CLK_50MHZ;
        }
    }

    return (freq);

}


/**
 *  @b Description
 *  @n
 *      The function is read the Efuse row config for Core ADPLL Trim Value
 *
 *  @param[in] ptrTopCtrlRegs
 *      Pointer to the MSS TOP CTRL Registers base address
 *
 *  @retval
 *      Efuse Row Value for Core ADPLL Trim Value.
 */
uint16_t CSL_TopCtrl_readCoreADPLLTrimEfuse (const CSL_top_ctrlRegs* ptrTopCtrlRegs)
{
    return (CSL_extract16 (ptrTopCtrlRegs->EFUSE1_ROW_40, \
                           COREADPLLTRIMCONFIG_END_BIT, \
                           COREADPLLTRIMCONFIG_START_BIT));
}



/**
 *  @b Description
 *  @n
 *      The function is read the Efuse row config for Core ADPLL Trim Value
 *
 *  @param[in] ptrTopCtrlRegs
 *      Pointer to the MSS TOP CTRL Registers base address
 *
 *  @retval
 *      Efuse Row Value for Core ADPLL Trim Value.
 */
uint16_t CSL_TopCtrl_readDspADPLLTrimEfuse (const CSL_top_ctrlRegs* ptrTopCtrlRegs)
{
    return (CSL_extract16 (ptrTopCtrlRegs->EFUSE1_ROW_40, \
                           DSPADPLLTRIMCONFIG_END_BIT, \
                           DSPADPLLTRIMCONFIG_START_BIT));
}

/**
 *  @b Description
 *  @n
 *      The function is read the Efuse row config for Core ADPLL Trim Value
 *
 *  @param[in] ptrTopCtrlRegs
 *      Pointer to the MSS TOP CTRL Registers base address
 *
 *  @retval
 *      Efuse Row Value for Core ADPLL Trim Value.
 */
uint16_t CSL_TopCtrl_readPerADPLLTrimEfuse (const CSL_top_ctrlRegs* ptrTopCtrlRegs)
{
    return (CSL_extract16 (ptrTopCtrlRegs->EFUSE1_ROW_41,
                           PERADPLLTRIMCONFIG_END_BIT,
                           PERADPLLTRIMCONFIG_START_BIT));
}

/**
 *  @b Description
 *  @n
 *      This function returns the Core ADPLL Trim value
 *
 *  \ingroup DRIVER_RCM_FUNCTIONS
 *
 *  @retval     coreADPLLTrimVal
 *                  Core ADPLL Trim Value
 */
static uint16_t SBL_RcmGetCoreTrimVal(void)
{
    CSL_top_ctrlRegs *ptrTopCtrlRegs;
    uint8_t coreADPLLValid;
    uint16_t coreADPLLTrimVal;

    ptrTopCtrlRegs = CSL_TopCtrl_getBaseAddress ();

    coreADPLLValid = CSL_TopCtrl_readCoreADPLLTrimValidEfuse (ptrTopCtrlRegs);

    if(coreADPLLValid == 1U)
    {
        coreADPLLTrimVal = CSL_TopCtrl_readCoreADPLLTrimEfuse (ptrTopCtrlRegs);
    }
    else
    {
        coreADPLLTrimVal = (uint16_t) RCM_CORE_ADPLL_DEFAULT_VALUE;
    }
    return (coreADPLLTrimVal);
}

/**
 *  @b Description
 *  @n
 *      This function returns the Core ADPLL Trim value
 *
 *  \ingroup DRIVER_RCM_FUNCTIONS
 *
 *  @retval     coreADPLLTrimVal
 *                  Core ADPLL Trim Value
 */
static uint16_t SBL_RcmGetDspTrimVal(void)
{
    CSL_top_ctrlRegs *ptrTopCtrlRegs;
    uint8_t dspADPLLValid;
    uint16_t dspADPLLTrimVal;

    ptrTopCtrlRegs = CSL_TopCtrl_getBaseAddress ();

    dspADPLLValid = CSL_TopCtrl_readDspADPLLTrimValidEfuse (ptrTopCtrlRegs);

    if(dspADPLLValid == 1U)
    {
        dspADPLLTrimVal = CSL_TopCtrl_readDspADPLLTrimEfuse (ptrTopCtrlRegs);
    }
    else
    {
        dspADPLLTrimVal = (uint16_t) RCM_DSP_ADPLL_DEFAULT_VALUE;
    }
    return (dspADPLLTrimVal);
}


/**
 *  @b Description
 *  @n
 *      This function returns the Core ADPLL Trim value
 *
 *  \ingroup DRIVER_RCM_FUNCTIONS
 *
 *  @retval     coreADPLLTrimVal
 *                  Core ADPLL Trim Value
 */
static uint16_t SBL_RcmGetPerTrimVal(void)
{
    CSL_top_ctrlRegs *ptrTopCtrlRegs;
    uint8_t perADPLLValid;
    uint16_t perADPLLTrimVal;

    ptrTopCtrlRegs = CSL_TopCtrl_getBaseAddress ();

    perADPLLValid = CSL_TopCtrl_readPerADPLLTrimValidEfuse (ptrTopCtrlRegs);

    if(perADPLLValid == 1U)
    {
        perADPLLTrimVal = CSL_TopCtrl_readPerADPLLTrimEfuse (ptrTopCtrlRegs);
    }
    else
    {
        perADPLLTrimVal = (uint16_t) RCM_PER_ADPLL_DEFAULT_VALUE;
    }
    return (perADPLLTrimVal);
}

/**
 *  @b Description
 *  @n
 *      This API Sets up the core PLL configuration switches the R5 to PLL clock.
 *
 *  \ingroup DRIVER_RCM_FUNCTIONS
 *
 *  @retval     None
 */
void SBL_RcmCoreApllConfig(Rcm_PllFoutFreqId outFreqId, Rcm_PllHsDivOutConfig *hsDivCfg)
{
    Rcm_XtalFreqId XTALFreq;
    uint16_t coreTrimVal;
    CSL_mss_toprcmRegs *ptrTopRCMRegs;
    Rcm_ADPLLJConfig_t const * adplljCfg;
    uint32_t hsDivOutRegVal;
    uint32_t Fout;

    Fout = SBL_FREQ_MHZ2HZ(gPLLFreqId2FOutMap[outFreqId]);
    /* read the Core ADPLL trim value */
    coreTrimVal = SBL_RcmGetCoreTrimVal ();
    /* Core PLL settings */
    ptrTopRCMRegs = CSL_TopRCM_getBaseAddress ();

    /* read the XTAL Frequency */
    XTALFreq = SBL_RcmGetXTALFrequency ();

    /* program PLL dividers and multipliers. The value are taken from  TPR12_ADPLLJ_Settings_1p0.xlsx */
    adplljCfg = SBL_getADPLLJConfig(gXTALInfo[XTALFreq].Finp, outFreqId);
    DebugP_assert(adplljCfg != NULL);

    if (adplljCfg != NULL)
    {
        if (gXTALInfo[XTALFreq].div2flag == false)
        {
            progPllCoreDivider (adplljCfg->N, 
                                0U /* N2 divider for bypass */, 
                                adplljCfg->M, 
                                adplljCfg->M2, 
                                adplljCfg->FracM);
        }
        else
        {
            uint32_t N;

            DebugP_assert(((adplljCfg->N + 1) % 2) == 0);
            /* Input XTAL freq is half. Divide input divider by 2 to get same output freq */
            N = ((adplljCfg->N + 1) / 2) - 1;
            progPllCoreDivider (N, 
                                0U /* N2 divider for bypass */, 
                                adplljCfg->M, 
                                adplljCfg->M2, 
                                adplljCfg->FracM);
        }
        /* Configure and Lock Core PLL */
        configurePllCore (coreTrimVal);

        /* Derive Clocks */
        /* TPR12_Ch08_Clock_Arch_0p91 is used as reference for below settings */
        /* Set clock divider values from Core PLL*/
        /* 400Mhz */
        if (hsDivCfg->hsdivOutEnMask & RCM_PLL_HSDIV_OUTPUT_ENABLE_0)
        {
            DebugP_assert((Fout % hsDivCfg->hsDivOutFreqHz[RCM_PLL_HSDIV_OUTPUT_IDX0]) == 0);
            hsDivOutRegVal = Fout / hsDivCfg->hsDivOutFreqHz[RCM_PLL_HSDIV_OUTPUT_IDX0];
            hsDivOutRegVal--;
            ptrTopRCMRegs->PLL_CORE_HSDIVIDER_CLKOUT0 = CSL_insert8 (ptrTopRCMRegs->PLL_CORE_HSDIVIDER_CLKOUT0, 4U, 0U, hsDivOutRegVal);
        }
        if (hsDivCfg->hsdivOutEnMask & RCM_PLL_HSDIV_OUTPUT_ENABLE_1)
        {
            DebugP_assert((Fout % hsDivCfg->hsDivOutFreqHz[RCM_PLL_HSDIV_OUTPUT_IDX1]) == 0);
            hsDivOutRegVal = Fout / hsDivCfg->hsDivOutFreqHz[RCM_PLL_HSDIV_OUTPUT_IDX1];
            hsDivOutRegVal--;
            ptrTopRCMRegs->PLL_CORE_HSDIVIDER_CLKOUT1 = CSL_insert8 (ptrTopRCMRegs->PLL_CORE_HSDIVIDER_CLKOUT1, 4U, 0U, hsDivOutRegVal);
        }
        if (hsDivCfg->hsdivOutEnMask & RCM_PLL_HSDIV_OUTPUT_ENABLE_2)
        {
            DebugP_assert((Fout % hsDivCfg->hsDivOutFreqHz[RCM_PLL_HSDIV_OUTPUT_IDX2]) == 0);
            hsDivOutRegVal = Fout / hsDivCfg->hsDivOutFreqHz[RCM_PLL_HSDIV_OUTPUT_IDX2];
            hsDivOutRegVal--;
            ptrTopRCMRegs->PLL_CORE_HSDIVIDER_CLKOUT2 = CSL_insert8 (ptrTopRCMRegs->PLL_CORE_HSDIVIDER_CLKOUT2, 4U, 0U, hsDivOutRegVal);
        }
        /* Core PLL output 3 not used.WIll not configure */
        DebugP_assert((hsDivCfg->hsdivOutEnMask & RCM_PLL_HSDIV_OUTPUT_ENABLE_3) == 0);
        /* Generate Trigger to latch these values */
        ptrTopRCMRegs->PLL_CORE_HSDIVIDER         = CSL_insert8 (ptrTopRCMRegs->PLL_CORE_HSDIVIDER, 2U, 2U, 0x1U);
        ptrTopRCMRegs->PLL_CORE_HSDIVIDER         = CSL_insert8 (ptrTopRCMRegs->PLL_CORE_HSDIVIDER, 2U, 2U, 0x0U);

        /* Ungate the clocks */
        if (hsDivCfg->hsdivOutEnMask & RCM_PLL_HSDIV_OUTPUT_ENABLE_0)
        {
            ptrTopRCMRegs->PLL_CORE_HSDIVIDER_CLKOUT0 = CSL_insert8 (ptrTopRCMRegs->PLL_CORE_HSDIVIDER_CLKOUT0, 8U, 8U, 0x1U);
        }
        if (hsDivCfg->hsdivOutEnMask & RCM_PLL_HSDIV_OUTPUT_ENABLE_1)
        {
            ptrTopRCMRegs->PLL_CORE_HSDIVIDER_CLKOUT1 = CSL_insert8 (ptrTopRCMRegs->PLL_CORE_HSDIVIDER_CLKOUT1, 8U, 8U, 0x1U);
        }
        if (hsDivCfg->hsdivOutEnMask & RCM_PLL_HSDIV_OUTPUT_ENABLE_2)
        {
            ptrTopRCMRegs->PLL_CORE_HSDIVIDER_CLKOUT2 = CSL_insert8 (ptrTopRCMRegs->PLL_CORE_HSDIVIDER_CLKOUT2, 8U, 8U, 0x1U);
        }

    }

    return;
}

static uint32_t SBL_RcmGetCR5SysclkInFrequency(void)
{
    uint32_t clkFreq = 0U;
    Rcm_ClkSrcInfo const * clkSrcInfo;
    Rcm_XtalFreqId clkFreqId;
    uint32_t Finp;


    clkSrcInfo = &gCR5ClkSrcInfoMap;
    clkFreqId = SBL_RcmGetXTALFrequency ();

    Finp = gXTALInfo[clkFreqId].Finp;
    clkFreq = SBL_RcmGetCoreHSDivOut(Finp, gXTALInfo[clkFreqId].div2flag, clkSrcInfo->hsDivOut);

    return (clkFreq);

}

static uint32_t SBL_RcmGetModuleClkDivVal(uint32_t inFreq, uint32_t outFreq)
{
    uint32_t moduleClkDivVal;

    DebugP_assert((inFreq % outFreq) == 0);
    moduleClkDivVal = inFreq / outFreq;
    moduleClkDivVal--;
    return moduleClkDivVal;
}

static uint32_t SBL_RcmGetModuleClkDivRegVal(uint32_t moduleClkDivVal)
{
    uint32_t moduleClkDivRegVal;

    moduleClkDivRegVal = (moduleClkDivVal & 0xF) | ((moduleClkDivVal & 0xF) << 4) | ((moduleClkDivVal & 0xF) << 8);
    return moduleClkDivRegVal;
}

static uint32_t SBL_RcmGetModuleClkDivFromRegVal(uint32_t moduleClkDivRegVal)
{
    uint32_t moduleClkDivVal;

    moduleClkDivVal = ((moduleClkDivRegVal & 0xF) + 1);
    return moduleClkDivVal;
}

void SBL_RcmSetCR5SysClock(uint32_t cr5FreqHz, uint32_t sysClkFreqHz)
{
    CSL_mss_toprcmRegs *ptrTopRCMRegs;
    uint32_t Finp;
    uint32_t moduleClkDivVal;

    Finp = SBL_RcmGetCR5SysclkInFrequency();
    moduleClkDivVal = SBL_RcmGetModuleClkDivVal(Finp, cr5FreqHz);

    ptrTopRCMRegs = CSL_TopRCM_getBaseAddress ();

    /* Divide by 1 to get the R5 Core Clock */
    ptrTopRCMRegs->MSS_CR5_DIV_VAL = CSL_insert16 (ptrTopRCMRegs->MSS_CR5_DIV_VAL, 11U, 0U, SBL_RcmGetModuleClkDivRegVal(moduleClkDivVal));

    /* Divide by 2 to get the VCLK */
    moduleClkDivVal = SBL_RcmGetModuleClkDivVal(Finp, sysClkFreqHz);

    ptrTopRCMRegs->SYS_CLK_DIV_VAL = CSL_insert16 (ptrTopRCMRegs->SYS_CLK_DIV_VAL, 11U, 0U, SBL_RcmGetModuleClkDivRegVal(moduleClkDivVal));

    /* Select CLKOUT2 as clock for R5 Core */
    ptrTopRCMRegs->MSS_CR5_CLK_SRC_SEL = CSL_insert16 (ptrTopRCMRegs->MSS_CR5_CLK_SRC_SEL, 11U, 0U, gCR5ClkSrcValMap[Rcm_CR5ClockSource_DPLL_CORE_HSDIV0_CLKOUT2]);
}

uint32_t SBL_RcmGetCR5Freq(void)
{
    uint32_t Finp;
    uint32_t moduleClkDivRegVal;
    uint32_t clkDivVal;
    CSL_mss_toprcmRegs *ptrTopRCMRegs;

    ptrTopRCMRegs = CSL_TopRCM_getBaseAddress ();
    Finp = SBL_RcmGetCR5SysclkInFrequency();
    moduleClkDivRegVal = ptrTopRCMRegs->MSS_CR5_DIV_VAL;
    clkDivVal = SBL_RcmGetModuleClkDivFromRegVal(moduleClkDivRegVal);
    return (Finp / clkDivVal);
}

/**
 *  @b Description
 *  @n
 *      This API programs the Clock Source Selection and Clock divider values
 *      for a specified peripheral Id.
 *
 *  @param[in]  periphID
 *      Peripheral Id
 *  @param[in]  clkSource
 *      Clock Source
 *  @param[in]  clkDivisor
 *      Clock Divider Value
 *
 *  \ingroup DRIVER_RCM_FUNCTIONS
 *
 *  @retval     Rcm_Return
 */
Rcm_Return SBL_RcmSetPeripheralClock (Rcm_PeripheralId periphID,
                                      Rcm_PeripheralClockSource clkSource,
                                      uint32_t freqHz)
{
    volatile uint32_t   *ptrClkSrcReg, *ptrClkDivReg;
    uint16_t            clkSrcVal;
    uint32_t            clkDivisor;
    Rcm_Return          retVal;
    uint32_t            Finp;

    Finp = SBL_RcmGetPeripheralClockFrequency(clkSource);
    clkDivisor = SBL_RcmGetModuleClkDivVal(Finp, freqHz);
    getClkSrcAndDivReg (periphID, clkSource, &clkSrcVal, &ptrClkSrcReg, &ptrClkDivReg);

    if ((ptrClkSrcReg != NULL) && (ptrClkDivReg != NULL) && (clkSrcVal != 0x888U))
    {
        uint16_t            clkDivVal;

        /* Create the Divider Value to be programmed */
        clkDivVal = ((uint16_t)clkDivisor & 0xFU);
        clkDivVal = (clkDivVal | (clkDivVal << 4U) | (clkDivVal << 8U));

        /* Write the Divider Value */
        *ptrClkDivReg = CSL_insert16 (*ptrClkDivReg, 11U, 0U, clkDivVal);

        /* Write the Clock Source Selection Value */
        *ptrClkSrcReg = CSL_insert16 (*ptrClkSrcReg, 11U, 0U, clkSrcVal);
        retVal = Rcm_Return_SUCCESS;
    }
    else
    {
        /* Error */
        retVal = Rcm_Return_ERROR;
    }

    return (retVal);
}

/**
 *  @b Description
 *  @n
 *      This API gets the frequency configured for a specified peripheral Id.
 *
 *  @param[in]  periphID
 *      Peripheral Id
 *  @param[in]  clkSource
 *      Clock Source
 *  @param[in]  clkDivisor
 *      Clock Divider Value
 *
 *  \ingroup DRIVER_RCM_FUNCTIONS
 *
 *  @retval     Rcm_Return
 */
Rcm_Return SBL_RcmGetPeripheralFreq (Rcm_PeripheralId periphID,
                                      uint32_t *freqHz)
{
    uint32_t            clkDivisorRegVal;
    uint32_t            clkDivisor;
    Rcm_Return          retVal;
    uint32_t            Finp;
    Rcm_PeripheralClockSource clkSource;

    retVal = getClkSrcAndDivValue(periphID, &clkSource, &clkDivisorRegVal);
    DebugP_assert(retVal == Rcm_Return_SUCCESS);
    if (Rcm_Return_SUCCESS == retVal)
    {
        Finp = SBL_RcmGetPeripheralClockFrequency(clkSource);
        clkDivisor = SBL_RcmGetModuleClkDivFromRegVal(clkDivisorRegVal);
        *freqHz = Finp / clkDivisor;
    }
    return (retVal);
}

/**
 *  @b Description
 *  @n
 *      This API returns the reset cause for R5 core. It also clears the Reset
 *      casue.
 *
 *  \ingroup DRIVER_RCM_FUNCTIONS
 *
 *  @retval     Reset Cause
 */
Rcm_ResetCause SBL_RcmGetResetCause (void)
{
    CSL_mss_rcmRegs *ptrRCMRegs;
    uint16_t    resetCauseBits;
    uint8_t     resetCause = 0U;

    ptrRCMRegs = CSL_RCM_getBaseAddress ();

    /* Read the Reset Cause Register bits */
    resetCauseBits = CSL_extract16 (ptrRCMRegs->MSS_RST_STATUS, 9U, 0U);

    if (resetCauseBits == 0x0U)
    {
        resetCause = 10U;
    }
    else
    {
        while ((resetCauseBits & 0x1U) != 0x1U)
        {
            resetCauseBits = resetCauseBits >> 1U;
            resetCause = resetCause + 1U;
        }

        /* clear the reset cause */
        ptrRCMRegs->MSS_RST_CAUSE_CLR = CSL_insert8 (ptrRCMRegs->MSS_RST_CAUSE_CLR, 2U, 0U, 0x7U);
    }

    return (Rcm_ResetCause) gResetBitToResetCause[resetCause];
}


/**
 *  @b Description
 *  @n
 *      This API returns the frequency of the clock which is passed as parameter
 *  @param[in]  clkSource
 *      clock source for which frequency is requested
 *
 *  \ingroup DRIVER_RCM_FUNCTIONS
 *
 *  @retval     Clock Frequency (in Hz)
 *  @note       return value of 0 indicates clock source not available
 */
uint32_t SBL_RcmGetPeripheralClockFrequency(Rcm_PeripheralClockSource clkSource)
{
    uint32_t clkFreq = 0U;
    Rcm_ClkSrcInfo const * clkSrcInfo;
    Rcm_XtalFreqId clkFreqId;
    uint32_t Finp;


    clkSrcInfo = &gPeripheralClkSrcInfoMap[clkSource];
    clkFreqId = SBL_RcmGetXTALFrequency ();
    switch (clkSrcInfo->pllId)
    {
        case RCM_PLLID_CORE:
        {
            Finp = gXTALInfo[clkFreqId].Finp;
            clkFreq = SBL_RcmGetCoreHSDivOut(Finp, gXTALInfo[clkFreqId].div2flag, clkSrcInfo->hsDivOut);
            if (clkSource == Rcm_PeripheralClockSource_SYS_CLK)
            {
                CSL_mss_toprcmRegs *ptrTopRCMRegs;
                uint32_t sysClkDiv;

                /* Core PLL settings */
                ptrTopRCMRegs = CSL_TopRCM_getBaseAddress ();
                sysClkDiv = CSL_FEXT(ptrTopRCMRegs->SYS_CLK_DIV_VAL, MSS_TOPRCM_SYS_CLK_DIV_VAL_SYS_CLK_DIV_VAL_CLKDIV);
                sysClkDiv = (sysClkDiv & 0xF) + 1;
                clkFreq = clkFreq / sysClkDiv;
            }
            break;
        }
        case RCM_PLLID_DSS:
        {
            Finp = gXTALInfo[clkFreqId].Finp;
            clkFreq = SBL_RcmGetDSSHSDivOut(Finp, gXTALInfo[clkFreqId].div2flag, clkSrcInfo->hsDivOut);
            break;
        }
        case RCM_PLLID_PER:
        {
            Finp = gXTALInfo[clkFreqId].Finp;
            clkFreq = SBL_RcmGetPerHSDivOut(Finp, gXTALInfo[clkFreqId].div2flag, clkSrcInfo->hsDivOut);
            break;
        }
        case RCM_PLLID_XTALCLK:
        {
            Finp = gXTALInfo[clkFreqId].Finp;
            if (gXTALInfo[clkFreqId].div2flag)
            {
                clkFreq = Finp/2;
            }
            else
            {
                clkFreq = Finp;
            }
            break;
        }
        default:
        {
            DebugP_assert(FALSE);
            break;
        }
    }
    return (clkFreq);

}


/**
 *  @b Description
 *  @n
 *      This API starts the memory initialization for TCMA and L2 memory
 *
 *  \ingroup DRIVER_RCM_FUNCTIONS
 *
 *  @retval     None
 */
void SBL_RcmStartMeminitTCMA(void)
{
    CSL_mss_ctrlRegs *mssCtrl = CSL_MSS_CTRL_getBaseAddress ();

    /* Check MEMINIT STATUS is zero to confirm no inprogress MEM INIT */
    while (CSL_FEXT(mssCtrl->MSS_ATCM_MEM_INIT_STATUS, MSS_CTRL_MSS_ATCM_MEM_INIT_STATUS_MSS_ATCM_MEM_INIT_STATUS_MEM_STATUS) != 0);

    /* Clear MEMINIT DONE before initiating MEMINIT */
    CSL_FINS(mssCtrl->MSS_ATCM_MEM_INIT_DONE, MSS_CTRL_MSS_ATCM_MEM_INIT_DONE_MSS_ATCM_MEM_INIT_DONE_MEM_INIT_DONE, 1);
    while (CSL_FEXT(mssCtrl->MSS_ATCM_MEM_INIT_DONE, MSS_CTRL_MSS_ATCM_MEM_INIT_DONE_MSS_ATCM_MEM_INIT_DONE_MEM_INIT_DONE) != 0);

    CSL_FINS(mssCtrl->MSS_ATCM_MEM_INIT, MSS_CTRL_MSS_ATCM_MEM_INIT_MSS_ATCM_MEM_INIT_MEM_INIT, 1);

}

#if defined (SOC_AWR294X)
/**
 *  @b Description
 *  @n
 *      This API starts the memory initialization for RSS TCM memory
 *
 *  \ingroup DRIVER_RCM_FUNCTIONS
 *
 *  @retval     None
 */
void SBL_RcmStartMeminitTCMBSS(void)
{    
    CSL_rss_ctrlRegs *rssCtrl = CSL_RSS_CTRL_getBaseAddress ();

    /* Check MEMINIT STATUS is zero to confirm no inprogress MEM INIT */
    while (CSL_FEXT(rssCtrl->BSS_TCM_MEMINIT_STATUS, MSS_CTRL_MSS_ATCM_MEM_INIT_STATUS_MSS_ATCM_MEM_INIT_STATUS_MEM_STATUS) != 0);

    /* Clear MEMINIT DONE before initiating MEMINIT */
    CSL_FINS(rssCtrl->BSS_TCM_MEMINIT_DONE, MSS_CTRL_MSS_ATCM_MEM_INIT_DONE_MSS_ATCM_MEM_INIT_DONE_MEM_INIT_DONE, 1);
    while (CSL_FEXT(rssCtrl->BSS_TCM_MEMINIT_DONE, MSS_CTRL_MSS_ATCM_MEM_INIT_DONE_MSS_ATCM_MEM_INIT_DONE_MEM_INIT_DONE) != 0);

    CSL_FINS(rssCtrl->BSS_TCM_MEMINIT, MSS_CTRL_MSS_ATCM_MEM_INIT_MSS_ATCM_MEM_INIT_MEM_INIT, 1);
}

/**
 *  @b Description
 *  @n
 *      This API starts the memory initialization for RSS Static memory
 *
 *  \ingroup DRIVER_RCM_FUNCTIONS
 *
 *  @retval     None
 */
void SBL_RcmStartMeminitStaticBSS(void)
{
    CSL_rss_proc_ctrlRegs *rssProcCtrl = CSL_RSS_PROC_CTRL_getBaseAddress ();

    /* Check MEMINIT STATUS is zero to confirm no inprogress MEM INIT */
    while (CSL_FEXT(rssProcCtrl->RSS_STATIC_MEM_MEMINIT_STATUS, MSS_CTRL_MSS_ATCM_MEM_INIT_STATUS_MSS_ATCM_MEM_INIT_STATUS_MEM_STATUS) != 0);

    /* Clear MEMINIT DONE before initiating MEMINIT */
    CSL_FINS(rssProcCtrl->RSS_STATIC_MEM_MEMINIT_DONE, MSS_CTRL_MSS_ATCM_MEM_INIT_DONE_MSS_ATCM_MEM_INIT_DONE_MEM_INIT_DONE, 1);
    while (CSL_FEXT(rssProcCtrl->RSS_STATIC_MEM_MEMINIT_DONE, MSS_CTRL_MSS_ATCM_MEM_INIT_DONE_MSS_ATCM_MEM_INIT_DONE_MEM_INIT_DONE) != 0);

    CSL_FINS(rssProcCtrl->RSS_STATIC_MEM_MEMINIT, MSS_CTRL_MSS_ATCM_MEM_INIT_MSS_ATCM_MEM_INIT_MEM_INIT, 1);
}

/**
 *  @b Description
 *  @n
 *      This API starts the memory initialization for RSS Shared memory
 *
 *  \ingroup DRIVER_RCM_FUNCTIONS
 *
 *  @retval     None
 */
void SBL_RcmStartMeminitSharedBSS(void)
{
    CSL_rss_ctrlRegs *rssCtrl = CSL_RSS_CTRL_getBaseAddress ();
    
    /* Check MEMINIT STATUS is zero to confirm no inprogress MEM INIT */
    while (CSL_FEXT(rssCtrl->RSS_SHARED_MEM_MEMINIT_STATUS, MSS_CTRL_MSS_ATCM_MEM_INIT_STATUS_MSS_ATCM_MEM_INIT_STATUS_MEM_STATUS) != 0);

    /* Clear MEMINIT DONE before initiating MEMINIT */
    CSL_FINS(rssCtrl->RSS_SHARED_MEM_MEMINIT_DONE, MSS_CTRL_MSS_ATCM_MEM_INIT_DONE_MSS_ATCM_MEM_INIT_DONE_MEM_INIT_DONE, 1);
    while (CSL_FEXT(rssCtrl->RSS_SHARED_MEM_MEMINIT_DONE, MSS_CTRL_MSS_ATCM_MEM_INIT_DONE_MSS_ATCM_MEM_INIT_DONE_MEM_INIT_DONE) != 0);

    CSL_FINS(rssCtrl->RSS_SHARED_MEM_MEMINIT, MSS_CTRL_MSS_ATCM_MEM_INIT_MSS_ATCM_MEM_INIT_MEM_INIT, 1);
}

/**
 *  @b Description
 *  @n
 *      This API configures the allocation of memory banks and sets the mode of operation
 *
 *  \ingroup DRIVER_RCM_FUNCTIONS
 *
 *  @retval     None
 */
void SBL_RcmBSSControl(void)
{    
    CSL_rss_ctrlRegs *rssCtrl = CSL_RSS_CTRL_getBaseAddress ();

    /* Allocate DSS L3 Bank as TCM for BSS */
    CSL_FINSR(rssCtrl->BSS_CONTROL, 30, 28, 0x7U);
    /* Sets FW development mode */
    CSL_FINSR(rssCtrl->BSS_CONTROL, 11, 0, 0x111U);
}

/**
 *  @b Description
 *  @n
 *      This API configures various BOOT INFO registers
 *
 *  \ingroup DRIVER_RCM_FUNCTIONS
 *
 *  @retval     None
 */
void SBL_RcmPopulateBSSControl(void)
{    
    CSL_rss_proc_ctrlRegs *rssProcCtrl = CSL_RSS_PROC_CTRL_getBaseAddress ();

    /*  Boot status register
        Bits 15:0 -> XTAL frequency in MHz as an unsigned number */
    CSL_FINSR(rssProcCtrl->RSS_CR4_BOOT_INFO_REG0, 15, 0, 0x0U);
    /*  Redundant configuration as above */
    CSL_FINSR(rssProcCtrl->RSS_CR4_BOOT_INFO_REG1, 15, 0, 0x0U);
    /* 
        Boot time Mailbox Memory configuration
        Bits 7:0 -> MSS
        Bits 15:8 -> DSS
                              MSS CR5 A	  DSS DSP
        Tx Buffer Size        512 B	      512 B
        Tx Buffer Offset      0x0000      0x0000   
    */
    CSL_FINSR(rssProcCtrl->RSS_CR4_BOOT_INFO_REG3, 15, 0, 0x8080U);
    /*  Redundant configuration as above */
    CSL_FINSR(rssProcCtrl->RSS_CR4_BOOT_INFO_REG4, 15, 0, 0x8080U);
}
#endif /* defined (SOC_AWR294X) */

/**
 *  @b Description
 *  @n
 *      This API starts the memory initialization for TCMA and L2 memory
 *
 *  \ingroup DRIVER_RCM_FUNCTIONS
 *
 *  @retval     None
 */
void SBL_RcmStartMeminitL2(void)
{
    CSL_mss_ctrlRegs*        ptrMSSCtrlRegs;

    ptrMSSCtrlRegs = CSL_MSS_CTRL_getBaseAddress ();

    /* Start the Initialization of L2 Memory */
    ptrMSSCtrlRegs->MSS_L2_MEM_INIT = CSL_insert8 (ptrMSSCtrlRegs->MSS_L2_MEM_INIT, 1U, 0U, 0x3U);

}

/**
 *  @b Description
 *  @n
 *      This API waits for Memory initialization for TCMA and L2 to complete
 *
 *  \ingroup DRIVER_RCM_FUNCTIONS
 *
 *  @retval     None
 */
void SBL_RcmWaitMeminitTCMA(void)
{
    CSL_mss_ctrlRegs*        mssCtrl;

    mssCtrl = CSL_MSS_CTRL_getBaseAddress ();

    while (CSL_FEXT(mssCtrl->MSS_ATCM_MEM_INIT_DONE, MSS_CTRL_MSS_ATCM_MEM_INIT_DONE_MSS_ATCM_MEM_INIT_DONE_MEM_INIT_DONE) != 1);
    CSL_FINS(mssCtrl->MSS_ATCM_MEM_INIT_DONE, MSS_CTRL_MSS_ATCM_MEM_INIT_DONE_MSS_ATCM_MEM_INIT_DONE_MEM_INIT_DONE, 1);
    /* Check MEMINIT STATUS is zero to confirm no inprogress MEM INIT */
    while (CSL_FEXT(mssCtrl->MSS_ATCM_MEM_INIT_STATUS, MSS_CTRL_MSS_ATCM_MEM_INIT_STATUS_MSS_ATCM_MEM_INIT_STATUS_MEM_STATUS) != 0);
    while (CSL_FEXT(mssCtrl->MSS_ATCM_MEM_INIT_DONE, MSS_CTRL_MSS_ATCM_MEM_INIT_DONE_MSS_ATCM_MEM_INIT_DONE_MEM_INIT_DONE) != 0);
}

#if defined (SOC_AWR294X)
/**
 *  @b Description
 *  @n
 *      This API waits for Memory initialization for RSS TCM to complete
 *
 *  \ingroup DRIVER_RCM_FUNCTIONS
 *
 *  @retval     None
 */
void SBL_RcmWaitMeminitTCMBSS(void)
{
    CSL_rss_ctrlRegs *rssCtrl = CSL_RSS_CTRL_getBaseAddress ();

    while (CSL_FEXT(rssCtrl->BSS_TCM_MEMINIT_DONE, MSS_CTRL_MSS_ATCM_MEM_INIT_DONE_MSS_ATCM_MEM_INIT_DONE_MEM_INIT_DONE) != 1);
    CSL_FINS(rssCtrl->BSS_TCM_MEMINIT_DONE, MSS_CTRL_MSS_ATCM_MEM_INIT_DONE_MSS_ATCM_MEM_INIT_DONE_MEM_INIT_DONE, 1);
    
    /* Check MEMINIT STATUS is zero to confirm no inprogress MEM INIT */
    while (CSL_FEXT(rssCtrl->BSS_TCM_MEMINIT_STATUS, MSS_CTRL_MSS_ATCM_MEM_INIT_STATUS_MSS_ATCM_MEM_INIT_STATUS_MEM_STATUS) != 0);
    while (CSL_FEXT(rssCtrl->BSS_TCM_MEMINIT_DONE, MSS_CTRL_MSS_ATCM_MEM_INIT_DONE_MSS_ATCM_MEM_INIT_DONE_MEM_INIT_DONE) != 0);
}

/**
 *  @b Description
 *  @n
 *      This API waits for Static Memory initialization for RSS to complete
 *
 *  \ingroup DRIVER_RCM_FUNCTIONS
 *
 *  @retval     None
 */
void SBL_RcmWaitMeminitStaticBSS(void)
{
    CSL_rss_proc_ctrlRegs *rssProcCtrl = CSL_RSS_PROC_CTRL_getBaseAddress ();
    
    while (CSL_FEXT(rssProcCtrl->RSS_STATIC_MEM_MEMINIT_DONE, MSS_CTRL_MSS_ATCM_MEM_INIT_DONE_MSS_ATCM_MEM_INIT_DONE_MEM_INIT_DONE) != 1);
    CSL_FINS(rssProcCtrl->RSS_STATIC_MEM_MEMINIT_DONE, MSS_CTRL_MSS_ATCM_MEM_INIT_DONE_MSS_ATCM_MEM_INIT_DONE_MEM_INIT_DONE, 1);
    
    /* Check MEMINIT STATUS is zero to confirm no inprogress MEM INIT */
    while (CSL_FEXT(rssProcCtrl->RSS_STATIC_MEM_MEMINIT_STATUS, MSS_CTRL_MSS_ATCM_MEM_INIT_STATUS_MSS_ATCM_MEM_INIT_STATUS_MEM_STATUS) != 0);
    while (CSL_FEXT(rssProcCtrl->RSS_STATIC_MEM_MEMINIT_DONE, MSS_CTRL_MSS_ATCM_MEM_INIT_DONE_MSS_ATCM_MEM_INIT_DONE_MEM_INIT_DONE) != 0);
    /* Write 0 after mem init is completed */
    CSL_FINS(rssProcCtrl->RSS_STATIC_MEM_MEMINIT, MSS_CTRL_MSS_ATCM_MEM_INIT_MSS_ATCM_MEM_INIT_MEM_INIT, 0);
}

/**
 *  @b Description
 *  @n
 *      This API waits for Shared Memory initialization for RSS to complete
 *
 *  \ingroup DRIVER_RCM_FUNCTIONS
 *
 *  @retval     None
 */
void SBL_RcmWaitMeminitSharedBSS(void)
{
    CSL_rss_ctrlRegs *rssCtrl = CSL_RSS_CTRL_getBaseAddress ();
    
    while (CSL_FEXT(rssCtrl->RSS_SHARED_MEM_MEMINIT_DONE, MSS_CTRL_MSS_ATCM_MEM_INIT_DONE_MSS_ATCM_MEM_INIT_DONE_MEM_INIT_DONE) != 1);
    CSL_FINS(rssCtrl->RSS_SHARED_MEM_MEMINIT_DONE, MSS_CTRL_MSS_ATCM_MEM_INIT_DONE_MSS_ATCM_MEM_INIT_DONE_MEM_INIT_DONE, 1);
    
    /* Check MEMINIT STATUS is zero to confirm no inprogress MEM INIT */
    while (CSL_FEXT(rssCtrl->RSS_SHARED_MEM_MEMINIT_STATUS, MSS_CTRL_MSS_ATCM_MEM_INIT_STATUS_MSS_ATCM_MEM_INIT_STATUS_MEM_STATUS) != 0);
    while (CSL_FEXT(rssCtrl->RSS_SHARED_MEM_MEMINIT_DONE, MSS_CTRL_MSS_ATCM_MEM_INIT_DONE_MSS_ATCM_MEM_INIT_DONE_MEM_INIT_DONE) != 0);
    /* Write 0 after mem init is completed */
    CSL_FINS(rssCtrl->RSS_SHARED_MEM_MEMINIT, MSS_CTRL_MSS_ATCM_MEM_INIT_MSS_ATCM_MEM_INIT_MEM_INIT, 0);
}
#endif /* defined (SOC_AWR294X) */

/**
 *  @b Description
 *  @n
 *      This API waits for Memory initialization for TCMA and L2 to complete
 *
 *  \ingroup DRIVER_RCM_FUNCTIONS
 *
 *  @retval     None
 */
void SBL_RcmWaitMeminitL2(void)
{
    CSL_mss_ctrlRegs*        ptrMSSCtrlRegs;

    ptrMSSCtrlRegs = CSL_MSS_CTRL_getBaseAddress ();

     /* wait for the L2 memory init to complete */
    while((ptrMSSCtrlRegs->MSS_L2_MEM_INIT_DONE & 0x3U) != 0x3U)
    {
        /* TBD - handle time out */
    }

    /* clear the status */
    ptrMSSCtrlRegs->MSS_L2_MEM_INIT_DONE = CSL_insert8 (ptrMSSCtrlRegs->MSS_L2_MEM_INIT_DONE, 1U, 0U, 0x3U);
}

/**
 *  @b Description
 *  @n
 *      This API Sets up the dsp PLL configuration switches the R5 to PLL clock.
 *
 *  \ingroup DRIVER_RCM_FUNCTIONS
 *
 *  @retval     None
 */
void SBL_RcmDspApllConfig(Rcm_PllFoutFreqId outFreqId, Rcm_PllHsDivOutConfig *hsDivCfg)
{
    Rcm_XtalFreqId XTALFreq;
    uint16_t dspTrimVal;
    CSL_mss_toprcmRegs *ptrTopRCMRegs;
    Rcm_ADPLLJConfig_t const * adplljCfg;
    uint32_t hsDivOutRegVal;
    uint32_t Fout;

    Fout = SBL_FREQ_MHZ2HZ(gPLLFreqId2FOutMap[outFreqId]);
    /* read the Core ADPLL trim value */
    dspTrimVal = SBL_RcmGetDspTrimVal ();

    /* read the XTAL Frequency */
    XTALFreq = SBL_RcmGetXTALFrequency ();

   /* program PLL dividers and multipliers. The value are taken from  TPR12_ADPLLJ_Settings_1p0.xlsx */
    adplljCfg = SBL_getADPLLJConfig(gXTALInfo[XTALFreq].Finp, outFreqId);
    DebugP_assert(adplljCfg != NULL);

    if (adplljCfg != NULL)
    {
        if (gXTALInfo[XTALFreq].div2flag == false)
        {
            progPllDspDivider (adplljCfg->N, 
                               0U /* N2 divider for bypass */, 
                               adplljCfg->M, 
                               adplljCfg->M2, 
                               adplljCfg->FracM);
        }
        else
        {
            uint32_t N;

            DebugP_assert(((adplljCfg->N + 1) % 2) == 0);
            /* Input XTAL freq is half. Divide input divider by 2 to get same output freq */
            N = ((adplljCfg->N + 1) / 2) - 1;
            progPllDspDivider (N, 
                               0U /* N2 divider for bypass */, 
                               adplljCfg->M, 
                               adplljCfg->M2, 
                               adplljCfg->FracM);
        }
        /* Configure and Lock Core PLL */
        configurePllDsp (dspTrimVal);

        /* Derive Clocks */
        // HSDIV-2 Settings
        /* Core PLL settings */
        ptrTopRCMRegs = CSL_TopRCM_getBaseAddress ();

        if (hsDivCfg->hsdivOutEnMask & RCM_PLL_HSDIV_OUTPUT_ENABLE_0)
        {
            DebugP_assert((Fout % hsDivCfg->hsDivOutFreqHz[RCM_PLL_HSDIV_OUTPUT_IDX0]) == 0);
            hsDivOutRegVal = Fout / hsDivCfg->hsDivOutFreqHz[RCM_PLL_HSDIV_OUTPUT_IDX0];
            hsDivOutRegVal--;
            //HW_WR_REG32(CSL_MSS_TOPRCM_U_BASE+PLL_DSP_HSDIVIDER_CLKOUT0, 0x0);    // CLKOUT0_DIV[4:0] = 4  -- 900M/(4+1) = 225MHz
            ptrTopRCMRegs->PLL_DSP_HSDIVIDER_CLKOUT0 = CSL_insert8 (ptrTopRCMRegs->PLL_DSP_HSDIVIDER_CLKOUT0, 4U, 0U, hsDivOutRegVal);
        }
        if (hsDivCfg->hsdivOutEnMask & RCM_PLL_HSDIV_OUTPUT_ENABLE_1)
        {
            DebugP_assert((Fout % hsDivCfg->hsDivOutFreqHz[RCM_PLL_HSDIV_OUTPUT_IDX1]) == 0);
            hsDivOutRegVal = Fout / hsDivCfg->hsDivOutFreqHz[RCM_PLL_HSDIV_OUTPUT_IDX1];
            hsDivOutRegVal--;
            //HW_WR_REG32(CSL_MSS_TOPRCM_U_BASE+PLL_DSP_HSDIVIDER_CLKOUT1, 0x3);    // CLKOUT1_DIV[4:0] = 1  -- 900M/(1+1) = 450MHz
            ptrTopRCMRegs->PLL_DSP_HSDIVIDER_CLKOUT1 = CSL_insert8 (ptrTopRCMRegs->PLL_DSP_HSDIVIDER_CLKOUT1, 4U, 0U, hsDivOutRegVal);
        }
        if (hsDivCfg->hsdivOutEnMask & RCM_PLL_HSDIV_OUTPUT_ENABLE_2)
        {
            DebugP_assert((Fout % hsDivCfg->hsDivOutFreqHz[RCM_PLL_HSDIV_OUTPUT_IDX2]) == 0);
            hsDivOutRegVal = Fout / hsDivCfg->hsDivOutFreqHz[RCM_PLL_HSDIV_OUTPUT_IDX2];
            hsDivOutRegVal--;
            //HW_WR_REG32(CSL_MSS_TOPRCM_U_BASE+PLL_DSP_HSDIVIDER_CLKOUT2, 0x4);    // CLKOUT2_DIV[4:0] = 8  -- 900M/(8+1) = 100MHz
            ptrTopRCMRegs->PLL_DSP_HSDIVIDER_CLKOUT2 = CSL_insert8 (ptrTopRCMRegs->PLL_DSP_HSDIVIDER_CLKOUT2, 4U, 0U, hsDivOutRegVal);
        }
        if (hsDivCfg->hsdivOutEnMask & RCM_PLL_HSDIV_OUTPUT_ENABLE_3)
        {
            DebugP_assert((Fout % hsDivCfg->hsDivOutFreqHz[RCM_PLL_HSDIV_OUTPUT_IDX3]) == 0);
            hsDivOutRegVal = Fout / hsDivCfg->hsDivOutFreqHz[RCM_PLL_HSDIV_OUTPUT_IDX3];
            hsDivOutRegVal--;
            //UnsedHW_WR_REG32(CSL_MSS_TOPRCM_U_BASE+PLL_DSP_HSDIVIDER_CLKOUT3, 0x109);  //+CLKOUT3_GATE[8]  = 1
            //Unsed HW_WR_REG32(CSL_MSS_TOPRCM_U_BASE+PLL_DSP_HSDIVIDER_CLKOUT3, 0x9);    // CLKOUT3_DIV[4:0] = 9  -- 900M/(9+1) = 90MHz unused
            ptrTopRCMRegs->PLL_DSP_HSDIVIDER_CLKOUT3 = CSL_insert8 (ptrTopRCMRegs->PLL_DSP_HSDIVIDER_CLKOUT3, 4U, 0U, hsDivOutRegVal);
        }

        /* Generate Trigger to latch these values */
        //HW_WR_REG32(CSL_MSS_TOPRCM_U_BASE+PLL_DSP_HSDIVIDER , 0x4);    // HSDIVIDER[2]     = 1
        ptrTopRCMRegs->PLL_DSP_HSDIVIDER         = CSL_insert8 (ptrTopRCMRegs->PLL_DSP_HSDIVIDER, 2U, 2U, 0x1U);
        //HW_WR_REG32(CSL_MSS_TOPRCM_U_BASE+PLL_DSP_HSDIVIDER        , 0x0);    // HSDIVIDER[2]     = 0
        ptrTopRCMRegs->PLL_DSP_HSDIVIDER         = CSL_insert8 (ptrTopRCMRegs->PLL_DSP_HSDIVIDER, 2U, 2U, 0x0U);

        /* Ungate the clocks */
        if (hsDivCfg->hsdivOutEnMask & RCM_PLL_HSDIV_OUTPUT_ENABLE_0)
        {
            //HW_WR_REG32(CSL_MSS_TOPRCM_U_BASE+PLL_DSP_HSDIVIDER_CLKOUT0, 0x100);  //+CLKOUT0_GATE[8]  = 1
            ptrTopRCMRegs->PLL_DSP_HSDIVIDER_CLKOUT0 = CSL_insert8 (ptrTopRCMRegs->PLL_DSP_HSDIVIDER_CLKOUT0, 8U, 8U, 0x1U);
        }
        if (hsDivCfg->hsdivOutEnMask & RCM_PLL_HSDIV_OUTPUT_ENABLE_1)
        {
            //HW_WR_REG32(CSL_MSS_TOPRCM_U_BASE+PLL_DSP_HSDIVIDER_CLKOUT1, 0x103);  //+CLKOUT1_GATE[8]  = 1
            ptrTopRCMRegs->PLL_DSP_HSDIVIDER_CLKOUT1 = CSL_insert8 (ptrTopRCMRegs->PLL_DSP_HSDIVIDER_CLKOUT1, 8U, 8U, 0x1U);
        }
        if (hsDivCfg->hsdivOutEnMask & RCM_PLL_HSDIV_OUTPUT_ENABLE_2)
        {
            //HW_WR_REG32(CSL_MSS_TOPRCM_U_BASE+PLL_DSP_HSDIVIDER_CLKOUT2, 0x104);  //+CLKOUT2_GATE[8]  = 1
            ptrTopRCMRegs->PLL_DSP_HSDIVIDER_CLKOUT2 = CSL_insert8 (ptrTopRCMRegs->PLL_DSP_HSDIVIDER_CLKOUT2, 8U, 8U, 0x1U);
        }
        if (hsDivCfg->hsdivOutEnMask & RCM_PLL_HSDIV_OUTPUT_ENABLE_3)
        {
            //HW_WR_REG32(CSL_MSS_TOPRCM_U_BASE+PLL_DSP_HSDIVIDER_CLKOUT2, 0x104);  //+CLKOUT2_GATE[8]  = 1
            ptrTopRCMRegs->PLL_DSP_HSDIVIDER_CLKOUT2 = CSL_insert8 (ptrTopRCMRegs->PLL_DSP_HSDIVIDER_CLKOUT3, 8U, 8U, 0x1U);
        }
    }

    return;
}

/**
 *  @b Description
 *  @n
 *      This API Sets up the dsp PLL configuration switches the R5 to PLL clock.
 *
 *  \ingroup DRIVER_RCM_FUNCTIONS
 *
 *  @retval     None
 */
void SBL_RcmPerApllConfig(Rcm_PllFoutFreqId outFreqId, Rcm_PllHsDivOutConfig *hsDivCfg)
{
    Rcm_XtalFreqId XTALFreq;
    uint16_t perTrimVal;
    CSL_mss_toprcmRegs *ptrTopRCMRegs;
    Rcm_ADPLLJConfig_t const * adplljCfg;
    uint32_t hsDivOutRegVal;
    uint32_t Fout;

    Fout = SBL_FREQ_MHZ2HZ(gPLLFreqId2FOutMap[outFreqId]);
    /* read the Core ADPLL trim value */
    perTrimVal = SBL_RcmGetPerTrimVal ();
    /* Core PLL settings */
    ptrTopRCMRegs = CSL_TopRCM_getBaseAddress ();

    /* read the XTAL Frequency */
    XTALFreq = SBL_RcmGetXTALFrequency ();

   /* program PLL dividers and multipliers. The value are taken from  TPR12_ADPLLJ_Settings_1p0.xlsx */
    adplljCfg = SBL_getADPLLJConfig(gXTALInfo[XTALFreq].Finp, outFreqId);
    DebugP_assert(adplljCfg != NULL);

    if (adplljCfg != NULL)
    {
        if (gXTALInfo[XTALFreq].div2flag == false)
        {
            progPllPerDivider (adplljCfg->N, 0U, adplljCfg->M, adplljCfg->M2, adplljCfg->FracM);
        }
        else
        {
            uint32_t N;

            DebugP_assert(((adplljCfg->N + 1) % 2) == 0);
            /* Input XTAL freq is half. Divide input divider by 2 to get same output freq */
            N = ((adplljCfg->N + 1) / 2) - 1;
        
            progPllPerDivider (N, 0U, adplljCfg->M, adplljCfg->M2, adplljCfg->FracM);
        
        }
        /* Configure and Lock Core PLL */
        configurePllPer (perTrimVal);

        /* Derive Clocks */
        /* TPR12_Ch08_Clock_Arch_0p91 is used as reference for below settings */
        /* Set clock divider values from PER PLL*/
        /* 1728 Mhz */
        // HSDIV-2 Settings
        if (hsDivCfg->hsdivOutEnMask & RCM_PLL_HSDIV_OUTPUT_ENABLE_0)
        {
            DebugP_assert((Fout % hsDivCfg->hsDivOutFreqHz[RCM_PLL_HSDIV_OUTPUT_IDX0]) == 0);
            hsDivOutRegVal = Fout / hsDivCfg->hsDivOutFreqHz[RCM_PLL_HSDIV_OUTPUT_IDX0];
            hsDivOutRegVal--;
            //HW_WR_REG32(CSL_MSS_TOPRCM_U_BASE+PLL_PER_HSDIVIDER_CLKOUT0, 0x0);    // CLKOUT0_DIV[4:0] = 0  -- 1.728G/(0+1) = 1.728GHz
            ptrTopRCMRegs->PLL_PER_HSDIVIDER_CLKOUT0 = CSL_insert8 (ptrTopRCMRegs->PLL_PER_HSDIVIDER_CLKOUT0, 4U, 0U, hsDivOutRegVal);
        }
        if (hsDivCfg->hsdivOutEnMask & RCM_PLL_HSDIV_OUTPUT_ENABLE_1)
        {
            DebugP_assert((Fout % hsDivCfg->hsDivOutFreqHz[RCM_PLL_HSDIV_OUTPUT_IDX1]) == 0);
            hsDivOutRegVal = Fout / hsDivCfg->hsDivOutFreqHz[RCM_PLL_HSDIV_OUTPUT_IDX1];
            hsDivOutRegVal--;
            //HW_WR_REG32(CSL_MSS_TOPRCM_U_BASE+PLL_PER_HSDIVIDER_CLKOUT1, 0x8);    // CLKOUT1_DIV[4:0] = 8  -- 1.728G/(8+1) = 192 MHz
            ptrTopRCMRegs->PLL_PER_HSDIVIDER_CLKOUT1 = CSL_insert8 (ptrTopRCMRegs->PLL_PER_HSDIVIDER_CLKOUT1, 4U, 0U, hsDivOutRegVal);
        }
        if (hsDivCfg->hsdivOutEnMask & RCM_PLL_HSDIV_OUTPUT_ENABLE_2)
        {
            DebugP_assert((Fout % hsDivCfg->hsDivOutFreqHz[RCM_PLL_HSDIV_OUTPUT_IDX2]) == 0);
            hsDivOutRegVal = Fout / hsDivCfg->hsDivOutFreqHz[RCM_PLL_HSDIV_OUTPUT_IDX2];
            hsDivOutRegVal--;
            //HW_WR_REG32(CSL_MSS_TOPRCM_U_BASE+PLL_PER_HSDIVIDER_CLKOUT2, 0x11);    // CLKOUT2_DIV[4:0] = 11  -- 1.8G/(17+1) = 96 MHz
            ptrTopRCMRegs->PLL_PER_HSDIVIDER_CLKOUT2 = CSL_insert8 (ptrTopRCMRegs->PLL_PER_HSDIVIDER_CLKOUT2, 4U, 0U, hsDivOutRegVal);
        }
        if (hsDivCfg->hsdivOutEnMask & RCM_PLL_HSDIV_OUTPUT_ENABLE_3)
        {
            DebugP_assert((Fout % hsDivCfg->hsDivOutFreqHz[RCM_PLL_HSDIV_OUTPUT_IDX3]) == 0);
            hsDivOutRegVal = Fout / hsDivCfg->hsDivOutFreqHz[RCM_PLL_HSDIV_OUTPUT_IDX3];
            hsDivOutRegVal--;
            //Unsed HW_WR_REG32(CSL_MSS_TOPRCM_U_BASE+PLL_DSP_HSDIVIDER_CLKOUT3, 0x9);    //  unused
            ptrTopRCMRegs->PLL_PER_HSDIVIDER_CLKOUT3 = CSL_insert8 (ptrTopRCMRegs->PLL_PER_HSDIVIDER_CLKOUT3, 4U, 0U, hsDivOutRegVal);
        }
        /* Generate Trigger to latch these values */
        //HW_WR_REG32(CSL_MSS_TOPRCM_U_BASE+PLL_PER_HSDIVIDER        , 0x4);    // HSDIVIDER[2]     = 1
        ptrTopRCMRegs->PLL_PER_HSDIVIDER         = CSL_insert8 (ptrTopRCMRegs->PLL_PER_HSDIVIDER, 2U, 2U, 0x1U);
        //HW_WR_REG32(CSL_MSS_TOPRCM_U_BASE+PLL_PER_HSDIVIDER        , 0x0);    // HSDIVIDER[2]     = 0
        ptrTopRCMRegs->PLL_PER_HSDIVIDER         = CSL_insert8 (ptrTopRCMRegs->PLL_PER_HSDIVIDER, 2U, 2U, 0x0U);

        /* Ungate the clocks */
        if (hsDivCfg->hsdivOutEnMask & RCM_PLL_HSDIV_OUTPUT_ENABLE_0)
        {
            //HW_WR_REG32(CSL_MSS_TOPRCM_U_BASE+PLL_PER_HSDIVIDER_CLKOUT0, 0x100);  //+CLKOUT0_GATE[8]  = 1
            ptrTopRCMRegs->PLL_PER_HSDIVIDER_CLKOUT0 = CSL_insert8 (ptrTopRCMRegs->PLL_PER_HSDIVIDER_CLKOUT0, 8U, 8U, 0x1U);
        }
        if (hsDivCfg->hsdivOutEnMask & RCM_PLL_HSDIV_OUTPUT_ENABLE_1)
        {
            //HW_WR_REG32(CSL_MSS_TOPRCM_U_BASE+PLL_PER_HSDIVIDER_CLKOUT1, 0x108);  //+CLKOUT1_GATE[8]  = 1
            ptrTopRCMRegs->PLL_PER_HSDIVIDER_CLKOUT1 = CSL_insert8 (ptrTopRCMRegs->PLL_PER_HSDIVIDER_CLKOUT1, 8U, 8U, 0x1U);
        }
        if (hsDivCfg->hsdivOutEnMask & RCM_PLL_HSDIV_OUTPUT_ENABLE_2)
        {
            //HW_WR_REG32(CSL_MSS_TOPRCM_U_BASE+PLL_PER_HSDIVIDER_CLKOUT2, 0x111);  //+CLKOUT2_GATE[8]  = 1
            ptrTopRCMRegs->PLL_PER_HSDIVIDER_CLKOUT2 = CSL_insert8 (ptrTopRCMRegs->PLL_PER_HSDIVIDER_CLKOUT2, 8U, 8U, 0x1U);
        }
        if (hsDivCfg->hsdivOutEnMask & RCM_PLL_HSDIV_OUTPUT_ENABLE_3)
        {
            //UnusedHW_WR_REG32(CSL_MSS_TOPRCM_U_BASE+PLL_DSP_HSDIVIDER_CLKOUT3, 0x109);  //+CLKOUT3_GATE[8]  = 1
            ptrTopRCMRegs->PLL_PER_HSDIVIDER_CLKOUT3 = CSL_insert8 (ptrTopRCMRegs->PLL_PER_HSDIVIDER_CLKOUT3, 8U, 8U, 0x1U);
        }
    }

    return;
}

/**
 *  @b Description
 *  @n
 *      This API programs the Clock Source Selection and Clock divider values
 *      for c66x core
 *
 *  @param[in]  clkSource
 *      Clock Source
 *  @param[in]  clkDivisor
 *      Clock Divider Value
 *
 *  \ingroup DRIVER_RCM_FUNCTIONS
 *
 *  @retval     Rcm_Return
 */
Rcm_Return SBL_RcmSetDspCoreClock (Rcm_DSPClockSource clkSource,
                                   uint32_t freqHz)
{
    volatile uint32_t   *ptrClkSrcReg, *ptrClkDivReg;
    uint16_t            clkSrcVal;
    Rcm_Return          retVal;
    CSL_dss_rcmRegs     *ptrDSSRcmRegs = CSL_DSSRCM_getBaseAddress();
    uint32_t            pllFout;
    uint32_t            clkDivisor;


    CSL_DSSRCM_enableAccess(ptrDSSRcmRegs);
    pllFout = SBL_RcmGetDSPClockFrequency(clkSource);
    getDSPClkSrcAndDivReg (clkSource, &clkSrcVal, &ptrClkSrcReg, &ptrClkDivReg);
    clkDivisor = SBL_RcmGetModuleClkDivVal(pllFout, freqHz);

    if ((ptrClkSrcReg != NULL) && (ptrClkDivReg != NULL) && (clkSrcVal != 0x888U))
    {
        uint16_t            clkDivVal;

        /* Create the Divider Value to be programmed */
        clkDivVal = ((uint16_t)clkDivisor & 0xFU);
        clkDivVal = (clkDivVal | (clkDivVal << 4U) | (clkDivVal << 8U));

        /* Write the Divider Value */
        *ptrClkDivReg = CSL_insert16 (*ptrClkDivReg, 11U, 0U, clkDivVal);

        /* Write the Clock Source Selection Value */
        *ptrClkSrcReg = CSL_insert16 (*ptrClkSrcReg, 11U, 0U, clkSrcVal);
        retVal = Rcm_Return_SUCCESS;
    }
    else
    {
        /* Error */
        retVal = Rcm_Return_ERROR;
    }

    return (retVal);
}

/**
 *  @b Description
 *  @n
 *      This API returns the frequency of the clock which is passed as parameter
 *  @param[in]  clkSource
 *      clock source for which frequency is requested
 *
 *  \ingroup DRIVER_RCM_FUNCTIONS
 *
 *  @retval     Clock Frequency (in Hz)
 *  @note       return value of 0 indicates clock source not available
 */
uint32_t SBL_RcmGetDSPClockFrequency(Rcm_DSPClockSource clkSource)
{
    uint32_t clkFreq = 0U;
    Rcm_ClkSrcInfo const * clkSrcInfo;
    Rcm_XtalFreqId clkFreqId;
    uint32_t Finp;


    clkSrcInfo = &gDspClkSrcInfoMap[clkSource];
    clkFreqId = SBL_RcmGetXTALFrequency ();
    switch (clkSrcInfo->pllId)
    {
        case RCM_PLLID_CORE:
        {
            Finp = gXTALInfo[clkFreqId].Finp;
            clkFreq = SBL_RcmGetCoreHSDivOut(Finp, gXTALInfo[clkFreqId].div2flag, clkSrcInfo->hsDivOut);
            break;
        }
        case RCM_PLLID_DSS:
        {
            Finp = gXTALInfo[clkFreqId].Finp;
            clkFreq = SBL_RcmGetDSSHSDivOut(Finp, gXTALInfo[clkFreqId].div2flag, clkSrcInfo->hsDivOut);
            break;
        }
        case RCM_PLLID_PER:
        {
            Finp = gXTALInfo[clkFreqId].Finp;
            clkFreq = SBL_RcmGetPerHSDivOut(Finp, gXTALInfo[clkFreqId].div2flag, clkSrcInfo->hsDivOut);
            break;
        }
        case RCM_PLLID_XTALCLK:
        {
            Finp = gXTALInfo[clkFreqId].Finp;
            if (gXTALInfo[clkFreqId].div2flag)
            {
                clkFreq = Finp/2;
            }
            else
            {
                clkFreq = Finp;
            }
            break;
        }
        default:
        {
            DebugP_assert(FALSE);
            break;
        }
    }
    return (clkFreq);
}

/**
 *  @b Description
 *  @n
 *      This API get the c66x core configured frequency 
 *
 *  @param[out]  freqHz
 *      c66x core clock frequency
 *
 *  \ingroup DRIVER_RCM_FUNCTIONS
 *
 *  @retval     Rcm_Return
 */
Rcm_Return SBL_RcmGetDspCoreFreq (uint32_t *freqHz)
{
    Rcm_Return          retVal;
    CSL_dss_rcmRegs     *ptrDSSRcmRegs = CSL_DSSRCM_getBaseAddress();
    uint32_t            pllFout;
    uint32_t            clkDivisorRegVal;
    uint32_t            clkDiv;
    Rcm_DSPClockSource  clkSource;


    CSL_DSSRCM_enableAccess(ptrDSSRcmRegs);
    retVal= getDSPClkSrcAndDivValue (&clkSource, &clkDivisorRegVal);

    DebugP_assert(retVal == Rcm_Return_SUCCESS);
    if (Rcm_Return_SUCCESS == retVal)
    {
        pllFout = SBL_RcmGetDSPClockFrequency(clkSource);
        clkDiv = SBL_RcmGetModuleClkDivFromRegVal(clkDivisorRegVal);
        *freqHz = (pllFout / clkDiv);
    }
    return (retVal);
}


static Rcm_ADPLLJConfig_t const * SBL_getADPLLJConfig(uint32_t Finp, Rcm_PllFoutFreqId foutFreqId)
{
    uint32_t i;
    Rcm_ADPLLJConfig_t const *adplljCfg;
    uint32_t Fout;

    Fout = gPLLFreqId2FOutMap[foutFreqId];
    for (i = 0; i < SBL_UTILS_ARRAYSIZE(gADPLLJConfigTbl); i++)
    {
        if ((gADPLLJConfigTbl[i].Finp == Finp) && (gADPLLJConfigTbl[i].Fout == Fout))
        {
            break;
        }
    }
    if (i < SBL_UTILS_ARRAYSIZE(gADPLLJConfigTbl))
    {
        adplljCfg = &gADPLLJConfigTbl[i];
    }
    else
    {
        adplljCfg = (Rcm_ADPLLJConfig_t const *)NULL;
    }
    return adplljCfg;
}


void SBL_RcmWaitMeminitDSSL2(uint32_t l2bankMask)
{
    CSL_dss_ctrlRegs *dssCtrl = CSL_DSS_CTRL_getBaseAddress();
    uint32_t          clearMemInitMask = 0;

    CSL_DSS_CTRL_enableAccess(dssCtrl);
    if (l2bankMask & RCM_MEMINIT_DSSL2_MEMBANK_VB00)
    {
        while (CSL_FEXT(dssCtrl->DSS_DSP_L2RAM_MEMINIT_DONE, DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_DONE_DSS_DSP_L2RAM_MEMINIT_DONE_VB00) != 1);
        clearMemInitMask |= CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_DONE_DSS_DSP_L2RAM_MEMINIT_DONE_VB00_MASK;
    }

    if (l2bankMask & RCM_MEMINIT_DSSL2_MEMBANK_VB01)
    {
        while (CSL_FEXT(dssCtrl->DSS_DSP_L2RAM_MEMINIT_DONE, DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_DONE_DSS_DSP_L2RAM_MEMINIT_DONE_VB01) != 1);
        clearMemInitMask |= CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_DONE_DSS_DSP_L2RAM_MEMINIT_DONE_VB01_MASK;
    }

    if (l2bankMask & RCM_MEMINIT_DSSL2_MEMBANK_VB10)
    {
        while (CSL_FEXT(dssCtrl->DSS_DSP_L2RAM_MEMINIT_DONE, DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_DONE_DSS_DSP_L2RAM_MEMINIT_DONE_VB10) != 1);
        clearMemInitMask |= CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_DONE_DSS_DSP_L2RAM_MEMINIT_DONE_VB10_MASK;
    }

    if (l2bankMask & RCM_MEMINIT_DSSL2_MEMBANK_VB11)
    {
        while (CSL_FEXT(dssCtrl->DSS_DSP_L2RAM_MEMINIT_DONE, DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_DONE_DSS_DSP_L2RAM_MEMINIT_DONE_VB11) != 1);
        clearMemInitMask |= CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_DONE_DSS_DSP_L2RAM_MEMINIT_DONE_VB11_MASK;
    }

    if (l2bankMask & RCM_MEMINIT_DSSL2_MEMBANK_VB20)
    {
        while (CSL_FEXT(dssCtrl->DSS_DSP_L2RAM_MEMINIT_DONE, DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_DONE_DSS_DSP_L2RAM_MEMINIT_DONE_VB20) != 1);
        clearMemInitMask |= CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_DONE_DSS_DSP_L2RAM_MEMINIT_DONE_VB20_MASK;
    }

    if (l2bankMask & RCM_MEMINIT_DSSL2_MEMBANK_VB21)
    {
        while (CSL_FEXT(dssCtrl->DSS_DSP_L2RAM_MEMINIT_DONE, DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_DONE_DSS_DSP_L2RAM_MEMINIT_DONE_VB21) != 1);
        clearMemInitMask |= CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_DONE_DSS_DSP_L2RAM_MEMINIT_DONE_VB21_MASK;
    }

    if (l2bankMask & RCM_MEMINIT_DSSL2_MEMBANK_VB30)
    {
        while (CSL_FEXT(dssCtrl->DSS_DSP_L2RAM_MEMINIT_DONE, DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_DONE_DSS_DSP_L2RAM_MEMINIT_DONE_VB30) != 1);
        clearMemInitMask |= CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_DONE_DSS_DSP_L2RAM_MEMINIT_DONE_VB30_MASK;
    }

    if (l2bankMask & RCM_MEMINIT_DSSL2_MEMBANK_VB31)
    {
        while (CSL_FEXT(dssCtrl->DSS_DSP_L2RAM_MEMINIT_DONE, DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_DONE_DSS_DSP_L2RAM_MEMINIT_DONE_VB31) != 1);
        clearMemInitMask |= CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_DONE_DSS_DSP_L2RAM_MEMINIT_DONE_VB31_MASK;
    }
    
    dssCtrl->DSS_DSP_L2RAM_MEMINIT_DONE = CSL_insert8 (dssCtrl->DSS_DSP_L2RAM_MEMINIT_DONE, 7U, 0U, clearMemInitMask);

    if (l2bankMask & RCM_MEMINIT_DSSL2_MEMBANK_VB00)
    {
        /* Check MEMINIT STATUS is zero to confirm no inprogress MEM INIT */
        while (CSL_FEXT(dssCtrl->DSS_DSP_L2RAM_MEMINIT_STATUS, DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_STATUS_DSS_DSP_L2RAM_MEMINIT_STATUS_VB00) != 0);
        while (CSL_FEXT(dssCtrl->DSS_DSP_L2RAM_MEMINIT_DONE, DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_DONE_DSS_DSP_L2RAM_MEMINIT_DONE_VB00) != 0);
    }

    if (l2bankMask & RCM_MEMINIT_DSSL2_MEMBANK_VB01)
    {
        /* Check MEMINIT STATUS is zero to confirm no inprogress MEM INIT */
        while (CSL_FEXT(dssCtrl->DSS_DSP_L2RAM_MEMINIT_STATUS, DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_STATUS_DSS_DSP_L2RAM_MEMINIT_STATUS_VB01) != 0);
        while (CSL_FEXT(dssCtrl->DSS_DSP_L2RAM_MEMINIT_DONE, DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_DONE_DSS_DSP_L2RAM_MEMINIT_DONE_VB01) != 0);
    }

    if (l2bankMask & RCM_MEMINIT_DSSL2_MEMBANK_VB10)
    {
        /* Check MEMINIT STATUS is zero to confirm no inprogress MEM INIT */
        while (CSL_FEXT(dssCtrl->DSS_DSP_L2RAM_MEMINIT_STATUS, DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_STATUS_DSS_DSP_L2RAM_MEMINIT_STATUS_VB10) != 0);
        while (CSL_FEXT(dssCtrl->DSS_DSP_L2RAM_MEMINIT_DONE, DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_DONE_DSS_DSP_L2RAM_MEMINIT_DONE_VB10) != 0);
    }

    if (l2bankMask & RCM_MEMINIT_DSSL2_MEMBANK_VB11)
    {
        /* Check MEMINIT STATUS is zero to confirm no inprogress MEM INIT */
        while (CSL_FEXT(dssCtrl->DSS_DSP_L2RAM_MEMINIT_STATUS, DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_STATUS_DSS_DSP_L2RAM_MEMINIT_STATUS_VB11) != 0);
        while (CSL_FEXT(dssCtrl->DSS_DSP_L2RAM_MEMINIT_DONE, DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_DONE_DSS_DSP_L2RAM_MEMINIT_DONE_VB11) != 0);
    }

    if (l2bankMask & RCM_MEMINIT_DSSL2_MEMBANK_VB20)
    {
        /* Check MEMINIT STATUS is zero to confirm no inprogress MEM INIT */
        while (CSL_FEXT(dssCtrl->DSS_DSP_L2RAM_MEMINIT_STATUS, DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_STATUS_DSS_DSP_L2RAM_MEMINIT_STATUS_VB20) != 0);
        while (CSL_FEXT(dssCtrl->DSS_DSP_L2RAM_MEMINIT_DONE, DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_DONE_DSS_DSP_L2RAM_MEMINIT_DONE_VB20) != 0);
    }

    if (l2bankMask & RCM_MEMINIT_DSSL2_MEMBANK_VB21)
    {
        /* Check MEMINIT STATUS is zero to confirm no inprogress MEM INIT */
        while (CSL_FEXT(dssCtrl->DSS_DSP_L2RAM_MEMINIT_STATUS, DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_STATUS_DSS_DSP_L2RAM_MEMINIT_STATUS_VB21) != 0);
        while (CSL_FEXT(dssCtrl->DSS_DSP_L2RAM_MEMINIT_DONE, DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_DONE_DSS_DSP_L2RAM_MEMINIT_DONE_VB21) != 0);
    }

    if (l2bankMask & RCM_MEMINIT_DSSL2_MEMBANK_VB30)
    {
        /* Check MEMINIT STATUS is zero to confirm no inprogress MEM INIT */
        while (CSL_FEXT(dssCtrl->DSS_DSP_L2RAM_MEMINIT_STATUS, DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_STATUS_DSS_DSP_L2RAM_MEMINIT_STATUS_VB30) != 0);
        while (CSL_FEXT(dssCtrl->DSS_DSP_L2RAM_MEMINIT_DONE, DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_DONE_DSS_DSP_L2RAM_MEMINIT_DONE_VB30) != 0);
    }

    if (l2bankMask & RCM_MEMINIT_DSSL2_MEMBANK_VB31)
    {
        /* Check MEMINIT STATUS is zero to confirm no inprogress MEM INIT */
        while (CSL_FEXT(dssCtrl->DSS_DSP_L2RAM_MEMINIT_STATUS, DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_STATUS_DSS_DSP_L2RAM_MEMINIT_STATUS_VB31) != 0);
        while (CSL_FEXT(dssCtrl->DSS_DSP_L2RAM_MEMINIT_DONE, DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_DONE_DSS_DSP_L2RAM_MEMINIT_DONE_VB31) != 0);
    }
}

void SBL_RcmStartMeminitDSSL2(uint32_t l2bankMask)
{
    CSL_dss_ctrlRegs *dssCtrl = CSL_DSS_CTRL_getBaseAddress ();
    uint32_t memBankInit = 0;

    CSL_DSS_CTRL_enableAccess(dssCtrl);
    if (l2bankMask & RCM_MEMINIT_DSSL2_MEMBANK_VB00)
    {
        memBankInit |= CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_START_DSS_DSP_L2RAM_MEMINIT_START_VB00_MASK;
    }
    if (l2bankMask & RCM_MEMINIT_DSSL2_MEMBANK_VB01)
    {
        memBankInit |= CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_START_DSS_DSP_L2RAM_MEMINIT_START_VB01_MASK;
    }
    if (l2bankMask & RCM_MEMINIT_DSSL2_MEMBANK_VB10)
    {
        memBankInit |= CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_START_DSS_DSP_L2RAM_MEMINIT_START_VB10_MASK;
    }
    if (l2bankMask & RCM_MEMINIT_DSSL2_MEMBANK_VB11)
    {
        memBankInit |= CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_START_DSS_DSP_L2RAM_MEMINIT_START_VB11_MASK;
    }
    if (l2bankMask & RCM_MEMINIT_DSSL2_MEMBANK_VB20)
    {
        memBankInit |= CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_START_DSS_DSP_L2RAM_MEMINIT_START_VB20_MASK;
    }
    if (l2bankMask & RCM_MEMINIT_DSSL2_MEMBANK_VB21)
    {
        memBankInit |= CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_START_DSS_DSP_L2RAM_MEMINIT_START_VB21_MASK;
    }
    if (l2bankMask & RCM_MEMINIT_DSSL2_MEMBANK_VB30)
    {
        memBankInit |= CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_START_DSS_DSP_L2RAM_MEMINIT_START_VB30_MASK;
    }
    if (l2bankMask & RCM_MEMINIT_DSSL2_MEMBANK_VB31)
    {
        memBankInit |= CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_START_DSS_DSP_L2RAM_MEMINIT_START_VB31_MASK;
    }

    /* Start the Initialization of L2 Memory */
    dssCtrl->DSS_DSP_L2RAM_MEMINIT_START = CSL_insert8 (dssCtrl->DSS_DSP_L2RAM_MEMINIT_START, 7U, 0U, memBankInit);
}

void SBL_RcmWaitMeminitDSSL3(uint32_t l3bankMask)
{
    CSL_dss_ctrlRegs *dssCtrl = CSL_DSS_CTRL_getBaseAddress ();
    uint32_t          clearMemInitMask = 0;

    CSL_DSS_CTRL_enableAccess(dssCtrl);
    if (l3bankMask & RCM_MEMINIT_DSSL3_MEMBANK_RAM0)
    {
        while (CSL_FEXT(dssCtrl->DSS_L3RAM_MEMINIT_DONE, DSS_CTRL_DSS_L3RAM_MEMINIT_DONE_DSS_L3RAM_MEMINIT_DONE_L3RAM0_MEMINIT_DONE) != 1);
        clearMemInitMask |= CSL_DSS_CTRL_DSS_L3RAM_MEMINIT_DONE_DSS_L3RAM_MEMINIT_DONE_L3RAM0_MEMINIT_DONE_MASK;
    }

    if (l3bankMask & RCM_MEMINIT_DSSL3_MEMBANK_RAM1)
    {
        while (CSL_FEXT(dssCtrl->DSS_L3RAM_MEMINIT_DONE, DSS_CTRL_DSS_L3RAM_MEMINIT_DONE_DSS_L3RAM_MEMINIT_DONE_L3RAM1_MEMINIT_DONE) != 1);
        clearMemInitMask |= CSL_DSS_CTRL_DSS_L3RAM_MEMINIT_DONE_DSS_L3RAM_MEMINIT_DONE_L3RAM1_MEMINIT_DONE_MASK;
    }

    if (l3bankMask & RCM_MEMINIT_DSSL3_MEMBANK_RAM2)
    {
        while (CSL_FEXT(dssCtrl->DSS_L3RAM_MEMINIT_DONE, DSS_CTRL_DSS_L3RAM_MEMINIT_DONE_DSS_L3RAM_MEMINIT_DONE_L3RAM2_MEMINIT_DONE) != 1);
        clearMemInitMask |= CSL_DSS_CTRL_DSS_L3RAM_MEMINIT_DONE_DSS_L3RAM_MEMINIT_DONE_L3RAM2_MEMINIT_DONE_MASK;
    }

    if (l3bankMask & RCM_MEMINIT_DSSL3_MEMBANK_RAM3)
    {
        while (CSL_FEXT(dssCtrl->DSS_L3RAM_MEMINIT_DONE, DSS_CTRL_DSS_L3RAM_MEMINIT_DONE_DSS_L3RAM_MEMINIT_DONE_L3RAM3_MEMINIT_DONE) != 1);
        clearMemInitMask |= CSL_DSS_CTRL_DSS_L3RAM_MEMINIT_DONE_DSS_L3RAM_MEMINIT_DONE_L3RAM3_MEMINIT_DONE_MASK;
    }
    dssCtrl->DSS_L3RAM_MEMINIT_DONE = CSL_insert8 (dssCtrl->DSS_L3RAM_MEMINIT_DONE, 3U, 0U, clearMemInitMask);

    if (l3bankMask & RCM_MEMINIT_DSSL3_MEMBANK_RAM0)
    {
        /* Check MEMINIT STATUS is zero to confirm no inprogress MEM INIT */
        while (CSL_FEXT(dssCtrl->DSS_L3RAM_MEMINIT_STATUS, DSS_CTRL_DSS_L3RAM_MEMINIT_STATUS_DSS_L3RAM_MEMINIT_STATUS_L3RAM0_MEMINIT_STATUS) != 0);
        while (CSL_FEXT(dssCtrl->DSS_L3RAM_MEMINIT_DONE, DSS_CTRL_DSS_L3RAM_MEMINIT_DONE_DSS_L3RAM_MEMINIT_DONE_L3RAM0_MEMINIT_DONE) != 0);
    }

    if (l3bankMask & RCM_MEMINIT_DSSL3_MEMBANK_RAM1)
    {
        /* Check MEMINIT STATUS is zero to confirm no inprogress MEM INIT */
        while (CSL_FEXT(dssCtrl->DSS_L3RAM_MEMINIT_STATUS, DSS_CTRL_DSS_L3RAM_MEMINIT_STATUS_DSS_L3RAM_MEMINIT_STATUS_L3RAM1_MEMINIT_STATUS) != 0);
        while (CSL_FEXT(dssCtrl->DSS_L3RAM_MEMINIT_DONE, DSS_CTRL_DSS_L3RAM_MEMINIT_DONE_DSS_L3RAM_MEMINIT_DONE_L3RAM1_MEMINIT_DONE) != 0);
    }

    if (l3bankMask & RCM_MEMINIT_DSSL3_MEMBANK_RAM2)
    {
        /* Check MEMINIT STATUS is zero to confirm no inprogress MEM INIT */
        while (CSL_FEXT(dssCtrl->DSS_L3RAM_MEMINIT_STATUS, DSS_CTRL_DSS_L3RAM_MEMINIT_STATUS_DSS_L3RAM_MEMINIT_STATUS_L3RAM2_MEMINIT_STATUS) != 0);
        while (CSL_FEXT(dssCtrl->DSS_L3RAM_MEMINIT_DONE, DSS_CTRL_DSS_L3RAM_MEMINIT_DONE_DSS_L3RAM_MEMINIT_DONE_L3RAM2_MEMINIT_DONE) != 0);
    }

    if (l3bankMask & RCM_MEMINIT_DSSL3_MEMBANK_RAM3)
    {
        /* Check MEMINIT STATUS is zero to confirm no inprogress MEM INIT */
        while (CSL_FEXT(dssCtrl->DSS_L3RAM_MEMINIT_STATUS, DSS_CTRL_DSS_L3RAM_MEMINIT_STATUS_DSS_L3RAM_MEMINIT_STATUS_L3RAM3_MEMINIT_STATUS) != 0);
        while (CSL_FEXT(dssCtrl->DSS_L3RAM_MEMINIT_DONE, DSS_CTRL_DSS_L3RAM_MEMINIT_DONE_DSS_L3RAM_MEMINIT_DONE_L3RAM3_MEMINIT_DONE) != 0);
    }
}

void SBL_RcmStartMeminitDSSL3(uint32_t l3bankMask)
{
    CSL_dss_ctrlRegs *dssCtrl = CSL_DSS_CTRL_getBaseAddress ();
    uint32_t memBankInit = 0;

    CSL_DSS_CTRL_enableAccess(dssCtrl);
    if (l3bankMask & RCM_MEMINIT_DSSL3_MEMBANK_RAM0)
    {
        memBankInit |= CSL_DSS_CTRL_DSS_L3RAM_MEMINIT_START_DSS_L3RAM_MEMINIT_START_L3RAM0_MEMINIT_START_MASK;
    }

    if (l3bankMask & RCM_MEMINIT_DSSL3_MEMBANK_RAM1)
    {
        memBankInit |= CSL_DSS_CTRL_DSS_L3RAM_MEMINIT_START_DSS_L3RAM_MEMINIT_START_L3RAM1_MEMINIT_START_MASK;
    }

    if (l3bankMask & RCM_MEMINIT_DSSL3_MEMBANK_RAM2)
    {
        memBankInit |= CSL_DSS_CTRL_DSS_L3RAM_MEMINIT_START_DSS_L3RAM_MEMINIT_START_L3RAM2_MEMINIT_START_MASK;
    }

    if (l3bankMask & RCM_MEMINIT_DSSL3_MEMBANK_RAM3)
    {
        memBankInit |= CSL_DSS_CTRL_DSS_L3RAM_MEMINIT_START_DSS_L3RAM_MEMINIT_START_L3RAM3_MEMINIT_START_MASK;
    }

    /* Start the Initialization of L2 Memory */
    dssCtrl->DSS_L3RAM_MEMINIT_START = CSL_insert8 (dssCtrl->DSS_L3RAM_MEMINIT_START, 3U, 0U, memBankInit);
}

void SBL_RcmStartMeminitTCMB(void)
{
    CSL_mss_ctrlRegs *mssCtrl = CSL_MSS_CTRL_getBaseAddress();

    /* Check MEMINIT STATUS is zero to confirm no inprogress MEM INIT */
    while (CSL_FEXT(mssCtrl->MSS_BTCM_MEM_INIT_STATUS, MSS_CTRL_MSS_BTCM_MEM_INIT_STATUS_MSS_BTCM_MEM_INIT_STATUS_MEM_STATUS) != 0);

    /* Clear MEMINIT DONE before initiating MEMINIT */
    CSL_FINS(mssCtrl->MSS_BTCM_MEM_INIT_DONE, MSS_CTRL_MSS_BTCM_MEM_INIT_DONE_MSS_BTCM_MEM_INIT_DONE_MEM_INIT_DONE, 1);
    while (CSL_FEXT(mssCtrl->MSS_BTCM_MEM_INIT_DONE, MSS_CTRL_MSS_BTCM_MEM_INIT_DONE_MSS_BTCM_MEM_INIT_DONE_MEM_INIT_DONE) != 0);

    CSL_FINS(mssCtrl->MSS_BTCM_MEM_INIT, MSS_CTRL_MSS_BTCM_MEM_INIT_MSS_BTCM_MEM_INIT_MEM_INIT, 1);
}


void SBL_RcmWaitMeminitTCMB(void)
{
    CSL_mss_ctrlRegs *mssCtrl = CSL_MSS_CTRL_getBaseAddress();

    while (CSL_FEXT(mssCtrl->MSS_BTCM_MEM_INIT_DONE, MSS_CTRL_MSS_BTCM_MEM_INIT_DONE_MSS_BTCM_MEM_INIT_DONE_MEM_INIT_DONE) != 1);
    CSL_FINS(mssCtrl->MSS_BTCM_MEM_INIT_DONE, MSS_CTRL_MSS_BTCM_MEM_INIT_DONE_MSS_BTCM_MEM_INIT_DONE_MEM_INIT_DONE, 1);

    /* Check MEMINIT STATUS is zero to confirm no inprogress MEM INIT */
    while (CSL_FEXT(mssCtrl->MSS_BTCM_MEM_INIT_STATUS, MSS_CTRL_MSS_BTCM_MEM_INIT_STATUS_MSS_BTCM_MEM_INIT_STATUS_MEM_STATUS) != 0);
    while (CSL_FEXT(mssCtrl->MSS_BTCM_MEM_INIT_DONE, MSS_CTRL_MSS_BTCM_MEM_INIT_DONE_MSS_BTCM_MEM_INIT_DONE_MEM_INIT_DONE) != 0);
}

static uint32_t SBL_RcmGetDSSFout(uint32_t Finp, bool div2flag)
{
    uint8_t pllSwitchFlag;
    CSL_mss_toprcmRegs *ptrTopRCMRegs;
    uint32_t FOut;

    ptrTopRCMRegs = CSL_TopRCM_getBaseAddress ();
    /* read the Core PLL Lock status */
    pllSwitchFlag = CSL_extract8 (ptrTopRCMRegs->PLL_DSP_STATUS, 10U, 10U);
    if (pllSwitchFlag)
    {

        uint32_t M, N, M2, FracM;

        N  = CSL_FEXT(ptrTopRCMRegs->PLL_DSP_M2NDIV, MSS_TOPRCM_PLL_DSP_M2NDIV_PLL_DSP_M2NDIV_N);
        M2 = CSL_FEXT(ptrTopRCMRegs->PLL_DSP_M2NDIV, MSS_TOPRCM_PLL_DSP_M2NDIV_PLL_DSP_M2NDIV_M2);
        M  = CSL_FEXT(ptrTopRCMRegs->PLL_DSP_MN2DIV, MSS_TOPRCM_PLL_DSP_MN2DIV_PLL_DSP_MN2DIV_M);
        FracM = CSL_FEXT(ptrTopRCMRegs->PLL_DSP_FRACDIV,MSS_TOPRCM_PLL_DSP_FRACDIV_PLL_DSP_FRACDIV_FRACTIONALM);
        FOut = SBL_RcmADPLLJGetFOut(Finp, N, M, M2, FracM, div2flag);
        DebugP_assert(FOut != 0);
    }
    else
    {
        uint32_t ULOWCLKEN = CSL_FEXT(ptrTopRCMRegs->PLL_DSP_CLKCTRL, MSS_TOPRCM_PLL_DSP_CLKCTRL_PLL_DSP_CLKCTRL_ULOWCLKEN);
        if (ULOWCLKEN == 0)
        {
            uint32_t N2;

            N2  = CSL_FEXT(ptrTopRCMRegs->PLL_DSP_MN2DIV, MSS_TOPRCM_PLL_DSP_MN2DIV_PLL_DSP_MN2DIV_N2);
            FOut = Finp/(N2 + 1);
        }
        else
        {
            FOut = Finp;
        }
    }
    return FOut;
}

static uint32_t SBL_RcmGetDSSHSDivOut(uint32_t Finp, bool div2flag, Rcm_PllHSDIVOutId hsDivOut)
{
    CSL_mss_toprcmRegs *ptrTopRCMRegs;
    uint32_t FOut;
    uint32_t clkDiv;

    FOut = SBL_RcmGetDSSFout(Finp, div2flag);
    ptrTopRCMRegs = CSL_TopRCM_getBaseAddress ();
    switch(hsDivOut)
    {
        case RCM_PLLHSDIV_OUT_0:
        {
            clkDiv = CSL_FEXT(ptrTopRCMRegs->PLL_DSP_HSDIVIDER_CLKOUT0, MSS_TOPRCM_PLL_DSP_HSDIVIDER_CLKOUT0_PLL_DSP_HSDIVIDER_CLKOUT0_DIV);
            break;
        }
        case RCM_PLLHSDIV_OUT_1:
        {
            clkDiv = CSL_FEXT(ptrTopRCMRegs->PLL_DSP_HSDIVIDER_CLKOUT1, MSS_TOPRCM_PLL_DSP_HSDIVIDER_CLKOUT1_PLL_DSP_HSDIVIDER_CLKOUT1_DIV);
            break;
        }
        case RCM_PLLHSDIV_OUT_2:
        {
            clkDiv = CSL_FEXT(ptrTopRCMRegs->PLL_DSP_HSDIVIDER_CLKOUT2, MSS_TOPRCM_PLL_DSP_HSDIVIDER_CLKOUT2_PLL_DSP_HSDIVIDER_CLKOUT2_DIV);
            break;
        }
        case RCM_PLLHSDIV_OUT_3:
        {
            clkDiv = CSL_FEXT(ptrTopRCMRegs->PLL_DSP_HSDIVIDER_CLKOUT3, MSS_TOPRCM_PLL_DSP_HSDIVIDER_CLKOUT3_PLL_DSP_HSDIVIDER_CLKOUT3_DIV);
            break;
        }
        case RCM_PLLHSDIV_OUT_NONE:
        {
            DebugP_assert(FALSE);
            clkDiv = 0;
            break;
        }
    }
    return (FOut/(clkDiv + 1));
}

static uint32_t SBL_RcmGetCoreFout(uint32_t Finp, bool div2flag)
{
    uint8_t pllSwitchFlag;
    CSL_mss_toprcmRegs *ptrTopRCMRegs;
    uint32_t FOut;

    ptrTopRCMRegs = CSL_TopRCM_getBaseAddress ();
    /* read the Core PLL Lock status */
    pllSwitchFlag = CSL_extract8 (ptrTopRCMRegs->PLL_CORE_STATUS, 10U, 10U);
    if (pllSwitchFlag)
    {
        uint32_t M, N, M2, FracM;

        N  = CSL_FEXT(ptrTopRCMRegs->PLL_CORE_M2NDIV, MSS_TOPRCM_PLL_CORE_M2NDIV_PLL_CORE_M2NDIV_N);
        M2 = CSL_FEXT(ptrTopRCMRegs->PLL_CORE_M2NDIV, MSS_TOPRCM_PLL_CORE_M2NDIV_PLL_CORE_M2NDIV_M2);
        M  = CSL_FEXT(ptrTopRCMRegs->PLL_CORE_MN2DIV, MSS_TOPRCM_PLL_CORE_MN2DIV_PLL_CORE_MN2DIV_M);
        FracM = CSL_FEXT(ptrTopRCMRegs->PLL_CORE_FRACDIV,MSS_TOPRCM_PLL_CORE_FRACDIV_PLL_CORE_FRACDIV_FRACTIONALM);
        FOut = SBL_RcmADPLLJGetFOut(Finp, N, M, M2, FracM,div2flag);
        DebugP_assert(FOut != 0);
    }
    else
    {
        uint32_t ULOWCLKEN = CSL_FEXT(ptrTopRCMRegs->PLL_CORE_CLKCTRL, MSS_TOPRCM_PLL_CORE_CLKCTRL_PLL_CORE_CLKCTRL_ULOWCLKEN);
        if (ULOWCLKEN == 0)
        {
            uint32_t N2;

            N2  = CSL_FEXT(ptrTopRCMRegs->PLL_CORE_MN2DIV, MSS_TOPRCM_PLL_CORE_MN2DIV_PLL_CORE_MN2DIV_N2);
            FOut = Finp/(N2 + 1);
        }
        else
        {
            FOut = Finp;
        }
    }
    return FOut;
}

static uint32_t SBL_RcmGetCoreHSDivOut(uint32_t Finp, bool div2flag, Rcm_PllHSDIVOutId hsDivOut)
{
    uint32_t FOut;
    uint32_t clkDiv;
    CSL_mss_toprcmRegs *ptrTopRCMRegs;

    ptrTopRCMRegs = CSL_TopRCM_getBaseAddress ();
    FOut = SBL_RcmGetCoreFout(Finp, div2flag);
    switch(hsDivOut)
    {
        case RCM_PLLHSDIV_OUT_0:
        {
            clkDiv = CSL_FEXT(ptrTopRCMRegs->PLL_CORE_HSDIVIDER_CLKOUT0, MSS_TOPRCM_PLL_CORE_HSDIVIDER_CLKOUT0_PLL_CORE_HSDIVIDER_CLKOUT0_DIV);
            break;
        }
        case RCM_PLLHSDIV_OUT_1:
        {
            clkDiv = CSL_FEXT(ptrTopRCMRegs->PLL_CORE_HSDIVIDER_CLKOUT1, MSS_TOPRCM_PLL_CORE_HSDIVIDER_CLKOUT1_PLL_CORE_HSDIVIDER_CLKOUT1_DIV);
            break;
        }
        case RCM_PLLHSDIV_OUT_2:
        {
            clkDiv = CSL_FEXT(ptrTopRCMRegs->PLL_CORE_HSDIVIDER_CLKOUT2, MSS_TOPRCM_PLL_CORE_HSDIVIDER_CLKOUT2_PLL_CORE_HSDIVIDER_CLKOUT2_DIV);
            break;
        }
        case RCM_PLLHSDIV_OUT_3:
        {
            clkDiv = CSL_FEXT(ptrTopRCMRegs->PLL_CORE_HSDIVIDER_CLKOUT3, MSS_TOPRCM_PLL_CORE_HSDIVIDER_CLKOUT3_PLL_CORE_HSDIVIDER_CLKOUT3_DIV);
            break;
        }
        case RCM_PLLHSDIV_OUT_NONE:
        {
            DebugP_assert(FALSE);
            clkDiv = 0;
            break;
        }
    }
    return (FOut/(clkDiv + 1));
}

static uint32_t SBL_RcmGetPerFout(uint32_t Finp, bool div2flag)
{
    uint8_t pllSwitchFlag;
    CSL_mss_toprcmRegs *ptrTopRCMRegs;
    uint32_t FOut;

    ptrTopRCMRegs = CSL_TopRCM_getBaseAddress ();
    /* read the Core PLL Lock status */
    pllSwitchFlag = CSL_extract8 (ptrTopRCMRegs->PLL_PER_STATUS, 10U, 10U);
    if (pllSwitchFlag)
    {
        uint32_t M, N, M2, FracM;

        N  = CSL_FEXT(ptrTopRCMRegs->PLL_PER_M2NDIV, MSS_TOPRCM_PLL_PER_M2NDIV_PLL_PER_M2NDIV_N);
        M2 = CSL_FEXT(ptrTopRCMRegs->PLL_PER_M2NDIV, MSS_TOPRCM_PLL_PER_M2NDIV_PLL_PER_M2NDIV_M2);
        M  = CSL_FEXT(ptrTopRCMRegs->PLL_PER_MN2DIV, MSS_TOPRCM_PLL_PER_MN2DIV_PLL_PER_MN2DIV_M);
        FracM = CSL_FEXT(ptrTopRCMRegs->PLL_PER_FRACDIV,MSS_TOPRCM_PLL_PER_FRACDIV_PLL_PER_FRACDIV_FRACTIONALM);
        FOut = SBL_RcmADPLLJGetFOut(Finp, N, M, M2, FracM, div2flag);
        DebugP_assert(FOut != 0);
    }
    else
    {
        uint32_t ULOWCLKEN = CSL_FEXT(ptrTopRCMRegs->PLL_PER_CLKCTRL, MSS_TOPRCM_PLL_PER_CLKCTRL_PLL_PER_CLKCTRL_ULOWCLKEN);
        if (ULOWCLKEN == 0)
        {
            uint32_t N2;

            N2  = CSL_FEXT(ptrTopRCMRegs->PLL_PER_MN2DIV, MSS_TOPRCM_PLL_PER_MN2DIV_PLL_PER_MN2DIV_N2);
            FOut = Finp/(N2 + 1);
        }
        else
        {
            FOut = Finp;
        }
    }
    return FOut;
}

static uint32_t SBL_RcmGetPerHSDivOut(uint32_t Finp, bool div2flag, Rcm_PllHSDIVOutId hsDivOut)
{
    uint32_t FOut;
    uint32_t clkDiv;
    CSL_mss_toprcmRegs *ptrTopRCMRegs;

    ptrTopRCMRegs = CSL_TopRCM_getBaseAddress ();

    FOut = SBL_RcmGetPerFout(Finp, div2flag);
    switch(hsDivOut)
    {
        case RCM_PLLHSDIV_OUT_0:
        {
            clkDiv = CSL_FEXT(ptrTopRCMRegs->PLL_PER_HSDIVIDER_CLKOUT0, MSS_TOPRCM_PLL_PER_HSDIVIDER_CLKOUT0_PLL_PER_HSDIVIDER_CLKOUT0_DIV);
            break;
        }
        case RCM_PLLHSDIV_OUT_1:
        {
            clkDiv = CSL_FEXT(ptrTopRCMRegs->PLL_PER_HSDIVIDER_CLKOUT1, MSS_TOPRCM_PLL_PER_HSDIVIDER_CLKOUT1_PLL_PER_HSDIVIDER_CLKOUT1_DIV);
            break;
        }
        case RCM_PLLHSDIV_OUT_2:
        {
            clkDiv = CSL_FEXT(ptrTopRCMRegs->PLL_PER_HSDIVIDER_CLKOUT2, MSS_TOPRCM_PLL_PER_HSDIVIDER_CLKOUT2_PLL_PER_HSDIVIDER_CLKOUT2_DIV);
            break;
        }
        case RCM_PLLHSDIV_OUT_3:
        {
            clkDiv = CSL_FEXT(ptrTopRCMRegs->PLL_PER_HSDIVIDER_CLKOUT3, MSS_TOPRCM_PLL_PER_HSDIVIDER_CLKOUT3_PLL_PER_HSDIVIDER_CLKOUT3_DIV);
            break;
        }
        case RCM_PLLHSDIV_OUT_NONE:
        {
            DebugP_assert(FALSE);
            clkDiv = 0;
            break;
        }
    }

    return (FOut/(clkDiv  + 1));
}

static uint32_t SBL_RcmADPLLJGetFOut(uint32_t Finp, uint32_t N, uint32_t M, uint32_t M2, uint32_t FracM, bool div2flag)
{
    uint32_t i;
    uint32_t FOut;
    uint32_t Nmatch;

    if (div2flag)
    {
        Nmatch = ((N + 1) * 2) - 1;
    }
    else
    {
        Nmatch = N;
    }
    for (i = 0; i < SBL_UTILS_ARRAYSIZE(gADPLLJConfigTbl); i++)
    {
        if ((gADPLLJConfigTbl[i].Finp == Finp)   &&
            (gADPLLJConfigTbl[i].FracM == FracM) &&
            (gADPLLJConfigTbl[i].M == M)         &&
            (gADPLLJConfigTbl[i].M2 == M2)       &&
            (gADPLLJConfigTbl[i].N  == Nmatch))
        {
            break;
        }
    }
    if (i < SBL_UTILS_ARRAYSIZE(gADPLLJConfigTbl))
    {
        FOut = SBL_FREQ_MHZ2HZ(gADPLLJConfigTbl[i].Fout);
    }
    else
    {
        FOut = 0;
    }
    return FOut;
}

void SBL_RcmGetEfuseQSPIConfig(Rcm_EfuseQspiConfig *qspiEfuseCfg)
{
    CSL_top_ctrlRegs* ptrTopCtrlRegs;

    ptrTopCtrlRegs = CSL_TopCtrl_getBaseAddress ();

    qspiEfuseCfg->QSPIClockFreqConfig  = CSL_TopCtrl_readQSPIClkFreqEfuse(ptrTopCtrlRegs);
    qspiEfuseCfg->flashClockModeConfig = CSL_TopCtrl_readFlashModeEfuse(ptrTopCtrlRegs);

}


