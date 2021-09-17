/*
*
* Copyright (c) 2021 Texas Instruments Incorporated
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
 *  \file main_rtos.c
 *
 *  \brief Main file for TI-RTOS build
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include <ti/csl/arch/csl_arch.h>
#include <ti/csl/soc.h>
#include <ti/csl/cslr.h>
#include <ti/board/board.h>

#include <ti/osal/osal.h>
#include <ti/osal/TaskP.h>

#include "lpm_ipc.h"
#include "lpm_boot.h"
#include "lpm_pmic.h"
#include "print_utils.h"

#include <ti/drv/sciclient/sciserver_tirtos.h>

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
static void BootApp_TaskFxn(void* a0, void* a1);

static void McuOnlyApp_TaskFxn(void* a0, void* a1);
extern uint32_t McuOnly_App();

static void IpcApp_TaskFxn(void* a0, void* a1);
extern int32_t Ipc_echo_test();

int32_t SetupSciServer(void);
/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

TaskP_Handle bootTask;
TaskP_Params bootTaskParams;
static uint8_t BootApp_TaskStack[APP_TASK_STACK] __attribute__((aligned(32)));
/**< Stack for the Boot task */

TaskP_Handle mcuOnlyTask;
TaskP_Params mcuOnlyTaskParams;
static uint8_t McuOnly_TaskStack[APP_TASK_STACK] __attribute__((aligned(32)));
/**< Stack for the MCU Only task */
SemaphoreP_Handle mcuOnlySyncSemHandle;
SemaphoreP_Params mcuOnlySyncSemParams;
/**< Sync semaphore for MCU ONLY task */

TaskP_Handle ipcTask;
TaskP_Params ipcTaskParams;
static uint8_t IpcApp_TaskStack[APP_TASK_STACK] __attribute__((aligned(32)));
/**< Stack for the IPC task */
SemaphoreP_Handle ipcSyncSemHandle;
SemaphoreP_Params ipcSyncSemParams;
/**< Sync semaphore for IPC task */

/* ========================================================================== */
/*                            External Variables                              */
/* ========================================================================== */

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */
int main(void)
{
    int32_t ret = CSL_PASS;

    OS_init();

    Board_init(BOARD_INIT_UART_STDIO);

    Sciclient_init(NULL_PTR);
    /* Initialize SCI Client Server */
    ret = SetupSciServer();
    if(ret != CSL_PASS)
    {
        OS_stop();
    }

    mcu_timer_init();

    AppUtils_Printf(MSG_NORMAL, "\nMCU R5F App started at %d usecs\r\n", (uint32_t)get_usec_timestamp());

    /* Initialize the task params */
    TaskP_Params_init(&bootTaskParams);
    //bootTaskParams.instance->name = BOOT_DEMO_TASK_NAME;
    bootTaskParams.priority       = BOOT_TASK_PRIORITY;
    bootTaskParams.stack          = BootApp_TaskStack;
    bootTaskParams.stacksize      = sizeof (BootApp_TaskStack);

    bootTask = TaskP_create(BootApp_TaskFxn, &bootTaskParams);
    if (NULL == bootTask)
    {
        //AppUtils_Printf(MSG_NORMAL, "\nBoot Task creation failed\r\n");
        OS_stop();
    }

    /* Initialize the task params */
    TaskP_Params_init(&mcuOnlyTaskParams);
    //mcuOnlyTaskParams.instance->name = MCU_ONLY_DEMO_TASK_NAME;
    mcuOnlyTaskParams.priority       = MCU_ONLY_TASK_PRIORITY;
    mcuOnlyTaskParams.stack          = McuOnly_TaskStack;
    mcuOnlyTaskParams.stacksize      = sizeof (McuOnly_TaskStack);

    mcuOnlyTask = TaskP_create(McuOnlyApp_TaskFxn, &mcuOnlyTaskParams);
    if (NULL == mcuOnlyTask)
    {
        //AppUtils_Printf(MSG_NORMAL, "\nMCU Only Task creation failed\r\n");
        OS_stop();
    }

    SemaphoreP_Params_init(&mcuOnlySyncSemParams);
    //mcuOnlySyncSemParams.instance->name = "mcuOnlySyncSem";
    mcuOnlySyncSemHandle = SemaphoreP_create(0, &mcuOnlySyncSemParams);
    if (NULL == mcuOnlySyncSemHandle)
    {
        //AppUtils_Printf(MSG_NORMAL, "\nMCU Only Task Sync semaphore creation failed\r\n");
        OS_stop();
    }

    /* Initialize the task params */
    TaskP_Params_init(&ipcTaskParams);
    ipcTaskParams.priority       = IPC_TASK_PRIORITY;
    ipcTaskParams.stack          = IpcApp_TaskStack;
    ipcTaskParams.stacksize      = sizeof (IpcApp_TaskStack);

    ipcTask = TaskP_create(IpcApp_TaskFxn, &ipcTaskParams);
    if (NULL == ipcTask)
    {
        //AppUtils_Printf(MSG_NORMAL, "\nIPC Task creation failed\r\n");
        OS_stop();
    }

    SemaphoreP_Params_init(&ipcSyncSemParams);
    //ipcSyncSemParams.instance->name = "mcuOnlySyncSem";
    ipcSyncSemHandle = SemaphoreP_create(0, &ipcSyncSemParams);
    if (NULL == ipcSyncSemHandle)
    {
        //AppUtils_Printf(MSG_NORMAL, "\nIPC Task Sync semaphore creation failed\r\n");
        OS_stop();
    }

    OS_start();    /* does not return */

    return(0);
}

