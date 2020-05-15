/*
 * SDL MPU Configuration
 *
 * SafeTI Diagnostics Library module for MPU configuration module
 *
 * Copyright (C) 2019 Texas Instruments Incorporated - http://www.ti.com/
 * ALL RIGHTS RESERVED
 */

/**
 * @file  sdl_mpuCfg.h
 *
 * @brief
 *  Header file for SDL MPU configuration interface.
 *  ============================================================================
 */

#ifndef INCLUDE_SDL_MPUCFG_H_
#define INCLUDE_SDL_MPUCFG_H_

#include <sdl_common.h>
#include <sdl_mpu.h>
#include <ti/csl/arch/r5/csl_arm_r5_mpu.h>

void SDL_MPUCfgAddRegion(const SDL_MPU_memConfig_t *pMemConfig);
void SDL_MPUResolve(uint32_t baseAddr, uint32_t regId);
void SDL_MPUCfgRemoveRegion(const SDL_MPU_memConfig_t *pMemConfig);

#endif /* INCLUDE_SDL_MPUCFG_H_ */
