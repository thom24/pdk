/*
 *  Copyright (C) 2020 Texas Instruments Incorporated - http://www.ti.com/
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
 */
/**
 * \file  mailbox_perf_main.c
 * \brief This is the main file for mailbox perf application that runs
 *        on each of the cores
**/

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include <stdint.h>
#include <stdio.h>
#include <ti/csl/csl_types.h>
#include <ti/csl/soc.h>
#include <ti/csl/arch/csl_arch.h>
#include <ti/csl/csl_rat.h>
#include "mailbox_app.h"
#if defined (SOC_AM65XX)
#include <ti/board/board.h>
#endif
#if defined (SOC_AM65XX) || defined (SOC_AM64X)
#include <ti/drv/uart/UART.h>
#include <ti/drv/uart/UART_stdio.h>
#endif

/* ========================================================================== */
/*                                 Macros                                     */
/* ========================================================================== */

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */
#if defined(SOC_AM64X) && defined(BUILD_M4F_0)
/*
  Custom RAT configuration for M4F to make Mailbox Registers accessible
*/
CSL_RatTranslationCfgInfo gCslM4RatCfg[8+1] __attribute__ ((section(".rat_cfg_buffer"), aligned (8))) =
{
    /* Add an entry for accessing MCU addresses including  MCU IPs */
    {
        .sizeInBytes        = (uint64_t) (0x01000000UL), /* Size in Bytes for the map */
        .baseAddress        = (uint32_t) (0x64000000U),  /* Translated base address */
        .translatedAddress  = (uint64_t) (0x04000000UL)  /* Physical addresses */
    },
    /* Add an entry for accessing MSRAM addresses */
    {
        .sizeInBytes        = (uint64_t) (0x00200000UL), /* Size in Bytes for the map */
        .baseAddress        = (uint32_t) (0x70000000U),  /* Translated base address */
        .translatedAddress  = (uint64_t) (0x70000000UL)  /* Physical addresses */
    },
    /* Add an entry for MAIN I2C/McSPI addresses */
    {
        .sizeInBytes        = (uint64_t) (0x00150000UL),  /* Size in Bytes for the map */
        .baseAddress        = (uint32_t) (0x60000000U),   /* Translated base address */
        .translatedAddress  = (uint64_t) (0x20000000UL)   /* Physical addresses */
    },
    /* Add an entry for MAIN GTC addresses */
    {
        .sizeInBytes        = (uint64_t) (0x00080000UL), /* Size in Bytes for the map */
        .baseAddress        = (uint32_t) (0x60A80000U),   /* Translated base address */
        .translatedAddress  = (uint64_t) (0x00A80000UL)   /* Physical addresses */
    },
    /* Add an entry for MAIN Timer addresses */
    {
        .sizeInBytes        = (uint64_t) (0x00080000UL),  /* Size in Bytes for the map */
        .baseAddress        = (uint32_t) (0x62400000U),   /* Translated base address */
        .translatedAddress  = (uint64_t) (0x02400000UL)   /* Physical addresses */
    },
    /* Add an entry for Main UART addresses */
    {
        .sizeInBytes        = (uint64_t) (0x00080000UL),  /* Size in Bytes for the map */
        .baseAddress        = (uint32_t) (0x62800000U),   /* Translated base address */
        .translatedAddress  = (uint64_t) (0x02800000UL)   /* Physical addresses */
    },
    /* Add an entry for accessing DDR addresses */
    {
        .sizeInBytes        = (uint64_t) (0x10000000UL), /* Size in Bytes for the map */
        .baseAddress        = (uint32_t) (0x80000000U),  /* Translated base address */
        .translatedAddress  = (uint64_t) (0x80000000UL)  /* Physical addresses */
    },
    {
        .sizeInBytes        = (uint64_t) (0x01000000UL),
        .baseAddress        = (uint32_t) (0x69000000U),
        .translatedAddress  = (uint64_t) (0x29000000U),
    },
    /* Last entry to quit RAT cfg */
    {
        .sizeInBytes        = (uint64_t) (0xDEADFACEUL), /* Size in Bytes for the map = END of MAP */
        .baseAddress        = (uint32_t) (0xDEADFACEU),  /* Translated base address   = END of MAP */
        .translatedAddress  = (uint64_t) (0xDEADFACEUL)  /* Physical addresses        = END of MAP */
    }
};
#endif

