/*
 * Copyright (C) 2020 Texas Instruments Incorporated - http://www.ti.com/
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of Texas Instruments Incorporated nor the names of
 * its contributors may be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

/**
 *  \file ESM.c
 *
 *  \brief The file implements the ESM Driver.
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdint.h>
#include <string.h>
#include <ti/drv/esm/esm.h>
#include <ti/osal/osal.h>

#include "esm_internal.h"

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

extern ESM_HwAttrs gESMHwCfgAttrs;

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/** @addtogroup ESM_DRIVER_INTERNAL_DATA_STRUCTURE
 @{ */

/* Master control block for ESM driver */
ESM_DriverMCB   gEsmMCB;
/** @}*/

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */
/* Function prototypes */
void ESM_highpriority_interrupt(uintptr_t arg);
void ESM_lowpriority_interrupt(uintptr_t arg);
void ESM_processInterrupt (uint32_t vec, int32_t* groupNum, int32_t* vecNum);

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */
/** @addtogroup ESM_DRIVER_INTERNAL_FUNCTION
 @{ */

/* INTERNAL FUNCTIONS */
/** \brief ESM high priority interrupt handler: handles Group1 and Group2 high priority errors.
 *         Group 3 errors do not raise an interrupt and hence not handled here.
 *
 */
void ESM_highpriority_interrupt(uintptr_t arg)
{
    uint32_t            esmioffhr;
    uint32_t            vec;
    int32_t             groupNum = MINUS_ONE, vecNum = MINUS_ONE;

    esmioffhr = ESMGetHighPriorityLvlIntrStatus(gEsmMCB.esmBaseAddr);
    vec = esmioffhr - 1U;

    ESM_processInterrupt(vec, &groupNum, &vecNum);
}

/* INTERNAL FUNCTIONS */
/** \brief ESM low priority interrupt handler: handles Group1 low priority errors.
 *         Group 2 and 3 errors are not handled here.
 *
 */
void ESM_lowpriority_interrupt(uintptr_t arg)
{
    uint32_t            esmioffhr;
    uint32_t            vec;
    int32_t             groupNum = MINUS_ONE, vecNum = MINUS_ONE;

    esmioffhr = ESMGetLowPriorityLvlIntrStatus(gEsmMCB.esmBaseAddr);
    vec = esmioffhr - 1U;

    ESM_processInterrupt(vec, &groupNum, &vecNum);
}

/** \brief Process the ESM interrupts.
*
*    @param[in] vec: vector offset of the highest pending interrupt.
*
*    @param[out] groupNum: Group number to which the offseted input vector belongs.
*
*    @param[out] vecNum: Vector number in the corresponding group number above.
*
*/
void ESM_processInterrupt (uint32_t vec, int32_t* groupNum, int32_t* vecNum)
{
    uint32_t            index;
    uint32_t            regVal = 0U;
    CSL_esmRegs*        ptrESMRegs;

    if (vec < 32U)
    {
        /* group 1 0-31 errors */
        ESMClearIntrStatus(gEsmMCB.esmBaseAddr, vec);
#ifdef ESM_DEBUG
        gEsmMCB.debugEsmISRCount[0]++;
#endif
        *groupNum = 1;
        *vecNum = vec;
    }
    else if (vec < 64U)
    {
        /* group 2 0-31 errors */
        vec = vec - 32;
        regVal = ((uint32_t) 1U << (vec));
        ptrESMRegs = gESMHwCfgAttrs.ptrESMRegs;
        CSL_REG_WR(&ptrESMRegs->ESMSR2, regVal);

#ifdef ESM_DEBUG
        gEsmMCB.debugEsmISRCount[1]++;
#endif
        *groupNum = 2;
        *vecNum = vec;
    }
    else if (vec < gESMHwCfgAttrs.numGroup1Err)
    {
        /* group 1 error 32 and above */
#ifdef ESM_DEBUG
        gEsmMCB.debugEsmISRCount[vec/32]++;
#endif
        vec = vec - 32;
        ESMClearIntrStatus(gEsmMCB.esmBaseAddr, vec);

        *groupNum = 1;
        *vecNum = vec;
    }
    else
    {
        DebugP_assert(0);
    }
    if (*groupNum != MINUS_ONE)
    {
        /* Check if notify function was registered? */
        for (index = 0; index < ESM_MAX_NOTIFIERS; index++)
        {
            if ((*vecNum == gEsmMCB.notifyParams[index].errorNumber) &&
                    (*groupNum == gEsmMCB.notifyParams[index].groupNumber))
            {
                gEsmMCB.notifyParams[index].notify(gEsmMCB.notifyParams[index].arg);
                break;
            }
        }
    }
}
/** @}*/


