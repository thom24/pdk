/*
 *  Copyright (C) 2017-2020 Texas Instruments Incorporated
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
 * \defgroup tisci_boardcfg tisci_boardcfg
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

#ifndef TISCI_BOARD_CFG_H
#define TISCI_BOARD_CFG_H



#define TISCI_BOARDCFG_ABI_MAJ_VALUE                  0x00
#define TISCI_BOARDCFG_ABI_MIN_VALUE                  0x01

#define TISCI_BOARDCFG_RM_ABI_MAJ_VALUE               0x00
#define TISCI_BOARDCFG_RM_ABI_MIN_VALUE               0x01

#define TISCI_BOARDCFG_SEC_ABI_MAJ_VALUE               0x00
#define TISCI_BOARDCFG_SEC_ABI_MIN_VALUE               0x01

/**
 * \brief Contains a unique magic number for each substructure and the size
 *      of the associated superstructure for data validation/API
 *      compatibility checks.
 *
 * \param magic Unique magic number for data integrity check.
 * \param size `sizeof(superstructure containing this header)` for data
 *           integrity check.
 */
struct tisci_boardcfg_substructure_header {
    uint16_t    magic;
    uint16_t    size;
} __attribute__((__packed__));

/**
 * \brief Board Config data ABI version.
 *
 * \param tisci_boardcfg_abi_maj Major Board Config data ABI version.
 * \param tisci_boardcfg_abi_min Minor Board Config data ABI version.
 */
struct tisci_boardcfg_abi_rev {
    uint8_t    tisci_boardcfg_abi_maj;
    uint8_t    tisci_boardcfg_abi_min;
} __attribute__((__packed__));

/**
 * \brief Resource Management host ID configuration entry describing a
 *        host's allowed system priorities as well as a host's supervisor
 *
 * \param host_id Processing entity host ID
 *
 * \param allowed_qos Allowed QoS level configuration for host ID:
 *
 *                    The host ID gets assigned a list of QoS levels which
 *                    are allowed. As QoS level is a 3-bit field, there are
 *                    8 possible order-IDs. Thus in one 16-bit word, flags
 *                    are set specifying whether or not the QoS level is
 *                    allowed for the host ID. For each QoS level, the value
 *                    of 01b means not allowed, 10b means allowed, and 11b
 *                    and 00b are invalid/errors. These are encoded in a
 *                    bitfield because there is one set of allowed QoS levels
 *                    for every host ID.
 *
 * \param allowed_orderid Allowed order-ID configuration for the host ID:
 *
 *                        The host ID gets assigned a list of order-IDs which
 *                        are allowed. As order-ID is a 4-bit field, there are
 *                        16 possible order-IDs. Thus in one 32-bit word, flags
 *                        are set specifying whether or not the order-ID is
 *                        allowed for the host ID. For each order-ID, the value
 *                        of 01b means not allowed, 10b means allowed, and 11b
 *                        and 00b are invalid/errors. These are encoded in a
 *                        bitfield because there is one set of allowed
 *                        order-IDs for every host ID.
 *
 * \param allowed_atype Allowed atype configuration for the host ID:
 *
 *                      The host ID gets assigned a list of atypes which
 *                      are allowed. atype is a 2-bit field with 3 possible
 *                      values. Thus in one 8-bit word, flags are set
 *                      specifying whether or not an atype value is
 *                      allowed for the host ID. For each atype, the value
 *                      of 01b means not allowed, 10b means allowed, and 11b
 *                      and 00b are invalid/errors. These are encoded in a
 *                      bitfield because there is one set of allowed atypes
 *                      for every host ID.
 *
 * \param allowed_priority Allowed bus priority configuration for host ID:
 *
 *                         The host ID gets assigned a list of bus priorities
 *                         which are allowed. As bus priority is a 3-bit field,
 *                         there are 8 possible bus priorities. Thus in one
 *                         16-bit word, flags are set specifying whether or not
 *                         the bus priority is allowed for the host ID. For each
 *                         bus priority, the value of 01b means not allowed,
 *                         10b means allowed, and 11b and 00b are
 *                         invalid/errors. These are encoded in a bitfield
 *                         because there is one set of allowed bus priorities
 *                         for every host ID.
 *
 * \param allowed sched_priority Allowed UDMAP channel scheduling priority
 *                               configuration for host ID:
 *
 *                               The host ID gets assigned a list of UDMAP
 *                               channel scheduling priorities which are
 *                               allowed. As UDMAP channel scheduling priority
 *                               is a 2-bit field, there are 4 possible UDMAP
 *                               channel scheduling priorities. Thus in one
 *                               8-bit word, flags are set specifying whether or
 *                               not UDMAP channel scheduling priority is
 *                               allowed for the host ID. For each priority, the
 *                               value of 01b means not allowed, 10b means
 *                               allowed, and 11b and 00b are invalid/errors.
 *                               These are encoded in a bitfield because there
 *                               is one set of allowed UDMAP channel scheduling
 *                               priorities for every host ID.
 */
