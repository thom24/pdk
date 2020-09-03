/*
 * SDR ECC
 *
 * Software Diagnostics Reference module for ECC
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
 * @file  sdr_ecc.h
 *
 * @brief
 *  Header file contains enumerations, structure definitions and function
 *  declarations for SDR ECC interface.
 *  ============================================================================
 */

#ifndef INCLUDE_SDR_ECC_H_
#define INCLUDE_SDR_ECC_H_

#include <stdint.h>
#include <stdbool.h>
#include <sdr_esm.h>

#include "sdr_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/** ===========================================================================
 *
 * @defgroup SDR_ECC_API SDR ECC API
 *
 * ============================================================================
 */

/**
@defgroup SDR_ECC_DATASTRUCT  SDR ECC Data Structures
@ingroup SDR_ECC_API
*/
/**
@defgroup SDR_ECC_FUNCTION  SDR ECC Functions
@ingroup SDR_ECC_API
*/
/**
@defgroup SDR_ECC_ENUM SDR ECC Enumerated Data Types
@ingroup SDR_ECC_API
*/
/**
@defgroup SDR_ECC_MACROS SDR ECC Macro defines
@ingroup SDR_ECC_API
*/

/** ===========================================================================
 *  \addtogroup SDR_ECC_ENUM
 *  @{
 * ============================================================================
 */

/** ---------------------------------------------------------------------------
 * \brief This enumerator defines the different ECC aggregator types
 * ----------------------------------------------------------------------------
 */
typedef enum {
    SDR_ECC_AGGR_TYPE_INJECT_ONLY = 1,
    /**<  Ecc aggregator inject only */
    SDR_ECC_AGGR_TYPE_FULL_FUNCTION = 2,
    /**<  Ecc aggregator full funtionality */
} SDR_ECC_AggregatorType;


/** ---------------------------------------------------------------------------
 * \brief      ECC Inject error types
 *
 * ----------------------------------------------------------------------------
 */
typedef enum {
    /** No error */
    SDR_INJECT_ECC_NO_ERROR = 0,
    /** 1-Bit ECC Error forcing once */
    SDR_INJECT_ECC_ERROR_FORCING_1BIT_ONCE = 1,
    /** 2-Bit ECC Error forcing once */
    SDR_INJECT_ECC_ERROR_FORCING_2BIT_ONCE = 2,
    /** 1-Bit ECC Error Force once on next any Ram read */
    SDR_INJECT_ECC_ERROR_FORCING_1BIT_N_ROW_ONCE = 3,
    /** 2-Bit ECC Error Force once on  next Ram read */
    SDR_INJECT_ECC_ERROR_FORCING_2BIT_N_ROW_ONCE = 4,
    /** 1-Bit ECC Error forcing once */
    SDR_INJECT_ECC_ERROR_FORCING_1BIT_REPEAT = 5,
    /** 2-Bit ECC Error forcing once */
    SDR_INJECT_ECC_ERROR_FORCING_2BIT_REPEAT = 6,
    /** 1-Bit ECC Error Force once on next any Ram read */
    SDR_INJECT_ECC_ERROR_FORCING_1BIT_N_ROW_REPEAT = 7,
    /** 2-Bit ECC Error Force once on  next Ram read */
    SDR_INJECT_ECC_ERROR_FORCING_2BIT_N_ROW_REPEAT = 8,
} SDR_ECC_InjectErrorType;


/** ---------------------------------------------------------------------------
 * \brief This enumerator defines the different ECC RAM ID types
 * ----------------------------------------------------------------------------
 */
typedef enum {
    SDR_ECC_RAM_ID_TYPE_WRAPPER = 0,
    /**<  Ecc RAM ID Wrapper type */
    SDR_ECC_RAM_ID_TYPE_INTERCONNECT = 1,
    /**<  Ecc RAM ID Interconnect/CBASS type */
} SDR_ECC_RamIdType;

/* @} */

/**
 *  \addtogroup SDR_ECC_MACROS
 *  @{
 */

/** ---------------------------------------------------------------------------
 * \brief This enumerator indicate ECC memory type
 *
 * ----------------------------------------------------------------------------
 */
typedef uint32_t SDR_ECC_MemType;


