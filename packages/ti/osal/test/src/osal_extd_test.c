/**
 *  \file   osal_extd_test.c
 *
 *  \brief  OSAL driver test application main file.
 *
 */

/*
 * Copyright (C) 2023 Texas Instruments Incorporated - http://www.ti.com/
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

/*===========================================================================*/
/*                             Includes                                      */
/*===========================================================================*/

#if !defined(BARE_METAL)
/* include for both safertos and freertos. */
#include <ti/osal/TaskP.h>
#include <ti/osal/EventP.h>
#endif

#if defined (FREERTOS)
#include "FreeRTOS.h"
#include <ti/osal/MemoryP.h>
#include <ti/osal/LoadP.h>
#endif

#if defined (SAFERTOS)
#include "SafeRTOS.h"
#endif

#if defined (BUILD_C7X)
#include <ti/csl/csl_clec.h>
#include <ti/csl/arch/csl_arch.h>
#endif

#if defined (BARE_METAL)
#include <ti/csl/csl_timer.h>
#endif

#include <stdio.h>
#include <string.h>
#include <math.h>

/* TI-RTOS Header files */
#include <ti/osal/osal.h>
#include <ti/osal/soc/osal_soc.h>
#include "OSAL_log.h"
#include "OSAL_board.h"
#include <ti/osal/src/nonos/Nonos_config.h>
#include <ti/osal/SwiP.h>
#include <ti/drv/sciclient/sciclient.h>

#include <ti/csl/soc.h>
#include <ti/csl/tistdtypes.h>
#include <ti/csl/arch/csl_arch.h>

/*===========================================================================*/
/*                             Macros & Typedefs                             */
/*===========================================================================*/

#define TWO_TIMER_INTERRUPT_TEST 0

#undef  ENABLE_GET_TIME_TEST
#define ENABLE_GET_TIME_TEST     1

#ifndef NULL_PTR
#define NULL_PTR ((void *)0x0)
#endif

#define INT_NUM_IRQ 32
#define LOOP_CNT 2
#define HEAP_MAIN 1024U
#define TSK_STACK (32U * 1024U)
/*Semaphore*/
#if defined(FREERTOS)
    #define SEMP_BLOCK_SIZE (1 * OSAL_FREERTOS_SEMAPHOREP_SIZE_BYTES)
#endif

#if defined(SAFERTOS)
    #define SEMP_BLOCK_SIZE (1 * OSAL_SAFERTOS_SEMAPHOREP_SIZE_BYTES)
#endif

#if defined(BARE_METAL)
    #define SEMP_BLOCK_SIZE (1 * OSAL_NONOS_SEMAPHOREP_SIZE_BYTES)
#endif

/* Test application stack size */
#if defined (BUILD_C7X)
/* Temp workaround to avoid assertion failure: A_stackSizeTooSmall : Task stack size must be >= 16KB.
  * until the Bug PDK-7605 is resolved */
#define APP_TSK_STACK_MAIN              (64U * 1024U)
#else
#define APP_TSK_STACK_MAIN              (16U * 1024U)
#endif

#ifdef BARE_METAL
/* No task support for Bare metal */
#else
/* Test application stack */
/* For SafeRTOS on R5F with FFI Support, task stack should be aligned to the stack size */
#if defined(SAFERTOS) && defined (BUILD_MCU)
static uint8_t  gAppTskStackMain[APP_TSK_STACK_MAIN] __attribute__(( aligned( APP_TSK_STACK_MAIN ))) = { 0 };
#else
static uint8_t  gAppTskStackMain[APP_TSK_STACK_MAIN] __attribute__(( aligned( 0x2000 )));
#endif
#endif

#define TaskP_PRIORITY_HIGHEST      ((int8_t)configMAX_PRIORITIES - 1)
#define TaskP_PRIORITY_LOWEST       0

/* ==========================================================================*/
/*                         Structure Declarations                            */
/* ==========================================================================*/

/* None */

/* ==========================================================================*/
/*                          Function Declarations                            */
/* ==========================================================================*/

/* None */

/*===========================================================================*/
/*                           Global Variables                                */
/*===========================================================================*/

volatile uint8_t gclockp_clockfxn_got_execute = 0U;
volatile uint8_t ghwip_isr_got_execute = 0U;
extern uint32_t gOsalHwiAllocCnt;

ClockP_Handle gclockp_handle;

/*Mailbox*/
MailboxP_Handle gmailboxp_handle;/*mailbox handle passed to mailbox post in isr*/
uint8_t gmailbox_str[] = {"Texas Instruments"};
void * gmsg_in_isr = gmailbox_str;/*msg parameter passed to mailbox_post which is getting call in isr*/
uint32_t gmailbox_timeout_isr = 1U;/*timeout passed to mailbox_post in isr*/

volatile uint64_t gTestlocalTimeout = 0x100000U;
volatile bool gFlagSwi = false, gFlagIRQ = false;

/* for HeapP_freertos */
static uint8_t  gHeapPbuf[HEAP_MAIN];

/* for EventP APIs */
EventP_Handle gEventP_handle;
uint32_t geventMask = EventP_ID_01;
extern int gOsalEventPFreeRtosPool[OSAL_FREERTOS_CONFIGNUM_EVENT];
extern uint32_t gOsalEventAllocCnt;
extern uint32_t gOsalQueueAllocCnt;

/*TaskP_RTOS*/
static uint8_t  gTskStack[TSK_STACK];
volatile uint8_t gtaskp_taskpfxn_got_execute = 0U;

/*Semaphore block*/
uint8_t semPMemBlock[SEMP_BLOCK_SIZE]
__attribute__ ((aligned(64)));

/*===========================================================================*/
/*                          Callback Functions                               */
/*===========================================================================*/

/*Callback function for clockp_create and gclockp_clockfxn_got_execute indicates it got called*/
void clockp_clockfxn(void *arg)
{
    gclockp_clockfxn_got_execute = 1U;
}

/*isr which calls ClockP_start and ghwip_isr_got_execute which indicates isr got executed*/
void clockp_start_isr(void *arg)
{
    ghwip_isr_got_execute = 1U;
    ClockP_start(gclockp_handle);
}

/*isr which calls MailboxP_post and ghwip_isr_got_execute which indicates isr got executed*/
void mailboxp_post_isr(void *arg)
{
    ghwip_isr_got_execute = 3U;
    MailboxP_post(gmailboxp_handle,gmsg_in_isr,gmailbox_timeout_isr);
}

/* ISR which is called to check whether the hardware ISR got executed in RegisterIntr_nonos, HwiP_nonos Tests */
void Hwi_IRQ(uintptr_t arg)
{
    gFlagIRQ = true;
    gTestlocalTimeout = 0x100000U;
}

/* ISR for SwiP_nonos Test */
void SwiP_nonosIRQ(uintptr_t arg0, uintptr_t arg1)
{
   gFlagSwi = true;
}

/*isr which calls EventP and ghwip_isr_got_execute which indicates isr got executed*/
void EventP_IRQ(void *arg)
{
    ghwip_isr_got_execute = 4U;
    gTestlocalTimeout = 0x300000U;
    EventP_post(gEventP_handle, geventMask);
    EventP_getPostedEvents(gEventP_handle);
}

void EventP_Neg_IRQ(void *arg)
{
      gTestlocalTimeout = 0x100000U;
      EventP_post(gEventP_handle, geventMask);
}

/*Callback function for taskp_create and gtaskp_taskpfxn_got_execute indicates it got called*/
static void Taskp_TaskpFxn(void *arg)
{
    gtaskp_taskpfxn_got_execute = 1U;
}

/*=============================================================================*/
/*                          Function Definitions                               */
/*=============================================================================*/

/*
 *  ======== Board_initOSAL ========
 */
void Board_initOSAL(void)
{
    Board_initCfg boardCfg;
    Board_STATUS  status;
    boardCfg = BOARD_INIT_PINMUX_CONFIG;
    boardCfg |= BOARD_INIT_MODULE_CLOCK;

    #if defined (UART_CONSOLE)
        boardCfg |= BOARD_INIT_UART_STDIO;
    #endif
    status = Board_init(boardCfg);

    if (BOARD_SOK != status)
    {
        OSAL_log(" Board_init() is not successful...unexpected results may happen \n");
    }

    return;
}

#if defined (BARE_METAL)
/*
 * Description: To test Cycle profile APIs by calling in this function
 */
void CycleprofilerP_nonos_Test(void)
{
    uint32_t time, timeC_high, timeC_low, timeA_high, timeA_low;
    uint64_t timeC, timeA;
    TimeStamp_Struct timestamp64;

    OSAL_log("\n Cycle Profile nonos Test:\n\t  wait for 5 seconds\n");
    CSL_armR5PmuResetCycleCnt();
    CycleprofilerP_init();

    osalArch_TimestampGet64(&timestamp64);
    timeA_high = timestamp64.hi;
    timeA_low = timestamp64.lo;
    time = CycleprofilerP_getTimeStamp();
    DebugP_assert((time - timeA_low) < 2000U);
    timeA = ((uint64_t)timeA_high << 32U) | (timeA_low);

    /* Delay of 2 seconds */
    Osal_delay(2000);

    CycleprofilerP_refreshCounter();

    /* Delay of 3 seconds */
    Osal_delay(3000);


    osalArch_TimestampGet64(&timestamp64);
    timeC_high = timestamp64.hi;
    timeC_low = timestamp64.lo;
    time = CycleprofilerP_getTimeStamp();
    DebugP_assert((time - timeC_low) < 2000U);
    timeC = ((uint64_t)timeC_high << 32U) | (timeC_low);

    uint64_t time_diff = timeC - timeA;
    uint64_t limit_low = ((uint64_t)5*(10000U*100000U));
    uint64_t limit_high = ((uint64_t)51*(10000U*10000U));

    if(limit_low <= time_diff)
    {
      if(time_diff < limit_high)
      {
          OSAL_log("\n CycleprofilerP nonos Test Passed!!\n");
      }
    }
    else
    {
      OSAL_log("\n CycleprofilerP nonos Test Failed!!\n");
    }
}

