/**
 *  \file   MIBSPI_soc.c
 *
 *  \brief  TPR12 device specific hardware attributes.
 *
 */

/*
 * Copyright (C) 2020 - present Texas Instruments Incorporated - http://www.ti.com/
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
#include <string.h>

#include <ti/csl/soc/tpr12/src/cslr_soc.h>
#include <ti/drv/mibspi/soc/MIBSPI_soc.h>
#include <ti/drv/mibspi/soc/MIBSPI_v0.h>
#include <ti/drv/mibspi/src/mibspi_utils.h>
#include <ti/drv/mibspi/src/mibspi_priv.h>

#define MIBSPI_SPIREV_MINOR_VAL                                        (0x0008U)
#define MIBSPI_SPIREV_CUSTOM_VAL                                       (0x0000U)
#define MIBSPI_SPIREV_MAJOR_VAL                                        (0x0003U)
#define MIBSPI_SPIREV_RTL_VAL                                          (0x0000U)
#define MIBSPI_SPIREV_FUNC_VAL                                         (0x0A05U)
#define MIBSPI_SPIREV_SCHEME_VAL                                       (0x0001U)

#if defined (BUILD_MCU)
#define MIBSPI_ENTRY_INDEX_MSS_SPIA                                 (0)
#define MIBSPI_ENTRY_INDEX_MSS_SPIB                                 (1)
#define MIBSPI_ENTRY_INDEX_RCSS_SPIA                                (2)
#define MIBSPI_ENTRY_INDEX_RCSS_SPIB                                (3)

#define MIBSPI_MSS_SPIA_INT0                               (CSL_MSS_INTR_MSS_SPIA_INT0)
#define MIBSPI_MSS_SPIA_INT1                               (CSL_MSS_INTR_MSS_SPIA_INT1)

#define MIBSPI_MSS_SPIB_INT0                               (CSL_MSS_INTR_MSS_SPIB_INT0)
#define MIBSPI_MSS_SPIB_INT1                               (CSL_MSS_INTR_MSS_SPIB_INT1)

#define MIBSPI_RCSS_SPIA_INT0                              (CSL_MSS_INTR_RCSS_SPIA_INT0)
#define MIBSPI_RCSS_SPIA_INT1                              (CSL_MSS_INTR_RCSS_SPIA_INT1)

#define MIBSPI_RCSS_SPIB_INT0                              (CSL_MSS_INTR_RCSS_SPIB_INT0)
#define MIBSPI_RCSS_SPIB_INT1                              (CSL_MSS_INTR_RCSS_SPIB_INT1)


/* TODO. THese defines in edma_soc.h for the specific SOC and not defined here */
#define EDMA_ID_MSS_TPCC_A                                 (0)
#define EDMA_ID_MSS_TPCC_B                                 (1)
#define EDMA_ID_DSS_TPCC_A                                 (2)
#define EDMA_ID_DSS_TPCC_B                                 (3)
#define EDMA_ID_DSS_TPCC_C                                 (4)
#define EDMA_ID_RCSS_TPCC_A                                (5)
#define EDMA_ID_RCSS_TPCC_B                                (6)
#define EDMA_ID_HSM_TPCC_A                                 (7)

#elif defined (BUILD_DSP_1)
#define MIBSPI_ENTRY_INDEX_RCSS_SPIA                                (0)
#define MIBSPI_ENTRY_INDEX_RCSS_SPIB                                (1)

#define MIBSPI_RCSS_SPIA_INT0                              (CSL_DSS_INTR_RCSS_SPIA_INT0)
#define MIBSPI_RCSS_SPIA_INT1                              (CSL_DSS_INTR_RCSS_SPIA_INT1)

#define MIBSPI_RCSS_SPIB_INT0                              (CSL_DSS_INTR_RCSS_SPIB_INT0)
#define MIBSPI_RCSS_SPIB_INT1                              (CSL_DSS_INTR_RCSS_SPIB_INT1)

/* TODO. THese defines in edma_soc.h for the specific SOC and not defined here */
#define EDMA_ID_DSS_TPCC_A                                 (0)
#define EDMA_ID_DSS_TPCC_B                                 (1)
#define EDMA_ID_DSS_TPCC_C                                 (2)
#define EDMA_ID_RCSS_TPCC_A                                (3)
#define EDMA_ID_RCSS_TPCC_B                                (4)

#else
    #error "Unsupported target build"
#endif

