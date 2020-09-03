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

#ifndef INCLUDE_SDR_ECC_SOC_H_
#define INCLUDE_SDR_ECC_SOC_H_

#include <stdint.h>
#include <sdr_ecc.h>
#include <ti/csl/csl_ecc_aggr.h>

#include "sdr_ecc_priv.h"

/* define MAX entry based on list of Subtypes */
#define SDR_PULSAR_CPU_RAM_ID_TABLE_MAX_ENTRIES (CSL_ECCAGGR_PULSAR_SL_CPU0_KS_VIM_RAMECC_VECTOR_ID+1u)


/* ECC Row Widths for MCU Pulsar RAM ID's with Wrapper type
 * metadata width = (ecc_row_width) + 2 + log2(ecc_row_width) */
#define CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_ITAG_RAM0_ROW_SIZE                             (22U)
#define CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_ITAG_RAM1_ROW_SIZE                             (22U)
#define CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_ITAG_RAM2_ROW_SIZE                             (22U)
#define CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_ITAG_RAM3_ROW_SIZE                             (22U)
#define CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_IDATA_BANK0_ROW_SIZE                           (64U)
#define CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_IDATA_BANK1_ROW_SIZE                           (64U)
#define CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_IDATA_BANK2_ROW_SIZE                           (64U)
#define CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_IDATA_BANK3_ROW_SIZE                           (64U)
#define CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_DTAG_RAM0_ROW_SIZE                             (22U)
#define CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_DTAG_RAM1_ROW_SIZE                             (22U)
#define CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_DTAG_RAM2_ROW_SIZE                             (22U)
#define CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_DTAG_RAM3_ROW_SIZE                             (22U)
#define CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_DDIRTY_RAM_ROW_SIZE                            (22U)
#define CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_DDATA_RAM0_ROW_SIZE                            (32U)
#define CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_DDATA_RAM1_ROW_SIZE                            (32U)
#define CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_DDATA_RAM2_ROW_SIZE                            (32U)
#define CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_DDATA_RAM3_ROW_SIZE                            (32U)
#define CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_DDATA_RAM4_ROW_SIZE                            (32U)
#define CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_DDATA_RAM5_ROW_SIZE                            (32U)
#define CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_DDATA_RAM6_ROW_SIZE                            (32U)
#define CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_DDATA_RAM7_ROW_SIZE                            (32U)
#define CSL_MCU_R5FSS0_CORE0_ECC_AGGR_PULSAR_SL_ATCM0_BANK0_ROW_SIZE                      (32U)
#define CSL_MCU_R5FSS0_CORE0_ECC_AGGR_PULSAR_SL_ATCM0_BANK1_ROW_SIZE                      (32U)
#define CSL_MCU_R5FSS0_CORE0_ECC_AGGR_PULSAR_SL_B0TCM0_BANK0_ROW_SIZE                     (32U)
#define CSL_MCU_R5FSS0_CORE0_ECC_AGGR_PULSAR_SL_B0TCM0_BANK1_ROW_SIZE                     (32U)
#define CSL_MCU_R5FSS0_CORE0_ECC_AGGR_PULSAR_SL_B1TCM0_BANK0_ROW_SIZE                     (32U)
#define CSL_MCU_R5FSS0_CORE0_ECC_AGGR_PULSAR_SL_B1TCM0_BANK1_ROW_SIZE                     (32U)
#define CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_KS_VIM_RAMECC_ROW_SIZE                         (32U)



/** ---------------------------------------------------------------------------
 * @brief This structure holds the list of Ram Ids for each memory subtype
 * ----------------------------------------------------------------------------
 */
