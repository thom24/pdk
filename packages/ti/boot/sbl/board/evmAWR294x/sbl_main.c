/**
 *  \file   sbl_main.c
 *
 *  \brief  This file contain main function, call the Board Initialization
 *          functions & slave core boot-up functions in sequence.
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

 /* TI RTOS header files */
#include "sbl_main.h"
#include "sbl_rcm.h"

/**********************************************************************
 ************************** Macros ************************************
 **********************************************************************/

/**********************************************************************
 ************************** Internal functions ************************
 **********************************************************************/

/**********************************************************************
 ************************** Global Variables **************************
 **********************************************************************/
extern sblProfileInfo_t sblProfileLog[MAX_PROFILE_LOG_ENTRIES];
extern uint32_t sblProfileLogIndx;
extern uint32_t sblProfileLogOvrFlw;

#pragma DATA_SECTION(sblProfileLogAddr, ".sbl_profile_info")
volatile sblProfileInfo_t * sblProfileLogAddr;

#pragma DATA_SECTION(sblProfileLogIndxAddr, ".sbl_profile_info")
volatile uint32_t *sblProfileLogIndxAddr;

#pragma DATA_SECTION(sblProfileLogOvrFlwAddr, ".sbl_profile_info")
volatile uint32_t *sblProfileLogOvrFlwAddr;

sblEntryPoint_t tpr12_evmEntry;

 /**************************************************************
 * MPU Configuration:
 * - Region0 : Background 4GB
 * - Region1 : TCMA
 * - Region2 : TCMB
 * - Region3 : L2 memory: read/write, no execute permission
 * - Region4 : R5 register space (0x0200:0000 - 0x3FF:FFFF)
 * - Region5 : Mailbox TX RAM
 * - Region6 : Mailbox RX RAM
 * - Region7 : External flash
 * - Region8 : QSPI register space
 * - Region9 : R5 Register space(0x0600:0000 - 0x7FF:FFFF)
 **************************************************************/
