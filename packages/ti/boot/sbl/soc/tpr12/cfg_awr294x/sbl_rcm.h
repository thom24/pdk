/*
 * drv_rcm.h
 *
 * RCM Driver header file
 *
 * Copyright (C) 2020 Texas Instruments Incorporated - http://www.ti.com/
 * ALL RIGHTS RESERVED
 *
 */

#ifndef SBL_RCM_H
#define SBL_RCM_H

/**************************************************************************
 *************************** Include Files ********************************
 **************************************************************************/

/* Standard Include Files. */
#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief
 *  Reset Causes
 *
 * @details
 *  This enumeration captures all the possible Reset Causes
 */
typedef enum Rcm_ResetCause_e
{
    /**
     * @brief   Value specifying Power ON Reset
     */
    Rcm_ResetCause_POWER_ON_RESET = 0x0U,
    /**
     * @brief   Value specifying Warm Reset
     */
    Rcm_ResetCause_WARM_RESET = 0x1U,
    /**
     * @brief   Value specifying STC Reset
     */
    Rcm_ResetCause_STC_RESET = 0x2U,
    /**
     * @brief   Value specifying R5 Core A Subsytem Reset
     */
    Rcm_ResetCause_MMR_CPU0_VIM0_RESET = 0x3U,
    /**
     * @brief   Value specifying R5 Core B Subsytem Reset
     */
    Rcm_ResetCause_MMR_CPU1_VIM1_RESET = 0x4U,
    /**
     * @brief   Value specifying R5 Core A (core only) Reset
     */
    Rcm_ResetCause_MMR_CPU0_RESET = 0x5U,
    /**
     * @brief   Value specifying R5 Core B (core only) Reset
     */
    Rcm_ResetCause_MMR_CPU1_RESET = 0x6U,
    /**
     * @brief   Value specifying R5 Core A Debug Reset
     */
    Rcm_ResetCause_DBG_CPU0_RESET = 0x7U,
    /**
     * @brief   Value specifying R5 Core B Debug Reset
     */
    Rcm_ResetCause_DBG_CPU1_RESET = 0x8U,
    /**
     * @brief   Value specifying R5 Reset due to FSM Trigger
     */
    Rcm_ResetCause_FSM_TRIGGER_RESET = 0x9U,
    /**
     * @brief   Value specifying R5 Reset due to Unknown reason
     */
    Rcm_ResetCause_RST_CAUSE_UNKNOWN = 0xAU,

}Rcm_ResetCause;

/**
 * @brief
 *  Peripheral Ids
 *
 * @details
 *  This enumeration captures all the Peripherals
 */
typedef enum Rcm_PeripheralId_e
{
    Rcm_PeripheralId_CSIRX,

    Rcm_PeripheralId_MSS_RTIA,
    Rcm_PeripheralId_MSS_RTIB,
    Rcm_PeripheralId_MSS_RTIC,
    Rcm_PeripheralId_MSS_WDT,
    Rcm_PeripheralId_MSS_QSPI,
    Rcm_PeripheralId_MSS_SPIA,
    Rcm_PeripheralId_MSS_SPIB,
    Rcm_PeripheralId_MSS_I2C,
    Rcm_PeripheralId_MSS_SCIA,
    Rcm_PeripheralId_MSS_SCIB,
    Rcm_PeripheralId_MSS_MCANA,
    Rcm_PeripheralId_MSS_MCANB,
    Rcm_PeripheralId_MSS_CPTS,
    Rcm_PeripheralId_MSS_CPSW,

    Rcm_PeripheralId_DSS_HWA,
    Rcm_PeripheralId_DSS_RTIA,
    Rcm_PeripheralId_DSS_RTIB,
    Rcm_PeripheralId_DSS_WDT,
    Rcm_PeripheralId_DSS_SCIA,

}Rcm_PeripheralId;