struct tisci_boardcfg_rm_host_cfg_entry {
    uint8_t    host_id;
    uint8_t    allowed_atype;
    uint16_t    allowed_qos;
    uint32_t    allowed_orderid;
    uint16_t    allowed_priority;
    uint8_t    allowed_sched_priority;
} __attribute__((__packed__));

/* \brief How many host configuration list entries */
#define TISCI_BOARDCFG_RM_HOST_CFG_ENTRIES (32U)

/**
 * \brief Resource Management host ID configuration describing a host's allowed
 *        system priorities
 *
 * \param subhdr Magic and size for integrity check.
 * \param host_cfg_entries @ref tisci_boardcfg_rm_host_cfg_entry entries up to
 *                         @ref TISCI_BOARDCFG_RM_HOST_CFG_ENTRIES
 */
struct tisci_boardcfg_rm_host_cfg {
    struct tisci_boardcfg_substructure_header    subhdr;
    struct tisci_boardcfg_rm_host_cfg_entry    host_cfg_entries[
        TISCI_BOARDCFG_RM_HOST_CFG_ENTRIES];
};

/**
 * \brief One contiguous block of any resource to be assigned to one
 *      processing entity.
 *
 * \param start_resource First resource number (inclusive) for a host_id
 *             and resource.
 * \param num_resource Number of continuous resources allocated to a
 *               host_id and resource. 0 means unused.
 * \param type Unique type value identifying the specific resource to which the
 *             range assignment applies
 * \param host_id Processing entity host ID to which the resource range is
 *                assigned
 */
struct tisci_boardcfg_rm_resasg_entry {
    uint16_t    start_resource;
    uint16_t    num_resource;
    uint16_t    type;
    uint8_t    host_id;
    uint8_t    reserved;
};

/**
 * \brief RM tisci_boardcfg resource assignment configuration.
 *
 * \param subhdr Magic and size for integrity check.
 * \param resasg_entries_size Size of resasg_entries array in bytes
 * \param resasg_entries Array of @ref tisci_boardcfg_rm_resasg_entry containing
 *                       resource range assignments to hosts.
 */
struct tisci_boardcfg_rm_resasg {
    struct tisci_boardcfg_substructure_header    subhdr;
    uint16_t                    resasg_entries_size;
    uint16_t                    reserved;
    struct tisci_boardcfg_rm_resasg_entry        resasg_entries[];
} __attribute__((__packed__));

/**
 * \brief Resource Management configuration.
 *
 * \param rev Resource Management board config ABI version (separate from Board
 *            Config ABI version)
 * \param host_cfg Array of host configuration structures.
 * \param resasg Resource assignment configuration structure.
 */
