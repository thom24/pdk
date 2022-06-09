/******************************************************************************
 * Copyright (c) 2022 Texas Instruments Incorporated - http://www.ti.com
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
 *****************************************************************************/

 /** \file board_clock.c
  *
  *  \brief This file contains initialization of wakeup and main PSC
  *  configuration structures and function definitions to get the number
  *  of wakeup and main PSC config exists.
  */

#include "board_clock.h"
#include "board_utils.h"
#include <ti/drv/sciclient/sciclient.h>

extern Board_initParams_t gBoardInitParams;

uint32_t gBoardClkModuleMcuIDInitGroupl[] = {
    TISCI_DEV_MCU_TIMER0,
    TISCI_DEV_MCU_TIMER2,
    TISCI_DEV_MCU_FSS0,
    TISCI_DEV_MCU_FSS0_OSPI_0,
    TISCI_DEV_MCU_FSS0_OSPI_1,
    TISCI_DEV_MCU_FSS0_FSAS_0,
    TISCI_DEV_MCU_FSS0_HYPERBUS1P0_0,
    TISCI_DEV_MCU_UART0,
    TISCI_DEV_WKUP_I2C0,
    TISCI_DEV_WKUP_UART0,
};

uint32_t gBoardClkModuleMcuIDDeinitGroupl[] = {
    TISCI_DEV_MCU_FSS0_OSPI_0,
    TISCI_DEV_MCU_FSS0_OSPI_1,
    TISCI_DEV_MCU_FSS0_FSAS_0,
    TISCI_DEV_MCU_FSS0_HYPERBUS1P0_0,
    TISCI_DEV_MCU_FSS0,
    TISCI_DEV_WKUP_I2C0,
};

uint32_t gBoardClkModuleMcuIDInitGroup2[] = {
    TISCI_DEV_MCU_ADC12FC_16FFC0,
    TISCI_DEV_MCU_ADC12FC_16FFC1,
    TISCI_DEV_MCU_CPSW0,
    TISCI_DEV_MCU_CPT2_AGGR0,
    TISCI_DEV_MCU_DCC0,
    TISCI_DEV_MCU_DCC1,
    TISCI_DEV_MCU_DCC2,
    TISCI_DEV_MCU_TIMER1,
    TISCI_DEV_MCU_TIMER3,
    TISCI_DEV_MCU_TIMER4,
    TISCI_DEV_MCU_TIMER5,
    TISCI_DEV_MCU_TIMER6,
    TISCI_DEV_MCU_TIMER7,
    TISCI_DEV_MCU_TIMER8,
    TISCI_DEV_MCU_TIMER9,
    TISCI_DEV_MCU_I3C0,
    TISCI_DEV_MCU_I3C1,
    TISCI_DEV_MCU_ESM0,
    TISCI_DEV_MCU_MCAN0,
    TISCI_DEV_MCU_MCAN1,
    TISCI_DEV_MCU_I2C0,
    TISCI_DEV_MCU_I2C1,
    TISCI_DEV_MCU_MCSPI0,
    TISCI_DEV_MCU_MCSPI1,
    TISCI_DEV_MCU_MCSPI2,
    TISCI_DEV_WKUP_ESM0,
    TISCI_DEV_WKUP_GPIO0,
    TISCI_DEV_WKUP_GPIO1,
    TISCI_DEV_WKUP_GPIOMUX_INTRTR0,
};

uint32_t gBoardClkModuleMainIDInitGroup1[] = {
    TISCI_DEV_DDR0,
    TISCI_DEV_DDR1,
    TISCI_DEV_DDR2,
    TISCI_DEV_DDR3,
    TISCI_DEV_EMIF_DATA_0_VD,
    TISCI_DEV_EMIF_DATA_1_VD,
    TISCI_DEV_MMCSD0,
    TISCI_DEV_MMCSD1,
    TISCI_DEV_GPIO4,
    TISCI_DEV_UART8,
    TISCI_DEV_GTC0,
};

uint32_t gBoardClkModuleMainIDDeinitGroup1[] = {
    TISCI_DEV_MMCSD0,
    TISCI_DEV_MMCSD1,
    TISCI_DEV_GTC0,
};

