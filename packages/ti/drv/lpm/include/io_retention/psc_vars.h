/*
*
* Copyright (c) 2021 Texas Instruments Incorporated
*
* All rights reserved not granted herein.
*
* Limited License.
*
* Texas Instruments Incorporated grants a world-wide, royalty-free, non-exclusive
* license under copyrights and patents it now or hereafter owns or controls to make,
* have made, use, import, offer to sell and sell ("Utilize") this software subject to the
* terms herein.  With respect to the foregoing patent license, such license is granted
* solely to the extent that any such patent is necessary to Utilize the software alone.
* The patent license shall not apply to any combinations which include this software,
* other than combinations with devices manufactured by or for TI ("TI Devices").
* No hardware patent is licensed hereunder.
*
* Redistributions must preserve existing copyright notices and reproduce this license
* (including the above copyright notice and the disclaimer and (if applicable) source
* code license limitations below) in the documentation and/or other materials provided
* with the distribution
*
* Redistribution and use in binary form, without modification, are permitted provided
* that the following conditions are met:
*
* *       No reverse engineering, decompilation, or disassembly of this software is
* permitted with respect to any software provided in binary form.
*
* *       any redistribution and use are licensed by TI for use only with TI Devices.
*
* *       Nothing shall obligate TI to provide you with source code for the software
* licensed and provided to you in object code.
*
* If software source code is provided to you, modification and redistribution of the
* source code are permitted provided that the following conditions are met:
*
* *       any redistribution and use of the source code, including any resulting derivative
* works, are licensed by TI for use only with TI Devices.
*
* *       any redistribution and use of any object code compiled from the source code
* and any resulting derivative works, are licensed by TI for use only with TI Devices.
*
* Neither the name of Texas Instruments Incorporated nor the names of its suppliers
*
* may be used to endorse or promote products derived from this software without
* specific prior written permission.
*
* DISCLAIMER.
*
* THIS SOFTWARE IS PROVIDED BY TI AND TI'S LICENSORS "AS IS" AND ANY EXPRESS
* OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL TI AND TI'S LICENSORS BE LIABLE FOR ANY DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
* OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
* OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
* OF THE POSSIBILITY OF SUCH DAMAGE.
*
*/

#ifndef _PSC_VARS_H_
#define _PSC_VARS_H_

#include <stdint.h>
#include <ti/drv/lpm/include/io_retention/psc.h>

/* Module Domain : LPSC_WKUP_ALWAYSON  Power Domain : PD_WKUP */
#define LPSC_WKUP_ALWAYSON_PDCTL       ((WKUP_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + ( 0 * 4))
#define LPSC_WKUP_ALWAYSON_MDCTL       ((WKUP_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + ( 0 * 4))

/* Module Domain : LPSC_DMSC           Power Domain : PD_WKUP */
#define LPSC_DMSC_PDCTL                ((WKUP_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + ( 0 * 4))
#define LPSC_DMSC_MDCTL                ((WKUP_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + ( 1 * 4))

/* Module Domain : LPSC_DEBUG2DMSC     Power Domain : PD_WKUP */
#define LPSC_DEBUG2DMSC_PDCTL          ((WKUP_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + ( 0 * 4))
#define LPSC_DEBUG2DMSC_MDCTL          ((WKUP_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + ( 2 * 4))

/* Module Domain : LPSC_WKUP_GPIO      Power Domain : PD_WKUP */
#define LPSC_WKUP_GPIO_PDCTL           ((WKUP_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + ( 0 * 4))
#define LPSC_WKUP_GPIO_MDCTL           ((WKUP_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + ( 3 * 4))

/* Module Domain : LPSC_WKUPMCU2MAIN   Power Domain : PD_WKUP */
#define LPSC_WKUPMCU2MAIN_PDCTL        ((WKUP_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + ( 0 * 4))
#define LPSC_WKUPMCU2MAIN_MDCTL        ((WKUP_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + ( 4 * 4))

/* Module Domain : LPSC_MAIN2WKUPMCU   Power Domain : PD_WKUP */
#define LPSC_MAIN2WKUPMCU_PDCTL        ((WKUP_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + ( 0 * 4))
#define LPSC_MAIN2WKUPMCU_MDCTL        ((WKUP_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + ( 5 * 4))

/* Module Domain : LPSC_MCU_TEST       Power Domain : PD_WKUP */
#define LPSC_MCU_TEST_PDCTL            ((WKUP_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + ( 0 * 4))
#define LPSC_MCU_TEST_MDCTL            ((WKUP_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + ( 6 * 4))

/* Module Domain : LPSC_MCU_DEBUG      Power Domain : PD_WKUP */
#define LPSC_MCU_DEBUG_PDCTL           ((WKUP_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + ( 0 * 4))
#define LPSC_MCU_DEBUG_MDCTL           ((WKUP_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + ( 7 * 4))

/* Module Domain : LPSC_MCU_MCAN_0     Power Domain : PD_WKUP */
#define LPSC_MCU_MCAN_0_PDCTL          ((WKUP_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + ( 0 * 4))
#define LPSC_MCU_MCAN_0_MDCTL          ((WKUP_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + ( 8 * 4))

/* Module Domain : LPSC_MCU_MCAN_1     Power Domain : PD_WKUP */
#define LPSC_MCU_MCAN_1_PDCTL          ((WKUP_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + ( 0 * 4))
#define LPSC_MCU_MCAN_1_MDCTL          ((WKUP_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + ( 9 * 4))

/* Module Domain : LPSC_MCU_OSPI_0     Power Domain : PD_WKUP */
#define LPSC_MCU_OSPI_0_PDCTL          ((WKUP_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + ( 0 * 4))
#define LPSC_MCU_OSPI_0_MDCTL          ((WKUP_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + (10 * 4))

/* Module Domain : LPSC_MCU_OSPI_1     Power Domain : PD_WKUP */
#define LPSC_MCU_OSPI_1_PDCTL          ((WKUP_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + ( 0 * 4))
#define LPSC_MCU_OSPI_1_MDCTL          ((WKUP_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + (11 * 4))