const SDR_RAMIdEntry_t SDR_ECC_mcuArmssRamIdTable[SDR_PULSAR_CPU_RAM_ID_TABLE_MAX_ENTRIES] =
{
 { CSL_ECCAGGR_PULSAR_SL_CPU0_ITAG_RAM0_VECTOR_ID, ((bool)true) },
 { CSL_ECCAGGR_PULSAR_SL_CPU0_ITAG_RAM1_VECTOR_ID, ((bool)true) },
 { CSL_ECCAGGR_PULSAR_SL_CPU0_ITAG_RAM2_VECTOR_ID, ((bool)true) },
 { CSL_ECCAGGR_PULSAR_SL_CPU0_ITAG_RAM3_VECTOR_ID, ((bool)true) },
 { CSL_ECCAGGR_PULSAR_SL_CPU0_IDATA_BANK0_VECTOR_ID, ((bool)true) },
 { CSL_ECCAGGR_PULSAR_SL_CPU0_IDATA_BANK1_VECTOR_ID, ((bool)true) },
 { CSL_ECCAGGR_PULSAR_SL_CPU0_IDATA_BANK2_VECTOR_ID, ((bool)true) },
 { CSL_ECCAGGR_PULSAR_SL_CPU0_IDATA_BANK3_VECTOR_ID, ((bool)true) },
 { CSL_ECCAGGR_PULSAR_SL_CPU0_DTAG_RAM0_VECTOR_ID, ((bool)true) },
 { CSL_ECCAGGR_PULSAR_SL_CPU0_DTAG_RAM1_VECTOR_ID, ((bool)true) },
 { CSL_ECCAGGR_PULSAR_SL_CPU0_DTAG_RAM2_VECTOR_ID, ((bool)true) },
 { CSL_ECCAGGR_PULSAR_SL_CPU0_DTAG_RAM3_VECTOR_ID, ((bool)true) },
 { CSL_ECCAGGR_PULSAR_SL_CPU0_DDIRTY_RAM_VECTOR_ID, ((bool)true) },
 { CSL_ECCAGGR_PULSAR_SL_CPU0_DDATA_RAM0_VECTOR_ID, ((bool)true) },
 { CSL_ECCAGGR_PULSAR_SL_CPU0_DDATA_RAM1_VECTOR_ID, ((bool)true) },
 { CSL_ECCAGGR_PULSAR_SL_CPU0_DDATA_RAM2_VECTOR_ID, ((bool)true) },
 { CSL_ECCAGGR_PULSAR_SL_CPU0_DDATA_RAM3_VECTOR_ID, ((bool)true) },
 { CSL_ECCAGGR_PULSAR_SL_CPU0_DDATA_RAM4_VECTOR_ID, ((bool)true) },
 { CSL_ECCAGGR_PULSAR_SL_CPU0_DDATA_RAM5_VECTOR_ID, ((bool)true) },
 { CSL_ECCAGGR_PULSAR_SL_CPU0_DDATA_RAM6_VECTOR_ID, ((bool)true) },
 { CSL_ECCAGGR_PULSAR_SL_CPU0_DDATA_RAM7_VECTOR_ID, ((bool)true) },
 { CSL_ECCAGGR_PULSAR_SL_CPU0_ATCM0_BANK0_VECTOR_ID, ((bool)true) },
 { CSL_ECCAGGR_PULSAR_SL_CPU0_ATCM0_BANK1_VECTOR_ID, ((bool)true) },
 { CSL_ECCAGGR_PULSAR_SL_CPU0_B0TCM0_BANK0_VECTOR_ID, ((bool)true) },
 { CSL_ECCAGGR_PULSAR_SL_CPU0_B0TCM0_BANK1_VECTOR_ID, ((bool)true) },
 { CSL_ECCAGGR_PULSAR_SL_CPU0_B1TCM0_BANK0_VECTOR_ID, ((bool)true) },
 { CSL_ECCAGGR_PULSAR_SL_CPU0_B1TCM0_BANK1_VECTOR_ID, ((bool)true) },
 { CSL_ECCAGGR_PULSAR_SL_CPU0_KS_VIM_RAMECC_VECTOR_ID, ((bool)false) },
};

/** ---------------------------------------------------------------------------
 * @brief This structure holds the memory config for each memory subtype
 * ----------------------------------------------------------------------------
 */