uint32_t gBoardClkModuleMainIDGroup2[] = {
    TISCI_DEV_ATL0,
    TISCI_DEV_CPSW1,
    TISCI_DEV_CPSW_9XUSS_J7AM0,
    TISCI_DEV_CPT2_AGGR1,
    TISCI_DEV_CPT2_AGGR5,
    TISCI_DEV_CPT2_AGGR2,
    TISCI_DEV_CPT2_AGGR4,
    TISCI_DEV_CPT2_AGGR3,
    TISCI_DEV_CPT2_AGGR0,
    TISCI_DEV_CSI_RX_IF0,
    TISCI_DEV_CSI_RX_IF1,
    TISCI_DEV_CSI_RX_IF2,
    TISCI_DEV_CSI_TX_IF_V2_0,
    TISCI_DEV_CSI_TX_IF_V2_1,
    TISCI_DEV_DCC0,
    TISCI_DEV_DCC1,
    TISCI_DEV_DCC2,
    TISCI_DEV_DCC3,
    TISCI_DEV_DCC4,
    TISCI_DEV_DCC5,
    TISCI_DEV_DCC6,
    TISCI_DEV_DCC7,
    TISCI_DEV_DCC8,
    TISCI_DEV_DCC9,
    TISCI_DEV_TIMER0,
    TISCI_DEV_TIMER1,
    TISCI_DEV_TIMER2,
    TISCI_DEV_TIMER3,
    TISCI_DEV_TIMER4,
    TISCI_DEV_TIMER5,
    TISCI_DEV_TIMER6,
    TISCI_DEV_TIMER7,
    TISCI_DEV_TIMER8,
    TISCI_DEV_TIMER9,
    TISCI_DEV_TIMER10,
    TISCI_DEV_TIMER11,
    TISCI_DEV_TIMER12,
    TISCI_DEV_TIMER13,
    TISCI_DEV_TIMER14,
    TISCI_DEV_TIMER15,
    TISCI_DEV_TIMER16,
    TISCI_DEV_TIMER17,
    TISCI_DEV_TIMER18,
    TISCI_DEV_TIMER19,
    TISCI_DEV_ECAP0,
    TISCI_DEV_ECAP1,
    TISCI_DEV_ECAP2,
    TISCI_DEV_ELM0,
    TISCI_DEV_EQEP0,
    TISCI_DEV_EQEP1,
    TISCI_DEV_EQEP2,
    TISCI_DEV_ESM0,
    TISCI_DEV_UART0,
    TISCI_DEV_GPIO0,
    TISCI_DEV_GPIO2,
    TISCI_DEV_GPIO6,
    TISCI_DEV_GPMC0,
    TISCI_DEV_LED0,
    TISCI_DEV_TIMESYNC_INTRTR0,
    TISCI_DEV_CSI_PSILSS0,
    TISCI_DEV_GPIOMUX_INTRTR0,
    TISCI_DEV_CMPEVENT_INTRTR0,
    TISCI_DEV_DPHY_RX0,
    TISCI_DEV_DPHY_RX1,
    TISCI_DEV_DPHY_RX2,
    TISCI_DEV_DSS_DSI0,
    TISCI_DEV_DSS_DSI1,
    TISCI_DEV_DSS_EDP0,
    TISCI_DEV_DSS0,
    TISCI_DEV_EPWM0,
    TISCI_DEV_EPWM1,
    TISCI_DEV_EPWM2,
    TISCI_DEV_EPWM3,
    TISCI_DEV_EPWM4,
    TISCI_DEV_EPWM5,
    TISCI_DEV_CODEC0,
    TISCI_DEV_CODEC1,
    TISCI_DEV_MCAN0,
    TISCI_DEV_MCAN1,
    TISCI_DEV_MCAN2,
    TISCI_DEV_MCAN3,
    TISCI_DEV_MCAN4,
    TISCI_DEV_MCAN5,
    TISCI_DEV_MCAN6,
    TISCI_DEV_MCAN7,
    TISCI_DEV_MCAN8,
    TISCI_DEV_MCAN9,
    TISCI_DEV_MCAN10,
    TISCI_DEV_MCAN11,
    TISCI_DEV_MCAN12,
    TISCI_DEV_MCAN13,
    TISCI_DEV_MCAN14,
    TISCI_DEV_MCAN15,
    TISCI_DEV_MCAN16,
    TISCI_DEV_MCAN17,
    TISCI_DEV_MCASP0,
    TISCI_DEV_MCASP1,
    TISCI_DEV_MCASP2,
    TISCI_DEV_MCASP3,
    TISCI_DEV_MCASP4,
    TISCI_DEV_I2C0,
    TISCI_DEV_I2C1,
    TISCI_DEV_I2C2,
    TISCI_DEV_I2C3,
    TISCI_DEV_I2C4,
    TISCI_DEV_I2C5,
    TISCI_DEV_I2C6,
    TISCI_DEV_PCIE0,
    TISCI_DEV_PCIE1,
    TISCI_DEV_PCIE2,
    TISCI_DEV_PCIE3,
    TISCI_DEV_MCSPI0,
    TISCI_DEV_MCSPI1,
    TISCI_DEV_MCSPI2,
    TISCI_DEV_MCSPI3,
    TISCI_DEV_MCSPI4,
    TISCI_DEV_MCSPI5,
    TISCI_DEV_MCSPI6,
    TISCI_DEV_MCSPI7,
    TISCI_DEV_UFS0,
    TISCI_DEV_UART1,
    TISCI_DEV_UART2,
    TISCI_DEV_UART3,
    TISCI_DEV_UART4,
    TISCI_DEV_UART5,
    TISCI_DEV_UART6,
    TISCI_DEV_UART7,
    TISCI_DEV_UART9,
    TISCI_DEV_USB0,
    TISCI_DEV_DPHY_TX0,
    TISCI_DEV_DPHY_TX1,
    TISCI_DEV_SERDES_10G0,
    TISCI_DEV_SERDES_10G1,
    TISCI_DEV_SERDES_10G2,
    TISCI_DEV_SERDES_10G4,
};

