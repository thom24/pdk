/*
 * Copyright (c) 2015-2020, Texas Instruments Incorporated
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
/**
 *  \ingroup DRV_OSAL_MODULE
 *  \defgroup DRV_OSAL_LoadP LoadP
 *            LoadP interface
 *
 *  @{
 */

/** ============================================================================
 *  @file       LoadP.h
 *
 *  @brief      Load module to measure CPU Load and Task Load for the RTOS Porting Interface
 *
 *
 *  ============================================================================
 */

#ifndef ti_osal_LoadP__include
#define ti_osal_LoadP__include

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <ti/osal/TaskP.h>

/** \brief Max size of task name string */
#define LOADP_STATS_TASK_NAME_MAX (32U)

/** \brief Max number of tasks whose information can be retrived */
#define LOADP_STATS_TASK_MAX (20U)

/*!
 *  @brief    Status codes for LoadP APIs
 */
typedef enum LoadP_Status_e
{
    /*! API completed successfully */
    LoadP_OK         = 0,
    /*! API failed */
    LoadP_FAILURE    = (-(int32_t)1)
} LoadP_Status;

/*!
 * @brief Load statistics info
 */
typedef struct LoadP_Stats_s
{
    char         name[LOADP_STATS_TASK_NAME_MAX];    /*!< Registered thread name */
    uint64_t     threadTime;                         /*!< Runtime for the thread */
    uint64_t     totalTime;                          /*!< Total Runtime */
    uint32_t     percentLoad;                        /*!< Percentage load of the thread */
} LoadP_Stats;

/*!
 *  @brief  Function to register a task for load measurement
 *  
 *  @param  taskHandle [in] A TaskP_Handle returned from \ref TaskP_create,
 *                          to be registered for task load measurement
 *  @param  name       [in] Task Name to be stored along with Load stats
 * 
 *  @return Status of the function
 *    - LoadP_OK: Registered the task for load measurement
 *    - LoadP_FAILURE: Failed to Register the task for load measurement
 */
extern LoadP_Status LoadP_registerTask(TaskP_Handle taskHandle, const char *name);

/*!
 *  @brief  Function to un-register a task from load measurement
 *  
 *  @param  taskHandle [in] A TaskP_Handle returned from \ref TaskP_create,
 *                          to be un-registered from task load measurement
 * 
 *  @return Status of the function
 *    - LoadP_OK: Un-registered the task from load measurement
 *    - LoadP_FAILURE: Failed to Un-register the task from load measurement
 */
extern LoadP_Status LoadP_unRegisterTask(TaskP_Handle taskHandle);

/*!
 *  @brief  Function to start load measurement
 *
 *          This starts the CPU load measurement and that for all the registered 
 *          tasks with \ref LoadP_registerTask
 */
extern void LoadP_start(void);

/*!
 *  @brief  Function to stop load measurement
 *
 *          This ends the CPU load measurement and that for all the registered 
 *          tasks with \ref LoadP_registerTask.
 */
extern void LoadP_stop(void);

/*!
 *  @brief  Function to get task load statistics
 *        
 *  @param  taskHandle [in]  A TaskP_Handle returned from \ref TaskP_create
 *                           and registered for load measurement using \ref LoadP_registerTask
 *  @param  stats      [out] Returned Task Load stats  
 * 
 *  @return Status of the function
 *    - LoadP_OK: LoadP_Stats populated with the task load statistics
 *    - LoadP_FAILURE: Failed to populate LoadP_Stats with the task load statistics
 */
extern LoadP_Status LoadP_getTaskLoad(TaskP_Handle taskHandle, LoadP_Stats *stats);

/*!
 *  @brief  Function to get CPU load in percentage
 *          
 *  @return CPU load in percentage
 */
extern uint32_t LoadP_getCPULoad(void);

/*!
 *  @brief  Callback Fxn to be executed post Load Update
 *          
 *          In case of TI-RTOS, This function is to be called by SYSBIOS 
 *          API Load_update for each update cycle.
 *          User can set this function as the 'Load.postUpdate'
 *          in the cfg file, when 'Load.updateInIdle' is enabled.
 *          For example,
 *         
 *              var Load = xdc.useModule('ti.sysbios.utils.Load');
 *              
 *              Load.taskEnabled  = true;
 *              Load.updateInIdle = true;
 *              Load.windowInMs   = 500;
 *              Load.postUpdate   = '&LoadP_update';
 *          
 *          In case of FreeRTOS, FreeRTOSConfig.h enables update in idle
 *          and idle task calls this API.
 *          FreeRTOSConfig.h:-
 * 
 *              #define configLOAD_UPDATE_IN_IDLE   (1)
 *              #define configLOAD_WINDOW_IN_MS     (500)
 *          
 *          This function is not intended to be invoked by user.
 */
extern void LoadP_update(void);


#ifdef __cplusplus
}
#endif

#endif /* ti_osal_LoadP__include */
/* @} */