#define MIBSPI_ENTRY_INDEX_MAX                                      (MIBSPI_ENTRY_INDEX_RCSS_SPIB)
#define MIBSPI_ENTRY_INDEX_CNT                                      (MIBSPI_ENTRY_INDEX_MAX + 1)


/* SPI objects */
MibSpiDriver_Object MibspiObjects[MIBSPI_ENTRY_INDEX_CNT] = 
{
#if defined (BUILD_MCU)
    [MIBSPI_ENTRY_INDEX_MSS_SPIA] = 
    {
        .mibspiHandle = NULL,
    },
    [MIBSPI_ENTRY_INDEX_MSS_SPIB] = 
    {
        .mibspiHandle = NULL,
    },
#endif
    [MIBSPI_ENTRY_INDEX_RCSS_SPIA] =
    {
        .mibspiHandle = NULL,
    },
    [MIBSPI_ENTRY_INDEX_RCSS_SPIB] = 
    {
        .mibspiHandle = NULL,
    },
};


/**************************************************************************
 ************************** Global Variables ******************************
 **************************************************************************/

/**
 * @brief   This is TPR12 MIBSPI platform configuration for MSS Subsystem.
 */
MibSpi_HwCfg gMibspiHwCfg[MIBSPI_ENTRY_INDEX_CNT] =
{
#if defined (BUILD_MCU)
    [MIBSPI_ENTRY_INDEX_MSS_SPIA] = 
    {
        /* MSS MibSPIA Hardware configuration */
        .mibspiInstId  = MIBSPI_INST_ID_MSS_SPIA,
        .ptrSpiRegBase = (CSL_mss_spiRegs *)CSL_MSS_SPIA_U_BASE,
        .ptrMibSpiRam =  (CSL_mibspiRam  *)CSL_MSS_SPIA_RAM_U_BASE,
        .clockSrcFreq = MSS_SYS_VCLK,
        .interrupt0Num = MIBSPI_MSS_SPIA_INT0,
        .interrupt1Num = MIBSPI_MSS_SPIA_INT1,
        .edmaCCId = EDMA_ID_MSS_TPCC_A, /* MSS TPCC A */
        .mibspiRamSize       = CSL_MIBSPIRAM_MAX_ELEMENTS,
        .numTransferGroups   = MIBSPI_UTILS_ARRAYSIZE(((CSL_mss_spiRegs *)CSL_MSS_SPIA_U_BASE)->TGCTRL),
        .numCsPins           = 1,
        .numParallelModePins = 0,
        .featureBitMap       = (MIBSPI_FEATURE_SPIENA_PIN),
        .numDmaReqLines      = 3,
        .dmaReqlineCfg       = 
        /* TPR12 MibSPI-A supports 6 DMA request lines */
        {
            [0] = 
            {
                EDMA_MSS_TPCC_A_EVT_SPIA_DMA_REQ0,
                EDMA_MSS_TPCC_A_EVT_SPIA_DMA_REQ1
            },
            [1] = 
            {
                EDMA_MSS_TPCC_A_EVT_SPIA_DMA_REQ2,
                EDMA_MSS_TPCC_A_EVT_SPIA_DMA_REQ3
            },
            [2] = 
            {
                EDMA_MSS_TPCC_A_EVT_SPIA_DMA_REQ4,
                EDMA_MSS_TPCC_A_EVT_SPIA_DMA_REQ5
            }
        },
        .versionInfo = 
        {
            .scheme = MIBSPI_SPIREV_SCHEME_VAL,
            .functionality = MIBSPI_SPIREV_FUNC_VAL,
            .rtl = MIBSPI_SPIREV_RTL_VAL,
            .major = MIBSPI_SPIREV_MAJOR_VAL,
            .custom = MIBSPI_SPIREV_CUSTOM_VAL,
            .minor = MIBSPI_SPIREV_MINOR_VAL,
        },
    },
    [MIBSPI_ENTRY_INDEX_MSS_SPIB] = 
    {
        /* MSS MIbSPIB Hardware configuration */
        .mibspiInstId = MIBSPI_INST_ID_MSS_SPIB,
        .ptrSpiRegBase = (CSL_mss_spiRegs *)CSL_MSS_SPIB_U_BASE,
        .ptrMibSpiRam  = (CSL_mibspiRam  *)CSL_MSS_SPIB_RAM_U_BASE,
        .clockSrcFreq = MSS_SYS_VCLK,
        .interrupt0Num = MIBSPI_MSS_SPIB_INT0,
        .interrupt1Num = MIBSPI_MSS_SPIB_INT1,

        .edmaCCId = EDMA_ID_MSS_TPCC_A, /* MSS TPCC A */
        .mibspiRamSize       = CSL_MIBSPIRAM_MAX_ELEMENTS,
        .numTransferGroups   = MIBSPI_UTILS_ARRAYSIZE(((CSL_mss_spiRegs *)CSL_MSS_SPIB_U_BASE)->TGCTRL),
        .numCsPins           = 3,
        .numParallelModePins = 0,
        .featureBitMap       = (MIBSPI_FEATURE_SPIENA_PIN),
        .numDmaReqLines = 3,
        .dmaReqlineCfg = 
        /* TPR12 MibSPI-B supports 6 DMA request lines */
        {
            [0] = 
            {
                EDMA_MSS_TPCC_A_EVT_SPIB_DMA_REQ0,
                EDMA_MSS_TPCC_A_EVT_SPIB_DMA_REQ1
            },
            [1] = 
            {
                EDMA_MSS_TPCC_A_EVT_SPIB_DMA_REQ2,
                EDMA_MSS_TPCC_A_EVT_SPIB_DMA_REQ3
            },
            [2] = 
            {
                EDMA_MSS_TPCC_A_EVT_SPIB_DMA_REQ4,
                EDMA_MSS_TPCC_A_EVT_SPIB_DMA_REQ5
            }
        },
        .versionInfo = 
        {
            .scheme = MIBSPI_SPIREV_SCHEME_VAL,
            .functionality = MIBSPI_SPIREV_FUNC_VAL,
            .rtl = MIBSPI_SPIREV_RTL_VAL,
            .major = MIBSPI_SPIREV_MAJOR_VAL,
            .custom = MIBSPI_SPIREV_CUSTOM_VAL,
            .minor = MIBSPI_SPIREV_MINOR_VAL,
        },

    },
#endif
    [MIBSPI_ENTRY_INDEX_RCSS_SPIA] = 
    {
        /* RCSS MIbSPIA Hardware configuration */
        .mibspiInstId  = MIBSPI_INST_ID_RCSS_SPIA,
        .ptrSpiRegBase = (CSL_mss_spiRegs *)CSL_RCSS_SPIA_U_BASE,
        .ptrMibSpiRam  = (CSL_mibspiRam  *)CSL_RCSS_SPIA_RAM_U_BASE,
        .clockSrcFreq  = MSS_SYS_VCLK,
        .interrupt0Num = MIBSPI_RCSS_SPIA_INT0,
        .interrupt1Num = MIBSPI_RCSS_SPIA_INT1,
        .edmaCCId      = EDMA_ID_RCSS_TPCC_A, /* RCSS TPCC A */
        .mibspiRamSize       = CSL_MIBSPIRAM_MAX_ELEMENTS,
        .numTransferGroups   = MIBSPI_UTILS_ARRAYSIZE(((CSL_mss_spiRegs *)CSL_RCSS_SPIA_U_BASE)->TGCTRL),
        .numCsPins           = 2,
        .numParallelModePins = 0,
        .featureBitMap       = (MIBSPI_FEATURE_SPIENA_PIN),
        .numDmaReqLines      = 3,
        .dmaReqlineCfg = 
        /* TPR12 MibSPI-B supports 6 DMA request lines */
        {
            [0] = 
            {
                EDMA_RCSS_TPCC_A_EVT_SPIA_DMA_REQ0,
                EDMA_RCSS_TPCC_A_EVT_SPIA_DMA_REQ1
            },
            [1] = 
            {
                EDMA_RCSS_TPCC_A_EVT_SPIA_DMA_REQ2,
                EDMA_RCSS_TPCC_A_EVT_SPIA_DMA_REQ3
            },
            [2] = 
            {
                EDMA_RCSS_TPCC_A_EVT_SPIA_DMA_REQ4,
                EDMA_RCSS_TPCC_A_EVT_SPIA_DMA_REQ5
            }
        },
        .versionInfo = 
        {
            .scheme = MIBSPI_SPIREV_SCHEME_VAL,
            .functionality = MIBSPI_SPIREV_FUNC_VAL,
            .rtl = MIBSPI_SPIREV_RTL_VAL,
            .major = MIBSPI_SPIREV_MAJOR_VAL,
            .custom = MIBSPI_SPIREV_CUSTOM_VAL,
            .minor = MIBSPI_SPIREV_MINOR_VAL,
        },

    },
    [MIBSPI_ENTRY_INDEX_RCSS_SPIB] = 
    {
        /* MSS MIbSPIB Hardware configuration */
        .mibspiInstId  = MIBSPI_INST_ID_RCSS_SPIB,
        .ptrSpiRegBase = (CSL_mss_spiRegs *)CSL_RCSS_SPIB_U_BASE,
        .ptrMibSpiRam  = (CSL_mibspiRam  *)CSL_RCSS_SPIB_RAM_U_BASE,
        .clockSrcFreq  = MSS_SYS_VCLK,
        .interrupt0Num = MIBSPI_RCSS_SPIB_INT0,
        .interrupt1Num = MIBSPI_RCSS_SPIB_INT1,
        .edmaCCId      = EDMA_ID_RCSS_TPCC_A, /* RCSS TPCC A */
        .mibspiRamSize       = CSL_MIBSPIRAM_MAX_ELEMENTS,
        .numTransferGroups   = MIBSPI_UTILS_ARRAYSIZE(((CSL_mss_spiRegs *)CSL_RCSS_SPIB_U_BASE)->TGCTRL),
        .numCsPins           = 2,
        .numParallelModePins = 0,
        .featureBitMap       = (MIBSPI_FEATURE_SPIENA_PIN),
        .numDmaReqLines = 3,
        .dmaReqlineCfg = 
        /* TPR12 MibSPI-B supports 6 DMA request lines */
        {
            [0] = 
            {
                EDMA_RCSS_TPCC_A_EVT_SPIB_DMA_REQ0,
                EDMA_RCSS_TPCC_A_EVT_SPIB_DMA_REQ1
            },
            [1] = 
            {
                EDMA_RCSS_TPCC_A_EVT_SPIB_DMA_REQ2,
                EDMA_RCSS_TPCC_A_EVT_SPIB_DMA_REQ3
            },
            [2] = 
            {
                EDMA_RCSS_TPCC_A_EVT_SPIB_DMA_REQ4,
                EDMA_RCSS_TPCC_A_EVT_SPIB_DMA_REQ5
            }
        },
        .versionInfo = 
        {
            .scheme = MIBSPI_SPIREV_SCHEME_VAL,
            .functionality = MIBSPI_SPIREV_FUNC_VAL,
            .rtl = MIBSPI_SPIREV_RTL_VAL,
            .major = MIBSPI_SPIREV_MAJOR_VAL,
            .custom = MIBSPI_SPIREV_CUSTOM_VAL,
            .minor = MIBSPI_SPIREV_MINOR_VAL,
        },

    },

};