struct tisci_boardcfg_rm {
    struct tisci_boardcfg_abi_rev        rev;
    struct tisci_boardcfg_rm_host_cfg    host_cfg;
    struct tisci_boardcfg_rm_resasg    resasg;
} __attribute__((__packed__));

/**
 * \brief Used to enable/disable features in DMSC based on usecase.
 *
 * \param subhdr Magic and size for integrity check.
 * \param main_isolation_enable Enable/disable support for DMSC main
 *                              isolation. If disabled, main isolation
 *                              SCI message will be rejected with NAK.
 * \param main_isolation_hostid    Host-ID allowed to send SCI-message for
 *                              main isolation. If mismatch, SCI message
 *                              will be rejected with NAK.
 */
struct tisci_boardcfg_control {
    struct tisci_boardcfg_substructure_header    subhdr;
    uint8_t                    main_isolation_enable;
    uint16_t                    main_isolation_hostid;
} __attribute__((__packed__));

/**
 * \brief Secure proxy configuration.
 *
 * \param subhdr Magic and size for integrity check.
 * \param scaling_factor Memory allocation for messages scaling factor. In
 *             current design, only value of "1" is supported. For
 *             future design, a value of "2" would double all memory
 *             allocations and credits, "3" would triple, and so on.
 * \param scaling_profile Memory allocation for messages profile number. In
 *              current design, only a value of "1" is supported.
 *              "0" is always invalid due to fault tolerance.
 * \param disable_main_nav_secure_proxy Do not configure main nav secure proxy.
 *                    This removes all MSMC memory demands
 *                    from DMSC but limits MPU channels to
 *                    one set of secure and one set of
 *                    insecure.
 */
struct tisci_boardcfg_secproxy {
    struct tisci_boardcfg_substructure_header    subhdr;
    uint8_t                    scaling_factor;
    uint8_t                    scaling_profile;
    uint8_t                    disable_main_nav_secure_proxy;
} __attribute__((__packed__));

/**
 * \brief Cache configuration so that MSMC can be used for main secure proxy
 *      backing memory and ring memory.
 *
 * \param subhdr Magic and size for integrity check.
 * \param msmc_cache_size Fraction of msmc to be cache in /32 units. Rounds
 *              up. Since current msmc support /8 allocation this
 *              means that 1/32 rounds up to 1/8.
 */
struct tisci_boardcfg_msmc {
    struct tisci_boardcfg_substructure_header    subhdr;
    uint8_t                    msmc_cache_size;
} __attribute__((__packed__));

/* \brief How many masters that are permitted in proc acl for access */
#define PROCESSOR_ACL_SECONDARY_MASTERS_MAX (3U)

/**
 * \brief A single entry of Processor Access Control List
 *
 * \param processor_id - What processor ID are we restricting control over?
 * \param proc_access_master - Who is the master who can override control
 * \param proc_access_secondary - list of upto @ref PROCESSOR_ACL_SECONDARY_MASTERS_MAX
 */
struct tisci_boardcfg_proc_acl_entry {
    uint8_t    processor_id;
    uint8_t    proc_access_master;
    uint8_t    proc_access_secondary[PROCESSOR_ACL_SECONDARY_MASTERS_MAX];
} __attribute__((__packed__));

/* \brief How many access control list entries for processors */
#define PROCESSOR_ACL_ENTRIES (32U)

/**
 * \brief Control list for which hosts can control which processors
 * \param subhdr - Magic and size for integrity check
 * \param proc_acl_entries - @ref tisci_boardcfg_proc_acl_entry entries upto @ref PROCESSOR_ACL_ENTRIES entries
 */
struct tisci_boardcfg_proc_acl {
    struct tisci_boardcfg_substructure_header    subhdr;
    struct tisci_boardcfg_proc_acl_entry        proc_acl_entries[
        PROCESSOR_ACL_ENTRIES];
} __attribute__((__packed__));