/* Module Domain : LPSC_MCU_HYPERBUS   Power Domain : PD_WKUP */
#define LPSC_MCU_HYPERBUS_PDCTL        ((WKUP_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + ( 0 * 4))
#define LPSC_MCU_HYPERBUS_MDCTL        ((WKUP_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + (12 * 4))

/* Module Domain : LPSC_MCU_I3C_0      Power Domain : PD_WKUP */
#define LPSC_MCU_I3C_0_PDCTL           ((WKUP_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + ( 0 * 4))
#define LPSC_MCU_I3C_0_MDCTL           ((WKUP_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + (13 * 4))

/* Module Domain : LPSC_MCU_I3C_1      Power Domain : PD_WKUP */
#define LPSC_MCU_I3C_1_PDCTL           ((WKUP_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + ( 0 * 4))
#define LPSC_MCU_I3C_1_MDCTL           ((WKUP_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + (14 * 4))

/* Module Domain : LPSC_MCU_ADC_0      Power Domain : PD_WKUP */
#define LPSC_MCU_ADC_0_PDCTL           ((WKUP_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + ( 0 * 4))
#define LPSC_MCU_ADC_0_MDCTL           ((WKUP_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + (15 * 4))

/* Module Domain : LPSC_MCU_ADC_1      Power Domain : PD_WKUP */
#define LPSC_MCU_ADC_1_PDCTL           ((WKUP_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + ( 0 * 4))
#define LPSC_MCU_ADC_1_MDCTL           ((WKUP_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + (16 * 4))

/* Module Domain : LPSC_WKUP_SPARE0    Power Domain : PD_WKUP */
#define LPSC_WKUP_SPARE0_PDCTL         ((WKUP_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + ( 0 * 4))
#define LPSC_WKUP_SPARE0_MDCTL         ((WKUP_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + (17 * 4))

/* Module Domain : LPSC_WKUP_SPARE1    Power Domain : PD_WKUP */
#define LPSC_WKUP_SPARE1_PDCTL         ((WKUP_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + ( 0 * 4))
#define LPSC_WKUP_SPARE1_MDCTL         ((WKUP_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + (18 * 4))

/* Module Domain : LPSC_MCU_R5_0       Power Domain : PD_MCU_PULSAR */
#define LPSC_MCU_R5_0_PDCTL            ((WKUP_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + ( 1 * 4))
#define LPSC_MCU_R5_0_MDCTL            ((WKUP_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + (19 * 4))

/* Module Domain : LPSC_MCU_R5_1       Power Domain : PD_MCU_PULSAR */
#define LPSC_MCU_R5_1_PDCTL            ((WKUP_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + ( 1 * 4))
#define LPSC_MCU_R5_1_MDCTL            ((WKUP_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + (20 * 4))

/* Module Domain : LPSC_MCU_PULSAR_PBIST_0 Power Domain : PD_MCU_PULSAR */
#define LPSC_MCU_PULSAR_PBIST_0_PDCTL  ((WKUP_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + ( 1 * 4))
#define LPSC_MCU_PULSAR_PBIST_0_MDCTL  ((WKUP_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + (21 * 4))

/* Module Domain : LPSC_MAIN_ALWAYSON  Power Domain : GP_CORE_CTL */
#define LPSC_MAIN_ALWAYSON_PDCTL       ((MAIN_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + ( 0 * 4))
#define LPSC_MAIN_ALWAYSON_MDCTL       ((MAIN_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + ( 0 * 4))

/* Module Domain : LPSC_MAIN_TEST      Power Domain : GP_CORE_CTL */
#define LPSC_MAIN_TEST_PDCTL           ((MAIN_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + ( 0 * 4))
#define LPSC_MAIN_TEST_MDCTL           ((MAIN_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + ( 1 * 4))

/* Module Domain : LPSC_MAIN_PBIST     Power Domain : GP_CORE_CTL */
#define LPSC_MAIN_PBIST_PDCTL          ((MAIN_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + ( 0 * 4))
#define LPSC_MAIN_PBIST_MDCTL          ((MAIN_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + ( 2 * 4))

/* Module Domain : LPSC_PER_AUDIO      Power Domain : GP_CORE_CTL */
#define LPSC_PER_AUDIO_PDCTL           ((MAIN_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + ( 0 * 4))
#define LPSC_PER_AUDIO_MDCTL           ((MAIN_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + ( 3 * 4))

/* Module Domain : LPSC_PER_ATL        Power Domain : GP_CORE_CTL */
#define LPSC_PER_ATL_PDCTL             ((MAIN_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + ( 0 * 4))
#define LPSC_PER_ATL_MDCTL             ((MAIN_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + ( 4 * 4))

/* Module Domain : LPSC_PER_MLB        Power Domain : GP_CORE_CTL */
#define LPSC_PER_MLB_PDCTL             ((MAIN_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + ( 0 * 4))
#define LPSC_PER_MLB_MDCTL             ((MAIN_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + ( 5 * 4))

/* Module Domain : LPSC_PER_MOTOR      Power Domain : GP_CORE_CTL */
#define LPSC_PER_MOTOR_PDCTL           ((MAIN_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + ( 0 * 4))
#define LPSC_PER_MOTOR_MDCTL           ((MAIN_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + ( 6 * 4))

/* Module Domain : LPSC_PER_MISCIO     Power Domain : GP_CORE_CTL */
#define LPSC_PER_MISCIO_PDCTL          ((MAIN_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + ( 0 * 4))
#define LPSC_PER_MISCIO_MDCTL          ((MAIN_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + ( 7 * 4))

/* Module Domain : LPSC_PER_GPMC       Power Domain : GP_CORE_CTL */
#define LPSC_PER_GPMC_PDCTL            ((MAIN_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + ( 0 * 4))
#define LPSC_PER_GPMC_MDCTL            ((MAIN_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + ( 8 * 4))

/* Module Domain : LPSC_PER_VPFE       Power Domain : GP_CORE_CTL */
#define LPSC_PER_VPFE_PDCTL            ((MAIN_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + ( 0 * 4))
#define LPSC_PER_VPFE_MDCTL            ((MAIN_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + ( 9 * 4))

