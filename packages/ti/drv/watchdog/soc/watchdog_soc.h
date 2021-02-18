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
 *  @defgroup DRV_WATCHDOG_SOC_MODULE Watchdog SOC specific interface
 *  @ingroup DRV_WATCHDOG_MODULE
 *
 *  @brief
 *  The section has SoC specific interface definitions for the Watchdog driver

 *  @{
 */

#ifndef WATCHDOG_SOC_H
#define WATCHDOG_SOC_H

#include <ti/csl/csl_utils.h>

#if defined(SOC_TPR12) || defined(SOC_AWR294X)
#include <ti/drv/watchdog/soc/watchdog_v0.h>
#define Watchdog_HwAttrs    Watchdog_v0_HwAttrs
extern void RTI_socEnableWatchdog();
extern void RTI_socTriggerWatchdogWarmReset();
extern int32_t Watchdog_socGetInitCfg(uint32_t idx, Watchdog_HwAttrs *cfg);
extern int32_t Watchdog_socSetInitCfg(uint32_t idx, const Watchdog_HwAttrs *cfg);
#define RTI_CLK_200M  (200000000U)
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

#endif /* WATCHDOG_SOC_H */
/* @} */

