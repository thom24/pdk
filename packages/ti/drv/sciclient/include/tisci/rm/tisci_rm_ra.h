/*
 *  Copyright (C) 2017-2019 Texas Instruments Incorporated
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
 * \defgroup tisci_rm_ra TISCI Resource Management Ring Accelerator
 *
 * DMSC controls the power management, security and resource management
 * of the device.
 *
 *
 * @{
 */
/**
 *  \file   tisci_rm_ra.h
 *
 *  \brief  This file contains:
 *
 *          WARNING!!: Autogenerated file from SYSFW. DO NOT MODIFY!!
 * System Firmware TISCI RM Ring Accelerator Messaging
 *
 * TISCI Protocol Definitions for Ring Accelerator IRQ messages
 *
 */

#ifndef RM_TISCI_RA_H
#define RM_TISCI_RA_H

#include <ti/drv/sciclient/include/tisci/tisci_protocol.h>

/**
 * The addr_lo parameter is valid for RM ring configure TISCI message
 */
#define TISCI_MSG_VALUE_RM_RING_ADDR_LO_VALID  (1u << 0u)
/**
 * The addr_hi parameter is valid for RM ring configure TISCI message
 */
#define TISCI_MSG_VALUE_RM_RING_ADDR_HI_VALID  (1u << 1u)
/**
 * The count parameter is valid for RM ring configure TISCI message
 */
#define TISCI_MSG_VALUE_RM_RING_COUNT_VALID    (1u << 2u)
/**
 * The mode parameter is valid for RM ring configure TISCI message
 */
#define TISCI_MSG_VALUE_RM_RING_MODE_VALID     (1u << 3u)
/**
 * The size parameter is valid for RM ring configure TISCI message
 */
#define TISCI_MSG_VALUE_RM_RING_SIZE_VALID     (1u << 4u)
/**
 * The order_id parameter is valid for RM ring configure TISCI message
 */
#define TISCI_MSG_VALUE_RM_RING_ORDER_ID_VALID (1u << 5u)
/**
 * The virtid parameter is valid for RM ring configure TISCI message
 */
#define TISCI_MSG_VALUE_RM_RING_VIRTID_VALID   (1u << 6u)

/**
 * Value for @ref tisci_msg_rm_ring_get_cfg_req::get_reset_cfg used to return
 * the non-real-time register configuration for a ring
 */
#define TISCI_MSG_VALUE_RM_RING_GET_CFG_REG    (0x0u)
/**
 * Value for @ref tisci_msg_rm_ring_get_cfg_req::get_reset_cfg used to return
 * the non-real-time register hardware reset configuration for a ring
 */
#define TISCI_MSG_VALUE_RM_RING_GET_CFG_RESET  (0x1u)

/**
 *  \anchor tisci_msg_value_rm_ring_mode
 *  \name Ring modes
 *  @{
 *  Ring modes for ring configuration request messages
 */
/**
 * Exposed ring mode for @ref tisci_msg_rm_ring_cfg_req::mode
 */
#define TISCI_MSG_VALUE_RM_RING_MODE_RING      (0x0u)
/**
 * Messaging ring mode for @ref tisci_msg_rm_ring_cfg_req::mode
 */
#define TISCI_MSG_VALUE_RM_RING_MODE_MESSAGE   (0x1u)
/**
 * Credentials ring mode for @ref tisci_msg_rm_ring_cfg_req::mode
 */
#define TISCI_MSG_VALUE_RM_RING_MODE_CREDENTIALS (0x2u)
/**
 * QM ring mode for @ref tisci_msg_rm_ring_cfg_req::mode
 */
#define TISCI_MSG_VALUE_RM_RING_MODE_QM        (0x3u)
/* @} */

/**
 * 4-byte ring element size for @ref tisci_msg_rm_ring_cfg_req::size
 */
#define TISCI_MSG_VALUE_RM_RING_SIZE_4B        (0x0u)
/**
 * 8-byte ring element size for @ref tisci_msg_rm_ring_cfg_req::size
 */
