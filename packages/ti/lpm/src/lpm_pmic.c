/*
*
* Copyright (c) 2020 Texas Instruments Incorporated
*
* All rights reserved not granted herein.
*
* Limited License.
*
* Texas Instruments Incorporated grants a world-wide, royalty-free, non-exclusive
* license under copyrights and patents it now or hereafter owns or controls to make,
* have made, use, import, offer to sell and sell ("Utilize") this software subject to the
* terms herein.  With respect to the foregoing patent license, such license is granted
* solely to the extent that any such patent is necessary to Utilize the software alone.
* The patent license shall not apply to any combinations which include this software,
* other than combinations with devices manufactured by or for TI ("TI Devices").
* No hardware patent is licensed hereunder.
*
* Redistributions must preserve existing copyright notices and reproduce this license
* (including the above copyright notice and the disclaimer and (if applicable) source
* code license limitations below) in the documentation and/or other materials provided
* with the distribution
*
* Redistribution and use in binary form, without modification, are permitted provided
* that the following conditions are met:
*
* *       No reverse engineering, decompilation, or disassembly of this software is
* permitted with respect to any software provided in binary form.
*
* *       any redistribution and use are licensed by TI for use only with TI Devices.
*
* *       Nothing shall obligate TI to provide you with source code for the software
* licensed and provided to you in object code.
*
* If software source code is provided to you, modification and redistribution of the
* source code are permitted provided that the following conditions are met:
*
* *       any redistribution and use of the source code, including any resulting derivative
* works, are licensed by TI for use only with TI Devices.
*
* *       any redistribution and use of any object code compiled from the source code
* and any resulting derivative works, are licensed by TI for use only with TI Devices.
*
* Neither the name of Texas Instruments Incorporated nor the names of its suppliers
*
* may be used to endorse or promote products derived from this software without
* specific prior written permission.
*
* DISCLAIMER.
*
* THIS SOFTWARE IS PROVIDED BY TI AND TI'S LICENSORS "AS IS" AND ANY EXPRESS
* OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL TI AND TI'S LICENSORS BE LIABLE FOR ANY DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
* OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
* OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
* OF THE POSSIBILITY OF SUCH DAMAGE.
*
*/
/**
 *  \file     mcu_only_app.c
 *
 *  \brief    This file implements switching the SoCs state from ACTIVE to
 *            MCU ONLY mode and then from MCU ONLY to ACTIVE mode.
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

//#include "Std_Types.h"
#include <stdio.h>

#include <ti/csl/cslr_gtc.h>

#include <ti/drv/spi/soc/SPI_soc.h>
#include <ti/board/board.h>
#include <ti/board/board_cfg.h>
#include <ti/board/src/flash/include/board_flash.h>
#if defined(SOC_J721E)
#include <ti/board/src/j721e_evm/include/board_control.h>
#endif
#if defined(SOC_J7200)
#include <ti/board/src/j7200_evm/include/board_control.h>
#endif

//#include "boot_cfg.h"
//#include "app_utils.h"
//#include "mcu_timer_multicore.h"
#include <ti/drv/sciclient/sciserver.h>

#include "lpm_ipc.h"
#include "print_utils.h"

#include <ti/drv/pmic/test/common/pmic_ut_common.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#define DDR_TEST_ADDRESS               (0xA0000000)
#define MSMC_TEST_ADDRESS              (0x70000000)
#define MCAN_REV_REG                   (0x02700000)

#define MSG_APP_NAME                   "MCU only app: "
/* ========================================================================== */
/*                         Structures and Enums                               */
/* ========================================================================== */

struct bootApp_boardcfg_rm_resasg
{
    struct tisci_boardcfg_substructure_header subhdr;
    uint16_t                    resasg_entries_size;
    uint16_t                    reserved;
} __attribute__((__packed__));

struct bootApp_boardcfg_rm
{
    struct tisci_boardcfg_abi_rev     rev;
    struct tisci_boardcfg_rm_host_cfg host_cfg;
    struct bootApp_boardcfg_rm_resasg resasg;
} __attribute__((__packed__));

