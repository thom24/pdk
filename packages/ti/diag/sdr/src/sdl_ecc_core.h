/*
 * SDL ECC
 *
 * SafeTI Diagnostics Library module for ECC
 *
 * Copyright (C) 2018-2019 Texas Instruments Incorporated - http://www.ti.com/
 * ALL RIGHTS RESERVED
 */

#ifndef INCLUDE_SDL_ECC_CORE_H_
#define INCLUDE_SDL_ECC_CORE_H_

#include <stdint.h>
#include <sdl_ecc.h>

SDL_Result SDL_ECC_configECCRam(uint32_t ramId);
uint32_t SDL_ECC_pollErrorEvent(SDL_ECC_MemType eccMemType,
                               SDL_ECC_MemSubType memSubType,
                               SDL_ECC_InjectErrorType errorType);

void SDL_ECC_enableECCEventCheck(SDL_ECC_MemType eccMemType,
                                SDL_ECC_MemSubType memSubType,
                                SDL_ECC_InjectErrorType errorType);
void SDL_ECC_disableECCEventCheck(SDL_ECC_MemType eccMemType,
                                 SDL_ECC_InjectErrorType errorType);
#endif /* INCLUDE_SDL_ECC_CORE_H_ */
