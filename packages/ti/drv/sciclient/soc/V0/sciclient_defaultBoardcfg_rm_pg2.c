/*
 * Copyright (c) 2018-2020, Texas Instruments Incorporated
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
 *  \file V0/sciclient_defaultBoardcfg_pg2.c
 *
 *  \brief File containing the boardcfg default data structure to
 *      send TISCI_MSG_BOARD_CONFIG message.
 *
 */
/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <ti/drv/sciclient/soc/sysfw/include/am65x/tisci_hosts.h>
#include <ti/drv/sciclient/soc/sysfw/include/am65x/tisci_boardcfg_constraints.h>
#include <ti/drv/sciclient/soc/V0/sciclient_defaultBoardcfg.h>

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

#if defined (BUILD_MCU1_0)
const struct tisci_local_rm_boardcfg gSciclient_boardCfgLow_rm_pg2
__attribute__(( aligned(128), section(".boardcfg_data") )) =
{
    .rm_boardcfg = {
        /* tisci_boardcfg_abi_rev */
        .rev = {
            .tisci_boardcfg_abi_maj = TISCI_BOARDCFG_RM_ABI_MAJ_VALUE,
            .tisci_boardcfg_abi_min = TISCI_BOARDCFG_RM_ABI_MIN_VALUE,
        },
        .host_cfg = {
            .subhdr = {
                .magic = TISCI_BOARDCFG_RM_HOST_CFG_MAGIC_NUM,
                .size  = sizeof(struct tisci_boardcfg_rm_host_cfg),
            },
            /* For now allowing all the atypes, QoS, orderId and priority */
            .host_cfg_entries = {
             [0] = {
                    /* Allowed atype configuration for the host ID. The host ID
                     * gets assigned a list of atypes which are allowed. atype
                     * is a 2-bit field with 3 possible values. Thus in one
                     * 8-bit word, flags are set specifying whether or not an
                     * atype value is allowed for the host ID. For each atype,
                     * the value of 01b means not allowed, 10b means allowed,
                     * and 11b and 00b are invalid/errors. These are encoded in
                     * a bitfield because there is one set of allowed atypes
                     * for every host ID.
                     */
                    .allowed_atype = 0b101010,
                    /* Allowed QoS level configuration for host ID. The host
                     * ID gets assigned a list of QoS levels which are allowed.
                     * As QoS level is a 3-bit field, there are 8 possible
                     * order-IDs. Thus in one 16-bit word, flags are set
                     * specifying whether or not the QoS level is allowed for
                     * the host ID. For each QoS level, the value of 01b means
                     * not allowed, 10b means allowed, and 11b and 00b are
                     * invalid/errors. These are encoded in a bitfield because
                     * there is one set of allowed QoS levels for every host ID.
                     */
                    .allowed_qos   = 0xAAAA,
                    /* Allowed order-ID configuration for the host ID. The host
                     * ID gets assigned a list of order-IDs which are allowed.
                     * As order-ID is a 4-bit field, there are 16 possible
                     * order-IDs. Thus in one 32-bit word, flags are set
                     * specifying whether or not the order-ID is allowed for
                     * the host ID. For each order-ID, the value of 01b means
                     * not allowed, 10b means allowed, and 11b and 00b are
                     * invalid/errors. These are encoded in a bitfield because
                     * there is one set of allowed order-IDs for every host ID.
                     */
                    .allowed_orderid = 0xAAAAAAAA,
                    /* Allowed bus priority configuration for host ID. The host
                     * ID gets assigned a list of bus priorities which are
                     * allowed. As bus priority is a 3-bit field, there are 8
                     * possible bus priorities. Thus in one 16-bit word, flags
                     * are set specifying whether or not the bus priority is
                     * allowed for the host ID. For each bus priority, the
                     * value of 01b means not allowed, 10b means allowed, and
                     * 11b and 00b are invalid/errors. These are encoded in a
                     * bitfield because there is one set of allowed bus
                     * priorities for every host ID.
                     */
                    .allowed_priority = 0xAAAA,
                    /* Allowed UDMAP channel scheduling priority configuration
                     * for host ID. The host ID gets assigned a list of UDMAP
                     * channel scheduling priorities which are allowed. As
                     * UDMAP channel scheduling priority is a 2-bit field,
                     * there are 4 possible UDMAP channel scheduling priorities.
                     * Thus in one 8-bit word, flags are set specifying whether
                     * or not UDMAP channel scheduling priority is allowed for
                     * the host ID. For each priority, the value of 01b means
                     * not allowed, 10b means allowed, and 11b and 00b are
                     * invalid/errors. These are encoded in a bitfield because
                     * there is one set of allowed UDMAP channel scheduling
                     * priorities for every host ID.
                     */
                    .allowed_sched_priority = 0xAA
                    },
                [1] = {
                        .allowed_atype = 0b101010,
                        .allowed_qos   = 0xAAAA,
                        .allowed_orderid = 0xAAAAAAAA,
                        .allowed_priority = 0xAAAA,
                        .allowed_sched_priority = 0xAA
                    },
                [2] = {
                        .allowed_atype = 0b101010,
                        .allowed_qos   = 0xAAAA,
                        .allowed_orderid = 0xAAAAAAAA,
                        .allowed_priority = 0xAAAA,
                        .allowed_sched_priority = 0xAA
                    },
                [3] = {
                        .allowed_atype = 0b101010,
                        .allowed_qos   = 0xAAAA,
                        .allowed_orderid = 0xAAAAAAAA,
                        .allowed_priority = 0xAAAA,
                        .allowed_sched_priority = 0xAA
                    },
                [4] = {
                        .allowed_atype = 0b101010,
                        .allowed_qos   = 0xAAAA,
                        .allowed_orderid = 0xAAAAAAAA,
                        .allowed_priority = 0xAAAA,
                        .allowed_sched_priority = 0xAA
                    },
                [5] = {
                        .allowed_atype = 0b101010,
                        .allowed_qos   = 0xAAAA,
                        .allowed_orderid = 0xAAAAAAAA,
                        .allowed_priority = 0xAAAA,
                        .allowed_sched_priority = 0xAA
                    },
                [6] = {
                        .allowed_atype = 0b101010,
                        .allowed_qos   = 0xAAAA,
                        .allowed_orderid = 0xAAAAAAAA,
                        .allowed_priority = 0xAAAA,
                        .allowed_sched_priority = 0xAA
                    },
                [7] = {
                        .allowed_atype = 0b101010,
                        .allowed_qos   = 0xAAAA,
                        .allowed_orderid = 0xAAAAAAAA,
                        .allowed_priority = 0xAAAA,
                        .allowed_sched_priority = 0xAA
                    },
                [8] = {
                        .allowed_atype = 0b101010,
                        .allowed_qos   = 0xAAAA,
                        .allowed_orderid = 0xAAAAAAAA,
                        .allowed_priority = 0xAAAA,
                        .allowed_sched_priority = 0xAA
                    },
                [9] = {
                        .allowed_atype = 0b101010,
                        .allowed_qos   = 0xAAAA,
                        .allowed_orderid = 0xAAAAAAAA,
                        .allowed_priority = 0xAAAA,
                        .allowed_sched_priority = 0xAA
                    },
                [10] = {
                        .allowed_atype = 0b101010,
                        .allowed_qos   = 0xAAAA,
                        .allowed_orderid = 0xAAAAAAAA,
                        .allowed_priority = 0xAAAA,
                        .allowed_sched_priority = 0xAA
                    },
                [11] = {
                        .allowed_atype = 0b101010,
                        .allowed_qos   = 0xAAAA,
                        .allowed_orderid = 0xAAAAAAAA,
                        .allowed_priority = 0xAAAA,
                        .allowed_sched_priority = 0xAA
                    },
                [12] = {
                        .allowed_atype = 0b101010,
                        .allowed_qos   = 0xAAAA,
                        .allowed_orderid = 0xAAAAAAAA,
                        .allowed_priority = 0xAAAA,
                        .allowed_sched_priority = 0xAA
                    },
                [13] = {
                        .allowed_atype = 0b101010,
                        .allowed_qos   = 0xAAAA,
                        .allowed_orderid = 0xAAAAAAAA,
                        .allowed_priority = 0xAAAA,
                        .allowed_sched_priority = 0xAA
                    },
                [14] = {
                        .allowed_atype = 0b101010,
                        .allowed_qos   = 0xAAAA,
                        .allowed_orderid = 0xAAAAAAAA,
                        .allowed_priority = 0xAAAA,
                        .allowed_sched_priority = 0xAA
                    },
                [15] = {
                        .allowed_atype = 0b101010,
                        .allowed_qos   = 0xAAAA,
                        .allowed_orderid = 0xAAAAAAAA,
                        .allowed_priority = 0xAAAA,
                        .allowed_sched_priority = 0xAA
                    },
                [16] = {
                        .allowed_atype = 0b101010,
                        .allowed_qos   = 0xAAAA,
                        .allowed_orderid = 0xAAAAAAAA,
                        .allowed_priority = 0xAAAA,
                        .allowed_sched_priority = 0xAA
                    },
                [17] = {
                        .allowed_atype = 0b101010,
                        .allowed_qos   = 0xAAAA,
                        .allowed_orderid = 0xAAAAAAAA,
                        .allowed_priority = 0xAAAA,
                        .allowed_sched_priority = 0xAA
                    },
                [18] = {
                        .allowed_atype = 0b101010,
                        .allowed_qos   = 0xAAAA,
                        .allowed_orderid = 0xAAAAAAAA,
                        .allowed_priority = 0xAAAA,
                        .allowed_sched_priority = 0xAA
                    },
                [19] = {
                        .allowed_atype = 0b101010,
                        .allowed_qos   = 0xAAAA,
                        .allowed_orderid = 0xAAAAAAAA,
                        .allowed_priority = 0xAAAA,
                        .allowed_sched_priority = 0xAA
                    },
                [20] = {
                        .allowed_atype = 0b101010,
                        .allowed_qos   = 0xAAAA,
                        .allowed_orderid = 0xAAAAAAAA,
                        .allowed_priority = 0xAAAA,
                        .allowed_sched_priority = 0xAA
                    },
                [21] = {
                        .allowed_atype = 0b101010,
                        .allowed_qos   = 0xAAAA,
                        .allowed_orderid = 0xAAAAAAAA,
                        .allowed_priority = 0xAAAA,
                        .allowed_sched_priority = 0xAA
                    },
                [22] = {
                        .allowed_atype = 0b101010,
                        .allowed_qos   = 0xAAAA,
                        .allowed_orderid = 0xAAAAAAAA,
                        .allowed_priority = 0xAAAA,
                        .allowed_sched_priority = 0xAA
                    },
                [23] = {
                        .allowed_atype = 0b101010,
                        .allowed_qos   = 0xAAAA,
                        .allowed_orderid = 0xAAAAAAAA,
                        .allowed_priority = 0xAAAA,
                        .allowed_sched_priority = 0xAA
                    },
                [24] = {
                        .allowed_atype = 0b101010,
                        .allowed_qos   = 0xAAAA,
                        .allowed_orderid = 0xAAAAAAAA,
                        .allowed_priority = 0xAAAA,
                        .allowed_sched_priority = 0xAA
                    },
                [25] = {
                        .allowed_atype = 0b101010,
                        .allowed_qos   = 0xAAAA,
                        .allowed_orderid = 0xAAAAAAAA,
                        .allowed_priority = 0xAAAA,
                        .allowed_sched_priority = 0xAA
                    },
                [26] = {
                        .allowed_atype = 0b101010,
                        .allowed_qos   = 0xAAAA,
                        .allowed_orderid = 0xAAAAAAAA,
                        .allowed_priority = 0xAAAA,
                        .allowed_sched_priority = 0xAA
                    },
                [27] = {
                        .allowed_atype = 0b101010,
                        .allowed_qos   = 0xAAAA,
                        .allowed_orderid = 0xAAAAAAAA,
                        .allowed_priority = 0xAAAA,
                        .allowed_sched_priority = 0xAA
                    },
                [28] = {
                        .allowed_atype = 0b101010,
                        .allowed_qos   = 0xAAAA,
                        .allowed_orderid = 0xAAAAAAAA,
                        .allowed_priority = 0xAAAA,
                        .allowed_sched_priority = 0xAA
                    },
                [29] = {
                        .allowed_atype = 0b101010,
                        .allowed_qos   = 0xAAAA,
                        .allowed_orderid = 0xAAAAAAAA,
                        .allowed_priority = 0xAAAA,
                        .allowed_sched_priority = 0xAA
                    },
                [30] = {
                        .allowed_atype = 0b101010,
                        .allowed_qos   = 0xAAAA,
                        .allowed_orderid = 0xAAAAAAAA,
                        .allowed_priority = 0xAAAA,
                        .allowed_sched_priority = 0xAA
                    },
                [31] = {
                        .allowed_atype = 0b101010,
                        .allowed_qos   = 0xAAAA,
                        .allowed_orderid = 0xAAAAAAAA,
                        .allowed_priority = 0xAAAA,
                        .allowed_sched_priority = 0xAA
                    }
            },
        },
        .resasg = {
            .subhdr = {
                .magic = TISCI_BOARDCFG_RM_RESASG_MAGIC_NUM,
                .size  = sizeof(struct tisci_boardcfg_rm_resasg),
            },
            .resasg_entries_size = TISCI_RESASG_UTYPE_CNT_PG2 *
                    sizeof(struct tisci_boardcfg_rm_resasg_entry),
        },
    },
.resasg_entries = {
        {
            .num_resource = 1024,
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_NAVSS0_MODSS_INTA0, TISCI_RESASG_SUBTYPE_GLOBAL_EVENT_SEVT),
            .start_resource = 20480,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 64U,
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_NAVSS0_MODSS_INTA0, TISCI_RESASG_SUBTYPE_IA_VINT),
            .start_resource = 0U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 1024,
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_NAVSS0_MODSS_INTA1, TISCI_RESASG_SUBTYPE_GLOBAL_EVENT_SEVT),
            .start_resource = 22528,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 64U,
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_NAVSS0_MODSS_INTA1, TISCI_RESASG_SUBTYPE_IA_VINT),
            .start_resource = 0U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 4592,
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_NAVSS0_UDMASS_INTA0, TISCI_RESASG_SUBTYPE_GLOBAL_EVENT_SEVT),
            .start_resource = 16,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 240U,
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_NAVSS0_UDMASS_INTA0, TISCI_RESASG_SUBTYPE_IA_VINT),
            .start_resource = 16U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 1528,
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_MCU_NAVSS0_INTR_AGGR_0, TISCI_RESASG_SUBTYPE_GLOBAL_EVENT_SEVT),
            .start_resource = 16392,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 248U,
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_MCU_NAVSS0_INTR_AGGR_0, TISCI_RESASG_SUBTYPE_IA_VINT),
            .start_resource = 8U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 28U,
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_MCU_ARMSS0_CPU1, TISCI_RESASG_SUBTYPE_MCU_ARMSS0_CPU1_INTR_IRQ_GROUP0_FROM_MCU_NAVSS0_INTR_ROUTER_0),
            .start_resource = 68U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 16U,
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_MCU_ARMSS0_CPU1, TISCI_RESASG_SUBTYPE_MCU_ARMSS0_CPU1_INTR_IRQ_GROUP0_FROM_WKUP_GPIOMUX_INTRTR0),
            .start_resource = 124U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 64U,
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_MCU_ARMSS0_CPU1, TISCI_RESASG_SUBTYPE_MCU_ARMSS0_CPU1_INTR_IRQ_GROUP0_FROM_MAIN2MCU_LVL_INTRTR0),
            .start_resource = 160U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 48U,
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_MCU_ARMSS0_CPU1, TISCI_RESASG_SUBTYPE_MCU_ARMSS0_CPU1_INTR_IRQ_GROUP0_FROM_MAIN2MCU_PLS_INTRTR0),
            .start_resource = 224U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 48U,
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_GIC0, TISCI_RESASG_SUBTYPE_GIC0_SPI_IRQ_GROUP0_FROM_NAVSS0_INTR_ROUTER_0),
            .start_resource = 80U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 32U,
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_GIC0, TISCI_RESASG_SUBTYPE_GIC0_SPI_IRQ_GROUP0_FROM_GPIOMUX_INTRTR0),
            .start_resource = 392U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 56U,
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_GIC0, TISCI_RESASG_SUBTYPE_GIC0_SPI_IRQ_GROUP1_FROM_NAVSS0_INTR_ROUTER_0),
            .start_resource = 448U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 16U,
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_GIC0, TISCI_RESASG_SUBTYPE_GIC0_SPI_IRQ_GROUP0_FROM_CMPEVENT_INTRTR0),
            .start_resource = 544U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 16U,
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_GIC0, TISCI_RESASG_SUBTYPE_GIC0_SPI_IRQ_GROUP0_FROM_WKUP_GPIOMUX_INTRTR0),
            .start_resource = 712U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 28U,
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_MCU_ARMSS0_CPU0, TISCI_RESASG_SUBTYPE_MCU_ARMSS0_CPU0_INTR_IRQ_GROUP0_FROM_MCU_NAVSS0_INTR_ROUTER_0),
            .start_resource = 68U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 16U,
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_MCU_ARMSS0_CPU0, TISCI_RESASG_SUBTYPE_MCU_ARMSS0_CPU0_INTR_IRQ_GROUP0_FROM_WKUP_GPIOMUX_INTRTR0),
            .start_resource = 124U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 64U,
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_MCU_ARMSS0_CPU0, TISCI_RESASG_SUBTYPE_MCU_ARMSS0_CPU0_INTR_IRQ_GROUP0_FROM_MAIN2MCU_LVL_INTRTR0),
            .start_resource = 160U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 48U,
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_MCU_ARMSS0_CPU0, TISCI_RESASG_SUBTYPE_MCU_ARMSS0_CPU0_INTR_IRQ_GROUP0_FROM_MAIN2MCU_PLS_INTRTR0),
            .start_resource = 224U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 1U,
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_NAVSS0, TISCI_RESASG_SUBTYPE_NAVSS0_CPTS0_HW1_PUSH_IRQ_GROUP0_FROM_TIMESYNC_INTRTR0),
            .start_resource = 0U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 1U,
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_NAVSS0, TISCI_RESASG_SUBTYPE_NAVSS0_CPTS0_HW2_PUSH_IRQ_GROUP0_FROM_TIMESYNC_INTRTR0),
            .start_resource = 1U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 1U,
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_NAVSS0, TISCI_RESASG_SUBTYPE_NAVSS0_CPTS0_HW3_PUSH_IRQ_GROUP0_FROM_TIMESYNC_INTRTR0),
            .start_resource = 2U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 1U,
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_NAVSS0, TISCI_RESASG_SUBTYPE_NAVSS0_CPTS0_HW4_PUSH_IRQ_GROUP0_FROM_TIMESYNC_INTRTR0),
            .start_resource = 3U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 1U,
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_NAVSS0, TISCI_RESASG_SUBTYPE_NAVSS0_CPTS0_HW5_PUSH_IRQ_GROUP0_FROM_TIMESYNC_INTRTR0),
            .start_resource = 4U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 1U,
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_NAVSS0, TISCI_RESASG_SUBTYPE_NAVSS0_CPTS0_HW6_PUSH_IRQ_GROUP0_FROM_TIMESYNC_INTRTR0),
            .start_resource = 5U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 1U,
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_NAVSS0, TISCI_RESASG_SUBTYPE_NAVSS0_CPTS0_HW7_PUSH_IRQ_GROUP0_FROM_TIMESYNC_INTRTR0),
            .start_resource = 6U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 1U,
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_NAVSS0, TISCI_RESASG_SUBTYPE_NAVSS0_CPTS0_HW8_PUSH_IRQ_GROUP0_FROM_TIMESYNC_INTRTR0),
            .start_resource = 7U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 8U,
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_ESM0, TISCI_RESASG_SUBTYPE_ESM0_ESM_PLS_EVENT0_IRQ_GROUP0_FROM_GPIOMUX_INTRTR0),
            .start_resource = 248U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 8U,
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_ESM0, TISCI_RESASG_SUBTYPE_ESM0_ESM_PLS_EVENT1_IRQ_GROUP0_FROM_GPIOMUX_INTRTR0),
            .start_resource = 256U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 8U,
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_ESM0, TISCI_RESASG_SUBTYPE_ESM0_ESM_PLS_EVENT2_IRQ_GROUP0_FROM_GPIOMUX_INTRTR0),
            .start_resource = 264U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 8U,
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_PDMA1, TISCI_RESASG_SUBTYPE_PDMA1_LEVENT_IN_IRQ_GROUP0_FROM_TIMESYNC_INTRTR0),
            .start_resource = 0U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 8U,
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_PDMA1, TISCI_RESASG_SUBTYPE_PDMA1_LEVENT_IN_IRQ_GROUP0_FROM_CMPEVENT_INTRTR0),
            .start_resource = 8U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 1U,
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_PRU_ICSSG0, TISCI_RESASG_SUBTYPE_PRU_ICSSG0_PR1_EDC0_LATCH0_IN_IRQ_GROUP0_FROM_TIMESYNC_INTRTR0),
            .start_resource = 0U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 1U,
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_PRU_ICSSG0, TISCI_RESASG_SUBTYPE_PRU_ICSSG0_PR1_EDC0_LATCH1_IN_IRQ_GROUP0_FROM_TIMESYNC_INTRTR0),
            .start_resource = 1U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 1U,
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_PRU_ICSSG0, TISCI_RESASG_SUBTYPE_PRU_ICSSG0_PR1_EDC1_LATCH0_IN_IRQ_GROUP0_FROM_TIMESYNC_INTRTR0),
            .start_resource = 2U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 1U,
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_PRU_ICSSG0, TISCI_RESASG_SUBTYPE_PRU_ICSSG0_PR1_EDC1_LATCH1_IN_IRQ_GROUP0_FROM_TIMESYNC_INTRTR0),
            .start_resource = 3U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 6U,
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_PRU_ICSSG0, TISCI_RESASG_SUBTYPE_PRU_ICSSG0_PR1_IEP0_CAP_INTR_REQ_IRQ_GROUP0_FROM_GPIOMUX_INTRTR0),
            .start_resource = 4U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 6U,
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_PRU_ICSSG0, TISCI_RESASG_SUBTYPE_PRU_ICSSG0_PR1_IEP1_CAP_INTR_REQ_IRQ_GROUP0_FROM_GPIOMUX_INTRTR0),
            .start_resource = 10U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 8U,
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_PRU_ICSSG0, TISCI_RESASG_SUBTYPE_PRU_ICSSG0_PR1_SLV_INTR_IRQ_GROUP0_FROM_NAVSS0_INTR_ROUTER_0),
            .start_resource = 46U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 8U,
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_PRU_ICSSG0, TISCI_RESASG_SUBTYPE_PRU_ICSSG0_PR1_SLV_INTR_IRQ_GROUP0_FROM_GPIOMUX_INTRTR0),
            .start_resource = 88U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 1U,
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_MCU_CPSW0, TISCI_RESASG_SUBTYPE_MCU_CPSW0_CPTS_HW3_PUSH_IRQ_GROUP0_FROM_TIMESYNC_INTRTR0),
            .start_resource = 0U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 1U,
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_MCU_CPSW0, TISCI_RESASG_SUBTYPE_MCU_CPSW0_CPTS_HW4_PUSH_IRQ_GROUP0_FROM_TIMESYNC_INTRTR0),
            .start_resource = 1U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 1U,
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_PRU_ICSSG2, TISCI_RESASG_SUBTYPE_PRU_ICSSG2_PR1_EDC0_LATCH0_IN_IRQ_GROUP0_FROM_TIMESYNC_INTRTR0),
            .start_resource = 0U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 1U,
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_PRU_ICSSG2, TISCI_RESASG_SUBTYPE_PRU_ICSSG2_PR1_EDC0_LATCH1_IN_IRQ_GROUP0_FROM_TIMESYNC_INTRTR0),
            .start_resource = 1U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 1U,
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_PRU_ICSSG2, TISCI_RESASG_SUBTYPE_PRU_ICSSG2_PR1_EDC1_LATCH0_IN_IRQ_GROUP0_FROM_TIMESYNC_INTRTR0),
            .start_resource = 2U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 1U,
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_PRU_ICSSG2, TISCI_RESASG_SUBTYPE_PRU_ICSSG2_PR1_EDC1_LATCH1_IN_IRQ_GROUP0_FROM_TIMESYNC_INTRTR0),
            .start_resource = 3U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 6U,
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_PRU_ICSSG2, TISCI_RESASG_SUBTYPE_PRU_ICSSG2_PR1_IEP0_CAP_INTR_REQ_IRQ_GROUP0_FROM_GPIOMUX_INTRTR0),
            .start_resource = 4U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 6U,
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_PRU_ICSSG2, TISCI_RESASG_SUBTYPE_PRU_ICSSG2_PR1_IEP1_CAP_INTR_REQ_IRQ_GROUP0_FROM_GPIOMUX_INTRTR0),
            .start_resource = 10U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 8U,
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_PRU_ICSSG2, TISCI_RESASG_SUBTYPE_PRU_ICSSG2_PR1_SLV_INTR_IRQ_GROUP0_FROM_NAVSS0_INTR_ROUTER_0),
            .start_resource = 46U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 8U,
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_PRU_ICSSG2, TISCI_RESASG_SUBTYPE_PRU_ICSSG2_PR1_SLV_INTR_IRQ_GROUP0_FROM_GPIOMUX_INTRTR0),
            .start_resource = 88U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 8U,
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_WKUP_ESM0, TISCI_RESASG_SUBTYPE_WKUP_ESM0_ESM_PLS_EVENT0_IRQ_GROUP0_FROM_WKUP_GPIOMUX_INTRTR0),
            .start_resource = 88U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 8U,
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_WKUP_ESM0, TISCI_RESASG_SUBTYPE_WKUP_ESM0_ESM_PLS_EVENT1_IRQ_GROUP0_FROM_WKUP_GPIOMUX_INTRTR0),
            .start_resource = 96U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 8U,
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_WKUP_ESM0, TISCI_RESASG_SUBTYPE_WKUP_ESM0_ESM_PLS_EVENT2_IRQ_GROUP0_FROM_WKUP_GPIOMUX_INTRTR0),
            .start_resource = 104U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 12U,
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_WKUP_DMSC0_CORTEX_M3_0, TISCI_RESASG_SUBTYPE_WKUP_DMSC0_CORTEX_M3_0_NVIC_IRQ_GROUP0_FROM_WKUP_GPIOMUX_INTRTR0),
            .start_resource = 184U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 1U,
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_PCIE0, TISCI_RESASG_SUBTYPE_PCIE0_PCIE_CPTS_HW2_PUSH_IRQ_GROUP0_FROM_TIMESYNC_INTRTR0),
            .start_resource = 0U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 1U,
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_PCIE1, TISCI_RESASG_SUBTYPE_PCIE1_PCIE_CPTS_HW2_PUSH_IRQ_GROUP0_FROM_TIMESYNC_INTRTR0),
            .start_resource = 0U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 1U,
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_PRU_ICSSG1, TISCI_RESASG_SUBTYPE_PRU_ICSSG1_PR1_EDC0_LATCH0_IN_IRQ_GROUP0_FROM_TIMESYNC_INTRTR0),
            .start_resource = 0U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 1U,
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_PRU_ICSSG1, TISCI_RESASG_SUBTYPE_PRU_ICSSG1_PR1_EDC0_LATCH1_IN_IRQ_GROUP0_FROM_TIMESYNC_INTRTR0),
            .start_resource = 1U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 1U,
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_PRU_ICSSG1, TISCI_RESASG_SUBTYPE_PRU_ICSSG1_PR1_EDC1_LATCH0_IN_IRQ_GROUP0_FROM_TIMESYNC_INTRTR0),
            .start_resource = 2U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 1U,
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_PRU_ICSSG1, TISCI_RESASG_SUBTYPE_PRU_ICSSG1_PR1_EDC1_LATCH1_IN_IRQ_GROUP0_FROM_TIMESYNC_INTRTR0),
            .start_resource = 3U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 6U,
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_PRU_ICSSG1, TISCI_RESASG_SUBTYPE_PRU_ICSSG1_PR1_IEP0_CAP_INTR_REQ_IRQ_GROUP0_FROM_GPIOMUX_INTRTR0),
            .start_resource = 4U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 6U,
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_PRU_ICSSG1, TISCI_RESASG_SUBTYPE_PRU_ICSSG1_PR1_IEP1_CAP_INTR_REQ_IRQ_GROUP0_FROM_GPIOMUX_INTRTR0),
            .start_resource = 10U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 8U,
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_PRU_ICSSG1, TISCI_RESASG_SUBTYPE_PRU_ICSSG1_PR1_SLV_INTR_IRQ_GROUP0_FROM_NAVSS0_INTR_ROUTER_0),
            .start_resource = 46U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 8U,
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_PRU_ICSSG1, TISCI_RESASG_SUBTYPE_PRU_ICSSG1_PR1_SLV_INTR_IRQ_GROUP0_FROM_GPIOMUX_INTRTR0),
            .start_resource = 88U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 63U,
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_NAVSS0_PROXY0, TISCI_RESASG_SUBTYPE_PROXY_PROXIES),
            .start_resource = 1U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 64U,
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_MCU_NAVSS0_PROXY0, TISCI_RESASG_SUBTYPE_PROXY_PROXIES),
            .start_resource = 0U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 7U,
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_NAVSS0_RINGACC0, TISCI_RESASG_SUBTYPE_RA_UDMAP_TX_H),
            .start_resource = 1U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 112U,
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_NAVSS0_RINGACC0, TISCI_RESASG_SUBTYPE_RA_UDMAP_TX),
            .start_resource = 8U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 32U,
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_NAVSS0_RINGACC0, TISCI_RESASG_SUBTYPE_RA_UDMAP_TX_EXT),
            .start_resource = 120U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 7U,
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_NAVSS0_RINGACC0, TISCI_RESASG_SUBTYPE_RA_UDMAP_RX_H),
            .start_resource = 153U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 142U,
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_NAVSS0_RINGACC0, TISCI_RESASG_SUBTYPE_RA_UDMAP_RX),
            .start_resource = 160U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 464U,
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_NAVSS0_RINGACC0, TISCI_RESASG_SUBTYPE_RA_GP),
            .start_resource = 304U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 2U,
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_MCU_NAVSS0_RINGACC0, TISCI_RESASG_SUBTYPE_RA_UDMAP_TX_H),
            .start_resource = 0U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 46U,
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_MCU_NAVSS0_RINGACC0, TISCI_RESASG_SUBTYPE_RA_UDMAP_TX),
            .start_resource = 2U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 2U,
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_MCU_NAVSS0_RINGACC0, TISCI_RESASG_SUBTYPE_RA_UDMAP_RX_H),
            .start_resource = 48U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 46U,
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_MCU_NAVSS0_RINGACC0, TISCI_RESASG_SUBTYPE_RA_UDMAP_RX),
            .start_resource = 50U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 160U,
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_MCU_NAVSS0_RINGACC0, TISCI_RESASG_SUBTYPE_RA_GP),
            .start_resource = 96U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 1U,
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_NAVSS0_RINGACC0, TISCI_RESASG_SUBTYPE_RA_ERROR_OES),
            .start_resource = 0U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 64U,
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_NAVSS0_RINGACC0, TISCI_RESASG_SUBTYPE_RA_VIRTID),
            .start_resource = 64U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 32U,
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_NAVSS0_RINGACC0, TISCI_RESASG_SUBTYPE_RA_MONITORS),
            .start_resource = 0U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 1U,
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_MCU_NAVSS0_RINGACC0, TISCI_RESASG_SUBTYPE_RA_ERROR_OES),
            .start_resource = 0U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 64U,
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_MCU_NAVSS0_RINGACC0, TISCI_RESASG_SUBTYPE_RA_VIRTID),
            .start_resource = 128U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 32U,
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_MCU_NAVSS0_RINGACC0, TISCI_RESASG_SUBTYPE_RA_MONITORS),
            .start_resource = 0U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 7U,
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_NAVSS0_UDMAP0, TISCI_RESASG_SUBTYPE_UDMAP_TX_HCHAN),
            .start_resource = 1U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 112U,
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_NAVSS0_UDMAP0, TISCI_RESASG_SUBTYPE_UDMAP_TX_CHAN),
            .start_resource = 8U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 32U,
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_NAVSS0_UDMAP0, TISCI_RESASG_SUBTYPE_UDMAP_TX_ECHAN),
            .start_resource = 120U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 6U,
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_NAVSS0_UDMAP0, TISCI_RESASG_SUBTYPE_UDMAP_RX_HCHAN),
            .start_resource = 2U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 142U,
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_NAVSS0_UDMAP0, TISCI_RESASG_SUBTYPE_UDMAP_RX_CHAN),
            .start_resource = 8U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 1024U,
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_NAVSS0_UDMAP0, TISCI_RESASG_SUBTYPE_GLOBAL_EVENT_TRIGGER),
            .start_resource = 49152U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 2U,
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_MCU_NAVSS0_UDMAP0, TISCI_RESASG_SUBTYPE_UDMAP_TX_HCHAN),
            .start_resource = 0U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 46U,
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_MCU_NAVSS0_UDMAP0, TISCI_RESASG_SUBTYPE_UDMAP_TX_CHAN),
            .start_resource = 2U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 2U,
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_MCU_NAVSS0_UDMAP0, TISCI_RESASG_SUBTYPE_UDMAP_RX_HCHAN),
            .start_resource = 0U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 46U,
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_MCU_NAVSS0_UDMAP0, TISCI_RESASG_SUBTYPE_UDMAP_RX_CHAN),
            .start_resource = 2U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 256U,
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_MCU_NAVSS0_UDMAP0, TISCI_RESASG_SUBTYPE_GLOBAL_EVENT_TRIGGER),
            .start_resource = 56320U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 150U,
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_NAVSS0_UDMAP0, TISCI_RESASG_SUBTYPE_UDMAP_RX_FLOW_COMMON),
            .start_resource = 150U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 1U,
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_NAVSS0_UDMAP0, TISCI_RESASG_SUBTYPE_UDMAP_INVALID_FLOW_OES),
            .start_resource = 0U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 1U,
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_NAVSS0_UDMAP0, TISCI_RESASG_SUBTYPE_UDMAP_GLOBAL_CONFIG),
            .start_resource = 0U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 48U,
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_MCU_NAVSS0_UDMAP0, TISCI_RESASG_SUBTYPE_UDMAP_RX_FLOW_COMMON),
            .start_resource = 48U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 1U,
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_MCU_NAVSS0_UDMAP0, TISCI_RESASG_SUBTYPE_UDMAP_INVALID_FLOW_OES),
            .start_resource = 0U,
            .host_id = TISCI_HOST_ID_ALL,
        },
        {
            .num_resource = 1U,
            .type = TISCI_RESASG_UTYPE(TISCI_DEV_MCU_NAVSS0_UDMAP0, TISCI_RESASG_SUBTYPE_UDMAP_GLOBAL_CONFIG),
            .start_resource = 0U,
            .host_id = TISCI_HOST_ID_ALL,
        },
    },
};
#endif
