/*
 * Copyright (C) 2018-2020 Texas Instruments Incorporated - http://www.ti.com/
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of Texas Instruments Incorporated nor the names of
 * its contributors may be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef SBL_SOC_CFG_H
#define SBL_SOC_CFG_H

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include <ti/csl/soc.h>
#include <ti/csl/csl_vtm.h>
#include <ti/drv/sciclient/sciclient.h>

#if defined (SOC_J721E)
#include <ti/csl/soc/j721e/src/cslr_wkup_ctrl_mmr.h>
#include <ti/csl/soc/j721e/src/cslr_mcu_pll_mmr.h>
#endif

#if defined (SOC_AM64X)
#include <ti/csl/soc/am64x/src/cslr_mcu_pll_mmr.h>
#endif
/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */
/* Structure holding information about a core that is needed to reset it. */
typedef struct
{
    /* Proc id of a core. */
    uint32_t    region_size;
    /* Device id of a core. */
    uint32_t    local_Addr;
    /* Clk id of a core. */
    uint32_t    soc_addr_lo;
    /* Startup freq of a core in Hz */
    uint32_t    soc_addr_hi;

}sblRatCfgInfo_t;

extern sblRatCfgInfo_t* sblRatCfgList;

/* Structure holding information about the Leo PMIC configuration*/
typedef struct
{
    /* I2C slave address of the PMIC. */
    uint8_t    slaveAddr;
    /* ID of the power resource. */
    uint8_t    powerResource;
    /* Voltage associated with resource in millivolts. */
    uint16_t    millivolts;

}sblCfgPmic_t;

extern uint16_t sblMapOtpVidToMilliVolts[256];

#define SBL_MAX_VTM_VDS        (8U)
#define SBL_OPP_LOW            (CSL_VTM_CFG1_OPPVID_OPP_LOW_DFLT_SHIFT)
#define SBL_OPP_NOM            (CSL_VTM_CFG1_OPPVID_OPP_NOM_DFLT_SHIFT)
#define SBL_OPP_ODR            (CSL_VTM_CFG1_OPPVID_OPP_ODR_DFLT_SHIFT)
#define SBL_OPP_TRB            (CSL_VTM_CFG1_OPPVID_OPP_TRB_DFLT_SHIFT)

#if defined (SOC_AM65XX)

#define SBL_MCU_ATCM_BASE      (CSL_MCU_ATCM_BASE)
#define SBL_MCU_ATCM_SIZE      (CSL_MCU_ATCM_SIZE)
#define SBL_MCU_BTCM_BASE      (CSL_MCU_BTCM_BASE)
#define SBL_MCU_BTCM_SIZE      (CSL_MCU_BTCM_SIZE)

#define SBL_MCU1_CPU1_ATCM_BASE_ADDR_SOC    (CSL_MCU_ARMSS0_CORE1_ATCM_BASE)
#define SBL_MCU2_CPU0_ATCM_BASE_ADDR_SOC    (0xBAD00000)
#define SBL_MCU2_CPU1_ATCM_BASE_ADDR_SOC    (0xBAD00000)
#define SBL_MCU3_CPU0_ATCM_BASE_ADDR_SOC    (0xBAD00000)
#define SBL_MCU3_CPU1_ATCM_BASE_ADDR_SOC    (0xBAD00000)

#define SBL_MCU1_CPU1_BTCM_BASE_ADDR_SOC    (CSL_MCU_ARMSS0_CORE1_BTCM_BASE)
#define SBL_MCU2_CPU0_BTCM_BASE_ADDR_SOC    (0xBAD00000)
#define SBL_MCU2_CPU1_BTCM_BASE_ADDR_SOC    (0xBAD00000)
#define SBL_MCU3_CPU0_BTCM_BASE_ADDR_SOC    (0xBAD00000)
#define SBL_MCU3_CPU1_BTCM_BASE_ADDR_SOC    (0xBAD00000)

#define SBL_C66X_L2SRAM_BASE                (0xBAD00000)
#define SBL_C66X_L2SRAM_SIZE                (0xBAD00000)
#define SBL_C66X_L1DMEM_BASE                (0xBAD00000)
#define SBL_C66X_L1DMEM_SIZE                (0xBAD00000)

#define SBL_C66X1_L2SRAM_BASE_ADDR_SOC      (0xBAD00000)
#define SBL_C66X2_L2SRAM_BASE_ADDR_SOC      (0xBAD00000)

#define SBL_C66X1_L1DMEM_BASE_ADDR_SOC      (0xBAD00000)
#define SBL_C66X2_L1DMEM_BASE_ADDR_SOC      (0xBAD00000)

#define SBL_C7X_L2SRAM_BASE                 (0xBAD00000)
#define SBL_C7X_L2SRAM_SIZE                 (0xBAD00000)
#define SBL_C7X_L1DMEM_BASE                 (0xBAD00000)
#define SBL_C7X_L1DMEM_SIZE                 (0xBAD00000)

#define SBL_C7X1_L2SRAM_BASE_ADDR_SOC       (0xBAD00000)
#define SBL_C7X2_L2SRAM_BASE_ADDR_SOC       (0xBAD00000)

#define SBL_C7X1_L1DMEM_BASE_ADDR_SOC       (0xBAD00000)
#define SBL_C7X2_L1DMEM_BASE_ADDR_SOC       (0xBAD00000)

#define SBL_M4F_IRAM_BASE                   (0xBAD00000)
#define SBL_M4F_IRAM_SIZE                   (0xBAD00000)
#define SBL_M4F_DRAM_BASE                   (0xBAD00000)
#define SBL_M4F_DRAM_SIZE                   (0xBAD00000)

#define SBL_M4F_IRAM_BASE_ADDR_SOC          (0xBAD00000)
#define SBL_M4F_DRAM_BASE_ADDR_SOC          (0xBAD00000)

#define SBL_UART_PLL_BASE                   (0xBAD00000)
#define SBL_UART_PLL_KICK0_OFFSET           (0xBAD00000)
#define SBL_UART_PLL_KICK0_OFFSET           (0xBAD00000)
#define SBL_UART_PLL_DIV_OFFSET             (0xBAD00000)
#define SBL_UART_PLL_DIV_VAL                (0xBAD00000)
#define SBL_UART_PLL_KICK0_UNLOCK_VAL       (0xBAD00000)
#define SBL_UART_PLL_KICK1_UNLOCK_VAL       (0xBAD00000)
#define SBL_UART_PLL_KICK_LOCK_VAL          (0xBAD00000)

