/*
 * SDL TEST
 *
 * SafeTI Diagnostics Library Test
 *
 * Copyright (C) 2018-2019 Texas Instruments Incorporated - http://www.ti.com/
 * ALL RIGHTS RESERVED
 */

#include "sdtf_common.h"
#include "sdl_exception.h"

#include <ti/csl/arch/r5/interrupt.h>

/* This is the list of exception handle and the parameters */
const CSL_R5ExptnHandlers SDTF_R5ExptnHandlers =
{
    .udefExptnHandler = &SDL_EXCEPTION_undefInstructionExptnHandler,
    .swiExptnHandler = &SDL_EXCEPTION_swIntrExptnHandler,
    .pabtExptnHandler = &SDL_EXCEPTION_prefetchAbortExptnHandler,
    .dabtExptnHandler = &SDL_EXCEPTION_dataAbortExptnHandler,
    .irqExptnHandler = &SDL_EXCEPTION_irqExptnHandler,
    .fiqExptnHandler = &SDL_EXCEPTION_fiqExptnHandler,
    .udefExptnHandlerArgs = ((void *)0u),
    .swiExptnHandlerArgs = ((void *)0u),
    .pabtExptnHandlerArgs = ((void *)0u),
    .dabtExptnHandlerArgs = ((void *)0u),
    .irqExptnHandlerArgs = ((void *)0u),
};

void SDTF_undefInstructionExptnCallback(void)
{
    SDTF_printf("\n Undefined Instruction exception");
}

void SDTF_swIntrExptnCallback(void)
{
    SDTF_printf("\n Software interrupt exception");
}

void SDTF_prefetchAbortExptnCallback(void)
{
    SDTF_printf("\n Prefetch Abort exception");
}
void SDTF_dataAbortExptnCallback(void)
{
    SDTF_printf("\n Data Abort exception");
}
void SDTF_irqExptnCallback(void)
{
    SDTF_printf("\n Irq exception");
}

void SDTF_fiqExptnCallback(void)
{
    SDTF_printf("\n Fiq exception");
}

void SDTF_exceptionInit(void)
{

    SDL_EXCEPTION_CallbackFunctions_t exceptionCallbackFunctions =
            {
             .udefExptnCallback = SDTF_undefInstructionExptnCallback,
             .swiExptnCallback = SDTF_swIntrExptnCallback,
             .pabtExptnCallback = SDTF_prefetchAbortExptnCallback,
             .dabtExptnCallback = SDTF_dataAbortExptnCallback,
             .irqExptnCallback = SDTF_irqExptnCallback,
             .fiqExptnCallback = SDTF_fiqExptnCallback,
            };

    /* Initialize SDL exception handler */
    SDL_EXCEPTION_init(&exceptionCallbackFunctions);
    /* Register SDL exception handler */
    Intc_RegisterExptnHandlers(&SDTF_R5ExptnHandlers);

    return;
}

int32_t SDTF_runExceptionApiTests(void)
{
    void *param = NULL;

    SDTF_printf("\n Exception API tests: starting");

    /* SDL_EXCEPTION_undefInstructionExptnHandler API test */
    SDL_EXCEPTION_undefInstructionExptnHandler(param);

    /* SDTF_swIntrExptnCallback API test */
    SDL_EXCEPTION_swIntrExptnHandler(param);

    /* SDTF_prefetchAbortExptnCallback API test */
    SDL_EXCEPTION_prefetchAbortExptnHandler(param);

    /* SDTF_dataAbortExptnCallback API test */
    SDL_EXCEPTION_dataAbortExptnHandler(param);

    /* SDTF_irqExptnCallback API test */
    SDL_EXCEPTION_irqExptnHandler(param);

    /* SDTF_fiqExptnCallback API test */
    SDL_EXCEPTION_fiqExptnHandler(param);

    SDTF_printf("\n Exception API tests complete");
    return 0;
}
