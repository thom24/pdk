/*
 * Copyright (c) 2018-2019, Texas Instruments Incorporated
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
 *  \file sciclient_rm.c
 *
 *  \brief File containing the SCICLIENT API interfaces to the resource
 *         management DMSC services.
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdint.h>
#include <ti/csl/csl_types.h>
#include <ti/drv/sciclient/sciclient.h>
#include <string.h>

#include <ti/csl/tistdtypes.h>
#include <ti/csl/soc.h>
#include <ti/csl/arch/csl_arch.h>
#include <ti/csl/hw_types.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */
#if defined(SOC_AM65XX)
/**
 *  \brief Mapping resources from AM65xx PG2.0 to AM65xx PG1.0.
 *         NOTE: Not all the PG2.0 resources are mapped to PG1.0. Due to this
 *               the API is programmed to return failure if a match is not
 *               found.
 *
 *  \param type     The input type from PG2.0. The same is modified to PG1.0
 *                  in place.
 *  \param subtype  The input subtype from PG2.0. The same is modified to PG1.0
 *                  in place.
 *  \return r       CSL_EFAIL: If the type/subtype is not found.
 *                  CSL_PASS:  If the type/subtype is found.
 */
static int32_t Sciclient_rmGetResourceRange_mapResources (uint16_t *type,
                                                          uint8_t *subtype);
#endif

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