#define SBL_ROM_UART_MODULE_INPUT_CLK       (48000000U)
#define SBL_SYSFW_UART_MODULE_INPUT_CLK     (96000000U)

#define SBL_MCU_UART_PADCONFIG_PULLUDEN_SHIFT       (CSL_WKUP_CTRL_MMR_CFG0_PADCONFIG18_PULLUDEN_SHIFT)
#define SBL_MCU_UART_PADCONFIG_PULLTYPESEL_SHIFT    (CSL_WKUP_CTRL_MMR_CFG0_PADCONFIG18_PULLTYPESEL_SHIFT)
#define SBL_MCU_UART_PADCONFIG_SLEWRATE_SHIFT       (CSL_WKUP_CTRL_MMR_CFG0_PADCONFIG18_SLEWRATE_SHIFT)
#define SBL_MCU_UART_PADCONFIG_RXACTIVE_SHIFT       (CSL_WKUP_CTRL_MMR_CFG0_PADCONFIG18_RXACTIVE_SHIFT)
#define SBL_MCU_UART_PADCONFIG_TX_DIS_SHIFT         (CSL_WKUP_CTRL_MMR_CFG0_PADCONFIG18_TX_DIS_SHIFT)
#define SBL_MCU_UART_PADCONFIG_MUXMODE_SHIFT        (CSL_WKUP_CTRL_MMR_CFG0_PADCONFIG18_MUXMODE_SHIFT)
#define SBL_MCU_UART_PADCONFIG_ADDR                 (CSL_WKUP_CTRL_MMR0_CFG0_BASE + CSL_WKUP_CTRL_MMR_CFG0_PADCONFIG18)

#define SBL_SYSFW_UART_PADCONFIG_PULLUDEN_SHIFT       (CSL_WKUP_CTRL_MMR_CFG0_PADCONFIG41_PULLUDEN_SHIFT)
#define SBL_SYSFW_UART_PADCONFIG_PULLTYPESEL_SHIFT    (CSL_WKUP_CTRL_MMR_CFG0_PADCONFIG41_PULLTYPESEL_SHIFT)
#define SBL_SYSFW_UART_PADCONFIG_SLEWRATE_SHIFT       (CSL_WKUP_CTRL_MMR_CFG0_PADCONFIG41_SLEWRATE_SHIFT)
#define SBL_SYSFW_UART_PADCONFIG_RXACTIVE_SHIFT       (CSL_WKUP_CTRL_MMR_CFG0_PADCONFIG41_RXACTIVE_SHIFT)
#define SBL_SYSFW_UART_PADCONFIG_TX_DIS_SHIFT         (CSL_WKUP_CTRL_MMR_CFG0_PADCONFIG41_TX_DIS_SHIFT)
#define SBL_SYSFW_UART_PADCONFIG_MUXMODE_SHIFT        (CSL_WKUP_CTRL_MMR_CFG0_PADCONFIG41_MUXMODE_SHIFT)
#define SBL_SYSFW_UART_PADCONFIG_ADDR                 (CSL_WKUP_CTRL_MMR0_CFG0_BASE + CSL_WKUP_CTRL_MMR_CFG0_PADCONFIG41)

#define SBL_VTM_CFG_BASE             (CSL_WKUP_VTM0_CFG0_BASE)
#define SBL_VTM_OPP_VID_MASK         (CSL_VTM_CFG1_OPPVID_OPP_LOW_DFLT_MASK)

#define SBL_DEV_ID_OSPI0            (TISCI_DEV_MCU_FSS0_OSPI_0)
#define SBL_CLK_ID_OSPI0            (TISCI_DEV_MCU_FSS0_OSPI_0_BUS_OSPI_RCLK_CLK)

#define SBL_DEV_ID_OSPI1            (TISCI_DEV_MCU_FSS0_OSPI_1)
#define SBL_CLK_ID_OSPI1            (TISCI_DEV_MCU_FSS0_OSPI_1_BUS_OSPI_PCLK_CLK)

#define SBL_DEV_ID_RTI0             (TISCI_DEV_MCU_RTI0)
#define SBL_DEV_ID_RTI1             (TISCI_DEV_MCU_RTI1)

#define SBL_DEV_ID_MPU_CLUSTER0     (TISCI_DEV_COMPUTE_CLUSTER_CPAC0)
#define SBL_DEV_ID_MPU_CLUSTER1     (TISCI_DEV_COMPUTE_CLUSTER_CPAC1)

#define SBL_PROC_ID_MPU1_CPU0       (SCICLIENT_PROCID_A53_CL0_C0)
#define SBL_DEV_ID_MPU1_CPU0        (TISCI_DEV_COMPUTE_CLUSTER_A53_0)
#define SBL_CLK_ID_MPU1_CPU0        (TISCI_DEV_COMPUTE_CLUSTER_A53_0_BUS_ARM0_CLK)
#define SBL_MPU1_CPU0_FREQ_HZ       (800000000)

#define SBL_PROC_ID_MPU1_CPU1       (SCICLIENT_PROCID_A53_CL0_C1)
#define SBL_DEV_ID_MPU1_CPU1        (TISCI_DEV_COMPUTE_CLUSTER_A53_1)
#define SBL_CLK_ID_MPU1_CPU1        (TISCI_DEV_COMPUTE_CLUSTER_A53_1_BUS_ARM0_CLK)
#define SBL_MPU1_CPU1_FREQ_HZ       (800000000)

#define SBL_PROC_ID_MPU2_CPU0       (SCICLIENT_PROCID_A53_CL1_C0)
#define SBL_DEV_ID_MPU2_CPU0        (TISCI_DEV_COMPUTE_CLUSTER_A53_2)
#define SBL_CLK_ID_MPU2_CPU0        (TISCI_DEV_COMPUTE_CLUSTER_A53_2_BUS_ARM1_CLK)
#define SBL_MPU2_CPU0_FREQ_HZ       (800000000)

