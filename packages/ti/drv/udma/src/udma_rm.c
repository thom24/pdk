/*
 *  Copyright (c) Texas Instruments Incorporated 2018
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
 *  \file udma_rm.c
 *
 *  \brief File containing the UDMA driver Resource Manager (RM)
 *  abstraction functions.
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <ti/drv/udma/src/udma_priv.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

static int32_t Udma_rmCheckResLeak(Udma_DrvHandle drvHandle,
                                   const uint32_t *allocFlag,
                                   uint32_t numRes,
                                   uint32_t arrSize);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

int32_t Udma_rmInit(Udma_DrvHandle drvHandle)
{
    int32_t             retVal = UDMA_SOK;
    uint32_t            i, offset, bitPos, bitMask;
    Udma_RmInitPrms    *rmInitPrms = &drvHandle->initPrms.rmInitPrms;
#if (UDMA_NUM_UTC_INSTANCE > 0)
    uint32_t            utcId;
#endif
#if ((UDMA_NUM_MAPPED_TX_GROUP + UDMA_NUM_MAPPED_RX_GROUP) > 0)
    uint32_t            mappedGrp;
#endif

#if defined (SOC_AM64X) || defined(SOC_AM65XX)
    /* Check if the default config has any overlap */
    /* Other SOC's queries from Sciclient Default BoardCfg - No need of Check */
    if(FALSE == drvHandle->initPrms.skipRmOverlapCheck)
    {
        retVal = Udma_rmCheckDefaultCfg();
    }
#endif

    if(UDMA_SOK == retVal)
    {
        /* Mark all resources as free */
        for(i = 0U; i < rmInitPrms->numBlkCopyCh; i++)
        {
            offset = i >> 5U;
            Udma_assert(drvHandle, offset < UDMA_RM_BLK_COPY_CH_ARR_SIZE);
            bitPos = i - (offset << 5U);
            bitMask = (uint32_t) 1U << bitPos;
            drvHandle->blkCopyChFlag[offset] |= bitMask;
        }
        for(i = 0U; i < rmInitPrms->numBlkCopyHcCh; i++)
        {
            offset = i >> 5U;
            Udma_assert(drvHandle, offset < UDMA_RM_BLK_COPY_HC_CH_ARR_SIZE);
            bitPos = i - (offset << 5U);
            bitMask = (uint32_t) 1U << bitPos;
            drvHandle->blkCopyHcChFlag[offset] |= bitMask;
        }
        for(i = 0U; i < rmInitPrms->numBlkCopyUhcCh; i++)
        {
            offset = i >> 5U;
            Udma_assert(drvHandle, offset < UDMA_RM_BLK_COPY_UHC_CH_ARR_SIZE);
            bitPos = i - (offset << 5U);
            bitMask = (uint32_t) 1U << bitPos;
            drvHandle->blkCopyUhcChFlag[offset] |= bitMask;
        }
        for(i = 0U; i < rmInitPrms->numTxCh; i++)
        {
            offset = i >> 5U;
            Udma_assert(drvHandle, offset < UDMA_RM_TX_CH_ARR_SIZE);
            bitPos = i - (offset << 5U);
            bitMask = (uint32_t) 1U << bitPos;
            drvHandle->txChFlag[offset] |= bitMask;
        }
        for(i = 0U; i < rmInitPrms->numRxCh; i++)
        {
            offset = i >> 5U;
            Udma_assert(drvHandle, offset < UDMA_RM_RX_CH_ARR_SIZE);
            bitPos = i - (offset << 5U);
            bitMask = (uint32_t) 1U << bitPos;
            drvHandle->rxChFlag[offset] |= bitMask;
        }
        for(i = 0U; i < rmInitPrms->numTxHcCh; i++)
        {
            offset = i >> 5U;
            Udma_assert(drvHandle, offset < UDMA_RM_TX_HC_CH_ARR_SIZE);
            bitPos = i - (offset << 5U);
            bitMask = (uint32_t) 1U << bitPos;
            drvHandle->txHcChFlag[offset] |= bitMask;
        }
        for(i = 0U; i < rmInitPrms->numRxHcCh; i++)
        {
            offset = i >> 5U;
            Udma_assert(drvHandle, offset < UDMA_RM_RX_HC_CH_ARR_SIZE);
            bitPos = i - (offset << 5U);
            bitMask = (uint32_t) 1U << bitPos;
            drvHandle->rxHcChFlag[offset] |= bitMask;
        }
        for(i = 0U; i < rmInitPrms->numTxUhcCh; i++)
        {
            offset = i >> 5U;
            Udma_assert(drvHandle, offset < UDMA_RM_TX_UHC_CH_ARR_SIZE);
            bitPos = i - (offset << 5U);
            bitMask = (uint32_t) 1U << bitPos;
            drvHandle->txUhcChFlag[offset] |= bitMask;
        }
        for(i = 0U; i < rmInitPrms->numRxUhcCh; i++)
        {
            offset = i >> 5U;
            Udma_assert(drvHandle, offset < UDMA_RM_RX_UHC_CH_ARR_SIZE);
            bitPos = i - (offset << 5U);
            bitMask = (uint32_t) 1U << bitPos;
            drvHandle->rxUhcChFlag[offset] |= bitMask;
        }
#if (UDMA_NUM_UTC_INSTANCE > 0)
        for(utcId = 0U; utcId < UDMA_NUM_UTC_INSTANCE; utcId++)
        {
            for(i = 0U; i < rmInitPrms->numUtcCh[utcId]; i++)
            {
                offset = i >> 5U;
                Udma_assert(drvHandle, offset < UDMA_RM_UTC_CH_ARR_SIZE);
                bitPos = i - (offset << 5U);
                bitMask = (uint32_t) 1U << bitPos;
                drvHandle->utcChFlag[utcId][offset] |= bitMask;
            }
        }
#endif
#if (UDMA_NUM_MAPPED_TX_GROUP > 0)
        for(mappedGrp = 0U; mappedGrp < UDMA_NUM_MAPPED_TX_GROUP; mappedGrp++)
        {
            for(i = 0U; i < rmInitPrms->numMappedTxCh[mappedGrp]; i++)
            {
                offset = i >> 5U;
                Udma_assert(drvHandle, offset < UDMA_RM_MAPPED_TX_CH_ARR_SIZE);
                bitPos = i - (offset << 5U);
                bitMask = (uint32_t) 1U << bitPos;
                drvHandle->mappedTxChFlag[mappedGrp][offset] |= bitMask;
            }
        }
#endif
#if (UDMA_NUM_MAPPED_RX_GROUP > 0)
        for(mappedGrp = 0U; mappedGrp < UDMA_NUM_MAPPED_RX_GROUP; mappedGrp++)
        {
            for(i = 0U; i < rmInitPrms->numMappedRxCh[mappedGrp]; i++)
            {
                offset = i >> 5U;
                Udma_assert(drvHandle, offset < UDMA_RM_MAPPED_RX_CH_ARR_SIZE);
                bitPos = i - (offset << 5U);
                bitMask = (uint32_t) 1U << bitPos;
                drvHandle->mappedRxChFlag[mappedGrp][offset] |= bitMask;
            }
        }
#endif
#if ((UDMA_NUM_MAPPED_TX_GROUP + UDMA_NUM_MAPPED_RX_GROUP) > 0)
        for(mappedGrp = 0U; mappedGrp < (UDMA_NUM_MAPPED_TX_GROUP + UDMA_NUM_MAPPED_RX_GROUP); mappedGrp++)
        {
            for(i = 0U; i < rmInitPrms->numMappedRing[mappedGrp]; i++)
            {
                offset = i >> 5U;
                Udma_assert(drvHandle, offset < UDMA_RM_MAPPED_RING_ARR_SIZE);
                bitPos = i - (offset << 5U);
                bitMask = (uint32_t) 1U << bitPos;
                drvHandle->mappedRingFlag[mappedGrp][offset] |= bitMask;
            }
        }
#endif
        for(i = 0U; i < rmInitPrms->numFreeRing; i++)
        {
            offset = i >> 5U;
            Udma_assert(drvHandle, offset < UDMA_RM_FREE_RING_ARR_SIZE);
            bitPos = i - (offset << 5U);
            bitMask = (uint32_t) 1U << bitPos;
            drvHandle->freeRingFlag[offset] |= bitMask;
        }
        for(i = 0U; i < rmInitPrms->numFreeFlow; i++)
        {
            offset = i >> 5U;
            Udma_assert(drvHandle, offset < UDMA_RM_FREE_FLOW_ARR_SIZE);
            bitPos = i - (offset << 5U);
            bitMask = (uint32_t) 1U << bitPos;
            drvHandle->freeFlowFlag[offset] |= bitMask;
        }
        for(i = 0U; i < rmInitPrms->numGlobalEvent; i++)
        {
            offset = i >> 5U;
            Udma_assert(drvHandle, offset < UDMA_RM_GLOBAL_EVENT_ARR_SIZE);
            bitPos = i - (offset << 5U);
            bitMask = (uint32_t) 1U << bitPos;
            drvHandle->globalEventFlag[offset] |= bitMask;
        }
        for(i = 0U; i < rmInitPrms->numVintr; i++)
        {
            offset = i >> 5U;
            Udma_assert(drvHandle, offset < UDMA_RM_VINTR_ARR_SIZE);
            bitPos = i - (offset << 5U);
            bitMask = (uint32_t) 1U << bitPos;
            drvHandle->vintrFlag[offset] |= bitMask;
        }
        for(i = 0U; i < rmInitPrms->numIrIntr; i++)
        {
            offset = i >> 5U;
            Udma_assert(drvHandle, offset < UDMA_RM_IR_INTR_ARR_SIZE);
            bitPos = i - (offset << 5U);
            bitMask = (uint32_t) 1U << bitPos;
            drvHandle->irIntrFlag[offset] |= bitMask;
        }
        for(i = 0U; i < rmInitPrms->numProxy; i++)
        {
            offset = i >> 5U;
            Udma_assert(drvHandle, offset < UDMA_RM_PROXY_ARR_SIZE);
            bitPos = i - (offset << 5U);
            bitMask = (uint32_t) 1U << bitPos;
            drvHandle->proxyFlag[offset] |= bitMask;
        }
        for(i = 0U; i < rmInitPrms->numRingMon; i++)
        {
            offset = i >> 5U;
            Udma_assert(drvHandle, offset < UDMA_RM_RING_MON_ARR_SIZE);
            bitPos = i - (offset << 5U);
            bitMask = (uint32_t) 1U << bitPos;
            drvHandle->ringMonFlag[offset] |= bitMask;
        }
    }

    return (retVal);
}

