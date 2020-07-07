/*
 * Copyright (C) 2020 Texas Instruments Incorporated - http://www.ti.com/
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of Texas Instruments Incorporated nor the names of
 * its contributors may be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

/**************************************************************************
 *************************** Include Files ********************************
 **************************************************************************/

/* Standard Include Files. */
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

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

/* PDK Include Files: */
#include <ti/drv/watchdog/watchdog.h>
#include <ti/drv/esm/esm.h>
#include <ti/csl/soc/tpr12/src/cslr_intr_esm_mss.h>

/**************************************************************************
 *************************** Global Definitions ***************************
 **************************************************************************/
/** \brief Number of messages sent */
#define WATCHDOG_APP_TEST_RESET          1
#define WATCHDOG_APP_TEST_INTERRUPT      2
#define WATCHDOG_APP_TEST_DSP_INTERRUPT  3

/* Global Variables */
volatile uint32_t       testSelection = 0;
volatile uint32_t       gWatchdogInt = 0;
volatile uint32_t       gWatchdogDspInt = 0;
Watchdog_Handle         watchdogHandle;
ESM_Handle              esmHandle;

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

/**
 *  @b Description
 *  @n
 *      Application implemented callback function to handle watchdog interrupt.
 *
 *   @param[in] handle
 *      Handle to the Watchdog driver
 *  @retval
 *      Not Applicable.
 */
static void watchdogCallback(Watchdog_Handle handle)
{
    gWatchdogInt++;
    if (gWatchdogInt < 10)
    {
        Watchdog_clear(watchdogHandle);
    }
    return;
}

/**
 *  @b Description
 *  @n
 *      Application implemented callback function to handle DSS ESM group errors. When the watchdog timeout causes
 *      a NMI to occur on DSS, that event inturn triggers a group1 ESM error on the MSS. The application can register
 *      a callback using the ESM notifier to handle these errors.
 *
 *   @param[in] arg
 *      Argument which is registered with the ESM module while registering the notify function.
 *  @retval
 *      Not Applicable.
 */
static void DSPNotifyMSSCallback(void* arg)
{
    gWatchdogDspInt++;
    return;
}

static int32_t watchdogNotifyTest()
{
    ESM_NotifyParams        notifyParams;
    int32_t                 errCode = 0;
    int32_t                 retVal = 0;

    notifyParams.groupNumber = 1;
    notifyParams.errorNumber = ESMG1_DSS_ESM_HI_INT;
    notifyParams.arg = NULL;
    notifyParams.notify = DSPNotifyMSSCallback;
    retVal = ESM_registerNotifier (esmHandle, &notifyParams, &errCode);
    if (retVal < 0)
    {
        printf ("Error: ESM Register Notifier failed\n");
        return -1;
    }
    else
    {
        printf ("Debug: ESM Register Notifier registered\n");
    }

    while (gWatchdogDspInt == 0);

    printf ("Debug: DSP Watchdog Timeout received......Exiting\n");

    return 0;
}

