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
 *  \file udma_ring.c
 *
 *  \brief File containing the UDMA driver ring related APIs.
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <ti/drv/udma/src/udma_priv.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/** \brief Max number of door bell ring that can be performed at one go */
#define UDMA_RING_MAX_DB_RING_CNT       (127U)

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

void Udma_ringSetCfgLcdma(Udma_DrvHandle drvHandle,
                                      Udma_RingHandle ringHandle,
                                      const Udma_RingPrms *ringPrms)
{
    uint32_t            addrHi, addrLo /*, elemSize*/;
    CSL_LcdmaRingaccRingCfg *lcdmaRingCfg;

    /* Configure ring object */
    ringHandle->drvHandle = drvHandle;
    Udma_assert(drvHandle, drvHandle->lcdmaRaRegs.pRingCfgRegs != NULL_PTR);
    Udma_assert(drvHandle, drvHandle->lcdmaRaRegs.pRingRtRegs != NULL_PTR);
    Udma_assert(drvHandle, ringHandle->ringNum < 100U);
    ringHandle->pLcdmaCfgRegs =
        &drvHandle->lcdmaRaRegs.pRingCfgRegs->RING[ringHandle->ringNum];
    ringHandle->pLcdmaRtRegs  =
        &drvHandle->lcdmaRaRegs.pRingRtRegs->RING[ringHandle->ringNum];

    lcdmaRingCfg = &ringHandle->lcdmaCfg;
    if(NULL_PTR != ringPrms)
    {
        lcdmaRingCfg->virtBase       = (void *) ringPrms->ringMem;
        lcdmaRingCfg->physBase       =
            Udma_virtToPhyFxn(ringPrms->ringMem, drvHandle, (Udma_ChHandle) NULL_PTR);
        lcdmaRingCfg->mode           = ringPrms->mode;
        lcdmaRingCfg->elCnt          = ringPrms->elemCnt;
        /* CSL expects ring size in bytes */
        lcdmaRingCfg->elSz           = ((uint32_t) 1U << (ringPrms->elemSize + 2U));
    }
    else
    {
        /* Init CSL ring object */
        lcdmaRingCfg = &ringHandle->lcdmaCfg;
        addrHi = CSL_REG32_FEXT(&ringHandle->pLcdmaCfgRegs->BA_HI, LCDMA_RINGACC_RING_CFG_RING_BA_HI_ADDR_HI);
        addrLo = CSL_REG32_FEXT(&ringHandle->pLcdmaCfgRegs->BA_LO, LCDMA_RINGACC_RING_CFG_RING_BA_LO_ADDR_LO);
        lcdmaRingCfg->physBase       = (uint64_t)((((uint64_t) addrHi) << 32UL) |
                                            ((uint64_t) addrLo));
        lcdmaRingCfg->virtBase       = Udma_phyToVirtFxn(lcdmaRingCfg->physBase, drvHandle, (Udma_ChHandle) NULL_PTR);
        lcdmaRingCfg->mode           = CSL_REG32_FEXT(&ringHandle->pLcdmaCfgRegs->SIZE, LCDMA_RINGACC_RING_CFG_RING_SIZE_QMODE);
        lcdmaRingCfg->elCnt          = CSL_REG32_FEXT(&ringHandle->pLcdmaCfgRegs->SIZE, LCDMA_RINGACC_RING_CFG_RING_SIZE_ELCNT);
        /* CSL expects ring size in bytes; ring_elsize for AM64x is hardcoded as 1=8bytes*/
        lcdmaRingCfg->elSz           = (uint32_t) 8U;
    }
    
    lcdmaRingCfg->credChkSecure  = 0U;
    lcdmaRingCfg->credSecure     = 0U;
    lcdmaRingCfg->credPriv       = 0U;
    lcdmaRingCfg->credPrivId     = CSL_LCDMA_RINGACC_CRED_PASSTHRU;
    lcdmaRingCfg->credVirtId     = CSL_LCDMA_RINGACC_CRED_PASSTHRU;
    CSL_lcdma_ringaccInitRingObj(ringHandle->ringNum, lcdmaRingCfg);

#if (UDMA_SOC_CFG_PROXY_PRESENT == 1)
    ringHandle->proxyAddr =
        CSL_proxyGetDataAddr(
            &drvHandle->proxyCfg,
            drvHandle->proxyTargetNumRing,
            drvHandle->initPrms.rmInitPrms.proxyThreadNum,
            lcdmaRingCfg->elSz);
#endif

    return;
}

void Udma_ringHandleClearRegsLcdma(Udma_RingHandle ringHandle)
{
    ringHandle->pLcdmaCfgRegs   = (volatile CSL_lcdma_ringacc_ring_cfgRegs_RING *) NULL_PTR;
    ringHandle->pLcdmaRtRegs    = (volatile CSL_lcdma_ringacc_ringrtRegs_ring *) NULL_PTR;
}

