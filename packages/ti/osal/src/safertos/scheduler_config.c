/*
    Copyright (C)2006 onward WITTENSTEIN aerospace & simulation limited. All rights reserved.

    This file is part of the SafeRTOS product, see projdefs.h for version number
    information.

    SafeRTOS is distributed exclusively by WITTENSTEIN high integrity systems,
    and is subject to the terms of the License granted to your organization,
    including its warranties and limitations on use and distribution. It cannot be
    copied or reproduced in any way except as permitted by the License.

    Licenses authorize use by processor, compiler, business unit, and product.

    WITTENSTEIN high integrity systems is a trading name of WITTENSTEIN
    aerospace & simulation ltd, Registered Office: Brown's Court, Long Ashton
    Business Park, Yanley Lane, Long Ashton, Bristol, BS41 9LB, UK.
    Tel: +44 (0) 1275 395 600, fax: +44 (0) 1275 393 630.
    E-mail: info@HighIntegritySystems.com

    www.HighIntegritySystems.com
*/

/* Generic headers */
#include <stdlib.h>
#include <string.h>

#include "SafeRTOS_API.h"

/*-----------------------------------------------------------------------------
 * Local Constants
 *---------------------------------------------------------------------------*/

/* Scheduler Initialisation Definitions */

/* SafeRTOS inspects the vector table to ensure
 * the necessary handlers have been installed. */
#define configSTACK_CHECK_MARGIN            ( 0U )
#define configSYSTEM_STACK_SIZE             ( 0x2000U )

/* The user configuration for the idle task. */
#define configIDLE_TASK_STACK_SIZE          ( configMINIMAL_STACK_SIZE )

/* The user configuration for the timer module. */
#define configTIMER_CMD_QUEUE_BUFFER_SIZE   ( ( configTIMER_QUEUE_LENGTH * sizeof( timerQueueMessageType ) ) + portQUEUE_OVERHEAD_BYTES )

/* Hook Functions */
portSETUP_TICK_HOOK pxApplicationSetupTickInterruptHook = NULL;          /* pxSetupTickInterruptHookFunction */
portTASK_DELETE_HOOK pxApplicationTaskDeleteHook = NULL;    /* pxTaskDeleteHookFunction */
portERROR_HOOK pxApplicationErrorHook = NULL;                            /* pxErrorHookFunction */
portIDLE_HOOK pxApplicationIdleHook = NULL;                                                 /* pxIdleHookFunction */
portTICK_HOOK pxApplicationTickHook = NULL;                                                 /* pxTickHookFunction */
portSVC_HOOK pxApplicationSvcHook = NULL;                       /* pxSvcHookFunction */

/*-----------------------------------------------------------------------------
 * Local Prototypes
 *---------------------------------------------------------------------------*/

/* The call to xTaskInitializeScheduler is included within a wrapper
 * initialisation function. */
portBaseType xInitializeScheduler( void );


/*-----------------------------------------------------------------------------
 * Local Variables
 *---------------------------------------------------------------------------*/

static portInt8Type acIdleTaskStack[ configIDLE_TASK_STACK_SIZE ] = { 0 };

/* Declare the stack for the timer task, it cannot be done in the timer
 * module as the syntax for alignment is port specific. Also the callback
 * functions are executed in the timer task and their complexity/stack
 * requirements are application specific. */
static portInt8Type acTimerTaskStack[ configTIMER_TASK_STACK_DEPTH ] = { 0 };

/* The buffer for the timer command queue. */
static portInt8Type acTimerCommandQueueBuffer[ configTIMER_CMD_QUEUE_BUFFER_SIZE ] = { 0 };

/*-----------------------------------------------------------------------------
 * Routines
 *---------------------------------------------------------------------------*/

portBaseType xInitializeScheduler( void )
{
    portBaseType xInitSchedResult;

    memset( acIdleTaskStack, 0xFF, sizeof( acIdleTaskStack ) );
    memset( acTimerTaskStack, 0xFF, sizeof( acTimerTaskStack ) );
    memset( acTimerCommandQueueBuffer, 0xFF, sizeof( acTimerCommandQueueBuffer ) );

    /* The structure passed to xTaskInitializeScheduler() to configure the kernel
     * with the application defined constants and call back functions. */
    xPORT_INIT_PARAMETERS xPortInit =
    {
        configCPU_CLOCK_HZ,                 /* ulCPUClockHz */
        configTICK_RATE_HZ,                 /* ulTickRateHz */

        /* Hook Functions */
        pxApplicationSetupTickInterruptHook,    /* pxSetupTickInterruptHookFunction */
        pxApplicationTaskDeleteHook,            /* pxTaskDeleteHookFunction */
        pxApplicationErrorHook,                 /* pxErrorHookFunction */
        pxApplicationIdleHook,                  /* pxIdleHookFunction */
        pxApplicationTickHook,                  /* pxTickHookFunction */
        pxApplicationSvcHook,                   /* pxSvcHookFunction */

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

    /* Initialise the kernel by passing in a pointer to the xPortInit structure
     * and return the resulting error code. */
    xInitSchedResult = xTaskInitializeScheduler( &xPortInit );

    /* If configGENERATE_RUN_TIME_STATS is defined then the following
      * macro must be defined to configure the timer/counter used to generate
      * the run time counter time base.   NOTE:  If configGENERATE_RUN_TIME_STATS
      * is set to 0 and the following line fails to build then ensure you do not
      * have portCONFIGURE_TIMER_FOR_RUN_TIME_STATS() defined in your
      * FreeRTOSConfig.h file. */
    portCONFIGURE_TIMER_FOR_RUN_TIME_STATS();

    return xInitSchedResult;
}
/*--------------------------------------------------------------------------*/
