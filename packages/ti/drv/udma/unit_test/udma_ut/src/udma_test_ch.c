/*
 *  Copyright (c) Texas Instruments Incorporated 2019-20
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
 *  \file udma_test_ch.c
 *
 *  \brief UDMA channel related test case file.
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

static int32_t udmaTestChPktdmaParamCheckTestLoop(UdmaTestTaskObj *taskObj);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* Global test pass/fail flag */
static volatile int32_t gUdmaTestChResult = UDMA_SOK;

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */


int32_t udmaTestChPktdmaParamCheckTc(UdmaTestTaskObj *taskObj)
{
    int32_t     retVal = UDMA_SOK;
    uint32_t    loopCnt = 0U;

    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: PKTDMA Channel Paramter Check Testcase ::\r\n", taskObj->taskId);
    GT_2trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: Loop count           : %d ::\r\n", taskObj->taskId, taskObj->loopCnt);

    gUdmaTestChResult = UDMA_SOK;
    while(loopCnt < taskObj->loopCnt)
    {
        retVal = udmaTestChPktdmaParamCheckTestLoop(taskObj);
        if(UDMA_SOK != retVal)
        {
            break;
        }

        loopCnt++;
    }

    retVal += gUdmaTestChResult;

    return (retVal);
}


static int32_t udmaTestChPktdmaParamCheckTestLoop(UdmaTestTaskObj *taskObj)
{
    int32_t             retVal = UDMA_SOK;
    uint32_t            elemCnt = 50U, ringMemSize;
    uint32_t            heapId = UTILS_MEM_HEAP_ID_MSMC;
    Udma_DrvHandle      drvHandle;
    uint32_t            chType;
    struct Udma_ChObj   chObj;
    Udma_ChHandle       chHandle = &chObj;
    Udma_ChPrms         chPrms;
    void               *ringMem = NULL;

    drvHandle = &taskObj->testObj->drvObj[UDMA_TEST_INST_ID_PKTDMA_0];
    ringMemSize = elemCnt * sizeof (uint64_t);
    ringMem = Utils_memAlloc(heapId, ringMemSize, UDMA_CACHELINE_ALIGNMENT);
    if(NULL == ringMem)
    {
        retVal = UDMA_EALLOC;
        GT_0trace(taskObj->traceMask, GT_ERR, " Ring memory allocation failure\r\n");
    }


#if ((UDMA_NUM_MAPPED_TX_GROUP + UDMA_NUM_MAPPED_RX_GROUP) > 0)
    /* PKTDMA Channel Open with no ring memory test */
    chType = UDMA_CH_TYPE_TX_MAPPED;
    UdmaChPrms_init(&chPrms, chType);
    chPrms.mappedChGrp = UDMA_MAPPED_TX_GROUP_CPSW;
    chPrms.peerChNum   = UDMA_PSIL_CH_CPSW2_TX;
    retVal = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
    if(UDMA_SOK != retVal)
    {
        GT_0trace(taskObj->traceMask, GT_ERR,
            " UDMA channel open failed!!\n");
    }
    if(UDMA_SOK == retVal)
    {
        if(chHandle->fqRing != (Udma_RingHandle) NULL_PTR)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                    " Ring allocated even when no ring memory was provided!!\n");
            retVal = UDMA_EFAIL;
        }
    }
    if(UDMA_SOK == retVal)
    {
        retVal = Udma_chClose(chHandle);
        if(UDMA_SOK != retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                " UDMA channel close failed!!\n");
        }
    }

    /* PKTDMA Mapped Channel Open with invalid mapped channel group negative test*/
    chType = UDMA_CH_TYPE_TX_MAPPED;
    UdmaChPrms_init(&chPrms, chType);
    chPrms.fqRingPrms.ringMem       = ringMem;
    chPrms.fqRingPrms.ringMemSize   = ringMemSize;
    chPrms.fqRingPrms.elemCnt       = elemCnt;
    chPrms.mappedChGrp = UDMA_NUM_MAPPED_TX_GROUP + UDMA_NUM_MAPPED_RX_GROUP;
    retVal = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
    if(UDMA_SOK == retVal)
    {
        GT_0trace(taskObj->traceMask, GT_ERR,
            " Channel Open did not fail for incorrect mappedChGrp check!!\n");
            retVal = UDMA_EFAIL;
    }
    else
    {
        retVal = UDMA_SOK;
    }
    
#endif

    if(NULL != ringMem)
    {
        retVal += Utils_memFree(heapId, ringMem, ringMemSize);
        if(UDMA_SOK != retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR, " Ring free failed!!\n");
        }
    }

    return(retVal);
}

