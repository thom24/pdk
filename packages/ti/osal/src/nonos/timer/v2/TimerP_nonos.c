/*
 * Copyright (c) 2020, Texas Instruments Incorporated
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
 *  ======== TimerP_csl_nonos.c ========
 */


#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include <ti/osal/TimerP.h>
#include <ti/csl/soc.h>
#include <ti/csl/csl_rti.h>
#include <ti/csl/arch/csl_arch.h>
#include <ti/osal/src/nonos/Nonos_config.h>

/* DM Timer Implementation */
extern void Osal_DebugP_assert(int32_t expression, const char *file, int32_t line);
#define TIMOSAL_Assert(expression) (Osal_DebugP_assert(((int32_t)((expression)?1:0)),\
                                                  __FILE__, __LINE__))

/* Local defines for the rti timer */
#define TIMERP_RTI_MAX_PERIOD                (0xffffffffU)
#define TIMERP_PRESCALER_DEF                 (1U)
/* Local Timer Struct */
typedef struct TimerP_Struct_s
{
  bool     used;       /* In use or not status */
  uint32_t id;         /* timer Id */
  uint32_t periodType; /* Period type, default micro seconds */
  uint32_t freqLo;     /* least siginificant 32-bits of frequency */
  uint32_t freqHi;     /* most siginificant 32-bits of frequency  */
  uint32_t startMode;  /* timer start mode */
  uint32_t runMode;    /* timer run mode   */
  uint32_t period;     /* Period of a tick */
  TimerP_Fxn tickFxn;  /* Timer Tick function */
  void*    arg;        /* Argument passed into the timer function. */
  uint32_t availMask;   /* Available timer mask */
  HwiP_Handle hwi;      /* Hwi handle for tickFxn */
  int32_t  eventId;     /* Event Id for C66x */
  int32_t  intNum;      /* Interrupt Number */
  uint32_t prescaler;
}TimerP_Struct;

/* As static allocation is used, the following provides the structs
 * The TimerP_Struct pool servers to get an available timer
 * handle during timer create API call.
 */
TimerP_Struct gTimerStructs[OSAL_NONOS_CONFIGNUM_TIMER];
static        uint32_t    gTimerInitDone = 0U;
static        uint32_t    gTimerAnyMask;

/* external variables */
extern uint32_t  gOsalTimerAllocCnt, gOsalTimerPeak;

/* Local functions  */
static uint32_t TimerP_getTimerBaseAddr(uint32_t timer_id);
static void TimerP_rtiTimerStub(uintptr_t arg);
static void  TimerP_rtiTimerInitDevice(const TimerP_Struct *timer, uint32_t baseAddr);
static bool TimerP_rtiTimerCheckOverflow(uint32_t a, uint32_t b);
static bool TimerP_rtiTimerSetMicroSeconds(TimerP_Struct *timer, uint32_t period);
static TimerP_Status TimerP_rtiTimerDeviceCfg(TimerP_Struct *timer, uint32_t baseAddr);
static TimerP_Status TimerP_rtiTimerInitObj(TimerP_Struct *timer, TimerP_Fxn tickFxn, const TimerP_Params *params);
static TimerP_Status TimerP_rtiTimerInstanceInit(TimerP_Struct *timer, uint32_t id, TimerP_Fxn tickFxn, const TimerP_Params *params);

/*
 * This private function returns the base address of the timer based on
 * the ID passed in.
 */
static uint32_t TimerP_getTimerBaseAddr(uint32_t timer_id)
{
  uint32_t addr;

  if (timer_id < TimerP_numTimerDevices) {
    addr = (uint32_t) gRtiTimerPInfoTbl[timer_id].baseAddr;
  }
  else {
    addr = (uint32_t)0U;
  }
  return (addr);
}

/*
 * This private function is a stub function for the timer ISR
 * function that is registered from the application
 */
static void TimerP_rtiTimerStub(uintptr_t arg)
{
  TimerP_Struct *timer = (TimerP_Struct *) arg;
  uint32_t baseAddr = TimerP_getTimerBaseAddr(timer->id);

  /* Disable the Timer interrupts */

  if (timer->runMode == (uint32_t)TimerP_RunMode_ONESHOT) {
    (void)TimerP_stop((TimerP_Handle) timer);
  }

  /* acknowledge the interrupt */
  if ((timer->id & 0x01U) != 0U) {
    (void)RTITmrIntStatusClear(baseAddr, RTI_TMR_INT_INT1_FLAG);
  }
  else {
    (void)RTITmrIntStatusClear(baseAddr, RTI_TMR_INT_INT0_FLAG);
  }

  /* call the user's ISR */
  timer->tickFxn((uintptr_t)timer->arg);

  /* Enable the Timer interrupts */
}