/**
 *  @b Description
 *  @n
 *      Configuration and functional watchdog driver test.
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
static int32_t watchdogTest()
{
    Watchdog_Params     watchdogParams;
    int32_t             errCode = 0;
    Watchdog_ResetMode  resetMode;
    Watchdog_WindowSize windowSize;

    /* Initializa the Watchdog driver */
    Watchdog_init();

    /* Initialize the Watchdog driver default parameters */
    Watchdog_Params_init(&watchdogParams);

    if (testSelection == WATCHDOG_APP_TEST_RESET)
    {
        watchdogParams.resetMode = Watchdog_RESET_ON;
    }
    if (testSelection == WATCHDOG_APP_TEST_INTERRUPT)
    {
        watchdogParams.resetMode = Watchdog_RESET_OFF;
        watchdogParams.callbackFxn = watchdogCallback;
    }

    watchdogParams.debugStallMode = Watchdog_DEBUG_STALL_ON;
    watchdogParams.windowSize = Watchdog_WINDOW_100_PERCENT;
    watchdogParams.preloadValue = 20;
    watchdogParams.esmHandle = esmHandle;


    /* Open the Watchdog driver */
    watchdogHandle = Watchdog_open(0, &watchdogParams);

    if (watchdogHandle == NULL)
    {
        printf ("Error: Watchdog Driver Open failed\n");
        return -1;
    }
    if (testSelection == WATCHDOG_APP_TEST_RESET)
    {
        /* Wait for the watchdog to expire and trigger a warm reset */
        while (1);
    }
    else if (testSelection == WATCHDOG_APP_TEST_INTERRUPT)
    {
        while (gWatchdogInt == 0);
        printf ("Debug: Watchdog Driver NMI received\n");
    }

    /* Configure the Watchdog reset mode ON */
    resetMode = Watchdog_RESET_ON;
    errCode = Watchdog_control (watchdogHandle, WATCHDOG_CMD_RESETMODE, (void* )&resetMode);
    if (errCode < 0)
    {
        printf ("Error: Watchdog control Set Reset Mode failed [Error code %d]\n", errCode);
        return -1;
    }

    /* Configure the Watchdog reset mode OFF */
    resetMode = Watchdog_RESET_OFF;
    errCode = Watchdog_control (watchdogHandle, WATCHDOG_CMD_RESETMODE, (void* )&resetMode);
    if (errCode < 0)
    {
        printf ("Error: Watchdog control Set Reset Mode failed [Error code %d]\n", errCode);
        return -1;
    }

    /* Configure the Watchdog Window size 50% */
    windowSize = Watchdog_WINDOW_50_PERCENT;
    errCode = Watchdog_control (watchdogHandle, WATCHDOG_CMD_WINDOWSIZE, (void* )&windowSize);
    if (errCode < 0)
    {
        printf ("Error: Watchdog control Set Window size failed [Error code %d]\n", errCode);
        return -1;
    }

    /* Configure the Watchdog Window size 25% */
    windowSize = Watchdog_WINDOW_25_PERCENT;
    errCode = Watchdog_control (watchdogHandle, WATCHDOG_CMD_WINDOWSIZE, (void* )&windowSize);
    if (errCode < 0)
    {
        printf ("Error: Watchdog control Set Window size failed [Error code %d]\n", errCode);
        return -1;
    }

    /* Configure the Watchdog Window size 12.5% */
    windowSize = Watchdog_WINDOW_12_5_PERCENT;
    errCode = Watchdog_control (watchdogHandle, WATCHDOG_CMD_WINDOWSIZE, (void* )&windowSize);
    if (errCode < 0)
    {
        printf ("Error: Watchdog control Set Window size failed [Error code %d]\n", errCode);
        return -1;
    }

    /* Configure the Watchdog Window size 6.25% */
    windowSize = Watchdog_WINDOW_6_25_PERCENT;
    errCode = Watchdog_control (watchdogHandle, WATCHDOG_CMD_WINDOWSIZE, (void* )&windowSize);
    if (errCode < 0)
    {
        printf ("Error: Watchdog control Set Window size failed [Error code %d]\n", errCode);
        return -1;
    }

    /* Configure the Watchdog Window size 3.125% */
    windowSize = Watchdog_WINDOW_3_125_PERCENT;
    errCode = Watchdog_control (watchdogHandle, WATCHDOG_CMD_WINDOWSIZE, (void* )&windowSize);
    if (errCode < 0)
    {
        printf ("Error: Watchdog control Set Window size failed [Error code %d]\n", errCode);
        return -1;
    }

    return 0;
}

/**
 *  @b Description
 *  @n
 *      System Initialization Task which initializes the various
 *      components in the system.
 *
 *  @retval
 *      Not Applicable.
 */
static void Test_initTask(UArg arg0, UArg arg1)
{
    int32_t         retVal = 0;

    while (1)
    {
        printf ("********************************************************\n");
        printf ("Watchdog Unit Test                                      \n");
        printf ("Please select the type of test to execute:  \n");
        printf ("1. Watchdog reset test                      \n");
        printf ("2. Watchdog interrupt CPU test              \n");
        printf ("3. Watchdog DSP interrupt CPU test          \n");
        printf ("*******************************************************\n");
        printf ("> Enter your selection: ");

        scanf("%d", &testSelection);

        while (testSelection == 0);

        /* Validate the selection: */
        if ((testSelection >= WATCHDOG_APP_TEST_RESET) && (testSelection <= WATCHDOG_APP_TEST_DSP_INTERRUPT))
            break;
    }
    if (testSelection == WATCHDOG_APP_TEST_RESET)
    {
        printf("Debug: Testing watchdog reset mode\n");
        retVal = watchdogTest();
    }
    else if (testSelection == WATCHDOG_APP_TEST_INTERRUPT)
    {
        printf("Debug: Testing watchdog interrupt mode\n");
        retVal = watchdogTest();
    }
    else if (testSelection == WATCHDOG_APP_TEST_DSP_INTERRUPT)
    {
        printf("Debug: Waiting for watchdog interrupt from DSP\n");
        retVal = watchdogNotifyTest();
    }

    if (retVal < 0)
    {
        printf("Debug: Watchdog testing failed\n");
    }
    else
    {
        printf("Debug: Watchdog testing passed\n");
    }

    /* Exit BIOS */
    BIOS_exit(0);

    return;
}

/**
 *  @b Description
 *  @n
 *      This is the entry point into the unit test code
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t main (void)
{
    Task_Params     taskParams;

    /* Initialize the ESM: Dont clear errors as TI RTOS does it */

    esmHandle = ESM_init(0U);
    if (esmHandle == NULL)
    {
        printf ("Error: ESM Module Initialization failed\n");
        return -1;
    }

    /* Initialize the Task Parameters. */
    Task_Params_init(&taskParams);
    taskParams.stackSize = 6*1024;
    Task_create(Test_initTask, &taskParams, NULL);

    /* Start BIOS */
    BIOS_start();
    return 0;
}