const SDR_MemConfig_t SDR_ECC_mcuArmssMemEntries[SDR_PULSAR_CPU_RAM_ID_TABLE_MAX_ENTRIES] =
{
    {SDR_ECC_R5F_MEM_SUBTYPE_ITAG_RAM0_VECTOR_ID, 0u,
                  CSL_ECCAGGR_PULSAR_SL_CPU0_ITAG_RAM0_SIZE, 4u,
                  CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_ITAG_RAM0_ROW_SIZE,  ((bool)false)        },
    {SDR_ECC_R5F_MEM_SUBTYPE_ITAG_RAM1_VECTOR_ID, 0u,
                  CSL_ECCAGGR_PULSAR_SL_CPU0_ITAG_RAM1_SIZE, 4u,
                  CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_ITAG_RAM1_ROW_SIZE, ((bool)false)         },
    {SDR_ECC_R5F_MEM_SUBTYPE_ITAG_RAM2_VECTOR_ID, 0u,
                  CSL_ECCAGGR_PULSAR_SL_CPU0_ITAG_RAM2_SIZE, 4u,
                  CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_ITAG_RAM2_ROW_SIZE, ((bool)false)         },
    {SDR_ECC_R5F_MEM_SUBTYPE_ITAG_RAM3_VECTOR_ID, 0u,
                  CSL_ECCAGGR_PULSAR_SL_CPU0_ITAG_RAM3_SIZE, 4u,
                  CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_ITAG_RAM3_ROW_SIZE, ((bool)false)         },
    {SDR_ECC_R5F_MEM_SUBTYPE_IDATA_BANK0_VECTOR_ID, 0u,
                  CSL_ECCAGGR_PULSAR_SL_CPU0_IDATA_BANK0_SIZE, 4u,
                  CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_IDATA_BANK0_ROW_SIZE, ((bool)false)       },
    {SDR_ECC_R5F_MEM_SUBTYPE_IDATA_BANK1_VECTOR_ID, 0u,
                  CSL_ECCAGGR_PULSAR_SL_CPU0_IDATA_BANK1_SIZE, 4u,
                  CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_IDATA_BANK1_ROW_SIZE, ((bool)false)       },
    {SDR_ECC_R5F_MEM_SUBTYPE_IDATA_BANK2_VECTOR_ID, 0u,
                  CSL_ECCAGGR_PULSAR_SL_CPU0_IDATA_BANK2_SIZE, 4u,
                  CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_IDATA_BANK2_ROW_SIZE, ((bool)false)       },
    {SDR_ECC_R5F_MEM_SUBTYPE_IDATA_BANK3_VECTOR_ID, 0u,
                  CSL_ECCAGGR_PULSAR_SL_CPU0_IDATA_BANK3_SIZE, 4u,
                  CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_IDATA_BANK3_ROW_SIZE, ((bool)false)       },
    {SDR_ECC_R5F_MEM_SUBTYPE_DTAG_RAM0_VECTOR_ID, 0u,
                  CSL_ECCAGGR_PULSAR_SL_CPU0_DTAG_RAM0_SIZE, 4u,
                  CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_DTAG_RAM0_ROW_SIZE, ((bool)false)         },
    {SDR_ECC_R5F_MEM_SUBTYPE_DTAG_RAM1_VECTOR_ID, 0u,
                  CSL_ECCAGGR_PULSAR_SL_CPU0_DTAG_RAM1_SIZE, 4u,
                  CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_DTAG_RAM1_ROW_SIZE, ((bool)false)         },
    {SDR_ECC_R5F_MEM_SUBTYPE_DTAG_RAM2_VECTOR_ID, 0u,
                  CSL_ECCAGGR_PULSAR_SL_CPU0_DTAG_RAM2_SIZE, 4u,
                  CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_DTAG_RAM2_ROW_SIZE, ((bool)false)         },
    {SDR_ECC_R5F_MEM_SUBTYPE_DTAG_RAM3_VECTOR_ID, 0u,
                  CSL_ECCAGGR_PULSAR_SL_CPU0_DTAG_RAM3_SIZE, 4u,
                  CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_DTAG_RAM3_ROW_SIZE, ((bool)false)         },
    {SDR_ECC_R5F_MEM_SUBTYPE_DDIRTY_RAM_VECTOR_ID, 0u,
                  CSL_ECCAGGR_PULSAR_SL_CPU0_DDIRTY_RAM_SIZE, 4u,
                  CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_DDIRTY_RAM_ROW_SIZE, ((bool)false)        },
    {SDR_ECC_R5F_MEM_SUBTYPE_DDATA_RAM0_VECTOR_ID, 0u,
                  CSL_ECCAGGR_PULSAR_SL_CPU0_DDATA_RAM0_SIZE, 4u,
                  CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_DDATA_RAM0_ROW_SIZE, ((bool)false)        },
    {SDR_ECC_R5F_MEM_SUBTYPE_DDATA_RAM1_VECTOR_ID, 0u,
                  CSL_ECCAGGR_PULSAR_SL_CPU0_DDATA_RAM1_SIZE, 4u,
                  CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_DDATA_RAM1_ROW_SIZE, ((bool)false)        },
    {SDR_ECC_R5F_MEM_SUBTYPE_DDATA_RAM2_VECTOR_ID, 0u,
                  CSL_ECCAGGR_PULSAR_SL_CPU0_DDATA_RAM2_SIZE, 4u,
                  CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_DDATA_RAM2_ROW_SIZE, ((bool)false)        },
    {SDR_ECC_R5F_MEM_SUBTYPE_DDATA_RAM3_VECTOR_ID, 0u,
                  CSL_ECCAGGR_PULSAR_SL_CPU0_DDATA_RAM3_SIZE, 4u,
                  CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_DDATA_RAM3_ROW_SIZE, ((bool)false)        },
    {SDR_ECC_R5F_MEM_SUBTYPE_DDATA_RAM4_VECTOR_ID ,0u,
                  CSL_ECCAGGR_PULSAR_SL_CPU0_DDATA_RAM4_SIZE, 4u,
                  CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_DDATA_RAM4_ROW_SIZE, ((bool)false)        },
    {SDR_ECC_R5F_MEM_SUBTYPE_DDATA_RAM5_VECTOR_ID, 0u,
                  CSL_ECCAGGR_PULSAR_SL_CPU0_DDATA_RAM5_SIZE, 4u,
                  CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_DDATA_RAM5_ROW_SIZE, ((bool)false)        },
    {SDR_ECC_R5F_MEM_SUBTYPE_DDATA_RAM6_VECTOR_ID, 0u,
                  CSL_ECCAGGR_PULSAR_SL_CPU0_DDATA_RAM6_SIZE, 4u,
                  CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_DDATA_RAM6_ROW_SIZE, ((bool)false)        },
    {SDR_ECC_R5F_MEM_SUBTYPE_DDATA_RAM7_VECTOR_ID, 0u,
                  CSL_ECCAGGR_PULSAR_SL_CPU0_DDATA_RAM7_SIZE, 4u,
                  CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_DDATA_RAM7_ROW_SIZE, ((bool)false)        },
    {SDR_ECC_R5F_MEM_SUBTYPE_ATCM0_BANK0_VECTOR_ID, 0u,
                  CSL_ECCAGGR_PULSAR_SL_CPU0_ATCM0_BANK0_SIZE, 8u,
                  CSL_MCU_R5FSS0_CORE0_ECC_AGGR_PULSAR_SL_ATCM0_BANK0_ROW_SIZE, ((bool)true)   },
    {SDR_ECC_R5F_MEM_SUBTYPE_ATCM0_BANK1_VECTOR_ID, 0x4u,
                  CSL_ECCAGGR_PULSAR_SL_CPU0_ATCM0_BANK1_SIZE, 8u,
                  CSL_MCU_R5FSS0_CORE0_ECC_AGGR_PULSAR_SL_ATCM0_BANK1_ROW_SIZE, ((bool)true)   },
    {SDR_ECC_R5F_MEM_SUBTYPE_B0TCM0_BANK0_VECTOR_ID, 0x41010000u,
                  CSL_ECCAGGR_PULSAR_SL_CPU0_B0TCM0_BANK0_SIZE, 16u,
                  CSL_MCU_R5FSS0_CORE0_ECC_AGGR_PULSAR_SL_B0TCM0_BANK0_ROW_SIZE, ((bool)true)  },
    {SDR_ECC_R5F_MEM_SUBTYPE_B0TCM0_BANK1_VECTOR_ID, 0x41010004u,
                  CSL_ECCAGGR_PULSAR_SL_CPU0_B0TCM0_BANK1_SIZE, 16u,
                  CSL_MCU_R5FSS0_CORE0_ECC_AGGR_PULSAR_SL_B0TCM0_BANK1_ROW_SIZE, ((bool)true)  },
    {SDR_ECC_R5F_MEM_SUBTYPE_B1TCM0_BANK0_VECTOR_ID, 0x41010008u,
                  CSL_ECCAGGR_PULSAR_SL_CPU0_B1TCM0_BANK0_SIZE, 16u,
                  CSL_MCU_R5FSS0_CORE0_ECC_AGGR_PULSAR_SL_B1TCM0_BANK0_ROW_SIZE, ((bool)true)  },
    {SDR_ECC_R5F_MEM_SUBTYPE_B1TCM0_BANK1_VECTOR_ID, 0x4101000cu,
                  CSL_ECCAGGR_PULSAR_SL_CPU0_B1TCM0_BANK1_SIZE, 16u,
                  CSL_MCU_R5FSS0_CORE0_ECC_AGGR_PULSAR_SL_B1TCM0_BANK1_ROW_SIZE, ((bool)true)  },
    {SDR_ECC_R5F_MEM_SUBTYPE_KS_VIM_RAM_VECTOR_ID, 0x40f82000u,
                  CSL_ECCAGGR_PULSAR_SL_CPU0_KS_VIM_RAMECC_SIZE, 4u,
                  CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_KS_VIM_RAMECC_ROW_SIZE, ((bool)true) },
};