int32_t Sciclient_rmGetResourceRange(
                const struct tisci_msg_rm_get_resource_range_req *req,
                struct tisci_msg_rm_get_resource_range_resp *resp,
                uint32_t timeout)
{
    int32_t r = CSL_PASS;
    Sciclient_ReqPrm_t sciReq ;
    struct tisci_msg_rm_get_resource_range_req req_copy;
    sciReq.messageType    = TISCI_MSG_RM_GET_RESOURCE_RANGE;
    sciReq.flags          = TISCI_MSG_FLAG_AOP;
    sciReq.pReqPayload    = (const uint8_t *) &req_copy;
    sciReq.reqPayloadSize = (uint32_t) sizeof(*req);
    sciReq.timeout        = timeout;
#if defined(SOC_AM65XX)
    uint32_t dev_id = HW_RD_REG32((CSL_WKUP_CTRL_MMR0_CFG0_BASE
                   + CSL_WKUP_CTRL_MMR_CFG0_JTAGID));
#endif

    Sciclient_RespPrm_t sciResp ;
    sciResp.flags           = 0;
    sciResp.pRespPayload    = (uint8_t *) resp;
    sciResp.respPayloadSize = (uint32_t) sizeof(*resp);
    memcpy(&req_copy, req, sizeof(struct tisci_msg_rm_get_resource_range_req));

#if defined(SOC_AM65XX)
            if (dev_id == 0x0BB5A02F) /* SR1 */
            {
                r = Sciclient_rmGetResourceRange_mapResources(&req_copy.type, &req_copy.subtype);
            }
            else if (dev_id == 0x1BB5A02F) /* SR2 */
            {
                /* Do not do anything */
            }
            else
            {
                r = CSL_EFAIL;
            }
#endif
    if (CSL_PASS == r)
    {
        r = Sciclient_service(&sciReq, &sciResp);
    }
    if ((r != CSL_PASS) ||
        ((sciResp.flags & TISCI_MSG_FLAG_ACK) != TISCI_MSG_FLAG_ACK)) {
        r = CSL_EFAIL;
    }

    return r;
}
#if defined(SOC_AM65XX)
static int32_t Sciclient_rmGetResourceRange_mapResources (uint16_t *type,
                                                          uint8_t *subtype)
{
    int32_t r = CSL_PASS;
    switch(*type)
    {
        case TISCI_DEV_GIC0:
            *type = TISCI_RESASG_TYPE_GIC_IRQ;
            switch (*subtype)
            {
                case TISCI_RESASG_SUBTYPE_GIC0_SPI_IRQ_GROUP0_FROM_NAVSS0_INTR_ROUTER_0:    *subtype = TISCI_RESASG_SUBTYPE_GIC_IRQ_MAIN_NAV_SET0; break;
                case TISCI_RESASG_SUBTYPE_GIC0_SPI_IRQ_GROUP0_FROM_GPIOMUX_INTRTR0:         *subtype = TISCI_RESASG_SUBTYPE_GIC_IRQ_WKUP_GPIO; break;
                case TISCI_RESASG_SUBTYPE_GIC0_SPI_IRQ_GROUP1_FROM_NAVSS0_INTR_ROUTER_0:    *subtype = TISCI_RESASG_SUBTYPE_GIC_IRQ_MAIN_NAV_SET1; break;
                case TISCI_RESASG_SUBTYPE_GIC0_SPI_IRQ_GROUP0_FROM_CMPEVENT_INTRTR0:        *subtype = TISCI_RESASG_SUBTYPE_GIC_IRQ_COMP_EVT; break;
                case TISCI_RESASG_SUBTYPE_GIC0_SPI_IRQ_GROUP0_FROM_WKUP_GPIOMUX_INTRTR0:    *subtype = TISCI_RESASG_SUBTYPE_GIC_IRQ_WKUP_GPIO; break;
                default: r = CSL_EFAIL;
            }
            break;
        case TISCI_DEV_PRU_ICSSG0: 
            *type = TISCI_RESASG_TYPE_ICSSG0_IRQ;
            switch (*subtype)
            {
                case TISCI_RESASG_SUBTYPE_PRU_ICSSG0_PR1_SLV_INTR_IRQ_GROUP0_FROM_NAVSS0_INTR_ROUTER_0: *subtype = TISCI_RESASG_SUBTYPE_ICSSG0_IRQ_MAIN_NAV; break;
                case TISCI_RESASG_SUBTYPE_PRU_ICSSG0_PR1_SLV_INTR_IRQ_GROUP0_FROM_GPIOMUX_INTRTR0:      *subtype = TISCI_RESASG_SUBTYPE_ICSSG0_IRQ_MAIN_GPIO; break;
                default: r = CSL_EFAIL;
            }
            break;
        case TISCI_DEV_PRU_ICSSG1: 
            *type = TISCI_RESASG_TYPE_ICSSG1_IRQ;
            switch (*subtype)
            {
                case TISCI_RESASG_SUBTYPE_PRU_ICSSG1_PR1_SLV_INTR_IRQ_GROUP0_FROM_NAVSS0_INTR_ROUTER_0: *subtype = TISCI_RESASG_SUBTYPE_ICSSG1_IRQ_MAIN_NAV; break;
                case TISCI_RESASG_SUBTYPE_PRU_ICSSG1_PR1_SLV_INTR_IRQ_GROUP0_FROM_GPIOMUX_INTRTR0:      *subtype = TISCI_RESASG_SUBTYPE_ICSSG1_IRQ_MAIN_GPIO; break;
                default: r = CSL_EFAIL;
            }
            break;
        case TISCI_DEV_PRU_ICSSG2:
            *type = TISCI_RESASG_TYPE_ICSSG2_IRQ;
            switch (*subtype)
            {
                case TISCI_RESASG_SUBTYPE_PRU_ICSSG2_PR1_SLV_INTR_IRQ_GROUP0_FROM_NAVSS0_INTR_ROUTER_0: *subtype = TISCI_RESASG_SUBTYPE_ICSSG2_IRQ_MAIN_NAV; break;
                case TISCI_RESASG_SUBTYPE_PRU_ICSSG2_PR1_SLV_INTR_IRQ_GROUP0_FROM_GPIOMUX_INTRTR0:      *subtype = TISCI_RESASG_SUBTYPE_ICSSG2_IRQ_MAIN_GPIO; break;
                default: r = CSL_EFAIL;
            }
            break;
        case TISCI_DEV_NAVSS0_MODSS_INTA0: 
            *type = TISCI_RESASG_TYPE_MAIN_NAV_MODSS_IA0;
            switch (*subtype)
            {
                case TISCI_RESASG_SUBTYPE_IA_VINT:             *subtype = TISCI_RESASG_SUBTYPE_MAIN_NAV_MODSS_IA0_VINT; break;
                case TISCI_RESASG_SUBTYPE_GLOBAL_EVENT_SEVT:   *subtype = TISCI_RESASG_SUBTYPE_MAIN_NAV_MODSS_IA0_SEVI; break;
                default: r = CSL_EFAIL;
            }
            break;
        case TISCI_DEV_NAVSS0_MODSS_INTA1: 
            *type = TISCI_RESASG_TYPE_MAIN_NAV_MODSS_IA1;
            switch (*subtype)
            {
                case TISCI_RESASG_SUBTYPE_IA_VINT:             *subtype = TISCI_RESASG_SUBTYPE_MAIN_NAV_MODSS_IA1_VINT; break;
                case TISCI_RESASG_SUBTYPE_GLOBAL_EVENT_SEVT:   *subtype = TISCI_RESASG_SUBTYPE_MAIN_NAV_MODSS_IA1_SEVI; break;
                default: r = CSL_EFAIL;
            }
            break;
        case TISCI_DEV_NAVSS0_RINGACC0: 
            *type = TISCI_RESASG_TYPE_MAIN_NAV_RA;
            switch (*subtype)
            {
                case TISCI_RESASG_SUBTYPE_RA_ERROR_OES:   *subtype = TISCI_RESASG_SUBTYPE_MAIN_NAV_RA_ERROR_OES; break;
                case TISCI_RESASG_SUBTYPE_RA_GP:          *subtype = TISCI_RESASG_SUBTYPE_MAIN_NAV_RA_RING_GP; break;
                case TISCI_RESASG_SUBTYPE_RA_UDMAP_RX:    *subtype = TISCI_RESASG_SUBTYPE_MAIN_NAV_RA_RING_UDMAP_RX; break;
                case TISCI_RESASG_SUBTYPE_RA_UDMAP_TX:    *subtype = TISCI_RESASG_SUBTYPE_MAIN_NAV_RA_RING_UDMAP_TX; break;
                case TISCI_RESASG_SUBTYPE_RA_UDMAP_TX_EXT:*subtype = TISCI_RESASG_SUBTYPE_MAIN_NAV_RA_RING_UDMAP_TX; break;
                case TISCI_RESASG_SUBTYPE_RA_UDMAP_RX_H:  *subtype = TISCI_RESASG_SUBTYPE_MAIN_NAV_RA_RING_UDMAP_RX; break;
                case TISCI_RESASG_SUBTYPE_RA_UDMAP_TX_H:  *subtype = TISCI_RESASG_SUBTYPE_MAIN_NAV_RA_RING_UDMAP_TX; break;
                case TISCI_RESASG_SUBTYPE_RA_VIRTID:      *subtype = TISCI_RESASG_SUBTYPE_MAIN_NAV_RA_VIRTID; break;
                default: r = CSL_EFAIL;
            }
            break;
        case TISCI_DEV_NAVSS0_UDMAP0:
            *type = TISCI_RESASG_TYPE_MAIN_NAV_UDMAP;
            switch (*subtype)
            {
                case TISCI_RESASG_SUBTYPE_UDMAP_RX_FLOW_COMMON:  *subtype = TISCI_RESASG_SUBTYPE_MAIN_NAV_UDMAP_RX_FLOW_COMMON; break;
                case TISCI_RESASG_SUBTYPE_UDMAP_INVALID_FLOW_OES:*subtype = TISCI_RESASG_SUBTYPE_MAIN_NAV_UDMAP_INVALID_FLOW_OES; break;
                case TISCI_RESASG_SUBTYPE_GLOBAL_EVENT_TRIGGER:  *subtype = TISCI_RESASG_SUBTYPE_MAIN_NAV_UDMAP_TRIGGER; break;
                case TISCI_RESASG_SUBTYPE_UDMAP_GLOBAL_CONFIG:   *subtype = TISCI_RESASG_SUBTYPE_MAIN_NAV_UDMAP_GCFG; break;
                case TISCI_RESASG_SUBTYPE_UDMAP_RX_CHAN:         *subtype = TISCI_RESASG_SUBTYPE_MAIN_NAV_UDMAP_RX_CHAN; break;
                case TISCI_RESASG_SUBTYPE_UDMAP_RX_HCHAN:        *subtype = TISCI_RESASG_SUBTYPE_MAIN_NAV_UDMAP_RX_HCHAN; break;
                case TISCI_RESASG_SUBTYPE_UDMAP_TX_CHAN:         *subtype = TISCI_RESASG_SUBTYPE_MAIN_NAV_UDMAP_TX_CHAN; break;
                case TISCI_RESASG_SUBTYPE_UDMAP_TX_ECHAN:        *subtype = TISCI_RESASG_SUBTYPE_MAIN_NAV_UDMAP_TX_ECHAN; break;
                case TISCI_RESASG_SUBTYPE_UDMAP_TX_HCHAN:        *subtype = TISCI_RESASG_SUBTYPE_MAIN_NAV_UDMAP_TX_HCHAN; break;
                default: r = CSL_EFAIL;
            }
            break;
        case TISCI_DEV_NAVSS0_UDMASS_INTA0:
            *type = TISCI_RESASG_TYPE_MAIN_NAV_UDMASS_IA0;
            switch (*subtype)
            {
                case TISCI_RESASG_SUBTYPE_IA_VINT          : *subtype = TISCI_RESASG_SUBTYPE_MAIN_NAV_UDMASS_IA0_VINT; break;
                case TISCI_RESASG_SUBTYPE_GLOBAL_EVENT_GEVT: *subtype = TISCI_RESASG_SUBTYPE_MAIN_NAV_UDMASS_IA0_GEVI; break;
                case TISCI_RESASG_SUBTYPE_GLOBAL_EVENT_MEVT: *subtype = TISCI_RESASG_SUBTYPE_MAIN_NAV_UDMASS_IA0_MEVI; break;
                case TISCI_RESASG_SUBTYPE_GLOBAL_EVENT_SEVT: *subtype = TISCI_RESASG_SUBTYPE_MAIN_NAV_UDMASS_IA0_SEVI; break;
                default: r = CSL_EFAIL;
            }
            break;
        case TISCI_DEV_MCU_NAVSS0_RINGACC0: 
            *type = TISCI_RESASG_TYPE_MCU_NAV_RA;
            switch (*subtype)
            {
                case TISCI_RESASG_SUBTYPE_RA_ERROR_OES : *subtype = TISCI_RESASG_SUBTYPE_MCU_NAV_RA_ERROR_OES; break;
                case TISCI_RESASG_SUBTYPE_RA_GP        : *subtype = TISCI_RESASG_SUBTYPE_MCU_NAV_RA_RING_GP; break;
                case TISCI_RESASG_SUBTYPE_RA_UDMAP_RX  : *subtype = TISCI_RESASG_SUBTYPE_MCU_NAV_RA_RING_UDMAP_RX; break;
                case TISCI_RESASG_SUBTYPE_RA_UDMAP_TX  : *subtype = TISCI_RESASG_SUBTYPE_MCU_NAV_RA_RING_UDMAP_TX; break;
                case TISCI_RESASG_SUBTYPE_RA_UDMAP_RX_H: *subtype = TISCI_RESASG_SUBTYPE_MCU_NAV_RA_RING_UDMAP_RX; break;
                case TISCI_RESASG_SUBTYPE_RA_UDMAP_TX_H: *subtype = TISCI_RESASG_SUBTYPE_MCU_NAV_RA_RING_UDMAP_TX; break;
                case TISCI_RESASG_SUBTYPE_RA_VIRTID    : *subtype = TISCI_RESASG_SUBTYPE_MCU_NAV_RA_VIRTID; break;
                default: r = CSL_EFAIL;
            }
            break;
        case TISCI_DEV_MCU_NAVSS0_UDMAP0:
            *type = TISCI_RESASG_TYPE_MCU_NAV_UDMAP;
            switch (*subtype)
            {
                case TISCI_RESASG_SUBTYPE_UDMAP_RX_FLOW_COMMON  : *subtype = TISCI_RESASG_SUBTYPE_MCU_NAV_UDMAP_RX_FLOW_COMMON; break;
                case TISCI_RESASG_SUBTYPE_UDMAP_INVALID_FLOW_OES: *subtype = TISCI_RESASG_SUBTYPE_MCU_NAV_UDMAP_INVALID_FLOW_OES; break;
                case TISCI_RESASG_SUBTYPE_GLOBAL_EVENT_TRIGGER  : *subtype = TISCI_RESASG_SUBTYPE_MCU_NAV_UDMAP_TRIGGER; break;
                case TISCI_RESASG_SUBTYPE_UDMAP_GLOBAL_CONFIG   : *subtype = TISCI_RESASG_SUBTYPE_MCU_NAV_UDMAP_GCFG; break;
                case TISCI_RESASG_SUBTYPE_UDMAP_RX_CHAN         : *subtype = TISCI_RESASG_SUBTYPE_MCU_NAV_UDMAP_RX_CHAN; break;
                case TISCI_RESASG_SUBTYPE_UDMAP_RX_HCHAN        : *subtype = TISCI_RESASG_SUBTYPE_MCU_NAV_UDMAP_RX_HCHAN; break;
                case TISCI_RESASG_SUBTYPE_UDMAP_TX_CHAN         : *subtype = TISCI_RESASG_SUBTYPE_MCU_NAV_UDMAP_TX_CHAN; break;
                case TISCI_RESASG_SUBTYPE_UDMAP_TX_HCHAN        : *subtype = TISCI_RESASG_SUBTYPE_MCU_NAV_UDMAP_TX_HCHAN; break;
                default: r = CSL_EFAIL;
            }
            break;
        case TISCI_DEV_MCU_NAVSS0_INTR_AGGR_0: 
            *type = TISCI_RESASG_TYPE_MCU_NAV_UDMASS_IA0;
            switch (*subtype)
            {
                case TISCI_RESASG_SUBTYPE_IA_VINT          : *subtype = TISCI_RESASG_SUBTYPE_MCU_NAV_UDMASS_IA0_VINT; break;
                case TISCI_RESASG_SUBTYPE_GLOBAL_EVENT_GEVT: *subtype = TISCI_RESASG_SUBTYPE_MCU_NAV_UDMASS_IA0_GEVI; break;
                case TISCI_RESASG_SUBTYPE_GLOBAL_EVENT_MEVT: *subtype = TISCI_RESASG_SUBTYPE_MCU_NAV_UDMASS_IA0_MEVI; break;
                case TISCI_RESASG_SUBTYPE_GLOBAL_EVENT_SEVT: *subtype = TISCI_RESASG_SUBTYPE_MCU_NAV_UDMASS_IA0_SEVI; break;
                default: r = CSL_EFAIL;
            }
            break;
        case TISCI_DEV_MCU_ARMSS0_CPU0:
            *type = TISCI_RESASG_TYPE_PULSAR_C0_IRQ;
            switch (*subtype)
            {
                case TISCI_RESASG_SUBTYPE_MCU_ARMSS0_CPU0_INTR_IRQ_GROUP0_FROM_MCU_NAVSS0_INTR_ROUTER_0: *subtype = TISCI_RESASG_SUBTYPE_PULSAR_C0_IRQ_MCU_NAV ; break;
                case TISCI_RESASG_SUBTYPE_MCU_ARMSS0_CPU0_INTR_IRQ_GROUP0_FROM_WKUP_GPIOMUX_INTRTR0    : *subtype = TISCI_RESASG_SUBTYPE_PULSAR_C0_IRQ_WKUP_GPIO; break;
                case TISCI_RESASG_SUBTYPE_MCU_ARMSS0_CPU0_INTR_IRQ_GROUP0_FROM_MAIN2MCU_LVL_INTRTR0    : *subtype = TISCI_RESASG_SUBTYPE_PULSAR_C0_IRQ_MAIN2MCU_LVL; break;
                case TISCI_RESASG_SUBTYPE_MCU_ARMSS0_CPU0_INTR_IRQ_GROUP0_FROM_MAIN2MCU_PLS_INTRTR0    : *subtype = TISCI_RESASG_SUBTYPE_PULSAR_C0_IRQ_MAIN2MCU_PLS; break;
                default: r = CSL_EFAIL;
            }
            break;
        case TISCI_DEV_MCU_ARMSS0_CPU1:
            *type = TISCI_RESASG_TYPE_PULSAR_C1_IRQ;
            switch (*subtype)
            {
                case TISCI_RESASG_SUBTYPE_MCU_ARMSS0_CPU1_INTR_IRQ_GROUP0_FROM_MCU_NAVSS0_INTR_ROUTER_0: *subtype = TISCI_RESASG_SUBTYPE_PULSAR_C1_IRQ_MCU_NAV ; break;
                case TISCI_RESASG_SUBTYPE_MCU_ARMSS0_CPU1_INTR_IRQ_GROUP0_FROM_WKUP_GPIOMUX_INTRTR0    : *subtype = TISCI_RESASG_SUBTYPE_PULSAR_C1_IRQ_WKUP_GPIO; break;
                case TISCI_RESASG_SUBTYPE_MCU_ARMSS0_CPU1_INTR_IRQ_GROUP0_FROM_MAIN2MCU_LVL_INTRTR0    : *subtype = TISCI_RESASG_SUBTYPE_PULSAR_C1_IRQ_MAIN2MCU_LVL; break;
                case TISCI_RESASG_SUBTYPE_MCU_ARMSS0_CPU1_INTR_IRQ_GROUP0_FROM_MAIN2MCU_PLS_INTRTR0    : *subtype = TISCI_RESASG_SUBTYPE_PULSAR_C1_IRQ_MAIN2MCU_PLS; break;
                default: r = CSL_EFAIL;
            }
            break;
        default: r = CSL_EFAIL; break;
    }
    return r;
}
#endif

