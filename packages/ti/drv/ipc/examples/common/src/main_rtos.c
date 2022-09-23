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
#elif (defined (SAFERTOS))
#include "Mmu.h"
#endif
#endif

#include <ti/drv/sciclient/sciclient.h>
#include <ti/board/board.h>

#if (defined (BUILD_MCU1_0) && (defined (SOC_J721E) || defined (SOC_J7200) || defined (SOC_J721S2) || defined (SOC_J784S4)))
#include <ti/drv/sciclient/src/sciclient/sciclient_priv.h>
#include <ti/drv/sciclient/sciserver_tirtos.h>
#endif

/* This needs to be enabled only for negative test cases */
#ifdef IPC_NEGATIVE_TEST
#include <ti/drv/ipc/examples/rtos/ipc_negative_test/ipc_neg_setup.h>
#endif
/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#define APP_TSK_STACK_MAIN              (32U * 1024U)
/**< Test application stack size */

#if (defined (BUILD_MCU1_0) && (defined (SOC_J721E) || defined (SOC_J7200) || defined (SOC_J721S2) || defined (SOC_J784S4)))
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

/* PSC defines*/
/* This is a HACK to POWER ON DRU's local to C7x */
#if defined ECHO_TEST_BTCM && defined FREERTOS && defined BUILD_MCU && defined SOC_J784S4
#define BOLTON_PSC_BASE             (0x00420000U)

#define PSC_MDCTL00                 (0xA00U)
#define PSC_MDSTAT00                (0x800U)
#define PSC_PDCTL00                 (0x300U)
#define PSC_PDSTAT00                (0x200U)
#define PSC_PTCMD                   (0x120U)
#define PSC_PTSTAT                  (0x128U)
#define PSC_PTCMD_H                 (0x124U)
#define PSC_PTSTAT_H                (0x12CU)

//PSC Parameter definitions
#define PSC_PD_OFF                  (0x0U)
#define PSC_PD_ON                   (0x1U)

#define PSC_SYNCRESETDISABLE        (0x0U)
#define PSC_SYNCRESET               (0x1U)
#define PSC_DISABLE                 (0x2U)
#define PSC_ENABLE                  (0x3U)

#define BOLTON_PSC_MDCTL_BASE       (BOLTON_PSC_BASE + PSC_MDCTL00)
#define BOLTON_PSC_MDSTAT_BASE      (BOLTON_PSC_BASE + PSC_MDSTAT00)
#define BOLTON_PSC_PDCTL_BASE       (BOLTON_PSC_BASE + PSC_PDCTL00)
#define BOLTON_PSC_PDSTAT_BASE      (BOLTON_PSC_BASE + PSC_PDSTAT00)
#define BOLTON_PSC_PTCMD_BASE       (BOLTON_PSC_BASE + PSC_PTCMD)
#define BOLTON_PSC_PTSTAT_BASE      (BOLTON_PSC_BASE + PSC_PTSTAT)
#define BOLTON_PSC_PTCMD            (BOLTON_PSC_PTCMD_BASE)
#define BOLTON_PSC_PTSTAT           (BOLTON_PSC_PTSTAT_BASE)
#define BOLTON_PSC_PTCMD_H          (BOLTON_PSC_BASE + PSC_PTCMD_H)
#define BOLTON_PSC_PTSTAT_H         (BOLTON_PSC_BASE + PSC_PTSTAT_H)

#define PSC_TIMEOUT                 (1000U)

#define BOLT_PD_ANA0                (4U)
#define BOLT_PD_ANA1                (5U)
#define BOLT_PD_ANA2                (6U)
#define BOLT_PD_ANA3                (7U)

#define LPSC_MSMC_L1_0              (14U)
#define LPSC_DRU_0                  (15U)
#define LPSC_ANA_PBIST_0            (16U)
#define LPSC_MSMC_L1_1              (17U)
#define LPSC_DRU_1                  (18U)
#define LPSC_ANA_PBIST_1            (19U)
#define LPSC_MSMC_L1_2              (20U)
#define LPSC_DRU_2                  (21U)
#define LPSC_ANA_PBIST_2            (22U)
#define LPSC_MSMC_L1_3              (23U)
#define LPSC_DRU_3                  (24U)
#define LPSC_ANA_PBIST_3            (25U)
#endif

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

