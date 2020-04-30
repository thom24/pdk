/*
 *  Copyright (c) Texas Instruments Incorporated 2020
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
 *  \file udma_rmcfg.c
 *
 *  \brief File containing the UDMA driver default RM configuration used to
 *  initialize the RM init parameters passed during driver init.
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <ti/drv/udma/src/udma_priv.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* TODO: Replace when CSLR is ready */
#define UDMA_DMSS0_EVENT_CNT      (4608U)
#define UDMA_DMSS0_VINTR_CNT      (184U)

/* Channel needs to be checked against Normal Capacity/HC/UHC.Hence need three sets to compare */
#define UDMA_RM_CHECK_MAX_ENTRY         (UDMA_NUM_CORE * 3U)
/* This is an assumption and should the max of all resource count across cores */
#define UDMA_RM_CHECK_MAX_WORDS         (150U)


/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/**
 *  \brief UDMA instance RM check parameters - local defined for passing
 *  multiple parameters to check function.
 */
typedef struct
{
    uint32_t                maxBlkCopyCh;
    uint32_t                maxTxCh;
    uint32_t                maxRxCh;
    uint32_t                maxUtcCh;
    uint32_t                maxFreeRing;
    uint32_t                maxFreeFlow;
    uint32_t                maxEvents;
    uint32_t                maxVintr;
    uint32_t                maxRingMon;
    const Udma_RmInitPrms  *selfCfg;
    const Udma_RmInitPrms  *crossCfg;
} Udma_RmInstCheckPrms;

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

static int32_t Udma_rmCheckInstOverlap(const Udma_RmInstCheckPrms *prms);
static int32_t Udma_rmCheckOverlap(uint32_t startRes[UDMA_RM_CHECK_MAX_ENTRY],
                                   uint32_t numRes[UDMA_RM_CHECK_MAX_ENTRY],
                                   uint32_t numEntries,
                                   uint32_t maxRes);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* This assumes all resources are equally split across cores.
 * Once RM is aligned across usecases, this will be fixed to reflect that */
/** \brief Main Navss default configuration */