struct bootApp_local_rm_boardcfg
{
    struct bootApp_boardcfg_rm            rm_boardcfg;
    struct tisci_boardcfg_rm_resasg_entry resasg_entries[TISCI_RESASG_ENTRIES_MAX];
};

#pragma DATA_SECTION(bootAppBoardCfg, ".sysfw_data_cfg_board")
struct tisci_boardcfg bootAppBoardCfg;
#pragma DATA_SECTION(bootAppBoardCfg_rm, ".sysfw_data_cfg_board_rm")
struct bootApp_local_rm_boardcfg bootAppBoardCfg_rm;
#pragma DATA_SECTION(bootAppBoardCfg_sec, ".sysfw_data_cfg_board_sec")
struct tisci_boardcfg_sec bootAppBoardCfg_sec;

extern uint32_t reloadedBootApp;

/* ========================================================================== */
/*              Internal Function Declarations                                */
/* ========================================================================== */

uint32_t ActiveToMcuSwitch(void);
uint32_t McuOnly_App(void);
uint32_t McuToActiveSwitch(void);

void VtmMaxOutrgAlertDisableForTmpSens1to4();
void SwResetMainDomain(void);
void PMICStateChangeActiveToMCUOnly(void);
void I2CInitPMIC(void);
void PMICStateChangeMCUOnlyToActive(void);
void BringBackMAINDomain(void);
int32_t EnableMCU2MAINBridges(void);
void AccessMainPeripherals(void);
void BoardConfigurationForMainDomain(void);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

I2C_Handle pmicI2cHandle  = NULL;

/* ========================================================================== */
/*                            External Variables                              */
/* ========================================================================== */

/* Sync semaphore for MCU ONLY task */
//extern SemaphoreP_Handle mcuOnlySyncSemHandle;

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

/* Local API for enabling peripheral clocks.
   Copied from board library.
 */
int32_t local_moduleClockEnable(uint32_t moduleId)
{
    int32_t retVal = 0;
    int32_t status = -1;
    uint32_t moduleState = 0U;
    uint32_t resetState = 0U;
    uint32_t contextLossState = 0U;

    /* Get the module state.
       No need to change the module state if it
       is already ON
     */
    status = Sciclient_pmGetModuleState(moduleId,
                                        &moduleState,
                                        &resetState,
                                        &contextLossState,
                                        SCICLIENT_SERVICE_WAIT_FOREVER);
    if(moduleState == TISCI_MSG_VALUE_DEVICE_HW_STATE_OFF)
    {
        status = Sciclient_pmSetModuleState(moduleId,
                                            TISCI_MSG_VALUE_DEVICE_SW_STATE_ON,
                                            (TISCI_MSG_FLAG_AOP |
                                             TISCI_MSG_FLAG_DEVICE_RESET_ISO),
                                             SCICLIENT_SERVICE_WAIT_FOREVER);
        if (status == 0)
        {
            status = Sciclient_pmSetModuleRst (moduleId,
                                               0x0U,
                                               SCICLIENT_SERVICE_WAIT_FOREVER);
            if (status != 0)
            {
                retVal = -1;
            }
        }
        else
        {
            retVal = -1;
        }
    }

    return retVal;
}