int32_t Sciclient_rmIrqSet(const struct tisci_msg_rm_irq_set_req *req,
                           const struct tisci_msg_rm_irq_set_resp *resp,
                           uint32_t timeout)
{
    int32_t r;
    Sciclient_ReqPrm_t sciReq ;
    sciReq.messageType    = TISCI_MSG_RM_IRQ_SET;
    sciReq.flags          = TISCI_MSG_FLAG_AOP;
    sciReq.pReqPayload    = (const uint8_t *) req;
    sciReq.reqPayloadSize = (uint32_t) sizeof(*req);
    sciReq.timeout        = timeout;

    Sciclient_RespPrm_t sciResp ;
    sciResp.flags           = 0;
    sciResp.pRespPayload    = (uint8_t *) resp;
    sciResp.respPayloadSize = (uint32_t) sizeof(*resp);


    r = Sciclient_service(&sciReq, &sciResp);
    if ((r != CSL_PASS) ||
        ((sciResp.flags & TISCI_MSG_FLAG_ACK) != TISCI_MSG_FLAG_ACK)) {
        r = CSL_EFAIL;
    }

    return r;
}

int32_t Sciclient_rmIrqRelease(const struct tisci_msg_rm_irq_release_req *req,
                               uint32_t timeout)
{
    int32_t r;

    Sciclient_ReqPrm_t sciReq ;
    sciReq.messageType    = TISCI_MSG_RM_IRQ_RELEASE;
    sciReq.flags          = TISCI_MSG_FLAG_AOP;
    sciReq.pReqPayload    = (const uint8_t *) req;
    sciReq.reqPayloadSize = (uint32_t) sizeof(*req);
    sciReq.timeout        = timeout;

    Sciclient_RespPrm_t sciResp ;
    struct tisci_msg_rm_irq_release_resp resp;
    sciResp.flags           = 0;
    sciResp.pRespPayload    = (uint8_t *)&resp;
    sciResp.respPayloadSize = sizeof(resp);

    r = Sciclient_service(&sciReq, &sciResp);
    if ((r != CSL_PASS) ||
        ((sciResp.flags & TISCI_MSG_FLAG_ACK) != TISCI_MSG_FLAG_ACK)) {
        r = CSL_EFAIL;
    }

    return r;
}

