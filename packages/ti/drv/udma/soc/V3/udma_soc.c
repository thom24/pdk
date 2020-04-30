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
 *  \file udma_soc.c
 *
 *  \brief File containing the UDMA driver SOC related configuration functions.
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


void Udma_initDrvHandle(Udma_DrvHandle drvHandle)
{
    uint32_t                 instId;
    CSL_BcdmaCfg    		 *pBcdmaRegs; 
    CSL_PktdmaCfg 	   	     *pPktdmaRegs; 
    CSL_LcdmaRingaccCfg      *pLcdmaRaRegs; 
    CSL_IntaggrCfg   	     *pIaRegs;

    instId = drvHandle->initPrms.instId;
    /*
     * BCDMA/PKTDMA config init
     */
    /* Init the config structure - one time step */
    if(UDMA_INST_ID_BCDMA_0 == instId)
    {
    	drvHandle->instType = UDMA_INST_TYPE_LCDMA_BCDMA;
    	pBcdmaRegs = &drvHandle->bcdmaRegs;
        pBcdmaRegs->pGenCfgRegs     = ((CSL_bcdma_gcfgRegs *) CSL_DMASS0_BCDMA_GCFG_BASE);
        pBcdmaRegs->pBcChanCfgRegs  = ((CSL_bcdma_bccfgRegs *) CSL_DMASS0_BCDMA_BCHAN_BASE);
        pBcdmaRegs->pTxChanCfgRegs  = ((CSL_bcdma_txccfgRegs *) CSL_DMASS0_BCDMA_TCHAN_BASE);
        pBcdmaRegs->pRxChanCfgRegs  = ((CSL_bcdma_rxccfgRegs *) CSL_DMASS0_BCDMA_RCHAN_BASE);
        pBcdmaRegs->pBcChanRtRegs   = ((CSL_bcdma_bcrtRegs *) CSL_DMASS0_BCDMA_BCHANRT_BASE);
        pBcdmaRegs->pTxChanRtRegs   = ((CSL_bcdma_txcrtRegs *) CSL_DMASS0_BCDMA_TCHANRT_BASE);
        pBcdmaRegs->pRxChanRtRegs   = ((CSL_bcdma_rxcrtRegs *) CSL_DMASS0_BCDMA_RCHANRT_BASE);
        drvHandle->trigGemOffset    = CSL_DMSS_GEM_BCDMA_TRIGGER_OFFSET;
        /* Fill other SOC specific parameters by reading from UDMA config
	     * registers */
	    CSL_bcdmaGetCfg(pBcdmaRegs);
	    
        pPktdmaRegs = &drvHandle->pktdmaRegs;
        memset(pPktdmaRegs, 0, sizeof(*pPktdmaRegs));
    }
    else
    {
    	drvHandle->instType = UDMA_INST_TYPE_LCDMA_PKTDMA;
    	pPktdmaRegs = &drvHandle->pktdmaRegs;
        pPktdmaRegs->pGenCfgRegs     = ((CSL_pktdma_gcfgRegs *) CSL_DMASS0_PKTDMA_GCFG_BASE);
        pPktdmaRegs->pRxFlowCfgRegs  = ((CSL_pktdma_rxfcfgRegs *) CSL_DMASS0_PKTDMA_RFLOW_BASE);
        pPktdmaRegs->pTxChanCfgRegs  = ((CSL_pktdma_txccfgRegs *) CSL_DMASS0_PKTDMA_TCHAN_BASE);
        pPktdmaRegs->pRxChanCfgRegs  = ((CSL_pktdma_rxccfgRegs *) CSL_DMASS0_PKTDMA_RCHAN_BASE);
        pPktdmaRegs->pTxChanRtRegs   = ((CSL_pktdma_txcrtRegs *) CSL_DMASS0_PKTDMA_TCHANRT_BASE);
        pPktdmaRegs->pRxChanRtRegs   = ((CSL_pktdma_rxcrtRegs *) CSL_DMASS0_PKTDMA_RCHANRT_BASE);
        drvHandle->trigGemOffset     = 0; 
        /* Fill other SOC specific parameters by reading from UDMA config
	     * registers */
	    CSL_pktdmaGetCfg(pPktdmaRegs);
        
        pBcdmaRegs = &drvHandle->bcdmaRegs;
        memset(pBcdmaRegs, 0, sizeof(*pBcdmaRegs));
    }
    
    /*
     * RA config init
     */
    drvHandle->raType = UDMA_RA_TYPE_LCDMA;
    pLcdmaRaRegs = &drvHandle->lcdmaRaRegs;
    if(UDMA_INST_ID_BCDMA_0 == instId)
    {
	    pLcdmaRaRegs->pRingCfgRegs   = (CSL_lcdma_ringacc_ring_cfgRegs *) CSL_DMASS0_BCDMA_RING_BASE; 
	    pLcdmaRaRegs->pRingRtRegs   = (CSL_lcdma_ringacc_ringrtRegs *) CSL_DMASS0_BCDMA_RINGRT_BASE;
	    pLcdmaRaRegs->pCredRegs    = (CSL_lcdma_ringacc_credRegs *) CSL_DMASS0_BCDMA_CRED_BASE; 
	    pLcdmaRaRegs->maxRings   = CSL_DMSS_BCDMA_NUM_BC_CHANS + CSL_DMSS_BCDMA_NUM_TX_CHANS + CSL_DMSS_BCDMA_NUM_RX_CHANS;	
    }
    else
    {
    	pLcdmaRaRegs->pRingCfgRegs   = (CSL_lcdma_ringacc_ring_cfgRegs *) CSL_DMASS0_PKTDMA_RING_BASE; 
	    pLcdmaRaRegs->pRingRtRegs   = (CSL_lcdma_ringacc_ringrtRegs *) CSL_DMASS0_PKTDMA_RINGRT_BASE;
	    pLcdmaRaRegs->pCredRegs    = (CSL_lcdma_ringacc_credRegs *) CSL_DMASS0_PKTDMA_CRED_BASE; 
	    pLcdmaRaRegs->maxRings   = CSL_DMSS_PKTDMA_NUM_TX_CHANS + CSL_DMSS_PKTDMA_NUM_RX_CHANS;	
    }

    /* IA config init */
    pIaRegs = &drvHandle->iaRegs;
    pIaRegs->pCfgRegs       = (CSL_intaggr_cfgRegs *) CSL_DMASS0_INTAGGR_CFG_BASE;
    pIaRegs->pImapRegs      = (CSL_intaggr_imapRegs *) CSL_DMASS0_INTAGGR_IMAP_BASE;
    pIaRegs->pIntrRegs      = (CSL_intaggr_intrRegs *) CSL_DMASS0_INTAGGR_INTR_BASE;
    pIaRegs->pL2gRegs       = (CSL_intaggr_l2gRegs *) CSL_DMASS0_INTAGGR_L2G_BASE;
    pIaRegs->pMcastRegs     = (CSL_intaggr_mcastRegs *) CSL_DMASS0_INTAGGR_MCAST_BASE;
    pIaRegs->pGcntCfgRegs   = (CSL_intaggr_gcntcfgRegs *) CSL_DMASS0_INTAGGR_GCNTCFG_BASE;
    pIaRegs->pGcntRtiRegs   = (CSL_intaggr_gcntrtiRegs *) CSL_DMASS0_INTAGGR_GCNTRTI_BASE;
    CSL_intaggrGetCfg(pIaRegs);

    drvHandle->iaGemOffset  = CSL_DMSS_GEM_INTA0_SEVI_OFFSET;
    drvHandle->devIdIa      = TISCI_DEV_DMASS0_INTAGGR_0;
    
#if defined (BUILD_MCU1_0)
    drvHandle->devIdCore    = TISCI_DEV_R5FSS0_CORE0;
#endif

#if defined (BUILD_MCU1_1)
    drvHandle->devIdCore    = TISCI_DEV_R5FSS0_CORE1;
#endif

#if defined (BUILD_MPU1_0)
    drvHandle->devIdCore    = TISCI_DEV_A53SS0_CORE_0;
#endif

#if defined (BUILD_MCU2_0)
    drvHandle->devIdCore    = TISCI_DEV_R5FSS1_CORE0;
#endif

#if defined (BUILD_MCU2_1)
    drvHandle->devIdCore    = TISCI_DEV_R5FSS1_CORE1;
#endif

#if defined (BUILD_M4F_0)
    drvHandle->devIdCore    = TISCI_DEV_MCU_M4FSS0_CORE0;
#endif

    /* Init other variables */
    drvHandle->maxProxy     = 0U; 
    drvHandle->maxRingMon   = CSL_DMSS_RINGACC_NUM_MONITORS;
    drvHandle->extChOffset  = 0U;
    drvHandle->srcIdRingIrq = drvHandle->devIdIa;
    if(UDMA_INST_ID_BCDMA_0 == instId)
    {
       /* DMSC differentiates block copy channel from split tr tx, if the index have an offset of 32.
        * The offset of 32 is chosen because it’s the next power of 2 above the max split tr tx channel (19).
        */ 
        drvHandle->blkCopyChOffset      = 32U; 
        drvHandle->txChOffset           = pBcdmaRegs->bcChanCnt;
	    drvHandle->rxChOffset   		= drvHandle->txChOffset + pBcdmaRegs->splitTxChanCnt;
       /* The srcIdx passed to Sciclient_rmIrqset API, will be ringNum + the corresponding following offset. 
        * So setting the offset as TISCI Start Idx - corresponding ringNum Offset (if any) */
        drvHandle->blkCopyRingIrqOffset = TISCI_BCDMA0_BC_RC_OES_IRQ_SRC_IDX_START; 
        drvHandle->txRingIrqOffset      = TISCI_BCDMA0_TX_RC_OES_IRQ_SRC_IDX_START - drvHandle->txChOffset;
        drvHandle->rxRingIrqOffset      = TISCI_BCDMA0_RX_RC_OES_IRQ_SRC_IDX_START - drvHandle->rxChOffset;
	    drvHandle->udmapSrcThreadOffset = CSL_PSILCFG_DMSS_BCDMA_STRM_PSILS_THREAD_OFFSET; 
	    drvHandle->udmapDestThreadOffset= CSL_PSILCFG_DMSS_BCDMA_STRM_PSILD_THREAD_OFFSET;
	    drvHandle->maxRings             = CSL_DMSS_BCDMA_NUM_BC_CHANS + CSL_DMSS_BCDMA_NUM_TX_CHANS + CSL_DMSS_BCDMA_NUM_RX_CHANS;
	    drvHandle->devIdRing            = TISCI_DEV_DMASS0_BCDMA_0;
	    drvHandle->devIdUdma        	= TISCI_DEV_DMASS0_BCDMA_0;
    }
    else
    {
        drvHandle->blkCopyChOffset      = 0U; /* Not used for PktDMA Instance */ 
        drvHandle->txChOffset           = 0U; /* Need to be updated, Since Special channels present */
	    drvHandle->rxChOffset   		= drvHandle->txChOffset + pPktdmaRegs->txChanCnt;	/* Need to be updated, Since Special channels present */
        drvHandle->blkCopyRingIrqOffset = 0U; 
        drvHandle->txRingIrqOffset      = TISCI_PKTDMA0_TX_FLOW_OES_IRQ_SRC_IDX_START; /* Need to be updated, Since Special channels present */
        drvHandle->rxRingIrqOffset      = TISCI_PKTDMA0_RX_FLOW_OES_IRQ_SRC_IDX_START - drvHandle->rxChOffset; /* Need to be updated, Since Special channels present */
	    drvHandle->udmapSrcThreadOffset = CSL_PSILCFG_DMSS_PKTDMA_STRM_PSILS_THREAD_OFFSET; 
	    drvHandle->udmapDestThreadOffset= CSL_PSILCFG_DMSS_PKTDMA_STRM_PSILD_THREAD_OFFSET;
	    drvHandle->maxRings             = CSL_DMSS_PKTDMA_NUM_TX_CHANS + CSL_DMSS_PKTDMA_NUM_RX_CHANS;
	    drvHandle->devIdRing            = TISCI_DEV_DMASS0_PKTDMA_0;
	    drvHandle->devIdUdma       		= TISCI_DEV_DMASS0_PKTDMA_0;
    }
    drvHandle->devIdPsil     = TISCI_DEV_DMASS0; 

    return;
}

