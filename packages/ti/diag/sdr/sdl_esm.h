/*
 * SDL ESM
 *
 * SafeTI Diagnostics Library module for Error Signaling Module
 *
 * Copyright (C) 2018-2019 Texas Instruments Incorporated - http://www.ti.com/
 * ALL RIGHTS RESERVED
 */

/**
 * @file  sdl_esm.h
 *
 * @brief
 *  Header file contains enumerations, structure definitions and function
 *  declarations for SDL Error Signaling Module interface.
 *
 *  The SDL ESM enumerations include:
 *      1. SDL ESM interrupt types
 *      2. SDL ESM interrupt sources
 *      3. SDL ESM Watchdog Timer IDs
 *
 *  The SDL ESM function macros include:
 *      1. Application provided callback function type for ECC/CCM/WDT specific ESM events
 *
 *  The SDL ESM data structures include:
 *      1. Structure of the ECC error sources which map to the ESM interrupt sources
 *      2. Structure of the ESM error configuration
 *      3. Structure of the initial ESM configuration
 *
 *  The SDL ESM APIs include:
 *      1. API to initialize the SDL ESM
 *      2. APIs to set/reset nERROR pin
 *      3. API to get nERROR pin status
 *      4. API to insert an ESM error
 *      5. API to execute self test of ESM module
 *      6. APIs to register handler for ECC/CCM with ESM
 *      7. APIs to register/de-register handler for WDT with ESM
 *      8. APIs to handle ESM high priority/low prioirty/config interrupts
 *      9. API to get ESM Interrupt Number corresponding to the input ESM interrupt type
 *      10.Application provided external callback function for ESM handling
 */

#ifndef INCLUDE_SDL_ESM_H_
#define INCLUDE_SDL_ESM_H_

#include <stdint.h>
#include <stdbool.h>

#include "sdl_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/** ===========================================================================
 *
 * @defgroup SDL_ESM_API SDL ESM API
 *
 * @section Overview
 * The SDL ESM module provides API to configure ESM events
 *
 * ============================================================================
 */
/**
@defgroup SDL_ESM_DATASTRUCT  SDL ESM Data Structures
@ingroup SDL_ESM_API
*/
/**
@defgroup SDL_ESM_FUNCTION  SDL ESM Functions
@ingroup SDL_ESM_API
*/
/**
@defgroup SDL_ESM_ENUM SDL ESM Enumerated Data Types
@ingroup SDL_ESM_API
*/

/**
@defgroup SDL_ESM_MACROS SDL ESM Macro defines
@ingroup SDL_ESM_API
*/

/** ===========================================================================
 *  @addtogroup SDL_ESM_ENUM
    @{
 * ============================================================================
 */
/** ---------------------------------------------------------------------------
 * \brief This enumerator defines the values for ESM interrupt types
 * ----------------------------------------------------------------------------
 */
typedef enum {
   SDL_ESM_INT_TYPE_HI = 1,
    /**< Interrupt type Hi  */
   SDL_ESM_INT_TYPE_LO = 2,
    /**< Interrupt type Lo  */
   SDL_ESM_INT_TYPE_CFG = 3,
    /**< Interrupt type Config  */
} SDL_ESM_IntType;

/** ---------------------------------------------------------------------------
 * \brief Defines the different SDL ESM interrupt sources
 * ----------------------------------------------------------------------------
 */
typedef enum {
    SDL_ESM_CCM_OUTPUT_COMPARE_BLOCK_INT = 1,
    /**<  CCM Interrupt source  Output compare block*/
    SDL_ESM_CCM_VIM_ERR_INT = 2,
    /**<  CCM Interrupt source VIM compare */
    SDL_ESM_CCM_INACTIVITY_MONITOR_INT = 3,
   /**<  CCM Interrupt source inactivity monitor */
    SDL_ESM_CCM_SELF_TEST_ERR_INT = 4,
   /**<  CCM Interrupt source Self test error */
    SDL_ESM_CCM_STAT_ERR_INT = 5,
   /**<  CCM Interrupt source self test or split mode interrupt */
} SDL_ESM_CCM_IntSrc;


/** ---------------------------------------------------------------------------
 * \brief Defines the different Timer id events
 * ----------------------------------------------------------------------------
 */
typedef enum {
    SDL_ESM_TIMER_ID_0 = 1,
    /**<  Timer Id 0*/
    SDL_ESM_TIMER_ID_1 = 2,
    /**<  Timer Id 1 */
} SDL_ESM_WDT_IntSrc;

