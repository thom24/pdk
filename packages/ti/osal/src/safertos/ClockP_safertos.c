/*
 * Copyright (c) 2021, Texas Instruments Incorporated
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
/*
 *  ======== ClockP_safertos.c ========
 */

#include <ti/osal/ClockP.h>

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ti/csl/csl_types.h>
#include <ti/osal/osal.h>
#include <ti/osal/DebugP.h>
#include <ti/osal/soc/osal_soc.h>

#include <SafeRTOS_API.h>
#include <timers.h>

extern portBaseType xPortInIsrContext( void );
/**
 * \brief Callback that is called when the clock expires
 *
 * \param args [in] user specific argument pointer that was passed via \ref ClockP_Params
 */
typedef void (*ClockP_FxnCallback)(void *args);

typedef struct ClockP_safertos_s
{
    bool                        used;
    timerControlBlockType       timerObj;
    timerHandleType             timerHndl;
    ClockP_FxnCallback          callback;
    void                        *arg;
} ClockP_safertos;

/* global pool of statically allocated clock pools */
static ClockP_safertos gOsalClockPSafeRtosPool[OSAL_FREERTOS_CONFIGNUM_CLOCK];

uint32_t gOsalClockAllocCnt, gOsalClockPeak;

void ClockP_timerCallbackFunction(timerHandleType xTimer)
{
    /*
     * TODO check what is the parameter the call back function is passing.
     * For now assuming it is passing the timer ID, which is index as
     * initialized at the time of creation.
     */
    portBaseType    *xTimerID = (portBaseType *)xTimer;
    ClockP_safertos *pTimer = &gOsalClockPSafeRtosPool[*xTimerID];

    if(pTimer != NULL_PTR && pTimer->callback && pTimer->used == TRUE)
    {
        pTimer->callback(pTimer->arg);
    }
}

void ClockP_Params_init(ClockP_Params *params)
{
    if(params != NULL_PTR)
    {
        params->pErrBlk     = NULL_PTR;
        params->startMode   = ClockP_StartMode_USER;
        params->period      = 0U;
        params->runMode     = ClockP_RunMode_ONESHOT;
        params->arg         = NULL_PTR;
    }

    return;
}

/*
 *  ======== ClockP_create ========
 */
ClockP_Handle ClockP_create(void *clockfxn,
                            const ClockP_Params *params)
{
    
    ClockP_safertos         *pTimer = (ClockP_safertos *)NULL_PTR;
    ClockP_safertos         *timerPool;
    portBaseType            uxAutoReload = pdFALSE;
    ClockP_Handle           ret_handle = NULL_PTR;
    uint32_t                i;
    uintptr_t               key;
    uint32_t                maxClocks;
    timerInitParametersType xTimerInitParameters;
    portBaseType            xCreateResult;

    /* Pick up the internal static memory block */
    timerPool = (ClockP_safertos *) &gOsalClockPSafeRtosPool[0];
    maxClocks  = OSAL_FREERTOS_CONFIGNUM_CLOCK; /* TODO add this macro */

    if(gOsalClockAllocCnt==0U) 
    {
        (void)memset((void *)gOsalClockPSafeRtosPool,0,sizeof(gOsalClockPSafeRtosPool));
    }

    key = HwiP_disable();

    for (i = 0; i < maxClocks; i++)
    {
        if (timerPool[i].used == FALSE)
        {
            timerPool[i].used = TRUE;
            /* Update statistics */
            gOsalClockAllocCnt++;
            if (gOsalClockAllocCnt > gOsalClockPeak)
            {
                gOsalClockPeak = gOsalClockAllocCnt;
            }
            break;
        }
    }
    HwiP_restore(key);

    if (i < maxClocks)
    {
        /* Grab the memory */
        pTimer = (ClockP_safertos *) &timerPool[i];
    }

    if((pTimer == NULL_PTR) || (params == NULL_PTR))
    {
        ret_handle = NULL_PTR;
    }
    else 
    {
        if(params->runMode == ClockP_RunMode_CONTINUOUS)
        {
            uxAutoReload = pdTRUE;
        }


        pTimer->callback = (ClockP_FxnCallback)clockfxn;
        pTimer->arg = params->arg;

        /* Update the xTimerInitParameters structure */

        /* pcTimerName */
        xTimerInitParameters.pcTimerName        = (const char *)NULL_PTR;
        /* xTimerPeriodInTicks */
        xTimerInitParameters.xTimerPeriodInTicks= params->period;
        /* xIsPeriodic */
        xTimerInitParameters.xIsPeriodic        = uxAutoReload;
        /* xTimerID is the index */
        xTimerInitParameters.xTimerID           = (portBaseType)i;
        /* pxNewTimer */
        xTimerInitParameters.pxNewTimer         = &pTimer->timerObj;
        /* pxCallbackFunction */
        xTimerInitParameters.pxCallbackFunction = ClockP_timerCallbackFunction;
        /* pxTimerInstance */
        xTimerInitParameters.pxTimerInstance    = NULL;
        /* pvObjec */
        xTimerInitParameters.pvObject           = NULL;

        xCreateResult = xTimerCreate(&xTimerInitParameters, pTimer->timerHndl);

        if(xCreateResult != pdPASS)
        {
            /* If there was an error reset the clock object and return NULL. */
            key = HwiP_disable();
            pTimer->used      = FALSE;
            /* Found the osal clock object to delete */
            if (gOsalClockAllocCnt > 0U)
            {
                gOsalClockAllocCnt--;
            }
            HwiP_restore(key);
            ret_handle = NULL_PTR;
        }
        else
        {
            if(params->startMode == ClockP_StartMode_AUTO)
            {
                xCreateResult = xTimerStart(pTimer->timerHndl, portMAX_DELAY);
                if(xCreateResult != pdPASS)
                {
                    /* If there was an error reset the clock object and return NULL. */
                    key = HwiP_disable();
                    pTimer->used      = FALSE;
                    /* Found the osal clock object to delete */
                    if (gOsalClockAllocCnt > 0U)
                    {
                        gOsalClockAllocCnt--;
                    }
                    HwiP_restore(key);
                    ret_handle = NULL_PTR;
                }
            }
            ret_handle = (ClockP_Handle)pTimer;
        }
    }
    return ret_handle;
}