int32_t Sciclient_rmRingCfg(const struct tisci_msg_rm_ring_cfg_req *req,
                            const struct tisci_msg_rm_ring_cfg_resp *resp,
                            uint32_t timeout)
{
    int32_t r;
    Sciclient_ReqPrm_t sciReq ;
    sciReq.messageType    = TISCI_MSG_RM_RING_CFG;
    sciReq.flags          = TISCI_MSG_FLAG_AOP;
    sciReq.pReqPayload    = (const uint8_t *) req;
    sciReq.reqPayloadSize = (uint32_t) sizeof(*req);
    sciReq.timeout        = timeout;

    Sciclient_RespPrm_t sciResp ;
    sciResp.flags           = 0;
    sciResp.pRespPayload    = (uint8_t *) resp;
    sciResp.respPayloadSize = (uint32_t) sizeof(*resp);

    r = Sciclient_service(&sciReq, &sciResp);
    if ((r != CSL_PASS) ||
        ((sciResp.flags & TISCI_MSG_FLAG_ACK) != TISCI_MSG_FLAG_ACK)) {
        r = CSL_EFAIL;
    }

    return r;
}

int32_t Sciclient_rmRingMonCfg(const struct tisci_msg_rm_ring_mon_cfg_req *req,
                               const struct tisci_msg_rm_ring_mon_cfg_resp *resp,
                               uint32_t timeout)
{
    int32_t r;
    Sciclient_ReqPrm_t sciReq ;
    sciReq.messageType    = TISCI_MSG_RM_RING_MON_CFG;
    sciReq.flags          = TISCI_MSG_FLAG_AOP;
    sciReq.pReqPayload    = (const uint8_t *) req;
    sciReq.reqPayloadSize = (uint32_t) sizeof(*req);
    sciReq.timeout        = timeout;

    Sciclient_RespPrm_t sciResp ;
    sciResp.flags           = 0;
    sciResp.pRespPayload    = (uint8_t *) resp;
    sciResp.respPayloadSize = (uint32_t) sizeof(*resp);

    r = Sciclient_service(&sciReq, &sciResp);
    if ((r != CSL_PASS) ||
        ((sciResp.flags & TISCI_MSG_FLAG_ACK) != TISCI_MSG_FLAG_ACK)) {
        r = CSL_EFAIL;
    }

    return r;
}

