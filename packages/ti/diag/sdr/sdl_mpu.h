/*
 * SDL MPU
 *
 * SafeTI Diagnostics Library module for MPU module
 *
 * Copyright (C) 2019 Texas Instruments Incorporated - http://www.ti.com/
 * ALL RIGHTS RESERVED
 */

/**
 * @file  sdl_mpu.h
 *
 * @brief
 *  Header file contains enumerations, structure definitions and function
 *  declarations for SDL MPU interface.
 *  ============================================================================
 */

#ifndef INCLUDE_SDL_MPU_H_
#define INCLUDE_SDL_MPU_H_

#include "sdl_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/** ===========================================================================
 *
 * @defgroup SDL_MPU_API SDL MPU API
 *
 * @section Overview
 * The SDL MPU module provides API for MPU Diagnostics
 *
 * ============================================================================
 */
/**
@defgroup SDL_MPU_DATASTRUCT  SDL MPU Data Structures
@ingroup SDL_MPU_API
*/
/**
@defgroup SDL_MPU_FUNCTION  SDL MPU Functions
@ingroup SDL_MPU_API
*/
/**
@defgroup SDL_MPU_ENUM SDL MPU Enumerated Data Types
@ingroup SDL_MPU_API
*/

/**
@defgroup SDL_MPU_MACROS SDL MPU Macro defines
@ingroup SDL_MPU_API
*/

/** ===========================================================================
 *  @addtogroup SDL_MPU_ENUM
    @{
 * ============================================================================
 */

/** ---------------------------------------------------------------------------
 * \brief This enumerator defines the type of Memory access to be used
 *        to trigger MPU exception
 * ----------------------------------------------------------------------------
 */
typedef enum {
    SDL_MPU_DATA_READ_ACCESS = 1,
    /**<  Use DATA Read access for MPU violation */
    SDL_MPU_DATA_WRITE_ACCESS = 2,
    /**<  Use DATA Write access for MPU violation */
} SDL_MPU_memAccessType;

/* @} */

/**
 *  \addtogroup SDL_MPU_MACROS
 *  @{
 */


/* @} */

/**
 *  \addtogroup SDL_MPU_DATASTRUCT
 *  @{
 */

/** ---------------------------------------------------------------------------
 * \brief MPU Data configuration
 *
 * This structure defines the elements of the MPU Data configuration
 * Please refer to ARM R5 Technical Reference Manual for details on MPU
 * The self test code assumes a free memory region ID to be provided which
 * configures the test memory location sub region to trigger the intended
 * MPU permission fault.
 * ----------------------------------------------------------------------------
 */
typedef struct SDL_MPU_memConfig_s
{
    uint32_t *pMemLocation;
    /**< Pointer to 16 byte aligned 32 byte memory block for self test 
         example location as 0x41C7EFE0 */
    SDL_MPU_memAccessType memAccessType;
    /**< Memory access type to be used to generate exception */
    uint32_t  mpuRegionId;
    /**< MPU Region ID to be used for MPU Self Test */
} SDL_MPU_memConfig_t;

/* @} */

/**
 *  \addtogroup SDL_MPU_FUNCTION
 *  @{
 */

/** ============================================================================
 *
 * \brief   Self Test API for MPU module
 *         NOTE: Need to call SDL_MPU_init function before calling this API
 *
 * \param   pMemConfig: Pointer to memory configuration to be used for MPU
 *                        exception

 * \param   loopCount: Based on this count the diagnostics will wait in a loop
 *                     value of 0 indicates WAIT_FOREVER
 *
 * \return  SDL_RETURN_PASS : Success; SDL_RETURN_FAIL for failures
 */
SDL_Result SDL_MPU_selfTest (const SDL_MPU_memConfig_t *pMemConfig,
                             uint32_t loopCount);

/* @} */

#ifdef __cplusplus
}
#endif  /* extern "C" */

#endif /* INCLUDE_SDL_MPU_H_ */