/*
 * This priviate function initializes the timer registers
 */
static void  TimerP_rtiTimerInitDevice(const TimerP_Struct *timer, uint32_t baseAddr)
{
  uint32_t key;

  key = (uint32_t)HwiP_disable();

  if ((timer->id & 0x01U) != 0U) {
    (void)RTITmrClear(baseAddr, RTI_TMR_CNT_BLK_INDEX_1, RTI_TMR_CMP_BLK_INDEX_1);
  }
  else {
    (void)RTITmrClear(baseAddr, RTI_TMR_CNT_BLK_INDEX_0, RTI_TMR_CMP_BLK_INDEX_0);
  }

  if (timer->hwi != NULL_PTR) {
    /* clear any previously latched timer interrupts */
    Osal_ClearInterrupt(timer->eventId, timer->intNum);
    Osal_DisableInterrupt(timer->eventId, timer->intNum);
  }

  HwiP_restore(key);
}

/*
 * This priviate function checks if there is any overflow in
 * timer period computations, i.e. the period is too big.
 */
static bool TimerP_rtiTimerCheckOverflow(uint32_t a, uint32_t b)
{
  return ((b > 0U) && (a > (TimerP_MAX_PERIOD/b)));
}

/*
 * This is a private function to set the period of the timer
 * register for a specified micro second value
 */
static bool TimerP_rtiTimerSetMicroSeconds(TimerP_Struct *timer, uint32_t period)
{
  uint64_t  counts;
  uint32_t  freqKHz;
  bool      ret = (bool) true;

  (void)TimerP_stop(timer);

  /*
   * The frequency of FRC is equal to the clock rate divided by (prescaler + 1)
   */
  freqKHz = ((timer->freqLo/(TIMERP_PRESCALER_DEF+1U)) + 500U)/ 1000U;
  if (TimerP_rtiTimerCheckOverflow(freqKHz, period/1000U)) {
    ret = (bool) false;
  }

  if (ret == (bool) true) {
    counts = ((uint64_t)freqKHz * (uint64_t)period) / (uint64_t)1000U;
    if (counts > 0xffffffffU) {
      ret = (bool) false;
    }
  }

  if (ret == (bool) true) {
    timer->period = (uint32_t)counts;
    timer->periodType = (uint32_t)TimerP_PeriodType_COUNTS;
    timer->prescaler = TIMERP_PRESCALER_DEF;
  }
  return(ret);
}

/*
 * Thi is a private function to configure the timer registers for a given timer
 * period
 */
static TimerP_Status TimerP_rtiTimerDeviceCfg(TimerP_Struct *timer, uint32_t baseAddr)
{
  uint32_t key;
  TimerP_Status retVal=TimerP_OK;
  rtiTmrConfig_t rtiCfg;

  /* initialize the timer */
  TimerP_rtiTimerInitDevice(timer, baseAddr);

  key = (uint32_t)HwiP_disable();

  /* set the period */
  if (timer->periodType == (uint32_t)TimerP_PeriodType_MICROSECS) {
    if (!TimerP_rtiTimerSetMicroSeconds(timer, timer->period)) {
      HwiP_restore(key);
      retVal = TimerP_FAILURE;
    }
  }

  if(retVal == TimerP_OK)
  {
    /* Set prescaler, enable interrupt and select counter for comparator */
    /* Use internal up-counter to clock free-running counter 0 */
    rtiCfg.clkSrc = RTI_TMR_CLK_SRC_COUNTER;
    rtiCfg.period = timer->period;
    rtiCfg.prescaler = timer->prescaler;
    if ((timer->id & 0x01U) != 0U) {
      rtiCfg.cntBlkIndex = RTI_TMR_CNT_BLK_INDEX_1;
      rtiCfg.cmpBlkIndex = RTI_TMR_CMP_BLK_INDEX_1;
    }
    else {
      rtiCfg.cntBlkIndex = RTI_TMR_CNT_BLK_INDEX_0;
      rtiCfg.cmpBlkIndex = RTI_TMR_CMP_BLK_INDEX_0;
    }
    (void)RTITmrConfigure(baseAddr, &rtiCfg);
    HwiP_restore(key);
  }

  return (retVal);
}

