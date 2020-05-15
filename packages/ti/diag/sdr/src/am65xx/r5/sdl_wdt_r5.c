/*
 * SDL WDT
 *
 * SafeTI Diagnostics Library module for Watchdog Timer
 *
 * Copyright (C) 2018-2019 Texas Instruments Incorporated - http://www.ti.com/
 * ALL RIGHTS RESERVED
 */

#include <stddef.h>
#include <stdbool.h>

#include <ti/csl/csl_rti.h>
#include <ti/csl/soc.h>

#include <sdl_common.h>
#include <sdl_wdt.h>

#include "sdl_wdt_priv.h"

/* Local funcions */
static uint32_t SDL_WDT_getBaseAddr(SDL_WDT_TimerId timerId);
static uint32_t SDL_WDT_getBaseAddrforIntSrc(SDL_ESM_WDT_IntSrc intSrc);

/** ============================================================================
 *
 * Requirement: REQ_TAG(PRSDK-2577) REQ_TAG(PRSDK-4211)
 * Design: did_safeti_diagnostics_library did_wdt_diagnostics
 *
 * \brief  Feeds configured Watchdog Timer
 *
 * \param  timerId:  Timer id of the watchdog timer to feed.
 *
 * \return  none
 */
void SDL_WDT_feedWatchdog(SDL_WDT_TimerId timerId)
{
    uint32_t baseAddr;

    baseAddr = SDL_WDT_getBaseAddr(timerId);

    (void)RTIDwwdService(baseAddr);
    return;
}

/** ============================================================================
 *
 * Requirement: REQ_TAG(PRSDK-2577) REQ_TAG(PRSDK-4211)
 * Design: did_safeti_diagnostics_library did_wdt_diagnostics
 *
 * \brief  Clear all events reported in status register
 *
 * \param  intSrc:  Interrupt source to identify which timer
 *
 * \return  none
 */
void SDL_WDT_clearAllInterruptEvents(SDL_ESM_WDT_IntSrc intSrc)
{
    uint32_t intrStatus;
    uint32_t baseAddr;

    baseAddr = SDL_WDT_getBaseAddrforIntSrc(intSrc);

    (void)RTIDwwdGetStatus(baseAddr, &intrStatus);
    /* Clear status if non-zero bits */
    if (intrStatus != 0u) {
        (void)RTIDwwdClearStatus(baseAddr, intrStatus);
    }
    return;
}

/** ============================================================================
 *
 * Requirement: REQ_TAG(PRSDK-2577) REQ_TAG(PRSDK-4211)
 * Design: did_safeti_diagnostics_library did_wdt_diagnostics
 *
 * \brief  Check if the event reported in the status register matches
 *         expected test type
 *
 * \param  intSrc:  Interrupt source to identify whitch Timer
 * \param  testType:  Interrupt source to identify whitch Timer
 *
 * \return  none
 */
bool SDL_WDT_checkWatchDogEvent(SDL_ESM_WDT_IntSrc intSrc,
                                SDL_WDT_TestType testType)
{
    uint32_t intrStatus;
    uint32_t statusMask;
    bool retValue;
    uint32_t baseAddr;

    retValue = (bool)false;

    baseAddr = SDL_WDT_getBaseAddrforIntSrc(intSrc);
    (void)RTIDwwdGetStatus(baseAddr, &intrStatus);

    /* Determine status mask based on test type */
    if (testType == SDL_WDT_TEST_TYPE_WINDOW_VIOLATION) {
        statusMask = RTI_DWWD_STATUS_TIME_WINDOW_VIOLATION;
    } else {
        statusMask = RTI_DWWD_STATUS_ENDTIME_WINDOW_VIOLATION;
    }

    /* Check event and report status */
    if ((intrStatus & statusMask) != 0u) {
        retValue = (bool)true;
        (void)RTIDwwdClearStatus(baseAddr, statusMask);
    }

    return retValue;
}

/** ============================================================================
 *
 * \brief  Get Base address of Watchdog timer based on timer Id
 *
 * \param  timerId:  Timer id of the watchdog timer.
 *
 * \return  none
 */
static uint32_t SDL_WDT_getBaseAddr(SDL_WDT_TimerId timerId)
{
    uint32_t baseAddr;

    switch(timerId) {
        case SDL_WDT_TIMER_1:
            baseAddr = CSL_MCU_RTI1_CFG_BASE;
            break;

        case SDL_WDT_TIMER_0:
        default:
            baseAddr = CSL_MCU_RTI0_CFG_BASE;
            break;
    }
    return baseAddr;
}

/** ============================================================================
 *
 * \brief  Get Base address of Watchdog timer based on timer Id
 *
 * \param  timerId:  Timer id of the watchdog timer.
 *
 * \return  none
 */
static uint32_t SDL_WDT_getBaseAddrforIntSrc(SDL_ESM_WDT_IntSrc intSrc)
{
    uint32_t baseAddr;

    switch(intSrc) {
        case SDL_ESM_TIMER_ID_1:
            baseAddr = CSL_MCU_RTI1_CFG_BASE;
            break;

        case SDL_ESM_TIMER_ID_0:
        default:
            baseAddr = CSL_MCU_RTI0_CFG_BASE;
            break;
    }
    return baseAddr;
}
