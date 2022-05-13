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


/* SafeRTOS Includes */
#include "SafeRTOS_API.h"
#include "aborts.h"

#include <ti/osal/TimerP.h>

/* Timer Hook function handlers targeting the TI PDK libraries. */


/* Timer interrupt handler function. */
static void prvTimerTickIsr( uintptr_t arg );
void vApplicationSetupTickInterruptHook( portUInt32Type ulTimerClockHz,
                                         portUInt32Type ulTickRateHz );

/*-------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * Private function definitions.
 *---------------------------------------------------------------------------*/

static void prvTimerTickIsr( uintptr_t arg )
{
    ( void ) arg;
    vTaskProcessSystemTickFromISR();
}

/*-------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * Public function definitions.
 *---------------------------------------------------------------------------*/

/* Tick timer setup using TI PDK timers and interrupts. */
void vApplicationSetupTickInterruptHook( portUInt32Type ulTimerClockHz,
                                         portUInt32Type ulTickRateHz )
{
    TimerP_Handle pxTickTimerHandle = NULL;
    TimerP_Params xTimerParams;

    ( void ) ulTimerClockHz;

    TimerP_Params_init( &xTimerParams );
    xTimerParams.runMode    = TimerP_RunMode_CONTINUOUS;
    xTimerParams.startMode  = TimerP_StartMode_USER;
    xTimerParams.periodType = TimerP_PeriodType_MICROSECS;
    xTimerParams.period     = ( 1000000UL / ulTickRateHz );

    pxTickTimerHandle = TimerP_create( configTIMER_ID,
                                       &prvTimerTickIsr,
                                       &xTimerParams );

    /* don't expect the handle to be null or the timer to start OK */
    if( ( NULL == pxTickTimerHandle ) ||
        ( TimerP_OK != TimerP_start( pxTickTimerHandle ) ) )
    {
        vApplicationAbort();
    }
}

/*-------------------------------------------------------------------------*/

