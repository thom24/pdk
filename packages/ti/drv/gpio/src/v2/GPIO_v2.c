/*
 *  Copyright (c) Texas Instruments Incorporated 2020
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
 *   \file  GPIO_v2.c
 *
 *   \brief
 *      The file implements the GPIO Driver.
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdint.h>
#include <string.h>
#include <ti/csl/csl_gpio.h>
#include <ti/drv/gpio/GPIO.h>
#include <ti/drv/gpio/soc/GPIO_soc.h>
#include <ti/drv/gpio/src/GPIO_osal.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* Uninitialized callbackInfo pinIndex */
#define PIN_INDEX_NOT_CONFIGURED (0xFFU)


/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/**
 * \brief
 *  GPIO Hardware Attributes
 *
 * \details
 *  The structure contains the hardware atrributes which are used
 *  to specify the platform specific configurations.
 */
typedef struct GPIO_MCB_t
{
    /**
     * \brief   GPIO driver has been initialized
     */
    bool                isInitialized;

    /**
     * \brief   HWI Handle to the high priority interrupts
     */
    HwiP_Handle         hwiHandleHigh[GPIO_INST_MAX];

    /**
     * \brief   HWI Handle to the low priority interrupts
     */
    HwiP_Handle         hwiHandleLow[GPIO_INST_MAX];

    /**
     * \brief   Number of high priority interrupts detected
     */
    uint32_t            numHighPriorityInterrupts[GPIO_INST_MAX];

    /**
     * \brief   Number of low priority interrupts detected
     */
    uint32_t            numLowPriorityInterrupts[GPIO_INST_MAX];

    /**
     * \brief   index in the GPIO_v2_config object corresponding to each pin.
     */
    uint8_t             pinIndex[GPIO_INST_MAX][GPIO_MAX_PORT][GPIO_MAX_PINS_PER_PORT];
}GPIO_MCB;


/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */
static int32_t GPIO_parseIndex(uint32_t index, uint32_t* baseAddr, uint32_t* inst, uint32_t* port, uint32_t* pin);

/* GPIO ISR: */
static void GPIO_HighPriorityISR (uintptr_t arg);
static void GPIO_LowPriorityISR (uintptr_t arg);
static void GPIO_processInterrupt (uint32_t inst, uint32_t pendingInterrupt);

/* GPIO Interface functions */
static void GPIO_init_v2(void);
static void GPIO_clearInt_v2(uint32_t index);
static void GPIO_disableInt_v2(uint32_t index);
static void GPIO_enableInt_v2(uint32_t index);
static uint32_t GPIO_read_v2(uint32_t index);
static void GPIO_setCallback_v2(uint32_t index, GPIO_CallbackFxn callback);
static void GPIO_setConfig_v2(uint32_t index, GPIO_PinConfig pinConfig);
static void GPIO_toggle_v2(uint32_t index);
static void GPIO_write_v2(uint32_t index, uint32_t value);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */
/* GPIO driver config data structure */
extern GPIO_v2_Config GPIO_v2_config;

/**
 * \brief
 *  Global GPIO MCB which tracks the GPIO driver related information.
 */
GPIO_MCB    gGPIOMCB;

/* GPIO function table for GPIO_v2 implementation */
const GPIO_FxnTable GPIO_FxnTable_v2 = {
    &GPIO_init_v2,
    &GPIO_read_v2,
    &GPIO_write_v2,
    &GPIO_clearInt_v2,
    &GPIO_disableInt_v2,
    &GPIO_enableInt_v2,
    &GPIO_setCallback_v2,
    &GPIO_setConfig_v2,
    &GPIO_toggle_v2,
};

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */
/**
 * \brief The function process the pending interrupt.
 *
 * \param  inst                 GPIO instance number.
 * \param  pendingInterrupt     Pending Interupt to be processed.
 *
 * \return None
 */
static void GPIO_processInterrupt (uint32_t inst, uint32_t pendingInterrupt)
{
    uint32_t    index, port, pin;

    /* Decode the pendingInterrupt:
     *  The Table mapping as per the documentation is as follows:-
     *      Interrupt 0  -> No Interrupt
     *      Interrupt 1  -> Port 0 Pin 0
     *      Interrupt 2  -> Port 0 Pin 1
     *      ...
     * NOTE: We have already verified that there there is a pending interrupt */
    pendingInterrupt = pendingInterrupt - 1U;
    port = pendingInterrupt / 8U;
    pin  = pendingInterrupt % 8U;
    index = gGPIOMCB.pinIndex[inst][port][pin];
    /* Invoke the callback function if one was registered */
    if ((index != PIN_INDEX_NOT_CONFIGURED) &&
        (index < GPIO_v2_config.numberOfCallbacks) &&
        (GPIO_v2_config.callbacks[index] != NULL))
    {
        GPIO_v2_config.callbacks[index]();
    }
    return;
}

