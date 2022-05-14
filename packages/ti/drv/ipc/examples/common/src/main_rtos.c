/*
 *  Copyright (c) Texas Instruments Incorporated 2018
 *  All rights reserved.
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
 *  \file main_tirtos.c
 *
 *  \brief Main file for TI-RTOS build
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <ti/drv/ipc/examples/common/src/ipc_setup.h>

#include <ti/osal/osal.h>
#include <ti/osal/TaskP.h>

#include "ipc_utils.h"
#include <ti/csl/csl_types.h>
#if defined (BUILD_C7X)
#include <ti/csl/soc.h>
#include <ti/csl/csl_clec.h>
#include <ti/csl/arch/csl_arch.h>

#include <ti/osal/soc/osal_soc.h>

#if (defined (FREERTOS))
#include <ti/kernel/freertos/portable/TI_CGT/c7x/Hwi.h>
#include <ti/kernel/freertos/portable/TI_CGT/c7x/Mmu.h>
#else
#include <ti/sysbios/family/c7x/Hwi.h>
#include <ti/sysbios/family/c7x/Mmu.h>
#endif
#endif

#include <ti/drv/sciclient/sciclient.h>
#include <ti/board/board.h>

#if (defined (BUILD_MCU1_0) && (defined (SOC_J721E) || defined (SOC_J7200) || defined (SOC_J721S2)))
#include <ti/drv/sciclient/src/sciclient/sciclient_priv.h>
#include <ti/drv/sciclient/sciserver_tirtos.h>
#endif

/* This needs to be enabled only for negative test cases */
#ifdef IPC_NEGATIVE_TEST
#include <ti/drv/ipc/examples/ipc_negative_test/ipc_neg_setup.h>
#endif
/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#define APP_TSK_STACK_MAIN              (32U * 1024U)
/**< Test application stack size */

#if (defined (BUILD_MCU1_0) && (defined (SOC_J721E) || defined (SOC_J7200) || defined (SOC_J721S2)))
/**< SCI Server Init Task stack size */
#define APP_SCISERVER_INIT_TSK_STACK        (32U * 1024U)
/* SCI Server Init Task Priority - must be higher than High priority Sciserver task */
#define IPC_INIT_SCISERVER_TASK_PRI         (6)
#endif

/* High Priority for SCI Server - must be higher than Low priority task */
#define IPC_SETUP_SCISERVER_TASK_PRI_HIGH   (5)
/*
 * Low Priority for SCI Server - must be higher than IPC echo test tasks
 * to prevent delay in handling Sciserver requests when test is performing
 * multicore ping/pong.
 */
#define IPC_SETUP_SCISERVER_TASK_PRI_LOW    (4)

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

static void taskFxn(void* a0, void* a1);

#if (defined (BUILD_MCU1_0) && (defined (SOC_J721E) || defined (SOC_J7200) || defined (SOC_J721S2)))
void Ipc_setupSciServer(void);
/**< Initialize SCI Server, to process RM/PM Requests by other cores */
#endif

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* Test application stack */
#if defined(SAFERTOS)
static uint8_t  gAppTskStackMain[APP_TSK_STACK_MAIN]
__attribute__ ((aligned(APP_TSK_STACK_MAIN)));
#else
static uint8_t  gAppTskStackMain[APP_TSK_STACK_MAIN]
__attribute__ ((aligned(8192)));
#endif

/* Variable to check if ipc_boardInit has completed or not*/
uint8_t  gBoardinit=0;

#if (defined (BUILD_MCU1_0) && (defined (SOC_J721E) || defined (SOC_J7200) || defined (SOC_J721S2)))
/* Sciserver Init TAsk stack */
#if defined(SAFERTOS)
static uint8_t  gSciserverInitTskStack[APP_SCISERVER_INIT_TSK_STACK]
__attribute__ ((aligned(APP_SCISERVER_INIT_TSK_STACK)));
#else
static uint8_t  gSciserverInitTskStack[APP_SCISERVER_INIT_TSK_STACK]
__attribute__ ((aligned(8192)));
#endif
extern Sciclient_ServiceHandle_t gSciclientHandle;
#endif

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */


