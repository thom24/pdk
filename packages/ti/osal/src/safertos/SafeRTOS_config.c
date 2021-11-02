/*
 *  Copyright (c) Texas Instruments Incorporated 2021
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
 *  \file  SafeRTOS_config.c
 *
 *  \brief The file implements the safertos OS init configuration.
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ti/csl/csl_types.h>
#include <ti/osal/TimerP.h>
#include <ti/csl/arch/csl_arch.h>
#include <ti/osal/src/nonos/Nonos_config.h>

#include "SafeRTOS_API.h"
#include "SafeRTOS_MPU.h"

#include "DebugP.h"

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* Scheduler Initialisation Definitions */

/* SafeRTOS inspects the vector table to ensure
 * the necessary handlers have been installed. */
#define configSTACK_CHECK_MARGIN            ( 0U )
#define configSYSTEM_STACK_SIZE             ( 0x2000U )

/* The user configuration for the idle task. */
#define configIDLE_TASK_STACK_SIZE          ( 4096U )

/* The user configuration for the timer module. */
#define configTIMER_CMD_QUEUE_BUFFER_SIZE   ( ( configTIMER_QUEUE_LENGTH * sizeof( timerQueueMessageType ) ) + portQUEUE_OVERHEAD_BYTES )

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

static portInt8Type acIdleTaskStack[ configIDLE_TASK_STACK_SIZE ] __attribute__( ( aligned( configIDLE_TASK_STACK_SIZE ) ) ) = { 0 };

/* Declare the stack for the timer task, it cannot be done in the timer
 * module as the syntax for alignment is port specific. Also the callback
 * functions are executed in the timer task and their complexity/stack
 * requirements are application specific. */
static portInt8Type acTimerTaskStack[ configTIMER_TASK_STACK_DEPTH ] __attribute__( ( aligned( configTIMER_TASK_STACK_DEPTH ) ) ) = { 0 };

/* The buffer for the timer command queue. */
static portInt8Type acTimerCommandQueueBuffer[ configTIMER_CMD_QUEUE_BUFFER_SIZE ] = { 0 };

/* Set DLFO bit is defined in utils_asm.asm. */
extern void vPortSetDLFOBit( void );

extern portBaseType xConfigureMPU();

static void prvAbortDelay( void )
{
    const portUnsignedBaseType uxDelayTime = 3500000;
    volatile portUnsignedBaseType uxCount = 0;

    for( uxCount = 0; uxCount < uxDelayTime; uxCount++ )
    {
        ; /* Do nothing. */
    }
}

void vApplicationAbort( void )
{
    for( ;; )
    {
        prvAbortDelay();
    }
}

void vDemoApplicationErrorHook( portTaskHandleType xHandleOfTaskWithError,
                                const portCharType *pcErrorString,
                                portBaseType xErrorCode )
{
    /* The parameters are not used, these lines prevent compiler warnings. */
    ( void ) xHandleOfTaskWithError;
    ( void ) pcErrorString;
    ( void ) xErrorCode;

    /* Will only get here if an internal kernel error occurs. */
    vApplicationAbort();
}

/* The structure passed to xTaskInitializeScheduler() to configure the kernel
 * with the application defined constants and call back functions. */
xPORT_INIT_PARAMETERS gSafertosPortInit =
{
    configCPU_CLOCK_HZ,                 /* ulCPUClockHz */
    configTICK_RATE_HZ,                 /* ulTickRateHz */

    /* Hook Functions */
    NULL,                               /* pxSetupTickInterruptHookFunction */
    NULL,                               /* pxTaskDeleteHookFunction */
    &vDemoApplicationErrorHook,         /* pxErrorHookFunction */
    NULL,                               /* pxIdleHookFunction */
    NULL,                               /* pxTickHookFunction */
    NULL,                               /* pvSvcHookFunction */

    /* System Stack parameters */
#if defined (_TMS320C6X)
    configSYSTEM_STACK_SIZE,            /* uxSystemStackSizeBytes */
#endif
    configSTACK_CHECK_MARGIN,           /* uxAdditionalStackCheckMarginBytes */

    /* Idle Task parameters */
    acIdleTaskStack,                    /* pcIdleTaskStackBuffer */
    configIDLE_TASK_STACK_SIZE,         /* uxIdleTaskStackSizeBytes */
#if defined (BUILD_MCU)
    pdFALSE,                            /* xIdleTaskUsingFPU */
    {                                   /* xIdleTaskMPUParameters */
        mpuUNPRIVILEGED_TASK,           /* The idle hook will be executed in unprivileged mode. */
        {
            { NULL, 0U, 0U, 0U },
            { NULL, 0U, 0U, 0U },
        }
    },
#endif
    NULL,                               /* pvIdleTaskTLSObject */

    /* Timer feature initialisation parameters */
    configTIMER_TASK_PRIORITY,          /* uxTimerTaskPriority */
    configTIMER_TASK_STACK_DEPTH,       /* uxTimerTaskStackSize */
    acTimerTaskStack,                   /* pcTimerTaskStackBuffer */
    configTIMER_QUEUE_LENGTH,           /* uxTimerCommandQueueLength */
    configTIMER_CMD_QUEUE_BUFFER_SIZE,  /* uxTimerCommandQueueBufferSize */
    acTimerCommandQueueBuffer,          /* pcTimerCommandQueueBuffer */
#if defined (BUILD_MCU)
    0,                               /* xEnableCache */
#endif
};

uint32_t gSaftRtosInitDone = (uint32_t) FALSE;

/*-----------------------------------------------------------------------------
 * Routines
 *---------------------------------------------------------------------------*/

