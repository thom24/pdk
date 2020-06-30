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
 *  \file udma_test_soc_v1.c
 *
 *  \brief UDMA SOC specific file for J7200.
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <udma_test.h>

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

int32_t udmaTestPrintPsilMacro(UdmaTestTaskObj *taskObj)
{
    int32_t     retVal = UDMA_SOK;

    GT_0trace(taskObj->traceMask, GT_INFO1, " \r\n");
    GT_0trace(taskObj->traceMask, GT_INFO1,
              " List of all PSIL Threads and Counts:\r\n");
    GT_0trace(taskObj->traceMask, GT_INFO1,
              " ------------------------------------\r\n");

    GT_2trace(taskObj->traceMask, GT_INFO1,
              " CPSW2_TX       : Thread Offset: 0x%0.4X, Thread Count: %d\r\n",
              UDMA_PSIL_CH_CPSW2_TX, UDMA_PSIL_CH_CPSW2_TX_CNT);
    GT_2trace(taskObj->traceMask, GT_INFO1,
              " SAUL0_TX       : Thread Offset: 0x%0.4X, Thread Count: %d\r\n",
              UDMA_PSIL_CH_SAUL0_TX, UDMA_PSIL_CH_SAUL0_TX_CNT);
    GT_2trace(taskObj->traceMask, GT_INFO1,
              " ICSS_G0_TX     : Thread Offset: 0x%0.4X, Thread Count: %d\r\n",
              UDMA_PSIL_CH_ICSS_G0_TX, UDMA_PSIL_CH_ICSS_G0_TX_CNT);
    GT_2trace(taskObj->traceMask, GT_INFO1,
              " ICSS_G1_TX     : Thread Offset: 0x%0.4X, Thread Count: %d\r\n",
              UDMA_PSIL_CH_ICSS_G1_TX, UDMA_PSIL_CH_ICSS_G1_TX_CNT);
    GT_0trace(taskObj->traceMask, GT_INFO1, " \r\n");

    GT_2trace(taskObj->traceMask, GT_INFO1,
              " CPSW2_RX       : Thread Offset: 0x%0.4X, Thread Count: %d\r\n",
              UDMA_PSIL_CH_CPSW2_RX, UDMA_PSIL_CH_CPSW2_RX_CNT);
    GT_2trace(taskObj->traceMask, GT_INFO1,
              " SAUL0_RX       : Thread Offset: 0x%0.4X, Thread Count: %d\r\n",
              UDMA_PSIL_CH_SAUL0_RX, UDMA_PSIL_CH_SAUL0_RX_CNT);
    GT_2trace(taskObj->traceMask, GT_INFO1,
              " ICSS_G0_RX     : Thread Offset: 0x%0.4X, Thread Count: %d\r\n",
              UDMA_PSIL_CH_ICSS_G0_RX, UDMA_PSIL_CH_ICSS_G0_RX_CNT);
    GT_2trace(taskObj->traceMask, GT_INFO1,
              " ICSS_G1_RX     : Thread Offset: 0x%0.4X, Thread Count: %d\r\n",
              UDMA_PSIL_CH_ICSS_G1_RX, UDMA_PSIL_CH_ICSS_G1_RX_CNT);

    return (retVal);
}