static void taskFxn(void* a0, void* a1);

#if (defined (BUILD_MCU1_0) && (defined (SOC_J721E) || defined (SOC_J7200) || defined (SOC_J721S2) || defined (SOC_J784S4)))
void Ipc_setupSciServer(void *arg0, void *arg1);
/**< Initialize SCI Server, to process RM/PM Requests by other cores */
#endif

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* Test application stack */
/* For SafeRTOS on R5F with FFI Support, task stack should be aligned to the stack size */
#if defined(SAFERTOS) && defined (BUILD_MCU)
static uint8_t  gAppTskStackMain[APP_TSK_STACK_MAIN]
__attribute__ ((aligned(APP_TSK_STACK_MAIN)));
#else
static uint8_t  gAppTskStackMain[APP_TSK_STACK_MAIN]
__attribute__ ((aligned(8192)));
#endif

/* Variable to check if ipc_boardInit has completed or not*/
uint8_t  gBoardinit=0;

#if (defined (BUILD_MCU1_0) && (defined (SOC_J721E) || defined (SOC_J7200) || defined (SOC_J721S2) || defined (SOC_J784S4)))
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

/* This is a HACK to POWER ON DRU's local to C7x */
#if defined ECHO_TEST_BTCM && defined FREERTOS && defined BUILD_MCU && defined SOC_J784S4
int32_t Board_SetPSCState(uint32_t pd_id, uint32_t md_id, uint32_t pd_state, uint32_t md_state)
{
    uint32_t regVal;
    uint32_t mdctl;
    uint32_t mdstat;
    uint32_t pdctl;
    uint32_t pdstat;
    uint32_t ptcmd;
    uint32_t ptstat;

    uint32_t loop_cnt = 0;
    int32_t ret = 0;

    uint32_t address_offset = 0;

    //Added for support beyond 32-domains
    if(pd_id > 31){
        ptcmd = BOLTON_PSC_PTCMD_H;
        ptstat = BOLTON_PSC_PTSTAT_H;
    }else{
        ptcmd = BOLTON_PSC_PTCMD;
        ptstat = BOLTON_PSC_PTSTAT;
    }

    mdctl = (BOLTON_PSC_MDCTL_BASE + ( 4U * md_id ) + address_offset) ;
    mdstat = (BOLTON_PSC_MDSTAT_BASE + ( 4U * md_id )+ address_offset);
    pdctl = (BOLTON_PSC_PDCTL_BASE + ( 4U * pd_id ) + address_offset);
    pdstat = (BOLTON_PSC_PDSTAT_BASE + ( 4U * pd_id )+ address_offset);

    // If state is already set, do nothing
    if ( (( HW_RD_REG32(pdstat) & 0x1 ) == pd_state) && (( HW_RD_REG32(mdstat) & 0x1f ) == md_state) )
    {
        ret = -1;
        return ret;
    }

    // Wait for GOSTAT to clear
    while( (loop_cnt < PSC_TIMEOUT) && (HW_RD_REG32(ptstat+address_offset) & (0x1 << (pd_id % 32))) != 0 )
    {
        loop_cnt++;
    }

    // Check if we got timeout error while waiting
    if (loop_cnt >= PSC_TIMEOUT)
    {
        ret = -1;
        return ret;
    }

    // Set PDCTL NEXT to new state
    regVal = (HW_RD_REG32(pdctl) & ~(0x1)) | pd_state;
    HW_WR_REG32(pdctl, regVal);

    // Set MDCTL NEXT to new state
    regVal = (HW_RD_REG32(mdctl) & ~(0x1f)) | md_state;
    HW_WR_REG32(mdctl, regVal);

    // Start power transition by setting PTCMD GO to 1
    regVal = (HW_RD_REG32(ptcmd+address_offset)) | (0x1<< (pd_id % 32));
    HW_WR_REG32((ptcmd+address_offset), regVal);

    loop_cnt = 0;

    // Wait for PTSTAT GOSTAT to clear
    while( (loop_cnt < PSC_TIMEOUT) && (HW_RD_REG32(ptstat+address_offset) & (0x1 << (pd_id % 32))) != 0 )
    {
        loop_cnt++;
    }

    // Check if we got timeout error while waiting
    if (loop_cnt >= PSC_TIMEOUT)
    {
        ret = -1;
        return ret;
    }

    // Verify power domain and module domain state got changed
    if ( (( HW_RD_REG32(pdstat) & 0x1 ) == pd_state) && (( HW_RD_REG32(mdstat) & 0x1f ) == md_state) )
    {
        ret = 0;
    }
    if (( HW_RD_REG32(pdstat) & 0x1 ) != pd_state)
    {
        ret = -1;
    }
    if (( HW_RD_REG32(mdstat) & 0x1f ) != md_state)
    {
        ret = -1;
    }

    return ret;
}
#endif

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

