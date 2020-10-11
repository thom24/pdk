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
    /**
     * @brief   Value specifying CSI RX
     */
    Rcm_PeripheralId_CSIRX,
    /**
     * @brief   Value specifying MCANA
     */
    Rcm_PeripheralId_MSS_MCANA,
    /**
     * @brief   Value specifying MCANB
     */
    Rcm_PeripheralId_MSS_MCANB,
    /**
     * @brief   Value specifying QSPI (Quad SPI)
     */
    Rcm_PeripheralId_MSS_QSPI,
    /**
     * @brief   Value specifying MSS RTIA (Timer)
     */
    Rcm_PeripheralId_MSS_RTIA,
    /**
     * @brief   Value specifying MSS RTIB (Timer)
     */
    Rcm_PeripheralId_MSS_RTIB,
    /**
     * @brief   Value specifying MSS RTIC (Timer)
     */
    Rcm_PeripheralId_MSS_RTIC,
    /**
     * @brief   Value specifying MSS WatchDog
     */
    Rcm_PeripheralId_MSS_WDT,
    /**
     * @brief   Value specifying MSS SPI-A
     */
    Rcm_PeripheralId_MSS_SPIA,
    /**
     * @brief   Value specifying MSS SPI-B
     */
    Rcm_PeripheralId_MSS_SPIB,
    /**
     * @brief   Value specifying MSS I2C
     */
    Rcm_PeripheralId_MSS_I2C,
    /**
     * @brief   Value specifying MSS SCI-A (UART)
     */
    Rcm_PeripheralId_MSS_SCIA,
    /**
     * @brief   Value specifying MSS SCI-B (UART)
     */
    Rcm_PeripheralId_MSS_SCIB,
    /**
     * @brief   Value specifying CPTS (Timesync module)
     */
    Rcm_PeripheralId_MSS_CPTS,
    /**
     * @brief   Value specifying CPSW (2 port ethernet switch)
     */
    Rcm_PeripheralId_MSS_CPSW,
    /**
     * @brief   Value specifying DSS RTIA (Timer)
     */
    Rcm_PeripheralId_DSS_RTIA,
    /**
     * @brief   Value specifying DSS RTIB (Timer)
     */
    Rcm_PeripheralId_DSS_RTIB,
    /**
     * @brief   Value specifying DSS WatchDog
     */
    Rcm_PeripheralId_DSS_WDT,
    /**
     * @brief   Value specifying DSS SCI-A (UART)
     */
    Rcm_PeripheralId_DSS_SCIA,
    /**
     * @brief   Value specifying RCSS I2CA
     */
    Rcm_PeripheralId_RCSS_I2CA,
    /**
     * @brief   Value specifying RCSS I2CB
     */
    Rcm_PeripheralId_RCSS_I2CB,
    /**
     * @brief   Value specifying RCSS SCIA
     */
    Rcm_PeripheralId_RCSS_SCIA,
    /**
     * @brief   Value specifying RCSS SPIA
     */
    Rcm_PeripheralId_RCSS_SPIA,
    /**
     * @brief   Value specifying RCSS SCIB
     */
    Rcm_PeripheralId_RCSS_SPIB,
    /**
     * @brief   Value specifying RCSS ATL
     */
    Rcm_PeripheralId_RCSS_ATL,
    /**
     * @brief   Value specifying RCSS MCASPA AUX
     */
    Rcm_PeripheralId_RCSS_MCASPA_AUX,
    /**
     * @brief   Value specifying RCSS MCASPB AUX
     */
    Rcm_PeripheralId_RCSS_MCASPB_AUX,
    /**
     * @brief   Value specifying RCSS MCASPC AUX
     */
    Rcm_PeripheralId_RCSS_MCASPC_AUX,
}Rcm_PeripheralId;

typedef enum Rcm_PeripheralClockSource_e
{
    /**
     * @brief   Value specifying Crystal Clock
     */
    Rcm_PeripheralClockSource_XTAL_CLK,
    /**
     * @brief   Value specifying System Clock (200Mhz)
     */
    Rcm_PeripheralClockSource_SYS_CLK,
    /**
     * @brief   Value specifying PLL Core Clock Out 2 (400 Mhz)
     */
    Rcm_PeripheralClockSource_DPLL_CORE_HSDIV0_CLKOUT2,
    /**
     * @brief   Value specifying PLL Core Clock Out 1 (192 Mhz)
     */
    Rcm_PeripheralClockSource_DPLL_PER_HSDIV0_CLKOUT1,
    /**
     * @brief   Value specifying PLL Core Clock Out 2 (96 Mhz)
     */
    Rcm_PeripheralClockSource_DPLL_PER_HSDIV0_CLKOUT2,
    /**
     * @brief   Value specifying PLL Core Clock Out 3 (172.8 Mhz)
     */
    Rcm_PeripheralClockSource_DPLL_PER_HSDIV0_CLKOUT3,
} Rcm_PeripheralClockSource;


typedef enum Rcm_DSPClockSource_e
{
    /**
     * @brief   Value specifying Crystal Clock
     */
    Rcm_DSPClockSource_XTAL_CLK,

    /**
     * @brief   Value specifying PLL DSP Clock Out 2 (450 Mhz)
     */
    Rcm_DSPClockSource_DPLL_DSP_HSDIV0_CLKOUT1,

    /**
     * @brief   Value specifying PLL Core Clock Out 1
     */
    Rcm_DSPClockSource_DPLL_CORE_HSDIV0_CLKOUT1

}Rcm_DSPClockSource;

typedef enum Rcm_CR5ClockSource_e
{
    /**
     * @brief   Value specifying PLL Core Clock Out 2
     */
    Rcm_CR5ClockSource_DPLL_CORE_HSDIV0_CLKOUT2

}Rcm_CR5ClockSource;

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
    RCM_PLL_FOUT_FREQID_CLK_900MHZ,
    RCM_PLL_FOUT_FREQID_CLK_2000MHZ,
    RCM_PLL_FOUT_FREQID_CLK_1800MHZ,
    RCM_PLL_FOUT_FREQID_CLK_1920MHZ,
    RCM_PLL_FOUT_FREQID_CLK_1699p21875MHZ,
    RCM_PLL_FOUT_FREQID_CLK_1728MHZ,
    RCM_PLL_FOUT_FREQID_CLK_1966p08MHZ,
    RCM_PLL_FOUT_FREQID_CLK_1806p336MHZ,
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


/**************************************************************************
 ************************* Extern Declarations ****************************
 **************************************************************************/
extern void SBL_RcmCoreApllConfig(Rcm_PllFoutFreqId outFreqId, Rcm_PllHsDivOutConfig *hsDivCfg);
extern void SBL_RcmDspApllConfig(Rcm_PllFoutFreqId outFreqId, Rcm_PllHsDivOutConfig *hsDivCfg);
extern void SBL_RcmPerApllConfig(Rcm_PllFoutFreqId outFreqId, Rcm_PllHsDivOutConfig *hsDivCfg);

extern void SBL_RcmStartMeminitTCMA(void);
extern void SBL_RcmStartMeminitTCMB(void);
extern void SBL_RcmStartMeminitL2(void);
extern void SBL_RcmStartMeminitDSSL2(uint32_t l2bankMask);
extern void SBL_RcmStartMeminitDSSL3(uint32_t l3bankMask);

extern void SBL_RcmWaitMeminitTCMA(void);
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

#ifdef __cplusplus
}
#endif

#endif /* SBL_RCM_H */