#define TISCI_MSG_VALUE_RM_RING_SIZE_8B        (0x1u)
/**
 * 16-byte ring element size for @ref tisci_msg_rm_ring_cfg_req::size
 */
#define TISCI_MSG_VALUE_RM_RING_SIZE_16B       (0x2u)
/**
 * 32-byte ring element size for @ref tisci_msg_rm_ring_cfg_req::size
 */
#define TISCI_MSG_VALUE_RM_RING_SIZE_32B       (0x3u)
/**
 * 64-byte ring element size for @ref tisci_msg_rm_ring_cfg_req::size
 */
#define TISCI_MSG_VALUE_RM_RING_SIZE_64B       (0x4u)
/**
 * 128-byte ring element size for @ref tisci_msg_rm_ring_cfg_req::size
 */
#define TISCI_MSG_VALUE_RM_RING_SIZE_128B      (0x5u)
/**
 * 256-byte ring element size for @ref tisci_msg_rm_ring_cfg_req::size
 */
#define TISCI_MSG_VALUE_RM_RING_SIZE_256B      (0x6u)

/**
 * The source parameter is valid for RM monitor configure TISCI message
 */
#define TISCI_MSG_VALUE_RM_MON_SOURCE_VALID    (1u << 0U)
/**
 * The mode parameter is valid for RM monitor configure TISCI message
 */
#define TISCI_MSG_VALUE_RM_MON_MODE_VALID      (1u << 1U)
/**
 * The queue parameter is valid for RM monitor configure TISCI message
 */
#define TISCI_MSG_VALUE_RM_MON_QUEUE_VALID     (1u << 2U)
/**
 * The data1_val parameter is valid for RM monitor configure TISCI message
 */
#define TISCI_MSG_VALUE_RM_MON_DATA0_VAL_VALID (1u << 3U)
/**
 * The data0_val parameter is valid for RM monitor configure TISCI message
 */
#define TISCI_MSG_VALUE_RM_MON_DATA1_VAL_VALID (1u << 4U)

/**
 * Element count is source for @ref tisci_msg_rm_ring_mon_cfg_req::source
 */
#define TISCI_MSG_VALUE_RM_MON_SRC_ELEM_CNT        (0U)
/**
 * Head packet size is source for @ref tisci_msg_rm_ring_mon_cfg_req::source
 */
#define TISCI_MSG_VALUE_RM_MON_SRC_HEAD_PKT_SIZE   (1U)
/**
 * Accumulated queue size is source for @ref tisci_msg_rm_ring_mon_cfg_req::source
 */
#define TISCI_MSG_VALUE_RM_MON_SRC_ACCUM_Q_SIZE    (2U)

/**
 * Disabled monitor mode for @ref tisci_msg_rm_ring_mon_cfg_req::mode
 */
#define TISCI_MSG_VALUE_RM_MON_MODE_DISABLED       (0U)
/**
 * Push/pop statistics capture mode for @ref tisci_msg_rm_ring_mon_cfg_req::mode
 */
#define TISCI_MSG_VALUE_RM_MON_MODE_PUSH_POP       (1U)
/**
 * Low/high threshold checks mode for @ref tisci_msg_rm_ring_mon_cfg_req::mode
 */
#define TISCI_MSG_VALUE_RM_MON_MODE_THRESHOLD      (2U)
/**
 * Low/high watermarking mode for @ref tisci_msg_rm_ring_mon_cfg_req::mode
 */
#define TISCI_MSG_VALUE_RM_MON_MODE_WATERMARK      (3U)
/**
 * Starvation counter mode for @ref tisci_msg_rm_ring_mon_cfg_req::mode
 */
#define TISCI_MSG_VALUE_RM_MON_MODE_STARVATION     (4U)

