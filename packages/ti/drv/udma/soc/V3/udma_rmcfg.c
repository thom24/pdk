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

/* Channel needs to be checked against different mapped ring groups(CPSW/SAUL/ICSSG0/ICSSG1) * (TX/RX). 
 * Hence need #UDMA_NUM_MAPPED_TX_GROUP + #UDMA_NUM_MAPPED_RX_GROUP sets to compare */
#define UDMA_RM_CHECK_MAX_ENTRY         (UDMA_NUM_CORE * (UDMA_NUM_MAPPED_TX_GROUP + UDMA_NUM_MAPPED_RX_GROUP))
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
    uint32_t                maxTxCh; /* In AM64x, this represents unmapped TX channels */
    uint32_t                maxRxCh; /* In AM64x, this represents unmapped RX channels */
    uint32_t                maxUtcCh;
    uint32_t                maxMappedTxCh[UDMA_NUM_MAPPED_TX_GROUP];
    uint32_t                maxMappedRxCh[UDMA_NUM_MAPPED_RX_GROUP];
    uint32_t                maxMappedRing[UDMA_NUM_MAPPED_TX_GROUP + UDMA_NUM_MAPPED_RX_GROUP];
    uint32_t                maxTotalMappedTxCh;
    uint32_t                maxTotalMappedRxCh;
    uint32_t                maxTotalMappedRing;
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
        12U,                                        /* numBlkCopyCh */

        0U,                                         /* startTxUhcCh */
        0U,                                         /* numTxUhcCh */
        0U,                                         /* startTxHcCh */
        0U,                                         /* numTxHcCh */
        0U,                                         /* startTxCh */
        6U,                                         /* numTxCh */

        0U,                                         /* startRxUhcCh */
        0U,                                         /* numRxUhcCh */
        0U,                                         /* startRxHcCh */
        0U,                                         /* numRxHcCh */
        0U,                                         /* startRxCh */
        6U,                                         /* numRxCh */

        {0U, 0U, 0U, 0U},                           /* startMappedTxCh[] */
        {0U, 0U, 0U, 0U},                           /* numMappedTxCh[] */

        {0U, 0U, 0U, 0U},                           /* startMappedRxCh[] */
        {0U, 0U, 0U, 0U},                           /* numMappedRxCh[] */

        {0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U},           /* startMappedRing[] */
        {0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U},           /* numMappedRing[] */

        0U,                                         /* startFreeFlow */
        0U,                                         /* numFreeFlow */
        0U,                                         /* startFreeRing */
        0U,                                         /* numFreeRing */

        15U,                                        /* startGlobalEvent */
        256U,                                       /* numGlobalEvent */
        4U,                                         /* startVintr */
        18U,                                        /* numVintr */
        68U,                                        /* startIrIntr */
        18U,                                        /* numIrIntr */

        0U,                                         /* proxyThreadNum */
        0U,                                         /* startC7xCoreIntr */
        0U,                                         /* startC66xCoreIntr */
 
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
        20U,                                        /* startBlkCopyCh */
        4U,                                         /* numBlkCopyCh */

        0U,                                         /* startTxUhcCh */
        0U,                                         /* numTxUhcCh */
        0U,                                         /* startTxHcCh */
        0U,                                         /* numTxHcCh */
        14U,                                        /* startTxCh */
        4U,                                         /* numTxCh */

        0U,                                         /* startRxUhcCh */
        0U,                                         /* numRxUhcCh */
        0U,                                         /* startRxHcCh */
        0U,                                         /* numRxHcCh */
        14U,                                        /* startRxCh */
        4U,                                         /* numRxCh */

        {0U, 0U, 0U, 0U},                           /* startMappedTxCh[] */
        {0U, 0U, 0U, 0U},                           /* numMappedTxCh[] */

        {0U, 0U, 0U, 0U},                           /* startMappedRxCh[] */
        {0U, 0U, 0U, 0U},                           /* numMappedRxCh[] */

        {0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U},           /* startMappedRing[] */
        {0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U},           /* numMappedRing[] */

        0U,                                         /* startFreeFlow */
        0U,                                         /* numFreeFlow */
        0U,                                         /* startFreeRing */
        0U,                                         /* numFreeRing */
 
        975U,                                       /* startGlobalEvent */
        128U,                                       /* numGlobalEvent */
        100U,                                       /* startVintr */
        6U,                                         /* numVintr */
        8U,                                         /* startIrIntr */
        8U,                                         /* numIrIntr */

        0U,                                         /* proxyThreadNum */
        0U,                                         /* startC7xCoreIntr */
        0U,                                         /* startC66xCoreIntr */

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
        24U,                                        /* startBlkCopyCh */
        2U,                                         /* numBlkCopyCh */

        0U,                                         /* startTxUhcCh */
        0U,                                         /* numTxUhcCh */
        0U,                                         /* startTxHcCh */
        0U,                                         /* numTxHcCh */
        18U,                                        /* startTxCh */
        2U,                                         /* numTxCh */

        0U,                                         /* startRxUhcCh */
        0U,                                         /* numRxUhcCh */
        0U,                                         /* startRxHcCh */
        0U,                                         /* numRxHcCh */
        18U,                                        /* startRxCh */
        2U,                                         /* numRxCh */

        {0U, 0U, 0U, 0U},                           /* startMappedTxCh[] */
        {0U, 0U, 0U, 0U},                           /* numMappedTxCh[] */

        {0U, 0U, 0U, 0U},                           /* startMappedRxCh[] */
        {0U, 0U, 0U, 0U},                           /* numMappedRxCh[] */

        {0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U},           /* startMappedRing[] */
        {0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U},           /* numMappedRing[] */

        0U,                                         /* startFreeFlow */
        0U,                                         /* numFreeFlow */
        0U,                                         /* startFreeRing */
        0U,                                         /* numFreeRing */
 
        1231U,                                      /* startGlobalEvent */
        64U,                                        /* numGlobalEvent */
        114U,                                       /* startVintr */
        6U,                                         /* numVintr */
        8U,                                         /* startIrIntr */
        8U,                                         /* numIrIntr */

        0U,                                         /* proxyThreadNum */
        0U,                                         /* startC7xCoreIntr */
        0U,                                         /* startC66xCoreIntr */

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
        12U,                                        /* startBlkCopyCh */
        6U,                                         /* numBlkCopyCh */

        0U,                                         /* startTxUhcCh */
        0U,                                         /* numTxUhcCh */
        0U,                                         /* startTxHcCh */
        0U,                                         /* numTxHcCh */
        6U,                                         /* startTxCh */
        6U,                                         /* numTxCh */

        0U,                                         /* startRxUhcCh */
        0U,                                         /* numRxUhcCh */
        0U,                                         /* startRxHcCh */
        0U,                                         /* numRxHcCh */
        6U,                                         /* startRxCh */
        6U,                                         /* numRxCh */

        {0U, 0U, 0U, 0U},                           /* startMappedTxCh[] */
        {0U, 0U, 0U, 0U},                           /* numMappedTxCh[] */

        {0U, 0U, 0U, 0U},                           /* startMappedRxCh[] */
        {0U, 0U, 0U, 0U},                           /* numMappedRxCh[] */

        {0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U},           /* startMappedRing[] */
        {0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U},           /* numMappedRing[] */

        0U,                                         /* startFreeFlow */
        0U,                                         /* numFreeFlow */
        0U,                                         /* startFreeRing */
        0U,                                         /* numFreeRing */

        527U,                                       /* startGlobalEvent */
        128U,                                       /* numGlobalEvent */
        52U,                                        /* startVintr */
        6U,                                         /* numVintr */
        8U,                                         /* startIrIntr */
        8U,                                         /* numIrIntr */

        0U,                                         /* proxyThreadNum */
        0U,                                         /* startC7xCoreIntr */
        0U,                                         /* startC66xCoreIntr */

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
        18U,                                        /* startBlkCopyCh */
        2U,                                         /* numBlkCopyCh */

        0U,                                         /* startTxUhcCh */
        0U,                                         /* numTxUhcCh */
        0U,                                         /* startTxHcCh */
        0U,                                         /* numTxHcCh */
        12U,                                        /* startTxCh */
        2U,                                         /* numTxCh */

        0U,                                         /* startRxUhcCh */
        0U,                                         /* numRxUhcCh */
        0U,                                         /* startRxHcCh */
        0U,                                         /* numRxHcCh */
        12U,                                        /* startRxCh */
        2U,                                         /* numRxCh */

        {0U, 0U, 0U, 0U},                           /* startMappedTxCh[] */
        {0U, 0U, 0U, 0U},                           /* numMappedTxCh[] */

        {0U, 0U, 0U, 0U},                           /* startMappedRxCh[] */
        {0U, 0U, 0U, 0U},                           /* numMappedRxCh[] */

        {0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U},           /* startMappedRing[] */
        {0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U},           /* numMappedRing[] */

        0U,                                         /* startFreeFlow */
        0U,                                         /* numFreeFlow */
        0U,                                         /* startFreeRing */
        0U,                                         /* numFreeRing */
 
        783U,                                       /* startGlobalEvent */
        64U,                                        /* numGlobalEvent */
        66U,                                        /* startVintr */
        6U,                                         /* numVintr */
        8U,                                         /* startIrIntr */
        8U,                                         /* numIrIntr */

        0U,                                         /* proxyThreadNum */
        0U,                                         /* startC7xCoreIntr */
        0U,                                         /* startC66xCoreIntr */

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
        26U,                                        /* startBlkCopyCh */
        1U,                                         /* numBlkCopyCh */

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

        {0U, 0U, 0U, 0U},                           /* startMappedTxCh[] */
        {0U, 0U, 0U, 0U},                           /* numMappedTxCh[] */

        {0U, 0U, 0U, 0U},                           /* startMappedRxCh[] */
        {0U, 0U, 0U, 0U},                           /* numMappedRxCh[] */

        {0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U},           /* startMappedRing[] */
        {0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U},           /* numMappedRing[] */

        0U,                                         /* startFreeFlow */
        0U,                                         /* numFreeFlow */
        0U,                                         /* startFreeRing */
        0U,                                         /* numFreeRing */
 
        1423U,                                      /* startGlobalEvent */
        32U,                                        /* numGlobalEvent */
        168U,                                       /* startVintr */
        8U,                                         /* numVintr */
        32U,                                        /* startIrIntr */
        8U,                                         /* numIrIntr */

        0U,                                         /* proxyThreadNum */
        0U,                                         /* startC7xCoreIntr */
        0U,                                         /* startC66xCoreIntr */

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
        4U,         /* numTxCh */

        0U,         /* startRxUhcCh */
        0U,         /* numRxUhcCh */
        0U,         /* startRxHcCh */
        0U,         /* numRxHcCh */
        0U,         /* startRxCh */
        4U,         /* numRxCh */

        {16U, 25U, 26U, 34U},  /* startMappedTxCh[] */
        {2U,  1U,  4U,  4U},   /* numMappedTxCh[] */

        {16U, 19U, 21U, 25U},  /* startMappedRxCh[] */
        {1U,  2U,  2U,  2U},   /* numMappedRxCh[] */

        {16U, 88U, 96U, 104U, 128U, 152U, 160U, 224U},   /* startMappedRing[] */
        {16U, 8U,  4U,  4U,   11U,  8U,   32U,  32U},    /* numMappedRing[] */

        0U,         /* startFreeFlow */
        0U,         /* numFreeFlow */
        0U,         /* startFreeRing */
        0U,         /* numFreeRing */

        271U,       /* startGlobalEvent */
        256U,       /* numGlobalEvent */
        22U,        /* startVintr */
        18U,        /* numVintr */
        86U,        /* startIrIntr */
        18U,        /* numIrIntr */

        0U,         /* proxyThreadNum */
        0U,         /* startC7xCoreIntr */
        0U,         /* startC66xCoreIntr */

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
        9U,         /* startTxCh */
        4U,         /* numTxCh */

        0U,         /* startRxUhcCh */
        0U,         /* numRxUhcCh */
        0U,         /* startRxHcCh */
        0U,         /* numRxHcCh */
        9U,         /* startRxCh */
        4U,         /* numRxCh */

        {21U, 0U, 0U, 0U},  /* startMappedTxCh[] */
        {1U, 0U, 0U, 0U},   /* numMappedTxCh[] */

        {0U, 0U, 0U, 0U},  /* startMappedRxCh[] */
        {0U, 0U, 0U, 0U},  /* numMappedRxCh[] */

        {56U, 0U, 0U, 0U, 141U, 0U, 0U, 0U},   /* startMappedRing[] */
        {8U,  0U, 0U, 0U, 1U,   0U, 0U, 0U},   /* numMappedRing[] */

        0U,         /* startFreeFlow */
        0U,         /* numFreeFlow */
        0U,         /* startFreeRing */
        0U,         /* numFreeRing */

        1103U,      /* startGlobalEvent */
        128U,       /* numGlobalEvent */
        92U,        /* startVintr */
        8U,         /* numVintr */
        68U,        /* startIrIntr */
        14U,        /* numIrIntr */

        0U,         /* proxyThreadNum */
        0U,         /* startC7xCoreIntr */
        0U,         /* startC66xCoreIntr */

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
        13U,        /* startTxCh */
        2U,         /* numTxCh */

        0U,         /* startRxUhcCh */
        0U,         /* numRxUhcCh */
        0U,         /* startRxHcCh */
        0U,         /* numRxHcCh */
        13U,        /* startRxCh */
        2U,         /* numRxCh */

        {22U, 0U, 0U, 0U},  /* startMappedTxCh[] */
        {1U, 0U, 0U, 0U},   /* numMappedTxCh[] */

        {0U, 0U, 0U, 0U},   /* startMappedRxCh[] */
        {0U, 0U, 0U, 0U},   /* numMappedRxCh[] */

        {64U, 0U, 0U, 0U, 142U, 0U, 0U, 0U},   /* startMappedRing[] */
        {8U,  0U, 0U, 0U, 1U,   0U, 0U, 0U},   /* numMappedRing[] */

        0U,         /* startFreeFlow */
        0U,         /* numFreeFlow */
        0U,         /* startFreeRing */
        0U,         /* numFreeRing */

        1295U,      /* startGlobalEvent */
        128U,       /* numGlobalEvent */
        106U,       /* startVintr */
        8U,         /* numVintr */
        82U,        /* startIrIntr */
        14U,        /* numIrIntr */

        0U,         /* proxyThreadNum */
        0U,         /* startC7xCoreIntr */
        0U,         /* startC66xCoreIntr */

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
        4U,         /* startTxCh */
        3U,         /* numTxCh */

        0U,         /* startRxUhcCh */
        0U,         /* numRxUhcCh */
        0U,         /* startRxHcCh */
        0U,         /* numRxHcCh */
        4U,         /* startRxCh */
        3U,         /* numRxCh */

        {18U, 0U,  30U, 38U},  /* startMappedTxCh[] */
        {2U,  0U,  4U,  4U},   /* numMappedTxCh[] */

        {0U, 0U,  23U, 27U},  /* startMappedRxCh[] */
        {0U, 0U,  2U,  2U},   /* numMappedRxCh[] */

        {32U, 0U, 100U, 108U, 139U, 0U, 192U, 256U},   /* startMappedRing[] */
        {16U, 0U, 4U,   4U,   1U,   0U, 32U,  32U},    /* numMappedRing[] */

        0U,         /* startFreeFlow */
        0U,         /* numFreeFlow */
        0U,         /* startFreeRing */
        0U,         /* numFreeRing */

        655U,       /* startGlobalEvent */
        128U,       /* numGlobalEvent */
        44U,        /* startVintr */
        8U,         /* numVintr */
        68U,        /* startIrIntr */
        14U,        /* numIrIntr */

        0U,         /* proxyThreadNum */
        0U,         /* startC7xCoreIntr */
        0U,         /* startC66xCoreIntr */

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
        7U,         /* startTxCh */
        2U,         /* numTxCh */

        0U,         /* startRxUhcCh */
        0U,         /* numRxUhcCh */
        0U,         /* startRxHcCh */
        0U,         /* numRxHcCh */
        7U,         /* startRxCh */
        2U,         /* numRxCh */

        {20U, 0U, 0U, 0U},  /* startMappedTxCh[] */
        {1U, 0U, 0U, 0U},   /* numMappedTxCh[] */

        {0U, 0U, 0U, 0U},  /* startMappedRxCh[] */
        {0U, 0U, 0U, 0U},  /* numMappedRxCh[] */

        {48U, 0U, 0U, 0U, 140U, 0U, 0U, 0U},   /* startMappedRing[] */
        {8U,  0U, 0U, 0U, 1U,   0U, 0U, 0U},   /* numMappedRing[] */

        0U,         /* startFreeFlow */
        0U,         /* numFreeFlow */
        0U,         /* startFreeRing */
        0U,         /* numFreeRing */

        847U,       /* startGlobalEvent */
        128U,       /* numGlobalEvent */
        58U,        /* startVintr */
        8U,         /* numVintr */
        82U,        /* startIrIntr */
        14U,        /* numIrIntr */

        0U,         /* proxyThreadNum */
        0U,         /* startC7xCoreIntr */
        0U,         /* startC66xCoreIntr */

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
        15U,        /* startTxCh */
        1U,         /* numTxCh */

        0U,         /* startRxUhcCh */
        0U,         /* numRxUhcCh */
        0U,         /* startRxHcCh */
        0U,         /* numRxHcCh */
        15U,        /* startRxCh */
        1U,         /* numRxCh */

        {23U, 0U, 0U, 0U},  /* startMappedTxCh[] */
        {1U, 0U, 0U, 0U},   /* numMappedTxCh[] */

        {0U, 0U, 0U, 0U},  /* startMappedRxCh[] */
        {0U, 0U, 0U, 0U},  /* numMappedRxCh[] */

        {72U, 0U, 0U, 0U, 143U, 0U, 0U, 0U},   /* startMappedRing[] */
        {8U,  0U, 0U, 0U, 1U,   0U, 0U, 0U},   /* numMappedRing[] */

        0U,         /* startFreeFlow */
        0U,         /* numFreeFlow */
        0U,         /* startFreeRing */
        0U,         /* numFreeRing */

        1455U,      /* startGlobalEvent */
        64U,        /* numGlobalEvent */
        176U,       /* startVintr */
        8U,         /* numVintr */
        40U,        /* startIrIntr */
        8U,         /* numIrIntr */

        0U,         /* proxyThreadNum */
        0U,         /* startC7xCoreIntr */
        0U,         /* startC66xCoreIntr */

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
    memset(prms.maxMappedTxCh, 0, sizeof(prms.maxMappedTxCh)); /* No mapped TX Channels for BCDMA */
    memset(prms.maxMappedRxCh, 0, sizeof(prms.maxMappedRxCh)); /* No mapped RX Channels for BCDMA */
    memset(prms.maxMappedRing, 0, sizeof(prms.maxMappedRing)); /* No mapped rings for BCDMA */
    prms.maxTotalMappedTxCh  = 0U;
    prms.maxTotalMappedRxCh  = 0U;
    prms.maxTotalMappedRing  = 0U;

    retVal += Udma_rmCheckInstOverlap(&prms);

    /* Check Pktdma instance */
    prms.selfCfg         = &gUdmaRmDefCfg_Pktdma[0U];
    prms.crossCfg        = &gUdmaRmDefCfg_Bcdma[0U];
    prms.maxBlkCopyCh    = 0;
    prms.maxTxCh         = CSL_DMSS_PKTDMA_TX_CHANS_UNMAPPED_CNT;
    prms.maxRxCh         = CSL_DMSS_PKTDMA_RX_CHANS_UNMAPPED_CNT;
    prms.maxUtcCh        = CSL_DMSS_PKTDMA_NUM_EXT_CHANS;
    prms.maxFreeRing     = 0;
    prms.maxFreeFlow     = 0;
    prms.maxEvents       = UDMA_DMSS0_EVENT_CNT;
    prms.maxVintr        = UDMA_DMSS0_VINTR_CNT;
    prms.maxRingMon      = CSL_DMSS_RINGACC_NUM_MONITORS;

    prms.maxMappedTxCh[UDMA_MAPPED_TX_GROUP_CPSW]    = CSL_DMSS_PKTDMA_TX_CHANS_CPSW_CNT;
    prms.maxMappedTxCh[UDMA_MAPPED_TX_GROUP_SAUL]    = CSL_DMSS_PKTDMA_TX_CHANS_SAUL0_CNT + CSL_DMSS_PKTDMA_TX_CHANS_SAUL1_CNT;
    prms.maxMappedTxCh[UDMA_MAPPED_TX_GROUP_ICSSG_0] = CSL_DMSS_PKTDMA_TX_CHANS_ICSSG0_CNT;
    prms.maxMappedTxCh[UDMA_MAPPED_TX_GROUP_ICSSG_1] = CSL_DMSS_PKTDMA_TX_CHANS_ICSSG1_CNT;

    prms.maxMappedRxCh[UDMA_MAPPED_RX_GROUP_CPSW - UDMA_NUM_MAPPED_TX_GROUP]    = CSL_DMSS_PKTDMA_RX_CHANS_CPSW_CNT;
    prms.maxMappedRxCh[UDMA_MAPPED_RX_GROUP_SAUL - UDMA_NUM_MAPPED_TX_GROUP]    = CSL_DMSS_PKTDMA_RX_CHANS_SAUL0_CNT + CSL_DMSS_PKTDMA_RX_CHANS_SAUL1_CNT +
                                                                                    CSL_DMSS_PKTDMA_RX_CHANS_SAUL2_CNT + CSL_DMSS_PKTDMA_RX_CHANS_SAUL3_CNT;
    prms.maxMappedRxCh[UDMA_MAPPED_RX_GROUP_ICSSG_0 - UDMA_NUM_MAPPED_TX_GROUP] = CSL_DMSS_PKTDMA_RX_CHANS_ICSSG0_CNT;
    prms.maxMappedRxCh[UDMA_MAPPED_RX_GROUP_ICSSG_1 - UDMA_NUM_MAPPED_TX_GROUP] = CSL_DMSS_PKTDMA_RX_CHANS_ICSSG1_CNT;

    prms.maxMappedRing[UDMA_MAPPED_TX_GROUP_CPSW]    = CSL_DMSS_PKTDMA_TX_FLOWS_CPSW_CNT;
    prms.maxMappedRing[UDMA_MAPPED_TX_GROUP_SAUL]    = CSL_DMSS_PKTDMA_TX_FLOWS_SAUL0_CNT + CSL_DMSS_PKTDMA_TX_FLOWS_SAUL1_CNT;
    prms.maxMappedRing[UDMA_MAPPED_TX_GROUP_ICSSG_0] = CSL_DMSS_PKTDMA_TX_FLOWS_ICSSG0_CNT;
    prms.maxMappedRing[UDMA_MAPPED_TX_GROUP_ICSSG_1] = CSL_DMSS_PKTDMA_TX_FLOWS_ICSSG1_CNT;
    prms.maxMappedRing[UDMA_MAPPED_RX_GROUP_CPSW]    = CSL_DMSS_PKTDMA_RX_FLOWS_CPSW_CNT;
    /* SAUL1 RX flows are shared with SAUL0 & 
     * SAUL3 RX flows are shared with SAUL2  */
    prms.maxMappedRing[UDMA_MAPPED_RX_GROUP_SAUL]    = CSL_DMSS_PKTDMA_RX_FLOWS_SAUL0_CNT + CSL_DMSS_PKTDMA_RX_FLOWS_SAUL2_CNT;
    prms.maxMappedRing[UDMA_MAPPED_RX_GROUP_ICSSG_0] = CSL_DMSS_PKTDMA_RX_FLOWS_ICSSG0_CNT;
    prms.maxMappedRing[UDMA_MAPPED_RX_GROUP_ICSSG_1] = CSL_DMSS_PKTDMA_RX_FLOWS_ICSSG1_CNT;

    prms.maxTotalMappedTxCh  = CSL_DMSS_PKTDMA_NUM_TX_CHANS;
    prms.maxTotalMappedRxCh  = CSL_DMSS_PKTDMA_NUM_RX_CHANS;
    prms.maxTotalMappedRing  = CSL_DMSS_PKTDMA_NUM_TX_FLOWS + CSL_DMSS_PKTDMA_NUM_RX_FLOWS;

    retVal += Udma_rmCheckInstOverlap(&prms);

    return (retVal);
}