int32_t local_PLLSetModuleClkFreq(uint32_t modId,
                                         uint32_t clkId,
                                         uint64_t clkRate)
{
    uint32_t i = 0U;
    int32_t status   = CSL_EFAIL;
    uint64_t respClkRate = 0;
    uint32_t numParents = 0U;
    uint32_t moduleClockParentChanged = 0U;
    uint32_t clockStatus = 0U;
    uint32_t origParent = 0U;
    uint32_t foundParent = 0U;

    /* Check if the clock is enabled or not */
    status = Sciclient_pmModuleGetClkStatus(modId,
                                            clkId,
                                            &clockStatus,
                                            SCICLIENT_SERVICE_WAIT_FOREVER);
    if (status == CSL_PASS)
    {
        /* Get the number of parents for the clock */
        status = Sciclient_pmGetModuleClkNumParent(modId,
                                                clkId,
                                                &numParents,
                                                SCICLIENT_SERVICE_WAIT_FOREVER);
        if ((status == CSL_PASS) && (numParents > 1U))
        {
            status = Sciclient_pmGetModuleClkParent(modId, clkId, &origParent,
                                       SCICLIENT_SERVICE_WAIT_FOREVER);
        }
    }
    if (status == CSL_PASS)
    {
        /* Disabling the clock */
        status = Sciclient_pmModuleClkRequest(
                                            modId,
                                            clkId,
                                            TISCI_MSG_VALUE_CLOCK_SW_STATE_UNREQ,
                                            0U,
                                            SCICLIENT_SERVICE_WAIT_FOREVER);
    }
    if (status == CSL_PASS)
    {
        foundParent = 0U;
        /* Try to loop and change parents of the clock */
        for(i=0U;i<numParents;i++)
        {
            if (numParents > 1U)
            {
                /* Setting the new parent */
                status = Sciclient_pmSetModuleClkParent(
                                            modId,
                                            clkId,
                                            clkId+i+1,
                                            SCICLIENT_SERVICE_WAIT_FOREVER);
                /* Check if the clock can be set to desirable freq. */
                if (status == CSL_PASS)
                {
                    moduleClockParentChanged = 1U;
                }
            }
            if (status == CSL_PASS)
            {
                status = Sciclient_pmQueryModuleClkFreq(modId,
                                                        clkId,
                                                        clkRate,
                                                        &respClkRate,
                                                        SCICLIENT_SERVICE_WAIT_FOREVER);
            }
            if ((status == CSL_PASS) && (respClkRate == clkRate))
            {
                foundParent = 1U;
                break;
            }
        }
    }
    if (foundParent == 1U)
    {
        /* Set the clock at the desirable frequency*/
        status = Sciclient_pmSetModuleClkFreq(
                                modId,
                                clkId,
                                clkRate,
                                TISCI_MSG_FLAG_CLOCK_ALLOW_FREQ_CHANGE,
                                SCICLIENT_SERVICE_WAIT_FOREVER);
    }
    else
    {
        status = CSL_EFAIL;
    }
    if ((status == CSL_PASS) &&
        (clockStatus == TISCI_MSG_VALUE_CLOCK_SW_STATE_UNREQ))
    {
        /* Restore the clock again to original state */
        status = Sciclient_pmModuleClkRequest(
                                            modId,
                                            clkId,
                                            clockStatus,
                                            0U,
                                            SCICLIENT_SERVICE_WAIT_FOREVER);
    }
    if ((status != CSL_PASS) && (moduleClockParentChanged == 1U))
    {
        /* Setting the original parent if failure */
        status = Sciclient_pmSetModuleClkParent(
                                    modId,
                                    clkId,
                                    origParent,
                                    SCICLIENT_SERVICE_WAIT_FOREVER);
    }
    return status;
}