/**
 * \brief Configures a Navigator Subsystem ring
 *
 * Configures the non-real-time registers of a Navigator Subsystem ring.
 * The ring index must be assigned to the host defined in the TISCI header via
 * the RM board configuration resource assignment range list.
 *
 * Standard TISCI header
 *
 * \param valid_params
 * Bitfield defining validity of ring configuration parameters.  The
 * ring configuration fields are not valid, and will not be used for ring
 * configuration, if their corresponding valid bit is zero.  Valid bit usage:
 *   0 - Valid bit for @ref tisci_msg_rm_ring_cfg_req::addr_lo
 *   1 - Valid bit for @ref tisci_msg_rm_ring_cfg_req::addr_hi
 *   2 - Valid bit for @ref tisci_msg_rm_ring_cfg_req::count
 *   3 - Valid bit for @ref tisci_msg_rm_ring_cfg_req::mode
 *   4 - Valid bit for @ref tisci_msg_rm_ring_cfg_req::size
 *   5 - Valid bit for @ref tisci_msg_rm_ring_cfg_req::order_id
 *   6 - Valid bit for @ref tisci_msg_rm_ring_cfg_req::virtid
 *
 * \param nav_id
 * SoC device ID of Navigator Subsystem where ring is located
 *
 * \param index
 * Ring index.
 *
 * \param addr_lo
 * 32 LSBs of ring base address to be programmed into the ring's RING_BA_LO
 * register.
 *
 * This field is only valid if
 * @ref TISCI_MSG_VALUE_RM_RING_ADDR_LO_VALID is set in
 * @ref tisci_msg_rm_ring_cfg_req::valid_params.
 *
 * \param addr_hi
 * 16 MSBs of ring base address to be programmed into the ring's RING_BA_HI
 * register.  Only the 16 LSBs of @ref addr_hi are used when programming
 * RING_BA_HI, the upper 16 bits are discarded.
 *
 * This field is only valid if
 * @ref TISCI_MSG_VALUE_RM_RING_ADDR_HI_VALID is set in
 * @ref tisci_msg_rm_ring_cfg_req::valid_params.
 *
 * \param count
 * Number of ring elements to be programmed into the size field of the ring's
 * RING_SIZE register.
 *
 * This field is only valid if
 * @ref TISCI_MSG_VALUE_RM_RING_COUNT_VALID is set in
 * @ref tisci_msg_rm_ring_cfg_req::valid_params.
 *
 * \param mode
 * Ring mode to be programmed into the qmode field of the ring's RING_SIZE
 * register.  Can be set to:
 * @ref TISCI_MSG_VALUE_RM_RING_MODE_RING
 * @ref TISCI_MSG_VALUE_RM_RING_MODE_MESSAGE
 * @ref TISCI_MSG_VALUE_RM_RING_MODE_CREDENTIALS
 * @ref TISCI_MSG_VALUE_RM_RING_MODE_QM
 *
 * This field is only valid if
 * @ref TISCI_MSG_VALUE_RM_RING_MODE_VALID is set in
 * @ref tisci_msg_rm_ring_cfg_req::valid_params.
 *
 * \param size
 * Encoded ring element size to be programmed into the elsize field of the
 * ring's RING_SIZE register.  To calculate the encoded size use the
 * formula (log2(size_bytes) - 2), where "size_bytes" cannot be greater than
 * 256 bytes.  Can be set to:
 * @ref TISCI_MSG_VALUE_RM_RING_SIZE_4B
 * @ref TISCI_MSG_VALUE_RM_RING_SIZE_8B
 * @ref TISCI_MSG_VALUE_RM_RING_SIZE_16B
 * @ref TISCI_MSG_VALUE_RM_RING_SIZE_32B
 * @ref TISCI_MSG_VALUE_RM_RING_SIZE_64B
 * @ref TISCI_MSG_VALUE_RM_RING_SIZE_128B
 * @ref TISCI_MSG_VALUE_RM_RING_SIZE_256B
 *
 * This field is only valid if
 * @ref TISCI_MSG_VALUE_RM_RING_SIZE_VALID is set in
 * @ref tisci_msg_rm_ring_cfg_req::valid_params.
 *
 * \param order_id
 * Ring bus order ID value to be programmed into the orderid field of
 * the ring's RING_ORDERID register.  When valid, the replace
 * field of the ring's RING_ORDERID register will be set to 1 so that the
 * programmed order ID will be used.
 *
 * This field is only valid if
 * @ref TISCI_MSG_VALUE_RM_RING_ORDER_ID_VALID is set in
 * @ref tisci_msg_rm_ring_cfg_req::valid_params.
 *
 * \param virtid
 * Ring virt ID value to be programmed into the virtid field of the ring's
 * RING_CONTROL2 ISC region register.  This field is only valid if
 * @ref TISCI_MSG_VALUE_RM_RING_VIRTID_VALID is set in
 * @ref tisci_msg_rm_ring_cfg_req::valid_params.
 */
