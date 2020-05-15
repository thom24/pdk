/*
 *  Copyright (c) 2018 Texas Instruments Incorporated
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
/* For Timer functions */
#include <ti/osal/osal.h>

/* For UART print outputs */
#include <ti/drv/uart/UART_stdio.h>

/* Safety Diagnostics Test framework header files */
#include "sdtf_common.h"
#include "sdtf_init.h"
#include "sdtf_test.h"
#include "sdtf_platform.h"

/*****************************************************************************
 * This is the main function for the Safety Diagnostic Test framework test
 * application.
 * Initializes the required peripherals and different SDL related modules
 * and calls the one shot test functions first and then initiates a periodic
 * function for periodic tests.
 */

int32_t main(void)
{
    
    int32_t retValue;

    TimerP_Params   periodicTimerParams;
    TimerP_Handle   periodicTimerHandle;
    TimerP_Status   timerStatus;

    /* Initialize the test framework */
    retValue = SDTF_init();
    if (retValue < 0) {
        /* print and exit */
        SDTF_printf("\nERR: SDTF_init failed");
        return -1;
    }
    SDTF_printf("\nSDTF init complete");

    SDTF_printf("\nSDTF One shot tests: Starting ");
    /* execute one shot functions */
    retValue = SDTF_oneShotTestAllModules();
    if (retValue < 0) {
        /* print and exit */
        SDTF_printf("\nERR: One shot tests failed");
        return -1;
    }
    SDTF_printf("\nSDTF One shot tests: Complete");

    SDTF_printf("\nSDTF Periodic tests: Starting ");
    /* Start Timer and register call back for periodic functions */
    /* Initialize timer parameters */
    TimerP_Params_init(&periodicTimerParams);

    /* Configure periodic timer for 1 sec*/
    periodicTimerParams.period = 1000000;  /*  1s */
    periodicTimerParams.periodType = TimerP_PeriodType_MICROSECS;
    periodicTimerParams.arg = &periodicTimerHandle;
    periodicTimerParams.startMode = TimerP_StartMode_USER;
    periodicTimerParams.runMode = TimerP_RunMode_CONTINUOUS;

    /* Create Periodic Timer */
    periodicTimerHandle = TimerP_create(TIMER_ID,  
                                     (TimerP_Fxn)&SDTF_triggerPeriodicTests,
                                     &periodicTimerParams);
    if (periodicTimerHandle == NULL) {
       SDTF_printf("ERR: Timer create failed\n");
       return -1;
    }

    /* Start the Timer */
    timerStatus = TimerP_start(periodicTimerHandle);
    if (timerStatus != TimerP_OK) {
        SDTF_printf("ERR: Could not start the timer %d \n", TIMER_ID);
       return -1;
    }

    /* Wait for all tasks ended */
    while(!SDTF_allTasksEnded) {
        /* Execute periodic tasks*/
        retValue = SDTF_runPeriodicTests();
        if (retValue < 0) {
            /* If failure: print and exit */
            SDTF_printf("\nERR: Periodic tests failed");
            return -1;
        }
    }

    SDTF_printf("\nSDTF Periodic tests: Complete");

    SDTF_printf("\nSDTF Interactive mode: Starting");

    /* Execute Interactive tests */
    retValue = SDTF_runInteractiveTests();

    SDTF_printf("\nSDTF Interactive mode: Complete");

    if ( retValue < 0) {
        /* If failure: print and exit */
        SDTF_printf("\nERR: Interactive test failed");
        return -1;
    }

    UART_printStatus("\n All Tests passed");

    /* Close timer task */

    return 0;

}