/* Module Domain : LPSC_PER_VPE        Power Domain : GP_CORE_CTL */
#define LPSC_PER_VPE_PDCTL             ((MAIN_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + ( 0 * 4))
#define LPSC_PER_VPE_MDCTL             ((MAIN_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + (10 * 4))

/* Module Domain : LPSC_PER_SPARE0     Power Domain : GP_CORE_CTL */
#define LPSC_PER_SPARE0_PDCTL          ((MAIN_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + ( 0 * 4))
#define LPSC_PER_SPARE0_MDCTL          ((MAIN_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + (11 * 4))

/* Module Domain : LPSC_PER_SPARE1     Power Domain : GP_CORE_CTL */
#define LPSC_PER_SPARE1_PDCTL          ((MAIN_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + ( 0 * 4))
#define LPSC_PER_SPARE1_MDCTL          ((MAIN_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + (12 * 4))

/* Module Domain : LPSC_MAIN_DEBUG     Power Domain : GP_CORE_CTL */
#define LPSC_MAIN_DEBUG_PDCTL          ((MAIN_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + ( 0 * 4))
#define LPSC_MAIN_DEBUG_MDCTL          ((MAIN_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + (13 * 4))

/* Module Domain : LPSC_EMIF_DATA_0    Power Domain : GP_CORE_CTL */
#define LPSC_EMIF_DATA_0_PDCTL         ((MAIN_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + ( 0 * 4))
#define LPSC_EMIF_DATA_0_MDCTL         ((MAIN_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + (14 * 4))

/* Module Domain : LPSC_EMIF_CFG_0     Power Domain : GP_CORE_CTL */
#define LPSC_EMIF_CFG_0_PDCTL          ((MAIN_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + ( 0 * 4))
#define LPSC_EMIF_CFG_0_MDCTL          ((MAIN_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + (15 * 4))

/* Module Domain : LPSC_EMIF_DATA_1    Power Domain : GP_CORE_CTL */
#define LPSC_EMIF_DATA_1_PDCTL         ((MAIN_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + ( 0 * 4))
#define LPSC_EMIF_DATA_1_MDCTL         ((MAIN_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + (16 * 4))

/* Module Domain : LPSC_EMIF_CFG_1     Power Domain : GP_CORE_CTL */
#define LPSC_EMIF_CFG_1_PDCTL          ((MAIN_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + ( 0 * 4))
#define LPSC_EMIF_CFG_1_MDCTL          ((MAIN_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + (17 * 4))

/* Module Domain : LPSC_PER_SPARE2     Power Domain : GP_CORE_CTL */
#define LPSC_PER_SPARE2_PDCTL          ((MAIN_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + ( 0 * 4))
#define LPSC_PER_SPARE2_MDCTL          ((MAIN_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + (18 * 4))

/* Module Domain : LPSC_CC_TOP_PBIST   Power Domain : GP_CORE_CTL */
#define LPSC_CC_TOP_PBIST_PDCTL        ((MAIN_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + ( 0 * 4))
#define LPSC_CC_TOP_PBIST_MDCTL        ((MAIN_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + (19 * 4))

/* Module Domain : LPSC_USB_0          Power Domain : GP_CORE_CTL */
#define LPSC_USB_0_PDCTL               ((MAIN_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + ( 0 * 4))
#define LPSC_USB_0_MDCTL               ((MAIN_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + (20 * 4))

/* Module Domain : LPSC_USB_1          Power Domain : GP_CORE_CTL */
#define LPSC_USB_1_PDCTL               ((MAIN_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + ( 0 * 4))
#define LPSC_USB_1_MDCTL               ((MAIN_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + (21 * 4))

/* Module Domain : LPSC_USB_2          Power Domain : GP_CORE_CTL */
#define LPSC_USB_2_PDCTL               ((MAIN_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + ( 0 * 4))
#define LPSC_USB_2_MDCTL               ((MAIN_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + (22 * 4))

/* Module Domain : LPSC_MMC4B_0        Power Domain : GP_CORE_CTL */
#define LPSC_MMC4B_0_PDCTL             ((MAIN_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + ( 0 * 4))
#define LPSC_MMC4B_0_MDCTL             ((MAIN_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + (23 * 4))

/* Module Domain : LPSC_MMC4B_1        Power Domain : GP_CORE_CTL */
#define LPSC_MMC4B_1_PDCTL             ((MAIN_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + ( 0 * 4))
#define LPSC_MMC4B_1_MDCTL             ((MAIN_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + (24 * 4))

/* Module Domain : LPSC_MMC8B_0        Power Domain : GP_CORE_CTL */
#define LPSC_MMC8B_0_PDCTL             ((MAIN_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + ( 0 * 4))
#define LPSC_MMC8B_0_MDCTL             ((MAIN_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + (25 * 4))

/* Module Domain : LPSC_UFS_0          Power Domain : GP_CORE_CTL */
#define LPSC_UFS_0_PDCTL               ((MAIN_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + ( 0 * 4))
#define LPSC_UFS_0_MDCTL               ((MAIN_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + (26 * 4))

/* Module Domain : LPSC_UFS_1          Power Domain : GP_CORE_CTL */
#define LPSC_UFS_1_PDCTL               ((MAIN_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + ( 0 * 4))
#define LPSC_UFS_1_MDCTL               ((MAIN_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + (27 * 4))

/* Module Domain : LPSC_PCIE_0         Power Domain : GP_CORE_CTL */
#define LPSC_PCIE_0_PDCTL              ((MAIN_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + ( 0 * 4))
#define LPSC_PCIE_0_MDCTL              ((MAIN_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + (28 * 4))

/* Module Domain : LPSC_PCIE_1         Power Domain : GP_CORE_CTL */
#define LPSC_PCIE_1_PDCTL              ((MAIN_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + ( 0 * 4))
#define LPSC_PCIE_1_MDCTL              ((MAIN_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + (29 * 4))

/* Module Domain : LPSC_PCIE_2         Power Domain : GP_CORE_CTL */
#define LPSC_PCIE_2_PDCTL              ((MAIN_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + ( 0 * 4))
#define LPSC_PCIE_2_MDCTL              ((MAIN_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + (30 * 4))