/**
 * @brief   The TPR12 Platform has 4 MIBSPI. Application
 * developers can modify the configuration definition below as per
 * requirements.
 */
MIBSPI_Config MIBSPI_config[MIBSPI_ENTRY_INDEX_CNT] =
{
#if defined (BUILD_MCU)
    [MIBSPI_ENTRY_INDEX_MSS_SPIA] = 
    /* MSS MibSPIA driver config */
    {
        .object  =  &MibspiObjects[MIBSPI_ENTRY_INDEX_MSS_SPIA], /* SPI Driver Object */
        .hwAttrs =  &gMibspiHwCfg[MIBSPI_ENTRY_INDEX_MSS_SPIA]  /* SPI Hw configuration */
    },

    [MIBSPI_ENTRY_INDEX_MSS_SPIB] = 
    /* MSS MibSPIB driver config */
    {
        .object  =  &MibspiObjects[MIBSPI_ENTRY_INDEX_MSS_SPIB], /* SPI Driver Object */
        .hwAttrs =  &gMibspiHwCfg[MIBSPI_ENTRY_INDEX_MSS_SPIB]   /* SPI Hw configuration */
    },
#endif

    [MIBSPI_ENTRY_INDEX_RCSS_SPIA] = 
    /* RCSS MibSPIA driver config */
    {
        .object  =  &MibspiObjects[MIBSPI_ENTRY_INDEX_RCSS_SPIA], /* SPI Driver Object */
        .hwAttrs =  &gMibspiHwCfg[MIBSPI_ENTRY_INDEX_RCSS_SPIA]   /* SPI Hw configuration */
    },

    [MIBSPI_ENTRY_INDEX_RCSS_SPIB] = 
    /* RCSS MibSPIB driver config */
    {
        .object  =  &MibspiObjects[MIBSPI_ENTRY_INDEX_RCSS_SPIB], /* SPI Driver Object */
        .hwAttrs =  &gMibspiHwCfg[MIBSPI_ENTRY_INDEX_RCSS_SPIB]   /* SPI Hw configuration */
    },
};

