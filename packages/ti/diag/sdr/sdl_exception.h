/*
 * SDL EXCEPTION
 *
 * SafeTI Diagnostics Library module for Exception handling
 *
 * Copyright (C) 2018-2019 Texas Instruments Incorporated - http://www.ti.com/
 * ALL RIGHTS RESERVED
 */

/**
 * @file  sdl_exception.h
 *
 * @brief
 *  Header file contains enumerations, structure definitions and function
 *  declarations for SDL EXCEPTION interface.
 *
 *  The SDL exception data structures include:
 *      1. Structure of call back functions for various exception events
 *
 *  The SDL exception APIs include:
 *      1. API to execute SDL Exception module initialization
 *      2. APIs to handle various exceptions
 */

#ifndef INCLUDE_SDL_EXCEPTION_H_
#define INCLUDE_SDL_EXCEPTION_H_
#include "sdl_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/** ===========================================================================
 *
 * @defgroup SDL_EXCEPTION_API SDL EXCEPTION API
 *
 * ============================================================================
 */
/**
@defgroup SDL_EXCEPTION_DATASTRUCT  SDL EXCEPTION Data Structures
@ingroup SDL_EXCEPTION_API
*/
/**
@defgroup SDL_EXCEPTION_FUNCTION  SDL EXCEPTION Functions
@ingroup SDL_EXCEPTION_API
*/
/**
@defgroup SDL_EXCEPTION_MACROS SDL EXCEPTION Macro defines
@ingroup SDL_EXCEPTION_API
*/

/**
 *  \addtogroup SDL_EXCEPTION_MACROS
 *  @{
 */


/** \brief Format of Call back function for ECC error events */
typedef void (*SDL_EXCEPTION_ECCCallback_t) (uint32_t intSource, uint32_t errorAddr);

/** \brief Format of Call back function for exception */
typedef void (*SDL_EXCEPTION_Callback_t) (void);

/* @} */

/**
 *  \addtogroup SDL_EXCEPTION_DATASTRUCT
 *  @{
 */
/** \brief Structure of call back functions for various exception events */
typedef struct SDL_EXCEPTION_CallbackFunctions_s
{
    SDL_EXCEPTION_Callback_t udefExptnCallback;
    /**< Undefined Instruction exception callback function */
    SDL_EXCEPTION_Callback_t swiExptnCallback;
    /**< Software Interrupt exception callback function  */
    SDL_EXCEPTION_Callback_t pabtExptnCallback;
    /**< Prefetch Abort exception callback function */
    SDL_EXCEPTION_Callback_t dabtExptnCallback;
    /**< Data Abort exception callback function */
    SDL_EXCEPTION_Callback_t irqExptnCallback;
    /**< Interrupt Request exception callback function.
     *   This will be used only if VIM is not enabled.
     */
    SDL_EXCEPTION_Callback_t fiqExptnCallback;
    /**< Fast Interrupt Request callback function.
     *   This will be used only if VIM is not enabled.
     */
}SDL_EXCEPTION_CallbackFunctions_t;


/* @} */

/**
 *  \addtogroup SDL_EXCEPTION_FUNCTION
 *  @{
 */

/** ============================================================================
 *
 * \brief   Initialise Exception module
 *
 * \param  callbackFunctions: Pointer to callback Functions structure
 *
 * \return  None
 */
void SDL_EXCEPTION_init(const SDL_EXCEPTION_CallbackFunctions_t *callbackFunctions);

/** ============================================================================
 *
 * \brief   Function to register ECC exception handler
 *
 * \param  ECCCallBackFunctionPtr: callback function to register
 *
 * \return  None
 */
void SDL_EXCEPTION_registerECCHandler(SDL_EXCEPTION_ECCCallback_t ECCCallBackFunctionPtr);

/** ============================================================================
 *
 * \brief  Undefined Instruction Exception Handler
 *
 * \param  param: Parameter pointer
 *
 * \return  None
 */
void SDL_EXCEPTION_undefInstructionExptnHandler(void *param);

/** ============================================================================
 *
 * \brief  SW Interrupt Exception Handler
 *
 * \param  param: Parameter pointer
 *
 * \return  None
 */
void SDL_EXCEPTION_swIntrExptnHandler(void *param);

/** ============================================================================
 *
 * \brief  Prefetch Abort Exception Handler
 *
 * \param  param: Parameter pointer
 *
 * \return  None
 */
void SDL_EXCEPTION_prefetchAbortExptnHandler(void *param);

/** ============================================================================
 *
 * \brief  Data Abort Exception Handler
 *
 * \param  param: Parameter pointer
 *
 * \return  None
 */
void SDL_EXCEPTION_dataAbortExptnHandler(void *param);

/** ============================================================================
 *
 * \brief  IRQ Exception Handler
 *
 * \param  param: Parameter pointer
 *
 * \return  None
 */
void SDL_EXCEPTION_irqExptnHandler(void *param);

/** ============================================================================
 *
 * \brief  FIQ Exception Handler
 *
 * \param  param: Parameter pointer
 *
 * \return  None
 */
void SDL_EXCEPTION_fiqExptnHandler(void *param);

/* @} */

#ifdef __cplusplus
}
#endif  /* extern "C" */

#endif /* INCLUDE_SDL_EXCEPTION_H_ */
