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

/*
 *   @file  main_mss.c
 *
 *   @brief
 *      Unit Test code for the Mailbox MSS
 *      Implements message exchange between MSS and DSS.
 *      Runs on MSS.
 *
 */

/* *************** Test definition: ***************************************************
1. MSS and DSS come out of reset and initialize mailbox driver.
2. DSS loops, waiting for message 1 from MSS
3. MSS sends message 1 to DSS and MSS waits for message 2 from DSS
4. DSS receives message 1 from MSS, DSS confirms that content matches expected values
5. DSS sends message 2 to MSS
6. MSS receives message 2 from DSS, MSS confirms that content matches expected values
   (if MAILBOX_TEST_BSS_DRIVER_INSTANCE is defined, MSS will receive a message from BSS
    when MSS is in the middle of processing the DSS message of step 6)
7. Steps 2-6 are repeated with another pattern, this time only 1 word message each way
   (no BSS interruption this time around)
8. MSS and DSS declare test PASS or FAIL
*/

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
/* Standard Include Files. */
#include <string.h>
#include <stdio.h>

/* BIOS/XDC Include Files. */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>

/* Mailbox Driver: */
#include <ti/drv/mailbox/mailbox.h>
#include <ti/drv/mailbox/src/mailbox_internal.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* None. */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

static void Test_appCallbackFunction(Mbox_Handle handle, Mailbox_Instance remoteEndpoint);

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None. */

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */
uint32_t gTestPatternWordReceive_0=0xAABBCCDD;
uint32_t gTestPatternWordReceive_1=0x01020304;
uint32_t gTestPatternWordReceive_2=0x05060708;
uint32_t gTestPatternWordReceive_3=0xFA12FA12;

uint32_t gTestPatternWordSend_0=0x11111111;
uint32_t gTestPatternWordSend_1=0x22222222;
uint32_t gTestPatternWordSend_2=0xFFFFFFFF;
uint32_t gTestPatternWordSend_3=0x88887777;

uint32_t volatile gTestFailFlag = 0;

