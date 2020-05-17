/*
 * SDL ECC
 *
 * Software Diagnostics Library module for ECC
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

/**
 * @file  sdl_ecc.h
 *
 * @brief
 *  Header file contains enumerations, structure definitions and function
 *  declarations for SDL ECC interface.
 *  ============================================================================
 */

#ifndef INCLUDE_SDL_ECC_H_
#define INCLUDE_SDL_ECC_H_

#include <stdint.h>
#include <stdbool.h>

#include "sdl_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/** ===========================================================================
 *
 * @defgroup SDL_ECC_API SDL ECC API
 *
 * ============================================================================
 */

/**
@defgroup SDL_ECC_DATASTRUCT  SDL ECC Data Structures
@ingroup SDL_ECC_API
*/
/**
@defgroup SDL_ECC_FUNCTION  SDL ECC Functions
@ingroup SDL_ECC_API
*/
/**
@defgroup SDL_ECC_ENUM SDL ECC Enumerated Data Types
@ingroup SDL_ECC_API
*/
/**
@defgroup SDL_ECC_MACROS SDL ECC Macro defines
@ingroup SDL_ECC_API
*/

/** ===========================================================================
 *  \addtogroup SDL_ECC_ENUM
 *  @{
 * ============================================================================
 */

/** ---------------------------------------------------------------------------
 * \brief This enumerator defines the different ECC aggregator types
 * ----------------------------------------------------------------------------
 */
typedef enum {
    SDL_ECC_AGGR_TYPE_INJECT_ONLY = 1,
    /**<  Ecc aggregator inject only */
    SDL_ECC_AGGR_TYPE_FULL_FUNCTION = 2,
    /**<  Ecc aggregator full funtionality */
} SDL_ECC_AggregatorType;


/** ---------------------------------------------------------------------------
 * \brief      ECC Inject error types
 *
 * ----------------------------------------------------------------------------
 */
typedef enum {
    /** No error */
    SDL_INJECT_ECC_NO_ERROR = 0,
    /** 1-Bit ECC Error forcing once */
    SDL_INJECT_ECC_ERROR_FORCING_1BIT_ONCE = 1,
    /** 2-Bit ECC Error forcing once */
    SDL_INJECT_ECC_ERROR_FORCING_2BIT_ONCE = 2,
    /** 1-Bit ECC Error Force once on next any Ram read */
    SDL_INJECT_ECC_ERROR_FORCING_1BIT_N_ROW_ONCE = 3,
    /** 2-Bit ECC Error Force once on  next Ram read */
    SDL_INJECT_ECC_ERROR_FORCING_2BIT_N_ROW_ONCE = 4,
    /** 1-Bit ECC Error forcing once */
    SDL_INJECT_ECC_ERROR_FORCING_1BIT_REPEAT = 5,
    /** 2-Bit ECC Error forcing once */
    SDL_INJECT_ECC_ERROR_FORCING_2BIT_REPEAT = 6,
    /** 1-Bit ECC Error Force once on next any Ram read */
    SDL_INJECT_ECC_ERROR_FORCING_1BIT_N_ROW_REPEAT = 7,
    /** 2-Bit ECC Error Force once on  next Ram read */
    SDL_INJECT_ECC_ERROR_FORCING_2BIT_N_ROW_REPEAT = 8,
} SDL_ECC_InjectErrorType;

/* @} */

/**
 *  \addtogroup SDL_ECC_MACROS
 *  @{
 */

/** ---------------------------------------------------------------------------
 * \brief This enumerator indicate ECC memory type
 *
 * ----------------------------------------------------------------------------
 */
typedef uint32_t SDL_ECC_MemType;

