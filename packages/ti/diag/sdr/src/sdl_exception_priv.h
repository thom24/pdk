/*
 * SDL EXCEPTION
 *
 * SafeTI Diagnostics Library module for handling exceptions
 *
 * Copyright (C) 2018-2019 Texas Instruments Incorporated - http://www.ti.com/
 * ALL RIGHTS RESERVED
 */

#ifndef INCLUDE_SDL_ECC_EXCEPTION_PRIV_H_
#define INCLUDE_SDL_ECC_EXCEPTION_PRIV_H_

#include <stdint.h>

/** ---------------------------------------------------------------------------
 * @brief This structure defines the elements of Exception software instance
 * ----------------------------------------------------------------------------
 */

typedef struct SDL_EXCEPTION_Instance_s
{
    SDL_EXCEPTION_CallbackFunctions_t callbackFunctions;
    /**< Exception callback functions structure stored here */
    SDL_EXCEPTION_ECCCallback_t ECCCallBackFunction;
    /**< Exception ECC callback function stored here */
    void *paramPtr;
    /**< Parameter pointer used for callback stored here */
}  SDL_EXCEPTION_Instance_t;

#endif /* INCLUDE_SDL_ECC_EXCEPTION_PRIV_H_ */