typedef enum Rcm_PeripheralClockSource_e
{
    Rcm_PeripheralClockSource_DPLL_CORE_HSDIV0_CLKOUT1,
    Rcm_PeripheralClockSource_DPLL_CORE_HSDIV0_CLKOUT2,
    Rcm_PeripheralClockSource_DPLL_DSP_HSDIV0_CLKOUT2,
    Rcm_PeripheralClockSource_DPLL_PER_HSDIV0_CLKOUT1,
    Rcm_PeripheralClockSource_DPLL_PER_HSDIV0_CLKOUT2,
    Rcm_PeripheralClockSource_FE1_REF_CLK,
    Rcm_PeripheralClockSource_RCCLK10M,
    Rcm_PeripheralClockSource_RCCLK32K,
    Rcm_PeripheralClockSource_SYS_CLK,
    Rcm_PeripheralClockSource_WUCPUCLK,
    Rcm_PeripheralClockSource_XREF_CLK0,
    Rcm_PeripheralClockSource_XREF_CLK1,
    Rcm_PeripheralClockSource_XTALCLK,
} Rcm_PeripheralClockSource;

typedef enum Rcm_HSIClockSource_e
{
    Rcm_HSIClockSource_PLL_CORE_CLK,
    Rcm_HSIClockSource_APLL_CLK_HSI,
    Rcm_HSIClockSource_APLL_1p8GHZ,
    Rcm_HSIClockSource_PLL_PER_CLK,
    Rcm_HSIClockSource_DPLL_CORE_HSDIV0_CLKOUT0,
    Rcm_HSIClockSource_RCCLK10M,
    Rcm_HSIClockSource_DPLL_DSP_HSDIV0_CLKOUT0,
    Rcm_HSIClockSource_DPLL_PER_HSDIV0_CLKOUT0,
} Rcm_HSIClockSource;

typedef enum Rcm_ApllId_e
{
    RCM_APLLID_1P2G,
    RCM_APLLID_1P8G,
} Rcm_ApllId;

typedef enum Rcm_RssBssFrcClockSource_e
{
    Rcm_RssBssFrcClockSource_XTALCLK,
    Rcm_RssBssFrcClockSource_WUCPUCLK,
    Rcm_RssBssFrcClockSource_SYS_CLK,
    Rcm_RssBssFrcClockSource_DPLL_PER_HSDIV0_CLKOUT1,
    Rcm_RssBssFrcClockSource_APLL_1p8G_HSDIV0_CLKOUT2,
    Rcm_RssBssFrcClockSource_RCCLK10M,
    Rcm_RssBssFrcClockSource_XREF_CLK0,
    Rcm_RssBssFrcClockSource_RCCLK32K,
} Rcm_RssBssFrcClockSource;


typedef enum Rcm_DSPClockSource_e
{
    Rcm_DSPClockSource_XTALCLK,
    Rcm_DSPClockSource_RCCLK10M,
    Rcm_DSPClockSource_DPLL_DSP_HSDIV0_CLKOUT1,
    Rcm_DSPClockSource_DPLL_DSP_HSDIV0_CLKOUT1_DITH,
    Rcm_DSPClockSource_DPLL_CORE_HSDIV0_CLKOUT1,
    Rcm_DSPClockSource_DPLL_PER_HSDIV0_CLKOUT3,
}Rcm_DSPClockSource;

typedef enum Rcm_CR5ClockSource_e
{
    Rcm_CR5ClockSource_XTALCLK,
    Rcm_CR5ClockSource_RCCLK10M,
    Rcm_CR5ClockSource_DPLL_CORE_HSDIV0_CLKOUT2,
    Rcm_CR5ClockSource_SYS_CLK,
    Rcm_CR5ClockSource_WUCPUCLK,
}Rcm_CR5ClockSource;

