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
 *  \file udma_rmcfg_common.c
 *
 *  \brief File containing the UDMA driver RM init parameters configuration 
 *  functions common to J721E/J7200/AM65XX.
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

/* None */

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

int32_t UdmaRmInitPrms_init(uint32_t instId, Udma_RmInitPrms *rmInitPrms)
{
    const Udma_RmDefBoardCfgPrms                *rmDefBoardCfgPrms;
    int32_t                                      retVal = UDMA_SOK;
    struct tisci_msg_rm_get_resource_range_resp  res = {0};
    uint32_t                                     splitResFlag;
    /* Offset Params - TODO Remove if feasible */
    uint32_t numtTxCh;
    uint32_t numRxCh;
    uint32_t numExtCh;

    /* Error check */
    if(NULL_PTR == rmInitPrms)
    {
        retVal = UDMA_EBADARGS;
    }

    if(UDMA_SOK == retVal)
    {
        rmDefBoardCfgPrms = Udma_rmGetDefBoardCfgPrms(instId);
    
        /* Assign offset Params - TODO Remove if feasible */
        if(UDMA_INST_ID_MCU_0 == instId)
        {
            numtTxCh    = CSL_NAVSS_MCU_UDMAP_NUM_TX_CHANS;    
            numRxCh     = CSL_NAVSS_MCU_UDMAP_NUM_RX_CHANS;
            numExtCh    = CSL_NAVSS_MCU_UDMAP_NUM_EXT_CHANS;
        }
        else
        {
            numtTxCh    = CSL_NAVSS_MAIN_UDMAP_NUM_TX_CHANS;    
            numRxCh     = CSL_NAVSS_MAIN_UDMAP_NUM_RX_CHANS;
            numExtCh    = CSL_NAVSS_MAIN_UDMAP_NUM_EXT_CHANS;
        }

        memset(rmInitPrms, 0, sizeof(*rmInitPrms));

        /* Ultra High Capacity Block Copy and TX Channels */
        retVal += Udma_rmGetSciclientDefaultBoardCfgRmRange(&rmDefBoardCfgPrms[UDMA_RM_RES_ID_TX_UHC], &res, NULL);
        /* Primary for Block Copy Channel */
        rmInitPrms->startBlkCopyUhcCh   = res.range_start;
        rmInitPrms->numBlkCopyUhcCh     = res.range_num;
        /* Secondary for TX Channel */
        rmInitPrms->startTxUhcCh        = res.range_start_sec;
        rmInitPrms->numTxUhcCh          = res.range_num_sec;

        /* High Capacity Block Copy and TX Channels */
        retVal += Udma_rmGetSciclientDefaultBoardCfgRmRange(&rmDefBoardCfgPrms[UDMA_RM_RES_ID_TX_HC], &res, NULL);
        /* Primary for Block Copy Channel */
        rmInitPrms->startBlkCopyHcCh    = res.range_start;
        rmInitPrms->numBlkCopyHcCh      = res.range_num;
        /* Secondary for TX Channel */
        rmInitPrms->startTxHcCh         = res.range_start_sec;
        rmInitPrms->numTxHcCh           = res.range_num_sec;

        /* Normal Capacity Block Copy and TX Channels */
        retVal += Udma_rmGetSciclientDefaultBoardCfgRmRange(&rmDefBoardCfgPrms[UDMA_RM_RES_ID_TX], &res, NULL);
        /* Primary for Block Copy Channel */
        rmInitPrms->startBlkCopyCh      = res.range_start;
        rmInitPrms->numBlkCopyCh        = res.range_num;
        /* Secondary for TX Channel */
        rmInitPrms->startTxCh           = res.range_start_sec;
        rmInitPrms->numTxCh             = res.range_num_sec;

        /* Ultra High Capacity RX Channels */
        retVal += Udma_rmGetSciclientDefaultBoardCfgRmRange(&rmDefBoardCfgPrms[UDMA_RM_RES_ID_RX_UHC], &res, NULL);
        /* Secondary for RX Channel (Primary for Block Copy Channel) */
        rmInitPrms->startRxUhcCh        = res.range_start_sec;
        rmInitPrms->numRxUhcCh          = res.range_num_sec;

        /* High Capacity RX Channels */
        retVal += Udma_rmGetSciclientDefaultBoardCfgRmRange(&rmDefBoardCfgPrms[UDMA_RM_RES_ID_RX_HC], &res, NULL);
        /* Secondary for RX Channel (Primary for Block Copy Channel) */
        rmInitPrms->startRxHcCh         = res.range_start_sec;
        rmInitPrms->numRxHcCh           = res.range_num_sec;

        /* Normal Capacity RX Channels */
        retVal += Udma_rmGetSciclientDefaultBoardCfgRmRange(&rmDefBoardCfgPrms[UDMA_RM_RES_ID_RX], &res, NULL);
        /* Secondary for RX Channel (Primary for Block Copy Channel) */
        rmInitPrms->startRxCh           = res.range_start_sec;
        rmInitPrms->numRxCh             = res.range_num_sec;

#if (UDMA_NUM_UTC_INSTANCE > 0)
        /* UTC - Extended Channels (MSMC_DRU/VPAC_TC0/VPAC_TC1/DMPAC) */
        retVal += Udma_rmGetSciclientDefaultBoardCfgRmRange(&rmDefBoardCfgPrms[UDMA_RM_RES_ID_UTC], &res, NULL);
        /* Primary for MSMC_DRU Channel */
        rmInitPrms->startUtcCh[UDMA_UTC_ID_MSMC_DRU0]       = res.range_start;
        rmInitPrms->numUtcCh[UDMA_UTC_ID_MSMC_DRU0]         = res.range_num;
        /* Sub offset. TODO: Remove offset if feasible */
        if(rmInitPrms->startUtcCh[UDMA_UTC_ID_MSMC_DRU0] >= numtTxCh)
        {
            rmInitPrms->startUtcCh[UDMA_UTC_ID_MSMC_DRU0]  -= numtTxCh; 
        }
#if (UDMA_NUM_UTC_INSTANCE > 1)
        /* Secondary for VPAC_TC0+VPAC_TC1+DMPAC Channels */
        /* Here the assumption taken to split the external channels for HWA is that,
         * all similar type of channels (VPAC_TC0/VPAC_TC1/DMPAC) will be reserved to same core */
        uint32_t vpac0Start = CSL_NAVSS_MAIN_UDMAP_NUM_TX_CHANS + UDMA_UTC_NUM_CH_DRU0;
        uint32_t vpac1Start = vpac0Start + UDMA_UTC_NUM_CH_VPAC_TC0;
        uint32_t dmpacStart = vpac1Start + UDMA_UTC_NUM_CH_VPAC_TC1;

        if((res.range_start_sec == vpac0Start) &&
           (res.range_start_sec + res.range_num_sec >= vpac0Start + UDMA_UTC_NUM_CH_VPAC_TC0))
        {
            rmInitPrms->startUtcCh[UDMA_UTC_ID_VPAC_TC0]    = UDMA_UTC_START_CH_VPAC_TC0;
            rmInitPrms->numUtcCh[UDMA_UTC_ID_VPAC_TC0]      = UDMA_UTC_NUM_CH_VPAC_TC0;
        }
        if((res.range_start_sec <= vpac1Start) &&
           (res.range_start_sec + res.range_num_sec >= vpac1Start + UDMA_UTC_NUM_CH_VPAC_TC1))
        {
            rmInitPrms->startUtcCh[UDMA_UTC_ID_VPAC_TC1]    = UDMA_UTC_START_CH_VPAC_TC1;
            rmInitPrms->numUtcCh[UDMA_UTC_ID_VPAC_TC1]      = UDMA_UTC_NUM_CH_VPAC_TC1;
        }
        if((res.range_start_sec <= dmpacStart) &&
           (res.range_start_sec + res.range_num_sec == dmpacStart + UDMA_UTC_NUM_CH_DMPAC_TC0))
        {
            rmInitPrms->startUtcCh[UDMA_UTC_ID_DMPAC_TC0]   = UDMA_UTC_START_CH_DMPAC_TC0;
            rmInitPrms->numUtcCh[UDMA_UTC_ID_DMPAC_TC0]     = UDMA_UTC_NUM_CH_DMPAC_TC0;
        }
#endif
#endif

        /* Free Flows */
        splitResFlag = 0U;
        retVal += Udma_rmGetSciclientDefaultBoardCfgRmRange(&rmDefBoardCfgPrms[UDMA_RM_RES_ID_RX_FLOW], &res, &splitResFlag);
        if(0U == splitResFlag)
        {
            rmInitPrms->startFreeFlow                           = res.range_start;
            rmInitPrms->numFreeFlow                             = res.range_num;
        }
        else
        {
            retVal += Udma_rmSetSharedResRmInitPrms(Udma_rmGetSharedResPrms(UDMA_RM_RES_ID_RX_FLOW),
                                                    Udma_getCoreId(),
                                                    res.range_start,
                                                    res.range_num,
                                                    &rmInitPrms->startFreeFlow,
                                                    &rmInitPrms->numFreeFlow);
        }
        /* Sub offset. TODO: Remove offset if feasible */
        if(rmInitPrms->startFreeFlow >= numRxCh)
        {
            rmInitPrms->startFreeFlow                      -= numRxCh; 
        }

        /* Free Rings */
        retVal += Udma_rmGetSciclientDefaultBoardCfgRmRange(&rmDefBoardCfgPrms[UDMA_RM_RES_ID_RING], &res, NULL);
        rmInitPrms->startFreeRing                           = res.range_start;
        rmInitPrms->numFreeRing                             = res.range_num;
        /* Sub offset. TODO: Remove offset if feasible */
        if(rmInitPrms->startFreeRing >= (numtTxCh + numRxCh + numExtCh))
        {
            rmInitPrms->startFreeRing                      -= (numtTxCh + numRxCh + numExtCh); 
        }

        /* Global Event */
        retVal += Udma_rmGetSciclientDefaultBoardCfgRmRange(&rmDefBoardCfgPrms[UDMA_RM_RES_ID_GLOBAL_EVENT], &res, NULL);
        retVal += Udma_rmSetSharedResRmInitPrms(Udma_rmGetSharedResPrms(UDMA_RM_RES_ID_GLOBAL_EVENT),
                                               instId,
                                               res.range_start,
                                               res.range_num,
                                               &rmInitPrms->startGlobalEvent,
                                               &rmInitPrms->numGlobalEvent);
        if(TISCI_DEV_MCU_NAVSS0_UDMASS_INTA_0 == rmDefBoardCfgPrms[UDMA_RM_RES_ID_GLOBAL_EVENT].sciclientReqType)
        {
            /* Substract Offset */  
            rmInitPrms->startGlobalEvent                   -=  CSL_NAVSS_GEM_MCU_UDMA_INTA0_SEVI_OFFSET;  
        }

        /* Virtual Interrupts */
        retVal += Udma_rmGetSciclientDefaultBoardCfgRmRange(&rmDefBoardCfgPrms[UDMA_RM_RES_ID_VINTR], &res, NULL);
        retVal += Udma_rmSetSharedResRmInitPrms(Udma_rmGetSharedResPrms(UDMA_RM_RES_ID_VINTR),
                                               instId,
                                               res.range_start,
                                               res.range_num,
                                               &rmInitPrms->startVintr,
                                               &rmInitPrms->numVintr);

        /* Interrupt Router Interrupts */
        retVal += Udma_rmGetSciclientDefaultBoardCfgRmRange(&rmDefBoardCfgPrms[UDMA_RM_RES_ID_IR_INTR], &res, NULL);
        retVal += Udma_rmSetSharedResRmInitPrms(Udma_rmGetSharedResPrms(UDMA_RM_RES_ID_IR_INTR),
                                               instId,
                                               res.range_start,
                                               res.range_num,
                                               &rmInitPrms->startIrIntr,
                                               &rmInitPrms->numIrIntr);
    #if defined (__C7100__) || defined (_TMS320C6X)
    #if defined (__C7100__)
        /* Start C7x Core Interrupt */
        rmInitPrms->startC7xCoreIntr                        = UDMA_C7X_CORE_INTR_OFFSET;
    #endif
    #if defined (_TMS320C6X)
        /* Start C6xx Core Interrupt */
        rmInitPrms->startC66xCoreIntr                       = UDMA_C66X_CORE_INTR_OFFSET;
    #endif
        if(UDMA_INST_ID_MCU_0 == instId)
        {
            uint32_t offset = 0U;
            /* Add the no. of IR Interrupts reserved for C7x in Main NAVSS Instance */  
            retVal += Udma_rmSetSharedResRmInitPrms(Udma_rmGetSharedResPrms(UDMA_RM_RES_ID_IR_INTR),
                                                   UDMA_INST_ID_MAIN_0,
                                                   res.range_start,
                                                   res.range_num,
                                                   NULL,
                                                   &offset);
        #if defined (__C7100__)
            rmInitPrms->startC7xCoreIntr                   +=  offset;  
        #endif
        #if defined (_TMS320C6X)
            rmInitPrms->startC66xCoreIntr                  +=  offset;  
        #endif
        }
    #endif

        /* Proxy */
        retVal += Udma_rmGetSciclientDefaultBoardCfgRmRange(&rmDefBoardCfgPrms[UDMA_RM_RES_ID_PROXY], &res, NULL);
        if(0U != res.range_num)
        {
            rmInitPrms->proxyThreadNum                      = res.range_start;
            rmInitPrms->startProxy                          = res.range_start + 1U;
            rmInitPrms->numProxy                            = res.range_num - 1U;
        }

        /* Ring Monitors */
        retVal += Udma_rmGetSciclientDefaultBoardCfgRmRange(&rmDefBoardCfgPrms[UDMA_RM_RES_ID_RING_MON], &res, NULL);
        rmInitPrms->startRingMon                            = res.range_start;
        rmInitPrms->numRingMon                              = res.range_num;
    }

    return (retVal);
}
