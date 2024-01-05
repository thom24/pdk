/**
* Note: This file was auto-generated by TI PinMux on 7/8/2020 at 8:43:51 AM.
*
* \file  J7200_pinmux_data.c
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

#include "J7200_pinmux.h"

/** Peripheral Pin Configurations */


static pinmuxPerCfg_t gSpi5PinCfg[] =
{
    /* MySPI5 -> SPI5_CLK -> W15 */
    {
        PIN_MCAN4_TX, PIN_MODE(8) | \
        ((PIN_PULL_DISABLE | PIN_INPUT_ENABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MySPI5 -> SPI5_CS0 -> W16 */
    {
        PIN_MCAN3_RX, PIN_MODE(8) | \
        ((PIN_PULL_DISABLE | PIN_INPUT_ENABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MySPI5 -> SPI5_CS1 -> V21 */
    {
        PIN_MCAN5_TX, PIN_MODE(8) | \
        ((PIN_PULL_DISABLE | PIN_INPUT_ENABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MySPI5 -> SPI5_D0 -> Y21 */
    {
        PIN_MCAN3_TX, PIN_MODE(8) | \
        ((PIN_PULL_DISABLE | PIN_INPUT_ENABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MySPI5 -> SPI5_D1 -> Y20 */
    {
        PIN_MCAN4_RX, PIN_MODE(8) | \
        ((PIN_PULL_DISABLE | PIN_INPUT_ENABLE) & (~PIN_PULL_DIRECTION))
    },
    {PINMUX_END}
};

static pinmuxModuleCfg_t gSpiPinCfg[] =
{
    {5, TRUE, gSpi5PinCfg},
    {PINMUX_END}
};


static pinmuxPerCfg_t gUart3PinCfg[] =
{
    /* MyUART3 -> UART3_CTSn -> T15 */
    {
        PIN_MCAN8_TX, PIN_MODE(11) | \
        ((PIN_PULL_DISABLE | PIN_INPUT_ENABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyUART3 -> UART3_RTSn -> U19 */
    {
        PIN_MCAN8_RX, PIN_MODE(11) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION & ~PIN_INPUT_ENABLE))
    },
    /* MyUART3 -> UART3_RXD -> Y18 */
    {
        PIN_MCAN2_TX, PIN_MODE(11) | \
        ((PIN_PULL_DISABLE | PIN_INPUT_ENABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyUART3 -> UART3_TXD -> Y19 */
    {
        PIN_MCAN2_RX, PIN_MODE(11) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION & ~PIN_INPUT_ENABLE))
    },
    {PINMUX_END}
};

static pinmuxModuleCfg_t gUartPinCfg[] =
{
    {3, TRUE, gUart3PinCfg},
    {PINMUX_END}
};


pinmuxBoardCfg_t gJ7200_MainPinmuxDataProfibus[] =
{
    {0, gSpiPinCfg},
    {1, gUartPinCfg},
    {PINMUX_END}
};

pinmuxBoardCfg_t gJ7200_WkupPinmuxDataProfibus[] =
{
    {PINMUX_END}
};