struct tisci_msg_rm_ring_cfg_req {
    uint32_t            valid_params;
    uint16_t            nav_id;
    uint16_t            index;
    uint32_t            addr_lo;
    uint32_t            addr_hi;
    uint32_t            count;
    uint8_t            mode;
    uint8_t            size;
    uint8_t            order_id;
    uint16_t            virtid;
} __attribute__((__packed__));

/**
 * \brief Response to configuring a ring.
 *
 * Standard TISCI header
 */
struct tisci_msg_rm_ring_cfg_resp {
} __attribute__((__packed__));

/**
 * \brief Get Navigator Subsystem ring's non-real-time register configuration
 *
 * Gets the configuration of the non-real-time register fields of a ring.  The
 * host, or a supervisor of the host, who owns the ring must be the requesting
 * host.  The values of the non-real-time registers are returned in
 * @ref tisci_msg_rm_ring_get_cfg_resp.  The reset_cfg parameter is used to
 * request either the existing non-real-time register values or the hardware
 * reset values for the ring's register fields.
 *
 * Standard TISCI header
 *
 * \param nav_id
 * SoC device ID of Navigator Subsystem in which the ring is located
 *
 * \param index
 * Ring index.
 *
 * \param get_reset_cfg
 * Switch defining which ring configuration is returned:
 * @ref TISCI_MSG_VALUE_RM_RING_GET_CFG_REG - Return non-real-time register
 * configuration
 * @ref TISCI_MSG_VALUE_RM_RING_GET_CFG_RESET - Return non-real-time register
 * hardware reset value configuration
 */
struct tisci_msg_rm_ring_get_cfg_req {
    uint16_t            nav_id;
    uint16_t            index;
    uint8_t            get_reset_cfg;
} __attribute__((__packed__));

/**
 * \brief Ring get configuration response message
 *
 * Response received by host processor after RM has handled
 * @ref tisci_msg_rm_ring_get_cfg_req.  The response contains the ring's
 * non-real-time register values.
 *
 * Standard TISCI header
 *
 * \param addr_lo
 * Ring 32 LSBs of base address.
 *
 * \param addr_hi
 * Ring 16 MSBs of base address.
 *
 * \param count
 * Ring number of elements.
 *
 * \param mode
 * Ring mode.  Will be a value from \ref tisci_msg_value_rm_ring_mode.
 *
 * \param size
 * Ring element size.
 *
 * \param order_id
 * Ring order ID.
 */
struct tisci_msg_rm_ring_get_cfg_resp {
    uint32_t            addr_lo;
    uint32_t            addr_hi;
    uint32_t            count;
    uint8_t            mode;
    uint8_t            size;
    uint8_t            order_id;
} __attribute__((__packed__));

