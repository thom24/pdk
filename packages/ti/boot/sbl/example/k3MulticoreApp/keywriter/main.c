/**
 *  \file   main.c
 *
 *  \brief  This file contains main function, UART init functions & call to 
 *  OTP Keywriter API
 *
 */

/*
 * Copyright (C) 2020 Texas Instruments Incorporated - http://www.ti.com/
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

#include "main.h"

volatile int dbg;

extern uint32_t keywr_end;

/* These two macros define size, and hex code of system firmware Key writer binary */
uint32_t gSciclient_firmware[(TIFS_KEYWRITER_BIN_SIZE_IN_BYTES+3)/4] = TIFS_KEYWRITER_BIN;

Sciclient_BoardCfgPrms_t sblBoardCfgPrms = {0};
Sciclient_BoardCfgPrms_t sblBoardCfgPmPrms = {0};
Sciclient_BoardCfgPrms_t sblBoardCfgSecPrms = {0};

static void OTP_SciClientInit(void)
{
	int32_t status = CSL_EFAIL;
	void *sysfw_ptr = gSciclient_firmware;
	/* SYSFW board configurations */
	Sciclient_DefaultBoardCfgInfo_t boardCfgInfo;
	Sciclient_ConfigPrms_t        config =
	{
		SCICLIENT_SERVICE_OPERATION_MODE_POLLED,
		NULL,
		1,
		0,
		TRUE
	};

	status = Sciclient_getDefaultBoardCfgInfo(&boardCfgInfo);
	if (status != CSL_PASS)
	{
		UART_printf("Sciclient get default board config...FAILED \n");
		KeywrErrLoop(__FILE__, __LINE__);
	}

	status = Sciclient_loadFirmware((const uint32_t *) sysfw_ptr);
	if (status != CSL_PASS)
    {
        /* UART_printf("TIFS load...FAILED \n"); */
		KeywrErrLoop(__FILE__, __LINE__);
    }

	status = Sciclient_init(&config);
	if (status != CSL_PASS)
	{
		UART_printf("Sciclient init ...FAILED \n");
		KeywrErrLoop(__FILE__, __LINE__);
	}


	sblBoardCfgPrms.boardConfigLow = (uint32_t)boardCfgInfo.boardCfgLow;
	sblBoardCfgPrms.boardConfigHigh = 0;
	sblBoardCfgPrms.boardConfigSize = boardCfgInfo.boardCfgLowSize;
	sblBoardCfgPrms.devGrp = DEVGRP_ALL;
	status = Sciclient_boardCfg(&sblBoardCfgPrms);
	if (status != CSL_PASS)
	{
		UART_printf("Sciclient board config ...FAILED \n");
		KeywrErrLoop(__FILE__, __LINE__);
	}

	 if (KEYWRITER_LOG_LEVEL > OTP_LOG_NONE)                                        
	 {                                                                        
	     UART_stdioDeInit();                                                  
	 }                                                                        
	 sblBoardCfgPmPrms.boardConfigLow = (uint32_t)boardCfgInfo.boardCfgLowPm; 
	 sblBoardCfgPmPrms.boardConfigHigh = 0;                                   
	 sblBoardCfgPmPrms.boardConfigSize = boardCfgInfo.boardCfgLowPmSize;      
	 sblBoardCfgPmPrms.devGrp = DEVGRP_ALL;                                   
	 status = Sciclient_boardCfgPm(&sblBoardCfgPmPrms);                       
	 if (status != CSL_PASS)                                                  
	 {                                                                        
	     UART_printf("Sciclient board config pm...FAILED \n");
	     KeywrErrLoop(__FILE__, __LINE__);                                      
	 }                                                                        
	 if (KEYWRITER_LOG_LEVEL > OTP_LOG_NONE)                                        
	 {
		/* Re-init UART for logging */                                                                        
	    UART_HwAttrs uart_cfg;                                               
	                                                                          
	    UART_socGetInitCfg(BOARD_UART_INSTANCE, &uart_cfg);                  
	    uart_cfg.frequency = SBL_SYSFW_UART_MODULE_INPUT_CLK;                
	    UART_socSetInitCfg(BOARD_UART_INSTANCE, &uart_cfg);                  
	    UART_stdioInit(BOARD_UART_INSTANCE);                                 
	 }
	
	
	sblBoardCfgSecPrms.boardConfigLow = (uint32_t)boardCfgInfo.boardCfgLowSec;
	sblBoardCfgSecPrms.boardConfigHigh = 0;
	sblBoardCfgSecPrms.boardConfigSize = boardCfgInfo.boardCfgLowSecSize;
	sblBoardCfgSecPrms.devGrp = DEVGRP_ALL;
	status = Sciclient_boardCfgSec(&sblBoardCfgSecPrms);
	if (status != CSL_PASS)
	{
		UART_printf("Sciclient board config sec...FAILED \n");
		KeywrErrLoop(__FILE__, __LINE__);
	}
	/* Secure ROM has left firewall regions for FSS DAT0 set.  Disable them for DMA usage. */
	uint16_t i;
	struct tisci_msg_fwl_set_firewall_region_resp respFwCtrl = {0};
	struct tisci_msg_fwl_set_firewall_region_req reqFwCtrl =
	{
		.fwl_id = (uint16_t) MCU_FSS0_S0_FWID,
		.region = (uint16_t) 0,
		.n_permission_regs = (uint32_t) 3,
		.control = (uint32_t) 0,
		.permissions[0] = (uint32_t) 0,
		.permissions[1] = (uint32_t) 0,
		.permissions[2] = (uint32_t) 0,
		.start_address = 0,
		.end_address = 0
	};

	for (i = 0; i < MCU_FSS0_S0_FW_REGIONS; i++)
	{
		reqFwCtrl.region = i;
		status = Sciclient_firewallSetRegion(&reqFwCtrl, &respFwCtrl, SCICLIENT_SERVICE_WAIT_FOREVER);
		if (status != CSL_PASS)
		{
			UART_printf("MCU FSS0_S0 firewall region # %d disable...FAILED \n", i);
		}
	}

	/* Skipping board cfg RM, since it is not included in sysfw for keywriter */

	/* Print System Firmware Version Information */

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
			UART_printf("\n OTP Keywriter ver: %s\n", (char *) response.str);
		}
		else
		{
			UART_printf(" OTP Keywriter Get Version failed \n");
			KeywrErrLoop(__FILE__, __LINE__);
		}
	}

}

