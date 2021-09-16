/*
*
* Copyright (c) 2019 Texas Instruments Incorporated
*
* All rights reserved not granted herein.
*
* Limited License.
*
* Texas Instruments Incorporated grants a world-wide, royalty-free, non-exclusive
* license under copyrights and patents it now or hereafter owns or controls to make,
* have made, use, import, offer to sell and sell ("Utilize") this software subject to the
* terms herein.  With respect to the foregoing patent license, such license is granted
* solely to the extent that any such patent is necessary to Utilize the software alone.
* The patent license shall not apply to any combinations which include this software,
* other than combinations with devices manufactured by or for TI ("TI Devices").
* No hardware patent is licensed hereunder.
*
* Redistributions must preserve existing copyright notices and reproduce this license
* (including the above copyright notice and the disclaimer and (if applicable) source
* code license limitations below) in the documentation and/or other materials provided
* with the distribution
*
* Redistribution and use in binary form, without modification, are permitted provided
* that the following conditions are met:
*
* *       No reverse engineering, decompilation, or disassembly of this software is
* permitted with respect to any software provided in binary form.
*
* *       any redistribution and use are licensed by TI for use only with TI Devices.
*
* *       Nothing shall obligate TI to provide you with source code for the software
* licensed and provided to you in object code.
*
* If software source code is provided to you, modification and redistribution of the
* source code are permitted provided that the following conditions are met:
*
* *       any redistribution and use of the source code, including any resulting derivative
* works, are licensed by TI for use only with TI Devices.
*
* *       any redistribution and use of any object code compiled from the source code
* and any resulting derivative works, are licensed by TI for use only with TI Devices.
*
* Neither the name of Texas Instruments Incorporated nor the names of its suppliers
*
* may be used to endorse or promote products derived from this software without
* specific prior written permission.
*
* DISCLAIMER.
*
* THIS SOFTWARE IS PROVIDED BY TI AND TI'S LICENSORS "AS IS" AND ANY EXPRESS
* OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL TI AND TI'S LICENSORS BE LIABLE FOR ANY DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
* OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
* OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
* OF THE POSSIBILITY OF SUCH DAMAGE.
*
*/

/**
 *  \file main_tirtos.c
 *
 *  \brief Main file for TI-RTOS build
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include <ti/csl/arch/csl_arch.h>
#include <ti/csl/soc.h>
#include <ti/csl/cslr.h>

#include <ti/osal/osal.h>
#include <ti/osal/TaskP.h>

#include "lpm_ipc.h"
#include "print_utils.h"

#if (defined (BUILD_MCU1_0) && (defined (SOC_J721E) || defined (SOC_J7200)))
#include <ti/drv/sciclient/sciserver_tirtos.h>
#endif

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* Test application stack size */
#define APP_TASK_STACK                  (10U * 1024U)
/**< Stack required for the stack */
#define BOOT_DEMO_TASK_NAME             ("BOOT MAIN DOMAIN")
#define MCU_ONLY_DEMO_TASK_NAME             ("MCU ONLY TASK")
/**< Task names */
#define BOOT_TASK_PRIORITY              (2)
#define MCU_ONLY_TASK_PRIORITY          (5)
#define IPC_TASK_PRIORITY               (3)
/**< Task Priority Levels */

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */
#if defined(CAN_PROFILE_TASK_ENABLED) || defined(CAN_RESP_TASK_ENABLED)
#    if defined(CAN_PROFILE_TASK_ENABLED)
static void CanApp_ProfileTaskFxn(void* a0, void* a1);
#    endif
#    if defined(CAN_RESP_TASK_ENABLED)
static void CanApp_FastResponseTaskFxn(void* a0, void* a1);
#    endif
#endif
#if defined(BOOT_TASK_ENABLED)
static void BootApp_TaskFxn(void* a0, void* a1);
#endif
#if defined(MCU_ONLY_TASK_ENABLED)
static void McuOnlyApp_TaskFxn(void* a0, void* a1);
extern uint32_t McuOnly_App();
#endif
#if defined(IPC_TASK_ENABLED)
static void IpcApp_TaskFxn(void* a0, void* a1);
extern int32_t Ipc_echo_test();
#endif