int32_t Udma_rmDeinit(Udma_DrvHandle drvHandle)
{
    int32_t             retVal = UDMA_SOK;
    Udma_RmInitPrms    *rmInitPrms = &drvHandle->initPrms.rmInitPrms;
#if (UDMA_NUM_UTC_INSTANCE > 0)
    uint32_t            utcId;
#endif
#if ((UDMA_NUM_MAPPED_TX_GROUP + UDMA_NUM_MAPPED_RX_GROUP) > 0)
    uint32_t            mappedGrp;
#endif

    retVal += Udma_rmCheckResLeak(
                  drvHandle,
                  &drvHandle->blkCopyChFlag[0U],
                  rmInitPrms->numBlkCopyCh,
                  UDMA_RM_BLK_COPY_CH_ARR_SIZE);
    retVal += Udma_rmCheckResLeak(
                  drvHandle,
                  &drvHandle->blkCopyHcChFlag[0U],
                  rmInitPrms->numBlkCopyHcCh,
                  UDMA_RM_BLK_COPY_HC_CH_ARR_SIZE);
    retVal += Udma_rmCheckResLeak(
                  drvHandle,
                  &drvHandle->blkCopyUhcChFlag[0U],
                  rmInitPrms->numBlkCopyUhcCh,
                  UDMA_RM_BLK_COPY_UHC_CH_ARR_SIZE);
    retVal += Udma_rmCheckResLeak(
                  drvHandle,
                  &drvHandle->txChFlag[0U],
                  rmInitPrms->numTxCh,
                  UDMA_RM_TX_CH_ARR_SIZE);
    retVal += Udma_rmCheckResLeak(
                  drvHandle,
                  &drvHandle->txHcChFlag[0U],
                  rmInitPrms->numTxHcCh,
                  UDMA_RM_TX_HC_CH_ARR_SIZE);
    retVal += Udma_rmCheckResLeak(
                  drvHandle,
                  &drvHandle->txUhcChFlag[0U],
                  rmInitPrms->numTxUhcCh,
                  UDMA_RM_TX_UHC_CH_ARR_SIZE);
    retVal += Udma_rmCheckResLeak(
                  drvHandle,
                  &drvHandle->rxChFlag[0U],
                  rmInitPrms->numRxCh,
                  UDMA_RM_RX_CH_ARR_SIZE);
    retVal += Udma_rmCheckResLeak(
                  drvHandle,
                  &drvHandle->rxHcChFlag[0U],
                  rmInitPrms->numRxHcCh,
                  UDMA_RM_RX_HC_CH_ARR_SIZE);
    retVal += Udma_rmCheckResLeak(
                  drvHandle,
                  &drvHandle->rxUhcChFlag[0U],
                  rmInitPrms->numRxUhcCh,
                  UDMA_RM_RX_UHC_CH_ARR_SIZE);
#if (UDMA_NUM_UTC_INSTANCE > 0)
    for(utcId = 0U; utcId < UDMA_NUM_UTC_INSTANCE; utcId++)
    {
        retVal += Udma_rmCheckResLeak(
                      drvHandle,
                      &drvHandle->utcChFlag[utcId][0U],
                      rmInitPrms->numUtcCh[utcId],
                      UDMA_RM_UTC_CH_ARR_SIZE);
    }
#endif
#if (UDMA_NUM_MAPPED_TX_GROUP > 0)
    for(mappedGrp = 0U; mappedGrp < UDMA_NUM_MAPPED_TX_GROUP; mappedGrp++)
    {
        retVal += Udma_rmCheckResLeak(
                      drvHandle,
                      &drvHandle->mappedTxChFlag[mappedGrp][0U],
                      rmInitPrms->numMappedTxCh[mappedGrp],
                      UDMA_RM_MAPPED_TX_CH_ARR_SIZE);
    }
#endif
#if (UDMA_NUM_MAPPED_RX_GROUP > 0)
    for(mappedGrp = 0U; mappedGrp < UDMA_NUM_MAPPED_RX_GROUP; mappedGrp++)
    {
        retVal += Udma_rmCheckResLeak(
                      drvHandle,
                      &drvHandle->mappedRxChFlag[mappedGrp][0U],
                      rmInitPrms->numMappedRxCh[mappedGrp],
                      UDMA_RM_MAPPED_RX_CH_ARR_SIZE);
    }
#endif
#if ((UDMA_NUM_MAPPED_TX_GROUP + UDMA_NUM_MAPPED_RX_GROUP) > 0)
    for(mappedGrp = 0U; mappedGrp < (UDMA_NUM_MAPPED_TX_GROUP + UDMA_NUM_MAPPED_RX_GROUP); mappedGrp++)
    {
        retVal += Udma_rmCheckResLeak(
                      drvHandle,
                      &drvHandle->mappedRingFlag[mappedGrp][0U],
                      rmInitPrms->numMappedRing[mappedGrp],
                      UDMA_RM_MAPPED_RING_ARR_SIZE);
    }
#endif
    retVal += Udma_rmCheckResLeak(
                  drvHandle,
                  &drvHandle->freeRingFlag[0U],
                  rmInitPrms->numFreeRing,
                  UDMA_RM_FREE_RING_ARR_SIZE);
    retVal += Udma_rmCheckResLeak(
                  drvHandle,
                  &drvHandle->freeFlowFlag[0U],
                  rmInitPrms->numFreeFlow,
                  UDMA_RM_FREE_FLOW_ARR_SIZE);
    retVal += Udma_rmCheckResLeak(
                  drvHandle,
                  &drvHandle->globalEventFlag[0U],
                  rmInitPrms->numGlobalEvent,
                  UDMA_RM_GLOBAL_EVENT_ARR_SIZE);
    retVal += Udma_rmCheckResLeak(
                  drvHandle,
                  &drvHandle->vintrFlag[0U],
                  rmInitPrms->numVintr,
                  UDMA_RM_VINTR_ARR_SIZE);
    retVal += Udma_rmCheckResLeak(
                  drvHandle,
                  &drvHandle->irIntrFlag[0U],
                  rmInitPrms->numIrIntr,
                  UDMA_RM_IR_INTR_ARR_SIZE);
    retVal += Udma_rmCheckResLeak(
                  drvHandle,
                  &drvHandle->proxyFlag[0U],
                  rmInitPrms->numProxy,
                  UDMA_RM_PROXY_ARR_SIZE);
    retVal += Udma_rmCheckResLeak(
                  drvHandle,
                  &drvHandle->ringMonFlag[0U],
                  rmInitPrms->numRingMon,
                  UDMA_RM_RING_MON_ARR_SIZE);

    return (retVal);
}

uint32_t Udma_rmAllocBlkCopyCh(uint32_t preferredChNum, Udma_DrvHandle drvHandle)
{
    uint32_t            i, offset, bitPos, bitMask;
    uint32_t            chNum = UDMA_DMA_CH_INVALID;
    Udma_RmInitPrms    *rmInitPrms = &drvHandle->initPrms.rmInitPrms;

    Udma_assert(drvHandle, drvHandle->initPrms.osalPrms.lockMutex != (Udma_OsalMutexLockFxn) NULL_PTR);
    drvHandle->initPrms.osalPrms.lockMutex(drvHandle->rmLock);

    if(UDMA_DMA_CH_ANY == preferredChNum)
    {
        /* Search and allocate from Blk Copy channel pool */
        for(i = 0U; i < rmInitPrms->numBlkCopyCh; i++)
        {
            offset = i >> 5U;
            Udma_assert(drvHandle, offset < UDMA_RM_BLK_COPY_CH_ARR_SIZE);
            bitPos = i - (offset << 5U);
            bitMask = (uint32_t) 1U << bitPos;
            if((drvHandle->blkCopyChFlag[offset] & bitMask) == bitMask)
            {
                drvHandle->blkCopyChFlag[offset] &= ~bitMask;
                chNum = i + rmInitPrms->startBlkCopyCh;  /* Add start offset */
                break;
            }
        }
    }
    else
    {
        /* Allocate specific Block Copy channel if free */
        /* Array bound check */
        if((preferredChNum >= rmInitPrms->startBlkCopyCh) &&
           (preferredChNum < (rmInitPrms->startBlkCopyCh + rmInitPrms->numBlkCopyCh)))
        {
            i = preferredChNum - rmInitPrms->startBlkCopyCh;
            offset = i >> 5U;
            Udma_assert(drvHandle, offset < UDMA_RM_BLK_COPY_CH_ARR_SIZE);
            bitPos = i - (offset << 5U);
            bitMask = (uint32_t) 1U << bitPos;
            if((drvHandle->blkCopyChFlag[offset] & bitMask) == bitMask)
            {
                drvHandle->blkCopyChFlag[offset] &= ~bitMask;
                chNum = preferredChNum;
            }
        }
    }

    Udma_assert(drvHandle, drvHandle->initPrms.osalPrms.unlockMutex != (Udma_OsalMutexUnlockFxn) NULL_PTR);
    drvHandle->initPrms.osalPrms.unlockMutex(drvHandle->rmLock);

    return (chNum);
}

void Udma_rmFreeBlkCopyCh(uint32_t chNum, Udma_DrvHandle drvHandle)
{
    uint32_t            i, offset, bitPos, bitMask;
    Udma_RmInitPrms    *rmInitPrms = &drvHandle->initPrms.rmInitPrms;

    Udma_assert(drvHandle, drvHandle->initPrms.osalPrms.lockMutex != (Udma_OsalMutexLockFxn) NULL_PTR);
    drvHandle->initPrms.osalPrms.lockMutex(drvHandle->rmLock);

    Udma_assert(drvHandle, chNum >= (rmInitPrms->startBlkCopyCh));
    Udma_assert(drvHandle,
        chNum < (rmInitPrms->startBlkCopyCh + rmInitPrms->numBlkCopyCh));
    i = chNum - rmInitPrms->startBlkCopyCh;
    offset = i >> 5U;
    Udma_assert(drvHandle, offset < UDMA_RM_BLK_COPY_CH_ARR_SIZE);
    bitPos = i - (offset << 5U);
    bitMask = (uint32_t) 1U << bitPos;
    Udma_assert(drvHandle,
        (drvHandle->blkCopyChFlag[offset] & bitMask) == 0U);
    drvHandle->blkCopyChFlag[offset] |= bitMask;

    Udma_assert(drvHandle, drvHandle->initPrms.osalPrms.unlockMutex != (Udma_OsalMutexUnlockFxn) NULL_PTR);
    drvHandle->initPrms.osalPrms.unlockMutex(drvHandle->rmLock);

    return;
}

uint32_t Udma_rmAllocBlkCopyHcCh(uint32_t preferredChNum, Udma_DrvHandle drvHandle)
{
    uint32_t            i, offset, bitPos, bitMask;
    uint32_t            chNum = UDMA_DMA_CH_INVALID;
    Udma_RmInitPrms    *rmInitPrms = &drvHandle->initPrms.rmInitPrms;

    Udma_assert(drvHandle, drvHandle->initPrms.osalPrms.lockMutex != (Udma_OsalMutexLockFxn) NULL_PTR);
    drvHandle->initPrms.osalPrms.lockMutex(drvHandle->rmLock);

    if(UDMA_DMA_CH_ANY == preferredChNum)
    {
        /* Search and allocate from Blk Copy high capacity channel pool */
        for(i = 0U; i < rmInitPrms->numBlkCopyHcCh; i++)
        {
            offset = i >> 5U;
            Udma_assert(drvHandle, offset < UDMA_RM_BLK_COPY_HC_CH_ARR_SIZE);
            bitPos = i - (offset << 5U);
            bitMask = (uint32_t) 1U << bitPos;
            if((drvHandle->blkCopyHcChFlag[offset] & bitMask) == bitMask)
            {
                drvHandle->blkCopyHcChFlag[offset] &= ~bitMask;
                chNum = i + rmInitPrms->startBlkCopyHcCh;  /* Add start offset */
                break;
            }
        }
    }
    else
    {
        /* Allocate specific Block Copy channel if free */
        /* Array bound check */
        if((preferredChNum >= rmInitPrms->startBlkCopyHcCh) &&
           (preferredChNum < (rmInitPrms->startBlkCopyHcCh + rmInitPrms->numBlkCopyHcCh)))
        {
            i = preferredChNum - rmInitPrms->startBlkCopyHcCh;
            offset = i >> 5U;
            Udma_assert(drvHandle, offset < UDMA_RM_BLK_COPY_HC_CH_ARR_SIZE);
            bitPos = i - (offset << 5U);
            bitMask = (uint32_t) 1U << bitPos;
            if((drvHandle->blkCopyHcChFlag[offset] & bitMask) == bitMask)
            {
                drvHandle->blkCopyHcChFlag[offset] &= ~bitMask;
                chNum = preferredChNum;
            }
        }
    }

    Udma_assert(drvHandle, drvHandle->initPrms.osalPrms.unlockMutex != (Udma_OsalMutexUnlockFxn) NULL_PTR);
    drvHandle->initPrms.osalPrms.unlockMutex(drvHandle->rmLock);

    return (chNum);
}

void Udma_rmFreeBlkCopyHcCh(uint32_t chNum, Udma_DrvHandle drvHandle)
{
    uint32_t            i, offset, bitPos, bitMask;
    Udma_RmInitPrms    *rmInitPrms = &drvHandle->initPrms.rmInitPrms;

    Udma_assert(drvHandle, drvHandle->initPrms.osalPrms.lockMutex != (Udma_OsalMutexLockFxn) NULL_PTR);
    drvHandle->initPrms.osalPrms.lockMutex(drvHandle->rmLock);

    Udma_assert(drvHandle, chNum >= (rmInitPrms->startBlkCopyHcCh));
    Udma_assert(drvHandle,
        chNum < (rmInitPrms->startBlkCopyHcCh + rmInitPrms->numBlkCopyHcCh));
    i = chNum - rmInitPrms->startBlkCopyHcCh;
    offset = i >> 5U;
    Udma_assert(drvHandle, offset < UDMA_RM_BLK_COPY_HC_CH_ARR_SIZE);
    bitPos = i - (offset << 5U);
    bitMask = (uint32_t) 1U << bitPos;
    Udma_assert(drvHandle,
        (drvHandle->blkCopyHcChFlag[offset] & bitMask) == 0U);
    drvHandle->blkCopyHcChFlag[offset] |= bitMask;

    Udma_assert(drvHandle, drvHandle->initPrms.osalPrms.unlockMutex != (Udma_OsalMutexUnlockFxn) NULL_PTR);
    drvHandle->initPrms.osalPrms.unlockMutex(drvHandle->rmLock);

    return;
}