#if (defined (BUILD_MCU1_0) && (defined (SOC_J721E) || defined (SOC_J7200) || defined (SOC_J721S2) || defined (SOC_J784S4)))
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
#if (defined (BUILD_MCU1_0) && (defined (SOC_J721E) || defined (SOC_J7200) || defined (SOC_J721S2) || defined (SOC_J784S4)))
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

    /*  This should be called before any other OS calls (like Task creation, OS_start, etc..) */
    OS_init();

    /* Initialize the task params */
    TaskP_Params_init(&taskParams);
    /* Set the task priority higher than the default priority (1) */
    taskParams.priority = 2;
    taskParams.stack        = gAppTskStackMain;
    taskParams.stacksize    = sizeof (gAppTskStackMain);

    task = TaskP_create(&taskFxn, &taskParams);
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

#if (defined (BUILD_MCU1_0) && (defined (SOC_J721E) || defined (SOC_J7200) || defined (SOC_J721S2) || defined (SOC_J784S4)))
    TaskP_Handle sciserverInitTask;
    TaskP_Params sciserverInitTaskParams;

    /* Initialize SCI Client Server */
    TaskP_Params_init(&sciserverInitTaskParams);
    sciserverInitTaskParams.priority     = IPC_INIT_SCISERVER_TASK_PRI;
    sciserverInitTaskParams.stack        = gSciserverInitTskStack;
    sciserverInitTaskParams.stacksize    = sizeof (gSciserverInitTskStack);

    sciserverInitTask = TaskP_create(&Ipc_setupSciServer, &sciserverInitTaskParams);
    if(NULL == sciserverInitTask)
    {
        OS_stop();
    }
#endif

#if defined (_TMS320C6X) 
#if defined (FREERTOS) || defined (SAFERTOS)
    ipc_cacheMarInit();
#endif
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

#if (defined (BUILD_MCU1_0) && (defined (SOC_J721E) || defined (SOC_J7200) || defined (SOC_J721S2) || defined (SOC_J784S4)))
void Ipc_setupSciServer(void *arg0, void *arg1)
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

/* This is a HACK to POWER ON DRU's local to C7x */
#if defined ECHO_TEST_BTCM && defined FREERTOS && defined BUILD_MCU && defined SOC_J784S4
    Board_SetPSCState(BOLT_PD_ANA0,LPSC_DRU_0, PSC_PD_ON, PSC_ENABLE);
    Board_SetPSCState(BOLT_PD_ANA1,LPSC_DRU_1, PSC_PD_ON, PSC_ENABLE);
    Board_SetPSCState(BOLT_PD_ANA2,LPSC_DRU_2, PSC_PD_ON, PSC_ENABLE);
    Board_SetPSCState(BOLT_PD_ANA3,LPSC_DRU_3, PSC_PD_ON, PSC_ENABLE);
#endif

    return;
}
#endif