int32_t Sciclient_rmUdmapGcfgCfg(
            const struct tisci_msg_rm_udmap_gcfg_cfg_req *req,
            const struct tisci_msg_rm_udmap_gcfg_cfg_resp *resp,
            uint32_t timeout)
{
    int32_t r;
    Sciclient_ReqPrm_t sciReq ;
    sciReq.messageType    = TISCI_MSG_RM_UDMAP_GCFG_CFG;
    sciReq.flags          = TISCI_MSG_FLAG_AOP;
    sciReq.pReqPayload    = (const uint8_t *) req;
    sciReq.reqPayloadSize = (uint32_t) sizeof(*req);
    sciReq.timeout        = timeout;

    Sciclient_RespPrm_t sciResp ;
    sciResp.flags           = 0;
    sciResp.pRespPayload    = (uint8_t *) resp;
    sciResp.respPayloadSize = (uint32_t) sizeof(*resp);



    r = Sciclient_service(&sciReq, &sciResp);
    if ((r != CSL_PASS) ||
        ((sciResp.flags & TISCI_MSG_FLAG_ACK) != TISCI_MSG_FLAG_ACK)) {
        r = CSL_EFAIL;
    }

    return r;
}

int32_t Sciclient_rmUdmapTxChCfg(
            const struct tisci_msg_rm_udmap_tx_ch_cfg_req *req,
            const struct tisci_msg_rm_udmap_tx_ch_cfg_resp *resp,
            uint32_t timeout)
{
    int32_t r;
    Sciclient_ReqPrm_t sciReq ;
    sciReq.messageType    = TISCI_MSG_RM_UDMAP_TX_CH_CFG;
    sciReq.flags          = TISCI_MSG_FLAG_AOP;
    sciReq.pReqPayload    = (const uint8_t *) req;
    sciReq.reqPayloadSize = (uint32_t) sizeof(*req);
    sciReq.timeout        = timeout;

    Sciclient_RespPrm_t sciResp ;
    sciResp.flags           = 0;
    sciResp.pRespPayload    = (uint8_t *) resp;
    sciResp.respPayloadSize = (uint32_t) sizeof(*resp);


    r = Sciclient_service(&sciReq, &sciResp);
    if ((r != CSL_PASS) ||
        ((sciResp.flags & TISCI_MSG_FLAG_ACK) != TISCI_MSG_FLAG_ACK)) {
        r = CSL_EFAIL;
    }

    return r;
}

