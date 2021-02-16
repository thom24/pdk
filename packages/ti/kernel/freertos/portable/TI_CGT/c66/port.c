/*
 * FreeRTOS Kernel V10.4.1
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * https://www.FreeRTOS.org
 * https://github.com/FreeRTOS
 *
 * 1 tab == 4 spaces!
 */
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
#include <stdint.h>
#include <stdio.h>
#include <FreeRTOS.h>
#include <task.h>
#include <ti/osal/osal.h>
#include <ti/osal/HwiP.h>
#include <ti/osal/DebugP.h>



/* Let the user override the pre-loading of the initial LR with the address of
 * prvTaskExitError() in case is messes up unwinding of the stack in the
 * debugger. */
#ifdef configTASK_RETURN_ADDRESS
    #define portTASK_RETURN_ADDRESS    configTASK_RETURN_ADDRESS
#else
    #define portTASK_RETURN_ADDRESS    prvTaskExitError
#endif


/* A critical section is exited when the critical section nesting count reaches
 * this value. */
#define portNO_CRITICAL_NESTING          ( ( uint32_t ) 0 )

/* Tasks are not created with a floating point context, but can be given a
 * floating point context after they have been created.  A variable is stored as
 * part of the tasks context that holds portNO_FLOATING_POINT_CONTEXT if the task
 * does not have an FPU context, or any other value if the task does have an FPU
 * context. */
#define portNO_FLOATING_POINT_CONTEXT    ( ( StackType_t ) 0 )

/* A variable is used to keep track of the critical section nesting.  This
 * variable has to be stored as part of the task context and must be initialised to
 * a non zero value to ensure interrupts don't inadvertently become unmasked before
 * the scheduler starts.  As it is stored as part of the task context it will
 * automatically be set to 0 when the first task is started. */
volatile uint32_t ulCriticalNesting = 9999UL;

/* Saved as part of the task context.  If ulPortTaskHasFPUContext is non-zero then
 * a floating point context must be saved and restored for the task. */
uint32_t ulPortTaskHasFPUContext = pdFALSE;

/* Set to 1 to pend a context switch from an ISR. */
uint32_t ulPortYieldRequired = pdFALSE;

/* Counts the interrupt nesting depth.  A context switch is only performed if
 * if the nesting depth is 0. */
uint32_t ulPortInterruptNesting = 0UL;

/* set to true when schedular gets enabled in xPortStartScheduler */
uint32_t ulPortSchedularRunning = pdFALSE;

/*
 * Task control block.  A task control block (TCB) is allocated for each task,
 * and stores task state information, including a pointer to the task's context
 * (the task's run time environment, including register values)
 */
typedef struct tskTaskControlBlockBaseClass       /* The old naming convention is used to prevent breaking kernel aware debuggers. */
{
    volatile void * pxTopOfStack; /*< Points to the location of the last item placed on the tasks stack.  THIS MUST BE THE FIRST MEMBER OF THE TCB STRUCT. */
} tskTaskControlBlockBaseClass_t;

extern tskTaskControlBlockBaseClass_t * pxCurrentTCB;
/*
 * Starts the first task executing.  This function is necessarily written in
 * assembly code so is implemented in portASM.s.
 */
extern void vPortRestoreTaskContext( void );

extern volatile cregister uint32_t TSCL, TSCH;

static void prvTaskExitError( void )
{
    /* A function that implements a task must not exit or attempt to return to
     * its caller as there is nothing to return to.  If a task wants to exit it
     * should instead call vTaskDelete( NULL ).
     *
     * Force an assert() to be triggered if configASSERT() is
     * defined, then stop here so application writers can catch the error. */
    DebugP_assert(0);
}

#define TaskSupport_buildTaskStack ti_sysbios_family_c62_TaskSupport_buildTaskStack

typedef void (*Task_FuncPtr)(uint32_t arg1, uint32_t arg2);

typedef void (*TaskSupport_FuncPtr)(void);


extern void * TaskSupport_buildTaskStack(void * stack, Task_FuncPtr fxn, TaskSupport_FuncPtr exit, TaskSupport_FuncPtr enter, uint32_t arg0, uint32_t arg1);
/*
 *  ======== Task_exit ========
 */
void Task_exit(void)
{
    prvTaskExitError();
}

void Task_enter(void)
{

}

void ti_sysbios_knl_Task_Func(uint32_t arg1, uint32_t arg2)
{
    TaskFunction_t pxCode = (TaskFunction_t)arg2;
    pxCode((void *)arg1);
}