/*
 * Description: Testing Negative condition check for the below mentioned SwiP APIs :
 *      1. SwiP_Params_init
 *      2. SwiP_create
 *      3. SwiP_post
 *      4. SwiP_delete
 */
void SwiP_nonos_Test(void)
{
    SwiP_Handle handle;
    SwiP_Params swiParams;
    SwiP_Status status;
    bool test_pass = true;

    SwiP_Params_init(&swiParams);
    handle = SwiP_create((SwiP_Fxn)&SwiP_nonosIRQ, &swiParams);
    if (NULL_PTR != handle)
    {
        OSAL_log("\n Failed to create software interrupt \n");
        test_pass = false;
    }

    if(true == test_pass)
    {
        status = SwiP_post(handle);
        if (SwiP_OK != status)
        {
            test_pass = false;
        }
        status = SwiP_delete(&handle);
        if(SwiP_OK == status)
        {
            OSAL_log("\n SwiP nonos Negative Test Passed!! \n");
        }
        else
        {
            OSAL_log("\n SwiP nonos Negative Test Failed!! \n");
        }
    }
    else
    {
            OSAL_log("\n SwiP nonos Negative Test Failed!! \n");
    }
}

/*
 * Description: Testing Negative condition check for the below mentioned APIs :
 *      1. Osal_RegisterInterrupt_initParams
 *      2. Osal_RegisterInterrupt
 *      3. Osal_DeleteInterrupt
 */
void RegisterIntr_nonos_NegTest(void)
{
     HwiP_Handle hwiHandle       = NULL;
     OsalRegisterIntrParams_t    *intrPrms = NULL_PTR;
     OsalInterruptRetCode_e      osalRetVal;
     bool test_pass = false;

     /* Osal_RegisterInterrupt_initParams Null check */
     Osal_RegisterInterrupt_initParams(intrPrms);

     /* Invalid parameters to the OsalRegisterIntrParams_t and Osal_RegisterInterrupt to check the Negative condition*/
     intrPrms->corepacConfig.corepacEventNum = CSL_INVALID_EVENT_ID;
     intrPrms->corepacConfig.isrRoutine      = NULL;
     intrPrms->corepacConfig.intVecNum       = CSL_INVALID_VEC_ID;

     /* Osal_RegisterInterrupt Negative condition check */
     osalRetVal = Osal_RegisterInterrupt(intrPrms, &hwiHandle);
     if(OSAL_INT_SUCCESS != osalRetVal)
     {
         OSAL_log("\n Osal_RegisterInterrupt Negative Test Passed!! \n");
         test_pass = true;
     }
     else
     {
         OSAL_log("\n Osal_RegisterInterrupt Negative Test Failed!! \n");
     }

     if (true == test_pass)
     {
         /* Osal_DeleteInterrupt positive condition check */
         osalRetVal = Osal_DeleteInterrupt(&hwiHandle, intrPrms->corepacConfig.corepacEventNum);
         if(OSAL_INT_SUCCESS == osalRetVal)
         {
             OSAL_log("\n Osal_DeleteInterrupt Positive Testcase Passed!! \n");
         }
         else
         {
             OSAL_log("\n Osal_DeleteInterrupt Positive Testcase Failed!! \n");
         }
         /* Osal_DeleteInterrupt Negative condition check */
         hwiHandle = NULL;
         osalRetVal = Osal_DeleteInterrupt(&hwiHandle, intrPrms->corepacConfig.corepacEventNum);
         if(OSAL_INT_SUCCESS != osalRetVal)
         {
             OSAL_log("\n Osal_DeleteInterrupt Negative Testcase Passed!! \n");
         }
         else
         {
             OSAL_log("\n Osal_DeleteInterrupt Negative Testcase Failed!! \n");
         }
     }
     else
     {
         OSAL_log("\n RegisterIntr nonos Negative Test failed!! \n");
     }
}

/*
 * Description: Null check for below mentioned APIs :
 *      1. HwiP_create
 *      2. HwiP_createDirect
 */
void HwiP_create_nonos_Null_test()
{
    uint32_t      interruptNum = CSL_INVALID_VEC_ID;
    HwiP_Params   *hwipParams = NULL_PTR;
    HwiP_Handle   handle;

    HwiP_Params_init(hwipParams);

    handle = HwiP_create(interruptNum, Hwi_IRQ, hwipParams);
    if (NULL_PTR == handle)
    {
       OSAL_log("\n HwiP_create nonos Null check Passed!! \n");
    }
    else
    {
       OSAL_log("\n HwiP_create nonos Null check Failed!! \n");
    }

    handle = HwiP_createDirect(interruptNum, NULL_PTR, hwipParams);
    if (NULL_PTR == handle)
    {
       OSAL_log("\n HwiP_createDirect nonos Null check Passed!! \n");
    }
    else
    {
       OSAL_log("\n HwiP_createDirect nonos Null check Failed!! \n");
    }
}


/*
 * Description: Hwi Allocation Count Null check for HwiP_delete API
 */
void HwiP_delete_AlloCnt_test()
{
    HwiP_Params hwiParams;
    HwiP_Handle handle;
    int32_t ret;
    bool test_pass = true;

    HwiP_Params_init(&hwiParams);

    handle = HwiP_createDirect(INT_NUM_IRQ, (HwiP_DirectFxn)Hwi_IRQ, &hwiParams);
    if (NULL_PTR == handle) {
     OSAL_log("Failed to create the HwiP handle \n");
     test_pass = false;
    }
    if (true == test_pass)
    {
       gOsalHwiAllocCnt = 0;
       ret = HwiP_delete(handle);
       if (HwiP_OK != ret)
       {
           OSAL_log(" Failed to delete HwiP handle \n");
           test_pass = false;
       }
    }
    if (true == test_pass)
    {
        OSAL_log("\n HwiP_delete_AlloCnt test Passed!! \n");
    }
    else
    {
        OSAL_log("\n HwiP_delete_AlloCnt test Failed!! \n");
    }
}

/*
 * Description: Null check for below mentioned APIs in the Utils_nonos
 *      1. Osal_getHwAttrs
 *      2. Osal_setHwAttrs
 *      3. Osal_getStaticMemStatus
 */
void Utils_nonos_test(void)
{
    Osal_HwAttrs          *hwAttrs = NULL_PTR;
    int32_t               osal_ret;
    uint32_t              ctrlBitMap = ( 0U );
    Osal_StaticMemStatus  *pMemStats = NULL_PTR;
    bool                  test_status = true;

    osal_ret = Osal_getHwAttrs(hwAttrs);
    if (osal_ret != osal_OK)
    {
        OSAL_log("\n Osal_getHwAttrs Null check test Passed!!\n");
    }
    else
    {
          test_status = false;
    }
    osal_ret = Osal_setHwAttrs(ctrlBitMap, hwAttrs);
    if (osal_ret != osal_OK)
    {
        OSAL_log("\n Osal_setHwAttrs Null check test Passed!!\n");
    }
    else
    {
        test_status = false;
    }
    osal_ret = Osal_getStaticMemStatus(pMemStats);
    if (osal_ret != osal_OK)
    {
        OSAL_log("\n Osal_getStaticMemStatus Null check test Passed!!\n");
    }
    else
    {
        test_status = false;
    }
    if(true != test_status)
    {
        OSAL_log("\n Utils_nonos Null check test Failed\n");
    }
}

/*
 * Description: Testing Osal_setHwAttrs by setting the Multi control bit parameter
 *
 */
void Utils_nonos_SetHwAttr_Multi_Test(void)
{
    Osal_HwAttrs      hwAttrs;
    int32_t           osal_ret;
    uint32_t          ctrlBitMap = ( OSAL_HWATTR_SET_EXT_CLK |
                                     OSAL_HWATTR_SET_HWACCESS_TYPE |
                                     OSAL_HWATTR_SET_CPU_FREQ);

    osal_ret = Osal_getHwAttrs(&hwAttrs);

    osal_ret = Osal_setHwAttrs(ctrlBitMap, &hwAttrs);
    if (osal_ret == osal_OK)
    {
        OSAL_log("\n Osal_setHwAttrs Multi Access test Passed!!\n");
    }
    else
    {
        OSAL_log("\n Osal_setHwAttrs Multi Access test Failed!!\n");
    }
}

#endif

#if defined (FREERTOS)
/*
 * Description: Testing Nullcheck condition for the LoadP_getTaskLoad API
 */
void loadP_freertos_test(void)
{
    TaskP_Handle      hLoadTestTask = NULL_PTR;
    LoadP_Stats       *loadStatsTask = NULL_PTR;
    LoadP_Status      status = LoadP_OK;

    LoadP_reset();

    status += LoadP_getTaskLoad(hLoadTestTask, loadStatsTask);
    if (LoadP_OK != status)
    {
        OSAL_log("\n LoadP_getTaskLoad Null check test Passed!!\n");
    }
    else
    {
        OSAL_log("\n LoadP_getTaskLoad Null check test Failed!!\n");
    }
}
#endif