/* EXTERNAL FUNCTIONS */

/** \brief Function intializes the ESM driver
*
*    @param[in] bClearErrors: boolean value to indicate if old ESM pending errors should be cleared or not
*                             value = 0: do not clear
*                             value = 1: clear all ESM group errors
*                             hint: If you using TI RTOS, then ESM errors are cleared before
*                                   entering main and this flag can be set to 0. For any other RTOS, check the
*                                   the RTOS implementation or set this flag to 1
*
*    @return    Handle to the ESM Driver
*
*/
ESM_Handle ESM_init(uint8_t bClearErrors)
{
    CSL_esmRegs*          ptrESMRegs;
    uint32_t              esmInitStatus;
    uint32_t              i;

    /* Initialize the allocated memory */
    memset ((void *)&gEsmMCB, 0U, sizeof(ESM_DriverMCB));

    ptrESMRegs          = gESMHwCfgAttrs.ptrESMRegs;
    gEsmMCB.esmBaseAddr = (uint32_t)ptrESMRegs;

    gEsmMCB.numGroup1Err = gESMHwCfgAttrs.numGroup1Err;

    OsalRegisterIntrParams_t interruptRegParams;

    /* Register ESM_highpriority_interrupt for MSS only. For DSS,
     * the ESM high priority interrupt is an NMI and application
     * needs to populate the NMI exception handler to hook up
     * ESM_highpriority_interrupt in the .cfg file, e.g.,
     * Exception.nmiHook = "&ESM_highpriority_interrupt"; */

#if defined (__TI_ARM_V7R4__)
    /* Initialize with defaults */
    Osal_RegisterInterrupt_initParams(&interruptRegParams);

    /* Populate the interrupt parameters */
    interruptRegParams.corepacConfig.name       = (char *)"ESM_HIGH_PRIORITY";
    interruptRegParams.corepacConfig.isrRoutine = ESM_highpriority_interrupt;
    interruptRegParams.corepacConfig.priority   = 0xFU;
    interruptRegParams.corepacConfig.intVecNum  = (int32_t)gESMHwCfgAttrs.highPrioIntNum;
    interruptRegParams.corepacConfig.corepacEventNum = (int32_t)gESMHwCfgAttrs.highPrioIntNum;
    /* Register interrupts */
    (void)Osal_RegisterInterrupt(&interruptRegParams,&(gEsmMCB.hwiHandleHi));

    /* Debug Message: */
    DebugP_log2 ("Debug: ESM Driver Registering HWI(High Priority) ISR [%p] for Interrupt %d\n",
                 (uintptr_t)gEsmMCB.hwiHandleHi, gESMHwCfgAttrs.highPrioIntNum);
#endif

    /* Initialize with defaults */
    Osal_RegisterInterrupt_initParams(&interruptRegParams);

    /* Populate the interrupt parameters */
    interruptRegParams.corepacConfig.name       = (char *)"ESM_LOW_PRIORITY";
    interruptRegParams.corepacConfig.isrRoutine = ESM_lowpriority_interrupt;
#if defined (__TI_ARM_V7R4__)
    interruptRegParams.corepacConfig.priority   = 0x8U;
    interruptRegParams.corepacConfig.intVecNum  = (int32_t)gESMHwCfgAttrs.lowPrioIntNum;
    interruptRegParams.corepacConfig.corepacEventNum = (int32_t)gESMHwCfgAttrs.lowPrioIntNum;
#elif defined (_TMS320C6X)
    interruptRegParams.corepacConfig.priority   = 0x1U;
    interruptRegParams.corepacConfig.intVecNum  = (int32_t)(OSAL_REGINT_INTVEC_EVENT_COMBINER); /* Host Interrupt vector */
    interruptRegParams.corepacConfig.corepacEventNum = (int32_t)gESMHwCfgAttrs.lowPrioIntNum;   /* Event going to INTC   */
#endif
    /* Register interrupts */
    (void)Osal_RegisterInterrupt(&interruptRegParams,&(gEsmMCB.hwiHandleLo));

    /* Debug Message: */
    DebugP_log2 ("Debug: ESM Driver Registering HWI(Low Priority) ISR [%p] for Interrupt %d\n",
                 (uintptr_t)gEsmMCB.hwiHandleLo, gESMHwCfgAttrs.lowPrioIntNum);


    if (bClearErrors == 1U)
    {
        /* Clear ESM Group 1 errors */
        for (i=0; i<gEsmMCB.numGroup1Err; i++)
        {
            if(ESMGetIntrStatus(gEsmMCB.esmBaseAddr, i))
            {
                ESMClearIntrStatus(gEsmMCB.esmBaseAddr, i);
            }
        }

	/* Clear ESM Group 2: 0-31 errors */
        /* read */
        esmInitStatus = CSL_REG_RD(&ptrESMRegs->ESMSR2);
        /*  clear */
        CSL_REG_WR(&ptrESMRegs->ESMSR2, esmInitStatus);

        /* Clear ESM Group 3: 0-31 errors */
        /* read */
        esmInitStatus = CSL_REG_RD(&ptrESMRegs->ESMSR3);
        /*  clear */
        CSL_REG_WR(&ptrESMRegs->ESMSR3, esmInitStatus);
    }

    return (ESM_Handle)&gEsmMCB;
}