volatile uint32_t testAppCallbackFlag=0;

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */
void Test_initTask(UArg arg0, UArg arg1)
{
    Mailbox_initParams initParam;
    Mailbox_openParams openParam;
    Mbox_Handle  handleDss;
    int32_t         errCode;
    uint32_t        bufferRx;
    uint32_t        bufferTx[3];
    uint32_t        size;
    Mailbox_Driver *driver;

    /* Setup the default Mailbox init Parameters */
    Mailbox_initParams_init(&initParam);
    initParam.localEndpoint = MAILBOX_INST_MSS_CR5A;

    /* Initialize the Mailbox */
    Mailbox_init(&initParam);

    /* Setup the default Mailbox open Parameters */
    Mailbox_openParams_init(&openParam);
    openParam.remoteEndpoint = MAILBOX_INST_DSP;
    openParam.cfg.readMode = MAILBOX_MODE_CALLBACK;
    openParam.cfg.readCallback = Test_appCallbackFunction;

    /* Open the  Instance to DSS */
    handleDss = Mailbox_open(&openParam, &errCode);
    if (handleDss == NULL)
    {
        printf("MSS: Error: Unable to open the Mailbox Instance\n");
        return;
    }
    if (errCode != 0)
    {
        printf("MSS: Error: Unable to open the Mailbox Instance. Error=%d\n",errCode);
        return;
    }
    printf("MSS: Mailbox Instance to DSS %p has been opened successfully\n", handleDss);

    /**************************************************************************
     * Test: Write message to DSS
     **************************************************************************/
    printf("MSS: ************ Writing message 1 to DSS ****************\n");
    memset ((void *)bufferTx, 0, sizeof(bufferTx));
    memcpy ((void *)(&bufferTx[0]), (void *)&gTestPatternWordSend_0, sizeof(gTestPatternWordSend_0));
    memcpy ((void *)(&bufferTx[1]), (void *)&gTestPatternWordSend_1, sizeof(gTestPatternWordSend_1));
    memcpy ((void *)(&bufferTx[2]), (void *)&gTestPatternWordSend_2, sizeof(gTestPatternWordSend_2));

    size = Mailbox_write(handleDss, (uint8_t*)bufferTx, sizeof(bufferTx));
    printf("MSS: Ack received from DSS for message 1. Size=%d\n",size);
    if(size != sizeof(bufferTx))
    {
        printf("MSS: Error. Write failed.\n");
        gTestFailFlag = 1;
    }


    /**************************************************************************
     * Test: Wait for message from DSS
     **************************************************************************/
    printf("MSS: ************ Waiting for message 2 from DSS ****************\n");

    /* wait for call back to set flag */
    while (testAppCallbackFlag == 0){}
    printf("MSS: Message 2 received.\n");

    /*read first word*/
    bufferRx = 0;
    size = Mailbox_read(handleDss, (uint8_t *)&bufferRx, 4);
    printf("MSS: Read %d bytes. Val=0x%x \n",size, bufferRx);
    if (bufferRx != gTestPatternWordReceive_0)
    {
        printf("MSS: Error. Pattern mismatch.\n");
        gTestFailFlag = 1;
    }

    /*read second word*/
    bufferRx = 0;
    size = Mailbox_read(handleDss, (uint8_t *)&bufferRx, 4);
    printf("MSS: Read %d bytes. Val=0x%x \n",size, bufferRx);
    if (bufferRx != gTestPatternWordReceive_1)
    {
        printf("MSS: Error. Pattern mismatch.\n");
        gTestFailFlag = 1;
    }

    /*read third word*/
    bufferRx = 0;
    size = Mailbox_read(handleDss, (uint8_t *)&bufferRx, 4);
    printf("MSS: Read %d bytes. Val=0x%x \n",size, bufferRx);
    if (bufferRx != gTestPatternWordReceive_2)
    {
        printf("MSS: Error. Pattern mismatch.\n");
        gTestFailFlag = 1;
    }

    printf("MSS: Flushing read\n");
    Mailbox_readFlush(handleDss);
    testAppCallbackFlag = 0;

    /**************************************************************************
     * Test: Write message to DSS
     **************************************************************************/
    printf("MSS: ************ Writing message 3 to DSS ****************\n");
    memset ((void *)bufferTx, 0, sizeof(bufferTx));
    memcpy ((void *)(&bufferTx[0]), (void *)&gTestPatternWordSend_3, sizeof(gTestPatternWordSend_3));
    size = Mailbox_write(handleDss, (uint8_t*)bufferTx, sizeof(gTestPatternWordSend_3));
    printf("MSS: Ack received from DSS for message 3. Size=%d\n",size);
    if(size != sizeof(gTestPatternWordSend_3))
    {
        printf("MSS: Error. Write failed.\n");
        gTestFailFlag = 1;
    }

    /**************************************************************************
     * Test: Wait for message from DSS
     **************************************************************************/
    printf("MSS: ************ Waiting for message 4 from DSS ****************\n");

    /* wait for call back to set flag */
    while (testAppCallbackFlag == 0){}
    printf("MSS: Message 4 received.\n");

    /*read first word*/
    bufferRx = 0;
    size = Mailbox_read(handleDss, (uint8_t *)&bufferRx, 4);
    printf("MSS: Read %d bytes. Val=0x%x \n",size, bufferRx);
    if (bufferRx != gTestPatternWordReceive_3)
    {
        printf("MSS: Error. Pattern mismatch.\n");
        gTestFailFlag = 1;
    }

    printf("MSS: Flushing read\n");
    Mailbox_readFlush(handleDss);
    testAppCallbackFlag = 0;


    /**************************************************************************
     * Test: Check driver internal stats
     **************************************************************************/
    driver = (Mailbox_Driver*)handleDss;
    printf("MSS: Checking driver internal stats\n");
    if( driver->txBoxStatus       != MAILBOX_TX_BOX_EMPTY            ||
        driver->txCount           != 2                               ||
        driver->rxCount           != 2                               ||
        driver->newMessageFlag    != MAILBOX_NEW_MESSAGE_NOT_RECEIVED||
        driver->numBytesRead      != 0                               ||
        driver->boxFullIsrCount   != 2                               ||
        driver->boxEmptyIsrCount  != 2                               ||
        driver->readFlushCount    != 2)
    {
        printf("MSS: Error. Internal stats failed.\n");
        printf("MSS: txBoxStatus      =%d\n",driver->txBoxStatus     );
        printf("MSS: txCount          =%d\n",driver->txCount         );
        printf("MSS: rxCount          =%d\n",driver->rxCount         );
        printf("MSS: newMessageFlag   =%d\n",driver->newMessageFlag  );
        printf("MSS: numBytesRead     =%d\n",driver->numBytesRead    );
        printf("MSS: boxFullIsrCount  =%d\n",driver->boxFullIsrCount );
        printf("MSS: boxEmptyIsrCount =%d\n",driver->boxEmptyIsrCount);
        printf("MSS: readFlushCount   =%d\n",driver->readFlushCount  );
        gTestFailFlag = 1;
    }
    else
    {
        printf("MSS: stats OK for DSS mailbox instance.\n");
    }

    if (Mailbox_close(handleDss) != 0)
    {
        printf("MSS: Error: Failed to close Dss mailbox\n");
        gTestFailFlag = 1;
        return;
    }

    /**************************************************************************
     * Test END
     **************************************************************************/
    printf("MSS: TEST ENDED\n");
    if(gTestFailFlag == 0)
    {
        printf("MSS: Test Passed\n");
    }
    else
    {
        printf("MSS: Test Failed\n");
    }

    /* Exit BIOS */
    BIOS_exit(0);

    return;
}

int main (void)
{
    Task_Params    taskParams;

    /* Initialize the Task Parameters. */
    Task_Params_init(&taskParams);
    taskParams.stackSize = 4*1024;
    Task_create(Test_initTask, &taskParams, NULL);

    /* Start BIOS */
    BIOS_start();
    return 0;
}

static void Test_appCallbackFunction(Mbox_Handle handle, Mailbox_Instance remoteEndpoint)
{
    testAppCallbackFlag=1;
}