int32_t udmaTestPrintPdmaMacro(UdmaTestTaskObj *taskObj)
{
    int32_t     retVal = UDMA_SOK;

    GT_0trace(taskObj->traceMask, GT_INFO1, " \r\n");
    GT_0trace(taskObj->traceMask, GT_INFO1,
              " List of all PDMA Threads:\r\n");
    GT_0trace(taskObj->traceMask, GT_INFO1,
              " -------------------------\r\n");

    GT_1trace(taskObj->traceMask, GT_INFO1,
              " MAIN0_MCSPI0_CH0_TX              : Thread ID: 0x%0.4X\r\n",
              UDMA_PDMA_CH_MAIN0_MCSPI0_CH0_TX);
    GT_1trace(taskObj->traceMask, GT_INFO1,
              " MAIN0_MCSPI0_CH1_TX              : Thread ID: 0x%0.4X\r\n",
              UDMA_PDMA_CH_MAIN0_MCSPI0_CH1_TX);
    GT_1trace(taskObj->traceMask, GT_INFO1,
              " MAIN0_MCSPI0_CH2_TX              : Thread ID: 0x%0.4X\r\n",
              UDMA_PDMA_CH_MAIN0_MCSPI0_CH2_TX);
    GT_1trace(taskObj->traceMask, GT_INFO1,
              " MAIN0_MCSPI0_CH3_TX              : Thread ID: 0x%0.4X\r\n",
              UDMA_PDMA_CH_MAIN0_MCSPI0_CH3_TX);
    GT_1trace(taskObj->traceMask, GT_INFO1,
              " MAIN0_MCSPI1_CH0_TX              : Thread ID: 0x%0.4X\r\n",
              UDMA_PDMA_CH_MAIN0_MCSPI1_CH0_TX);
    GT_1trace(taskObj->traceMask, GT_INFO1,
              " MAIN0_MCSPI1_CH1_TX              : Thread ID: 0x%0.4X\r\n",
              UDMA_PDMA_CH_MAIN0_MCSPI1_CH1_TX);
    GT_1trace(taskObj->traceMask, GT_INFO1,
              " MAIN0_MCSPI1_CH2_TX              : Thread ID: 0x%0.4X\r\n",
              UDMA_PDMA_CH_MAIN0_MCSPI1_CH2_TX);
    GT_1trace(taskObj->traceMask, GT_INFO1,
              " MAIN0_MCSPI1_CH3_TX              : Thread ID: 0x%0.4X\r\n",
              UDMA_PDMA_CH_MAIN0_MCSPI1_CH3_TX);
    GT_1trace(taskObj->traceMask, GT_INFO1,
              " MAIN0_MCSPI2_CH0_TX              : Thread ID: 0x%0.4X\r\n",
              UDMA_PDMA_CH_MAIN0_MCSPI2_CH0_TX);
    GT_1trace(taskObj->traceMask, GT_INFO1,
              " MAIN0_MCSPI2_CH1_TX              : Thread ID: 0x%0.4X\r\n",
              UDMA_PDMA_CH_MAIN0_MCSPI2_CH1_TX);
    GT_1trace(taskObj->traceMask, GT_INFO1,
              " MAIN0_MCSPI2_CH2_TX              : Thread ID: 0x%0.4X\r\n",
              UDMA_PDMA_CH_MAIN0_MCSPI2_CH2_TX);
    GT_1trace(taskObj->traceMask, GT_INFO1,
              " MAIN0_MCSPI2_CH3_TX              : Thread ID: 0x%0.4X\r\n",
              UDMA_PDMA_CH_MAIN0_MCSPI2_CH3_TX);
    GT_1trace(taskObj->traceMask, GT_INFO1,
              " MAIN0_MCSPI3_CH0_TX              : Thread ID: 0x%0.4X\r\n",
              UDMA_PDMA_CH_MAIN0_MCSPI3_CH0_TX);
    GT_1trace(taskObj->traceMask, GT_INFO1,
              " MAIN0_MCSPI3_CH1_TX              : Thread ID: 0x%0.4X\r\n",
              UDMA_PDMA_CH_MAIN0_MCSPI3_CH1_TX);
    GT_1trace(taskObj->traceMask, GT_INFO1,
              " MAIN0_MCSPI3_CH2_TX              : Thread ID: 0x%0.4X\r\n",
              UDMA_PDMA_CH_MAIN0_MCSPI3_CH2_TX);
    GT_1trace(taskObj->traceMask, GT_INFO1,
              " MAIN0_MCSPI3_CH3_TX              : Thread ID: 0x%0.4X\r\n",
              UDMA_PDMA_CH_MAIN0_MCSPI3_CH3_TX);
    GT_1trace(taskObj->traceMask, GT_INFO1,
              " MAIN0_UART0_TX                   : Thread ID: 0x%0.4X\r\n",
              UDMA_PDMA_CH_MAIN0_UART0_TX);
    GT_1trace(taskObj->traceMask, GT_INFO1,
              " MAIN0_UART1_TX                   : Thread ID: 0x%0.4X\r\n",
              UDMA_PDMA_CH_MAIN0_UART1_TX);
    GT_1trace(taskObj->traceMask, GT_INFO1,
              " MAIN1_MCSPI4_CH0_TX              : Thread ID: 0x%0.4X\r\n",
              UDMA_PDMA_CH_MAIN1_MCSPI4_CH0_TX);
    GT_1trace(taskObj->traceMask, GT_INFO1,
              " MAIN1_MCSPI4_CH1_TX              : Thread ID: 0x%0.4X\r\n",
              UDMA_PDMA_CH_MAIN1_MCSPI4_CH1_TX);
    GT_1trace(taskObj->traceMask, GT_INFO1,
              " MAIN1_MCSPI4_CH2_TX              : Thread ID: 0x%0.4X\r\n",
              UDMA_PDMA_CH_MAIN1_MCSPI4_CH2_TX);
    GT_1trace(taskObj->traceMask, GT_INFO1,
              " MAIN1_MCSPI4_CH3_TX              : Thread ID: 0x%0.4X\r\n",
              UDMA_PDMA_CH_MAIN1_MCSPI4_CH3_TX);
    GT_1trace(taskObj->traceMask, GT_INFO1,
              " MAIN1_UART0_TX                   : Thread ID: 0x%0.4X\r\n",
              UDMA_PDMA_CH_MAIN1_UART2_TX);
    GT_1trace(taskObj->traceMask, GT_INFO1,
              " MAIN1_UART1_TX                   : Thread ID: 0x%0.4X\r\n",
              UDMA_PDMA_CH_MAIN1_UART3_TX);
    GT_1trace(taskObj->traceMask, GT_INFO1,
              " MAIN1_UART0_TX                   : Thread ID: 0x%0.4X\r\n",
              UDMA_PDMA_CH_MAIN1_UART4_TX);
    GT_1trace(taskObj->traceMask, GT_INFO1,
              " MAIN1_UART1_TX                   : Thread ID: 0x%0.4X\r\n",
              UDMA_PDMA_CH_MAIN1_UART5_TX);
    GT_1trace(taskObj->traceMask, GT_INFO1,
              " MAIN1_UART1_TX                   : Thread ID: 0x%0.4X\r\n",
              UDMA_PDMA_CH_MAIN1_UART6_TX);
    GT_1trace(taskObj->traceMask, GT_INFO1,
              " MAIN1_MCAN0_CH0_TX               : Thread ID: 0x%0.4X\r\n",
              UDMA_PDMA_CH_MAIN1_MCAN0_CH0_TX);
    GT_1trace(taskObj->traceMask, GT_INFO1,
              " MAIN1_MCAN0_CH1_TX               : Thread ID: 0x%0.4X\r\n",
              UDMA_PDMA_CH_MAIN1_MCAN0_CH1_TX);
    GT_1trace(taskObj->traceMask, GT_INFO1,
              " MAIN1_MCAN0_CH2_TX               : Thread ID: 0x%0.4X\r\n",
              UDMA_PDMA_CH_MAIN1_MCAN0_CH2_TX);
    GT_1trace(taskObj->traceMask, GT_INFO1,
              " MAIN1_MCAN1_CH0_TX               : Thread ID: 0x%0.4X\r\n",
              UDMA_PDMA_CH_MAIN1_MCAN1_CH0_TX);
    GT_1trace(taskObj->traceMask, GT_INFO1,
              " MAIN1_MCAN1_CH1_TX               : Thread ID: 0x%0.4X\r\n",
              UDMA_PDMA_CH_MAIN1_MCAN1_CH1_TX);
    GT_1trace(taskObj->traceMask, GT_INFO1,
              " MAIN1_MCAN1_CH2_TX               : Thread ID: 0x%0.4X\r\n",
              UDMA_PDMA_CH_MAIN1_MCAN1_CH2_TX);
GT_1trace(taskObj->traceMask, GT_INFO1,
              " MAIN0_MCSPI0_CH0_RX              : Thread ID: 0x%0.4X\r\n",
              UDMA_PDMA_CH_MAIN0_MCSPI0_CH0_RX);
    GT_1trace(taskObj->traceMask, GT_INFO1,
              " MAIN0_MCSPI0_CH1_RX              : Thread ID: 0x%0.4X\r\n",
              UDMA_PDMA_CH_MAIN0_MCSPI0_CH1_RX);
    GT_1trace(taskObj->traceMask, GT_INFO1,
              " MAIN0_MCSPI0_CH2_RX              : Thread ID: 0x%0.4X\r\n",
              UDMA_PDMA_CH_MAIN0_MCSPI0_CH2_RX);
    GT_1trace(taskObj->traceMask, GT_INFO1,
              " MAIN0_MCSPI0_CH3_RX              : Thread ID: 0x%0.4X\r\n",
              UDMA_PDMA_CH_MAIN0_MCSPI0_CH3_RX);
    GT_1trace(taskObj->traceMask, GT_INFO1,
              " MAIN0_MCSPI1_CH0_RX              : Thread ID: 0x%0.4X\r\n",
              UDMA_PDMA_CH_MAIN0_MCSPI1_CH0_RX);
    GT_1trace(taskObj->traceMask, GT_INFO1,
              " MAIN0_MCSPI1_CH1_RX              : Thread ID: 0x%0.4X\r\n",
              UDMA_PDMA_CH_MAIN0_MCSPI1_CH1_RX);
    GT_1trace(taskObj->traceMask, GT_INFO1,
              " MAIN0_MCSPI1_CH2_RX              : Thread ID: 0x%0.4X\r\n",
              UDMA_PDMA_CH_MAIN0_MCSPI1_CH2_RX);
    GT_1trace(taskObj->traceMask, GT_INFO1,
              " MAIN0_MCSPI1_CH3_RX              : Thread ID: 0x%0.4X\r\n",
              UDMA_PDMA_CH_MAIN0_MCSPI1_CH3_RX);
    GT_1trace(taskObj->traceMask, GT_INFO1,
              " MAIN0_MCSPI2_CH0_RX              : Thread ID: 0x%0.4X\r\n",
              UDMA_PDMA_CH_MAIN0_MCSPI2_CH0_RX);
    GT_1trace(taskObj->traceMask, GT_INFO1,
              " MAIN0_MCSPI2_CH1_RX              : Thread ID: 0x%0.4X\r\n",
              UDMA_PDMA_CH_MAIN0_MCSPI2_CH1_RX);
    GT_1trace(taskObj->traceMask, GT_INFO1,
              " MAIN0_MCSPI2_CH2_RX              : Thread ID: 0x%0.4X\r\n",
              UDMA_PDMA_CH_MAIN0_MCSPI2_CH2_RX);
    GT_1trace(taskObj->traceMask, GT_INFO1,
              " MAIN0_MCSPI2_CH3_RX              : Thread ID: 0x%0.4X\r\n",
              UDMA_PDMA_CH_MAIN0_MCSPI2_CH3_RX);
    GT_1trace(taskObj->traceMask, GT_INFO1,
              " MAIN0_MCSPI3_CH0_RX              : Thread ID: 0x%0.4X\r\n",
              UDMA_PDMA_CH_MAIN0_MCSPI3_CH0_RX);
    GT_1trace(taskObj->traceMask, GT_INFO1,
              " MAIN0_MCSPI3_CH1_RX              : Thread ID: 0x%0.4X\r\n",
              UDMA_PDMA_CH_MAIN0_MCSPI3_CH1_RX);
    GT_1trace(taskObj->traceMask, GT_INFO1,
              " MAIN0_MCSPI3_CH2_RX              : Thread ID: 0x%0.4X\r\n",
              UDMA_PDMA_CH_MAIN0_MCSPI3_CH2_RX);
    GT_1trace(taskObj->traceMask, GT_INFO1,
              " MAIN0_MCSPI3_CH3_RX              : Thread ID: 0x%0.4X\r\n",
              UDMA_PDMA_CH_MAIN0_MCSPI3_CH3_RX);
    GT_1trace(taskObj->traceMask, GT_INFO1,
              " MAIN0_UART0_RX                   : Thread ID: 0x%0.4X\r\n",
              UDMA_PDMA_CH_MAIN0_UART0_RX);
    GT_1trace(taskObj->traceMask, GT_INFO1,
              " MAIN0_UART1_RX                   : Thread ID: 0x%0.4X\r\n",
              UDMA_PDMA_CH_MAIN0_UART1_RX);
    GT_1trace(taskObj->traceMask, GT_INFO1,
              " MAIN1_MCSPI4_CH0_RX              : Thread ID: 0x%0.4X\r\n",
              UDMA_PDMA_CH_MAIN1_MCSPI4_CH0_RX);
    GT_1trace(taskObj->traceMask, GT_INFO1,
              " MAIN1_MCSPI4_CH1_RX              : Thread ID: 0x%0.4X\r\n",
              UDMA_PDMA_CH_MAIN1_MCSPI4_CH1_RX);
    GT_1trace(taskObj->traceMask, GT_INFO1,
              " MAIN1_MCSPI4_CH2_RX              : Thread ID: 0x%0.4X\r\n",
              UDMA_PDMA_CH_MAIN1_MCSPI4_CH2_RX);
    GT_1trace(taskObj->traceMask, GT_INFO1,
              " MAIN1_MCSPI4_CH3_RX              : Thread ID: 0x%0.4X\r\n",
              UDMA_PDMA_CH_MAIN1_MCSPI4_CH3_RX);
    GT_1trace(taskObj->traceMask, GT_INFO1,
              " MAIN1_UART0_RX                   : Thread ID: 0x%0.4X\r\n",
              UDMA_PDMA_CH_MAIN1_UART2_RX);
    GT_1trace(taskObj->traceMask, GT_INFO1,
              " MAIN1_UART1_RX                   : Thread ID: 0x%0.4X\r\n",
              UDMA_PDMA_CH_MAIN1_UART3_RX);
    GT_1trace(taskObj->traceMask, GT_INFO1,
              " MAIN1_UART0_RX                   : Thread ID: 0x%0.4X\r\n",
              UDMA_PDMA_CH_MAIN1_UART4_RX);
    GT_1trace(taskObj->traceMask, GT_INFO1,
              " MAIN1_UART1_RX                   : Thread ID: 0x%0.4X\r\n",
              UDMA_PDMA_CH_MAIN1_UART5_RX);
    GT_1trace(taskObj->traceMask, GT_INFO1,
              " MAIN1_UART1_RX                   : Thread ID: 0x%0.4X\r\n",
              UDMA_PDMA_CH_MAIN1_UART6_RX);
    GT_1trace(taskObj->traceMask, GT_INFO1,
              " MAIN1_MCAN0_CH0_RX               : Thread ID: 0x%0.4X\r\n",
              UDMA_PDMA_CH_MAIN1_MCAN0_CH0_RX);
    GT_1trace(taskObj->traceMask, GT_INFO1,
              " MAIN1_MCAN0_CH1_RX               : Thread ID: 0x%0.4X\r\n",
              UDMA_PDMA_CH_MAIN1_MCAN0_CH1_RX);
    GT_1trace(taskObj->traceMask, GT_INFO1,
              " MAIN1_MCAN0_CH2_RX               : Thread ID: 0x%0.4X\r\n",
              UDMA_PDMA_CH_MAIN1_MCAN0_CH2_RX);
    GT_1trace(taskObj->traceMask, GT_INFO1,
              " MAIN1_MCAN1_CH0_RX               : Thread ID: 0x%0.4X\r\n",
              UDMA_PDMA_CH_MAIN1_MCAN1_CH0_RX);
    GT_1trace(taskObj->traceMask, GT_INFO1,
              " MAIN1_MCAN1_CH1_RX               : Thread ID: 0x%0.4X\r\n",
              UDMA_PDMA_CH_MAIN1_MCAN1_CH1_RX);
    GT_1trace(taskObj->traceMask, GT_INFO1,
              " MAIN1_MCAN1_CH2_RX               : Thread ID: 0x%0.4X\r\n",
              UDMA_PDMA_CH_MAIN1_MCAN1_CH2_RX);
    GT_1trace(taskObj->traceMask, GT_INFO1,
              " MAIN1_ADC0_CH0_RX                : Thread ID: 0x%0.4X\r\n",
              UDMA_PDMA_CH_MAIN1_ADC0_CH0_RX);
    GT_1trace(taskObj->traceMask, GT_INFO1,
              " MAIN1_ADC0_CH1_RX                : Thread ID: 0x%0.4X\r\n",
              UDMA_PDMA_CH_MAIN1_ADC0_CH1_RX);

    return (retVal);
}

