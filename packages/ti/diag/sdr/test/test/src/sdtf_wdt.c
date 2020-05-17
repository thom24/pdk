/*
 * SDTF WDT
 *
 * Software Diagnostics Library Test
 *
 *  Copyright (c) Texas Instruments Incorporated 2019-2020
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
 *
 */

#include <stdint.h>

#include <ti/csl/hw_types.h>
#include <ti/csl/csl_rti.h>
#include <ti/csl/soc.h>

#include <sdl_common.h>
#include <sdl_wdt.h>

#include <ti/osal/osal.h>

#include "sdtf_common.h"
#include "sdtf_wdt.h"
#include "sdtf_test.h"
#include "sdtf_profile.h"

typedef enum rtiClockSource
{
    RTI_CLOCK_SOURCE_HFOSC0_CLKOUT = 0U,
    /**< to select clock frequency of hfosc0 */
    RTI_CLOCK_SOURCE_LFOSC_CLKOUT = 1U,
    /**< to select clock frequency of lfosc */
    RTI_CLOCK_SOURCE_12MHZ = 2U,
    /**< to select clock frequency of 12 MHz */
    RTI_CLOCK_SOURCE_32KHZ = 3U,
    /**< to select clock frequency of 32KHz */
}rtiClockSource_t;

#ifdef SDTF_WDT_THROUGH_INTERRUPT
/* ============================================= */
volatile uint32_t isrFlag = 0U;

static void RTIAppISR(uintptr_t handle)
{
    uint32_t intrStatus;

    RTIDwwdGetStatus(CSL_MCU_RTI1_CFG_BASE, &intrStatus);
    RTIDwwdClearStatus(CSL_MCU_RTI1_CFG_BASE, intrStatus);
    SDTF_printf(": Interrupt Generated!!!\n");

    isrFlag  = 1U;
}

static void RTIInterruptConfig()
{
    OsalRegisterIntrParams_t intrPrms;
    OsalInterruptRetCode_e osalRetVal;
    HwiP_Handle hwiHandle;
    uint32_t rtiIntNum;

    isrFlag  = 0U;

    Osal_RegisterInterrupt_initParams(&intrPrms);
    intrPrms.corepacConfig.arg          = (uintptr_t)0;
    intrPrms.corepacConfig.priority     = 1U;
    intrPrms.corepacConfig.corepacEventNum = 0U; /* NOT USED ? */

    rtiIntNum = CSL_MCU0_INTR_RTI1_INTR_WWD;

    intrPrms.corepacConfig.isrRoutine   = &RTIAppISR;
    intrPrms.corepacConfig.intVecNum    = rtiIntNum;

    osalRetVal = Osal_RegisterInterrupt(&intrPrms, &hwiHandle);
    if(OSAL_INT_SUCCESS != osalRetVal)
    {
        SDTF_printf("Error Could not register ISR !!!\n");
    }

    return;
}
#endif

int32_t SDTF_WDT_init(void)
{
    int32_t result = 0;
    uint32_t preloadValue;
    int32_t cslStatus = CSL_PASS;
    uint32_t intrStatus;

    /* Add RTI handling through direct CPU interrupt */
    /* Disable this for now to avoid interference with self test */
    /* RTIInterruptConfig(); */

    /* Clear any pending interrupt */
    RTIDwwdGetStatus(CSL_MCU_RTI1_CFG_BASE, &intrStatus);
    RTIDwwdClearStatus(CSL_MCU_RTI1_CFG_BASE, intrStatus);

    /* Calculate Reload value */
    preloadValue = (uint32_t)SDTF_RTI_CLOCK_SOURCE_32KHZ*(uint32_t)SDTF_RTI_DWWD_TIMEOUT_VALUE;

    /* Select RTI module clock source */
    HW_WR_FIELD32(CSL_MCU_CTRL_MMR0_CFG0_BASE +
                  CSL_MCU_CTRL_MMR_CFG0_MCU_RTI1_CLKSEL,
                  CSL_MCU_CTRL_MMR_CFG0_MCU_RTI1_CLKSEL_CLK_SEL,
                  RTI_CLOCK_SOURCE_32KHZ);
    /* Configure Watchdog */
    cslStatus = RTIDwwdWindowConfig(CSL_MCU_RTI1_CFG_BASE,
                                    RTI_DWWD_REACTION_GENERATE_NMI,
                                    preloadValue,
                                    RTI_RTIDWWDSIZECTRL_DWWDSIZE_50_PERCENT);
    if (cslStatus !=  CSL_PASS) {
        result = -1;
    }

    if (result == 0) {
        cslStatus =  RTIDwwdWindowVerifyConfig(CSL_MCU_RTI1_CFG_BASE,
                                               RTI_DWWD_REACTION_GENERATE_NMI,
                                               preloadValue,
                                               RTI_RTIDWWDSIZECTRL_DWWDSIZE_50_PERCENT);
        if (cslStatus !=  CSL_PASS) {
            result = -1;
        }
    }

    RTIDwwdCounterEnable(CSL_MCU_RTI1_CFG_BASE);

    return result;

}


void SDTF_WDT_feedWatchdogTimer(void)
{
    RTIDwwdService(CSL_MCU_RTI1_CFG_BASE);
}

