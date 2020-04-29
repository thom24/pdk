/*
 *   @file  main.c
 *
 *   @brief
 *      Unit Test common code for the MibSpi Driver
 *
 *  \par
 *  NOTE:
 *      (C) Copyright 2020 Texas Instruments, Inc.
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

/**************************************************************************
 *************************** Include Files ********************************
 **************************************************************************/
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>

/* BIOS/XDC Include Files. */
#include <xdc/std.h>
#include <xdc/cfg/global.h>
#include <xdc/runtime/IHeap.h>
#include <xdc/runtime/System.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/Memory.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Event.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/heaps/HeapBuf.h>
#include <ti/sysbios/heaps/HeapMem.h>
#include <ti/sysbios/knl/Event.h>

#include <ti/board/board.h>
#include <ti/drv/edma/edma.h>
/* SPI test include files */
#include "mibspi_test_common.h"
#include "MIBSPI_log.h"

/**************************************************************************
 *************************** Local Definitions *********************************
 **************************************************************************/

/**************************************************************************
 *************************** Global Definitions ********************************
 **************************************************************************/
/* System DMA handle, created in init Task */
EDMA_Handle          gDmaHandle[MIBSPI_TEST_NUM_SPIINSTANCES] = {NULL,NULL};
enum MibSpi_InstanceId gMibspiInst[MIBSPI_TEST_NUM_SPIINSTANCES] = {MIBSPI_INST_ID_MSS_SPIA, MIBSPI_INST_ID_MSS_SPIB};
    
/* Test case global variables */
bool gXWR1xxxLoopbackTest = true;
bool gXWR1xxxSlaveReady = false;


/*
 *  ======== Board_initMIBSPI ========
 */
static bool Board_initMIBSPI(void)
{
    Board_initCfg boardCfg;
    Board_STATUS  status;

    boardCfg = BOARD_INIT_PINMUX_CONFIG |  
               BOARD_INIT_MODULE_CLOCK  |
               BOARD_INIT_UNLOCK_MMR;

    status = Board_init(boardCfg);

    if (status != BOARD_SOK)
    {
        return false;
    }
    else
    {
        return true;
    }
}



/**
 *  @b Description
 *  @n
 *      System Initialization Task which initializes the various
 *      components in the system.
 *
 *   @param[in] arg0               Task Arg0
 *   @param[in] arg1               Task Arg1
 *
 *  @retval
 *      Not Applicable.
 */
static void Test_initTask(UArg arg0, UArg arg1)
{
    int32_t         retVal = 0;
    MibSpi_HwCfg cfg;
    int32_t i;
    MIBSPI_UtilsPrms utilsPrms;

    if (Board_initMIBSPI() != true)
    {
        printf("Board_initMIBSPI() failed\n");
        return;
    }
    for (i = 0; i < sizeof(gMibspiInst)/sizeof(gMibspiInst[0]); i++)
    {
        retVal = MIBSPI_socGetInitCfg(gMibspiInst[i], &cfg);
        if (retVal < 0)
        {
            printf("SPI_socGetInitCfg failed with error=%d\n", retVal);
            return;
        }
#ifdef MIBSPI_DMA_ENABLE
        EDMA_instanceInfo_t instanceInfo;
        int32_t errorCode;

        gDmaHandle[i] = EDMA_getHandle(cfg.edmaCCId, &instanceInfo);
        if(gDmaHandle[i] == NULL)
        {
            /* Open DMA driver instance 0 for SPI test */
            gDmaHandle[i] = EDMA_open(cfg.edmaCCId, &errorCode, &instanceInfo);
        }
        if(gDmaHandle[i] == NULL)
        {
            printf("Open DMA driver failed with error=%d\n", retVal);
            return;
        }
#else
        gDmaHandle[i] = NULL;
#endif
    }
    /* Initialize the SPI */
    utilsPrms.printFxn = MIBSPI_log;
    utilsPrms.traceFxn = MIBSPI_log;
    MIBSPI_init(&utilsPrms);

    /**************************************************************************
     * Test: One instace API test - SPIA
     **************************************************************************/
    Test_spiAPI_oneInstance(0);
    MIBSPI_log("Debug: Finished API Test for SPIA!\n");

        /**************************************************************************
     * Test: One instace API test - SPIB
         **************************************************************************/
    Test_spiAPI_oneInstance(1);
    MIBSPI_log("Debug: Finished API Test for SPIB!\n");

        /**************************************************************************
     * Test: two instaces API test - SPIA & SPIB
         **************************************************************************/
    Test_spiAPI_twoInstance();
    MIBSPI_log("Debug: Finished API Test for SPIA + SPIB!\n");

        /**************************************************************************
     * Test: LoopBack Test
         **************************************************************************/
    if(gXWR1xxxLoopbackTest)
    {
        MIBSPI_log("Debug: SPI loopback Test for SPIA!\n");

        /* MibSPIA loopback test , MibSPIA only supports one slave */
        Test_loopback_oneInstance(0U, 0U);

        MIBSPI_log("Debug: SPI loopback Test for SPIB on Slave 0!\n");

        /* MibSPIB loopback test, slave -0 */
        Test_loopback_oneInstance(1U, 0U);

        MIBSPI_log("Debug: SPI loopback Test for SPIB on Slave 1!\n");

        /* MibSPIB loopback test, slave -1 */
        Test_loopback_oneInstance(1U, 1U);

        MIBSPI_log("Debug: SPI loopback Test for SPIB on Slave 2!\n");

        /* MibSPIB loopback test, slave -2 */
        Test_loopback_oneInstance(1U, 2U);
        MIBSPI_log("Debug: Loopback test finished!\n");
    }


    MIBSPI_log("Debug: SPI Test is done!\n");

    /* After test all done, terminate DSP by calling BIOS_exit().
        This is required by MCPI test framework script.
        MCPI test framework script waits for DSP halt,
        if DSP doesn't halt, script will wait until timeout then claim test fail.
     */
    BIOS_exit(0);

    return;
}

/**
 *  @b Description
 *  @n
 *      Entry point into the test code.
 *
 *  @retval
 *      Not Applicable.
 */
int main (void)
{
    Task_Params      taskParams;

    /* Debug Message: */
    MIBSPI_log ("******************************************\n");
    MIBSPI_log ("Debug: MibSPI Driver Test Application Start \n");
    MIBSPI_log ("******************************************\n");

    /* Initialize the Task Parameters. */
    Task_Params_init(&taskParams);
    taskParams.stackSize = 6*1024;
    Task_create(Test_initTask, &taskParams, NULL);

    /* Start BIOS */
    BIOS_start();

    return 0;

}