/** \brief Select RAM MCU R5F core 0 memory type */
#define SDR_ECC_MEMTYPE_MCU_R5F0_CORE (0u)
/** \brief Select RAM MCU R5F core 1 memory type */
#define SDR_ECC_MEMTYPE_MCU_R5F1_CORE (1u)
/** \brief Select MCU ADC0 type */
#define SDR_ECC_MEMTYPE_MCU_ADC0 (2u)
/** \brief Select MCU ADC1 type */
#define SDR_ECC_MEMTYPE_MCU_ADC1 (3u)
/** \brief Select MCU CPSW0 type */
#define SDR_ECC_MEMTYPE_MCU_CPSW0 (4u)
/** \brief Select MCU Flash Subsystem Hyperbus 0 type */
#define SDR_ECC_MEMTYPE_MCU_FSS0_HPB0 (5u)
/** \brief Select MCU Flash Subsystem Octal Spi0 type */
#define SDR_ECC_MEMTYPE_MCU_FSS0_OSPI0 (6u)
/** \brief Select MCU Flash Subsystem Octal Spi0 type */
#define SDR_ECC_MEMTYPE_MCU_FSS0_OSPI1 (7u)
/** \brief Select MCU MCAN0 type */
#define SDR_ECC_MEMTYPE_MCU_MCAN0 (8u)
/** \brief Select MCU MCAN1 type */
#define SDR_ECC_MEMTYPE_MCU_MCAN1 (9u)
/** \brief Select MCU MSRAM0 type */
#define SDR_ECC_MEMTYPE_MCU_MSRAM0 (10u)
/** \brief Select MCU NAVSS 0 type */
#define SDR_ECC_MEMTYPE_MCU_NAVSS0 (11u)
/** \brief Select MCU PDMA0 type */
#define SDR_ECC_MEMTYPE_MCU_PDMA0 (12u)
/** \brief Select MCU PDMA0 type */
#define SDR_ECC_MEMTYPE_MCU_PDMA1 (13u)
/** \brief Select MCU PSRAM0 type */
#define SDR_ECC_MEMTYPE_MCU_PSRAM0 (14u)
/** \brief Select MCU CBASS type */
#define SDR_ECC_MEMTYPE_MCU_CBASS_ECC_AGGR0 (15u)


/* These aggregators require using RAT to map into memory space */

/** \brief Select Main MSMC AGGR0 type */
#define SDR_ECC_MEMTYPE_MAIN_MSMC_AGGR0 (16u)
/** \brief Select Main MSMC AGGR1 type */
#define SDR_ECC_MEMTYPE_MAIN_MSMC_AGGR1 (17u)
/** \brief Select Main MSMC AGGR2 type */
#define SDR_ECC_MEMTYPE_MAIN_MSMC_AGGR2 (18u)

// TODO - REMOVE
/** \brief Select Main A72 AGGR0 type */
#define SDR_ECC_MEMTYPE_MAIN_A72_AGGR0 (19u)


/** ---------------------------------------------------------------------------
 * \brief This enumerator indicate ECC memory Sub Type
 *
 * ----------------------------------------------------------------------------
 */
typedef uint32_t SDR_ECC_MemSubType;

/* The following are the memory sub type for Memory type
   SDR_ECC_MEMTYPE_MCU_R5F0_CORE & SDR_ECC_MEMTYPE_MCU_R5F1_CORE */