volatile uint32_t ccs_halt = 0x0;
void BoardConfigurationForMainDomain(void)
{
    int32_t retVal;
    Sciclient_DefaultBoardCfgInfo_t boardCfgInfo;

    AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME
                    "Configuring Sciclient_board for MAIN domain\n");

    retVal = Sciclient_getDefaultBoardCfgInfo(&boardCfgInfo);
    if (retVal != CSL_PASS)
    {
         AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME
                         "Sciclient_getDefaultBoardCfgInfo() failed.\n");
    }

    Sciclient_BoardCfgPrms_t bootAppBoardCfgPrms = {
                                                    .boardConfigLow = (uint32_t)&bootAppBoardCfg,
                                                    .boardConfigHigh = 0,
                                                    .boardConfigSize = sizeof(bootAppBoardCfg),
                                                    .devGrp = DEVGRP_01
                                                   };
    Sciclient_BoardCfgPrms_t bootAppBoardCfgPmPrms = {
                                                      .boardConfigLow = (uint32_t)boardCfgInfo.boardCfgLowPm,
                                                      .boardConfigHigh = 0,
                                                      .boardConfigSize = boardCfgInfo.boardCfgLowPmSize,
                                                      .devGrp = DEVGRP_01
                                                     };
    Sciclient_BoardCfgPrms_t bootAppBoardCfgRmPrms = {
                                                      .boardConfigLow = (uint32_t)&bootAppBoardCfg_rm,
                                                      .boardConfigHigh = 0,
                                                      .boardConfigSize = sizeof(bootAppBoardCfg_rm),
                                                      .devGrp = DEVGRP_01
                                                     };
    Sciclient_BoardCfgPrms_t bootAppBoardCfgSecPrms = {
                                                       .boardConfigLow = (uint32_t)&bootAppBoardCfg_sec,
                                                       .boardConfigHigh = 0,
                                                       .boardConfigSize = sizeof(bootAppBoardCfg_sec),
                                                       .devGrp = DEVGRP_01
                                                      };

    if(ccs_halt == 0xDEADBEEF)
    {
        AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME
                        "Connect CCS and break out of ccs_halt loop!!\n");
	    while(ccs_halt);
    }

    retVal = Sciclient_boardCfg(&bootAppBoardCfgPrms);
    if (retVal != CSL_PASS)
    {
         AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME
                         "Sciclient_boardCfg() failed.\n");
    }
    retVal = Sciclient_boardCfgPm(&bootAppBoardCfgPmPrms);
    if (retVal != CSL_PASS)
    {
         AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME 
                         "Sciclient_boardCfgPm() failed.\n");
    }
    retVal = Sciclient_boardCfgSec(&bootAppBoardCfgSecPrms);
    if (retVal != CSL_PASS)
    {
         AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME
                         "Sciclient_boardCfgSec() failed.\n");
    }
    retVal = Sciclient_boardCfgRm(&bootAppBoardCfgRmPrms);
    if (retVal != CSL_PASS)
    {
         AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME
                         "Sciclient_boardCfgRm() failed.\n");
    }

    AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME "Doing board init...\n");
    Board_uartDeInit();
    Board_initCfg cfg = BOARD_INIT_PINMUX_CONFIG | BOARD_INIT_PLL_MAIN | BOARD_INIT_MODULE_CLOCK_MAIN | BOARD_INIT_DDR | BOARD_INIT_UART_STDIO;
    Board_init(cfg);
}

void SetupI2CTransfer(I2C_Handle handle,  uint32_t slaveAddr,
                      uint8_t *writeData, uint32_t numWriteBytes,
                      uint8_t *readData,  uint32_t numReadBytes)
{
    bool status;
    I2C_Transaction i2cTransaction;

    I2C_transactionInit(&i2cTransaction);
    i2cTransaction.slaveAddress = slaveAddr;
    i2cTransaction.writeBuf = (uint8_t *)&writeData[0];
    i2cTransaction.writeCount = numWriteBytes;
    i2cTransaction.readBuf = (uint8_t *)&readData[0];
    i2cTransaction.readCount = numReadBytes;
    status = I2C_transfer(handle, &i2cTransaction);
    if(FALSE == status)
    {
        AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME "\n Data Transfer failed. \n");
    }
}

int32_t EnableMCU2MAINBridges(void)
{
    int32_t status;
    /* Enable both bridges, needed to establish communication between MCU and
       MAIN domain */
    /* These are in the MCU devgrp and hence we can enable then w/o passing the
       board configuration for the MAIN devgrp */
    status = Sciclient_pmSetModuleState(TISCI_DEV_WKUPMCU2MAIN_VD,
                                        TISCI_MSG_VALUE_DEVICE_SW_STATE_ON,
                                        TISCI_MSG_FLAG_AOP | TISCI_MSG_FLAG_DEVICE_RESET_ISO,
                                        0xFFFFFFFFU);
    status += Sciclient_pmSetModuleState(TISCI_DEV_MAIN2WKUPMCU_VD,
                                            TISCI_MSG_VALUE_DEVICE_SW_STATE_ON,
                                            TISCI_MSG_FLAG_AOP | TISCI_MSG_FLAG_DEVICE_RESET_ISO,
                                            0xFFFFFFFFU);

    return status;
}

