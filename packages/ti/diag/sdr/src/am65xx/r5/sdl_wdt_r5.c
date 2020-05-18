/*
 * SDL WDT
 *
 * Software Diagnostics Library module for Watchdog Timer
 *
 *  Copyright (c) Texas Instruments Incorporated 2018-2020
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

#include <stddef.h>
#include <stdbool.h>

#include <ti/csl/csl_rti.h>
#include <ti/csl/soc.h>

#include <sdl_common.h>
#include <sdl_wdt.h>

#include <sdl_wdt_priv.h>

/* Local funcions */
static uint32_t SDL_WDT_getBaseAddr(SDL_WDT_TimerId timerId);
static uint32_t SDL_WDT_getBaseAddrforIntSrc(SDL_ESM_WDT_IntSrc intSrc);

/** ============================================================================
 *
 * Requirement: REQ_TAG(PRSDK-2577) REQ_TAG(PRSDK-4211)
 * Design: did_software_diagnostics_reference did_wdt_diagnostics
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
 * Design: did_software_diagnostics_reference did_wdt_diagnostics
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
 * Design: did_software_diagnostics_reference did_wdt_diagnostics
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
