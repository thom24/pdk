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
const struct tisci_boardcfg_sec gBoardConfigLow_security
__attribute__(( aligned(128), section(".boardcfg_data") )) =
{
    /* boardcfg_abi_rev */
    .rev = {
        .tisci_boardcfg_abi_maj = TISCI_BOARDCFG_SEC_ABI_MAJ_VALUE,
        .tisci_boardcfg_abi_min = TISCI_BOARDCFG_SEC_ABI_MIN_VALUE,
    },

    /* boardcfg_proc_acl */
    .processor_acl_list = {
        .subhdr = {
            .magic = TISCI_BOARDCFG_PROC_ACL_MAGIC_NUM,
            .size = sizeof(struct tisci_boardcfg_proc_acl),
        },
        .proc_acl_entries = {0},
    },

    /* boardcfg_host_hierarchy */
    .host_hierarchy = {
        .subhdr = {
            .magic = TISCI_BOARDCFG_HOST_HIERARCHY_MAGIC_NUM,
            .size = sizeof(struct tisci_boardcfg_host_hierarchy),
        },
        .host_hierarchy_entries = {0},
    },
};
#endif