uint32_t Udma_rmAllocBlkCopyUhcCh(uint32_t preferredChNum, Udma_DrvHandle drvHandle)
{
    uint32_t            i, offset, bitPos, bitMask;
    uint32_t            chNum = UDMA_DMA_CH_INVALID;
    Udma_RmInitPrms    *rmInitPrms = &drvHandle->initPrms.rmInitPrms;

    Udma_assert(drvHandle, drvHandle->initPrms.osalPrms.lockMutex != (Udma_OsalMutexLockFxn) NULL_PTR);
    drvHandle->initPrms.osalPrms.lockMutex(drvHandle->rmLock);

    if(UDMA_DMA_CH_ANY == preferredChNum)
    {
        /* Search and allocate from Blk Copy ultra high capacity channel pool */
        for(i = 0U; i < rmInitPrms->numBlkCopyUhcCh; i++)
        {
            offset = i >> 5U;
            Udma_assert(drvHandle, offset < UDMA_RM_BLK_COPY_UHC_CH_ARR_SIZE);
            bitPos = i - (offset << 5U);
            bitMask = (uint32_t) 1U << bitPos;
            if((drvHandle->blkCopyUhcChFlag[offset] & bitMask) == bitMask)
            {
                drvHandle->blkCopyUhcChFlag[offset] &= ~bitMask;
                chNum = i + rmInitPrms->startBlkCopyUhcCh;  /* Add start offset */
                break;
            }
        }
    }
    else
    {
        /* Allocate specific Block Copy channel if free */
        /* Array bound check */
        if((preferredChNum >= rmInitPrms->startBlkCopyUhcCh) &&
           (preferredChNum < (rmInitPrms->startBlkCopyUhcCh + rmInitPrms->numBlkCopyUhcCh)))
        {
            i = preferredChNum - rmInitPrms->startBlkCopyUhcCh;
            offset = i >> 5U;
            Udma_assert(drvHandle, offset < UDMA_RM_BLK_COPY_UHC_CH_ARR_SIZE);
            bitPos = i - (offset << 5U);
            bitMask = (uint32_t) 1U << bitPos;
            if((drvHandle->blkCopyUhcChFlag[offset] & bitMask) == bitMask)
            {
                drvHandle->blkCopyUhcChFlag[offset] &= ~bitMask;
                chNum = preferredChNum;
            }
        }
    }

    Udma_assert(drvHandle, drvHandle->initPrms.osalPrms.unlockMutex != (Udma_OsalMutexUnlockFxn) NULL_PTR);
    drvHandle->initPrms.osalPrms.unlockMutex(drvHandle->rmLock);

    return (chNum);
}

void Udma_rmFreeBlkCopyUhcCh(uint32_t chNum, Udma_DrvHandle drvHandle)
{
    uint32_t            i, offset, bitPos, bitMask;
    Udma_RmInitPrms    *rmInitPrms = &drvHandle->initPrms.rmInitPrms;

    Udma_assert(drvHandle, drvHandle->initPrms.osalPrms.lockMutex != (Udma_OsalMutexLockFxn) NULL_PTR);
    drvHandle->initPrms.osalPrms.lockMutex(drvHandle->rmLock);

    Udma_assert(drvHandle, chNum >= (rmInitPrms->startBlkCopyUhcCh));
    Udma_assert(drvHandle,
        chNum < (rmInitPrms->startBlkCopyUhcCh + rmInitPrms->numBlkCopyUhcCh));
    i = chNum - rmInitPrms->startBlkCopyUhcCh;
    offset = i >> 5U;
    Udma_assert(drvHandle, offset < UDMA_RM_BLK_COPY_UHC_CH_ARR_SIZE);
    bitPos = i - (offset << 5U);
    bitMask = (uint32_t) 1U << bitPos;
    Udma_assert(drvHandle,
        (drvHandle->blkCopyUhcChFlag[offset] & bitMask) == 0U);
    drvHandle->blkCopyUhcChFlag[offset] |= bitMask;

    Udma_assert(drvHandle, drvHandle->initPrms.osalPrms.unlockMutex != (Udma_OsalMutexUnlockFxn) NULL_PTR);
    drvHandle->initPrms.osalPrms.unlockMutex(drvHandle->rmLock);

    return;
}

uint32_t Udma_rmAllocTxCh(uint32_t preferredChNum, Udma_DrvHandle drvHandle)
{
    uint32_t            i, offset, bitPos, bitMask;
    uint32_t            chNum = UDMA_DMA_CH_INVALID;
    Udma_RmInitPrms    *rmInitPrms = &drvHandle->initPrms.rmInitPrms;

    Udma_assert(drvHandle, drvHandle->initPrms.osalPrms.lockMutex != (Udma_OsalMutexLockFxn) NULL_PTR);
    drvHandle->initPrms.osalPrms.lockMutex(drvHandle->rmLock);

    if(UDMA_DMA_CH_ANY == preferredChNum)
    {
        /* Search and allocate from TX channel pool */
        for(i = 0U; i < rmInitPrms->numTxCh; i++)
        {
            offset = i >> 5U;
            Udma_assert(drvHandle, offset < UDMA_RM_TX_CH_ARR_SIZE);
            bitPos = i - (offset << 5U);
            bitMask = (uint32_t) 1U << bitPos;
            if((drvHandle->txChFlag[offset] & bitMask) == bitMask)
            {
                drvHandle->txChFlag[offset] &= ~bitMask;
                chNum = i + rmInitPrms->startTxCh;  /* Add start offset */
                break;
            }
        }
    }
    else
    {
        /* Allocate specific TX channel if free */
        /* Array bound check */
        if((preferredChNum >= rmInitPrms->startTxCh) &&
           (preferredChNum < (rmInitPrms->startTxCh + rmInitPrms->numTxCh)))
        {
            i = preferredChNum - rmInitPrms->startTxCh;
            offset = i >> 5U;
            Udma_assert(drvHandle, offset < UDMA_RM_TX_CH_ARR_SIZE);
            bitPos = i - (offset << 5U);
            bitMask = (uint32_t) 1U << bitPos;
            if((drvHandle->txChFlag[offset] & bitMask) == bitMask)
            {
                drvHandle->txChFlag[offset] &= ~bitMask;
                chNum = preferredChNum;
            }
        }
    }

    Udma_assert(drvHandle, drvHandle->initPrms.osalPrms.unlockMutex != (Udma_OsalMutexUnlockFxn) NULL_PTR);
    drvHandle->initPrms.osalPrms.unlockMutex(drvHandle->rmLock);

    return (chNum);
}

void Udma_rmFreeTxCh(uint32_t chNum, Udma_DrvHandle drvHandle)
{
    uint32_t            i, offset, bitPos, bitMask;
    Udma_RmInitPrms    *rmInitPrms = &drvHandle->initPrms.rmInitPrms;

    Udma_assert(drvHandle, drvHandle->initPrms.osalPrms.lockMutex != (Udma_OsalMutexLockFxn) NULL_PTR);
    drvHandle->initPrms.osalPrms.lockMutex(drvHandle->rmLock);

    Udma_assert(drvHandle, chNum >= (rmInitPrms->startTxCh));
    Udma_assert(drvHandle,
        chNum < (rmInitPrms->startTxCh + rmInitPrms->numTxCh));
    i = chNum - rmInitPrms->startTxCh;
    offset = i >> 5U;
    Udma_assert(drvHandle, offset < UDMA_RM_TX_CH_ARR_SIZE);
    bitPos = i - (offset << 5U);
    bitMask = (uint32_t) 1U << bitPos;
    Udma_assert(drvHandle,
        (drvHandle->txChFlag[offset] & bitMask) == 0U);
    drvHandle->txChFlag[offset] |= bitMask;

    Udma_assert(drvHandle, drvHandle->initPrms.osalPrms.unlockMutex != (Udma_OsalMutexUnlockFxn) NULL_PTR);
    drvHandle->initPrms.osalPrms.unlockMutex(drvHandle->rmLock);

    return;
}

uint32_t Udma_rmAllocRxCh(uint32_t preferredChNum, Udma_DrvHandle drvHandle)
{
    uint32_t            i, offset, bitPos, bitMask;
    uint32_t            chNum = UDMA_DMA_CH_INVALID;
    Udma_RmInitPrms    *rmInitPrms = &drvHandle->initPrms.rmInitPrms;

    Udma_assert(drvHandle, drvHandle->initPrms.osalPrms.lockMutex != (Udma_OsalMutexLockFxn) NULL_PTR);
    drvHandle->initPrms.osalPrms.lockMutex(drvHandle->rmLock);

    if(UDMA_DMA_CH_ANY == preferredChNum)
    {
        /* Search and allocate from RX channel pool */
        for(i = 0U; i < rmInitPrms->numRxCh; i++)
        {
            offset = i >> 5U;
            Udma_assert(drvHandle, offset < UDMA_RM_RX_CH_ARR_SIZE);
            bitPos = i - (offset << 5U);
            bitMask = (uint32_t) 1U << bitPos;
            if((drvHandle->rxChFlag[offset] & bitMask) == bitMask)
            {
                drvHandle->rxChFlag[offset] &= ~bitMask;
                chNum = i + rmInitPrms->startRxCh;  /* Add start offset */
                break;
            }
        }
    }
    else
    {
        /* Allocate specific RX channel if free */
        /* Array bound check */
        if((preferredChNum >= rmInitPrms->startRxCh) &&
           (preferredChNum < (rmInitPrms->startRxCh + rmInitPrms->numRxCh)))
        {
            i = preferredChNum - rmInitPrms->startRxCh;
            offset = i >> 5U;
            Udma_assert(drvHandle, offset < UDMA_RM_RX_CH_ARR_SIZE);
            bitPos = i - (offset << 5U);
            bitMask = (uint32_t) 1U << bitPos;
            if((drvHandle->rxChFlag[offset] & bitMask) == bitMask)
            {
                drvHandle->rxChFlag[offset] &= ~bitMask;
                chNum = preferredChNum;
            }
        }
    }

    Udma_assert(drvHandle, drvHandle->initPrms.osalPrms.unlockMutex != (Udma_OsalMutexUnlockFxn) NULL_PTR);
    drvHandle->initPrms.osalPrms.unlockMutex(drvHandle->rmLock);

    return (chNum);
}

void Udma_rmFreeRxCh(uint32_t chNum, Udma_DrvHandle drvHandle)
{
    uint32_t            i, offset, bitPos, bitMask;
    Udma_RmInitPrms    *rmInitPrms = &drvHandle->initPrms.rmInitPrms;

    Udma_assert(drvHandle, drvHandle->initPrms.osalPrms.lockMutex != (Udma_OsalMutexLockFxn) NULL_PTR);
    drvHandle->initPrms.osalPrms.lockMutex(drvHandle->rmLock);

    Udma_assert(drvHandle, chNum >= rmInitPrms->startRxCh);
    Udma_assert(drvHandle,
        chNum < (rmInitPrms->startRxCh + rmInitPrms->numRxCh));
    i = chNum - rmInitPrms->startRxCh;
    offset = i >> 5U;
    Udma_assert(drvHandle, offset < UDMA_RM_RX_CH_ARR_SIZE);
    bitPos = i - (offset << 5U);
    bitMask = (uint32_t) 1U << bitPos;
    Udma_assert(drvHandle,
        (drvHandle->rxChFlag[offset] & bitMask) == 0U);
    drvHandle->rxChFlag[offset] |= bitMask;

    Udma_assert(drvHandle, drvHandle->initPrms.osalPrms.unlockMutex != (Udma_OsalMutexUnlockFxn) NULL_PTR);
    drvHandle->initPrms.osalPrms.unlockMutex(drvHandle->rmLock);

    return;
}

uint32_t Udma_rmAllocTxHcCh(uint32_t preferredChNum, Udma_DrvHandle drvHandle)
{
    uint32_t            i, offset, bitPos, bitMask;
    uint32_t            chNum = UDMA_DMA_CH_INVALID;
    Udma_RmInitPrms    *rmInitPrms = &drvHandle->initPrms.rmInitPrms;

    Udma_assert(drvHandle, drvHandle->initPrms.osalPrms.lockMutex != (Udma_OsalMutexLockFxn) NULL_PTR);
    drvHandle->initPrms.osalPrms.lockMutex(drvHandle->rmLock);

    if(UDMA_DMA_CH_ANY == preferredChNum)
    {
        /* Search and allocate from TX HC channel pool */
        for(i = 0U; i < rmInitPrms->numTxHcCh; i++)
        {
            offset = i >> 5U;
            Udma_assert(drvHandle, offset < UDMA_RM_TX_HC_CH_ARR_SIZE);
            bitPos = i - (offset << 5U);
            bitMask = (uint32_t) 1U << bitPos;
            if((drvHandle->txHcChFlag[offset] & bitMask) == bitMask)
            {
                drvHandle->txHcChFlag[offset] &= ~bitMask;
                chNum = i + rmInitPrms->startTxHcCh;  /* Add start offset */
                break;
            }
        }
    }
    else
    {
        /* Allocate specific TX HC channel if free */
        /* Array bound check */
        if((preferredChNum >= rmInitPrms->startTxHcCh) &&
           (preferredChNum < (rmInitPrms->startTxHcCh + rmInitPrms->numTxHcCh)))
        {
            i = preferredChNum - rmInitPrms->startTxHcCh;
            offset = i >> 5U;
            Udma_assert(drvHandle, offset < UDMA_RM_TX_HC_CH_ARR_SIZE);
            bitPos = i - (offset << 5U);
            bitMask = (uint32_t) 1U << bitPos;
            if((drvHandle->txHcChFlag[offset] & bitMask) == bitMask)
            {
                drvHandle->txHcChFlag[offset] &= ~bitMask;
                chNum = preferredChNum;
            }
        }
    }

    Udma_assert(drvHandle, drvHandle->initPrms.osalPrms.unlockMutex != (Udma_OsalMutexUnlockFxn) NULL_PTR);
    drvHandle->initPrms.osalPrms.unlockMutex(drvHandle->rmLock);

    return (chNum);
}

