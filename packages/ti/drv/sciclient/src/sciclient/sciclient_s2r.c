/*
 *  Copyright (c) Texas Instruments Incorporated 2023
 *  All rights reserved.
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
 */

/**
 * \file sciclient_s2r.c
 *
 * \brief This file contains the code which load DDR retention and PMIC S2R code
 * into SRAM
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <string.h>
#include <ti/csl/arch/csl_arch.h>
#include <ti/board/board.h>
#if defined(SOC_J7200)
#include <ti/drv/lpm/src/s2r/j7200_evm/lpm_s2r.h>
#include <ti/board/src/j7200_evm/include/board_internal.h>
#include <ti/csl/soc/j7200/src/cslr_soc.h>
#endif
#if defined(SOC_J784S4)
#include <ti/drv/lpm/src/s2r/j784s4_evm/lpm_s2r.h>
#include <ti/board/src/j784s4_evm/include/board_internal.h>
#include <ti/csl/soc/j784s4/src/cslr_soc.h>
#endif
#include <ti/drv/sciclient/src/sciclient/sciclient_s2r.h>
#include <ti/csl/src/ip/msmc/cslr_msmc.h>
#include <ti/drv/sciclient/sciclient.h>
#include <TimerP.h>

static void asm_function(void);

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#define CSL_MSMC_CFGS0_RD(r) CSL_REG32_RD_OFF(CSL_COMPUTE_CLUSTER0_MSMC_CFGS0_BASE, r)
#define CSL_MSMC_CFGS0_WR(r, v) CSL_REG32_WR_OFF(CSL_COMPUTE_CLUSTER0_MSMC_CFGS0_BASE, r, v)

/**
 * Address in BT SRAM where to load the code that will take care of the
 * DDR retention and PMIC S2R configuration.
 */
#define SCICLIENT_S2R_SRAM_CODE_ADDRESS 0x41011000

/* Address to point the sp register in SRAM */
#define SCICLIENT_S2R_SRAM_STACK_POINTER 0x41014000

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

uint32_t lpm_sram_s2r[] = LPM_SRAM_S2R;

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

static void S2R_cleanAllDCache(void)
{
    uint32_t set, way;
    uint32_t numSets = CSL_armR5CacheGetNumSets();
    uint32_t numWays = CSL_armR5CacheGetNumWays();

    for (way = 0; way < numWays ; way++)
    {
        for (set = 0; set < numSets; set++)
        {
            CSL_armR5CacheCleanDcacheSetWay(set, way);
        }
    }
}

static bool S2R_cleanL3Cache(void)
{
    uint64_t t;

    CSL_MSMC_CFGS0_WR(CSL_MSMC_CFGS0_CACHE_CTRL, 0);

    t = TimerP_getTimeInUsecs();
    while (CSL_MSMC_CFGS0_RD(CSL_MSMC_CFGS0_CACHE_CTRL) != 0)
    {
	    /*
	     * timeout set to 100ms
	     * Maximum value measured for J7200 is 250us
	     */
	    if (TimerP_getTimeInUsecs() - t > 100000)
		    return false;
    }

    return true;
}

#define UART_DEBUG_ADDR CSL_UART8_BASE
//#define UART_DEBUG_ADDR CSL_UART0_BASE

/**
 * \brief   This function writes a byte on the serial console
 *
 * This function is based on UART_putc from ti/drv/uart/src/UART_stdio.c.
 *
 * \param   baseAdd  Address of the UART port to use.
 *          byteTx   The byte to be transmitted.
 *
 */
