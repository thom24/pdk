/******************************************************************************
 * Copyright (c) 2019 Texas Instruments Incorporated - http://www.ti.com
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
 *  \file board_pinmux.c
 *
 *  \brief This file Enables pinmux for the board 
 *
 */

#include <ti/csl/soc.h>				
#include "board_internal.h"
#include "board_pinmux.h"

#ifndef BUILD_M4F
#ifdef VLAB_SIM
/* am64xx_main_padcfg_ctrl_mmr */
#define MAIN_PADCONFIG_CTRL_BASE    0x000F0000
#define CTRL_MMR0_PARTITION_SIZE    0x4000
#define MAIN_CTRL_PINCFG_BASE       (MAIN_PADCONFIG_CTRL_BASE + (1 * CTRL_MMR0_PARTITION_SIZE))

#define MAIN_UART0_RXD  0x0230
#define MAIN_UART0_TXD  0x0234
#define MAIN_UART0_CTSn 0x0238
#define MAIN_UART0_RTSn 0x023c
#define MAIN_UART1_RXD  0x0240
#define MAIN_UART1_TXD  0x0244
#define MAIN_UART1_CTSn 0x0248
#define MAIN_UART1_RTSn 0x024c

#define MAIN_SPI0_CS0   0x0208U

void Board_uartPinmxCfg()
{
    volatile uint32_t *addr = (volatile uint32_t *)(MAIN_CTRL_PINCFG_BASE + MAIN_UART0_RXD);
    uint32_t i;

    for (i = 0; i < 8; i++)
    {
        *addr++ = 0x54000;
    }
}

void Board_ospiPinmxCfg()
{
    volatile uint32_t *addr = (volatile uint32_t *)MAIN_CTRL_PINCFG_BASE;
    uint32_t ospiData[15] =
    {
        0x24000, 0x64000, 0x264000, 0x54000,
        0x54000, 0x54000, 0x054000, 0x54000,
        0x54000, 0x54000, 0x054000, 0x14000,
        0x14000, 0x04002, 0x4001
    };
    uint32_t i;

    for (i = 0; i < 15; i++)
    {
        *addr++ = ospiData[i];
    }
}

void Board_spiPinmxCfg()
{
    volatile uint32_t *addr = (volatile uint32_t *)(MAIN_CTRL_PINCFG_BASE + MAIN_SPI0_CS0);
    uint32_t spiData[5] =
    {
        PIN_MODE(0) | ((~PIN_PULL_DISABLE) & (PIN_PULL_DIRECTION & ~PIN_INPUT_ENABLE)), /* SPI0_CS0 */
        PIN_MODE(0) | ((~PIN_PULL_DISABLE) & (PIN_PULL_DIRECTION & ~PIN_INPUT_ENABLE)), /* SPI0_CS1 */
        PIN_MODE(0) | ((PIN_PULL_DISABLE | PIN_INPUT_ENABLE) & (~PIN_PULL_DIRECTION)),  /* SPI0_CLK */
        PIN_MODE(0) | ((PIN_PULL_DISABLE | PIN_INPUT_ENABLE) & (~PIN_PULL_DIRECTION)),  /* SPI0_D0 */
        PIN_MODE(0) | ((PIN_PULL_DISABLE | PIN_INPUT_ENABLE) & (~PIN_PULL_DIRECTION))   /* SPI0_D1 */
     };
    uint32_t i;

    for (i = 0; i < 5; i++)
    {
        *addr++ = spiData[i];
    }
}

#endif  /* #ifdef VLAB_SIM */
#endif  /* #ifndef BUILD_M4F */

/* Default pinmux configuration of UART Tx pin used by ROM/SBL */
#define BOARD_UART_TX_PINMUX_VAL            (PIN_MODE(0) | ((PIN_PULL_DISABLE) & \
                                                 (~PIN_PULL_DIRECTION & ~PIN_INPUT_ENABLE)))