#if ( portHAS_STACK_OVERFLOW_CHECKING == 1 )
StackType_t *pxPortInitialiseStack(StackType_t * pxTopOfStack, StackType_t * pxEndOfStack, TaskFunction_t pxCode, void * pvParameters )
#else
StackType_t *pxPortInitialiseStack(StackType_t * pxTopOfStack, TaskFunction_t pxCode, void * pvParameters )
#endif
{
#if ( portHAS_STACK_OVERFLOW_CHECKING == 1 )
    {
        (void) pxEndOfStack;
    }
#endif

    pxTopOfStack = TaskSupport_buildTaskStack(pxTopOfStack, ti_sysbios_knl_Task_Func, Task_exit, Task_enter, (uint32_t)pvParameters, (uint32_t)pxCode);

    return pxTopOfStack;
}

extern void Hwi_switchFromBootStack(void);
extern void Hwi_Module_startup(void);

TimerP_Handle pTickTimerHandle = NULL;

static void prvPorttimerTickIsr(uintptr_t args)
{
    void vPortTimerTickHandler();

    vPortTimerTickHandler();
}

static void prvPortInitTickTimer(void)
{

    TimerP_Params timerParams;

    TimerP_Params_init(&timerParams);
    timerParams.runMode    = TimerP_RunMode_CONTINUOUS;
    timerParams.startMode  = TimerP_StartMode_USER;
    timerParams.periodType = TimerP_PeriodType_MICROSECS;
    timerParams.period     = (portTICK_PERIOD_MS * 1000);
    timerParams.intNum     = configTIMER_INT_NUM;

    pTickTimerHandle = TimerP_create(configTIMER_ID, &prvPorttimerTickIsr, &timerParams);

    /* don't expect the handle to be null */
    DebugP_assert (pTickTimerHandle != NULL);

}

static void prvPortStartTickTimer(void)
{
    TimerP_Status status;
    status = TimerP_start(pTickTimerHandle);

    /* don't expect the handle to be null */
    DebugP_assert (status == TimerP_OK);

}

BaseType_t xPortStartScheduler(void)
{
    /* Interrupts are turned off in the CPU itself to ensure tick does
     * not execute	while the scheduler is being started.  Interrupts are
     * automatically turned back on in the CPU when the first task starts
     * executing.
     */
    portDISABLE_INTERRUPTS();

    Hwi_Module_startup();
    prvPortInitTickTimer();
    Hwi_switchFromBootStack();
    
    /* Start the ISR handling of the timer that generates the tick ISR. */
    prvPortStartTickTimer();
    ulPortSchedularRunning = pdTRUE;

    /* Start the first task executing. */
    vPortRestoreTaskContext();

    /* Will only get here if vTaskStartScheduler() was called with the CPU in
     * a non-privileged mode or the binary point register was not set to its lowest
     * possible value.  prvTaskExitError() is referenced to prevent a compiler
     * warning about it being defined but not referenced in the case that the user
     * defines their own exit address. */
    ( void ) prvTaskExitError;

    return pdTRUE;
}

void vPortYeildFromISR( uint32_t xSwitchRequired )
{
    if( xSwitchRequired != pdFALSE )
    {
        ulPortYieldRequired = pdTRUE;
    }
}

void vPortTimerTickHandler()
{
    if( ulPortSchedularRunning == pdTRUE )
    {
        /* Increment the RTOS tick. */
        if( xTaskIncrementTick() != pdFALSE )
        {
            ulPortYieldRequired = pdTRUE;
        }
    }
}

void vPortTaskUsesFPU( void )
{
    /* A task is registering the fact that it needs an FPU context.  Set the
     * FPU flag (which is saved as part of the task context). */
    ulPortTaskHasFPUContext = pdTRUE;

}


/* initialize high resolution timer for CPU and task load calculation */
void vPortConfigTimerForRunTimeStats()
{

    /* we assume clock is initialized before the schedular is started */
    /* start TSC */
    TSCL = 0;
}

/* return current counter value of high speed counter in units of 10's of usecs */
uint32_t uiPortGetRunTimeCounterValue()
{
    uint64_t ts = ((uint64_t)TSCL | ((uint64_t) TSCH << 32U));
    uint64_t timeInUsecs;

    timeInUsecs = (ts * 1000000) / configCPU_CLOCK_HZ;
    /* note, there is no overflow protection for this 32b value in FreeRTOS
     *
     * Dividing by 10 to reduce the resolution and avoid overflow for longer duration
     * This will overflow after
     * ((0x100000000/1000000)/(60*60))*10 hours ~ 12 hrs
     */
    return (uint32_t)(timeInUsecs/10);
}

