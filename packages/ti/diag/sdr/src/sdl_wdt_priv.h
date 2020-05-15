/*
 * SDL WDT
 *
 * SafeTI Diagnostics Library module for Watchdog Timer
 *
 * Copyright (C) 2019 Texas Instruments Incorporated - http://www.ti.com/
 * ALL RIGHTS RESERVED
 */

#ifndef INCLUDE_SDL_WDT_PRIV_H_
#define INCLUDE_SDL_WDT_PRIV_H_

#include <sdl_wdt.h>
#include <sdl_esm.h>

void SDL_WDT_feedWatchdog(SDL_WDT_TimerId timerId);
void SDL_WDT_clearAllInterruptEvents(SDL_ESM_WDT_IntSrc intSrc);
bool SDL_WDT_checkWatchDogEvent(SDL_ESM_WDT_IntSrc intSrc, SDL_WDT_TestType testType);

#endif /* INCLUDE_SDL_WDT_PRIV_H_ */