void Udma_rmFreeTxHcCh(uint32_t chNum, Udma_DrvHandle drvHandle)
{
    uint32_t            i, offset, bitPos, bitMask;
    Udma_RmInitPrms    *rmInitPrms = &drvHandle->initPrms.rmInitPrms;

    Udma_assert(drvHandle, drvHandle->initPrms.osalPrms.lockMutex != (Udma_OsalMutexLockFxn) NULL_PTR);
    drvHandle->initPrms.osalPrms.lockMutex(drvHandle->rmLock);

    Udma_assert(drvHandle, chNum >= (rmInitPrms->startTxHcCh));
    Udma_assert(drvHandle,
        chNum < (rmInitPrms->startTxHcCh + rmInitPrms->numTxHcCh));
    i = chNum - rmInitPrms->startTxHcCh;
    offset = i >> 5U;
    Udma_assert(drvHandle, offset < UDMA_RM_TX_HC_CH_ARR_SIZE);
    bitPos = i - (offset << 5U);
    bitMask = (uint32_t) 1U << bitPos;
    Udma_assert(drvHandle,
        (drvHandle->txHcChFlag[offset] & bitMask) == 0U);
    drvHandle->txHcChFlag[offset] |= bitMask;

    Udma_assert(drvHandle, drvHandle->initPrms.osalPrms.unlockMutex != (Udma_OsalMutexUnlockFxn) NULL_PTR);
    drvHandle->initPrms.osalPrms.unlockMutex(drvHandle->rmLock);

    return;
}

uint32_t Udma_rmAllocRxHcCh(uint32_t preferredChNum, Udma_DrvHandle drvHandle)
{
    uint32_t            i, offset, bitPos, bitMask;
    uint32_t            chNum = UDMA_DMA_CH_INVALID;
    Udma_RmInitPrms    *rmInitPrms = &drvHandle->initPrms.rmInitPrms;

    Udma_assert(drvHandle, drvHandle->initPrms.osalPrms.lockMutex != (Udma_OsalMutexLockFxn) NULL_PTR);
    drvHandle->initPrms.osalPrms.lockMutex(drvHandle->rmLock);

    if(UDMA_DMA_CH_ANY == preferredChNum)
    {
        /* Search and allocate from RX HC channel pool */
        for(i = 0U; i < rmInitPrms->numRxHcCh; i++)
        {
            offset = i >> 5U;
            Udma_assert(drvHandle, offset < UDMA_RM_RX_HC_CH_ARR_SIZE);
            bitPos = i - (offset << 5U);
            bitMask = (uint32_t) 1U << bitPos;
            if((drvHandle->rxHcChFlag[offset] & bitMask) == bitMask)
            {
                drvHandle->rxHcChFlag[offset] &= ~bitMask;
                chNum = i + rmInitPrms->startRxHcCh;  /* Add start offset */
                break;
            }
        }
    }
    else
    {
        /* Allocate specific RX HC channel if free */
        /* Array bound check */
        if((preferredChNum >= rmInitPrms->startRxHcCh) &&
           (preferredChNum < (rmInitPrms->startRxHcCh + rmInitPrms->numRxHcCh)))
        {
            i = preferredChNum - rmInitPrms->startRxHcCh;
            offset = i >> 5U;
            Udma_assert(drvHandle, offset < UDMA_RM_RX_HC_CH_ARR_SIZE);
            bitPos = i - (offset << 5U);
            bitMask = (uint32_t) 1U << bitPos;
            if((drvHandle->rxHcChFlag[offset] & bitMask) == bitMask)
            {
                drvHandle->rxHcChFlag[offset] &= ~bitMask;
                chNum = preferredChNum;
            }
        }
    }

    Udma_assert(drvHandle, drvHandle->initPrms.osalPrms.unlockMutex != (Udma_OsalMutexUnlockFxn) NULL_PTR);
    drvHandle->initPrms.osalPrms.unlockMutex(drvHandle->rmLock);

    return (chNum);
}

void Udma_rmFreeRxHcCh(uint32_t chNum, Udma_DrvHandle drvHandle)
{
    uint32_t            i, offset, bitPos, bitMask;
    Udma_RmInitPrms    *rmInitPrms = &drvHandle->initPrms.rmInitPrms;

    Udma_assert(drvHandle, drvHandle->initPrms.osalPrms.lockMutex != (Udma_OsalMutexLockFxn) NULL_PTR);
    drvHandle->initPrms.osalPrms.lockMutex(drvHandle->rmLock);

    Udma_assert(drvHandle, chNum >= rmInitPrms->startRxHcCh);
    Udma_assert(drvHandle,
        chNum < (rmInitPrms->startRxHcCh + rmInitPrms->numRxHcCh));
    i = chNum - rmInitPrms->startRxHcCh;
    offset = i >> 5U;
    Udma_assert(drvHandle, offset < UDMA_RM_RX_HC_CH_ARR_SIZE);
    bitPos = i - (offset << 5U);
    bitMask = (uint32_t) 1U << bitPos;
    Udma_assert(drvHandle,
        (drvHandle->rxHcChFlag[offset] & bitMask) == 0U);
    drvHandle->rxHcChFlag[offset] |= bitMask;

    Udma_assert(drvHandle, drvHandle->initPrms.osalPrms.unlockMutex != (Udma_OsalMutexUnlockFxn) NULL_PTR);
    drvHandle->initPrms.osalPrms.unlockMutex(drvHandle->rmLock);

    return;
}

uint32_t Udma_rmAllocTxUhcCh(uint32_t preferredChNum, Udma_DrvHandle drvHandle)
{
    uint32_t            i, offset, bitPos, bitMask;
    uint32_t            chNum = UDMA_DMA_CH_INVALID;
    Udma_RmInitPrms    *rmInitPrms = &drvHandle->initPrms.rmInitPrms;

    Udma_assert(drvHandle, drvHandle->initPrms.osalPrms.lockMutex != (Udma_OsalMutexLockFxn) NULL_PTR);
    drvHandle->initPrms.osalPrms.lockMutex(drvHandle->rmLock);

    if(UDMA_DMA_CH_ANY == preferredChNum)
    {
        /* Search and allocate from TX UHC channel pool */
        for(i = 0U; i < rmInitPrms->numTxUhcCh; i++)
        {
            offset = i >> 5U;
            Udma_assert(drvHandle, offset < UDMA_RM_TX_UHC_CH_ARR_SIZE);
            bitPos = i - (offset << 5U);
            bitMask = (uint32_t) 1U << bitPos;
            if((drvHandle->txUhcChFlag[offset] & bitMask) == bitMask)
            {
                drvHandle->txUhcChFlag[offset] &= ~bitMask;
                chNum = i + rmInitPrms->startTxUhcCh;  /* Add start offset */
                break;
            }
        }
    }
    else
    {
        /* Allocate specific TX UHC channel if free */
        /* Array bound check */
        if((preferredChNum >= rmInitPrms->startTxUhcCh) &&
           (preferredChNum < (rmInitPrms->startTxUhcCh + rmInitPrms->numTxUhcCh)))
        {
            i = preferredChNum - rmInitPrms->startTxUhcCh;
            offset = i >> 5U;
            Udma_assert(drvHandle, offset < UDMA_RM_TX_UHC_CH_ARR_SIZE);
            bitPos = i - (offset << 5U);
            bitMask = (uint32_t) 1U << bitPos;
            if((drvHandle->txUhcChFlag[offset] & bitMask) == bitMask)
            {
                drvHandle->txUhcChFlag[offset] &= ~bitMask;
                chNum = preferredChNum;
            }
        }
    }

    Udma_assert(drvHandle, drvHandle->initPrms.osalPrms.unlockMutex != (Udma_OsalMutexUnlockFxn) NULL_PTR);
    drvHandle->initPrms.osalPrms.unlockMutex(drvHandle->rmLock);

    return (chNum);
}

void Udma_rmFreeTxUhcCh(uint32_t chNum, Udma_DrvHandle drvHandle)
{
    uint32_t            i, offset, bitPos, bitMask;
    Udma_RmInitPrms    *rmInitPrms = &drvHandle->initPrms.rmInitPrms;

    Udma_assert(drvHandle, drvHandle->initPrms.osalPrms.lockMutex != (Udma_OsalMutexLockFxn) NULL_PTR);
    drvHandle->initPrms.osalPrms.lockMutex(drvHandle->rmLock);

    Udma_assert(drvHandle, chNum >= (rmInitPrms->startTxUhcCh));
    Udma_assert(drvHandle,
        chNum < (rmInitPrms->startTxUhcCh + rmInitPrms->numTxUhcCh));
    i = chNum - rmInitPrms->startTxUhcCh;
    offset = i >> 5U;
    Udma_assert(drvHandle, offset < UDMA_RM_TX_UHC_CH_ARR_SIZE);
    bitPos = i - (offset << 5U);
    bitMask = (uint32_t) 1U << bitPos;
    Udma_assert(drvHandle,
        (drvHandle->txUhcChFlag[offset] & bitMask) == 0U);
    drvHandle->txUhcChFlag[offset] |= bitMask;

    Udma_assert(drvHandle, drvHandle->initPrms.osalPrms.unlockMutex != (Udma_OsalMutexUnlockFxn) NULL_PTR);
    drvHandle->initPrms.osalPrms.unlockMutex(drvHandle->rmLock);

    return;
}

uint32_t Udma_rmAllocRxUhcCh(uint32_t preferredChNum, Udma_DrvHandle drvHandle)
{
    uint32_t            i, offset, bitPos, bitMask;
    uint32_t            chNum = UDMA_DMA_CH_INVALID;
    Udma_RmInitPrms    *rmInitPrms = &drvHandle->initPrms.rmInitPrms;

    Udma_assert(drvHandle, drvHandle->initPrms.osalPrms.lockMutex != (Udma_OsalMutexLockFxn) NULL_PTR);
    drvHandle->initPrms.osalPrms.lockMutex(drvHandle->rmLock);

    if(UDMA_DMA_CH_ANY == preferredChNum)
    {
        /* Search and allocate from RX UHC channel pool */
        for(i = 0U; i < rmInitPrms->numRxUhcCh; i++)
        {
            offset = i >> 5U;
            Udma_assert(drvHandle, offset < UDMA_RM_RX_UHC_CH_ARR_SIZE);
            bitPos = i - (offset << 5U);
            bitMask = (uint32_t) 1U << bitPos;
            if((drvHandle->rxUhcChFlag[offset] & bitMask) == bitMask)
            {
                drvHandle->rxUhcChFlag[offset] &= ~bitMask;
                chNum = i + rmInitPrms->startRxUhcCh;  /* Add start offset */
                break;
            }
        }
    }
    else
    {
        /* Allocate specific RX UHC channel if free */
        /* Array bound check */
        if((preferredChNum >= rmInitPrms->startRxUhcCh) &&
           (preferredChNum < (rmInitPrms->startRxUhcCh + rmInitPrms->numRxUhcCh)))
        {
            i = preferredChNum - rmInitPrms->startRxUhcCh;
            offset = i >> 5U;
            Udma_assert(drvHandle, offset < UDMA_RM_RX_UHC_CH_ARR_SIZE);
            bitPos = i - (offset << 5U);
            bitMask = (uint32_t) 1U << bitPos;
            if((drvHandle->rxUhcChFlag[offset] & bitMask) == bitMask)
            {
                drvHandle->rxUhcChFlag[offset] &= ~bitMask;
                chNum = preferredChNum;
            }
        }
    }

    Udma_assert(drvHandle, drvHandle->initPrms.osalPrms.unlockMutex != (Udma_OsalMutexUnlockFxn) NULL_PTR);
    drvHandle->initPrms.osalPrms.unlockMutex(drvHandle->rmLock);

    return (chNum);
}

void Udma_rmFreeRxUhcCh(uint32_t chNum, Udma_DrvHandle drvHandle)
{
    uint32_t            i, offset, bitPos, bitMask;
    Udma_RmInitPrms    *rmInitPrms = &drvHandle->initPrms.rmInitPrms;

    Udma_assert(drvHandle, drvHandle->initPrms.osalPrms.lockMutex != (Udma_OsalMutexLockFxn) NULL_PTR);
    drvHandle->initPrms.osalPrms.lockMutex(drvHandle->rmLock);

    Udma_assert(drvHandle, chNum >= rmInitPrms->startRxUhcCh);
    Udma_assert(drvHandle,
        chNum < (rmInitPrms->startRxUhcCh + rmInitPrms->numRxUhcCh));
    i = chNum - rmInitPrms->startRxUhcCh;
    offset = i >> 5U;
    Udma_assert(drvHandle, offset < UDMA_RM_RX_UHC_CH_ARR_SIZE);
    bitPos = i - (offset << 5U);
    bitMask = (uint32_t) 1U << bitPos;
    Udma_assert(drvHandle,
        (drvHandle->rxUhcChFlag[offset] & bitMask) == 0U);
    drvHandle->rxUhcChFlag[offset] |= bitMask;

    Udma_assert(drvHandle, drvHandle->initPrms.osalPrms.unlockMutex != (Udma_OsalMutexUnlockFxn) NULL_PTR);
    drvHandle->initPrms.osalPrms.unlockMutex(drvHandle->rmLock);

    return;
}