/*
 * This is a private function to initialize the timer counter, period and compare
 * registers before starting the timers
 */
static TimerP_Status TimerP_rtiTimerInitObj(TimerP_Struct *timer, TimerP_Fxn tickFxn, const TimerP_Params *params)
{
  timer->tickFxn = tickFxn;

  /*
   * Update the default initializations to user configured values
   */
  if ( params->extfreqLo != TimerP_USE_DEFAULT) {
    timer->freqLo   = (uint32_t)params->extfreqLo;
  }
  else  {
    if (params->intfreqLo != TimerP_USE_DEFAULT)  {
      timer->freqLo = (uint32_t)params->intfreqLo;
    }
    else {
      timer->freqLo = (uint32_t)TimerP_getDefaultFreqLo(timer->id);
    }
  }

  if ( params->extfreqHi != TimerP_USE_DEFAULT) {
    timer->freqHi   =  (uint32_t)params->extfreqHi;
  }
  else  {
    if (params->intfreqHi != TimerP_USE_DEFAULT)  {
      timer->freqHi = (uint32_t)params->intfreqHi;
    }
    else {
      timer->freqHi =  (uint32_t)TimerP_getDefaultFreqHi(timer->id);
    }
  }

  if ( params->period != 0u) {
    timer->period   = params->period;
  }
  else  {
    timer->period   = 0u;
  }

  if ( params->arg != NULL_PTR) {
    timer->arg     = params->arg;
  }

  if ( params->intNum != TimerP_USE_DEFAULT) {
    timer->intNum = params->intNum;
  }
  else {
    timer->intNum = gRtiTimerPInfoTbl[timer->id].intNum;
  }

#if defined (_TMS320C6X)
  if ( params->eventId != TimerP_USE_DEFAULT) {
    timer->eventId = params->eventId;
  }
  else {
    timer->eventId = gRtiTimerPInfoTbl[timer->id].eventId;
  }
#else
  timer->eventId = 0;
#endif

  timer->periodType = params->periodType;
  timer->startMode  = params->startMode;
  timer->runMode    = params->runMode;

  return(TimerP_OK);
}

/*
 * This is a private function to initialize the timer instance, that sets up the timer ISR to
 * a specified timer id and sets up the timer compare, counter and period registers
 */
