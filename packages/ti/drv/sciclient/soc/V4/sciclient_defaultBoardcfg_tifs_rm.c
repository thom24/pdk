/*
 * K3 System Firmware Resource Management Configuration Data
 * Auto generated from K3 Resource Partitioning tool
 *
 * Copyright (c) 2018-2021, Texas Instruments Incorporated
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
 *  \file V4/sciclient_defaultBoardcfg.c
 *
 *  \brief File containing the boardcfg default data structure to
 *      send TISCI_MSG_BOARD_CONFIG message.
 *
 */
/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <ti/drv/sciclient/soc/sysfw/include/j721s2/tisci_hosts.h>
#include <ti/drv/sciclient/soc/sysfw/include/j721s2/tisci_boardcfg_constraints.h>
#include <ti/drv/sciclient/soc/sysfw/include/j721s2/tisci_devices.h>
#include <ti/drv/sciclient/soc/V4/sciclient_defaultBoardcfg.h>

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

#if defined (BUILD_MCU1_0)
const struct tisci_local_rm_boardcfg gBoardConfigLow_rm
__attribute__(( aligned(128), section(".boardcfg_data") )) =
{
    .rm_boardcfg = {
        .rev = {
            .tisci_boardcfg_abi_maj = TISCI_BOARDCFG_RM_ABI_MAJ_VALUE,
            .tisci_boardcfg_abi_min = TISCI_BOARDCFG_RM_ABI_MIN_VALUE,
        },
        .host_cfg = {
            .subhdr = {
                .magic = TISCI_BOARDCFG_RM_HOST_CFG_MAGIC_NUM,
                .size = (uint16_t) sizeof(struct tisci_boardcfg_rm_host_cfg),
            },
            .host_cfg_entries = {
            },
        },
        .resasg = {
            .subhdr = {
                .magic = TISCI_BOARDCFG_RM_RESASG_MAGIC_NUM,
                .size = (uint16_t) sizeof(struct tisci_boardcfg_rm_resasg),
            },
            .resasg_entries_size = 47 * sizeof(struct tisci_boardcfg_rm_resasg_entry),
        },
    },
    .resasg_entries = {
        {
            .num_resource = 32,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_BCDMA_0, TISCI_RESASG_SUBTYPE_BCDMA_RING_SPLIT_TR_RX_CHAN),
            .start_resource = 16,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 16,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_BCDMA_0, TISCI_RESASG_SUBTYPE_BCDMA_RING_SPLIT_TR_TX_CHAN),
            .start_resource = 0,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 32,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_BCDMA_0, TISCI_RESASG_SUBTYPE_BCDMA_SPLIT_TR_RX_CHAN),
            .start_resource = 0,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 16,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_BCDMA_0, TISCI_RESASG_SUBTYPE_BCDMA_SPLIT_TR_TX_CHAN),
            .start_resource = 0,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 64,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_MODSS_INTA_0, TISCI_RESASG_SUBTYPE_IA_VINT),
            .start_resource = 0,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 64,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_MODSS_INTA_1, TISCI_RESASG_SUBTYPE_IA_VINT),
            .start_resource = 0,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 64,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_PROXY_0, TISCI_RESASG_SUBTYPE_PROXY_PROXIES),
            .start_resource = 0,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 1,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_RINGACC_0, TISCI_RESASG_SUBTYPE_RA_ERROR_OES),
            .start_resource = 0,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 78,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_RINGACC_0, TISCI_RESASG_SUBTYPE_RA_UDMAP_RX),
            .start_resource = 345,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 81,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_RINGACC_0, TISCI_RESASG_SUBTYPE_RA_UDMAP_TX),
            .start_resource = 4,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 32,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_RINGACC_0, TISCI_RESASG_SUBTYPE_RA_UDMAP_TX_EXT),
            .start_resource = 85,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 2,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_RINGACC_0, TISCI_RESASG_SUBTYPE_RA_UDMAP_RX_H),
            .start_resource = 343,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 2,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_RINGACC_0, TISCI_RESASG_SUBTYPE_RA_UDMAP_RX_UH),
            .start_resource = 341,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 2,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_RINGACC_0, TISCI_RESASG_SUBTYPE_RA_UDMAP_TX_H),
            .start_resource = 2,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 2,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_RINGACC_0, TISCI_RESASG_SUBTYPE_RA_UDMAP_TX_UH),
            .start_resource = 0,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 32,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_RINGACC_0, TISCI_RESASG_SUBTYPE_RA_MONITORS),
            .start_resource = 0,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 142,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_UDMAP_0, TISCI_RESASG_SUBTYPE_UDMAP_RX_FLOW_COMMON),
            .start_resource = 82,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 1,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_UDMAP_0, TISCI_RESASG_SUBTYPE_UDMAP_INVALID_FLOW_OES),
            .start_resource = 0,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 78,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_UDMAP_0, TISCI_RESASG_SUBTYPE_UDMAP_RX_CHAN),
            .start_resource = 4,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 2,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_UDMAP_0, TISCI_RESASG_SUBTYPE_UDMAP_RX_HCHAN),
            .start_resource = 2,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 2,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_UDMAP_0, TISCI_RESASG_SUBTYPE_UDMAP_RX_UHCHAN),
            .start_resource = 0,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 81,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_UDMAP_0, TISCI_RESASG_SUBTYPE_UDMAP_TX_CHAN),
            .start_resource = 4,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 32,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_UDMAP_0, TISCI_RESASG_SUBTYPE_UDMAP_TX_ECHAN),
            .start_resource = 85,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 2,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_UDMAP_0, TISCI_RESASG_SUBTYPE_UDMAP_TX_HCHAN),
            .start_resource = 2,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 2,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_UDMAP_0, TISCI_RESASG_SUBTYPE_UDMAP_TX_UHCHAN),
            .start_resource = 0,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 222,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_UDMASS_INTA_0, TISCI_RESASG_SUBTYPE_IA_VINT),
            .start_resource = 34,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 16,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_UDMASS_INTA_0, TISCI_RESASG_SUBTYPE_IA_BCDMA_TX_CHAN_ERROR_OES),
            .start_resource = 1536,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 16,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_UDMASS_INTA_0, TISCI_RESASG_SUBTYPE_IA_BCDMA_TX_CHAN_DATA_COMPLETION_OES),
            .start_resource = 2048,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 16,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_UDMASS_INTA_0, TISCI_RESASG_SUBTYPE_IA_BCDMA_TX_CHAN_RING_COMPLETION_OES),
            .start_resource = 2560,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 32,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_UDMASS_INTA_0, TISCI_RESASG_SUBTYPE_IA_BCDMA_RX_CHAN_ERROR_OES),
            .start_resource = 3072,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 32,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_UDMASS_INTA_0, TISCI_RESASG_SUBTYPE_IA_BCDMA_RX_CHAN_DATA_COMPLETION_OES),
            .start_resource = 3584,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 32,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_UDMASS_INTA_0, TISCI_RESASG_SUBTYPE_IA_BCDMA_RX_CHAN_RING_COMPLETION_OES),
            .start_resource = 4096,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 63,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_MCU_NAVSS0_PROXY0, TISCI_RESASG_SUBTYPE_PROXY_PROXIES),
            .start_resource = 1,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 1,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_MCU_NAVSS0_RINGACC0, TISCI_RESASG_SUBTYPE_RA_ERROR_OES),
            .start_resource = 0,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 156,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_MCU_NAVSS0_RINGACC0, TISCI_RESASG_SUBTYPE_RA_GP),
            .start_resource = 96,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 43,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_MCU_NAVSS0_RINGACC0, TISCI_RESASG_SUBTYPE_RA_UDMAP_RX),
            .start_resource = 50,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 44,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_MCU_NAVSS0_RINGACC0, TISCI_RESASG_SUBTYPE_RA_UDMAP_TX),
            .start_resource = 2,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 2,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_MCU_NAVSS0_RINGACC0, TISCI_RESASG_SUBTYPE_RA_UDMAP_RX_H),
            .start_resource = 48,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 2,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_MCU_NAVSS0_RINGACC0, TISCI_RESASG_SUBTYPE_RA_UDMAP_TX_H),
            .start_resource = 0,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 32,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_MCU_NAVSS0_RINGACC0, TISCI_RESASG_SUBTYPE_RA_MONITORS),
            .start_resource = 0,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 48,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_MCU_NAVSS0_UDMAP_0, TISCI_RESASG_SUBTYPE_UDMAP_RX_FLOW_COMMON),
            .start_resource = 48,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 1,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_MCU_NAVSS0_UDMAP_0, TISCI_RESASG_SUBTYPE_UDMAP_INVALID_FLOW_OES),
            .start_resource = 0,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 43,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_MCU_NAVSS0_UDMAP_0, TISCI_RESASG_SUBTYPE_UDMAP_RX_CHAN),
            .start_resource = 2,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 2,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_MCU_NAVSS0_UDMAP_0, TISCI_RESASG_SUBTYPE_UDMAP_RX_HCHAN),
            .start_resource = 0,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 44,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_MCU_NAVSS0_UDMAP_0, TISCI_RESASG_SUBTYPE_UDMAP_TX_CHAN),
            .start_resource = 2,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 2,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_MCU_NAVSS0_UDMAP_0, TISCI_RESASG_SUBTYPE_UDMAP_TX_HCHAN),
            .start_resource = 0,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 234,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_MCU_NAVSS0_UDMASS_INTA_0, TISCI_RESASG_SUBTYPE_IA_VINT),
            .start_resource = 22,
            .host_id = TISCI_HOST_ID_ALL,
        },
    }
};
#endif