#if (UDMA_NUM_UTC_INSTANCE > 0)
uint32_t Udma_rmAllocExtCh(uint32_t preferredChNum,
                           Udma_DrvHandle drvHandle,
                           const Udma_UtcInstInfo *utcInfo)
{
    uint32_t            chNum = UDMA_DMA_CH_INVALID;
    uint32_t            i, offset, bitPos, bitMask;
    uint32_t            utcId;
    Udma_RmInitPrms    *rmInitPrms = &drvHandle->initPrms.rmInitPrms;

    Udma_assert(drvHandle, utcInfo != NULL_PTR);
    utcId = utcInfo->utcId;
    Udma_assert(drvHandle, utcId < UDMA_NUM_UTC_INSTANCE);
    Udma_assert(drvHandle,
        rmInitPrms->startUtcCh[utcId] >= utcInfo->startCh);
    Udma_assert(drvHandle,
        rmInitPrms->startUtcCh[utcId] < (utcInfo->startCh + utcInfo->numCh));
    Udma_assert(drvHandle,
        (rmInitPrms->startUtcCh[utcId] + rmInitPrms->numUtcCh[utcId]) <=
            (utcInfo->startCh + utcInfo->numCh));

    Udma_assert(drvHandle, drvHandle->initPrms.osalPrms.lockMutex != (Udma_OsalMutexLockFxn) NULL_PTR);
    drvHandle->initPrms.osalPrms.lockMutex(drvHandle->rmLock);

    if(UDMA_DMA_CH_ANY == preferredChNum)
    {
        /* Search and allocate from specific external channel pool */
        for(i = 0U; i < rmInitPrms->numUtcCh[utcId]; i++)
        {
            offset = i >> 5U;
            Udma_assert(drvHandle, offset < UDMA_RM_UTC_CH_ARR_SIZE);
            bitPos = i - (offset << 5U);
            bitMask = (uint32_t) 1U << bitPos;
            if((drvHandle->utcChFlag[utcId][offset] & bitMask) == bitMask)
            {
                drvHandle->utcChFlag[utcId][offset] &= ~bitMask;
                chNum = i + rmInitPrms->startUtcCh[utcId];  /* Add start offset */
                break;
            }
        }
    }
    else
    {
        if(preferredChNum < rmInitPrms->numUtcCh[utcId])
        {
            i = preferredChNum;
            offset = i >> 5U;
            Udma_assert(drvHandle, offset < UDMA_RM_UTC_CH_ARR_SIZE);
            bitPos = i - (offset << 5U);
            bitMask = (uint32_t) 1U << bitPos;
            if((drvHandle->utcChFlag[utcId][offset] & bitMask) == bitMask)
            {
                drvHandle->utcChFlag[utcId][offset] &= ~bitMask;
                chNum = preferredChNum + rmInitPrms->startUtcCh[utcId];
            }
        }
    }

    Udma_assert(drvHandle, drvHandle->initPrms.osalPrms.unlockMutex != (Udma_OsalMutexUnlockFxn) NULL_PTR);
    drvHandle->initPrms.osalPrms.unlockMutex(drvHandle->rmLock);

    return (chNum);
}

void Udma_rmFreeExtCh(uint32_t chNum,
                      Udma_DrvHandle drvHandle,
                      const Udma_UtcInstInfo *utcInfo)
{
    uint32_t            i, offset, bitPos, bitMask;
    uint32_t            utcId;
    Udma_RmInitPrms    *rmInitPrms = &drvHandle->initPrms.rmInitPrms;

    Udma_assert(drvHandle, utcInfo != NULL_PTR);
    utcId = utcInfo->utcId;

    Udma_assert(drvHandle, drvHandle->initPrms.osalPrms.lockMutex != (Udma_OsalMutexLockFxn) NULL_PTR);
    drvHandle->initPrms.osalPrms.lockMutex(drvHandle->rmLock);
    Udma_assert(drvHandle, chNum >= rmInitPrms->startUtcCh[utcId]);
    Udma_assert(drvHandle,
        chNum < (rmInitPrms->startUtcCh[utcId] + rmInitPrms->numUtcCh[utcId]));
    i = chNum - rmInitPrms->startUtcCh[utcId];
    offset = i >> 5U;
    Udma_assert(drvHandle, offset < UDMA_RM_UTC_CH_ARR_SIZE);
    bitPos = i - (offset << 5U);
    bitMask = (uint32_t) 1U << bitPos;
    Udma_assert(drvHandle,
        (drvHandle->utcChFlag[utcId][offset] & bitMask) == 0U);
    drvHandle->utcChFlag[utcId][offset] |= bitMask;

    Udma_assert(drvHandle, drvHandle->initPrms.osalPrms.unlockMutex != (Udma_OsalMutexUnlockFxn) NULL_PTR);
    drvHandle->initPrms.osalPrms.unlockMutex(drvHandle->rmLock);

    return;
}
#endif

#if (UDMA_NUM_MAPPED_TX_GROUP > 0)
uint32_t Udma_rmAllocMappedTxCh(uint32_t preferredChNum, 
                                Udma_DrvHandle drvHandle,
                                const uint32_t mappedChGrp)                 
{
    uint32_t            chNum = UDMA_DMA_CH_INVALID;
    uint32_t            i, offset, bitPos, bitMask;
    Udma_RmInitPrms    *rmInitPrms = &drvHandle->initPrms.rmInitPrms;

    Udma_assert(drvHandle, mappedChGrp < UDMA_NUM_MAPPED_TX_GROUP);

    Udma_assert(drvHandle, drvHandle->initPrms.osalPrms.lockMutex != (Udma_OsalMutexLockFxn) NULL_PTR);
    drvHandle->initPrms.osalPrms.lockMutex(drvHandle->rmLock);

    if(UDMA_DMA_CH_ANY == preferredChNum)
    {
        /* Search and allocate from specific mapped TX channel pool */
        for(i = 0U; i < rmInitPrms->numMappedTxCh[mappedChGrp]; i++)
        {
            offset = i >> 5U;
            Udma_assert(drvHandle, offset < UDMA_RM_MAPPED_TX_CH_ARR_SIZE);
            bitPos = i - (offset << 5U);
            bitMask = (uint32_t) 1U << bitPos;
            if((drvHandle->mappedTxChFlag[mappedChGrp][offset] & bitMask) == bitMask)
            {
                drvHandle->mappedTxChFlag[mappedChGrp][offset] &= ~bitMask;
                chNum = i + rmInitPrms->startMappedTxCh[mappedChGrp];  /* Add start offset */
                break;
            }
        }
    }
    else
    {
        /* Allocate specific mapped TX channel if free */
        /* Array bound check */
        if((preferredChNum >= rmInitPrms->startMappedTxCh[mappedChGrp]) &&
           (preferredChNum < (rmInitPrms->startMappedTxCh[mappedChGrp] + rmInitPrms->numMappedTxCh[mappedChGrp])))
        {
            i = preferredChNum;
            offset = i >> 5U;
            Udma_assert(drvHandle, offset < UDMA_RM_MAPPED_TX_CH_ARR_SIZE);
            bitPos = i - (offset << 5U);
            bitMask = (uint32_t) 1U << bitPos;
            if((drvHandle->mappedTxChFlag[mappedChGrp][offset] & bitMask) == bitMask)
            {
                drvHandle->mappedTxChFlag[mappedChGrp][offset] &= ~bitMask;
                chNum = preferredChNum;
            }
        }
    }

    Udma_assert(drvHandle, drvHandle->initPrms.osalPrms.unlockMutex != (Udma_OsalMutexUnlockFxn) NULL_PTR);
    drvHandle->initPrms.osalPrms.unlockMutex(drvHandle->rmLock);

    return (chNum);
}

void Udma_rmFreeMappedTxCh(uint32_t chNum,
                           Udma_DrvHandle drvHandle,
                           const uint32_t mappedChGrp)
{
    uint32_t            i, offset, bitPos, bitMask;
    Udma_RmInitPrms    *rmInitPrms = &drvHandle->initPrms.rmInitPrms;

    Udma_assert(drvHandle, drvHandle->initPrms.osalPrms.lockMutex != (Udma_OsalMutexLockFxn) NULL_PTR);
    drvHandle->initPrms.osalPrms.lockMutex(drvHandle->rmLock);

    Udma_assert(drvHandle, chNum >= rmInitPrms->startMappedTxCh[mappedChGrp]);
    Udma_assert(drvHandle,
        chNum < (rmInitPrms->startMappedTxCh[mappedChGrp] + rmInitPrms->numMappedTxCh[mappedChGrp]));
    i = chNum - rmInitPrms->startMappedTxCh[mappedChGrp];
    offset = i >> 5U;
    Udma_assert(drvHandle, offset < UDMA_RM_MAPPED_TX_CH_ARR_SIZE);
    bitPos = i - (offset << 5U);
    bitMask = (uint32_t) 1U << bitPos;
    Udma_assert(drvHandle,
        (drvHandle->mappedTxChFlag[mappedChGrp][offset] & bitMask) == 0U);
    drvHandle->mappedTxChFlag[mappedChGrp][offset] |= bitMask;

    Udma_assert(drvHandle, drvHandle->initPrms.osalPrms.unlockMutex != (Udma_OsalMutexUnlockFxn) NULL_PTR);
    drvHandle->initPrms.osalPrms.unlockMutex(drvHandle->rmLock);

    return;
}
#endif

#if (UDMA_NUM_MAPPED_RX_GROUP > 0)
uint32_t Udma_rmAllocMappedRxCh(uint32_t preferredChNum, 
                                Udma_DrvHandle drvHandle,
                                const uint32_t mappedChGrp)                 
{
    uint32_t            chNum = UDMA_DMA_CH_INVALID;
    uint32_t            i, offset, bitPos, bitMask;
    Udma_RmInitPrms    *rmInitPrms = &drvHandle->initPrms.rmInitPrms;

    Udma_assert(drvHandle, mappedChGrp < UDMA_NUM_MAPPED_RX_GROUP);

    Udma_assert(drvHandle, drvHandle->initPrms.osalPrms.lockMutex != (Udma_OsalMutexLockFxn) NULL_PTR);
    drvHandle->initPrms.osalPrms.lockMutex(drvHandle->rmLock);

    if(UDMA_DMA_CH_ANY == preferredChNum)
    {
        /* Search and allocate from specific mapped RX channel pool */
        for(i = 0U; i < rmInitPrms->numMappedRxCh[mappedChGrp]; i++)
        {
            offset = i >> 5U;
            Udma_assert(drvHandle, offset < UDMA_RM_MAPPED_RX_CH_ARR_SIZE);
            bitPos = i - (offset << 5U);
            bitMask = (uint32_t) 1U << bitPos;
            if((drvHandle->mappedRxChFlag[mappedChGrp][offset] & bitMask) == bitMask)
            {
                drvHandle->mappedRxChFlag[mappedChGrp][offset] &= ~bitMask;
                chNum = i + rmInitPrms->startMappedRxCh[mappedChGrp];  /* Add start offset */
                break;
            }
        }
    }
    else
    {
        /* Allocate specific mapped RX channel if free */
        /* Array bound check */
        if((preferredChNum >= rmInitPrms->startMappedRxCh[mappedChGrp]) &&
           (preferredChNum < (rmInitPrms->startMappedRxCh[mappedChGrp] + rmInitPrms->numMappedRxCh[mappedChGrp])))
        {
            i = preferredChNum;
            offset = i >> 5U;
            Udma_assert(drvHandle, offset < UDMA_RM_MAPPED_RX_CH_ARR_SIZE);
            bitPos = i - (offset << 5U);
            bitMask = (uint32_t) 1U << bitPos;
            if((drvHandle->mappedRxChFlag[mappedChGrp][offset] & bitMask) == bitMask)
            {
                drvHandle->mappedRxChFlag[mappedChGrp][offset] &= ~bitMask;
                chNum = preferredChNum;
            }
        }
    }

    Udma_assert(drvHandle, drvHandle->initPrms.osalPrms.unlockMutex != (Udma_OsalMutexUnlockFxn) NULL_PTR);
    drvHandle->initPrms.osalPrms.unlockMutex(drvHandle->rmLock);

    return (chNum);
}