typedef enum Rcm_HSDIVClkOutMuxClockSource_e
{
    Rcm_HSDIVClkOutMuxClockSource_DPLL_CORE_HSDIV0_CLKOUT2_PreMux,
    Rcm_HSDIVClkOutMuxClockSource_DPLL_DSP_HSDIV0_CLKOUT1_PreMux,
    Rcm_HSDIVClkOutMuxClockSource_DPLL_DSP_HSDIV0_CLKOUT2_PreMux,
    Rcm_HSDIVClkOutMuxClockSource_DPLL_PER_HSDIV0_CLKOUT1_PreMux,
    Rcm_HSDIVClkOutMuxClockSource_APLL_1p2G_HSDIV0_CLKOUT0,
    Rcm_HSDIVClkOutMuxClockSource_APLL_1p2G_HSDIV0_CLKOUT1,
    Rcm_HSDIVClkOutMuxClockSource_APLL_1p2G_HSDIV0_CLKOUT2,
    Rcm_HSDIVClkOutMuxClockSource_APLL_1p2G_HSDIV0_CLKOUT3,
    Rcm_HSDIVClkOutMuxClockSource_APLL_1p8G_HSDIV0_CLKOUT0,
    Rcm_HSDIVClkOutMuxClockSource_APLL_1p8G_HSDIV0_CLKOUT1,
    Rcm_HSDIVClkOutMuxClockSource_APLL_400MHZ,
}Rcm_HSDIVClkOutMuxClockSource;

typedef enum Rcm_HSDIVClkOutMuxId_e
{
    Rcm_HSDIVClkOutMuxId_DPLL_CORE_OUT2,
    Rcm_HSDIVClkOutMuxId_DPLL_DSP_OUT1,
    Rcm_HSDIVClkOutMuxId_DPLL_DSP_OUT2,
    Rcm_HSDIVClkOutMuxId_DPLL_PER_OUT1,
}Rcm_HSDIVClkOutMuxId;


typedef enum Rcm_RssClkSrcId_e
{
    Rcm_RssClkSrcId_WUCPUCLK,
    Rcm_RssClkSrcId_XTALCLK,
    Rcm_RssClkSrcId_DPLL_CORE_HSDIV0_CLKOUT2_MUXED,
    Rcm_RssClkSrcId_DPLL_PER_HSDIV0_CLKOUT1_MUXED,
    Rcm_RssClkSrcId_APLL_1P8_HSDIV0_CLKOUT2,
    Rcm_RssClkSrcId_RCCLK10M,
    Rcm_RssClkSrcId_SYS_CLK,
} Rcm_RssClkSrcId;

/**
 * @brief
 *  RCM Error code
 *
 * @details
 *  The enumeration describes all the possible return and error codes which
 *  the RCM driver functions can return.
 */
typedef enum Rcm_Return_e
{
    /**
     * @brief   Success/pass return code
     */
    Rcm_Return_SUCCESS = 0xB4A95785U,

    /**
     * @brief   General or unspecified failure/error
     */
    Rcm_Return_ERROR   = 0xCF57A047U
}Rcm_Return;

/**
 * @brief
 *  SOP Modes
 *
 * @details
 *  The enumeration describes all the possible SOP Modes
 */
typedef enum RcmSOPMode_e
{
    /**
     * @brief   SOP Mode 1 - ATPG Mode
     */
    Rcm_SOPMode_MODE_ATPG = 0x1U,
    /**
     * @brief   SOP Mode 2 - FLED Mode
     */
    Rcm_SOPMode_MODE_FLED,
    /**
     * @brief   SOP Mode 3 - THB(Test) Mode
     */
    Rcm_SOPMode_MODE_THB,
    /**
     * @brief   SOP Mode 4 - QSPI Functional Mode
     */
    Rcm_SOPMode_MODE_QSPI_FUNCTIONAL,
    /**
     * @brief   SOP Mode 5 - UART Functional Mode
     */
    Rcm_SOPMode_MODE_UART_FUNCTIONAL,
    /**
     * @brief  INVALID SOP Mode
     */
    Rcm_SOPMode_MODE_INVALID,
}Rcm_SOPMode;