static TimerP_Status TimerP_rtiTimerInstanceInit(TimerP_Struct *timer, uint32_t id, TimerP_Fxn tickFxn, const TimerP_Params *params)
{
  TimerP_Status ret = TimerP_OK;
  uint32_t      key;
  uint32_t      i;
  uint32_t      tempId = 0xffffu;
  OsalRegisterIntrParams_t interruptRegParams;
  uint32_t      baseAddr;
  int32_t       intNum;
  uint32_t      timerPAnyMask = (uint32_t)(TIMERP_ANY_MASK);

  if ((id != TimerP_ANY) && (id >= TimerP_numTimerDevices)) {
    ret = TimerP_FAILURE;
  }

  /* Get the timer Id */
  if (ret == TimerP_OK)
  {
      /* Set the available timer id mask to all */
    if (gTimerInitDone == 0U)
    {
      gTimerAnyMask  = TIMERP_AVAILABLE_MASK;
      gTimerInitDone = 1U;
    }

    timer->availMask = gTimerAnyMask;

    key = (uint32_t)HwiP_disable();

    if (id == TimerP_ANY) {
      for (i = 0U; i < TimerP_numTimerDevices; i++) {
        uint32_t shift, nshift;
        shift  = ((uint32_t) 1U) << i;
        nshift = ~shift;
        if (((timerPAnyMask    & shift) != (uint32_t) 0U ) &&
            ((timer->availMask & shift) != (uint32_t) 0U )) {
          timer->availMask &= nshift;
          tempId = i;
          break;
        }
      }
    }
    else
    {
	  if ((timer->availMask & ((uint32_t)1u << id)) > 0U)
	  {
        uint32_t shift, nshift;
        shift  = ((uint32_t) 1u) << id;
        nshift = ~shift;
        timer->availMask &= nshift;
        tempId = id;
      }
    }

    gTimerAnyMask =  timer->availMask;

    /* Initialize the timer state object */
    timer->id = tempId; /* Record the timer Id */
    baseAddr = TimerP_getTimerBaseAddr(timer->id);

    HwiP_restore(key);

    if (tempId == 0xffffU) {
      ret = TimerP_NOT_AVAILABLE;
    }
  }

  /* Initialize the timer state object */
  if (ret == TimerP_OK) {
    ret = TimerP_rtiTimerInitObj(timer, tickFxn, params);
  }

  /* Create the Hwi Funtion for the tick funtion */
  if (ret == TimerP_OK)
  {
    if (timer->tickFxn != (TimerP_Fxn) NULL_PTR)
    {
      intNum          = timer->intNum;
      /* Initialize with defaults */
      Osal_RegisterInterrupt_initParams(&interruptRegParams);

      /* Populate the interrupt parameters */
      interruptRegParams.corepacConfig.arg=(uintptr_t) timer;
      interruptRegParams.corepacConfig.name=(char *) NULL_PTR;
      interruptRegParams.corepacConfig.isrRoutine=TimerP_rtiTimerStub;

#if defined (__ARM_ARCH_7A__) || defined (__aarch64__) || ((__ARM_ARCH == 7) && (__ARM_ARCH_PROFILE == 'R') )
#if defined(SOC_AM335x) || defined(SOC_AM437x) || defined(SOC_AM65XX) || defined(SOC_J721E) || defined(SOC_J7200) || defined(SOC_J721S2)
      interruptRegParams.corepacConfig.triggerSensitivity =  (uint32_t)OSAL_ARM_GIC_TRIG_TYPE_HIGH_LEVEL;
      interruptRegParams.corepacConfig.priority = 0x20U;
#else
      interruptRegParams.corepacConfig.triggerSensitivity =  (uint32_t)OSAL_ARM_GIC_TRIG_TYPE_EDGE;
#endif
#endif

#if defined(_TMS320C6X)
      interruptRegParams.corepacConfig.corepacEventNum= timer->eventId; /* Event going in to CPU */
#endif
      interruptRegParams.corepacConfig.intVecNum=intNum; /* Host Interrupt vector */

      /* Register interrupts */
      (void)Osal_RegisterInterrupt(&interruptRegParams,&(timer->hwi));

      if (timer->hwi == NULL_PTR)
      {
        ret = TimerP_ISR_HOOK_ERR;
      }
    }
    else
    {
      timer->hwi = NULL_PTR;
    }
  }

  /* timer post init */
  if (ret == TimerP_OK)
  {
     ret = TimerP_rtiTimerDeviceCfg(timer, baseAddr);

     if (timer->startMode == (uint32_t)TimerP_StartMode_AUTO) {
         (void)TimerP_start(timer);
     }
  }

  return (ret);
}


/* Interface functions */

/* Default parameter initialization module function
 * more details on the documentation of this interface
 * function can be found in the TimerP.h interface
 * header file */
void TimerP_Params_init(TimerP_Params *params)
{
    TIMOSAL_Assert((params == NULL_PTR));

    if(params != NULL_PTR) {
      /* Set the default values */
      params->arg         = NULL_PTR;
      params->extfreqHi   = TimerP_USE_DEFAULT;
      params->extfreqLo   = TimerP_USE_DEFAULT;
      params->intfreqHi   = TimerP_USE_DEFAULT;
      params->intfreqLo   = TimerP_USE_DEFAULT;
      params->name        = (char *) NULL_PTR;
      params->period      = 0;
      params->runMode     = (uint32_t)TimerP_RunMode_CONTINUOUS;
      params->startMode   = (uint32_t)TimerP_StartMode_AUTO;
      params->periodType  = (uint32_t)TimerP_PeriodType_MICROSECS;
      params->intNum      = (int32_t)TimerP_USE_DEFAULT;
#if defined (_TMS320C6X)
      params->eventId     = TimerP_USE_DEFAULT;
#endif
    }
    return;
}

/* Timer create module function
 * more details on the documentation of this interface
 * function can be found in the TimerP.h interface
 * header file */
TimerP_Handle TimerP_create(int32_t id,
                            TimerP_Fxn tickFxn,
                            const TimerP_Params *inParams)

