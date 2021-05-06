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
 *   \file  awr294x/GPIO_soc.c
 *
 *  \brief  awr294x SOC specific GPIO hardware attributes.
 *
 *   This file contains the GPIO hardware attributes like base address and
 *   interrupt ids.
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdint.h>
#include <string.h>
#include <ti/csl/csl_utils.h>
#include <ti/csl/soc.h>
#include <ti/drv/gpio/soc/GPIO_soc.h>

#include <ti/drv/gpio/src/v2/GPIO_v2_priv.h>
/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/**
 * \brief
 *  Global Variable which points to the GPIO AWR294X hardware attributes
 */
#if defined (__TI_ARM_V7R4__)
static GPIO_v2_HwAttrs gMssGpioHwAtrrib =
{
    CSL_MSS_GIO_U_BASE,
    CSL_MSS_INTR_MSS_GIO_INT0,      /* High Interrupt   */
    CSL_MSS_INTR_MSS_GIO_INT1       /* Low Interrupt    */
};

static GPIO_v2_HwAttrs gRSSGpioHwAtrrib =
{
    CSL_RSS_GIO_U_BASE,
    CSL_MSS_INTR_RSS_GIO_INT0,     /* High Interrupt   */
    CSL_MSS_INTR_RSS_GIO_INT1      /* Low Interrupt    */
};
#endif

#if defined (_TMS320C6X)
static GPIO_v2_HwAttrs gRSSGpioHwAtrrib =
{
    CSL_RSS_GIO_U_BASE,
    CSL_DSS_INTR_RSS_GIO_INT0,     /* High Interrupt   */
    CSL_DSS_INTR_RSS_GIO_INT1      /* Low Interrupt    */
};

#endif

/**
 * \brief GPIO configuration structure for AWR294X.
 */
CSL_PUBLIC_CONST GPIOConfigList GPIO_config =
{
    {
    #if defined (_TMS320C6X)
        &GPIO_FxnTable_v2,
        NULL,
        NULL
    #else
        &GPIO_FxnTable_v2,
        NULL,
        &gMssGpioHwAtrrib
    #endif
    },
    {
        &GPIO_FxnTable_v2,
        NULL,
        &gRSSGpioHwAtrrib
    },
    {
        NULL,
        NULL,
        NULL
    }
};

/* GPIO Driver pin configuration structure */
static GPIO_PinConfig gGpioPinConfigs[GPIO_ALL_INST_MAX_PINS] =
{
    /* Place holder for pin config for all pins.
       By default no pins will be configured at init */
    GPIO_DO_NOT_CONFIG, GPIO_DO_NOT_CONFIG, GPIO_DO_NOT_CONFIG, GPIO_DO_NOT_CONFIG,
    GPIO_DO_NOT_CONFIG, GPIO_DO_NOT_CONFIG, GPIO_DO_NOT_CONFIG, GPIO_DO_NOT_CONFIG,

    GPIO_DO_NOT_CONFIG, GPIO_DO_NOT_CONFIG, GPIO_DO_NOT_CONFIG, GPIO_DO_NOT_CONFIG,
    GPIO_DO_NOT_CONFIG, GPIO_DO_NOT_CONFIG, GPIO_DO_NOT_CONFIG, GPIO_DO_NOT_CONFIG,

    GPIO_DO_NOT_CONFIG, GPIO_DO_NOT_CONFIG, GPIO_DO_NOT_CONFIG, GPIO_DO_NOT_CONFIG,
    GPIO_DO_NOT_CONFIG, GPIO_DO_NOT_CONFIG, GPIO_DO_NOT_CONFIG, GPIO_DO_NOT_CONFIG,

    GPIO_DO_NOT_CONFIG, GPIO_DO_NOT_CONFIG, GPIO_DO_NOT_CONFIG, GPIO_DO_NOT_CONFIG,
    GPIO_DO_NOT_CONFIG, GPIO_DO_NOT_CONFIG, GPIO_DO_NOT_CONFIG, GPIO_DO_NOT_CONFIG,

    GPIO_DO_NOT_CONFIG, GPIO_DO_NOT_CONFIG, GPIO_DO_NOT_CONFIG, GPIO_DO_NOT_CONFIG,
    GPIO_DO_NOT_CONFIG, GPIO_DO_NOT_CONFIG, GPIO_DO_NOT_CONFIG, GPIO_DO_NOT_CONFIG,

    GPIO_DO_NOT_CONFIG, GPIO_DO_NOT_CONFIG, GPIO_DO_NOT_CONFIG, GPIO_DO_NOT_CONFIG,
    GPIO_DO_NOT_CONFIG, GPIO_DO_NOT_CONFIG, GPIO_DO_NOT_CONFIG, GPIO_DO_NOT_CONFIG,

    GPIO_DO_NOT_CONFIG, GPIO_DO_NOT_CONFIG, GPIO_DO_NOT_CONFIG, GPIO_DO_NOT_CONFIG,
    GPIO_DO_NOT_CONFIG, GPIO_DO_NOT_CONFIG, GPIO_DO_NOT_CONFIG, GPIO_DO_NOT_CONFIG,

    GPIO_DO_NOT_CONFIG, GPIO_DO_NOT_CONFIG, GPIO_DO_NOT_CONFIG, GPIO_DO_NOT_CONFIG,
    GPIO_DO_NOT_CONFIG, GPIO_DO_NOT_CONFIG, GPIO_DO_NOT_CONFIG, GPIO_DO_NOT_CONFIG
};

