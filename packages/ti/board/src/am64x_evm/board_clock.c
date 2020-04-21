/******************************************************************************
 * Copyright (c) 2019 Texas Instruments Incorporated - http://www.ti.com
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
 *
 *****************************************************************************/

 /** \file board_clock.c
  *
  *  \brief This file contains initialization of wakeup and main PSC
  *  configuration structures and function definitions to get the number
  *  of wakeup and main PSC config exists.
  */

#include "board_clock.h"
#include <ti/drv/sciclient/sciclient.h>

/**
 * \brief wkup PSC configuration parameters
 *
 *  This structure provides the device-level view with module association to
 *  the clock, power, and voltage domains.
 * 
 *  The PSC provides the user with an interface to control several important
 *  power and clock operations. The device has two PSC - WKUP_PSC0 and PSC0
 *  in WKUPSS and MAIN SoC, respectively.
 * 
 *  PSC: The Power Sleep Controller is the device has several power domains
 *  that can be turned ON for operation or OFF to minimize power dissipation,
 *  which includes a Global Power Sleep Controller(GPSC) and Local Power 
 *  Sleep Controller(LPSC).
 * 
 *  GPSC: Global Power Sleep Controller, is used to control the power gating
 *  of various power domains.
 * 
 *  LPSC: Local Power Sleep Controller, manages the clock gating for to each 
 *  logic block. For modules with a dedicated clock or multiple clocks, the 
 *  LPSC communicates with the PLL controller to enable and disable that
 *  module's clock(s) at the source. For modules that share a clock with
 *  other modules, the LPSC controls the clock gating logic for each module.    
 */
const pscConfig wkupPscConfigs[] =
{

};

/**
 * \brief main PSC configuration parameters
 *
 *  This structure provides the device-level view with module association to
 *  the clock, power, and voltage domains.
 * 
 *  The PSC provides the user with an interface to control several important
 *  power and clock operations. The device has two PSC - WKUP_PSC0 and PSC0
 *  in WKUPSS and MAIN SoC, respectively.
 * 
 *  PSC: The Power Sleep Controller is the device has several power domains
 *  that can be turned ON for operation or OFF to minimize power dissipation,
 *  which includes a Global Power Sleep Controller(GPSC) and Local Power 
 *  Sleep Controller(LPSC).
 * 
 *  GPSC: Global Power Sleep Controller, is used to control the power gating
 *  of various power domains.
 * 
 *  LPSC: Local Power Sleep Controller, manages the clock gating for to each 
 *  logic block. For modules with a dedicated clock or multiple clocks, the 
 *  LPSC communicates with the PLL controller to enable and disable that
 *  module's clock(s) at the source. For modules that share a clock with
 *  other modules, the LPSC controls the clock gating logic for each module.    
 */
const pscConfig mainPscConfigs[] =
{

};

/**
 *  \brief    This function is used to get the number of \
 *            wkup PSC configs exists.
 *
 * \return
 * \n         uint32_t - Number of wkup PSC configs.
 */
uint32_t Board_getNumWkupPscCconfigs(void)
{
    return (sizeof(wkupPscConfigs) / sizeof(pscConfig));
}

/**
 *  \brief    This function is used to get the number of \
 *            main PSC configs exists.
 *
 * \return
 * \n         uint32_t - Number of main PSC configs.
 */
uint32_t Board_getNumMainPscCconfigs(void)
{
    return (sizeof(mainPscConfigs) / sizeof(pscConfig));
}

Board_STATUS Board_moduleClockInit(void)
{
    Board_STATUS status = BOARD_SOK;
#if defined(__TI_ARM_V7R4__)
    int32_t      ret;
    uint64_t     mcuClkFreq;
#if 0
    ret = Sciclient_pmGetModuleClkFreq(TISCI_DEV_MCU_ARMSS0_CPU0,
                                       TISCI_DEV_MCU_ARMSS0_CPU0_BUS_CPU_CLK,
                                       &mcuClkFreq,
                                       SCICLIENT_SERVICE_WAIT_FOREVER);
#else
    /* 
     * Temporary hard-code R5 core clock to 800 MHz (default on Zebu)
     * until Sciclent PM is supported
     */
    mcuClkFreq = 800000000U;
    ret = 0U;
#endif
    if(ret == 0)
    {
        Osal_HwAttrs  hwAttrs;
        uint32_t      ctrlBitmap;

        ret = Osal_getHwAttrs(&hwAttrs);
        if(ret == 0)
        {
            /*
             * Change the timer input clock frequency configuration
               based on R5 CPU clock configured
             */
            hwAttrs.cpuFreqKHz = (int32_t)(mcuClkFreq/1000U);
            ctrlBitmap         = OSAL_HWATTR_SET_CPU_FREQ;
            ret = Osal_setHwAttrs(ctrlBitmap, &hwAttrs);
        }
    }
    if(ret != 0)
    {
        status = BOARD_INIT_CLOCK_FAIL;
    }
#endif    
    return (status);
}