int32_t Udma_ringQueueRawLcdma(Udma_DrvHandle  drvHandle, Udma_RingHandle ringHandle, uint64_t phyDescMem)
{
    int32_t         retVal = UDMA_SOK;

    if(TISCI_MSG_VALUE_RM_RING_MODE_RING == ringHandle->lcdmaCfg.mode)
    {
        /* Use direct memory access for RING mode */
        retVal = CSL_lcdma_ringaccPush64(
            &ringHandle->drvHandle->lcdmaRaRegs,
            &ringHandle->lcdmaCfg,
            (uint64_t) phyDescMem,
            &Udma_lcdmaRingaccMemOps);
    }
    else
    {
#if (UDMA_SOC_CFG_PROXY_PRESENT == 1)
        /* Use proxy for other modes, if available */
        retVal = Udma_ringProxyQueueRaw(ringHandle, drvHandle, phyDescMem);
#else
        retVal = CSL_lcdma_ringaccPush64x32(
            &ringHandle->drvHandle->lcdmaRaRegs,
            &ringHandle->lcdmaCfg,
            (uint64_t) phyDescMem,
            &Udma_lcdmaRingaccMemOps);
#endif
    }

    return (retVal);
}

int32_t Udma_ringDequeueRawLcdma(Udma_DrvHandle  drvHandle, Udma_RingHandle ringHandle, uint64_t *phyDescMem)
{
    int32_t         retVal = UDMA_SOK, cslRetVal;

    if(TISCI_MSG_VALUE_RM_RING_MODE_RING == ringHandle->lcdmaCfg.mode)
    {
        /* Use direct memory access for RING mode */
        cslRetVal = CSL_lcdma_ringaccPop64(
                &ringHandle->drvHandle->lcdmaRaRegs,
                &ringHandle->lcdmaCfg,
                phyDescMem,
                &Udma_lcdmaRingaccMemOps);
        if(0 != cslRetVal)
        {
            retVal = UDMA_ETIMEOUT;
        }
    }
    else
    {
#if (UDMA_SOC_CFG_PROXY_PRESENT == 1)
        /* Use proxy for other modes, if available */
        retVal = Udma_ringProxyDequeueRaw(ringHandle, drvHandle, phyDescMem);
#else
        cslRetVal = CSL_lcdma_ringaccPop64x32(
                &ringHandle->drvHandle->lcdmaRaRegs,
                &ringHandle->lcdmaCfg,
                phyDescMem,
                &Udma_lcdmaRingaccMemOps);
        if(0 != cslRetVal)
        {
            retVal = UDMA_ETIMEOUT;
        }
#endif
    }

    return (retVal);
}

int32_t Udma_ringFlushRawLcdma(Udma_DrvHandle drvHandle, Udma_RingHandle ringHandle, uint64_t *phyDescMem)
{
    int32_t         retVal = UDMA_SOK, cslRetVal;

    cslRetVal = CSL_lcdma_ringaccPop64x32(
            &ringHandle->drvHandle->lcdmaRaRegs,
            &ringHandle->lcdmaCfg,
            phyDescMem,
            &Udma_lcdmaRingaccMemOps);
    if(0 != cslRetVal)
    {
        retVal = UDMA_ETIMEOUT;
    }
    return (retVal);
}

void Udma_ringPrimeLcdma(Udma_RingHandle ringHandle, uint64_t phyDescMem)
{
    volatile uint64_t        *ringPtr;
    CSL_LcdmaRingaccRingCfg  *pLcdmaRing;
    uintptr_t                 tempPtr;

    pLcdmaRing = &ringHandle->lcdmaCfg;
    tempPtr = (uintptr_t)(pLcdmaRing->wrIdx * pLcdmaRing->elSz) +
              (uintptr_t)pLcdmaRing->virtBase;
    ringPtr = (volatile uint64_t *)(tempPtr);
    *ringPtr = phyDescMem;

    /* Book keeping */
    pLcdmaRing->wrIdx++;
    if(pLcdmaRing->wrIdx >= pLcdmaRing->elCnt)
    {
        pLcdmaRing->wrIdx = 0U;
    }
    pLcdmaRing->wrOcc++;

    return;
}

void Udma_ringSetForwardDoorBellLcdma(Udma_RingHandle ringHandle, int32_t count)
{
    uint32_t    regVal;
    int32_t     dbRingCnt, thisDbRingCnt;

    dbRingCnt = count;
    while(dbRingCnt != 0)
    {
        if(dbRingCnt < UDMA_RING_MAX_DB_RING_CNT)
        {
            thisDbRingCnt = dbRingCnt;
            regVal = CSL_FMK(LCDMA_RINGACC_RINGRT_RING_FDB_CNT, thisDbRingCnt);
        }
        else
        {
            thisDbRingCnt = UDMA_RING_MAX_DB_RING_CNT;
            regVal = CSL_FMK(LCDMA_RINGACC_RINGRT_RING_FDB_CNT, thisDbRingCnt);
        }
        CSL_REG32_WR(&ringHandle->pLcdmaRtRegs->FDB, regVal);
        dbRingCnt -= thisDbRingCnt;
    }

    return;
}
