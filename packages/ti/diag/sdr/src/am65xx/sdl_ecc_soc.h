/*
 * SDL ECC
 *
 * SafeTI Diagnostics Library module for ECC
 *
 * Copyright (C) 2018-2019 Texas Instruments Incorporated - http://www.ti.com/
 * ALL RIGHTS RESERVED
 */

#ifndef INCLUDE_SDL_ECC_SOC_H_
#define INCLUDE_SDL_ECC_SOC_H_

#include <stdint.h>
#include <sdl_ecc.h>
#include <ti/csl/csl_ecc_aggr.h>

#include "sdl_ecc_priv.h"

/* define MAX entry based on list of Subtypes */
#define SDL_PULSAR_CPU_RAM_ID_TABLE_MAX_ENTRIES (CSL_ECCAGGR_PULSAR_SL_CPU0_KS_VIM_RAMECC_VECTOR_ID+1u)

/** ---------------------------------------------------------------------------
 * @brief This structure holds the list of Ram Ids for each memory subtype
 * ----------------------------------------------------------------------------
 */
const SDL_RAMIdEntry_t SDL_ECC_mcuArmssRamIdTable[SDL_PULSAR_CPU_RAM_ID_TABLE_MAX_ENTRIES] =
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
const SDL_MemConfig_t SDL_ECC_mcuArmssMemEntries[SDL_PULSAR_CPU_RAM_ID_TABLE_MAX_ENTRIES] =
{
    {0u,  CSL_ECCAGGR_PULSAR_SL_CPU0_ITAG_RAM0_SIZE, 4u, ((bool)false)              },
    {0u,  CSL_ECCAGGR_PULSAR_SL_CPU0_ITAG_RAM1_SIZE, 4u, ((bool)false)              },
    {0u,  CSL_ECCAGGR_PULSAR_SL_CPU0_ITAG_RAM2_SIZE, 4u, ((bool)false)              },
    {0u,  CSL_ECCAGGR_PULSAR_SL_CPU0_ITAG_RAM3_SIZE, 4u, ((bool)false)              },
    {0u,  CSL_ECCAGGR_PULSAR_SL_CPU0_IDATA_BANK0_SIZE, 4u, ((bool)false)            },
    {0u,  CSL_ECCAGGR_PULSAR_SL_CPU0_IDATA_BANK1_SIZE, 4u, ((bool)false)            },
    {0u,  CSL_ECCAGGR_PULSAR_SL_CPU0_IDATA_BANK2_SIZE, 4u, ((bool)false)            },
    {0u,  CSL_ECCAGGR_PULSAR_SL_CPU0_IDATA_BANK3_SIZE, 4u, ((bool)false)            },
    {0u,  CSL_ECCAGGR_PULSAR_SL_CPU0_DTAG_RAM0_SIZE, 4u, ((bool)false)              },
    {0u,  CSL_ECCAGGR_PULSAR_SL_CPU0_DTAG_RAM1_SIZE, 4u, ((bool)false)              },
    {0u,  CSL_ECCAGGR_PULSAR_SL_CPU0_DTAG_RAM2_SIZE, 4u, ((bool)false)              },
    {0u,  CSL_ECCAGGR_PULSAR_SL_CPU0_DTAG_RAM3_SIZE, 4u, ((bool)false)              },
    {0u,  CSL_ECCAGGR_PULSAR_SL_CPU0_DDIRTY_RAM_SIZE, 4u, ((bool)false)             },
    {0u,  CSL_ECCAGGR_PULSAR_SL_CPU0_DDATA_RAM0_SIZE, 4u, ((bool)false)             },
    {0u,  CSL_ECCAGGR_PULSAR_SL_CPU0_DDATA_RAM1_SIZE, 4u, ((bool)false)             },
    {0u,  CSL_ECCAGGR_PULSAR_SL_CPU0_DDATA_RAM2_SIZE, 4u, ((bool)false)             },
    {0u,  CSL_ECCAGGR_PULSAR_SL_CPU0_DDATA_RAM3_SIZE, 4u, ((bool)false)             },
    {0u,  CSL_ECCAGGR_PULSAR_SL_CPU0_DDATA_RAM4_SIZE, 4u, ((bool)false)             },
    {0u,  CSL_ECCAGGR_PULSAR_SL_CPU0_DDATA_RAM5_SIZE, 4u, ((bool)false)             },
    {0u,  CSL_ECCAGGR_PULSAR_SL_CPU0_DDATA_RAM6_SIZE, 4u, ((bool)false)             },
    {0u,  CSL_ECCAGGR_PULSAR_SL_CPU0_DDATA_RAM7_SIZE, 4u, ((bool)false)             },
    {0u,  CSL_ECCAGGR_PULSAR_SL_CPU0_ATCM0_BANK0_SIZE, 8u, ((bool)true)             },
    {0x4u,  CSL_ECCAGGR_PULSAR_SL_CPU0_ATCM0_BANK1_SIZE, 8u, ((bool)true)           },
    {0x41010000u,  CSL_ECCAGGR_PULSAR_SL_CPU0_B0TCM0_BANK0_SIZE, 16u, ((bool)true)  },
    {0x41010004u,  CSL_ECCAGGR_PULSAR_SL_CPU0_B0TCM0_BANK1_SIZE, 16u, ((bool)true)  },
    {0x41010008u,  CSL_ECCAGGR_PULSAR_SL_CPU0_B1TCM0_BANK0_SIZE, 16u, ((bool)true)  },
    {0x4101000cu,  CSL_ECCAGGR_PULSAR_SL_CPU0_B1TCM0_BANK1_SIZE, 16u, ((bool)true)  },
    {0x40f82000u,  CSL_ECCAGGR_PULSAR_SL_CPU0_KS_VIM_RAMECC_SIZE, 4u, ((bool)true) },
};

/* Max entries based on max mem type */
#define SDL_ECC_AGGREGATOR_MAX_ENTRIES (SDL_ECC_MEMTYPE_MCU_PSRAM0+1u)

CSL_ecc_aggrRegs * const SDL_ECC_aggrBaseAddressTable[SDL_ECC_AGGREGATOR_MAX_ENTRIES] =
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

#endif /* INCLUDE_SDL_ECC_SOC_H_ */
