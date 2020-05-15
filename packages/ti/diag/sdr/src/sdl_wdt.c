/*
 * SDL WDT
 *
 * SafeTI Diagnostics Library module for Watchdog Timer
 *
 * Copyright (C) 2018-2019 Texas Instruments Incorporated - http://www.ti.com/
 * ALL RIGHTS RESERVED
 */

#include <stddef.h>

#include <sdl_common.h>
#include <sdl_wdt.h>
#include <sdl_esm.h>

#include "sdl_wdt_priv.h"

/** ---------------------------------------------------------------------------
 *
 * \brief Defines the values for WDT flag used to track self test
 *
 * ----------------------------------------------------------------------------
 */
typedef enum {
    SDL_WDT_TEST_FLAG_NONE=0,
    /**< Error flag no action */
    SDL_WDT_TEST_FLAG_INPROGRESS=1,
    /**< Error flag to indicate self test in progress */
    SDL_WDT_TEST_FLAG_TRIGGERED=2,
    /**< Error flag to indicate error triggerred */
} SDL_WDT_TestFlag;

/** ---------------------------------------------------------------------------
 * \brief This structure defines the elements of WDT software instance
 *
 * ----------------------------------------------------------------------------
 */
typedef struct SDL_WDT_instance_s
{
    uint64_t elapsedTime;
    /**< Recorded elapsed time in timestamp units */
    uint32_t startTimeStamp;
    /**< Recorded Start time reference */
    uint32_t lastTimeStamp;
    /**< Recorded previous timestamp */
    volatile uint32_t triggerTimeStamp;
    /**< Time stamp when WDT event triggered */
    volatile SDL_ESM_WDT_IntSrc selfTestintSrc;
    /**< Interrupt source expeced with watchdog event */
    volatile SDL_WDT_TimerId selfTestTimerId;
    /**<  Self test timer id */
    volatile SDL_WDT_TestFlag testFlag;
    /**< Test flag to track self test status */
    volatile SDL_WDT_TestType testType;
    /**< Test type */
}  SDL_WDT_Instance_t;

/* Global WDT instance */
static SDL_WDT_Instance_t SDL_WDT_instance;


/* Local function prototypes */
static uint64_t SDL_WDT_getLowTimeout(const SDL_WDT_TimerConfig_t *pTimerConfig);
static SDL_ESM_WDT_IntSrc SDL_WDT_getIntSource(SDL_WDT_TimerId timerId);
static bool SDL_ESM_WDT_callBackFunction (SDL_ESM_WDT_IntSrc intSrc);

/** ============================================================================
 *
 * Requirement: REQ_TAG(PRSDK-2577) REQ_TAG(PRSDK-4211)
 * Design: did_safeti_diagnostics_library did_wdt_diagnostics
 *
 * \brief  Runs diagnostics on configured Watchdog Timer
 *
 * \param  pTimerConfig:  Pointer to timer configuration
 * \param  pTestConfig:   Pointer to test configuration
 *
 * \return  SDL_RETURN_PASS : Success; SDL_RETURN_FAIL for failures
 */
