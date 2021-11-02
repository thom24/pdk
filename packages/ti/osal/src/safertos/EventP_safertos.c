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
 *  ======== EventP_safertos.c ========
 */

#include <ti/osal/EventP.h>

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ti/csl/csl_types.h>
#include <ti/osal/osal.h>
#include <ti/osal/soc/osal_soc.h>

#include <SafeRTOS_API.h>
#include <eventgroups.h>

extern portBaseType xPortInIsrContext( void );

uint32_t  gOsalEventAllocCnt, gOsalEventPeak;

/*!
 *  @brief    EventP_safertos structure
 */
typedef struct EventP_safertos_s
{
    bool                    used;
    eventGroupType          eventObj;
    eventGroupHandleType    eventHndl;
} EventP_safertos;

/* global pool of statically allocated event pools */
static EventP_safertos gOsalEventPSafeRtosPool[OSAL_FREERTOS_CONFIGNUM_EVENT];

/*
 *  ======== EventP_Params_init ========
 */
void EventP_Params_init(EventP_Params *params)
{

}

/*
 *  ======== EventP_create ========
 */
EventP_Handle EventP_create(EventP_Params *params)
{
    EventP_Handle       ret_handle;
    EventP_safertos     *handle = (EventP_safertos *) NULL_PTR;
    EventP_safertos     *eventPool;
    uint32_t            i;
    uintptr_t           key;
    uint32_t            maxEvent;
    portBaseType        xCreateResult;

    DebugP_assert((params != NULL_PTR));

    /* Pick up the internal static memory block */
    eventPool       = (EventP_safertos *) &gOsalEventPSafeRtosPool[0];
    maxEvent        = OSAL_FREERTOS_CONFIGNUM_EVENT;/* TODO this needs to be updated */
    
    if(gOsalEventAllocCnt==0U)
    {
        (void)memset( (void *)gOsalEventPSafeRtosPool,0,sizeof(gOsalEventPSafeRtosPool));
    }

    key = HwiP_disable();

     for (i = 0; i < maxEvent; i++)
     {
         if (eventPool[i].used == FALSE)
         {
             eventPool[i].used = TRUE;
             /* Update statistics */
             gOsalEventAllocCnt++;
             if (gOsalEventAllocCnt > gOsalEventPeak)
             {
                 gOsalEventPeak = gOsalEventAllocCnt;
             }
             break;
         }
     }
    HwiP_restore(key);

    if (i < maxEvent)
    {
        /* Grab the memory */
        handle = (EventP_safertos *) &eventPool[i];
    }

    if (handle == NULL_PTR) {
        ret_handle = NULL_PTR;
    }
    else
    {
        xCreateResult = xEventGroupCreate(&(handle->eventObj), &handle->eventHndl);
        if((xCreateResult == errNULL_PARAMETER_SUPPLIED) || (xCreateResult == errEVENT_GROUP_ALREADY_IN_USE))
        {
            /* If there was an error reset the event object and return NULL. */
            key = HwiP_disable();
            handle->used = FALSE;
            /* Found the osal event object to delete */
            if (gOsalEventAllocCnt > 0U)
            {
                gOsalEventAllocCnt--;
            }
            HwiP_restore(key);
            ret_handle = NULL_PTR;
        }
        if(xCreateResult == pdPASS)
        {
            ret_handle = ((EventP_Handle)handle);
        }
    }

    return ret_handle;
}

/*
 *  ======== EventP_delete ========
 */
EventP_Status EventP_delete(EventP_Handle *handle)
{
    DebugP_assert((handle != NULL_PTR));

    uintptr_t       key;
    EventP_Status   ret_val = EventP_OK;
    EventP_safertos *event = (EventP_safertos *)*handle;
    portBaseType    xCreateResult;

    if((event != NULL_PTR) && (event->used==TRUE))
    {
        xCreateResult = xEventGroupDelete(event->eventHndl);

        if(xCreateResult == pdPASS)
        {
            key = HwiP_disable();
            event->used = FALSE;
            /* Found the osal event object to delete */
            if (gOsalEventAllocCnt > 0U)
            {
                gOsalEventAllocCnt--;
            }
            HwiP_restore(key);
            ret_val = EventP_OK;
        }
        else
        {
            ret_val = EventP_FAILURE;
        }
    }
    else
    {
       ret_val = EventP_FAILURE;
    }  

    return ret_val;
}