void ipc_initSciclient()
{
    int32_t ret = CSL_PASS;
    Sciclient_ConfigPrms_t        config;

    /* Now reinitialize it as default parameter */
    ret = Sciclient_configPrmsInit(&config);
    if (ret != CSL_PASS)
    {
        App_printf("Sciclient_configPrmsInit Failed\n");
    }

#if (defined (BUILD_MCU1_0) && (defined (SOC_J721E) || defined (SOC_J7200) || defined (SOC_J721S2)))
    if (ret == CSL_PASS)
    {
        ret = Sciclient_boardCfgParseHeader(
            (uint8_t *)SCISERVER_COMMON_X509_HEADER_ADDR,
            &config.inPmPrms, &config.inRmPrms);
        if (ret != CSL_PASS)
        {
            App_printf("Sciclient_boardCfgParseHeader Failed\n");
        }
    }
#endif

    if (ret == CSL_PASS)
    {
        ret = Sciclient_init(&config);
        if (ret != CSL_PASS)
        {
            App_printf("Sciclient_init Failed\n");
        }
#if (defined (BUILD_MCU1_0) && (defined (SOC_J721E) || defined (SOC_J7200) || defined (SOC_J721S2)))
        if (gSciclientHandle.rmBoardConfigComplete == SCICLIENT_FT_PASS)
        {
            App_printf("Sciclient_boardCfgRm init Passed\n");         
        }
        else
        {
            App_printf("Sciclient_boardCfgRm init FAILED!\n");
        }
#endif
    }
}

#if !defined(A72_LINUX_OS)
void ipc_boardInit()
{
    Board_initCfg           boardCfg;

    boardCfg = BOARD_INIT_PINMUX_CONFIG |
               BOARD_INIT_UART_STDIO;
    Board_init(boardCfg);

}
#endif

#if defined (_TMS320C6X)
/* To set C66 timer interrupts */
void ipc_timerInterruptInit(void)
{
    int32_t status = 0;

    struct tisci_msg_rm_irq_set_req     rmIrqReq;
    struct tisci_msg_rm_irq_set_resp    rmIrqResp;

    /* On C66x builds we define OS timer tick in the configuration file to
     * trigger event #21 for C66x_1 and #20 for C66x_2. Map
     * DMTimer 0 interrupt to these events through DMSC RM API.
     */
    rmIrqReq.valid_params           = TISCI_MSG_VALUE_RM_DST_ID_VALID |
                                      TISCI_MSG_VALUE_RM_DST_HOST_IRQ_VALID;
    rmIrqReq.src_index              = 0U;
#if defined(BUILD_C66X_1)
    rmIrqReq.dst_id                 = TISCI_DEV_C66SS0_CORE0;
    rmIrqReq.dst_host_irq           = 21U;
    rmIrqReq.src_id                 = TISCI_DEV_TIMER0;
#elif defined(BUILD_C66X_2)
    rmIrqReq.dst_id                 = TISCI_DEV_C66SS1_CORE0;
    rmIrqReq.dst_host_irq           = 20U;
    rmIrqReq.src_id                 = TISCI_DEV_TIMER1;
#endif
    /* Unused params */
    rmIrqReq.global_event           = 0U;
    rmIrqReq.ia_id                  = 0U;
    rmIrqReq.vint                   = 0U;
    rmIrqReq.vint_status_bit_index  = 0U;
    rmIrqReq.secondary_host         = TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST;

    status = Sciclient_rmIrqSet(&rmIrqReq, &rmIrqResp, SCICLIENT_SERVICE_WAIT_FOREVER);
    if(status != 0)
    {
        App_printf(" ERROR: failed to setup timer interrupt !!!\n" );
    }

    return;
}
#endif