/*
 * Description: To test MutexP nonos API by calling in this function
 */
void MutexP_Test(void)
{
    MutexP_Status         status = MutexP_OK;
    MutexP_Object         mutexObj;
    MutexP_Handle         mutexHandle;
    bool test_pass        = true;

    mutexHandle = MutexP_create(&mutexObj);
    if (NULL_PTR == mutexHandle)
    {
        OSAL_log("\t Failed to create mutex \n");
        test_pass = false;
    }

    if(true == test_pass)
    {
        status = MutexP_lock(mutexHandle, MutexP_WAIT_FOREVER);
        if(MutexP_OK != status)
        {
            OSAL_log("\t Failed to lock mutex \n");
            test_pass = false;
        }
        if (true == test_pass)
        {
            status = MutexP_unlock(mutexHandle);
            if(MutexP_OK != status)
            {
                OSAL_log("\t Failed to unlock mutex \n");
                test_pass = false;
            }
        }
        status = MutexP_delete(mutexHandle);
        if(MutexP_OK != status)
        {
            OSAL_log("\t Failed to delete mutex \n");
            test_pass = false;
        }

        if(true == test_pass)
        {
            OSAL_log("\n MutexP nonos Test Passed!! \n");
        }
    }
    else
    {
        OSAL_log("\n MutexP nonos Test Failed!! \n");
    }
}

/*
 * Description: Testing Null parameter check for MutexP_create API
 */
void MutexP_create_NullCheck(void)
{
    MutexP_Object         *mutexObj = (MutexP_Object *)NULL;
    MutexP_Handle         mutexHandle;

    mutexHandle = MutexP_create(mutexObj);
    if(NULL == mutexHandle)
    {
       OSAL_log("\n MutexP_create Null check Passed!! \n");
    }
    else
    {
        OSAL_log("\n MutexP_create Null check Failed!! \n");
    }
}

/*
 * Description: Testing Negative condition for the Osal_RegisterInterruptDirect API
 */
void RegisterIntrDirect_NegTest(void)
{
    HwiP_Handle hwiHandle       = NULL;
    OsalRegisterIntrParams_t    intrPrms;
    OsalInterruptRetCode_e      osalRetVal;
    HwiP_DirectFxn              isrFxn = (HwiP_DirectFxn)Hwi_IRQ;

    Osal_RegisterInterrupt_initParams(&intrPrms);

    /* Populating invalid interrupt parameters to check the Negative condition */
    intrPrms.corepacConfig.arg             = (uintptr_t) 0;
    intrPrms.corepacConfig.priority        = 1U;
    intrPrms.corepacConfig.corepacEventNum = CSL_INVALID_EVENT_ID;
    intrPrms.corepacConfig.isrRoutine      = NULL;
    intrPrms.corepacConfig.intVecNum       = 0U;

    /* Register interrupt */
    osalRetVal = Osal_RegisterInterruptDirect(&intrPrms, isrFxn, &hwiHandle);

    if (OSAL_INT_SUCCESS == osalRetVal)
    {
        OSAL_log("\n Osal_RegisterInterruptDirect Negative Test failed!! \n");
    }
    else
    {
        OSAL_log("\n Osal_RegisterInterruptDirect Negative Test passed!! \n");
    }
}

void RegisterIntr_pos_Test(void)
{
    HwiP_Handle                 hwiHandle;
    OsalRegisterIntrParams_t    intrPrms;
    OsalInterruptRetCode_e      osalRetVal;
    HwiP_DirectFxn              isrFxn = (HwiP_DirectFxn)Hwi_IRQ;
    bool                        test_pass = true;

    Osal_RegisterInterrupt_initParams(&intrPrms);

    /* Populating invalid interrupt parameters to check the Negative condition */
    intrPrms.corepacConfig.priority        = 1U;
    intrPrms.corepacConfig.corepacEventNum = EventP_ID_NONE;
    intrPrms.corepacConfig.isrRoutine      = Hwi_IRQ;
    intrPrms.corepacConfig.enableIntr      = 0U;

    /* Register interrupt */
    osalRetVal = Osal_RegisterInterruptDirect(&intrPrms, isrFxn, &hwiHandle);

    if (OSAL_INT_SUCCESS != osalRetVal)
    {
        OSAL_log("\n Interrupt not register for the event!! \n");
        test_pass = false;
    }

    Osal_DisableInterrupt(intrPrms.corepacConfig.corepacEventNum, 0U);

    osalRetVal = Osal_DeleteInterrupt(&hwiHandle, intrPrms.corepacConfig.corepacEventNum);
    if(OSAL_INT_SUCCESS != osalRetVal)
    {
        OSAL_log("\n Failed to delete Interrupt corresponding to an event \n");
        test_pass = false;
    }
    if(true == test_pass)
    {
        OSAL_log("\n Register Interrupt positive Testcase Passed!! \n");
    }
    else
    {
        OSAL_log("\n Register Interrupt positive Testcase Failed!! \n");
    }
}

void RegisterIntr_Neg_Test(void)
{
    HwiP_Handle                 hwiHandle = NULL;
    OsalRegisterIntrParams_t    intrPrms;
    OsalInterruptRetCode_e      osalRetVal;
    HwiP_DirectFxn              isrFxn = NULL_PTR;  //(HwiP_DirectFxn)Hwi_IRQ;
    bool                        test_pass = true;

    Osal_RegisterInterrupt_initParams(&intrPrms);

    /* Populating invalid interrupt parameters to check the Negative condition */
    intrPrms.corepacConfig.arg             = (uintptr_t) 0;
    intrPrms.corepacConfig.priority        = 1U;
    intrPrms.corepacConfig.corepacEventNum = CSL_INVALID_EVENT_ID;
    intrPrms.corepacConfig.isrRoutine      = NULL;
    intrPrms.corepacConfig.intVecNum       = 0U;

    /* Register interrupt */
    osalRetVal = Osal_RegisterInterruptDirect(&intrPrms, isrFxn, &hwiHandle);

    if (OSAL_INT_SUCCESS == osalRetVal)
    {
        OSAL_log("\n Interrupt not register for the event \n");
        test_pass = false;
    }

    Osal_DisableInterrupt(intrPrms.corepacConfig.corepacEventNum, 0U);

    osalRetVal = Osal_DeleteInterrupt(&hwiHandle, intrPrms.corepacConfig.corepacEventNum);
    if(OSAL_INT_SUCCESS != osalRetVal)
    {
        test_pass = false;
    }
    if(true == test_pass)
    {
        OSAL_log("\n Register InterruptDirect Negative Testcase Passed!! \n");
    }
    else
    {
        OSAL_log("\n Register InterruptDirect Negative Testcase Failed!! \n");
    }
}

/* RegisterIntr_nonos.c Testcases */
void RegisterIntr_Test(void)
{
    RegisterIntrDirect_NegTest();
    RegisterIntr_pos_Test();
    RegisterIntr_Neg_Test();
}

/*
 * Description: Testing negative condition check for the below mentioned APIs :
 *      1. QueueP_Params_init
 *      2. QueueP_create
 *      3. QueueP_getQPtr
 *      4. QueueP_put
 *      5. QueueP_get
 *      6. QueueP_delete
 */
void QueueP_nonos_Test(void)
{
    QueueP_Params *params = (QueueP_Params*)NULL_PTR;
    QueueP_Handle handle, handle_val[6];
    QueueP_Status status;
    void * elem = NULL;
    uint32_t i = 0U;

    /* checking QueueP_Params_init else condition */
    QueueP_Params_init(params);

    /* To test "for (i = 0; i < maxQueue; i++)" loop in QueueP_create API */
    for (i = 0U; i < LOOP_CNT; i++)
    {
          handle = QueueP_create(params);
          handle_val[i] = handle;
          if (NULL_PTR == handle_val[i])
          {
              OSAL_log("\n\t Creating Queue failed \n");
          }
    }

    QueueP_getQPtr(handle_val[0]);

    /* Here addr_handle is used to get the memory location of the handle
     * we are corrupting the content of the handle and passing in a corrupt handle to the driver
     * to test negative condition in QueueP_put API
     */
    uint32_t (*addr_handle) = handle_val[0];
    for(i = 0U; i <= LOOP_CNT; i++)
    {
        addr_handle[i] = 0U;
    }
    status = QueueP_put(handle_val[0], elem);
    if(QueueP_OK != status)
    {
        OSAL_log("\n QueueP_put Negative Test Passed!! \n");
    }
    else
    {
        OSAL_log("\n QueueP_put Negative Test Failed!! \n");
    }

    QueueP_get(handle_val[0]);

    for(i = 0U; i < LOOP_CNT; i++)
    {
        status = QueueP_delete(handle_val[i]);
    }

    if(QueueP_OK == status)
    {
        OSAL_log("\n QueueP_delete Nullcheck Test Passed!! \n");
    }
    else
    {
        OSAL_log("\n QueueP_delete Nullcheck Test Failed!! \n");
    }
}

/*
 * Description: Testing negative condition for below mentioned APIs :
 *      1. HeapP_Params_init
 *      2. HeapP_create
 *      3. HeapP_alloc
 *      4. HeapP_free
 *      5. HeapP_delete
 */