/**
 * \brief The function is the registered ISR which handles the high
 *      priority GPIO Interrupts
 *
 * \param  arg  GPIO Instance Id
 *
 * \return None
 */
static void GPIO_HighPriorityISR (uintptr_t arg)
{
    uint32_t     pendingInterrupt;
    GPIO_v2_HwAttrs  *gpioHwAttr;
    uint32_t        inst;

    /* We need to process all the interrupts which are pending */
    while (1)
    {
        /* Get the hardware attrib structure. */
        inst   = (uint32_t) arg;
        GPIO_getHwAttr(&gpioHwAttr, inst);

        /* Get the high priority pending GPIO interrupt: */
        pendingInterrupt = GPIO_getHighLevelPendingInterrupt(gpioHwAttr->gpioBaseAddr);

        /* Have we processed all the GPIO interrupts? */
        if (pendingInterrupt == 0U)
        {
            /* YES: There are no more pending interrupts. */
            break;
        }

        /* Increment the statistics: */
        gGPIOMCB.numHighPriorityInterrupts[inst]++;

        /* Process the pending interrupt: */
        GPIO_processInterrupt (inst, pendingInterrupt);
    }
}

/**
 * \brief The function is the registered ISR which handles the low
 *      priority GPIO Interrupts
 *
 * \param  arg  GPIO Instance Id
 *
 * \return None
 */
static void GPIO_LowPriorityISR (uintptr_t arg)
{
    uint32_t     pendingInterrupt;
    GPIO_v2_HwAttrs  *gpioHwAttr;
    uint32_t        inst;

    /* We need to process all the interrupts which are pending */
    while (1)
    {
        /* Get the hardware attrib structure. */
        inst   = (uint32_t) arg;
        GPIO_getHwAttr(&gpioHwAttr, inst);

        /* Get the low priority pending GPIO interrupt: */
        pendingInterrupt = GPIO_getLowLevelPendingInterrupt(gpioHwAttr->gpioBaseAddr);

        /* Have we processed all the GPIO interrupts? */
        if (pendingInterrupt == 0U)
        {
            /* YES: There are no more pending interrupts. */
            break;
        }

        /* Increment the statistics: */
        gGPIOMCB.numLowPriorityInterrupts[inst]++;

        /* Process the pending interrupt: */
        GPIO_processInterrupt (inst, pendingInterrupt);
    }
}

/**
 * \brief The function is used to parse the index passed.
 *
 * \param  index    GPIO index passed
 * \param  baseAddr GPIO module Base Address
 * \param  inst     GPIO instance Id
 * \param  port     GPIO Port Number
 * \param  pin      GPIO Pin Number
 *
 * \return          Status
 *                      0        - Successful
 *                      Non Zero - Index passed is invalid
 */

static int32_t GPIO_parseIndex(uint32_t index, uint32_t* baseAddr, uint32_t* inst, uint32_t* port, uint32_t* pin)
{
    int32_t     retVal = 0;
    GPIO_v2_HwAttrs  *gpioHwAttr = NULL;
    uint32_t    pinCfg;

    if ((gGPIOMCB.isInitialized == true) &&
        (index < GPIO_v2_config.numberOfPinConfigs))
    {
        pinCfg = GPIO_v2_config.pinConfigs[index];
        *inst  = (pinCfg & 0xFF00U) >> 8U;
        *port = ((pinCfg & 0xFFU) / 8U);
        *pin  = ((pinCfg & 0xFFU) % 8U);
        if ((*inst > GPIO_INST_MAX) || (*port > GPIO_MAX_PORT) || (*pin > GPIO_MAX_PINS_PER_PORT))
        {
            retVal = -1;
        }
    }

    if (retVal == 0)
    {
        retVal = GPIO_getHwAttr(&gpioHwAttr, *inst);
    }

    if ((retVal == 0) && (gpioHwAttr != NULL))
    {
        *baseAddr = gpioHwAttr->gpioBaseAddr;
    }
    return retVal;
}

/*
 *  ======== GPIO_init ========
 */