/*********************************************************************
 *
 * \brief   Execute Watchdog self test for end violation
 *
 * \param   None
 *
 * \return  0 : Success; < 0 for failures
 */
int32_t SDTF_runWDTSelfTestEndViolation(void)
{
    SDL_WDT_TimerConfig_t timerConfig;
    SDL_WDT_TestConfig_t testConfig;
    SDL_Result result;
    int32_t retVal=0;

    SDTF_printf("\n Watchdog self test end violation: starting");

    SDTF_profileBegin(SDTF_PROFILE_ONESHOT);

    timerConfig.timerId = SDL_WDT_TIMER_1;
    timerConfig.timerPeriod = SDTF_RTI_DWWD_TIMEOUT_VALUE * SDTF_PROFILE_CLOCK_FREQUENCY;
    timerConfig.windowSize = SDTF_RTI_DWWDSIZE;

    testConfig.testType = SDL_WDT_TEST_TYPE_END_VIOLATION;
    testConfig.timeoutPeriod = (uint64_t)20000*SDTF_PROFILE_CLOCK_FREQUENCY;

    result = SDL_WDT_selftest (&timerConfig, &testConfig);

    SDTF_profileEnd(SDTF_PROFILE_ONESHOT);

    if (result != SDL_PASS ) {
        SDTF_printf("\n Watchdog self test end violation failed");
       retVal = -1;
    } else {
        SDTF_printf("\n Watchdog self test end violation complete");
        SDTF_printf("\n Cycles taken %u", SDTF_profileDelta(SDTF_PROFILE_ONESHOT));
    }

    return retVal;
}

/*********************************************************************
 *
 * \brief   Execute Watchdog self test for window Violation
 *
 * \param   None
 *
 * \return  0 : Success; < 0 for failures
 */
int32_t SDTF_runWDTSelfTestWindowViolation(void)
{
    SDL_WDT_TimerConfig_t timerConfig;
    SDL_WDT_TestConfig_t testConfig;
    SDL_Result result;
    int32_t retVal=0;

    SDTF_printf("\n Watchdog self test window violation: starting");

    SDTF_profileBegin(SDTF_PROFILE_ONESHOT);

    timerConfig.timerId = SDL_WDT_TIMER_1;
    timerConfig.timerPeriod = SDTF_RTI_DWWD_TIMEOUT_VALUE * SDTF_PROFILE_CLOCK_FREQUENCY;
    timerConfig.windowSize = SDTF_RTI_DWWDSIZE;

    testConfig.testType = SDL_WDT_TEST_TYPE_WINDOW_VIOLATION;
    testConfig.timeoutPeriod = (uint64_t)20000*SDTF_PROFILE_CLOCK_FREQUENCY;

    result = SDL_WDT_selftest (&timerConfig, &testConfig);

    SDTF_profileEnd(SDTF_PROFILE_ONESHOT);

    if (result != SDL_PASS ) {
        SDTF_printf("\n Watchdog self test window violation failed");
       retVal = -1;
    } else {
        SDTF_printf("\n Watchdog self test window violation complete");
        SDTF_printf("\n Cycles taken %u", SDTF_profileDelta(SDTF_PROFILE_ONESHOT));
    }

    return retVal;
}

/*********************************************************************
 *
 * \brief   Execute Watchdog negative tests
 *
 * \param   None
 *
 * \return  0 : Success; < 0 for failures
 */
int32_t SDTF_runWDTNegativeTests(void)
{
    SDL_WDT_TimerConfig_t timerConfig;
    SDL_WDT_TestConfig_t testConfig;
    SDL_Result result;
    int32_t retVal=0;

    SDTF_printf("\n Watchdog negative tests: starting");

    timerConfig.timerId = SDL_WDT_TIMER_1;
    timerConfig.timerPeriod = SDTF_RTI_DWWD_TIMEOUT_VALUE * SDTF_PROFILE_CLOCK_FREQUENCY;
    timerConfig.windowSize = SDTF_RTI_DWWDSIZE;

    testConfig.testType = SDL_WDT_TEST_TYPE_END_VIOLATION;
    testConfig.timeoutPeriod = (uint64_t)20000*SDTF_PROFILE_CLOCK_FREQUENCY;

    result = SDL_WDT_selftest (NULL_PTR, &testConfig);

    /* There is expecte error here */
    if (result == SDL_PASS) {
        retVal = -1;
    }

    if (retVal == 0) {
        result = SDL_WDT_selftest (&timerConfig, NULL_PTR);

        /* There is expecte error here */
        if (result == SDL_PASS) {
            retVal = -1;
        }
    }

    if (retVal == 0) {

        /* Specifically give lower timeout */
        testConfig.timeoutPeriod = (uint64_t)10*SDTF_PROFILE_CLOCK_FREQUENCY;

        result = SDL_WDT_selftest (&timerConfig, &testConfig);

        /* There is expecte error here */
        if (result == SDL_PASS) {
            retVal = -1;
        }
    }

    if (retVal != 0 ) {
        SDTF_printf("\n Watchdog negative tests failed");
       retVal = -1;
    } else {
        SDTF_printf("\n Watchdog negative tests complete");
    }

    return retVal;
}