/* ========================================================================== */
/*                 Internal Function Declarations                             */
/* ========================================================================== */

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

void padConfig_prcmEnable()
{
#if defined (SOC_AM65XX)
    /*Pad configurations */
    Board_initCfg boardCfg;
    boardCfg = BOARD_INIT_UNLOCK_MMR | BOARD_INIT_UART_STDIO |
               BOARD_INIT_MODULE_CLOCK | BOARD_INIT_PINMUX_CONFIG;
    Board_init(boardCfg);
#endif

}

void test_Mailbox_runPerfTests(void)
{
    int32_t retValue;

    retValue = perfTestInit();
    if (retValue != 0)
    {
        MailboxAppPrint("Perf Test init failed\n");
    }
    if (retValue == 0)
    {
        retValue = perfTestRun();
#ifdef SYSTEM_MASTER
        if (retValue == 0)
        {
            MailboxAppPrint("Polling mode: Perf Test Run Passed\n");
        }
        else
        {
            MailboxAppPrint("Polling mode: Perf Test Run failed\n");
        }
#endif
    }
}
void test_Mailbox_runInterruptModePerfTests(void)
{
    int32_t retValue;

    retValue = perfTestInterruptModeInit();
    if (retValue != 0)
    {
        MailboxAppPrint("Interrupt mode: Perf Test init failed\n");
    }
    if (retValue == 0)
    {
        retValue = perfTestRun();
#ifdef SYSTEM_MASTER
        if (retValue == 0)
        {
            MailboxAppPrint("Interrupt mode: Perf Test Run Passed\n");
        }
        else
        {
            MailboxAppPrint("Interrupt mode: Perf Test Run failed\n");
        }
#endif
    }
}
void test_mailbox_perf_app_runner(void)
{
    test_Mailbox_runPerfTests();
    test_Mailbox_runInterruptModePerfTests();
}

int main(void)
{
    /*Pad configuration and PRCM enable*/
    padConfig_prcmEnable();
#if (defined (SOC_AM65XX) || defined(SOC_AM64X))
#ifdef BUILD_MCU
    {
        uint32_t val;

        /* Initialize and start PMU timer */

        CSL_armR5PmuCfg(0, 0, 1);
        /* Clear the overflow */
        val = CSL_armR5PmuReadCntrOverflowStatus();
        val &= 0x80000000;
        CSL_armR5PmuClearCntrOverflowStatus(val);
        CSL_armR5PmuCfgCntr(CSL_ARM_R5_PMU_CYCLE_COUNTER_NUM, CSL_ARM_R5_PMU_EVENT_TYPE_CYCLE_CNT);
        CSL_armR5PmuEnableAllCntrs(1);
        CSL_armR5PmuEnableCntr(CSL_ARM_R5_PMU_CYCLE_COUNTER_NUM, 1);
    }
#endif
#endif
#ifdef SYSTEM_MASTER
    MailboxAppPrint("\nMaster: Mailbox Application starting\n");
#endif
    test_mailbox_perf_app_runner();

    return 0;
}

void MailboxAppPrint(const char * str)
{
#ifdef USE_STD_PRINTF
    printf(str);
#else
#if defined (SOC_AM65XX) || defined (SOC_AM64X)
    UART_printf(str);
#else
    UARTConfigPuts(uartBaseAddr, str, -1);
#endif
#endif
}

void MailboxAppPrintf(const char *pFormat, ...)
{
    char     printString[256];
    va_list  vaArgP;

    /* Start the varargs processing. */
    va_start(vaArgP, pFormat);

    vsnprintf(&printString[0U], sizeof(printString), pFormat, vaArgP);

    MailboxAppPrint(printString);
    /* End the varargs processing. */
    va_end(vaArgP);

    return;
}

/********************************* End of file ******************************/
