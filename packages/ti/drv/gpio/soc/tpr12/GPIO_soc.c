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
 *   \file  tpr12/GPIO_soc.c
 *
 *  \brief  tpr12 SOC specific GPIO hardware attributes.
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
 *  Global Variable which points to the GPIO TPR12 hardware attributes
 */
#if defined (__TI_ARM_V7R4__)
static GPIO_v2_HwAttrs gMssGpioHwAtrrib =
{
    CSL_MSS_GIO_U_BASE,
    CSL_MSS_INTR_MSS_GIO_INT0,      /* High Interrupt   */
    CSL_MSS_INTR_MSS_GIO_INT1       /* Low Interrupt    */
};

static GPIO_v2_HwAttrs gRcssGpioHwAtrrib =
{
    CSL_RCSS_GIO_U_BASE,
    CSL_MSS_INTR_RCSS_GIO_INT0,     /* High Interrupt   */
    CSL_MSS_INTR_RCSS_GIO_INT1      /* Low Interrupt    */
};
#endif

#if defined (_TMS320C6X)
static GPIO_v2_HwAttrs gRcssGpioHwAtrrib =
{
    CSL_RCSS_GIO_U_BASE,
    CSL_DSS_INTR_RCSS_GIO_INT0,     /* High Interrupt   */
    CSL_DSS_INTR_RCSS_GIO_INT1      /* Low Interrupt    */
};

#endif

/**
 * \brief GPIO configuration structure for TPR12.
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
        &gRcssGpioHwAtrrib
    },
    {
        NULL,
        NULL,
        NULL
    }
};

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
        else if (gpioInst == GPIO_INST_RCSS)
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

