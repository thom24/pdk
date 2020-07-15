/******************************************************************************
 * Copyright (c) 2019 - 2020 Texas Instruments Incorporated - http://www.ti.com
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
#ifdef SIM_BUILD
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

#define MAIN_GPMC0_AD0  0x003cU

#ifdef SIM_BUILD
// PinMux Changes
#include <stdint.h>

//Region of pinmux padconfigs
//Can't find the CSL defines for the padconfig0 offset, need to replace
//The offsets were found from the AM64X MAIN and MCU padcfg_ctrl pdf documents
#define TEMP_REPLACE_WITH_CSL_MAIN_PADCONFIG0_OFFSET    (0x4000)
#define PINMUX_MAIN_REG_BASE                            (CSL_PADCFG_CTRL0_CFG0_BASE + TEMP_REPLACE_WITH_CSL_MAIN_PADCONFIG0_OFFSET)

//All defaults except:
//Receiver Disabled
//High-Z
#define PINMUX_DEFAULT_REG_VALUE        (0x08010000U)
#define PINMUX_DEFAULT                  (0xffU)

//Pullup/down/none
#define PINMUX_PD                       (0x0U)
#define PINMUX_PU                       (0x2U)
#define PINMUX_NP                       (0x1U)

#define PINMUX_DRIVESTRENGTH_0          (0x0U)
#define PINMUX_DRIVESTRENGTH_1          (0x1U)
#define PINMUX_DRIVESTRENGTH_2          (0x2U)
#define PINMUX_DRIVESTRENGTH_3          (0x3U)

//The pinmux mmrs are stupid about this:
#define PINMUX_RX_ACTIVE                (0x1U)
#define PINMUX_RX_DISABLE               (0x0U)
#define PINMUX_TX_ACTIVE                (0x0U)
#define PINMUX_TX_DISABLE               (0x1U)

typedef enum {
    PINMUX_MUX_MODE_0 = 0,
    PINMUX_MUX_MODE_1,
    PINMUX_MUX_MODE_2,
    PINMUX_MUX_MODE_3,
    PINMUX_MUX_MODE_4,
    PINMUX_MUX_MODE_5,
    PINMUX_MUX_MODE_6,
    PINMUX_MUX_MODE_7,
    PINMUX_MUX_MODE_8,
    PINMUX_MUX_MODE_9,
    PINMUX_MUX_MODE_10,
    PINMUX_MUX_MODE_11,
    PINMUX_MUX_MODE_12,
    PINMUX_MUX_MODE_13,
    PINMUX_MUX_MODE_14,
    PINMUX_MUX_MODE_15,
}muxmode_t;

typedef struct{
    uint32_t    mmrBaseAddress;
    uint32_t    pinmuxRegOffset;
    muxmode_t   muxmode; //0 through 15
    uint8_t     pu_pd;
    uint8_t     driveStrength;
    uint8_t     rx;
    uint8_t     tx;
} pinmux_t;


pinmux_t PINMUX_MAIN_ICSSG0_MII0_APP2_array_board [] = {
//pinmux reg base,  Reg offset, Muxmode,  PUPD,    DRIVE_STRENGTH,    RX,  TX

{PINMUX_MAIN_REG_BASE, 0x0160, PINMUX_MUX_MODE_1, PINMUX_DEFAULT, PINMUX_DEFAULT, PINMUX_RX_ACTIVE , PINMUX_TX_DISABLE}, //PAD ->PRG0_PRU0GPO0 ; PIN ->PRG0_MII0_RD0 ; PINMUX_MUX_MODE_ ->PINMUX_MUX_MODE_1, PADCONFIG88
{PINMUX_MAIN_REG_BASE, 0x0164, PINMUX_MUX_MODE_1, PINMUX_DEFAULT, PINMUX_DEFAULT, PINMUX_RX_ACTIVE , PINMUX_TX_DISABLE}, //PAD ->PRG0_PRU0GPO1 ; PIN ->PRG0_MII0_RD1 ; PINMUX_MUX_MODE_ ->PINMUX_MUX_MODE_1, PADCONFIG89
{PINMUX_MAIN_REG_BASE, 0x0168, PINMUX_MUX_MODE_1, PINMUX_DEFAULT, PINMUX_DEFAULT, PINMUX_RX_ACTIVE , PINMUX_TX_DISABLE}, //PAD ->PRG0_PRU0GPO2 ; PIN ->PRG0_MII0_RD2 ; PINMUX_MUX_MODE_ ->PINMUX_MUX_MODE_1, PADCONFIG90
{PINMUX_MAIN_REG_BASE, 0x016C, PINMUX_MUX_MODE_1, PINMUX_DEFAULT, PINMUX_DEFAULT, PINMUX_RX_ACTIVE , PINMUX_TX_DISABLE}, //PAD ->PRG0_PRU0GPO3 ; PIN ->PRG0_MII0_RD3 ; PINMUX_MUX_MODE_ ->PINMUX_MUX_MODE_1, PADCONFIG91
{PINMUX_MAIN_REG_BASE, 0x0170, PINMUX_MUX_MODE_1, PINMUX_DEFAULT, PINMUX_DEFAULT, PINMUX_RX_ACTIVE , PINMUX_TX_DISABLE}, //PAD ->PRG0_PRU0GPO4 ; PIN ->PRG0_MII0_RX_CTL ; PINMUX_MUX_MODE_ ->PINMUX_MUX_MODE_1, PADCONFIG92
{PINMUX_MAIN_REG_BASE, 0x0174, PINMUX_MUX_MODE_1, PINMUX_DEFAULT, PINMUX_DEFAULT, PINMUX_RX_ACTIVE , PINMUX_TX_DISABLE}, //PAD ->PRG0_PRU0GPO5 ; PIN ->PRG0_MII0_RXER ; PINMUX_MUX_MODE_ ->PINMUX_MUX_MODE_1, PADCONFIG93
{PINMUX_MAIN_REG_BASE, 0x0178, PINMUX_MUX_MODE_1, PINMUX_DEFAULT, PINMUX_DEFAULT, PINMUX_RX_ACTIVE , PINMUX_TX_DISABLE}, //PAD ->PRG0_PRU0GPO6 ; PIN ->PRG0_MII0_RXC ; PINMUX_MUX_MODE_ ->PINMUX_MUX_MODE_1, PADCONFIG94
{PINMUX_MAIN_REG_BASE, 0x0180, PINMUX_MUX_MODE_1, PINMUX_DEFAULT, PINMUX_DEFAULT, PINMUX_RX_ACTIVE , PINMUX_TX_DISABLE}, //PAD ->PRG0_PRU0GPO8 ; PIN ->PRG0_MII0_RXLINK ; PINMUX_MUX_MODE_ ->PINMUX_MUX_MODE_1, PADCONFIG96
{PINMUX_MAIN_REG_BASE, 0x018C, PINMUX_MUX_MODE_0, PINMUX_DEFAULT, PINMUX_DEFAULT, PINMUX_RX_DISABLE, PINMUX_TX_ACTIVE }, //PAD ->PRG0_PRU0GPO11 ; PIN ->PRG0_MII0_TD0 ; PINMUX_MUX_MODE_ ->PINMUX_MUX_MODE_0, PADCONFIG99
{PINMUX_MAIN_REG_BASE, 0x0190, PINMUX_MUX_MODE_0, PINMUX_DEFAULT, PINMUX_DEFAULT, PINMUX_RX_DISABLE, PINMUX_TX_ACTIVE }, //PAD ->PRG0_PRU0GPO12 ; PIN ->PRG0_MII0_TD1 ; PINMUX_MUX_MODE_ ->PINMUX_MUX_MODE_0, PADCONFIG100
{PINMUX_MAIN_REG_BASE, 0x0194, PINMUX_MUX_MODE_0, PINMUX_DEFAULT, PINMUX_DEFAULT, PINMUX_RX_DISABLE, PINMUX_TX_ACTIVE }, //PAD ->PRG0_PRU0GPO13 ; PIN ->PRG0_MII0_TD2 ; PINMUX_MUX_MODE_ ->PINMUX_MUX_MODE_0, PADCONFIG101
{PINMUX_MAIN_REG_BASE, 0x0198, PINMUX_MUX_MODE_0, PINMUX_DEFAULT, PINMUX_DEFAULT, PINMUX_RX_DISABLE, PINMUX_TX_ACTIVE }, //PAD ->PRG0_PRU0GPO14 ; PIN ->PRG0_MII0_TD3 ; PINMUX_MUX_MODE_ ->PINMUX_MUX_MODE_0, PADCONFIG102
{PINMUX_MAIN_REG_BASE, 0x019C, PINMUX_MUX_MODE_0, PINMUX_DEFAULT, PINMUX_DEFAULT, PINMUX_RX_DISABLE, PINMUX_TX_ACTIVE }, //PAD ->PRG0_PRU0GPO15 ; PIN ->PRG0_MII0_TX_CTL ; PINMUX_MUX_MODE_ ->PINMUX_MUX_MODE_0, PADCONFIG103
{PINMUX_MAIN_REG_BASE, 0x01A0, PINMUX_MUX_MODE_1, PINMUX_DEFAULT, PINMUX_DEFAULT, PINMUX_RX_ACTIVE , PINMUX_TX_DISABLE}  //PAD ->PRG0_PRU0GPO16 ; PIN ->PRG0_MII0_TXC ; PINMUX_MUX_MODE_ ->PINMUX_MUX_MODE_1, PADCONFIG104
};

pinmux_t PINMUX_MAIN_ICSSG0_MII1_APP2_array_board [] = {
//pinmux reg base,  Reg offset, Muxmode,  PUPD,    DRIVE_STRENGTH,    RX,  TX

{PINMUX_MAIN_REG_BASE, 0x01B0, PINMUX_MUX_MODE_1, PINMUX_DEFAULT, PINMUX_DEFAULT, PINMUX_RX_ACTIVE , PINMUX_TX_DISABLE},  //PAD ->PRG0_PRU1GPO0 ; PIN ->PRG0_MII1_RD0 ; PINMUX_MUX_MODE_ ->PINMUX_MUX_MODE_1, PADCONFIG108
{PINMUX_MAIN_REG_BASE, 0x01B4, PINMUX_MUX_MODE_1, PINMUX_DEFAULT, PINMUX_DEFAULT, PINMUX_RX_ACTIVE , PINMUX_TX_DISABLE}, //PAD ->PRG0_PRU1GPO1 ; PIN ->PRG0_MII1_RD1 ; PINMUX_MUX_MODE_ ->PINMUX_MUX_MODE_1, PADCONFIG109
{PINMUX_MAIN_REG_BASE, 0x01B8, PINMUX_MUX_MODE_1, PINMUX_DEFAULT, PINMUX_DEFAULT, PINMUX_RX_ACTIVE , PINMUX_TX_DISABLE}, //PAD ->PRG0_PRU1GPO2 ; PIN ->PRG0_MII1_RD2 ; PINMUX_MUX_MODE_ ->PINMUX_MUX_MODE_1, PADCONFIG110
{PINMUX_MAIN_REG_BASE, 0x01BC, PINMUX_MUX_MODE_1, PINMUX_DEFAULT, PINMUX_DEFAULT, PINMUX_RX_ACTIVE , PINMUX_TX_DISABLE}, //PAD ->PRG0_PRU1GPO3 ; PIN ->PRG0_MII1_RD3 ; PINMUX_MUX_MODE_ ->PINMUX_MUX_MODE_1, PADCONFIG111
{PINMUX_MAIN_REG_BASE, 0x01C0, PINMUX_MUX_MODE_1, PINMUX_DEFAULT, PINMUX_DEFAULT, PINMUX_RX_ACTIVE , PINMUX_TX_DISABLE}, //PAD ->PRG0_PRU1GPO4 ; PIN ->PRG0_MII1_RX_CTL ; PINMUX_MUX_MODE_ ->PINMUX_MUX_MOdE_1, PADCONFIG112
{PINMUX_MAIN_REG_BASE, 0x01C4, PINMUX_MUX_MODE_1, PINMUX_DEFAULT, PINMUX_DEFAULT, PINMUX_RX_ACTIVE , PINMUX_TX_DISABLE}, //PAD ->PRG0_PRU1GPO5 ; PIN ->PRG0_MII1_RXER ; PINMUX_MUX_MODE_ ->PINMUX_MUX_MODE_1, PADCONFIG113
{PINMUX_MAIN_REG_BASE, 0x01C8, PINMUX_MUX_MODE_1, PINMUX_DEFAULT, PINMUX_DEFAULT, PINMUX_RX_ACTIVE , PINMUX_TX_DISABLE}, //PAD ->PRG0_PRU1GPO6 ; PIN ->PRG0_MII1_RXC ; PINMUX_MUX_MODE_ ->PINMUX_MUX_MODE_1, PADCONFIG114
{PINMUX_MAIN_REG_BASE, 0x01d0, PINMUX_MUX_MODE_1, PINMUX_DEFAULT, PINMUX_DEFAULT, PINMUX_RX_ACTIVE , PINMUX_TX_DISABLE}, //PAD ->PRG0_PRU1GPO8 ; PIN ->PRG0_MII1_RXLINK ; PINMUX_MUX_MODE_ ->PINMUX_MUX_MODE_1, PADCONFIG116
{PINMUX_MAIN_REG_BASE, 0x01DC, PINMUX_MUX_MODE_0, PINMUX_DEFAULT, PINMUX_DEFAULT, PINMUX_RX_DISABLE, PINMUX_TX_ACTIVE }, //PAD ->PRG0_PRU1GPO11 ; PIN ->PRG0_MII1_TD0 ; PINMUX_MUX_MODE_ ->PINMUX_MUX_MODE_0, PADCONFIG119
{PINMUX_MAIN_REG_BASE, 0x01E0, PINMUX_MUX_MODE_0, PINMUX_DEFAULT, PINMUX_DEFAULT, PINMUX_RX_DISABLE, PINMUX_TX_ACTIVE }, //PAD ->PRG0_PRU1GPO12 ; PIN ->PRG0_MII1_TD1 ; PINMUX_MUX_MODE_ ->PINMUX_MUX_MODE_0, PADCONFIG120
{PINMUX_MAIN_REG_BASE, 0x01E4, PINMUX_MUX_MODE_0, PINMUX_DEFAULT, PINMUX_DEFAULT, PINMUX_RX_DISABLE, PINMUX_TX_ACTIVE }, //PAD ->PRG0_PRU1GPO13 ; PIN ->PRG0_MII1_TD2 ; PINMUX_MUX_MODE_ ->PINMUX_MUX_MODE_0, PADCONFIG121
{PINMUX_MAIN_REG_BASE, 0x01E8, PINMUX_MUX_MODE_0, PINMUX_DEFAULT, PINMUX_DEFAULT, PINMUX_RX_DISABLE, PINMUX_TX_ACTIVE }, //PAD ->PRG0_PRU1GPO14 ; PIN ->PRG0_MII1_TD3 ; PINMUX_MUX_MODE_ ->PINMUX_MUX_MODE_0, PADCONFIG122
{PINMUX_MAIN_REG_BASE, 0x01EC, PINMUX_MUX_MODE_0, PINMUX_DEFAULT, PINMUX_DEFAULT, PINMUX_RX_DISABLE, PINMUX_TX_ACTIVE }, //PAD ->PRG0_PRU1GPO15 ; PIN ->PRG0_MII1_TX_CTL ; PINMUX_MUX_MODE_ ->PINMUX_MUX_MODE_0, PADCONFIG123
{PINMUX_MAIN_REG_BASE, 0x01F0, PINMUX_MUX_MODE_1, PINMUX_DEFAULT, PINMUX_DEFAULT, PINMUX_RX_ACTIVE , PINMUX_TX_DISABLE}  //PAD ->PRG0_PRU1GPO16 ; PIN ->PRG0_MII1_TXC ; PINMUX_MUX_MODE_ ->PINMUX_MUX_MODE_1, PADCONFIG124
};

void set_pinmux_board(pinmux_t *Array, uint8_t arraysize){
    uint32_t c = 0;
    uint32_t i = 0;
    uint32_t mmr_reg_value = 0;
    uint32_t errors = 0;

    //Pointer for loop
    pinmux_t * arrayPtr = NULL;
    arrayPtr = Array;

    //Loop on each pinmux_t struct
    for (i = 0; i < arraysize; i++){
        //Set PADCONFIG register to defined PINMUX_DEFAULTs
        *(volatile uint32_t *)(uintptr_t)(arrayPtr->mmrBaseAddress + arrayPtr->pinmuxRegOffset) = PINMUX_DEFAULT_REG_VALUE;

        //Read the PADCONFIG register
        mmr_reg_value = *(volatile uint32_t *)(uintptr_t)(arrayPtr->mmrBaseAddress + arrayPtr->pinmuxRegOffset);

        //Mux mode slection
        if(arrayPtr->muxmode != PINMUX_DEFAULT){
            mmr_reg_value  &= (0xFFFFFFF0);     // Mux mode
            mmr_reg_value  |= (arrayPtr->muxmode & 0x0000000F);
        }

        //PU-PD Selection
        if(arrayPtr->pu_pd != PINMUX_DEFAULT){
            mmr_reg_value &= (0xFFFCFFFF);       // PU-PD
            mmr_reg_value |= ((arrayPtr->pu_pd) << 16);
        }

        //Drive strength selection
        if(arrayPtr->driveStrength != PINMUX_DEFAULT){
            mmr_reg_value &= (0xFFE7FFFF);       // Drive strength
            mmr_reg_value |= ((arrayPtr->driveStrength) << 19);
        }

        //Receive enable/disable
        if(arrayPtr->rx != PINMUX_DEFAULT){
            mmr_reg_value &= (0xFFFBFFFF);       // Drive strength
            mmr_reg_value |= ((arrayPtr->rx) << 18);
        }

        //Transmit enable/disable
        if(arrayPtr->tx != PINMUX_DEFAULT){
            mmr_reg_value &= (0xFFDFFFFF);       // Drive strength
            mmr_reg_value |= ((arrayPtr->tx) << 21);
        }

        //Writing to Padconfig register
        *(volatile uint32_t *)(uintptr_t)(arrayPtr->mmrBaseAddress + arrayPtr->pinmuxRegOffset) = mmr_reg_value;

        for(c = 0; c < 20; c++);

        //Verify write
        if((*(volatile uint32_t *)(uintptr_t)(arrayPtr->mmrBaseAddress + arrayPtr->pinmuxRegOffset)) != mmr_reg_value) errors++;

        arrayPtr++;
    }
}

//PinMux change End
#endif /* ifdef SIM_BUILD */

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

