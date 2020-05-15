/*
 * SDL CCM
 *
 * SafeTI Diagnostics Library module for CPU Comparator Module
 *
 * Copyright (C) 2018-2019 Texas Instruments Incorporated - http://www.ti.com/
 * ALL RIGHTS RESERVED
 */

/**
 * @file  sdl_ccm.h
 *
 * @brief
 *  Header file contains enumerations, structure definitions and function
 *  declarations for SDL CPU Comparator Module interface.
 *
 *  The SDL CCM enumerations include:
 *      1. SDL CCM monitor type
 *      2. SDL CCM self test type (normal or force error)
 *
 *  The SDL CCM data structures include:
 *      1. SDL CCM error status structure
 *
 *  The SDL CCM APIs include:
 *      1. API to initialize the SDL CCM
 *      2. API to inject a forced error on SDL CCM
 *      3. API to execute self test on SDL CCM
 *      4. API to execute Self test on SDL CCM by inverting the polarity
 *      5. Application provided callback function for CCM error handling
 */

#ifndef INCLUDE_SDL_CCM_H_
#define INCLUDE_SDL_CCM_H_

#include <stdint.h>
#include <stdbool.h>

#include "sdl_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/** ===========================================================================
 *
 * @defgroup SDL_CCM_API SDL CCM API
 *
 * ============================================================================
 */
/**
@defgroup SDL_CCM_DATASTRUCT  SDL CCM Data Structures
@ingroup SDL_CCM_API
*/
/**
@defgroup SDL_CCM_FUNCTION  SDL CCM Functions
@ingroup SDL_CCM_API
*/
/**
@defgroup SDL_CCM_ENUM SDL CCM Enumerated Data Types
@ingroup SDL_CCM_API
*/

/** ===========================================================================
 *  @addtogroup SDL_CCM_ENUM
    @{
 * ============================================================================
 */

/** ---------------------------------------------------------------------------
 * \brief Defines the different CCM monitor types
 *
 * ---------------------------------------------------------------------------
 */
typedef enum {
   SDL_CCM_MONITOR_TYPE_OUTPUT_COMPARE_BLOCK = 1,
    /**<  CCM monitor type Output compare block*/
   SDL_CCM_MONITOR_TYPE_VIM = 2,
    /**<  CCM monitor type VIM */
  SDL_CCM_MONITOR_TYPE_INACTIVITY_MONITOR = 3,
   /**<  CCM monitor type inactivity monitor */
} SDL_CCM_MonitorType;

/** ---------------------------------------------------------------------------
 * \brief Types of self test for CCM module
 *
 * ---------------------------------------------------------------------------
 */
typedef enum {
   SDL_CCM_SELFTEST_TYPE_NORMAL = 1,
    /**<  Self Test type normal */
   SDL_CCM_SELFTEST_TYPE_ERR_FORCING = 2,
    /**<  Self Test type Error forcing */
} SDL_CCM_SelfTestType;

/* @} */

/**
 *  \addtogroup SDL_CCM_DATASTRUCT
 *  @{
 */

/** ---------------------------------------------------------------------------
 * \brief Structure for CCM error status
 *
 * ----------------------------------------------------------------------------
 */
typedef struct SDL_CCM_ErrorStatus_s
{
    bool compareErrorFlag;
    /**< CCM compare error flag: true indicates error */
    bool selfTestErrorFlag;
    /**< CCM self test error flag: true indicates error */
    bool selfTestErrorTypeFlag;
    /**< CCM self test error type flag: true indicates error */

} SDL_CCM_ErrorStatus_t;

/* @} */

/**
 *  \addtogroup SDL_CCM_FUNCTION
 *  @{
 */

/** ============================================================================
 *
 * \brief   Initializes CCM module
 *
 * \param  pInitConfig Init configuration structure pointer
 *
 * \return  SDL_PASS : Success; SDL_FAIL for failures
 *          NOTE: In case of SDL_FAIL the peripheral may be configured partially
 */
SDL_Result SDL_CCM_init (void);

/** ============================================================================
 *
 * \brief   Injects forced error on CCM
 *
 * \param   monitorType: Type of monitor to inject error
 *
 * \pre     SDL_CCM_init should be called before calling this function
 *
 * \return  SDL_PASS : Success; SDL_FAIL for failures
 */
SDL_Result SDL_CCM_injectError (SDL_CCM_MonitorType monitorType);

/** ============================================================================
 *
 * \brief   Execute Self test on CCM
 *          NOTE: SDL_CCM_init should be called before calling this function
 *
 * \param   monitorType: Monitor Type
 * \param   selfTestType: Self test type
 * \param   maxCheckCount: max count to check for self test
 *                         completion ( 0 means indefinite)
 * \param   pErrorStatus: Pointer to Error status structure to report
 *                        error status
 *
 *
 * \return  SDL_PASS : Success; SDL_FAIL for failures
 */
SDL_Result SDL_CCM_selfTest (SDL_CCM_MonitorType monitorType,
                             SDL_CCM_SelfTestType selfTestType,
                             uint32_t maxCheckCount,
                             SDL_CCM_ErrorStatus_t *pErrorStatus);

/** ============================================================================
 *
 * \brief   Execute Self test on CCM by inverting the polarity
 *
 * \param   polarityInversionMask: Mask used to invert polarity of
 *            selected signals. ( Meaning of signals refer to hardware
 *            manuals)
 * \param   maxCheckCount: Maximum count to recheck status of error trigger
 *
 * \return  SDL_PASS : Success; SDL_FAIL for failures
 */
SDL_Result SDL_CCM_selfTestPolarityInvert (uint32_t polarityInversionMask,
                                           uint32_t maxCheckCount);

/** ============================================================================
 *
 * \brief   Application provided external callback function for CCM error handling
 *          Called inside the library functions when CCM errors occur.
 *          NOTE: This is application supplied and not part of the SDL Library
 *          If not supplied by application this will result in an linker error
 *
 * \param  monitorType: Error source for the ECC error event.
 * \param  address: Address at which the ECC error occurred.
 *
 *
 * \return  None
 */
void SDL_CCM_applicationCallbackFunction(SDL_CCM_MonitorType monitorType,
                                         SDL_CCM_ErrorStatus_t *pErrorStatus);

/* @} */

#ifdef __cplusplus
}
#endif  /* extern "C" */

#endif /* INCLUDE_SDL_CCM_H_ */