/**
 * \brief Disables module clock
 *
 * \return  BOARD_SOK   - Clock disable successful.
 *          BOARD_FAIL  - Clock disable failed.
 *
 */
Board_STATUS Board_moduleClockDisable(uint32_t moduleId)
{
    Board_STATUS retVal = BOARD_SOK;
    int32_t      status = CSL_EFAIL;
    uint32_t     moduleState = 0U;
    uint32_t     resetState = 0U;
    uint32_t     contextLossState = 0U;

    /* Get the module state.
       No need to change the module state if it
       is already OFF
     */
    status = Sciclient_pmGetModuleState(moduleId,
                                        &moduleState,
                                        &resetState,
                                        &contextLossState,
                                        SCICLIENT_SERVICE_WAIT_FOREVER);
    if(moduleState != TISCI_MSG_VALUE_DEVICE_HW_STATE_OFF)
    {
        status = Sciclient_pmSetModuleState(moduleId,
                                            TISCI_MSG_VALUE_DEVICE_SW_STATE_AUTO_OFF,
                                            (TISCI_MSG_FLAG_AOP |
                                             TISCI_MSG_FLAG_DEVICE_RESET_ISO),
                                             SCICLIENT_SERVICE_WAIT_FOREVER);
        if (status == CSL_PASS)
        {
            status = Sciclient_pmSetModuleRst (moduleId,
                                               0x1U,
                                               SCICLIENT_SERVICE_WAIT_FOREVER);
            if (status != CSL_PASS)
            {
                retVal = BOARD_FAIL;
            }
        }
        else
        {
            retVal = BOARD_FAIL;
        }
    }

    return retVal;
}

/**
 * \brief Enables module clock
 *
 * \return  BOARD_SOK   - Clock enable sucessful.
 *          BOARD_FAIL  - Clock enable failed.
 *
 */
