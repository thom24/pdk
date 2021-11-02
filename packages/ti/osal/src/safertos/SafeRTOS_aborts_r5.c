/*
 *  Copyright (C) 2018-2021 Texas Instruments Incorporated
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
 *  \file   SafeRTOS_aborts_r5.c.c
 *
 **/

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include <ti/csl/arch/csl_arch.h>
#include <ti/osal/osal.h>

extern CSL_R5ExptnHandlers gExptnHandlers;

void vUndefAbort(void)
{
    /* Go into an infinite loop.*/
    volatile uint32_t loop = 1;
    while(loop)
        ;
}

void vPrefetchAbort(void)
{
    /* Go into an infinite loop.*/
    volatile uint32_t loop = 1;
    while(loop)
        ;
}

/* Data Abort handler starts execution in HwiP_data_abort_handler, defined in SafeRTOS_abort_r5_asm.asm
 * After some initial assembly logic it then branches to this function.
 * After exiting this function it does some more assembly to return to the next instruction
 * following the one which caused the exception.
 */
void vDataAbort_c(void)
{
    /* Call registered call back */
    if (gExptnHandlers.dabtExptnHandler != (exptnHandlerPtr)NULL)
    {
        gExptnHandlers.dabtExptnHandler(gExptnHandlers.dabtExptnHandlerArgs);
    }
    else
    {
        /* Go into an infinite loop.*/
        volatile uint32_t loop = 1;
        while(loop)
            ;
    }
}
