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
 *  \file udma_test_common.c
 *
 *  \brief Common code that can be shared across test case files.
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <udma_test.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#if defined (__C7100__)
/* UART not yet supported */
#else
#define APPUTILS_UART_INPUT
#endif

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

static uint32_t gUdmaTestCounter = 0;
static uint32_t gUdmaPassCounter = 0;
static uint32_t gUdmaFailCounter = 0;

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

/**
 *  Initialize driver for all instances
 */
int32_t udmaTestInitDriver(UdmaTestObj *testObj)
{
    int32_t         retVal = UDMA_SOK;
    uint32_t        instId;
    Udma_InitPrms   initPrms;
    Udma_DrvHandle  drvHandle;

    for(instId = UDMA_INST_ID_START; instId <= UDMA_INST_ID_MAX; instId++)
    {
        /* UDMA driver init */
        drvHandle = &testObj->drvObj[instId];
        UdmaInitPrms_init(instId, &initPrms);
        initPrms.virtToPhyFxn       = &Udma_appVirtToPhyFxn;
        initPrms.phyToVirtFxn       = &Udma_appPhyToVirtFxn;
        initPrms.printFxn           = &udmaDrvPrint;
        /* Override RM params for HC and UHC blockcpy channels as they are not
         * part of default table */
#if (UDMA_SOC_CFG_UDMAP_PRESENT == 1)
        if(UDMA_INST_ID_MAIN_0 == instId)
        {
            initPrms.rmInitPrms.startBlkCopyUhcCh = UDMA_TEST_MAIN_UHC_START;
            initPrms.rmInitPrms.numBlkCopyUhcCh   = UDMA_TEST_MAX_MAIN_BC_UHC_CH;
            initPrms.rmInitPrms.startBlkCopyHcCh  = UDMA_TEST_MAIN_HC_START;
            initPrms.rmInitPrms.numBlkCopyHcCh    = UDMA_TEST_MAX_MAIN_BC_HC_CH;
        }
        if(UDMA_INST_ID_MCU_0 == instId)
        {
            initPrms.rmInitPrms.startBlkCopyUhcCh = UDMA_TEST_MCU_UHC_START;
            initPrms.rmInitPrms.numBlkCopyUhcCh   = UDMA_TEST_MAX_MCU_BC_UHC_CH;
            initPrms.rmInitPrms.startBlkCopyHcCh  = UDMA_TEST_MCU_HC_START;
            initPrms.rmInitPrms.numBlkCopyHcCh    = UDMA_TEST_MAX_MCU_BC_HC_CH;
        }
#endif
#if (UDMA_SOC_CFG_LCDMA_PRESENT == 1)
        if(UDMA_INST_ID_BCDMA_0 == instId)
        {
            initPrms.rmInitPrms.startBlkCopyUhcCh = UDMA_TEST_BCDMA_UHC_START;
            initPrms.rmInitPrms.numBlkCopyUhcCh   = UDMA_TEST_MAX_BCDMA_BC_UHC_CH;
            initPrms.rmInitPrms.startBlkCopyHcCh  = UDMA_TEST_BCDMA_HC_START;
            initPrms.rmInitPrms.numBlkCopyHcCh    = UDMA_TEST_MAX_BCDMA_BC_HC_CH;
        }
        if(UDMA_INST_ID_PKTDMA_0 == instId)
        {
            initPrms.rmInitPrms.startBlkCopyUhcCh = UDMA_TEST_PKTDMA_UHC_START;
            initPrms.rmInitPrms.numBlkCopyUhcCh   = UDMA_TEST_MAX_PKTDMA_BC_UHC_CH;
            initPrms.rmInitPrms.startBlkCopyHcCh  = UDMA_TEST_PKTDMA_HC_START;
            initPrms.rmInitPrms.numBlkCopyHcCh    = UDMA_TEST_MAX_PKTDMA_BC_HC_CH;

            if((UDMA_NUM_MAPPED_TX_GROUP + UDMA_NUM_MAPPED_RX_GROUP) > 0U);
            {
                UdmaTestRmInitPrmsMapped_init(&initPrms);
            }
        }
#endif

        retVal += Udma_init(drvHandle, &initPrms);
        if(UDMA_SOK != retVal)
        {
            GT_1trace(testObj->traceMask, GT_ERR,
                      " UDMA instance %d init failed!!\n", instId);
        }
    }

    return (retVal);
}