static void GPIO_init_v2(void)
{
    uint32_t                    instIndex, portIndex;
    GPIO_v2_HwAttrs             *gpioHwAttr;
    int32_t                     retVal;
    OsalRegisterIntrParams_t    interruptRegParams;
    uint32_t                    i,j,k;

    /* Initialize the GPIO Driver: */
    memset ((void *)&gGPIOMCB, 0, sizeof(GPIO_MCB));

    /* Initialize all entries with 'not configured' key */
    for (i = 0; i <= GPIO_INST_MAX; i++)
    {
	    for (j = 0; j < GPIO_MAX_PORT; j++)
        {
            for (k = 0; k < GPIO_MAX_PORT; k++)
            {
                gGPIOMCB.pinIndex[i][j][k] = PIN_INDEX_NOT_CONFIGURED;
            }
	    }
	}

    for (instIndex = GPIO_INST_MIN; instIndex <= GPIO_INST_MAX; instIndex++)
    {
        retVal = GPIO_getHwAttr(&gpioHwAttr, instIndex);
        if ((retVal == 0) && (gpioHwAttr != NULL))
        {
            /* Ensure that the GPIO is operating normally */
            GPIO_moduleEnable(gpioHwAttr->gpioBaseAddr);

            /* Disable the interrupts for all pins */
            GPIO_disableMultipleInterrupt(gpioHwAttr->gpioBaseAddr, 0xFFFFFFFFU);

            /* Cycle through all the GPIO Ports: */
            for (portIndex = 0U; portIndex < GPIO_MAX_PORT; portIndex++)
            {
                /* Disable the output buffer for all the pins: */
                GPIO_setOutputDataDirectionMultiple(gpioHwAttr->gpioBaseAddr, portIndex, 0U);

                /* No data is to be sent out for all the pins: */
                GPIO_sendDataMultiple(gpioHwAttr->gpioBaseAddr, portIndex, 0U);

                /* Disable open drain capability for all the pins:  */
                GPIO_setOpenDrainStatusMultiple(gpioHwAttr->gpioBaseAddr, portIndex, 0U);
            }

            /* The instance is supported. */
            /* Register the High Prirority interrupts: */
            Osal_RegisterInterrupt_initParams(&interruptRegParams);
            interruptRegParams.corepacConfig.isrRoutine  = (&GPIO_HighPriorityISR);
            interruptRegParams.corepacConfig.arg         = (uintptr_t)(instIndex);

            interruptRegParams.corepacConfig.corepacEventNum = gpioHwAttr->highInterruptNum;
        #if defined(_TMS320C6X)
            interruptRegParams.corepacConfig.intVecNum = OSAL_REGINT_INTVEC_EVENT_COMBINER;
        #else
            interruptRegParams.corepacConfig.intVecNum = gpioHwAttr->highInterruptNum;
        #endif

            GPIO_osalRegisterInterrupt(&interruptRegParams,&(gGPIOMCB.hwiHandleHigh[instIndex]));

            /* Register the Low Priority Interrupt: */
            Osal_RegisterInterrupt_initParams(&interruptRegParams);
            interruptRegParams.corepacConfig.isrRoutine  = (&GPIO_LowPriorityISR);
            interruptRegParams.corepacConfig.arg         = (uintptr_t)(instIndex);

            interruptRegParams.corepacConfig.corepacEventNum = gpioHwAttr->lowInterruptNum;
        #if defined(_TMS320C6X)
            interruptRegParams.corepacConfig.intVecNum = OSAL_REGINT_INTVEC_EVENT_COMBINER;
        #else
            interruptRegParams.corepacConfig.intVecNum = gpioHwAttr->lowInterruptNum;
        #endif

            GPIO_osalRegisterInterrupt(&interruptRegParams,&(gGPIOMCB.hwiHandleLow[instIndex]));
        }
    }

    /* Driver has been initialized */
    gGPIOMCB.isInitialized = true;

    /* Now call set_config for all the pins. */
    for (i = 0; i < GPIO_v2_config.numberOfPinConfigs; i++)
    {
        if ((GPIO_v2_config.pinConfigs[i] & GPIO_DO_NOT_CONFIG) !=
            GPIO_DO_NOT_CONFIG)
        {
            GPIO_setConfig_v2(i, GPIO_v2_config.pinConfigs[i]);
        }
    }

    return;
}

/*
 *  ======== GPIO_clearInt ========
 */