void HeapP_freertos_NegTest(void)
{
    memset( gHeapPbuf, 0x00, sizeof( gHeapPbuf ) );

    HeapP_Params params;
    HeapP_Handle handle, memhandle, handle_val[LOOP_CNT];
    HeapP_Status status;
    uint32_t size = 10U, free_size = 5U;
    uint32_t i;

    HeapP_Params_init(&params);

    params.buf = gHeapPbuf;
    params.size = sizeof(gHeapPbuf);

    /* Testing "for (i = 0; i < maxHeap; i++)" loop in HeapP_create API */
    for (i = 0U; i < LOOP_CNT; i++)
    {
        handle = HeapP_create(&params);
        handle_val[i] = handle;
    }

    memhandle = HeapP_alloc(handle_val[0], size);
    if(NULL_PTR == memhandle)
    {
        OSAL_log("\n\t Heap allocation failed \n");
    }

    status = HeapP_free(handle_val[0], memhandle, free_size);
    if(HeapP_OK != status)
    {
        OSAL_log("\n\t Heap is not freed \n");
    }
    for(i = 0; i < LOOP_CNT; i++)
    {
        status = HeapP_delete(handle_val[i]);
    }
    if(HeapP_OK != status)
    {
        OSAL_log("\n\t Deleting the heap failed \n");
        OSAL_log("\n HeapP Freertos Negative Test Failed!! \n");
    }
    else
    {
        OSAL_log("\n HeapP Freertos Negative Test Passed!! \n");
    }
}

/*
 * Description: Testing Null paramter check for the below mentioned APIs :
 *      1. HeapP_create
 *      2. HeapP_alloc
 *      3. HeapP_free
 *      4. HeapP_delete
 */
void HeapP_freertos_NullTest(void)
{
    memset( gHeapPbuf, 0xFF, sizeof( gHeapPbuf ) );
    HeapP_Params params;
    HeapP_Handle handle;
    HeapP_Status status;
    void *fhandle = NULL_PTR;
    uint32_t size = 10U, free_size = 5U;

    HeapP_Params_init(&params);

    params.buf = gHeapPbuf;
    params.size = sizeof(gHeapPbuf);

    handle = HeapP_create(&params);
    if(NULL_PTR == handle)
    {
        OSAL_log("\n\t HEAP create failed\n");
    }
    /* Here addr_handle is used to get the memory location of the handle
    * we are corrupting the content of the handle and passing in a corrupt handle to the driver
    * to test negative condition for HeapP_alloc API
    */
    uint32_t *addr_handle = handle;
    *addr_handle = 0U;
    status = HeapP_free(handle, fhandle, free_size);
    fhandle = HeapP_alloc(handle, size);
    status = HeapP_free(handle, fhandle, free_size);

    status = HeapP_delete(handle);
    if(HeapP_OK != status)
    {
        OSAL_log("\n HeapP_delete Negative Test Passed!! \n");
    }
    else
    {
        OSAL_log("\n HeapP_delete Negative Test Failed!! \n");
    }
}

/*
 * Description: Testing null parameter check for HeapP_Params_init API
 */
void HeapP_Params_init_NullTest(void)
{
    HeapP_Params *params = (HeapP_Params*)NULL_PTR;

    HeapP_Params_init(params);
    OSAL_log("\n HeapP_Params_init Nullcheck Passed!! \n");
}

/*
 * Description  : for ISR context in EventP_post and EventP_getPostedEvents APIs
 */
static void Osal_isInISRContext_EventP_Test(void)
{
    uint32_t      interruptNum = INT_NUM_IRQ;
    HwiP_Params   hwipParams;
    HwiP_Handle   handle;
    HwiP_Status   status;
    int8_t        ret;
    volatile int intCount = 0;
    bool test_pass = true;

    HwiP_Params_init(&hwipParams);

    handle = HwiP_create(interruptNum, (HwiP_Fxn)EventP_IRQ, &hwipParams);

    if(NULL_PTR == handle)
    {
        OSAL_log("\t Failed to create the HwiP handle for start \n");
        test_pass = false;
    }

    if(true == test_pass)
    {
        HwiP_enableInterrupt(interruptNum);

        while (intCount != LOOP_CNT - 1)
        {
            ret = HwiP_post(interruptNum);
            if(osal_UNSUPPORTED == ret)
            {
                OSAL_log("\t HwiP_post unsupported/failed!\n");
                test_pass = false;
                break;
            }

            /* Wait for software timeout, ISR should hit
            * otherwise return the test as failed */
            while (gTestlocalTimeout != 0U)
            {
                gTestlocalTimeout--;
                if (ghwip_isr_got_execute)
                {
                    ghwip_isr_got_execute = 0;
                    intCount++;
                    break;
                }
            }
            /* Wait is over - did not get any interrupts posted/received
            * declare the test as fail
            */
            if (gTestlocalTimeout == 0)
            {
                OSAL_log("\t Failed to get interrupts \n");
            }
        }

        OSAL_log("\t %d IRQs received.\n",intCount);

        status = HwiP_delete(handle);
        if(HwiP_OK != status)
        {
            OSAL_log("\t HwiP delete failed\n");
            test_pass = false;
        }

        if((true == test_pass) && (4 == ghwip_isr_got_execute))
        {
            OSAL_log("\n EventP ISR executed!!\n");
        }
    }
}

/*
 * Description: Testing ISR condition for the below mentioned APIs
 *      1. EventP_post
 *      2. EventP_getPostedEvents
 */
void EventP_ISR_Test()
{
    EventP_Params   params;
    EventP_Status   status;
    uint8_t         loop;

    EventP_Params_init(&params);

    gEventP_handle = EventP_create(&params);
    if (NULL_PTR == gEventP_handle)
    {
       OSAL_log("\t Failed to create event \n");
    }

    for(loop = 0U; loop < LOOP_CNT ; loop++)
    {
        Osal_isInISRContext_EventP_Test();
        if(1U == loop)
        {
            /*Code Snippet: if (gOsalEventAllocCnt > 0U) */
            gOsalEventAllocCnt = 0U;
        }
    }

    status = EventP_delete(&gEventP_handle);
    if(EventP_OK != status)
    {
        OSAL_log("\t Failed to delete event \n");
    }
    else
    {
        OSAL_log("\n EventP ISR Test passed!! \n");
    }
}

/*
 * Description: Testing Negative condition for the below mentioned APIs
 *      1. EventP_create
 *      2. EventP_post
 */
void EventP_Neg_Test(void)
{
    EventP_Params   params;
    EventP_Status   status;
    uint32_t        retEventMask;
    uint32_t        *addr_handle = NULL;
    int8_t          loop;
    bool            test_pass = true;

    EventP_Params_init(&params);

    gEventP_handle = EventP_create(&params);
    if (NULL_PTR == gEventP_handle)
    {
          OSAL_log("\t Failed to create event \n");
          test_pass = false;
    }

    if(true == test_pass)
    {
          /* to check else condition of "if((NULL_PTR != event) && ((bool)true == event->used))" statement"*/
          /* Test 1: event post test */
          addr_handle = gEventP_handle;
          for(loop = 0U; loop <= LOOP_CNT; loop++)
          {
              addr_handle[loop] = 0U;
          }
          status = EventP_post(gEventP_handle, EventP_ID_NONE);
          if(EventP_OK == status)
          {
              OSAL_log("\t Negative condition check of EventP_test Failed \n");
          }

          /* Test : event get posted events test */
          retEventMask = EventP_getPostedEvents(gEventP_handle);
          if((retEventMask & EventP_ID_00) == EventP_ID_00)
          {
              OSAL_log("\t EventP_getPostedEvents returned %d, but expect %d \n",EventP_ID_00, retEventMask);
          }

          /* Checking the Negative condition */
          retEventMask = EventP_wait(gEventP_handle, geventMask, EventP_WaitMode_ALL, EventP_WAIT_FOREVER);
          if((retEventMask & geventMask) == geventMask)
          {
              OSAL_log("\t EventP_wait returned %d, but expect %d \n", retEventMask, geventMask);
          }

          status = EventP_delete(&gEventP_handle);
          if(EventP_OK == status)
          {
              OSAL_log("\t Failed to check the negative condition of delete event \n");
              test_pass = false;
          }
          if(true == test_pass)
          {
              OSAL_log("\n EventP Negative Test passed!! \n");
          }
          else
          {
              OSAL_log("\n EventP Negative Test failed!! \n");
          }
      }
}

uint32_t event_num = 20;

/*
 * Description: Testing Maximum event creation condition
 */
void EventP_Max_Test(void)
{
    EventP_Params   params;
    EventP_Handle   handle[2];
    EventP_Status   status;
    uint32_t        loop;
    bool            test_pass = false;


    EventP_Params_init(&params);

    for (loop = 0U; loop <= LOOP_CNT ; loop++)
    {
        handle[loop] = EventP_create(&params);
        if (NULL_PTR != handle[loop])
        {
            test_pass = true;
        }
    }
    if(true == test_pass)
    {
        for (loop = 0; loop < LOOP_CNT ; loop++)
        {
            status = EventP_delete(&handle[loop]);
            if(EventP_OK != status)
            {
                OSAL_log("Failed to delete event \n");
                OSAL_log("\t Maximum event creation test Failed!! \n");
                test_pass = false;
            }
        }
        if(true == test_pass)
        {
            OSAL_log("\n\t Maximum event creation test Passed!! \n");
        }
    }
    else
    {
        OSAL_log("\t Maximum event creation test Failed!! \n");
    }
}

void EventP_Test(void)
{
    EventP_ISR_Test();
    EventP_Neg_Test();
}

/*
 * Description  : Testing null parameter check for ClockP_Params_init API
 */