/* Defining  only for MPU1_0 */
const Udma_RmInitPrms gUdmaRmDefCfg_Bcdma[UDMA_NUM_CORE] =
{
    /* MPU1_0 */
    {
        0U,                                         /* startBlkCopyUhcCh */
        0U,                                         /* numBlkCopyUhcCh */
        0U,                                         /* startBlkCopyHcCh */
        0U,                                         /* numBlkCopyHcCh */
        0U,                                         /* startBlkCopyCh */
        0U,                                         /* numBlkCopyCh */

        0U,                                         /* startTxUhcCh */
        0U,                                         /* numTxUhcCh */
        0U,                                         /* startTxHcCh */
        0U,                                         /* numTxHcCh */
        0U,                                         /* startTxCh */
        0U,                                         /* numTxCh */

        0U,                                         /* startRxUhcCh */
        0U,                                         /* numRxUhcCh */
        0U,                                         /* startRxHcCh */
        0U,                                         /* numRxHcCh */
        0U,                                         /* startRxCh */
        0U,                                         /* numRxCh */

        0U,                                         /* startFreeFlow */
        0U,                                         /* numFreeFlow */
        0U,                                         /* startFreeRing */
        0U,                                         /* numFreeRing */

        0U,                                         /* startGlobalEvent */
        0U,                                         /* numGlobalEvent */
        0U,                                         /* startVintr */
        0U,                                         /* numVintr */
        0U,                                         /* startIrIntr */
        0U,                                         /* numIrIntr */

        0U,                                         /* proxyThreadNum */
        0U,                                         /* startC7xCoreIntr */
 
        0U,                                         /* startProxy */
        0U,                                         /* numProxy */

        0U,                                         /* startRingMon */
        0U,                                         /* numRingMon */
    },
    /* MCU2_0 */
    {
        0U,                                         /* startBlkCopyUhcCh */
        0U,                                         /* numBlkCopyUhcCh */
        0U,                                         /* startBlkCopyHcCh */
        0U,                                         /* numBlkCopyHcCh */
        0U,                                         /* startBlkCopyCh */
        0U,                                         /* numBlkCopyCh */

        0U,                                         /* startTxUhcCh */
        0U,                                         /* numTxUhcCh */
        0U,                                         /* startTxHcCh */
        0U,                                         /* numTxHcCh */
        0U,                                         /* startTxCh */
        0U,                                         /* numTxCh */

        0U,                                         /* startRxUhcCh */
        0U,                                         /* numRxUhcCh */
        0U,                                         /* startRxHcCh */
        0U,                                         /* numRxHcCh */
        0U,                                         /* startRxCh */
        0U,                                         /* numRxCh */

        0U,                                         /* startFreeFlow */
        0U,                                         /* numFreeFlow */
        0U,                                         /* startFreeRing */
        0U,                                         /* numFreeRing */
 
        0U,                                         /* startGlobalEvent */
        0U,                                         /* numGlobalEvent */
        0U,                                         /* startVintr */
        0U,                                         /* numVintr */
        0U,                                         /* startIrIntr */
        0U,                                         /* numIrIntr */

        0U,                                         /* proxyThreadNum */
        0U,                                         /* startC7xCoreIntr */

        0U,                                         /* startProxy */
        0U,                                         /* numProxy */

        0U,                                         /* startRingMon */
        0U,                                         /* numRingMon */
    },
    /* MCU2_1 */
    {
        0U,                                         /* startBlkCopyUhcCh */
        0U,                                         /* numBlkCopyUhcCh */
        0U,                                         /* startBlkCopyHcCh */
        0U,                                         /* numBlkCopyHcCh */
        0U,                                         /* startBlkCopyCh */
        0U,                                         /* numBlkCopyCh */

        0U,                                         /* startTxUhcCh */
        0U,                                         /* numTxUhcCh */
        0U,                                         /* startTxHcCh */
        0U,                                         /* numTxHcCh */
        0U,                                         /* startTxCh */
        0U,                                         /* numTxCh */

        0U,                                         /* startRxUhcCh */
        0U,                                         /* numRxUhcCh */
        0U,                                         /* startRxHcCh */
        0U,                                         /* numRxHcCh */
        0U,                                         /* startRxCh */
        0U,                                         /* numRxCh */

        0U,                                         /* startFreeFlow */
        0U,                                         /* numFreeFlow */
        0U,                                         /* startFreeRing */
        0U,                                         /* numFreeRing */
 
        0U,                                         /* startGlobalEvent */
        0U,                                         /* numGlobalEvent */
        0U,                                         /* startVintr */
        0U,                                         /* numVintr */
        0U,                                         /* startIrIntr */
        0U,                                         /* numIrIntr */

        0U,                                         /* proxyThreadNum */
        0U,                                         /* startC7xCoreIntr */

        0U,                                         /* startProxy */
        0U,                                         /* numProxy */

        0U,                                         /* startRingMon */
        0U,                                         /* numRingMon */
    },
    /* MCU1_0 */
    {
        0U,                                         /* startBlkCopyUhcCh */
        0U,                                         /* numBlkCopyUhcCh */
        0U,                                         /* startBlkCopyHcCh */
        0U,                                         /* numBlkCopyHcCh */
        0U,                                         /* startBlkCopyCh */
        28U,                                        /* numBlkCopyCh */

        0U,                                         /* startTxUhcCh */
        0U,                                         /* numTxUhcCh */
        0U,                                         /* startTxHcCh */
        0U,                                         /* numTxHcCh */
        0U,                                         /* startTxCh */
        20U,                                        /* numTxCh */

        0U,                                         /* startRxUhcCh */
        0U,                                         /* numRxUhcCh */
        0U,                                         /* startRxHcCh */
        0U,                                         /* numRxHcCh */
        0U,                                         /* startRxCh */
        16U,                                        /* numRxCh */

        0U,                                         /* startFreeFlow */
        0U,                                         /* numFreeFlow */
        0U,                                         /* startFreeRing */
        0U,                                         /* numFreeRing */

        38U,                                        /* startGlobalEvent */
        974U,                                       /* numGlobalEvent */
        44U,                                        /* startVintr */
        28U,                                        /* numVintr */
        68U,                                        /* startIrIntr */
        28U,                                        /* numIrIntr */

        0U,                                         /* proxyThreadNum */
        0U,                                         /* startC7xCoreIntr */

        0U,                                         /* startProxy */
        0U,                                         /* numProxy */

        0U,                                         /* startRingMon */
        0U,                                         /* numRingMon */
    },
    /* MCU1_1 */
    {
        0U,                                         /* startBlkCopyUhcCh */
        0U,                                         /* numBlkCopyUhcCh */
        0U,                                         /* startBlkCopyHcCh */
        0U,                                         /* numBlkCopyHcCh */
        0U,                                         /* startBlkCopyCh */
        0U,                                        /* numBlkCopyCh */

        0U,                                         /* startTxUhcCh */
        0U,                                         /* numTxUhcCh */
        0U,                                         /* startTxHcCh */
        0U,                                         /* numTxHcCh */
        0U,                                         /* startTxCh */
        0U,                                         /* numTxCh */

        0U,                                         /* startRxUhcCh */
        0U,                                         /* numRxUhcCh */
        0U,                                         /* startRxHcCh */
        0U,                                         /* numRxHcCh */
        0U,                                         /* startRxCh */
        0U,                                         /* numRxCh */

        0U,                                         /* startFreeFlow */
        0U,                                         /* numFreeFlow */
        0U,                                         /* startFreeRing */
        0U,                                         /* numFreeRing */
 
        0U,                                         /* startGlobalEvent */
        0U,                                         /* numGlobalEvent */
        0U,                                         /* startVintr */
        0U,                                         /* numVintr */
        0U,                                         /* startIrIntr */
        0U,                                         /* numIrIntr */

        0U,                                         /* proxyThreadNum */
        0U,                                         /* startC7xCoreIntr */

        0U,                                         /* startProxy */
        0U,                                         /* numProxy */

        0U,                                         /* startRingMon */
        0U,                                         /* numRingMon */
    },
    /* M4F_0 */
    {
        0U,                                         /* startBlkCopyUhcCh */
        0U,                                         /* numBlkCopyUhcCh */
        0U,                                         /* startBlkCopyHcCh */
        0U,                                         /* numBlkCopyHcCh */
        0U,                                         /* startBlkCopyCh */
        0U,                                         /* numBlkCopyCh */

        0U,                                         /* startTxUhcCh */
        0U,                                         /* numTxUhcCh */
        0U,                                         /* startTxHcCh */
        0U,                                         /* numTxHcCh */
        0U,                                         /* startTxCh */
        0U,                                         /* numTxCh */

        0U,                                         /* startRxUhcCh */
        0U,                                         /* numRxUhcCh */
        0U,                                         /* startRxHcCh */
        0U,                                         /* numRxHcCh */
        0U,                                         /* startRxCh */
        0U,                                         /* numRxCh */

        0U,                                         /* startFreeFlow */
        0U,                                         /* numFreeFlow */
        0U,                                         /* startFreeRing */
        0U,                                         /* numFreeRing */
 
        0U,                                         /* startGlobalEvent */
        0U,                                         /* numGlobalEvent */
        0U,                                         /* startVintr */
        0U,                                         /* numVintr */
        0U,                                         /* startIrIntr */
        0U,                                         /* numIrIntr */

        0U,                                         /* proxyThreadNum */
        0U,                                         /* startC7xCoreIntr */

        0U,                                         /* startProxy */
        0U,                                         /* numProxy */

        0U,                                         /* startRingMon */
        0U,                                         /* numRingMon */
    }
};