{
  uint32_t          i;
  uintptr_t         key;
  TimerP_Handle     ret_handle;
  TimerP_Params     params;

  if (inParams != NULL_PTR)
  {
    params = *inParams;
  }
  else
  {
    TimerP_Params_init(&params);
  }

  key = HwiP_disable();

  for (i = 0; i < OSAL_NONOS_CONFIGNUM_TIMER; i++)
  {
    if (gTimerStructs[i].used == (bool)false)
    {
      gTimerStructs[i].used = (bool)true;

      /* Update statistics */
      gOsalTimerAllocCnt++;
      if (gOsalTimerAllocCnt > gOsalTimerPeak)
      {
          gOsalTimerPeak = gOsalTimerAllocCnt;
      }

      gTimerStructs[i].id  = (uint32_t)id;

      break;
    }
  }

  HwiP_restore(key);

  if (i == OSAL_NONOS_CONFIGNUM_TIMER)
  {
    ret_handle = NULL_PTR;
  }
  else
  {
    /* Start the timer for that period/mode */
    ret_handle = ((TimerP_Handle)&gTimerStructs[i]);
  }

  if (ret_handle != NULL_PTR)
  {
    if (TimerP_rtiTimerInstanceInit(&gTimerStructs[i], (uint32_t) id, tickFxn, &params) != TimerP_OK) {
      /* Free up the consumed timer memory */
      gTimerStructs[i].used = (bool)false;
      ret_handle = NULL_PTR;
    }
  }

  /* start timer if it is auto mode */
  if ((params.startMode == (uint32_t)TimerP_StartMode_AUTO) &&
      (ret_handle != NULL_PTR) )
  {
     if(TimerP_start((TimerP_Handle)&gTimerStructs[i]) != TimerP_OK) {
        gTimerStructs[i].used = (bool)false;
        /* TBD: should we call TimerP_delete() here? */
        ret_handle = NULL_PTR;
     }
  }
  return (ret_handle);
}


/* Timer delete module function
 * more details on the documentation of this interface
 * function can be found in the TimerP.h interface
 * header file */
TimerP_Status TimerP_delete(TimerP_Handle handle)
{
  /* Release or free the memory */
  uint32_t key;
  uint32_t id;
  uint32_t index = (uint32_t)(( ((uintptr_t) handle) - ((uintptr_t) gTimerStructs) )/sizeof(TimerP_Struct)); /* struct subtraction */
  TimerP_Status ret = TimerP_OK;
  TimerP_Struct *timer = (TimerP_Struct *) handle;
  TIMOSAL_Assert((handle == NULL_PTR));

  key = (uint32_t)HwiP_disable();

  if((timer != NULL_PTR) && (gTimerStructs[index].used))
  {
    /* clear the ISR that was set before */
    if (timer->hwi != NULL) {
      (void)HwiP_delete(timer->hwi);
    }

    /* reset the timer's bit field in the mask and clear the used flag */
    gTimerStructs[index].used = (bool)false;
    id = gTimerStructs[index].id;
    uint32_t shift = ((uint32_t) 1u) << id;
    if(((gTimerAnyMask & shift)) == 0U)
    {
      gTimerAnyMask |= shift;
    }
    else
    {
      ret = TimerP_FAILURE;	
    }
    /* Found the osal timer object to delete */
    if (gOsalTimerAllocCnt > 0U)
    {
      gOsalTimerAllocCnt--;
    }
  }
  else
  {
    ret = TimerP_FAILURE;
  }

  HwiP_restore(key);

  return (ret);
}


/* Timer start module function
 * more details on the documentation of this interface
 * function can be found in the TimerP.h interface
 * header file */
TimerP_Status TimerP_start(TimerP_Handle handle)
{
  uint32_t  key;
  TimerP_Struct *timer = (TimerP_Struct *) handle;
  TimerP_Status retVal = TimerP_OK;

  uint32_t baseAddr = TimerP_getTimerBaseAddr(timer->id);

  if (baseAddr == 0U) {
    retVal = TimerP_NOT_AVAILABLE;
  }
  else
  {

    key = (uint32_t)HwiP_disable();

    (void)TimerP_stop(handle);

    if ((timer->id & 0x01U) != 0U) {
      (void)RTITmrIntStatusClear(baseAddr, RTI_TMR_INT_INT1_FLAG);
    }
    else {
      (void)RTITmrIntStatusClear(baseAddr, RTI_TMR_INT_INT0_FLAG);
    }
    if(timer->hwi != NULL_PTR) {
      Osal_ClearInterrupt(timer->eventId, timer->intNum);
      Osal_EnableInterrupt(timer->eventId, timer->intNum);
    }

    if ((timer->id & 0x01U) != 0U) {
      (void)RTITmrEnable(baseAddr, RTI_TMR_CNT_BLK_INDEX_1);
    }
    else {
      (void)RTITmrEnable(baseAddr, RTI_TMR_CNT_BLK_INDEX_0);
    }

    HwiP_restore(key);
  }
  return (retVal);
}

/* Timer stop module function
 * more details on the documentation of this interface
 * function can be found in the TimerP.h interface
 * header file */