/* @} */

/**
 *  \addtogroup SDL_ESM_MACROS
 *  @{
 */


/** \brief Invalid interrupt number */
#define SDL_ESM_INTNUMBER_INVALID (0xffffffffu)

/** \brief Address field: Error Address invalid */
#define SDL_ESM_ERRORADDR_INVALID (0xffffffffu)

/** \brief Maximum number of EVENT words */
#define SDL_ESM_MAX_EVENT_MAP_NUM_WORDS (4u)

/**
 *  \anchor sdlEsmEccErrorSource_t
 *  \name ESM ECC Error source type
 *  @{
 */
/**
 * \brief  Source of the ECC error which maps to the ESM interrupt source
 */
typedef uint32_t sdlEsmEccErrorSource_t;

/** \brief MCU CPU0 detected 1-bit ECC error source */
#define SDL_ESM_ECC_PARAM_MCU_CPU0_SEC_ERROR (1u)
/** \brief MCU CPU0 detected 2-bit ECC error source */
#define SDL_ESM_ECC_PARAM_MCU_CPU0_DED_ERROR (2u)
/** \brief MCU CPU1 detected 1-bit ECC error source */
#define SDL_ESM_ECC_PARAM_MCU_CPU1_SEC_ERROR (3u)
/** \brief MCU CPU1 detected 2-bit ECC error source */
#define SDL_ESM_ECC_PARAM_MCU_CPU1_DED_ERROR (4u)
/* @} */

/* @} */

/**
 *  \addtogroup SDL_ESM_DATASTRUCT
 *  @{
 */

/** ============================================================================
 *
 * \brief   Call back function for ECC specific ESM events
 *
 * \param   intSource: Interrupt source
 * \param   errorAddr: Error Address where ECC error occurred 
 *
 * \return  None
 */
typedef void (* SDL_ESM_ECCCallback_t) (uint32_t intSource, uint32_t errorAddr);

/** ============================================================================
 *
 * \brief   Call back function for CCM specific ESM events
 *
 * \param   intSrc: Interrupt source
 *
 * \return  None
 */
typedef void (* SDL_ESM_CCMCallback_t) (SDL_ESM_CCM_IntSrc intSrc);

/** ============================================================================
 *
 * \brief   Call back function for WDT specific ESM events
 *
 * \param   intSrc:  Interrupt source for WDT events
 *
 * \return  SDL_RETURN_PASS : Success; SDL_RETURN_FAIL for failures
 */
typedef bool (* SDL_ESM_WDTCallback_t) (SDL_ESM_WDT_IntSrc intSrc);

/** ---------------------------------------------------------------------------
 * \brief ESM error configuration
 *
 * This structure defines the elements ESM error configuration
 * ----------------------------------------------------------------------------
 */
typedef struct SDL_ESM_Errorconfig_s
{
    uint32_t groupNumber;
    /**< Group number of error event  */
    uint32_t bitNumber;
    /**< Bit number within the group  */
} SDL_ESM_ErrorConfig_t;

/** ---------------------------------------------------------------------------
 * \brief ESM init configuration
 *
 * This structure defines ESM Init configuration
 * ----------------------------------------------------------------------------
 */
typedef struct SDL_ESM_InitConfig_s
{
    SDL_ESM_ErrorConfig_t esmErrorConfig;
    /**< Error event to be used for self test */
    uint32_t eventMap[SDL_ESM_MAX_EVENT_MAP_NUM_WORDS];
    /**< ESM Event bitmap */
    uint32_t eventPriorityMap[SDL_ESM_MAX_EVENT_MAP_NUM_WORDS];
    /**< ESM Event Priority bitmap */
    uint32_t errorOutputMap[SDL_ESM_MAX_EVENT_MAP_NUM_WORDS];
    /**< ESM bitmap for driving error pin: When selected error event occurs
     *  the error output pin will be asserted
     *  It is the application responsibility to reset the error
     *  if the system did not crash or lockup */
} SDL_ESM_InitConfig_t;

/* @} */

/**
 *  \addtogroup SDL_ESM_FUNCTION
 *  @{
 */

/** ============================================================================
 *
 * \brief   Initializes ESM module for SDL
 *
 * \param   esmInitConfig: Configuration for ESM
 *
 * \return  SDL_RETURN_PASS : Success; SDL_RETURN_FAIL for failures
 */
SDL_Result SDL_ESM_init (const SDL_ESM_InitConfig_t *esmInitConfig);