typedef enum RcmPllFoutFreqId_e
{
    RCM_PLL_FOUT_FREQID_CLK_800MHZ,
    RCM_PLL_FOUT_FREQID_CLK_2000MHZ,
    RCM_PLL_FOUT_FREQID_CLK_900MHZ,
    RCM_PLL_FOUT_FREQID_CLK_1800MHZ,
    RCM_PLL_FOUT_FREQID_CLK_1920MHZ,
    RCM_PLL_FOUT_FREQID_CLK_1699p21875MHZ,
    RCM_PLL_FOUT_FREQID_CLK_1728MHZ,
    RCM_PLL_FOUT_FREQID_CLK_1966p08MHZ,
    RCM_PLL_FOUT_FREQID_CLK_1806p336MHZ,
    RCM_PLL_FOUT_FREQID_CLK_600MHZ,
} Rcm_PllFoutFreqId;

#define RCM_MEMINIT_DSSL2_MEMBANK_VB00                                (1U << 0U)
#define RCM_MEMINIT_DSSL2_MEMBANK_VB01                                (1U << 1U)
#define RCM_MEMINIT_DSSL2_MEMBANK_VB10                                (1U << 2U)
#define RCM_MEMINIT_DSSL2_MEMBANK_VB11                                (1U << 3U)
#define RCM_MEMINIT_DSSL2_MEMBANK_VB20                                (1U << 4U)
#define RCM_MEMINIT_DSSL2_MEMBANK_VB21                                (1U << 5U)
#define RCM_MEMINIT_DSSL2_MEMBANK_VB30                                (1U << 6U)
#define RCM_MEMINIT_DSSL2_MEMBANK_VB31                                (1U << 7U)
#define RCM_MEMINIT_DSSL2_MEMBANK_ALL        (RCM_MEMINIT_DSSL2_MEMBANK_VB00 | \
                                              RCM_MEMINIT_DSSL2_MEMBANK_VB01 | \
                                              RCM_MEMINIT_DSSL2_MEMBANK_VB10 | \
                                              RCM_MEMINIT_DSSL2_MEMBANK_VB11 | \
                                              RCM_MEMINIT_DSSL2_MEMBANK_VB20 | \
                                              RCM_MEMINIT_DSSL2_MEMBANK_VB21 | \
                                              RCM_MEMINIT_DSSL2_MEMBANK_VB30 | \
                                              RCM_MEMINIT_DSSL2_MEMBANK_VB31)


#define RCM_MEMINIT_DSSL3_MEMBANK_RAM0                                (1U << 0U)
#define RCM_MEMINIT_DSSL3_MEMBANK_RAM1                                (1U << 1U)
#define RCM_MEMINIT_DSSL3_MEMBANK_RAM2                                (1U << 2U)
#define RCM_MEMINIT_DSSL3_MEMBANK_RAM3                                (1U << 3U)
#define RCM_MEMINIT_DSSL3_MEMBANK_ALL        (RCM_MEMINIT_DSSL3_MEMBANK_RAM0 | \
                                              RCM_MEMINIT_DSSL3_MEMBANK_RAM1 | \
                                              RCM_MEMINIT_DSSL3_MEMBANK_RAM2 | \
                                              RCM_MEMINIT_DSSL3_MEMBANK_RAM3)

#define RCM_PLL_HSDIV_OUTPUT_ENABLE_0                                 (1U << 0U)
#define RCM_PLL_HSDIV_OUTPUT_ENABLE_1                                 (1U << 1U)
#define RCM_PLL_HSDIV_OUTPUT_ENABLE_2                                 (1U << 2U)
#define RCM_PLL_HSDIV_OUTPUT_ENABLE_3                                 (1U << 3U)
#define RCM_PLL_HSDIV_OUTPUT_ENABLE_ALL     (RCM_PLL_HSDIV_OUTPUT_ENABLE_0   | \
                                             RCM_PLL_HSDIV_OUTPUT_ENABLE_1   | \
                                             RCM_PLL_HSDIV_OUTPUT_ENABLE_2   | \
                                             RCM_PLL_HSDIV_OUTPUT_ENABLE_3)