static void mmr_unlock(uint32_t base, uint32_t partition){
	/*  Translate the base address */
	uint32_t part_base = base + partition * CTRL_MMR0_PARTITION_SIZE;

	HW_WR_REG32(part_base + CTRLMMR_LOCK_KICK0, CTRLMMR_LOCK_KICK0_UNLOCK_VAL);
	HW_WR_REG32(part_base + CTRLMMR_LOCK_KICK1, CTRLMMR_LOCK_KICK1_UNLOCK_VAL);
}

int main()
{
	int32_t status = CSL_EFAIL;
	uint32_t debug_response = 0;
	uint32_t *keywriter_cert = &keywr_end + 1;
	UART_HwAttrs uart_cfg;
	
	/* padconfig unlock */
	mmr_unlock(WKUP_CTRL_BASE, 7);
	/* pinmux for R5 logs */
	HW_WR_REG32(BOARD_MCU_UART_TX_PINMUX_ADDR, PIN_OUTPUT | PIN_MODE(0));
	/* pinmux for M3 logs */
	HW_WR_REG32(WKUP_UART_TXD_MUX_ADDR, PIN_OUTPUT | PIN_MODE(0) );

	UART_socGetInitCfg(BOARD_UART_INSTANCE, &uart_cfg);
	uart_cfg.frequency = SBL_ROM_UART_MODULE_INPUT_CLK;
	uart_cfg.enableInterrupt = FALSE;
	UART_socSetInitCfg(BOARD_UART_INSTANCE, &uart_cfg);
	UART_stdioInit(BOARD_UART_INSTANCE);

	UART_printf("%s (%s - %s)\n", OTP_VERSION_STR, __DATE__, __TIME__);

	OTP_SciClientInit();

	UART_printf("Beginning key programming sequence \n");
	UART_printf("Taking OTP configuration from 0x%x\n", (uint32_t *)keywriter_cert);	
	status = Sciclient_otpProcessKeyCfg((uint32_t *)keywriter_cert, SCICLIENT_SERVICE_WAIT_FOREVER, &debug_response);
	if (status != CSL_PASS)
	{
		UART_printf("Something wrong happened!!\n");
	}
	
	UART_printf("Debug response: 0x%x\n", debug_response);
	
	UART_printf("Key programming is complete \n");

	asm volatile (" wfi");

	return 0;
}
