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
 *  \file sciclient_boardcfg.c
 *
 *  \brief File containing the APIs to send board configuration
 *       messages.
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <ti/drv/sciclient/src/sciclient_priv.h>
#include <ti/csl/arch/csl_arch.h>

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

#if defined (BUILD_MCU1_0)
const uint32_t gSciclient_boardCfgLow[(SCICLIENT_BOARDCFG_SIZE_IN_BYTES+3U)/4U]
    __attribute__(( aligned(128), section(".boardcfg_data") ))
    = SCICLIENT_BOARDCFG;
const uint32_t gSciclient_boardCfgLow_rm[(SCICLIENT_BOARDCFG_RM_SIZE_IN_BYTES+3U)/4U]
    __attribute__(( aligned(128), section(".boardcfg_data") ))
    = SCICLIENT_BOARDCFG_RM;
const uint32_t gSciclient_boardCfgLow_sec[(SCICLIENT_BOARDCFG_SECURITY_SIZE_IN_BYTES+3U)/4U]
    __attribute__(( aligned(128), section(".boardcfg_data") ))
    = SCICLIENT_BOARDCFG_SECURITY;
const uint32_t gSciclient_boardCfgLow_pm[(SCICLIENT_BOARDCFG_PM_SIZE_IN_BYTES+3U)/4U]
    __attribute__(( aligned(128), section(".boardcfg_data") ))
    = SCICLIENT_BOARDCFG_PM;

#if defined (SOC_AM65XX)
const uint32_t gSciclient_boardCfgLow_rm_pg2[(SCICLIENT_BOARDCFG_RM_PG2_SIZE_IN_BYTES+3U)/4U]
    __attribute__(( aligned(128), section(".boardcfg_data") ))
    = SCICLIENT_BOARDCFG_RM_PG2;
#endif

int32_t Sciclient_boardCfg(const Sciclient_BoardCfgPrms_t * pInPrms)
{
    int32_t retVal = CSL_PASS;
    struct tisci_msg_board_config_req request = {
        .tisci_boardcfgp_low  = (uint32_t) gSciclient_boardCfgLow,
        .tisci_boardcfgp_high = (uint32_t) 0x0U,
        .tisci_boardcfg_size  = (uint16_t) SCICLIENT_BOARDCFG_SIZE_IN_BYTES,
        .tisci_boardcfg_devgrp = (uint8_t) DEVGRP_ALL
    };

    /* NULL pInPrms will retain default values */
    if (pInPrms != NULL)
    {
        request.tisci_boardcfgp_low = pInPrms->boardConfigLow;
        request.tisci_boardcfgp_high = pInPrms->boardConfigHigh;
        request.tisci_boardcfg_size = pInPrms->boardConfigSize;
        request.tisci_boardcfg_devgrp = pInPrms->devGrp;
    }

    Sciclient_ReqPrm_t reqParam = {
        .messageType    = (uint16_t) TISCI_MSG_BOARD_CONFIG,
        .flags          = (uint32_t) TISCI_MSG_FLAG_AOP,
        .pReqPayload    = (const uint8_t *) &request,
        .reqPayloadSize = (uint32_t) sizeof (request),
        .timeout        = (uint32_t) SCICLIENT_SERVICE_WAIT_FOREVER
    };
    Sciclient_RespPrm_t respParam = {
        .flags           = (uint32_t) 0,   /* Populated by the API */
        .pRespPayload    = (uint8_t *) 0,
        .respPayloadSize = (uint32_t) 0
    };
    CSL_armR5CacheWbInv((const void*) request.tisci_boardcfgp_low,
                        request.tisci_boardcfg_size);
    if((CSL_PASS != Sciclient_service(&reqParam, &respParam))
        || ((respParam.flags & TISCI_MSG_FLAG_ACK) != TISCI_MSG_FLAG_ACK))
    {
        retVal = CSL_EFAIL;
    }

    return retVal;
}