/* Max entries based on max mem type */
#define SDR_ECC_AGGREGATOR_MAX_ENTRIES (SDR_ECC_MEMTYPE_MCU_PSRAM0+1u)

CSL_ecc_aggrRegs * const SDR_ECC_aggrBaseAddressTable[SDR_ECC_AGGREGATOR_MAX_ENTRIES] =
{
    ((CSL_ecc_aggrRegs *)((uintptr_t)CSL_MCU_ARMSS0_CORE0_ECC_AGGR_BASE)),
    ((CSL_ecc_aggrRegs *)((uintptr_t)CSL_MCU_ARMSS0_CORE1_ECC_AGGR_BASE)),
    ((CSL_ecc_aggrRegs *)((uintptr_t)CSL_MCU_ADC0_ECC_BASE)),
    ((CSL_ecc_aggrRegs *)((uintptr_t)CSL_MCU_ADC1_ECC_BASE)),
    ((CSL_ecc_aggrRegs *)((uintptr_t)CSL_MCU_CPSW0_ECC_BASE)),
    ((CSL_ecc_aggrRegs *)((uintptr_t)CSL_MCU_FSS0_HPB_ECC_AGGR_BASE)),
    ((CSL_ecc_aggrRegs *)((uintptr_t)CSL_MCU_FSS0_OSPI0_ECC_AGGR_BASE)),
    ((CSL_ecc_aggrRegs *)((uintptr_t)CSL_MCU_FSS0_OSPI1_ECC_AGGR_BASE)),
    ((CSL_ecc_aggrRegs *)((uintptr_t)CSL_MCU_MCAN0_ECC_AGGR_BASE)),
    ((CSL_ecc_aggrRegs *)((uintptr_t)CSL_MCU_MCAN1_ECC_AGGR_BASE)),
    ((CSL_ecc_aggrRegs *)((uintptr_t)CSL_MCU_MSRAM0_ECC_AGGR_BASE)),
    ((CSL_ecc_aggrRegs *)((uintptr_t)CSL_MCU_NAVSS0_ECCAGGR0_BASE)),
    ((CSL_ecc_aggrRegs *)((uintptr_t)CSL_MCU_PDMA0_ECC_AGGR_BASE)),
    ((CSL_ecc_aggrRegs *)((uintptr_t)CSL_MCU_PDMA1_ECC_AGGR_BASE)),
    ((CSL_ecc_aggrRegs *)((uintptr_t)CSL_PSRAMECC0_ECC_AGGR_BASE)),
};

#endif /* INCLUDE_SDR_ECC_SOC_H_ */