/** \brief Select memory subtype ITAG RAM0 */
#define SDR_ECC_R5F_MEM_SUBTYPE_ITAG_RAM0_VECTOR_ID (0U)
/** \brief Select memory subtype ITAG RAM1 */
#define SDR_ECC_R5F_MEM_SUBTYPE_ITAG_RAM1_VECTOR_ID (1U)
/** \brief Select memory subtype ITAG RAM2 */
#define SDR_ECC_R5F_MEM_SUBTYPE_ITAG_RAM2_VECTOR_ID (2U)
/** \brief Select memory subtype ITAG RAM3 */
#define SDR_ECC_R5F_MEM_SUBTYPE_ITAG_RAM3_VECTOR_ID (3U)
/** \brief Select memory subtype IDATA BANK0 */
#define SDR_ECC_R5F_MEM_SUBTYPE_IDATA_BANK0_VECTOR_ID (4U)
/** \brief Select memory subtype IDATA BANK1 */
#define SDR_ECC_R5F_MEM_SUBTYPE_IDATA_BANK1_VECTOR_ID (5U)
/** \brief Select memory subtype IDATA BANK2 */
#define SDR_ECC_R5F_MEM_SUBTYPE_IDATA_BANK2_VECTOR_ID (6U)
/** \brief Select memory subtype IDATA BANK3 */
#define SDR_ECC_R5F_MEM_SUBTYPE_IDATA_BANK3_VECTOR_ID (7U)
/** \brief Select memory subtype DTAG RAM0 */
#define SDR_ECC_R5F_MEM_SUBTYPE_DTAG_RAM0_VECTOR_ID (8U)
/** \brief Select memory subtype DTAG RAM1 */
#define SDR_ECC_R5F_MEM_SUBTYPE_DTAG_RAM1_VECTOR_ID (9U)
/** \brief Select memory subtype DTAG RAM2 */
#define SDR_ECC_R5F_MEM_SUBTYPE_DTAG_RAM2_VECTOR_ID (10U)
/** \brief Select memory subtype DTAG RAM3 */
#define SDR_ECC_R5F_MEM_SUBTYPE_DTAG_RAM3_VECTOR_ID (11U)
/** \brief Select memory subtype DDIRTY RAM */
#define SDR_ECC_R5F_MEM_SUBTYPE_DDIRTY_RAM_VECTOR_ID (12U)
/** \brief Select memory subtype DDATA RAM0 */
#define SDR_ECC_R5F_MEM_SUBTYPE_DDATA_RAM0_VECTOR_ID (13U)
/** \brief Select memory subtype DDATA RAM1 */
#define SDR_ECC_R5F_MEM_SUBTYPE_DDATA_RAM1_VECTOR_ID (14U)
/** \brief Select memory subtype DDATA RAM2 */
#define SDR_ECC_R5F_MEM_SUBTYPE_DDATA_RAM2_VECTOR_ID (15U)
/** \brief Select memory subtype DDATA RAM3 */
#define SDR_ECC_R5F_MEM_SUBTYPE_DDATA_RAM3_VECTOR_ID (16U)
/** \brief Select memory subtype DDATA RAM4 */
#define SDR_ECC_R5F_MEM_SUBTYPE_DDATA_RAM4_VECTOR_ID (17U)
/** \brief Select memory subtype DDATA RAM5 */
#define SDR_ECC_R5F_MEM_SUBTYPE_DDATA_RAM5_VECTOR_ID (18U)
/** \brief Select memory subtype DDATA RAM6 */
#define SDR_ECC_R5F_MEM_SUBTYPE_DDATA_RAM6_VECTOR_ID (19U)
/** \brief Select memory subtype DDATA RAM7 */
#define SDR_ECC_R5F_MEM_SUBTYPE_DDATA_RAM7_VECTOR_ID (20U)
/** \brief Select memory subtype ATCM0 BANK0 */
#define SDR_ECC_R5F_MEM_SUBTYPE_ATCM0_BANK0_VECTOR_ID (21U)
/** \brief Select memory subtype ATCM0 BANK1 */
#define SDR_ECC_R5F_MEM_SUBTYPE_ATCM0_BANK1_VECTOR_ID (22U)
/** \brief Select memory subtype B0TCM0 BANK0 */
#define SDR_ECC_R5F_MEM_SUBTYPE_B0TCM0_BANK0_VECTOR_ID (23U)
/** \brief Select memory subtype B0TCM0 BANK1 */
#define SDR_ECC_R5F_MEM_SUBTYPE_B0TCM0_BANK1_VECTOR_ID (24U)
/** \brief Select memory subtype B1TCM0 BANK0 */
#define SDR_ECC_R5F_MEM_SUBTYPE_B1TCM0_BANK0_VECTOR_ID (25U)
/** \brief Select memory subtype B1TCM0 BANK1 */
#define SDR_ECC_R5F_MEM_SUBTYPE_B1TCM0_BANK1_VECTOR_ID (26U)
/** \brief Select memory subtype VIM RAM */
#define SDR_ECC_R5F_MEM_SUBTYPE_KS_VIM_RAM_VECTOR_ID (27U)
/** \brief Select memory subtype VBUSM2AXI EDC */
#define SDR_ECC_R5F_MEM_SUBTYPE_VBUSM2AXI_EDC_VECTOR_ID (28U)
/** \brief Select memory subtype AXI2VBUSM MEM MST WRITE EDC */
#define SDR_ECC_R5F_MEM_SUBTYPE_AXI2VBUSM_MEM_MST_WRITE_EDC_VECTOR_ID (29U)
/** \brief Select memory subtype AXI2VBUSM MEM MST READ EDC */
#define SDR_ECC_R5F_MEM_SUBTYPE_AXI2VBUSM_MEM_MST_READ_EDC_VECTOR_ID (30U)
/** \brief Select memory subtype AXI2VBUSM PERIPH MST WRITE EDC */
#define SDR_ECC_R5F_MEM_SUBTYPE_AXI2VBUSM_PERIPH_MST_WRITE_EDC_VECTOR_ID (31U)
/** \brief Select memory subtype AXI2VBUSM PERIPH MST READ EDC */
#define SDR_ECC_R5F_MEM_SUBTYPE_AXI2VBUSM_PERIPH_MST_READ_EDC_VECTOR_ID (32U)
/** \brief Select memory subtype AHB2VBUSP EDC */
#define SDR_ECC_R5F_MEM_SUBTYPE_AHB2VBUSP_EDC_VECTOR_ID (33U)
/** \brief Select memory subtype SCRP EDC */
#define SDR_ECC_R5F_MEM_SUBTYPE_SCRP_EDC_VECTOR_ID (34U)
/** \brief Select memory subtype ECC AGGR EDC */
#define SDR_ECC_R5F_MEM_SUBTYPE_ECC_AGGR_EDC_ID (35U)

