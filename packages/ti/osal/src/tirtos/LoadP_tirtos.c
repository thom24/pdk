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
 *  ======== LoadP_tirtos.c ========
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */


#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ti/osal/TaskP.h>
#include <ti/osal/LoadP.h>
#include <ti/sysbios/utils/Load.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/osal/src/tirtos/tirtos_config.h>


/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

typedef struct LoadP_taskLoadObj_s
{
    bool         used;
    TaskP_Handle pTsk;
    char         name[LOADP_STATS_TASK_NAME_MAX];
    uint64_t     threadTime;
} LoadP_taskLoadObj;

typedef struct LoadP_tirtos_s
{   
    LoadP_taskLoadObj taskLoadObj[LOADP_STATS_TASK_MAX];
    uint64_t          idlTskTime;
    uint64_t          totalTime;
} LoadP_tirtos;

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

static LoadP_tirtos     gLoadP_tirtos;
static bool             gLoadP_startLoadCalc = FALSE;
static bool             gLoadP_initDone = FALSE;

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
            (void)memset( (void *)&gLoadP_tirtos,0,sizeof(gLoadP_tirtos));
            gLoadP_initDone = TRUE;
        }

        for (i = 0; i < LOADP_STATS_TASK_MAX; i++)
        {
            pHndl = &gLoadP_tirtos.taskLoadObj[i];

            if (pHndl->used == FALSE)
            {
                pHndl->used        = TRUE;
                pHndl->pTsk        = taskHandle;
                pHndl->threadTime  = 0U;
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
            pHndl = &gLoadP_tirtos.taskLoadObj[i];

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
    uintptr_t           key;
    uint32_t            i;
    LoadP_taskLoadObj   *pHndl;
    
    if(!gLoadP_initDone)
    {
        (void)memset( (void *)&gLoadP_tirtos,0,sizeof(gLoadP_tirtos));
        gLoadP_initDone = TRUE;
    }

    Load_update();
    
    key = HwiP_disable();
    gLoadP_tirtos.idlTskTime = 0U;
    gLoadP_tirtos.totalTime  = 0U;
    
    for (i = 0; i < LOADP_STATS_TASK_MAX; i++)
    {
        pHndl = &gLoadP_tirtos.taskLoadObj[i];

        if (pHndl->used == TRUE)
        {
            pHndl->threadTime = 0U;
        }
    }

    gLoadP_startLoadCalc = TRUE;

    HwiP_restore(key);

}

void LoadP_stop(void)
{
    uintptr_t    key;

    Load_update();
    if(!Load_updateInIdle)
    {
        LoadP_update();
    }

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
        Load_update();
        if(!Load_updateInIdle)
        {
            LoadP_update();
        }
        for (i = 0; i < LOADP_STATS_TASK_MAX; i++)
        {
            pHndl = &gLoadP_tirtos.taskLoadObj[i];

            if ((pHndl->used == TRUE) && (pHndl->pTsk == taskHandle))
            {
                stats->threadTime  = pHndl->threadTime;
                stats->totalTime   = gLoadP_tirtos.totalTime;
                stats->percentLoad = (uint32_t)(pHndl->threadTime / (gLoadP_tirtos.totalTime/(uint64_t)100U));
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
    Load_update();
    if(!Load_updateInIdle)
    {
        LoadP_update();
    }
    return ((uint32_t) 100U - (uint32_t) (gLoadP_tirtos.idlTskTime / (gLoadP_tirtos.totalTime/(uint64_t)100U)));
}

/* Function called by Load_update for each update cycle */
void LoadP_update(void)
{
    uint32_t            i;
    Load_Stat           tskLoadStat, idlTskLoadStat;
    LoadP_taskLoadObj   *pHndl;

    if(gLoadP_startLoadCalc)
    {
        Load_getTaskLoad(Task_getIdleTask(), &idlTskLoadStat);

        gLoadP_tirtos.totalTime   += idlTskLoadStat.totalTime;
        gLoadP_tirtos.idlTskTime  += idlTskLoadStat.threadTime;

        /* Call the load updated function of each registered task one by one */
        for (i = 0; i < LOADP_STATS_TASK_MAX; i++)
        {
            pHndl = &gLoadP_tirtos.taskLoadObj[i];

            if ((pHndl->used == TRUE) && (pHndl->pTsk != NULL_PTR))
            {
                Load_getTaskLoad((Task_Handle)pHndl->pTsk, &tskLoadStat);
                pHndl->threadTime += tskLoadStat.threadTime;
            }
        }
    }

    return;
}