void Board_gpmcPinmxCfg()
{
    volatile uint32_t *addr = (volatile uint32_t *)(MAIN_CTRL_PINCFG_BASE + MAIN_GPMC0_AD0);
    uint32_t gpmcData[69] =
    {
        0x50000, 0x50000, 0x50000, 0x50000,
        0x40000, 0x40000, 0x50000, 0x50000,
        0x50000, 0x50000, 0x50000, 0x50000,
        0x50000, 0x50000, 0x50000, 0x50000,
        0x50004, 0x50000, 0x40000, 0x40000,
        0x40000, 0x40000, 0x40000, 0x60000,
        0x60000, 0x40000, 0x50000, 0x40000,
        0x40000, 0x40000, 0x40000, 0x50008,
        0x50008, 0x50008, 0x50008, 0x50008,
        0x50008, 0x50008, 0x50008, 0x50008,
        0x50008, 0x50008, 0x50008, 0x50008,
        0x50008, 0x50008, 0x50008, 0x40008,
        0x50008, 0x50008, 0x40008, 0x60008,
        0x40008, 0x40008, 0x40008, 0x40008,
        0x40008, 0x40008, 0x40008, 0x40008,
        0x40008, 0x40008, 0x40008, 0x40008,
        0x40008, 0x40008, 0x40008, 0x40008,
        0x40008
    };
    uint32_t i;

    for (i = 0; i < 69; i++)
    {
        *addr++ = gpmcData[i];
    }
}

#endif  /* #ifdef SIM_BUILD */
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

#ifdef SIM_BUILD
    Board_uartPinmxCfg();
    Board_ospiPinmxCfg();
    Board_spiPinmxCfg();
    Board_gpmcPinmxCfg();
	set_pinmux_board(PINMUX_MAIN_ICSSG0_MII0_APP2_array_board, (sizeof(PINMUX_MAIN_ICSSG0_MII0_APP2_array_board)/sizeof(pinmux_t)));
	set_pinmux_board(PINMUX_MAIN_ICSSG0_MII1_APP2_array_board, (sizeof(PINMUX_MAIN_ICSSG0_MII1_APP2_array_board)/sizeof(pinmux_t)));
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
#endif /* #ifdef SIM_BUILD */
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