#define SBL_PROC_ID_MPU2_CPU1       (SCICLIENT_PROCID_A53_CL1_C1)
#define SBL_DEV_ID_MPU2_CPU1        (TISCI_DEV_COMPUTE_CLUSTER_A53_3)
#define SBL_CLK_ID_MPU2_CPU1        (TISCI_DEV_COMPUTE_CLUSTER_A53_3_BUS_ARM1_CLK)
#define SBL_MPU2_CPU1_FREQ_HZ       (800000000)

#define SBL_PROC_ID_MCU1_CPU0       (SCICLIENT_PROCID_R5_CL0_C0)
#define SBL_DEV_ID_MCU1_CPU0        (TISCI_DEV_MCU_ARMSS0_CPU0)
#define SBL_CLK_ID_MCU1_CPU0        (TISCI_DEV_MCU_ARMSS0_CPU0_BUS_CPU_CLK)
#define SBL_MCU1_CPU0_FREQ_HZ       (400000000)

#define SBL_PROC_ID_MCU1_CPU1       (SCICLIENT_PROCID_R5_CL0_C1)
#define SBL_DEV_ID_MCU1_CPU1        (TISCI_DEV_MCU_ARMSS0_CPU1)
#define SBL_CLK_ID_MCU1_CPU1        (TISCI_DEV_MCU_ARMSS0_CPU1_BUS_CPU_CLK)
#define SBL_MCU1_CPU1_FREQ_HZ       (400000000)

#define SBL_PROC_ID_MCU2_CPU0       (0xBAD00000)
#define SBL_DEV_ID_MCU2_CPU0        (0xBAD00000)
#define SBL_CLK_ID_MCU2_CPU0        (0xBAD00000)
#define SBL_MCU2_CPU0_FREQ_HZ       (0xBAD00000)

#define SBL_PROC_ID_MCU2_CPU1       (0xBAD00000)
#define SBL_DEV_ID_MCU2_CPU1        (0xBAD00000)
#define SBL_CLK_ID_MCU2_CPU1        (0xBAD00000)
#define SBL_MCU2_CPU1_FREQ_HZ       (0xBAD00000)

#define SBL_PROC_ID_MCU3_CPU0       (0xBAD00000)
#define SBL_DEV_ID_MCU3_CPU0        (0xBAD00000)
#define SBL_CLK_ID_MCU3_CPU0        (0xBAD00000)
#define SBL_MCU3_CPU0_FREQ_HZ       (0xBAD00000)

#define SBL_PROC_ID_MCU3_CPU1       (0xBAD00000)
#define SBL_DEV_ID_MCU3_CPU1        (0xBAD00000)
#define SBL_CLK_ID_MCU3_CPU1        (0xBAD00000)
#define SBL_MCU3_CPU1_FREQ_HZ       (0xBAD00000)

#define SBL_PROC_ID_DSP1_C66X       (0xBAD00000)
#define SBL_DEV_ID_DSP1_C66X        (0xBAD00000)
#define SBL_CLK_ID_DSP1_C66X        (0xBAD00000)
#define SBL_DSP1_C66X_FREQ_HZ       (0xBAD00000)

#define SBL_PROC_ID_DSP2_C66X       (0xBAD00000)
#define SBL_DEV_ID_DSP2_C66X        (0xBAD00000)
#define SBL_CLK_ID_DSP2_C66X        (0xBAD00000)
#define SBL_DSP2_C66X_FREQ_HZ       (0xBAD00000)

#define SBL_PROC_ID_DSP1_C7X        (0xBAD00000)
#define SBL_DEV_ID_DSP1_C7X         (0xBAD00000)
#define SBL_CLK_ID_DSP1_C7X         (0xBAD00000)
#define SBL_DSP1_C7X_FREQ_HZ        (0xBAD00000)

#define SBL_PROC_ID_DSP2_C7X        (0xBAD00000)
#define SBL_DEV_ID_DSP2_C7X         (0xBAD00000)
#define SBL_CLK_ID_DSP2_C7X         (0xBAD00000)
#define SBL_DSP2_C7X_FREQ_HZ        (0xBAD00000)

#define SBL_PROC_ID_M4F_CPU0        (0xBAD00000)
#define SBL_DEV_ID_M4F_CPU0         (0xBAD00000)
#define SBL_CLK_ID_M4F_CPU0         (0xBAD00000)
#define SBL_M4F_CPU0_FREQ_HZ        (0xBAD00000)

#define SBL_M4F_IRAM_BASE_ADDR_SOC  (0xBAD00000)
#define SBL_M4F_DRAM_BASE_ADDR_SOC  (0xBAD00000)


#if defined (SBL_ENABLE_DEV_GRP_MCU)
#define SBL_PLL_INIT                (BOARD_INIT_PLL)
#define SBL_CLOCK_INIT              (BOARD_INIT_MODULE_CLOCK)
#define SBL_DEVGRP                  (DEVGRP_ALL)
#else
#define SBL_PLL_INIT                (BOARD_INIT_PLL)
#define SBL_CLOCK_INIT              (BOARD_INIT_MODULE_CLOCK)
#define SBL_DEVGRP                  (DEVGRP_ALL)
#endif

#endif

#if defined (SOC_J721E)

#define SBL_MCU_ATCM_BASE      (CSL_MCU_ARMSS_ATCM_BASE)
#define SBL_MCU_ATCM_SIZE      (CSL_MCU_ARMSS_ATCM_SIZE)
#define SBL_MCU_BTCM_BASE      (CSL_MCU_ARMSS_BTCM_BASE)
#define SBL_MCU_BTCM_SIZE      (CSL_MCU_ARMSS_BTCM_SIZE)

#define SBL_MCU1_CPU1_ATCM_BASE_ADDR_SOC    (CSL_MCU_R5FSS0_CORE1_ATCM_BASE)
#define SBL_MCU2_CPU0_ATCM_BASE_ADDR_SOC    (CSL_R5FSS0_CORE0_ATCM_BASE)
#define SBL_MCU2_CPU1_ATCM_BASE_ADDR_SOC    (CSL_R5FSS0_CORE1_ATCM_BASE)
#define SBL_MCU3_CPU0_ATCM_BASE_ADDR_SOC    (CSL_R5FSS1_CORE0_ATCM_BASE)
#define SBL_MCU3_CPU1_ATCM_BASE_ADDR_SOC    (CSL_R5FSS1_CORE1_ATCM_BASE)

