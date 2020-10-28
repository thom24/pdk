/**
* Note: This file was auto-generated by TI PinMux on 10/23/2020 at 6:39:51 PM.
*
* \file  AM64x_pinmux_data.c
*
* \brief  This file contains the pin mux configurations for the boards.
*         These are prepared based on how the peripherals are extended on
*         the boards.
*
* \copyright Copyright (CU) 2020 Texas Instruments Incorporated -
*             http://www.ti.com/
*/

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include "AM64x_pinmux.h"

/** Peripheral Pin Configurations */

static pinmuxPerCfg_t gAdc0PinCfg[] =
{
    /* MyADC1 -> ADC0_AIN0 -> G20 */
    {
        PIN_ADC0_AIN0, PIN_MODE(0) | \
        ((PIN_PULL_DISABLE | PIN_INPUT_ENABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyADC1 -> ADC0_AIN1 -> F20 */
    {
        PIN_ADC0_AIN1, PIN_MODE(0) | \
        ((PIN_PULL_DISABLE | PIN_INPUT_ENABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyADC1 -> ADC0_AIN2 -> E21 */
    {
        PIN_ADC0_AIN2, PIN_MODE(0) | \
        ((PIN_PULL_DISABLE | PIN_INPUT_ENABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyADC1 -> ADC0_AIN3 -> D20 */
    {
        PIN_ADC0_AIN3, PIN_MODE(0) | \
        ((PIN_PULL_DISABLE | PIN_INPUT_ENABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyADC1 -> ADC0_AIN4 -> G21 */
    {
        PIN_ADC0_AIN4, PIN_MODE(0) | \
        ((PIN_PULL_DISABLE | PIN_INPUT_ENABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyADC1 -> ADC0_AIN5 -> F21 */
    {
        PIN_ADC0_AIN5, PIN_MODE(0) | \
        ((PIN_PULL_DISABLE | PIN_INPUT_ENABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyADC1 -> ADC0_AIN6 -> F19 */
    {
        PIN_ADC0_AIN6, PIN_MODE(0) | \
        ((PIN_PULL_DISABLE | PIN_INPUT_ENABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyADC1 -> ADC0_AIN7 -> E20 */
    {
        PIN_ADC0_AIN7, PIN_MODE(0) | \
        ((PIN_PULL_DISABLE | PIN_INPUT_ENABLE) & (~PIN_PULL_DIRECTION))
    },
    {PINMUX_END}
};

static pinmuxModuleCfg_t gAdcPinCfg[] =
{
    {0, TRUE, gAdc0PinCfg},
    {PINMUX_END}
};


static pinmuxPerCfg_t gFsi_rx0PinCfg[] =
{
    /* MyFSI_RX1 -> FSI_RX0_CLK -> V19 */
    {
        PIN_GPMC0_AD8, PIN_MODE(1) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION & ~PIN_INPUT_ENABLE))
    },
    /* MyFSI_RX1 -> FSI_RX0_D0 -> T17 */
    {
        PIN_GPMC0_AD9, PIN_MODE(1) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION & ~PIN_INPUT_ENABLE))
    },
    /* MyFSI_RX1 -> FSI_RX0_D1 -> R16 */
    {
        PIN_GPMC0_AD10, PIN_MODE(1) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION & ~PIN_INPUT_ENABLE))
    },
    {PINMUX_END}
};

static pinmuxModuleCfg_t gFsi_rxPinCfg[] =
{
    {0, TRUE, gFsi_rx0PinCfg},
    {PINMUX_END}
};


static pinmuxPerCfg_t gFsi_tx0PinCfg[] =
{
    /* MyFSI_TX1 -> FSI_TX0_CLK -> T19 */
    {
        PIN_GPMC0_BE1N, PIN_MODE(1) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION & ~PIN_INPUT_ENABLE))
    },
    /* MyFSI_TX1 -> FSI_TX0_D0 -> Y21 */
    {
        PIN_GPMC0_AD14, PIN_MODE(1) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION & ~PIN_INPUT_ENABLE))
    },
    /* MyFSI_TX1 -> FSI_TX0_D1 -> Y20 */
    {
        PIN_GPMC0_AD15, PIN_MODE(1) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION & ~PIN_INPUT_ENABLE))
    },
    {PINMUX_END}
};

static pinmuxModuleCfg_t gFsi_txPinCfg[] =
{
    {0, TRUE, gFsi_tx0PinCfg},
    {PINMUX_END}
};


static pinmuxPerCfg_t gGpio0PinCfg[] =
{
    /* MyGPIO1 -> GPIO0_12 -> L18 */
    {
        PIN_OSPI0_CSN1, PIN_MODE(7) | \
        ((PIN_PULL_DISABLE | PIN_INPUT_ENABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyGPIO1 -> GPIO0_13 -> K17 */
    {
        PIN_OSPI0_CSN2, PIN_MODE(7) | \
        ((PIN_PULL_DISABLE | PIN_INPUT_ENABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyGPIO1 -> GPIO0_14 -> L17 */
    {
        PIN_OSPI0_CSN3, PIN_MODE(7) | \
        ((PIN_PULL_DISABLE | PIN_INPUT_ENABLE) & (~PIN_PULL_DIRECTION))
    },
    {PINMUX_END}
};

static pinmuxPerCfg_t gGpio1PinCfg[] =
{
    /* MyGPIO2 -> GPIO1_43 -> C13 */
    {
        PIN_SPI0_CS1, PIN_MODE(7) | \
        ((PIN_PULL_DISABLE | PIN_INPUT_ENABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyGPIO2 -> GPIO1_70 -> C19 */
    {
        PIN_EXTINTN, PIN_MODE(7) | \
        ((PIN_PULL_DISABLE | PIN_INPUT_ENABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyGPIO2 -> GPIO1_78 -> C20 */
    {
        PIN_MMC1_SDWP, PIN_MODE(7) | \
        ((PIN_PULL_DISABLE | PIN_INPUT_ENABLE) & (~PIN_PULL_DIRECTION))
    },
    {PINMUX_END}
};

static pinmuxModuleCfg_t gGpioPinCfg[] =
{
    {0, TRUE, gGpio0PinCfg},
    {1, TRUE, gGpio1PinCfg},
    {PINMUX_END}
};


static pinmuxPerCfg_t gI2c0PinCfg[] =
{
    /* MyI2C1 -> I2C0_SCL -> A18 */
    {
        PIN_I2C0_SCL, PIN_MODE(0) | \
        ((PIN_PULL_DIRECTION | PIN_INPUT_ENABLE) & (~PIN_PULL_DISABLE))
    },
    /* MyI2C1 -> I2C0_SDA -> B18 */
    {
        PIN_I2C0_SDA, PIN_MODE(0) | \
        ((PIN_PULL_DIRECTION | PIN_INPUT_ENABLE) & (~PIN_PULL_DISABLE))
    },
    {PINMUX_END}
};

static pinmuxPerCfg_t gI2c1PinCfg[] =
{
    /* MyI2C2 -> I2C1_SCL -> C18 */
    {
        PIN_I2C1_SCL, PIN_MODE(0) | \
        ((PIN_PULL_DIRECTION | PIN_INPUT_ENABLE) & (~PIN_PULL_DISABLE))
    },
    /* MyI2C2 -> I2C1_SDA -> B19 */
    {
        PIN_I2C1_SDA, PIN_MODE(0) | \
        ((PIN_PULL_DIRECTION | PIN_INPUT_ENABLE) & (~PIN_PULL_DISABLE))
    },
    {PINMUX_END}
};

static pinmuxModuleCfg_t gI2cPinCfg[] =
{
    {0, TRUE, gI2c0PinCfg},
    {1, TRUE, gI2c1PinCfg},
    {PINMUX_END}
};


static pinmuxPerCfg_t gMcan0PinCfg[] =
{
    /* MyMCAN1 -> MCAN0_RX -> B17 */
    {
        PIN_MCAN0_RX, PIN_MODE(0) | \
        ((PIN_PULL_DISABLE | PIN_INPUT_ENABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyMCAN1 -> MCAN0_TX -> A17 */
    {
        PIN_MCAN0_TX, PIN_MODE(0) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION & ~PIN_INPUT_ENABLE))
    },
    {PINMUX_END}
};

static pinmuxPerCfg_t gMcan1PinCfg[] =
{
    /* MyMCAN2 -> MCAN1_RX -> D17 */
    {
        PIN_MCAN1_RX, PIN_MODE(0) | \
        ((PIN_PULL_DISABLE | PIN_INPUT_ENABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyMCAN2 -> MCAN1_TX -> C17 */
    {
        PIN_MCAN1_TX, PIN_MODE(0) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION & ~PIN_INPUT_ENABLE))
    },
    {PINMUX_END}
};

static pinmuxModuleCfg_t gMcanPinCfg[] =
{
    {0, TRUE, gMcan0PinCfg},
    {1, TRUE, gMcan1PinCfg},
    {PINMUX_END}
};


static pinmuxPerCfg_t gMcu_gpio0PinCfg[] =
{
    /* MyMCU_GPIO1 -> MCU_GPIO0_5 -> A7 */
    {
        PIN_MCU_SPI1_CS0, PIN_MODE(7) | \
        ((PIN_PULL_DISABLE | PIN_INPUT_ENABLE) & (~PIN_PULL_DIRECTION))
    },
    {PINMUX_END}
};

static pinmuxModuleCfg_t gMcu_gpioPinCfg[] =
{
    {0, TRUE, gMcu_gpio0PinCfg},
    {PINMUX_END}
};


static pinmuxPerCfg_t gMcu_i2c0PinCfg[] =
{
    /* MyMCU_I2C1 -> MCU_I2C0_SCL -> E9 */
    {
        PIN_MCU_I2C0_SCL, PIN_MODE(0) | \
        ((PIN_PULL_DIRECTION | PIN_INPUT_ENABLE) & (~PIN_PULL_DISABLE))
    },
    /* MyMCU_I2C1 -> MCU_I2C0_SDA -> A10 */
    {
        PIN_MCU_I2C0_SDA, PIN_MODE(0) | \
        ((PIN_PULL_DIRECTION | PIN_INPUT_ENABLE) & (~PIN_PULL_DISABLE))
    },
    {PINMUX_END}
};

static pinmuxPerCfg_t gMcu_i2c1PinCfg[] =
{
    /* MyMCU_I2C2 -> MCU_I2C1_SCL -> A11 */
    {
        PIN_MCU_I2C1_SCL, PIN_MODE(0) | \
        ((PIN_PULL_DIRECTION | PIN_INPUT_ENABLE) & (~PIN_PULL_DISABLE))
    },
    /* MyMCU_I2C2 -> MCU_I2C1_SDA -> B10 */
    {
        PIN_MCU_I2C1_SDA, PIN_MODE(0) | \
        ((PIN_PULL_DIRECTION | PIN_INPUT_ENABLE) & (~PIN_PULL_DISABLE))
    },
    {PINMUX_END}
};

static pinmuxModuleCfg_t gMcu_i2cPinCfg[] =
{
    {0, TRUE, gMcu_i2c0PinCfg},
    {1, TRUE, gMcu_i2c1PinCfg},
    {PINMUX_END}
};


static pinmuxPerCfg_t gMcu_system0PinCfg[] =
{
    /* MyMCU_SYSTEM1 -> MCU_PORz -> B21 */
    {
        PIN_MCU_PORZ, PIN_MODE(0) | \
        ((PIN_PULL_DISABLE | PIN_INPUT_ENABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyMCU_SYSTEM1 -> MCU_RESETSTATz -> B13 */
    {
        PIN_MCU_RESETSTATZ, PIN_MODE(0) | \
        ((PIN_PULL_DISABLE | PIN_INPUT_ENABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyMCU_SYSTEM1 -> MCU_RESETz -> B12 */
    {
        PIN_MCU_RESETZ, PIN_MODE(0) | \
        ((PIN_PULL_DISABLE | PIN_INPUT_ENABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyMCU_SYSTEM1 -> MCU_SAFETY_ERRORn -> A20 */
    {
        PIN_MCU_SAFETY_ERRORN, PIN_MODE(0) | \
        ((PIN_PULL_DISABLE | PIN_INPUT_ENABLE) & (~PIN_PULL_DIRECTION))
    },
    {PINMUX_END}
};

static pinmuxModuleCfg_t gMcu_systemPinCfg[] =
{
    {0, TRUE, gMcu_system0PinCfg},
    {PINMUX_END}
};


static pinmuxPerCfg_t gMcu_uart0PinCfg[] =
{
    /* MyMCU_UART1 -> MCU_UART0_CTSn -> D8 */
    {
        PIN_MCU_UART0_CTSN, PIN_MODE(0) | \
        ((PIN_PULL_DISABLE | PIN_INPUT_ENABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyMCU_UART1 -> MCU_UART0_RTSn -> E8 */
    {
        PIN_MCU_UART0_RTSN, PIN_MODE(0) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION & ~PIN_INPUT_ENABLE))
    },
    /* MyMCU_UART1 -> MCU_UART0_RXD -> A9 */
    {
        PIN_MCU_UART0_RXD, PIN_MODE(0) | \
        ((PIN_PULL_DISABLE | PIN_INPUT_ENABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyMCU_UART1 -> MCU_UART0_TXD -> A8 */
    {
        PIN_MCU_UART0_TXD, PIN_MODE(0) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION & ~PIN_INPUT_ENABLE))
    },
    {PINMUX_END}
};

static pinmuxPerCfg_t gMcu_uart1PinCfg[] =
{
    /* MyMCU_UART2 -> MCU_UART1_CTSn -> B8 */
    {
        PIN_MCU_UART1_CTSN, PIN_MODE(0) | \
        ((PIN_PULL_DISABLE | PIN_INPUT_ENABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyMCU_UART2 -> MCU_UART1_RTSn -> B9 */
    {
        PIN_MCU_UART1_RTSN, PIN_MODE(0) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION & ~PIN_INPUT_ENABLE))
    },
    /* MyMCU_UART2 -> MCU_UART1_RXD -> C9 */
    {
        PIN_MCU_UART1_RXD, PIN_MODE(0) | \
        ((PIN_PULL_DISABLE | PIN_INPUT_ENABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyMCU_UART2 -> MCU_UART1_TXD -> D9 */
    {
        PIN_MCU_UART1_TXD, PIN_MODE(0) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION & ~PIN_INPUT_ENABLE))
    },
    {PINMUX_END}
};

static pinmuxModuleCfg_t gMcu_uartPinCfg[] =
{
    {0, TRUE, gMcu_uart0PinCfg},
    {1, TRUE, gMcu_uart1PinCfg},
    {PINMUX_END}
};


static pinmuxPerCfg_t gMdio0PinCfg[] =
{
    /* MyMDIO1 -> MDIO0_MDC -> R2 */
    {
        PIN_PRG0_PRU1_GPO19, PIN_MODE(4) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION & ~PIN_INPUT_ENABLE))
    },
    /* MyMDIO1 -> MDIO0_MDIO -> P5 */
    {
        PIN_PRG0_PRU1_GPO18, PIN_MODE(4) | \
        ((PIN_PULL_DISABLE | PIN_INPUT_ENABLE) & (~PIN_PULL_DIRECTION))
    },
    {PINMUX_END}
};

static pinmuxModuleCfg_t gMdioPinCfg[] =
{
    {0, TRUE, gMdio0PinCfg},
    {PINMUX_END}
};


static pinmuxPerCfg_t gMmc11PinCfg[] =
{
    /* MyMMC11 -> MMC1_CMD -> J19 */
    {
        PIN_MMC1_CMD, PIN_MODE(0) | \
        ((PIN_PULL_DISABLE | PIN_INPUT_ENABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyMMC11 -> MMC1_CLK -> L20 */
    {
        PIN_MMC1_CLK, PIN_MODE(0) | \
        ((PIN_PULL_DISABLE | PIN_INPUT_ENABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyMMC11 -> MMC1_DAT0 -> K21 */
    {
        PIN_MMC1_DAT0, PIN_MODE(0) | \
        ((PIN_PULL_DISABLE | PIN_INPUT_ENABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyMMC11 -> MMC1_DAT1 -> L21 */
    {
        PIN_MMC1_DAT1, PIN_MODE(0) | \
        ((PIN_PULL_DISABLE | PIN_INPUT_ENABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyMMC11 -> MMC1_DAT2 -> K19 */
    {
        PIN_MMC1_DAT2, PIN_MODE(0) | \
        ((PIN_PULL_DISABLE | PIN_INPUT_ENABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyMMC11 -> MMC1_DAT3 -> K18 */
    {
        PIN_MMC1_DAT3, PIN_MODE(0) | \
        ((PIN_PULL_DISABLE | PIN_INPUT_ENABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyMMC11 -> MMC1_SDCD -> D19 */
    {
        PIN_MMC1_SDCD, PIN_MODE(0) | \
        ((PIN_PULL_DISABLE | PIN_INPUT_ENABLE) & (~PIN_PULL_DIRECTION))
    },
    {PINMUX_END}
};

static pinmuxModuleCfg_t gMmc1PinCfg[] =
{
    {1, TRUE, gMmc11PinCfg},
    {PINMUX_END}
};


static pinmuxPerCfg_t gOspi0PinCfg[] =
{
    /* MyOSPI1 -> OSPI0_CLK -> N20 */
    {
        PIN_OSPI0_CLK, PIN_MODE(0) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION & ~PIN_INPUT_ENABLE))
    },
    /* MyOSPI1 -> OSPI0_CSn0 -> L19 */
    {
        PIN_OSPI0_CSN0, PIN_MODE(0) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION & ~PIN_INPUT_ENABLE))
    },
    /* MyOSPI1 -> OSPI0_D0 -> M19 */
    {
        PIN_OSPI0_D0, PIN_MODE(0) | \
        ((PIN_PULL_DISABLE | PIN_INPUT_ENABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyOSPI1 -> OSPI0_D1 -> M18 */
    {
        PIN_OSPI0_D1, PIN_MODE(0) | \
        ((PIN_PULL_DISABLE | PIN_INPUT_ENABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyOSPI1 -> OSPI0_D2 -> M20 */
    {
        PIN_OSPI0_D2, PIN_MODE(0) | \
        ((PIN_PULL_DISABLE | PIN_INPUT_ENABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyOSPI1 -> OSPI0_D3 -> M21 */
    {
        PIN_OSPI0_D3, PIN_MODE(0) | \
        ((PIN_PULL_DISABLE | PIN_INPUT_ENABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyOSPI1 -> OSPI0_D4 -> P21 */
    {
        PIN_OSPI0_D4, PIN_MODE(0) | \
        ((PIN_PULL_DISABLE | PIN_INPUT_ENABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyOSPI1 -> OSPI0_D5 -> P20 */
    {
        PIN_OSPI0_D5, PIN_MODE(0) | \
        ((PIN_PULL_DISABLE | PIN_INPUT_ENABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyOSPI1 -> OSPI0_D6 -> N18 */
    {
        PIN_OSPI0_D6, PIN_MODE(0) | \
        ((PIN_PULL_DISABLE | PIN_INPUT_ENABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyOSPI1 -> OSPI0_D7 -> M17 */
    {
        PIN_OSPI0_D7, PIN_MODE(0) | \
        ((PIN_PULL_DISABLE | PIN_INPUT_ENABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyOSPI1 -> OSPI0_DQS -> N19 */
    {
        PIN_OSPI0_DQS, PIN_MODE(0) | \
        ((PIN_PULL_DISABLE | PIN_INPUT_ENABLE) & (~PIN_PULL_DIRECTION))
    },
    {PINMUX_END}
};

static pinmuxModuleCfg_t gOspiPinCfg[] =
{
    {0, TRUE, gOspi0PinCfg},
    {PINMUX_END}
};


static pinmuxPerCfg_t gPru_icssg0_mdio0PinCfg[] =
{
    /* MyPRU_ICSSG0_MDIO1 -> PRG0_MDIO0_MDC -> P3 */
    {
        PIN_PRG0_MDIO0_MDC, PIN_MODE(0) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION & ~PIN_INPUT_ENABLE))
    },
    /* MyPRU_ICSSG0_MDIO1 -> PRG0_MDIO0_MDIO -> P2 */
    {
        PIN_PRG0_MDIO0_MDIO, PIN_MODE(0) | \
        ((PIN_PULL_DISABLE | PIN_INPUT_ENABLE) & (~PIN_PULL_DIRECTION))
    },
    {PINMUX_END}
};

static pinmuxModuleCfg_t gPru_icssg0_mdioPinCfg[] =
{
    {0, TRUE, gPru_icssg0_mdio0PinCfg},
    {PINMUX_END}
};


static pinmuxPerCfg_t gPru_icssg0_pru0PinCfg[] =
{
    /* MyPRU_ICSSG0_PRU1 -> PRG0_PRU0_GPO8 -> T2 */
    {
        PIN_PRG0_PRU0_GPO8, PIN_MODE(0) | \
        ((PIN_PULL_DISABLE | PIN_INPUT_ENABLE) & (~PIN_PULL_DIRECTION))
    },
    {PINMUX_END}
};

static pinmuxPerCfg_t gPru_icssg0_pru1PinCfg[] =
{
    /* MyPRU_ICSSG0_PRU2 -> PRG0_PRU1_GPO8 -> R1 */
    {
        PIN_PRG0_PRU1_GPO8, PIN_MODE(0) | \
        ((PIN_PULL_DISABLE | PIN_INPUT_ENABLE) & (~PIN_PULL_DIRECTION))
    },
    {PINMUX_END}
};

static pinmuxModuleCfg_t gPru_icssg0_pruPinCfg[] =
{
    {0, TRUE, gPru_icssg0_pru0PinCfg},
    {1, TRUE, gPru_icssg0_pru1PinCfg},
    {PINMUX_END}
};


static pinmuxPerCfg_t gPru_icssg1_mdio0PinCfg[] =
{
    /* MyPRU_ICSSG1_MDIO1 -> PRG1_MDIO0_MDC -> Y6 */
    {
        PIN_PRG1_MDIO0_MDC, PIN_MODE(0) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION & ~PIN_INPUT_ENABLE))
    },
    /* MyPRU_ICSSG1_MDIO1 -> PRG1_MDIO0_MDIO -> AA6 */
    {
        PIN_PRG1_MDIO0_MDIO, PIN_MODE(0) | \
        ((PIN_PULL_DISABLE | PIN_INPUT_ENABLE) & (~PIN_PULL_DIRECTION))
    },
    {PINMUX_END}
};

static pinmuxModuleCfg_t gPru_icssg1_mdioPinCfg[] =
{
    {0, TRUE, gPru_icssg1_mdio0PinCfg},
    {PINMUX_END}
};


static pinmuxPerCfg_t gPru_icssg1_rgmii1PinCfg[] =
{
    /* MyPRU_ICSSG1_RGMII1 -> PRG1_RGMII1_RD0 -> Y7 */
    {
        PIN_PRG1_PRU0_GPO0, PIN_MODE(2) | \
        ((PIN_PULL_DISABLE | PIN_INPUT_ENABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyPRU_ICSSG1_RGMII1 -> PRG1_RGMII1_RD1 -> U8 */
    {
        PIN_PRG1_PRU0_GPO1, PIN_MODE(2) | \
        ((PIN_PULL_DISABLE | PIN_INPUT_ENABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyPRU_ICSSG1_RGMII1 -> PRG1_RGMII1_RD2 -> W8 */
    {
        PIN_PRG1_PRU0_GPO2, PIN_MODE(2) | \
        ((PIN_PULL_DISABLE | PIN_INPUT_ENABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyPRU_ICSSG1_RGMII1 -> PRG1_RGMII1_RD3 -> V8 */
    {
        PIN_PRG1_PRU0_GPO3, PIN_MODE(2) | \
        ((PIN_PULL_DISABLE | PIN_INPUT_ENABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyPRU_ICSSG1_RGMII1 -> PRG1_RGMII1_RXC -> AA7 */
    {
        PIN_PRG1_PRU0_GPO6, PIN_MODE(2) | \
        ((PIN_PULL_DISABLE | PIN_INPUT_ENABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyPRU_ICSSG1_RGMII1 -> PRG1_RGMII1_RX_CTL -> Y8 */
    {
        PIN_PRG1_PRU0_GPO4, PIN_MODE(2) | \
        ((PIN_PULL_DISABLE | PIN_INPUT_ENABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyPRU_ICSSG1_RGMII1 -> PRG1_RGMII1_TD0 -> AA8 */
    {
        PIN_PRG1_PRU0_GPO11, PIN_MODE(2) | \
        ((PIN_PULL_DISABLE | PIN_INPUT_ENABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyPRU_ICSSG1_RGMII1 -> PRG1_RGMII1_TD1 -> U9 */
    {
        PIN_PRG1_PRU0_GPO12, PIN_MODE(2) | \
        ((PIN_PULL_DISABLE | PIN_INPUT_ENABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyPRU_ICSSG1_RGMII1 -> PRG1_RGMII1_TD2 -> W9 */
    {
        PIN_PRG1_PRU0_GPO13, PIN_MODE(2) | \
        ((PIN_PULL_DISABLE | PIN_INPUT_ENABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyPRU_ICSSG1_RGMII1 -> PRG1_RGMII1_TD3 -> AA9 */
    {
        PIN_PRG1_PRU0_GPO14, PIN_MODE(2) | \
        ((PIN_PULL_DISABLE | PIN_INPUT_ENABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyPRU_ICSSG1_RGMII1 -> PRG1_RGMII1_TXC -> V9 */
    {
        PIN_PRG1_PRU0_GPO16, PIN_MODE(2) | \
        ((PIN_PULL_DISABLE | PIN_INPUT_ENABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyPRU_ICSSG1_RGMII1 -> PRG1_RGMII1_TX_CTL -> Y9 */
    {
        PIN_PRG1_PRU0_GPO15, PIN_MODE(2) | \
        ((PIN_PULL_DISABLE | PIN_INPUT_ENABLE) & (~PIN_PULL_DIRECTION))
    },
    {PINMUX_END}
};

static pinmuxPerCfg_t gPru_icssg1_rgmii2PinCfg[] =
{
    /* MyPRU_ICSSG1_RGMII2 -> PRG1_RGMII2_RD0 -> W11 */
    {
        PIN_PRG1_PRU1_GPO0, PIN_MODE(2) | \
        ((PIN_PULL_DISABLE | PIN_INPUT_ENABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyPRU_ICSSG1_RGMII2 -> PRG1_RGMII2_RD1 -> V11 */
    {
        PIN_PRG1_PRU1_GPO1, PIN_MODE(2) | \
        ((PIN_PULL_DISABLE | PIN_INPUT_ENABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyPRU_ICSSG1_RGMII2 -> PRG1_RGMII2_RD2 -> AA12 */
    {
        PIN_PRG1_PRU1_GPO2, PIN_MODE(2) | \
        ((PIN_PULL_DISABLE | PIN_INPUT_ENABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyPRU_ICSSG1_RGMII2 -> PRG1_RGMII2_RD3 -> Y12 */
    {
        PIN_PRG1_PRU1_GPO3, PIN_MODE(2) | \
        ((PIN_PULL_DISABLE | PIN_INPUT_ENABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyPRU_ICSSG1_RGMII2 -> PRG1_RGMII2_RXC -> U11 */
    {
        PIN_PRG1_PRU1_GPO6, PIN_MODE(2) | \
        ((PIN_PULL_DISABLE | PIN_INPUT_ENABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyPRU_ICSSG1_RGMII2 -> PRG1_RGMII2_RX_CTL -> W12 */
    {
        PIN_PRG1_PRU1_GPO4, PIN_MODE(2) | \
        ((PIN_PULL_DISABLE | PIN_INPUT_ENABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyPRU_ICSSG1_RGMII2 -> PRG1_RGMII2_TD0 -> AA10 */
    {
        PIN_PRG1_PRU1_GPO11, PIN_MODE(2) | \
        ((PIN_PULL_DISABLE | PIN_INPUT_ENABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyPRU_ICSSG1_RGMII2 -> PRG1_RGMII2_TD1 -> V10 */
    {
        PIN_PRG1_PRU1_GPO12, PIN_MODE(2) | \
        ((PIN_PULL_DISABLE | PIN_INPUT_ENABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyPRU_ICSSG1_RGMII2 -> PRG1_RGMII2_TD2 -> U10 */
    {
        PIN_PRG1_PRU1_GPO13, PIN_MODE(2) | \
        ((PIN_PULL_DISABLE | PIN_INPUT_ENABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyPRU_ICSSG1_RGMII2 -> PRG1_RGMII2_TD3 -> AA11 */
    {
        PIN_PRG1_PRU1_GPO14, PIN_MODE(2) | \
        ((PIN_PULL_DISABLE | PIN_INPUT_ENABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyPRU_ICSSG1_RGMII2 -> PRG1_RGMII2_TXC -> Y10 */
    {
        PIN_PRG1_PRU1_GPO16, PIN_MODE(2) | \
        ((PIN_PULL_DISABLE | PIN_INPUT_ENABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyPRU_ICSSG1_RGMII2 -> PRG1_RGMII2_TX_CTL -> Y11 */
    {
        PIN_PRG1_PRU1_GPO15, PIN_MODE(2) | \
        ((PIN_PULL_DISABLE | PIN_INPUT_ENABLE) & (~PIN_PULL_DIRECTION))
    },
    {PINMUX_END}
};

static pinmuxModuleCfg_t gPru_icssg1_rgmiiPinCfg[] =
{
    {1, TRUE, gPru_icssg1_rgmii1PinCfg},
    {2, TRUE, gPru_icssg1_rgmii2PinCfg},
    {PINMUX_END}
};


static pinmuxPerCfg_t gRgmii1PinCfg[] =
{
    /* MyRGMII1 -> RGMII1_RD0 -> W5 */
    {
        PIN_PRG0_PRU1_GPO7, PIN_MODE(4) | \
        ((PIN_PULL_DISABLE | PIN_INPUT_ENABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyRGMII1 -> RGMII1_RD1 -> Y5 */
    {
        PIN_PRG0_PRU1_GPO9, PIN_MODE(4) | \
        ((PIN_PULL_DISABLE | PIN_INPUT_ENABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyRGMII1 -> RGMII1_RD2 -> V6 */
    {
        PIN_PRG0_PRU1_GPO10, PIN_MODE(4) | \
        ((PIN_PULL_DISABLE | PIN_INPUT_ENABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyRGMII1 -> RGMII1_RD3 -> V5 */
    {
        PIN_PRG0_PRU1_GPO17, PIN_MODE(4) | \
        ((PIN_PULL_DISABLE | PIN_INPUT_ENABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyRGMII1 -> RGMII1_RXC -> AA5 */
    {
        PIN_PRG0_PRU0_GPO10, PIN_MODE(4) | \
        ((PIN_PULL_DISABLE | PIN_INPUT_ENABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyRGMII1 -> RGMII1_RX_CTL -> W6 */
    {
        PIN_PRG0_PRU0_GPO9, PIN_MODE(4) | \
        ((PIN_PULL_DISABLE | PIN_INPUT_ENABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyRGMII1 -> RGMII1_TD0 -> V15 */
    {
        PIN_PRG1_PRU1_GPO7, PIN_MODE(4) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION & ~PIN_INPUT_ENABLE))
    },
    /* MyRGMII1 -> RGMII1_TD1 -> V14 */
    {
        PIN_PRG1_PRU1_GPO9, PIN_MODE(4) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION & ~PIN_INPUT_ENABLE))
    },
    /* MyRGMII1 -> RGMII1_TD2 -> W14 */
    {
        PIN_PRG1_PRU1_GPO10, PIN_MODE(4) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION & ~PIN_INPUT_ENABLE))
    },
    /* MyRGMII1 -> RGMII1_TD3 -> AA14 */
    {
        PIN_PRG1_PRU1_GPO17, PIN_MODE(4) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION & ~PIN_INPUT_ENABLE))
    },
    /* MyRGMII1 -> RGMII1_TXC -> U14 */
    {
        PIN_PRG1_PRU0_GPO10, PIN_MODE(4) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION & ~PIN_INPUT_ENABLE))
    },
    /* MyRGMII1 -> RGMII1_TX_CTL -> U15 */
    {
        PIN_PRG1_PRU0_GPO9, PIN_MODE(4) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION & ~PIN_INPUT_ENABLE))
    },
    {PINMUX_END}
};

static pinmuxModuleCfg_t gRgmiiPinCfg[] =
{
    {1, TRUE, gRgmii1PinCfg},
    {PINMUX_END}
};


static pinmuxPerCfg_t gSpi0PinCfg[] =
{
    /* MySPI1 -> SPI0_CLK -> D13 */
    {
        PIN_SPI0_CLK, PIN_MODE(0) | \
        ((PIN_PULL_DISABLE | PIN_INPUT_ENABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MySPI1 -> SPI0_CS0 -> D12 */
    {
        PIN_SPI0_CS0, PIN_MODE(0) | \
        ((PIN_PULL_DISABLE | PIN_INPUT_ENABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MySPI1 -> SPI0_D0 -> A13 */
    {
        PIN_SPI0_D0, PIN_MODE(0) | \
        ((PIN_PULL_DISABLE | PIN_INPUT_ENABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MySPI1 -> SPI0_D1 -> A14 */
    {
        PIN_SPI0_D1, PIN_MODE(0) | \
        ((PIN_PULL_DISABLE | PIN_INPUT_ENABLE) & (~PIN_PULL_DIRECTION))
    },
    {PINMUX_END}
};

static pinmuxModuleCfg_t gSpiPinCfg[] =
{
    {0, TRUE, gSpi0PinCfg},
    {PINMUX_END}
};


static pinmuxPerCfg_t gSystem0PinCfg[] =
{
    /* MySYSTEM1 -> EXT_REFCLK1 -> A19 */
    {
        PIN_EXT_REFCLK1, PIN_MODE(0) | \
        ((PIN_PULL_DISABLE | PIN_INPUT_ENABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MySYSTEM1 -> GPMC0_FCLK_MUX -> R17 */
    {
        PIN_GPMC0_CLK, PIN_MODE(4) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION & ~PIN_INPUT_ENABLE))
    },
    /* MySYSTEM1 -> PORz_OUT -> E17 */
    {
        PIN_PORZ_OUT, PIN_MODE(0) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION & ~PIN_INPUT_ENABLE))
    },
    /* MySYSTEM1 -> RESETSTATz -> F16 */
    {
        PIN_RESETSTATZ, PIN_MODE(0) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION & ~PIN_INPUT_ENABLE))
    },
    /* MySYSTEM1 -> SYNC0_OUT -> D18 */
    {
        PIN_ECAP0_IN_APWM_OUT, PIN_MODE(1) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION & ~PIN_INPUT_ENABLE))
    },
    {PINMUX_END}
};

static pinmuxModuleCfg_t gSystemPinCfg[] =
{
    {0, TRUE, gSystem0PinCfg},
    {PINMUX_END}
};


static pinmuxPerCfg_t gUart0PinCfg[] =
{
    /* MyUART1 -> UART0_CTSn -> B16 */
    {
        PIN_UART0_CTSN, PIN_MODE(0) | \
        ((PIN_PULL_DISABLE | PIN_INPUT_ENABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyUART1 -> UART0_RTSn -> A16 */
    {
        PIN_UART0_RTSN, PIN_MODE(0) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION & ~PIN_INPUT_ENABLE))
    },
    /* MyUART1 -> UART0_RXD -> D15 */
    {
        PIN_UART0_RXD, PIN_MODE(0) | \
        ((PIN_PULL_DISABLE | PIN_INPUT_ENABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyUART1 -> UART0_TXD -> C16 */
    {
        PIN_UART0_TXD, PIN_MODE(0) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION & ~PIN_INPUT_ENABLE))
    },
    {PINMUX_END}
};

static pinmuxPerCfg_t gUart1PinCfg[] =
{
    /* MyUART2 -> UART1_RXD -> E15 */
    {
        PIN_UART1_RXD, PIN_MODE(0) | \
        ((PIN_PULL_DISABLE | PIN_INPUT_ENABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyUART2 -> UART1_TXD -> E14 */
    {
        PIN_UART1_TXD, PIN_MODE(0) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION & ~PIN_INPUT_ENABLE))
    },
    {PINMUX_END}
};

static pinmuxPerCfg_t gUart3PinCfg[] =
{
    /* MyUART3 -> UART3_RXD -> D16 */
    {
        PIN_UART1_CTSN, PIN_MODE(4) | \
        ((PIN_PULL_DISABLE | PIN_INPUT_ENABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyUART3 -> UART3_TXD -> E16 */
    {
        PIN_UART1_RTSN, PIN_MODE(4) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION & ~PIN_INPUT_ENABLE))
    },
    {PINMUX_END}
};

static pinmuxModuleCfg_t gUartPinCfg[] =
{
    {0, TRUE, gUart0PinCfg},
    {1, TRUE, gUart1PinCfg},
    {3, TRUE, gUart3PinCfg},
    {PINMUX_END}
};


pinmuxBoardCfg_t gAM64x_MainPinmuxData[] =
{
    {0, gAdcPinCfg},
    {1, gFsi_rxPinCfg},
    {2, gFsi_txPinCfg},
    {3, gGpioPinCfg},
    {4, gI2cPinCfg},
    {5, gMcanPinCfg},
    {6, gMdioPinCfg},
    {7, gMmc1PinCfg},
    {8, gOspiPinCfg},
    {9, gPru_icssg0_mdioPinCfg},
    {10, gPru_icssg0_pruPinCfg},
    {11, gPru_icssg1_mdioPinCfg},
    {12, gPru_icssg1_rgmiiPinCfg},
    {13, gRgmiiPinCfg},
    {14, gSpiPinCfg},
    {15, gSystemPinCfg},
    {16, gUartPinCfg},
    {PINMUX_END}
};

pinmuxBoardCfg_t gAM64x_WkupPinmuxData[] =
{
    {0, gMcu_gpioPinCfg},
    {1, gMcu_i2cPinCfg},
    {2, gMcu_systemPinCfg},
    {3, gMcu_uartPinCfg},
    {PINMUX_END}
};