int32_t Sciclient_boardCfgPm(const Sciclient_BoardCfgPrms_t * pInPrms)
{
    int32_t retVal = CSL_PASS;
    struct tisci_msg_board_config_pm_req request = {
        .tisci_boardcfg_pmp_low  = (uint32_t) gSciclient_boardCfgLow_pm, /* PM Board Config structure
                                                 definition removed from TISCI */
        .tisci_boardcfg_pmp_high = (uint32_t) 0x0U,
        .tisci_boardcfg_pm_size  = (uint16_t) SCICLIENT_BOARDCFG_PM_SIZE_IN_BYTES,
        .tisci_boardcfg_pm_devgrp = (uint8_t) DEVGRP_ALL

    };

    /* NULL pInPrms will retain default values */
    if (pInPrms != NULL)
    {
        request.tisci_boardcfg_pmp_low = pInPrms->boardConfigLow;
        request.tisci_boardcfg_pmp_high = pInPrms->boardConfigHigh;
        request.tisci_boardcfg_pm_size = pInPrms->boardConfigSize;
        request.tisci_boardcfg_pm_devgrp = pInPrms->devGrp;
    }

    Sciclient_ReqPrm_t reqParam = {
        .messageType    = (uint16_t) TISCI_MSG_BOARD_CONFIG_PM,
        .flags          = (uint32_t) TISCI_MSG_FLAG_AOP,
        .pReqPayload    = (const uint8_t *) &request,
        .reqPayloadSize = (uint32_t) sizeof (request),
        .timeout        = (uint32_t) SCICLIENT_SERVICE_WAIT_FOREVER
    };
    Sciclient_RespPrm_t respParam = {
        .flags           = (uint32_t) 0,   /* Populated by the API */
        .pRespPayload    = (uint8_t *) 0,
        .respPayloadSize = (uint32_t) 0
    };
    CSL_armR5CacheWbInv((const void*) request.tisci_boardcfg_pmp_low,
                            request.tisci_boardcfg_pm_size);
    if((CSL_PASS != Sciclient_service(&reqParam, &respParam))
        || ((respParam.flags & TISCI_MSG_FLAG_ACK) != TISCI_MSG_FLAG_ACK))
    {
        retVal = CSL_EFAIL;
    }
    return retVal;
}

int32_t Sciclient_boardCfgRm(const Sciclient_BoardCfgPrms_t * pInPrms)
{
    int32_t retVal = CSL_PASS;

#if defined(SOC_AM65XX)
    /* Configure RM based on Device ID */
    /* Maxwell PG1 and PG2 must be configured differently */
    uint32_t pBoardConfigLow_rm;
    uint16_t boardConfigSize_rm;

    /* Read Device ID (which is different for each PG verison) */
    uint32_t dev_id = HW_RD_REG32((CSL_WKUP_CTRL_MMR0_CFG0_BASE
				   + CSL_WKUP_CTRL_MMR_CFG0_JTAGID));

    if (dev_id == 0x0BB5A02F) /* PG1 */
    {
      pBoardConfigLow_rm = (uint32_t)gSciclient_boardCfgLow_rm;
      boardConfigSize_rm = SCICLIENT_BOARDCFG_RM_SIZE_IN_BYTES;
    }
    else if (dev_id == 0x1BB5A02F) /* PG2 */
    {
      pBoardConfigLow_rm = (uint32_t)gSciclient_boardCfgLow_rm_pg2;
      boardConfigSize_rm = SCICLIENT_BOARDCFG_RM_PG2_SIZE_IN_BYTES;
    }
    else
    {
      /* Should not be possible to get here. However, it would not be wise 
       * .. to return CSL_EFAIL if we do just in case pInPrms is not NULL.
       * .. Configure for PG1, CSL should catch the issue below. */
      pBoardConfigLow_rm = (uint32_t)gSciclient_boardCfgLow_rm;
      boardConfigSize_rm = SCICLIENT_BOARDCFG_RM_SIZE_IN_BYTES;
    }

    struct tisci_msg_board_config_rm_req request = {
        .tisci_boardcfg_rmp_low  = pBoardConfigLow_rm,
        .tisci_boardcfg_rmp_high = (uint32_t) 0x0U,
        .tisci_boardcfg_rm_size  = (uint16_t) boardConfigSize_rm,
        .tisci_boardcfg_rm_devgrp = (uint8_t) DEVGRP_ALL
    };
#else
    struct tisci_msg_board_config_rm_req request = {
        .tisci_boardcfg_rmp_low  = (uint32_t) gSciclient_boardCfgLow_rm,
        .tisci_boardcfg_rmp_high = (uint32_t) 0x0U,
        .tisci_boardcfg_rm_size  = (uint16_t) SCICLIENT_BOARDCFG_RM_SIZE_IN_BYTES,
        .tisci_boardcfg_rm_devgrp = (uint8_t) DEVGRP_ALL
    };
#endif

    /* NULL pInPrms will retain default values */
    if (pInPrms != NULL)
    {
        request.tisci_boardcfg_rmp_low = pInPrms->boardConfigLow;
        request.tisci_boardcfg_rmp_high = pInPrms->boardConfigHigh;
        request.tisci_boardcfg_rm_size = pInPrms->boardConfigSize;
        request.tisci_boardcfg_rm_devgrp = pInPrms->devGrp;
    }

    Sciclient_ReqPrm_t reqParam = {
        .messageType    = (uint16_t) TISCI_MSG_BOARD_CONFIG_RM,
        .flags          = (uint32_t) TISCI_MSG_FLAG_AOP,
        .pReqPayload    = (const uint8_t *) &request,
        .reqPayloadSize = (uint32_t) sizeof (request),
        .timeout        = (uint32_t) SCICLIENT_SERVICE_WAIT_FOREVER
    };
    Sciclient_RespPrm_t respParam = {
        .flags           = (uint32_t) 0,   /* Populated by the API */
        .pRespPayload    = (uint8_t *) 0,
        .respPayloadSize = (uint32_t) 0
    };
    CSL_armR5CacheWbInv((const void*) request.tisci_boardcfg_rmp_low,
                            request.tisci_boardcfg_rm_size);
    if((CSL_PASS != Sciclient_service(&reqParam, &respParam))
        || ((respParam.flags & TISCI_MSG_FLAG_ACK) != TISCI_MSG_FLAG_ACK))
    {
        retVal = CSL_EFAIL;
    }
    return retVal;
}

