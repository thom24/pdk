/*
 *  Copyright ( C ) 2022 Texas Instruments Incorporated
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
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES ( INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION ) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  ( INCLUDING NEGLIGENCE OR OTHERWISE ) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdlib.h>
#include <string.h>
#include <ti/osal/DebugP.h>
#include "SafeRTOS_API.h"

#define MAIN_TASK_PRI  ( 4U )

#define MAIN_TASK_SIZE ( 4096U )
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
        xStatus = xTaskStartScheduler();
    }
    
    /* The Scheduler should now be running the Demonstration tasks.
     * So this line should never be reached, if it is,
     * then the Scheduler initialisation has failed! */
    DebugP_assert( xStatus );

    return 0;
}
