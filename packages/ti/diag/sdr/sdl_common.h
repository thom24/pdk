/*
 * SDL Common
 *
 * SafeTI Diagnostics Library common module which defines common data
 * structures
 *
 * Copyright (C) 2018-2019 Texas Instruments Incorporated - http://www.ti.com/
 * ALL RIGHTS RESERVED
 */

/**
 * @file  sdl_common.h
 *
 * @brief
 *  Header file contains enumerations, structure definitions and function
 *  declarations for SDL COMMON interface.
 *
 *  The SDL common enumerations include:
 *      1. SDL API function return result value
 *      2. SDL error code asserted
 *
 *  The SDL common APIs include:
 *      1. Application provided external assert function
 *      2. Application provided get timestamp function
 */

#ifndef INCLUDE_SDL_COMMON_H_
#define INCLUDE_SDL_COMMON_H_
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \mainpage  SafeTI(TM) Diagnostics Library
 *
 *
 * SafeTI(TM) Diagnostics Library(SDL) provides software interface for
 * Diagnostic functions to be used for application involving Functional Safety.
 *
 * <b>
 * Also refer to top level user guide for detailed features,
 * limitations and usage description.
 * </b>
 *
 *
 * The SDL includes the following modules
 * - <b> SDL COMMON API </b> (See \ref SDL_COMMON_API) <br>
 * - <b> SDL ECC API </b> (See \ref SDL_ECC_API) <br>
 * - <b> SDL ESM API </b> (See \ref SDL_ESM_API) <br>
 * - <b> SDL EXCEPTION API </b> (See \ref SDL_EXCEPTION_API) <br>
 * - <b> SDL CCM API </b> (See \ref SDL_CCM_API) <br>
 * - <b> SDL CRC API </b> (See \ref SDL_CRC_API) <br>
 * - <b> SDL MPU API </b> (See \ref SDL_MPU_API) <br>
 * - <b> SDL WDT API </b> (See \ref SDL_WDT_API) <br>
 */

/** ===========================================================================
 *
 * @defgroup SDL_COMMON_API SDL COMMON API
 *
 * ============================================================================
 */
/**
@defgroup SDL_COMMON_FUNCTION  SDL COMMON Functions
@ingroup SDL_COMMON_API
*/
/**
@defgroup SDL_COMMON_ENUM SDL COMMON Enumerated Data Types
@ingroup SDL_COMMON_API
*/

/**
@defgroup SDL_COMMON_MACROS SDL COMMON Macro defines
@ingroup SDL_COMMON_API
*/


/** ===========================================================================
 *  \addtogroup SDL_COMMON_ENUM
 *  @{
 * ============================================================================
 */

/** ---------------------------------------------------------------------------
 * \brief This enumerator indicate SDL result
 *
 * ----------------------------------------------------------------------------
 */
typedef enum {
    SDL_PASS = 0,
    /**< SDL Result pass */
    SDL_FAIL = -1,
    /**< SDL Result fail */
    SDL_BADARGS = -2,
    /**< SDL Result bad arguments error */
} SDL_Result;


/** ---------------------------------------------------------------------------
 * \enum SDL_assertErrorNumber
 * \brief Defines the different Assert error codes
 */

typedef enum {
    SDL_ESM_INT_SRC_OUT_OF_BOUNDS=1,
    /**< Assert when ESM Interrupt source out of bounds */
    SDL_ECC_INTERRUPT_WITH_NOEVENT=2,
    /**< Assert when ECC interrupt triggered with no event */
    SDL_ECC_RAM_ID_NOT_FOUND=3,
    /**< Assert when ECC Ram Id not found */
    SDL_CCM_READ_REG_FAILURE=4,
    /**< Assert CCM Read Register failure */
    SDL_CCM_INTERRUPT_WITHOUT_ANY_ERROR=5,
    /**< Assert CCM interupt received wihtout any error */
    SDL_CCM_CONFIG_REG_FAILURE=6,
    /**< Assert CCM Config Register failure */
    SDL_CCM_INVALID_INTERRUPT_SOURCE=7,
    /**< Assert CCM Invalid interrupt source */
} SDL_assertErrorNumber;
/* @} */

/**
 *  \addtogroup SDL_COMMON_MACROS
 *  @{
 */


/* @} */

/**
 *  \addtogroup SDL_COMMON_FUNCTION
 *  @{
 */

/** ============================================================================
 *
 * \brief   Application provided external assert function
 *          Called inside the library functions when unexpected conditions
 *          are encountered.
 *          NOTE: This is application supplied and not part of the SDL Library
 *          If not supplied by application this will result in an linker error
 *          NOTE: This function is not expected to return as the unexpected
 *          condition encountered will cause indeterminate behaviour if it
 *          returns. Application will need to add code here which will bring the system
 *          to a safe state as required by the application. But the function
 *          should not return.
 *
 * \param  errorNumber: Error number for the assert
 *
 * \return  None
 */
void SDL_assertExternalFunction(SDL_assertErrorNumber errorNumber);

/** ============================================================================
 *
 * \brief   Application provided timestamp function
 *          This function will be called inside the library function to get
 *          timestamp.
 *          NOTE: This is application supplied and not part of the SDL Library
 *          If not supplied by application this will result in an linker error
 *
 * \return  Timestamp value
 */
uint32_t SDL_getTime(void);

/* @} */

#ifdef __cplusplus
}
#endif  /* extern "C" */

#endif /* INCLUDE_SDL_COMMON_H_ */