void ClockP_Params_init_NullTest()
{
    ClockP_Params_init(NULL);
    ClockP_create(clockp_clockfxn,NULL);
    OSAL_log("\n ClockP_Params_init_NullTest Passed!!\n");
}

/*
 * Description  : Testing overflow condition for ClockP_create API
 */
void ClockP_create_test_clocks_limit()
{
    uint32_t i = 0U;
    /* having 2 clocks */
    ClockP_Params clockp_params;
    ClockP_Params_init(&clockp_params);
    clockp_params.period = 50;  /*to get 50msec period*/
    clockp_params.runMode = ClockP_RunMode_CONTINUOUS;
    clockp_params.startMode = ClockP_StartMode_AUTO;

    for(i = 0U; i <= (OSAL_SAFERTOS_CONFIGNUM_CLOCK + 1U); i++)/*to overflow max clock limit*/
    {
        ClockP_create(clockp_clockfxn,&clockp_params);
    }
    OSAL_log("\n ClockP_create_test_clocks_limit passed!!\n");

}

/*
 * Description  : Testing null parameter check for ClockP_timerCallbackFunction API
 */
void ClockP_create_callback_NullTest()
{
    ClockP_Params clockp_params;
    ClockP_Params_init(&clockp_params);
    clockp_params.period = 50;  /*to get 50msec period*/
    clockp_params.runMode = ClockP_RunMode_CONTINUOUS;
    clockp_params.startMode = ClockP_StartMode_AUTO;

    ClockP_create(NULL,&clockp_params);
    OSAL_log("\n ClockP_create_callback_NullTest passed!!\n");
}

/*
 * Description  : Testing null parameter check for ClockP_delete API
 */
void ClockP_delete_NullTest()
{
    /*testing negative scenario of 1st if*/
    ClockP_delete(NULL);

    /*testing xCreateResult != pass scenario*/
    OSAL_log("\n ClockP_delete_NullTest passed!!\n");
}

/*
 * Description  : Helper function for ClockP_start, to check if OSAL is in ISR context or not
 */
static void Osal_isInISRContext_clockp_start()
{
    uint32_t interruptNum = 160;
    const HwiP_Params *hwip_test_params;
    HwiP_Params hwipParams;
    HwiP_Handle hHwi;
    HwiP_Status hwiStatus;
    int8_t ret;

    HwiP_Params_init(&hwipParams);

    hHwi = HwiP_create(interruptNum,(HwiP_Fxn) clockp_start_isr,(void *)&hwip_test_params);
    if (hHwi == NULL_PTR)
    {
      OSAL_log("\t Failed to create the HwiP handle for start \n");
    }

    HwiP_enableInterrupt(interruptNum);

    ret = HwiP_post(interruptNum);
    if(ret == osal_UNSUPPORTED)
    { /* In case of unsupported SOC/error */
        OSAL_log("\t HwiP_post unsupported/failed!\n");
    }
    hwiStatus = HwiP_delete( hHwi );
    if(hwiStatus == HwiP_FAILURE)
    {
        OSAL_log("\t HwiP delete failed\n");
    }


    if(1U == ghwip_isr_got_execute)
        OSAL_log("\nClockP_start ISR executed\n");
}

/*
 * Description  : Testing ISR in ClockP_start API
 */
void ClockP_start_HwiP_Test()
{
    /*Testing Osal_isInISRContext = 1*/
    ClockP_Params clockp_params;
    ClockP_Params_init(&clockp_params);
    clockp_params.period = 50;  /*to get 50msec period*/

    gclockp_handle = ClockP_create(clockp_clockfxn,&clockp_params);
    if (gclockp_handle == NULL_PTR)
    {
      OSAL_log("\t ClockP Create error in clcockp_start test \n");
    }

    Osal_isInISRContext_clockp_start();

    /*testing negative condition of 1st if*/
    ClockP_start(NULL);

    if (ClockP_delete(gclockp_handle) != ClockP_OK)
    {
        OSAL_log("\t Err: Could not delete the clock \n");
    }
    OSAL_log("\nClockP_start_test passed!!\n");
}

/*
 * Description  : Testing ClockP safertos by calling the below mentioned APIs
 *      1. ClockP_Params_init
 *      2. ClockP_create
 *      3. ClockP_start
 *      4. ClockP_stop
 *      5. ClockP_delete
 */
void Clockp_safertos_Test()
{
    ClockP_Handle handle;
    ClockP_Status clockp_timerstatus;
    bool ret=true;
    /*test case for ClockP_Params_init*/
    ClockP_Params clockp_params;
    ClockP_Params_init(&clockp_params);
    clockp_params.period = 50;  /*to get 50msec period*/

    /*test case for ClockP_create */
    handle = ClockP_create(clockp_clockfxn,&clockp_params);

    /*test case for ClockP_start*/
    clockp_timerstatus = ClockP_start(handle);
    if(clockp_timerstatus != ClockP_OK)
    {
        OSAL_log("\t Err: Could not start the clock\n");
        ret = false;
    }

    while((true == ret) && (0U == gclockp_clockfxn_got_execute))
    {

    }

    /*test case for ClockP_stop*/
    if(true == ret)
    {
        clockp_timerstatus = ClockP_stop(handle);
        if (clockp_timerstatus != ClockP_OK)
        {
            OSAL_log("\t Err: Could not stop the clock \n");
            ret = false;
        }
    }

    /*test case for ClockP_delete*/
    if (true == ret)
    {
      if (ClockP_delete(handle) != ClockP_OK)
      {
        OSAL_log("\t Err: Could not delete the clock \n");
        ret = false;
      }
    }
    OSAL_log("\n Test Case for ClockP safertos flow passed!!\n");

}

/*
 * Description  : Testing ClockP freertos by calling below mentioned APIs :
 *      1. ClockP_Params_init
 *      2. ClockP_create
 *      3. ClockP_start
 *      4. ClockP_stop
 *      5. ClockP_delete
 */
void Clockp_freertos_Test()
{
    ClockP_Handle handle;
    ClockP_Status clockp_timerstatus;
    bool ret=true;
    /*test case for ClockP_Params_init*/
    ClockP_Params clockp_params;
    ClockP_Params_init(&clockp_params);
    clockp_params.period = 50;  /*to get 50msec period*/

    /*test case for ClockP_create */
    handle = ClockP_create(clockp_clockfxn,&clockp_params);
    if(handle == NULL)
    {
        OSAL_log("\t Err : ClockP_create failed for freertos\n");
    }

    /*test case for ClockP_start*/
    clockp_timerstatus = ClockP_start(handle);
    if(clockp_timerstatus != ClockP_OK)
    {
        OSAL_log("\t Err: Could not start the clock\n");
        ret = false;
    }

    while((true == ret) && (0U == gclockp_clockfxn_got_execute))
    {

    }

    /*test case for ClockP_stop*/
    if(true == ret)
    {
        clockp_timerstatus = ClockP_stop(handle);
        if (clockp_timerstatus != ClockP_OK)
        {
            OSAL_log("\t Err: Could not stop the clock \n");
            ret = false;
        }
    }

    /*test case for ClockP_delete*/
    if (true == ret)
    {
      if (ClockP_delete(handle) != ClockP_OK)
      {
        OSAL_log("\t Err: Could not delete the clock \n");
        ret = false;
      }
    }

    OSAL_log("\n Clockp_freertos_Test passed!!\n");
}

/*
 * Description  : Testing limit overflow condition for ClockP_create API in freertos
 */
void ClockP_create_test_clocks_limit_freertos()
{
    uint32_t i = 0U;
    /* having 2 clocks */
    ClockP_Params clockp_params;
    ClockP_Params_init(&clockp_params);
    clockp_params.period = 50;  /*to get 50msec period*/
    clockp_params.runMode = ClockP_RunMode_CONTINUOUS;
    clockp_params.startMode = ClockP_StartMode_AUTO;

    for(i = 0U; i <= (OSAL_FREERTOS_CONFIGNUM_CLOCK + 1U); i++)/*to overflow max clock limit*/
    {
        ClockP_create(clockp_clockfxn,&clockp_params);
    }

    OSAL_log("\n ClockP_create_test_clocks_limit_freertos passed!!\n");
}

/*
 * Description  : Testing negative scenarios of functions ClockP_Params_init,ClockP_start,
                  ClockP_stop by passing NULL_PTR
 */
void ClockP_init_start_stop_null_test()
{
    ClockP_Params *params = NULL_PTR;
    ClockP_Status ret_start,ret_stop;
    ClockP_Params_init(params);
    ret_start = ClockP_start(NULL_PTR);
    if(ret_start != ClockP_FAILURE)
    {
        OSAL_log("NULL Parameter test failed for ClockP_start\n" );
    }
    ret_stop = ClockP_stop(NULL_PTR);
    if(ret_stop == ClockP_FAILURE)
    {
        OSAL_log("NULL Parameter test for ClockP_stop passed!!\n");
    }
    else
    {
        OSAL_log("NULL Parameter test failed for ClockP_stop\n" );
    }    
}

/*
 * Description  : Testing what will happen if ptimer->used is negative i.e. zero in
                  ClockP_start,ClockP_stop
 */