TimerP_Status TimerP_stop(TimerP_Handle handle)
{
  uint32_t  key;
  TimerP_Struct *timer = (TimerP_Struct *) handle;
  TimerP_Status retVal = TimerP_OK;

  uint32_t baseAddr = TimerP_getTimerBaseAddr(timer->id);

  if (baseAddr == 0U) {
    retVal = TimerP_NOT_AVAILABLE;
  }
  else
  {
    key = (uint32_t)HwiP_disable();

    if ((timer->id & 0x01U) != 0U) {
      (void)RTITmrDisable(baseAddr, RTI_TMR_CNT_BLK_INDEX_1);
    }
    else {
      (void)RTITmrDisable(baseAddr, RTI_TMR_CNT_BLK_INDEX_0);
    }

    if(timer->hwi != NULL_PTR) {
      Osal_ClearInterrupt(timer->eventId, timer->intNum);
      Osal_DisableInterrupt(timer->eventId, timer->intNum);
    }

    HwiP_restore(key);
  }

  return(retVal);
}

/* Timer set micro seconds module function
 * more details on the documentation of this interface
 * function can be found in the TimerP.h interface
 * header file */
TimerP_Status TimerP_setPeriodMicroSecs(TimerP_Handle handle, uint32_t microsecs)
{
  TimerP_Struct *timer = (TimerP_Struct *) handle;
  TimerP_Status retVal = TimerP_OK;
  if (!TimerP_rtiTimerSetMicroSeconds(timer, microsecs)) {
    retVal = TimerP_FAILURE;
  }
  return (retVal);
}

/* Get time in micro seconds */
uint64_t TimerP_getTimeInUsecs(void)
{
    TimeStamp_Struct timestamp64;
    uint64_t         cur_ts, freq;
    uint32_t         tsFreqKHz;

    /* Get the timestamp */
    osalArch_TimestampGet64(&timestamp64);

    /* Get the frequency of the timeStatmp provider */
    tsFreqKHz  = (uint32_t)osalArch_TimeStampGetFreqKHz();

    cur_ts = ((uint64_t) timestamp64.hi << 32U) | timestamp64.lo;
    freq = (uint64_t) (tsFreqKHz);

    return ((cur_ts*1000u)/freq);
}

/* Get timer reload count */
uint32_t TimerP_getReloadCount(TimerP_Handle handle)
{
  uint32_t      reloadCnt = 0U;
  uint32_t      rticomp;
  TimerP_Struct *timer = (TimerP_Struct *) handle;
  uint32_t      baseAddr = TimerP_getTimerBaseAddr(timer->id);

  if (baseAddr != 0U) 
  {
    if ((timer->id & 0x01U) != 0U) 
    {
      rticomp = RTITmrCompareGet(baseAddr, RTI_TMR_CNT_BLK_INDEX_1);
    }
    else 
    {
      rticomp = RTITmrCompareGet(baseAddr, RTI_TMR_CNT_BLK_INDEX_0);
    }
  } 

  /* return 0xFFFFFFFF - value, since this is the expected format to calculate current time */
  reloadCnt = 0xFFFFFFFFU - (rticomp) - 1UL;

  return (reloadCnt);
}

/* Get timer current count */
uint32_t TimerP_getCount(TimerP_Handle handle)
{
  uint32_t      count = 0U;
  rtiTmrCnts_t  rtiCnts;
  uint32_t      rticomp;
  TimerP_Struct *timer = (TimerP_Struct *) handle;
  uint32_t      baseAddr = TimerP_getTimerBaseAddr(timer->id);

  if (baseAddr != 0U) 
  {
    if ((timer->id & 0x01U) != 0U) 
    {
      (void)RTITmrCounterGet(baseAddr, RTI_TMR_CNT_BLK_INDEX_1, &rtiCnts);
      rticomp = RTITmrCompareGet(baseAddr, RTI_TMR_CNT_BLK_INDEX_1);
    }
    else 
    {
      (void)RTITmrCounterGet(baseAddr, RTI_TMR_CNT_BLK_INDEX_0, &rtiCnts);
      rticomp = RTITmrCompareGet(baseAddr, RTI_TMR_CNT_BLK_INDEX_0);
    }
  } 

  /* return 0xFFFFFFFF - value, since this is the expected format to calculate current time */
  count = 0xFFFFFFFFU - (rticomp - rtiCnts.frc) - 1UL;

  return (count);

}

/* This file implements the RTI timer osal functions on devices */