#define RCM_PLL_HSDIV_OUTPUT_IDX0                                            (0)
#define RCM_PLL_HSDIV_OUTPUT_IDX1                                            (1)
#define RCM_PLL_HSDIV_OUTPUT_IDX2                                            (2)
#define RCM_PLL_HSDIV_OUTPUT_IDX3                                            (3)
#define RCM_PLL_HSDIV_OUTPUT_COUNT               (RCM_PLL_HSDIV_OUTPUT_IDX3 + 1)

typedef struct Rcm_PllHsDivOutConfig_s
{
    uint32_t hsdivOutEnMask;
    uint32_t hsDivOutFreqHz[RCM_PLL_HSDIV_OUTPUT_COUNT];
} Rcm_PllHsDivOutConfig;

#define RCM_EFUSE_QSPICLOCKFREQ_40MHz                                     (0x0U)
#define RCM_EFUSE_QSPICLOCKFREQ_60MHz                                     (0x1U)
#define RCM_EFUSE_QSPICLOCKFREQ_80MHz                                     (0x2U)

#define RCM_EFUSE_FLASHCLKMODE_0                                          (0x0U)
#define RCM_EFUSE_FLASHCLKMODE_3                                          (0x3U)

#define RCM_EFUSE_R5_CLK_300_SYS_CLK_150MHz                               (0x0U)
#define RCM_EFUSE_R5_CLK_200_SYS_CLK_200MHz                               (0x5U)
#define RCM_EFUSE_R5_CLK_400_SYS_CLK_200MHz                               (0xAU)


typedef struct Rcm_EfuseQspiConfig_s
{
    /**
     * @brief   Efuse value for Flash Clock Mode. Only 1-0 bits are valid
     *          RCM_EFUSE_FLASHCLKMODE_0    - Phase 0 : Polarity 0
     *          RCM_EFUSE_FLASHCLKMODE_3    - Phase 1 : Polarity 1
     */
    uint8_t           flashClockModeConfig;
    /**
     * @brief   Efuse value for QSPI Clock Frequency. Only 1-0 bits are valid
     *          RCM_EFUSE_QSPICLOCKFREQ_40MHz   - QSPI output clock 40Mhz
     *          RCM_EFUSE_QSPICLOCKFREQ_60MHz   - QSPI output clock 57.14Mhz
     *          RCM_EFUSE_QSPICLOCKFREQ_80MHz   - QSPI output clock 80Mhz
     */
    uint8_t           QSPIClockFreqConfig;
} Rcm_EfuseQspiConfig;

typedef struct Rcm_EfuseBootFreqConfig_s
{
    uint32_t           r5FreqHz;
    uint32_t           sysClkFreqHz;
} Rcm_EfuseBootFreqConfig;

typedef struct Rcm_DeviceFreqConfig_s
{
    uint32_t           dspFreqHz;
    uint32_t           r5FreqHz;
    uint32_t           sysClkFreqHz;
} Rcm_DeviceFreqConfig;

typedef enum RcmEfusePkgType_e
{
    RCM_EFUSE_DEVICE_PKG_TYPE_LOP, /* 0b011 */
    RCM_EFUSE_DEVICE_PKG_TYPE_ETS /* 0b010 */
} RcmEfusePkgType;


/**************************************************************************
 ************************* Extern Declarations ****************************
 **************************************************************************/
extern void SBL_RcmCoreDpllConfig(Rcm_PllFoutFreqId outFreqId, Rcm_PllHsDivOutConfig *hsDivCfg);
extern void SBL_RcmDspDpllConfig(Rcm_PllFoutFreqId outFreqId, Rcm_PllHsDivOutConfig *hsDivCfg);
extern void SBL_RcmPerDpllConfig(Rcm_PllFoutFreqId outFreqId, Rcm_PllHsDivOutConfig *hsDivCfg);
extern void SBL_RcmApllHSDivConfig(Rcm_ApllId apllId, Rcm_PllHsDivOutConfig *hsDivCfg);