static int32_t Udma_rmCheckInstOverlap(const Udma_RmInstCheckPrms *prms)
{
    int32_t     retVal = UDMA_SOK;
    uint32_t    startRes[UDMA_RM_CHECK_MAX_ENTRY];
    uint32_t    numRes[UDMA_RM_CHECK_MAX_ENTRY];
    uint32_t    startIdx, numIdx, startMappedIdx;
    uint32_t    mappedGrp, coreId;

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

    /* Check Mapped TX channels */
    startIdx = 0U; numIdx = 0U;
    /* In AM64x, mapped channels start from CPSW */
    startMappedIdx = CSL_DMSS_PKTDMA_TX_CHANS_CPSW_START;
    for(mappedGrp = 0U; mappedGrp < UDMA_NUM_MAPPED_TX_GROUP; mappedGrp++)
    {
        for(coreId = 0U; coreId < UDMA_NUM_CORE; coreId++)
        {
            startRes[startIdx] = prms->selfCfg[coreId].startMappedTxCh[mappedGrp]; 
            numRes[numIdx] = prms->selfCfg[coreId].numMappedTxCh[mappedGrp]; 
            /* Check for each mapped group, whether the chNum is bound to its range*/
            if((0 != numRes[numIdx]) &&
               ((startRes[startIdx] < startMappedIdx) || 
                (startRes[startIdx] + numRes[numIdx] > startMappedIdx + prms->maxMappedTxCh[mappedGrp])))
            {
                /* Resource going out of bound */
                retVal += UDMA_EFAIL;
            }
            startIdx++; numIdx++; 
        }
        startMappedIdx += prms->maxMappedTxCh[mappedGrp];
    }
    retVal += Udma_rmCheckOverlap(startRes, numRes, startIdx, prms->maxTotalMappedTxCh);

    /* Check Mapped RX channels */
    startIdx = 0U; numIdx = 0U;
    /* In AM64x, mapped channels start from CPSW */
    startMappedIdx = CSL_DMSS_PKTDMA_RX_CHANS_CPSW_START;
    for(mappedGrp = 0U; mappedGrp < UDMA_NUM_MAPPED_RX_GROUP; mappedGrp++)
    {
        for(coreId = 0U; coreId < UDMA_NUM_CORE; coreId++)
        {
            startRes[startIdx] = prms->selfCfg[coreId].startMappedRxCh[mappedGrp];
            numRes[numIdx] = prms->selfCfg[coreId].numMappedRxCh[mappedGrp];
            /* Check for each mapped group, whether the chNum is bound to its range*/
            if((0 != numRes[numIdx]) &&
               ((startRes[startIdx] < startMappedIdx) || 
                (startRes[startIdx] + numRes[numIdx] > startMappedIdx + prms->maxMappedRxCh[mappedGrp])))
            {
                /* Resource going out of bound */
                retVal += UDMA_EFAIL;
            }
            startIdx++; numIdx++; 
        }
        startMappedIdx += prms->maxMappedRxCh[mappedGrp];
    }
    retVal += Udma_rmCheckOverlap(startRes, numRes, startIdx, prms->maxTotalMappedRxCh);

    /* Check Mapped rings */
    startIdx = 0U; numIdx = 0U; 
    /* In AM64x, mapped rings start from CPSW */
    startMappedIdx = CSL_DMSS_PKTDMA_TX_FLOWS_CPSW_START; 
    for(mappedGrp = 0U; mappedGrp < (UDMA_NUM_MAPPED_TX_GROUP + UDMA_NUM_MAPPED_RX_GROUP); mappedGrp++)
    {
        for(coreId = 0U; coreId < UDMA_NUM_CORE; coreId++)
        {
            startRes[startIdx] = prms->selfCfg[coreId].startMappedRing[mappedGrp]; 
            numRes[numIdx] = prms->selfCfg[coreId].numMappedRing[mappedGrp]; 
            /* Check for each mapped group, whether the ringNum is bound to its range*/
            if((0 != numRes[numIdx]) &&
               ((startRes[startIdx] < startMappedIdx) || 
                (startRes[startIdx] + numRes[numIdx] > startMappedIdx + prms->maxMappedRing[mappedGrp])))
            {
                /* Resource going out of bound */
                retVal += UDMA_EFAIL;
            }
            startIdx++; numIdx++; 
        }
        startMappedIdx += prms->maxMappedRing[mappedGrp];
        /* In between mapped TX rings and RX rings, there are a set of unmapped RX rings.
         * So If the mappedGrp is the last mapped TX ring group (in AM64x - UDMA_MAPPED_TX_GROUP_ICSSG_1),
         * Then update the startMappedIdx as the start of first mapped RX ring ( in AM64x - CSL_DMSS_PKTDMA_RX_FLOWS_CPSW_START) */
        if(UDMA_MAPPED_TX_GROUP_ICSSG_1 == mappedGrp)
        {
            startMappedIdx = CSL_DMSS_PKTDMA_RX_FLOWS_CPSW_START;
        }
    }
    retVal += Udma_rmCheckOverlap(startRes, numRes, startIdx, prms->maxTotalMappedRing);

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