/** \brief Select memory subtype MSMC MMR BusECC */
#define SDR_ECC_MAIN_MSMC_MEM_INTERCONN_SUBTYPE  (20) /* CSL_ECC_AGGR_COMPUTE_CLUSTER0_MSMC_ES_ECC_AGGR0_MSMC_MMR_BUSECC_ID */
/** \brief Select memory subtype MSMC CLEC SRAM ECC */
#define SDR_ECC_MAIN_MSMC_MEM_WRAPPER_SUBTYPE  (100) /* CSL_COMPUTE_CLUSTER0_MSMC_ECC_AGGR0_CLEC_SRAM_RAMECC_ID */

/* The following are the memory sub type for Memory type
   SDR_ECC_MEMTYPE_MCU_CBASS */

/** \brief Select memory subtype write ramecc */
#define SDR_ECC_MCU_CBASS_MEM_SUBTYPE_WR_RAMECC_ID (0U)
/** \brief Select memory subtype read ramecc */
#define SDR_ECC_MCU_CBASS_MEM_SUBTYPE_RD_RAMECC_ID (1U)
/** \brief Select memory subtype edc control */
#define SDR_ECC_MCU_CBASS_MEM_SUBTYPE_EDC_CTRL_ID (2U)



/** ---------------------------------------------------------------------------
 * \brief This enumerator defines the different types of ECC memory Checker
 * Group types for Interconnect RAM ID's
 * ----------------------------------------------------------------------------
 */
typedef enum {
    SDR_ECC_GROUP_CHECKER_TYPE_REDUNDANT = 0,
    /**<  Ecc Group Checker Redundant type */
    SDR_ECC_GROUP_CHECKER_TYPE_PARITY = 1,
    /**<  Ecc Group Checker Parity type */
    SDR_ECC_GROUP_CHECKER_TYPE_EDC = 2,
    /**<  Ecc Group Checker EDC type */
} SDR_ECC_GrpChkType;


/** ---------------------------------------------------------------------------
 * \brief This enumerator indicates ECC memory SubSubType (Checker Groups)
 * for SDR_ECC_R5F_MEM_SUBTYPE_VBUSM2AXI_EDC_VECTOR_ID Memory Subtype
 * (Interconnect RAM ID Type)
 *
 * ----------------------------------------------------------------------------
 */
typedef uint32_t SDR_ECC_MemSubSubType;