int32_t udmaTestDeinitDriver(UdmaTestObj *testObj)
{
    int32_t         retVal = UDMA_SOK;
    uint32_t        instId;
    Udma_DrvHandle  drvHandle;

    for(instId = UDMA_INST_ID_START; instId <= UDMA_INST_ID_MAX; instId++)
    {
        /* UDMA driver deinit */
        drvHandle = &testObj->drvObj[instId];
        retVal += Udma_deinit(drvHandle);
        if(UDMA_SOK != retVal)
        {
            GT_1trace(testObj->traceMask, GT_ERR,
                      " UDMA instance %d deinit failed!!\n", instId);
        }
    }

    return (retVal);
}

/**
 *  udmaTestInitVariables
 *  Initialize the global variables and pointers.
 */
int32_t udmaTestInitVariables(const UdmaTestObj *testObj)
{
    int32_t retVal = UDMA_SOK;

    return (retVal);
}

/**
 *  udmaTestFreeVariables
 *  Free the allocated handles and buffers.
 */
void udmaTestFreeVariables(const UdmaTestObj *testObj)
{
    return;
}

void udmaTestLogTestResult(const UdmaTestObj *testObj,
                           int32_t            testResult,
                           uint32_t           testCaseId,
                           char              *testcaseInfo)
{
    if(UDMA_SOK == testResult)
    {
        gUdmaPassCounter++;
        GT_1trace(testObj->traceMask, GT_INFO,
                  " |TEST RESULT|PASS|%d|\r\n", testCaseId);
    }
    else
    {
        gUdmaFailCounter++;
        GT_1trace(testObj->traceMask, GT_INFO,
                  " |TEST RESULT|FAIL|%d|\r\n", testCaseId);
    }

    gUdmaTestCounter++;
    GT_1trace(testObj->traceMask, GT_INFO,
              " |TEST END|:: %d ::\r\n", testCaseId);

    return;
}

void udmaTestPrintTestResult(const UdmaTestObj *testObj,
                             uint32_t           skipCount,
                             uint32_t           disableCount)
{
    GT_0trace(testObj->traceMask, GT_INFO,
              "\r\n *********Test Case Statistics*****************");
    GT_1trace(testObj->traceMask, GT_INFO,
              "\r\n      Total Test Case         : %d",
              (gUdmaTestCounter + skipCount + disableCount));
    GT_1trace(testObj->traceMask, GT_INFO,
              "\r\n      Total Test Case Executed: %d", gUdmaTestCounter);
    GT_1trace(testObj->traceMask, GT_INFO,
              "\r\n      Total Test Case Passed  : %d", gUdmaPassCounter);
    GT_1trace(testObj->traceMask, GT_INFO,
              "\r\n      Total Test Case Failed  : %d", gUdmaFailCounter);
    GT_1trace(testObj->traceMask, GT_INFO,
              "\r\n      Total Test Case Skipped : %d", skipCount);
    GT_1trace(testObj->traceMask, GT_INFO,
              "\r\n      Total Test Case Disabled: %d", disableCount);
    GT_0trace(testObj->traceMask, GT_INFO,
              "\r\n *********************************************\r\n \r\n");

    if(gUdmaFailCounter > 0U)
    {
        GT_0trace(testObj->traceMask, GT_INFO,
                  "\r\n Some tests have failed. \r\n");
    }
    else
    {
        GT_0trace(testObj->traceMask, GT_INFO,
                  "\r\n All tests have passed. \r\n");
    }

    return;
}

void udmaTestResetTestResult(void)
{
    gUdmaTestCounter = 0;
    gUdmaPassCounter = 0;
    gUdmaFailCounter = 0;

    return;
}

char AppUtils_getChar(void)
{
    char ch;

#if defined (APPUTILS_UART_INPUT)
    UART_scanFmt("%c", &ch);
#else
    scanf("%c", &ch);
#endif

    return (ch);
}

int32_t AppUtils_getCharTimeout(char *ch, uint32_t msec)
{
    int32_t retVal = UDMA_EFAIL;
#if defined (APPUTILS_UART_INPUT)
/* Timeout not yet supported in UART console */
#if 0
    uint32_t startTime, elaspsedTime;

    startTime = AppUtils_getCurTimeInMsec();
    do
    {
        retVal = UARTGetcNonBlocking((uint8 *) ch);
        if(retVal == 0U)
        {
            retVal = UDMA_SOK;
            break;
        }
        retVal       = UDMA_EFAIL;
        elaspsedTime = AppUtils_getElapsedTimeInMsec(startTime);
    } while(elaspsedTime <= msec);
#else
    UART_scanFmt("%c", ch);
    retVal = UDMA_SOK;
#endif
#else
    /* Timeout not supported by stdio scanf */
    scanf("%c", ch);
    retVal = UDMA_SOK;
#endif

    return (retVal);
}

int32_t AppUtils_getNum(void)
{
    int32_t num;

#if defined (APPUTILS_UART_INPUT)
    UART_scanFmt("%d", &num);
#else
    scanf("%d", &num);
#endif

    return (num);
}