/*
 *  ======== EventP_wait ========
 */
uint32_t EventP_wait(EventP_Handle handle, uint32_t eventMask,
                    uint8_t waitMode, uint32_t timeout)
{
    DebugP_assert((handle != NULL_PTR));
    DebugP_assert(eventMask <= EventP_ID_23);
    DebugP_assert(eventMask != EventP_ID_NONE);

    EventP_safertos *event = (EventP_safertos *)handle;

    eventBitsType pxEventBitsSet = 0;
    uint32_t        eventBits = 0U;
    portBaseType    xCreateResult;

    if((event != NULL_PTR) && (event->used==TRUE) && (waitMode <= EventP_WaitMode_ALL))
    {
        xCreateResult = xEventGroupWaitBits(event->eventHndl,
                                                  (eventBitsType)eventMask,
                                                  (portBaseType)1U,
                                                  (portBaseType)waitMode,
                                                  &pxEventBitsSet,
                                                  (portTickType)timeout);
        if(xCreateResult == pdPASS)
        {
            /* all the event mask bits heve been set, so updating eventBits with eventMask*/
            eventBits = pxEventBitsSet;
        }
        else
        {
            /* There was en error so updating eventBits with zero */
            eventBits = 0U;
        }
    }

    return eventBits;    
}

/*
 *  ======== EventP_post ========
 */
EventP_Status EventP_post(EventP_Handle handle, uint32_t eventMask)
{
    DebugP_assert((handle != NULL_PTR));
    DebugP_assert(eventMask <= EventP_ID_23);

    EventP_Status   ret_val = EventP_OK;
    EventP_safertos *event = (EventP_safertos *)handle;
    portBaseType    xCreateResult;
    
    if((event != NULL_PTR) && (event->used==TRUE))
    {
        if( xPortInIsrContext() )
        {   
            portBaseType xHigherPriorityTaskWoken = pdFALSE;

            xCreateResult  = xEventGroupSetBitsFromISR(event->eventHndl,
                                                 (eventBitsType)eventMask,
                                                 &xHigherPriorityTaskWoken);

            if(xCreateResult == pdPASS)
            {
                portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
                ret_val = EventP_OK;
            }
            else
            {
                ret_val = EventP_FAILURE;
            }
        }
        else
        {
            xCreateResult  = xEventGroupSetBits(event->eventHndl, (eventBitsType)eventMask);
            if(xCreateResult == pdPASS)
            {
                ret_val = EventP_OK;
            }
            else
            {
                ret_val = EventP_FAILURE;
            }
        }
    }
    else
    {
        ret_val = EventP_FAILURE;
    }

    return ret_val;
}

/*
 *  ======== EventP_getPostedEvents ========
 */
uint32_t EventP_getPostedEvents(EventP_Handle handle)
{
    DebugP_assert((handle != NULL_PTR));

    EventP_safertos *event = (EventP_safertos *)handle;
    uint32_t        eventBitsGet = 0U;
    portBaseType    xCreateResult;
    
    if((event != NULL_PTR) && (event->used==TRUE))
    {
        if( xPortInIsrContext() )
        {   
            xCreateResult = xEventGroupGetBitsFromISR(event->eventHndl, (eventBitsType *)&eventBitsGet);
            if(xCreateResult != pdPASS)
            {
                /* error has occurred, updating eventBitsGet to zero */
                /* TODO this can be updated to 0xFF to report error */
                eventBitsGet = 0U;
            }
        }
        else
        {
            xCreateResult = xEventGroupGetBits(event->eventHndl, (eventBitsType *)&eventBitsGet);
            if(xCreateResult != pdPASS)
            {
                /* error has occurred, updating eventBitsGet to zero */
                /* TODO this can be updated to 0xFF to report error */
                eventBitsGet = 0U;
            }
        }
    }

    return eventBitsGet;
}
