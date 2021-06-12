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
 *  ======== LoadP_freertos.c ========
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <ti/osal/TaskP.h>
#include <ti/osal/LoadP.h>

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ti/csl/csl_types.h>
#include <ti/osal/osal.h>
#include <ti/osal/DebugP.h>
#include <ti/osal/soc/osal_soc.h>

#include <FreeRTOS.h>
#include <task.h>


/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

typedef struct LoadP_taskLoadObj_s
{
    bool                used;
    TaskP_Handle        pTsk;
    bool                taskDeleted;
    char                name[LOADP_STATS_TASK_NAME_MAX];
    uint64_t            threadTime;
    uint32_t            lastUpdate_threadTime;
} LoadP_taskLoadObj;


typedef struct LoadP_freertos_s
{   
    LoadP_taskLoadObj taskLoadObj[LOADP_STATS_TASK_MAX];
    uint64_t          idlTskTime;
    uint64_t          totalTime;
    uint32_t          lastUpdate_idlTskTime;
    uint32_t          lastUpdate_totalTime;
} LoadP_freertos;

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

static void Load_updateTaskLoad(LoadP_taskLoadObj *pHndl, bool init, bool delete);
static void Load_init(void);
extern TaskHandle_t TaskP_getFreertosHandle(TaskP_Handle handle);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

static LoadP_freertos   gLoadP_freertos;
static bool             gLoadP_initDone = FALSE;
bool                    gLoadP_startLoadCalc = FALSE;

uint32_t    gLoadP_taskRegCnt = 0U , gLoadP_taskRegPeak = 0U;

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

LoadP_Status LoadP_registerTask(TaskP_Handle taskHandle, const char *name)
{
    LoadP_Status        ret_val = LoadP_FAILURE; 
    LoadP_taskLoadObj   *pHndl = (LoadP_taskLoadObj *)NULL_PTR;
    uint32_t            i;
    uintptr_t           key;

    if((taskHandle != NULL_PTR) && (name != NULL_PTR))
    {      
        key = HwiP_disable();

        if(!gLoadP_initDone)
        {
            (void)memset( (void *)&gLoadP_freertos,0,sizeof(gLoadP_freertos));
            gLoadP_initDone = TRUE;
        }

        for (i = 0; i < LOADP_STATS_TASK_MAX; i++)
        {
            pHndl = &gLoadP_freertos.taskLoadObj[i];

            if (pHndl->used == FALSE)
            {
                pHndl->used        = TRUE;
                pHndl->pTsk        = taskHandle;
                pHndl->taskDeleted = FALSE;
                Load_updateTaskLoad(pHndl, TRUE, FALSE); 
                strncpy(pHndl->name, name, LOADP_STATS_TASK_NAME_MAX);
                pHndl->name[LOADP_STATS_TASK_NAME_MAX-1]=0;

                /* Update statistics */
                gLoadP_taskRegCnt++;
                if (gLoadP_taskRegCnt > gLoadP_taskRegPeak)
                {
                    gLoadP_taskRegPeak = gLoadP_taskRegCnt;
                }

                ret_val = LoadP_OK;

                break;
            }
        }

        HwiP_restore(key);
    }

    return ret_val;
}

LoadP_Status LoadP_unRegisterTask(TaskP_Handle taskHandle)
{
    uintptr_t           key;
    LoadP_Status        ret_val = LoadP_FAILURE;
    uint32_t            i;
    LoadP_taskLoadObj   *pHndl = (LoadP_taskLoadObj *)NULL_PTR;

    if(taskHandle != NULL_PTR)
    {
        key = HwiP_disable();

        for (i = 0; i < LOADP_STATS_TASK_MAX; i++)
        {
            pHndl = &gLoadP_freertos.taskLoadObj[i];

            if ((pHndl->used == TRUE) && (pHndl->pTsk == taskHandle))
            {
                pHndl->used = FALSE;

                /* Found the osal load object to delete */
                if (gLoadP_taskRegCnt > 0U)
                {
                    gLoadP_taskRegCnt--;
                }
                
                ret_val = LoadP_OK;
            }
        }

        HwiP_restore(key);
    }

    return ret_val;
}

void LoadP_start(void)
{
    uintptr_t    key;
    
    if(!gLoadP_initDone)
    {
        (void)memset( (void *)&gLoadP_freertos,0,sizeof(gLoadP_freertos));
        gLoadP_initDone = TRUE;
    }
    
    Load_init();
    
    key = HwiP_disable();
    gLoadP_startLoadCalc = TRUE;
    HwiP_restore(key);

}

void LoadP_stop(void)
{
    uintptr_t    key;

    LoadP_update();

    key = HwiP_disable();
    gLoadP_startLoadCalc = FALSE;
    HwiP_restore(key);
}