/** \brief Select RAM MCU R5F core 0 memory type */
#define SDL_ECC_MEMTYPE_MCU_R5F0_CORE (0u)
/** \brief Select RAM MCU R5F core 1 memory type */
#define SDL_ECC_MEMTYPE_MCU_R5F1_CORE (1u)
/** \brief Select MCU ADC0 type */
#define SDL_ECC_MEMTYPE_MCU_ADC0 (2u)
/** \brief Select MCU ADC1 type */
#define SDL_ECC_MEMTYPE_MCU_ADC1 (3u)
/** \brief Select MCU CPSW0 type */
#define SDL_ECC_MEMTYPE_MCU_CPSW0 (4u)
/** \brief Select MCU Flash Subsystem Hyperbus 0 type */
#define SDL_ECC_MEMTYPE_MCU_FSS0_HPB0 (5u)
/** \brief Select MCU Flash Subsystem Octal Spi0 type */
#define SDL_ECC_MEMTYPE_MCU_FSS0_OSPI0 (6u)
/** \brief Select MCU Flash Subsystem Octal Spi0 type */
#define SDL_ECC_MEMTYPE_MCU_FSS0_OSPI1 (7u)
/** \brief Select MCU MCAN0 type */
#define SDL_ECC_MEMTYPE_MCU_MCAN0 (8u)
/** \brief Select MCU MCAN1 type */
#define SDL_ECC_MEMTYPE_MCU_MCAN1 (9u)
/** \brief Select MCU MSRAM0 type */
#define SDL_ECC_MEMTYPE_MCU_MSRAM0 (10u)
/** \brief Select MCU NAVSS 0 type */
#define SDL_ECC_MEMTYPE_MCU_NAVSS0 (11u)
/** \brief Select MCU PDMA0 type */
#define SDL_ECC_MEMTYPE_MCU_PDMA0 (12u)
/** \brief Select MCU PDMA0 type */
#define SDL_ECC_MEMTYPE_MCU_PDMA1 (13u)
/** \brief Select MCU PSRAM0 type */
#define SDL_ECC_MEMTYPE_MCU_PSRAM0 (14u)

/** ---------------------------------------------------------------------------
 * \brief This enumerator indicate ECC memory Sub Type
 *
 * ----------------------------------------------------------------------------
 */
typedef uint32_t SDL_ECC_MemSubType;