const CSL_ArmR5MpuRegionCfg gCslR5MpuCfg[CSL_ARM_R5F_MPU_REGIONS_MAX] =
{
    {
        /* Region 0 configuration: complete 32 bit address space = 4Gbits */
        .regionId         = 0U,
        .enable           = 1U,
        .baseAddr         = 0x0U,
        .size             = CSL_ARM_R5_MPU_REGION_SIZE_4GB,
        .subRegionEnable  = CSL_ARM_R5_MPU_SUB_REGION_ENABLE_ALL,
        .exeNeverControl  = 1U,
        .accessPermission = CSL_ARM_R5_ACC_PERM_PRIV_USR_RD_WR,
        .shareable        = 0U,
        .cacheable        = (uint32_t)FALSE,
        .cachePolicy      = CSL_ARM_R5_CACHE_POLICY_NON_CACHEABLE, /* CachePolicy is dont care if CacheAble is FALSE */
        .memAttr          = CSL_ARM_R5_MEM_ATTR_STRONGLY_ORDERED,
    },
    {
        /* Region 1 configuration: 32K bytes ATCM for exception vector execution */
        .regionId         = 1U,
        .enable           = 1U,
        .baseAddr         = CSL_MSS_TCMA_RAM_BASE,
        .size             = CSL_ARM_R5_MPU_REGION_SIZE_32KB,
        .subRegionEnable  = CSL_ARM_R5_MPU_SUB_REGION_ENABLE_ALL,
        .exeNeverControl  = 0U,
        .accessPermission = CSL_ARM_R5_ACC_PERM_PRIV_USR_RD_WR,
        .shareable        = 0U,
        .cacheable        = (uint32_t)TRUE,
        .cachePolicy      = CSL_ARM_R5_CACHE_POLICY_NON_CACHEABLE,
        .memAttr          = 0U, /* MemAttr is dont care if CacheABle is TRUE */
    },
    {
        /* Region 2 configuration: 32K bytes BTCM RAM */
        .regionId         = 2U,
        .enable           = 1U,
        .baseAddr         = CSL_MSS_TCMB_RAM_BASE,
        .size             = CSL_ARM_R5_MPU_REGION_SIZE_16KB,
        .subRegionEnable  = CSL_ARM_R5_MPU_SUB_REGION_ENABLE_ALL,
        .exeNeverControl  = 0U,
        .accessPermission = CSL_ARM_R5_ACC_PERM_PRIV_USR_RD_WR,
        .shareable        = 0U,
        .cacheable        = (uint32_t)TRUE,
        .cachePolicy      = CSL_ARM_R5_CACHE_POLICY_NON_CACHEABLE,
        .memAttr          = 0U,
    },
    {
        /* Region 3 configuration: L2 memory: read/write, no execute permission */
        .regionId         = 3U,
        .enable           = 1U,
        .baseAddr         = CSL_MSS_L2_RAM_BASE,
        .size             = CSL_ARM_R5_MPU_REGION_SIZE_1MB,
        .subRegionEnable  = CSL_ARM_R5_MPU_SUB_REGION_ENABLE_ALL,
        .exeNeverControl  = 0U,
        .accessPermission = CSL_ARM_R5_ACC_PERM_PRIV_USR_RD_WR,
        .shareable        = 0U,
        .cacheable        = (uint32_t)TRUE,
        .cachePolicy      = CSL_ARM_R5_MEM_ATTR_CACHED_WT_NO_WA,
        .memAttr          = 0U,
    },
    {
        /* Region 4 configuration: R5 register space (0x0200:0000 - 0x3FF:FFFF) */
        .regionId         = 4U,
        .enable           = 1U,
        .baseAddr         = CSL_MSS_SPIA_RAM_U_BASE,
        .size             = CSL_ARM_R5_MPU_REGION_SIZE_32MB,
        .subRegionEnable  = CSL_ARM_R5_MPU_SUB_REGION_ENABLE_ALL,
        .exeNeverControl  = 1U,
        .accessPermission = CSL_ARM_R5_ACC_PERM_PRIV_USR_RD_WR,
        .shareable        = 0U,
        .cacheable        = (uint32_t)FALSE,
        .cachePolicy      = 0U,
        .memAttr          = CSL_ARM_R5_MEM_ATTR_STRONGLY_ORDERED,
    },
    {
        /* Region 5 configuration: Mailbox TX RAM (MSS -> HSM) */
        .regionId         = 5U,
        .enable           = 1U,
        .baseAddr         = CSL_HSM_MBOX_U_BASE,
        .size             = CSL_ARM_R5_MPU_REGION_SIZE_1MB,
        .subRegionEnable  = CSL_ARM_R5_MPU_SUB_REGION_ENABLE_ALL,
        .exeNeverControl  = 1U,
        .accessPermission = CSL_ARM_R5_ACC_PERM_PRIV_USR_RD_WR,
        .shareable        = 0U,
        .cacheable        = (uint32_t)FALSE,
        .cachePolicy      = 0U,
        .memAttr          = CSL_ARM_R5_MEM_ATTR_STRONGLY_ORDERED,
    },
    {
        /* Region 6 configuration: Mailbox RX RAM (HSM -> MSS) */
        .regionId         = 6U,
        .enable           = 1U,
        .baseAddr         = CSL_MSS_MBOX_U_BASE,
        .size             = CSL_ARM_R5_MPU_REGION_SIZE_1MB,
        .subRegionEnable  = CSL_ARM_R5_MPU_SUB_REGION_ENABLE_ALL,
        .exeNeverControl  = 1U,
        .accessPermission = CSL_ARM_R5_ACC_PERM_PRIV_USR_RD_WR,
        .shareable        = 0U,
        .cacheable        = (uint32_t)FALSE,
        .cachePolicy      = 0U,
        .memAttr          = CSL_ARM_R5_MEM_ATTR_STRONGLY_ORDERED,
    },
    {
        /* Region 7 configuration: External flash */
        .regionId         = 7U,
        .enable           = 1U,
        .baseAddr         = CSL_EXT_FLASH_U_BASE,
        .size             = CSL_ARM_R5_MPU_REGION_SIZE_32MB,
        .subRegionEnable  = CSL_ARM_R5_MPU_SUB_REGION_ENABLE_ALL,
        .exeNeverControl  = 1U,
        .accessPermission = CSL_ARM_R5_ACC_PERM_PRIV_USR_RD_WR,
        .shareable        = 0U,
        .cacheable        = (uint32_t)FALSE,
        .cachePolicy      = 0U,
        .memAttr          = CSL_ARM_R5_MEM_ATTR_STRONGLY_ORDERED,
    },
    {
        /* Region 8 configuration: QSPI register space */
        .regionId         = 8U,
        .enable           = 1U,
        .baseAddr         = CSL_MSS_QSPI_U_BASE,
        .size             = CSL_ARM_R5_MPU_REGION_SIZE_1MB,
        .subRegionEnable  = CSL_ARM_R5_MPU_SUB_REGION_ENABLE_ALL,
        .exeNeverControl  = 1U,
        .accessPermission = CSL_ARM_R5_ACC_PERM_PRIV_USR_RD_WR,
        .shareable        = 0U,
        .cacheable        = (uint32_t)FALSE,
        .cachePolicy      = 0U,
        .memAttr          = CSL_ARM_R5_MEM_ATTR_STRONGLY_ORDERED,
    },
    {
        /* Region 9 configuration: R5 Register space(0x0600:0000 - 0x7FF:FFFF) */
        .regionId         = 9U,
        .enable           = 1U,
        .baseAddr         = CSL_DSS_RCM_U_BASE,
        .size             = CSL_ARM_R5_MPU_REGION_SIZE_32MB,
        .subRegionEnable  = CSL_ARM_R5_MPU_SUB_REGION_ENABLE_ALL,
        .exeNeverControl  = 1U,
        .accessPermission = CSL_ARM_R5_ACC_PERM_PRIV_USR_RD_WR,
        .shareable        = 0U,
        .cacheable        = (uint32_t)FALSE,
        .cachePolicy      = 0U,
        .memAttr          = CSL_ARM_R5_MEM_ATTR_STRONGLY_ORDERED,
    },
    {
        /* Region 10 configuration: CORE R5B TCMA memory: read/write, no execute permission */
        .regionId         = 10U,
        .enable           = 1U,
        .baseAddr         = CSL_MSS_TCMA_CR5B_U_BASE,
        .size             = CSL_ARM_R5_MPU_REGION_SIZE_16KB,
        .subRegionEnable  = CSL_ARM_R5_MPU_SUB_REGION_ENABLE_ALL,
        .exeNeverControl  = 1U,
        .accessPermission = CSL_ARM_R5_ACC_PERM_PRIV_USR_RD_WR,
        .shareable        = 0U,
        .cacheable        = (uint32_t)TRUE,
        .cachePolicy      = CSL_ARM_R5_MEM_ATTR_CACHED_WT_NO_WA,
        .memAttr          = 0U,
    },
    {
        /* Region 11 configuration: CORE R5B TCMB memory: read/write, no execute permission */
        .regionId         = 11U,
        .enable           = 1U,
        .baseAddr         = CSL_MSS_TCMB_CR5B_U_BASE,
        .size             = CSL_ARM_R5_MPU_REGION_SIZE_16KB,
        .subRegionEnable  = CSL_ARM_R5_MPU_SUB_REGION_ENABLE_ALL,
        .exeNeverControl  = 1U,
        .accessPermission = CSL_ARM_R5_ACC_PERM_PRIV_USR_RD_WR,
        .shareable        = 0U,
        .cacheable        = (uint32_t)TRUE,
        .cachePolicy      = CSL_ARM_R5_MEM_ATTR_CACHED_WT_NO_WA,
        .memAttr          = 0U,
    },
    {
        /* Region 12 configuration: DSS DSP L1P memory: read/write, no execute permission */
        .regionId         = 12U,
        .enable           = 1U,
        .baseAddr         = CSL_DSS_L1P_U_BASE,
        .size             = CSL_ARM_R5_MPU_REGION_SIZE_32KB,
        .subRegionEnable  = CSL_ARM_R5_MPU_SUB_REGION_ENABLE_ALL,
        .exeNeverControl  = 1U,
        .accessPermission = CSL_ARM_R5_ACC_PERM_PRIV_USR_RD_WR,
        .shareable        = 0U,
        .cacheable        = (uint32_t)TRUE,
        .cachePolicy      = CSL_ARM_R5_MEM_ATTR_CACHED_WT_NO_WA,
        .memAttr          = 0U,
    },
    {
        /* Region 13 configuration: DSS DSP L1D memory: read/write, no execute permission */
        .regionId         = 13U,
        .enable           = 1U,
        .baseAddr         = CSL_DSS_L1D_U_BASE,
        .size             = CSL_ARM_R5_MPU_REGION_SIZE_32KB,
        .subRegionEnable  = CSL_ARM_R5_MPU_SUB_REGION_ENABLE_ALL,
        .exeNeverControl  = 1U,
        .accessPermission = CSL_ARM_R5_ACC_PERM_PRIV_USR_RD_WR,
        .shareable        = 0U,
        .cacheable        = (uint32_t)TRUE,
        .cachePolicy      = CSL_ARM_R5_MEM_ATTR_CACHED_WT_NO_WA,
        .memAttr          = 0U,
    },
    {
        /* Region 14 configuration: DSS DSP L2 memory: read/write, no execute permission */
        .regionId         = 14U,
        .enable           = 1U,
        .baseAddr         = CSL_DSS_L2_U_BASE,
        .size             = CSL_ARM_R5_MPU_REGION_SIZE_512KB,
        .subRegionEnable  = CSL_ARM_R5_MPU_SUB_REGION_ENABLE_ALL,
        .exeNeverControl  = 1U,
        .accessPermission = CSL_ARM_R5_ACC_PERM_PRIV_USR_RD_WR,
        .shareable        = 0U,
        .cacheable        = (uint32_t)TRUE,
        .cachePolicy      = CSL_ARM_R5_MEM_ATTR_CACHED_WT_NO_WA,
        .memAttr          = 0U,
    },
    {
        /* Region 15 configuration: DSS L3 memory: read/write, no execute permission */
        .regionId         = 15U,
        .enable           = 1U,
        .baseAddr         = CSL_DSS_L3_U_BASE,
        .size             = CSL_ARM_R5_MPU_REGION_SIZE_4MB,
        .subRegionEnable  = CSL_ARM_R5_MPU_SUB_REGION_ENABLE_ALL,
        .exeNeverControl  = 1U,
        .accessPermission = CSL_ARM_R5_ACC_PERM_PRIV_USR_RD_WR,
        .shareable        = 0U,
        .cacheable        = (uint32_t)TRUE,
        .cachePolicy      = CSL_ARM_R5_MEM_ATTR_CACHED_WT_NO_WA,
        .memAttr          = 0U,
    },
};