/** \brief Select memory subsubtype VBUSM2AXI_EDC_VECTOR_SRC_CREQ */
#define SDR_ECC_R5F_MEM_SUBSUBTYPE_VBUSM2AXI_EDC_VECTOR_SRC_CREQ (0U)
/** \brief Select memory subsubtype VBUSM2AXI_EDC_VECTOR_INT_UNCOR_CROUTEID */
#define SDR_ECC_R5F_MEM_SUBSUBTYPE_VBUSM2AXI_EDC_VECTOR_INT_UNCOR_CROUTEID (1U)
/** \brief Select memory subsubtype VBUSM2AXI_EDC_VECTOR_INT_UNCOR_CORDERID */
#define SDR_ECC_R5F_MEM_SUBSUBTYPE_VBUSM2AXI_EDC_VECTOR_INT_UNCOR_CORDERID (2U)
/** \brief Select memory subsubtype VBUSM2AXI_EDC_VECTOR_INT_UNCOR_CID */
#define SDR_ECC_R5F_MEM_SUBSUBTYPE_VBUSM2AXI_EDC_VECTOR_INT_UNCOR_CID (3U)
/** \brief Select memory subsubtype VBUSM2AXI_EDC_VECTOR_INT_UNCOR_CADDRESS */
#define SDR_ECC_R5F_MEM_SUBSUBTYPE_VBUSM2AXI_EDC_VECTOR_INT_UNCOR_CADDRESS (4U)
/** \brief Select memory subsubtype VBUSM2AXI_EDC_VECTOR_INT_UNCOR_CDIR */
#define SDR_ECC_R5F_MEM_SUBSUBTYPE_VBUSM2AXI_EDC_VECTOR_INT_UNCOR_CDIR (5U)
/** \brief Select memory subsubtype VBUSM2AXI_EDC_VECTOR_INT_UNCOR_CBYTECNT */
#define SDR_ECC_R5F_MEM_SUBSUBTYPE_VBUSM2AXI_EDC_VECTOR_INT_UNCOR_CBYTECNT (6U)
/** \brief Select memory subsubtype VBUSM2AXI_EDC_VECTOR_INT_UNCOR_CAMODE */
#define SDR_ECC_R5F_MEM_SUBSUBTYPE_VBUSM2AXI_EDC_VECTOR_INT_UNCOR_CAMODE (7U)
/** \brief Select memory subsubtype VBUSM2AXI_EDC_VECTOR_INT_UNCOR_CCLSIZE */
#define SDR_ECC_R5F_MEM_SUBSUBTYPE_VBUSM2AXI_EDC_VECTOR_INT_UNCOR_CCLSIZE (8U)
/** \brief Select memory subsubtype VBUSM2AXI_EDC_VECTOR_INT_UNCOR_CSECURE */
#define SDR_ECC_R5F_MEM_SUBSUBTYPE_VBUSM2AXI_EDC_VECTOR_INT_UNCOR_CSECURE (9U)
/** \brief Select memory subsubtype VBUSM2AXI_EDC_VECTOR_INT_UNCOR_CPRIV */
#define SDR_ECC_R5F_MEM_SUBSUBTYPE_VBUSM2AXI_EDC_VECTOR_INT_UNCOR_CPRIV (10U)
/** \brief Select memory subsubtype VBUSM2AXI_EDC_VECTOR_INT_UNCOR_CDTYPE */
#define SDR_ECC_R5F_MEM_SUBSUBTYPE_VBUSM2AXI_EDC_VECTOR_INT_UNCOR_CDTYPE (11U)
/** \brief Select memory subsubtype VBUSM2AXI_EDC_VECTOR_INT_UNCOR_CEMUDBG */
#define SDR_ECC_R5F_MEM_SUBSUBTYPE_VBUSM2AXI_EDC_VECTOR_INT_UNCOR_CEMUDBG (12U)
/** \brief Select memory subsubtype VBUSM2AXI_EDC_VECTOR_INT_UNCOR_CINTEREST */
#define SDR_ECC_R5F_MEM_SUBSUBTYPE_VBUSM2AXI_EDC_VECTOR_INT_UNCOR_CINTEREST (13U)
/** \brief Select memory subsubtype VBUSM2AXI_EDC_VECTOR_INT_UNCOR_CFLUSH */
#define SDR_ECC_R5F_MEM_SUBSUBTYPE_VBUSM2AXI_EDC_VECTOR_INT_UNCOR_CFLUSH (14U)
/** \brief Select memory subsubtype VBUSM2AXI_EDC_VECTOR_INT_UNCOR_CPRIORITY */
#define SDR_ECC_R5F_MEM_SUBSUBTYPE_VBUSM2AXI_EDC_VECTOR_INT_UNCOR_CPRIORITY (15U)
/** \brief Select memory subsubtype VBUSM2AXI_EDC_VECTOR_INT_UNCOR_CRSEL */
#define SDR_ECC_R5F_MEM_SUBSUBTYPE_VBUSM2AXI_EDC_VECTOR_INT_UNCOR_CRSEL (16U)
/** \brief Select memory subsubtype VBUSM2AXI_EDC_VECTOR_INT_UNCOR_CCINNER */
#define SDR_ECC_R5F_MEM_SUBSUBTYPE_VBUSM2AXI_EDC_VECTOR_INT_UNCOR_CCINNER (17U)
/** \brief Select memory subsubtype VBUSM2AXI_EDC_VECTOR_INT_UNCOR_CMEMTYPE */
#define SDR_ECC_R5F_MEM_SUBSUBTYPE_VBUSM2AXI_EDC_VECTOR_INT_UNCOR_CMEMTYPE (18U)
/** \brief Select memory subsubtype VBUSM2AXI_EDC_VECTOR_INT_UNCOR_CSDOMAIN */
#define SDR_ECC_R5F_MEM_SUBSUBTYPE_VBUSM2AXI_EDC_VECTOR_INT_UNCOR_CSDOMAIN (19U)
/** \brief Select memory subsubtype VBUSM2AXI_EDC_VECTOR_SRC_WREQ */
#define SDR_ECC_R5F_MEM_SUBSUBTYPE_VBUSM2AXI_EDC_VECTOR_SRC_WREQ (20U)
/** \brief Select memory subsubtype VBUSM2AXI_EDC_VECTOR_INT_UNCOR_WDATA_31_0 */
#define SDR_ECC_R5F_MEM_SUBSUBTYPE_VBUSM2AXI_EDC_VECTOR_INT_UNCOR_WDATA_31_0 (21U)
/** \brief Select memory subsubtype VBUSM2AXI_EDC_VECTOR_INT_UNCOR_WDATA_63_32 */
#define SDR_ECC_R5F_MEM_SUBSUBTYPE_VBUSM2AXI_EDC_VECTOR_INT_UNCOR_WDATA_63_32 (22U)
/** \brief Select memory subsubtype VBUSM2AXI_EDC_VECTOR_INT_UNCOR_WBYTEN */
#define SDR_ECC_R5F_MEM_SUBSUBTYPE_VBUSM2AXI_EDC_VECTOR_INT_UNCOR_WBYTEN (23U)
/** \brief Select memory subsubtype VBUSM2AXI_EDC_VECTOR_SRC_SREADY */
#define SDR_ECC_R5F_MEM_SUBSUBTYPE_VBUSM2AXI_EDC_VECTOR_SRC_SREADY (24U)
/** \brief Select memory subsubtype VBUSM2AXI_EDC_VECTOR_SRC_RMREADY */
#define SDR_ECC_R5F_MEM_SUBSUBTYPE_VBUSM2AXI_EDC_VECTOR_SRC_RMREADY (25U)
/** \brief Select memory subsubtype VBUSM2AXI_EDC_VECTOR_CFIFO_STATE */
#define SDR_ECC_R5F_MEM_SUBSUBTYPE_VBUSM2AXI_EDC_VECTOR_CFIFO_STATE (26U)
/** \brief Select memory subsubtype VBUSM2AXI_EDC_VECTOR_WCFIFO_WR_PTR */
#define SDR_ECC_R5F_MEM_SUBSUBTYPE_VBUSM2AXI_EDC_VECTOR_WCFIFO_WR_PTR (27U)
/** \brief Select memory subsubtype VBUSM2AXI_EDC_VECTOR_WCFIFO_RD_PTR */
#define SDR_ECC_R5F_MEM_SUBSUBTYPE_VBUSM2AXI_EDC_VECTOR_WCFIFO_RD_PTR (28U)
/** \brief Select memory subsubtype VBUSM2AXI_EDC_VECTOR_WCFIFO_OCC */
#define SDR_ECC_R5F_MEM_SUBSUBTYPE_VBUSM2AXI_EDC_VECTOR_WCFIFO_OCC (29U)
/** \brief Select memory subsubtype VBUSM2AXI_EDC_VECTOR_WCFIFO_AOCC */
#define SDR_ECC_R5F_MEM_SUBSUBTYPE_VBUSM2AXI_EDC_VECTOR_WCFIFO_AOCC (30U)
/** \brief Select memory subsubtype VBUSM2AXI_EDC_VECTOR_WCFIFO_RD_RDY */
#define SDR_ECC_R5F_MEM_SUBSUBTYPE_VBUSM2AXI_EDC_VECTOR_WCFIFO_RD_RDY (31U)
/** \brief Select memory subsubtype VBUSM2AXI_EDC_VECTOR_WCFIFO_WR_RDY */
#define SDR_ECC_R5F_MEM_SUBSUBTYPE_VBUSM2AXI_EDC_VECTOR_WCFIFO_WR_RDY (32U)
/** \brief Select memory subsubtype VBUSM2AXI_EDC_VECTOR_WCFIFO_OCC_IS_ONE */
#define SDR_ECC_R5F_MEM_SUBSUBTYPE_VBUSM2AXI_EDC_VECTOR_WCFIFO_OCC_IS_ONE (33U)
/** \brief Select memory subsubtype VBUSM2AXI_EDC_VECTOR_WCFIFO_OCC_IS_ZERO */
#define SDR_ECC_R5F_MEM_SUBSUBTYPE_VBUSM2AXI_EDC_VECTOR_WCFIFO_OCC_IS_ZERO (34U)
/** \brief Select memory subsubtype VBUSM2AXI_EDC_VECTOR_WDFIFO_STATE */
#define SDR_ECC_R5F_MEM_SUBSUBTYPE_VBUSM2AXI_EDC_VECTOR_WDFIFO_STATE (35U)
/** \brief Select memory subsubtype VBUSM2AXI_EDC_VECTOR_WSFIFO_STATE */
#define SDR_ECC_R5F_MEM_SUBSUBTYPE_VBUSM2AXI_EDC_VECTOR_WSFIFO_STATE (36U)
/** \brief Select memory subsubtype VBUSM2AXI_EDC_VECTOR_SFIFO_OCC */
#define SDR_ECC_R5F_MEM_SUBSUBTYPE_VBUSM2AXI_EDC_VECTOR_SFIFO_OCC (37U)
/** \brief Select memory subsubtype VBUSM2AXI_EDC_VECTOR_SFIFO_AOCC */
#define SDR_ECC_R5F_MEM_SUBSUBTYPE_VBUSM2AXI_EDC_VECTOR_SFIFO_AOCC (38U)
/** \brief Select memory subsubtype VBUSM2AXI_EDC_VECTOR_SFIFO_RD_RDY */
#define SDR_ECC_R5F_MEM_SUBSUBTYPE_VBUSM2AXI_EDC_VECTOR_SFIFO_RD_RDY (39U)
/** \brief Select memory subsubtype VBUSM2AXI_EDC_VECTOR_SFIFO_WR_RDY */
#define SDR_ECC_R5F_MEM_SUBSUBTYPE_VBUSM2AXI_EDC_VECTOR_SFIFO_WR_RDY (40U)
/** \brief Select memory subsubtype VBUSM2AXI_EDC_VECTOR_SFIFO_OCC_IS_ONE */
#define SDR_ECC_R5F_MEM_SUBSUBTYPE_VBUSM2AXI_EDC_VECTOR_SFIFO_OCC_IS_ONE (41U)
/** \brief Select memory subsubtype VBUSM2AXI_EDC_VECTOR_SFIFO_OCC_IS_ZERO */
#define SDR_ECC_R5F_MEM_SUBSUBTYPE_VBUSM2AXI_EDC_VECTOR_SFIFO_OCC_IS_ZERO (42U)
/** \brief Select memory subsubtype VBUSM2AXI_EDC_VECTOR_RDFIFO_OCC */
#define SDR_ECC_R5F_MEM_SUBSUBTYPE_VBUSM2AXI_EDC_VECTOR_RDFIFO_OCC (43U)
/** \brief Select memory subsubtype VBUSM2AXI_EDC_VECTOR_RDFIFO_AOCC */
#define SDR_ECC_R5F_MEM_SUBSUBTYPE_VBUSM2AXI_EDC_VECTOR_RDFIFO_AOCC (44U)
/** \brief Select memory subsubtype VBUSM2AXI_EDC_VECTOR_RDFIFO_RD_RDY */
#define SDR_ECC_R5F_MEM_SUBSUBTYPE_VBUSM2AXI_EDC_VECTOR_RDFIFO_RD_RDY (45U)
/** \brief Select memory subsubtype VBUSM2AXI_EDC_VECTOR_RDFIFO_WR_RDY */
#define SDR_ECC_R5F_MEM_SUBSUBTYPE_VBUSM2AXI_EDC_VECTOR_RDFIFO_WR_RDY (46U)
/** \brief Select memory subsubtype VBUSM2AXI_EDC_VECTOR_RDFIFO_OCC_IS_ONE */
#define SDR_ECC_R5F_MEM_SUBSUBTYPE_VBUSM2AXI_EDC_VECTOR_RDFIFO_OCC_IS_ONE (47U)
/** \brief Select memory subsubtype VBUSM2AXI_EDC_VECTOR_RDFIFO_OCC_IS_ZERO */
#define SDR_ECC_R5F_MEM_SUBSUBTYPE_VBUSM2AXI_EDC_VECTOR_RDFIFO_OCC_IS_ZERO (48U)