/** \brief Select memory subtype ITAG RAM0 */
#define SDL_ECC_R5F_MEM_SUBTYPE_ITAG_RAM0_VECTOR_ID (0U)
/** \brief Select memory subtype ITAG RAM1 */
#define SDL_ECC_R5F_MEM_SUBTYPE_ITAG_RAM1_VECTOR_ID (1U)
/** \brief Select memory subtype ITAG RAM2 */
#define SDL_ECC_R5F_MEM_SUBTYPE_ITAG_RAM2_VECTOR_ID (2U)
/** \brief Select memory subtype ITAG RAM3 */
#define SDL_ECC_R5F_MEM_SUBTYPE_ITAG_RAM3_VECTOR_ID (3U)
/** \brief Select memory subtype IDATA BANK0 */
#define SDL_ECC_R5F_MEM_SUBTYPE_IDATA_BANK0_VECTOR_ID (4U)
/** \brief Select memory subtype IDATA BANK1 */
#define SDL_ECC_R5F_MEM_SUBTYPE_IDATA_BANK1_VECTOR_ID (5U)
/** \brief Select memory subtype IDATA BANK2 */
#define SDL_ECC_R5F_MEM_SUBTYPE_IDATA_BANK2_VECTOR_ID (6U)
/** \brief Select memory subtype IDATA BANK3 */
#define SDL_ECC_R5F_MEM_SUBTYPE_IDATA_BANK3_VECTOR_ID (7U)
/** \brief Select memory subtype DTAG RAM0 */
#define SDL_ECC_R5F_MEM_SUBTYPE_DTAG_RAM0_VECTOR_ID (8U)
/** \brief Select memory subtype DTAG RAM1 */
#define SDL_ECC_R5F_MEM_SUBTYPE_DTAG_RAM1_VECTOR_ID (9U)
/** \brief Select memory subtype DTAG RAM2 */
#define SDL_ECC_R5F_MEM_SUBTYPE_DTAG_RAM2_VECTOR_ID (10U)
/** \brief Select memory subtype DTAG RAM3 */
#define SDL_ECC_R5F_MEM_SUBTYPE_DTAG_RAM3_VECTOR_ID (11U)
/** \brief Select memory subtype DDIRTY RAM */
#define SDL_ECC_R5F_MEM_SUBTYPE_DDIRTY_RAM_VECTOR_ID (12U)
/** \brief Select memory subtype DDATA RAM0 */
#define SDL_ECC_R5F_MEM_SUBTYPE_DDATA_RAM0_VECTOR_ID (13U)
/** \brief Select memory subtype DDATA RAM1 */
#define SDL_ECC_R5F_MEM_SUBTYPE_DDATA_RAM1_VECTOR_ID (14U)
/** \brief Select memory subtype DDATA RAM2 */
#define SDL_ECC_R5F_MEM_SUBTYPE_DDATA_RAM2_VECTOR_ID (15U)
/** \brief Select memory subtype DDATA RAM3 */
#define SDL_ECC_R5F_MEM_SUBTYPE_DDATA_RAM3_VECTOR_ID (16U)
/** \brief Select memory subtype DDATA RAM4 */
#define SDL_ECC_R5F_MEM_SUBTYPE_DDATA_RAM4_VECTOR_ID (17U)
/** \brief Select memory subtype DDATA RAM5 */
#define SDL_ECC_R5F_MEM_SUBTYPE_DDATA_RAM5_VECTOR_ID (18U)
/** \brief Select memory subtype DDATA RAM6 */
#define SDL_ECC_R5F_MEM_SUBTYPE_DDATA_RAM6_VECTOR_ID (19U)
/** \brief Select memory subtype DDATA RAM7 */
#define SDL_ECC_R5F_MEM_SUBTYPE_DDATA_RAM7_VECTOR_ID (20U)
/** \brief Select memory subtype ATCM0 BANK0 */
#define SDL_ECC_R5F_MEM_SUBTYPE_ATCM0_BANK0_VECTOR_ID (21U)
/** \brief Select memory subtype ATCM0 BANK1 */
#define SDL_ECC_R5F_MEM_SUBTYPE_ATCM0_BANK1_VECTOR_ID (22U)
/** \brief Select memory subtype B0TCM0 BANK0 */
#define SDL_ECC_R5F_MEM_SUBTYPE_B0TCM0_BANK0_VECTOR_ID (23U)
/** \brief Select memory subtype B0TCM0 BANK1 */
#define SDL_ECC_R5F_MEM_SUBTYPE_B0TCM0_BANK1_VECTOR_ID (24U)
/** \brief Select memory subtype B1TCM0 BANK0 */
#define SDL_ECC_R5F_MEM_SUBTYPE_B1TCM0_BANK0_VECTOR_ID (25U)
/** \brief Select memory subtype B1TCM0 BANK1 */
#define SDL_ECC_R5F_MEM_SUBTYPE_B1TCM0_BANK1_VECTOR_ID (26U)
/** \brief Select memory subtype VIM RAM */
#define SDL_ECC_R5F_MEM_SUBTYPE_KS_VIM_RAM_VECTOR_ID (27U)
/** \brief Select memory subtype VBUSM2AXI EDC */
#define SDL_ECC_R5F_MEM_SUBTYPE_VBUSM2AXI_EDC_VECTOR_ID (28U)
/** \brief Select memory subtype AXI2VBUSM MEM MST WRITE EDC */
#define SDL_ECC_R5F_MEM_SUBTYPE_AXI2VBUSM_MEM_MST_WRITE_EDC_VECTOR_ID (29U)
/** \brief Select memory subtype AXI2VBUSM MEM MST READ EDC */
#define SDL_ECC_R5F_MEM_SUBTYPE_AXI2VBUSM_MEM_MST_READ_EDC_VECTOR_ID (30U)
/** \brief Select memory subtype AXI2VBUSM PERIPH MST WRITE EDC */
#define SDL_ECC_R5F_MEM_SUBTYPE_AXI2VBUSM_PERIPH_MST_WRITE_EDC_VECTOR_ID (31U)
/** \brief Select memory subtype AXI2VBUSM PERIPH MST READ EDC */
#define SDL_ECC_R5F_MEM_SUBTYPE_AXI2VBUSM_PERIPH_MST_READ_EDC_VECTOR_ID (32U)
/** \brief Select memory subtype AHB2VBUSP EDC */
#define SDL_ECC_R5F_MEM_SUBTYPE_AHB2VBUSP_EDC_VECTOR_ID (33U)
/** \brief Select memory subtype SCRP EDC */
#define SDL_ECC_R5F_MEM_SUBTYPE_SCRP_EDC_VECTOR_ID (34U)
/** \brief Select memory subtype ECC AGGR EDC */
#define SDL_ECC_R5F_MEM_SUBTYPE_ECC_AGGR_EDC_ID (35U)

/** /brief Format of ECC error Call back function */
typedef void (*SDL_ECC_ErrorCallback_t) (uint32_t errorSrc, uint32_t address);

/** /brief Format of VIM DED vector function */
typedef void (*SDL_ECC_VIMDEDVector_t) (void);

/* @} */

/**
 *  \addtogroup SDL_ECC_DATASTRUCT
 *  @{
 */

/** ---------------------------------------------------------------------------
 * \brief This structure defines the elements of ECC  Init configuration
 * ----------------------------------------------------------------------------
 */
