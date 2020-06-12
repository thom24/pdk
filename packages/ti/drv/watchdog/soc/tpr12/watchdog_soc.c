/*
 * Copyright (C) 2020 Texas Instruments Incorporated - http://www.ti.com/
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of Texas Instruments Incorporated nor the names of
 * its contributors may be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

/**
 *  \file   watchdog_soc.c
 *
 *  \brief  TPR12 device specific hardware attributes.
 *
 */

/**************************************************************************
 *************************** Include Files ********************************
 **************************************************************************/

#include <ti/csl/soc.h>
#include <ti/drv/watchdog/soc/watchdog_soc.h>

/**************************************************************************
 ************************** Global Variables ******************************
 **************************************************************************/
/**************************************************************************
 * On TPR12, Watchdog is available on both the DSS and MSS. The Watchdog
 * platform configuration is MSS and DSS specific
 **************************************************************************/

#if defined(BUILD_MCU)

/**
 * @brief   There is only 1 Watchdog instance
 * available. This should *NOT* be modified by the customer.
 */
Watchdog_HwAttrs gWatchdogHwCfg[1] =
{
    /* Watchdog Hardware configuration:
     * - Base address of memory mapped registers
     * - ESM group number
     * - ESM error number */
    {
        CSL_MSS_RTIB_U_BASE,
        2,
        ESMG2_WDT_NMI_REQ 
    }
};

Watchdog_Config Watchdog_config[] =
{
    {
        &gWatchdogFxnTable,             /* Watchdog Driver Function Table:     */
        (void *)NULL,                   /* Watchdog Driver Object:             */
        (void *)&gWatchdogHwCfg         /* Watchdog Hw configuration:          */
    },
    {
        NULL,
        NULL,
        NULL
    }
};

#elif defined (BUILD_DSP_1)
/**
 * @brief   There is only 1 Watchdog instance
 * available. This should *NOT* be modified by the customer.
 */
Watchdog_HwAttrs gWatchdogHwCfg[1] =
{
    /* Watchdog Hardware configuration:
     * - Base address of memory mapped registers
     * - ESM group number
     * - ESM error number */
    {
        CSL_DSS_RTIA_U_BASE,
        2,
        CSL_DSS_ESMG2_DSS_WDT_NMI_REQ 
    }
};

Watchdog_Config Watchdog_config[] =
{
    {
        &gWatchdogFxnTable,             /* Watchdog Driver Function Table:     */
        (void *)NULL,                   /* Watchdog Driver Object:             */
        (void *)&gWatchdogHwCfg         /* Watchdog Hw configuration:          */
    },
    {
        NULL,
        NULL,
        NULL
    }
};
#endif
