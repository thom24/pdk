/*
 * SDL ECC
 *
 * SafeTI Diagnostics Library module for ECC
 *
 * Copyright (C) 2018-2019 Texas Instruments Incorporated - http://www.ti.com/
 * ALL RIGHTS RESERVED
 */

#ifndef INCLUDE_SDL_ECC_PRIV_H_
#define INCLUDE_SDL_ECC_PRIV_H_

#include <stdbool.h>

#define SDL_ECC_EVENT_FOUND (1u)

typedef struct SDL_RAMIdEntry_s
{
    uint32_t RAMId;
    /**< Unique Ram Identifier */
    bool aggregatorTypeInjectOnly;
    /**< Variable to track if the ECC aggregator configuration
     * is only inject only or a full fledged ecc aggregator
     * including tracking of error status */
} SDL_RAMIdEntry_t;

/** ---------------------------------------------------------------------------
 * \brief This structure defines the elements of memory configuration
 * ----------------------------------------------------------------------------
 */
typedef struct SDL_MemConfig_s
{
    uintptr_t memStartAddr;
    /**< Memory start address */
    uint32_t size;
    /**< Size of memory in bytes */
    uint32_t stride;
    /**< Stride of memory in bytes */
    bool readable;
    /**< Memory section whether it is directly readable */
}  SDL_MemConfig_t;

#endif /* INCLUDE_SDL_ECC_PRIV_H_ */