SDL_Result SDL_WDT_selftest (const SDL_WDT_TimerConfig_t *pTimerConfig,
                             const SDL_WDT_TestConfig_t *pTestConfig)
{

    SDL_Result result = SDL_PASS;
    uint32_t timeStamp, timeDelta, triggerDelta;
    uint64_t triggerTime, feedDeltaTime;
    uint64_t lowTimeout;

    feedDeltaTime = ((uint64_t)0ULL);

    SDL_WDT_instance.testFlag = SDL_WDT_TEST_FLAG_NONE;

    if ((pTimerConfig == NULL)
            || (pTestConfig == NULL)){
        result = SDL_BADARGS;
    }

    if ( result == SDL_PASS ) {
        /* Register callback function with ESM */
        (void)SDL_ESM_registerWDTHandler(&SDL_ESM_WDT_callBackFunction);

        /* Calculate the low time out period */
        lowTimeout = SDL_WDT_getLowTimeout(pTimerConfig);

        /* Initialize variables */
        SDL_WDT_instance.elapsedTime = ((uint64_t)0ULL);
        SDL_WDT_instance.testType = pTestConfig->testType;

        /* Record Timer Id */
        SDL_WDT_instance.selfTestTimerId = pTimerConfig->timerId;

        /* Get the interrupt source */
        SDL_WDT_instance.selfTestintSrc = SDL_WDT_getIntSource(pTimerConfig->timerId);

        /* Feed the watchdog to start */
        SDL_WDT_feedWatchdog(pTimerConfig->timerId);

        /* Get start time and record it as both start time and last time stamp */
        timeStamp = SDL_getTime();
        SDL_WDT_instance.startTimeStamp = timeStamp;
        SDL_WDT_instance.lastTimeStamp = timeStamp;

        if (pTestConfig->testType == SDL_WDT_TEST_TYPE_END_VIOLATION) {
            /* Wait for the right time to feed the watchdog */
            do
            {
                /* Get the current time */
                timeStamp = SDL_getTime();

                /* Get delta from last timestamp */
                timeDelta = timeStamp - SDL_WDT_instance.lastTimeStamp;

                /* update last time stamp */
                SDL_WDT_instance.lastTimeStamp = timeStamp;

                /* Update delta time from start time */
                SDL_WDT_instance.elapsedTime += (uint64_t)timeDelta;

            } while (SDL_WDT_instance.elapsedTime < lowTimeout);

            feedDeltaTime = SDL_WDT_instance.elapsedTime;
        }


        /* Set the Test flag to in progress */
        SDL_WDT_instance.testFlag = SDL_WDT_TEST_FLAG_INPROGRESS;

        /* Feed the watchdog */
        SDL_WDT_feedWatchdog(pTimerConfig->timerId);

        /* Wait in a loop for events */
        while(SDL_WDT_instance.testFlag != SDL_WDT_TEST_FLAG_TRIGGERED ) {
            /* Get time stamp */
            timeStamp = SDL_getTime();

            /* Get delta from last timestamp */
            timeDelta = timeStamp - SDL_WDT_instance.lastTimeStamp;

            /* update last time stamp */
            SDL_WDT_instance.lastTimeStamp = timeStamp;

            /* Update delta time from start time */
            SDL_WDT_instance.elapsedTime += (uint64_t)timeDelta;

            /* Check if Timeout exceeded */
            if (SDL_WDT_instance.elapsedTime  > pTestConfig->timeoutPeriod) {
                result = SDL_FAIL;
                break;
            }
        }

        /* Clear self test flag */
        SDL_WDT_instance.testFlag = SDL_WDT_TEST_FLAG_NONE;

        /* Check for timeout */
        if ( result == SDL_PASS ) {
            if ( SDL_WDT_instance.triggerTimeStamp > SDL_WDT_instance.lastTimeStamp) {
                /* Check if the WDT event occured within the window */
                triggerDelta = SDL_WDT_instance.triggerTimeStamp-SDL_WDT_instance.lastTimeStamp;
            } else {
                triggerDelta = ((uint32_t)0u);
            }

            /* Calculate Trigger time */
            triggerTime = (SDL_WDT_instance.elapsedTime - feedDeltaTime) + (uint64_t)triggerDelta ;

            if (pTestConfig->testType == SDL_WDT_TEST_TYPE_END_VIOLATION) {
                /* Make sure Trigger time is within the range */
                if ((triggerTime < lowTimeout)
                    || (triggerTime > (pTestConfig->timeoutPeriod))) {
                    result = SDL_FAIL;
                }
            }
        }
        /* De-register WDT handler */
        SDL_ESM_deRegisterWDTHandler();
    }

    return result;
}