void AccessMainPeripherals(void)
{
    uint32_t readValDDR;
    uint32_t writeValDDR = 0xDEADBEEF;
    AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME"Writing to DDR...\n");
    HW_WR_REG32(DDR_TEST_ADDRESS, writeValDDR);
    AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME"Reading from DDR...\n");
    readValDDR = HW_RD_REG32(DDR_TEST_ADDRESS);
    if(writeValDDR == readValDDR)
    {
        AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME
                        "Read value matches the value written for DDR!\n");
    }

    uint32_t readValMSMC;
    uint32_t writeValMSMC = 0xC0DEC0DE;
    AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME"Writing to MSMC...\n");
    HW_WR_REG32(MSMC_TEST_ADDRESS, writeValMSMC);
    AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME"Reading from MSMC...\n");
    readValMSMC = HW_RD_REG32(MSMC_TEST_ADDRESS);
    if(writeValMSMC == readValMSMC)
    {
        AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME
                        "Read value matches the value written for MSMC!\n");
    }

    uint32_t readValMCAN;
    readValMCAN = HW_RD_REG32(MCAN_REV_REG);
    AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME
                    "Reading MAIN MCAN0 Revision register...\n");
    AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME
                    "MAIN MCAN0 MCANSS_PID = 0x%x\n", readValMCAN);

    return;
}

void BringBackMAINDomain(void)
{
    int32_t status;

    AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME
                    "Configure WKUPMCU2MAIN and MAIN2WKUPMCU Bridges...\n");
    /* Enable WKUPMCU2MAIN and MAIN2WKUPMCU bridges,
       this needs to be done before sending the RM, PM, Sec and common board cfg
       for DEVGRP01 */
    status = EnableMCU2MAINBridges();

    /* Pass board config for MAIN domain i.e. DEVGRP01 */
    if(status == CSL_PASS)
    {
        AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME
                        "WKUPMCU2MAIN and MAIN2WKUPMCU Bridges configured successfully!\n");
        BoardConfigurationForMainDomain();
    }

    AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME
                    "Trying to access MAIN domain peripherals...\n");
    AccessMainPeripherals();

    return;
}

/* Bring back main_domain */
uint32_t McuToActiveSwitch(void)
{
    uint32_t MAIN_VDOM_CTRL = 0x43018070;

    AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME
                    "PMIC STATE CHANGE: MCU ONLY -> ACTIVE...\n");
    PMICStateChangeMCUOnlyToActive();
    AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME
                    "PMIC STATE CHANGE: MCU ONLY -> ACTIVE...Done\n");

    /* Disabling MAIN domain deep sleep isolation */
    AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME
                    "Disabling MAIN domain deep sleep isolation...\n");
    CSL_REG32_WR(MAIN_VDOM_CTRL, 0x0);

    /* Enable the MAIN domain, till now we have only changed the PMIC state
       This will enable the modules in the MAIN domain */
    BringBackMAINDomain();

    return 0;
}

/* Refer TISCI_MSG_SYS_RESET in TISCI user guide for more details
   http://software-dl.ti.com/tisci/esd/latest/2_tisci_msgs/pm/sysreset.html */