LoadP_Status LoadP_getTaskLoad(TaskP_Handle taskHandle, LoadP_Stats *stats)
{
    uint32_t            i;
    LoadP_taskLoadObj   *pHndl;
    LoadP_Status        ret_val = LoadP_FAILURE;

    if(stats != NULL_PTR)
    {
        LoadP_update();
        for (i = 0; i < LOADP_STATS_TASK_MAX; i++)
        {
            pHndl = &gLoadP_freertos.taskLoadObj[i];

            if ((pHndl->used == TRUE) && (pHndl->pTsk == taskHandle))
            {
                stats->threadTime  = pHndl->threadTime;
                stats->totalTime   = gLoadP_freertos.totalTime;
                stats->percentLoad = (uint32_t)(pHndl->threadTime / (gLoadP_freertos.totalTime/(uint64_t)100U));
                strncpy(stats->name, pHndl->name, LOADP_STATS_TASK_NAME_MAX);
                stats->name[LOADP_STATS_TASK_NAME_MAX-1]=0;

                ret_val = LoadP_OK;
            }
        }
    }

    return ret_val;
}

uint32_t LoadP_getCPULoad(void)
{
    LoadP_update();

    return ((uint32_t) 100U - (uint32_t) (gLoadP_freertos.idlTskTime / (gLoadP_freertos.totalTime/(uint64_t)100U)));;
}


void LoadP_update(void)
{
    uint32_t            i;
    uint32_t            curTime;
    uint32_t            delta;
    LoadP_taskLoadObj   *pHndl;
    uintptr_t           key;

    if(gLoadP_startLoadCalc)
    {
        key = HwiP_disable();

        curTime = (uint32_t)ulTaskGetIdleRunTimeCounter();
        delta = curTime - gLoadP_freertos.lastUpdate_idlTskTime;
        gLoadP_freertos.lastUpdate_idlTskTime = curTime;

        gLoadP_freertos.idlTskTime += delta;
        
        curTime = (uint32_t)uiPortGetRunTimeCounterValue();
        delta = curTime - gLoadP_freertos.lastUpdate_totalTime;
        gLoadP_freertos.lastUpdate_totalTime = curTime;

        gLoadP_freertos.totalTime += delta;

        HwiP_restore(key);

        for (i = 0; i < LOADP_STATS_TASK_MAX; i++)
        {
            pHndl = &gLoadP_freertos.taskLoadObj[i];

            if ((pHndl->used == TRUE) && (pHndl->pTsk != NULL_PTR))
            {
                Load_updateTaskLoad(pHndl, FALSE, FALSE);
            }
        }
    }

    return;
}

/* ========================================================================================================================== */

void Load_init(void)
{
    uint32_t            i;
    LoadP_taskLoadObj   *pHndl;
    uintptr_t           key;

    key = HwiP_disable();

    gLoadP_freertos.idlTskTime = 0U;
    gLoadP_freertos.lastUpdate_idlTskTime = (uint32_t) ulTaskGetIdleRunTimeCounter();

    gLoadP_freertos.totalTime = 0U;
    gLoadP_freertos.lastUpdate_totalTime = (uint32_t) uiPortGetRunTimeCounterValue();

    HwiP_restore(key);

    for (i = 0; i < LOADP_STATS_TASK_MAX; i++)
    {
        pHndl = &gLoadP_freertos.taskLoadObj[i];

        if ((pHndl->used == TRUE) && (pHndl->pTsk != NULL_PTR))
        {
            Load_updateTaskLoad(pHndl, TRUE, FALSE); 
        }
    }

    return;
}

void Load_updateTaskLoad(LoadP_taskLoadObj *pHndl, bool init, bool delete)
{
    TaskStatus_t    tskStat;
    uint32_t        delta;
    uintptr_t       key;
    
    if(!pHndl->taskDeleted)
    {
        key = HwiP_disable();

        vTaskGetInfo( TaskP_getFreertosHandle(pHndl->pTsk),
                    &tskStat,
                    pdFALSE,
                    eRunning  );
        if(init)
        {
            pHndl->threadTime = 0U;
        }
        else
        {
            delta =  tskStat.ulRunTimeCounter - pHndl->lastUpdate_threadTime;
            pHndl->threadTime += delta;
        }
        pHndl->lastUpdate_threadTime = tskStat.ulRunTimeCounter;
        
        if(delete)
        {
            pHndl->taskDeleted = TRUE;
        }

        HwiP_restore(key);
    }
}

/* Load_updateDeleteTaskLoad Internal API is used to update the load of a task just before deleting.
 * This function gets invoked from TaskP_delete() API, if Load measuremnent is started.
 * This function will check if the task was registered for load measurement
 * and update the load if registered. Also sets the the taskDeleted flag in LoadP_taskLoadObj.
 * This is necessary because, once the task is deleted,
 * TaskP_getFreertosHandle can't return the TaskHandle_t
 * and it won't be possible to get the task stats with vTaskGetInfo API */
void Load_updateDeleteTaskLoad(TaskP_Handle taskHandle)
{
    uint32_t            i;
    LoadP_taskLoadObj   *pHndl;
    
    for (i = 0; i < LOADP_STATS_TASK_MAX; i++)
    {
        pHndl = &gLoadP_freertos.taskLoadObj[i];

        if ((pHndl->used == TRUE) && (pHndl->pTsk == taskHandle))
        {
            Load_updateTaskLoad(pHndl, FALSE, TRUE);
        }
    }
}

