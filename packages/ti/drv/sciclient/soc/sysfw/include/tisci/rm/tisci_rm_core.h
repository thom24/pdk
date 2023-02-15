/*
 *  Copyright (C) 2017-2023 Texas Instruments Incorporated
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
 * \defgroup tisci_rm_core tisci_rm_core
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
 * System Firmware TISCI RM Core Messaging
 *
 * TISCI Protocol Definitions for RM core messages
 *
 */

#ifndef RM_TISCI_CORE_H
#define RM_TISCI_CORE_H


/**
 * \brief Retrieves a host's assigned range for a resource
 *
 * Returns the range for a unique resource type assigned to the specified host,
 * or secondary host.  The unique resource type is formed by combining the
 * 10 LSB of type and the 6 LSB of subtype.
 *
 * Unique types which do not map to an SoC resource will not be NACK'd.  Instead
 * the tisci_msg_rm_get_resource_range_resp range_start and range_num values are
 * both zeroed.  This provides a known response mechanism across varied SoCs.
 *
 * \param hdr
 * Standard TISCI header
 *
 * \param type
 * Resource assignment type used to form unique resource type.  Only 10 LSB are
 * valid.  Types are found in the "Enumeration of Resource Type IDs" section of
 * the DMSC public document.
 *
 * \param subtype
 * Resource assignment subtype used to form unique resource type.  Only 6 LSB
 * are valid.  Subtypes are found in the "Enumeration of Resource Type IDs"
 * section of the DMSC public document.
 *
 * \param secondary_host
 * The host processing entity attempting to allocate the ring if not set to
 * @ref TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST.  Otherwise, the host within
 * the TISCI message header is the host attempting to allocate the ring.
 *
 * Populating secondary_host with @ref TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST
 * specifies the value as unused.
 */
struct tisci_msg_rm_get_resource_range_req {
    struct tisci_header    hdr;
    uint16_t            type;
    uint8_t            subtype;
    uint8_t            secondary_host;
} __attribute__((__packed__));

/**
 * \brief Get resource range response message
 *
 * Response sent to host processor containing the requested resource range
 * assigned to the host who sent the range request.
 *
 * The range_start and range_num values are BOTH zero if the type and subtype
 * in the range request do not form a unique resource type for the SoC.  In
 * this case, the response will still contain an ACK.
 *
 * \param hdr
 * Standard TISCI header
 *
 * \param range_start
 * Start index of retrieved resource range.
 *
 * \param range_num
 * Number of resources in the range.  Zero if the resource is not valid.
 *
 * \param range_start_sec
 * Start index of retrieved secondary resource range.  Zero if a second
 * resource range does not exist or if the resource is not valid.
 *
 * \param range_num_sec
 * Number of resources in secondary range.  Zero if a second resource range
 * does not exist or if the resource is not valid.
 */
struct tisci_msg_rm_get_resource_range_resp {
    struct tisci_header    hdr;
    uint16_t            range_start;
    uint16_t            range_num;
    uint16_t            range_start_sec;
    uint16_t            range_num_sec;
} __attribute__((__packed__));

#endif /* RM_TISCI_CORE_H */

/* @} */