#define SBL_MCU1_CPU1_BTCM_BASE_ADDR_SOC    (CSL_MCU_R5FSS0_CORE1_BTCM_BASE)
#define SBL_MCU2_CPU0_BTCM_BASE_ADDR_SOC    (CSL_R5FSS0_CORE0_BTCM_BASE)
#define SBL_MCU2_CPU1_BTCM_BASE_ADDR_SOC    (CSL_R5FSS0_CORE1_BTCM_BASE)
#define SBL_MCU3_CPU0_BTCM_BASE_ADDR_SOC    (CSL_R5FSS1_CORE0_BTCM_BASE)
#define SBL_MCU3_CPU1_BTCM_BASE_ADDR_SOC    (CSL_R5FSS1_CORE1_BTCM_BASE)

#define SBL_C66X_L2SRAM_BASE                (CSL_C66_COREPAC_L2_BASE)
#define SBL_C66X_L2SRAM_SIZE                (CSL_C66_COREPAC_L2_SIZE)
#define SBL_C66X_L1DMEM_BASE                (CSL_C66_COREPAC_L1D_BASE)
#define SBL_C66X_L1DMEM_SIZE                (CSL_C66_COREPAC_L1D_BASE)

#define SBL_C66X1_L2SRAM_BASE_ADDR_SOC      (0xBAD00000)
#define SBL_C66X2_L2SRAM_BASE_ADDR_SOC      (0xBAD00000)

#define SBL_C66X1_L1DMEM_BASE_ADDR_SOC      (0xBAD00000)
#define SBL_C66X2_L1DMEM_BASE_ADDR_SOC      (0xBAD00000)

#define SBL_C7X_L2SRAM_BASE                 (0xBAD00000)
#define SBL_C7X_L2SRAM_SIZE                 (0xBAD00000)
#define SBL_C7X_L1DMEM_BASE                 (0xBAD00000)
#define SBL_C7X_L1DMEM_SIZE                 (0xBAD00000)

#define SBL_C7X1_L2SRAM_BASE_ADDR_SOC       (0xBAD00000)
#define SBL_C7X2_L2SRAM_BASE_ADDR_SOC       (0xBAD00000)

#define SBL_C7X1_L1DMEM_BASE_ADDR_SOC       (0xBAD00000)
#define SBL_C7X2_L1DMEM_BASE_ADDR_SOC       (0xBAD00000)

#define SBL_M4F_IRAM_BASE                   (0xBAD00000)
#define SBL_M4F_IRAM_SIZE                   (0xBAD00000)
#define SBL_M4F_DRAM_BASE                   (0xBAD00000)
#define SBL_M4F_DRAM_SIZE                   (0xBAD00000)

#define SBL_M4F_IRAM_BASE_ADDR_SOC          (0xBAD00000)
#define SBL_M4F_DRAM_BASE_ADDR_SOC          (0xBAD00000)

#define SBL_UART_PLL_BASE                   (CSL_MCU_PLL0_CFG_BASE)
#define SBL_UART_PLL_KICK0_OFFSET           (CSL_MCU_PLL_MMR_CFG_PLL1_LOCKKEY0)
#define SBL_UART_PLL_KICK1_OFFSET           (CSL_MCU_PLL_MMR_CFG_PLL1_LOCKKEY1)
#define SBL_UART_PLL_DIV_OFFSET             (CSL_MCU_PLL_MMR_CFG_PLL1_HSDIV_CTRL3)
#define SBL_UART_PLL_DIV_VAL                (0x00008031)
#define SBL_UART_PLL_KICK0_UNLOCK_VAL       (0x68EF3490)
#define SBL_UART_PLL_KICK1_UNLOCK_VAL       (0xD172BC5A)
#define SBL_UART_PLL_KICK_LOCK_VAL          (0x0)
#define SBL_ROM_UART_MODULE_INPUT_CLK       (48000000U)
#define SBL_SYSFW_UART_MODULE_INPUT_CLK     (96000000U)

#define SBL_MCU_UART_PADCONFIG_PULLUDEN_SHIFT       (CSL_WKUP_CTRL_MMR_CFG0_PADCONFIG18_PULLUDEN_SHIFT)
#define SBL_MCU_UART_PADCONFIG_PULLTYPESEL_SHIFT    (CSL_WKUP_CTRL_MMR_CFG0_PADCONFIG18_PULLTYPESEL_SHIFT)
#define SBL_MCU_UART_PADCONFIG_RXACTIVE_SHIFT       (CSL_WKUP_CTRL_MMR_CFG0_PADCONFIG18_RXACTIVE_SHIFT)
#define SBL_MCU_UART_PADCONFIG_TX_DIS_SHIFT         (CSL_WKUP_CTRL_MMR_CFG0_PADCONFIG18_TX_DIS_SHIFT)
#define SBL_MCU_UART_PADCONFIG_MUXMODE_SHIFT        (CSL_WKUP_CTRL_MMR_CFG0_PADCONFIG18_MUXMODE_SHIFT)
#define SBL_MCU_UART_PADCONFIG_ADDR                 (CSL_WKUP_CTRL_MMR0_CFG0_BASE + CSL_WKUP_CTRL_MMR_CFG0_PADCONFIG18)

#define SBL_SYSFW_UART_PADCONFIG_PULLUDEN_SHIFT       (CSL_WKUP_CTRL_MMR_CFG0_PADCONFIG56_PULLUDEN_SHIFT)
#define SBL_SYSFW_UART_PADCONFIG_PULLTYPESEL_SHIFT    (CSL_WKUP_CTRL_MMR_CFG0_PADCONFIG56_PULLTYPESEL_SHIFT)
#define SBL_SYSFW_UART_PADCONFIG_RXACTIVE_SHIFT       (CSL_WKUP_CTRL_MMR_CFG0_PADCONFIG56_RXACTIVE_SHIFT)
#define SBL_SYSFW_UART_PADCONFIG_TX_DIS_SHIFT         (CSL_WKUP_CTRL_MMR_CFG0_PADCONFIG56_TX_DIS_SHIFT)
#define SBL_SYSFW_UART_PADCONFIG_MUXMODE_SHIFT        (CSL_WKUP_CTRL_MMR_CFG0_PADCONFIG56_MUXMODE_SHIFT)
#define SBL_SYSFW_UART_PADCONFIG_ADDR                 (CSL_WKUP_CTRL_MMR0_CFG0_BASE + CSL_WKUP_CTRL_MMR_CFG0_PADCONFIG56)

