/*
 *  Copyright (C) 2018-2021 Texas Instruments Incorporated
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

#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <ti/osal/osal.h>
#include <ti/osal/DebugP.h>
#include <ti/osal/HwiP.h>
#include <ti/osal/SemaphoreP.h>
#include <ti/osal/TaskP.h>
#include <FreeRTOS.h>
#include <task.h>
#include <ti/csl/soc.h>

TaskHandle_t TaskP_getFreertosHandle(TaskP_Handle handle);

/*
 * IMPORTANT NOTES:
 *
 * Alignment of stack is not strictly needed for R5F but debug is easier if stack is nicely
 * aligned.
 *
 * Task priority, 0 is lowest priority, configMAX_PRIORITIES-1 is highest
 * For this example any valid task priority can be set.
 *
 * See FreeRTOSConfig.h for configMAX_PRIORITIES and StackType_t type.
 * FreeRTOSConfig.h can be found under kernel/freertos/config/${device}/${cpu}/
 *
 * In this example,
 * We create task's, semaphore's, ISR's and stack for the tasks using static allocation.
 * We dont need to delete these semaphore's since static allocation is used.
 *
 * One MUST not return out of a FreeRTOS task instead one MUST call vTaskDelete instead.
 */

#define NUM_TASK_SWITCHES      (1000000u)

#if defined (SOC_TPR12) || defined (SOC_AWR294X)
    #if defined(BUILD_MCU1_0) || defined(BUILD_MCU1_1)
        #define PING_INT_NUM           (236u)
        #define PONG_INT_NUM           (237u)
    #endif
    #ifdef BUILD_C66X_1
        #define PING_INT_NUM           (8u)
        #define PING_EVT_ID            (CSL_DSS_INTR_DSS_RTIB_0)
        #define PONG_INT_NUM           (9u)
        #define PONG_EVT_ID            (CSL_DSS_INTR_DSS_RTIB_1)
    #endif
#endif

#ifdef SOC_AM65XX
    #if defined (BUILD_MCU1_0) || defined (BUILD_MCU1_1)
        #define PING_INT_NUM           (CSL_MCU0_INTR_MAIN2MCU_LVL_INTR0_OUTL_0)
        #define PONG_INT_NUM           (CSL_MCU0_INTR_MAIN2MCU_LVL_INTR0_OUTL_1)
    #endif
#endif

#ifdef SOC_J721E
    #ifdef BUILD_MCU1_0
        #define PING_INT_NUM           (CSLR_MCU_R5FSS0_CORE0_INTR_MAIN2MCU_LVL_INTRTR0_OUTL_0)
        #define PONG_INT_NUM           (CSLR_MCU_R5FSS0_CORE0_INTR_MAIN2MCU_LVL_INTRTR0_OUTL_1)
    #endif
    #ifdef BUILD_MCU1_1
        #define PING_INT_NUM           (CSLR_MCU_R5FSS0_CORE1_INTR_MAIN2MCU_LVL_INTRTR0_OUTL_0)
        #define PONG_INT_NUM           (CSLR_MCU_R5FSS0_CORE1_INTR_MAIN2MCU_LVL_INTRTR0_OUTL_1)
#endif
    #ifdef BUILD_MCU2_0
        #define PING_INT_NUM           (CSLR_R5FSS0_CORE0_INTR_R5FSS0_INTROUTER0_OUTL_0)
        #define PONG_INT_NUM           (CSLR_R5FSS0_CORE0_INTR_R5FSS0_INTROUTER0_OUTL_1)
    #endif
    #ifdef BUILD_MCU2_1
        #define PING_INT_NUM           (CSLR_R5FSS0_CORE1_INTR_R5FSS0_INTROUTER0_OUTL_0)
        #define PONG_INT_NUM           (CSLR_R5FSS0_CORE1_INTR_R5FSS0_INTROUTER0_OUTL_1)
    #endif
    #ifdef BUILD_MCU3_0
        #define PING_INT_NUM           (CSLR_R5FSS1_CORE0_INTR_R5FSS1_INTROUTER0_OUTL_0)
        #define PONG_INT_NUM           (CSLR_R5FSS1_CORE0_INTR_R5FSS1_INTROUTER0_OUTL_1)
    #endif
    #ifdef BUILD_MCU3_1
        #define PING_INT_NUM           (CSLR_R5FSS1_CORE1_INTR_R5FSS1_INTROUTER0_OUTL_0)
        #define PONG_INT_NUM           (CSLR_R5FSS1_CORE1_INTR_R5FSS1_INTROUTER0_OUTL_1)
    #endif
#endif