/**
 * \brief A single entry of Host hierarchy List
 *
 * \param host_id - Processing entity Host ID whose supervisor is specified
 * \param supervisor_host_id - Processing entity Host ID that is the supervisor
 *                             of the host_id
 */
struct tisci_boardcfg_host_hierarchy_entry {
    uint8_t    host_id;
    uint8_t    supervisor_host_id;
} __attribute__((__packed__));

/** \brief How many host hierarchy list entries for hosts */
#define HOST_HIERARCHY_ENTRIES (32U)

/**
 * \brief List of SoC hosts and their supervising hosts
 * \param subhdr - Magic and size for integrity check
 * \param host_hierarchy_entries - @ref tisci_boardcfg_host_hierarchy_entry entries
 *                                 upto @ref HOST_HIERARCHY_ENTRIES entries
 */
struct tisci_boardcfg_host_hierarchy {
    struct tisci_boardcfg_substructure_header    subhdr;
    struct tisci_boardcfg_host_hierarchy_entry    host_hierarchy_entries[HOST_HIERARCHY_ENTRIES];
} __attribute__((__packed__));

/**
 * \brief access configuration for one OTP MMR. Each MMR is 32 bit wide.
 *
 * \param host_id Id of the host owning the MMR
 *
 * \param host_perms 2 bit wide fields specifying permissions
 *                   bit 1:0 - 10b - non-secure, any other value secure
 *                   bit 7:2 - Reserved for future use
 */
struct tisci_boardcfg_extended_otp_entry {
    uint8_t    host_id;
    uint8_t    host_perms;
} __attribute__((__packed__));

/**
 * \brief Maximum number of OTP rows allowed by design
 */
#define MAX_NUM_EXT_OTP_MMRS (32U)

/**
 * \brief Access configuration for each OTP row
 * \param subhdr Magic and size for integrity check
 * \param otp_entry access configurations for each OTP MMR
 * \param write_host_id ID of the host allowed to perform OTP write/lock operations.
 */
struct tisci_boardcfg_extended_otp {
    struct tisci_boardcfg_substructure_header    subhdr;
    struct tisci_boardcfg_extended_otp_entry    otp_entry[MAX_NUM_EXT_OTP_MMRS];
    uint8_t                    write_host_id;
} __attribute__((__packed__));
/**
 * \brief Format of the complete board configuration.
 *
 * \param tisci_boardcfg_abi_rev Secure Board Config ABI version (separate from DMSC ABI version)
 * \param tisci_boardcfg_proc_acl Processor Access control list
 * \param tisci_boardcfg_host_hierarchy Host hierarchy list
 * \param otp_config  OTP Configuration
 */
struct tisci_boardcfg_sec {
    struct tisci_boardcfg_abi_rev        rev;
    struct tisci_boardcfg_proc_acl    processor_acl_list;
    struct tisci_boardcfg_host_hierarchy    host_hierarchy;
    struct tisci_boardcfg_extended_otp    otp_config;
} __attribute__((__packed__));

/**
 * \def TISCI_BOARDCFG_TRACE_DST_UART0
 * Traces to UART0 in wakeupss enabled.
 *
 * \def TISCI_BOARDCFG_TRACE_DST_ITM
 * Traces to UART attached to ITM(JTAG) enabled.
 *
 * \def TISCI_BOARDCFG_TRACE_DST_MEM
 * Traces to memory buffer enabled.
 */
#define TISCI_BOARDCFG_TRACE_DST_UART0                TISCI_BIT(0)
#define TISCI_BOARDCFG_TRACE_DST_ITM                  TISCI_BIT(2)
#define TISCI_BOARDCFG_TRACE_DST_MEM                  TISCI_BIT(3)

