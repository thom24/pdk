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
#define SDR_PULSAR_CPU_RAM_ID_TABLE_MAX_ENTRIES (CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_ECC_AGGR_EDC_CTRL_RAM_ID+1u)

/* define Max memEntries for each aggregator (i.e. the number of RAM ID's with
 * Wrapper type) */
#define SDR_PULSAR_CPU_WRAPPER_RAM_IDS_TOTAL_ENTRIES                                      (28U)
#define SDR_MSMC_AGGR0_WRAPPER_RAM_IDS_TOTAL_ENTRIES                                      (1U)
#define SDR_MCU_CBASS_WRAPPER_RAM_IDS_TOTAL_ENTRIES                                       (2U)

#define SDR_PULSAR_CPU_RAM_ID_VBUSM2_AXI_EDC_VECTOR_GRP_MAX_ENTRIES (CSL_MCU_R5FSS0_CORE0_ECC_AGGR_KSBUS_VBUSM2AXI0_EDC_CTRL_MAX_NUM_CHECKERS)


/** ------------------------------------------------------------------------------------
 * @brief This structure holds the list of Ram Ids for each memory subtype in MCU domain
 * -------------------------------------------------------------------------------------
 */
const SDR_RAMIdEntry_t SDR_ECC_mcuArmssRamIdTable[SDR_PULSAR_CPU_RAM_ID_TABLE_MAX_ENTRIES] =
{
 { CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_ITAG_RAM0_RAM_ID,
       CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_ITAG_RAM0_INJECT_TYPE,
       CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_ITAG_RAM0_ECC_TYPE },
 { CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_ITAG_RAM1_RAM_ID,
       CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_ITAG_RAM1_INJECT_TYPE,
       CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_ITAG_RAM1_ECC_TYPE },
 { CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_ITAG_RAM2_RAM_ID,
       CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_ITAG_RAM2_INJECT_TYPE,
       CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_ITAG_RAM2_ECC_TYPE },
 { CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_ITAG_RAM3_RAM_ID,
       CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_ITAG_RAM3_INJECT_TYPE,
       CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_ITAG_RAM3_ECC_TYPE },
 { CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_IDATA_BANK0_RAM_ID,
       CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_IDATA_BANK0_INJECT_TYPE,
       CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_IDATA_BANK0_ECC_TYPE },
 { CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_IDATA_BANK1_RAM_ID,
       CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_IDATA_BANK1_INJECT_TYPE,
       CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_IDATA_BANK1_ECC_TYPE },
 { CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_IDATA_BANK2_RAM_ID,
       CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_IDATA_BANK2_INJECT_TYPE,
       CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_IDATA_BANK2_ECC_TYPE },
 { CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_IDATA_BANK3_RAM_ID,
       CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_IDATA_BANK3_INJECT_TYPE,
       CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_IDATA_BANK3_ECC_TYPE },
 { CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_DTAG_RAM0_RAM_ID,
       CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_DTAG_RAM0_INJECT_TYPE,
       CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_DTAG_RAM0_ECC_TYPE },
 { CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_DTAG_RAM1_RAM_ID,
       CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_DTAG_RAM1_INJECT_TYPE,
       CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_DTAG_RAM1_ECC_TYPE },
 { CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_DTAG_RAM2_RAM_ID,
       CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_DTAG_RAM2_INJECT_TYPE,
       CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_DTAG_RAM2_ECC_TYPE },
 { CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_DTAG_RAM3_RAM_ID,
       CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_DTAG_RAM3_INJECT_TYPE,
       CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_DTAG_RAM3_ECC_TYPE },
 { CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_DDIRTY_RAM_RAM_ID,
       CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_DDIRTY_RAM_INJECT_TYPE,
       CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_DDIRTY_RAM_ECC_TYPE },
 { CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_DDATA_RAM0_RAM_ID,
       CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_DDATA_RAM0_INJECT_TYPE,
       CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_DDATA_RAM0_ECC_TYPE },
 { CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_DDATA_RAM1_RAM_ID,
       CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_DDATA_RAM1_INJECT_TYPE,
       CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_DDATA_RAM1_ECC_TYPE },
 { CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_DDATA_RAM2_RAM_ID,
       CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_DDATA_RAM2_INJECT_TYPE,
       CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_DDATA_RAM2_ECC_TYPE },
 { CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_DDATA_RAM3_RAM_ID,
       CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_DDATA_RAM3_INJECT_TYPE,
       CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_DDATA_RAM3_ECC_TYPE },
 { CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_DDATA_RAM4_RAM_ID,
       CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_DDATA_RAM4_INJECT_TYPE,
       CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_DDATA_RAM4_ECC_TYPE },
 { CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_DDATA_RAM5_RAM_ID,
       CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_DDATA_RAM5_INJECT_TYPE,
       CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_DDATA_RAM5_ECC_TYPE },
 { CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_DDATA_RAM6_RAM_ID,
       CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_DDATA_RAM6_INJECT_TYPE,
       CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_DDATA_RAM6_ECC_TYPE },
 { CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_DDATA_RAM7_RAM_ID,
       CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_DDATA_RAM7_INJECT_TYPE,
       CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_DDATA_RAM7_ECC_TYPE },
 { CSL_MCU_R5FSS0_CORE0_ECC_AGGR_PULSAR_SL_ATCM0_BANK0_RAM_ID,
       CSL_MCU_R5FSS0_CORE0_ECC_AGGR_PULSAR_SL_ATCM0_BANK0_INJECT_TYPE,
       CSL_MCU_R5FSS0_CORE0_ECC_AGGR_PULSAR_SL_ATCM0_BANK0_ECC_TYPE },
 { CSL_MCU_R5FSS0_CORE0_ECC_AGGR_PULSAR_SL_ATCM0_BANK1_RAM_ID,
       CSL_MCU_R5FSS0_CORE0_ECC_AGGR_PULSAR_SL_ATCM0_BANK1_INJECT_TYPE,
       CSL_MCU_R5FSS0_CORE0_ECC_AGGR_PULSAR_SL_ATCM0_BANK1_ECC_TYPE },
 { CSL_MCU_R5FSS0_CORE0_ECC_AGGR_PULSAR_SL_B0TCM0_BANK0_RAM_ID,
       CSL_MCU_R5FSS0_CORE0_ECC_AGGR_PULSAR_SL_B0TCM0_BANK0_INJECT_TYPE,
       CSL_MCU_R5FSS0_CORE0_ECC_AGGR_PULSAR_SL_B0TCM0_BANK0_ECC_TYPE },
 { CSL_MCU_R5FSS0_CORE0_ECC_AGGR_PULSAR_SL_B0TCM0_BANK1_RAM_ID,
       CSL_MCU_R5FSS0_CORE0_ECC_AGGR_PULSAR_SL_B0TCM0_BANK1_INJECT_TYPE,
       CSL_MCU_R5FSS0_CORE0_ECC_AGGR_PULSAR_SL_B0TCM0_BANK1_ECC_TYPE },
 { CSL_MCU_R5FSS0_CORE0_ECC_AGGR_PULSAR_SL_B1TCM0_BANK0_RAM_ID,
       CSL_MCU_R5FSS0_CORE0_ECC_AGGR_PULSAR_SL_B1TCM0_BANK0_INJECT_TYPE,
       CSL_MCU_R5FSS0_CORE0_ECC_AGGR_PULSAR_SL_B1TCM0_BANK0_ECC_TYPE },
 { CSL_MCU_R5FSS0_CORE0_ECC_AGGR_PULSAR_SL_B1TCM0_BANK1_RAM_ID,
       CSL_MCU_R5FSS0_CORE0_ECC_AGGR_PULSAR_SL_B1TCM0_BANK1_INJECT_TYPE,
       CSL_MCU_R5FSS0_CORE0_ECC_AGGR_PULSAR_SL_B1TCM0_BANK1_ECC_TYPE },
 { CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_KS_VIM_RAMECC_RAM_ID,
       CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_KS_VIM_RAMECC_INJECT_TYPE,
       CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_KS_VIM_RAMECC_ECC_TYPE },
 { CSL_MCU_R5FSS0_CORE0_ECC_AGGR_KSBUS_VBUSM2AXI0_EDC_CTRL_RAM_ID,
       CSL_MCU_R5FSS0_CORE0_ECC_AGGR_KSBUS_VBUSM2AXI0_EDC_CTRL_INJECT_TYPE,
       CSL_MCU_R5FSS0_CORE0_ECC_AGGR_KSBUS_VBUSM2AXI0_EDC_CTRL_ECC_TYPE }, // 28 - Interconnect Type
 { CSL_MCU_R5FSS0_CORE0_ECC_AGGR_MEM_MST0_KSBUS_AXI2VBUSM_W_EDC_CTRL_RAM_ID,
       CSL_MCU_R5FSS0_CORE0_ECC_AGGR_MEM_MST0_KSBUS_AXI2VBUSM_W_EDC_CTRL_INJECT_TYPE,
       CSL_MCU_R5FSS0_CORE0_ECC_AGGR_MEM_MST0_KSBUS_AXI2VBUSM_W_EDC_CTRL_ECC_TYPE }, // 29 - Interconnect Type
 { CSL_MCU_R5FSS0_CORE0_ECC_AGGR_MEM_MST0_KSBUS_AXI2VBUSM_R_EDC_CTRL_RAM_ID,
       CSL_MCU_R5FSS0_CORE0_ECC_AGGR_MEM_MST0_KSBUS_AXI2VBUSM_R_EDC_CTRL_INJECT_TYPE,
       CSL_MCU_R5FSS0_CORE0_ECC_AGGR_MEM_MST0_KSBUS_AXI2VBUSM_R_EDC_CTRL_ECC_TYPE }, // 30 - Interconnect Type
 { CSL_MCU_R5FSS0_CORE0_ECC_AGGR_PERIPH_M_MST0_KSBUS_AXI2VBUSM_W_EDC_CTRL_RAM_ID,
       CSL_MCU_R5FSS0_CORE0_ECC_AGGR_PERIPH_M_MST0_KSBUS_AXI2VBUSM_W_EDC_CTRL_INJECT_TYPE,
       CSL_MCU_R5FSS0_CORE0_ECC_AGGR_PERIPH_M_MST0_KSBUS_AXI2VBUSM_W_EDC_CTRL_ECC_TYPE }, // 31 - Interconnect Type
 { CSL_MCU_R5FSS0_CORE0_ECC_AGGR_PERIPH_M_MST0_KSBUS_AXI2VBUSM_R_EDC_CTRL_RAM_ID,
       CSL_MCU_R5FSS0_CORE0_ECC_AGGR_PERIPH_M_MST0_KSBUS_AXI2VBUSM_R_EDC_CTRL_INJECT_TYPE,
       CSL_MCU_R5FSS0_CORE0_ECC_AGGR_PERIPH_M_MST0_KSBUS_AXI2VBUSM_R_EDC_CTRL_ECC_TYPE }, // 32 - Interconnect Type
 { CSL_MCU_R5FSS0_CORE0_ECC_AGGR_PULSAR_AHB2VBUSP_CPU0_EDC_CTRL_RAM_ID,
       CSL_MCU_R5FSS0_CORE0_ECC_AGGR_PULSAR_AHB2VBUSP_CPU0_EDC_CTRL_INJECT_TYPE,
       CSL_MCU_R5FSS0_CORE0_ECC_AGGR_PULSAR_AHB2VBUSP_CPU0_EDC_CTRL_ECC_TYPE }, // 33 - Interconnect Type
 { CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_CFG_SCRP_INTERFACE0_GCLK_CLK_EDC_CTRL_RAM_ID,
       CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_CFG_SCRP_INTERFACE0_GCLK_CLK_EDC_CTRL_INJECT_TYPE,
       CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_CFG_SCRP_INTERFACE0_GCLK_CLK_EDC_CTRL_ECC_TYPE }, // 34 - Interconnect Type
 { CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_ECC_AGGR_EDC_CTRL_RAM_ID,
       CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_ECC_AGGR_EDC_CTRL_INJECT_TYPE,
       CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_ECC_AGGR_EDC_CTRL_ECC_TYPE }, // 35 - Interconnect Type
};

