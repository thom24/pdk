/*
 *  Copyright (c) Texas Instruments Incorporated 2021
 *  All rights reserved.
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
 */

/**
 *  \ingroup DRV_IPC_MODULE
 *  \defgroup DRV_IPC_SOC_MODULE IPC SoC Config
 *            This is IPC documentation specific to J721S2 SoC
 *
 *  @{
 */

/**
 *  \file ipc_soc.h
 *
 *  \brief IPC Low Level Driver J721S2 SOC specific file.
 */
#ifndef IPC_SOC_V3_H_
#define IPC_SOC_V3_H_

#include <ti/drv/ipc/include/ipc_config.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief VRing Buffer Size required for all core
 * combinations.
 * TBD: This number can be smaller, if needed, for # cores on J721S2
 */
#define IPC_VRING_BUFFER_SIZE   (0x1C00000U)

/** \brief Core definitions */
#define    IPC_MPU1_0           (0U)    /**< ARM A72 - VM0 */
#define    IPC_MCU1_0           (1U)    /**< ARM MCU  R5F - core0 */
#define    IPC_MCU1_1           (2U)    /**< ARM MCU  R5F - core1 */
#define    IPC_MCU2_0           (3U)    /**< ARM Main R5F - core0 */
#define    IPC_MCU2_1           (4U)    /**< ARM Main R5F - core1 */
#define    IPC_MCU3_0           (5U)    /**< ARM Main R5F - core2 */
#define    IPC_MCU3_1           (6U)    /**< ARM Main R5F - core3 */
#define    IPC_C7X_1            (7U)    /**< DSP C7x - core0 */
#define    IPC_C7X_2            (8U)    /**< DSP C7x - core1 */
#define    IPC_MPU1_1           (9U)    /**< ARM A72 - VM1 */
#define    IPC_MAX_PROCS       (10U)    /**< Maximum Processors */

#define    IPC_MAILBOX_CLUSTER_CNT              (12U)
#define    IPC_MAILBOX_USER_CNT                  (4U)
#define    MAIN_NAVSS_MAILBOX_INPUTINTR_MAX    (440U)
#define    MAIN_NAVSS_MAILBOX_OUTPUTINTR_MAX   (512U)

/* Refer to J721S2 interrupt mapping table */
#ifndef QNX_OS
#define    NAVSS512_MPU1_0_INPUT_MAILBOX_OFFSET             (182U)
#else
#define    NAVSS512_MPU1_0_INPUT_MAILBOX_OFFSET             (112U)
#endif
#define    NAVSS512_MCU1R5F0_INPUT_MAILBOX_OFFSET           (400U)
#define    NAVSS512_MCU1R5F1_INPUT_MAILBOX_OFFSET           (404U)
#define    NAVSS512_MCU2R5F0_INPUT_MAILBOX_OFFSET           (216U)
#define    NAVSS512_MCU2R5F1_INPUT_MAILBOX_OFFSET           (248U)
#define    NAVSS512_MCU3R5F0_INPUT_MAILBOX_OFFSET           (280U)
#define    NAVSS512_MCU3R5F1_INPUT_MAILBOX_OFFSET           (312U)
#define    NAVSS512_C7X1_INPUT_MAILBOX_OFFSET               (188U)
#define    NAVSS512_C7X2_INPUT_MAILBOX_OFFSET               (0U) /* TBD: placeholder for actual final value */

#define IPC_MCU_NAVSS0_INTR0_CFG_BASE    (CSL_NAVSS0_INTR0_INTR_ROUTER_CFG_BASE)

/* Request Sciclient for available resource in group2 NavSS
 * For C7x, DMSC does not configure or map CLEC router
 * it must be done by C7x software.
 * StartEvent = 672 (CSLR_GIC500SS0_SPI_NAVSS0_INTR_0_OUTL_INTR_128)
 * ClecEvent  = 1664 corresponds to 672
 */
#define   C7X_CLEC_BASE_ADDR              (CSL_COMPUTE_CLUSTER0_CLEC_BASE)

/* Base NAVSS event from group 2 */
#define   IPC_C7X_COMPUTE_CLUSTER_OFFSET  (CSLR_GIC500SS0_SPI_NAVSS0_INT_0_OUTL_INTR_128)

/* CLEC event corresponding to Base NAVSS event */
#define   C7X1_CLEC_BASE_GR2_NAVSS        (1664U)
#define   C7X2_CLEC_BASE_GR2_NAVSS        (0U) /* TBD: placeholder for actual final value */

/* User selected IRQ number */
#define   C7X1_MBINTR_OFFSET              (2U)
#define   C7X2_MBINTR_OFFSET              (2U) /* TBD: placeholder for actual final value */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

/* None */


/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */



/* @} */

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */
/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

#if defined(BUILD_C7X_1) || defined(BUILD_C7X_2)
void Ipc_configClecRouter(uint32_t coreEvent);
#endif

#ifdef IPC_SUPPORT_SCICLIENT
int32_t Ipc_sciclientIrqRelease(uint16_t remoteId, uint32_t clusterId,
        uint32_t userId, uint32_t intNumber);
int32_t Ipc_sciclientIrqSet(uint16_t remoteId, uint32_t clusterId,
        uint32_t userId, uint32_t intNumber);
#endif
int32_t Ipc_getIntNumRange(uint32_t coreIndex, uint16_t *rangeStartP,
        uint16_t *rangeNumP);

/* ========================================================================== */
/*                       Static Function Definitions                          */
/* ========================================================================== */

/* None */

#ifdef __cplusplus
}
#endif

#endif /* #ifndef IPC_SOC_V3_H_ */

/* @} */