static void _Lpm_debugPutC(uint32_t baseAdd, uint8_t byteTx)
{
    uint32_t lcrRegValue = CSL_REG32_RD_OFF(baseAdd, UART_LCR);

    CSL_REG32_WR_OFF(baseAdd, UART_LCR, lcrRegValue & 0x7FU);
    /*
    ** Waits indefinitely until the THR and Transmitter Shift Registers are
    ** empty.
    */
    while(((unsigned int) UART_LSR_TX_SR_E_MASK |
           (unsigned int) UART_LSR_TX_FIFO_E_MASK) !=
          (CSL_REG32_RD_OFF(baseAdd, UART_LSR) &
           ((unsigned int) UART_LSR_TX_SR_E_MASK |
            (unsigned int) UART_LSR_TX_FIFO_E_MASK)))
    {
        /* Busy wait */
    }

    CSL_REG32_WR_OFF(baseAdd, UART_THR, byteTx);
    CSL_REG32_WR_OFF(baseAdd, UART_LCR, lcrRegValue);
}

/**
 * \brief  Wrapper function to call _Lpm_debugPutC
 *
 * \param  byteTx The byte to be transmitted.
 *
 */
static void Lpm_debugPutC(uint8_t byteTx)
{
#ifndef DEBUG_UART_MAIN
    static bool Lpm_uartConfigured = false;
    if (Lpm_uartConfigured == false)
    {
        CSL_REG32_WR_OFF(CSL_MCU_UART0_BASE, UART_LCR,
                         UART_FRAME_WORD_LENGTH_8 | UART_FRAME_NUM_STB_1 | UART_PARITY_NONE);
	Lpm_uartConfigured = true;
    }
#endif
    _Lpm_debugPutC(UART_DEBUG_ADDR, byteTx);
}

/**
 * \brief  This function writes data from a specified buffer onto the transmitter
 *         FIFO of UART
 *
 * \param  str  Pointer to a buffer to transmit.
 *         len  Number of bytes to be transmitted.
 *
 */
static void Lpm_debugPutS(const char *str, uint32_t len)
{
    unsigned int i;
    for (i = 0; i < len && str[i] != '\0'; i++)
    {
        if (str[i] == '\n')
        {
           Lpm_debugPutC('\r');
        }
        Lpm_debugPutC(str[i]);
    }
}

/**
 * \brief This function calls to the common procedure in Lpm_debugPrintf.
 *        This functions writes the output in hex format.
 *
 * This function is a partial copy of UART_convertVal from
 * ti/drv/uart/src/UART_stdio.c. Only the hexadecimal support was kept.
 *
 * \param ulValue  The value of the resolved variable passed as argument
 *        ulPos    The buffer position of the printed value
 *        ulCount  Total number of character to be printed
 *        cFill    The char to be placed between number
 *        pcBuf    buffer storing all the numbers
 *
 * \return 0.
 *
 */
static int32_t Lpm_debugConvertVal(uint32_t ulValue, uint32_t ulPos, uint32_t ulCount, char cFill, char *pcBuf)
{
    const char *const g_pcHex = "0123456789abcdef";
    uint32_t ulBase = 16;
    uint32_t ulIdx;
    uint32_t count = ulCount;
    uint32_t pos = ulPos;

    for (ulIdx = 1u;
         (((ulIdx * ulBase) <= ulValue) &&
          (((ulIdx * ulBase) / ulBase) == ulIdx));
         (ulIdx = (ulIdx * ulBase)))
    {
        count--;
    }

    /* Provide additional padding at the beginning of the
     * string conversion if needed. */
    if ((count > 1u) && (count < 16u))
    {
        for (count--; count != 0U; count--)
        {
            pcBuf[pos] = cFill;
            pos++;
        }
    }

    /* Convert the value into a string. */
    for (; ulIdx != 0U; ulIdx /= ulBase)
    {
        pcBuf[pos] = g_pcHex[(ulValue / ulIdx) % ulBase];
        pos++;
    }

    /* Write the string. */
    (void)Lpm_debugPutS(pcBuf, pos);

    return 0;
}