static void GPIO_clearInt_v2(uint32_t index)
{
    int32_t     retVal = 0;
    uint32_t    baseAddr, inst, port, pin;

    retVal = GPIO_parseIndex(index, &baseAddr, &inst, &port, &pin);
    if (retVal == 0)
    {
        /* Check port value as only 4 port can generate interrupt. */
        if (port > GPIO_MAX_INTERRUPT_PORT)
        {
            retVal = -1;
        }
    }

    if (retVal == 0)
    {
        /* Clear the interrupt: */
        GPIO_clearInterrupt (baseAddr, port, pin);
    }
    return;
}

/*
 *  ======== GPIO_disableInt ========
 */
static void GPIO_disableInt_v2(uint32_t index)
{
    int32_t     retVal = 0;
    uint32_t    baseAddr, inst, port, pin;

    retVal = GPIO_parseIndex(index, &baseAddr, &inst, &port, &pin);
    if (retVal == 0)
    {
        /* Check port value as only 4 port can generate interrupt. */
        if (port > GPIO_MAX_INTERRUPT_PORT)
        {
            retVal = -1;
        }
    }

    if (retVal == 0)
    {
        /* Disable the interrupt: */
        GPIO_disableInterrupt (baseAddr, port, pin);
    }
    return;
}

/*
 *  ======== GPIO_enableInt ========
 */
static void GPIO_enableInt_v2(uint32_t index)
{
    int32_t     retVal = 0;
    uint32_t    baseAddr, inst, port, pin;

    retVal = GPIO_parseIndex(index, &baseAddr, &inst, &port, &pin);
    if (retVal == 0)
    {
        /* Check port value as only 4 port can generate interrupt. */
        if (port > GPIO_MAX_INTERRUPT_PORT)
        {
            retVal = -1;
        }
    }

    if (retVal == 0)
    {
        /* Enable the interrupt: */
        GPIO_enableInterrupt (baseAddr, port, pin);
    }
    return;
}

/*
 *  ======== GPIO_read ========
 */
static uint32_t GPIO_read_v2(uint32_t index)
{
    int32_t     retVal = 0;
    uint32_t    baseAddr, inst, port, pin;
    uint32_t    pinVal = 0;

    retVal = GPIO_parseIndex(index, &baseAddr, &inst, &port, &pin);

    if (retVal == 0)
    {
        pinVal = GPIO_getData(baseAddr, port, pin);
    }
    return pinVal;
}

/*
 *  ======== GPIO_setCallback ========
 */
static void GPIO_setCallback_v2(uint32_t index, GPIO_CallbackFxn callback)
{
    uint32_t key;
    if (index < GPIO_v2_config.numberOfCallbacks)
    {
        /* Disable preemption while configuring the GPIO */
        key = GPIO_osalHardwareIntDisable();

        if (GPIO_v2_config.callbacks[index] != callback)
        {
            GPIO_v2_config.callbacks[index] = callback;
        }

        /* Restore the interrupts */
        GPIO_osalHardwareIntRestore(key);
    }

    return;
}

/*
 *  ======== GPIO_setConfig ========
 */