int32_t Sciclient_rmUdmapRxChCfg(
            const struct tisci_msg_rm_udmap_rx_ch_cfg_req *req,
            const struct tisci_msg_rm_udmap_rx_ch_cfg_resp *resp,
            uint32_t timeout)
{
    int32_t r;
    Sciclient_ReqPrm_t sciReq ;
    sciReq.messageType    = TISCI_MSG_RM_UDMAP_RX_CH_CFG;
    sciReq.flags          = TISCI_MSG_FLAG_AOP;
    sciReq.pReqPayload    = (const uint8_t *) req;
    sciReq.reqPayloadSize = (uint32_t) sizeof(*req);
    sciReq.timeout        = timeout;

    Sciclient_RespPrm_t sciResp ;
    sciResp.flags           = 0;
    sciResp.pRespPayload    = (uint8_t *) resp;
    sciResp.respPayloadSize = (uint32_t) sizeof(*resp);



    r = Sciclient_service(&sciReq, &sciResp);
    if ((r != CSL_PASS) ||
        ((sciResp.flags & TISCI_MSG_FLAG_ACK) != TISCI_MSG_FLAG_ACK)) {
        r = CSL_EFAIL;
    }

    return r;
}

int32_t Sciclient_rmUdmapFlowCfg(
            const struct tisci_msg_rm_udmap_flow_cfg_req *req,
            const struct tisci_msg_rm_udmap_flow_cfg_resp *resp,
            uint32_t timeout)
{
    int32_t r;
    Sciclient_ReqPrm_t sciReq ;
    sciReq.messageType    = TISCI_MSG_RM_UDMAP_FLOW_CFG;
    sciReq.flags          = TISCI_MSG_FLAG_AOP;
    sciReq.pReqPayload    = (const uint8_t *) req;
    sciReq.reqPayloadSize = (uint32_t) sizeof(*req);
    sciReq.timeout        = timeout;

    Sciclient_RespPrm_t sciResp ;
    sciResp.flags           = 0;
    sciResp.pRespPayload    = (uint8_t *) resp;
    sciResp.respPayloadSize = (uint32_t) sizeof(*resp);



    r = Sciclient_service(&sciReq, &sciResp);
    if ((r != CSL_PASS) ||
        ((sciResp.flags & TISCI_MSG_FLAG_ACK) != TISCI_MSG_FLAG_ACK)) {
        r = CSL_EFAIL;
    }

    return r;
}

