/*
 * Copyright (C) 2018-2020 Texas Instruments Incorporated - http://www.ti.com/
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of Texas Instruments Incorporated nor the names of
 * its contributors may be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
#include <ti/csl/tistdtypes.h>
#include <ti/csl/soc.h>
#include <ti/csl/arch/csl_arch.h>
#include <ti/csl/hw_types.h>
#include <ti/drv/uart/UART_stdio.h>
#include <ti/drv/uart/soc/UART_soc.h>
#include <ti/board/board.h>
#include <sbl_soc.h>
#include <sbl_soc_cfg.h>
#include <sbl_err_trap.h>
#include <sbl_sci_client.h>

#ifdef __cplusplus
#pragma DATA_SECTION(".firmware")
#else
#pragma WEAK (SBL_ReadSysfwImage)
#pragma DATA_SECTION(gSciclient_firmware, ".firmware")
#endif
uint32_t gSciclient_firmware[1];

#if SCICLIENT_FIRMWARE_SIZE_IN_BYTES > SBL_SYSFW_MAX_SIZE
#error "SYSFW too large...update SBL_SYSFW_MAX_SIZE"
#endif

uint32_t SBL_IsSysfwEnc(uint8_t *x509_cert_ptr)
{
    static uint32_t retVal = SBL_SYSFW_NOT_PROCESSED;
    uint8_t *encSeqPtr = NULL;
    uint32_t outer_cert_len = 0;
    uint8_t *innerCertStart = NULL;
    uint32_t inner_cert_len = 0;
    /* oid encoding of encryption seq extension for RBL - 1.3.6.1.4.1.294.1.4 */
    uint8_t enc_seq_oid[] = {0x06, 0x09, 0x2B, 0x06, 0x01, 0x04, 0x01, 0x82, 0x26, 0x01, 0x04};

    SBL_ADD_PROFILE_POINT;

    if (x509_cert_ptr)
    {
        uint32_t SBL_GetCertLen(uint8_t *x509_cert_ptr);

        outer_cert_len = SBL_GetCertLen(x509_cert_ptr);
        innerCertStart = x509_cert_ptr + outer_cert_len;
        inner_cert_len = SBL_GetCertLen(innerCertStart);
    }

    if (inner_cert_len)
    {
        uint8_t *SBL_FindSeq(uint8_t *x509_cert_ptr, uint32_t x509_cert_size, uint8_t *seq_oid, uint8_t seq_len);

        encSeqPtr = SBL_FindSeq(innerCertStart, inner_cert_len, enc_seq_oid, sizeof(enc_seq_oid));
        if (encSeqPtr)
        {
            /* SYSFW is double signed and encrypted */
            retVal = SBL_SYSFW_ENCRYPTED;
        }
        else
        {
            /* SYSFW is double signed but not encrypted */
            SBL_log(SBL_LOG_ERR,"invalid SYSFW signature \n");
            SblErrLoop(__FILE__, __LINE__);
        }
    }
    else if ((x509_cert_ptr) && (retVal == SBL_SYSFW_NOT_PROCESSED))
    {
        /* SYSFW is single signed */
        retVal = SBL_SYSFW_CLEAR_TEXT;
    }

    SBL_ADD_PROFILE_POINT;

    return retVal;
}