Board_STATUS Board_moduleClockEnable(uint32_t moduleId)
{
    Board_STATUS retVal = BOARD_SOK;
    int32_t      status = CSL_EFAIL;
    uint32_t     moduleState = 0U;
    uint32_t     resetState = 0U;
    uint32_t     contextLossState = 0U;

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
        if(gBoardInitParams.pscMode == BOARD_PSC_DEVICE_MODE_NONEXCLUSIVE)
        {
            status = Sciclient_pmSetModuleState(moduleId,
                                                TISCI_MSG_VALUE_DEVICE_SW_STATE_ON,
                                                (TISCI_MSG_FLAG_AOP |
                                                 TISCI_MSG_FLAG_DEVICE_RESET_ISO),
                                                 SCICLIENT_SERVICE_WAIT_FOREVER);
        }
        else
        {
            status = Sciclient_pmSetModuleState(moduleId,
                                                TISCI_MSG_VALUE_DEVICE_SW_STATE_ON,
                                                (TISCI_MSG_FLAG_AOP |
                                                 TISCI_MSG_FLAG_DEVICE_EXCLUSIVE |
                                                 TISCI_MSG_FLAG_DEVICE_RESET_ISO),
                                                 SCICLIENT_SERVICE_WAIT_FOREVER);
        }
        if (status == CSL_PASS)
        {
            status = Sciclient_pmSetModuleRst (moduleId,
                                               0x0U,
                                               SCICLIENT_SERVICE_WAIT_FOREVER);
            if (status != CSL_PASS)
            {
                retVal = BOARD_FAIL;
            }
        }
        else
        {
            retVal = BOARD_FAIL;
        }
    }

    return retVal;
}

/**
 * \brief clock Initialization function
 *
 * \return  BOARD_SOK              - Clock initialization successful.
 *          BOARD_INIT_CLOCK_FAIL  - Clock initialization failed.
 *
 */
static Board_STATUS Board_moduleClockInit(uint32_t *clkData, uint32_t size)
{
    Board_STATUS  status = BOARD_SOK;
    uint32_t index;

    for(index = 0; index < size; index++)
    {
        status = Board_moduleClockEnable(clkData[index]);
        if(status != BOARD_SOK)
        {
            status = BOARD_INIT_CLOCK_FAIL;
            break;
        }
    }

    return status;
}

/**
 * \brief Clock de-initialization function
 *
 *
 * \return  BOARD_SOK              - Clock de-initialization successful.
 *          BOARD_INIT_CLOCK_FAIL  - Clock de-initialization failed.
 *
 */
static Board_STATUS Board_moduleClockDeinit(uint32_t *clkData, uint32_t size)
{
    Board_STATUS  status = BOARD_SOK;
    uint32_t index;

    for(index = 0; index < size; index++)
    {
        status = Board_moduleClockDisable(clkData[index]);
        if(status != BOARD_SOK)
        {
            status = BOARD_INIT_CLOCK_FAIL;
            break;
        }
    }

    return status;
}

/**
 * \brief clock Initialization function for MCU domain
 *
 * Enables different power domains and peripheral clocks of the MCU.
 * Some of the power domains and peripherals will be OFF by default.
 * Enabling the power domains is mandatory before accessing using
 * board interfaces connected to those peripherals.
 *
 * \return  BOARD_SOK              - Clock initialization sucessful.
 *          BOARD_INIT_CLOCK_FAIL  - Clock initialization failed.
 *
 */
Board_STATUS Board_moduleClockInitMcu(void)
{
    Board_STATUS  status = BOARD_SOK;
    uint32_t size;

    /* Restoring MCU DMtimer0 FCLK to HFOSC0 (changed by ROM) */
    HW_WR_REG32((CSL_MCU_CTRL_MMR0_CFG0_BASE + CSL_MCU_CTRL_MMR_CFG0_MCU_TIMER0_CLKSEL), 0);

    if((gBoardInitParams.mcuClkGrp == BOARD_MCU_CLOCK_GROUP_ALL) ||
       (gBoardInitParams.mcuClkGrp == BOARD_MCU_CLOCK_GROUP1))
    {
        size = sizeof(gBoardClkModuleMcuIDInitGroupl) / sizeof(uint32_t);
        status = Board_moduleClockInit(gBoardClkModuleMcuIDInitGroupl, size);
        if(status != BOARD_SOK)
        {
            return status;
        }
    }

    if((gBoardInitParams.mcuClkGrp == BOARD_MCU_CLOCK_GROUP_ALL) ||
       (gBoardInitParams.mcuClkGrp == BOARD_MCU_CLOCK_GROUP2))
    {
        size = sizeof(gBoardClkModuleMcuIDInitGroup2) / sizeof(uint32_t);
        status = Board_moduleClockInit(gBoardClkModuleMcuIDInitGroup2, size);
        if(status != BOARD_SOK)
        {
            return status;
        }
    }

    return status;
}