void UdmaTestRmInitPrmsMapped_init(Udma_InitPrms *initPrms)
{
    /* Assign start of Mapped TX Channels */
    initPrms->rmInitPrms.startMappedTxCh[UDMA_MAPPED_TX_GROUP_CPSW]    = CSL_DMSS_PKTDMA_TX_CHANS_CPSW_START;
    initPrms->rmInitPrms.startMappedTxCh[UDMA_MAPPED_TX_GROUP_SAUL]    = CSL_DMSS_PKTDMA_TX_CHANS_SAUL0_START;
    initPrms->rmInitPrms.startMappedTxCh[UDMA_MAPPED_TX_GROUP_ICSSG_0] = CSL_DMSS_PKTDMA_TX_CHANS_ICSSG0_START;
    initPrms->rmInitPrms.startMappedTxCh[UDMA_MAPPED_TX_GROUP_ICSSG_1] = CSL_DMSS_PKTDMA_TX_CHANS_ICSSG1_START;

    /* Assign num of Mapped TX Channels */
    initPrms->rmInitPrms.numMappedTxCh[UDMA_MAPPED_TX_GROUP_CPSW]    = CSL_DMSS_PKTDMA_TX_CHANS_CPSW_CNT;
    initPrms->rmInitPrms.numMappedTxCh[UDMA_MAPPED_TX_GROUP_SAUL]    = CSL_DMSS_PKTDMA_TX_CHANS_SAUL0_CNT + 
                                                                          CSL_DMSS_PKTDMA_TX_CHANS_SAUL1_CNT;
    initPrms->rmInitPrms.numMappedTxCh[UDMA_MAPPED_TX_GROUP_ICSSG_0] = CSL_DMSS_PKTDMA_TX_CHANS_ICSSG0_CNT;
    initPrms->rmInitPrms.numMappedTxCh[UDMA_MAPPED_TX_GROUP_ICSSG_1] = CSL_DMSS_PKTDMA_TX_CHANS_ICSSG1_CNT;
    
    /* Assign start of Mapped RX Channels */
    initPrms->rmInitPrms.startMappedRxCh[UDMA_MAPPED_RX_GROUP_CPSW- UDMA_NUM_MAPPED_TX_GROUP]    = CSL_DMSS_PKTDMA_RX_CHANS_CPSW_START;
    initPrms->rmInitPrms.startMappedRxCh[UDMA_MAPPED_RX_GROUP_SAUL- UDMA_NUM_MAPPED_TX_GROUP]    = CSL_DMSS_PKTDMA_RX_CHANS_SAUL0_START;
    initPrms->rmInitPrms.startMappedRxCh[UDMA_MAPPED_RX_GROUP_ICSSG_0- UDMA_NUM_MAPPED_TX_GROUP] = CSL_DMSS_PKTDMA_RX_CHANS_ICSSG0_START;
    initPrms->rmInitPrms.startMappedRxCh[UDMA_MAPPED_RX_GROUP_ICSSG_1- UDMA_NUM_MAPPED_TX_GROUP] = CSL_DMSS_PKTDMA_RX_CHANS_ICSSG1_START;

    /* Assign num of Mapped RX Channels */
    initPrms->rmInitPrms.numMappedRxCh[UDMA_MAPPED_RX_GROUP_CPSW- UDMA_NUM_MAPPED_TX_GROUP]    = CSL_DMSS_PKTDMA_RX_CHANS_CPSW_CNT;
    initPrms->rmInitPrms.numMappedRxCh[UDMA_MAPPED_RX_GROUP_SAUL- UDMA_NUM_MAPPED_TX_GROUP]    = CSL_DMSS_PKTDMA_RX_CHANS_SAUL0_CNT + CSL_DMSS_PKTDMA_RX_CHANS_SAUL1_CNT +
                                                                                                  CSL_DMSS_PKTDMA_RX_CHANS_SAUL2_CNT + CSL_DMSS_PKTDMA_RX_CHANS_SAUL3_CNT;
    initPrms->rmInitPrms.numMappedRxCh[UDMA_MAPPED_RX_GROUP_ICSSG_0- UDMA_NUM_MAPPED_TX_GROUP] = CSL_DMSS_PKTDMA_RX_CHANS_ICSSG0_CNT;
    initPrms->rmInitPrms.numMappedRxCh[UDMA_MAPPED_RX_GROUP_ICSSG_1- UDMA_NUM_MAPPED_TX_GROUP] = CSL_DMSS_PKTDMA_RX_CHANS_ICSSG1_CNT;

    /* Assign start of Mapped Rings */
    initPrms->rmInitPrms.startMappedRing[UDMA_MAPPED_TX_GROUP_CPSW]    = CSL_DMSS_PKTDMA_TX_FLOWS_CPSW_START;
    initPrms->rmInitPrms.startMappedRing[UDMA_MAPPED_TX_GROUP_SAUL]    = CSL_DMSS_PKTDMA_TX_FLOWS_SAUL0_START;
    initPrms->rmInitPrms.startMappedRing[UDMA_MAPPED_TX_GROUP_ICSSG_0] = CSL_DMSS_PKTDMA_TX_FLOWS_ICSSG0_START;
    initPrms->rmInitPrms.startMappedRing[UDMA_MAPPED_TX_GROUP_ICSSG_1] = CSL_DMSS_PKTDMA_TX_FLOWS_ICSSG1_START;
    initPrms->rmInitPrms.startMappedRing[UDMA_MAPPED_RX_GROUP_CPSW]    = CSL_DMSS_PKTDMA_RX_FLOWS_CPSW_START;
    initPrms->rmInitPrms.startMappedRing[UDMA_MAPPED_RX_GROUP_SAUL]    = CSL_DMSS_PKTDMA_RX_FLOWS_SAUL0_START; 
    initPrms->rmInitPrms.startMappedRing[UDMA_MAPPED_RX_GROUP_ICSSG_0] = CSL_DMSS_PKTDMA_RX_FLOWS_ICSSG0_START;
    initPrms->rmInitPrms.startMappedRing[UDMA_MAPPED_RX_GROUP_ICSSG_1] = CSL_DMSS_PKTDMA_RX_FLOWS_ICSSG1_START;

    /* Assign num of Mapped Rings */
    initPrms->rmInitPrms.numMappedRing[UDMA_MAPPED_TX_GROUP_CPSW]    = CSL_DMSS_PKTDMA_TX_FLOWS_CPSW_CNT;
    initPrms->rmInitPrms.numMappedRing[UDMA_MAPPED_TX_GROUP_SAUL]    = CSL_DMSS_PKTDMA_TX_FLOWS_SAUL0_CNT + CSL_DMSS_PKTDMA_TX_FLOWS_SAUL1_CNT;
    initPrms->rmInitPrms.numMappedRing[UDMA_MAPPED_TX_GROUP_ICSSG_0] = CSL_DMSS_PKTDMA_TX_FLOWS_ICSSG0_CNT;
    initPrms->rmInitPrms.numMappedRing[UDMA_MAPPED_TX_GROUP_ICSSG_1] = CSL_DMSS_PKTDMA_TX_FLOWS_ICSSG1_CNT;
    initPrms->rmInitPrms.numMappedRing[UDMA_MAPPED_RX_GROUP_CPSW]    = CSL_DMSS_PKTDMA_RX_FLOWS_CPSW_CNT;
    initPrms->rmInitPrms.numMappedRing[UDMA_MAPPED_RX_GROUP_SAUL]    = CSL_DMSS_PKTDMA_RX_FLOWS_SAUL0_CNT + CSL_DMSS_PKTDMA_RX_FLOWS_SAUL2_CNT; /* SAUL1 RX flows are shared with SAUL0; And SAUL3 RX flows are shared with SAUL2 */
    initPrms->rmInitPrms.numMappedRing[UDMA_MAPPED_RX_GROUP_ICSSG_0] = CSL_DMSS_PKTDMA_RX_FLOWS_ICSSG0_CNT;
    initPrms->rmInitPrms.numMappedRing[UDMA_MAPPED_RX_GROUP_ICSSG_1] = CSL_DMSS_PKTDMA_RX_FLOWS_ICSSG1_CNT;
}

