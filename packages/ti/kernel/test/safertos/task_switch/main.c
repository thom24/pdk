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

#include <stdlib.h>
#include <string.h>
#include <ti/osal/DebugP.h>
#include "SafeRTOS_API.h"

#define MAIN_TASK_PRI  ( configMAX_PRIORITIES-1 )

#define MAIN_TASK_SIZE ( 4096u )
static portInt8Type  gMainTaskStack[MAIN_TASK_SIZE] __attribute__( ( aligned( MAIN_TASK_SIZE ) ) );

/* Declare task TCB:
 * Due to the use of the MPU background region, by default, all RAM can only
 * be accessed in privileged mode unless a specific MPU region has been setup
 * allowing unprivileged access. */
static xTCB xMainTaskTCB = { 0 };


/* The call to xTaskInitializeScheduler is included within a wrapper
 * initialisation function. */
portBaseType xInitializeScheduler( void );

void task_switch_main( void *args );

int main( void )
{
    portBaseType xStatus;

    /* Initialise the Kernel Scheduler. */
    xStatus = xInitializeScheduler();

    /* Everything OK? */
    if( pdPASS == xStatus )
    {
        /* Yes, try to create the demonstration tasks. */

        memset( gMainTaskStack, 0xFF, sizeof( gMainTaskStack ) );

        /* The structure passed to xTaskCreate() to create the check task. */
        static const xTaskParameters xMainTaskParams =
        {
            &task_switch_main,                  /* The function that implements the task being created. */
            "SafeRTOS_main",                    /* The name of the task being created. The kernel does not use this itself, its just to assist debugging. */
            &xMainTaskTCB,                      /* TCB for the check task. */
            gMainTaskStack,                     /* The buffer allocated for use as the task stack. */
            MAIN_TASK_SIZE,                     /* The size of the buffer allocated for use as the task stack - note this is in BYTES! */
            NULL,                               /* The task parameter, not used in this case. */
            MAIN_TASK_PRI,                      /* The priority to assigned to the task being created. */
            NULL,                               /* Thread Local Storage not used. */
            pdFALSE,                            /* Check task does not use the FPU. */
            {                                   /* MPU task parameters. */
                mpuPRIVILEGED_TASK,             /* Check task is privileged. */
                {
                    { NULL, 0U, 0U, 0U },       /* No additional region definitions are required. */
                    { NULL, 0U, 0U, 0U },
                }
            }
        };

        /* Create the main task. */
        xStatus = xTaskCreate( &xMainTaskParams,       /* The structure containing the task parameters created at the start of this function. */
                                    NULL );            /* This parameter can be used to receive a handle to the created task, but is not used in this case. */

    }
    
    /* Everything OK? */
    if( pdPASS == xStatus )
    {
        /* Yes, try to start the Scheduler. */
        xStatus = xTaskStartScheduler( pdTRUE );
    }
    
    /* The Scheduler should now be running the Demonstration tasks.
     * So this line should never be reached, if it is,
     * then the Scheduler initialisation has failed! */
    DebugP_assert( xStatus );

    return 0;
}