/* Module Domain : LPSC_PCIE_3         Power Domain : GP_CORE_CTL */
#define LPSC_PCIE_3_PDCTL              ((MAIN_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + ( 0 * 4))
#define LPSC_PCIE_3_MDCTL              ((MAIN_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + (31 * 4))

/* Module Domain : LPSC_SAUL           Power Domain : GP_CORE_CTL */
#define LPSC_SAUL_PDCTL                ((MAIN_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + ( 0 * 4))
#define LPSC_SAUL_MDCTL                ((MAIN_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + (32 * 4))

/* Module Domain : LPSC_PER_I3C        Power Domain : GP_CORE_CTL */
#define LPSC_PER_I3C_PDCTL             ((MAIN_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + ( 0 * 4))
#define LPSC_PER_I3C_MDCTL             ((MAIN_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + (33 * 4))

/* Module Domain : LPSC_MAIN_MCANSS_0  Power Domain : PD_MCANSS */
#define LPSC_MAIN_MCANSS_0_PDCTL       ((MAIN_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + ( 1 * 4))
#define LPSC_MAIN_MCANSS_0_MDCTL       ((MAIN_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + (34 * 4))

/* Module Domain : LPSC_MAIN_MCANSS_1  Power Domain : PD_MCANSS */
#define LPSC_MAIN_MCANSS_1_PDCTL       ((MAIN_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + ( 1 * 4))
#define LPSC_MAIN_MCANSS_1_MDCTL       ((MAIN_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + (35 * 4))

/* Module Domain : LPSC_MAIN_MCANSS_2  Power Domain : PD_MCANSS */
#define LPSC_MAIN_MCANSS_2_PDCTL       ((MAIN_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + ( 1 * 4))
#define LPSC_MAIN_MCANSS_2_MDCTL       ((MAIN_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + (36 * 4))

/* Module Domain : LPSC_MAIN_MCANSS_3  Power Domain : PD_MCANSS */
#define LPSC_MAIN_MCANSS_3_PDCTL       ((MAIN_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + ( 1 * 4))
#define LPSC_MAIN_MCANSS_3_MDCTL       ((MAIN_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + (37 * 4))

/* Module Domain : LPSC_MAIN_MCANSS_4  Power Domain : PD_MCANSS */
#define LPSC_MAIN_MCANSS_4_PDCTL       ((MAIN_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + ( 1 * 4))
#define LPSC_MAIN_MCANSS_4_MDCTL       ((MAIN_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + (38 * 4))

/* Module Domain : LPSC_MAIN_MCANSS_5  Power Domain : PD_MCANSS */
#define LPSC_MAIN_MCANSS_5_PDCTL       ((MAIN_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + ( 1 * 4))
#define LPSC_MAIN_MCANSS_5_MDCTL       ((MAIN_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + (39 * 4))

/* Module Domain : LPSC_MAIN_MCANSS_6  Power Domain : PD_MCANSS */
#define LPSC_MAIN_MCANSS_6_PDCTL       ((MAIN_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + ( 1 * 4))
#define LPSC_MAIN_MCANSS_6_MDCTL       ((MAIN_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + (40 * 4))

/* Module Domain : LPSC_MAIN_MCANSS_7  Power Domain : PD_MCANSS */
#define LPSC_MAIN_MCANSS_7_PDCTL       ((MAIN_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + ( 1 * 4))
#define LPSC_MAIN_MCANSS_7_MDCTL       ((MAIN_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + (41 * 4))

/* Module Domain : LPSC_MAIN_MCANSS_8  Power Domain : PD_MCANSS */
#define LPSC_MAIN_MCANSS_8_PDCTL       ((MAIN_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + ( 1 * 4))
#define LPSC_MAIN_MCANSS_8_MDCTL       ((MAIN_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + (42 * 4))

/* Module Domain : LPSC_MAIN_MCANSS_9  Power Domain : PD_MCANSS */
#define LPSC_MAIN_MCANSS_9_PDCTL       ((MAIN_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + ( 1 * 4))
#define LPSC_MAIN_MCANSS_9_MDCTL       ((MAIN_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + (43 * 4))

/* Module Domain : LPSC_MAIN_MCANSS_10 Power Domain : PD_MCANSS */
#define LPSC_MAIN_MCANSS_10_PDCTL      ((MAIN_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + ( 1 * 4))
#define LPSC_MAIN_MCANSS_10_MDCTL      ((MAIN_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + (44 * 4))

/* Module Domain : LPSC_MAIN_MCANSS_11 Power Domain : PD_MCANSS */
#define LPSC_MAIN_MCANSS_11_PDCTL      ((MAIN_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + ( 1 * 4))
#define LPSC_MAIN_MCANSS_11_MDCTL      ((MAIN_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + (45 * 4))

/* Module Domain : LPSC_MAIN_MCANSS_12 Power Domain : PD_MCANSS */
#define LPSC_MAIN_MCANSS_12_PDCTL      ((MAIN_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + ( 1 * 4))
#define LPSC_MAIN_MCANSS_12_MDCTL      ((MAIN_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + (46 * 4))

/* Module Domain : LPSC_MAIN_MCANSS_13 Power Domain : PD_MCANSS */
#define LPSC_MAIN_MCANSS_13_PDCTL      ((MAIN_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + ( 1 * 4))
#define LPSC_MAIN_MCANSS_13_MDCTL      ((MAIN_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + (47 * 4))

/* Module Domain : LPSC_DSS            Power Domain : PD_DSS */
#define LPSC_DSS_PDCTL                 ((MAIN_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + ( 2 * 4))
#define LPSC_DSS_MDCTL                 ((MAIN_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + (48 * 4))

/* Module Domain : LPSC_DSS_PBIST      Power Domain : PD_DSS */
#define LPSC_DSS_PBIST_PDCTL           ((MAIN_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + ( 2 * 4))
#define LPSC_DSS_PBIST_MDCTL           ((MAIN_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + (49 * 4))

/* Module Domain : LPSC_DSI            Power Domain : PD_DSS */
#define LPSC_DSI_PDCTL                 ((MAIN_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + ( 2 * 4))
#define LPSC_DSI_MDCTL                 ((MAIN_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + (50 * 4))