static int32_t MIBSPI_socValidateCfg(const MibSpi_HwCfg *cfg)
{
    int32_t status = MIBSPI_STATUS_SUCCESS;

    
    if (cfg->numDmaReqLines > MIBSPI_UTILS_ARRAYSIZE(cfg->dmaReqlineCfg))
    {
        status = MIBSPI_STATUS_ERROR;
    }
    if (cfg->numCsPins > MIBSPI_MAX_CS)
    {
        status = MIBSPI_STATUS_ERROR;
    }
    if (cfg->numTransferGroups > MIBSPI_NUM_TRANS_GROUP)
    {
        status = MIBSPI_STATUS_ERROR;
    }
    return status;
}


/**
 * \brief  This API gets the SoC level of SPI intial configuration
 *
 * \param  idx       SPI instance index.
 * \param  cfg       Pointer to SPI SOC initial config.
 *
 * \return 0 success: -1: error
 *
 */
int32_t MIBSPI_socGetInitCfg(enum MibSpi_InstanceId id, MibSpi_HwCfg *cfg)
{
    int32_t ret = 0;
    uint32_t index;

    ret = MIBSPI_socGetInstIndex(id, &index);
    if (ret == MIBSPI_STATUS_SUCCESS)
    {
        Mibspi_assert(MIBSPI_socValidateCfg(&gMibspiHwCfg[index]) == MIBSPI_STATUS_SUCCESS);
        *cfg = gMibspiHwCfg[index];
    }
    else
    {
        ret = (int32_t)(-1);
    }
    return ret;
}