/** /brief Format of ECC error Call back function */
typedef void (*SDR_ECC_ErrorCallback_t) (uint32_t errorSrc, uint32_t address);

/** /brief Format of VIM DED vector function */
typedef void (*SDR_ECC_VIMDEDVector_t) (void);

/* @} */

/**
 *  \addtogroup SDR_ECC_DATASTRUCT
 *  @{
 */

/** ---------------------------------------------------------------------------
 * \brief This structure defines the elements of ECC  Init configuration
 * ----------------------------------------------------------------------------
 */
typedef struct SDR_ECC_InitConfig_s
{
    uint32_t numRams;
    /**< Max number of memory sections ECC is enabled on R5F,
         the memory sections include ATCM, VIM, BTCM RAMs */
    SDR_ECC_MemSubType *pMemSubTypeList;
    /**< Pointer to list of Vector ID types of ECC enabled memory sections */
} SDR_ECC_InitConfig_t;

/** ---------------------------------------------------------------------------
 * \brief This structure defines the inject error configuration
 * ----------------------------------------------------------------------------
 */
typedef struct SDR_ECC_InjectErrorConfig_s
{
    uint32_t *pErrMem;
    /**< Address to inject error */
    uint32_t flipBitMask;
    /**< Bit location to flip bits */
    SDR_ECC_MemSubSubType chkGrp;
    /**< Group checker (for Interconnect RAM ID's only) */
}  SDR_ECC_InjectErrorConfig_t;