int main(void)
{
    TaskP_Handle task;
    TaskP_Params taskParams;



#if defined ECHO_TEST_BTCM && defined FREERTOS && defined BUILD_MCU
    /* Relocate FreeRTOS Reset Vectors from BTCM*/
    void _freertosresetvectors (void);  
    memcpy((void *)0x0, (void *)_freertosresetvectors, 0x40);
#endif

#if defined ECHO_TEST_BTCM && defined SAFERTOS && defined BUILD_MCU
    /* Relocate FreeRTOS Reset Vectors from BTCM*/
    void _safeRTOSrstvectors (void);
    memcpy((void *)0x0, (void *)_safeRTOSrstvectors, 0x40);
#endif

    OS_init();

    /* Initialize the task params */
    TaskP_Params_init(&taskParams);
    /* Set the task priority higher than the default priority (1) */
    taskParams.priority = 2;
    taskParams.stack        = gAppTskStackMain;
    taskParams.stacksize    = sizeof (gAppTskStackMain);

    task = TaskP_create(taskFxn, &taskParams);
    if(NULL == task)
    {
        OS_stop();
    }
    OS_start();    /* does not return */

    return(0);
}

static void taskFxn(void* a0, void* a1)
{

    /* Initialize SCI Client - It must be called before board init */
    ipc_initSciclient();
#if !defined(A72_LINUX_OS)
    ipc_boardInit();
    gBoardinit=1;
#endif

#if (defined (BUILD_MCU1_0) && (defined (SOC_J721E) || defined (SOC_J7200) || defined (SOC_J721S2)))
    TaskP_Handle sciserverInitTask;
    TaskP_Params sciserverInitTaskParams;

    /* Initialize SCI Client Server */
    TaskP_Params_init(&sciserverInitTaskParams);
    sciserverInitTaskParams.priority     = IPC_INIT_SCISERVER_TASK_PRI;
    sciserverInitTaskParams.stack        = gSciserverInitTskStack;
    sciserverInitTaskParams.stacksize    = sizeof (gSciserverInitTskStack);

    sciserverInitTask = TaskP_create(Ipc_setupSciServer, &sciserverInitTaskParams);
    if(NULL == sciserverInitTask)
    {
        OS_stop();
    }
#endif

#if defined (_TMS320C6X)
    ipc_timerInterruptInit();
#endif

#if defined (_TMS320C6X) && defined FREERTOS
    ipc_cacheMarInit();
#endif

#ifdef IPC_NEGATIVE_TEST
    Ipc_echo_neg_test();
#else
    Ipc_echo_test();
#endif
}

#if defined(BUILD_C7X)
void InitMmu(void)
{
    IpcInitMmu(FALSE);
    IpcInitMmu(TRUE);
    OsalCfgClecAccessCtrl(false);

}
#endif

#if defined(BUILD_MPU)
extern void Osal_initMmuDefault(void);
void InitMmu(void)
{
    Osal_initMmuDefault();
}
#endif

#if (defined (BUILD_MCU1_0) && (defined (SOC_J721E) || defined (SOC_J7200) || defined (SOC_J721S2)))
void Ipc_setupSciServer(void)
{

    Sciserver_TirtosCfgPrms_t appPrms;
    int32_t ret = CSL_PASS;
    char *version_str = NULL;
    char *rmpmhal_version_str = NULL;

    ret = Sciserver_tirtosInitPrms_Init(&appPrms);

    appPrms.taskPriority[SCISERVER_TASK_USER_LO] =
                                            IPC_SETUP_SCISERVER_TASK_PRI_LOW;
    appPrms.taskPriority[SCISERVER_TASK_USER_HI] =
                                            IPC_SETUP_SCISERVER_TASK_PRI_HIGH;

    if (ret == CSL_PASS)
    {
        ret = Sciserver_tirtosInit(&appPrms);
    }

    version_str = Sciserver_getVersionStr();
    rmpmhal_version_str = Sciserver_getRmPmHalVersionStr();
    App_printf("DM Built On: %s %s\n", __DATE__, __TIME__);
    App_printf("Sciserver Version: %s\n", version_str);
    App_printf("RM_PM_HAL Version: %s\n", rmpmhal_version_str);

    if (ret == CSL_PASS)
    {
        App_printf("Starting Sciserver..... PASSED\n");
    }
    else
    {
        App_printf("Starting Sciserver..... FAILED\n");
    }

    return;
}
#endif