#define BOARD_SYSFW_UART_TX_PINMUX_ADDR           (MAIN_PADCONFIG_CTRL_BASE + CSL_MAIN_PADCFG_CTRL_MMR_CFG0_PADCONFIG141)
#define BOARD_SBL_UART_TX_PINMUX_ADDR           (MAIN_PADCONFIG_CTRL_BASE + CSL_MAIN_PADCFG_CTRL_MMR_CFG0_PADCONFIG145)
#define BOARD_UART_TX_LOCK_KICK_ADDR        (MAIN_PADCONFIG_CTRL_BASE + \
                                                 CSL_MAIN_PADCFG_CTRL_MMR_CFG0_LOCK1_KICK0)


Board_STATUS Board_pinmuxConfig (void)
{
#ifndef BUILD_M4F
    /* Board_unlockMMR */
    HW_WR_REG32(BOARD_UART_TX_LOCK_KICK_ADDR, KICK0_UNLOCK_VAL);
    HW_WR_REG32(BOARD_UART_TX_LOCK_KICK_ADDR + 4U, KICK1_UNLOCK_VAL);

#ifdef VLAB_SIM
    Board_uartPinmxCfg();
    Board_ospiPinmxCfg();
    Board_spiPinmxCfg();
#else
    pinmuxModuleCfg_t* pModuleData = NULL;
    pinmuxPerCfg_t* pInstanceData = NULL;
    int32_t i, j, k;

    for(i = 0; PINMUX_END != gAM64xxMainPinmuxData[i].moduleId; i++)
    {
        pModuleData = gAM64xxMainPinmuxData[i].modulePinCfg;
        for(j = 0; (PINMUX_END != pModuleData[j].modInstNum); j++)
        {
            if(pModuleData[j].doPinConfig == TRUE)
            {
                pInstanceData = pModuleData[j].instPins;
                for(k = 0; (PINMUX_END != pInstanceData[k].pinOffset); k++)
                {
                    HW_WR_REG32((CSL_PADCFG_CTRL0_CFG0_BASE + 0x4000 + pInstanceData[k].pinOffset),
                                (pInstanceData[k].pinSettings));
                }
            }
        }
    }

	for(i = 0; PINMUX_END != gAM64xxWkupPinmuxData[i].moduleId; i++)
    {
        pModuleData = gAM64xxWkupPinmuxData[i].modulePinCfg;
        for(j = 0; (PINMUX_END != pModuleData[j].modInstNum); j++)
        {
            if(pModuleData[j].doPinConfig == TRUE)
            {
                pInstanceData = pModuleData[j].instPins;
                for(k = 0; (PINMUX_END != pInstanceData[k].pinOffset); k++)
                {
                    HW_WR_REG32((CSL_MCU_PADCFG_CTRL0_CFG0_BASE + 0x4000 + pInstanceData[k].pinOffset),
                                 (pInstanceData[k].pinSettings));
                }
            }
        }
    }
#endif /* #ifdef VLAB_SIM */
#endif /* #ifndef BUILD_M4F */
    return BOARD_SOK;
}

void Board_uartTxPinmuxConfig(void)
{
#ifndef BUILD_M4F
    /* Board_unlockMMR */
    HW_WR_REG32(BOARD_UART_TX_LOCK_KICK_ADDR, KICK0_UNLOCK_VAL);
    HW_WR_REG32(BOARD_UART_TX_LOCK_KICK_ADDR + 4U, KICK1_UNLOCK_VAL);

    /* Configure pinmux for UART Tx pin */
    HW_WR_REG32(BOARD_SBL_UART_TX_PINMUX_ADDR, BOARD_UART_TX_PINMUX_VAL);     

    /* Configure pinmux for SYSFW Tx pin */
    HW_WR_REG32(BOARD_SYSFW_UART_TX_PINMUX_ADDR, BOARD_UART_TX_PINMUX_VAL);
    
    /* Board_lockMMR */
    HW_WR_REG32(BOARD_UART_TX_LOCK_KICK_ADDR + 4U, 0);
    HW_WR_REG32(BOARD_UART_TX_LOCK_KICK_ADDR, 0);
#endif
}
