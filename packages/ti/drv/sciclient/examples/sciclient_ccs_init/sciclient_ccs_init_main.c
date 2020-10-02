/*
 *  Copyright (C) 2018 Texas Instruments Incorporated
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
 *  \file sciclient_ccs_init_main.c
 *
 *  \brief Implementation of System firmware boot test for CCS initialization
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <ti/csl/tistdtypes.h>
#include <ti/csl/soc.h>
#include <ti/csl/arch/csl_arch.h>
#include <ti/csl/hw_types.h>
#include <sciclient.h>
#include <ti/board/board.h>
#include <ti/drv/sciclient/src/sciclient_priv.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */
/* Set desired DDR PLL frequency here */
#define CSL_DDR_PLL_CLK_FREQ                 (400000000)

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

#if defined (BUILD_MCU1_0)
const struct tisci_boardcfg gBoardConfigLow_debug
__attribute__(( aligned(128), section(".boardcfg_data") )) =
{
    /* tisci_boardcfg_abi_rev */
    .rev = {
        .tisci_boardcfg_abi_maj = TISCI_BOARDCFG_ABI_MAJ_VALUE,
        .tisci_boardcfg_abi_min = TISCI_BOARDCFG_ABI_MIN_VALUE,
    },

    /* tisci_boardcfg_control */
    .control = {
        .subhdr = {
            .magic = TISCI_BOARDCFG_CONTROL_MAGIC_NUM,
            .size = sizeof(struct tisci_boardcfg_control),
        },
        /* Enable/disable support for System Firmware main isolation.
         * If disabled, main isolation SCI message will be rejected with NAK.
         */
        .main_isolation_enable = 0x5A,
        /* Host-ID allowed to send SCI-message for main isolation.
         * If mismatch, SCI message will be rejected with NAK.
         */
#if defined (SOC_AM64X)
        .main_isolation_hostid = TISCI_HOST_ID_MAIN_0_R5_0,
#else
        .main_isolation_hostid = TISCI_HOST_ID_R5_1,
#endif
    },

    /* tisci_boardcfg_sec_proxy */
    .secproxy = {
        .subhdr = {
            .magic = TISCI_BOARDCFG_SECPROXY_MAGIC_NUM,
            .size = sizeof(struct tisci_boardcfg_secproxy),
        },
        /* Memory allocation for messages scaling factor. In current design,
         * only value of “1” is supported. For future design, a value of “2”
         * would double all memory allocations and credits, “3” would triple,
         * and so on.
         */
        .scaling_factor = 0x1,
        /* Memory allocation for messages profile number. In current design,
         * only a value of “1” is supported. “0” is always invalid due to
         * fault tolerance.
         */
        .scaling_profile = 0x1,
        /* Do not configure main nav secure proxy. This removes all MSMC memory
         * demands from System Firmware but limits MPU channels to one set of
         * secure and one set of insecure. In current design, supports only “0”.
         */
        .disable_main_nav_secure_proxy = 0,
    },

    /* tisci_boardcfg_msmc */
    .msmc = {
        .subhdr = {
            .magic = TISCI_BOARDCFG_MSMC_MAGIC_NUM,
            .size = sizeof(struct tisci_boardcfg_msmc),
        },
        /* If the whole memory is X MB the value you write to this field is n.
         * The value of n sets the cache size as n * X/32. The value of n should
         * be given in steps of 4, which makes the size of cache to be
         * configured in steps on X/8 MB.
         */
        .msmc_cache_size = 0x00,
    },

    /* boardcfg_dbg_cfg */
    .debug_cfg = {
        .subhdr = {
            .magic = TISCI_BOARDCFG_DBG_CFG_MAGIC_NUM,
            .size = sizeof(struct tisci_boardcfg_dbg_cfg),
        },
        /* This enables the trace for DMSC logging. Should be used only for
         * debug. Profiling should not be done with this enabled.
         */
        .trace_dst_enables = (TISCI_BOARDCFG_TRACE_DST_UART0 |
                              TISCI_BOARDCFG_TRACE_DST_ITM |
                              TISCI_BOARDCFG_TRACE_DST_MEM),
        .trace_src_enables = (TISCI_BOARDCFG_TRACE_SRC_PM |
                              TISCI_BOARDCFG_TRACE_SRC_RM |
                              TISCI_BOARDCFG_TRACE_SRC_SEC |
                              TISCI_BOARDCFG_TRACE_SRC_BASE |
                              TISCI_BOARDCFG_TRACE_SRC_USER |
                              TISCI_BOARDCFG_TRACE_SRC_SUPR)
    }
};
#endif