int32_t SetupSciServer(void);
/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

#if defined(CAN_PROFILE_TASK_ENABLED)
static canAppTaskObj_t  CanApp_TestPrms;
/**< Test parameters */
#endif
#if defined(CAN_PROFILE_TASK_ENABLED) || defined(CAN_RESP_TASK_ENABLED)
TaskP_Handle canTask;
TaskP_Params canTaskParams;
static uint8_t CanApp_TaskStack[APP_TASK_STACK] __attribute__((aligned(32)));
/**< Stack for the CAN task */
#endif
#if defined(BOOT_TASK_ENABLED)
TaskP_Handle bootTask;
TaskP_Params bootTaskParams;
static uint8_t BootApp_TaskStack[APP_TASK_STACK] __attribute__((aligned(32)));
/**< Stack for the Boot task */
#endif
#if defined(MCU_ONLY_TASK_ENABLED)
TaskP_Handle mcuOnlyTask;
TaskP_Params mcuOnlyTaskParams;
static uint8_t McuOnly_TaskStack[APP_TASK_STACK] __attribute__((aligned(32)));
/**< Stack for the MCU Only task */
SemaphoreP_Handle mcuOnlySyncSemHandle;
SemaphoreP_Params mcuOnlySyncSemParams;
/**< Sync semaphore for MCU ONLY task */
#endif
#if defined(IPC_TASK_ENABLED)
TaskP_Handle ipcTask;
TaskP_Params ipcTaskParams;
static uint8_t IpcApp_TaskStack[APP_TASK_STACK] __attribute__((aligned(32)));
/**< Stack for the IPC task */
SemaphoreP_Handle ipcSyncSemHandle;
SemaphoreP_Params ipcSyncSemParams;
/**< Sync semaphore for IPC task */
#endif

/* ========================================================================== */
/*                            External Variables                              */
/* ========================================================================== */

