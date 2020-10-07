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
#include <xdc/std.h>
#include <xdc/runtime/System.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>

#include <ti/osal/DebugP.h>

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
void multiChannelTest (void);

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

/*Global array to keep handles of channels between mss and dss*/
Mbox_Handle  handleArray[MAILBOX_CH_ID_MAX + 1];



/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */
int32_t Test_mailboxWrite(Mbox_Handle handle, uint8_t * buffer, uint32_t bufferSize)
{
    int32_t retVal;

    do {
		retVal = Mailbox_write(handle, buffer, bufferSize);
		if (retVal == MAILBOX_ETXACKTIMEDOUT)
		{
			System_printf("MSS: Error. Write timedout.Rewriting msg \n");
		}
    } while (retVal == MAILBOX_ETXACKTIMEDOUT);
	return retVal;
}

void Test_initTask(UArg arg0, UArg arg1)
{
    Mailbox_initParams initParam;
    Mailbox_openParams openParam;
    Mbox_Handle  handleDss;
    int32_t         errCode;
    uint32_t        bufferRx;
    uint32_t        bufferTx[3];
    int32_t        size;
    Mailbox_Driver *driver;

    /* Setup the default Mailbox init Parameters */
    Mailbox_initParams_init(&initParam);
    initParam.localEndpoint = MAILBOX_INST_MSS_CR5A;

    /* Initialize the Mailbox */
    Mailbox_init(&initParam);

    multiChannelTest();

    /* Setup the default Mailbox open Parameters */
    Mailbox_openParams_init(&openParam);
    openParam.remoteEndpoint = MAILBOX_INST_DSP;
    openParam.cfg.readMode = MAILBOX_MODE_CALLBACK;
    openParam.cfg.readCallback = Test_appCallbackFunction;
    openParam.cfg.writeTimeout = 1000U;

    /* Open the  Instance to DSS */
    handleDss = Mailbox_open(&openParam, &errCode);
    if (handleDss == NULL)
    {
        System_printf("MSS: Error: Unable to open the Mailbox Instance\n");
        return;
    }
    if (errCode != 0)
    {
        System_printf("MSS: Error: Unable to open the Mailbox Instance. Error=%d\n",errCode);
        return;
    }
    System_printf("MSS: Mailbox Instance to DSS %p has been opened successfully\n", handleDss);

    /**************************************************************************
     * Test: Write message to DSS
     **************************************************************************/
    System_printf("MSS: ************ Writing message 1 to DSS ****************\n");
    memset ((void *)bufferTx, 0, sizeof(bufferTx));
    memcpy ((void *)(&bufferTx[0]), (void *)&gTestPatternWordSend_0, sizeof(gTestPatternWordSend_0));
    memcpy ((void *)(&bufferTx[1]), (void *)&gTestPatternWordSend_1, sizeof(gTestPatternWordSend_1));
    memcpy ((void *)(&bufferTx[2]), (void *)&gTestPatternWordSend_2, sizeof(gTestPatternWordSend_2));

    size = Test_mailboxWrite(handleDss, (uint8_t*)bufferTx, sizeof(bufferTx));
    System_printf("MSS: Ack received from DSS for message 1. Size=%d\n",size);
    if(size != sizeof(bufferTx))
    {
        System_printf("MSS: Error. Write failed.\n");
        gTestFailFlag = 1;
    }


    /**************************************************************************
     * Test: Wait for message from DSS
     **************************************************************************/
    System_printf("MSS: ************ Waiting for message 2 from DSS ****************\n");

    /* wait for call back to set flag */
    while (testAppCallbackFlag == 0){}
    System_printf("MSS: Message 2 received.\n");

    /*read first word*/
    bufferRx = 0;
    size = Mailbox_read(handleDss, (uint8_t *)&bufferRx, 4);
    System_printf("MSS: Read %d bytes. Val=0x%x \n",size, bufferRx);
    if (bufferRx != gTestPatternWordReceive_0)
    {
        System_printf("MSS: Error. Pattern mismatch.\n");
        gTestFailFlag = 1;
    }

    /*read second word*/
    bufferRx = 0;
    size = Mailbox_read(handleDss, (uint8_t *)&bufferRx, 4);
    System_printf("MSS: Read %d bytes. Val=0x%x \n",size, bufferRx);
    if (bufferRx != gTestPatternWordReceive_1)
    {
        System_printf("MSS: Error. Pattern mismatch.\n");
        gTestFailFlag = 1;
    }

    /*read third word*/
    bufferRx = 0;
    size = Mailbox_read(handleDss, (uint8_t *)&bufferRx, 4);
    System_printf("MSS: Read %d bytes. Val=0x%x \n",size, bufferRx);
    if (bufferRx != gTestPatternWordReceive_2)
    {
        System_printf("MSS: Error. Pattern mismatch.\n");
        gTestFailFlag = 1;
    }

    System_printf("MSS: Flushing read\n");
    Mailbox_readFlush(handleDss);
    testAppCallbackFlag = 0;

    /**************************************************************************
     * Test: Write message to DSS
     **************************************************************************/
    System_printf("MSS: ************ Writing message 3 to DSS ****************\n");
    memset ((void *)bufferTx, 0, sizeof(bufferTx));
    memcpy ((void *)(&bufferTx[0]), (void *)&gTestPatternWordSend_3, sizeof(gTestPatternWordSend_3));
    size = Test_mailboxWrite(handleDss, (uint8_t*)bufferTx, sizeof(gTestPatternWordSend_3));
    System_printf("MSS: Ack received from DSS for message 3. Size=%d\n",size);
    if(size != sizeof(gTestPatternWordSend_3))
    {
        System_printf("MSS: Error. Write failed.\n");
        gTestFailFlag = 1;
    }

    /**************************************************************************
     * Test: Wait for message from DSS
     **************************************************************************/
    System_printf("MSS: ************ Waiting for message 4 from DSS ****************\n");

    /* wait for call back to set flag */
    while (testAppCallbackFlag == 0){}
    System_printf("MSS: Message 4 received.\n");

    /*read first word*/
    bufferRx = 0;
    size = Mailbox_read(handleDss, (uint8_t *)&bufferRx, 4);
    System_printf("MSS: Read %d bytes. Val=0x%x \n",size, bufferRx);
    if (bufferRx != gTestPatternWordReceive_3)
    {
        System_printf("MSS: Error. Pattern mismatch.\n");
        gTestFailFlag = 1;
    }

    System_printf("MSS: Flushing read\n");
    Mailbox_readFlush(handleDss);
    testAppCallbackFlag = 0;


    /**************************************************************************
     * Test: Check driver internal stats
     **************************************************************************/
    driver = (Mailbox_Driver*)handleDss;
    System_printf("MSS: Checking driver internal stats\n");
    if( driver->txBoxStatus       != MAILBOX_TX_BOX_EMPTY            ||
        driver->txCount           != 2                               ||
        driver->rxCount           != 2                               ||
        driver->newMessageFlag    != MAILBOX_NEW_MESSAGE_NOT_RECEIVED||
        driver->numBytesRead      != 0                               ||
        driver->boxFullIsrCount   != 2                               ||
        driver->boxEmptyIsrCount  != 2                               ||
        driver->readFlushCount    != 2)
    {
        System_printf("MSS: Error. Internal stats failed.\n");
        System_printf("MSS: txBoxStatus      =%d\n",driver->txBoxStatus     );
        System_printf("MSS: txCount          =%d\n",driver->txCount         );
        System_printf("MSS: rxCount          =%d\n",driver->rxCount         );
        System_printf("MSS: newMessageFlag   =%d\n",driver->newMessageFlag  );
        System_printf("MSS: numBytesRead     =%d\n",driver->numBytesRead    );
        System_printf("MSS: boxFullIsrCount  =%d\n",driver->boxFullIsrCount );
        System_printf("MSS: boxEmptyIsrCount =%d\n",driver->boxEmptyIsrCount);
        System_printf("MSS: readFlushCount   =%d\n",driver->readFlushCount  );
        gTestFailFlag = 1;
    }
    else
    {
        System_printf("MSS: stats OK for DSS mailbox instance.\n");
    }

    if (Mailbox_close(handleDss) != 0)
    {
        System_printf("MSS: Error: Failed to close Dss mailbox\n");
        gTestFailFlag = 1;
        return;
    }

    /**************************************************************************
     * Test END
     **************************************************************************/
    System_printf("MSS: TEST ENDED\n");
    if(gTestFailFlag == 0)
    {
        System_printf("MSS: Test Passed\n");
    }
    else
    {
        System_printf("MSS: Test Failed\n");
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

volatile uint32_t testAppCallbackFlag1=0;
static void Test_appCallbackFunction1(Mbox_Handle handle, Mailbox_Instance remoteEndpoint)
{
    testAppCallbackFlag1=1;
}
volatile uint32_t testAppCallbackFlag3=0;
static void Test_appCallbackFunction3(Mbox_Handle handle, Mailbox_Instance remoteEndpoint)
{
    testAppCallbackFlag3=1;
}

void Test_channel1Task(UArg arg0, UArg arg1)
{
    uint32_t        bufferRx;
    uint32_t        size;

    while(1)
    {
        while (testAppCallbackFlag1 == 0)
        {
            Task_sleep(1);
            Task_yield();
        }
        testAppCallbackFlag1 = 0;
        System_printf("MSS: Channel 1 received message.\n");

        /*read first word*/
        bufferRx = 0;
        size = Mailbox_read(handleArray[1], (uint8_t *)&bufferRx, 4);
        System_printf("MSS: Read %d bytes. Val=0x%x \n",size, bufferRx);
        if (bufferRx != gTestPatternWordReceive_1)
        {
            System_printf("MSS: Error. Pattern mismatch.\n");
            gTestFailFlag = 1;
            return;
        }
        Mailbox_readFlush(handleArray[1]);
    }

}

void Test_channel3Task(UArg arg0, UArg arg1)
{
    uint32_t        bufferRx;
    uint32_t        size;

    while(1)
    {
        while (testAppCallbackFlag3 == 0)
        {
            Task_sleep(1);
            Task_yield();
        }
        testAppCallbackFlag3 = 0;
        System_printf("MSS: Channel 3 received message.\n");

        /*read first word*/
        bufferRx = 0;
        size = Mailbox_read(handleArray[3], (uint8_t *)&bufferRx, 4);
        System_printf("MSS: Read %d bytes. Val=0x%x \n",size, bufferRx);
        if (bufferRx != gTestPatternWordReceive_3)
        {
            System_printf("MSS: Error. Pattern mismatch.\n");
            gTestFailFlag = 1;
            return;
        }
        Mailbox_readFlush(handleArray[3]);
    }

}

void Test_channel4Task(UArg arg0, UArg arg1)
{
    uint32_t        bufferRx;
    uint32_t        size;

    while(1)
    {
        size = Mailbox_read(handleArray[4], (uint8_t *)&bufferRx, 4);
        System_printf("MSS: Channel 4 received message.\n");
        System_printf("MSS: Read %d bytes. Val=0x%x \n",size, bufferRx);
        if (bufferRx != gTestPatternWordReceive_2)
        {
            System_printf("MSS: Error. Pattern mismatch.\n");
            gTestFailFlag = 1;
            return;
        }
        Mailbox_readFlush(handleArray[4]);
    }

}

extern Mailbox_MCB gMailboxMCB;

Task_Handle multiChTaskHandle[MAILBOX_CH_ID_MAX];
void multiChannelTest (void)
{
    int32_t         errCode,i,size;
    Task_Params        taskParams;
    uint32_t        bufferRx;
    Mailbox_openParams openParam;
    uint32_t syncData;

    System_printf("*************************************************\n");
    System_printf("\nMSS: Starting Multichannel Test.\n");

    /****** ch 1 *********************************************/
    /* Setup the default Mailbox open Parameters */
    Mailbox_openParams_init(&openParam);
    openParam.remoteEndpoint   = MAILBOX_INST_DSP;
    openParam.cfg.chType       = MAILBOX_CHTYPE_MULTI;
    openParam.cfg.chId         = MAILBOX_CH_ID_1;
    openParam.cfg.readMode     = MAILBOX_MODE_CALLBACK;
    openParam.cfg.readCallback = Test_appCallbackFunction1;
    openParam.cfg.writeMode    = MAILBOX_MODE_POLLING;


    handleArray[1] = Mailbox_open(&openParam, &errCode);
    if (handleArray[1] == NULL)
    {
        System_printf("MSS: Error: Unable to open the Mailbox Instance\n");
        gTestFailFlag = 1;
        return;
    }
    if (errCode != 0)
    {
        System_printf("MSS: Error: Unable to open the Mailbox Instance. Error=%d\n",errCode);
        gTestFailFlag = 1;
        return;
    }
    System_printf("MSS: Mailbox Instance to DSS %p has been opened successfully\n", handleArray[1]);

    /****** ch 3 *********************************************/
    Mailbox_openParams_init(&openParam);
    openParam.remoteEndpoint   = MAILBOX_INST_DSP;
    openParam.cfg.chType       = MAILBOX_CHTYPE_MULTI;
    openParam.cfg.chId         = MAILBOX_CH_ID_3;
    openParam.cfg.readMode     = MAILBOX_MODE_CALLBACK;
    openParam.cfg.readCallback = Test_appCallbackFunction3;
    openParam.cfg.writeMode    = MAILBOX_MODE_BLOCKING;
    openParam.cfg.writeTimeout = 1000U;

    handleArray[3] = Mailbox_open(&openParam, &errCode);
    if (handleArray[3] == NULL)
    {
        System_printf("MSS: Error: Unable to open the Mailbox Instance\n");
        gTestFailFlag = 1;
        return;
    }
    if (errCode != 0)
    {
        System_printf("MSS: Error: Unable to open the Mailbox Instance. Error=%d\n",errCode);
        gTestFailFlag = 1;
        return;
    }
    System_printf("MSS: Mailbox Instance to DSS %p has been opened successfully\n", handleArray[3]);

    /****** ch 4 *********************************************/
    Mailbox_openParams_init(&openParam);
    openParam.remoteEndpoint   = MAILBOX_INST_DSP;
    openParam.cfg.chType       = MAILBOX_CHTYPE_MULTI;
    openParam.cfg.chId         = MAILBOX_CH_ID_4;
    openParam.cfg.readMode     = MAILBOX_MODE_BLOCKING;
    openParam.cfg.writeMode    = MAILBOX_MODE_BLOCKING;
    openParam.cfg.writeTimeout = 1000U;

    handleArray[4] = Mailbox_open(&openParam, &errCode);
    if (handleArray[4] == NULL)
    {
        System_printf("MSS: Error: Unable to open the Mailbox Instance\n");
        gTestFailFlag = 1;
        return;
    }
    if (errCode != 0)
    {
        System_printf("MSS: Error: Unable to open the Mailbox Instance. Error=%d\n",errCode);
        gTestFailFlag = 1;
        return;
    }
    System_printf("MSS: Mailbox Instance to DSS %p has been opened successfully\n", handleArray[4]);

    /****** ch 7 *********************************************/
    Mailbox_openParams_init(&openParam);
    openParam.remoteEndpoint   = MAILBOX_INST_DSP;
    openParam.cfg.chType       = MAILBOX_CHTYPE_MULTI;
    openParam.cfg.chId         = MAILBOX_CH_ID_7;
    openParam.cfg.readMode     = MAILBOX_MODE_BLOCKING;
    openParam.cfg.writeMode    = MAILBOX_MODE_BLOCKING;
    openParam.cfg.writeTimeout = 1000U;

    handleArray[7] = Mailbox_open(&openParam, &errCode);
    if (handleArray[7] == NULL)
    {
        System_printf("MSS: Error: Unable to open the Mailbox Instance\n");
        gTestFailFlag = 1;
        return;
    }
    if (errCode != 0)
    {
        System_printf("MSS: Error: Unable to open the Mailbox Instance. Error=%d\n",errCode);
        gTestFailFlag = 1;
        return;
    }
    System_printf("MSS: Mailbox Instance to DSS %p has been opened successfully\n", handleArray[7]);

    /****** ch 0 for Sync ************************************/
    Mailbox_openParams_init(&openParam);
    openParam.remoteEndpoint = MAILBOX_INST_DSP;
    openParam.cfg.chType       = MAILBOX_CHTYPE_MULTI;
    openParam.cfg.chId         = MAILBOX_CH_ID_0;
    openParam.cfg.readMode     = MAILBOX_MODE_BLOCKING;
    openParam.cfg.writeMode    = MAILBOX_MODE_BLOCKING;
    openParam.cfg.writeTimeout = 1000U;

    handleArray[0] = Mailbox_open(&openParam, &errCode);

    System_printf("MSS: Waiting for sync msg from dss.\n");

    /* wait for Sync msg from remote core. */
    Mailbox_read(handleArray[0], (uint8_t*)&syncData, sizeof(syncData));
    Mailbox_readFlush(handleArray[0]);

    syncData = 0xFFFFFFFFU;
    /* Send sync data to dss core. */
    Test_mailboxWrite(handleArray[0], (uint8_t*)&syncData, sizeof(syncData));

    /***************************************************/

    Task_Params_init(&taskParams);
    taskParams.stackSize = 2*1024;
    multiChTaskHandle[1] = Task_create(Test_channel1Task, &taskParams, NULL);

    Task_Params_init(&taskParams);
    taskParams.stackSize = 2*1024;
    multiChTaskHandle[3] = Task_create(Test_channel3Task, &taskParams, NULL);

    Task_Params_init(&taskParams);
    taskParams.stackSize = 2*1024;
    multiChTaskHandle[4] = Task_create(Test_channel4Task, &taskParams, NULL);

    for(i=0;i<3;i++)
    {
        /* channels 3 and 4 are write blocking so can write back to back*/
        System_printf("MSS: ************ Writing a message in CHANNEL 3 to DSS ****************\n");
        size = Test_mailboxWrite(handleArray[3], (uint8_t*)&gTestPatternWordSend_3, sizeof(gTestPatternWordSend_3));
        if(size != sizeof(gTestPatternWordSend_3))
        {
            System_printf("MSS: Error. Write failed. Error=%d\n",size);
            gTestFailFlag = 1;
        }

        System_printf("MSS: ************ Writing a message in CHANNEL 4 to DSS ****************\n");
        size = Test_mailboxWrite(handleArray[4], (uint8_t*)&gTestPatternWordSend_2, sizeof(gTestPatternWordSend_2));
        if(size != sizeof(gTestPatternWordSend_2))
        {
            System_printf("MSS: Error. Write failed. Error=%d\n",size);
            gTestFailFlag = 1;
        }

    }

    System_printf("MSS: ************ Writing a message in CHANNEL 1 to DSS ****************\n");
    size = Test_mailboxWrite(handleArray[1], (uint8_t*)&gTestPatternWordSend_1, sizeof(gTestPatternWordSend_1));
    if(size != sizeof(gTestPatternWordSend_1))
    {
        System_printf("MSS: Error. Write failed. Error=%d\n",size);
        gTestFailFlag = 1;
    }

    System_printf("MSS: ************ Writing a message in CHANNEL 7 to DSS ****************\n");
    MULTI_CH7_WRITE:
    size = Test_mailboxWrite(handleArray[7], (uint8_t*)&gTestPatternWordSend_0, sizeof(gTestPatternWordSend_0));
    if (size == MAILBOX_ECHINUSE)
    {
        /* Previous write is in polling mode and did not get the Ack.
         * Try write after some time.
         */
        Task_sleep(1);
        goto MULTI_CH7_WRITE;
    }
    if(size != sizeof(gTestPatternWordSend_0))
    {
        System_printf("MSS: Error. Write failed. Error=%d\n",size);
        gTestFailFlag = 1;
    }

    /* Multichannel test end once a message is received in channel 7 which is readBlocking*/
    size = Mailbox_read(handleArray[7], (uint8_t *)&bufferRx, 4);
    System_printf("MSS: Channel 7 received message.\n");
    System_printf("MSS: Read %d bytes. Val=0x%x \n",size, bufferRx);
    if (bufferRx != gTestPatternWordReceive_0)
    {
        System_printf("MSS: Error. Pattern mismatch.\n");
        gTestFailFlag = 1;
        return;
    }
    Mailbox_readFlush(handleArray[7]);

    /*close all channels*/
    for(i=0;i<=MAILBOX_CH_ID_MAX;i++)
    {
        /* Open the  Instance */
        if((i==0) ||(i==1) || (i==3) || (i==4) || (i==7))
        {
            if (Mailbox_close(handleArray[i]) != 0)
            {
                System_printf("MSS: Error: Failed to close instance %d\n",i);
                gTestFailFlag = 1;
            }
            System_printf("MSS: closed instance %d\n",i);
        }
    }

    /*close auxiliary tasks*/
    Task_delete(&multiChTaskHandle[1]);
    Task_delete(&multiChTaskHandle[3]);
    Task_delete(&multiChTaskHandle[4]);

    Task_sleep(4);
}