/* This is used to make sure we are using the FreeRTOS API from within a valid interrupt priority level
 * In our R%F port this means IRQ.
 * i.e FreeRTOS API should not be called from FIQ, however right now we dont enforce it by checking
 * if we are in FIQ when this API is called.
 */
void vPortValidateInterruptPriority()
{
}

/* This is called as part of vTaskEndScheduler(), in our port, there is nothing to do here.
 * interrupt are disabled by FreeRTOS before calling this.
 */
void vPortEndScheduler()
{
    /* nothing to do */
}

/* configCHECK_FOR_STACK_OVERFLOW is set to 1, so the application must provide an
 * implementation of vApplicationStackOverflowHook()
 */
void vApplicationStackOverflowHook( TaskHandle_t xTask,
                                    char * pcTaskName )
{
    DebugP_log1("[FreeRTOS] Stack overflow detected for task [%s]", (uintptr_t)pcTaskName);
    DebugP_assert(0);
}

/* configSUPPORT_STATIC_ALLOCATION is set to 1, so the application must provide an
 * implementation of vApplicationGetIdleTaskMemory() to provide the memory that is
 * used by the Idle task.
 */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer,
                                    StackType_t **ppxIdleTaskStackBuffer,
                                    uint32_t *pulIdleTaskStackSize )
{
/* If the buffers to be provided to the Idle task are declared inside this
 * function then they must be declared static – otherwise they will be allocated on
 * the stack and so not exists after this function exits.
 */
static StaticTask_t xIdleTaskTCB;
static StackType_t uxIdleTaskStack[ configMINIMAL_STACK_SIZE ];

    /* Pass out a pointer to the StaticTask_t structure in which the Idle task’s
     * state will be stored.
     */
    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;

    /* Pass out the array that will be used as the Idle task’s stack. */
    *ppxIdleTaskStackBuffer = uxIdleTaskStack;

    /* Pass out the size of the array pointed to by *ppxIdleTaskStackBuffer.
     * Note that, as the array is necessarily of type StackType_t,
     * configMINIMAL_STACK_SIZE is specified in words, not bytes.
     */
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}

/* configSUPPORT_STATIC_ALLOCATION and configUSE_TIMERS are both set to 1, so the
 * application must provide an implementation of vApplicationGetTimerTaskMemory()
 * to provide the memory that is used by the Timer service task.
 */
void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTaskTCBBuffer,
                                     StackType_t **ppxTimerTaskStackBuffer,
                                     uint32_t *pulTimerTaskStackSize )
{
/* If the buffers to be provided to the Timer task are declared inside this
 * function then they must be declared static – otherwise they will be allocated on
 * the stack and so not exists after this function exits.
 */
static StaticTask_t xTimerTaskTCB;
static StackType_t uxTimerTaskStack[ configTIMER_TASK_STACK_DEPTH ];

    /* Pass out a pointer to the StaticTask_t structure in which the Timer
     * task's state will be stored.
     */
    *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;

    /* Pass out the array that will be used as the Timer task’s stack. */
    *ppxTimerTaskStackBuffer = uxTimerTaskStack;

    /* Pass out the size of the array pointed to by *ppxTimerTaskStackBuffer.
     * Note that, as the array is necessarily of type StackType_t,
     * configTIMER_TASK_STACK_DEPTH is specified in words, not bytes.
     */
    *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}

extern void ti_sysbios_family_c62_TaskSupport_swap__E( void **oldtskContext, void **newtskContext);


void vPortRestoreTaskContext()
{
    void * dummyTaskSp;

    ti_sysbios_family_c62_TaskSupport_swap__E( &dummyTaskSp,  (void **)(&pxCurrentTCB->pxTopOfStack));
}

void vPortYield()
{
    void **oldSP;
    void **newSP;

    oldSP = (void **)(&pxCurrentTCB->pxTopOfStack);
    vTaskSwitchContext();
    newSP = (void **)(&pxCurrentTCB->pxTopOfStack);
    ti_sysbios_family_c62_TaskSupport_swap__E(oldSP, newSP);
}

/*
 * Returns true if the current core is in ISR context; low prio ISR, med prio ISR or timer tick ISR. High prio ISRs
 * aren't detected here, but they normally cannot call C code, so that should not be an issue anyway.
 */
BaseType_t xPortInIsrContext()
{
    BaseType_t inISR = false;
    if (ulPortInterruptNesting != 0)
    {
        inISR =  true;
    }
    return inISR;
}

void vPortAssertIfInISR()
{
    if( xPortInIsrContext() )
    {
        DebugP_log0( "port_interruptNesting\n\n");
    }

    configASSERT( !xPortInIsrContext() );
}

