/*
 * Copyright (C) 2024 Texas Instruments Incorporated - http://www.ti.com/
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

/**
 *  \file   osal_extended_testapp.c
 *
 *  \brief  OSAL driver test application main file.
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

#include "osal_extended_test.h"

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

volatile uint64_t gTestlocalTimeout = 0x100000U;
volatile bool gFlagSwi = false, gFlagIRQ = false;

/* for EventP APIs */
EventP_Handle gEventP_handle;
uint32_t geventMask = EventP_ID_01;
extern int gOsalEventPFreeRtosPool[OSAL_FREERTOS_CONFIGNUM_EVENT];
extern uint32_t gOsalEventAllocCnt;

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
    int32_t result = osal_OK;
    Board_initOSAL();

#if defined(SAFERTOS)

    ClockP_start_HwiP_Test();

    Clockp_safertos_Test();

    ClockP_create_test_clocks_limit();

    ClockP_Params_init_NullTest();

    ClockP_delete_NullTest();

    ClockP_create_callback_NullTest();

#endif

#if defined(FREERTOS)
    Clockp_freertos_Test();

    ClockP_create_test_clocks_limit_freertos();

    ClockP_start_stop_ptimer_used_false_test();

    ClockP_timerCallbackFunction_NULL_test();

    DebugP_log_Test();

    result += OsalApp_heapFreertosTest();

    result += OsalApp_queueTests();

    Sema_create_extended_mem_block_test();

    Semaphore_create_null_param_test();

    EventP_Max_Test();

    loadP_freertos_test();

#endif

#if defined(BARE_METAL)

    result += OsalApp_cacheTests();

    CycleprofilerP_nonos_Test();

    SwiP_nonos_Test();

    RegisterIntr_nonos_NegTest();

    RegisterIntrDirect_NegTest();

    Utils_nonos_test();

    Utils_nonos_SetHwAttr_Multi_Test();
    
    result += OsalApp_hwiTests();

    SemaphoreP_create_extended_mem_block_test();

    SemaphoreP_pend_test();

    Sema_pend_semacount0_test();

    Sema_delete_sems_used_zero_test();

    result += OsalApp_cacheTests();

    result += OsalApp_mutexTests();

#endif

#if !defined(BARE_METAL)

    RegisterIntr_Test();

    EventP_Test();

    result += OsalApp_taskTests();

    Semaphore_flow_test();

    Semaphore_compile_time_size_test();

    ClockP_init_start_stop_null_test();

    result += OsalApp_mutexTests();
    
    result += OsalApp_hwiTests();
    
    result += OsalApp_mailboxTests();

#endif

    if(osal_OK == result)
    {
        OSAL_log("\n All tests have passed. \n");
    }
    else
    {
        OSAL_log("\n Some tests have failed. \n");
    }

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

    TaskP_Params taskParams;

    OS_init();
    memset( gAppTskStackMain, 0xFF, sizeof( gAppTskStackMain ) );
    TaskP_Params_init(&taskParams);
    taskParams.priority     = 2;
    taskParams.stack        = gAppTskStackMain;
    taskParams.stacksize    = sizeof (gAppTskStackMain);

    if (NULL_PTR != TaskP_self())
    {
        OSAL_log("TaskP self should return NULL, as this is main thread..!");
    }
    else
    {
        TaskP_create(&OSAL_tests, &taskParams);
        OS_start();
    }
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