uint32_t sciclientInitTimeStamp[30] = {0};
uint32_t sciclientInitTimeCount = 0;

void dmtimer0_read()
{
#if !defined(SOC_AM64X)
        sciclientInitTimeStamp[sciclientInitTimeCount] =  *(volatile uint32_t*)0x4413303C;
        sciclientInitTimeCount = (sciclientInitTimeCount + 1)%30;
#endif
}
void dmtimer0_enable()
{
#if !defined(SOC_AM64X)
        /* Unlock the the PM Ctrl registers */
        *(volatile uint32_t *)0x44130020 = 0x8a6b7cda;
        *(volatile uint32_t *)0x44130024 = 0x823caef9;
        /* Set the DMTimer Source to be MOSC Clock - 25 MHz for AM65x */
        *(volatile uint32_t *)0x44130200 = 0x2;
        /* Start the timer */
        *(volatile uint32_t *)0x44133038 = 0x3;
        sciclientInitTimeCount = 0;
        dmtimer0_read();
#endif
}

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

static int32_t App_getRevisionTest(void);
#if defined (SOC_AM65XX)
static int32_t setPLLClk(uint32_t modId, uint32_t clkId, uint64_t clkRate);
#endif

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

int32_t main(void)
{
    App_getRevisionTest();

    return 0;
}

/* ========================================================================== */
/*                 Internal Function Definitions                              */
/* ========================================================================== */

