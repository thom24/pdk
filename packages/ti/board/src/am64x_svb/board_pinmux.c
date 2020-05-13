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

static void Board_uartPinmxCfg()
{
    volatile uint32_t *addr = (volatile uint32_t *)(MAIN_CTRL_PINCFG_BASE + MAIN_UART0_RXD);
    uint32_t i;

    for (i = 0; i < 8; i++)
    {
        *addr++ = 0x54000;
    }
}
#endif  /* #ifdef VLAB_SIM */
#endif  /* #ifndef BUILD_M4F */

Board_STATUS Board_pinmuxConfig (void)
{
#if !defined(BUILD_M4F)
    pinmuxModuleCfg_t* pModuleData = NULL;
    pinmuxPerCfg_t* pInstanceData = NULL;
    int32_t i, j, k;

#ifndef BUILD_M4F
#ifdef VLAB_SIM
    Board_uartPinmxCfg();
#endif
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
#endif
#endif /* #ifndef BUILD_M4F */
    return BOARD_SOK;
}
