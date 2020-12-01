/**
 *  \file   main_gpmc_probing_example.c
 *
 *  \brief  Probing test application main file. This application will write the data
 *          to GPMC SRAM memory using CPU multiple times to enable probing
 *
 */

/*
 * Copyright (C) 2016 -2020 Texas Instruments Incorporated - http://www.ti.com/
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

#include <stdio.h>
#include <string.h>

#ifdef USE_BIOS
/* XDCtools Header files */
#include <xdc/std.h>
#include <xdc/cfg/global.h>
#include <xdc/runtime/System.h>
#include <xdc/runtime/Error.h>

#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/utils/Load.h>
#endif

/* GPMC Header files */
#include <ti/drv/gpmc/GPMC.h>
#include <ti/drv/gpmc/soc/GPMC_soc.h>
#include <ti/drv/gpmc/test/src/GPMC_board.h>
#include <ti/drv/gpmc/test/src/GPMC_log.h>

/* Board Header files */
#include <ti/board/board.h>
#include <ti/board/src/flash/include/board_flash.h>

#include <ti/drv/sciclient/sciclient.h>
#include <ti/osal/TimerP.h>
#ifdef GPMC_DMA_ENABLE
#include <ti/osal/CacheP.h>
#include <ti/drv/udma/udma.h>
#endif

/**********************************************************************
 ************************** Macros ************************************
 **********************************************************************/
#define GPMC_PROFILE        /* Enable profiling */


/**********************************************************************
 ************************** Global Variables **************************
 **********************************************************************/
uint8_t txBuf[TEST_DATA_LEN]  __attribute__((aligned(4))) __attribute__((section(".benchmark_buffer")));
GPMC_Transaction transaction;


/**********************************************************************
 ************************** Internal functions ************************
 **********************************************************************/
/*
 *  ======== GeneratePattern ========
 */
static void GeneratePattern(uint8_t *txBuf, uint8_t *rxBuf, uint32_t length)
{
    volatile uint32_t idx;
    volatile uint8_t *txPtr = txBuf;
    volatile uint8_t *rxPtr = rxBuf;

    for(idx = 0; idx < length; idx++)
    {
        *txPtr++ = (uint8_t)idx;
        *rxPtr++ = (uint8_t)0U;
    }
}

void GPMC_initConfig()
{
    GPMC_v1_HwAttrs gpmc_cfg;

    /* Get the default GPMC init configurations */
    GPMC_socGetInitCfg(BOARD_GPMC_INSTANCE, &gpmc_cfg);

    /* Modify the default GPMC configurations */

    /* Set the default GPMC init configurations */
    GPMC_socSetInitCfg(BOARD_GPMC_INSTANCE, &gpmc_cfg);
}

uint32_t gpmc_test()
{
    uint32_t    testPassed = TRUE;
    uint32_t    i;
    GPMC_Params params;
    GPMC_Handle handle;
#ifdef GPMC_PROFILE
    uint32_t    testLen = TEST_DATA_LEN;
    uint64_t    startTime;                  /* start time stamp in usec */
    uint64_t    elapsedTime;                /* elapsed time in usec */
    float       xferRate;
    uint32_t    xferRateInt;
#endif

    GPMC_initConfig();
    GPMC_Params_init(&params);
    handle = GPMC_open(BOARD_GPMC_INSTANCE, &params);
    if (handle == NULL)
    {
        GPMC_log("\n GPMC open failed. \n");
        testPassed = FALSE;
        goto Err;
    }

    /* Generate the data */
    GeneratePattern(txBuf, NULL, TEST_DATA_LEN);

    /* Write data */
    transaction.transType = GPMC_TRANSACTION_TYPE_WRITE;
    transaction.offset    = 0U;
    transaction.count     = TEST_DATA_LEN;
    transaction.txBuf     = txBuf;
    transaction.rxBuf     = NULL;
    transaction.arg       = NULL;

    for(i=0 ; i<1000 ; i++)
    {
	#ifdef GPMC_PROFILE
	    /* Get start time stamp for the write performance measurement */
	    startTime = TimerP_getTimeInUsecs();
	#endif
    	
    	if(GPMC_transfer(handle, &transaction))
    	{    		
		#ifdef GPMC_PROFILE
		    elapsedTime = TimerP_getTimeInUsecs() - startTime;
		    /* calculate the write transfer rate in MBps */
		    xferRate = (float) (((float)testLen) / elapsedTime);
		    xferRateInt = (uint32_t)xferRate;
		    GPMC_log("\n GPMC write %d bytes at transfer rate %d MBps \n", testLen, xferRateInt);
		#endif
		}
		else
		{
	        GPMC_log("\n GPMC write failed. \n");
	        break;
	    }
	}

Err:
    if (handle != NULL)
    {
        GPMC_close(handle);
    }

    return (testPassed);
}