/* Module Domain : LPSC_EDP_0          Power Domain : PD_DSS */
#define LPSC_EDP_0_PDCTL               ((MAIN_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + ( 2 * 4))
#define LPSC_EDP_0_MDCTL               ((MAIN_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + (51 * 4))

/* Module Domain : LPSC_EDP_1          Power Domain : PD_DSS */
#define LPSC_EDP_1_PDCTL               ((MAIN_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + ( 2 * 4))
#define LPSC_EDP_1_MDCTL               ((MAIN_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + (52 * 4))

/* Module Domain : LPSC_CSIRX_0        Power Domain : PD_DSS */
#define LPSC_CSIRX_0_PDCTL             ((MAIN_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + ( 2 * 4))
#define LPSC_CSIRX_0_MDCTL             ((MAIN_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + (53 * 4))

/* Module Domain : LPSC_CSIRX_1        Power Domain : PD_DSS */
#define LPSC_CSIRX_1_PDCTL             ((MAIN_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + ( 2 * 4))
#define LPSC_CSIRX_1_MDCTL             ((MAIN_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + (54 * 4))

/* Module Domain : LPSC_CSIRX_2        Power Domain : PD_DSS */
#define LPSC_CSIRX_2_PDCTL             ((MAIN_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + ( 2 * 4))
#define LPSC_CSIRX_2_MDCTL             ((MAIN_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + (55 * 4))

/* Module Domain : LPSC_CSITX_0        Power Domain : PD_DSS */
#define LPSC_CSITX_0_PDCTL             ((MAIN_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + ( 2 * 4))
#define LPSC_CSITX_0_MDCTL             ((MAIN_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + (56 * 4))

/* Module Domain : LPSC_TX_DPHY_0      Power Domain : PD_DSS */
#define LPSC_TX_DPHY_0_PDCTL           ((MAIN_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + ( 2 * 4))
#define LPSC_TX_DPHY_0_MDCTL           ((MAIN_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + (57 * 4))

/* Module Domain : LPSC_CSIRX_PHY_0    Power Domain : PD_DSS */
#define LPSC_CSIRX_PHY_0_PDCTL         ((MAIN_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + ( 2 * 4))
#define LPSC_CSIRX_PHY_0_MDCTL         ((MAIN_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + (58 * 4))

/* Module Domain : LPSC_CSIRX_PHY_1    Power Domain : PD_DSS */
#define LPSC_CSIRX_PHY_1_PDCTL         ((MAIN_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + ( 2 * 4))
#define LPSC_CSIRX_PHY_1_MDCTL         ((MAIN_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + (59 * 4))

/* Module Domain : LPSC_CSIRX_PHY_2    Power Domain : PD_DSS */
#define LPSC_CSIRX_PHY_2_PDCTL         ((MAIN_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + ( 2 * 4))
#define LPSC_CSIRX_PHY_2_MDCTL         ((MAIN_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + (60 * 4))

/* Module Domain : LPSC_ICSSG_0        Power Domain : PD_ICSS */
#define LPSC_ICSSG_0_PDCTL             ((MAIN_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + ( 3 * 4))
#define LPSC_ICSSG_0_MDCTL             ((MAIN_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + (61 * 4))

/* Module Domain : LPSC_ICSSG_1        Power Domain : PD_ICSS */
#define LPSC_ICSSG_1_PDCTL             ((MAIN_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + ( 3 * 4))
#define LPSC_ICSSG_1_MDCTL             ((MAIN_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + (62 * 4))

/* Module Domain : LPSC_9GSS           Power Domain : PD_9GSS */
#define LPSC_9GSS_PDCTL                ((MAIN_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + ( 4 * 4))
#define LPSC_9GSS_MDCTL                ((MAIN_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + (63 * 4))

/* Module Domain : LPSC_SERDES_0       Power Domain : PD_SERDES_0 */
#define LPSC_SERDES_0_PDCTL            ((MAIN_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + ( 5 * 4))
#define LPSC_SERDES_0_MDCTL            ((MAIN_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + (64 * 4))

/* Module Domain : LPSC_SERDES_1       Power Domain : PD_SERDES_1 */
#define LPSC_SERDES_1_PDCTL            ((MAIN_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + ( 6 * 4))
#define LPSC_SERDES_1_MDCTL            ((MAIN_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + (65 * 4))

/* Module Domain : LPSC_SERDES_2       Power Domain : PD_SERDES_2 */
#define LPSC_SERDES_2_PDCTL            ((MAIN_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + ( 7 * 4))
#define LPSC_SERDES_2_MDCTL            ((MAIN_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + (66 * 4))

/* Module Domain : LPSC_SERDES_3       Power Domain : PD_SERDES_3 */
#define LPSC_SERDES_3_PDCTL            ((MAIN_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + ( 8 * 4))
#define LPSC_SERDES_3_MDCTL            ((MAIN_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + (67 * 4))

/* Module Domain : LPSC_SERDES_4       Power Domain : PD_SERDES_4 */
#define LPSC_SERDES_4_PDCTL            ((MAIN_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + ( 9 * 4))
#define LPSC_SERDES_4_MDCTL            ((MAIN_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + (68 * 4))

/* Module Domain : LPSC_SERDES_5       Power Domain : PD_SERDES_5 */
#define LPSC_SERDES_5_PDCTL            ((MAIN_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + (10 * 4))
#define LPSC_SERDES_5_MDCTL            ((MAIN_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + (69 * 4))

/* Module Domain : LPSC_DMTIMER_0      Power Domain : PD_TIMER */
#define LPSC_DMTIMER_0_PDCTL           ((MAIN_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + (11 * 4))
#define LPSC_DMTIMER_0_MDCTL           ((MAIN_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + (70 * 4))

/* Module Domain : LPSC_DMTIMER_1      Power Domain : PD_TIMER */
#define LPSC_DMTIMER_1_PDCTL           ((MAIN_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + (11 * 4))
#define LPSC_DMTIMER_1_MDCTL           ((MAIN_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + (71 * 4))