int32_t Sciclient_rmUdmapFlowSizeThreshCfg(
            const struct tisci_msg_rm_udmap_flow_size_thresh_cfg_req *req,
            const struct tisci_msg_rm_udmap_flow_size_thresh_cfg_resp *resp,
            uint32_t timeout)
{
    int32_t r;
    Sciclient_ReqPrm_t sciReq ;
    sciReq.messageType    = TISCI_MSG_RM_UDMAP_FLOW_SIZE_THRESH_CFG;
    sciReq.flags          = TISCI_MSG_FLAG_AOP;
    sciReq.pReqPayload    = (const uint8_t *) req;
    sciReq.reqPayloadSize = (uint32_t) sizeof(*req);
    sciReq.timeout        = timeout;

    Sciclient_RespPrm_t sciResp ;
    sciResp.flags           = 0;
    sciResp.pRespPayload    = (uint8_t *) resp;
    sciResp.respPayloadSize = (uint32_t) sizeof(*resp);



    r = Sciclient_service(&sciReq, &sciResp);
    if ((r != CSL_PASS) ||
        ((sciResp.flags & TISCI_MSG_FLAG_ACK) != TISCI_MSG_FLAG_ACK)) {
        r = CSL_EFAIL;
    }

    return r;
}

int32_t Sciclient_rmPsilPair(const struct tisci_msg_rm_psil_pair_req *req,
                             uint32_t timeout)
{
    int32_t r;
    Sciclient_ReqPrm_t sciReq ;
    sciReq.messageType    = TISCI_MSG_RM_PSIL_PAIR;
    sciReq.flags          = TISCI_MSG_FLAG_AOP;
    sciReq.pReqPayload    = (const uint8_t *) req;
    sciReq.reqPayloadSize = (uint32_t) sizeof(*req);
    sciReq.timeout        = timeout;

    Sciclient_RespPrm_t sciResp ;
    struct tisci_msg_rm_psil_pair_resp resp;
    sciResp.flags           = 0;
    sciResp.pRespPayload    = (uint8_t*)&resp;
    sciResp.respPayloadSize = sizeof(resp);

    r = Sciclient_service(&sciReq, &sciResp);
    if ((r != CSL_PASS) ||
        ((sciResp.flags & TISCI_MSG_FLAG_ACK) != TISCI_MSG_FLAG_ACK)) {
        r = CSL_EFAIL;
    }

    return r;
}