static void GPIO_setConfig_v2(uint32_t index, GPIO_PinConfig pinConfig)
{
    int32_t     retVal = 0;
    uint32_t    baseAddr, inst, port, pin;
    uint32_t    key;
    GPIO_PinConfig  gpioPinConfig;

    retVal = GPIO_parseIndex(index, &baseAddr, &inst, &port, &pin);

    if (retVal == 0)
    {
        /* Disable preemption while configuring the GPIO */
        key = GPIO_osalHardwareIntDisable();

        /* Input or output? */
        if (pinConfig & GPIO_CFG_INPUT)
        {
            /* Input: Disable the output data direction */
            GPIO_setOutputDataDirection (baseAddr, port, pin, 0U);
        }
        else
        {
            /* Output: Set the data direction */
            GPIO_setOutputDataDirection (baseAddr, port, pin, 1U);

            /* Is the GPIO Port/Pin open open drain? */
            if (pinConfig & GPIO_CFG_OUT_OD_NOPULL)
            {
                /* YES: Open Drain */
                GPIO_setOpenDrainStatus (baseAddr, port, pin, 1U);
            }
        }

        /* Update pinConfig with the latest GPIO configuration */
        gpioPinConfig = GPIO_v2_config.pinConfigs[index];
        gpioPinConfig &= ~GPIO_CFG_IO_MASK;
        gpioPinConfig |= (pinConfig & GPIO_CFG_IO_MASK);
        GPIO_v2_config.pinConfigs[index] = gpioPinConfig;

        /* Do we need to enable interrupts for this? */
        /* Only the first 4 ports are capable of handling interrupts. */
        if (((pinConfig & GPIO_CFG_INT_MASK) != 0U) &&
            (port < GPIO_MAX_INTERRUPT_PORT))
        {
            /* Do we need to configure the interrupt on the falling edge? */
            if (pinConfig & GPIO_CFG_IN_INT_FALLING)
            {
                /* Honor the polarity for the falling edge */
                GPIO_honorPolarity (baseAddr, port, pin);
                GPIO_configPolarity (baseAddr, port, pin, 0U);
            }

            /* Do we need to configure the interrupt on the rising edge? */
            if (pinConfig & GPIO_CFG_IN_INT_RISING)
            {
                /* YES: Honor the polarity for the rising edge */
                GPIO_honorPolarity (baseAddr, port, pin);
                GPIO_configPolarity (baseAddr, port, pin, 1U);
            }

            /* Do we need to handle interrupts on both edges? */
            if (pinConfig & GPIO_CFG_IN_INT_BOTH_EDGES)
            {
                /* YES: Ignore the polarity since interrupts can be triggered on either edge */
                GPIO_ignorePolarity (baseAddr, port, pin);
            }

            /* Is the interrupt a high or low level? */
            if (pinConfig & GPIO_CFG_IN_INT_LOW)
            {
                /* YES: Mark this as a low level interrupt */
                GPIO_markLowLevelInterrupt (baseAddr, port, pin);
            }
            else
            {
                /* YES: Mark this as a high level interrupt */
                GPIO_markHighLevelInterrupt (baseAddr, port, pin);
            }

            /* Update pinConfig with the latest interrupt configuration */
            gpioPinConfig = GPIO_v2_config.pinConfigs[index];
            gpioPinConfig &= ~(GPIO_CFG_INT_MASK);
            gpioPinConfig |= (pinConfig & GPIO_CFG_INT_MASK);
            GPIO_v2_config.pinConfigs[index] = gpioPinConfig;
        }

        /* Store the index */
        gGPIOMCB.pinIndex[inst][port][pin] = index;

        /* Restore the interrupts */
        GPIO_osalHardwareIntRestore(key);
    }
    return;
}

/*
 *  ======== GPIO_toggle ========
 */
static void GPIO_toggle_v2(uint32_t index)
{
    int32_t     retVal = 0;
    uint32_t    baseAddr, inst, port, pin;
    uint32_t    key;

    retVal = GPIO_parseIndex(index, &baseAddr, &inst, &port, &pin);

    if (retVal == 0)
    {
        /* Disable preemption while configuring the GPIO */
        key = GPIO_osalHardwareIntDisable();

        /* Is the value on the pin currently 1 */
        if ((GPIO_v2_config.pinConfigs [index] & GPIO_CFG_OUT_HIGH) == GPIO_CFG_OUT_HIGH)
        {
            /* YES: Write a 0 */
            GPIO_write_v2 (index, 0U);
        }
        else
        {
            /* NO: Write a 1 */
            GPIO_write_v2 (index, 1U);
        }

        /* Restore the interrupts */
        GPIO_osalHardwareIntRestore(key);
    }
    return;
}

/*
 *  ======== GPIO_write ========
 */
static void GPIO_write_v2(uint32_t index, uint32_t value)
{
    int32_t     retVal = 0;
    uint32_t    baseAddr, inst, port, pin;
    uint32_t    key;

    retVal = GPIO_parseIndex(index, &baseAddr, &inst, &port, &pin);

    if (retVal == 0)
    {
        /* Disable preemption while configuring the GPIO */
        key = GPIO_osalHardwareIntDisable();

        /* Setup the pin configuration to track the last value written */
        if (value == 1U)
        {
            GPIO_v2_config.pinConfigs [index] = GPIO_v2_config.pinConfigs [index] | GPIO_CFG_OUT_HIGH;
        }
        else
        {
            GPIO_v2_config.pinConfigs [index] = GPIO_v2_config.pinConfigs [index] & (~GPIO_CFG_OUT_HIGH);
        }

        /* Restore the interrupts */
        GPIO_osalHardwareIntRestore(key);

        /* Send out the data */
        GPIO_sendData (baseAddr, port, pin, value);
    }
}

