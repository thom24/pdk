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
 *  \file V0/sciclient_defaultBoardcfg_rm_linux.c
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
const struct tisci_local_rm_boardcfg gSciclient_boardCfgLow_rm
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
            .resasg_entries_size = 158U * sizeof(struct tisci_boardcfg_rm_resasg_entry),
        },
    },
    .resasg_entries = {
        /* compare event IR */
        {
            .start_resource = 0,
            .num_resource = 12,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_CMPEVENT_INTRTR0,
					TISCI_RESASG_SUBTYPE_IR_OUTPUT),
            .host_id = TISCI_HOST_ID_A53_2,
        },
        {
            .start_resource = 12,
            .num_resource = 4,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_CMPEVENT_INTRTR0,
					TISCI_RESASG_SUBTYPE_IR_OUTPUT),
            .host_id = TISCI_HOST_ID_A53_3,
        },
        {
            .start_resource = 16,
            .num_resource = 8,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_CMPEVENT_INTRTR0,
					TISCI_RESASG_SUBTYPE_IR_OUTPUT),
            .host_id = TISCI_HOST_ID_R5_0,
        },
        {
            .start_resource = 24,
            .num_resource = 8,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_CMPEVENT_INTRTR0,
					TISCI_RESASG_SUBTYPE_IR_OUTPUT),
            .host_id = TISCI_HOST_ID_R5_2,
        },

        /* Main 2 MCU level IRQ IR */
        {
            .start_resource = 0,
            .num_resource = 32,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
					TISCI_RESASG_SUBTYPE_IR_OUTPUT),
            .host_id = TISCI_HOST_ID_R5_0,
        },
        {
            .start_resource = 32,
            .num_resource = 32,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
					TISCI_RESASG_SUBTYPE_IR_OUTPUT),
            .host_id = TISCI_HOST_ID_R5_2,
        },

        /* Main 2 MCU Pulse IRQ IR */
        {
            .start_resource = 0,
            .num_resource = 24,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_MAIN2MCU_PLS_INTRTR0,
					TISCI_RESASG_SUBTYPE_IR_OUTPUT),
            .host_id = TISCI_HOST_ID_R5_0,
        },
        {
            .start_resource = 24,
            .num_resource = 24,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_MAIN2MCU_PLS_INTRTR0,
					TISCI_RESASG_SUBTYPE_IR_OUTPUT),
            .host_id = TISCI_HOST_ID_R5_2,
        },

        /* Main GPIO IR */
        {
            .start_resource = 0,
            .num_resource = 20,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_GPIOMUX_INTRTR0,
					TISCI_RESASG_SUBTYPE_IR_OUTPUT),
            .host_id = TISCI_HOST_ID_A53_2,
        },
        {
            .start_resource = 20,
            .num_resource = 12,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_GPIOMUX_INTRTR0,
					TISCI_RESASG_SUBTYPE_IR_OUTPUT),
            .host_id = TISCI_HOST_ID_A53_3,
        },

        /* Timesync Router */
        {
            .start_resource = 0,
            .num_resource = 40,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_TIMESYNC_INTRTR0,
					TISCI_RESASG_SUBTYPE_IR_OUTPUT),
            .host_id = TISCI_HOST_ID_ALL,
        },

        /* WKUP GPIO IR */
        {
            .start_resource = 0,
            .num_resource = 4,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_WKUP_GPIOMUX_INTRTR0,
					TISCI_RESASG_SUBTYPE_IR_OUTPUT),
            .host_id = TISCI_HOST_ID_A53_2,
        },
        {
            .start_resource = 4,
            .num_resource = 4,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_WKUP_GPIOMUX_INTRTR0,
					TISCI_RESASG_SUBTYPE_IR_OUTPUT),
            .host_id = TISCI_HOST_ID_A53_3,
        },
        {
            .start_resource = 8,
            .num_resource = 4,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_WKUP_GPIOMUX_INTRTR0,
					TISCI_RESASG_SUBTYPE_IR_OUTPUT),
            .host_id = TISCI_HOST_ID_R5_0,
        },
        {
            .start_resource = 12,
            .num_resource = 4,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_WKUP_GPIOMUX_INTRTR0,
					TISCI_RESASG_SUBTYPE_IR_OUTPUT),
            .host_id = TISCI_HOST_ID_R5_2,
        },

        /* Main Nav IA VINT */
        {
            .start_resource = 16,
            .num_resource = 80,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_UDMASS_INTA0,
					TISCI_RESASG_SUBTYPE_IA_VINT),
            .host_id = TISCI_HOST_ID_A53_2,
        },
        {
            .start_resource = 96,
            .num_resource = 30,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_UDMASS_INTA0,
					TISCI_RESASG_SUBTYPE_IA_VINT),
            .host_id = TISCI_HOST_ID_A53_3,
        },
        {
            .start_resource = 126,
            .num_resource = 50,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_UDMASS_INTA0,
					TISCI_RESASG_SUBTYPE_IA_VINT),
            .host_id = TISCI_HOST_ID_R5_0,
        },
        {
            .start_resource = 176,
            .num_resource = 50,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_UDMASS_INTA0,
					TISCI_RESASG_SUBTYPE_IA_VINT),
            .host_id = TISCI_HOST_ID_R5_2,
        },
        {
            .start_resource = 226,
            .num_resource = 30,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_UDMASS_INTA0,
					TISCI_RESASG_SUBTYPE_IA_VINT),
            .host_id = TISCI_HOST_ID_ALL,
        },

        /* Main Nav IA global events */
        {
            .start_resource = 16,
            .num_resource = 1024,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_UDMASS_INTA0,
					TISCI_RESASG_SUBTYPE_GLOBAL_EVENT_SEVT),
            .host_id = TISCI_HOST_ID_A53_2,
        },
        {
            .start_resource = 1040,
            .num_resource = 512,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_UDMASS_INTA0,
					TISCI_RESASG_SUBTYPE_GLOBAL_EVENT_SEVT),
            .host_id = TISCI_HOST_ID_A53_3,
        },
        {
            .start_resource = 1552,
            .num_resource = 512,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_UDMASS_INTA0,
					TISCI_RESASG_SUBTYPE_GLOBAL_EVENT_SEVT),
            .host_id = TISCI_HOST_ID_R5_0,
        },
        {
            .start_resource = 2064,
            .num_resource = 512,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_UDMASS_INTA0,
					TISCI_RESASG_SUBTYPE_GLOBAL_EVENT_SEVT),
            .host_id = TISCI_HOST_ID_R5_2,
        },
        {
            .start_resource = 2576,
            .num_resource = 2032,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_UDMASS_INTA0,
					TISCI_RESASG_SUBTYPE_GLOBAL_EVENT_SEVT),
            .host_id = TISCI_HOST_ID_ALL,
        },

        /* MODSS INTA0 VINT */
        {
            .start_resource = 0,
            .num_resource = 64,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_MODSS_INTA0,
					TISCI_RESASG_SUBTYPE_IA_VINT),
            .host_id = TISCI_HOST_ID_ALL,
        },

        /* MODSS INTA0 global events */
        {
            .start_resource = 20480,
            .num_resource = 1024,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_MODSS_INTA0,
					TISCI_RESASG_SUBTYPE_GLOBAL_EVENT_SEVT),
            .host_id = TISCI_HOST_ID_ALL,
        },

        /* MODSS INTA1 VINT */
        {
            .start_resource = 0,
            .num_resource = 64,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_MODSS_INTA1,
					TISCI_RESASG_SUBTYPE_IA_VINT),
            .host_id = TISCI_HOST_ID_ALL,
        },

        /* MODSS INTA1 global events */
        {
            .start_resource = 22528,
            .num_resource = 1024,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_MODSS_INTA1,
					TISCI_RESASG_SUBTYPE_GLOBAL_EVENT_SEVT),
            .host_id = TISCI_HOST_ID_ALL,
        },

        /* Main Nav IR */
        {
            .start_resource = 16,
            .num_resource = 64,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_INTR_ROUTER_0,
					TISCI_RESASG_SUBTYPE_IR_OUTPUT),
            .host_id = TISCI_HOST_ID_A53_2,
        },
        {
            .start_resource = 80,
            .num_resource = 40,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_INTR_ROUTER_0,
					TISCI_RESASG_SUBTYPE_IR_OUTPUT),
            .host_id = TISCI_HOST_ID_A53_3,
        },
        {
            .start_resource = 120,
            .num_resource = 4,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_INTR_ROUTER_0,
					TISCI_RESASG_SUBTYPE_IR_OUTPUT),
            .host_id = TISCI_HOST_ID_R5_0,
        },
        {
            .start_resource = 124,
            .num_resource = 4,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_INTR_ROUTER_0,
					TISCI_RESASG_SUBTYPE_IR_OUTPUT),
            .host_id = TISCI_HOST_ID_R5_2,
        },
        {
            .start_resource = 128,
            .num_resource = 24,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_INTR_ROUTER_0,
					TISCI_RESASG_SUBTYPE_IR_OUTPUT),
            .host_id = TISCI_HOST_ID_ALL,
        },

        /* Main Nav nonsecure proxies */
        {
            .start_resource = 1,
            .num_resource = 12,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_PROXY0,
					TISCI_RESASG_SUBTYPE_PROXY_PROXIES),
            .host_id = TISCI_HOST_ID_A53_2,
        },
        {
            .start_resource = 13,
            .num_resource = 4,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_PROXY0,
					TISCI_RESASG_SUBTYPE_PROXY_PROXIES),
            .host_id = TISCI_HOST_ID_A53_3,
        },
        {
            .start_resource = 17,
            .num_resource = 16,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_PROXY0,
					TISCI_RESASG_SUBTYPE_PROXY_PROXIES),
            .host_id = TISCI_HOST_ID_R5_0,
        },
        {
            .start_resource = 33,
            .num_resource = 16,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_PROXY0,
					TISCI_RESASG_SUBTYPE_PROXY_PROXIES),
            .host_id = TISCI_HOST_ID_R5_2,
        },
        {
            .start_resource = 49,
            .num_resource = 15,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_PROXY0,
					TISCI_RESASG_SUBTYPE_PROXY_PROXIES),
            .host_id = TISCI_HOST_ID_ALL,
        },

        /* Main Nav Ring Error OES */
        {
            .start_resource = 0,
            .num_resource = 1,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_RINGACC0,
					TISCI_RESASG_SUBTYPE_RA_ERROR_OES),
            .host_id = TISCI_HOST_ID_ALL,
        },

        /* Main Nav GP Ring / Free Ring */
        {
            .start_resource = 304,
            .num_resource = 100,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_RINGACC0,
					TISCI_RESASG_SUBTYPE_RA_GP),
            .host_id = TISCI_HOST_ID_A53_2,
        },
        {
            .start_resource = 404,
            .num_resource = 50,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_RINGACC0,
					TISCI_RESASG_SUBTYPE_RA_GP),
            .host_id = TISCI_HOST_ID_A53_3,
        },
        {
            .start_resource = 454,
            .num_resource = 100,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_RINGACC0,
					TISCI_RESASG_SUBTYPE_RA_GP),
            .host_id = TISCI_HOST_ID_R5_0,
        },
        {
            .start_resource = 554,
            .num_resource = 100,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_RINGACC0,
					TISCI_RESASG_SUBTYPE_RA_GP),
            .host_id = TISCI_HOST_ID_R5_2,
        },
        {
            .start_resource = 654,
            .num_resource = 114,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_RINGACC0,
					TISCI_RESASG_SUBTYPE_RA_GP),
            .host_id = TISCI_HOST_ID_ALL,
        },

        /* Main Nav Normal Capacity RX ring */
        {
            .start_resource = 160,
            .num_resource = 64,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_RINGACC0,
					TISCI_RESASG_SUBTYPE_RA_UDMAP_RX),
            .host_id = TISCI_HOST_ID_A53_2,
        },
        {
            .start_resource = 224,
            .num_resource = 32,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_RINGACC0,
					TISCI_RESASG_SUBTYPE_RA_UDMAP_RX),
            .host_id = TISCI_HOST_ID_A53_3,
        },
        {
            .start_resource = 256,
            .num_resource = 20,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_RINGACC0,
					TISCI_RESASG_SUBTYPE_RA_UDMAP_RX),
            .host_id = TISCI_HOST_ID_R5_0,
        },
        {
            .start_resource = 276,
            .num_resource = 16,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_RINGACC0,
					TISCI_RESASG_SUBTYPE_RA_UDMAP_RX),
            .host_id = TISCI_HOST_ID_R5_2,
        },
        {
            .start_resource = 292,
            .num_resource = 10,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_RINGACC0,
					TISCI_RESASG_SUBTYPE_RA_UDMAP_RX),
            .host_id = TISCI_HOST_ID_ALL,
        },

        /* Main Nav Normal Capacity TX ring */
        {
            .start_resource = 8,
            .num_resource = 50,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_RINGACC0,
					TISCI_RESASG_SUBTYPE_RA_UDMAP_TX),
            .host_id = TISCI_HOST_ID_A53_2,
        },
        {
            .start_resource = 58,
            .num_resource = 16,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_RINGACC0,
					TISCI_RESASG_SUBTYPE_RA_UDMAP_TX),
            .host_id = TISCI_HOST_ID_A53_3,
        },
        {
            .start_resource = 74,
            .num_resource = 20,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_RINGACC0,
					TISCI_RESASG_SUBTYPE_RA_UDMAP_TX),
            .host_id = TISCI_HOST_ID_R5_0,
        },
        {
            .start_resource = 94,
            .num_resource = 16,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_RINGACC0,
					TISCI_RESASG_SUBTYPE_RA_UDMAP_TX),
            .host_id = TISCI_HOST_ID_R5_2,
        },
        {
            .start_resource = 110,
            .num_resource = 10,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_RINGACC0,
					TISCI_RESASG_SUBTYPE_RA_UDMAP_TX),
            .host_id = TISCI_HOST_ID_ALL,
        },

        /* Main Nav TX rings for extended channels */
        {
            .start_resource = 120,
            .num_resource = 4,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_RINGACC0,
					TISCI_RESASG_SUBTYPE_RA_UDMAP_TX_EXT),
            .host_id = TISCI_HOST_ID_A53_2,
        },
        {
            .start_resource = 124,
            .num_resource = 4,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_RINGACC0,
					TISCI_RESASG_SUBTYPE_RA_UDMAP_TX_EXT),
            .host_id = TISCI_HOST_ID_A53_3,
        },
        {
            .start_resource = 128,
            .num_resource = 12,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_RINGACC0,
					TISCI_RESASG_SUBTYPE_RA_UDMAP_TX_EXT),
            .host_id = TISCI_HOST_ID_R5_0,
        },
        {
            .start_resource = 140,
            .num_resource = 12,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_RINGACC0,
					TISCI_RESASG_SUBTYPE_RA_UDMAP_TX_EXT),
            .host_id = TISCI_HOST_ID_R5_2,
        },

        /* Main Nav High Capacity RX ring */
        {
            .start_resource = 154,
            .num_resource = 2,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_RINGACC0,
					TISCI_RESASG_SUBTYPE_RA_UDMAP_RX_H),
            .host_id = TISCI_HOST_ID_A53_2,
        },
        {
            .start_resource = 156,
            .num_resource = 2,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_RINGACC0,
					TISCI_RESASG_SUBTYPE_RA_UDMAP_RX_H),
            .host_id = TISCI_HOST_ID_R5_0,
        },
        {
            .start_resource = 158,
            .num_resource = 2,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_RINGACC0,
					TISCI_RESASG_SUBTYPE_RA_UDMAP_RX_H),
            .host_id = TISCI_HOST_ID_R5_2,
        },

        /* Main Nav High Capacity TX ring */
        {
            .start_resource = 1,
            .num_resource = 3,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_RINGACC0,
					TISCI_RESASG_SUBTYPE_RA_UDMAP_TX_H),
            .host_id = TISCI_HOST_ID_A53_2,
        },
        {
            .start_resource = 4,
            .num_resource = 2,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_RINGACC0,
					TISCI_RESASG_SUBTYPE_RA_UDMAP_TX_H),
            .host_id = TISCI_HOST_ID_R5_0,
        },
        {
            .start_resource = 6,
            .num_resource = 2,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_RINGACC0,
					TISCI_RESASG_SUBTYPE_RA_UDMAP_TX_H),
            .host_id = TISCI_HOST_ID_R5_2,
        },

        /* virt_id range */
        {
            .start_resource = 2,
            .num_resource = 1,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_RINGACC0,
					TISCI_RESASG_SUBTYPE_RA_VIRTID),
            .host_id = TISCI_HOST_ID_A53_2,
        },
        {
            .start_resource = 3,
            .num_resource = 1,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_RINGACC0,
					TISCI_RESASG_SUBTYPE_RA_VIRTID),
            .host_id = TISCI_HOST_ID_A53_3,
        },
        {
            .start_resource = 4,
            .num_resource = 4,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_RINGACC0,
					TISCI_RESASG_SUBTYPE_RA_VIRTID),
            .host_id = TISCI_HOST_ID_ALL,
        },

        /* Main Nav ring monitors */
        {
            .start_resource = 0,
            .num_resource = 8,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_RINGACC0,
					TISCI_RESASG_SUBTYPE_RA_MONITORS),
            .host_id = TISCI_HOST_ID_A53_2,
        },
        {
            .start_resource = 8,
            .num_resource = 4,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_RINGACC0,
					TISCI_RESASG_SUBTYPE_RA_MONITORS),
            .host_id = TISCI_HOST_ID_A53_3,
        },
        {
            .start_resource = 12,
            .num_resource = 8,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_RINGACC0,
					TISCI_RESASG_SUBTYPE_RA_MONITORS),
            .host_id = TISCI_HOST_ID_R5_0,
        },
        {
            .start_resource = 20,
            .num_resource = 8,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_RINGACC0,
					TISCI_RESASG_SUBTYPE_RA_MONITORS),
            .host_id = TISCI_HOST_ID_R5_2,
        },
        {
            .start_resource = 28,
            .num_resource = 4,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_RINGACC0,
					TISCI_RESASG_SUBTYPE_RA_MONITORS),
            .host_id = TISCI_HOST_ID_ALL,
        },

        /* Main Nav Free RX Flow */
        {
            .start_resource = 150,
            .num_resource = 64,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_UDMAP0,
					TISCI_RESASG_SUBTYPE_UDMAP_RX_FLOW_COMMON),
            .host_id = TISCI_HOST_ID_A53_2,
        },
        {
            .start_resource = 214,
            .num_resource = 8,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_UDMAP0,
					TISCI_RESASG_SUBTYPE_UDMAP_RX_FLOW_COMMON),
            .host_id = TISCI_HOST_ID_A53_3,
        },
        {
            .start_resource = 222,
            .num_resource = 64,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_UDMAP0,
					TISCI_RESASG_SUBTYPE_UDMAP_RX_FLOW_COMMON),
            .host_id = TISCI_HOST_ID_R5_0,
        },
        {
            .start_resource = 286,
            .num_resource = 8,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_UDMAP0,
					TISCI_RESASG_SUBTYPE_UDMAP_RX_FLOW_COMMON),
            .host_id = TISCI_HOST_ID_R5_2,
        },
        {
            .start_resource = 294,
            .num_resource = 6,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_UDMAP0,
					TISCI_RESASG_SUBTYPE_UDMAP_RX_FLOW_COMMON),
            .host_id = TISCI_HOST_ID_ALL,
        },

        /* Main Nav invalid flow OES */
        {
            .start_resource = 0,
            .num_resource = 1,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_UDMAP0,
					TISCI_RESASG_SUBTYPE_UDMAP_INVALID_FLOW_OES),
            .host_id = TISCI_HOST_ID_ALL,
        },

        /* Main Nav UDMA global event trigger */
        {
            .start_resource = 49152,
            .num_resource = 1024,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_UDMAP0,
					TISCI_RESASG_SUBTYPE_GLOBAL_EVENT_TRIGGER),
            .host_id = TISCI_HOST_ID_ALL,
        },

        /* Main Nav UDMA global event config */
        {
            .start_resource = 0,
            .num_resource = 1,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_UDMAP0,
					TISCI_RESASG_SUBTYPE_UDMAP_GLOBAL_CONFIG),
            .host_id = TISCI_HOST_ID_ALL,
        },

        /* Main Nav Normal Capacity RX channel */
        {
            .start_resource = 8,
            .num_resource = 64,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_UDMAP0,
					TISCI_RESASG_SUBTYPE_UDMAP_RX_CHAN),
            .host_id = TISCI_HOST_ID_A53_2,
        },
        {
            .start_resource = 72,
            .num_resource = 32,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_UDMAP0,
					TISCI_RESASG_SUBTYPE_UDMAP_RX_CHAN),
            .host_id = TISCI_HOST_ID_A53_3,
        },
        {
            .start_resource = 104,
            .num_resource = 20,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_UDMAP0,
					TISCI_RESASG_SUBTYPE_UDMAP_RX_CHAN),
            .host_id = TISCI_HOST_ID_R5_0,
        },
        {
            .start_resource = 124,
            .num_resource = 16,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_UDMAP0,
					TISCI_RESASG_SUBTYPE_UDMAP_RX_CHAN),
            .host_id = TISCI_HOST_ID_R5_2,
        },
        {
            .start_resource = 140,
            .num_resource = 10,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_UDMAP0,
					TISCI_RESASG_SUBTYPE_UDMAP_RX_CHAN),
            .host_id = TISCI_HOST_ID_ALL,
        },

        /* Main Nav High Capacity RX channel */
        {
            .start_resource = 2,
            .num_resource = 2,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_UDMAP0,
					TISCI_RESASG_SUBTYPE_UDMAP_RX_HCHAN),
            .host_id = TISCI_HOST_ID_A53_2,
        },
        {
            .start_resource = 4,
            .num_resource = 2,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_UDMAP0,
					TISCI_RESASG_SUBTYPE_UDMAP_RX_HCHAN),
            .host_id = TISCI_HOST_ID_R5_0,
        },
        {
            .start_resource = 6,
            .num_resource = 2,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_UDMAP0,
					TISCI_RESASG_SUBTYPE_UDMAP_RX_HCHAN),
            .host_id = TISCI_HOST_ID_R5_2,
        },

        /* Main Nav Normal Capacity TX channel */
        {
            .start_resource = 8,
            .num_resource = 50,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_UDMAP0,
					TISCI_RESASG_SUBTYPE_UDMAP_TX_CHAN),
            .host_id = TISCI_HOST_ID_A53_2,
        },
        {
            .start_resource = 58,
            .num_resource = 16,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_UDMAP0,
					TISCI_RESASG_SUBTYPE_UDMAP_TX_CHAN),
            .host_id = TISCI_HOST_ID_A53_3,
        },
        {
            .start_resource = 74,
            .num_resource = 20,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_UDMAP0,
					TISCI_RESASG_SUBTYPE_UDMAP_TX_CHAN),
            .host_id = TISCI_HOST_ID_R5_0,
        },
        {
            .start_resource = 94,
            .num_resource = 16,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_UDMAP0,
					TISCI_RESASG_SUBTYPE_UDMAP_TX_CHAN),
            .host_id = TISCI_HOST_ID_R5_2,
        },
        {
            .start_resource = 110,
            .num_resource = 10,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_UDMAP0,
					TISCI_RESASG_SUBTYPE_UDMAP_TX_CHAN),
            .host_id = TISCI_HOST_ID_ALL,
        },

        /* Main Nav extended TX channels */
        {
            .start_resource = 120,
            .num_resource = 4,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_UDMAP0,
					TISCI_RESASG_SUBTYPE_UDMAP_TX_ECHAN),
            .host_id = TISCI_HOST_ID_A53_2,
        },
        {
            .start_resource = 124,
            .num_resource = 4,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_UDMAP0,
					TISCI_RESASG_SUBTYPE_UDMAP_TX_ECHAN),
            .host_id = TISCI_HOST_ID_A53_3,
        },
        {
            .start_resource = 128,
            .num_resource = 12,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_UDMAP0,
					TISCI_RESASG_SUBTYPE_UDMAP_TX_ECHAN),
            .host_id = TISCI_HOST_ID_R5_0,
        },
        {
            .start_resource = 140,
            .num_resource = 12,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_UDMAP0,
					TISCI_RESASG_SUBTYPE_UDMAP_TX_ECHAN),
            .host_id = TISCI_HOST_ID_R5_2,
        },

        /* Main Nav High Capacity TX channel */
        {
            .start_resource = 1,
            .num_resource = 3,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_UDMAP0,
					TISCI_RESASG_SUBTYPE_UDMAP_TX_HCHAN),
            .host_id = TISCI_HOST_ID_A53_2,
        },
        {
            .start_resource = 4,
            .num_resource = 2,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_UDMAP0,
					TISCI_RESASG_SUBTYPE_UDMAP_TX_HCHAN),
            .host_id = TISCI_HOST_ID_R5_0,
        },
        {
            .start_resource = 6,
            .num_resource = 2,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_NAVSS0_UDMAP0,
					TISCI_RESASG_SUBTYPE_UDMAP_TX_HCHAN),
            .host_id = TISCI_HOST_ID_R5_2,
        },

        /* MCU Nav IA VINT */
        {
            .start_resource = 8,
            .num_resource = 80,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_MCU_NAVSS0_INTR_AGGR_0,
					TISCI_RESASG_SUBTYPE_IA_VINT),
            .host_id = TISCI_HOST_ID_A53_2,
        },
        {
            .start_resource = 88,
            .num_resource = 30,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_MCU_NAVSS0_INTR_AGGR_0,
					TISCI_RESASG_SUBTYPE_IA_VINT),
            .host_id = TISCI_HOST_ID_A53_3,
        },
        {
            .start_resource = 118,
            .num_resource = 50,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_MCU_NAVSS0_INTR_AGGR_0,
					TISCI_RESASG_SUBTYPE_IA_VINT),
            .host_id = TISCI_HOST_ID_R5_0,
        },
        {
            .start_resource = 168,
            .num_resource = 50,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_MCU_NAVSS0_INTR_AGGR_0,
					TISCI_RESASG_SUBTYPE_IA_VINT),
            .host_id = TISCI_HOST_ID_R5_2,
        },
        {
            .start_resource = 218,
            .num_resource = 38,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_MCU_NAVSS0_INTR_AGGR_0,
					TISCI_RESASG_SUBTYPE_IA_VINT),
            .host_id = TISCI_HOST_ID_ALL,
        },

        /* MCU Nav IA global events */
        {
            .start_resource = 16392,
            .num_resource = 512,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_MCU_NAVSS0_INTR_AGGR_0,
					TISCI_RESASG_SUBTYPE_GLOBAL_EVENT_SEVT),
            .host_id = TISCI_HOST_ID_A53_2,
        },
        {
            .start_resource = 16904,
            .num_resource = 128,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_MCU_NAVSS0_INTR_AGGR_0,
					TISCI_RESASG_SUBTYPE_GLOBAL_EVENT_SEVT),
            .host_id = TISCI_HOST_ID_A53_3,
        },
        {
            .start_resource = 17032,
            .num_resource = 256,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_MCU_NAVSS0_INTR_AGGR_0,
					TISCI_RESASG_SUBTYPE_GLOBAL_EVENT_SEVT),
            .host_id = TISCI_HOST_ID_R5_0,
        },
        {
            .start_resource = 17288,
            .num_resource = 256,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_MCU_NAVSS0_INTR_AGGR_0,
					TISCI_RESASG_SUBTYPE_GLOBAL_EVENT_SEVT),
            .host_id = TISCI_HOST_ID_R5_2,
        },
        {
            .start_resource = 17544,
            .num_resource = 376,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_MCU_NAVSS0_INTR_AGGR_0,
					TISCI_RESASG_SUBTYPE_GLOBAL_EVENT_SEVT),
            .host_id = TISCI_HOST_ID_ALL,
        },

        /* MCU Nav IR */
        {
            .start_resource = 4,
            .num_resource = 28,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_MCU_NAVSS0_INTR_ROUTER_0,
					TISCI_RESASG_SUBTYPE_IR_OUTPUT),
            .host_id = TISCI_HOST_ID_R5_0,
        },
        {
            .start_resource = 36,
            .num_resource = 28,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_MCU_NAVSS0_INTR_ROUTER_0,
					TISCI_RESASG_SUBTYPE_IR_OUTPUT),
            .host_id = TISCI_HOST_ID_R5_2,
        },

        /* MCU Nav nonsecure proxies */
        {
            .start_resource = 0,
            .num_resource = 12,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_MCU_NAVSS0_PROXY0,
					TISCI_RESASG_SUBTYPE_PROXY_PROXIES),
            .host_id = TISCI_HOST_ID_A53_2,
        },
        {
            .start_resource = 12,
            .num_resource = 4,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_MCU_NAVSS0_PROXY0,
					TISCI_RESASG_SUBTYPE_PROXY_PROXIES),
            .host_id = TISCI_HOST_ID_A53_3,
        },
        {
            .start_resource = 16,
            .num_resource = 24,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_MCU_NAVSS0_PROXY0,
					TISCI_RESASG_SUBTYPE_PROXY_PROXIES),
            .host_id = TISCI_HOST_ID_R5_0,
        },
        {
            .start_resource = 40,
            .num_resource = 24,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_MCU_NAVSS0_PROXY0,
					TISCI_RESASG_SUBTYPE_PROXY_PROXIES),
            .host_id = TISCI_HOST_ID_R5_2,
        },

        /* MCU Nav Free RX Flow */
        {
            .start_resource = 48,
            .num_resource = 16,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_MCU_NAVSS0_UDMAP0,
					TISCI_RESASG_SUBTYPE_UDMAP_RX_FLOW_COMMON),
            .host_id = TISCI_HOST_ID_A53_2,
        },
        {
            .start_resource = 64,
            .num_resource = 4,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_MCU_NAVSS0_UDMAP0,
					TISCI_RESASG_SUBTYPE_UDMAP_RX_FLOW_COMMON),
            .host_id = TISCI_HOST_ID_A53_3,
        },
        {
            .start_resource = 68,
            .num_resource = 16,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_MCU_NAVSS0_UDMAP0,
					TISCI_RESASG_SUBTYPE_UDMAP_RX_FLOW_COMMON),
            .host_id = TISCI_HOST_ID_R5_0,
        },
        {
            .start_resource = 84,
            .num_resource = 8,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_MCU_NAVSS0_UDMAP0,
					TISCI_RESASG_SUBTYPE_UDMAP_RX_FLOW_COMMON),
            .host_id = TISCI_HOST_ID_R5_2,
        },
        {
            .start_resource = 92,
            .num_resource = 4,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_MCU_NAVSS0_UDMAP0,
					TISCI_RESASG_SUBTYPE_UDMAP_RX_FLOW_COMMON),
            .host_id = TISCI_HOST_ID_ALL,
        },

        /* MCU Nav Ring Error OES */
        {
            .start_resource = 0,
            .num_resource = 1,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_MCU_NAVSS0_UDMAP0,
					TISCI_RESASG_SUBTYPE_UDMAP_INVALID_FLOW_OES),
            .host_id = TISCI_HOST_ID_ALL,
        },

        /* MCU Nav UDMA global event trigger */
        {
            .start_resource = 56320,
            .num_resource = 256,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_MCU_NAVSS0_UDMAP0,
					TISCI_RESASG_SUBTYPE_GLOBAL_EVENT_TRIGGER),
            .host_id = TISCI_HOST_ID_ALL,
        },

        /* MCU Nav UDMA global event config */
        {
            .start_resource = 0,
            .num_resource = 1,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_MCU_NAVSS0_UDMAP0,
					TISCI_RESASG_SUBTYPE_UDMAP_GLOBAL_CONFIG),
            .host_id = TISCI_HOST_ID_ALL,
        },

        /* MCU Nav Normal Capacity RX channels */
        {
            .start_resource = 2,
            .num_resource = 14,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_MCU_NAVSS0_UDMAP0,
					TISCI_RESASG_SUBTYPE_UDMAP_RX_CHAN),
            .host_id = TISCI_HOST_ID_A53_2,
        },
        {
            .start_resource = 16,
            .num_resource = 4,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_MCU_NAVSS0_UDMAP0,
					TISCI_RESASG_SUBTYPE_UDMAP_RX_CHAN),
            .host_id = TISCI_HOST_ID_A53_3,
        },
        {
            .start_resource = 20,
            .num_resource = 14,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_MCU_NAVSS0_UDMAP0,
					TISCI_RESASG_SUBTYPE_UDMAP_RX_CHAN),
            .host_id = TISCI_HOST_ID_R5_0,
        },
        {
            .start_resource = 34,
            .num_resource = 14,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_MCU_NAVSS0_UDMAP0,
					TISCI_RESASG_SUBTYPE_UDMAP_RX_CHAN),
            .host_id = TISCI_HOST_ID_R5_2,
        },

        /* MCU Nav High Capacity RX channel */
        {
            .start_resource = 0,
            .num_resource = 2,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_MCU_NAVSS0_UDMAP0,
					TISCI_RESASG_SUBTYPE_UDMAP_RX_HCHAN),
            .host_id = TISCI_HOST_ID_R5_0,
        },

        /* MCU Nav Normal Capacity TX channels */
        {
            .start_resource = 2,
            .num_resource = 14,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_MCU_NAVSS0_UDMAP0,
					TISCI_RESASG_SUBTYPE_UDMAP_TX_CHAN),
            .host_id = TISCI_HOST_ID_A53_2,
        },
        {
            .start_resource = 16,
            .num_resource = 4,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_MCU_NAVSS0_UDMAP0,
					TISCI_RESASG_SUBTYPE_UDMAP_TX_CHAN),
            .host_id = TISCI_HOST_ID_A53_3,
        },
        {
            .start_resource = 20,
            .num_resource = 14,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_MCU_NAVSS0_UDMAP0,
					TISCI_RESASG_SUBTYPE_UDMAP_TX_CHAN),
            .host_id = TISCI_HOST_ID_R5_0,
        },
        {
            .start_resource = 34,
            .num_resource = 14,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_MCU_NAVSS0_UDMAP0,
					TISCI_RESASG_SUBTYPE_UDMAP_TX_CHAN),
            .host_id = TISCI_HOST_ID_R5_2,
        },

        /* MCU Nav High Capacity TX channel */
        {
            .start_resource = 0,
            .num_resource = 2,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_MCU_NAVSS0_UDMAP0,
					TISCI_RESASG_SUBTYPE_UDMAP_TX_HCHAN),
            .host_id = TISCI_HOST_ID_R5_0,
        },

        /* MCU Nav Ring Error OES */
        {
            .start_resource = 0,
            .num_resource = 1,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_MCU_NAVSS0_RINGACC0,
					TISCI_RESASG_SUBTYPE_RA_ERROR_OES),
            .host_id = TISCI_HOST_ID_ALL,
        },

        /* MCU Nav Free Ring */
        {
            .start_resource = 96,
            .num_resource = 32,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_MCU_NAVSS0_RINGACC0,
					TISCI_RESASG_SUBTYPE_RA_GP),
            .host_id = TISCI_HOST_ID_A53_2,
        },
        {
            .start_resource = 128,
            .num_resource = 8,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_MCU_NAVSS0_RINGACC0,
					TISCI_RESASG_SUBTYPE_RA_GP),
            .host_id = TISCI_HOST_ID_A53_3,
        },
        {
            .start_resource = 136,
            .num_resource = 60,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_MCU_NAVSS0_RINGACC0,
					TISCI_RESASG_SUBTYPE_RA_GP),
            .host_id = TISCI_HOST_ID_R5_0,
        },
        {
            .start_resource = 196,
            .num_resource = 60,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_MCU_NAVSS0_RINGACC0,
					TISCI_RESASG_SUBTYPE_RA_GP),
            .host_id = TISCI_HOST_ID_R5_2,
        },

        /* MCU Nav Normal Capacity Rx ring */
        {
            .start_resource = 50,
            .num_resource = 14,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_MCU_NAVSS0_RINGACC0,
					TISCI_RESASG_SUBTYPE_RA_UDMAP_RX),
            .host_id = TISCI_HOST_ID_A53_2,
        },
        {
            .start_resource = 64,
            .num_resource = 4,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_MCU_NAVSS0_RINGACC0,
					TISCI_RESASG_SUBTYPE_RA_UDMAP_RX),
            .host_id = TISCI_HOST_ID_A53_3,
        },
        {
            .start_resource = 68,
            .num_resource = 14,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_MCU_NAVSS0_RINGACC0,
					TISCI_RESASG_SUBTYPE_RA_UDMAP_RX),
            .host_id = TISCI_HOST_ID_R5_0,
        },
        {
            .start_resource = 82,
            .num_resource = 14,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_MCU_NAVSS0_RINGACC0,
					TISCI_RESASG_SUBTYPE_RA_UDMAP_RX),
            .host_id = TISCI_HOST_ID_R5_2,
        },

        /* MCU Nav Normal Capacity Tx ring */
        {
            .start_resource = 2,
            .num_resource = 14,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_MCU_NAVSS0_RINGACC0,
					TISCI_RESASG_SUBTYPE_RA_UDMAP_TX),
            .host_id = TISCI_HOST_ID_A53_2,
        },
        {
            .start_resource = 16,
            .num_resource = 4,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_MCU_NAVSS0_RINGACC0,
					TISCI_RESASG_SUBTYPE_RA_UDMAP_TX),
            .host_id = TISCI_HOST_ID_A53_3,
        },
        {
            .start_resource = 20,
            .num_resource = 14,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_MCU_NAVSS0_RINGACC0,
					TISCI_RESASG_SUBTYPE_RA_UDMAP_TX),
            .host_id = TISCI_HOST_ID_R5_0,
        },
        {
            .start_resource = 34,
            .num_resource = 14,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_MCU_NAVSS0_RINGACC0,
					TISCI_RESASG_SUBTYPE_RA_UDMAP_TX),
            .host_id = TISCI_HOST_ID_R5_2,
        },

        /* MCU Nav High Capacity Rx ring */
        {
            .start_resource = 48,
            .num_resource = 2,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_MCU_NAVSS0_RINGACC0,
					TISCI_RESASG_SUBTYPE_RA_UDMAP_RX_H),
            .host_id = TISCI_HOST_ID_R5_0,
        },

        /* MCU Nav High Capacity Tx ring */
        {
            .start_resource = 0,
            .num_resource = 2,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_MCU_NAVSS0_RINGACC0,
					TISCI_RESASG_SUBTYPE_RA_UDMAP_TX_H),
            .host_id = TISCI_HOST_ID_R5_0,
        },

        /* virt_id range */
        {
            .start_resource = 2,
            .num_resource = 1,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_MCU_NAVSS0_RINGACC0,
					TISCI_RESASG_SUBTYPE_RA_VIRTID),
            .host_id = TISCI_HOST_ID_A53_2,
        },
        {
            .start_resource = 3,
            .num_resource = 1,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_MCU_NAVSS0_RINGACC0,
					TISCI_RESASG_SUBTYPE_RA_VIRTID),
            .host_id = TISCI_HOST_ID_A53_3,
        },
        {
            .start_resource = 4,
            .num_resource = 4,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_MCU_NAVSS0_RINGACC0,
					TISCI_RESASG_SUBTYPE_RA_VIRTID),
            .host_id = TISCI_HOST_ID_ALL,
        },

        /* MCU Nav ring monitors */
        {
            .start_resource = 0,
            .num_resource = 8,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_MCU_NAVSS0_RINGACC0,
					TISCI_RESASG_SUBTYPE_RA_MONITORS),
            .host_id = TISCI_HOST_ID_A53_2,
        },
        {
            .start_resource = 8,
            .num_resource = 4,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_MCU_NAVSS0_RINGACC0,
					TISCI_RESASG_SUBTYPE_RA_MONITORS),
            .host_id = TISCI_HOST_ID_A53_3,
        },
        {
            .start_resource = 12,
            .num_resource = 8,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_MCU_NAVSS0_RINGACC0,
					TISCI_RESASG_SUBTYPE_RA_MONITORS),
            .host_id = TISCI_HOST_ID_R5_0,
        },
        {
            .start_resource = 20,
            .num_resource = 8,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_MCU_NAVSS0_RINGACC0,
					TISCI_RESASG_SUBTYPE_RA_MONITORS),
            .host_id = TISCI_HOST_ID_R5_2,
        },
        {
            .start_resource = 28,
            .num_resource = 4,
            .type = TISCI_RESASG_UTYPE (TISCI_DEV_MCU_NAVSS0_RINGACC0,
					TISCI_RESASG_SUBTYPE_RA_MONITORS),
            .host_id = TISCI_HOST_ID_ALL,
        },

    },
};
#endif