/**
 * \brief clock Initialization function for MAIN domain
 *
 * Enables different power domains and peripheral clocks of the SoC.
 * Some of the power domains and peripherals will be OFF by default.
 * Enabling the power domains is mandatory before accessing using
 * board interfaces connected to those peripherals.
 *
 * \return  BOARD_SOK              - Clock initialization successful.
 *          BOARD_INIT_CLOCK_FAIL  - Clock initialization failed.
 *
 */
Board_STATUS Board_moduleClockInitMain(void)
{
    Board_STATUS  status = BOARD_SOK;
    uint32_t size;

    if((gBoardInitParams.mainClkGrp == BOARD_MAIN_CLOCK_GROUP_ALL) ||
       (gBoardInitParams.mainClkGrp == BOARD_MAIN_CLOCK_GROUP1))
    {
        size = sizeof(gBoardClkModuleMainIDInitGroup1) / sizeof(uint32_t);
        status = Board_moduleClockInit(gBoardClkModuleMainIDInitGroup1, size);
        if(status != BOARD_SOK)
        {
            return status;
        }
    }

    if((gBoardInitParams.mainClkGrp == BOARD_MAIN_CLOCK_GROUP_ALL) ||
       (gBoardInitParams.mainClkGrp == BOARD_MAIN_CLOCK_GROUP2))
    {
        size = sizeof(gBoardClkModuleMainIDGroup2) / sizeof(uint32_t);
        status = Board_moduleClockInit(gBoardClkModuleMainIDGroup2, size);
    }

    return status;
}

/**
 * \brief clock de-initialization function for MCU domain
 *
 * Disables different power domains and peripheral clocks of the SoC.
 *
 * \return  BOARD_SOK              - Clock de-initialization successful. 
 *          BOARD_INIT_CLOCK_FAIL  - Clock de-initialization failed.
 *
 */
Board_STATUS Board_moduleClockDeinitMcu(void)
{
    Board_STATUS  status = BOARD_SOK;
    uint32_t size;

    if((gBoardInitParams.mcuClkGrp == BOARD_MCU_CLOCK_GROUP_ALL) ||
       (gBoardInitParams.mcuClkGrp == BOARD_MCU_CLOCK_GROUP1))
    {
        size = sizeof(gBoardClkModuleMcuIDDeinitGroupl) / sizeof(uint32_t);
        status = Board_moduleClockDeinit(gBoardClkModuleMcuIDDeinitGroupl, size);
        if(status != BOARD_SOK)
        {
            return status;
        }
    }

    if((gBoardInitParams.mcuClkGrp == BOARD_MCU_CLOCK_GROUP_ALL) ||
       (gBoardInitParams.mcuClkGrp == BOARD_MCU_CLOCK_GROUP2))
    {
        size = sizeof(gBoardClkModuleMcuIDInitGroup2) / sizeof(uint32_t);
        status = Board_moduleClockDeinit(gBoardClkModuleMcuIDInitGroup2, size);
        if(status != BOARD_SOK)
        {
            return status;
        }
    }

    return status;
}

/**
 * \brief clock de-initialization function for MAIN domain
 *
 * Disables different power domains and peripheral clocks of the SoC.
 *
 * \return  BOARD_SOK              - Clock de-initialization successful.
 *          BOARD_INIT_CLOCK_FAIL  - Clock de-initialization failed.
 *
 */
Board_STATUS Board_moduleClockDeinitMain(void)
{
    Board_STATUS  status = BOARD_SOK;
    uint32_t size;

    if((gBoardInitParams.mainClkGrp == BOARD_MAIN_CLOCK_GROUP_ALL) ||
       (gBoardInitParams.mainClkGrp == BOARD_MAIN_CLOCK_GROUP1))
    {
        size = sizeof(gBoardClkModuleMainIDDeinitGroup1) / sizeof(uint32_t);
        status = Board_moduleClockDeinit(gBoardClkModuleMainIDDeinitGroup1, size);
        if(status != BOARD_SOK)
        {
            return status;
        }
    }

    if((gBoardInitParams.mainClkGrp == BOARD_MAIN_CLOCK_GROUP_ALL) ||
       (gBoardInitParams.mainClkGrp == BOARD_MAIN_CLOCK_GROUP2))
    {
        size = sizeof(gBoardClkModuleMainIDGroup2) / sizeof(uint32_t);
        status = Board_moduleClockDeinit(gBoardClkModuleMainIDGroup2, size);
    }

    return status;
}