#define SBL_VTM_CFG_BASE             (CSL_WKUP_VTM0_MMR_VBUSP_CFG1_BASE)
#define SBL_VTM_OPP_VID_MASK         (CSL_VTM_CFG1_OPPVID_OPP_LOW_DFLT_MASK)

#define SBL_DEV_ID_OSPI0            (TISCI_DEV_MCU_FSS0_OSPI_0)
#define SBL_CLK_ID_OSPI0            (TISCI_DEV_MCU_FSS0_OSPI_0_OSPI_RCLK_CLK)

#define SBL_DEV_ID_OSPI1            (TISCI_DEV_MCU_FSS0_OSPI_1)
#define SBL_CLK_ID_OSPI1            (TISCI_DEV_MCU_FSS0_OSPI_1_OSPI_RCLK_CLK)

#define SBL_DEV_ID_RTI0             (TISCI_DEV_MCU_RTI0)
#define SBL_DEV_ID_RTI1             (TISCI_DEV_MCU_RTI1)

#define SBL_DEV_ID_MPU_CLUSTER0     (TISCI_DEV_A72SS0)
#define SBL_DEV_ID_MPU_CLUSTER1     (0xBAD00000)

#define SBL_PROC_ID_MPU1_CPU0       (SCICLIENT_PROC_ID_COMPUTE_CLUSTER_J7ES_TB_VDC_MAIN_0_P0_A72_DUAL_1MB)
#define SBL_DEV_ID_MPU1_CPU0        (TISCI_DEV_A72SS0_CORE0)
#define SBL_CLK_ID_MPU1_CPU0        (TISCI_DEV_A72SS0_CORE0_ARM_CLK_CLK)
#define SBL_MPU1_CPU0_FREQ_HZ       (2000000000)

#define SBL_PROC_ID_MPU1_CPU1       (SCICLIENT_PROC_ID_COMPUTE_CLUSTER_J7ES_TB_VDC_MAIN_0_P0_A72_DUAL_1MB_1)
#define SBL_DEV_ID_MPU1_CPU1        (TISCI_DEV_A72SS0_CORE1)
#define SBL_CLK_ID_MPU1_CPU1        (TISCI_DEV_A72SS0_CORE1_ARM_CLK_CLK)

#define SBL_MPU1_CPU1_FREQ_HZ       (2000000000)

#define SBL_PROC_ID_MPU2_CPU0       (0xBAD00000)
#define SBL_DEV_ID_MPU2_CPU0        (0xBAD00000)
#define SBL_CLK_ID_MPU2_CPU0        (0xBAD00000)
#define SBL_MPU2_CPU0_FREQ_HZ       (0xBAD00000)

#define SBL_PROC_ID_MPU2_CPU1       (0xBAD00000)
#define SBL_DEV_ID_MPU2_CPU1        (0xBAD00000)
#define SBL_CLK_ID_MPU2_CPU1        (0xBAD00000)
#define SBL_MPU2_CPU1_FREQ_HZ       (0xBAD00000)

#define SBL_PROC_ID_MCU1_CPU0       (SCICLIENT_PROC_ID_PULSAR_SL_MCU_0_CPU0)
#define SBL_DEV_ID_MCU1_CPU0        (TISCI_DEV_MCU_R5FSS0_CORE0)
#define SBL_CLK_ID_MCU1_CPU0        (TISCI_DEV_MCU_R5FSS0_CORE0_CPU_CLK)
#define SBL_MCU1_CPU0_FREQ_HZ       (1000000000)

#define SBL_PROC_ID_MCU1_CPU1       (SCICLIENT_PROC_ID_PULSAR_SL_MCU_0_CPU1)
#define SBL_DEV_ID_MCU1_CPU1        (TISCI_DEV_MCU_R5FSS0_CORE1)
#define SBL_CLK_ID_MCU1_CPU1        (TISCI_DEV_MCU_R5FSS0_CORE1_CPU_CLK)
#define SBL_MCU1_CPU1_FREQ_HZ       (1000000000)

#define SBL_PROC_ID_MCU2_CPU0       (SCICLIENT_PROC_ID_PULSAR_SL_MAIN_0_CPU0)
#define SBL_DEV_ID_MCU2_CPU0        (TISCI_DEV_R5FSS0_CORE0)
#define SBL_CLK_ID_MCU2_CPU0        (TISCI_DEV_R5FSS0_CORE0_CPU_CLK)
#define SBL_MCU2_CPU0_FREQ_HZ       (1000000000)

#define SBL_PROC_ID_MCU2_CPU1       (SCICLIENT_PROC_ID_PULSAR_SL_MAIN_0_CPU1)
#define SBL_DEV_ID_MCU2_CPU1        (TISCI_DEV_R5FSS0_CORE1)
#define SBL_CLK_ID_MCU2_CPU1        (TISCI_DEV_R5FSS0_CORE1_CPU_CLK)
#define SBL_MCU2_CPU1_FREQ_HZ       (1000000000)

#define SBL_PROC_ID_MCU3_CPU0       (SCICLIENT_PROC_ID_PULSAR_SL_MAIN_1_CPU0)
#define SBL_DEV_ID_MCU3_CPU0        (TISCI_DEV_R5FSS1_CORE0)
#define SBL_CLK_ID_MCU3_CPU0        (TISCI_DEV_R5FSS1_CORE0_CPU_CLK)
#define SBL_MCU3_CPU0_FREQ_HZ       (1000000000)

#define SBL_PROC_ID_MCU3_CPU1       (SCICLIENT_PROC_ID_PULSAR_SL_MAIN_1_CPU1)
#define SBL_DEV_ID_MCU3_CPU1        (TISCI_DEV_R5FSS1_CORE1)
#define SBL_CLK_ID_MCU3_CPU1        (TISCI_DEV_R5FSS1_CORE1_CPU_CLK)
#define SBL_MCU3_CPU1_FREQ_HZ       (1000000000)

