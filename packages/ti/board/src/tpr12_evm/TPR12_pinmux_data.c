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


static pinmuxPerCfg_t gTpr12_pmic_clkout0PinCfg[] =
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
    { 0, TRUE, gTpr12_pmic_clkout0PinCfg},
    {PINMUX_END}
};
       
static pinmuxPerCfg_t gTpr12_mss_rs2320PinCfg[] =
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
    { 0, TRUE, gTpr12_mss_rs2320PinCfg},
    {PINMUX_END}
};
       
static pinmuxPerCfg_t gTpr12_mss_i2c0PinCfg[] =
{
    /* MyMSS_I2C1 -> MSS_I2CA_SCL -> F18 */
    {
        CSL_MSS_IOMUX_PADBZ_CFG_REG, PIN_MODE(3) | \
        ((~PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyMSS_I2C1 -> MSS_I2CA_SDA -> F16 */
    {
        CSL_MSS_IOMUX_PADBY_CFG_REG, PIN_MODE(3) | \
        ((~PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    {PINMUX_END}
};
static pinmuxModuleCfg_t gMss_i2cPinCfg[] =
{
    { 0, TRUE, gTpr12_mss_i2c0PinCfg},
    {PINMUX_END}
};
       
static pinmuxPerCfg_t gTpr12_mss_rgmii0PinCfg[] =
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
    { 0, TRUE, gTpr12_mss_rgmii0PinCfg},
    {PINMUX_END}
};
       
static pinmuxPerCfg_t gTpr12_rcss_gpio50PinCfg[] =
{
    /* MyRCSS_GPIO1 -> RCSS_GPIO50_PIN -> B15 */
    {
        CSL_MSS_IOMUX_PADDW_CFG_REG, PIN_MODE(12) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    {PINMUX_END}
};
static pinmuxPerCfg_t gTpr12_rcss_gpio34PinCfg[] =
{
    /* MyRCSS_GPIO1 -> RCSS_GPIO34_PIN -> U18 */
    {
        CSL_MSS_IOMUX_PADCS_CFG_REG, PIN_MODE(7) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    {PINMUX_END}
};
static pinmuxPerCfg_t gTpr12_rcss_gpio48PinCfg[] =
{
    /* MyRCSS_GPIO1 -> RCSS_GPIO48_PIN -> C15 */
    {
        CSL_MSS_IOMUX_PADDU_CFG_REG, PIN_MODE(12) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    {PINMUX_END}
};
static pinmuxPerCfg_t gTpr12_rcss_gpio42PinCfg[] =
{
    /* MyRCSS_GPIO1 -> RCSS_GPIO42_PIN -> V17 */
    {
        CSL_MSS_IOMUX_PADCY_CFG_REG, PIN_MODE(0) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    {PINMUX_END}
};
static pinmuxPerCfg_t gTpr12_rcss_gpio51PinCfg[] =
{
    /* MyRCSS_GPIO1 -> RCSS_GPIO51_PIN -> A15 */
    {
        CSL_MSS_IOMUX_PADDX_CFG_REG, PIN_MODE(12) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    {PINMUX_END}
};
static pinmuxPerCfg_t gTpr12_rcss_gpio35PinCfg[] =
{
    /* MyRCSS_GPIO1 -> RCSS_GPIO35_PIN -> W18 */
    {
        CSL_MSS_IOMUX_PADCX_CFG_REG, PIN_MODE(10) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    {PINMUX_END}
};
static pinmuxPerCfg_t gTpr12_rcss_gpio49PinCfg[] =
{
    /* MyRCSS_GPIO1 -> RCSS_GPIO49_PIN -> A16 */
    {
        CSL_MSS_IOMUX_PADDV_CFG_REG, PIN_MODE(12) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    {PINMUX_END}
};
static pinmuxPerCfg_t gTpr12_rcss_gpio43PinCfg[] =
{
    /* MyRCSS_GPIO1 -> RCSS_GPIO43_PIN -> W17 */
    {
        CSL_MSS_IOMUX_PADCZ_CFG_REG, PIN_MODE(0) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    {PINMUX_END}
};
static pinmuxModuleCfg_t gRcss_gpioPinCfg[] =
{
    { 50, TRUE, gTpr12_rcss_gpio50PinCfg},
    { 34, TRUE, gTpr12_rcss_gpio34PinCfg},
    { 48, TRUE, gTpr12_rcss_gpio48PinCfg},
    { 42, TRUE, gTpr12_rcss_gpio42PinCfg},
    { 51, TRUE, gTpr12_rcss_gpio51PinCfg},
    { 35, TRUE, gTpr12_rcss_gpio35PinCfg},
    { 49, TRUE, gTpr12_rcss_gpio49PinCfg},
    { 43, TRUE, gTpr12_rcss_gpio43PinCfg},
    {PINMUX_END}
};
       

pinmuxBoardCfg_t gTPR12PinmuxData[] =
{
    {0, gPmic_clkoutPinCfg},
    {1, gMss_rs232PinCfg},
    {2, gMss_i2cPinCfg},
    {3, gMss_rgmiiPinCfg},
    {4, gRcss_gpioPinCfg},
    {PINMUX_END}
};