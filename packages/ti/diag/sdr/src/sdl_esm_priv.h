/*
 * SDL ESM
 *
 * SafeTI Diagnostics Library module for ESM
 *
 * Copyright (C) 2018-2019 Texas Instruments Incorporated - http://www.ti.com/
 * ALL RIGHTS RESERVED
 */

#ifndef INCLUDE_SDL_ESM_PRIV_H_
#define INCLUDE_SDL_ESM_PRIV_H_

#include <stddef.h>
#include <stdbool.h>

#include <sdl_esm.h>

/** ---------------------------------------------------------------------------
 * @brief This structure defines the elements of ESM software instance
 * ----------------------------------------------------------------------------
 */
typedef struct SDL_ESM_Instance_s
{
    SDL_ESM_InitConfig_t esmInitConfig;
    /**< Store esm Init configuration */
    SDL_ESM_ECCCallback_t eccCallBackFunction;
    /**< Store ECC callback function */
    SDL_ESM_CCMCallback_t CCMCallBackFunction;
    /**< Store CCM callback function */
    SDL_ESM_WDTCallback_t WDTCallBackFunction;
    /**< Store WDT callback function */
    volatile bool selfTestFlag;
    /**< selfTest Flag */
}  SDL_ESM_Instance_t;

/** ============================================================================
 *
 * \brief   Handle any event that needs to be handled locally before
 *          reporting to application
 *
 * \param  pInstance: Pointer to ESM instance
 * \param  intSrc: Source interrupt number
 *
 * \return  true: if event handled; false if not handled
 */
bool SDL_ESM_handleIntSrc(const SDL_ESM_Instance_t *pInstance, uint32_t intSrc);

#endif /* INCLUDE_SDL_ESM_PRIV_H_ */