#ifdef SOC_J7200
    #ifdef BUILD_MCU1_0
        #define PING_INT_NUM           (CSLR_MCU_R5FSS0_CORE0_INTR_MAIN2MCU_LVL_INTRTR0_OUTL_0)
        #define PONG_INT_NUM           (CSLR_MCU_R5FSS0_CORE0_INTR_MAIN2MCU_LVL_INTRTR0_OUTL_1)
    #endif
    #ifdef BUILD_MCU1_1
        #define PING_INT_NUM           (CSLR_MCU_R5FSS0_CORE1_INTR_MAIN2MCU_LVL_INTRTR0_OUTL_0)
        #define PONG_INT_NUM           (CSLR_MCU_R5FSS0_CORE1_INTR_MAIN2MCU_LVL_INTRTR0_OUTL_1)
#endif
    #ifdef BUILD_MCU2_0
        #define PING_INT_NUM           (CSLR_R5FSS0_CORE0_INTR_COMPUTE_CLUSTER0_MSMC_EN_SOC_EVENTS_OUT_LEVEL_0)
        #define PONG_INT_NUM           (CSLR_R5FSS0_CORE0_INTR_COMPUTE_CLUSTER0_MSMC_EN_SOC_EVENTS_OUT_LEVEL_1)
    #endif
    #ifdef BUILD_MCU2_1
        #define PING_INT_NUM           (CSLR_R5FSS0_CORE1_INTR_COMPUTE_CLUSTER0_MSMC_EN_SOC_EVENTS_OUT_LEVEL_0)
        #define PONG_INT_NUM           (CSLR_R5FSS0_CORE1_INTR_COMPUTE_CLUSTER0_MSMC_EN_SOC_EVENTS_OUT_LEVEL_1)
    #endif
#endif

#define PING_TASK_PRI  (2u)
#define PONG_TASK_PRI  (3u)

#define PING_TASK_SIZE (4096u)
StackType_t gPingTaskStack[PING_TASK_SIZE] __attribute__((aligned(32)));

#define PONG_TASK_SIZE (4096u)
StackType_t gPongTaskStack[PONG_TASK_SIZE] __attribute__((aligned(32)));

TaskP_Handle gPingTask;
SemaphoreP_Handle gPingSem;

TaskP_Handle gPongTask;
SemaphoreP_Handle gPongSem;

static void ping_isr(uintptr_t arg)
{
    SemaphoreP_post(gPongSem);
}

static void pong_isr(uintptr_t arg)
{
    SemaphoreP_post(gPingSem);
}

void ping_main(void *args)
{
    uint32_t count; /* loop `count` times */
    volatile uint64_t curTime; /* time in units of 10's of usecs */

    DebugP_log0("\r\n");
    DebugP_log0("[FreeRTOS] ping task ... start !!!\r\n");
    { /* switch between ping and pong tasks using semaphores */
        count = NUM_TASK_SWITCHES;
        curTime = uiPortGetRunTimeCounterValue();
        while(count--)
        {
            SemaphoreP_post(gPongSem);
            SemaphoreP_pend(gPingSem, SemaphoreP_WAIT_FOREVER);
        }
        curTime = uiPortGetRunTimeCounterValue() - curTime;
        curTime *= 10;

        DebugP_log0("\r\n");
        DebugP_log1("execution time for task switches = %" PRId64 " us\r\n", curTime);
        DebugP_log1("number of task switches = %" PRId32 " \r\n", (uint32_t)NUM_TASK_SWITCHES*2);
        DebugP_log1("time per task switch (semaphore give/take) = %" PRId32 " ns\r\n", (uint32_t)(curTime*1000/(NUM_TASK_SWITCHES*2)));
    }
    { /* switch between ping and pong tasks using direct-to-task notifications */
        count = NUM_TASK_SWITCHES;
        curTime = uiPortGetRunTimeCounterValue();
        while(count--)
        {
            xTaskNotifyGive( TaskP_getFreertosHandle(gPongTask)); /* wake up pong task */
            ulTaskNotifyTake( pdTRUE, portMAX_DELAY); /* wait for pong to signal */
        }
        curTime = uiPortGetRunTimeCounterValue() - curTime;
        curTime *= 10;

        DebugP_log0("\r\n");
        DebugP_log1("execution time for task switches = %" PRId64 " us\r\n", curTime);
        DebugP_log1("number of task switches = %" PRId32 " \r\n", (uint32_t)NUM_TASK_SWITCHES*2);
        DebugP_log1("time per task switch (direct-to-task notification give/take) = %" PRId32 " ns\r\n", (uint32_t)(curTime*1000/(NUM_TASK_SWITCHES*2)));
    }
    { /* switch from ping task to ISR to pong task and back to ping task using semaphores, here there is a task switch */
        HwiP_Params hwiParams;
        HwiP_Handle hHwi;
        HwiP_Status hwiStatus;

        HwiP_Params_init(&hwiParams);
#ifdef BUILD_C66X_1
        hwiParams.evtId = PING_EVT_ID;
#endif
        hHwi = HwiP_create(PING_INT_NUM, ping_isr, &hwiParams);
        DebugP_assert(hHwi != NULL);


        count = NUM_TASK_SWITCHES;
        curTime = uiPortGetRunTimeCounterValue();
        while(count--)
        {
            HwiP_post(PING_INT_NUM);
            SemaphoreP_pend(gPingSem, SemaphoreP_WAIT_FOREVER);
        }
        curTime = uiPortGetRunTimeCounterValue() - curTime;
        curTime *= 10;

        hwiStatus = HwiP_delete(hHwi);
        DebugP_assert(hwiStatus == HwiP_OK);

        DebugP_log0("\r\n");
        DebugP_log1("execution time for task - ISR - task - task switches = %" PRId64 " us\r\n", curTime);
        DebugP_log1("number of ISRs = %" PRId32 " \r\n", (uint32_t)NUM_TASK_SWITCHES*2);
        DebugP_log1("time per task - ISR - task switch (semaphore give/take) = %" PRId32 " ns\r\n", (uint32_t)(curTime*1000/(2*NUM_TASK_SWITCHES)));
    }

    /* delay some time, just to show delay works */
    vTaskDelay( 100 / portTICK_PERIOD_MS);
    vTaskDelay( 101 / portTICK_PERIOD_MS);

    DebugP_log0("\r\n");
    DebugP_log0("[FreeRTOS] ping task ... done !!!\r\n");
    DebugP_log0("\r\n");
    DebugP_log0("All tests have passed!!\r\n");
    printf("All tests have passed!!\r\n");
}