/**
 * \def TISCI_BOARDCFG_TRACE_SRC_PM
 * Traces from power management are allowed.
 *
 * \def TISCI_BOARDCFG_TRACE_SRC_RM
 * Traces from resource management are allowed.
 *
 * \def TISCI_BOARDCFG_TRACE_SRC_SEC
 * Traces from security management are allowed.
 *
 * \def TISCI_BOARDCFG_TRACE_SRC_BASE
 * Traces from baseport are allowed.
 *
 * \def TISCI_BOARDCFG_TRACE_SRC_USER
 * Traces from user tasks are allowed.
 *
 * \def TISCI_BOARDCFG_TRACE_SRC_SUPR
 * Traces from supervisor tasks are allowed.
 */
#define TISCI_BOARDCFG_TRACE_SRC_PM                   TISCI_BIT(0)
#define TISCI_BOARDCFG_TRACE_SRC_RM                   TISCI_BIT(1)
#define TISCI_BOARDCFG_TRACE_SRC_SEC                  TISCI_BIT(2)
#define TISCI_BOARDCFG_TRACE_SRC_BASE                 TISCI_BIT(3)
#define TISCI_BOARDCFG_TRACE_SRC_USER                 TISCI_BIT(4)
#define TISCI_BOARDCFG_TRACE_SRC_SUPR                 TISCI_BIT(5)

/**
 * \brief Debug console configuration.
 *
 * \param subhdr Magic and size for integrity check.
 * \param trace_dst_enables;
 * \param trace_src_enables;
 */
struct tisci_boardcfg_dbg_cfg {
    struct tisci_boardcfg_substructure_header    subhdr;
    uint16_t                    trace_dst_enables;
    uint16_t                    trace_src_enables;
} __attribute__((__packed__));

/**
 * \brief Format of the complete board configuration.
 *
 * \param tisci_boardcfg_abi_rev Board Config ABI version (separate from DMSC ABI version)
 * \param control DMSC feature control selections
 * \param secproxy Secure proxy configuration
 * \param msmc MSMC configuration
 * \param debug_cfg Debug/trace configuration
 */
struct tisci_boardcfg {
    struct tisci_boardcfg_abi_rev        rev;
    struct tisci_boardcfg_control        control;
    struct tisci_boardcfg_secproxy    secproxy;
    struct tisci_boardcfg_msmc        msmc;
    struct tisci_boardcfg_dbg_cfg        debug_cfg;
} __attribute__((__packed__));


/**
 * \brief structure to hold the board configuration hashes received via X509 certificate
 *
 * \param sec_bcfg_hash Hash of encrypted security board configuration blob
 * \param rm_bcfg_hash Hash of RM board configuration blob
 * \param pm_bcfg_hash Hash of PM board configuration blob
 * \param core_bcfg_hash Hash of main board configuration blob
 * \param sec_bcfg_iv initial vector used during security board configuration encryption
 * \param sec_bcfg_rs byte string used to verify that security board configuration is
 *                    decrypted correctly.
 * \param sec_bcfg_ver version of the security board configuration
 * \param sec_bcfg_num_iter Number of iterations for the KDF
 * \param hashes_received Flag indicating whether SYSFW outer certificate contained tisci_boardcfg
 *                        information
 */
struct tisci_boardcfg_hashes_data {
    uint8_t    sec_bcfg_hash[TISCI_BOARDCFG_HASH_LEN_BYTES];
    uint8_t    rm_bcfg_hash[TISCI_BOARDCFG_HASH_LEN_BYTES];
    uint8_t    pm_bcfg_hash[TISCI_BOARDCFG_HASH_LEN_BYTES];
    uint8_t    core_bcfg_hash[TISCI_BOARDCFG_HASH_LEN_BYTES];
    uint8_t    sec_bcfg_iv[TISCI_BOARDCFG_SEC_IV_LEN];
    uint8_t    sec_bcfg_rs[TISCI_BOARDCFG_SEC_RS_LEN];
    uint8_t    sec_bcfg_ver;
    uint8_t    sec_bcfg_num_iter;
    uint8_t    hashes_received;
};
#endif          /* TISCI_BOARD_CFG_H */

/* @} */