#if defined(SOC_AM64X)
uint32_t GPMC_configClk(uint32_t freq)
{
    GPMC_v1_HwAttrs gpmc_cfg;
    int32_t         retVal;
    uint64_t        gpmc_clk;
    uint32_t        parClk;
    uint32_t        clkID = TISCI_DEV_GPMC0_FUNC_CLK;
    uint32_t        devID = TISCI_DEV_GPMC0;

    /* Get the default GPMC init configurations */
    GPMC_socGetInitCfg(BOARD_GPMC_INSTANCE, &gpmc_cfg);

    retVal = Sciclient_pmModuleClkRequest(devID,
                                          clkID,
                                          TISCI_MSG_VALUE_CLOCK_SW_STATE_REQ,
                                          TISCI_MSG_FLAG_AOP,
                                          SCICLIENT_SERVICE_WAIT_FOREVER);
    if (retVal != CSL_PASS)
    {
        GPMC_log("\n Sciclient_pmModuleClkRequest failed");
        goto clk_cfg_exit;
    }

    /* Set parent clock */
    parClk = TISCI_DEV_GPMC0_FUNC_CLK_PARENT_HSDIV4_16FFT_MAIN_0_HSDIVOUT3_CLK;
    retVal = Sciclient_pmSetModuleClkParent(devID,
                                            clkID,
                                            parClk,
                                            SCICLIENT_SERVICE_WAIT_FOREVER);

    if (retVal != CSL_PASS)
    {
        GPMC_log("\n Sciclient_pmSetModuleClkParent failed");
        goto clk_cfg_exit;
    }

    gpmc_clk = (uint64_t)freq;
    retVal = Sciclient_pmSetModuleClkFreq(devID,
                                          clkID,
                                          gpmc_clk,
                                          TISCI_MSG_FLAG_AOP,
                                          SCICLIENT_SERVICE_WAIT_FOREVER);

    if (retVal != CSL_PASS)
    {
        GPMC_log("\n Sciclient_pmSetModuleClkFreq failed");
        goto clk_cfg_exit;
    }

    gpmc_clk = 0;
    retVal = Sciclient_pmGetModuleClkFreq(devID,
                                          clkID,
                                          &gpmc_clk,
                                          SCICLIENT_SERVICE_WAIT_FOREVER);
    if (retVal != CSL_PASS)
    {
        GPMC_log("\n Sciclient_pmGetModuleClkFreq failed");
        goto clk_cfg_exit;
    }

    GPMC_log("\n GPMC CLK running at %d Hz. \n", freq);

clk_cfg_exit:
    if (retVal == CSL_PASS)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
#endif

uint32_t Board_initGPMC(void)
{
    uint32_t      retVal = TRUE;
    Board_initCfg boardCfg;
    Board_STATUS  boardStatus;

    boardCfg = BOARD_INIT_MODULE_CLOCK  |
               BOARD_INIT_PINMUX_CONFIG |
               BOARD_INIT_UART_STDIO;
    boardStatus = Board_init(boardCfg);
    if (boardStatus != BOARD_SOK)
    {
        retVal = FALSE;
    }

#if defined(SOC_AM64X)
    if (retVal == TRUE)
    {
        retVal = GPMC_configClk(GPMC_MODULE_CLK_80MHZ);
    }
#endif
    return (retVal);
}

int main(void)
{
    uint32_t testResult = FALSE;

    if (Board_initGPMC() == FALSE)
    {
        return (0);
    }

    GPMC_init();

    testResult = gpmc_test();

    if (testResult)
    {
        GPMC_log("\n GPMC Write x100 has successfully completed! \n");
    }
    else
    {
        GPMC_log("\n Test Failed! \n");
    }

    while (1)
    {
    }
}