/* GPIO Driver call back functions */
static GPIO_CallbackFxn gGpioCallbackFunctions[GPIO_ALL_INST_MAX_PINS] = {0U};

/* ========================================================================== */
/*                          Function Definition                               */
/* ========================================================================== */
int32_t GPIO_getHwAttr (GPIO_v2_HwAttrs **gpioHwAttr, uint32_t gpioInst)
{
    int32_t retVal = 0;
    if (gpioHwAttr == NULL)
    {
        retVal = -1;
    }
    if (retVal == 0)
    {
        if (gpioInst == GPIO_INST_MCU)
        {
            *gpioHwAttr = (GPIO_v2_HwAttrs *)(GPIO_config[0].hwAttrs);
        }
        else if (gpioInst == GPIO_INST_RSS)
        {
            *gpioHwAttr = (GPIO_v2_HwAttrs *)(GPIO_config[1].hwAttrs);
        }
        else
        {
            *gpioHwAttr = NULL;
            retVal = -1;
        }
    }
    return retVal;
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

int32_t GPIO_parseIndex(uint32_t index, uint32_t* baseAddr, uint32_t* inst, uint32_t* port, uint32_t* pin)
{
    int32_t     retVal = 0;
    GPIO_v2_HwAttrs  *gpioHwAttr = NULL;

    if (index > SOC_AWR294X_GPIO_MAX)
    {
        retVal = -1;
    }
    if ((baseAddr == NULL) || (inst == NULL) || (port == NULL) || (pin == NULL))
    {
        retVal = -1;
    }
    if (retVal == 0)
    {
        GPIO_decodeIndex(index, inst, port, pin);
        /* Check if the inst is supported. */
        retVal = GPIO_getHwAttr(&gpioHwAttr, *inst);
    }
    if ((retVal == 0) && (gpioHwAttr != NULL))
    {
        *baseAddr = gpioHwAttr->gpioBaseAddr;
    }
    return retVal;
}

int32_t GPIO_v2_getConfig(uint32_t index, GPIO_PinConfig *pinConfig)
{
    int32_t retVal = 0;
    if ((pinConfig == NULL) || (index > SOC_AWR294X_GPIO_MAX))
    {
        retVal = -1;
    }
    if (retVal == 0)
    {
        *pinConfig = gGpioPinConfigs[index];
    }
    return retVal;
}

int32_t GPIO_v2_setConfig(uint32_t index, GPIO_PinConfig pinConfig)
{
    int32_t retVal = 0;
    if (index > SOC_AWR294X_GPIO_MAX)
    {
        retVal = -1;
    }
    if (retVal == 0)
    {
        gGpioPinConfigs[index] = pinConfig;
    }
    return retVal;
}

int32_t GPIO_v2_getCallback(uint32_t index, GPIO_CallbackFxn *callback)
{
    int32_t retVal = 0;
    if ((callback == NULL) || (index > SOC_AWR294X_GPIO_MAX))
    {
        retVal = -1;
    }
    if (retVal == 0)
    {
        *callback = gGpioCallbackFunctions[index];
    }
    return retVal;
}

int32_t GPIO_v2_setCallback(uint32_t index, GPIO_CallbackFxn callback)
{
    int32_t retVal = 0;
    if (index > SOC_AWR294X_GPIO_MAX)
    {
        retVal = -1;
    }
    if (retVal == 0)
    {
        gGpioCallbackFunctions[index] = callback;
    }
    return retVal;
}