/** \brief MCU Navss default configuration */
const Udma_RmInitPrms gUdmaRmDefCfg_Pktdma[UDMA_NUM_CORE] =
{
    /* MPU1_0 */
    {
        0U,         /* startBlkCopyUhcCh */
        0U,         /* numBlkCopyUhcCh */
        0U,         /* startBlkCopyHcCh */
        0U,         /* numBlkCopyHcCh */
        0U,         /* startBlkCopyCh */
        0U,         /* numBlkCopyCh */

        0U,         /* startTxUhcCh */
        0U,         /* numTxUhcCh */
        0U,         /* startTxHcCh */
        0U,         /* numTxHcCh */
        0U,         /* startTxCh */
        0U,         /* numTxCh */

        0U,         /* startRxUhcCh */
        0U,         /* numRxUhcCh */
        0U,         /* startRxHcCh */
        0U,         /* numRxHcCh */
        0U,         /* startRxCh */
        0U,         /* numRxCh */

        0U,         /* startFreeFlow */
        0U,         /* numFreeFlow */
        0U,         /* startFreeRing */
        0U,         /* numFreeRing */

        0U,         /* startGlobalEvent */
        0U,         /* numGlobalEvent */
        0U,         /* startVintr */
        0U,         /* numVintr */
        0U,         /* startIrIntr */
        0U,         /* numIrIntr */

        0U,         /* proxyThreadNum */
        0U,         /* startC7xCoreIntr */

        0U,         /* startProxy */
        0U,         /* numProxy */

        0U,         /* startRingMon */
        0U,         /* numRingMon */
    },
    /* MCU2_0 */
    {
        0U,         /* startBlkCopyUhcCh */
        0U,         /* numBlkCopyUhcCh */
        0U,         /* startBlkCopyHcCh */
        0U,         /* numBlkCopyHcCh */
        0U,         /* startBlkCopyCh */
        0U,         /* numBlkCopyCh */

        0U,         /* startTxUhcCh */
        0U,         /* numTxUhcCh */
        0U,         /* startTxHcCh */
        0U,         /* numTxHcCh */
        0U,         /* startTxCh */
        0U,         /* numTxCh */

        0U,         /* startRxUhcCh */
        0U,         /* numRxUhcCh */
        0U,         /* startRxHcCh */
        0U,         /* numRxHcCh */
        0U,         /* startRxCh */
        0U,         /* numRxCh */

        0U,         /* startFreeFlow */
        0U,         /* numFreeFlow */
        0U,         /* startFreeRing */
        0U,         /* numFreeRing */

        0U,         /* startGlobalEvent */
        0U,         /* numGlobalEvent */
        0U,         /* startVintr */
        0U,         /* numVintr */
        0U,         /* startIrIntr */
        0U,         /* numIrIntr */

        0U,         /* proxyThreadNum */
        0U,         /* startC7xCoreIntr */

        0U,         /* startProxy */
        0U,         /* numProxy */

        0U,         /* startRingMon */
        0U,         /* numRingMon */
    },
    /* MCU2_1 */
    {
        0U,         /* startBlkCopyUhcCh */
        0U,         /* numBlkCopyUhcCh */
        0U,         /* startBlkCopyHcCh */
        0U,         /* numBlkCopyHcCh */
        0U,         /* startBlkCopyCh */
        0U,         /* numBlkCopyCh */

        0U,         /* startTxUhcCh */
        0U,         /* numTxUhcCh */
        0U,         /* startTxHcCh */
        0U,         /* numTxHcCh */
        0U,         /* startTxCh */
        0U,         /* numTxCh */

        0U,         /* startRxUhcCh */
        0U,         /* numRxUhcCh */
        0U,         /* startRxHcCh */
        0U,         /* numRxHcCh */
        0U,         /* startRxCh */
        0U,         /* numRxCh */

        0U,         /* startFreeFlow */
        0U,         /* numFreeFlow */
        0U,         /* startFreeRing */
        0U,         /* numFreeRing */

        0U,         /* startGlobalEvent */
        0U,         /* numGlobalEvent */
        0U,         /* startVintr */
        0U,         /* numVintr */
        0U,         /* startIrIntr */
        0U,         /* numIrIntr */

        0U,         /* proxyThreadNum */
        0U,         /* startC7xCoreIntr */

        0U,         /* startProxy */
        0U,         /* numProxy */

        0U,         /* startRingMon */
        0U,         /* numRingMon */
    },
    /* MCU1_0 */
    {
        0U,         /* startBlkCopyUhcCh */
        0U,         /* numBlkCopyUhcCh */
        0U,         /* startBlkCopyHcCh */
        0U,         /* numBlkCopyHcCh */
        0U,         /* startBlkCopyCh */
        0U,         /* numBlkCopyCh */

        0U,         /* startTxUhcCh */
        0U,         /* numTxUhcCh */
        0U,         /* startTxHcCh */
        0U,         /* numTxHcCh */
        0U,         /* startTxCh */
        0U,         /* numTxCh */

        0U,         /* startRxUhcCh */
        0U,         /* numRxUhcCh */
        0U,         /* startRxHcCh */
        0U,         /* numRxHcCh */
        0U,         /* startRxCh */
        0U,         /* numRxCh */

        0U,         /* startFreeFlow */
        0U,         /* numFreeFlow */
        0U,         /* startFreeRing */
        0U,         /* numFreeRing */

        0U,         /* startGlobalEvent */
        0U,         /* numGlobalEvent */
        0U,         /* startVintr */
        0U,         /* numVintr */
        0U,         /* startIrIntr */
        0U,         /* numIrIntr */

        0U,         /* proxyThreadNum */
        0U,         /* startC7xCoreIntr */

        0U,         /* startProxy */
        0U,         /* numProxy */

        0U,         /* startRingMon */
        0U,         /* numRingMon */
    },
    /* MCU1_1 */
    {
        0U,         /* startBlkCopyUhcCh */
        0U,         /* numBlkCopyUhcCh */
        0U,         /* startBlkCopyHcCh */
        0U,         /* numBlkCopyHcCh */
        0U,         /* startBlkCopyCh */
        0U,         /* numBlkCopyCh */

        0U,         /* startTxUhcCh */
        0U,         /* numTxUhcCh */
        0U,         /* startTxHcCh */
        0U,         /* numTxHcCh */
        0U,         /* startTxCh */
        0U,         /* numTxCh */

        0U,         /* startRxUhcCh */
        0U,         /* numRxUhcCh */
        0U,         /* startRxHcCh */
        0U,         /* numRxHcCh */
        0U,         /* startRxCh */
        0U,         /* numRxCh */

        0U,         /* startFreeFlow */
        0U,         /* numFreeFlow */
        0U,         /* startFreeRing */
        0U,         /* numFreeRing */

        0U,         /* startGlobalEvent */
        0U,         /* numGlobalEvent */
        0U,         /* startVintr */
        0U,         /* numVintr */
        0U,         /* startIrIntr */
        0U,         /* numIrIntr */

        0U,         /* proxyThreadNum */
        0U,         /* startC7xCoreIntr */

        0U,         /* startProxy */
        0U,         /* numProxy */

        0U,         /* startRingMon */
        0U,         /* numRingMon */
    },
    /* M4F_0 */
    {
        0U,         /* startBlkCopyUhcCh */
        0U,         /* numBlkCopyUhcCh */
        0U,         /* startBlkCopyHcCh */
        0U,         /* numBlkCopyHcCh */
        0U,         /* startBlkCopyCh */
        0U,         /* numBlkCopyCh */

        0U,         /* startTxUhcCh */
        0U,         /* numTxUhcCh */
        0U,         /* startTxHcCh */
        0U,         /* numTxHcCh */
        0U,         /* startTxCh */
        0U,         /* numTxCh */

        0U,         /* startRxUhcCh */
        0U,         /* numRxUhcCh */
        0U,         /* startRxHcCh */
        0U,         /* numRxHcCh */
        0U,         /* startRxCh */
        0U,         /* numRxCh */

        0U,         /* startFreeFlow */
        0U,         /* numFreeFlow */
        0U,         /* startFreeRing */
        0U,         /* numFreeRing */

        0U,         /* startGlobalEvent */
        0U,         /* numGlobalEvent */
        0U,         /* startVintr */
        0U,         /* numVintr */
        0U,         /* startIrIntr */
        0U,         /* numIrIntr */

        0U,         /* proxyThreadNum */
        0U,         /* startC7xCoreIntr */

        0U,         /* startProxy */
        0U,         /* numProxy */

        0U,         /* startRingMon */
        0U,         /* numRingMon */
    }
};

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