/** ============================================================================ 
 *
 * \brief   Function sets the nERROR pin active.
 *
 * \return    None
 */
void SDL_ESM_setNError(void);

/** ============================================================================
 *
 * \brief  Function sets the nERROR pin inactive.
 *
 * \return  SDL_RETURN_PASS : Success; SDL_RETURN_FAIL for failures
 */
SDL_Result SDL_ESM_resetNError(void);

/** ============================================================================
 *
 * \brief   Function returns the status of nERROR pin
 *
 * \return    true : active; false not active
 */
bool SDL_ESM_getNErrorStatus(void);

/** ============================================================================
 *
 * \brief   Insert ESM error
 *
 * \param   esmErrorConfig: Configuration of ESM error
 *
 * \return  SDL_RETURN_PASS : Success; SDL_RETURN_FAIL for failures
 */
SDL_Result SDL_ESM_errorInsert (const SDL_ESM_ErrorConfig_t *esmErrorConfig);

/** ============================================================================
 *
 * \brief  Execute self test of ESM module
 *
 * \param   loopCount: Number of iterations to check status before timing out
 *
 * \return  SDL_RETURN_PASS : Success; SDL_RETURN_FAIL for failures
 */
SDL_Result SDL_ESM_selfTest (uint32_t loopCount);

/** ============================================================================
 *
 * \brief   Register handler for ECC with ESM
 *
 * \param  eccCallBackFunctionPtr: Call back function to register
 *
 * \return  SDL_RETURN_PASS : Success; SDL_RETURN_FAIL for failures
 */
SDL_Result SDL_ESM_registerECCHandler(SDL_ESM_ECCCallback_t eccCallBackFunctionPtr);


/** ============================================================================
 *
 * \brief   Register callback function for CCM errrors
 *
 * \param   CCMCallBackFunctionPtr: Callback function pointer
 *
 * \return  SDL_PASS : Success; SDL_FAIL for failures
 */
SDL_Result SDL_ESM_registerCCMHandler(SDL_ESM_CCMCallback_t CCMCallBackFunctionPtr);


/** ============================================================================
 *
 * \brief   Register callback function for WDT events
 *
 * \param   WDTCallBackFunctionPtr: Callback function pointer
 *
 * \return  SDL_PASS : Success; SDL_FAIL for failures
 */
SDL_Result SDL_ESM_registerWDTHandler(SDL_ESM_WDTCallback_t WDTCallBackFunctionPtr);

/** ============================================================================
 *
 * \brief   De-Register callback function for WDT events
 *
 * \return  SDL_PASS : Success; SDL_FAIL for failures
 */
void SDL_ESM_deRegisterWDTHandler(void);

/** ============================================================================
 *
 * \brief   Esm Hi Interrupt Handler
 *
 * \param  arg: argument for handler
 *
 * \return  None
 */
void SDL_ESM_hiInterruptHandler (uintptr_t arg);

/** ============================================================================
 *
 * \brief   Esm Lo Interrupt Handler
 *
 * \param  arg: argument for handler
 *
 * \return  None
 */
void SDL_ESM_loInterruptHandler (uintptr_t arg);

/** ============================================================================
 *
 *
 * \brief   Esm Config Interrupt Handler
 *
 * \param  arg: argument for handler
 *
 * \return  None
 */
void SDL_ESM_configInterruptHandler (uintptr_t arg);

/** ============================================================================
 *
 * \brief   Esm get Interrupt Number corresponding to the
 *          input interrupt type
 *
 * \param   esmIntType: ESM Interrupt type
 *
 * \return  Interrupt Number or SDL_ESM_INTNUMBER_INVALID error
 */
uint32_t SDL_ESM_getIntNumber(SDL_ESM_IntType esmIntType);

/** ============================================================================
 *
 * \brief   Application provided external callback function for ESM handling
 *          Called inside the library functions when ESM error events occur.
 *          NOTE: This is application supplied and not part of the SDL Library
 *          If not supplied by application this will result in an linker error
 *
 * \param  arg: Argument provided by application as part of interrupt registration
 * \param  grpChannel: Group channel number of the event source
 * \param  index: index within the group of the event source
 * \param  instSrc: Interrupt source number which is the esm event number
 *
 *
 * \return  None
 */
void SDL_ESM_applicationCallbackFunction(uintptr_t arg, uint32_t grpChannel, uint32_t index,
                                         uint32_t intSrc);

/* @} */

#ifdef __cplusplus
}
#endif  /* extern "C" */

#endif /* INCLUDE_SDL_ESM_H_ */
