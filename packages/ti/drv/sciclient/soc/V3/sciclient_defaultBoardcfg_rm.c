/*
 * Copyright (c) 2018, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/**
 *  \file V3/sciclient_defaultBoardcfg.c
 *
 *  \brief File containing the boardcfg default data structure to
 *      send TISCI_MSG_BOARD_CONFIG message.
 *
 */
/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include <ti/drv/sciclient/soc/V3/sciclient_defaultBoardcfg.h>

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

#if defined (BUILD_MCU1_0)
const struct tisci_local_rm_boardcfg gBoardConfigLow_rm
__attribute__(( aligned(128), section(".boardcfg_data") )) =
{
    .rm_boardcfg = {
        .rev = {
            .tisci_boardcfg_abi_maj = 0x0,
            .tisci_boardcfg_abi_min = 0x1,
        },
        .host_cfg = {
            .subhdr = {
                .magic = TISCI_BOARDCFG_RM_HOST_CFG_MAGIC_NUM,
                .size = sizeof(struct tisci_boardcfg_rm_host_cfg),
            },
            .host_cfg_entries = {0},
        },
        .resasg = {
            .subhdr = {
                .magic = TISCI_BOARDCFG_RM_RESASG_MAGIC_NUM,
                .size = sizeof(struct tisci_boardcfg_rm_resasg),
            },
            .resasg_entries_size = TISCI_RESASG_UTYPE_CNT * sizeof(struct tisci_boardcfg_rm_resasg_entry),
        },
    },
    .resasg_entries = {
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_MCU_M4FSS0_CORE0, TISCI_RESASG_SUBTYPE_MCU_M4FSS0_CORE0_NVIC_IRQ_GROUP0_FROM_MCU_MCU_GPIOMUX_INTROUTER0),
            .start_resource = 0U,
            .num_resource = 4U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_MCU_M4FSS0_CORE0, TISCI_RESASG_SUBTYPE_MCU_M4FSS0_CORE0_NVIC_IRQ_GROUP0_FROM_DMASS0_INTAGGR_0),
            .start_resource = 32U,
            .num_resource = 16U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_CPSW0, TISCI_RESASG_SUBTYPE_CPSW0_CPTS_HW1_PUSH_IRQ_GROUP0_FROM_TIMESYNC_EVENT_INTROUTER0),
            .start_resource = 0U,
            .num_resource = 1U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_CPSW0, TISCI_RESASG_SUBTYPE_CPSW0_CPTS_HW2_PUSH_IRQ_GROUP0_FROM_TIMESYNC_EVENT_INTROUTER0),
            .start_resource = 1U,
            .num_resource = 1U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_CPSW0, TISCI_RESASG_SUBTYPE_CPSW0_CPTS_HW3_PUSH_IRQ_GROUP0_FROM_TIMESYNC_EVENT_INTROUTER0),
            .start_resource = 2U,
            .num_resource = 1U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_CPSW0, TISCI_RESASG_SUBTYPE_CPSW0_CPTS_HW4_PUSH_IRQ_GROUP0_FROM_TIMESYNC_EVENT_INTROUTER0),
            .start_resource = 3U,
            .num_resource = 1U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_CPSW0, TISCI_RESASG_SUBTYPE_CPSW0_CPTS_HW5_PUSH_IRQ_GROUP0_FROM_TIMESYNC_EVENT_INTROUTER0),
            .start_resource = 4U,
            .num_resource = 1U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_CPSW0, TISCI_RESASG_SUBTYPE_CPSW0_CPTS_HW6_PUSH_IRQ_GROUP0_FROM_TIMESYNC_EVENT_INTROUTER0),
            .start_resource = 5U,
            .num_resource = 1U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_CPSW0, TISCI_RESASG_SUBTYPE_CPSW0_CPTS_HW7_PUSH_IRQ_GROUP0_FROM_TIMESYNC_EVENT_INTROUTER0),
            .start_resource = 6U,
            .num_resource = 1U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_CPSW0, TISCI_RESASG_SUBTYPE_CPSW0_CPTS_HW8_PUSH_IRQ_GROUP0_FROM_TIMESYNC_EVENT_INTROUTER0),
            .start_resource = 7U,
            .num_resource = 1U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_DMASS0_BCDMA_0, TISCI_RESASG_SUBTYPE_RA_ERROR_OES),
            .start_resource = 0U,
            .num_resource = 1U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_DMASS0_BCDMA_0, TISCI_RESASG_SUBTYPE_GLOBAL_EVENT_TRIGGER),
            .start_resource = 50176U,
            .num_resource = 136U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_DMASS0_BCDMA_0, TISCI_RESASG_SUBTYPE_UDMAP_GLOBAL_CONFIG),
            .start_resource = 0U,
            .num_resource = 1U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_DMASS0_BCDMA_0, TISCI_RESASG_SUBTYPE_BCDMA_RING_BLOCK_COPY_CHAN),
            .start_resource = 0U,
            .num_resource = 28U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_DMASS0_BCDMA_0, TISCI_RESASG_SUBTYPE_BCDMA_RING_SPLIT_TR_RX_CHAN),
            .start_resource = 48U,
            .num_resource = 20U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_DMASS0_BCDMA_0, TISCI_RESASG_SUBTYPE_BCDMA_RING_SPLIT_TR_TX_CHAN),
            .start_resource = 28U,
            .num_resource = 20U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_DMASS0_BCDMA_0, TISCI_RESASG_SUBTYPE_BCDMA_BLOCK_COPY_CHAN),
            .start_resource = 0U,
            .num_resource = 28U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_DMASS0_BCDMA_0, TISCI_RESASG_SUBTYPE_BCDMA_SPLIT_TR_RX_CHAN),
            .start_resource = 0U,
            .num_resource = 20U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_DMASS0_BCDMA_0, TISCI_RESASG_SUBTYPE_BCDMA_SPLIT_TR_TX_CHAN),
            .start_resource = 0U,
            .num_resource = 20U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_DMASS0_INTAGGR_0, TISCI_RESASG_SUBTYPE_DMASS0_INTAGGR_0_INTAGGR_LEVI_PEND_IRQ_GROUP0_FROM_CMP_EVENT_INTROUTER0),
            .start_resource = 0U,
            .num_resource = 8U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_DMASS0_INTAGGR_0, TISCI_RESASG_SUBTYPE_DMASS0_INTAGGR_0_INTAGGR_LEVI_PEND_IRQ_GROUP0_FROM_TIMESYNC_EVENT_INTROUTER0),
            .start_resource = 8U,
            .num_resource = 8U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_DMASS0_INTAGGR_0, TISCI_RESASG_SUBTYPE_DMASS0_INTAGGR_0_INTAGGR_LEVI_PEND_IRQ_GROUP0_FROM_MAIN_GPIOMUX_INTROUTER0),
            .start_resource = 16U,
            .num_resource = 8U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_DMASS0_INTAGGR_0, TISCI_RESASG_SUBTYPE_DMASS0_INTAGGR_0_INTAGGR_LEVI_PEND_IRQ_GROUP1_FROM_MAIN_GPIOMUX_INTROUTER0),
            .start_resource = 24U,
            .num_resource = 2U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_DMASS0_INTAGGR_0, TISCI_RESASG_SUBTYPE_GLOBAL_EVENT_SEVT),
            .start_resource = 15U,
            .num_resource = 1521U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_DMASS0_INTAGGR_0, TISCI_RESASG_SUBTYPE_IA_VINT_TO_GICSS0),
            .start_resource = 4U,
            .num_resource = 36U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_DMASS0_INTAGGR_0, TISCI_RESASG_SUBTYPE_IA_VINT_TO_R5FSS0_CORE0_AND_R5FSS0_CORE1),
            .start_resource = 44U,
            .num_resource = 28U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_DMASS0_INTAGGR_0, TISCI_RESASG_SUBTYPE_IA_VINT_TO_R5FSS0_CORE0),
            .start_resource = 72U,
            .num_resource = 8U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_DMASS0_INTAGGR_0, TISCI_RESASG_SUBTYPE_IA_VINT_TO_R5FSS0_CORE1),
            .start_resource = 80U,
            .num_resource = 8U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_DMASS0_INTAGGR_0, TISCI_RESASG_SUBTYPE_IA_VINT_TO_R5FSS1_CORE0_AND_R5FSS1_CORE1),
            .start_resource = 92U,
            .num_resource = 28U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_DMASS0_INTAGGR_0, TISCI_RESASG_SUBTYPE_IA_VINT_TO_R5FSS1_CORE0),
            .start_resource = 120U,
            .num_resource = 8U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_DMASS0_INTAGGR_0, TISCI_RESASG_SUBTYPE_IA_VINT_TO_R5FSS1_CORE1),
            .start_resource = 128U,
            .num_resource = 8U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_DMASS0_INTAGGR_0, TISCI_RESASG_SUBTYPE_IA_VINT_TO_PRU_ICSSG0),
            .start_resource = 152U,
            .num_resource = 8U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_DMASS0_INTAGGR_0, TISCI_RESASG_SUBTYPE_IA_VINT_TO_PRU_ICSSG1),
            .start_resource = 160U,
            .num_resource = 8U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_DMASS0_INTAGGR_0, TISCI_RESASG_SUBTYPE_IA_VINT_TO_MCU_M4FSS0_CORE0),
            .start_resource = 168U,
            .num_resource = 16U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_DMASS0_INTAGGR_0, TISCI_RESASG_SUBTYPE_IA_TIMERMGR_EVT_OES),
            .start_resource = 0U,
            .num_resource = 1024U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_DMASS0_INTAGGR_0, TISCI_RESASG_SUBTYPE_IA_PKTDMA_TX_CHAN_ERROR_OES),
            .start_resource = 4096U,
            .num_resource = 42U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_DMASS0_INTAGGR_0, TISCI_RESASG_SUBTYPE_IA_PKTDMA_TX_FLOW_COMPLETION_OES),
            .start_resource = 4608U,
            .num_resource = 112U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_DMASS0_INTAGGR_0, TISCI_RESASG_SUBTYPE_IA_PKTDMA_RX_CHAN_ERROR_OES),
            .start_resource = 5120U,
            .num_resource = 29U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_DMASS0_INTAGGR_0, TISCI_RESASG_SUBTYPE_IA_PKTDMA_RX_FLOW_COMPLETION_OES),
            .start_resource = 5632U,
            .num_resource = 176U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_DMASS0_INTAGGR_0, TISCI_RESASG_SUBTYPE_IA_PKTDMA_RX_FLOW_STARVATION_OES),
            .start_resource = 6144U,
            .num_resource = 176U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_DMASS0_INTAGGR_0, TISCI_RESASG_SUBTYPE_IA_PKTDMA_RX_FLOW_FIREWALL_OES),
            .start_resource = 6656U,
            .num_resource = 176U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_DMASS0_INTAGGR_0, TISCI_RESASG_SUBTYPE_IA_BCDMA_CHAN_ERROR_OES),
            .start_resource = 8192U,
            .num_resource = 28U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_DMASS0_INTAGGR_0, TISCI_RESASG_SUBTYPE_IA_BCDMA_CHAN_DATA_COMPLETION_OES),
            .start_resource = 8704U,
            .num_resource = 28U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_DMASS0_INTAGGR_0, TISCI_RESASG_SUBTYPE_IA_BCDMA_CHAN_RING_COMPLETION_OES),
            .start_resource = 9216U,
            .num_resource = 28U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_DMASS0_INTAGGR_0, TISCI_RESASG_SUBTYPE_IA_BCDMA_TX_CHAN_ERROR_OES),
            .start_resource = 9728U,
            .num_resource = 20U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_DMASS0_INTAGGR_0, TISCI_RESASG_SUBTYPE_IA_BCDMA_TX_CHAN_DATA_COMPLETION_OES),
            .start_resource = 10240U,
            .num_resource = 20U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_DMASS0_INTAGGR_0, TISCI_RESASG_SUBTYPE_IA_BCDMA_TX_CHAN_RING_COMPLETION_OES),
            .start_resource = 10752U,
            .num_resource = 20U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_DMASS0_INTAGGR_0, TISCI_RESASG_SUBTYPE_IA_BCDMA_RX_CHAN_ERROR_OES),
            .start_resource = 11264U,
            .num_resource = 20U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_DMASS0_INTAGGR_0, TISCI_RESASG_SUBTYPE_IA_BCDMA_RX_CHAN_DATA_COMPLETION_OES),
            .start_resource = 11776U,
            .num_resource = 20U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_DMASS0_INTAGGR_0, TISCI_RESASG_SUBTYPE_IA_BCDMA_RX_CHAN_RING_COMPLETION_OES),
            .start_resource = 12288U,
            .num_resource = 20U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_DMASS0_PKTDMA_0, TISCI_RESASG_SUBTYPE_RA_ERROR_OES),
            .start_resource = 0U,
            .num_resource = 1U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_DMASS0_PKTDMA_0, TISCI_RESASG_SUBTYPE_UDMAP_GLOBAL_CONFIG),
            .start_resource = 0U,
            .num_resource = 1U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_DMASS0_PKTDMA_0, TISCI_RESASG_SUBTYPE_PKTDMA_RING_UNMAPPED_TX_CHAN),
            .start_resource = 0U,
            .num_resource = 16U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_DMASS0_PKTDMA_0, TISCI_RESASG_SUBTYPE_PKTDMA_RING_CPSW_TX_CHAN),
            .start_resource = 16U,
            .num_resource = 64U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_DMASS0_PKTDMA_0, TISCI_RESASG_SUBTYPE_PKTDMA_RING_SAUL_TX_0_CHAN),
            .start_resource = 80U,
            .num_resource = 8U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_DMASS0_PKTDMA_0, TISCI_RESASG_SUBTYPE_PKTDMA_RING_SAUL_TX_1_CHAN),
            .start_resource = 88U,
            .num_resource = 8U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_DMASS0_PKTDMA_0, TISCI_RESASG_SUBTYPE_PKTDMA_RING_ICSSG_0_TX_CHAN),
            .start_resource = 96U,
            .num_resource = 8U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_DMASS0_PKTDMA_0, TISCI_RESASG_SUBTYPE_PKTDMA_RING_ICSSG_1_TX_CHAN),
            .start_resource = 104U,
            .num_resource = 8U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_DMASS0_PKTDMA_0, TISCI_RESASG_SUBTYPE_PKTDMA_RING_UNMAPPED_RX_CHAN),
            .start_resource = 112U,
            .num_resource = 16U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_DMASS0_PKTDMA_0, TISCI_RESASG_SUBTYPE_PKTDMA_RING_CPSW_RX_CHAN),
            .start_resource = 128U,
            .num_resource = 16U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_DMASS0_PKTDMA_0, TISCI_RESASG_SUBTYPE_PKTDMA_RING_SAUL_RX_0_CHAN),
            .start_resource = 144U,
            .num_resource = 8U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_DMASS0_PKTDMA_0, TISCI_RESASG_SUBTYPE_PKTDMA_RING_SAUL_RX_1_CHAN),
            .start_resource = 144U,
            .num_resource = 8U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_DMASS0_PKTDMA_0, TISCI_RESASG_SUBTYPE_PKTDMA_RING_SAUL_RX_2_CHAN),
            .start_resource = 152U,
            .num_resource = 8U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_DMASS0_PKTDMA_0, TISCI_RESASG_SUBTYPE_PKTDMA_RING_SAUL_RX_3_CHAN),
            .start_resource = 152U,
            .num_resource = 8U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_DMASS0_PKTDMA_0, TISCI_RESASG_SUBTYPE_PKTDMA_RING_ICSSG_0_RX_CHAN),
            .start_resource = 160U,
            .num_resource = 64U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_DMASS0_PKTDMA_0, TISCI_RESASG_SUBTYPE_PKTDMA_RING_ICSSG_1_RX_CHAN),
            .start_resource = 224U,
            .num_resource = 64U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_DMASS0_PKTDMA_0, TISCI_RESASG_SUBTYPE_PKTDMA_UNMAPPED_TX_CHAN),
            .start_resource = 0U,
            .num_resource = 16U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_DMASS0_PKTDMA_0, TISCI_RESASG_SUBTYPE_PKTDMA_CPSW_TX_CHAN),
            .start_resource = 16U,
            .num_resource = 8U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_DMASS0_PKTDMA_0, TISCI_RESASG_SUBTYPE_PKTDMA_SAUL_TX_0_CHAN),
            .start_resource = 24U,
            .num_resource = 1U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_DMASS0_PKTDMA_0, TISCI_RESASG_SUBTYPE_PKTDMA_SAUL_TX_1_CHAN),
            .start_resource = 25U,
            .num_resource = 1U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_DMASS0_PKTDMA_0, TISCI_RESASG_SUBTYPE_PKTDMA_ICSSG_0_TX_CHAN),
            .start_resource = 26U,
            .num_resource = 8U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_DMASS0_PKTDMA_0, TISCI_RESASG_SUBTYPE_PKTDMA_ICSSG_1_TX_CHAN),
            .start_resource = 34U,
            .num_resource = 8U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_DMASS0_PKTDMA_0, TISCI_RESASG_SUBTYPE_PKTDMA_UNMAPPED_RX_CHAN),
            .start_resource = 0U,
            .num_resource = 16U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_DMASS0_PKTDMA_0, TISCI_RESASG_SUBTYPE_PKTDMA_FLOW_UNMAPPED_RX_CHAN),
            .start_resource = 0U,
            .num_resource = 16U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_DMASS0_PKTDMA_0, TISCI_RESASG_SUBTYPE_PKTDMA_CPSW_RX_CHAN),
            .start_resource = 16U,
            .num_resource = 1U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_DMASS0_PKTDMA_0, TISCI_RESASG_SUBTYPE_PKTDMA_FLOW_CPSW_RX_CHAN),
            .start_resource = 16U,
            .num_resource = 16U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_DMASS0_PKTDMA_0, TISCI_RESASG_SUBTYPE_PKTDMA_SAUL_RX_0_CHAN),
            .start_resource = 17U,
            .num_resource = 1U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_DMASS0_PKTDMA_0, TISCI_RESASG_SUBTYPE_PKTDMA_FLOW_SAUL_RX_0_CHAN),
            .start_resource = 32U,
            .num_resource = 8U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_DMASS0_PKTDMA_0, TISCI_RESASG_SUBTYPE_PKTDMA_SAUL_RX_1_CHAN),
            .start_resource = 18U,
            .num_resource = 1U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_DMASS0_PKTDMA_0, TISCI_RESASG_SUBTYPE_PKTDMA_FLOW_SAUL_RX_1_CHAN),
            .start_resource = 32U,
            .num_resource = 8U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_DMASS0_PKTDMA_0, TISCI_RESASG_SUBTYPE_PKTDMA_SAUL_RX_2_CHAN),
            .start_resource = 19U,
            .num_resource = 1U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_DMASS0_PKTDMA_0, TISCI_RESASG_SUBTYPE_PKTDMA_FLOW_SAUL_RX_2_CHAN),
            .start_resource = 40U,
            .num_resource = 8U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_DMASS0_PKTDMA_0, TISCI_RESASG_SUBTYPE_PKTDMA_SAUL_RX_3_CHAN),
            .start_resource = 20U,
            .num_resource = 1U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_DMASS0_PKTDMA_0, TISCI_RESASG_SUBTYPE_PKTDMA_FLOW_SAUL_RX_3_CHAN),
            .start_resource = 40U,
            .num_resource = 8U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_DMASS0_PKTDMA_0, TISCI_RESASG_SUBTYPE_PKTDMA_ICSSG_0_RX_CHAN),
            .start_resource = 21U,
            .num_resource = 4U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_DMASS0_PKTDMA_0, TISCI_RESASG_SUBTYPE_PKTDMA_FLOW_ICSSG_0_RX_CHAN),
            .start_resource = 48U,
            .num_resource = 64U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_DMASS0_PKTDMA_0, TISCI_RESASG_SUBTYPE_PKTDMA_ICSSG_1_RX_CHAN),
            .start_resource = 25U,
            .num_resource = 4U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_DMASS0_PKTDMA_0, TISCI_RESASG_SUBTYPE_PKTDMA_FLOW_ICSSG_1_RX_CHAN),
            .start_resource = 112U,
            .num_resource = 64U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_DMASS0_RINGACC_0, TISCI_RESASG_SUBTYPE_RA_ERROR_OES),
            .start_resource = 0U,
            .num_resource = 1U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_DMASS0_RINGACC_0, TISCI_RESASG_SUBTYPE_RA_VIRTID),
            .start_resource = 64U,
            .num_resource = 64U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_DMASS0_RINGACC_0, TISCI_RESASG_SUBTYPE_RA_GENERIC_IPC),
            .start_resource = 20U,
            .num_resource = 12U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_MCU_ESM0, TISCI_RESASG_SUBTYPE_MCU_ESM0_ESM_PLS_EVENT0_IRQ_GROUP0_FROM_MCU_MCU_GPIOMUX_INTROUTER0),
            .start_resource = 88U,
            .num_resource = 4U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_MCU_ESM0, TISCI_RESASG_SUBTYPE_MCU_ESM0_ESM_PLS_EVENT1_IRQ_GROUP0_FROM_MCU_MCU_GPIOMUX_INTROUTER0),
            .start_resource = 92U,
            .num_resource = 4U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_MCU_ESM0, TISCI_RESASG_SUBTYPE_MCU_ESM0_ESM_PLS_EVENT2_IRQ_GROUP0_FROM_MCU_MCU_GPIOMUX_INTROUTER0),
            .start_resource = 96U,
            .num_resource = 4U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_GICSS0, TISCI_RESASG_SUBTYPE_GICSS0_SPI_IRQ_GROUP0_FROM_MAIN_GPIOMUX_INTROUTER0),
            .start_resource = 32U,
            .num_resource = 16U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_GICSS0, TISCI_RESASG_SUBTYPE_GICSS0_SPI_IRQ_GROUP0_FROM_CMP_EVENT_INTROUTER0),
            .start_resource = 48U,
            .num_resource = 16U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_GICSS0, TISCI_RESASG_SUBTYPE_GICSS0_SPI_IRQ_GROUP0_FROM_DMASS0_INTAGGR_0),
            .start_resource = 68U,
            .num_resource = 36U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_GICSS0, TISCI_RESASG_SUBTYPE_GICSS0_SPI_IRQ_GROUP0_FROM_MCU_MCU_GPIOMUX_INTROUTER0),
            .start_resource = 104U,
            .num_resource = 4U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_PRU_ICSSG0, TISCI_RESASG_SUBTYPE_PRU_ICSSG0_PR1_EDC0_LATCH0_IN_IRQ_GROUP0_FROM_TIMESYNC_EVENT_INTROUTER0),
            .start_resource = 0U,
            .num_resource = 1U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_PRU_ICSSG0, TISCI_RESASG_SUBTYPE_PRU_ICSSG0_PR1_EDC0_LATCH1_IN_IRQ_GROUP0_FROM_TIMESYNC_EVENT_INTROUTER0),
            .start_resource = 1U,
            .num_resource = 1U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_PRU_ICSSG0, TISCI_RESASG_SUBTYPE_PRU_ICSSG0_PR1_EDC1_LATCH0_IN_IRQ_GROUP0_FROM_TIMESYNC_EVENT_INTROUTER0),
            .start_resource = 2U,
            .num_resource = 1U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_PRU_ICSSG0, TISCI_RESASG_SUBTYPE_PRU_ICSSG0_PR1_EDC1_LATCH1_IN_IRQ_GROUP0_FROM_TIMESYNC_EVENT_INTROUTER0),
            .start_resource = 3U,
            .num_resource = 1U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_PRU_ICSSG0, TISCI_RESASG_SUBTYPE_PRU_ICSSG0_PR1_IEP0_CAP_INTR_REQ_IRQ_GROUP0_FROM_MAIN_GPIOMUX_INTROUTER0),
            .start_resource = 4U,
            .num_resource = 6U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_PRU_ICSSG0, TISCI_RESASG_SUBTYPE_PRU_ICSSG0_PR1_IEP1_CAP_INTR_REQ_IRQ_GROUP0_FROM_MAIN_GPIOMUX_INTROUTER0),
            .start_resource = 10U,
            .num_resource = 6U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_PRU_ICSSG0, TISCI_RESASG_SUBTYPE_PRU_ICSSG0_PR1_SLV_INTR_IRQ_GROUP0_FROM_DMASS0_INTAGGR_0),
            .start_resource = 16U,
            .num_resource = 8U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_PRU_ICSSG0, TISCI_RESASG_SUBTYPE_PRU_ICSSG0_PR1_SLV_INTR_IRQ_GROUP0_FROM_MAIN_GPIOMUX_INTROUTER0),
            .start_resource = 46U,
            .num_resource = 8U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_PRU_ICSSG1, TISCI_RESASG_SUBTYPE_PRU_ICSSG1_PR1_EDC0_LATCH0_IN_IRQ_GROUP0_FROM_TIMESYNC_EVENT_INTROUTER0),
            .start_resource = 0U,
            .num_resource = 1U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_PRU_ICSSG1, TISCI_RESASG_SUBTYPE_PRU_ICSSG1_PR1_EDC0_LATCH1_IN_IRQ_GROUP0_FROM_TIMESYNC_EVENT_INTROUTER0),
            .start_resource = 1U,
            .num_resource = 1U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_PRU_ICSSG1, TISCI_RESASG_SUBTYPE_PRU_ICSSG1_PR1_EDC1_LATCH0_IN_IRQ_GROUP0_FROM_TIMESYNC_EVENT_INTROUTER0),
            .start_resource = 2U,
            .num_resource = 1U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_PRU_ICSSG1, TISCI_RESASG_SUBTYPE_PRU_ICSSG1_PR1_EDC1_LATCH1_IN_IRQ_GROUP0_FROM_TIMESYNC_EVENT_INTROUTER0),
            .start_resource = 3U,
            .num_resource = 1U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_PRU_ICSSG1, TISCI_RESASG_SUBTYPE_PRU_ICSSG1_PR1_IEP0_CAP_INTR_REQ_IRQ_GROUP0_FROM_MAIN_GPIOMUX_INTROUTER0),
            .start_resource = 4U,
            .num_resource = 6U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_PRU_ICSSG1, TISCI_RESASG_SUBTYPE_PRU_ICSSG1_PR1_IEP1_CAP_INTR_REQ_IRQ_GROUP0_FROM_MAIN_GPIOMUX_INTROUTER0),
            .start_resource = 10U,
            .num_resource = 6U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_PRU_ICSSG1, TISCI_RESASG_SUBTYPE_PRU_ICSSG1_PR1_SLV_INTR_IRQ_GROUP0_FROM_DMASS0_INTAGGR_0),
            .start_resource = 16U,
            .num_resource = 8U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_PRU_ICSSG1, TISCI_RESASG_SUBTYPE_PRU_ICSSG1_PR1_SLV_INTR_IRQ_GROUP0_FROM_MAIN_GPIOMUX_INTROUTER0),
            .start_resource = 46U,
            .num_resource = 8U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_CPTS0, TISCI_RESASG_SUBTYPE_CPTS0_CPTS_HW1_PUSH_IRQ_GROUP0_FROM_TIMESYNC_EVENT_INTROUTER0),
            .start_resource = 0U,
            .num_resource = 1U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_CPTS0, TISCI_RESASG_SUBTYPE_CPTS0_CPTS_HW2_PUSH_IRQ_GROUP0_FROM_TIMESYNC_EVENT_INTROUTER0),
            .start_resource = 1U,
            .num_resource = 1U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_CPTS0, TISCI_RESASG_SUBTYPE_CPTS0_CPTS_HW3_PUSH_IRQ_GROUP0_FROM_TIMESYNC_EVENT_INTROUTER0),
            .start_resource = 2U,
            .num_resource = 1U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_CPTS0, TISCI_RESASG_SUBTYPE_CPTS0_CPTS_HW4_PUSH_IRQ_GROUP0_FROM_TIMESYNC_EVENT_INTROUTER0),
            .start_resource = 3U,
            .num_resource = 1U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_CPTS0, TISCI_RESASG_SUBTYPE_CPTS0_CPTS_HW5_PUSH_IRQ_GROUP0_FROM_TIMESYNC_EVENT_INTROUTER0),
            .start_resource = 4U,
            .num_resource = 1U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_CPTS0, TISCI_RESASG_SUBTYPE_CPTS0_CPTS_HW6_PUSH_IRQ_GROUP0_FROM_TIMESYNC_EVENT_INTROUTER0),
            .start_resource = 5U,
            .num_resource = 1U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_CPTS0, TISCI_RESASG_SUBTYPE_CPTS0_CPTS_HW7_PUSH_IRQ_GROUP0_FROM_TIMESYNC_EVENT_INTROUTER0),
            .start_resource = 6U,
            .num_resource = 1U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_CPTS0, TISCI_RESASG_SUBTYPE_CPTS0_CPTS_HW8_PUSH_IRQ_GROUP0_FROM_TIMESYNC_EVENT_INTROUTER0),
            .start_resource = 7U,
            .num_resource = 1U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_PCIE0, TISCI_RESASG_SUBTYPE_PCIE0_PCIE_CPTS_HW2_PUSH_IRQ_GROUP0_FROM_TIMESYNC_EVENT_INTROUTER0),
            .start_resource = 0U,
            .num_resource = 1U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_R5FSS0_CORE0, TISCI_RESASG_SUBTYPE_R5FSS0_CORE0_INTR_IRQ_GROUP0_FROM_DMASS0_INTAGGR_0),
            .start_resource = 8U,
            .num_resource = 8U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_R5FSS0_CORE0, TISCI_RESASG_SUBTYPE_R5FSS0_CORE0_INTR_IRQ_GROUP0_FROM_MAIN_GPIOMUX_INTROUTER0),
            .start_resource = 32U,
            .num_resource = 24U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_R5FSS0_CORE0, TISCI_RESASG_SUBTYPE_R5FSS0_CORE0_INTR_IRQ_GROUP1_FROM_DMASS0_INTAGGR_0),
            .start_resource = 66U,
            .num_resource = 30U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_R5FSS0_CORE0, TISCI_RESASG_SUBTYPE_R5FSS0_CORE0_INTR_IRQ_GROUP0_FROM_MCU_MCU_GPIOMUX_INTROUTER0),
            .start_resource = 104U,
            .num_resource = 4U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_R5FSS0_CORE1, TISCI_RESASG_SUBTYPE_R5FSS0_CORE1_INTR_IRQ_GROUP0_FROM_DMASS0_INTAGGR_0),
            .start_resource = 8U,
            .num_resource = 8U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_R5FSS0_CORE1, TISCI_RESASG_SUBTYPE_R5FSS0_CORE1_INTR_IRQ_GROUP0_FROM_MAIN_GPIOMUX_INTROUTER0),
            .start_resource = 32U,
            .num_resource = 24U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_R5FSS0_CORE1, TISCI_RESASG_SUBTYPE_R5FSS0_CORE1_INTR_IRQ_GROUP1_FROM_DMASS0_INTAGGR_0),
            .start_resource = 64U,
            .num_resource = 32U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_R5FSS0_CORE1, TISCI_RESASG_SUBTYPE_R5FSS0_CORE1_INTR_IRQ_GROUP0_FROM_MCU_MCU_GPIOMUX_INTROUTER0),
            .start_resource = 104U,
            .num_resource = 4U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_R5FSS1_CORE0, TISCI_RESASG_SUBTYPE_R5FSS1_CORE0_INTR_IRQ_GROUP0_FROM_DMASS0_INTAGGR_0),
            .start_resource = 8U,
            .num_resource = 8U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_R5FSS1_CORE0, TISCI_RESASG_SUBTYPE_R5FSS1_CORE0_INTR_IRQ_GROUP0_FROM_MAIN_GPIOMUX_INTROUTER0),
            .start_resource = 32U,
            .num_resource = 16U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_R5FSS1_CORE0, TISCI_RESASG_SUBTYPE_R5FSS1_CORE0_INTR_IRQ_GROUP0_FROM_CMP_EVENT_INTROUTER0),
            .start_resource = 48U,
            .num_resource = 8U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_R5FSS1_CORE0, TISCI_RESASG_SUBTYPE_R5FSS1_CORE0_INTR_IRQ_GROUP1_FROM_DMASS0_INTAGGR_0),
            .start_resource = 66U,
            .num_resource = 30U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_R5FSS1_CORE0, TISCI_RESASG_SUBTYPE_R5FSS1_CORE0_INTR_IRQ_GROUP0_FROM_MCU_MCU_GPIOMUX_INTROUTER0),
            .start_resource = 104U,
            .num_resource = 4U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_R5FSS1_CORE1, TISCI_RESASG_SUBTYPE_R5FSS1_CORE1_INTR_IRQ_GROUP0_FROM_DMASS0_INTAGGR_0),
            .start_resource = 8U,
            .num_resource = 8U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_R5FSS1_CORE1, TISCI_RESASG_SUBTYPE_R5FSS1_CORE1_INTR_IRQ_GROUP0_FROM_MAIN_GPIOMUX_INTROUTER0),
            .start_resource = 32U,
            .num_resource = 16U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_R5FSS1_CORE1, TISCI_RESASG_SUBTYPE_R5FSS1_CORE1_INTR_IRQ_GROUP0_FROM_CMP_EVENT_INTROUTER0),
            .start_resource = 48U,
            .num_resource = 8U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_R5FSS1_CORE1, TISCI_RESASG_SUBTYPE_R5FSS1_CORE1_INTR_IRQ_GROUP1_FROM_DMASS0_INTAGGR_0),
            .start_resource = 64U,
            .num_resource = 32U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_R5FSS1_CORE1, TISCI_RESASG_SUBTYPE_R5FSS1_CORE1_INTR_IRQ_GROUP0_FROM_MCU_MCU_GPIOMUX_INTROUTER0),
            .start_resource = 104U,
            .num_resource = 4U,
            .host_id = TISCI_HOST_ID_ALL,
        },
    },
};
#endif