uint32_t AppUtils_getCurTimeInMsec(void)
{
    uint64_t curTimeMsec = 0, curTimeUsec = 0;

#if defined (UDMA_UT_BAREMETAL) && defined (BUILD_C7X_1)
	/* C7x baremetal doesnot support getting time timestamp */
#else
    curTimeUsec = TimerP_getTimeInUsecs();
#endif
    curTimeMsec = (curTimeUsec / 1000U);

    return ((uint32_t) curTimeMsec);
}

uint32_t AppUtils_getElapsedTimeInMsec(uint32_t startTime)
{
    uint32_t     elapsedTimeMsec = 0U, currTime;

    currTime = AppUtils_getCurTimeInMsec();
    if (currTime < startTime)
    {
        /* Counter overflow occured */
        elapsedTimeMsec = (0xFFFFFFFFU - startTime) + currTime + 1U;
    }
    else
    {
        elapsedTimeMsec = currTime - startTime;
    }

    return (elapsedTimeMsec);
}

void udmaTestCalcPerformance(UdmaTestTaskObj *taskObj, uint32_t durationMs)
{
    uint32_t        chCnt;
    uint64_t        totalTransfer, curTransfer;
    UdmaTestChObj  *chObj;

    totalTransfer = 0u;
    for(chCnt = 0U ; chCnt < taskObj->numCh; chCnt++)
    {
        chObj = taskObj->chObj[chCnt];
        GT_assert(taskObj->traceMask, chObj != NULL);

        curTransfer = ((uint64_t) chObj->icnt[0] * (uint64_t) chObj->icnt[1] *
                       (uint64_t) chObj->icnt[2] * (uint64_t) chObj->icnt[3] *
                       (uint64_t) taskObj->loopCnt);
        if((UDMA_TT_BLK_CPY == taskObj->testType) ||
           (UDMA_TT_DRU_DIRECT == taskObj->testType) ||
           (UDMA_TT_DRU_INDIRECT == taskObj->testType))
        {
            /* Memcpy is for both read and write */
            curTransfer *= 2U;
        }
        totalTransfer += curTransfer;
    }

    /* Calculate performance */
    taskObj->durationMs = durationMs;
    taskObj->totalTransfer = totalTransfer;
    taskObj->mps = (uint64_t)
                ((totalTransfer / (uint64_t) 1000) / (uint64_t) durationMs);

    GT_0trace(taskObj->traceMask, GT_INFO1, " \r\n");
    GT_2trace(taskObj->traceMask, GT_INFO1,
        " |TEST RESULT|:: Task:%d: Throughput: %d MB/s ::\r\n", taskObj->taskId, taskObj->mps);

    return;
}

void udmaTestCalcTotalPerformance(UdmaTestObj *testObj, uint32_t durationMs)
{
    uint32_t        taskCnt;
    uint64_t        totalTransfer, mps;
    UdmaTestTaskObj *taskObj;

    totalTransfer = 0u;
    for(taskCnt = 0U; taskCnt < testObj->testPrms->numTasks; taskCnt++)
    {
        taskObj = &testObj->taskObj[taskCnt];
        totalTransfer += taskObj->totalTransfer;
    }

    /* Calculate performance */
    mps = (uint64_t) ((totalTransfer / (uint64_t) 1000) / (uint64_t) durationMs);

    GT_0trace(testObj->traceMask, GT_INFO, " \r\n");
    GT_1trace(testObj->traceMask, GT_INFO,
        " |TEST RESULT|:: Throughput: %d MB/s ::\r\n", mps);

    return;
}

int32_t udmaTestCompareRingHwOccDriver(Udma_RingHandle ringHandle, uint32_t cnt, uint32_t direction)
{
    int32_t     retVal = UDMA_EFAIL;
    uint32_t    retry = 10000U;

    /* There is some delay expected between HWOCC update and ring push */
    while(retry > 0U)
    {
        if(udmaTestGetRingHwOccDriver(ringHandle, direction) == cnt)
        {
            retVal = UDMA_SOK;
            break;
        }
        retry--;
    }

    return (retVal);
}
uint32_t udmaTestGetRingHwOccDriver(Udma_RingHandle ringHandle, uint32_t direction)
{
    uint32_t occ = 0U;
    Udma_DrvHandle  drvHandle = ringHandle->drvHandle;

    if(UDMA_TEST_RING_ACC_DIRECTION_FORWARD == direction)
    {
        occ = drvHandle->ringGetForwardRingOcc(ringHandle);
    }
    else
    {
        occ = drvHandle->ringGetReverseRingOcc(ringHandle);
    }
    return (occ);
}