#if defined(CAN_PROFILE_TASK_ENABLED) || defined(CAN_RESP_TASK_ENABLED)
extern uint32 CanIf_DrvStatus;
/**< CAN IF Driver Status, defined in CanIf.c */
#endif

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */
int main(void)
{
    int32_t ret = CSL_PASS;

    OS_init();

#if defined(UART_ENABLED)
    AppUtils_Init();
#endif
    Sciclient_init(NULL_PTR);
    /* Initialize SCI Client Server */
    ret = SetupSciServer();
    if(ret != CSL_PASS)
    {
        OS_stop();
    }

#if defined(MULTICORE_PROFILE_TIMER_ENABLED)
    mcu_timer_init();
#endif

#if defined(UART_ENABLED) && defined(UART_PRINT_DEBUG)
    AppUtils_Printf(MSG_NORMAL, "\nMCU R5F App started at %d usecs\r\n", (uint32_t)get_usec_timestamp());
#endif

#if defined(CAN_PROFILE_TASK_ENABLED) || defined(CAN_RESP_TASK_ENABLED)
    CanApp_Startup();
    CanApp_PowerAndClkSrc();
    CanApp_PlatformInit();

    /* Initialize dummy CAN IF */
    CanIf_Init(NULL);

    if (CANIF_DRV_INITIALIZED == CanIf_DrvStatus)
    {
#    if defined(CAN_PROFILE_TASK_ENABLED)
        /* Initialize the task params */
        TaskP_Params_init(&canTaskParams);
        //canTaskParams.instance->name = CAN_PROFILE_DEMO_TASK_NAME;
        canTaskParams.priority       = CAN_TASK_PRIORITY;
        canTaskParams.stack          = CanApp_TaskStack;
        canTaskParams.stacksize      = sizeof (CanApp_TaskStack);

        canTask = TaskP_create(CanApp_ProfileTaskFxn, &canTaskParams);

#    endif
#    if defined(CAN_RESP_TASK_ENABLED)
        /* Initialize the task params */
        TaskP_Params_init(&canTaskParams);
        //canTaskParams.instance->name = CAN_RESP_DEMO_TASK_NAME;
        canTaskParams.priority       = CAN_TASK_PRIORITY;
        canTaskParams.stack          = CanApp_TaskStack;
        canTaskParams.stacksize      = sizeof (CanApp_TaskStack);

        canTask = TaskP_create(CanApp_FastResponseTaskFxn, &canTaskParams);
#    endif
        if (NULL == canTask)
        {
#    if defined(UART_ENABLED)
            AppUtils_Printf(MSG_NORMAL, "\nCAN Task creation failed\r\n");
#    endif
            OS_stop();
        }
    }
#endif /* defined(CAN_PROFILE_TASK_ENABLED) || defined(CAN_RESP_TASK_ENABLED) */

#if defined(BOOT_TASK_ENABLED)
    /* Initialize the task params */
    TaskP_Params_init(&bootTaskParams);
    //bootTaskParams.instance->name = BOOT_DEMO_TASK_NAME;
    bootTaskParams.priority       = BOOT_TASK_PRIORITY;
    bootTaskParams.stack          = BootApp_TaskStack;
    bootTaskParams.stacksize      = sizeof (BootApp_TaskStack);

    bootTask = TaskP_create(BootApp_TaskFxn, &bootTaskParams);
    if (NULL == bootTask)
    {
#    if defined(UART_ENABLED)
        AppUtils_Printf(MSG_NORMAL, "\nBoot Task creation failed\r\n");
#    endif
        OS_stop();
    }
#endif

#if defined(MCU_ONLY_TASK_ENABLED)
    /* Initialize the task params */
    TaskP_Params_init(&mcuOnlyTaskParams);
    //mcuOnlyTaskParams.instance->name = MCU_ONLY_DEMO_TASK_NAME;
    mcuOnlyTaskParams.priority       = MCU_ONLY_TASK_PRIORITY;
    mcuOnlyTaskParams.stack          = McuOnly_TaskStack;
    mcuOnlyTaskParams.stacksize      = sizeof (McuOnly_TaskStack);

    mcuOnlyTask = TaskP_create(McuOnlyApp_TaskFxn, &mcuOnlyTaskParams);
    if (NULL == mcuOnlyTask)
    {
#    if defined(UART_ENABLED)
        AppUtils_Printf(MSG_NORMAL, "\nMCU Only Task creation failed\r\n");
#    endif
        OS_stop();
    }

    SemaphoreP_Params_init(&mcuOnlySyncSemParams);
    //mcuOnlySyncSemParams.instance->name = "mcuOnlySyncSem";
    mcuOnlySyncSemHandle = SemaphoreP_create(0, &mcuOnlySyncSemParams);
    if (NULL == mcuOnlySyncSemHandle)
    {
#    if defined(UART_ENABLED)
        AppUtils_Printf(MSG_NORMAL, "\nMCU Only Task Sync semaphore creation failed\r\n");
#    endif
        OS_stop();
    }

#endif

#if defined(IPC_TASK_ENABLED)
    /* Initialize the task params */
    TaskP_Params_init(&ipcTaskParams);
    ipcTaskParams.priority       = IPC_TASK_PRIORITY;
    ipcTaskParams.stack          = IpcApp_TaskStack;
    ipcTaskParams.stacksize      = sizeof (IpcApp_TaskStack);

    ipcTask = TaskP_create(IpcApp_TaskFxn, &ipcTaskParams);
    if (NULL == ipcTask)
    {
#    if defined(UART_ENABLED)
        AppUtils_Printf(MSG_NORMAL, "\nIPC Task creation failed\r\n");
#    endif
        OS_stop();
    }

    SemaphoreP_Params_init(&ipcSyncSemParams);
    //ipcSyncSemParams.instance->name = "mcuOnlySyncSem";
    ipcSyncSemHandle = SemaphoreP_create(0, &ipcSyncSemParams);
    if (NULL == ipcSyncSemHandle)
    {
#    if defined(UART_ENABLED)
        AppUtils_Printf(MSG_NORMAL, "\nIPC Task Sync semaphore creation failed\r\n");
#    endif
        OS_stop();
    }

#endif

    OS_start();    /* does not return */

    return(0);
}

