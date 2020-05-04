/******************************************************************************
 * Copyright (c) 2020 Texas Instruments Incorporated - http://www.ti.com
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

/**
*
* \file  TPR12_pinmux_data.c
*
* \brief  This file contains the pin mux configurations for the boards.
*         These are prepared based on how the peripherals are extended on
*         the boards.
*
*/

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include "TPR12_pinmux.h"

/** Peripheral Pin Configurations */

static pinmuxPerCfg_t gDss_uart0PinCfg[] =
{
    /* MyDSS_UART1 -> DSS_UARTA_TX -> J19 */
    {
        CSL_MSS_IOMUX_PADDC_CFG_REG, PIN_MODE(4) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyDSS_UART1 -> DSS_UARTA_RX -> H19 */
    {
        CSL_MSS_IOMUX_PADDD_CFG_REG, PIN_MODE(1) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    {PINMUX_END}
};

static pinmuxModuleCfg_t gDss_uartPinCfg[] =
{
    {0, TRUE, gDss_uart0PinCfg},
    {PINMUX_END}
};


static pinmuxPerCfg_t gError0PinCfg[] =
{
    /* MyERROR1 -> NERROR_IN -> L3 */
    {
        CSL_MSS_IOMUX_PADAR_CFG_REG, PIN_MODE(0) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyERROR1 -> WARM_RESET -> K1 */
    {
        CSL_MSS_IOMUX_PADAS_CFG_REG, PIN_MODE(0) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyERROR1 -> NERROR_OUT -> L1 */
    {
        CSL_MSS_IOMUX_PADAT_CFG_REG, PIN_MODE(0) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    {PINMUX_END}
};

static pinmuxModuleCfg_t gErrorPinCfg[] =
{
    {0, TRUE, gError0PinCfg},
    {PINMUX_END}
};


static pinmuxPerCfg_t gFe1_refclk0PinCfg[] =
{
    /* MyFE1_REFCLK1 -> FE1_REFCLK -> H1 */
    {
        CSL_MSS_IOMUX_PADAB_CFG_REG, PIN_MODE(7) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    {PINMUX_END}
};

static pinmuxModuleCfg_t gFe1_refclkPinCfg[] =
{
    {0, TRUE, gFe1_refclk0PinCfg},
    {PINMUX_END}
};


static pinmuxPerCfg_t gFe2_refclk0PinCfg[] =
{
    /* MyFE2_REFCLK1 -> FE2_REFCLK -> J2 */
    {
        CSL_MSS_IOMUX_PADAC_CFG_REG, PIN_MODE(7) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    {PINMUX_END}
};

static pinmuxModuleCfg_t gFe2_refclkPinCfg[] =
{
    {0, TRUE, gFe2_refclk0PinCfg},
    {PINMUX_END}
};


static pinmuxPerCfg_t gGpio9PinCfg[] =
{
    /* MyGPIO49 -> RCSS_GPIO_49 -> E19 */
    {
        CSL_MSS_IOMUX_PADBX_CFG_REG, PIN_MODE(0) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyGPIO9 -> MSS_GPIO_9 -> B19 */
    {
        CSL_MSS_IOMUX_PADDN_CFG_REG, PIN_MODE(0) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    {PINMUX_END}
};

static pinmuxPerCfg_t gGpio8PinCfg[] =
{
    /* MyGPIO28 -> MSS_GPIO_28 -> G3 */
    {
        CSL_MSS_IOMUX_PADBB_CFG_REG, PIN_MODE(0) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyGPIO8 -> MSS_GPIO_8 -> U18 */
    {
        CSL_MSS_IOMUX_PADCS_CFG_REG, PIN_MODE(3) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    {PINMUX_END}
};

static pinmuxPerCfg_t gGpio2PinCfg[] =
{
    /* MyGPIO2 -> MSS_GPIO_2 -> H2 */
    {
        CSL_MSS_IOMUX_PADAZ_CFG_REG, PIN_MODE(1) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyGPIO12 -> MSS_GPIO_12 -> A18 */
    {
        CSL_MSS_IOMUX_PADDQ_CFG_REG, PIN_MODE(0) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    {PINMUX_END}
};

static pinmuxPerCfg_t gGpio0PinCfg[] =
{
    /* MyGPIO10 -> MSS_GPIO_10 -> B18 */
    {
        CSL_MSS_IOMUX_PADDO_CFG_REG, PIN_MODE(0) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    {PINMUX_END}
};

static pinmuxPerCfg_t gGpio1PinCfg[] =
{
    /* MyGPIO11 -> MSS_GPIO_11 -> C17 */
    {
        CSL_MSS_IOMUX_PADDP_CFG_REG, PIN_MODE(0) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    {PINMUX_END}
};

static pinmuxPerCfg_t gGpio3PinCfg[] =
{
    /* MyGPIO13 -> MSS_GPIO_13 -> B17 */
    {
        CSL_MSS_IOMUX_PADDR_CFG_REG, PIN_MODE(0) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    {PINMUX_END}
};

static pinmuxModuleCfg_t gGpioPinCfg[] =
{
    {9, TRUE, gGpio9PinCfg},
    {8, TRUE, gGpio8PinCfg},
    {2, TRUE, gGpio2PinCfg},
    {0, TRUE, gGpio0PinCfg},
    {1, TRUE, gGpio1PinCfg},
    {3, TRUE, gGpio3PinCfg},
    {PINMUX_END}
};


static pinmuxPerCfg_t gHw_sync_fe10PinCfg[] =
{
    /* MyHW_SYNC_FE11 -> HW_SYNC_FE1 -> V17 */
    {
        CSL_MSS_IOMUX_PADCY_CFG_REG, PIN_MODE(3) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    {PINMUX_END}
};

static pinmuxModuleCfg_t gHw_sync_fe1PinCfg[] =
{
    {0, TRUE, gHw_sync_fe10PinCfg},
    {PINMUX_END}
};


static pinmuxPerCfg_t gHw_sync_fe20PinCfg[] =
{
    /* MyHW_SYNC_FE21 -> HW_SYNC_FE2 -> W17 */
    {
        CSL_MSS_IOMUX_PADCZ_CFG_REG, PIN_MODE(3) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    {PINMUX_END}
};

static pinmuxModuleCfg_t gHw_sync_fe2PinCfg[] =
{
    {0, TRUE, gHw_sync_fe20PinCfg},
    {PINMUX_END}
};


static pinmuxPerCfg_t gJtag0PinCfg[] =
{
    /* MyJTAG1 -> TCK -> C3 */
    {
        CSL_MSS_IOMUX_PADAU_CFG_REG, PIN_MODE(1) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyJTAG1 -> TMS -> D4 */
    {
        CSL_MSS_IOMUX_PADAV_CFG_REG, PIN_MODE(1) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyJTAG1 -> TDI -> C5 */
    {
        CSL_MSS_IOMUX_PADAW_CFG_REG, PIN_MODE(1) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyJTAG1 -> TDO -> D6 */
    {
        CSL_MSS_IOMUX_PADAX_CFG_REG, PIN_MODE(1) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    {PINMUX_END}
};

static pinmuxModuleCfg_t gJtagPinCfg[] =
{
    {0, TRUE, gJtag0PinCfg},
    {PINMUX_END}
};


static pinmuxPerCfg_t gMss_epwm0PinCfg[] =
{
    /* MyMSS_EPWM1 -> MSS_EPWMA0 -> E3 */
    {
        CSL_MSS_IOMUX_PADAY_CFG_REG, PIN_MODE(18) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    {PINMUX_END}
};

static pinmuxModuleCfg_t gMss_epwmPinCfg[] =
{
    {0, TRUE, gMss_epwm0PinCfg},
    {PINMUX_END}
};


static pinmuxPerCfg_t gMss_i2c0PinCfg[] =
{
    /* MyMSS_I2C1 -> MSS_I2CA_SCL -> F18 */
    {
        CSL_MSS_IOMUX_PADBZ_CFG_REG, PIN_MODE(3) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyMSS_I2C1 -> MSS_I2CA_SDA -> F16 */
    {
        CSL_MSS_IOMUX_PADBY_CFG_REG, PIN_MODE(3) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    {PINMUX_END}
};

static pinmuxModuleCfg_t gMss_i2cPinCfg[] =
{
    {0, TRUE, gMss_i2c0PinCfg},
    {PINMUX_END}
};


static pinmuxPerCfg_t gMss_mcan0PinCfg[] =
{
    /* MyMSS_MCAN1 -> MSS_MCANA_RX -> B2 */
    {
        CSL_MSS_IOMUX_PADAF_CFG_REG, PIN_MODE(1) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyMSS_MCAN1 -> MSS_MCANA_TX -> A2 */
    {
        CSL_MSS_IOMUX_PADAG_CFG_REG, PIN_MODE(1) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyMSS_MCAN2 -> MSS_MCANB_RX -> C1 */
    {
        CSL_MSS_IOMUX_PADAD_CFG_REG, PIN_MODE(1) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyMSS_MCAN2 -> MSS_MCANB_TX -> B1 */
    {
        CSL_MSS_IOMUX_PADAE_CFG_REG, PIN_MODE(1) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    {PINMUX_END}
};

static pinmuxModuleCfg_t gMss_mcanPinCfg[] =
{
    {0, TRUE, gMss_mcan0PinCfg},
    {PINMUX_END}
};


static pinmuxPerCfg_t gMss_mdio0PinCfg[] =
{
    /* MyMSS_MDIO1 -> MSS_MDIO_DATA -> P19 */
    {
        CSL_MSS_IOMUX_PADCM_CFG_REG, PIN_MODE(1) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyMSS_MDIO1 -> MSS_MDIO_CLK -> R19 */
    {
        CSL_MSS_IOMUX_PADCN_CFG_REG, PIN_MODE(1) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    {PINMUX_END}
};

static pinmuxModuleCfg_t gMss_mdioPinCfg[] =
{
    {0, TRUE, gMss_mdio0PinCfg},
    {PINMUX_END}
};


static pinmuxPerCfg_t gMss_mibspi0PinCfg[] =
{
    /* MyMSS_MIBSPI1 -> MSS_MIBSPIB_CS1 -> E18 */
    {
        CSL_MSS_IOMUX_PADAA_CFG_REG, PIN_MODE(6) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyMSS_MIBSPI1 -> MSS_MIBSPIB_CS2 -> E17 */
    {
        CSL_MSS_IOMUX_PADBC_CFG_REG, PIN_MODE(17) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyMSS_MIBSPI1 -> MSS_MIBSPIB_MOSI -> C18 */
    {
        CSL_MSS_IOMUX_PADAH_CFG_REG, PIN_MODE(1) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyMSS_MIBSPI1 -> MSS_MIBSPIB_MISO -> C19 */
    {
        CSL_MSS_IOMUX_PADAI_CFG_REG, PIN_MODE(1) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyMSS_MIBSPI1 -> MSS_MIBSPIB_CLK -> D18 */
    {
        CSL_MSS_IOMUX_PADAJ_CFG_REG, PIN_MODE(1) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyMSS_MIBSPI1 -> MSS_MIBSPIB_CS0 -> D19 */
    {
        CSL_MSS_IOMUX_PADAK_CFG_REG, PIN_MODE(1) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyMSS_MIBSPI2 -> MSS_MIBSPIA_HOSTIRQ -> G18 */
    {
        CSL_MSS_IOMUX_PADDL_CFG_REG, PIN_MODE(16) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyMSS_MIBSPI2 -> MSS_MIBSPIA_MOSI -> H18 */
    {
        CSL_MSS_IOMUX_PADDH_CFG_REG, PIN_MODE(16) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyMSS_MIBSPI2 -> MSS_MIBSPIA_MISO -> G17 */
    {
        CSL_MSS_IOMUX_PADDI_CFG_REG, PIN_MODE(16) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyMSS_MIBSPI2 -> MSS_MIBSPIA_CLK -> G19 */
    {
        CSL_MSS_IOMUX_PADDJ_CFG_REG, PIN_MODE(16) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyMSS_MIBSPI2 -> MSS_MIBSPIA_CS0 -> F19 */
    {
        CSL_MSS_IOMUX_PADDK_CFG_REG, PIN_MODE(16) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    {PINMUX_END}
};

static pinmuxModuleCfg_t gMss_mibspiPinCfg[] =
{
    {0, TRUE, gMss_mibspi0PinCfg},
    {PINMUX_END}
};


static pinmuxPerCfg_t gMss_qspi0PinCfg[] =
{
    /* MyMSS_QSPI1 -> MSS_QSPI_0 -> C2 */
    {
        CSL_MSS_IOMUX_PADAL_CFG_REG, PIN_MODE(1) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyMSS_QSPI1 -> MSS_QSPI_1 -> D2 */
    {
        CSL_MSS_IOMUX_PADAM_CFG_REG, PIN_MODE(1) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyMSS_QSPI1 -> MSS_QSPI_2 -> D1 */
    {
        CSL_MSS_IOMUX_PADAN_CFG_REG, PIN_MODE(1) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyMSS_QSPI1 -> MSS_QSPI_3 -> E2 */
    {
        CSL_MSS_IOMUX_PADAO_CFG_REG, PIN_MODE(1) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyMSS_QSPI1 -> MSS_QSPI_CLK -> E1 */
    {
        CSL_MSS_IOMUX_PADAP_CFG_REG, PIN_MODE(1) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyMSS_QSPI1 -> MSS_QSPI_CS -> F2 */
    {
        CSL_MSS_IOMUX_PADAQ_CFG_REG, PIN_MODE(1) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    {PINMUX_END}
};

static pinmuxModuleCfg_t gMss_qspiPinCfg[] =
{
    {0, TRUE, gMss_qspi0PinCfg},
    {PINMUX_END}
};


static pinmuxPerCfg_t gMss_rgmii0PinCfg[] =
{
    /* MyMSS_RGMII1 -> MSS_RGMII_TCTL -> J18 */
    {
        CSL_MSS_IOMUX_PADCA_CFG_REG, PIN_MODE(3) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyMSS_RGMII1 -> MSS_RGMII_RCTL -> J17 */
    {
        CSL_MSS_IOMUX_PADCB_CFG_REG, PIN_MODE(3) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyMSS_RGMII1 -> MSS_RGMII_TD3 -> K18 */
    {
        CSL_MSS_IOMUX_PADCC_CFG_REG, PIN_MODE(3) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyMSS_RGMII1 -> MSS_RGMII_TD2 -> K16 */
    {
        CSL_MSS_IOMUX_PADCD_CFG_REG, PIN_MODE(3) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyMSS_RGMII1 -> MSS_RGMII_TD1 -> L17 */
    {
        CSL_MSS_IOMUX_PADCE_CFG_REG, PIN_MODE(3) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyMSS_RGMII1 -> MSS_RGMII_TD0 -> L18 */
    {
        CSL_MSS_IOMUX_PADCF_CFG_REG, PIN_MODE(3) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyMSS_RGMII1 -> MSS_RGMII_TCLK -> K19 */
    {
        CSL_MSS_IOMUX_PADCG_CFG_REG, PIN_MODE(3) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyMSS_RGMII1 -> MSS_RGMII_RCLK -> M19 */
    {
        CSL_MSS_IOMUX_PADCH_CFG_REG, PIN_MODE(3) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyMSS_RGMII1 -> MSS_RGMII_RD3 -> L19 */
    {
        CSL_MSS_IOMUX_PADCI_CFG_REG, PIN_MODE(3) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyMSS_RGMII1 -> MSS_RGMII_RD2 -> M18 */
    {
        CSL_MSS_IOMUX_PADCJ_CFG_REG, PIN_MODE(3) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyMSS_RGMII1 -> MSS_RGMII_RD1 -> N19 */
    {
        CSL_MSS_IOMUX_PADCK_CFG_REG, PIN_MODE(3) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyMSS_RGMII1 -> MSS_RGMII_RD0 -> P18 */
    {
        CSL_MSS_IOMUX_PADCL_CFG_REG, PIN_MODE(3) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    {PINMUX_END}
};

static pinmuxModuleCfg_t gMss_rgmiiPinCfg[] =
{
    {0, TRUE, gMss_rgmii0PinCfg},
    {PINMUX_END}
};


static pinmuxPerCfg_t gMss_rs2320PinCfg[] =
{
    /* MyMSS_RS2321 -> MSS_RS232_RX -> G2 */
    {
        CSL_MSS_IOMUX_PADBD_CFG_REG, PIN_MODE(1) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyMSS_RS2321 -> MSS_RS232_TX -> G1 */
    {
        CSL_MSS_IOMUX_PADBE_CFG_REG, PIN_MODE(1) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    {PINMUX_END}
};

static pinmuxModuleCfg_t gMss_rs232PinCfg[] =
{
    {0, TRUE, gMss_rs2320PinCfg},
    {PINMUX_END}
};


static pinmuxPerCfg_t gMss_uart0PinCfg[] =
{
    /* MyMSS_UART1 -> MSS_UARTA_RX -> U3 */
    {
        CSL_MSS_IOMUX_PADDA_CFG_REG, PIN_MODE(5) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyMSS_UART1 -> MSS_UARTA_TX -> W2 */
    {
        CSL_MSS_IOMUX_PADDB_CFG_REG, PIN_MODE(5) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyMSS_UART2 -> MSS_UARTB_TX -> V9 */
    {
        CSL_MSS_IOMUX_PADDE_CFG_REG, PIN_MODE(6) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    {PINMUX_END}
};

static pinmuxModuleCfg_t gMss_uartPinCfg[] =
{
    {0, TRUE, gMss_uart0PinCfg},
    {PINMUX_END}
};


static pinmuxPerCfg_t gPmic_clkout0PinCfg[] =
{
    /* MyPMIC_CLKOUT1 -> PMIC_CLKOUT -> F1 */
    {
        CSL_MSS_IOMUX_PADBA_CFG_REG, PIN_MODE(1) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    {PINMUX_END}
};

static pinmuxModuleCfg_t gPmic_clkoutPinCfg[] =
{
    {0, TRUE, gPmic_clkout0PinCfg},
    {PINMUX_END}
};


static pinmuxPerCfg_t gRcss_mibspi0PinCfg[] =
{
    /* MyRCSS_MIBSPI1 -> RCSS_MIBSPIA_MOSI -> R18 */
    {
        CSL_MSS_IOMUX_PADCO_CFG_REG, PIN_MODE(1) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyRCSS_MIBSPI1 -> RCSS_MIBSPIA_MISO -> R17 */
    {
        CSL_MSS_IOMUX_PADCP_CFG_REG, PIN_MODE(1) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyRCSS_MIBSPI1 -> RCSS_MIBSPIA_CLK -> T18 */
    {
        CSL_MSS_IOMUX_PADCQ_CFG_REG, PIN_MODE(1) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyRCSS_MIBSPI1 -> RCSS_MIBSPIA_CS0 -> T19 */
    {
        CSL_MSS_IOMUX_PADCR_CFG_REG, PIN_MODE(1) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyRCSS_MIBSPI2 -> RCSS_MIBSPIB_MOSI -> U19 */
    {
        CSL_MSS_IOMUX_PADCT_CFG_REG, PIN_MODE(1) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyRCSS_MIBSPI2 -> RCSS_MIBSPIB_MISO -> V18 */
    {
        CSL_MSS_IOMUX_PADCU_CFG_REG, PIN_MODE(1) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyRCSS_MIBSPI2 -> RCSS_MIBSPIB_CLK -> V19 */
    {
        CSL_MSS_IOMUX_PADCV_CFG_REG, PIN_MODE(1) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyRCSS_MIBSPI2 -> RCSS_MIBSPIB_CS0 -> U17 */
    {
        CSL_MSS_IOMUX_PADCW_CFG_REG, PIN_MODE(1) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    {PINMUX_END}
};

static pinmuxModuleCfg_t gRcss_mibspiPinCfg[] =
{
    {0, TRUE, gRcss_mibspi0PinCfg},
    {PINMUX_END}
};


static pinmuxPerCfg_t gRcss_uart0PinCfg[] =
{
    /* MyRCSS_UART1 -> RCSS_UARTA_RX -> B16 */
    {
        CSL_MSS_IOMUX_PADDT_CFG_REG, PIN_MODE(2) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyRCSS_UART1 -> RCSS_UARTA_TX -> A17 */
    {
        CSL_MSS_IOMUX_PADDS_CFG_REG, PIN_MODE(2) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    {PINMUX_END}
};

static pinmuxModuleCfg_t gRcss_uartPinCfg[] =
{
    {0, TRUE, gRcss_uart0PinCfg},
    {PINMUX_END}
};


static pinmuxPerCfg_t gTrace0PinCfg[] =
{
    /* MyTRACE1 -> TRACE_DATA_0 -> V16 */
    {
        CSL_MSS_IOMUX_PADBF_CFG_REG, PIN_MODE(0) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyTRACE1 -> TRACE_DATA_1 -> U15 */
    {
        CSL_MSS_IOMUX_PADBG_CFG_REG, PIN_MODE(0) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyTRACE1 -> TRACE_DATA_2 -> W16 */
    {
        CSL_MSS_IOMUX_PADBH_CFG_REG, PIN_MODE(0) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyTRACE1 -> TRACE_DATA_3 -> V15 */
    {
        CSL_MSS_IOMUX_PADBI_CFG_REG, PIN_MODE(0) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyTRACE1 -> TRACE_DATA_4 -> W15 */
    {
        CSL_MSS_IOMUX_PADBJ_CFG_REG, PIN_MODE(0) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyTRACE1 -> TRACE_DATA_5 -> V14 */
    {
        CSL_MSS_IOMUX_PADBK_CFG_REG, PIN_MODE(0) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyTRACE1 -> TRACE_DATA_6 -> U13 */
    {
        CSL_MSS_IOMUX_PADBL_CFG_REG, PIN_MODE(0) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyTRACE1 -> TRACE_DATA_7 -> W14 */
    {
        CSL_MSS_IOMUX_PADBM_CFG_REG, PIN_MODE(0) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyTRACE1 -> TRACE_DATA_8 -> V13 */
    {
        CSL_MSS_IOMUX_PADBN_CFG_REG, PIN_MODE(0) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyTRACE1 -> TRACE_DATA_9 -> W13 */
    {
        CSL_MSS_IOMUX_PADBO_CFG_REG, PIN_MODE(0) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyTRACE1 -> TRACE_DATA_10 -> U11 */
    {
        CSL_MSS_IOMUX_PADBP_CFG_REG, PIN_MODE(0) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyTRACE1 -> TRACE_DATA_11 -> V11 */
    {
        CSL_MSS_IOMUX_PADBQ_CFG_REG, PIN_MODE(0) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyTRACE1 -> TRACE_DATA_12 -> W11 */
    {
        CSL_MSS_IOMUX_PADBR_CFG_REG, PIN_MODE(0) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyTRACE1 -> TRACE_DATA_13 -> V10 */
    {
        CSL_MSS_IOMUX_PADBS_CFG_REG, PIN_MODE(0) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyTRACE1 -> TRACE_DATA_14 -> W10 */
    {
        CSL_MSS_IOMUX_PADBT_CFG_REG, PIN_MODE(0) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyTRACE1 -> TRACE_DATA_15 -> T10 */
    {
        CSL_MSS_IOMUX_PADBU_CFG_REG, PIN_MODE(0) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyTRACE1 -> TRACE_CLK -> W12 */
    {
        CSL_MSS_IOMUX_PADBV_CFG_REG, PIN_MODE(0) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyTRACE1 -> TRACE_CTL -> V12 */
    {
        CSL_MSS_IOMUX_PADBW_CFG_REG, PIN_MODE(0) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    {PINMUX_END}
};

static pinmuxModuleCfg_t gTracePinCfg[] =
{
    {0, TRUE, gTrace0PinCfg},
    {PINMUX_END}
};


static pinmuxPerCfg_t gXref_clk00PinCfg[] =
{
    /* MyXREF_CLK01 -> XREF_CLK0 -> J1 */
    {
        CSL_MSS_IOMUX_PADDF_CFG_REG, PIN_MODE(1) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    {PINMUX_END}
};

static pinmuxModuleCfg_t gXref_clk0PinCfg[] =
{
    {0, TRUE, gXref_clk00PinCfg},
    {PINMUX_END}
};


static pinmuxPerCfg_t gXref_clk10PinCfg[] =
{
    /* MyXREF_CLK11 -> XREF_CLK1 -> K2 */
    {
        CSL_MSS_IOMUX_PADDG_CFG_REG, PIN_MODE(1) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    {PINMUX_END}
};

static pinmuxModuleCfg_t gXref_clk1PinCfg[] =
{
    {0, TRUE, gXref_clk10PinCfg},
    {PINMUX_END}
};


pinmuxBoardCfg_t gTPR12PinmuxData[] =
{
    {0, gDss_uartPinCfg},
    {1, gErrorPinCfg},
    {2, gFe1_refclkPinCfg},
    {3, gFe2_refclkPinCfg},
    {4, gGpioPinCfg},
    {5, gHw_sync_fe1PinCfg},
    {6, gHw_sync_fe2PinCfg},
    {7, gJtagPinCfg},
    {8, gMss_epwmPinCfg},
    {9, gMss_i2cPinCfg},
    {10, gMss_mcanPinCfg},
    {11, gMss_mdioPinCfg},
    {12, gMss_mibspiPinCfg},
    {13, gMss_qspiPinCfg},
    {14, gMss_rgmiiPinCfg},
    {15, gMss_rs232PinCfg},
    {16, gMss_uartPinCfg},
    {17, gPmic_clkoutPinCfg},
    {18, gRcss_mibspiPinCfg},
    {19, gRcss_uartPinCfg},
    {20, gTracePinCfg},
    {21, gXref_clk0PinCfg},
    {22, gXref_clk1PinCfg},
    {PINMUX_END}
};
