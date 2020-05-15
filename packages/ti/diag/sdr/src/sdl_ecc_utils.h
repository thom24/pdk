/*
 * SDL ECC
 *
 * SafeTI Diagnostics Library module for ECC
 *
 * Copyright (C) 2018-2019 Texas Instruments Incorporated - http://www.ti.com/
 * ALL RIGHTS RESERVED
 */

#ifndef INCLUDE_SDL_ECC_UTILS_H_
#define INCLUDE_SDL_ECC_UTILS_H_

void SDL_ECC_UTILS_enableECCATCM(void);
void SDL_ECC_UTILS_enableECCB0TCM(void);
void SDL_ECC_UTILS_enableECCB1TCM(void);

void SDL_ECC_UTILS_configSecIntr(uint32_t eventCountInitValue,
                                 uint32_t eventTypeBitMap,
                                 uint32_t counterNumber );

#endif /* INCLUDE_SDL_ECC_UTILS_H_ */