/**
 * \brief Configures a Navigator Subsystem ring monitor.  Configures the
 * real-time registers of a Navigator Subsystem ring monitor.  The ring monitor
 * index must be assigned to the host defined in the TISCI header via the RM
 * board configuration resource assignment range list.  The channelized
 * firewalls covering the ring monitor registers are configured to allow the
 * host read-only access.
 *
 * Standard TISCI header
 *
 * \param valid_params
 * Bitfield defining validity of ring monitor configuration parameters.  The
 * ring monitor configuration fields are not valid, and will not be used for
 * ring monitor configuration, if their corresponding valid bit is zero.  Valid
 * bit usage:
 *   0 - Valid bit for @ref tisci_msg_rm_ring_mon_cfg_req::source
 *   1 - Valid bit for @ref tisci_msg_rm_ring_mon_cfg_req::mode
 *   2 - Valid bit for @ref tisci_msg_rm_ring_mon_cfg_req::queue
 *   3 - Valid bit for @ref tisci_msg_rm_ring_mon_cfg_req::data0_val
 *   4 - Valid bit for @ref tisci_msg_rm_ring_mon_cfg_req::data1_val
 *
 * \param nav_id
 * SoC device ID of Navigator Subsystem where ring monitor is located
 *
 * \param index
 * Ring monitor index.
 *
 * \param source
 * Monitor source selection programmed into RINGACC_CONTROL register.  Can be
 * set to:
 * @ref TISCI_MSG_VALUE_RM_MON_SRC_ELEM_CNT
 * @ref TISCI_MSG_VALUE_RM_MON_SRC_HEAD_PKT_SIZE
 * @ref TISCI_MSG_VALUE_RM_MON_SRC_ACCUM_Q_SIZE
 * This field is only valid if
 * @ref TISCI_MSG_VALUE_RM_MON_SOURCE_VALID is set in
 * @ref tisci_msg_rm_ring_mon_cfg_req::valid_params.
 *
 * \param mode
 * Monitor mode programmed into RINGACC_CONTROL register.  Can be set to:
 * @ref TISCI_MSG_VALUE_RM_MON_MODE_DISABLED
 * @ref TISCI_MSG_VALUE_RM_MON_MODE_PUSH_POP
 * @ref TISCI_MSG_VALUE_RM_MON_MODE_THRESHOLD
 * @ref TISCI_MSG_VALUE_RM_MON_MODE_WATERMARK
 * @ref TISCI_MSG_VALUE_RM_MON_MODE_STARVATION
 * This field is only valid if
 * @ref TISCI_MSG_VALUE_RM_MON_MODE_VALID is set in
 * @ref tisci_msg_rm_ring_mon_cfg_req::valid_params.
 *
 * \param queue
 * Queue, or ring, to monitor programmed into RINGACC_QUEUE register.  The
 * specified queue must be assigned to the host, or a subordinate of the host,
 * requesting the ring monitor configuration.
 * This field is only valid if
 * @ref TISCI_MSG_VALUE_RM_MON_QUEUE_VALID is set in
 * @ref tisci_msg_rm_ring_mon_cfg_req::valid_params.
 *
 * \param data0_val
 * Low threshold value programmed into RINGACC_DATA0 register when the ring
 * monitor mode is configured to low/high threshold checking.  Values specified
 * in this field are ignored for other monitor modes.
 * This field is only valid if
 * @ref TISCI_MSG_VALUE_RM_MON_DATA0_VAL_VALID is set in
 * @ref tisci_msg_rm_ring_mon_cfg_req::valid_params.
 *
 * \param data1_val
 * High threshold value programmed into RINGACC_DATA1 register when the ring
 * monitor mode is configured to low/high threshold checking.  Values specified
 * in this field are ignored for other monitor modes.
 * This field is only valid if
 * @ref TISCI_MSG_VALUE_RM_MON_DATA1_VAL_VALID is set in
 * @ref tisci_msg_rm_ring_mon_cfg_req::valid_params.
 */
struct tisci_msg_rm_ring_mon_cfg_req {
    uint32_t            valid_params;
    uint16_t            nav_id;
    uint16_t            index;
    uint8_t            source;
    uint8_t            mode;
    uint16_t            queue;
    uint32_t            data0_val;
    uint32_t            data1_val;
} __attribute__((__packed__));

/**
 * \brief Response to configuring a ring monitor.
 *
 * Standard TISCI header
 */
struct tisci_msg_rm_ring_mon_cfg_resp {
} __attribute__((__packed__));

#endif /* RM_TISCI_RA_H */

/* @} */