void ClockP_start_stop_ptimer_used_false_test()
{
    ClockP_Handle handle;
    ClockP_Status ret_start,ret_stop,ret_delete;
    /*test case for ClockP_Params_init*/
    ClockP_Params clockp_params;
    ClockP_Params_init(&clockp_params);
    clockp_params.period = 50;  /*to get 50msec period*/
    uint32_t *addr;

    /*test case for ClockP_create */
    handle = ClockP_create(clockp_clockfxn,&clockp_params);
    addr = handle;
    /*to test branch pTimer->used = false in clockp start and stop*/
    addr[0]=0U;
    ret_start = ClockP_start(handle);
    if(ret_start == ClockP_FAILURE)
    {
        OSAL_log("pTimer->used == 0 test Passed for ClockP_start\n" );
    }
    else
    {
        OSAL_log("pTimer->used == 0 test failed for ClockP_start\n" );
    }
    ret_stop = ClockP_stop(handle);
    if(ret_stop == ClockP_FAILURE)
    {
        OSAL_log("pTimer->used == 0 test passed for ClockP_stop\n" );
    }
    else
    {
        OSAL_log("pTimer->used == 0 test failed for ClockP_stop\n" );
    }
    ret_delete = ClockP_delete(NULL_PTR);/*Testing it for NULL_PTR paramter input*/
    if(ret_delete == ClockP_FAILURE)
    {
        OSAL_log("pTimer->used == 0 test for ClockP_delete passed !!\n" );
    }
    else
    {
        OSAL_log("pTimer->used == 0 test failed for ClockP_delete\n" );
    }
}

/*
 * Description:Testing ClockP_timerCallbackFunction checking negative scenario when pTimer is NULL_PTR
 */
void ClockP_timerCallbackFunction_NULL_test()
{
    ClockP_Params clockp_params;
    ClockP_Handle ret_handle;
    ClockP_Status ret_delete;
    ClockP_Params_init(&clockp_params);
    clockp_params.period = 50;  /*to get 50msec period*/
    ret_handle = ClockP_create(NULL_PTR,&clockp_params);
    ret_delete = ClockP_delete(ret_handle);
    if(ret_delete == ClockP_FAILURE)
    {
        OSAL_log("NULL Parameter test for ClockP_delete passed !!\n" );
    }
    else
    {
        OSAL_log("NULL Parameter test failed for ClockP_delete\n" );
    }
}


/*
 * Description  : Testing by storing data in array and reading it and then passing to CacheP_wb
 */
void CacheP_wb_Test()
{
    uint8_t cachep_nonos_arr[1000];
    int32_t i = 0;
    for(i = 0; i < 1000; i++)
    {
        cachep_nonos_arr[i]='T';
    }
    if(cachep_nonos_arr[0] == 'T' && cachep_nonos_arr[999] == 'T')
        OSAL_log("\t array written properly\n");
    else
        OSAL_log("\t array not written properly\n");

    CacheP_wb(cachep_nonos_arr,1000);
    OSAL_log("\n CacheP_wb_Test passed!!\n");
}

/*
 * Description  : Testing by storing data in array and reading it and then paasing to CacheP_Inv
 */
void CacheP_Inv_Test()
{
    uint8_t cachep_nonos_arr[1000];
    int32_t i = 0;
    for(i = 0; i < 1000; i++)
    {
        cachep_nonos_arr[i]='T';
    }
    if(cachep_nonos_arr[0] == 'T' && cachep_nonos_arr[999] == 'T')
        OSAL_log("\t array written properly\n");
    else
        OSAL_log("\t array not written properly\n");

    CacheP_Inv(cachep_nonos_arr,1000);
    OSAL_log("\n CacheP_Inv_Test passed!!\n");
}

/*
 * Description  : Testing below mentioned DebugP APIs by passing required data :
 *      1. DebugP_log0
 *      2. DebugP_log1
 *      3. DebugP_log2
 *      4. DebugP_log3
 *      5. DebugP_log4
 */
void DebugP_log_Test()
{
    const char *format = NULL_PTR;
    uintptr_t p1=1,p2=1,p3=1,p4=1;

    DebugP_log0(format);

    DebugP_log1(format, p1);

    DebugP_log2(format, p1, p2);

    DebugP_log3(format, p1, p2, p3);

    DebugP_log4(format, p1, p2, p3, p4);

    OSAL_log("\nDebugP_log_Test passed!!\n");
}

/*
 * Description  : Testing freertos mailbox by calling below mentioned APIs :
 *      1. MailboxP_Params_init
 *      2. MailboxP_create
 *      3. MailboxP_delete
 *      4. MailboxP_post
 *      5. MailboxP_pend
 */
void MailboxP_freertos_Test()
{
    MailboxP_Params params;
    MailboxP_Handle ret_handle;
    uint8_t str[] = {"Texas Instruments"};
    void * msg = str;
    uint32_t timeout = 1;
    MailboxP_Status ret_val = MailboxP_OK;
    uint8_t data_arr[30];

    MailboxP_Params_init(&params);
    params.buf = data_arr;
    params.bufsize = 50;
    params.count = 5;
    params.size  = 6;
    ret_handle = data_arr;

    ret_handle = MailboxP_create(&params);
    if(ret_handle == NULL_PTR)
    {
        OSAL_log("\t Err : Mailbox creation failed\n");
    }

    ret_val = MailboxP_post(ret_handle,msg,timeout);
    if(ret_val == MailboxP_TIMEOUT)
    {
        OSAL_log("\t Err : MailboxP_TIMEOUT\n");
    }

    MailboxP_pend(ret_handle,msg,timeout);

    MailboxP_getNumPendingMsgs(ret_handle);

    MailboxP_delete(ret_handle);
    OSAL_log("\n MailboxP_freertos_Test passed!!\n");
}

/*
 * Description  : Testing null parameter check for MailboxP_Params_init API
 */
void MailboxP_params_init_null_test()
{
    MailboxP_Params_init(NULL_PTR);
    OSAL_log("\n MailboxP_params_init_null_test passed!!\n");
}

/*
 * Description  : Testing multiple mailbox create for MailboxP_create API
 */
void MailboxP_create_multiple_test()
{
    MailboxP_Params params;
    MailboxP_Handle ret_handle;
    uint8_t data_arr[30];
    uint32_t i = 0U;

    MailboxP_Params_init(&params);
    params.buf = data_arr;
    params.bufsize = 50;
    params.count = 5;
    params.size  = 6;
    ret_handle = data_arr;

    ret_handle = MailboxP_create(&params);
    for(i = 0U;i <= (OSAL_FREERTOS_CONFIGNUM_MAILBOX + 1U); i++)
    {
        MailboxP_create(&params);

        MailboxP_delete(ret_handle);
    }

    OSAL_log("\n MailboxP_create_multiple_test passed!!\n");
}

/*
 * Description  : Testing null parameter check for MailboxP_delete API
 */
void MailboxP_delete_null_test()
{
    MailboxP_Params params;
    MailboxP_Handle ret_handle;
    uint8_t data_arr[30];

    MailboxP_Params_init(&params);
    params.buf = data_arr;
    params.bufsize = 50;
    params.count = 5;
    params.size  = 6;
    ret_handle = data_arr;

    ret_handle = MailboxP_create(&params);
    MailboxP_create(&params);

    uint32_t (*addr_handle) = ret_handle;
    addr_handle[0] = 0U;                     /*To make mailbox->used zero to achieve negative case*/
    MailboxP_delete(ret_handle);
    OSAL_log("\n MailboxP_delete_null_test passed!!\n");
}

/*
 * Description  : Testing the scenario when MailboxP_post in in ISR context.
 */
void MailboxP_post_hwiP_test()
{
    uint32_t interruptNum = INT_NUM_IRQ;
    const HwiP_Params *hwip_test_params;
    HwiP_Params hwipParams;
    HwiP_Handle hHwi;

    MailboxP_Params params;
    MailboxP_Handle ret_handle;

    uint8_t data_arr[30];

    MailboxP_Params_init(&params);
    params.buf = data_arr;
    params.bufsize = 50;
    params.count = 5;
    params.size  = 6;
    ret_handle = data_arr;

    ret_handle = MailboxP_create(&params);
    if(NULL_PTR == ret_handle)
    {
        OSAL_log("\t Err : Mailbox creation failed\n");
    }

    HwiP_Params_init(&hwipParams);

    hHwi = HwiP_create(interruptNum,(HwiP_Fxn) mailboxp_post_isr,(void *)&hwip_test_params);
    if (NULL_PTR == hHwi)
    {
      OSAL_log("\t Failed to create the HwiP handle for stop \n");
    }

    HwiP_enableInterrupt(interruptNum);

    HwiP_post(interruptNum);

    HwiP_delete( hHwi );

    if(3U == ghwip_isr_got_execute)
    {
        OSAL_log("\t MailboxP_Post ISR executed\n");
    }

    OSAL_log("\n MailboxP_post_hwiP_test passed!!\n");
}

/*
 * Description  : Testing scenario when MailboxP_post is not in ISR and timeout is 0.
 */
void MailboxP_wait_forever_test()
{
    MailboxP_Params params;
    MailboxP_Handle ret_handle;

    MailboxP_Status ret_val = MailboxP_OK;
    uint8_t data_arr[30];

    MailboxP_Params_init(&params);
    params.buf = data_arr;
    params.bufsize = 50;
    params.count = 5;
    params.size  = 6;
    ret_handle = data_arr;

    uint8_t str[] = {"Texas Instruments"};
    void * msg = str;
    uint32_t timeout = 0U;

    ret_handle = MailboxP_create(&params);
    if(NULL_PTR == ret_handle)
    {
        OSAL_log("\t Err : Mailbox creation failed\n");
    }

    ret_val = MailboxP_post(ret_handle,msg,timeout);
    if(ret_val == MailboxP_TIMEOUT)
    {
        OSAL_log("\t Err : MailboxP_TIMEOUT\n");
    }
    OSAL_log("\n MailboxP_wait_forever_Test passed!!\n");
}