volatile uint32_t loopSwResetMainDomain = 0;
void SwResetMainDomain(void)
{
    struct tisci_msg_sys_reset_req request;
    struct tisci_msg_sys_reset_resp response = {0};

    Sciclient_ReqPrm_t reqParam;
    Sciclient_RespPrm_t respParam;

    memset(&request, 0, sizeof(request));
    request.domain = 0x2; /* 0x2 corresponds to the MAIN domain */

    reqParam.messageType    = (uint16_t) TISCI_MSG_SYS_RESET;
    reqParam.flags          = (uint32_t) TISCI_MSG_FLAG_AOP;
    reqParam.pReqPayload    = (const uint8_t *) &request;
    reqParam.reqPayloadSize = (uint32_t) sizeof (request);
    reqParam.timeout        = (uint32_t) SCICLIENT_SERVICE_WAIT_FOREVER;
    respParam.flags           = (uint32_t) 0;   /* Populated by the API */
    respParam.pRespPayload    = (uint8_t *) &response;
    respParam.respPayloadSize = (uint32_t) sizeof (response);

    /* For debug purpose */
    if(loopSwResetMainDomain == 0xDEADBEEF)
    {
        AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME
                        "Connect CCS and change loopSwResetMainDomain to 0!\n");
        AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME
                        "After that the MAIN domain will be reset!\n");
    }

    while(loopSwResetMainDomain == 0xDEADBEEF);

    int32_t retVal;
    retVal = Sciclient_service(&reqParam, &respParam);

    if ((respParam.flags & TISCI_MSG_FLAG_ACK) == 0) {
            AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME
                            "retVal = %d\nGOT NACK! resp flag = 0x%08x\n",
                            retVal, respParam.flags);
    }
}

Pmic_CoreHandle_t *pPmicCoreHandle = NULL;

/*!
 * \brief   GPIO Unity Test App wrapper Function for LEO PMIC-A
 */
int32_t initPMIC(void)
{
    int32_t status                = PMIC_ST_SUCCESS;
    Pmic_CoreCfg_t pmicConfigData = {0U};

    /* Fill parameters to pmicConfigData */
    pmicConfigData.pmicDeviceType      = PMIC_DEV_LEO_TPS6594X;
    pmicConfigData.validParams        |= PMIC_CFG_DEVICE_TYPE_VALID_SHIFT;

    pmicConfigData.commMode            = PMIC_INTF_DUAL_I2C;
    pmicConfigData.validParams        |= PMIC_CFG_COMM_MODE_VALID_SHIFT;

    pmicConfigData.i2c1Speed            = PMIC_I2C_STANDARD_MODE;
    pmicConfigData.validParams         |= PMIC_CFG_I2C1_SPEED_VALID_SHIFT;

    pmicConfigData.i2c2Speed            = PMIC_I2C_STANDARD_MODE;
    pmicConfigData.validParams         |= PMIC_CFG_I2C2_SPEED_VALID_SHIFT;

#if defined(SOC_J721E)
    pmicConfigData.slaveAddr           = J721E_LEO_PMICA_SLAVE_ADDR;
    pmicConfigData.validParams        |= PMIC_CFG_SLAVEADDR_VALID_SHIFT;

    pmicConfigData.qaSlaveAddr         = J721E_LEO_PMICA_WDG_SLAVE_ADDR;
    pmicConfigData.validParams        |= PMIC_CFG_QASLAVEADDR_VALID_SHIFT;

    pmicConfigData.nvmSlaveAddr        = J721E_LEO_PMICA_PAGE1_SLAVE_ADDR;
    pmicConfigData.validParams        |= PMIC_CFG_NVMSLAVEADDR_VALID_SHIFT;
#endif

#if defined(SOC_J7200)
    pmicConfigData.slaveAddr           = J7VCL_LEO_PMICA_SLAVE_ADDR;
    pmicConfigData.validParams        |= PMIC_CFG_SLAVEADDR_VALID_SHIFT;

    pmicConfigData.qaSlaveAddr         = J7VCL_LEO_PMICA_WDG_SLAVE_ADDR;
    pmicConfigData.validParams        |= PMIC_CFG_QASLAVEADDR_VALID_SHIFT;

    pmicConfigData.nvmSlaveAddr        = J7VCL_LEO_PMICA_PAGE1_SLAVE_ADDR;
    pmicConfigData.validParams        |= PMIC_CFG_NVMSLAVEADDR_VALID_SHIFT;
#endif

    pmicConfigData.pFnPmicCommIoRead    = test_pmic_regRead;
    pmicConfigData.validParams         |= PMIC_CFG_COMM_IO_RD_VALID_SHIFT;

    pmicConfigData.pFnPmicCommIoWrite   = test_pmic_regWrite;
    pmicConfigData.validParams         |= PMIC_CFG_COMM_IO_WR_VALID_SHIFT;

    pmicConfigData.pFnPmicCritSecStart  = test_pmic_criticalSectionStartFn;
    pmicConfigData.validParams         |= PMIC_CFG_CRITSEC_START_VALID_SHIFT;

    pmicConfigData.pFnPmicCritSecStop   = test_pmic_criticalSectionStopFn;
    pmicConfigData.validParams         |= PMIC_CFG_CRITSEC_STOP_VALID_SHIFT;

    status = test_pmic_appInit(&pPmicCoreHandle, &pmicConfigData);

    return status;
}