/**
 * \brief  This API sets the SoC level of SPI intial configuration
 *
 * \param  idx       SPI instance index.
 * \param  cfg       Pointer to SPI SOC initial config.
 *
 * \return           0 success: -1: error
 *
 */
int32_t MIBSPI_socSetInitCfg(enum MibSpi_InstanceId id, const MibSpi_HwCfg *cfg)
{
    int32_t ret = 0;
    uint32_t index;

    ret = MIBSPI_socGetInstIndex(id, &index);
    if (ret == MIBSPI_STATUS_SUCCESS)
    {
        Mibspi_assert(MIBSPI_socValidateCfg(cfg) == MIBSPI_STATUS_SUCCESS);
        gMibspiHwCfg[index] = *cfg;
    }
    else
    {
        ret = (int32_t)(-1);
    }
    return ret;
}

/**
 * \brief  This API gets the MIBSPI instance id  matching the MIBSPI instance name
 *
 * \param  mibspiInstName [in] Unique string identifying the MIBSPI instance
 * \param  idx[out]  MIBSPI instance index.
 *
 * \return           MIBSPI_SUCCESS success: MIBSPI_STATUS_ERROR : error
 *
 */
int32_t MIBSPI_socGetInstIndex(enum MibSpi_InstanceId mibspiInstId, uint32_t *idx)
{
    int32_t status = MIBSPI_STATUS_SUCCESS;
    uint32_t i;

    for (i = 0; i< MIBSPI_UTILS_ARRAYSIZE(gMibspiHwCfg); i++)
    {
        if (mibspiInstId == gMibspiHwCfg[i].mibspiInstId)
        {
            break;
        }
    }
    if (i < MIBSPI_UTILS_ARRAYSIZE(gMibspiHwCfg))
    {
        *idx   = i;
        status = MIBSPI_STATUS_SUCCESS;
    }
    else
    {
        *idx   = ~0U;
        status = MIBSPI_STATUS_ERROR;
    }
    return status;
}

MIBSPI_Handle MIBSPI_socGetInstHandle(enum MibSpi_InstanceId mibspiInstanceId)
{
    MIBSPI_Handle hMibspi;
    uint32_t idx;
    int32_t status;

    status = MIBSPI_socGetInstIndex(mibspiInstanceId, &idx);
    if (status == MIBSPI_STATUS_SUCCESS)
    {
        Mibspi_assert(idx < MIBSPI_UTILS_ARRAYSIZE(gMibspiHwCfg));
        hMibspi = (MIBSPI_Handle)MIBSPI_config[idx].object;
    
    }
    else
    {
        hMibspi = NULL;
    }
    return hMibspi;
}