void pong_main(void *args)
{
    uint32_t count; /* loop `count` times */

    count = NUM_TASK_SWITCHES;
    while(count--)
    {
        SemaphoreP_pend(gPongSem, SemaphoreP_WAIT_FOREVER);
        SemaphoreP_post(gPingSem);
    }
    count = NUM_TASK_SWITCHES;
    while(count--)
    {
        ulTaskNotifyTake( pdTRUE, portMAX_DELAY); /* wait for ping to signal */
        xTaskNotifyGive( TaskP_getFreertosHandle(gPingTask)); /* wake up ping task */
    }
    {
        HwiP_Params hwiParams;
        HwiP_Handle hHwi;
        HwiP_Status hwiStatus;

        HwiP_Params_init(&hwiParams);
#ifdef BUILD_C66X_1
        hwiParams.evtId = PONG_EVT_ID;
#endif
        hHwi = HwiP_create(PONG_INT_NUM, pong_isr, &hwiParams);
        DebugP_assert(hHwi != NULL);

        count = NUM_TASK_SWITCHES;
        while(count--)
        {
            SemaphoreP_pend(gPongSem, SemaphoreP_WAIT_FOREVER);
            HwiP_post(PONG_INT_NUM);
        }
        hwiStatus = HwiP_delete(hHwi);
        DebugP_assert(hwiStatus == HwiP_OK);
    }
}

void task_switch_main(void *args)
{
    SemaphoreP_Params semParams;
    TaskP_Params      taskParams;

    /* Open drivers to open the UART driver for console */
    //Drivers_open();

    SemaphoreP_Params_init(&semParams);
    semParams.mode = SemaphoreP_Mode_BINARY;

    /* first create the semaphores */
    gPingSem = SemaphoreP_create(0, &semParams);
    configASSERT(gPingSem != NULL);
    gPongSem = SemaphoreP_create(0, &semParams);
    configASSERT(gPongSem != NULL);

    TaskP_Params_init(&taskParams);
    taskParams.name = "pong";
    taskParams.stacksize = PONG_TASK_SIZE;
    taskParams.stack = gPongTaskStack;
    taskParams.priority = PONG_TASK_PRI;
    taskParams.arg0 = NULL;

    gPongTask = TaskP_create(pong_main, &taskParams);
    configASSERT(gPongTask != NULL);

    TaskP_Params_init(&taskParams);
    taskParams.name = "ping";
    taskParams.stacksize = PING_TASK_SIZE;
    taskParams.stack = gPingTaskStack;
    taskParams.priority = PING_TASK_PRI;
    taskParams.arg0 = NULL;
    gPingTask = TaskP_create(ping_main, &taskParams);
    configASSERT(gPingTask != NULL);

    /* Dont close drivers to keep the UART driver open for console */
    /* Drivers_close(); */
}