/* Module Domain : LPSC_DMTIMER_2      Power Domain : PD_TIMER */
#define LPSC_DMTIMER_2_PDCTL           ((MAIN_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + (11 * 4))
#define LPSC_DMTIMER_2_MDCTL           ((MAIN_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + (72 * 4))

/* Module Domain : LPSC_DMTIMER_3      Power Domain : PD_TIMER */
#define LPSC_DMTIMER_3_PDCTL           ((MAIN_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + (11 * 4))
#define LPSC_DMTIMER_3_MDCTL           ((MAIN_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + (73 * 4))

/* Module Domain : LPSC_C71X_0         Power Domain : PD_C71X_0 */
#define LPSC_C71X_0_PDCTL              ((MAIN_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + (12 * 4))
#define LPSC_C71X_0_MDCTL              ((MAIN_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + (74 * 4))

/* Module Domain : LPSC_C71X_0_PBIST   Power Domain : PD_C71X_0 */
#define LPSC_C71X_0_PBIST_PDCTL        ((MAIN_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + (12 * 4))
#define LPSC_C71X_0_PBIST_MDCTL        ((MAIN_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + (75 * 4))

/* Module Domain : LPSC_C71X_1         Power Domain : PD_C71X_1 */
#define LPSC_C71X_1_PDCTL              ((MAIN_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + (13 * 4))
#define LPSC_C71X_1_MDCTL              ((MAIN_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + (76 * 4))

/* Module Domain : LPSC_C71X_1_PBIST   Power Domain : PD_C71X_1 */
#define LPSC_C71X_1_PBIST_PDCTL        ((MAIN_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + (13 * 4))
#define LPSC_C71X_1_PBIST_MDCTL        ((MAIN_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + (77 * 4))

/* Module Domain : LPSC_A72_CLUSTER_0  Power Domain : PD_A72_CLUSTER_0 */
#define LPSC_A72_CLUSTER_0_PDCTL       ((MAIN_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + (14 * 4))
#define LPSC_A72_CLUSTER_0_MDCTL       ((MAIN_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + (78 * 4))

/* Module Domain : LPSC_A72_CLUSTER_0_PBIST Power Domain : PD_A72_CLUSTER_0 */
#define LPSC_A72_CLUSTER_0_PBIST_PDCTL ((MAIN_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + (14 * 4))
#define LPSC_A72_CLUSTER_0_PBIST_MDCTL ((MAIN_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + (79 * 4))

/* Module Domain : LPSC_A72_0          Power Domain : PD_A72_0 */
#define LPSC_A72_0_PDCTL               ((MAIN_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + (15 * 4))
#define LPSC_A72_0_MDCTL               ((MAIN_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + (80 * 4))

/* Module Domain : LPSC_A72_1          Power Domain : PD_A72_1 */
#define LPSC_A72_1_PDCTL               ((MAIN_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + (16 * 4))
#define LPSC_A72_1_MDCTL               ((MAIN_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + (81 * 4))

/* Module Domain : LPSC_A72_CLUSTER_1  Power Domain : PD_A72_CLUSTER_1 */
#define LPSC_A72_CLUSTER_1_PDCTL       ((MAIN_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + (17 * 4))
#define LPSC_A72_CLUSTER_1_MDCTL       ((MAIN_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + (82 * 4))

/* Module Domain : LPSC_A72_CLUSTER_1_PBIST Power Domain : PD_A72_CLUSTER_1 */
#define LPSC_A72_CLUSTER_1_PBIST_PDCTL ((MAIN_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + (17 * 4))
#define LPSC_A72_CLUSTER_1_PBIST_MDCTL ((MAIN_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + (83 * 4))

/* Module Domain : LPSC_A72_2          Power Domain : PD_A72_2 */
#define LPSC_A72_2_PDCTL               ((MAIN_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + (18 * 4))
#define LPSC_A72_2_MDCTL               ((MAIN_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + (84 * 4))

/* Module Domain : LPSC_A72_3          Power Domain : PD_A72_3 */
#define LPSC_A72_3_PDCTL               ((MAIN_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + (19 * 4))
#define LPSC_A72_3_MDCTL               ((MAIN_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + (85 * 4))

/* Module Domain : LPSC_GPUCOM         Power Domain : PD_GPUCOM */
#define LPSC_GPUCOM_PDCTL              ((MAIN_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + (20 * 4))
#define LPSC_GPUCOM_MDCTL              ((MAIN_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + (86 * 4))

/* Module Domain : LPSC_GPUPBIST       Power Domain : PD_GPUCOM */
#define LPSC_GPUPBIST_PDCTL            ((MAIN_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + (20 * 4))
#define LPSC_GPUPBIST_MDCTL            ((MAIN_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + (87 * 4))

/* Module Domain : LPSC_GPUCORE        Power Domain : PD_GPUCORE */
#define LPSC_GPUCORE_PDCTL             ((MAIN_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + (21 * 4))
#define LPSC_GPUCORE_MDCTL             ((MAIN_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + (88 * 4))

/* Module Domain : LPSC_C66X_0         Power Domain : PD_C66X_0 */
#define LPSC_C66X_0_PDCTL              ((MAIN_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + (22 * 4))
#define LPSC_C66X_0_MDCTL              ((MAIN_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + (89 * 4))

/* Module Domain : LPSC_C66X_PBIST_0   Power Domain : PD_C66X_0 */
#define LPSC_C66X_PBIST_0_PDCTL        ((MAIN_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + (22 * 4))
#define LPSC_C66X_PBIST_0_MDCTL        ((MAIN_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + (90 * 4))

/* Module Domain : LPSC_C66X_1         Power Domain : PD_C66X_1 */
#define LPSC_C66X_1_PDCTL              ((MAIN_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + (23 * 4))
#define LPSC_C66X_1_MDCTL              ((MAIN_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + (91 * 4))

/* Module Domain : LPSC_C66X_PBIST_1   Power Domain : PD_C66X_1 */
#define LPSC_C66X_PBIST_1_PDCTL        ((MAIN_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + (23 * 4))
#define LPSC_C66X_PBIST_1_MDCTL        ((MAIN_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + (92 * 4))