extern void SBL_RcmStartMeminitTCMA(void);
#if defined (SOC_AWR294X)
extern void SBL_RcmStartMeminitTCMBSS(void);
extern void SBL_RcmStartMeminitStaticBSS(void);
extern void SBL_RcmStartMeminitSharedBSS(void);
extern void SBL_RcmBSSControl(void);
extern void SBL_RcmPopulateBSSControl(void);
#endif /* defined (SOC_AWR294X) */
extern void SBL_RcmStartMeminitTCMB(void);
extern void SBL_RcmStartMeminitL2(void);
extern void SBL_RcmStartMeminitDSSL2(uint32_t l2bankMask);
extern void SBL_RcmStartMeminitDSSL3(uint32_t l3bankMask);

extern void SBL_RcmWaitMeminitTCMA(void);
#if defined (SOC_AWR294X)
extern void SBL_RcmWaitMeminitTCMBSS(void);
extern void SBL_RcmWaitMeminitStaticBSS(void);
extern void SBL_RcmWaitMeminitSharedBSS(void);
#endif /* defined (SOC_AWR294X) */
extern void SBL_RcmWaitMeminitTCMB(void);
extern void SBL_RcmWaitMeminitL2(void);
extern void SBL_RcmWaitMeminitDSSL2(uint32_t l2bankMask);
extern void SBL_RcmWaitMeminitDSSL3(uint32_t l3bankMask);


extern void SBL_RcmSetCR5SysClock(uint32_t cr5FreqHz, uint32_t sysClkFreqHz);
extern uint32_t SBL_RcmGetCR5CoreFrequency(void);

extern Rcm_Return SBL_RcmSetDspCoreClock (Rcm_DSPClockSource clkSource, uint32_t freqHz);
extern uint32_t SBL_RcmGetDSPClockFrequency(Rcm_DSPClockSource clkSource);

extern Rcm_Return SBL_RcmSetPeripheralClock(Rcm_PeripheralId PeriphID, Rcm_PeripheralClockSource clkSource, uint32_t freqHz);
extern uint32_t SBL_RcmGetPeripheralClockFrequency(Rcm_PeripheralClockSource clkSource);

extern Rcm_ResetCause SBL_RcmGetResetCause(void);

extern void SBL_RcmGetEfuseQSPIConfig(Rcm_EfuseQspiConfig *qspiEfuseCfg);

extern Rcm_Return SBL_RcmGetPeripheralFreq (Rcm_PeripheralId periphID,
                                            uint32_t *freqHz);
extern uint32_t SBL_RcmGetCR5Freq(void);

extern Rcm_Return SBL_RcmSetHSDivMux (Rcm_HSDIVClkOutMuxId clkOutMuxId, 
                               Rcm_HSDIVClkOutMuxClockSource muxSource);

extern Rcm_Return SBL_RcmSetRssClkFreq (Rcm_RssClkSrcId rssClkSrcId,
                                        uint32_t freqHz);
extern void SBL_RcmBSSR4Unhalt(void);
extern void SBL_RcmWaitBSSBootComplete(void);
extern void SBL_RcmGetEfuseBootFrequency(Rcm_EfuseBootFreqConfig *bootFreqEfuseCfg);
extern void SBL_RcmGetDeviceFrequency(Rcm_DeviceFreqConfig *deviceFreqEfuseCfg);
extern void SBL_RcmGetPackageType(RcmEfusePkgType *deviceTypeEfuse);
extern void SBL_RcmCoreApllHSDivConfig(Rcm_PllHsDivOutConfig *hsDivCfg);
extern void SBL_rcmConfigEthMacIf(void);

extern uint32_t SBL_rcmIsDualCoreSwitchSupported(void);
extern Rcm_Return SBL_RcmSetHSIClock (Rcm_HSIClockSource clkSource,
                               uint32_t freqHz);
extern void SBL_RcmCoreDpllHSDivOutEnable(uint32_t hsDivOutIdx, uint32_t divVal);

extern void SBL_RcmCoreDpllDisable(void);
extern void SBL_RcmDspDpllDisable(void);
extern void SBL_RcmPerDpllDisable(void);

#ifdef __cplusplus
}
#endif

#endif /* SBL_RCM_H */
