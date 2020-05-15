/*
 * SDL CRC
 *
 * SafeTI Diagnostics Library module for CRC  module
 *
 * Copyright (C) 2019 Texas Instruments Incorporated - http://www.ti.com/
 * ALL RIGHTS RESERVED
 */

/**
 * @file  sdl_wdt.h
 *
 * @brief
 *  Header file contains enumerations, structure definitions and function
 *  declarations for SDL CRC interface.
 *  ============================================================================
 */

#ifndef INCLUDE_SDL_CRC_H_
#define INCLUDE_SDL_CRC_H_

#include "sdl_common.h"
#include <ti/csl/csl_crc.h>

#ifdef __cplusplus
extern "C" {
#endif

/** ===========================================================================
 *
 * @defgroup SDL_CRC_API SDL CRC API
 *
 * @section Overview
 * The SDL CRC module provides API for WatchDog Timer Diagnostics
 *
 * ============================================================================
 */
/**
@defgroup SDL_CRC_DATASTRUCT  SDL CRC Data Structures
@ingroup SDL_CRC_API
*/
/**
@defgroup SDL_CRC_FUNCTION  SDL CRC Functions
@ingroup SDL_CRC_API
*/
/**
@defgroup SDL_CRC_ENUM SDL CRC Enumerated Data Types
@ingroup SDL_CRC_API
*/

/**
@defgroup SDL_CRC_MACROS SDL CRC Macro defines
@ingroup SDL_CRC_API
*/

/** ===========================================================================
 *  @addtogroup SDL_CRC_ENUM
    @{
 * ============================================================================
 */
/** ---------------------------------------------------------------------------
 * \brief This enumerator defines the Data size for input CRC DATA
 * ----------------------------------------------------------------------------
 */
typedef enum {
    SDL_CRC_DATA_8_BIT = 1,
    /**< 8 Bit data packed */
    SDL_CRC_DATA_16_BIT = 2,
    /**< 16 Bit data packed */
    SDL_CRC_DATA_32_BIT = 3,
    /**< 32 Bit data packed */
} SDL_CRC_dataBitSize;
/* @} */

/**
 *  \addtogroup SDL_CRC_MACROS
 *  @{
 */


/* @} */

/**
 *  \addtogroup SDL_CRC_DATASTRUCT
 *  @{
 */

/** ---------------------------------------------------------------------------
 * \brief CRC Data configuration
 *
 * This structure defines the elements of the CRC Data configuration
 * ----------------------------------------------------------------------------
 */
typedef struct SDL_CRC_dataConfig_s
{
    uint32_t *pCRCData;
    /**< Pointer to Data used for CRC  */
    uint32_t size;
    /**< Size of Data in Bytes  */
    SDL_CRC_dataBitSize dataBitSize;
    /**< Data Bit size  */
    crcOperationMode_t crcOpMode;
    /**< CRC operation mode, only full CPU mode is supported  */
    uint32_t refCRCValueLSW;
    /**< Reference CRC value Lower 32 bits */
    uint32_t refCRCValueMSW;
    /**< Reference CRC value Upper 32 bits */

} SDL_CRC_dataConfig_t;

/* @} */

/**
 *  \addtogroup SDL_CRC_FUNCTION
 *  @{
 */

/** ============================================================================
 *
 * \brief   Initializes CRC module for SDL
 *
 * \return  None
 */
void SDL_CRC_init (void);

/** ============================================================================
 *
 * \brief   Self Test API for CRC module
 *
 * \param   crcChannel: CRC channel number to be used
 * \param   pDataConfig: Pointer to data configuration
 *
 * \return  SDL_PASS: Success; SDL_FAIL: Failure; SDL_BADARGS: Bad arguments error
 */
SDL_Result SDL_CRC_selftest (uint32_t crcChannel, const SDL_CRC_dataConfig_t *pDataConfig);

/* @} */

#ifdef __cplusplus
}
#endif  /* extern "C" */

#endif /* INCLUDE_SDL_CRC_H_ */