/** @fn int32_t ESM_close(void)
*   \brief Close the ESM driver
*
*    @param[in] handle: Handle to the ESM Driver
*
*    @return    Value < 0 - in case of error
*               Value > 0 - Success
*
*/
int32_t ESM_close(ESM_Handle handle)
{
    ESM_DriverMCB*      ptrEsmMCB = NULL;
    int32_t             retVal = 0;

    /* Get the pointer to the ESM Driver Block */
    ptrEsmMCB = (ESM_DriverMCB*)handle;
    if (ptrEsmMCB == NULL)
    {
        retVal = MINUS_ONE;
    }
    else
    {
        /* Was the HWI registered?  */
        if (ptrEsmMCB->hwiHandleHi)
        {
            /* YES: Delete and unregister the interrupt handler. */
            (void)Osal_DeleteInterrupt(ptrEsmMCB->hwiHandleHi, (int32_t)gESMHwCfgAttrs.highPrioIntNum);
        }
        /* Was the HWI registered?  */
        if (ptrEsmMCB->hwiHandleLo)
        {
            /* YES: Delete and unregister the interrupt handler. */
            (void)Osal_DeleteInterrupt(ptrEsmMCB->hwiHandleLo, (int32_t)gESMHwCfgAttrs.lowPrioIntNum);
        }
    }
    return retVal;
}