static void Lpm_debugPrintf(const char *pcString, ...)
{
    uint32_t ulIdx, ulValue, ulPos, ulCount;
    char    pcBuf[16], cFill;
    va_list  vaArgP;
    const char *pStr = pcString;

    /* Start the varargs processing. */
    (void)va_start(vaArgP, pcString);

    /* Loop while there are more characters in the string. */
    while (*pStr != (char)0U)
    {
        /* Find the first non-% character, or the end of the string. */
        for (ulIdx = 0;
             (pStr[ulIdx] != (char) '%') &&
             (pStr[ulIdx] != (char) '\0');
             ulIdx++)
        {}

        /* Write this portion of the string. */
        (void)Lpm_debugPutS(pStr, ulIdx);

        /* Skip the portion of the string that was written. */
        pStr += ulIdx;

        /* See if the next character is a %. */
        if (*pStr == (char) '%')
        {
            /* Skip the %. */
            pStr++;

            /* Set the digit count to zero, and the fill character to space
             * (i.e. to the defaults). */
            ulCount = 0;
            cFill   = (char) ' ';

            /* Determine how to handle the next character. */ 
            while((*pStr >= (char)'0') && (*pStr <= (char)'9'))
            {
                /* If this is a zero, and it is the first digit, then the
                 * fill character is a zero instead of a space. */
                if ((pStr[-1] == (char) '0') && (ulCount == 0U))
                {
                    cFill = (char) '0';
                }

                /* Update the digit count. */
                ulCount *= 10u;
                ulCount += ((uint32_t)(*pStr)) - (uint32_t) '0';

                /* Get the next character. */
                pStr++;
            }
            switch (*pStr)
            {
                /* Handle the %x and %X commands.  Note that they are treated
                 * identically; i.e. %X will use lower case letters for a-f
                 * instead of the upper case letters is should use.  We also
                 * alias %p to %x. */
                case (char) 'x':
                case (char) 'X':
                case (char) 'p':
                {
                    /* Get the value from the varargs. */
                    ulValue = (uint32_t)va_arg(vaArgP, uint32_t);

                    /* Reset the buffer position. */
                    ulPos = 0;

                    /* Determine the number of digits in the string version of
                     * the value. */
                    (void)Lpm_debugConvertVal(ulValue, ulPos, ulCount, cFill, pcBuf);
 
                    break;
                }

                /* Handle all other commands. */
                default:
                {
                    /* Indicate an error. */
                    (void)Lpm_debugPutS("ERROR", 5u);

                    /* This command has been handled. */
                    break;
                }
            }
            pStr++;
        }
    }

    /* End the varargs processing. */
    va_end(vaArgP);
}


void S2R_goRetention(void)
{
    //if (!S2R_cleanL3Cache())
    //{
    //   S2R_debugPrintf("Failed to clean L3 cache\n");
    //    Sciclient_pmDomainReset(DOMGRP_00, SCICLIENT_SERVICE_WAIT_FOREVER);
    //}

    /* load DDR retention code and PMIC S2R into SRAM */
    memcpy((void*)SCICLIENT_S2R_SRAM_CODE_ADDRESS,
           (const void*)&lpm_sram_s2r[0],
           LPM_SRAM_S2R_SIZE_IN_BYTES);

    int i=0;
    uint8_t *code, *sram;
    code = (uint8_t *)&lpm_sram_s2r[0];
    sram = (uint8_t *)SCICLIENT_S2R_SRAM_CODE_ADDRESS;
    for(i = 0; i < LPM_SRAM_S2R_SIZE_IN_BYTES; i++) {
	    if (*code != *sram)
		    return;
	    code +=1;
	    sram +=1;
    }

    //S2R_debugPrintf("Suspending\n");
    Lpm_debugPrintf("TOTO\n");

    asm_function();

    /* Never reach this point */
}

static void asm_function(void)
{
    asm volatile("mov sp, %0" : : "r" (SCICLIENT_S2R_SRAM_STACK_POINTER));
    asm volatile("blx %0" : : "r" (SCICLIENT_S2R_SRAM_CODE_ADDRESS));
}