#define SBL_PROC_ID_DSP1_C66X       (SCICLIENT_PROC_ID_K3_C66_COREPAC_MAIN_0_K3_C66_CGEM_288KB_0)
#define SBL_DEV_ID_DSP1_C66X        (TISCI_DEV_C66SS0_CORE0)
#define SBL_CLK_ID_DSP1_C66X        (TISCI_DEV_C66SS0_CORE0_GEM_TRC_CLK)
#define SBL_DSP1_C66X_FREQ_HZ       (1350000000)

#define SBL_PROC_ID_DSP2_C66X       (SCICLIENT_PROC_ID_K3_C66_COREPAC_MAIN_1_K3_C66_CGEM_288KB_0)
#define SBL_DEV_ID_DSP2_C66X        (TISCI_DEV_C66SS1_CORE0)
#define SBL_CLK_ID_DSP2_C66X        (TISCI_DEV_C66SS0_CORE0_GEM_TRC_CLK)
#define SBL_DSP2_C66X_FREQ_HZ       (1350000000)

#define SBL_PROC_ID_DSP1_C7X        (SCICLIENT_PROC_ID_COMPUTE_CLUSTER_J7ES_TB_VDC_MAIN_0_P4_C711_512KB)
#define SBL_DEV_ID_DSP1_C7X         (TISCI_DEV_C71SS0)
#define SBL_CLK_ID_DSP1_C7X         (TISCI_DEV_C71SS0_C7X_CLK)
#define SBL_DSP1_C7X_FREQ_HZ        (1000000000)

#define SBL_PROC_ID_DSP2_C7X        (0xBAD00000)
#define SBL_DEV_ID_DSP2_C7X         (0xBAD00000)
#define SBL_CLK_ID_DSP2_C7X         (0xBAD00000)
#define SBL_DSP2_C7X_FREQ_HZ        (0xBAD00000)

#define SBL_PROC_ID_M4F_CPU0        (0xBAD00000)
#define SBL_DEV_ID_M4F_CPU0         (0xBAD00000)
#define SBL_CLK_ID_M4F_CPU0         (0xBAD00000)
#define SBL_M4F_CPU0_FREQ_HZ        (0xBAD00000)

#if defined (SBL_ENABLE_DEV_GRP_MCU)
#define SBL_PLL_INIT                (BOARD_INIT_PLL_MCU)
#define SBL_CLOCK_INIT              (BOARD_INIT_MODULE_CLOCK_MCU)
#define SBL_DEVGRP                  (DEVGRP_00) /* MCU-wakeup DEVGRP */
#else
#define SBL_PLL_INIT                (BOARD_INIT_PLL)
#define SBL_CLOCK_INIT              (BOARD_INIT_MODULE_CLOCK)
#define SBL_DEVGRP                  (DEVGRP_ALL)
#endif

#define SBL_HYPERFLASH_BASE_ADDRESS      (CSL_MCU_FSS0_DAT_REG1_BASE)
#define SBL_HYPERFLASH_CTLR_BASE_ADDRESS (CSL_FSS0_HPB_CTRL_BASE)

#endif


#if defined (SOC_AM64X)

#define SBL_MCU_ATCM_BASE      (CSL_R5FSS0_ATCM_BASE)
#define SBL_MCU_ATCM_SIZE      (CSL_R5FSS0_ATCM_SIZE)
#define SBL_MCU_BTCM_BASE      (CSL_R5FSS0_BTCM_BASE)
#define SBL_MCU_BTCM_SIZE      (CSL_R5FSS0_BTCM_SIZE)

#define SBL_MCU1_CPU1_ATCM_BASE_ADDR_SOC    (CSL_R5FSS0_CORE1_ATCM_BASE)
#define SBL_MCU2_CPU0_ATCM_BASE_ADDR_SOC    (CSL_R5FSS1_CORE0_ATCM_BASE)
#define SBL_MCU2_CPU1_ATCM_BASE_ADDR_SOC    (CSL_R5FSS1_CORE1_ATCM_BASE)
#define SBL_MCU3_CPU0_ATCM_BASE_ADDR_SOC    (0xBAD00000)
#define SBL_MCU3_CPU1_ATCM_BASE_ADDR_SOC    (0xBAD00000)

#define SBL_MCU1_CPU1_BTCM_BASE_ADDR_SOC    (CSL_R5FSS0_CORE1_BTCM_BASE)
#define SBL_MCU2_CPU0_BTCM_BASE_ADDR_SOC    (CSL_R5FSS1_CORE0_BTCM_BASE)
#define SBL_MCU2_CPU1_BTCM_BASE_ADDR_SOC    (CSL_R5FSS1_CORE1_BTCM_BASE)
#define SBL_MCU3_CPU0_BTCM_BASE_ADDR_SOC    (0xBAD00000)
#define SBL_MCU3_CPU1_BTCM_BASE_ADDR_SOC    (0xBAD00000)

#define SBL_M4F_IRAM_BASE                   (0x00000000)
#define SBL_M4F_IRAM_SIZE                   (CSL_MCU_M4FSS0_IRAM_SIZE)
#define SBL_M4F_DRAM_BASE                   (0x00030000)
#define SBL_M4F_DRAM_SIZE                   (CSL_MCU_M4FSS0_DRAM_SIZE)

#define SBL_M4F_IRAM_BASE_ADDR_SOC          (CSL_MCU_M4FSS0_IRAM_BASE)
#define SBL_M4F_DRAM_BASE_ADDR_SOC          (CSL_MCU_M4FSS0_DRAM_BASE)

#define SBL_C66X_L2SRAM_BASE                (0xBAD00000)
#define SBL_C66X_L2SRAM_SIZE                (0xBAD00000)
#define SBL_C66X_L1DMEM_BASE                (0xBAD00000)
#define SBL_C66X_L1DMEM_SIZE                (0xBAD00000)

#define SBL_C66X1_L2SRAM_BASE_ADDR_SOC      (0xBAD00000)
#define SBL_C66X2_L2SRAM_BASE_ADDR_SOC      (0xBAD00000)

#define SBL_C66X1_L1DMEM_BASE_ADDR_SOC      (0xBAD00000)
#define SBL_C66X2_L1DMEM_BASE_ADDR_SOC      (0xBAD00000)

#define SBL_C7X_L2SRAM_BASE                 (0xBAD00000)
#define SBL_C7X_L2SRAM_SIZE                 (0xBAD00000)
#define SBL_C7X_L1DMEM_BASE                 (0xBAD00000)
#define SBL_C7X_L1DMEM_SIZE                 (0xBAD00000)