int main()
{
    Board_STATUS boardStatus;
    Board_initCfg boardCfg;
    cpu_core_id_t core_id;

    SBL_ADD_PROFILE_POINT;
    boardCfg = (BOARD_INIT_UNLOCK_MMR | BOARD_INIT_PINMUX_CONFIG);
    boardStatus = Board_init(boardCfg);
    DebugP_assert(boardStatus == BOARD_SOK);

    /* Any SoC specific Init. */
    SBL_SocEarlyInit(FALSE);

    if (SBL_LOG_LEVEL > SBL_LOG_ERR)
    {
        Rcm_Return retVal;

        #if (BOARD_UART_INSTANCE != 0)
            #error "Pinmux config assumes board instance is UART instance MSS SCIA"
        #endif
        retVal = SBL_RcmSetPeripheralClock(Rcm_PeripheralId_MSS_SCIA, Rcm_PeripheralClockSource_SYS_CLK, SBL_MSS_SCIA_FREQ_HZ);
        DebugP_assert(retVal == Rcm_Return_SUCCESS);
        /* Configure UART Tx pinmux. */
        // SBL_UARTAPinmuxConfig();
    }

    SBL_ADD_PROFILE_POINT;

    if (SBL_LOG_LEVEL > SBL_LOG_NONE)
    {
        UART_HwAttrs uart_cfg;
        UART_Params uartPrms;
        Rcm_Return retVal;

        UART_socGetInitCfg(BOARD_UART_INSTANCE, &uart_cfg);
        /* Use UART fclk freq setup by ROM */
        retVal =  SBL_RcmGetPeripheralFreq(Rcm_PeripheralId_MSS_SCIA, &uart_cfg.clockFrequency);
        DebugP_assert(retVal == Rcm_Return_SUCCESS);
        /* Disable the UART interrupt */
        uart_cfg.enableInterrupt = FALSE;
        /* Disable the DMA mode for UART */
        uart_cfg.dmaMode = FALSE;
        UART_socSetInitCfg(BOARD_UART_INSTANCE, &uart_cfg);
        /* Init UART for logging. */
        UART_init();
        UART_Params_init(&uartPrms);
        uartPrms.readDataMode = UART_DATA_BINARY;
        UART_stdioInit2(BOARD_UART_INSTANCE, &uartPrms);
    }

    SBL_ADD_PROFILE_POINT;

    SBL_log(SBL_LOG_MIN, "%s (%s - %s)\n", SBL_VERSION_STR, __DATE__, __TIME__);

    SBL_ADD_PROFILE_POINT;

#if !defined(SBL_SKIP_LATE_INIT)
    SBL_ADD_PROFILE_POINT;
    /* Any SoC specific Init. */
    SBL_SocLateInit();
#endif

#if defined(SBL_ENABLE_PLL)
    {
        Rcm_PllHsDivOutConfig hsDivCfg;
        Rcm_Return retVal;


        SBL_log(SBL_LOG_MAX, "Initlialzing PLLs ...");
        SBL_ADD_PROFILE_POINT;
        retVal = SBL_RcmSetHSDivMux(Rcm_HSDIVClkOutMuxId_DPLL_CORE_OUT2, 
                                    Rcm_HSDIVClkOutMuxClockSource_DPLL_CORE_HSDIV0_CLKOUT2_PreMux);
        DebugP_assert(retVal == Rcm_Return_SUCCESS);

        hsDivCfg.hsdivOutEnMask = (RCM_PLL_HSDIV_OUTPUT_ENABLE_1 |
                                   RCM_PLL_HSDIV_OUTPUT_ENABLE_2);
        hsDivCfg.hsDivOutFreqHz[RCM_PLL_HSDIV_OUTPUT_IDX1] = SBL_FREQ_MHZ2HZ(450U);
        hsDivCfg.hsDivOutFreqHz[RCM_PLL_HSDIV_OUTPUT_IDX2] = SBL_FREQ_MHZ2HZ(450U);
        SBL_RcmDspDpllConfig(RCM_PLL_FOUT_FREQID_CLK_900MHZ, &hsDivCfg);
        retVal = SBL_RcmSetHSDivMux(Rcm_HSDIVClkOutMuxId_DPLL_DSP_OUT1, 
                           Rcm_HSDIVClkOutMuxClockSource_DPLL_DSP_HSDIV0_CLKOUT1_PreMux);
        DebugP_assert(retVal == Rcm_Return_SUCCESS);
        retVal = SBL_RcmSetHSDivMux(Rcm_HSDIVClkOutMuxId_DPLL_DSP_OUT2, 
                           Rcm_HSDIVClkOutMuxClockSource_DPLL_DSP_HSDIV0_CLKOUT2_PreMux);
        DebugP_assert(retVal == Rcm_Return_SUCCESS);

        hsDivCfg.hsdivOutEnMask = RCM_PLL_HSDIV_OUTPUT_ENABLE_1;
        hsDivCfg.hsDivOutFreqHz[RCM_PLL_HSDIV_OUTPUT_IDX1] = SBL_FREQ_MHZ2HZ(200U);
        SBL_RcmPerDpllConfig(RCM_PLL_FOUT_FREQID_CLK_800MHZ, &hsDivCfg);
        retVal = SBL_RcmSetHSDivMux(Rcm_HSDIVClkOutMuxId_DPLL_PER_OUT1, 
                           Rcm_HSDIVClkOutMuxClockSource_DPLL_PER_HSDIV0_CLKOUT1_PreMux);
        DebugP_assert(retVal == Rcm_Return_SUCCESS);
        SBL_log(SBL_LOG_MAX, "done.\n");
    }
#endif

#if defined(SBL_ENABLE_CLOCKS)
    {
        Rcm_Return retVal;
        Rcm_EfuseBootFreqConfig bootFreqEfuseCfg;

        SBL_log(SBL_LOG_MAX, "InitlialzingClocks ...");
        SBL_ADD_PROFILE_POINT;
        SBL_RcmGetEfuseBootFrequency(&bootFreqEfuseCfg);
        SBL_RcmSetCR5SysClock(bootFreqEfuseCfg.r5FreqHz, bootFreqEfuseCfg.sysClkFreqHz);

        retVal = SBL_RcmSetRssClkFreq (Rcm_RssClkSrcId_SYS_CLK, bootFreqEfuseCfg.sysClkFreqHz);
        DebugP_assert(retVal == Rcm_Return_SUCCESS);
        SBL_moduleClockInit(Rcm_ModuleClkInitStage_PRE_APLL_SWITCH);
        SBL_log(SBL_LOG_MAX, "done.\n");
    }
#endif

    SBL_log(SBL_LOG_MAX, "Begin parsing user application\n");

    /* Image Copy */
    SBL_ImageCopy(&tpr12_evmEntry);

    /* Export SBL logs */
    sblProfileLogAddr = sblProfileLog;
    sblProfileLogIndxAddr = &sblProfileLogIndx;
    sblProfileLogOvrFlwAddr = &sblProfileLogOvrFlw;
    /* First Boot BSS R4 so that APLL switch happens and we reconfigure module clocks */
#if defined(SBL_ENABLE_CLOCKS)
    if (tpr12_evmEntry.CpuEntryPoint[RSS1_R4_ID] != SBL_INVALID_ENTRY_ADDR)
    {
        Rcm_PllHsDivOutConfig hsDivCfg;
        Rcm_Return retVal;
        Rcm_DeviceFreqConfig deviceFreqEfuseCfg;
        

        SBL_ADD_PROFILE_POINT;
        SBL_log(SBL_LOG_MAX, "Initiating BSS Boot  ...");
        SBL_SlaveCoreBoot(RSS1_R4_ID, NULL, &tpr12_evmEntry);
        SBL_log(SBL_LOG_MAX, "Waiting for BSS Boot complete to allow APLL switch ...");
        SBL_RcmWaitBSSBootComplete();

        SBL_RcmGetDeviceFrequency(&deviceFreqEfuseCfg);
        hsDivCfg.hsdivOutEnMask = (RCM_PLL_HSDIV_OUTPUT_ENABLE_0 |
                                   RCM_PLL_HSDIV_OUTPUT_ENABLE_1 |
                                   RCM_PLL_HSDIV_OUTPUT_ENABLE_2 | 
                                   RCM_PLL_HSDIV_OUTPUT_ENABLE_3);
        /* RCM_PLL_HSDIV_OUTPUT_IDX0 to 400/300 based on e-fuse */
        hsDivCfg.hsDivOutFreqHz[RCM_PLL_HSDIV_OUTPUT_IDX0] = SBL_FREQ_MHZ2HZ(400U);
        hsDivCfg.hsDivOutFreqHz[RCM_PLL_HSDIV_OUTPUT_IDX1] = SBL_FREQ_MHZ2HZ(400U);
        hsDivCfg.hsDivOutFreqHz[RCM_PLL_HSDIV_OUTPUT_IDX2] = SBL_FREQ_MHZ2HZ(240U);
        hsDivCfg.hsDivOutFreqHz[RCM_PLL_HSDIV_OUTPUT_IDX3] = SBL_FREQ_MHZ2HZ(200U);
        SBL_RcmApllHSDivConfig(RCM_APLLID_1P2G, &hsDivCfg);

   
        retVal = SBL_RcmSetHSDivMux(Rcm_HSDIVClkOutMuxId_DPLL_CORE_OUT2, 
                           Rcm_HSDIVClkOutMuxClockSource_APLL_1p2G_HSDIV0_CLKOUT0);
        DebugP_assert(retVal == Rcm_Return_SUCCESS);
        retVal = SBL_RcmSetHSDivMux(Rcm_HSDIVClkOutMuxId_DPLL_DSP_OUT2, 
                           Rcm_HSDIVClkOutMuxClockSource_APLL_1p2G_HSDIV0_CLKOUT2);
        DebugP_assert(retVal == Rcm_Return_SUCCESS);
        retVal = SBL_RcmSetHSDivMux(Rcm_HSDIVClkOutMuxId_DPLL_PER_OUT1, 
                           Rcm_HSDIVClkOutMuxClockSource_APLL_1p2G_HSDIV0_CLKOUT3);
        DebugP_assert(retVal == Rcm_Return_SUCCESS);


        hsDivCfg.hsdivOutEnMask = (RCM_PLL_HSDIV_OUTPUT_ENABLE_0 |
                                   RCM_PLL_HSDIV_OUTPUT_ENABLE_1 |
                                   RCM_PLL_HSDIV_OUTPUT_ENABLE_2);
        hsDivCfg.hsDivOutFreqHz[RCM_PLL_HSDIV_OUTPUT_IDX0] = SBL_FREQ_MHZ2HZ(360U);
        /* RCM_PLL_HSDIV_OUTPUT_IDX1 to 450/360 based on e-fuse */
        hsDivCfg.hsDivOutFreqHz[RCM_PLL_HSDIV_OUTPUT_IDX1] = SBL_FREQ_MHZ2HZ(450U);
        hsDivCfg.hsDivOutFreqHz[RCM_PLL_HSDIV_OUTPUT_IDX2] = SBL_FREQ_MHZ2HZ(200U);
        SBL_RcmApllHSDivConfig(RCM_APLLID_1P8G, &hsDivCfg);
        retVal = SBL_RcmSetHSDivMux(Rcm_HSDIVClkOutMuxId_DPLL_DSP_OUT1, 
                           Rcm_HSDIVClkOutMuxClockSource_APLL_1p8G_HSDIV0_CLKOUT1);
        DebugP_assert(retVal == Rcm_Return_SUCCESS);

        SBL_RcmSetCR5SysClock(SBL_MCU1_CPU0_FREQ_HZ, SBL_SYSCLK_FREQ_HZ);

        SBL_moduleClockInit(Rcm_ModuleClkInitStage_POST_APLL_SWITCH);
        /* Switch RSS Clock Src to Rcm_RssClkSrcId_DPLL_PER_HSDIV0_CLKOUT1_MUXED and output freq should be 200 Mhz*/
        retVal = SBL_RcmSetRssClkFreq(Rcm_RssClkSrcId_DPLL_PER_HSDIV0_CLKOUT1_MUXED, SBL_FREQ_MHZ2HZ(200U));
        DebugP_assert(retVal == Rcm_Return_SUCCESS);

        /*Configure CBUFF/Aurora*/
        /*Enable the clock to the HSI Clock mux :*/
        SBL_RcmCoreDpllHSDivOutEnable(RCM_PLL_HSDIV_OUTPUT_IDX0, 0x2);
        /* Switch HSI clock source to Rcm_HSIClockSource_APLL_CLK_HSI */
        retVal = SBL_RcmSetHSIClock (Rcm_HSIClockSource_APLL_CLK_HSI,
                                     SBL_FREQ_MHZ2HZ(400U));
        SBL_rcmConfigEthMacIf();
        /* Disable DPLL */
        SBL_RcmCoreDpllDisable();
        SBL_RcmDspDpllDisable();
        SBL_RcmPerDpllDisable();

        SBL_log(SBL_LOG_MAX, "done.\n");
    }
#endif

    for (core_id = SBL_FIRST_CORE_ID; core_id <= SBL_LAST_CORE_ID; core_id ++)
    {
        /* Try booting all cores other than the cluster running the SBL */
        if ((tpr12_evmEntry.CpuEntryPoint[core_id] != SBL_INVALID_ENTRY_ADDR) &&
            (core_id != MCU1_CPU0_ID) &&
            (core_id != RSS1_R4_ID))
        {
            SBL_SlaveCoreBoot(core_id, NULL, &tpr12_evmEntry);
        }
    }


    /* Boot the core running SBL in the end */
    if ((tpr12_evmEntry.CpuEntryPoint[MCU1_CPU0_ID] != SBL_INVALID_ENTRY_ADDR) ||
        (tpr12_evmEntry.CpuEntryPoint[MCU1_CPU1_ID] != SBL_INVALID_ENTRY_ADDR))
    {
        SBL_SlaveCoreBoot(MCU1_CPU0_ID, NULL, &tpr12_evmEntry);
        /* Execute a WFI */
        SBL_localR5CoreTriggerReset();
    }

    return 0;
}