void Udma_rmFreeMappedRxCh(uint32_t chNum,
                           Udma_DrvHandle drvHandle,
                           const uint32_t mappedChGrp)
{
    uint32_t            i, offset, bitPos, bitMask;
    Udma_RmInitPrms    *rmInitPrms = &drvHandle->initPrms.rmInitPrms;

    Udma_assert(drvHandle, drvHandle->initPrms.osalPrms.lockMutex != (Udma_OsalMutexLockFxn) NULL_PTR);
    drvHandle->initPrms.osalPrms.lockMutex(drvHandle->rmLock);

    Udma_assert(drvHandle, chNum >= rmInitPrms->startMappedRxCh[mappedChGrp]);
    Udma_assert(drvHandle,
        chNum < (rmInitPrms->startMappedRxCh[mappedChGrp] + rmInitPrms->numMappedRxCh[mappedChGrp]));
    i = chNum - rmInitPrms->startMappedRxCh[mappedChGrp];
    offset = i >> 5U;
    Udma_assert(drvHandle, offset < UDMA_RM_MAPPED_RX_CH_ARR_SIZE);
    bitPos = i - (offset << 5U);
    bitMask = (uint32_t) 1U << bitPos;
    Udma_assert(drvHandle,
        (drvHandle->mappedRxChFlag[mappedChGrp][offset] & bitMask) == 0U);
    drvHandle->mappedRxChFlag[mappedChGrp][offset] |= bitMask;

    Udma_assert(drvHandle, drvHandle->initPrms.osalPrms.unlockMutex != (Udma_OsalMutexUnlockFxn) NULL_PTR);
    drvHandle->initPrms.osalPrms.unlockMutex(drvHandle->rmLock);

    return;
}
#endif

#if((UDMA_NUM_MAPPED_TX_GROUP + UDMA_NUM_MAPPED_RX_GROUP) > 0)
uint32_t Udma_rmAllocMappedRing(Udma_DrvHandle drvHandle,
                                const uint32_t mappedRingGrp,
                                const uint32_t mappedChNum)                 
{
    uint32_t    ringNum = UDMA_RING_INVALID;
    uint32_t    i,offset, bitPos, bitMask;  
    uint32_t    loopStart, loopMax;      
    int32_t     retVal = UDMA_SOK;

    Udma_RmInitPrms             *rmInitPrms = &drvHandle->initPrms.rmInitPrms;
    Udma_MappedChRingAttributes  chAttr;

    Udma_assert(drvHandle, mappedRingGrp < (UDMA_NUM_MAPPED_TX_GROUP + UDMA_NUM_MAPPED_RX_GROUP));

    retVal = Udma_getMappedChRingAttributes(drvHandle, mappedRingGrp, mappedChNum, &chAttr);
    
    if((UDMA_SOK == retVal) && (0U != chAttr.numFreeRing))
    {
        /* Derive the intersecting pool (loopStart and loopMax) based on the rings reserved for the core (rmcfg)
        * and the permissible range for the given channel(free rings reserved for specific channels).
        * 
        * Core_ring_Start (rmInitPrms->startMappedRing) & Core_ring_End (rmInitPrms->startMappedRing +rmInitPrms->numMappedRing)
        * refers to the range of reserved rings for the core.
        * Channel_ring_Start (chAttr->startFreeRing) & Channel_ring_End (chAttr->startFreeRing + chAttr->numFreeRing) 
        * refers to permissible range of free rings for the particular channel.
        * 
        * CASE 'A' refers to those that affects the loopStart
        * CASE 'B' refers to those that affects the loopMax
        */

        /* Default Loop Values*/
        loopStart = 0;
        loopMax   = rmInitPrms->numMappedRing[mappedRingGrp];

        /* CASE_A_1 : Channel_ring_Start > Core_ring_Start */
        if(chAttr.startFreeRing > rmInitPrms->startMappedRing[mappedRingGrp])
        {
            /* Update loopStart to start from Channel_ring_Start,
            * so as to skip the starting rings which are reserved for the core, 
            * but can't be used for the current channel */
            loopStart = chAttr.startFreeRing - rmInitPrms->startMappedRing[mappedRingGrp]; 
        }
        /* For all other CASE 'A's, loopStart should be 0 itself. */

        /* CASE_B_1 : Channel_ring_End < Core_ring_End */
        if((chAttr.startFreeRing + chAttr.numFreeRing) < (rmInitPrms->startMappedRing[mappedRingGrp] + rmInitPrms->numMappedRing[mappedRingGrp]))
        {
            /* Update loopMax to stop at Channel_ring_End,
            * so as to skip the last rings which are reserved for the core, 
            * but can't be used for the current channel */
            Udma_assert(drvHandle, (chAttr.startFreeRing + chAttr.numFreeRing) >= rmInitPrms->startMappedRing[mappedRingGrp]);
            loopMax = (chAttr.startFreeRing + chAttr.numFreeRing) - rmInitPrms->startMappedRing[mappedRingGrp];
        }
        /* For all other CASE 'B's, loopMax should be rmInitPrms->numMappedRing[mappedRingGrp] itself. */

        Udma_assert(drvHandle, drvHandle->initPrms.osalPrms.lockMutex != (Udma_OsalMutexLockFxn) NULL_PTR);
        drvHandle->initPrms.osalPrms.lockMutex(drvHandle->rmLock);

        /* Search and allocate from derived intersecting pool */
        for(i = loopStart; i < loopMax; i++)
        {
            offset = i >> 5U;
            Udma_assert(drvHandle, offset < UDMA_RM_MAPPED_RING_ARR_SIZE);
            bitPos = i - (offset << 5U);
            bitMask = (uint32_t) 1U << bitPos;
            if((drvHandle->mappedRingFlag[mappedRingGrp][offset] & bitMask) == bitMask)
            {
                drvHandle->mappedRingFlag[mappedRingGrp][offset] &= ~bitMask;
                ringNum = i + rmInitPrms->startMappedRing[mappedRingGrp];  /* Add start offset */
                break;
            }
        }

        Udma_assert(drvHandle, drvHandle->initPrms.osalPrms.unlockMutex != (Udma_OsalMutexUnlockFxn) NULL_PTR);
        drvHandle->initPrms.osalPrms.unlockMutex(drvHandle->rmLock);
    }

    return (ringNum);
}

void Udma_rmFreeMappedRing(uint32_t ringNum,
                           Udma_DrvHandle drvHandle,
                           const uint32_t mappedRingGrp,
                           const uint32_t mappedChNum)
{
    uint32_t            i, offset, bitPos, bitMask;
    Udma_RmInitPrms    *rmInitPrms = &drvHandle->initPrms.rmInitPrms;
    Udma_MappedChRingAttributes  chAttr;   
    int32_t     retVal = UDMA_SOK;

    Udma_assert(drvHandle, mappedRingGrp < (UDMA_NUM_MAPPED_TX_GROUP + UDMA_NUM_MAPPED_RX_GROUP));

    retVal = Udma_getMappedChRingAttributes(drvHandle, mappedRingGrp, mappedChNum, &chAttr);
    Udma_assert(drvHandle, UDMA_SOK == retVal);

    /* Free up only the free mapped ring - ignore default mapped ring */
    if(ringNum != chAttr.defaultRing)
    {
        Udma_assert(drvHandle, drvHandle->initPrms.osalPrms.lockMutex != (Udma_OsalMutexLockFxn) NULL_PTR);
        drvHandle->initPrms.osalPrms.lockMutex(drvHandle->rmLock);

        Udma_assert(drvHandle, ringNum >= rmInitPrms->startMappedRing[mappedRingGrp]);
        Udma_assert(drvHandle,
            ringNum < (rmInitPrms->startMappedRing[mappedRingGrp] + rmInitPrms->numMappedRing[mappedRingGrp]));
        i = ringNum - rmInitPrms->startMappedRing[mappedRingGrp];
        offset = i >> 5U;
        Udma_assert(drvHandle, offset < UDMA_RM_MAPPED_RING_ARR_SIZE);
        bitPos = i - (offset << 5U);
        bitMask = (uint32_t) 1U << bitPos;
        Udma_assert(drvHandle,
            (drvHandle->mappedRingFlag[mappedRingGrp][offset] & bitMask) == 0U);
        drvHandle->mappedRingFlag[mappedRingGrp][offset] |= bitMask;

        Udma_assert(drvHandle, drvHandle->initPrms.osalPrms.unlockMutex != (Udma_OsalMutexUnlockFxn) NULL_PTR);
        drvHandle->initPrms.osalPrms.unlockMutex(drvHandle->rmLock);
    }
    return;
}
#endif

uint16_t Udma_rmAllocFreeRing(Udma_DrvHandle drvHandle)
{
    uint16_t            ringNum = UDMA_RING_INVALID;

#if (UDMA_SOC_CFG_UDMAP_PRESENT == 1)
    uint16_t            i, offset, temp;
    uint32_t            bitPos, bitMask;
    Udma_RmInitPrms    *rmInitPrms = &drvHandle->initPrms.rmInitPrms;

    Udma_assert(drvHandle, drvHandle->initPrms.osalPrms.lockMutex != (Udma_OsalMutexLockFxn) NULL_PTR);
    drvHandle->initPrms.osalPrms.lockMutex(drvHandle->rmLock);

    for(i = 0U; i < rmInitPrms->numFreeRing; i++)
    {
        offset = i >> 5U;
        Udma_assert(drvHandle, offset < UDMA_RM_FREE_RING_ARR_SIZE);
        temp = i - (offset << 5U);
        bitPos = (uint32_t) temp;
        bitMask = (uint32_t) 1U << bitPos;
        if((drvHandle->freeRingFlag[offset] & bitMask) == bitMask)
        {
            drvHandle->freeRingFlag[offset] &= ~bitMask;
            ringNum = (uint16_t)rmInitPrms->startFreeRing;  /* Add start offset */
            ringNum += i;
            ringNum += (uint16_t)(drvHandle->udmapRegs.txChanCnt +
                                  drvHandle->udmapRegs.txExtUtcChanCnt +
                                  drvHandle->udmapRegs.rxChanCnt);
            break;
        }
    }

    Udma_assert(drvHandle, drvHandle->initPrms.osalPrms.unlockMutex != (Udma_OsalMutexUnlockFxn) NULL_PTR);
    drvHandle->initPrms.osalPrms.unlockMutex(drvHandle->rmLock);
#endif

    return (ringNum);
}

void Udma_rmFreeFreeRing(uint16_t ringNum, Udma_DrvHandle drvHandle)
{
    
#if (UDMA_SOC_CFG_UDMAP_PRESENT == 1)
    uint32_t            i, offset, bitPos, bitMask;
    Udma_RmInitPrms    *rmInitPrms = &drvHandle->initPrms.rmInitPrms;
    uint32_t            freeRingOffset = rmInitPrms->startFreeRing +
                                 drvHandle->udmapRegs.txChanCnt +
                                 drvHandle->udmapRegs.txExtUtcChanCnt +
                                 drvHandle->udmapRegs.rxChanCnt;

    /* Free up only the free ring - ignore FQ rings */
    if(ringNum >= freeRingOffset)
    {
        Udma_assert(drvHandle, drvHandle->initPrms.osalPrms.lockMutex != (Udma_OsalMutexLockFxn) NULL_PTR);
        drvHandle->initPrms.osalPrms.lockMutex(drvHandle->rmLock);

        i = ringNum - freeRingOffset;
        offset = i >> 5U;
        Udma_assert(drvHandle, offset < UDMA_RM_FREE_RING_ARR_SIZE);
        bitPos = i - (offset << 5U);
        bitMask = (uint32_t) 1U << bitPos;
        Udma_assert(drvHandle, (drvHandle->freeRingFlag[offset] & bitMask) == 0U);
        drvHandle->freeRingFlag[offset] |= bitMask;

        Udma_assert(drvHandle, drvHandle->initPrms.osalPrms.unlockMutex != (Udma_OsalMutexUnlockFxn) NULL_PTR);
        drvHandle->initPrms.osalPrms.unlockMutex(drvHandle->rmLock);
    }
#endif

    return;
}

uint16_t Udma_rmAllocProxy(Udma_DrvHandle drvHandle)
{
    uint16_t            i, offset, proxyNum = UDMA_PROXY_INVALID, temp;
    uint32_t            bitPos, bitMask;
    Udma_RmInitPrms    *rmInitPrms = &drvHandle->initPrms.rmInitPrms;

    Udma_assert(drvHandle, drvHandle->initPrms.osalPrms.lockMutex != (Udma_OsalMutexLockFxn) NULL_PTR);
    drvHandle->initPrms.osalPrms.lockMutex(drvHandle->rmLock);

    for(i = 0U; i < rmInitPrms->numProxy; i++)
    {
        offset = i >> 5U;
        Udma_assert(drvHandle, offset < UDMA_RM_PROXY_ARR_SIZE);
        temp = i - (offset << 5U);
        bitPos = (uint32_t) temp;
        bitMask = (uint32_t) 1U << bitPos;
        if((drvHandle->proxyFlag[offset] & bitMask) == bitMask)
        {
            drvHandle->proxyFlag[offset] &= ~bitMask;
            proxyNum = (uint16_t)rmInitPrms->startProxy;  /* Add start offset */
            proxyNum += i;
            break;
        }
    }

    Udma_assert(drvHandle, drvHandle->initPrms.osalPrms.unlockMutex != (Udma_OsalMutexUnlockFxn) NULL_PTR);
    drvHandle->initPrms.osalPrms.unlockMutex(drvHandle->rmLock);

    return (proxyNum);
}