/*
 *  ======== ClockP_delete ========
 */
ClockP_Status ClockP_delete(ClockP_Handle handle)
{
    ClockP_safertos *pTimer = (ClockP_safertos*)handle;
    uintptr_t       key;
    ClockP_Status   ret = ClockP_OK;
    portBaseType    xCreateResult;

    if ((pTimer != NULL_PTR) && (pTimer->used == TRUE))
    {
        xCreateResult = xTimerDelete(pTimer->timerHndl, portMAX_DELAY);

        if(xCreateResult == pdPASS)
        {
            key = HwiP_disable();
            pTimer->used      = FALSE;
            pTimer->timerHndl = NULL;
            pTimer->callback  = NULL;
            pTimer->arg       = NULL;
            if (gOsalClockAllocCnt > 0U)
            {
                gOsalClockAllocCnt--;
            }
            HwiP_restore(key);
            ret = ClockP_OK;
        }
        else
        {
            ret = ClockP_FAILURE;
        }
    }
    else
    {
        ret = ClockP_FAILURE;
    }

    return ret;
}

ClockP_Status ClockP_start(ClockP_Handle handle)
{
    ClockP_Status   ret       = ClockP_OK;
    ClockP_safertos *pTimer = (ClockP_safertos*)handle;
    portBaseType    xCreateResult;

    if ((pTimer != NULL_PTR) && (pTimer->used == TRUE))
    {
        if( xPortInIsrContext() )
        {
            portBaseType xHigherPriorityTaskWoken = 0;

            /* timeout is ignored when in ISR mode */
            xCreateResult = xTimerStartFromISR(pTimer->timerHndl, &xHigherPriorityTaskWoken);
            portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        }
        else
        {
            xCreateResult = xTimerStart(pTimer->timerHndl, portMAX_DELAY);
        }
        if(xCreateResult == pdPASS)
        {
            ret = ClockP_OK;
        }
        else
        {
            ret = ClockP_FAILURE;
        }

    }
    else
    {
        ret = ClockP_FAILURE;
    }

    return ret;
}

ClockP_Status ClockP_stop(ClockP_Handle handle)
{
    ClockP_Status   ret       = ClockP_OK;
    ClockP_safertos *pTimer   = (ClockP_safertos*)handle;
    portBaseType    xCreateResult;

    if ((pTimer != NULL_PTR) && (pTimer->used == TRUE))
    {
        if( xPortInIsrContext() )
        {
            portBaseType xHigherPriorityTaskWoken = 0;

            /* timeout is ignored when in ISR mode */
            xCreateResult = xTimerStopFromISR(pTimer->timerHndl, &xHigherPriorityTaskWoken);
            portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        }
        else
        {
            xCreateResult = xTimerStop(pTimer->timerHndl, portMAX_DELAY);
        }
        if(xCreateResult == pdPASS)
        {
            ret = ClockP_OK;
        }
        else
        {
            ret = ClockP_FAILURE;
        }
    }
    else
    {
        ret = ClockP_FAILURE;
    }

    return ret;
}

/* Nothing past this point */