int32_t Sciclient_rmPsilUnpair(const struct tisci_msg_rm_psil_unpair_req *req,
                               uint32_t timeout)
{
    int32_t r;
    Sciclient_ReqPrm_t sciReq ;
    sciReq.messageType    = TISCI_MSG_RM_PSIL_UNPAIR;
    sciReq.flags          = TISCI_MSG_FLAG_AOP;
    sciReq.pReqPayload    = (const uint8_t *) req;
    sciReq.reqPayloadSize = (uint32_t) sizeof(*req);
    sciReq.timeout        = timeout;

    Sciclient_RespPrm_t sciResp ;
    struct tisci_msg_rm_psil_unpair_resp resp;
    sciResp.flags           = 0;
    sciResp.pRespPayload    = (uint8_t *)&resp;
    sciResp.respPayloadSize = sizeof(resp);

    r = Sciclient_service(&sciReq, &sciResp);
    if ((r != CSL_PASS) ||
        ((sciResp.flags & TISCI_MSG_FLAG_ACK) != TISCI_MSG_FLAG_ACK)) {
        r = CSL_EFAIL;
    }

    return r;
}

int32_t Sciclient_rmPsilRead(const struct tisci_msg_rm_psil_read_req *req,
                             struct tisci_msg_rm_psil_read_resp *resp,
                             uint32_t timeout)
{
    int32_t r;
    Sciclient_ReqPrm_t sciReq ;
    sciReq.messageType    = TISCI_MSG_RM_PSIL_READ;
    sciReq.flags          = TISCI_MSG_FLAG_AOP;
    sciReq.pReqPayload    = (const uint8_t *) req;
    sciReq.reqPayloadSize = (uint32_t) sizeof(*req);
    sciReq.timeout        = timeout;

    Sciclient_RespPrm_t sciResp ;
    sciResp.flags           = 0;
    sciResp.pRespPayload    = (uint8_t *) resp;
    sciResp.respPayloadSize = (uint32_t) sizeof(*resp);

    r = Sciclient_service(&sciReq, &sciResp);
    if ((r != CSL_PASS) ||
        ((sciResp.flags & TISCI_MSG_FLAG_ACK) != TISCI_MSG_FLAG_ACK)) {
        r = CSL_EFAIL;
    }

    return r;
}

int32_t Sciclient_rmPsilWrite(const struct tisci_msg_rm_psil_write_req *req,
                              uint32_t timeout)
{
    int32_t r;
    Sciclient_ReqPrm_t sciReq ;
    sciReq.messageType    = TISCI_MSG_RM_PSIL_WRITE;
    sciReq.flags          = TISCI_MSG_FLAG_AOP;
    sciReq.pReqPayload    = (const uint8_t *) req;
    sciReq.reqPayloadSize = (uint32_t) sizeof(*req);
    sciReq.timeout        = timeout;

    Sciclient_RespPrm_t sciResp ;
    struct tisci_msg_rm_psil_write_resp resp;
    sciResp.flags           = 0;
    sciResp.pRespPayload    = (uint8_t*)&resp;
    sciResp.respPayloadSize = sizeof(resp);

    r = Sciclient_service(&sciReq, &sciResp);
    if ((r != CSL_PASS) ||
        ((sciResp.flags & TISCI_MSG_FLAG_ACK) != TISCI_MSG_FLAG_ACK)) {
        r = CSL_EFAIL;
    }

    return r;
}

int32_t Sciclient_rmSetProxyCfg(const struct tisci_msg_rm_proxy_cfg_req *req, uint32_t timeout)
{
    int32_t r;
    Sciclient_ReqPrm_t sciReq ;
    sciReq.messageType = TISCI_MSG_RM_PROXY_CFG;
    sciReq.flags       = TISCI_MSG_FLAG_AOP;
    sciReq.pReqPayload    = (const uint8_t *) req;
    sciReq.reqPayloadSize = (uint32_t) sizeof(*req);
    sciReq.timeout        = timeout;

    Sciclient_RespPrm_t sciResp ;
    struct tisci_msg_rm_proxy_cfg_resp resp;
    sciResp.flags           = 0;
    sciResp.pRespPayload    = (uint8_t*)&resp;
    sciResp.respPayloadSize = sizeof(resp);

    r = Sciclient_service(&sciReq, &sciResp);
    if ((r != CSL_PASS) ||
        ((sciResp.flags & TISCI_MSG_FLAG_ACK) != TISCI_MSG_FLAG_ACK)) {
        r = CSL_EFAIL;
    }

    return r;
}


/* -------------------------------------------------------------------------- */
/*                 Internal Function Definitions                              */
/* -------------------------------------------------------------------------- */

/* None */