void OS_init( void )
{
    portBaseType xInitSchedResult;

    memset( acIdleTaskStack, 0xFF, sizeof( acIdleTaskStack ) );
    memset( acTimerTaskStack, 0xFF, sizeof( acTimerTaskStack ) );
    memset( acTimerCommandQueueBuffer, 0xFF, sizeof( acTimerCommandQueueBuffer ) );


    /* Initialise the kernel by passing in a pointer to the xPORT_INIT_PARAMETERS structure
     * and return the resulting error code. */
    xInitSchedResult = xTaskInitializeScheduler( &gSafertosPortInit );

    /* Assert that xInitSchedResult is successful */
    DebugP_assert((xInitSchedResult == pdPASS));

    xInitSchedResult = xConfigureMPU();

    /* Assert that xInitSchedResult is successful */
    DebugP_assert((xInitSchedResult == pdPASS));

    gSaftRtosInitDone = TRUE;

    /* If configGENERATE_RUN_TIME_STATS is defined then the following
      * macro must be defined to configure the timer/counter used to generate
      * the run time counter time base.   NOTE:  If configGENERATE_RUN_TIME_STATS
      * is set to 0 and the following line fails to build then ensure you do not
      * have portCONFIGURE_TIMER_FOR_RUN_TIME_STATS() defined in your
      * FreeRTOSConfig.h file. */
    /* TODO find alternative in safertosportCONFIGURE_TIMER_FOR_RUN_TIME_STATS(); */

    return;
}

/* return current counter value of high speed counter in units of 10's of usecs */
portUInt32Type ulPortGetRunTimeCounterValue( void )
{
    portUInt64Type ullTimeInUsecs = TimerP_getTimeInUsecs();

    /* note, there is no overflow protection for this 32b value in SafeRTOS
     *
     * Dividing by 10 to reduce the resolution and avoid overflow for longer duration
     */
    return ( portUInt32Type )( ullTimeInUsecs/10 );
}

/*--------------------------------------------------------------------------*/
__attribute__((section(".startupCode")))void __mpu_init( void )
{
    /* Override the startup code mpu initialisation. */
}
/*-------------------------------------------------------------------------*/

/*****************************************************************************/
/* _SYSTEM_POST_CINIT() - _system_post_cinit() is a hook function called in  */
/* the C/C++ auto-initialization function after cinit() and before pinit().  */
/*                                                                           */
/* The version of _system_post_cinit() below is skeletal and is provided to  */
/* illustrate the interface and provide default behavior.  To replace this   */
/* version rewrite the routine and include it as part of the current project.*/
/* The linker will include the updated version if it is linked in prior to   */
/* linking with the C/C++ runtime library.                                   */
/*****************************************************************************/

/*---------------------------------------------------------------------------*/
/* __TI_default_system_post_cinit indicates that the default                 */
__attribute__((section(".startupCode")))void _system_post_cinit(void)
{
    osalArch_Config_t cfg;

    cfg.disableIrqOnInit = true;
    osalArch_Init(&cfg);

    uint32_t            loopCnt = 0U, regAddr;
    CSL_ArmR5CPUInfo    info;
    uint32_t            maxIntrs;
    CSL_vimRegs         *pRegs;

    CSL_armR5GetCpuID(&info);

#if defined(CSL_MAIN_DOMAIN_VIM_BASE_ADDR0) && defined(CSL_MAIN_DOMAIN_VIM_BASE_ADDR1)
    if (info.grpId == (uint32_t)CSL_ARM_R5_CLUSTER_GROUP_ID_0)
    {
        /* MCU SS Pulsar R5 SS */
        regAddr = (info.cpuID == CSL_ARM_R5_CPU_ID_0)?
                                 CSL_MCU_DOMAIN_VIM_BASE_ADDR0:
                                 CSL_MCU_DOMAIN_VIM_BASE_ADDR1;

    }
    else
    {
        /* MAIN SS Pulsar R5 SS */
        regAddr = (info.cpuID == CSL_ARM_R5_CPU_ID_0)?
                                 CSL_MAIN_DOMAIN_VIM_BASE_ADDR0:
                                 CSL_MAIN_DOMAIN_VIM_BASE_ADDR1;

    }
#else
    /* MCU SS Pulsar R5 SS */
    regAddr = (info.cpuID == CSL_ARM_R5_CPU_ID_0)?
                             CSL_MCU_DOMAIN_VIM_BASE_ADDR0:
                             CSL_MCU_DOMAIN_VIM_BASE_ADDR1;
#endif
    pRegs       = (CSL_vimRegs *)(uintptr_t) regAddr;
    maxIntrs    = pRegs->INFO;

#if defined (SOC_AM65XX) || defined (SOC_J721E)
    /* Limit the outstanding transactions to 2
     * only for AM65xx and J721e platforms
     * Later SoCs do not have this issue
     */
    vPortSetDLFOBit();
#else
    /* Fix for PRSDK-8161
     * For AM64x and J7200, there is no issue of limitting outstanding
     * transactions. R5F core can support full 7 outstanding transactions
     */
#endif

    /* Disable/Clear pending Interrupts in VIM before enabling CPU Interrupts */
    /* This is done to prevent serving any bogus interrupt */
    for (loopCnt = 0U ; loopCnt < maxIntrs; loopCnt++)
    {
        /* Disable interrupt in vim */
        CSL_vimSetIntrEnable((CSL_vimRegs *)(uintptr_t)regAddr,
                                    loopCnt,
                                    false);
        /* Clear interrupt status */
        CSL_vimClrIntrPending((CSL_vimRegs *)(uintptr_t)regAddr,
                                     loopCnt);
    }
}