/* Module Domain : LPSC_PULSAR_0_R5_0  Power Domain : PD_PULSAR_0 */
#define LPSC_PULSAR_0_R5_0_PDCTL       ((MAIN_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + (24 * 4))
#define LPSC_PULSAR_0_R5_0_MDCTL       ((MAIN_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + (93 * 4))

/* Module Domain : LPSC_PULSAR_0_R5_1  Power Domain : PD_PULSAR_0 */
#define LPSC_PULSAR_0_R5_1_PDCTL       ((MAIN_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + (24 * 4))
#define LPSC_PULSAR_0_R5_1_MDCTL       ((MAIN_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + (94 * 4))

/* Module Domain : LPSC_PULSAR_PBIST_0 Power Domain : PD_PULSAR_0 */
#define LPSC_PULSAR_PBIST_0_PDCTL      ((MAIN_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + (24 * 4))
#define LPSC_PULSAR_PBIST_0_MDCTL      ((MAIN_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + (95 * 4))

/* Module Domain : LPSC_PULSAR_1_R5_0  Power Domain : PD_PULSAR_1 */
#define LPSC_PULSAR_1_R5_0_PDCTL       ((MAIN_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + (25 * 4))
#define LPSC_PULSAR_1_R5_0_MDCTL       ((MAIN_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + (96 * 4))

/* Module Domain : LPSC_PULSAR_1_R5_1  Power Domain : PD_PULSAR_1 */
#define LPSC_PULSAR_1_R5_1_PDCTL       ((MAIN_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + (25 * 4))
#define LPSC_PULSAR_1_R5_1_MDCTL       ((MAIN_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + (97 * 4))

/* Module Domain : LPSC_PULSAR_PBIST_1 Power Domain : PD_PULSAR_1 */
#define LPSC_PULSAR_PBIST_1_PDCTL      ((MAIN_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + (25 * 4))
#define LPSC_PULSAR_PBIST_1_MDCTL      ((MAIN_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + (98 * 4))

/* Module Domain : LPSC_DECODE_0       Power Domain : PD_DECODE_0 */
#define LPSC_DECODE_0_PDCTL            ((MAIN_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + (26 * 4))
#define LPSC_DECODE_0_MDCTL            ((MAIN_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + (99 * 4))

/* Module Domain : LPSC_DECODE_PBIST   Power Domain : PD_DECODE_0 */
#define LPSC_DECODE_PBIST_PDCTL        ((MAIN_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + (26 * 4))
#define LPSC_DECODE_PBIST_MDCTL        ((MAIN_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + (100 * 4))

/* Module Domain : LPSC_ENCODE_0       Power Domain : PD_ENCODE_0 */
#define LPSC_ENCODE_0_PDCTL            ((MAIN_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + (27 * 4))
#define LPSC_ENCODE_0_MDCTL            ((MAIN_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + (101 * 4))

/* Module Domain : LPSC_ENCODE_PBIST   Power Domain : PD_ENCODE_0 */
#define LPSC_ENCODE_PBIST_PDCTL        ((MAIN_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + (27 * 4))
#define LPSC_ENCODE_PBIST_MDCTL        ((MAIN_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + (102 * 4))

/* Module Domain : LPSC_DMPAC          Power Domain : PD_DMPAC */
#define LPSC_DMPAC_PDCTL               ((MAIN_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + (28 * 4))
#define LPSC_DMPAC_MDCTL               ((MAIN_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + (103 * 4))

/* Module Domain : LPSC_SDE            Power Domain : PD_DMPAC */
#define LPSC_SDE_PDCTL                 ((MAIN_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + (28 * 4))
#define LPSC_SDE_MDCTL                 ((MAIN_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + (104 * 4))

/* Module Domain : LPSC_DMPAC_PBIST    Power Domain : PD_DMPAC */
#define LPSC_DMPAC_PBIST_PDCTL         ((MAIN_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + (28 * 4))
#define LPSC_DMPAC_PBIST_MDCTL         ((MAIN_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + (105 * 4))

/* Module Domain : LPSC_VPAC           Power Domain : PD_VPAC */
#define LPSC_VPAC_PDCTL                ((MAIN_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + (29 * 4))
#define LPSC_VPAC_MDCTL                ((MAIN_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + (106 * 4))

/* Module Domain : LPSC_VPAC_PBIST     Power Domain : PD_VPAC */
#define LPSC_VPAC_PBIST_PDCTL          ((MAIN_PSC << PSC_NUM_POSITION) + PSC_PDCTL_BASE + (29 * 4))
#define LPSC_VPAC_PBIST_MDCTL          ((MAIN_PSC << PSC_NUM_POSITION) + PSC_MDCTL_BASE + (107 * 4))

/* Maximum number of PD and LPSC */
#define wkup_max_pd   2
#define wkup_max_md  22
#define main_max_pd  30
#define main_max_md 108