int32_t Sciclient_boardCfgSec(const Sciclient_BoardCfgPrms_t * pInPrms)
{
    int32_t retVal = CSL_PASS;
    struct tisci_msg_board_config_security_req request = {
        .tisci_boardcfg_securityp_low  = (uint32_t) gSciclient_boardCfgLow_sec,
        .tisci_boardcfg_securityp_high = (uint32_t) 0x0U,
        .tisci_boardcfg_security_size  = (uint16_t) SCICLIENT_BOARDCFG_SECURITY_SIZE_IN_BYTES,
        .tisci_boardcfg_security_devgrp = (uint8_t) DEVGRP_ALL
    };

    /* NULL pInPrms will retain default values */
    if (pInPrms != NULL)
    {
        request.tisci_boardcfg_securityp_low = pInPrms->boardConfigLow;
        request.tisci_boardcfg_securityp_high = pInPrms->boardConfigHigh;
        request.tisci_boardcfg_security_size = pInPrms->boardConfigSize;
        request.tisci_boardcfg_security_devgrp = pInPrms->devGrp;
    }

    Sciclient_ReqPrm_t reqParam = {
        .messageType    = (uint16_t) TISCI_MSG_BOARD_CONFIG_SECURITY,
        .flags          = (uint32_t) TISCI_MSG_FLAG_AOP, 
        .pReqPayload    = (const uint8_t *) &request,
        .reqPayloadSize = (uint32_t) sizeof (request),
        .timeout        = (uint32_t) SCICLIENT_SERVICE_WAIT_FOREVER
    };
    Sciclient_RespPrm_t respParam = {
        .flags           = (uint32_t) 0,   /* Populated by the API */
        .pRespPayload    = (uint8_t *) 0,
        .respPayloadSize = (uint32_t) 0
    };
    CSL_armR5CacheWbInv((const void*) request.tisci_boardcfg_securityp_low,
                            request.tisci_boardcfg_security_size);
    if((CSL_PASS != Sciclient_service(&reqParam, &respParam))
        || ((respParam.flags & TISCI_MSG_FLAG_ACK) != TISCI_MSG_FLAG_ACK))
    {
        retVal = CSL_EFAIL;
    }
    return retVal;
}

#endif