/*
Description  : Creating a task having priority zero to fulfill condition TaskP_PRIORITY_LOWEST >= taskPriority
*/
void TaskP_priority_zero_test()
{
    TaskP_Params params;
    TaskP_Handle tskHandle = NULL;

    TaskP_Params_init(&params);
    params.priority = 0;
    params.stack    = gTskStack;
    params.stacksize= sizeof (gTskStack);

    tskHandle = TaskP_create((TaskP_Fxn)Taskp_TaskpFxn,(void *)&params);
    if(params.priority == TaskP_PRIORITY_LOWEST)
    {
        OSAL_log("Task got created with the lowest possible priority\n");
    }
    else
    {
        OSAL_log("Lowest priority 0 not got set to task\n");
    }
    if(tskHandle == NULL)
    {
        OSAL_log("Task handle is not valid\n");
    }
    else
    {
        OSAL_log("Task handle is valid deleted a task\n");
        TaskP_delete(tskHandle);
    }
    OSAL_log("TaskP_priority_zero_test passed !!\n");
}

/*
Description  : Creating more than 1 tasks so i++ will get trigger and additionally when task
               tasks count go beyond limit that test also
*/
void TaskP_create_max_task_creation_test()
{
    TaskP_Params params;
    TaskP_Handle tskHandle = NULL;
    TaskP_Params_init(&params);
    params.priority = 17;
    params.stack    = gTskStack;
    params.stacksize= sizeof (gTskStack);
#if defined(FREERTOS)
    int i = 0;
    for(i=0;i<=(OSAL_FREERTOS_CONFIGNUM_TASK+1);i++)
#endif
#if defined(SAFERTOS)
    int i = 0;
    for(i=0;i<=(OSAL_SAFERTOS_CONFIGNUM_TASK+1);i++)
#endif
    {
        tskHandle = TaskP_create((TaskP_Fxn)Taskp_TaskpFxn,(void *)&params);
    }

    if(tskHandle == NULL)
    {
        OSAL_log("Task cration fail while checking for max task creation\n");
    }
    else
    {     
        OSAL_log("task handle is valid and deleting the task\n");
        TaskP_delete(tskHandle);
    }
    OSAL_log("TaskP_create_max_task_creation_test passed !!\n");
}

/*
Description  : Creating a task having priority more than zero to fulfill condition TaskP_PRIORITY_HIGHEST < taskPriority
               calling TaskP_sleepInMsecs to configure sleep time in msec.
               Calling TaskP_self to get handle of current task
               TaskP_selfmacro calls TaskP_self
               finally calling taskp sleep
*/
void TaskP_safertos_auxilary_test()
{
    TaskP_Params params;
    TaskP_Handle taskp_handle;
    TaskP_Params_init(&params);
    params.priority = 3;
    params.stack    = gTskStack;
    params.stacksize= sizeof (gTskStack);
    TaskP_sleepInMsecs(10);
    taskp_handle = TaskP_create((TaskP_Fxn)Taskp_TaskpFxn,(void *)&params);

    if(taskp_handle == TaskP_selfmacro())
    {
        OSAL_log("received task handle is correct\n");
    }
    else
    {
        OSAL_log("Current task handle and received task handle not same\n");
    }
    TaskP_sleep(1000);
    OSAL_log("TaskP_safertos_auxilary_test passed !!\n");
}

/*
 * Description:Testing semaphore creation,posting and deletion flow for freertos,safertos
 */
void Semaphore_flow_test()
{
    SemaphoreP_Params params;
    uint32_t count = 2;
    SemaphoreP_Handle ret_handle;
    SemaphoreP_Status sem_post_stat;
    uint32_t sema_count = 0;
    SemaphoreP_Status sema_pend_stat;
    SemaphoreP_Status sema_delete_stat;
    uint32_t ctrlBitMap = OSAL_HWATTR_SET_SEMP_EXT_BASE;
    Osal_HwAttrs hwAttrs;
    hwAttrs.extSemaphorePBlock.base = (uintptr_t) & semPMemBlock[0];
#if defined(FREERTOS)
    hwAttrs.extSemaphorePBlock.size = (1*OSAL_FREERTOS_SEMAPHOREP_SIZE_BYTES);
#endif
#if defined(SAFERTOS)
    hwAttrs.extSemaphorePBlock.size = (1*OSAL_SAFERTOS_SEMAPHOREP_SIZE_BYTES);
#endif
    /* Set the extended memmory block for semaphore operations */
    Osal_setHwAttrs(ctrlBitMap,&hwAttrs);
    SemaphoreP_Params_init(&params);
    ret_handle = SemaphoreP_create(count,&params);
    if(ret_handle == NULL_PTR)
    {
        OSAL_log("Err : Semaphore creation failed\n");
    }
    sem_post_stat = SemaphoreP_post(ret_handle);
    if(sem_post_stat == SemaphoreP_OK)
    {
        OSAL_log("Semaphore post suceeded\n");
    }
    else
    {
        OSAL_log("Err : Semaphore post failed\n");
    }
    sema_count = SemaphoreP_getCount(ret_handle);

    if(sema_count == 3)
    {
        OSAL_log("Semaphore Get count was successful\n");
    }
    else
    {
        OSAL_log("Received semaphore count is incorrect\n");
    }
    sema_pend_stat = SemaphoreP_pend ( ret_handle , 100 ) ;
    if(sema_pend_stat == SemaphoreP_TIMEOUT)
    {
        OSAL_log("Err : Semaphore timeout\n");
    }
    sema_delete_stat = SemaphoreP_delete(ret_handle) ;
    if(sema_delete_stat == SemaphoreP_FAILURE)
    {
        OSAL_log("Err : Semaphore not deleted\n");
    }
    OSAL_log("Semaphore_flow_test passed !!\n");
}

/*
Description : Testing SemaphoreP_compileTime_SizeChk function
*/
void Semaphore_compile_time_size_test()
{
    SemaphoreP_Params params;
    uint32_t count = 2;
    SemaphoreP_Handle ret_handle;
    SemaphoreP_Status sema_delete_stat;
    ret_handle = SemaphoreP_create(count,&params);
    if(ret_handle == NULL_PTR)
    {
        OSAL_log("Err : Semaphore creation failed\n");
    }
    SemaphoreP_compileTime_SizeChk();
    sema_delete_stat = SemaphoreP_delete(ret_handle);
    if(sema_delete_stat == SemaphoreP_FAILURE)
    {
        OSAL_log("Err : Semaphore not deleted\n");
    }
    OSAL_log ("Semaphore_compile_time_size_test passed !!\n") ;
}

/*
Description : Set the extended memmory block for semaphore operations and test semaphore
*/
void Sema_create_extended_mem_block_test()
{
    uint32_t ctrlBitMap = OSAL_HWATTR_SET_SEMP_EXT_BASE;
    Osal_HwAttrs hwAttrs;
    hwAttrs.extSemaphorePBlock.base = (uintptr_t) & semPMemBlock[0];
    hwAttrs.extSemaphorePBlock.size = (1*OSAL_FREERTOS_SEMAPHOREP_SIZE_BYTES);
    SemaphoreP_Params params;
    uint32_t count = 2;
    SemaphoreP_Handle ret_handle;
    SemaphoreP_Status sema_delete_stat;

    /* Set the extended memmory block for semaphore operations */
    Osal_setHwAttrs(ctrlBitMap,&hwAttrs);
    SemaphoreP_Params_init(&params);
    ret_handle = SemaphoreP_create(count,&params);
    if(ret_handle == NULL_PTR)
    {
        OSAL_log("Err : Freertos Semaphore creation failed\n");
    }

    SemaphoreP_postFromClock(ret_handle);
    SemaphoreP_postFromISR(ret_handle);
    sema_delete_stat=SemaphoreP_delete ( ret_handle );
    if(sema_delete_stat == SemaphoreP_FAILURE)
    {
        OSAL_log("Err : Freertos Semaphore not deleted\n");
    }

    /*added to execute for loop for more semaphore creation*/
    SemaphoreP_create(count,&params);
    SemaphoreP_create(count,&params);
    SemaphoreP_create(count,&params);

    sema_delete_stat = 0;
    uint32_t *addr_handle = ret_handle;
    addr_handle[0] = 0U;
    SemaphoreP_delete(ret_handle);/*Test negative scenario when sems->used zero*/
    if(sema_delete_stat == SemaphoreP_FAILURE)
    {
        OSAL_log("Err : Freertos Semaphore not deleted\n");
    }
    OSAL_log("Sema_create_extended_mem_block_test passed !!\n");
}

/*
Description : Testing semaphore creation with NULL parameter
*/
void Semaphore_create_null_param_test()
{
    SemaphoreP_Params params;
    SemaphoreP_Handle ret_handle;
    uint32_t count = 2U;
    SemaphoreP_Params_init(&params);

    ret_handle = SemaphoreP_create(count,NULL_PTR);
    if(ret_handle != NULL_PTR)
    {
        OSAL_log("Semaphore_create_null_param_test passed !!\n");
    }
    else
    {
        OSAL_log("NULL_PTR test for semaphore_create failed\n");
    }

    
}

/*
Description:Testing SemaphoreP_compileTime_SizeChk function
*/
void SemaphoreP_compileTime_size_check_test()
{
    SemaphoreP_compileTime_SizeChk();/*Checking size during compile time so no check
                                      check can be added in running app*/
    OSAL_log("SemaphoreP_compileTime_Size_check_test passed !!\n");
}