/* List of all default OFF LPSCs except lpsc_lock=1 */
#define STRUCT_LPSC_DEFAULT_OFF \
{\
 {LPSC_WKUPMCU2MAIN_PDCTL            , LPSC_WKUPMCU2MAIN_MDCTL            }, /*   0 */ \
 {LPSC_MCU_I3C_0_PDCTL               , LPSC_MCU_I3C_0_MDCTL               }, /*   1 */ \
 {LPSC_MCU_ADC_0_PDCTL               , LPSC_MCU_ADC_0_MDCTL               }, /*   2 */ \
 {LPSC_MCU_R5_0_PDCTL                , LPSC_MCU_R5_0_MDCTL                }, /*   3 */ \
 {LPSC_MCU_R5_1_PDCTL                , LPSC_MCU_R5_1_MDCTL                }, /*   4 */ \
 {LPSC_MCU_PULSAR_PBIST_0_PDCTL      , LPSC_MCU_PULSAR_PBIST_0_MDCTL      }, /*   5 */ \
 {LPSC_PER_ATL_PDCTL                 , LPSC_PER_ATL_MDCTL                 }, /*   6 */ \
 {LPSC_PER_MOTOR_PDCTL               , LPSC_PER_MOTOR_MDCTL               }, /*   7 */ \
 {LPSC_PER_MISCIO_PDCTL              , LPSC_PER_MISCIO_MDCTL              }, /*   8 */ \
 {LPSC_PER_SPARE0_PDCTL              , LPSC_PER_SPARE0_MDCTL              }, /*   9 */ \
 {LPSC_EMIF_DATA_0_PDCTL             , LPSC_EMIF_DATA_0_MDCTL             }, /*  10 */ \
 {LPSC_EMIF_CFG_0_PDCTL              , LPSC_EMIF_CFG_0_MDCTL              }, /*  11 */ \
 {LPSC_CC_TOP_PBIST_PDCTL            , LPSC_CC_TOP_PBIST_MDCTL            }, /*  12 */ \
 {LPSC_USB_0_PDCTL                   , LPSC_USB_0_MDCTL                   }, /*  13 */ \
 {LPSC_MMC4B_0_PDCTL                 , LPSC_MMC4B_0_MDCTL                 }, /*  14 */ \
 {LPSC_MMC8B_0_PDCTL                 , LPSC_MMC8B_0_MDCTL                 }, /*  15 */ \
 {LPSC_PCIE_1_PDCTL                  , LPSC_PCIE_1_MDCTL                  }, /*  16 */ \
 {LPSC_PER_I3C_PDCTL                 , LPSC_PER_I3C_MDCTL                 }, /*  17 */ \
 {LPSC_MAIN_MCANSS_0_PDCTL           , LPSC_MAIN_MCANSS_0_MDCTL           }, /*  18 */ \
 {LPSC_MAIN_MCANSS_1_PDCTL           , LPSC_MAIN_MCANSS_1_MDCTL           }, /*  19 */ \
 {LPSC_MAIN_MCANSS_2_PDCTL           , LPSC_MAIN_MCANSS_2_MDCTL           }, /*  20 */ \
 {LPSC_MAIN_MCANSS_3_PDCTL           , LPSC_MAIN_MCANSS_3_MDCTL           }, /*  21 */ \
 {LPSC_MAIN_MCANSS_4_PDCTL           , LPSC_MAIN_MCANSS_4_MDCTL           }, /*  22 */ \
 {LPSC_MAIN_MCANSS_5_PDCTL           , LPSC_MAIN_MCANSS_5_MDCTL           }, /*  23 */ \
 {LPSC_MAIN_MCANSS_6_PDCTL           , LPSC_MAIN_MCANSS_6_MDCTL           }, /*  24 */ \
 {LPSC_MAIN_MCANSS_7_PDCTL           , LPSC_MAIN_MCANSS_7_MDCTL           }, /*  25 */ \
 {LPSC_MAIN_MCANSS_8_PDCTL           , LPSC_MAIN_MCANSS_8_MDCTL           }, /*  26 */ \
 {LPSC_MAIN_MCANSS_9_PDCTL           , LPSC_MAIN_MCANSS_9_MDCTL           }, /*  27 */ \
 {LPSC_MAIN_MCANSS_10_PDCTL          , LPSC_MAIN_MCANSS_10_MDCTL          }, /*  28 */ \
 {LPSC_MAIN_MCANSS_11_PDCTL          , LPSC_MAIN_MCANSS_11_MDCTL          }, /*  29 */ \
 {LPSC_MAIN_MCANSS_12_PDCTL          , LPSC_MAIN_MCANSS_12_MDCTL          }, /*  30 */ \
 {LPSC_MAIN_MCANSS_13_PDCTL          , LPSC_MAIN_MCANSS_13_MDCTL          }, /*  31 */ \
 {LPSC_DSS_PDCTL                     , LPSC_DSS_MDCTL                     }, /*  32 */ \
 {LPSC_DSS_PBIST_PDCTL               , LPSC_DSS_PBIST_MDCTL               }, /*  33 */ \
 {LPSC_DSI_PDCTL                     , LPSC_DSI_MDCTL                     }, /*  34 */ \
 {LPSC_EDP_0_PDCTL                   , LPSC_EDP_0_MDCTL                   }, /*  35 */ \
 {LPSC_EDP_1_PDCTL                   , LPSC_EDP_1_MDCTL                   }, /*  36 */ \
 {LPSC_CSIRX_0_PDCTL                 , LPSC_CSIRX_0_MDCTL                 }, /*  37 */ \
 {LPSC_CSIRX_1_PDCTL                 , LPSC_CSIRX_1_MDCTL                 }, /*  38 */ \
 {LPSC_CSIRX_2_PDCTL                 , LPSC_CSIRX_2_MDCTL                 }, /*  39 */ \
 {LPSC_CSITX_0_PDCTL                 , LPSC_CSITX_0_MDCTL                 }, /*  40 */ \
 {LPSC_TX_DPHY_0_PDCTL               , LPSC_TX_DPHY_0_MDCTL               }, /*  41 */ \
 {LPSC_CSIRX_PHY_0_PDCTL             , LPSC_CSIRX_PHY_0_MDCTL             }, /*  42 */ \
 {LPSC_9GSS_PDCTL                    , LPSC_9GSS_MDCTL                    }, /*  43 */ \
 {LPSC_SERDES_0_PDCTL                , LPSC_SERDES_0_MDCTL                }, /*  44 */ \
 {LPSC_A72_CLUSTER_0_PDCTL           , LPSC_A72_CLUSTER_0_MDCTL           }, /*  45 */ \
 {LPSC_A72_CLUSTER_0_PBIST_PDCTL     , LPSC_A72_CLUSTER_0_PBIST_MDCTL     }, /*  46 */ \
 {LPSC_A72_0_PDCTL                   , LPSC_A72_0_MDCTL                   }, /*  47 */ \
 {LPSC_A72_1_PDCTL                   , LPSC_A72_1_MDCTL                   }, /*  48 */ \
 {LPSC_PULSAR_0_R5_0_PDCTL           , LPSC_PULSAR_0_R5_0_MDCTL           }, /*  49 */ \
 {LPSC_PULSAR_0_R5_1_PDCTL           , LPSC_PULSAR_0_R5_1_MDCTL           }, /*  50 */ \
 {LPSC_PULSAR_PBIST_0_PDCTL          , LPSC_PULSAR_PBIST_0_MDCTL          }, /*  51 */ \
}

#endif