static void McuOnlyApp_TaskFxn(void* a0, void* a1)
{
    uint64_t timeMcuOnlyAppStart, timeMcuOnlyAppFinish;

    AppUtils_Printf(MSG_NORMAL, "Waiting for Boot Task to complete...\n");
    SemaphoreP_pend(mcuOnlySyncSemHandle, osal_WAIT_FOREVER);
    AppUtils_Printf(MSG_NORMAL, "Boot Task to completed!\n");

    timeMcuOnlyAppStart = get_usec_timestamp();
    McuOnly_App();
    timeMcuOnlyAppFinish = get_usec_timestamp();

    AppUtils_Printf(MSG_NORMAL, "\nMCU Only Task started at %d usecs and finished at %d usecs\r\n",
                    (uint32_t)timeMcuOnlyAppStart,
                    (uint32_t)timeMcuOnlyAppFinish);

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
        //AppUtils_Printf(MSG_NORMAL, "\nBoot Task re-creation failed\r\n");
        OS_stop();
    }

    TaskP_delete(&ipcTask);
    /* Initialize the task params */
    TaskP_Params_init(&ipcTaskParams);
    ipcTaskParams.priority       = IPC_TASK_PRIORITY;
    ipcTaskParams.stack          = IpcApp_TaskStack;
    ipcTaskParams.stacksize      = sizeof (IpcApp_TaskStack);

    ipcTask = TaskP_create(IpcApp_TaskFxn, &ipcTaskParams);
    if (NULL == ipcTask)
    {
        //AppUtils_Printf(MSG_NORMAL, "\nIPC Task re-creation failed\r\n");
        OS_stop();
    }

    SemaphoreP_delete(ipcSyncSemHandle);
    SemaphoreP_Params_init(&ipcSyncSemParams);
    //ipcSyncSemParams.instance->name = "mcuOnlySyncSem";
    ipcSyncSemHandle = SemaphoreP_create(0, &ipcSyncSemParams);
    if (NULL == ipcSyncSemHandle)
    {
        //AppUtils_Printf(MSG_NORMAL, "\nIPC Task Sync semaphore creation failed\r\n");
        OS_stop();
    }

    return;
}

static void BootApp_TaskFxn(void* a0, void* a1)
{
    uint64_t timeBootAppStart, timeBootAppFinish;

    AppUtils_Printf(MSG_NORMAL, "Waiting for IPC Task to complete...\n");
    SemaphoreP_pend(ipcSyncSemHandle, osal_WAIT_FOREVER);
    AppUtils_Printf(MSG_NORMAL, "IPC Task to completed!\n");

    timeBootAppStart = get_usec_timestamp();
    Boot_App();
    timeBootAppFinish = get_usec_timestamp();

    AppUtils_Printf(MSG_NORMAL, "\nMCU Boot Task started at %d usecs and finished at %d usecs\r\n",
                    (uint32_t)timeBootAppStart,
                    (uint32_t)timeBootAppFinish);

    TaskP_sleep(1000);
    AppUtils_Printf(MSG_NORMAL, "Calling rpmsg_exit_responseTask\n");
    rpmsg_exit_responseTask();
    TaskP_sleep(1000);
    AppUtils_Printf(MSG_NORMAL, "responder task should have exited\n");

    /* Post a semaphore to start the MCU ONLY task */
    SemaphoreP_post(mcuOnlySyncSemHandle);

    return;
}

static void IpcApp_TaskFxn(void* a0, void* a1)
{
    uint64_t timeIPCStart, timeIPCFinish;

    timeIPCStart = get_usec_timestamp();
    Ipc_echo_test();
    timeIPCFinish = get_usec_timestamp();

    AppUtils_Printf(MSG_NORMAL, "\nIPC Task started at %d usecs and finished at %d usecs\r\n",
                    (uint32_t)timeIPCStart,
                    (uint32_t)timeIPCFinish);

    TaskP_sleep(5*1000);
    /* Post a semaphore to start the IPC task */
    SemaphoreP_post(ipcSyncSemHandle);

    return;
}

int32_t SetupSciServer(void)
{

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

    return ret;
}
