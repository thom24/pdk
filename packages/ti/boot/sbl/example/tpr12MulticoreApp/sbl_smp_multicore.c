/*
 * Copyright (c) 2018, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <string.h>
#include <ti/board/board.h>
#include <ti/drv/uart/UART_stdio.h>
#include "sbl_slave_core_boot.h"
#include "sbl_soc_cfg.h"
#include "sbl_soc.h"
#include <ti/csl/arch/csl_arch.h>
#include <ti/osal/osal.h>
#include <ti/osal/CacheP.h>

#define DSP_CORE_UP_PATTERN    0xC0FFEE

#ifdef BUILD_C66X_1
	#define DSP_POKE_MEM_ADDR ((volatile int *)0xC0200000)
#else
	#ifdef BUILD_MCU1_0
		#define DSP_POKE_MEM_ADDR ((volatile int *)0x10200000)
	#endif
#endif


#ifdef BUILD_C66X_1
int main()
{
    CacheP_Inv((const void *)DSP_POKE_MEM_ADDR,sizeof(int));

	*(DSP_POKE_MEM_ADDR) = DSP_CORE_UP_PATTERN;
    CacheP_wbInv((const void *)DSP_POKE_MEM_ADDR,sizeof(int));
    return 0;
}
#endif

#if defined (BUILD_MCU1_0)

int main()
{
#if !defined(tpr12_qt)
    Board_initCfg boardCfg;

    boardCfg = BOARD_INIT_PINMUX_CONFIG |
        BOARD_INIT_MODULE_CLOCK |
        BOARD_INIT_UART_STDIO;
    /* Board Init UART for logging. */
    Board_init(boardCfg);
    UART_printf("MPU SMP boot test\r\n");
#endif

    CacheP_Inv((const void *)DSP_POKE_MEM_ADDR,sizeof(int));
    while (*(DSP_POKE_MEM_ADDR) != DSP_CORE_UP_PATTERN)
    {
        CacheP_Inv((const void *)DSP_POKE_MEM_ADDR,sizeof(int));
    }
 
    /* Check if MPUs have run in AMP mode */
    if (*(DSP_POKE_MEM_ADDR) != DSP_CORE_UP_PATTERN)
    {
#if !defined(tpr12_qt)
        UART_printf("Some tests have failed\r\n");
#else
        printf("Some tests have failed\r\n");
#endif
    }
    else
    {
#if !defined(tpr12_qt)
        UART_printf("All tests have passed\r\n");
#else
        printf("All tests have passed\r\n");
#endif
    }
    return 0;
}
#endif