void Udma_rmFreeProxy(uint16_t proxyNum, Udma_DrvHandle drvHandle)
{
    uint32_t            i, offset, bitPos, bitMask;
    Udma_RmInitPrms    *rmInitPrms = &drvHandle->initPrms.rmInitPrms;

    Udma_assert(drvHandle, drvHandle->initPrms.osalPrms.lockMutex != (Udma_OsalMutexLockFxn) NULL_PTR);
    drvHandle->initPrms.osalPrms.lockMutex(drvHandle->rmLock);

    i = proxyNum - rmInitPrms->startProxy;
    offset = i >> 5U;
    Udma_assert(drvHandle, offset < UDMA_RM_PROXY_ARR_SIZE);
    bitPos = i - (offset << 5U);
    bitMask = (uint32_t) 1U << bitPos;
    Udma_assert(drvHandle, (drvHandle->proxyFlag[offset] & bitMask) == 0U);
    drvHandle->proxyFlag[offset] |= bitMask;

    Udma_assert(drvHandle, drvHandle->initPrms.osalPrms.unlockMutex != (Udma_OsalMutexUnlockFxn) NULL_PTR);
    drvHandle->initPrms.osalPrms.unlockMutex(drvHandle->rmLock);

    return;
}

uint32_t Udma_rmAllocEvent(Udma_DrvHandle drvHandle)
{
    uint32_t            globalEvent = UDMA_EVENT_INVALID;
    uint32_t            i, offset, bitPos, bitMask;
    Udma_RmInitPrms    *rmInitPrms = &drvHandle->initPrms.rmInitPrms;

    Udma_assert(drvHandle, drvHandle->initPrms.osalPrms.lockMutex != (Udma_OsalMutexLockFxn) NULL_PTR);
    drvHandle->initPrms.osalPrms.lockMutex(drvHandle->rmLock);

    for(i = 0U; i < rmInitPrms->numGlobalEvent; i++)
    {
        offset = i >> 5U;
        Udma_assert(drvHandle, offset < UDMA_RM_GLOBAL_EVENT_ARR_SIZE);
        bitPos = i - (offset << 5U);
        bitMask = (uint32_t) 1U << bitPos;
        if((drvHandle->globalEventFlag[offset] & bitMask) == bitMask)
        {
            drvHandle->globalEventFlag[offset] &= ~bitMask;
            globalEvent = i + rmInitPrms->startGlobalEvent;  /* Add start offset */
            break;
        }
    }

    Udma_assert(drvHandle, drvHandle->initPrms.osalPrms.unlockMutex != (Udma_OsalMutexUnlockFxn) NULL_PTR);
    drvHandle->initPrms.osalPrms.unlockMutex(drvHandle->rmLock);

    return (globalEvent);
}

void Udma_rmFreeEvent(uint32_t globalEvent, Udma_DrvHandle drvHandle)
{
    uint32_t            i, offset, bitPos, bitMask;
    Udma_RmInitPrms    *rmInitPrms = &drvHandle->initPrms.rmInitPrms;

    Udma_assert(drvHandle,
        globalEvent < (rmInitPrms->startGlobalEvent + rmInitPrms->numGlobalEvent));
    Udma_assert(drvHandle, globalEvent >= rmInitPrms->startGlobalEvent);

    Udma_assert(drvHandle, drvHandle->initPrms.osalPrms.lockMutex != (Udma_OsalMutexLockFxn) NULL_PTR);
    drvHandle->initPrms.osalPrms.lockMutex(drvHandle->rmLock);

    i = globalEvent - rmInitPrms->startGlobalEvent;
    offset = i >> 5U;
    Udma_assert(drvHandle, offset < UDMA_RM_GLOBAL_EVENT_ARR_SIZE);
    bitPos = i - (offset << 5U);
    bitMask = (uint32_t) 1U << bitPos;
    Udma_assert(drvHandle, (drvHandle->globalEventFlag[offset] & bitMask) == 0U);
    drvHandle->globalEventFlag[offset] |= bitMask;

    Udma_assert(drvHandle, drvHandle->initPrms.osalPrms.unlockMutex != (Udma_OsalMutexUnlockFxn) NULL_PTR);
    drvHandle->initPrms.osalPrms.unlockMutex(drvHandle->rmLock);

    return;
}

uint32_t Udma_rmAllocVintr(Udma_DrvHandle drvHandle)
{
    uint32_t            i, offset, bitPos, bitMask;
    uint32_t            vintrNum = UDMA_EVENT_INVALID;
    Udma_RmInitPrms    *rmInitPrms = &drvHandle->initPrms.rmInitPrms;

    Udma_assert(drvHandle, drvHandle->initPrms.osalPrms.lockMutex != (Udma_OsalMutexLockFxn) NULL_PTR);
    drvHandle->initPrms.osalPrms.lockMutex(drvHandle->rmLock);

    for(i = 0U; i < rmInitPrms->numVintr; i++)
    {
        offset = i >> 5U;
        Udma_assert(drvHandle, offset < UDMA_RM_VINTR_ARR_SIZE);
        bitPos = i - (offset << 5U);
        bitMask = (uint32_t) 1U << bitPos;
        if((drvHandle->vintrFlag[offset] & bitMask) == bitMask)
        {
            drvHandle->vintrFlag[offset] &= ~bitMask;
            vintrNum = i + rmInitPrms->startVintr;  /* Add start offset */
            break;
        }
    }

    Udma_assert(drvHandle, drvHandle->initPrms.osalPrms.unlockMutex != (Udma_OsalMutexUnlockFxn) NULL_PTR);
    drvHandle->initPrms.osalPrms.unlockMutex(drvHandle->rmLock);

    return (vintrNum);
}

void Udma_rmFreeVintr(uint32_t vintrNum, Udma_DrvHandle drvHandle)
{
    uint32_t            i, offset, bitPos, bitMask;
    Udma_RmInitPrms    *rmInitPrms = &drvHandle->initPrms.rmInitPrms;

    Udma_assert(drvHandle,
        vintrNum < (rmInitPrms->startVintr + rmInitPrms->numVintr));
    Udma_assert(drvHandle, vintrNum >= rmInitPrms->startVintr);

    Udma_assert(drvHandle, drvHandle->initPrms.osalPrms.lockMutex != (Udma_OsalMutexLockFxn) NULL_PTR);
    drvHandle->initPrms.osalPrms.lockMutex(drvHandle->rmLock);

    i = vintrNum - rmInitPrms->startVintr;
    offset = i >> 5U;
    Udma_assert(drvHandle, offset < UDMA_RM_VINTR_ARR_SIZE);
    bitPos = i - (offset << 5U);
    bitMask = (uint32_t) 1U << bitPos;
    Udma_assert(drvHandle, (drvHandle->vintrFlag[offset] & bitMask) == 0U);
    drvHandle->vintrFlag[offset] |= bitMask;

    Udma_assert(drvHandle, drvHandle->initPrms.osalPrms.unlockMutex != (Udma_OsalMutexUnlockFxn) NULL_PTR);
    drvHandle->initPrms.osalPrms.unlockMutex(drvHandle->rmLock);

    return;
}

uint32_t Udma_rmAllocVintrBit(Udma_EventHandle eventHandle)
{
    uint32_t                i;
    uint32_t                vintrBitNum = UDMA_EVENT_INVALID;
    uint64_t                bitMask;
    Udma_EventHandle        masterEventHandle;
    const Udma_EventPrms   *eventPrms;
    Udma_DrvHandle          drvHandle = eventHandle->drvHandle;

    masterEventHandle = eventHandle;
    eventPrms = &eventHandle->eventPrms;
    if(NULL_PTR != eventPrms->masterEventHandle)
    {
        /* Shared event. Get the master handle */
        masterEventHandle = eventPrms->masterEventHandle;
    }

    Udma_assert(drvHandle, drvHandle->initPrms.osalPrms.lockMutex != (Udma_OsalMutexLockFxn) NULL_PTR);
    drvHandle->initPrms.osalPrms.lockMutex(drvHandle->rmLock);

    for(i = 0U; i < UDMA_MAX_EVENTS_PER_VINTR; i++)
    {
        bitMask = ((uint64_t) 1U << i);
        if((masterEventHandle->vintrBitAllocFlag & bitMask) == 0U)
        {
            masterEventHandle->vintrBitAllocFlag |= bitMask;
            vintrBitNum = i;
            break;
        }
    }

    Udma_assert(drvHandle, drvHandle->initPrms.osalPrms.unlockMutex != (Udma_OsalMutexUnlockFxn) NULL_PTR);
    drvHandle->initPrms.osalPrms.unlockMutex(drvHandle->rmLock);

    return (vintrBitNum);
}

void Udma_rmFreeVintrBit(uint32_t vintrBitNum,
                         Udma_DrvHandle drvHandle,
                         Udma_EventHandle eventHandle)
{
    uint64_t                bitMask;
    Udma_EventHandle        masterEventHandle;
    const Udma_EventPrms   *eventPrms;

    masterEventHandle = eventHandle;
    eventPrms = &eventHandle->eventPrms;
    if(NULL_PTR != eventPrms->masterEventHandle)
    {
        /* Shared event. Get the master handle */
        masterEventHandle = eventPrms->masterEventHandle;
    }

    Udma_assert(drvHandle, drvHandle->initPrms.osalPrms.lockMutex != (Udma_OsalMutexLockFxn) NULL_PTR);
    drvHandle->initPrms.osalPrms.lockMutex(drvHandle->rmLock);

    Udma_assert(drvHandle, vintrBitNum < UDMA_MAX_EVENTS_PER_VINTR);
    bitMask = ((uint64_t) 1U << vintrBitNum);
    Udma_assert(drvHandle,
        (masterEventHandle->vintrBitAllocFlag & bitMask) == bitMask);
    masterEventHandle->vintrBitAllocFlag &= ~bitMask;

    Udma_assert(drvHandle, drvHandle->initPrms.osalPrms.unlockMutex != (Udma_OsalMutexUnlockFxn) NULL_PTR);
    drvHandle->initPrms.osalPrms.unlockMutex(drvHandle->rmLock);

    return;
}

uint32_t Udma_rmAllocIrIntr(uint32_t preferredIrIntrNum,
                              Udma_DrvHandle drvHandle)
{
    uint32_t            i, offset, bitPos, bitMask;
    uint32_t            irIntrNum = UDMA_INTR_INVALID;
    Udma_RmInitPrms    *rmInitPrms = &drvHandle->initPrms.rmInitPrms;

    Udma_assert(drvHandle, drvHandle->initPrms.osalPrms.lockMutex != (Udma_OsalMutexLockFxn) NULL_PTR);
    drvHandle->initPrms.osalPrms.lockMutex(drvHandle->rmLock);

    if(UDMA_CORE_INTR_ANY == preferredIrIntrNum)
    {
        /* Search and allocate from pool */
        for(i = 0U; i < rmInitPrms->numIrIntr; i++)
        {
            offset = i >> 5U;
            Udma_assert(drvHandle, offset < UDMA_RM_IR_INTR_ARR_SIZE);
            bitPos = i - (offset << 5U);
            bitMask = (uint32_t) 1U << bitPos;
            if((drvHandle->irIntrFlag[offset] & bitMask) == bitMask)
            {
                drvHandle->irIntrFlag[offset] &= ~bitMask;
                irIntrNum = i + rmInitPrms->startIrIntr;    /* Add start offset */
                break;
            }
        }
    }
    else
    {
        /* Allocate specific IR interrupt number if free */
        /* Array bound check */
        if((preferredIrIntrNum >= rmInitPrms->startIrIntr) &&
           (preferredIrIntrNum < (rmInitPrms->startIrIntr + rmInitPrms->numIrIntr)))
        {
            i = preferredIrIntrNum - rmInitPrms->startIrIntr;
            offset = i >> 5U;
            Udma_assert(drvHandle, offset < UDMA_RM_IR_INTR_ARR_SIZE);
            bitPos = i - (offset << 5U);
            bitMask = (uint32_t) 1U << bitPos;
            if((drvHandle->irIntrFlag[offset] & bitMask) == bitMask)
            {
                drvHandle->irIntrFlag[offset] &= ~bitMask;
                irIntrNum = preferredIrIntrNum;
            }
        }
    }

    Udma_assert(drvHandle, drvHandle->initPrms.osalPrms.unlockMutex != (Udma_OsalMutexUnlockFxn) NULL_PTR);
    drvHandle->initPrms.osalPrms.unlockMutex(drvHandle->rmLock);

    return (irIntrNum);
}