void UdmaRmInitPrms_init(uint32_t instId, Udma_RmInitPrms *rmInitPrms)
{
    const Udma_RmInitPrms *rmInitDefaultCfg;

    if(NULL_PTR != rmInitPrms)
    {
        rmInitDefaultCfg = Udma_rmGetDefaultCfg(instId);
        (void) memcpy(rmInitPrms, rmInitDefaultCfg, sizeof (Udma_RmInitPrms));
    }

    return;
}

uint32_t Udma_getCoreId(void)
{
    uint32_t coreId;

#if defined (BUILD_MPU1_0)
    coreId = UDMA_CORE_ID_MPU1_0;
#endif
#if defined (BUILD_MCU2_0)
    coreId = UDMA_CORE_ID_MCU2_0;
#endif
#if defined (BUILD_MCU2_1)
    coreId = UDMA_CORE_ID_MCU2_1;
#endif
#if defined (BUILD_MCU1_0)
    coreId = UDMA_CORE_ID_MCU1_0;
#endif
#if defined (BUILD_MCU1_1)
    coreId = UDMA_CORE_ID_MCU1_1;
#endif
#if defined (BUILD_M4F_0)
    coreId = UDMA_CORE_ID_M4F_0;
#endif

    return (coreId);
}

uint32_t Udma_isCacheCoherent(void)
{
    uint32_t isCacheCoherent;

#if defined (BUILD_MPU1_0)
    isCacheCoherent = TRUE;
#else
    isCacheCoherent = FALSE;
#endif

    return (isCacheCoherent);
}