const Udma_RmInitPrms *Udma_rmGetDefaultCfg(uint32_t instId)
{
    uint32_t                coreId;
    const Udma_RmInitPrms  *rmInitPrms;

    coreId = Udma_getCoreId();
    if(UDMA_INST_ID_BCDMA_0 == instId)
    {
        rmInitPrms = &gUdmaRmDefCfg_Bcdma[coreId];
    }
    else
    {
        rmInitPrms = &gUdmaRmDefCfg_Pktdma[coreId];
    }

    return (rmInitPrms);
}

int32_t Udma_rmCheckDefaultCfg(void)
{
    int32_t                 retVal = UDMA_SOK;
    Udma_RmInstCheckPrms    prms;

    /* Check Bcdma instance */
    prms.selfCfg         = &gUdmaRmDefCfg_Bcdma[0U];
    prms.crossCfg        = &gUdmaRmDefCfg_Pktdma[0U];
    prms.maxBlkCopyCh    = CSL_DMSS_BCDMA_NUM_BC_CHANS;
    prms.maxTxCh         = CSL_DMSS_BCDMA_NUM_TX_CHANS;
    prms.maxRxCh         = CSL_DMSS_BCDMA_NUM_RX_CHANS;
    prms.maxUtcCh        = CSL_DMSS_BCDMA_NUM_EXT_CHANS;
    prms.maxFreeRing     = 0;
    prms.maxFreeFlow     = 0;
    prms.maxEvents       = UDMA_DMSS0_EVENT_CNT;
    prms.maxVintr        = UDMA_DMSS0_VINTR_CNT;
    prms.maxRingMon      = CSL_DMSS_RINGACC_NUM_MONITORS;
    retVal += Udma_rmCheckInstOverlap(&prms);

    /* Check Pktdma instance */
    prms.selfCfg         = &gUdmaRmDefCfg_Pktdma[0U];
    prms.crossCfg        = &gUdmaRmDefCfg_Bcdma[0U];
    prms.maxBlkCopyCh    = 0;
    prms.maxTxCh         = CSL_DMSS_PKTDMA_NUM_TX_CHANS;
    prms.maxRxCh         = CSL_DMSS_PKTDMA_NUM_RX_CHANS;
    prms.maxUtcCh        = CSL_DMSS_PKTDMA_NUM_EXT_CHANS;
    prms.maxFreeRing     = 0;
    prms.maxFreeFlow     = 0;
    prms.maxEvents       = UDMA_DMSS0_EVENT_CNT;
    prms.maxVintr        = UDMA_DMSS0_VINTR_CNT;
    prms.maxRingMon      = CSL_DMSS_RINGACC_NUM_MONITORS;
    retVal += Udma_rmCheckInstOverlap(&prms);

    return (retVal);
}