#define SBL_C7X1_L2SRAM_BASE_ADDR_SOC       (0xBAD00000)
#define SBL_C7X2_L2SRAM_BASE_ADDR_SOC       (0xBAD00000)

#define SBL_C7X1_L1DMEM_BASE_ADDR_SOC       (0xBAD00000)
#define SBL_C7X2_L1DMEM_BASE_ADDR_SOC       (0xBAD00000)



#define SBL_UART_PLL_BASE                   (CSL_MCU_PLL0_CFG_BASE)
#define SBL_UART_PLL_KICK0_OFFSET           (CSL_MCU_PLL_MMR_CFG_PLL1_LOCKKEY0)
#define SBL_UART_PLL_KICK1_OFFSET           (CSL_MCU_PLL_MMR_CFG_PLL1_LOCKKEY1)
#define SBL_UART_PLL_DIV_OFFSET             (CSL_MCU_PLL_MMR_CFG_PLL1_HSDIV_CTRL3)
#define SBL_UART_PLL_DIV_VAL                (0x00008031)
#define SBL_UART_PLL_KICK0_UNLOCK_VAL       (0x68EF3490)
#define SBL_UART_PLL_KICK1_UNLOCK_VAL       (0xD172BC5A)
#define SBL_UART_PLL_KICK_LOCK_VAL          (0x0)
#define SBL_ROM_UART_MODULE_INPUT_CLK       (3686400U)
#define SBL_SYSFW_UART_MODULE_INPUT_CLK     (3686400U)

#define SBL_MCU_UART_PADCONFIG_PULLUDEN_SHIFT       (CSL_MAIN_PADCFG_CTRL_MMR_CFG0_PADCONFIG141_PULLUDEN_SHIFT)
#define SBL_MCU_UART_PADCONFIG_PULLTYPESEL_SHIFT    (CSL_MAIN_PADCFG_CTRL_MMR_CFG0_PADCONFIG141_PULLTYPESEL_SHIFT)
#define SBL_MCU_UART_PADCONFIG_RXACTIVE_SHIFT       (CSL_MAIN_PADCFG_CTRL_MMR_CFG0_PADCONFIG141_RXACTIVE_SHIFT)
#define SBL_MCU_UART_PADCONFIG_TX_DIS_SHIFT         (CSL_MAIN_PADCFG_CTRL_MMR_CFG0_PADCONFIG141_TX_DIS_SHIFT)
#define SBL_MCU_UART_PADCONFIG_MUXMODE_SHIFT        (CSL_MAIN_PADCFG_CTRL_MMR_CFG0_PADCONFIG141_MUXMODE_SHIFT)
#define SBL_MCU_UART_PADCONFIG_ADDR                 (CSL_PADCFG_CTRL0_CFG0_BASE + CSL_MAIN_PADCFG_CTRL_MMR_CFG0_PADCONFIG141)

#define SBL_SYSFW_UART_PADCONFIG_PULLUDEN_SHIFT       (CSL_MAIN_PADCFG_CTRL_MMR_CFG0_PADCONFIG145_PULLUDEN_SHIFT)
#define SBL_SYSFW_UART_PADCONFIG_PULLTYPESEL_SHIFT    (CSL_MAIN_PADCFG_CTRL_MMR_CFG0_PADCONFIG145_PULLTYPESEL_SHIFT)
#define SBL_SYSFW_UART_PADCONFIG_RXACTIVE_SHIFT       (CSL_MAIN_PADCFG_CTRL_MMR_CFG0_PADCONFIG145_RXACTIVE_SHIFT)
#define SBL_SYSFW_UART_PADCONFIG_TX_DIS_SHIFT         (CSL_MAIN_PADCFG_CTRL_MMR_CFG0_PADCONFIG145_TX_DIS_SHIFT)
#define SBL_SYSFW_UART_PADCONFIG_MUXMODE_SHIFT        (CSL_MAIN_PADCFG_CTRL_MMR_CFG0_PADCONFIG145_MUXMODE_SHIFT)
#define SBL_SYSFW_UART_PADCONFIG_ADDR                 (CSL_PADCFG_CTRL0_CFG0_BASE + CSL_MAIN_PADCFG_CTRL_MMR_CFG0_PADCONFIG145)

#define SBL_VTM_CFG_BASE             (CSL_VTM0_MMR_VBUSP_CFG1_BASE)
#define SBL_VTM_OPP_VID_MASK         (CSL_VTM_CFG1_VTM_VD_OPPVID_OPP_0_MASK)

#define SBL_DEV_ID_OSPI0            (TISCI_DEV_FSS0_OSPI_0)
#define SBL_CLK_ID_OSPI0            (TISCI_DEV_FSS0_OSPI_0_OSPI_RCLK_CLK)

#define SBL_DEV_ID_RTI0             (TISCI_DEV_RTI0)
#define SBL_DEV_ID_RTI1             (TISCI_DEV_RTI1)
#define SBL_DEV_ID_RTI8             (TISCI_DEV_RTI8)
#define SBL_DEV_ID_RTI9             (TISCI_DEV_RTI9)
#define SBL_DEV_ID_RTI10            (TISCI_DEV_RTI10)
#define SBL_DEV_ID_RTI11            (TISCI_DEV_RTI11)
#define SBL_DEV_ID_MCU_RTI0         (TISCI_DEV_MCU_RTI0)

#define SBL_DEV_ID_MPU_CLUSTER0     (TISCI_DEV_A53SS0)
#define SBL_DEV_ID_MPU_CLUSTER1     (0xBAD00000)

#define SBL_DEV_ID_M4F_CLUSTER0     (TISCI_DEV_MCU_M4FSS0_CORE0)


#define SBL_PROC_ID_MPU1_CPU0       (SCICLIENT_PROCID_A53_CL0_C0) 
#define SBL_DEV_ID_MPU1_CPU0        (TISCI_DEV_A53SS0_CORE_0)
#define SBL_CLK_ID_MPU1_CPU0        (TISCI_DEV_A53SS0_COREPAC_ARM_CLK_CLK)
#define SBL_MPU1_CPU0_FREQ_HZ       (800000000)