/** ----------------------------------------------------------------------------------
 * @brief This structure holds the memory config for each memory subtype in MCU domain
 * -----------------------------------------------------------------------------------
 */

/* NOTE: For VIM RAM Id, with a 32 bit vector, the 2 LSB bits are not used.
ECC is done only on  30 bits. But need to add a correction of 2 to get the address
calculation to be right */
#define SDR_ECC_VIM_RAM_ID_WIDTH_CORRECTION     (2U)

const SDR_MemConfig_t SDR_ECC_mcuArmssMemEntries[SDR_PULSAR_CPU_WRAPPER_RAM_IDS_TOTAL_ENTRIES] =
{
    {SDR_ECC_R5F_MEM_SUBTYPE_ITAG_RAM0_VECTOR_ID, 0u,
                  CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_ITAG_RAM0_RAM_SIZE, 4u,
                  CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_ITAG_RAM0_ROW_WIDTH,  ((bool)false)        },
    {SDR_ECC_R5F_MEM_SUBTYPE_ITAG_RAM1_VECTOR_ID, 0u,
                  CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_ITAG_RAM1_RAM_SIZE, 4u,
                  CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_ITAG_RAM1_ROW_WIDTH, ((bool)false)         },
    {SDR_ECC_R5F_MEM_SUBTYPE_ITAG_RAM2_VECTOR_ID, 0u,
                  CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_ITAG_RAM2_RAM_SIZE, 4u,
                  CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_ITAG_RAM2_ROW_WIDTH, ((bool)false)         },
    {SDR_ECC_R5F_MEM_SUBTYPE_ITAG_RAM3_VECTOR_ID, 0u,
                  CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_ITAG_RAM3_RAM_SIZE, 4u,
                  CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_ITAG_RAM3_ROW_WIDTH, ((bool)false)         },
    {SDR_ECC_R5F_MEM_SUBTYPE_IDATA_BANK0_VECTOR_ID, 0u,
                  CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_IDATA_BANK0_RAM_SIZE, 4u,
                  CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_IDATA_BANK0_ROW_WIDTH, ((bool)false)       },
    {SDR_ECC_R5F_MEM_SUBTYPE_IDATA_BANK1_VECTOR_ID, 0u,
                  CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_IDATA_BANK1_RAM_SIZE, 4u,
                  CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_IDATA_BANK1_ROW_WIDTH, ((bool)false)       },
    {SDR_ECC_R5F_MEM_SUBTYPE_IDATA_BANK2_VECTOR_ID, 0u,
                  CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_IDATA_BANK2_RAM_SIZE, 4u,
                  CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_IDATA_BANK2_ROW_WIDTH, ((bool)false)       },
    {SDR_ECC_R5F_MEM_SUBTYPE_IDATA_BANK3_VECTOR_ID, 0u,
                  CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_IDATA_BANK3_RAM_SIZE, 4u,
                  CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_IDATA_BANK3_ROW_WIDTH, ((bool)false)       },
    {SDR_ECC_R5F_MEM_SUBTYPE_DTAG_RAM0_VECTOR_ID, 0u,
                  CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_DTAG_RAM0_RAM_SIZE, 4u,
                  CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_DTAG_RAM0_ROW_WIDTH, ((bool)false)         },
    {SDR_ECC_R5F_MEM_SUBTYPE_DTAG_RAM1_VECTOR_ID, 0u,
                  CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_DTAG_RAM1_RAM_SIZE, 4u,
                  CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_DTAG_RAM1_ROW_WIDTH, ((bool)false)         },
    {SDR_ECC_R5F_MEM_SUBTYPE_DTAG_RAM2_VECTOR_ID, 0u,
                  CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_DTAG_RAM2_RAM_SIZE, 4u,
                  CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_DTAG_RAM2_ROW_WIDTH, ((bool)false)         },
    {SDR_ECC_R5F_MEM_SUBTYPE_DTAG_RAM3_VECTOR_ID, 0u,
                  CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_DTAG_RAM3_RAM_SIZE, 4u,
                  CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_DTAG_RAM3_ROW_WIDTH, ((bool)false)         },
    {SDR_ECC_R5F_MEM_SUBTYPE_DDIRTY_RAM_VECTOR_ID, 0u,
                  CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_DDIRTY_RAM_RAM_SIZE, 4u,
                  CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_DDIRTY_RAM_ROW_WIDTH, ((bool)false)        },
    {SDR_ECC_R5F_MEM_SUBTYPE_DDATA_RAM0_VECTOR_ID, 0u,
                  CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_DDATA_RAM0_RAM_SIZE, 4u,
                  CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_DDATA_RAM0_ROW_WIDTH, ((bool)false)        },
    {SDR_ECC_R5F_MEM_SUBTYPE_DDATA_RAM1_VECTOR_ID, 0u,
                  CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_DDATA_RAM1_RAM_SIZE, 4u,
                  CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_DDATA_RAM1_ROW_WIDTH, ((bool)false)        },
    {SDR_ECC_R5F_MEM_SUBTYPE_DDATA_RAM2_VECTOR_ID, 0u,
                  CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_DDATA_RAM2_RAM_SIZE, 4u,
                  CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_DDATA_RAM2_ROW_WIDTH, ((bool)false)        },
    {SDR_ECC_R5F_MEM_SUBTYPE_DDATA_RAM3_VECTOR_ID, 0u,
                  CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_DDATA_RAM3_RAM_SIZE, 4u,
                  CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_DDATA_RAM3_ROW_WIDTH, ((bool)false)        },
    {SDR_ECC_R5F_MEM_SUBTYPE_DDATA_RAM4_VECTOR_ID,0u,
                  CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_DDATA_RAM4_RAM_SIZE, 4u,
                  CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_DDATA_RAM4_ROW_WIDTH, ((bool)false)        },
    {SDR_ECC_R5F_MEM_SUBTYPE_DDATA_RAM5_VECTOR_ID, 0u,
                  CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_DDATA_RAM5_RAM_SIZE, 4u,
                  CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_DDATA_RAM5_ROW_WIDTH, ((bool)false)        },
    {SDR_ECC_R5F_MEM_SUBTYPE_DDATA_RAM6_VECTOR_ID, 0u,
                  CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_DDATA_RAM6_RAM_SIZE, 4u,
                  CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_DDATA_RAM6_ROW_WIDTH, ((bool)false)        },
    {SDR_ECC_R5F_MEM_SUBTYPE_DDATA_RAM7_VECTOR_ID, 0u,
                  CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_DDATA_RAM7_RAM_SIZE, 4u,
                  CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_DDATA_RAM7_ROW_WIDTH, ((bool)false)        },
    {SDR_ECC_R5F_MEM_SUBTYPE_ATCM0_BANK0_VECTOR_ID, 0u,
                  CSL_MCU_R5FSS0_CORE0_ECC_AGGR_PULSAR_SL_ATCM0_BANK0_RAM_SIZE, 8u,
                  CSL_MCU_R5FSS0_CORE0_ECC_AGGR_PULSAR_SL_ATCM0_BANK0_ROW_WIDTH, ((bool)true)   },
    {SDR_ECC_R5F_MEM_SUBTYPE_ATCM0_BANK1_VECTOR_ID, 0x4u,
                  CSL_MCU_R5FSS0_CORE0_ECC_AGGR_PULSAR_SL_ATCM0_BANK1_RAM_SIZE, 8u,
                  CSL_MCU_R5FSS0_CORE0_ECC_AGGR_PULSAR_SL_ATCM0_BANK1_ROW_WIDTH, ((bool)true)   },
    {SDR_ECC_R5F_MEM_SUBTYPE_B0TCM0_BANK0_VECTOR_ID, 0x41010000u,
                  CSL_MCU_R5FSS0_CORE0_ECC_AGGR_PULSAR_SL_B0TCM0_BANK0_RAM_SIZE, 16u,
                  CSL_MCU_R5FSS0_CORE0_ECC_AGGR_PULSAR_SL_B0TCM0_BANK0_ROW_WIDTH, ((bool)true)  },
    {SDR_ECC_R5F_MEM_SUBTYPE_B0TCM0_BANK1_VECTOR_ID, 0x41010004u,
                  CSL_MCU_R5FSS0_CORE0_ECC_AGGR_PULSAR_SL_B0TCM0_BANK1_RAM_SIZE, 16u,
                  CSL_MCU_R5FSS0_CORE0_ECC_AGGR_PULSAR_SL_B0TCM0_BANK1_ROW_WIDTH, ((bool)true)  },
    {SDR_ECC_R5F_MEM_SUBTYPE_B1TCM0_BANK0_VECTOR_ID, 0x41010008u,
                  CSL_MCU_R5FSS0_CORE0_ECC_AGGR_PULSAR_SL_B1TCM0_BANK0_RAM_SIZE, 16u,
                  CSL_MCU_R5FSS0_CORE0_ECC_AGGR_PULSAR_SL_B1TCM0_BANK0_ROW_WIDTH, ((bool)true)  },
    {SDR_ECC_R5F_MEM_SUBTYPE_B1TCM0_BANK1_VECTOR_ID, 0x4101000cu,
                  CSL_MCU_R5FSS0_CORE0_ECC_AGGR_PULSAR_SL_B1TCM0_BANK1_RAM_SIZE, 16u,
                  CSL_MCU_R5FSS0_CORE0_ECC_AGGR_PULSAR_SL_B1TCM0_BANK1_ROW_WIDTH, ((bool)true)  },
    /* NOTE: VIM width and size needs correction: see note above */
    {SDR_ECC_R5F_MEM_SUBTYPE_KS_VIM_RAM_VECTOR_ID, 0x40f82000u,
                  ((CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_KS_VIM_RAMECC_RAM_SIZE *(CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_KS_VIM_RAMECC_ROW_WIDTH+SDR_ECC_VIM_RAM_ID_WIDTH_CORRECTION))/CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_KS_VIM_RAMECC_ROW_WIDTH), 4u,
                  (CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_KS_VIM_RAMECC_ROW_WIDTH+SDR_ECC_VIM_RAM_ID_WIDTH_CORRECTION), ((bool)true) },
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


/** ----------------------------------------------------------------------------------
 * @brief This structure holds the ECC interconnect Group Checker information for
 * SDR_ECC_R5F_MEM_SUBTYPE_VBUSM2AXI_EDC_VECTOR_ID RAM ID
 * -----------------------------------------------------------------------------------
 */
const SDR_GrpChkConfig_t SDR_ECC_ramIdVbusM2AxiEdcVectorGrpEntries[SDR_PULSAR_CPU_RAM_ID_VBUSM2_AXI_EDC_VECTOR_GRP_MAX_ENTRIES] =
{
    {CSL_MCU_R5FSS0_CORE0_ECC_AGGR_KSBUS_VBUSM2AXI0_EDC_CTRL_GROUP_0_CHECKER_TYPE,
        CSL_MCU_R5FSS0_CORE0_ECC_AGGR_KSBUS_VBUSM2AXI0_EDC_CTRL_GROUP_0_WIDTH},
    {CSL_MCU_R5FSS0_CORE0_ECC_AGGR_KSBUS_VBUSM2AXI0_EDC_CTRL_GROUP_1_CHECKER_TYPE,
        CSL_MCU_R5FSS0_CORE0_ECC_AGGR_KSBUS_VBUSM2AXI0_EDC_CTRL_GROUP_1_WIDTH},
    {CSL_MCU_R5FSS0_CORE0_ECC_AGGR_KSBUS_VBUSM2AXI0_EDC_CTRL_GROUP_2_CHECKER_TYPE,
        CSL_MCU_R5FSS0_CORE0_ECC_AGGR_KSBUS_VBUSM2AXI0_EDC_CTRL_GROUP_2_WIDTH},
    {CSL_MCU_R5FSS0_CORE0_ECC_AGGR_KSBUS_VBUSM2AXI0_EDC_CTRL_GROUP_3_CHECKER_TYPE,
        CSL_MCU_R5FSS0_CORE0_ECC_AGGR_KSBUS_VBUSM2AXI0_EDC_CTRL_GROUP_3_WIDTH},
    {CSL_MCU_R5FSS0_CORE0_ECC_AGGR_KSBUS_VBUSM2AXI0_EDC_CTRL_GROUP_4_CHECKER_TYPE,
        CSL_MCU_R5FSS0_CORE0_ECC_AGGR_KSBUS_VBUSM2AXI0_EDC_CTRL_GROUP_4_WIDTH},
    {CSL_MCU_R5FSS0_CORE0_ECC_AGGR_KSBUS_VBUSM2AXI0_EDC_CTRL_GROUP_5_CHECKER_TYPE,
        CSL_MCU_R5FSS0_CORE0_ECC_AGGR_KSBUS_VBUSM2AXI0_EDC_CTRL_GROUP_5_WIDTH},
    {CSL_MCU_R5FSS0_CORE0_ECC_AGGR_KSBUS_VBUSM2AXI0_EDC_CTRL_GROUP_6_CHECKER_TYPE,
        CSL_MCU_R5FSS0_CORE0_ECC_AGGR_KSBUS_VBUSM2AXI0_EDC_CTRL_GROUP_6_WIDTH},
    {CSL_MCU_R5FSS0_CORE0_ECC_AGGR_KSBUS_VBUSM2AXI0_EDC_CTRL_GROUP_7_CHECKER_TYPE,
        CSL_MCU_R5FSS0_CORE0_ECC_AGGR_KSBUS_VBUSM2AXI0_EDC_CTRL_GROUP_7_WIDTH},
    {CSL_MCU_R5FSS0_CORE0_ECC_AGGR_KSBUS_VBUSM2AXI0_EDC_CTRL_GROUP_8_CHECKER_TYPE,
        CSL_MCU_R5FSS0_CORE0_ECC_AGGR_KSBUS_VBUSM2AXI0_EDC_CTRL_GROUP_8_WIDTH},
    {CSL_MCU_R5FSS0_CORE0_ECC_AGGR_KSBUS_VBUSM2AXI0_EDC_CTRL_GROUP_9_CHECKER_TYPE,
        CSL_MCU_R5FSS0_CORE0_ECC_AGGR_KSBUS_VBUSM2AXI0_EDC_CTRL_GROUP_9_WIDTH},
    {CSL_MCU_R5FSS0_CORE0_ECC_AGGR_KSBUS_VBUSM2AXI0_EDC_CTRL_GROUP_10_CHECKER_TYPE,
        CSL_MCU_R5FSS0_CORE0_ECC_AGGR_KSBUS_VBUSM2AXI0_EDC_CTRL_GROUP_10_WIDTH},
    {CSL_MCU_R5FSS0_CORE0_ECC_AGGR_KSBUS_VBUSM2AXI0_EDC_CTRL_GROUP_11_CHECKER_TYPE,
        CSL_MCU_R5FSS0_CORE0_ECC_AGGR_KSBUS_VBUSM2AXI0_EDC_CTRL_GROUP_11_WIDTH},
    {CSL_MCU_R5FSS0_CORE0_ECC_AGGR_KSBUS_VBUSM2AXI0_EDC_CTRL_GROUP_12_CHECKER_TYPE,
        CSL_MCU_R5FSS0_CORE0_ECC_AGGR_KSBUS_VBUSM2AXI0_EDC_CTRL_GROUP_12_WIDTH},
    {CSL_MCU_R5FSS0_CORE0_ECC_AGGR_KSBUS_VBUSM2AXI0_EDC_CTRL_GROUP_13_CHECKER_TYPE,
        CSL_MCU_R5FSS0_CORE0_ECC_AGGR_KSBUS_VBUSM2AXI0_EDC_CTRL_GROUP_13_WIDTH},
    {CSL_MCU_R5FSS0_CORE0_ECC_AGGR_KSBUS_VBUSM2AXI0_EDC_CTRL_GROUP_14_CHECKER_TYPE,
        CSL_MCU_R5FSS0_CORE0_ECC_AGGR_KSBUS_VBUSM2AXI0_EDC_CTRL_GROUP_14_WIDTH},
    {CSL_MCU_R5FSS0_CORE0_ECC_AGGR_KSBUS_VBUSM2AXI0_EDC_CTRL_GROUP_15_CHECKER_TYPE,
        CSL_MCU_R5FSS0_CORE0_ECC_AGGR_KSBUS_VBUSM2AXI0_EDC_CTRL_GROUP_15_WIDTH},
    {CSL_MCU_R5FSS0_CORE0_ECC_AGGR_KSBUS_VBUSM2AXI0_EDC_CTRL_GROUP_16_CHECKER_TYPE,
        CSL_MCU_R5FSS0_CORE0_ECC_AGGR_KSBUS_VBUSM2AXI0_EDC_CTRL_GROUP_16_WIDTH},
    {CSL_MCU_R5FSS0_CORE0_ECC_AGGR_KSBUS_VBUSM2AXI0_EDC_CTRL_GROUP_17_CHECKER_TYPE,
        CSL_MCU_R5FSS0_CORE0_ECC_AGGR_KSBUS_VBUSM2AXI0_EDC_CTRL_GROUP_17_WIDTH},
    {CSL_MCU_R5FSS0_CORE0_ECC_AGGR_KSBUS_VBUSM2AXI0_EDC_CTRL_GROUP_18_CHECKER_TYPE,
        CSL_MCU_R5FSS0_CORE0_ECC_AGGR_KSBUS_VBUSM2AXI0_EDC_CTRL_GROUP_18_WIDTH},
    {CSL_MCU_R5FSS0_CORE0_ECC_AGGR_KSBUS_VBUSM2AXI0_EDC_CTRL_GROUP_19_CHECKER_TYPE,
        CSL_MCU_R5FSS0_CORE0_ECC_AGGR_KSBUS_VBUSM2AXI0_EDC_CTRL_GROUP_19_WIDTH},
    {CSL_MCU_R5FSS0_CORE0_ECC_AGGR_KSBUS_VBUSM2AXI0_EDC_CTRL_GROUP_20_CHECKER_TYPE,
        CSL_MCU_R5FSS0_CORE0_ECC_AGGR_KSBUS_VBUSM2AXI0_EDC_CTRL_GROUP_20_WIDTH},
    {CSL_MCU_R5FSS0_CORE0_ECC_AGGR_KSBUS_VBUSM2AXI0_EDC_CTRL_GROUP_21_CHECKER_TYPE,
        CSL_MCU_R5FSS0_CORE0_ECC_AGGR_KSBUS_VBUSM2AXI0_EDC_CTRL_GROUP_21_WIDTH},
    {CSL_MCU_R5FSS0_CORE0_ECC_AGGR_KSBUS_VBUSM2AXI0_EDC_CTRL_GROUP_22_CHECKER_TYPE,
        CSL_MCU_R5FSS0_CORE0_ECC_AGGR_KSBUS_VBUSM2AXI0_EDC_CTRL_GROUP_22_WIDTH},
    {CSL_MCU_R5FSS0_CORE0_ECC_AGGR_KSBUS_VBUSM2AXI0_EDC_CTRL_GROUP_23_CHECKER_TYPE,
        CSL_MCU_R5FSS0_CORE0_ECC_AGGR_KSBUS_VBUSM2AXI0_EDC_CTRL_GROUP_23_WIDTH},
    {CSL_MCU_R5FSS0_CORE0_ECC_AGGR_KSBUS_VBUSM2AXI0_EDC_CTRL_GROUP_24_CHECKER_TYPE,
        CSL_MCU_R5FSS0_CORE0_ECC_AGGR_KSBUS_VBUSM2AXI0_EDC_CTRL_GROUP_24_WIDTH},
    {CSL_MCU_R5FSS0_CORE0_ECC_AGGR_KSBUS_VBUSM2AXI0_EDC_CTRL_GROUP_25_CHECKER_TYPE,
        CSL_MCU_R5FSS0_CORE0_ECC_AGGR_KSBUS_VBUSM2AXI0_EDC_CTRL_GROUP_25_WIDTH},
    {CSL_MCU_R5FSS0_CORE0_ECC_AGGR_KSBUS_VBUSM2AXI0_EDC_CTRL_GROUP_26_CHECKER_TYPE,
        CSL_MCU_R5FSS0_CORE0_ECC_AGGR_KSBUS_VBUSM2AXI0_EDC_CTRL_GROUP_26_WIDTH},
    {CSL_MCU_R5FSS0_CORE0_ECC_AGGR_KSBUS_VBUSM2AXI0_EDC_CTRL_GROUP_27_CHECKER_TYPE,
        CSL_MCU_R5FSS0_CORE0_ECC_AGGR_KSBUS_VBUSM2AXI0_EDC_CTRL_GROUP_27_WIDTH},
    {CSL_MCU_R5FSS0_CORE0_ECC_AGGR_KSBUS_VBUSM2AXI0_EDC_CTRL_GROUP_28_CHECKER_TYPE,
        CSL_MCU_R5FSS0_CORE0_ECC_AGGR_KSBUS_VBUSM2AXI0_EDC_CTRL_GROUP_28_WIDTH},
    {CSL_MCU_R5FSS0_CORE0_ECC_AGGR_KSBUS_VBUSM2AXI0_EDC_CTRL_GROUP_29_CHECKER_TYPE,
        CSL_MCU_R5FSS0_CORE0_ECC_AGGR_KSBUS_VBUSM2AXI0_EDC_CTRL_GROUP_29_WIDTH},
    {CSL_MCU_R5FSS0_CORE0_ECC_AGGR_KSBUS_VBUSM2AXI0_EDC_CTRL_GROUP_30_CHECKER_TYPE,
        CSL_MCU_R5FSS0_CORE0_ECC_AGGR_KSBUS_VBUSM2AXI0_EDC_CTRL_GROUP_30_WIDTH},
    {CSL_MCU_R5FSS0_CORE0_ECC_AGGR_KSBUS_VBUSM2AXI0_EDC_CTRL_GROUP_31_CHECKER_TYPE,
        CSL_MCU_R5FSS0_CORE0_ECC_AGGR_KSBUS_VBUSM2AXI0_EDC_CTRL_GROUP_31_WIDTH},
    {CSL_MCU_R5FSS0_CORE0_ECC_AGGR_KSBUS_VBUSM2AXI0_EDC_CTRL_GROUP_32_CHECKER_TYPE,
        CSL_MCU_R5FSS0_CORE0_ECC_AGGR_KSBUS_VBUSM2AXI0_EDC_CTRL_GROUP_32_WIDTH},
    {CSL_MCU_R5FSS0_CORE0_ECC_AGGR_KSBUS_VBUSM2AXI0_EDC_CTRL_GROUP_33_CHECKER_TYPE,
        CSL_MCU_R5FSS0_CORE0_ECC_AGGR_KSBUS_VBUSM2AXI0_EDC_CTRL_GROUP_33_WIDTH},
    {CSL_MCU_R5FSS0_CORE0_ECC_AGGR_KSBUS_VBUSM2AXI0_EDC_CTRL_GROUP_34_CHECKER_TYPE,
        CSL_MCU_R5FSS0_CORE0_ECC_AGGR_KSBUS_VBUSM2AXI0_EDC_CTRL_GROUP_34_WIDTH},
    {CSL_MCU_R5FSS0_CORE0_ECC_AGGR_KSBUS_VBUSM2AXI0_EDC_CTRL_GROUP_35_CHECKER_TYPE,
        CSL_MCU_R5FSS0_CORE0_ECC_AGGR_KSBUS_VBUSM2AXI0_EDC_CTRL_GROUP_35_WIDTH},
    {CSL_MCU_R5FSS0_CORE0_ECC_AGGR_KSBUS_VBUSM2AXI0_EDC_CTRL_GROUP_36_CHECKER_TYPE,
        CSL_MCU_R5FSS0_CORE0_ECC_AGGR_KSBUS_VBUSM2AXI0_EDC_CTRL_GROUP_36_WIDTH},
    {CSL_MCU_R5FSS0_CORE0_ECC_AGGR_KSBUS_VBUSM2AXI0_EDC_CTRL_GROUP_37_CHECKER_TYPE,
        CSL_MCU_R5FSS0_CORE0_ECC_AGGR_KSBUS_VBUSM2AXI0_EDC_CTRL_GROUP_37_WIDTH},
    {CSL_MCU_R5FSS0_CORE0_ECC_AGGR_KSBUS_VBUSM2AXI0_EDC_CTRL_GROUP_38_CHECKER_TYPE,
        CSL_MCU_R5FSS0_CORE0_ECC_AGGR_KSBUS_VBUSM2AXI0_EDC_CTRL_GROUP_38_WIDTH},
    {CSL_MCU_R5FSS0_CORE0_ECC_AGGR_KSBUS_VBUSM2AXI0_EDC_CTRL_GROUP_39_CHECKER_TYPE,
        CSL_MCU_R5FSS0_CORE0_ECC_AGGR_KSBUS_VBUSM2AXI0_EDC_CTRL_GROUP_39_WIDTH},
    {CSL_MCU_R5FSS0_CORE0_ECC_AGGR_KSBUS_VBUSM2AXI0_EDC_CTRL_GROUP_40_CHECKER_TYPE,
        CSL_MCU_R5FSS0_CORE0_ECC_AGGR_KSBUS_VBUSM2AXI0_EDC_CTRL_GROUP_40_WIDTH},
    {CSL_MCU_R5FSS0_CORE0_ECC_AGGR_KSBUS_VBUSM2AXI0_EDC_CTRL_GROUP_41_CHECKER_TYPE,
        CSL_MCU_R5FSS0_CORE0_ECC_AGGR_KSBUS_VBUSM2AXI0_EDC_CTRL_GROUP_41_WIDTH},
    {CSL_MCU_R5FSS0_CORE0_ECC_AGGR_KSBUS_VBUSM2AXI0_EDC_CTRL_GROUP_42_CHECKER_TYPE,
        CSL_MCU_R5FSS0_CORE0_ECC_AGGR_KSBUS_VBUSM2AXI0_EDC_CTRL_GROUP_42_WIDTH},
    {CSL_MCU_R5FSS0_CORE0_ECC_AGGR_KSBUS_VBUSM2AXI0_EDC_CTRL_GROUP_43_CHECKER_TYPE,
        CSL_MCU_R5FSS0_CORE0_ECC_AGGR_KSBUS_VBUSM2AXI0_EDC_CTRL_GROUP_43_WIDTH},
    {CSL_MCU_R5FSS0_CORE0_ECC_AGGR_KSBUS_VBUSM2AXI0_EDC_CTRL_GROUP_44_CHECKER_TYPE,
        CSL_MCU_R5FSS0_CORE0_ECC_AGGR_KSBUS_VBUSM2AXI0_EDC_CTRL_GROUP_44_WIDTH},
    {CSL_MCU_R5FSS0_CORE0_ECC_AGGR_KSBUS_VBUSM2AXI0_EDC_CTRL_GROUP_45_CHECKER_TYPE,
        CSL_MCU_R5FSS0_CORE0_ECC_AGGR_KSBUS_VBUSM2AXI0_EDC_CTRL_GROUP_45_WIDTH},
    {CSL_MCU_R5FSS0_CORE0_ECC_AGGR_KSBUS_VBUSM2AXI0_EDC_CTRL_GROUP_46_CHECKER_TYPE,
        CSL_MCU_R5FSS0_CORE0_ECC_AGGR_KSBUS_VBUSM2AXI0_EDC_CTRL_GROUP_46_WIDTH},
    {CSL_MCU_R5FSS0_CORE0_ECC_AGGR_KSBUS_VBUSM2AXI0_EDC_CTRL_GROUP_47_CHECKER_TYPE,
        CSL_MCU_R5FSS0_CORE0_ECC_AGGR_KSBUS_VBUSM2AXI0_EDC_CTRL_GROUP_47_WIDTH},
    {CSL_MCU_R5FSS0_CORE0_ECC_AGGR_KSBUS_VBUSM2AXI0_EDC_CTRL_GROUP_48_CHECKER_TYPE,
        CSL_MCU_R5FSS0_CORE0_ECC_AGGR_KSBUS_VBUSM2AXI0_EDC_CTRL_GROUP_48_WIDTH},
};

#endif /* INCLUDE_SDR_ECC_SOC_H_ */