static int32_t Udma_rmCheckInstOverlap(const Udma_RmInstCheckPrms *prms)
{
    int32_t     retVal = UDMA_SOK;
    uint32_t    startRes[UDMA_RM_CHECK_MAX_ENTRY];
    uint32_t    numRes[UDMA_RM_CHECK_MAX_ENTRY];
    uint32_t    startIdx, numIdx;
    uint32_t    coreId;

    /* Check BlkCpy channels */
    startIdx = 0U; numIdx = 0U;
    for(coreId = 0U; coreId < UDMA_NUM_CORE; coreId++)
    {    
        startRes[startIdx] = prms->selfCfg[coreId].startBlkCopyCh; startIdx++;
        startRes[startIdx] = prms->selfCfg[coreId].startBlkCopyHcCh; startIdx++;
        startRes[startIdx] = prms->selfCfg[coreId].startBlkCopyUhcCh; startIdx++;
        numRes[numIdx] = prms->selfCfg[coreId].numBlkCopyCh; numIdx++;
        numRes[numIdx] = prms->selfCfg[coreId].numBlkCopyHcCh; numIdx++;
        numRes[numIdx] = prms->selfCfg[coreId].numBlkCopyUhcCh; numIdx++;
    }
    retVal += Udma_rmCheckOverlap(startRes, numRes, startIdx, prms->maxBlkCopyCh);

    /* Check Tx channels */
    startIdx = 0U; numIdx = 0U;
    for(coreId = 0U; coreId < UDMA_NUM_CORE; coreId++)
    {    
        startRes[startIdx] = prms->selfCfg[coreId].startTxCh; startIdx++;
        startRes[startIdx] = prms->selfCfg[coreId].startTxHcCh; startIdx++;
        startRes[startIdx] = prms->selfCfg[coreId].startTxUhcCh; startIdx++;
        numRes[numIdx] = prms->selfCfg[coreId].numTxCh; numIdx++;
        numRes[numIdx] = prms->selfCfg[coreId].numTxHcCh; numIdx++;
        numRes[numIdx] = prms->selfCfg[coreId].numTxUhcCh; numIdx++;
    }
    retVal += Udma_rmCheckOverlap(startRes, numRes, startIdx, prms->maxTxCh);


    /* Check RX channels */
    startIdx = 0U; numIdx = 0U;
    for(coreId = 0U; coreId < UDMA_NUM_CORE; coreId++)
    {
        startRes[startIdx] = prms->selfCfg[coreId].startRxCh; startIdx++;
        startRes[startIdx] = prms->selfCfg[coreId].startRxHcCh; startIdx++;
        startRes[startIdx] = prms->selfCfg[coreId].startRxUhcCh; startIdx++;
        numRes[numIdx] = prms->selfCfg[coreId].numRxCh; numIdx++;
        numRes[numIdx] = prms->selfCfg[coreId].numRxHcCh; numIdx++;
        numRes[numIdx] = prms->selfCfg[coreId].numRxUhcCh; numIdx++;
    }
    retVal += Udma_rmCheckOverlap(startRes, numRes, startIdx, prms->maxRxCh);

    /* Check free ring */
    startIdx = 0U; numIdx = 0U;
    for(coreId = 0U; coreId < UDMA_NUM_CORE; coreId++)
    {
        startRes[startIdx] = prms->selfCfg[coreId].startFreeRing; startIdx++;
        numRes[numIdx] = prms->selfCfg[coreId].numFreeRing; numIdx++;
    }
    retVal += Udma_rmCheckOverlap(startRes, numRes, startIdx, prms->maxFreeRing);

    /* Check free flow */
    startIdx = 0U; numIdx = 0U;
    for(coreId = 0U; coreId < UDMA_NUM_CORE; coreId++)
    {
        startRes[startIdx] = prms->selfCfg[coreId].startFreeFlow; startIdx++;
        numRes[numIdx] = prms->selfCfg[coreId].numFreeFlow; numIdx++;
    }
    retVal += Udma_rmCheckOverlap(startRes, numRes, startIdx, prms->maxFreeFlow);

    /*
     * Note the cross usage!! This is intentional as events/interrupts
     * are allocated based on core and not based on UDMA instance
     *
     */
    /* Check events */
    startIdx = 0U; numIdx = 0U;
    for(coreId = 0U; coreId < UDMA_NUM_CORE; coreId++)
    {
        startRes[startIdx] = prms->selfCfg[coreId].startGlobalEvent; startIdx++;
        startRes[startIdx] = prms->crossCfg[coreId].startGlobalEvent; startIdx++;
        numRes[numIdx] = prms->selfCfg[coreId].numGlobalEvent; numIdx++;
        numRes[numIdx] = prms->crossCfg[coreId].numGlobalEvent; numIdx++;
    }
    retVal += Udma_rmCheckOverlap(startRes, numRes, startIdx, prms->maxEvents);

    /* Check VINTR */
    startIdx = 0U; numIdx = 0U;
    for(coreId = 0U; coreId < UDMA_NUM_CORE; coreId++)
    {
        startRes[startIdx] = prms->selfCfg[coreId].startVintr; startIdx++;
        startRes[startIdx] = prms->crossCfg[coreId].startVintr; startIdx++;
        numRes[numIdx] = prms->selfCfg[coreId].numVintr; numIdx++;
        numRes[numIdx] = prms->crossCfg[coreId].numVintr; numIdx++;
    }
    retVal += Udma_rmCheckOverlap(startRes, numRes, startIdx, prms->maxVintr);

    /* Check ring monitor */
    startIdx = 0U; numIdx = 0U;
    for(coreId = 0U; coreId < UDMA_NUM_CORE; coreId++)
    {
        startRes[startIdx] = prms->selfCfg[coreId].startRingMon; startIdx++;
        numRes[numIdx] = prms->selfCfg[coreId].numRingMon; numIdx++;
    }
    retVal += Udma_rmCheckOverlap(startRes, numRes, startIdx, prms->maxRingMon);

    return (retVal);
}