static int32_t App_getRevisionTest(void)
{
    int32_t status = CSL_EFAIL;
    Sciclient_ConfigPrms_t        config =
    {
        SCICLIENT_SERVICE_OPERATION_MODE_POLLED,
        NULL
    };

    #if defined (BUILD_MCU1_0 )
    struct tisci_msg_version_req req = {0};
    const Sciclient_ReqPrm_t      reqPrm =
    {
        TISCI_MSG_VERSION,
        TISCI_MSG_FLAG_AOP,
        (const uint8_t *)&req,
        sizeof(req),
        SCICLIENT_SERVICE_WAIT_FOREVER
    };
    #endif
    struct tisci_msg_version_resp response;
    Sciclient_RespPrm_t           respPrm =
    {
        0,
        (uint8_t *) &response,
        sizeof (response)
    };

    status = Sciclient_init(&config);
    dmtimer0_enable();
#if defined (SOC_J721E) || defined (SOC_AM65XX) || defined (SOC_J7200)
    if (CSL_PASS == status)
    {
        Sciclient_BoardCfgPrms_t boardCfgPrms =
        {
            .boardConfigLow = (uint32_t) &gBoardConfigLow_debug,
            .boardConfigHigh = 0,
            .boardConfigSize = sizeof(gBoardConfigLow_debug),
            #if defined (SOC_J721E) || defined (SOC_AM65XX) || defined (SOC_J7200)
            .devGrp = DEVGRP_00
            #endif
        };
        printf(" \nDMSC Board Configuration with Debug enable \n");
        dmtimer0_read();
        status = Sciclient_boardCfg(&boardCfgPrms);
        dmtimer0_read();
    }
    else
    {
        printf("\nSciclient Init Failed.\n");
    }
    if (CSL_PASS == status)
    {
        uint32_t boardCfgLow[] = SCICLIENT_BOARDCFG_PM;
        Sciclient_BoardCfgPrms_t boardCfgPrms_pm =
        {
            .boardConfigLow = (uint32_t)boardCfgLow,
            .boardConfigHigh = 0,
            .boardConfigSize = 0,
            #if defined (SOC_J721E) || defined (SOC_AM65XX) || defined (SOC_J7200)
            .devGrp = DEVGRP_00
            #endif
        };
        printf("\nSciclient Board Configuration has passed \n");
        dmtimer0_read();
        status = Sciclient_boardCfgPm(&boardCfgPrms_pm);
        dmtimer0_read();

        if (status == CSL_PASS)
        {
            uint32_t boardCfgLow[] = SCICLIENT_BOARDCFG_RM;
            Sciclient_BoardCfgPrms_t boardCfgPrms_rm =
            {
                .boardConfigLow = (uint32_t) boardCfgLow,
                .boardConfigHigh = 0,
                .boardConfigSize = SCICLIENT_BOARDCFG_RM_SIZE_IN_BYTES,
                #if defined (SOC_J721E) || defined (SOC_AM65XX) || defined (SOC_J7200)
                .devGrp = DEVGRP_00
                #endif
            };

            dmtimer0_read();
            status = Sciclient_boardCfgRm(&boardCfgPrms_rm);
            dmtimer0_read();
        }
        else
        {
            printf("\nSciclient PM Board Configuration has failed \n");
        }
        if (status == CSL_PASS)
        {
            uint32_t boardCfgLow[] = SCICLIENT_BOARDCFG_SECURITY;
            Sciclient_BoardCfgPrms_t boardCfgPrms_security =
            {
                .boardConfigLow = (uint32_t) boardCfgLow,
                .boardConfigHigh = 0,
                .boardConfigSize = SCICLIENT_BOARDCFG_SECURITY_SIZE_IN_BYTES,
                #if defined (SOC_J721E) || defined (SOC_AM65XX) || defined (SOC_J7200)
                .devGrp = DEVGRP_00
                #endif
            };
            printf("\nSciclient PM Board Configuration has Passed \n");
            dmtimer0_read();
            status = Sciclient_boardCfgSec(&boardCfgPrms_security) ;
            dmtimer0_read();
        }
        else
        {
            printf("\nSciclient RM Board Configuration has failed \n");
        }
    }
    else
    {
        printf("\nSciclient Common Board Configuration has failed \n");
    }

    if (CSL_PASS == status)
    {
        printf ("\nSciclient Dev Group 00 initilization done");
#if defined(SOC_J721E) || defined (SOC_J7200) || defined (SOC_J7200)
        /* This is specifically required if you are booting in MCU_ONLY boot mode. */
        status = Sciclient_pmSetModuleState(TISCI_DEV_WKUPMCU2MAIN_VD,
                                            TISCI_MSG_VALUE_DEVICE_SW_STATE_ON,
                                            TISCI_MSG_FLAG_AOP | TISCI_MSG_FLAG_DEVICE_RESET_ISO,
                                            0xFFFFFFFFU);
        status += Sciclient_pmSetModuleState(TISCI_DEV_MAIN2WKUPMCU_VD,
                                            TISCI_MSG_VALUE_DEVICE_SW_STATE_ON,
                                            TISCI_MSG_FLAG_AOP | TISCI_MSG_FLAG_DEVICE_RESET_ISO,
                                            0xFFFFFFFFU);
        if (status == CSL_PASS)
        {
            printf("\nEnabled the TISCI_DEV_WKUPMCU2MAIN_VD, TISCI_DEV_MAIN2WKUPMCU_VD.");
        }
#endif
    }
#if defined(SOC_J721E) || defined (SOC_AM65XX) || defined (SOC_J7200)
    if (CSL_PASS == status)
    {
        printf ("\nSciclient Dev Group 01 initilization started");
        Sciclient_BoardCfgPrms_t boardCfgPrms =
        {
            .boardConfigLow = (uint32_t) &gBoardConfigLow_debug,
            .boardConfigHigh = 0,
            .boardConfigSize = sizeof(gBoardConfigLow_debug),
            .devGrp = DEVGRP_01
        };
        printf(" \nDMSC Board Configuration with Debug enable \n");
        status = Sciclient_boardCfg(&boardCfgPrms);
    }
    if (CSL_PASS == status)
    {
        uint32_t boardCfgLow[] = SCICLIENT_BOARDCFG_PM;
        Sciclient_BoardCfgPrms_t boardCfgPrms_pm =
        {
            .boardConfigLow = (uint32_t)boardCfgLow,
            .boardConfigHigh = 0,
            .boardConfigSize = 0,
            .devGrp = DEVGRP_01
        };
        status = Sciclient_boardCfgPm(&boardCfgPrms_pm);
        if (status == CSL_PASS)
        {
            uint32_t boardCfgLow[] = SCICLIENT_BOARDCFG_RM;
            Sciclient_BoardCfgPrms_t boardCfgPrms_rm =
            {
                .boardConfigLow = (uint32_t) boardCfgLow,
                .boardConfigHigh = 0,
                .boardConfigSize = SCICLIENT_BOARDCFG_RM_SIZE_IN_BYTES,
                .devGrp = DEVGRP_01
            };

            status = Sciclient_boardCfgRm(&boardCfgPrms_rm);
        }
        else
        {
            printf("\nSciclient PM Board Configuration has failed \n");
        }
        if (status == CSL_PASS)
        {
            uint32_t boardCfgLow[] = SCICLIENT_BOARDCFG_SECURITY;
            Sciclient_BoardCfgPrms_t boardCfgPrms_security =
            {
                .boardConfigLow = (uint32_t) boardCfgLow,
                .boardConfigHigh = 0,
                .boardConfigSize = SCICLIENT_BOARDCFG_SECURITY_SIZE_IN_BYTES,
                .devGrp = DEVGRP_01
            };
            status = Sciclient_boardCfgSec(&boardCfgPrms_security) ;
        }
        else
        {
            printf("\nSciclient RM Board Configuration has failed \n");
        }
    }
    else
    {
        printf("\nSciclient Devgrp_01 Board Configuration has failed \n");
    }
#endif
#else
    if (CSL_PASS == status)
    {
        printf(" \nDMSC Board Configuration with Debug enable \n");
        dmtimer0_read();
        status = Sciclient_boardCfg(NULL);
        dmtimer0_read();
    }
    else
    {
        printf("\nSciclient Init Failed.\n");
    }
    if (CSL_PASS == status)
    {
        if (status == CSL_PASS)
        {
            dmtimer0_read();
            status = Sciclient_boardCfgPm(NULL);
            dmtimer0_read();
        }
    }
    else
    {
        printf("\nSciclient Common Board Configuration has failed \n");
    }
    if (CSL_PASS == status)
    {
        if (status == CSL_PASS)
        {
            dmtimer0_read();
            status = Sciclient_boardCfgRm(NULL);
            dmtimer0_read();
        }
    }
    else
    {
        printf("\nSciclient PM Board Configuration has failed \n");
    }
    if (status != CSL_PASS) 
    {
        printf("\nSciclient RM Board Configuration has failed \n");
    }
    if (status == CSL_PASS)
    {
        dmtimer0_read();
        status = Sciclient_boardCfgSec(NULL) ;
        dmtimer0_read();
    }
    if (status != CSL_PASS) 
    {
        printf("\nSciclient Security Board Configuration has failed \n");
    }
#endif
    if (status == CSL_PASS)
    {
        status = Sciclient_service(&reqPrm, &respPrm);
        if (CSL_PASS == status)
        {
            if (respPrm.flags == TISCI_MSG_FLAG_ACK)
            {
                status = CSL_PASS;
                printf(" DMSC Firmware Version %s\n",
                                  (char *) response.str);
                printf(" Firmware revision 0x%x\n", response.version);
                printf(" ABI revision %d.%d\n", response.abi_major,
                                  response.abi_minor);
            }
            else
            {
                printf(" DMSC Firmware Get Version failed \n");
            }
        }
        else
        {
            printf(" DMSC Firmware Get Version failed \n");
        }
    }
    else
    {
        printf("\nSciclient Security Board Configuration has failed \n");
    }
    /* Set DDR PLL to 400 Mhz. SYSFW default sets this to 333.33 Mhz */
    /* Comment this code if LPDDR is used */
    #if defined(SOC_AM65XX)
    if (status == CSL_PASS)
    {
        /* Set DDR PLL to 400 Mhz. SYSFW default sets this to 333.33 Mhz */
        /* Comment this code if LPDDR is used */
        setPLLClk(TISCI_DEV_DDRSS0, TISCI_DEV_DDRSS0_BUS_DDRSS_BYP_4X_CLK, CSL_DDR_PLL_CLK_FREQ);
        if (CSL_PASS == status)
        {
            printf(" DDR PLL set to %f MHz \n", (float)(CSL_DDR_PLL_CLK_FREQ/1000000) );
        }
        else
        {
            printf(" Failed to change DDR PLL clock \n");
        }
    }
    else
    {
        printf(" #####################################################\n");
        printf(" Something has gone wrong. Please check!!\n");
        printf(" #####################################################\n");
    }
    #endif
    if (status == CSL_PASS)
    {
        status = Sciclient_deinit();
    }
    return status;
}