/** \brief Register the notifers the ESM module will call back if error interrupt is detected.
*
*   @param[in] handle: Handle to the ESM Driver.
*
*   @param[in] params: Notifier error number and callback function.
*
*   @param[out]  errCode
*      Error code populated on error.
*
*    @return    Success -   Notifier index. Used when deregistering the notifier.
*               Error   -   ESM Error code
*
*/
int32_t ESM_registerNotifier(ESM_Handle handle, ESM_NotifyParams* params, int32_t* errCode)
{
    ESM_DriverMCB*      ptrEsmMCB = NULL;
    int32_t             retVal = 0;
    uintptr_t           key;
    int32_t             notifyIndex;

    /* Get the pointer to the ESM Driver Block */
    ptrEsmMCB = (ESM_DriverMCB*)handle;
    if ((ptrEsmMCB == NULL) || (params == NULL))
    {
        *errCode = ESM_EINVAL;
        retVal = MINUS_ONE;
    }
    else
    {
        /* Critical Section Protection: Notify registration needs to be
         * protected against multiple threads */
        key = HwiP_disable();

        /* Find a free notifier index */
        for (notifyIndex = 0; notifyIndex < ESM_MAX_NOTIFIERS; notifyIndex++)
        {
            if (gEsmMCB.notifyParams[notifyIndex].groupNumber == 0)
            {
                break;
            }
        }

        if (notifyIndex == ESM_MAX_NOTIFIERS)
        {
            /* Max allowed notifiers have already been registered */
            *errCode = ESM_ENOMEM;
            retVal = MINUS_ONE;
        }
        else
        {
            /* Check if the notifier handles group 1 or group 2 errors.
               Group 2 errors are enabled by default. Group 1 errors have to be explicitly enabled.
               Also, user can configure the interrupt priority level and influence on ERROR pin for group 1 errors.
               For group 2 errors, the interrupt priority level is always high, and the influence on ERROR pin is on always.
             */
            if (params->groupNumber == 1)
            {
                ESMEnableIntr(gEsmMCB.esmBaseAddr, params->errorNumber);
                /* Configure the interrupt priority level */
                ESMSetIntrPriorityLvl(gEsmMCB.esmBaseAddr, params->errorNumber, params->setIntrPriorityLvlHigh);
                /* Configure the failure influence on ERROR pin */
                ESMSetInfluenceOnErrPin(gEsmMCB.esmBaseAddr, params->errorNumber, params->enableInfluenceOnErrPin);
            }
            /* Unmask Group 2 ESM errors to enable the generation of NMI. */
            if (params->groupNumber == 2)
            {
                ESM_socConfigErrorGating(params->groupNumber, params->errorNumber, 0);
            }

            memcpy ((void *)&ptrEsmMCB->notifyParams[notifyIndex], (void *)params, sizeof (ESM_NotifyParams));
            retVal = notifyIndex;
        }
        /* Release the critical section: */
        HwiP_restore(key);
    }
    return retVal;
}

/** \brief Deregister the ESM notifers.
*
*   @param[in] handle: Handle to the ESM Driver.
*
*   @param[in] notifyIndex: Notifier index returned when the notifier was registered.
*
*   @param[out]  errCode
*      Error code populated on error.
*
*    @return    Success -   0
*               Error   -   ESM Error code
*
*/
int32_t ESM_deregisterNotifier(ESM_Handle handle, int32_t notifyIndex, int32_t* errCode)
{
    ESM_DriverMCB*      ptrEsmMCB = NULL;
    int32_t             retVal = 0;
    uintptr_t           key;
    uint32_t            groupNumber;
    uint32_t            errorNumber;

    /* Get the pointer to the ESM Driver Block */
    ptrEsmMCB = (ESM_DriverMCB*)handle;
    if ((ptrEsmMCB == NULL) || (notifyIndex >= ESM_MAX_NOTIFIERS))
    {
        *errCode = ESM_EINVAL;
        retVal = MINUS_ONE;
    }
    else
    {
        /* Critical Section Protection: Notify registration needs to be
         * protected against multiple threads */
        key = HwiP_disable();

        groupNumber = gEsmMCB.notifyParams[notifyIndex].groupNumber;
        errorNumber = gEsmMCB.notifyParams[notifyIndex].errorNumber;

        /* Check if the notifier is to handle group 1 or group 2 errors.
         * Group 2 errors are enabled by default. Group 1 errors was explicitly enabled
         * and now needs to be disabled. Also reset the interrupt priority level to low
         * and disable the influence on ERROR pin.
         */
        if (groupNumber == 1)
        {
            ESMDisableIntr(gEsmMCB.esmBaseAddr, errorNumber);
            ESMSetIntrPriorityLvl(gEsmMCB.esmBaseAddr, errorNumber, 0);
            ESMSetInfluenceOnErrPin(gEsmMCB.esmBaseAddr, errorNumber, 0);
        }
        /* Gating Group 2 ESM errors to disable the generation of NMI. */
        if (groupNumber == 2)
        {
            ESM_socConfigErrorGating(groupNumber, errorNumber, 1);
        }
        memset ((void *)&ptrEsmMCB->notifyParams[notifyIndex], 0, sizeof (ESM_NotifyParams));

        /* Release the critical section: */
        HwiP_restore(key);
    }
    return retVal;
}