/** ======================================================================
 *
 * Requirement: REQ_TAG(PRSDK-2577) REQ_TAG(PRSDK-4211)
 * Design: did_safeti_diagnostics_library did_wdt_diagnostics
 *
 * \brief   WDT call back function registered with ESM handler
 *
 *
 * \return  None
 */
static bool SDL_ESM_WDT_callBackFunction (SDL_ESM_WDT_IntSrc intSrc)
{
    bool retVal = (bool)false;

    /* If self test intercept event */
    if ((SDL_WDT_instance.testFlag == SDL_WDT_TEST_FLAG_INPROGRESS)
        && (intSrc == SDL_WDT_instance.selfTestintSrc)) {
        if (SDL_WDT_checkWatchDogEvent(intSrc, SDL_WDT_instance.testType)) {
            SDL_WDT_instance.triggerTimeStamp = SDL_getTime();
            SDL_WDT_instance.testFlag = SDL_WDT_TEST_FLAG_TRIGGERED;
        }
        retVal = (bool)true;
    } else {
        SDL_WDT_clearAllInterruptEvents(intSrc);
    }

    return retVal;
}

/** ============================================================================
 *
 * Requirement: REQ_TAG(PRSDK-2577) REQ_TAG(PRSDK-4211)
 * Design: did_safeti_diagnostics_library did_wdt_diagnostics
 *
 * \brief  Calculates the low timeout period for the windowed watchdog.
 *
 * \param  pTimerConfig:  Pointer to timer configuration
 *
 * \return Calculated Low timeout period
 */
static uint64_t SDL_WDT_getLowTimeout(const SDL_WDT_TimerConfig_t *pTimerConfig)
{
    uint64_t loTimeout;


    /* Calculate teh timeout period based on window size */
    switch(pTimerConfig->windowSize) {

        case SDL_WDT_WINDOWSIZE_50_PERCENT:
            loTimeout = (uint64_t)(pTimerConfig->timerPeriod >> 1);
            break;

        case SDL_WDT_WINDOWSIZE_25_PERCENT:
            loTimeout = pTimerConfig->timerPeriod
                        - ((uint64_t)(pTimerConfig->timerPeriod >> 2));
            break;

        case SDL_WDT_WINDOWSIZE_12_5_PERCENT:
            loTimeout = pTimerConfig->timerPeriod
                        - ((uint64_t)(pTimerConfig->timerPeriod >> 3));
            break;

        case SDL_WDT_WINDOWSIZE_6_25_PERCENT:
            loTimeout = pTimerConfig->timerPeriod
                        - ((uint64_t)(pTimerConfig->timerPeriod >> 4));
            break;

        case SDL_WDT_WINDOWSIZE_3_125_PERCENT:
            loTimeout = pTimerConfig->timerPeriod
                        - ((uint64_t)(pTimerConfig->timerPeriod >> 5));
            break;

        case SDL_WDT_WINDOWSIZE_100_PERCENT:
        default:
            loTimeout = ((uint64_t)0ULL);
            break;
    }

    return loTimeout;

}

/** ============================================================================
 *
 * Requirement: REQ_TAG(PRSDK-2577) REQ_TAG(PRSDK-4211)
 * Design: did_safeti_diagnostics_library did_wdt_diagnostics
 *
 * \brief  Gets watch dog timer interrupt source for the given Timer id
 *
 * \param  timerId: Timer Id which timer to get the corresponding interrupt source
 *
 * \return Interrupt source number
 */
static SDL_ESM_WDT_IntSrc SDL_WDT_getIntSource(SDL_WDT_TimerId timerId)
{
    SDL_ESM_WDT_IntSrc intSrc;


    /* Calculate teh timeout period based on window size */
    switch(timerId) {

        case SDL_WDT_TIMER_1:
            intSrc = SDL_ESM_TIMER_ID_1;
            break;

        case SDL_WDT_TIMER_0:
        default:
            intSrc = SDL_ESM_TIMER_ID_0;
            break;
    }

    return intSrc;

}