#if defined(SOC_AM65XX)
/**
 * \brief  PLL clock configuration
 *
 * This function is used to set the PLL Module clock frequency
 *
 * \param  moduleId [IN]  Module for which the state should be set.
 *                        Refer Sciclient_PmDeviceIds in sciclient_fmwMsgParams.h
 * \param  clockId  [IN]  Clock Id for the module.
 *                        Refer Sciclient_PmModuleClockIds in sciclient_fmwMsgParams.h
 * \param  clkRate  [IN]  Value of the clock frequency to be set
 *
 * \return int32_t
 *                CSL_PASS - on Success
 *                CSL_EFAIL - on Failure
 *
 */
static int32_t setPLLClk(uint32_t modId,
                             uint32_t clkId,
                             uint64_t clkRate)
{
    uint8_t parentId = 0;
    uint32_t i = 0U;
    int32_t status   = 0;
    uint64_t respClkRate = 0;
    uint32_t clockBaseId = 0U;
    uint32_t numParents = 0U;
    uint32_t clockStatus;

    status = Sciclient_pmQueryModuleClkFreq(modId,
                                        clkId,
                                        clkRate,
                                        &respClkRate,
                                        SCICLIENT_SERVICE_WAIT_FOREVER);
    if(status == CSL_PASS)
    {
        /* Check if the clock is enabled or not */
        status = Sciclient_pmModuleGetClkStatus(modId,
                                                clkId,
                                                &clockStatus,
                                                SCICLIENT_SERVICE_WAIT_FOREVER);
    }

    if(status == CSL_PASS)
    {
        /* Check if the clock is enabled or not */
        status = Sciclient_pmGetModuleClkNumParent(modId,
                                                clkId,
                                                &numParents,
                                                SCICLIENT_SERVICE_WAIT_FOREVER);
    }

    if ((status == CSL_PASS) && (respClkRate == clkRate))
    {
        status = Sciclient_pmSetModuleClkFreq(
                                modId,
                                clkId,
                                clkRate,
                                TISCI_MSG_FLAG_CLOCK_ALLOW_FREQ_CHANGE,
                                SCICLIENT_SERVICE_WAIT_FOREVER);
        if (status == CSL_PASS)
        {
            if (clockStatus == TISCI_MSG_VALUE_CLOCK_SW_STATE_UNREQ)
            {
                /* Enable the clock */
                status = Sciclient_pmModuleClkRequest(
                                                    modId,
                                                    clkId,
                                                    TISCI_MSG_VALUE_CLOCK_SW_STATE_REQ,
                                                    0U,
                                                    SCICLIENT_SERVICE_WAIT_FOREVER);
            }
        }
    }
    else if (status == CSL_PASS)
    {
        /* Try to loop and change parents of the clock */
        for(i = 0U; i < numParents; i++)
        {
            /* Disabling the clock */
            status = Sciclient_pmModuleClkRequest(modId,
                                                  clkId,
                                                  TISCI_MSG_VALUE_CLOCK_SW_STATE_UNREQ,
                                                  0U,
                                                  SCICLIENT_SERVICE_WAIT_FOREVER);
            if (status == CSL_PASS)
            {
                clockStatus = TISCI_MSG_VALUE_CLOCK_SW_STATE_UNREQ;
                /* Setting the new parent */
                status = Sciclient_pmSetModuleClkParent(
                                        modId,
                                        clkId,
                                        clkId+i+1,
                                        SCICLIENT_SERVICE_WAIT_FOREVER);
            }
            /* Check if the clock can be set to desirable freq. */
            if (status == CSL_PASS)
            {
                status = Sciclient_pmQueryModuleClkFreq(modId,
                                                        clkId,
                                                        clkRate,
                                                        &respClkRate,
                                                        SCICLIENT_SERVICE_WAIT_FOREVER);
            }
            if (status == CSL_PASS)
            {
                if(respClkRate == clkRate)
                {
                    break;
                }
                else
                {
                    status = CSL_EFAIL;
                }
            }
            parentId++;
            clockBaseId++;
        }

        if (status == CSL_PASS)
        {
            /* Set the clock at the desirable frequency*/
            status = Sciclient_pmSetModuleClkFreq(
                                    modId,
                                    clkId,
                                    clkRate,
                                    TISCI_MSG_FLAG_CLOCK_ALLOW_FREQ_CHANGE,
                                    SCICLIENT_SERVICE_WAIT_FOREVER);
        }
        if((status == CSL_PASS) && (clockStatus == TISCI_MSG_VALUE_CLOCK_SW_STATE_UNREQ))
        {
            /*Enable the clock again */
            status = Sciclient_pmModuleClkRequest(
                                                modId,
                                                clkId,
                                                TISCI_MSG_VALUE_CLOCK_SW_STATE_REQ,
                                                0U,
                                                SCICLIENT_SERVICE_WAIT_FOREVER);
        }
    }

    return status;
}
#endif