#if defined(MCU_ONLY_TASK_ENABLED)
static void McuOnlyApp_TaskFxn(void* a0, void* a1)
{
    uint64_t timeMcuOnlyAppStart, timeMcuOnlyAppFinish;

    AppUtils_Printf(MSG_NORMAL, "Waiting for Boot Task to complete...\n");
    SemaphoreP_pend(mcuOnlySyncSemHandle, osal_WAIT_FOREVER);
    AppUtils_Printf(MSG_NORMAL, "Boot Task to completed!\n");

    timeMcuOnlyAppStart = get_usec_timestamp();
    McuOnly_App();
    timeMcuOnlyAppFinish = get_usec_timestamp();

#ifdef UART_ENABLED
    AppUtils_Printf(MSG_NORMAL, "\nMCU Only Task started at %d usecs and finished at %d usecs\r\n",
                    (uint32_t)timeMcuOnlyAppStart,
                    (uint32_t)timeMcuOnlyAppFinish);
#endif

#if defined(BOOT_TASK_ENABLED)
    TaskP_delete(&bootTask);
    /* Initialize the task params */
    TaskP_Params_init(&bootTaskParams);
    //bootTaskParams.instance->name = BOOT_DEMO_TASK_NAME;
    bootTaskParams.priority       = BOOT_TASK_PRIORITY;
    bootTaskParams.stack          = BootApp_TaskStack;
    bootTaskParams.stacksize      = sizeof (BootApp_TaskStack);

    bootTask = TaskP_create(BootApp_TaskFxn, &bootTaskParams);
    if (NULL == bootTask)
    {
#    if defined(UART_ENABLED)
        AppUtils_Printf(MSG_NORMAL, "\nBoot Task re-creation failed\r\n");
#    endif
        OS_stop();
    }
#endif

#if defined(IPC_TASK_ENABLED)
    TaskP_delete(&ipcTask);
    /* Initialize the task params */
    TaskP_Params_init(&ipcTaskParams);
    ipcTaskParams.priority       = IPC_TASK_PRIORITY;
    ipcTaskParams.stack          = IpcApp_TaskStack;
    ipcTaskParams.stacksize      = sizeof (IpcApp_TaskStack);

    ipcTask = TaskP_create(IpcApp_TaskFxn, &ipcTaskParams);
    if (NULL == ipcTask)
    {
#    if defined(UART_ENABLED)
        AppUtils_Printf(MSG_NORMAL, "\nIPC Task re-creation failed\r\n");
#    endif
        OS_stop();
    }

    SemaphoreP_delete(ipcSyncSemHandle);
    SemaphoreP_Params_init(&ipcSyncSemParams);
    //ipcSyncSemParams.instance->name = "mcuOnlySyncSem";
    ipcSyncSemHandle = SemaphoreP_create(0, &ipcSyncSemParams);
    if (NULL == ipcSyncSemHandle)
    {
#    if defined(UART_ENABLED)
        AppUtils_Printf(MSG_NORMAL, "\nIPC Task Sync semaphore creation failed\r\n");
#    endif
        OS_stop();
    }

#endif

    return;
}
#endif

