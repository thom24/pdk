/*
 *  Copyright (C) 2017-2022 Texas Instruments Incorporated
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
/**
 * \ingroup TISCI
 * \defgroup tisci_boardcfg_macros tisci_boardcfg_macros
 *
 * DMSC controls the power management, security and resource management
 * of the device.
 *
 *
 * @{
 */
/**
 *
 *  \brief  This file contains:
 *
 *          WARNING!!: Autogenerated file from SYSFW. DO NOT MODIFY!!
 * System Firmware Source File
 *
 * Board Configuration Data Structures
 *
 */
#ifndef TISCI_BOARDCFG_MACROS_H
#define TISCI_BOARDCFG_MACROS_H

/** Length of the hash in bytes. We use SHA2-512 as the hash algorithm */
#define TISCI_BOARDCFG_HASH_LEN_BYTES (64U)

/** Length of the initial vector used for security tisci_boardcfg encryption in bytes */
#define TISCI_BOARDCFG_SEC_IV_LEN (16U)

/** Length of the string used for security tisci_boardcfg validation after decryption in bytes */
#define TISCI_BOARDCFG_SEC_RS_LEN (32U)

/** Length of the tisci_boardcfg array for hosts that can be allowed to unlock the JTAG via TISCI API */
#define TISCI_BOARDCFG_SEC_MAX_NUM_JTAG_UNLOCK_HOSTS (4U)

#define TISCI_BOARDCFG_SECPROXY_SCALING_FACTOR_MASK                   (0x01U)
#define TISCI_BOARDCFG_SECPROXY_SCALING_PROFILE_MASK                  (0x01U)
#define TISCI_BOARDCFG_SECPROXY_DISABLE_MAIN_NAV_SECURE_PROXY_MASK    (0x01U)

#define TISCI_BOARDCFG_DEBUG_TRACE_DST_ENABLES_MASK                   (0x0FU)
#define TISCI_BOARDCFG_DEBUG_TRACE_SRC_ENABLES_MASK                   (0x3FU)

#define TISCI_BOARDCFG_CONTROL_MAGIC_NUM                              (0xC1D3U)
#define TISCI_BOARDCFG_SECPROXY_MAGIC_NUM                             (0x1207U)
#define TISCI_BOARDCFG_MSMC_MAGIC_NUM                                 (0xA5C3U)
#define TISCI_BOARDCFG_PROC_ACL_MAGIC_NUM                             (0xF1EAU)
#define TISCI_BOARDCFG_HOST_HIERARCHY_MAGIC_NUM                       (0x8D27U)
#define TISCI_BOARDCFG_DBG_CFG_MAGIC_NUM                              (0x020CU)
#define TISCI_BOARDCFG_PMIC_CFG_MAGIC_NUM                             (0x3172U)
#define TISCI_BOARDCFG_OTP_CFG_MAGIC_NUM                              (0x4081U)
#define TISCI_BOARDCFG_DKEK_CFG_MAGIC_NUM                             (0x5170U)
#define TISCI_BOARDCFG_SA2UL_CFG_MAGIC_NUM                            (0xB4FDU)
#define TISCI_BOARDCFG_SA2UL_CFG_MAGIC_NUM_RSVD                       (0x23BEU)
#define TISCI_BOARDCFG_SEC_DBG_CTRL_MAGIC_NUM                         (0x42AFU)
#define TISCI_BOARDCFG_SEC_HANDOVER_CFG_MAGIC_NUM                     (0x608FU)

/*
 * NOTE: We are trying to assign the board configuration number with a pattern.
 * alternatively increment and decrement the digits in previous
 * board configuration macro. This should allow for 16 magic words
 * before needing to pick a new seed.
 * e.g. 0x3172U -> 0x4081U, 0x42AFU -> 0x33BEU
 */
#endif

/* @} */