void PMICStateChangeActiveToMCUOnly(void)
{
    int32_t pmicStatus = PMIC_ST_SUCCESS;
    uint8_t pmicState  = PMIC_FSM_MCU_ONLY_STATE;

    /* Init pmic interface */
    pmicStatus = initPMIC();
    if(PMIC_ST_SUCCESS != pmicStatus)
    {
        AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME"initPMIC failed!\n");
    }

    pmicStatus = Pmic_fsmSetNsleepSignalMask(pPmicCoreHandle,
                                         PMIC_NSLEEP1_SIGNAL,
                                         PMIC_NSLEEPX_UNMASK);
    if(PMIC_ST_SUCCESS != pmicStatus)
    {
        AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME"Pmic_fsmSetNsleepSignalMask failed!\n");
    }

    pmicStatus = Pmic_fsmSetNsleepSignalMask(pPmicCoreHandle,
                                         PMIC_NSLEEP2_SIGNAL,
                                         PMIC_NSLEEPX_UNMASK);
    if(PMIC_ST_SUCCESS != pmicStatus)
    {
        AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME"Pmic_fsmSetNsleepSignalMask failed!\n");
    }

    pmicStatus = Pmic_fsmSetMissionState(pPmicCoreHandle, pmicState);
    if(PMIC_ST_SUCCESS != pmicStatus)
    {
        AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME"Pmic_fsmSetMissionState failed!\n");
    }

    pmicStatus = Pmic_irqClrErrStatus(pPmicCoreHandle, PMIC_IRQ_ALL);
    if(PMIC_ST_SUCCESS != pmicStatus)
    {
        AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME"Pmic_irqClrErrStatus failed!\n");
    }

    return;
}

void PMICStateChangeMCUOnlyToActive(void)
{
    int32_t pmicStatus = PMIC_ST_SUCCESS;
    uint8_t pmicState  = PMIC_FSM_ACTIVE_STATE;

    pmicStatus = Pmic_fsmSetNsleepSignalMask(pPmicCoreHandle,
                                         PMIC_NSLEEP1_SIGNAL,
                                         PMIC_NSLEEPX_MASK);
    if(PMIC_ST_SUCCESS != pmicStatus)
    {
        AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME"Pmic_fsmSetNsleepSignalMask failed!\n");
    }

    pmicStatus = Pmic_fsmSetNsleepSignalMask(pPmicCoreHandle,
                                         PMIC_NSLEEP2_SIGNAL,
                                         PMIC_NSLEEPX_MASK);
    if(PMIC_ST_SUCCESS != pmicStatus)
    {
        AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME"Pmic_fsmSetNsleepSignalMask failed!\n");
    }

    pmicStatus = Pmic_fsmSetMissionState(pPmicCoreHandle, pmicState);
    if(PMIC_ST_SUCCESS != pmicStatus)
    {
        AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME"Pmic_fsmSetMissionState failed!\n");
    }
    pmicStatus = Pmic_fsmSetNsleepSignalMask(pPmicCoreHandle,
                                         PMIC_NSLEEP1_SIGNAL,
                                         PMIC_NSLEEPX_UNMASK);
    if(PMIC_ST_SUCCESS != pmicStatus)
    {
        AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME"Pmic_fsmSetNsleepSignalMask failed!\n");
    }

    pmicStatus = Pmic_fsmSetNsleepSignalMask(pPmicCoreHandle,
                                         PMIC_NSLEEP2_SIGNAL,
                                         PMIC_NSLEEPX_UNMASK);
    if(PMIC_ST_SUCCESS != pmicStatus)
    {
        AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME"Pmic_fsmSetNsleepSignalMask failed!\n");
    }

    pmicStatus = Pmic_irqClrErrStatus(pPmicCoreHandle, PMIC_IRQ_ALL);
    if(PMIC_ST_SUCCESS != pmicStatus)
    {
        AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME"Pmic_irqClrErrStatus failed!\n");
    }

    return;
}