#if defined(BOOT_TASK_ENABLED)
static void BootApp_TaskFxn(void* a0, void* a1)
{
    uint64_t timeBootAppStart, timeBootAppFinish;

    AppUtils_Printf(MSG_NORMAL, "Waiting for IPC Task to complete...\n");
    SemaphoreP_pend(ipcSyncSemHandle, osal_WAIT_FOREVER);
    AppUtils_Printf(MSG_NORMAL, "IPC Task to completed!\n");

    timeBootAppStart = get_usec_timestamp();
    Boot_App();
    timeBootAppFinish = get_usec_timestamp();

#ifdef UART_ENABLED
    AppUtils_Printf(MSG_NORMAL, "\nMCU Boot Task started at %d usecs and finished at %d usecs\r\n",
                    (uint32_t)timeBootAppStart,
                    (uint32_t)timeBootAppFinish);
#endif

    TaskP_sleep(1000);
    AppUtils_Printf(MSG_NORMAL, "Calling rpmsg_exit_responseTask\n");
    rpmsg_exit_responseTask();
    TaskP_sleep(1000);
    AppUtils_Printf(MSG_NORMAL, "responder task should have exited\n");

#if defined(MCU_ONLY_TASK_ENABLED)
    /* Post a semaphore to start the MCU ONLY task */
    SemaphoreP_post(mcuOnlySyncSemHandle);
#endif

    return;
}
#endif

#if defined(CAN_PROFILE_TASK_ENABLED)
static void CanApp_ProfileTaskFxn(void* a0, void* a1)
{
    Utils_prfInit();

    Utils_prfLoadRegister (TaskP_self(), CAN_PROFILE_DEMO_TASK_NAME);

    CanApp_TestPrms.numTxPackets = APP_NUM_MSG_PER_ITERATION;
    CanApp_TestPrms.numIterations = APP_NUM_ITERATION;

    CanApp_ProfileTest(&CanApp_TestPrms);

    TaskP_yield();

    CanApp_Shutdown();

    Utils_prfLoadUnRegister (TaskP_self());
    Utils_prfDeInit();
    return;
}
#endif

#if defined(CAN_RESP_TASK_ENABLED)
static void CanApp_FastResponseTaskFxn(void* a0, void* a1)
{
    CanApp_ResponseTest();

    TaskP_yield();

    CanApp_Shutdown();

    return;
}
#endif /* CAN_RESP_TASK_ENABLED */

#if defined(IPC_TASK_ENABLED)
static void IpcApp_TaskFxn(void* a0, void* a1)
{
    uint64_t timeMcuOnlyAppStart, timeMcuOnlyAppFinish;

    timeMcuOnlyAppStart = get_usec_timestamp();
    Ipc_echo_test();
    timeMcuOnlyAppFinish = get_usec_timestamp();

#ifdef UART_ENABLED
    AppUtils_Printf(MSG_NORMAL, "\nIPC Task started at %d usecs and finished at %d usecs\r\n",
                    (uint32_t)timeMcuOnlyAppStart,
                    (uint32_t)timeMcuOnlyAppFinish);
#endif

    TaskP_sleep(5*1000);
    /* Post a semaphore to start the IPC task */
    SemaphoreP_post(ipcSyncSemHandle);

    return;
}
#endif /* IPC_TASK_ENABLED */

int32_t SetupSciServer(void)
{

#if (defined (BUILD_MCU1_0) && (defined (SOC_J721E) || defined (SOC_J7200)))
    Sciserver_TirtosCfgPrms_t appPrms;
    Sciclient_ConfigPrms_t clientPrms;
    int32_t ret = CSL_PASS;

    appPrms.taskPriority[SCISERVER_TASK_USER_LO] = 6;
    appPrms.taskPriority[SCISERVER_TASK_USER_HI] = 7;

    /* Sciclient needs to be initialized before Sciserver. Sciserver depends on
     * Sciclient API to execute message forwarding */
    ret = Sciclient_configPrmsInit(&clientPrms);
    //clientPrms.opModeFlag = SCICLIENT_SERVICE_OPERATION_MODE_INTERRUPT;

    if (ret == CSL_PASS)
    {
        ret = Sciclient_init(&clientPrms);
    }

    if (ret == CSL_PASS)
    {
        ret = Sciserver_tirtosInit(&appPrms);
    }

    if (ret == CSL_PASS)
    {
        AppUtils_Printf(MSG_NORMAL, "Starting Sciserver..... PASSED\n");
    }
    else
    {
        AppUtils_Printf(MSG_NORMAL, "Starting Sciserver..... FAILED\n");
    }

#endif
    return ret;
}
