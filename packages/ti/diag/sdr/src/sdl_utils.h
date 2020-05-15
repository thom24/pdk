/*
 * SDL UTILS
 *
 * SafeTI Diagnostics Library utilities
 *
 * Copyright (C) 2018-2019 Texas Instruments Incorporated - http://www.ti.com/
 * ALL RIGHTS RESERVED
 */

#ifndef INCLUDE_SDL_UTILS_H_
#define INCLUDE_SDL_UTILS_H_

uint32_t SDL_UTILS_getCFLR(void);

uint32_t SDL_UTILS_getPMOVSR(void);

uint32_t SDL_UTILS_getDFSR( void );
uint32_t SDL_UTILS_getDFAR( void );

uint32_t SDL_UTILS_getIFSR( void );
uint32_t SDL_UTILS_getIFAR( void );

#endif /* INCLUDE_SDL_UTILS_H_ */
