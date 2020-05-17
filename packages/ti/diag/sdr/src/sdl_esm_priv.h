/*
 * SDL ESM
 *
 * Software Diagnostics Library module for ESM
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
