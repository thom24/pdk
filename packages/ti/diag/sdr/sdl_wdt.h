/*
 * SDL WDT
 *
 * SafeTI Diagnostics Library module for Watch Dog Timer module
 *
 * Copyright (C) 2019 Texas Instruments Incorporated - http://www.ti.com/
 * ALL RIGHTS RESERVED
 */

/**
 * @file  sdl_wdt.h
 *
 * @brief
 *  Header file contains enumerations, structure definitions and function
 *  declarations for SDL WDT interface.
 *  ============================================================================
 */

#ifndef INCLUDE_SDL_WDT_H_
#define INCLUDE_SDL_WDT_H_

#include "sdl_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/** ===========================================================================
 *
 * @defgroup SDL_WDT_API SDL WDT API
 *
 * @section Overview
 * The SDL WDT module provides API for WatchDog Timer Diagnostics
 *
 * ============================================================================
 */
/**
@defgroup SDL_WDT_DATASTRUCT  SDL WDT Data Structures
@ingroup SDL_WDT_API
*/
/**
@defgroup SDL_WDT_FUNCTION  SDL WDT Functions
@ingroup SDL_WDT_API
*/
/**
@defgroup SDL_WDT_ENUM SDL WDT Enumerated Data Types
@ingroup SDL_WDT_API
*/

/**
@defgroup SDL_WDT_MACROS SDL WDT Macro defines
@ingroup SDL_WDT_API
*/

/** ===========================================================================
 *  @addtogroup SDL_WDT_ENUM
    @{
 * ============================================================================
 */

/** ---------------------------------------------------------------------------
 * \brief This enumerator defines the different Watchdog Timer Ids
 * ----------------------------------------------------------------------------
 */
typedef enum {
    SDL_WDT_TIMER_0 = 1,
    /**<  Timer id 0 */
    SDL_WDT_TIMER_1 = 2,
    /**<  Timer id 1 */
} SDL_WDT_TimerId;

/** ---------------------------------------------------------------------------
 * \brief This enumerator defines the different Watchdog window sizes
 * ----------------------------------------------------------------------------
 */
typedef enum {
    SDL_WDT_WINDOWSIZE_100_PERCENT = 1,
    /**<  Watchdog window size 100 % */
    SDL_WDT_WINDOWSIZE_50_PERCENT = 2,
    /**<  Watchdog window size 50 % */
    SDL_WDT_WINDOWSIZE_25_PERCENT = 3,
    /**<  Watchdog window size 25 % */
    SDL_WDT_WINDOWSIZE_12_5_PERCENT = 4,
    /**<  Watchdog window size 12.5 % */
    SDL_WDT_WINDOWSIZE_6_25_PERCENT = 5,
    /**<  Watchdog window size 6.25 % */
    SDL_WDT_WINDOWSIZE_3_125_PERCENT = 6,
    /**<  Watchdog window size 3.125 % */
} SDL_WDT_WindowSize;

/** ---------------------------------------------------------------------------
 * \brief This enumerator defines the type of test
 * ----------------------------------------------------------------------------
 */
typedef enum {
    SDL_WDT_TEST_TYPE_END_VIOLATION = 1,
    /**<  Test type End Violation */
    SDL_WDT_TEST_TYPE_WINDOW_VIOLATION = 2,
    /**<  Test type window Violation */
} SDL_WDT_TestType;

/* @} */

/**
 *  \addtogroup SDL_WDT_MACROS
 *  @{
 */


/* @} */

/**
 *  \addtogroup SDL_WDT_DATASTRUCT
 *  @{
 */

/** ---------------------------------------------------------------------------
 * \brief WDT Timer configuration
 *
 * This structure defines the elements of the Timer configuration
 * ----------------------------------------------------------------------------
 */
typedef struct SDL_WDT_TimerConfig_s
{
    uint64_t timerPeriod;
    /**< Watchdog timer period in timestamp units */
    SDL_WDT_TimerId timerId;
    /**< Unique Timer identifier  */
    SDL_WDT_WindowSize windowSize;
    /**< Window size  */
} SDL_WDT_TimerConfig_t;

/** ---------------------------------------------------------------------------
 * \brief Watchdog timer test configuation
 *
 * This structure defines the elements of the test configuration
 * ----------------------------------------------------------------------------
 */
typedef struct SDL_WDT_TestConfig_s
{
    uint64_t timeoutPeriod;
    /**< Watchdog time out period in timestamp units  */
    SDL_WDT_TestType testType;
    /**< Test type  */
} SDL_WDT_TestConfig_t;

/* @} */

/**
 *  \addtogroup SDL_WDT_FUNCTION
 *  @{
 */

/** ============================================================================
 *
 * \brief  Runs diagnostics on configured Watchdog Timer
 *
 * \param  pTimerConfig:  Pointer to timer configuration
 * \param  pTestConfig:   Pointer to test configuration
 *
 * \return  SDL_RETURN_PASS : Success; SDL_RETURN_FAIL for failures
 */
SDL_Result SDL_WDT_selftest (const SDL_WDT_TimerConfig_t *pTimerConfig,
                             const SDL_WDT_TestConfig_t *pTestConfig);

/* @} */

#ifdef __cplusplus
}
#endif  /* extern "C" */

#endif /* INCLUDE_SDL_WDT_H_ */