/* @} */

/**
 *  \addtogroup SDR_ECC_FUNCTION
 *  @{
 */

/** ============================================================================*
 *
 * \brief   Initializes an ESM module for usage with ECC module
 *
 * \param   esmInstType: Instance of ESM
 *
 * \return  SDR_PASS : Success; SDR_FAIL for failures
 */
SDR_Result SDR_ECC_initEsm (const SDR_ESM_InstanceType esmInstType);

/** ============================================================================*
 *
 * \brief   Initializes ECC module for ECC detection
 *
 * \param  eccMemType ECC memory type
 * \param  pECCInitConfig     Pointer to Ecc init configuration
 *
 * \return  SDR_PASS : Success; SDR_FAIL for failuresn
 */
SDR_Result SDR_ECC_init (SDR_ECC_MemType eccMemType,
                        const SDR_ECC_InitConfig_t *pECCInitConfig);

/** ============================================================================
 *
 * \brief   Initializes Memory to be ready for ECC error detection.
 *          Assumes ECC is already enabled.
 *
 * \param  eccMemType ECC memory type
 * \param  memSubType: Memory subtype
 *
 * \return  SDR_PASS : Success; SDR_FAIL for failures
 */
SDR_Result SDR_ECC_initMemory (SDR_ECC_MemType eccMemType,
                               SDR_ECC_MemSubType memSubType);

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
 * \return  SDR_PASS : Success; SDR_FAIL for failures
 */