typedef struct SDL_ECC_InitConfig_s
{
    uint32_t numRams;
    /**< Max number of memory sections ECC is enabled on R5F,
         the memory sections include ATCM, VIM, BTCM RAMs */
    SDL_ECC_MemSubType *pMemSubTypeList;
    /**< Pointer to list of Vector ID types of ECC enabled memory sections */
} SDL_ECC_InitConfig_t;

/** ---------------------------------------------------------------------------
 * \brief This structure defines the inject error configuration
 * ----------------------------------------------------------------------------
 */
typedef struct SDL_ECC_InjectErrorConfig_s
{
    uint32_t *pErrMem;
    /**< Address to inject error */
    uint32_t flipBitMask;
    /**< Bit location to flip bits */
}  SDL_ECC_InjectErrorConfig_t;

/* @} */

/**
 *  \addtogroup SDL_ECC_FUNCTION
 *  @{
 */

/** ============================================================================*
 *
 * \brief   Initializes ECC module for ECC detection
 *
 * \param  eccMemType ECC memory type
 * \param  pECCInitConfig     Pointer to Ecc init configuration
 *
 * \return  SDL_PASS : Success; SDL_FAIL for failuresn
 */
SDL_Result SDL_ECC_init (SDL_ECC_MemType eccMemType,
                        const SDL_ECC_InitConfig_t *pECCInitConfig);

/** ============================================================================
 *
 * \brief   Initializes Memory to be ready for ECC error detection.
 *          Assumes ECC is already enabled.
 *
 * \param  eccMemType ECC memory type
 * \param  memSubType: Memory subtype
 *
 * \return  SDL_PASS : Success; SDL_FAIL for failures
 */
SDL_Result SDL_ECC_initMemory (SDL_ECC_MemType eccMemType,
                               SDL_ECC_MemSubType memSubType);

/** ============================================================================
 *
 * \brief   Runs self test by injecting and error and monitor response
 *          Assumes ECC is already enabled.
 *
 * \param  eccMemType ECC memory type
 * \param  memSubType: Memory subtype
 * \param  errorType: ECC Self test type
 * \param  pECCErrorConfig: Pointer to Error configuration
 * \param  selfTestTimeOut: Number of retries before time out
 *
 * \return  SDL_PASS : Success; SDL_FAIL for failures
 */
SDL_Result SDL_ECC_selfTest(SDL_ECC_MemType eccMemType,
                            SDL_ECC_MemSubType memSubType,
                            SDL_ECC_InjectErrorType errorType,
                            const SDL_ECC_InjectErrorConfig_t *pECCErrorConfig,
                            uint32_t selfTestTimeOut);

/** ============================================================================
 *
 * \brief   Injects ECC error at specified location
 *          Assumes ECC is already enabled.
 *
 * \param  eccMemType ECC memory type
 * \param  memSubType: Memory subtype
 * \param  errorType: ECC error type
 * \param  pECCErrorConfig: Pointer to Error configuration
 *
 * \return  SDL_PASS : Success; SDL_FAIL for failures
 */
SDL_Result SDL_ECC_injectError(SDL_ECC_MemType eccMemType,
                               SDL_ECC_MemSubType memSubType,
                               SDL_ECC_InjectErrorType errorType,
                               const SDL_ECC_InjectErrorConfig_t *pECCErrorConfig);

/** ============================================================================
 *
 * \brief   Register Handler for VIM DED ECC error
 *
 * \param  VIMDEDHandler: This is function to be called when an Double bit ECC
 *                        error happens on the VIM RAM. NOTE: That the regular
 *                        vector load will be replaced with this registered
 *                        function.
 *
 * \return  None
 */
void SDL_ECC_registerVIMDEDHandler(SDL_ECC_VIMDEDVector_t VIMDEDHandler);

/** ============================================================================
 *
 * \brief   Application provided external callback function for ECC handling
 *          Called inside the library functions when ECC errors occur.
 *          NOTE: This is application supplied and not part of the SDL Library
 *          If not supplied by application this will result in an linker error
 *
 * \param  errorSrc: Error source for the ECC error event.
 * \param  address: Address at which the ECC error occurred.
 *
 *
 * \return  None
 */
void SDL_ECC_applicationCallbackFunction(uint32_t errorSrc, uint32_t address);

/* @} */

#ifdef __cplusplus
}
#endif  /* extern "C" */

#endif /* INCLUDE_SDL_ECC_H_ */