void Udma_rmFreeIrIntr(uint32_t irIntrNum, Udma_DrvHandle drvHandle)
{
    uint32_t            i, offset, bitPos, bitMask;
    Udma_RmInitPrms    *rmInitPrms = &drvHandle->initPrms.rmInitPrms;

    Udma_assert(drvHandle,
        irIntrNum < (rmInitPrms->startIrIntr + rmInitPrms->numIrIntr));
    Udma_assert(drvHandle, irIntrNum >= rmInitPrms->startIrIntr);

    Udma_assert(drvHandle, drvHandle->initPrms.osalPrms.lockMutex != (Udma_OsalMutexLockFxn) NULL_PTR);
    drvHandle->initPrms.osalPrms.lockMutex(drvHandle->rmLock);

    i = irIntrNum - rmInitPrms->startIrIntr;
    offset = i >> 5U;
    Udma_assert(drvHandle, offset < UDMA_RM_IR_INTR_ARR_SIZE);
    bitPos = i - (offset << 5U);
    bitMask = (uint32_t) 1U << bitPos;
    Udma_assert(drvHandle, (drvHandle->irIntrFlag[offset] & bitMask) == 0U);
    drvHandle->irIntrFlag[offset] |= bitMask;

    Udma_assert(drvHandle, drvHandle->initPrms.osalPrms.unlockMutex != (Udma_OsalMutexUnlockFxn) NULL_PTR);
    drvHandle->initPrms.osalPrms.unlockMutex(drvHandle->rmLock);

    return;
}

uint16_t Udma_rmAllocRingMon(Udma_DrvHandle drvHandle)
{
    uint16_t            i, offset, ringMonNum = UDMA_RING_MON_INVALID, temp;
    uint32_t            bitPos, bitMask;
    Udma_RmInitPrms    *rmInitPrms = &drvHandle->initPrms.rmInitPrms;

    Udma_assert(drvHandle, drvHandle->initPrms.osalPrms.lockMutex != (Udma_OsalMutexLockFxn) NULL_PTR);
    drvHandle->initPrms.osalPrms.lockMutex(drvHandle->rmLock);

    for(i = 0U; i < rmInitPrms->numRingMon; i++)
    {
        offset = i >> 5U;
        Udma_assert(drvHandle, offset < UDMA_RM_RING_MON_ARR_SIZE);
        temp = i - (offset << 5U);
        bitPos = (uint32_t) temp;
        bitMask = (uint32_t) 1U << bitPos;
        if((drvHandle->ringMonFlag[offset] & bitMask) == bitMask)
        {
            drvHandle->ringMonFlag[offset] &= ~bitMask;
            ringMonNum = (uint16_t)rmInitPrms->startRingMon;  /* Add start offset */
            ringMonNum += i;
            break;
        }
    }

    Udma_assert(drvHandle, drvHandle->initPrms.osalPrms.unlockMutex != (Udma_OsalMutexUnlockFxn) NULL_PTR);
    drvHandle->initPrms.osalPrms.unlockMutex(drvHandle->rmLock);

    return (ringMonNum);
}

void Udma_rmFreeRingMon(uint16_t ringMonNum, Udma_DrvHandle drvHandle)
{
    uint32_t            i, offset, bitPos, bitMask;
    Udma_RmInitPrms    *rmInitPrms = &drvHandle->initPrms.rmInitPrms;
    uint32_t            ringMonOffset = rmInitPrms->startRingMon;

    if(ringMonNum >= ringMonOffset)
    {
        Udma_assert(drvHandle, drvHandle->initPrms.osalPrms.lockMutex != (Udma_OsalMutexLockFxn) NULL_PTR);
        drvHandle->initPrms.osalPrms.lockMutex(drvHandle->rmLock);

        i = ringMonNum - ringMonOffset;
        offset = i >> 5U;
        Udma_assert(drvHandle, offset < UDMA_RM_RING_MON_ARR_SIZE);
        bitPos = i - (offset << 5U);
        bitMask = (uint32_t) 1U << bitPos;
        Udma_assert(drvHandle, (drvHandle->ringMonFlag[offset] & bitMask) == 0U);
        drvHandle->ringMonFlag[offset] |= bitMask;

        Udma_assert(drvHandle, drvHandle->initPrms.osalPrms.unlockMutex != (Udma_OsalMutexUnlockFxn) NULL_PTR);
        drvHandle->initPrms.osalPrms.unlockMutex(drvHandle->rmLock);
    }

    return;
}

uint32_t Udma_rmTranslateIrOutput(Udma_DrvHandle drvHandle, uint32_t irIntrNum)
{
    uint32_t    coreIntrNum = UDMA_INTR_INVALID;

    if(drvHandle->instType != UDMA_INST_TYPE_NORMAL)
    {
        /* In case of devices like AM64x, where there are no Interrupt Routers,
         * irIntrNum refers to coreIntrNum number itself. */
        coreIntrNum = irIntrNum;
    }
    else
    {
#if (UDMA_SOC_CFG_UDMAP_PRESENT == 1)
#if defined (_TMS320C6X)
        /* For C66x Sciclient translates NAVSS IR Idx to corresponding C66 IR Idx,
         * Not the Core Interrupt Idx. */
        Udma_RmInitPrms    *rmInitPrms = &drvHandle->initPrms.rmInitPrms;
        
        coreIntrNum = irIntrNum - rmInitPrms->startIrIntr;
        coreIntrNum += rmInitPrms->startC66xCoreIntr;
#else
        int32_t  retVal = UDMA_SOK;
        uint16_t dstId = 0U;

        retVal = Sciclient_rmIrqTranslateIrOutput(drvHandle->devIdIr,
                                                irIntrNum,
                                                drvHandle->devIdCore,
                                                &dstId); 
        if(UDMA_SOK == retVal)
        {
            coreIntrNum = dstId;
        }
#endif
#endif
    }

    return (coreIntrNum);
}

uint32_t Udma_rmTranslateCoreIntrInput(Udma_DrvHandle drvHandle, uint32_t coreIntrNum)
{
    uint32_t    irIntrNum = UDMA_INTR_INVALID;

    if(drvHandle->instType != UDMA_INST_TYPE_NORMAL)
    {
        /* In case of devices like AM64x, where there are no Interrupt Routers,
         * coreIntrNum refers to irIntrNum number itself. */
        irIntrNum = coreIntrNum;
    }
    else
    {
#if (UDMA_SOC_CFG_UDMAP_PRESENT == 1)
#if defined (_TMS320C6X)
        /* For C66x Sciclient expects C66 IR Idx to translate to NAVSS IR Idx.
         * Not the Core Interrupt Idx. */
        Udma_RmInitPrms    *rmInitPrms = &drvHandle->initPrms.rmInitPrms;

        irIntrNum = coreIntrNum - rmInitPrms->startC66xCoreIntr;
        irIntrNum += rmInitPrms->startIrIntr;
#else
        int32_t  retVal = UDMA_SOK;
        uint16_t irId = 0U;
        
        retVal = Sciclient_rmIrqTranslateIrqInput(drvHandle->devIdCore,
                                                  coreIntrNum,
                                                  drvHandle->devIdIr,
                                                  &irId); 
        if(UDMA_SOK == retVal)
        {
            irIntrNum = irId;
        } 
#endif
#endif
    }

    return (irIntrNum);
} 

int32_t Udma_rmGetSciclientDefaultBoardCfgRmRange(const Udma_RmDefBoardCfgPrms *rmDefBoardCfgPrms,
                                                  struct tisci_msg_rm_get_resource_range_resp *res,
                                                  uint32_t *splitResFlag)
{
    int32_t                                     retVal = UDMA_SOK;
    struct tisci_msg_rm_get_resource_range_req  req = {0};

    req.type           = rmDefBoardCfgPrms->sciclientReqType;
    req.subtype        = rmDefBoardCfgPrms->sciclientReqSubtype;
    req.secondary_host = rmDefBoardCfgPrms->sciclientSecHost;

    res->range_num       = 0;
    res->range_start     = 0;
    res->range_num_sec   = 0;
    res->range_start_sec = 0;

    /* Get resource number range */
    retVal =  Sciclient_rmGetResourceRange(
                &req,
                res,
                UDMA_SCICLIENT_TIMEOUT);
    if((CSL_PASS != retVal) || 
       ((res->range_num == 0) && (res->range_num_sec == 0)))
    {
        /* If range_num and range_num_sec = 0 (no entry for the core),
         * There is no reservation for the current core.
         * In this case, Try with HOST_ID_ALL*/
        req.secondary_host = TISCI_HOST_ID_ALL;

        retVal = Sciclient_rmGetResourceRange(
                    &req,
                    res,
                    UDMA_SCICLIENT_TIMEOUT);
        if((CSL_PASS == retVal) && (res->range_num != 0)) 
        {
            /* If range_num != 0, 
             * ie, When using TISCI_HOST_ID_ALL entry, 
             * Set the split resource flag, if passed.
             * (no need to check for range_num_sec, 
             *  since there will only be single entry for TISCI_HOST_ID_ALL) */
             if(NULL_PTR != splitResFlag)
            { 
                *splitResFlag = 1;
            }
        }
    }
    
    return (retVal);
}

int32_t Udma_rmSetSharedResRmInitPrms(const Udma_RmSharedResPrms *rmSharedResPrms,
                                      uint32_t  instId,
                                      uint32_t  rangeStart,
                                      uint32_t  rangeTotalNum,
                                      uint32_t *start,
                                      uint32_t *num)
{
    int32_t     retVal = UDMA_SOK;
    uint32_t    i;
    uint32_t    sumInstShare = 0U;
    uint32_t    instFinalShare[UDMA_RM_SHARED_RES_MAX_INST];
    uint32_t    splitCnt = 0U;
    uint32_t    splitShare;
    uint32_t    splitMod;
    uint32_t    udmaResvCnt;
    uint32_t    numInst = rmSharedResPrms->numInst; 
    uint32_t    minReq = rmSharedResPrms->minReq;
    uint32_t    startResrvCnt = rmSharedResPrms->startResrvCnt;
    uint32_t    endResrvCnt = rmSharedResPrms->endResrvCnt;
    uint32_t    numUnresvRes = (rangeTotalNum > (startResrvCnt + endResrvCnt)) ?
                                    (rangeTotalNum - (startResrvCnt + endResrvCnt)) : 0U;

    /* Error Check */
    if(NULL_PTR == rmSharedResPrms)
    {
        retVal = UDMA_EBADARGS;
    }
    if(UDMA_SOK == retVal)
    {
        /* Check for minimum requirement with total usable*/
        if((numInst > UDMA_RM_SHARED_RES_MAX_INST) ||
           (numUnresvRes < (numInst * minReq)))
        {
            retVal = UDMA_EBADARGS;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Populate share for each instance; Get sum of shares for validation */
        for(i = 0U;i < numInst; i++)
        {
            if(rmSharedResPrms->instShare[i] == UDMA_RM_SHARED_RES_CNT_MIN)
            {
                sumInstShare += minReq;
                instFinalShare[i] = minReq;
            }
            else if(rmSharedResPrms->instShare[i] == UDMA_RM_SHARED_RES_CNT_REST)
            {
                sumInstShare += minReq;
                splitCnt++;
            }
            else
            {
                sumInstShare += rmSharedResPrms->instShare[i];
                instFinalShare[i] = rmSharedResPrms->instShare[i];
            }
        }
        /* Check for sum of share with total usable */
        if(numUnresvRes < sumInstShare)
        {
            retVal = UDMA_EINVALID_PARAMS;
        }
    }

    if(UDMA_SOK == retVal)
    {
        if(splitCnt > 0U)
        {
            /* Populate for #UDMA_RM_SHARED_RES_CNT_REST with rest of the available resources */
            udmaResvCnt = sumInstShare - (splitCnt * minReq);
            splitShare = (numUnresvRes - udmaResvCnt) / splitCnt;
            splitMod = (numUnresvRes - udmaResvCnt) % splitCnt;
            for(i = 0U; i < numInst; i++)
            {
                if(rmSharedResPrms->instShare[i] == UDMA_RM_SHARED_RES_CNT_REST)
                {
                    instFinalShare[i] = splitShare + splitMod;
                    splitMod = 0U;
                }
            }
        }

        *num = instFinalShare[instId];
        /* Calculate the start for requested instance */
        *start = rangeStart + startResrvCnt;
        for(i = 0U; i < instId; i++)
        {
            *start += instFinalShare[i];
        }
    }
    
    return (retVal);
}

static int32_t Udma_rmCheckResLeak(Udma_DrvHandle drvHandle,
                                   const uint32_t *allocFlag,
                                   uint32_t numRes,
                                   uint32_t arrSize)
{
    int32_t     retVal = UDMA_SOK;
    uint32_t    i, offset, bitMask;

    offset = 0;
    i = numRes;
    while(i > 0U)
    {
        if(i >= (uint32_t)32U)          /* 32 resource per array element */
        {
            bitMask = (uint32_t) 0xFFFFFFFFU;
            i -= 32U;
        }
        else
        {
            bitMask = ((uint32_t)1U << i) - ((uint32_t)1U);
            i = 0U;
        }

        Udma_assert(drvHandle, offset < arrSize);
        if((allocFlag[offset] & bitMask) != bitMask)
        {
            retVal = UDMA_EFAIL;        /* Some resources not freed */
            break;
        }
        offset++;
    }

    return (retVal);
}