void SBL_SciClientInit(void)
{
    int32_t status = CSL_EFAIL;
    void *sysfw_ptr = gSciclient_firmware;

#ifndef SBL_SKIP_SYSFW_INIT
    /* SYSFW board configurations */
    Sciclient_BoardCfgPrms_t sblBoardCfgPrms =
    {
        .boardConfigLow = (uint32_t)gSciclient_boardCfgLow,
	.boardConfigHigh = 0,
	.boardConfigSize = SCICLIENT_BOARDCFG_SIZE_IN_BYTES,
	.devGrp = SBL_DEVGRP
    };

    Sciclient_BoardCfgPrms_t sblBoardCfgPmPrms =
    {
        .boardConfigLow = (uint32_t)gSciclient_boardCfgLow_pm,
	.boardConfigHigh = 0,
	.boardConfigSize = SCICLIENT_BOARDCFG_PM_SIZE_IN_BYTES,
	.devGrp = SBL_DEVGRP
    };

#if defined(SOC_AM65XX)
    /* Configure RM based on Device ID */
    /* Maxwell PG1 and PG2 must be configured differently */
    uint32_t pBoardConfigLow_rm = 0U;
    uint16_t boardConfigSize_rm = 0U;
    uint32_t dev_id = HW_RD_REG32((CSL_WKUP_CTRL_MMR0_CFG0_BASE
				   + CSL_WKUP_CTRL_MMR_CFG0_JTAGID));

    SBL_log(SBL_LOG_ERR,"DEVICE ID 0x%x\n", dev_id);
    if (dev_id == 0x0BB5A02F) /* SR1 */
    {
      /* RM */
      pBoardConfigLow_rm = (uint32_t)gSciclient_boardCfgLow_rm;
      boardConfigSize_rm = SCICLIENT_BOARDCFG_RM_SIZE_IN_BYTES;
    }
    else if (dev_id == 0x1BB5A02F) /* SR2 */
    {
      /* RM */
      pBoardConfigLow_rm = (uint32_t)gSciclient_boardCfgLow_rm_sr2;
      boardConfigSize_rm = SCICLIENT_BOARDCFG_RM_SR2_SIZE_IN_BYTES;
    }
    else
    {
      SBL_log(SBL_LOG_ERR,"Invalid device ID: 0x%x\n", dev_id);
      SblErrLoop(__FILE__, __LINE__);
    }
    
    Sciclient_BoardCfgPrms_t sblBoardCfgRmPrms =
    {
	.boardConfigLow = pBoardConfigLow_rm,
	.boardConfigHigh = 0,
	.boardConfigSize = boardConfigSize_rm,
	.devGrp = SBL_DEVGRP
    };
#else
    /* Non-Maxwell devices may configure as normal*/
    Sciclient_BoardCfgPrms_t sblBoardCfgRmPrms =
    {
        .boardConfigLow = (uint32_t)gSciclient_boardCfgLow_rm,
	.boardConfigHigh = 0,
	.boardConfigSize = SCICLIENT_BOARDCFG_RM_SIZE_IN_BYTES,
	.devGrp = SBL_DEVGRP
    };
#endif

    Sciclient_BoardCfgPrms_t sblBoardCfgSecPrms =
    {
        .boardConfigLow = (uint32_t)gSciclient_boardCfgLow_sec,
	.boardConfigHigh = 0,
	.boardConfigSize = SCICLIENT_BOARDCFG_SECURITY_SIZE_IN_BYTES,
	.devGrp = SBL_DEVGRP
    };

    Sciclient_ConfigPrms_t        config =
    {
        SCICLIENT_SERVICE_OPERATION_MODE_POLLED,
    };
#endif

    SBL_ADD_PROFILE_POINT;

    status = SBL_ReadSysfwImage(&sysfw_ptr, SBL_SYSFW_MAX_SIZE);
    if (status != CSL_PASS)
    {
        SBL_log(SBL_LOG_ERR,"SYSFW read...FAILED \n");
        SblErrLoop(__FILE__, __LINE__);
    }
    else
    {
        SBL_ADD_PROFILE_POINT;
        SBL_IsSysfwEnc((uint8_t *) sysfw_ptr);
    }

#ifndef SBL_SKIP_SYSFW_INIT

    SBL_ADD_PROFILE_POINT;
    status = Sciclient_loadFirmware((const uint32_t *) sysfw_ptr);
    if (status != CSL_PASS)
    {
        SBL_log(SBL_LOG_ERR,"SYSFW load...FAILED \n");
        SblErrLoop(__FILE__, __LINE__);
    }

    SBL_ADD_PROFILE_POINT;
    status = Sciclient_init(&config);
    if (status != CSL_PASS)
    {
        SBL_log(SBL_LOG_ERR,"SYSFW init ...FAILED \n");
        SblErrLoop(__FILE__, __LINE__);
    }

#ifndef SBL_SKIP_BRD_CFG_BOARD
    SBL_ADD_PROFILE_POINT;
    status = Sciclient_boardCfg(&sblBoardCfgPrms);
    if (status != CSL_PASS)
    {
        SBL_log(SBL_LOG_ERR,"SYSFW board config ...FAILED \n");
        SblErrLoop(__FILE__, __LINE__);
    }
#endif

#ifndef SBL_SKIP_BRD_CFG_PM
    if (SBL_LOG_LEVEL > SBL_LOG_NONE)
    {
        SBL_ADD_PROFILE_POINT;
        UART_stdioDeInit();
    }
    SBL_ADD_PROFILE_POINT;
    status = Sciclient_boardCfgPm(&sblBoardCfgPmPrms);
    if (status != CSL_PASS)
    {
        SBL_log(SBL_LOG_ERR,"SYSFW board config pm...FAILED \n")
        SblErrLoop(__FILE__, __LINE__);
    }
    
    if (SBL_LOG_LEVEL > SBL_LOG_NONE)
    {
        /* Re-init UART for logging */
        UART_HwAttrs uart_cfg;

        SBL_ADD_PROFILE_POINT;
        UART_socGetInitCfg(BOARD_UART_INSTANCE, &uart_cfg);
        uart_cfg.frequency = SBL_SYSFW_UART_MODULE_INPUT_CLK;
        UART_socSetInitCfg(BOARD_UART_INSTANCE, &uart_cfg);
        UART_stdioInit(BOARD_UART_INSTANCE);
    }
#endif

#ifndef SBL_SKIP_BRD_CFG_RM
    SBL_ADD_PROFILE_POINT;
    status = Sciclient_boardCfgRm(&sblBoardCfgRmPrms);
    if (status != CSL_PASS)
    {
        SBL_log(SBL_LOG_ERR,"SYSFW board config rm...FAILED \n");
        SblErrLoop(__FILE__, __LINE__);
    }
#endif

#ifndef SBL_SKIP_BRD_CFG_SEC
    SBL_ADD_PROFILE_POINT;
    status = Sciclient_boardCfgSec(&sblBoardCfgSecPrms);
    if (status != CSL_PASS)
    {
        SBL_log(SBL_LOG_ERR,"SYSFW board config sec...FAILED \n");
        SblErrLoop(__FILE__, __LINE__);
    }
#endif

    /* Get SYSFW version */
    SBL_ADD_PROFILE_POINT;

    if (SBL_LOG_LEVEL > SBL_LOG_ERR)
    {
        struct tisci_msg_version_req req = {0};
        const Sciclient_ReqPrm_t      reqPrm =
        {
            TISCI_MSG_VERSION,
            TISCI_MSG_FLAG_AOP,
            (const uint8_t *)&req,
            sizeof(req),
            SCICLIENT_SERVICE_WAIT_FOREVER
        };

        struct tisci_msg_version_resp response;
        Sciclient_RespPrm_t           respPrm =
        {
            0,
            (uint8_t *) &response,
            (uint32_t)sizeof (response)
        };

        status = Sciclient_service(&reqPrm, &respPrm);
        if (CSL_PASS == status)
        {
            if (respPrm.flags == (uint32_t)TISCI_MSG_FLAG_ACK)
            {
                SBL_ADD_PROFILE_POINT;
                SBL_log(SBL_LOG_MIN,"SYSFW  ver: %s\n", (char *) response.str);
            }
            else
            {
                SBL_log(SBL_LOG_ERR,"SYSFW Get Version failed \n");
                SblErrLoop(__FILE__, __LINE__);
            }
        }
    }

    /* RTI seems to be turned on by ROM. Turning it off so that Power domain can transition */
    Sciclient_pmSetModuleState(SBL_DEV_ID_RTI0, TISCI_MSG_VALUE_DEVICE_SW_STATE_AUTO_OFF, TISCI_MSG_FLAG_AOP, SCICLIENT_SERVICE_WAIT_FOREVER);
    Sciclient_pmSetModuleState(SBL_DEV_ID_RTI1, TISCI_MSG_VALUE_DEVICE_SW_STATE_AUTO_OFF, TISCI_MSG_FLAG_AOP, SCICLIENT_SERVICE_WAIT_FOREVER);
#endif

    SBL_ADD_PROFILE_POINT;
}