static int32_t Udma_rmCheckOverlap(uint32_t startRes[UDMA_RM_CHECK_MAX_ENTRY],
                                   uint32_t numRes[UDMA_RM_CHECK_MAX_ENTRY],
                                   uint32_t numEntries,
                                   uint32_t maxRes)
{
    uint32_t                i, j;
    uint32_t                offset, bitPos, bitMask;
    int32_t                 retVal = UDMA_SOK;
    uint32_t                mask[UDMA_RM_CHECK_MAX_WORDS];

    /* Reset */
    for(i = 0U; i < UDMA_RM_CHECK_MAX_WORDS; i++)
    {
        mask[i] = 0U;
    }
    for(j = 0U; j < numEntries; j++)
    {
        if((startRes[j]+numRes[j]) > maxRes)
        {
            /* Resource going out of bound */
            retVal = UDMA_EFAIL;
        }
        else
        {
            for(i = 0U; i < numRes[j]; i++)
            {
                offset = (i + startRes[j]) >> 5U;
                bitPos = (i + startRes[j]) - (offset << 5U);
                bitMask = (uint32_t) 1U << bitPos;
                if((offset >= UDMA_RM_CHECK_MAX_WORDS) ||
                   ((mask[offset] & bitMask) == bitMask))
                {
                    /* Resource already allocate or array out of bound */
                    retVal = UDMA_EFAIL;
                    break;
                }
                else
                {
                    mask[offset] |= bitMask;
                }
            }
        }

        /* Break on error */
        if(retVal != UDMA_SOK)
        {
            break;
        }
    }

    return (retVal);
}