/*
Description:Testing semaphore creation with extended memory block
*/
void SemaphoreP_create_extended_mem_block_test()
{
    uint32_t ctrlBitMap = OSAL_HWATTR_SET_SEMP_EXT_BASE;
    Osal_HwAttrs hwAttrs;
    hwAttrs.extSemaphorePBlock.base = (uintptr_t) & semPMemBlock[0];
    hwAttrs.extSemaphorePBlock.size = (1*OSAL_NONOS_SEMAPHOREP_SIZE_BYTES);
    SemaphoreP_Params params;
    uint32_t count = 2U;
    SemaphoreP_Handle ret_handle;
    SemaphoreP_Status sema_delete_stat;
    uint32_t B_Sema_count;
    uint32_t timeout = 10U;
    SemaphoreP_Status ret_val = SemaphoreP_OK;

    /* Set the extended memmory block for semaphore operations */
    Osal_setHwAttrs(ctrlBitMap,&hwAttrs);
    SemaphoreP_Params_init(&params);
    ret_handle = SemaphoreP_create(count,&params);
    if(ret_handle == NULL_PTR)
    {
        UART_printf("Err : Semaphore creation failed\n");
    }

    B_Sema_count = SemaphoreP_getCount(ret_handle);

    if(B_Sema_count != 2U)
    {
        OSAL_log("Err : Count is wrong\n");
    }

    ret_val = SemaphoreP_pend(ret_handle, timeout);
    if(ret_val == SemaphoreP_FAILURE)
    {
        OSAL_log("Err : Semaphore not pended \n");
    }

    sema_delete_stat=SemaphoreP_delete(ret_handle);
    if(sema_delete_stat == SemaphoreP_FAILURE)
    {
        UART_printf("Err : Semaphore not deleted\n");
    }
    OSAL_log("B_SemaphoreP_create_extended_mem_block_test passed !!\n");
}

/*
Description:Testing SemaphoreP_pend function
*/
void SemaphoreP_pend_test()
{
    uint32_t ctrlBitMap = OSAL_HWATTR_SET_SEMP_EXT_BASE;
    Osal_HwAttrs hwAttrs;
    hwAttrs.extSemaphorePBlock.base = (uintptr_t) & semPMemBlock[0];
    hwAttrs.extSemaphorePBlock.size = (1*OSAL_FREERTOS_SEMAPHOREP_SIZE_BYTES);
    SemaphoreP_Params params;
    uint32_t count = 2U;
    SemaphoreP_Handle ret_handle;
    SemaphoreP_Status sema_delete_stat;
    uint32_t B_Sema_count;
    uint32_t timeout = 10U;
    SemaphoreP_Status ret_val = SemaphoreP_OK;

    /* Set the extended memmory block for semaphore operations */
    Osal_setHwAttrs(ctrlBitMap,&hwAttrs);
    SemaphoreP_Params_init(&params);
    ret_handle = SemaphoreP_create(count,&params);
    if(ret_handle == NULL_PTR)
    {
        UART_printf("Err : Semaphore creation failed\n");
    }

    B_Sema_count = SemaphoreP_getCount(ret_handle);

    if(B_Sema_count != 2U)
    {
        OSAL_log("Err : Count is wrong\n");
    }

    ret_val = SemaphoreP_pend(ret_handle, timeout);
    if(ret_val == SemaphoreP_FAILURE)
    {
        OSAL_log("Err : Semaphore not pended \n");
    }

    sema_delete_stat=SemaphoreP_delete(ret_handle);
    if(sema_delete_stat == SemaphoreP_FAILURE)
    {
        UART_printf("Err : Semaphore not deleted\n");
    }
    OSAL_log("SemaphoreP_pend_test passed !!\n");
}

/*
Description: Testing SemaphoreP_pend function when semS->count is zero
*/
void Sema_pend_semacount0_test()
{
    SemaphoreP_Params params;
    uint32_t count = 2U;
    SemaphoreP_Handle ret_handle;
    SemaphoreP_Status sema_delete_stat;
    uint32_t timeout = 10U;
    SemaphoreP_Status ret_val = SemaphoreP_OK;
    params.mode = SemaphoreP_Mode_BINARY;

    SemaphoreP_Params_init(&params);
    ret_handle = SemaphoreP_create(count,&params);
    if(ret_handle == NULL_PTR)
    {
        UART_printf("Err : Semaphore creation failed\n");
    }

    /*pending thrice and as semacount is 2 it will fail at 3rd pend*/
    SemaphoreP_pend(ret_handle, timeout);
    SemaphoreP_pend(ret_handle, timeout);
    ret_val = SemaphoreP_pend(ret_handle, timeout);
    if(ret_val == SemaphoreP_TIMEOUT)
    {
        OSAL_log("Semaphore pend returned failure. Test is successful \n");
    }

    sema_delete_stat=SemaphoreP_delete(ret_handle);
    if(sema_delete_stat == SemaphoreP_FAILURE)
    {
        UART_printf("Err : Semaphore not deleted\n");
    }
    OSAL_log("Sema_pend_semacount0_test passed !!\n");
}

/*
Description: Testing SemaphoreP_delete when semS->used is zero
*/
void Sema_delete_sems_used_zero_test()
{
    SemaphoreP_Params params;
    uint32_t count = 2U;
    SemaphoreP_Handle ret_handle;
    SemaphoreP_Status sema_delete_stat;
    params.mode = SemaphoreP_Mode_BINARY;

    SemaphoreP_Params_init(&params);
    ret_handle = SemaphoreP_create(count,&params);
    uint32_t *addr_handle = ret_handle;
    addr_handle[0] = 0U;
    sema_delete_stat=SemaphoreP_delete(ret_handle);
    if(sema_delete_stat == SemaphoreP_FAILURE)
    {
        UART_printf("Sema_delete_sems_used_zero_test passed!!\n");
    }
}



#if defined(BARE_METAL)
void OSAL_tests()
#else
void OSAL_tests(void *arg0, void *arg1)
#endif
{
    Board_initOSAL();

#if defined(SAFERTOS)

    ClockP_start_HwiP_Test();

    Clockp_safertos_Test();

    ClockP_create_test_clocks_limit();

    ClockP_Params_init_NullTest();

    ClockP_delete_NullTest();

    ClockP_create_callback_NullTest();

    TaskP_safertos_auxilary_test();

#endif

#if defined(FREERTOS)
    Clockp_freertos_Test();

    ClockP_create_test_clocks_limit_freertos();

    ClockP_start_stop_ptimer_used_false_test();

    ClockP_timerCallbackFunction_NULL_test();

    DebugP_log_Test();

    MailboxP_freertos_Test();

    MailboxP_params_init_null_test();

    MailboxP_create_multiple_test();

    MailboxP_delete_null_test();

    //MailboxP_post_hwiP_test();

    MailboxP_wait_forever_test();

    QueueP_nonos_Test();

    HeapP_freertos_NegTest();

    HeapP_freertos_NullTest();

    HeapP_Params_init_NullTest();

    Sema_create_extended_mem_block_test();

	Semaphore_create_null_param_test();

    EventP_Max_Test();

    loadP_freertos_test();

#endif

#if defined(BARE_METAL)
    CycleprofilerP_nonos_Test();

    MutexP_Test();

    MutexP_create_NullCheck();

    SwiP_nonos_Test();

    RegisterIntr_nonos_NegTest();

    RegisterIntrDirect_NegTest();

    HwiP_create_nonos_Null_test();

    HwiP_delete_AlloCnt_test();

    Utils_nonos_test();

    Utils_nonos_SetHwAttr_Multi_Test();

    SemaphoreP_create_extended_mem_block_test();

    SemaphoreP_pend_test();

    Sema_pend_semacount0_test();

    Sema_delete_sems_used_zero_test();

    //CacheP_wb_Test();

    //CacheP_Inv_Test();
#endif

#if !defined(BARE_METAL)

    RegisterIntr_Test();

    EventP_Test();

    MutexP_create_NullCheck();

    MutexP_Test();

    TaskP_priority_zero_test();

    TaskP_create_max_task_creation_test();

    Semaphore_flow_test();

    Semaphore_compile_time_size_test();

    ClockP_init_start_stop_null_test();

#endif

#ifdef BARE_METAL
    while (1)
    {
    }
#endif
}


/*
 *  ======== main ========
 */
int main(void)
{
#ifdef BARE_METAL
    OSAL_tests();
#else
    /* All other platforms have the task created under RTSC cfg file
     * hence not needed to be created again as below
     * For AM65XX TPR12 and J7 the common RTSC cfg file is used and hence there is
     * no test application specific task is created in teh RTSC cfg file
     */
#if defined (SOC_J721E) || defined(SOC_J7200) || defined(SOC_J721S2) || defined(SOC_J784S4)
    TaskP_Params taskParams;

    OS_init();
    memset( gAppTskStackMain, 0xFF, sizeof( gAppTskStackMain ) );
    TaskP_Params_init(&taskParams);
    taskParams.priority     = 2;
    taskParams.stack        = gAppTskStackMain;
    taskParams.stacksize    = sizeof (gAppTskStackMain);
#if defined(USE_BIOS)
    taskParams.pErrBlk      = &eb;
#endif
    TaskP_create(&OSAL_tests, &taskParams);
#endif
    OS_start();
#endif
    return (0);
}

#if defined(BUILD_MPU) || defined (BUILD_C7X)
extern void Osal_initMmuDefault(void);
void InitMmu(void)
{
    Osal_initMmuDefault();
}
#endif