SDR_Result SDR_ECC_selfTest(SDR_ECC_MemType eccMemType,
                            SDR_ECC_MemSubType memSubType,
                            SDR_ECC_InjectErrorType errorType,
                            const SDR_ECC_InjectErrorConfig_t *pECCErrorConfig,
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
 * \return  SDR_PASS : Success; SDR_FAIL for failures
 */
SDR_Result SDR_ECC_injectError(SDR_ECC_MemType eccMemType,
                               SDR_ECC_MemSubType memSubType,
                               SDR_ECC_InjectErrorType errorType,
                               const SDR_ECC_InjectErrorConfig_t *pECCErrorConfig);

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
void SDR_ECC_registerVIMDEDHandler(SDR_ECC_VIMDEDVector_t VIMDEDHandler);

/** ============================================================================
 *
 * \brief   Application provided external callback function for ECC handling
 *          Called inside the reference functions when ECC errors occur.
 *          NOTE: This is application supplied and not part of the SDR
 *          If not supplied by application this will result in an linker error
 *
 * \param  errorSrc: Error source for the ECC error event.
 * \param  address: Address at which the ECC error occurred.
 * \param  ramId: RAM ID at which the ECC error occurred.
 * \param  bitLocation: bitLocation at which the ECC error occurred.
 * \param  bitErrorGroup: group checker that reported the error
 *         (Interconnect ECC type only).
 *
 *
 * \return  None
 */
void SDR_ECC_applicationCallbackFunction(uint32_t errorSrc,
                                         uint32_t address,
                                         uint32_t ramId,
                                         uint64_t bitErrorOffset,
                                         uint32_t bitErrorGroup);

/* @} */

#ifdef __cplusplus
}
#endif  /* extern "C" */

#endif /* INCLUDE_SDR_ECC_H_ */