uint32_t ActiveToMcuSwitch()
{
    uint32_t MAIN_VDOM_CTRL = 0x43018070;
    /* Issue a SW reset to the MAIN domain */
    AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME
                    "Issueing a SW reset to the MAIN domain...\n");
    SwResetMainDomain();

    /* Enabling MAIN domain deep sleep isolation */
    AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME
                    "Enabling MAIN domain deep sleep isolation...\n");
    CSL_REG32_WR(MAIN_VDOM_CTRL, 0x1);

    AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME
                    "PMIC STATE CHANGE: ACTIVE -> MCU ONLY...\n");

    /* Change PMIC state from ACTIVE to MCU ONLY */

    /* NOTE: JTAG power also compromises after the transition, so you will not
       be able to connect JTAG in MCU ONLY mode */
    PMICStateChangeActiveToMCUOnly();
    AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME
                    "PMIC STATE CHANGE: ACTIVE -> MCU ONLY...Done\n");

    return 0;
}

/* MCU Only task */
uint32_t McuOnly_App()
{
    uint32_t status = 0;
    AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME "Inside MCU ONLY task!\n");

    AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME
                    "STATE INFO :: CURRENTLY IN ACTIVE MODE!\n");
    AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME "LED LD5 should be ON\n");
    AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME
                    "Sleeping for 10s, please measure TP133 and TP134!\n");
    AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME
                    "Expected values in ACTIVE mode:\nTP133: HIGH\nTP134: HIGH\n");
    TaskP_sleep(10000);

    /* Change state from ACTIVE to MCU ONLY */
    if (0 == status)
    {
        AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME
                        "############################ACTIVE -> MCU ONLY MODE############################\n");
        /* The status is dummy currently */
        status = ActiveToMcuSwitch();
        AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME
                        "#########################ACTIVE -> MCU ONLY MODE DONE##########################\n");
    }
    else
    {
        AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME">> ERROR :: Status not correct!!!\n");
    }

    AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME 
                    "STATE INFO :: NOW IN MCU ONLY MODE!\n");
    AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME "LED LD5 should be OFF\n");
    AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME
                    "Sleeping for 10s, please measure TP133 and TP134!\n");
    AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME
                    "Expected values in MCU ONLY mode:\nTP133: HIGH\nTP134: LOW\n");
    TaskP_sleep(10000);

    /* Change state from MCU ONLY to ACTIVE */
    if (0 == status)
    {
        AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME
                        "############################MCU ONLY -> ACTIVE MODE############################\n");
        /* The status is dummy currently */
        status = McuToActiveSwitch();
        AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME
                        "#########################MCU ONLY -> ACTIVE MODE DONE##########################\n");
    }
    else
    {
        AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME">> ERROR :: Status not correct!!!\n");
    }

    AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME
                    "STATE INFO :: CURRENTLY IN ACTIVE MODE!\n");
    AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME "LED LD5 should be ON\n");
    AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME
                    "Expected values in ACTIVE mode:\nTP133: HIGH\nTP134: HIGH\n");

    reloadedBootApp = 1;

    return status;
}
