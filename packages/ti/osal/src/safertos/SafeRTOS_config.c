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

#include <stdint.h>
#include <string.h>
#include <ti/csl/csl_types.h>

#include "SafeRTOS_API.h"

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* Scheduler Initialisation Definitions */

/* SafeRTOS inspects the vector table to ensure
 * the necessary handlers have been installed. */
#define configSTACK_CHECK_MARGIN            ( 0U )
#define configSYSTEM_STACK_SIZE             ( 0x2000U )

/* The user configuration for the idle task. */
#define configIDLE_TASK_STACK_SIZE          ( configMINIMAL_STACK_SIZE )

/* The user configuration for the timer module. */
#define configTIMER_CMD_QUEUE_BUFFER_SIZE   ( ( configTIMER_QUEUE_LENGTH * sizeof( timerQueueMessageType ) ) + portQUEUE_OVERHEAD_BYTES )

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

static portInt8Type acIdleTaskStack[ configIDLE_TASK_STACK_SIZE ] = { 0 };

/* Declare the stack for the timer task, it cannot be done in the timer
 * module as the syntax for alignment is port specific. Also the callback
 * functions are executed in the timer task and their complexity/stack
 * requirements are application specific. */
static portInt8Type acTimerTaskStack[ configTIMER_TASK_STACK_DEPTH ] = { 0 };

/* The buffer for the timer command queue. */
static portInt8Type acTimerCommandQueueBuffer[ configTIMER_CMD_QUEUE_BUFFER_SIZE ] = { 0 };

/* The structure passed to xTaskInitializeScheduler() to configure the kernel
 * with the application defined constants and call back functions. */
xPORT_INIT_PARAMETERS gSafertosPortInit =
{
    configCPU_CLOCK_HZ,                 /* ulCPUClockHz */
    configTICK_RATE_HZ,                 /* ulTickRateHz */

    /* Hook Functions */
    NULL,                               /* pxSetupTickInterruptHookFunction */
    NULL,                               /* pxTaskDeleteHookFunction */
    NULL,                               /* pxErrorHookFunction */
    NULL,                               /* pxIdleHookFunction */
    NULL,                               /* pxTickHookFunction */
    NULL,                               /* pxSvcHookFunction */

    /* System Stack parameters */
    configSYSTEM_STACK_SIZE,            /* uxSystemStackSizeBytes */
    configSTACK_CHECK_MARGIN,           /* uxAdditionalStackCheckMarginBytes */

    /* Idle Task parameters */
    acIdleTaskStack,                    /* pcIdleTaskStackBuffer */
    configIDLE_TASK_STACK_SIZE,         /* uxIdleTaskStackSizeBytes */
    NULL,                               /* pvIdleTaskTLSObject */

    /* Timer feature initialisation parameters */
    configTIMER_TASK_PRIORITY,          /* uxTimerTaskPriority */
    configTIMER_TASK_STACK_DEPTH,        /* uxTimerTaskStackSize */
    acTimerTaskStack,                   /* pcTimerTaskStackBuffer */
    configTIMER_QUEUE_LENGTH,          /* uxTimerCommandQueueLength */
    configTIMER_CMD_QUEUE_BUFFER_SIZE,  /* uxTimerCommandQueueBufferSize */
    acTimerCommandQueueBuffer,          /* pcTimerCommandQueueBuffer */
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

    gSaftRtosInitDone = TRUE;

    /* If configGENERATE_RUN_TIME_STATS is defined then the following
      * macro must be defined to configure the timer/counter used to generate
      * the run time counter time base.   NOTE:  If configGENERATE_RUN_TIME_STATS
      * is set to 0 and the following line fails to build then ensure you do not
      * have portCONFIGURE_TIMER_FOR_RUN_TIME_STATS() defined in your
      * FreeRTOSConfig.h file. */
    portCONFIGURE_TIMER_FOR_RUN_TIME_STATS();

    return;
}

/*--------------------------------------------------------------------------*/
