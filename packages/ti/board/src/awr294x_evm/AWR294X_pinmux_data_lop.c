
/**
* Note: This file was auto-generated by TI PinMux on 4/30/2021 at 7:35:21 PM.
*
* \file  AWR294X_pinmux_data.c
*
* \brief  This file contains the pin mux configurations for the boards.
*         These are prepared based on how the peripherals are extended on
*         the boards.
*
* \copyright Copyright (CU) 2021 Texas Instruments Incorporated -
*             http://www.ti.com/
*/

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include "AWR294X_pinmux.h"

/** Peripheral Pin Configurations */


static pinmuxPerCfg_t gMyerror0PinCfg[] =
{
    /* MyERROR1 -> WARM_RESET -> A7 */
    {
        CSL_MSS_IOMUX_PADAS_CFG_REG, PIN_MODE(0) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyERROR1 -> NERROR_OUT -> A8 */
    {
        CSL_MSS_IOMUX_PADAT_CFG_REG, PIN_MODE(0) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    {PINMUX_END}
};
static pinmuxModuleCfg_t gErrorPinCfg[] =
{
    { 0, TRUE, gMyerror0PinCfg},
    {PINMUX_END}
};
       
static pinmuxPerCfg_t gMyjtag0PinCfg[] =
{
    /* MyJTAG1 -> TCK -> B13 */
    {
        CSL_MSS_IOMUX_PADAU_CFG_REG, PIN_MODE(1) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyJTAG1 -> TMS -> B14 */
    {
        CSL_MSS_IOMUX_PADAV_CFG_REG, PIN_MODE(1) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyJTAG1 -> TDI -> B15 */
    {
        CSL_MSS_IOMUX_PADAW_CFG_REG, PIN_MODE(1) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyJTAG1 -> TDO -> B16 */
    {
        CSL_MSS_IOMUX_PADAX_CFG_REG, PIN_MODE(1) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    {PINMUX_END}
};
static pinmuxModuleCfg_t gJtagPinCfg[] =
{
    { 0, TRUE, gMyjtag0PinCfg},
    {PINMUX_END}
};
       
static pinmuxPerCfg_t gMymcu_clkout0PinCfg[] =
{
    /* MyMCU_CLKOUT1 -> MCU_CLKOUT -> T18 */
    {
        CSL_MSS_IOMUX_PADAY_CFG_REG, PIN_MODE(1) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    {PINMUX_END}
};
static pinmuxModuleCfg_t gMcu_clkoutPinCfg[] =
{
    { 0, TRUE, gMymcu_clkout0PinCfg},
    {PINMUX_END}
};
       
static pinmuxPerCfg_t gMymss_epwm0PinCfg[] =
{
    /* MyMSS_EPWM1 -> MSS_EPWMA_0 -> B19 */
    {
        CSL_MSS_IOMUX_PADBP_CFG_REG, PIN_MODE(3) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyMSS_EPWM1 -> MSS_EPWMA_1 -> A18 */
    {
        CSL_MSS_IOMUX_PADBQ_CFG_REG, PIN_MODE(3) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    {PINMUX_END}
};
static pinmuxPerCfg_t gMymss_epwm1PinCfg[] =
{
    /* MyMSS_EPWM1 -> MSS_EPWMB_0 -> B18 */
    {
        CSL_MSS_IOMUX_PADBT_CFG_REG, PIN_MODE(4) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    {PINMUX_END}
};
static pinmuxModuleCfg_t gMss_epwmPinCfg[] =
{
    { 0, TRUE, gMymss_epwm0PinCfg},
    { 1, TRUE, gMymss_epwm1PinCfg},
    {PINMUX_END}
};
       
static pinmuxPerCfg_t gMymss_gpio0PinCfg[] =
{
    /* MyMSS_GPIO1 -> MSS_GPIO0 -> B10 */
    {
        CSL_MSS_IOMUX_PADDE_CFG_REG, PIN_MODE(0) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    {PINMUX_END}
};
static pinmuxPerCfg_t gMymss_gpio1PinCfg[] =
{
    /* MyMSS_GPIO1 -> MSS_GPIO16 -> A11 */
    {
        CSL_MSS_IOMUX_PADAC_CFG_REG, PIN_MODE(0) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    {PINMUX_END}
};

static pinmuxPerCfg_t gMymss_gpio2PinCfg[] =
{
    /* MyMSS_GPIO1 -> MSS_GPIO2 -> E19 */
    {
        CSL_MSS_IOMUX_PADAZ_CFG_REG, PIN_MODE(1) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    {PINMUX_END}
};
static pinmuxPerCfg_t gMymss_gpio3PinCfg[] =
{
    /* MyMSS_GPIO1 -> MSS_GPIO27 -> R19 */
    {
        CSL_MSS_IOMUX_PADBJ_CFG_REG, PIN_MODE(6) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    {PINMUX_END}
};
static pinmuxPerCfg_t gMymss_gpio4PinCfg[] =
{
    /* MyMSS_GPIO1 -> MSS_GPIO4 -> R20 */
    {
        CSL_MSS_IOMUX_PADBK_CFG_REG, PIN_MODE(1) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    {PINMUX_END}
};
static pinmuxPerCfg_t gMymss_gpio5PinCfg[] =
{
    /* MyMSS_GPIO1 -> MSS_GPIO1 -> T19 */
    {
        CSL_MSS_IOMUX_PADBH_CFG_REG, PIN_MODE(6) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    {PINMUX_END}
};
static pinmuxPerCfg_t gMymss_gpio6PinCfg[] =
{
    /* MyMSS_GPIO1 -> MSS_GPIO9 -> T20 */
    {
        CSL_MSS_IOMUX_PADBI_CFG_REG, PIN_MODE(7) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    {PINMUX_END}
};
static pinmuxPerCfg_t gMymss_gpio7PinCfg[] =
{
    /* MyMSS_GPIO1 -> MSS_GPIO5 -> P19 */
    {
        CSL_MSS_IOMUX_PADBL_CFG_REG, PIN_MODE(1) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    {PINMUX_END}
};
static pinmuxPerCfg_t gMymss_gpio8PinCfg[] =
{
    /* MyMSS_GPIO1 -> MSS_GPIO6 -> N19 */
    {
        CSL_MSS_IOMUX_PADBM_CFG_REG, PIN_MODE(1) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    {PINMUX_END}
};
static pinmuxPerCfg_t gMymss_gpio9PinCfg[] =
{
    /* MyMSS_GPIO1 -> MSS_GPIO13 -> B12 */
    {
        CSL_MSS_IOMUX_PADAB_CFG_REG, PIN_MODE(0) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    {PINMUX_END}
};
static pinmuxPerCfg_t gMymss_gpio10PinCfg[] =
{
    /* MyMSS_GPIO1 -> MSS_GPIO10 -> A10 */
    {
        CSL_MSS_IOMUX_PADDC_CFG_REG, PIN_MODE(3) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    {PINMUX_END}
};
static pinmuxPerCfg_t gMymss_gpio11PinCfg[] =
{
    /* MyMSS_GPIO1 -> MSS_GPIO11 -> B11 */
    {
        CSL_MSS_IOMUX_PADDD_CFG_REG, PIN_MODE(3) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    {PINMUX_END}
};
static pinmuxPerCfg_t gMymss_gpio12PinCfg[] =
{
    /* MyMSS_GPIO1 -> MSS_GPIO28 -> U20 */
    {
        CSL_MSS_IOMUX_PADBB_CFG_REG, PIN_MODE(0) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    {PINMUX_END}
};
static pinmuxPerCfg_t gMymss_gpio13PinCfg[] =
{
    /* MyMSS_GPIO1 -> MSS_GPIO29 -> W18 */
    {
        CSL_MSS_IOMUX_PADBC_CFG_REG, PIN_MODE(0) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    {PINMUX_END}
};
static pinmuxModuleCfg_t gMss_gpioPinCfg[] =
{
    { 0, TRUE, gMymss_gpio0PinCfg},
    { 1, TRUE, gMymss_gpio1PinCfg},
    { 2, TRUE, gMymss_gpio2PinCfg},
    { 3, TRUE, gMymss_gpio3PinCfg},
    { 4, TRUE, gMymss_gpio4PinCfg},
    { 5, TRUE, gMymss_gpio5PinCfg},
    { 6, TRUE, gMymss_gpio6PinCfg},
    { 7, TRUE, gMymss_gpio7PinCfg},
    { 8, TRUE, gMymss_gpio8PinCfg},
    { 9, TRUE, gMymss_gpio9PinCfg},
    { 10, TRUE, gMymss_gpio10PinCfg},
    { 11, TRUE, gMymss_gpio11PinCfg},
    { 12, TRUE, gMymss_gpio12PinCfg},
    { 13, TRUE, gMymss_gpio13PinCfg},
    {PINMUX_END}
};
       
static pinmuxPerCfg_t gMymss_i2c0PinCfg[] =
{
    /* MyMSS_I2C1 -> MSS_I2CA_SDA -> W5 */
    {
        CSL_MSS_IOMUX_PADBY_CFG_REG, PIN_MODE(3) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyMSS_I2C1 -> MSS_I2CA_SCL -> Y5 */
    {
        CSL_MSS_IOMUX_PADBZ_CFG_REG, PIN_MODE(3) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    {PINMUX_END}
};
static pinmuxModuleCfg_t gMss_i2cPinCfg[] =
{
    { 0, TRUE, gMymss_i2c0PinCfg},
    {PINMUX_END}
};
       
static pinmuxPerCfg_t gMymss_mcan0PinCfg[] =
{
    /* MyMSS_MCAN1 -> MSS_MCANA_RX -> C19 */
    {
        CSL_MSS_IOMUX_PADBO_CFG_REG, PIN_MODE(4) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyMSS_MCAN1 -> MSS_MCANA_TX -> D19 */
    {
        CSL_MSS_IOMUX_PADBN_CFG_REG, PIN_MODE(4) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    {PINMUX_END}
};
static pinmuxPerCfg_t gMymss_mcan1PinCfg[] =
{
    /* MyMSS_MCAN1 -> MSS_MCANB_RX -> B17 */
    {
        CSL_MSS_IOMUX_PADBS_CFG_REG, PIN_MODE(5) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyMSS_MCAN1 -> MSS_MCANB_TX -> A17 */
    {
        CSL_MSS_IOMUX_PADBR_CFG_REG, PIN_MODE(5) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    {PINMUX_END}
};
static pinmuxModuleCfg_t gMss_mcanPinCfg[] =
{
    { 0, TRUE, gMymss_mcan0PinCfg},
    { 1, TRUE, gMymss_mcan1PinCfg},
    {PINMUX_END}
};
       
static pinmuxPerCfg_t gMymss_mdio0PinCfg[] =
{
    /* MyMSS_MDIO1 -> MSS_MDIO_DATA -> V1 */
    {
        CSL_MSS_IOMUX_PADCM_CFG_REG, PIN_MODE(1) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyMSS_MDIO1 -> MSS_MDIO_CLK -> V2 */
    {
        CSL_MSS_IOMUX_PADCN_CFG_REG, PIN_MODE(1) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    {PINMUX_END}
};
static pinmuxModuleCfg_t gMss_mdioPinCfg[] =
{
    { 0, TRUE, gMymss_mdio0PinCfg},
    {PINMUX_END}
};
       
static pinmuxPerCfg_t gMymss_mibspi0PinCfg[] =
{
    /* MyMSS_MIBSPI1 -> MSS_MIBSPIA_HOSTIRQ -> W17 */
    {
        CSL_MSS_IOMUX_PADAA_CFG_REG, PIN_MODE(1) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyMSS_MIBSPI1 -> MSS_MIBSPIA_MOSI -> Y18 */
    {
        CSL_MSS_IOMUX_PADCO_CFG_REG, PIN_MODE(5) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyMSS_MIBSPI1 -> MSS_MIBSPIA_MISO -> Y19 */
    {
        CSL_MSS_IOMUX_PADCP_CFG_REG, PIN_MODE(5) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyMSS_MIBSPI1 -> MSS_MIBSPIA_CLK -> W19 */
    {
        CSL_MSS_IOMUX_PADCQ_CFG_REG, PIN_MODE(5) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyMSS_MIBSPI1 -> MSS_MIBSPIA_CS0 -> V18 */
    {
        CSL_MSS_IOMUX_PADCR_CFG_REG, PIN_MODE(5) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    {PINMUX_END}
};
static pinmuxPerCfg_t gMymss_mibspi1PinCfg[] =
{
    /* MyMSS_MIBSPI1 -> MSS_MIBSPIB_MOSI -> Y15 */
    {
        CSL_MSS_IOMUX_PADAH_CFG_REG, PIN_MODE(1) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyMSS_MIBSPI1 -> MSS_MIBSPIB_MISO -> W15 */
    {
        CSL_MSS_IOMUX_PADAI_CFG_REG, PIN_MODE(1) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyMSS_MIBSPI1 -> MSS_MIBSPIB_CLK -> W16 */
    {
        CSL_MSS_IOMUX_PADAJ_CFG_REG, PIN_MODE(1) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyMSS_MIBSPI1 -> MSS_MIBSPIB_CS0 -> Y17 */
    {
        CSL_MSS_IOMUX_PADAK_CFG_REG, PIN_MODE(1) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    {PINMUX_END}
};
static pinmuxModuleCfg_t gMss_mibspiPinCfg[] =
{
    { 0, TRUE, gMymss_mibspi0PinCfg},
    { 1, TRUE, gMymss_mibspi1PinCfg},
    {PINMUX_END}
};
       
static pinmuxPerCfg_t gMymss_qspi0PinCfg[] =
{
    /* MyMSS_QSPI1 -> MSS_QSPI_0 -> W7 */
    {
        CSL_MSS_IOMUX_PADAL_CFG_REG, PIN_MODE(1) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyMSS_QSPI1 -> MSS_QSPI_1 -> W13 */
    {
        CSL_MSS_IOMUX_PADAM_CFG_REG, PIN_MODE(1) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyMSS_QSPI1 -> MSS_QSPI_2 -> W14 */
    {
        CSL_MSS_IOMUX_PADAN_CFG_REG, PIN_MODE(1) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyMSS_QSPI1 -> MSS_QSPI_3 -> Y10 */
    {
        CSL_MSS_IOMUX_PADAO_CFG_REG, PIN_MODE(1) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyMSS_QSPI1 -> MSS_QSPI_CLK -> W8 */
    {
        CSL_MSS_IOMUX_PADAP_CFG_REG, PIN_MODE(1) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyMSS_QSPI1 -> MSS_QSPI_CS -> Y11 */
    {
        CSL_MSS_IOMUX_PADAQ_CFG_REG, PIN_MODE(1) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    {PINMUX_END}
};
static pinmuxModuleCfg_t gMss_qspiPinCfg[] =
{
    { 0, TRUE, gMymss_qspi0PinCfg},
    {PINMUX_END}
};
       
static pinmuxPerCfg_t gMymss_rgmii0PinCfg[] =
{
    /* MyMSS_RGMII1 -> MSS_RGMII_TCTL -> W1 */
    {
        CSL_MSS_IOMUX_PADCA_CFG_REG, PIN_MODE(3) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyMSS_RGMII1 -> MSS_RGMII_RCTL -> V8 */
    {
        CSL_MSS_IOMUX_PADCB_CFG_REG, PIN_MODE(3) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyMSS_RGMII1 -> MSS_RGMII_TD3 -> V7 */
    {
        CSL_MSS_IOMUX_PADCC_CFG_REG, PIN_MODE(3) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyMSS_RGMII1 -> MSS_RGMII_TD2 -> V5 */
    {
        CSL_MSS_IOMUX_PADCD_CFG_REG, PIN_MODE(3) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyMSS_RGMII1 -> MSS_RGMII_TD1 -> Y2 */
    {
        CSL_MSS_IOMUX_PADCE_CFG_REG, PIN_MODE(3) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyMSS_RGMII1 -> MSS_RGMII_TD0 -> W2 */
    {
        CSL_MSS_IOMUX_PADCF_CFG_REG, PIN_MODE(3) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyMSS_RGMII1 -> MSS_RGMII_TCLK -> V4 */
    {
        CSL_MSS_IOMUX_PADCG_CFG_REG, PIN_MODE(3) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyMSS_RGMII1 -> MSS_RGMII_RCLK -> W3 */
    {
        CSL_MSS_IOMUX_PADCH_CFG_REG, PIN_MODE(3) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyMSS_RGMII1 -> MSS_RGMII_RD3 -> V3 */
    {
        CSL_MSS_IOMUX_PADCI_CFG_REG, PIN_MODE(3) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyMSS_RGMII1 -> MSS_RGMII_RD2 -> V6 */
    {
        CSL_MSS_IOMUX_PADCJ_CFG_REG, PIN_MODE(3) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyMSS_RGMII1 -> MSS_RGMII_RD1 -> W6 */
    {
        CSL_MSS_IOMUX_PADCK_CFG_REG, PIN_MODE(3) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyMSS_RGMII1 -> MSS_RGMII_RD0 -> Y3 */
    {
        CSL_MSS_IOMUX_PADCL_CFG_REG, PIN_MODE(3) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    {PINMUX_END}
};
static pinmuxModuleCfg_t gMss_rgmiiPinCfg[] =
{
    { 0, TRUE, gMymss_rgmii0PinCfg},
    {PINMUX_END}
};
       
static pinmuxPerCfg_t gMymss_rs2320PinCfg[] =
{
    /* MyMSS_RS2321 -> MSS_RS232_RX -> C20 */
    {
        CSL_MSS_IOMUX_PADBD_CFG_REG, PIN_MODE(1) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyMSS_RS2321 -> MSS_RS232_TX -> B20 */
    {
        CSL_MSS_IOMUX_PADBE_CFG_REG, PIN_MODE(1) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    {PINMUX_END}
};
static pinmuxModuleCfg_t gMss_rs232PinCfg[] =
{
    { 0, TRUE, gMymss_rs2320PinCfg},
    {PINMUX_END}
};
       
static pinmuxPerCfg_t gMymss_uart0PinCfg[] =
{
    /* MyMSS_UART1 -> MSS_UARTA_RX -> A13 */
    {
        CSL_MSS_IOMUX_PADDA_CFG_REG, PIN_MODE(5) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyMSS_UART1 -> MSS_UARTA_TX -> A12 */
    {
        CSL_MSS_IOMUX_PADDB_CFG_REG, PIN_MODE(5) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    {PINMUX_END}
};
static pinmuxModuleCfg_t gMss_uartPinCfg[] =
{
    { 0, TRUE, gMymss_uart0PinCfg},
    {PINMUX_END}
};
       
static pinmuxPerCfg_t gMypmic_clkout0PinCfg[] =
{
    /* MyPMIC_CLKOUT1 -> PMIC_CLKOUT -> U19 */
    {
        CSL_MSS_IOMUX_PADBA_CFG_REG, PIN_MODE(1) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    {PINMUX_END}
};
static pinmuxModuleCfg_t gPmic_clkoutPinCfg[] =
{
    { 0, TRUE, gMypmic_clkout0PinCfg},
    {PINMUX_END}
};
       
static pinmuxPerCfg_t gMyxref_clk00PinCfg[] =
{
    /* MyXREF_CLK01 -> XREF_CLK0 -> B9 */
    {
        CSL_MSS_IOMUX_PADDF_CFG_REG, PIN_MODE(1) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    {PINMUX_END}
};
static pinmuxModuleCfg_t gXref_clk0PinCfg[] =
{
    { 0, TRUE, gMyxref_clk00PinCfg},
    {PINMUX_END}
};
       
static pinmuxPerCfg_t gMyxref_clk10PinCfg[] =
{
    /* MyXREF_CLK11 -> XREF_CLK1 -> A9 */
    {
        CSL_MSS_IOMUX_PADDG_CFG_REG, PIN_MODE(1) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION))
    },
    {PINMUX_END}
};
static pinmuxModuleCfg_t gXref_clk1PinCfg[] =
{
    { 0, TRUE, gMyxref_clk10PinCfg},
    {PINMUX_END}
};
       

pinmuxBoardCfg_t gAWR294XPinmuxData[] =
{
    {0, gErrorPinCfg},
    {1, gJtagPinCfg},
    {2, gMcu_clkoutPinCfg},
    {3, gMss_epwmPinCfg},
    {4, gMss_gpioPinCfg},
    {5, gMss_i2cPinCfg},
    {6, gMss_mcanPinCfg},
    {7, gMss_mdioPinCfg},
    {8, gMss_mibspiPinCfg},
    {9, gMss_qspiPinCfg},
    {10, gMss_rgmiiPinCfg},
    {11, gMss_rs232PinCfg},
    {12, gMss_uartPinCfg},
    {13, gPmic_clkoutPinCfg},
    {14, gXref_clk0PinCfg},
    {15, gXref_clk1PinCfg},
    {PINMUX_END}
};