#define SBL_PROC_ID_MPU1_CPU1       (SCICLIENT_PROCID_A53_CL0_C1) 
#define SBL_DEV_ID_MPU1_CPU1        (TISCI_DEV_A53SS0_CORE_1)
#define SBL_CLK_ID_MPU1_CPU1        (TISCI_DEV_A53SS0_COREPAC_ARM_CLK_CLK)
#define SBL_MPU1_CPU1_FREQ_HZ       (800000000)

#define SBL_PROC_ID_MPU2_CPU0       (0xBAD00000)
#define SBL_DEV_ID_MPU2_CPU0        (0xBAD00000)
#define SBL_CLK_ID_MPU2_CPU0        (0xBAD00000)
#define SBL_MPU2_CPU0_FREQ_HZ       (800000000)

#define SBL_PROC_ID_MPU2_CPU1       (0xBAD00000)
#define SBL_DEV_ID_MPU2_CPU1        (0xBAD00000)
#define SBL_CLK_ID_MPU2_CPU1        (0xBAD00000)
#define SBL_MPU2_CPU1_FREQ_HZ       (800000000)

#define SBL_PROC_ID_MCU1_CPU0       (SCICLIENT_PROCID_R5_CL0_C0)
#define SBL_DEV_ID_MCU1_CPU0        (TISCI_DEV_R5FSS0_CORE0)
#define SBL_CLK_ID_MCU1_CPU0        (TISCI_DEV_R5FSS0_CORE0_CPU_CLK)
#define SBL_MCU1_CPU0_FREQ_HZ       (800000000)

#define SBL_PROC_ID_MCU1_CPU1       (SCICLIENT_PROCID_R5_CL0_C1)
#define SBL_DEV_ID_MCU1_CPU1        (TISCI_DEV_R5FSS0_CORE1)
#define SBL_CLK_ID_MCU1_CPU1        (TISCI_DEV_R5FSS0_CORE1_CPU_CLK)
#define SBL_MCU1_CPU1_FREQ_HZ       (800000000)

#define SBL_PROC_ID_MCU2_CPU0       (SCICLIENT_PROCID_R5_CL1_C0)
#define SBL_DEV_ID_MCU2_CPU0        (TISCI_DEV_R5FSS1_CORE0)
#define SBL_CLK_ID_MCU2_CPU0        (TISCI_DEV_R5FSS1_CORE0_CPU_CLK)
#define SBL_MCU2_CPU0_FREQ_HZ       (1000000000)

#define SBL_PROC_ID_MCU2_CPU1       (SCICLIENT_PROCID_R5_CL1_C1)
#define SBL_DEV_ID_MCU2_CPU1        (TISCI_DEV_R5FSS1_CORE1)
#define SBL_CLK_ID_MCU2_CPU1        (TISCI_DEV_R5FSS1_CORE1_CPU_CLK)
#define SBL_MCU2_CPU1_FREQ_HZ       (1000000000)

#define SBL_PROC_ID_MCU3_CPU0       (0xBAD00000)
#define SBL_DEV_ID_MCU3_CPU0        (0xBAD00000)
#define SBL_CLK_ID_MCU3_CPU0        (0xBAD00000)
#define SBL_MCU3_CPU0_FREQ_HZ       (1000000000)

#define SBL_PROC_ID_MCU3_CPU1       (0xBAD00000)
#define SBL_DEV_ID_MCU3_CPU1        (0xBAD00000)
#define SBL_CLK_ID_MCU3_CPU1        (0xBAD00000)
#define SBL_MCU3_CPU1_FREQ_HZ       (1000000000)

#define SBL_PROC_ID_DSP1_C66X       (0xBAD00000)
#define SBL_DEV_ID_DSP1_C66X        (0xBAD00000)
#define SBL_CLK_ID_DSP1_C66X        (0xBAD00000)
#define SBL_DSP1_C66X_FREQ_HZ       (1350000000)

#define SBL_PROC_ID_DSP2_C66X       (0xBAD00000)
#define SBL_DEV_ID_DSP2_C66X        (0xBAD00000)
#define SBL_CLK_ID_DSP2_C66X        (0xBAD00000)
#define SBL_DSP2_C66X_FREQ_HZ       (1350000000)

#define SBL_PROC_ID_DSP1_C7X        (0xBAD00000)
#define SBL_DEV_ID_DSP1_C7X         (0xBAD00000)
#define SBL_CLK_ID_DSP1_C7X         (0xBAD00000)
#define SBL_DSP1_C7X_FREQ_HZ        (1000000000)

#define SBL_PROC_ID_DSP2_C7X        (0xBAD00000)
#define SBL_DEV_ID_DSP2_C7X         (0xBAD00000)
#define SBL_CLK_ID_DSP2_C7X         (0xBAD00000)
#define SBL_DSP2_C7X_FREQ_HZ        (0xBAD00000)

#define SBL_PROC_ID_M4F_CPU0       (SCICLIENT_PROCID_MCU_M4FSS0_C0)
#define SBL_DEV_ID_M4F_CPU0        (TISCI_DEV_MCU_M4FSS0_CORE0)
#define SBL_CLK_ID_M4F_CPU0        (TISCI_DEV_MCU_M4FSS0_CORE0_VBUS_CLK)
#define SBL_M4F_CPU0_FREQ_HZ       (400000000)


#if defined (SBL_ENABLE_DEV_GRP_MCU)
#define SBL_PLL_INIT                (BOARD_INIT_PLL_MCU)
#define SBL_CLOCK_INIT              (BOARD_INIT_MODULE_CLOCK_MCU)
#define SBL_DEVGRP                  (DEVGRP_00) /* MCU-wakeup DEVGRP */
#else
#define SBL_PLL_INIT                (BOARD_INIT_PLL)
#define SBL_CLOCK_INIT              (BOARD_INIT_MODULE_CLOCK)
#define SBL_DEVGRP                  (DEVGRP_ALL)
#endif

#define SBL_HYPERFLASH_BASE_ADDRESS      (CSL_MCU_FSS0_DAT_REG1_BASE)
#define SBL_HYPERFLASH_CTLR_BASE_ADDRESS (CSL_FSS0_HPB_CTRL_BASE)

#endif

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */
void SBL_RAT_Config(sblRatCfgInfo_t *remap_list);
void SBL_SocEarlyInit(void);
void SBL_SocLateInit(void);
#endif
