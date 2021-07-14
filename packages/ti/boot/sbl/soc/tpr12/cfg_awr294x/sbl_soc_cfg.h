/*
 * Copyright (C) 2018-2020 Texas Instruments Incorporated - http://www.ti.com/
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

#ifndef SBL_SOC_CFG_H
#define SBL_SOC_CFG_H

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include <ti/csl/soc.h>


/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#define SBL_FREQ_MHZ2HZ(x)          ((x) * 1000 * 1000)
#define SBL_FREQ_HZ2MHZ(x)          ((x) / (1000 * 1000))

#define SBL_MCU1_CPU0_FREQ_HZ       SBL_FREQ_MHZ2HZ(400)

#define SBL_MCU1_CPU1_FREQ_HZ       (SBL_MCU1_CPU0_FREQ_HZ)

#define SBL_DSP1_C66X_FREQ_HZ       SBL_FREQ_MHZ2HZ(450)

#define SBL_SYSCLK_FREQ_HZ          (SBL_MCU1_CPU0_FREQ_HZ / 2)

#define SBL_MSS_SCIA_FREQ_HZ                             (SBL_FREQ_MHZ2HZ(200U))
#define SBL_MSS_SCIB_FREQ_HZ                             (SBL_FREQ_MHZ2HZ(200U))
#define SBL_MSS_RTIA_FREQ_HZ                             (SBL_FREQ_MHZ2HZ(200U))
#define SBL_MSS_RTIB_FREQ_HZ                             (SBL_FREQ_MHZ2HZ(200U))
#define SBL_MSS_RTIC_FREQ_HZ                             (SBL_FREQ_MHZ2HZ(200U))
#define SBL_MSS_WDT_FREQ_HZ                              (SBL_FREQ_MHZ2HZ(200U))
#define SBL_MSS_SPIA_FREQ_HZ                             (SBL_FREQ_MHZ2HZ(200U))
#define SBL_MSS_SPIB_FREQ_HZ                             (SBL_FREQ_MHZ2HZ(200U))
#define SBL_MSS_I2C_FREQ_HZ                              (SBL_FREQ_MHZ2HZ(200U))
#define SBL_MCANA_FREQ_HZ                                (SBL_FREQ_MHZ2HZ(80U))
#define SBL_MCANB_FREQ_HZ                                (SBL_FREQ_MHZ2HZ(80U))
#define SBL_CSIRX_FREQ_HZ                                (SBL_FREQ_MHZ2HZ(96U))
#define SBL_QSPI_FREQ_HZ                                 (SBL_FREQ_MHZ2HZ(80U))
#define SBL_DSS_RTIA_FREQ_HZ                             (SBL_FREQ_MHZ2HZ(200U))
#define SBL_DSS_RTIB_FREQ_HZ                             (SBL_FREQ_MHZ2HZ(200U))
#define SBL_DSS_WDT_FREQ_HZ                              (SBL_FREQ_MHZ2HZ(200U))
#define SBL_DSS_SCIA_FREQ_HZ                             (SBL_FREQ_MHZ2HZ(200U))
#define SBL_CPSW_FREQ_HZ                                 (SBL_FREQ_MHZ2HZ(200U))

#define SBL_MSS_SCIA_SYS150_FREQ_HZ                      (SBL_FREQ_MHZ2HZ(150U))
#define SBL_MSS_SCIB_SYS150_FREQ_HZ                      (SBL_FREQ_MHZ2HZ(150U))
#define SBL_MSS_RTIA_SYS150_FREQ_HZ                      (SBL_FREQ_MHZ2HZ(150U))
#define SBL_MSS_RTIB_SYS150_FREQ_HZ                      (SBL_FREQ_MHZ2HZ(150U))
#define SBL_MSS_RTIC_SYS150_FREQ_HZ                      (SBL_FREQ_MHZ2HZ(150U))
#define SBL_MSS_WDT_SYS150_FREQ_HZ                       (SBL_FREQ_MHZ2HZ(150U))
#define SBL_MSS_SPIA_SYS150_FREQ_HZ                      (SBL_FREQ_MHZ2HZ(150U))
#define SBL_MSS_SPIB_SYS150_FREQ_HZ                      (SBL_FREQ_MHZ2HZ(150U))
#define SBL_MSS_I2C_SYS150_FREQ_HZ                       (SBL_FREQ_MHZ2HZ(150U))
#define SBL_MCANA_SYS150_FREQ_HZ                         (SBL_FREQ_MHZ2HZ(60U))
#define SBL_MCANB_SYS150_FREQ_HZ                         (SBL_FREQ_MHZ2HZ(60U))
#define SBL_CSIRX_SYS150_FREQ_HZ                         (SBL_FREQ_MHZ2HZ(96U))
#define SBL_QSPI_SYS150_FREQ_HZ                          (SBL_FREQ_MHZ2HZ(60U))
#define SBL_DSS_RTIA_SYS150_FREQ_HZ                      (SBL_FREQ_MHZ2HZ(150U))
#define SBL_DSS_RTIB_SYS150_FREQ_HZ                      (SBL_FREQ_MHZ2HZ(150U))
#define SBL_DSS_WDT_SYS150_FREQ_HZ                       (SBL_FREQ_MHZ2HZ(150U))
#define SBL_DSS_SCIA_SYS150_FREQ_HZ                      (SBL_FREQ_MHZ2HZ(150U))
#define SBL_CPSW_SYS150_FREQ_HZ                          (SBL_FREQ_MHZ2HZ(150U))

/* Macro representing the offset where the App Image has to be written/Read from
   the QSPI Flash.
*/
#define QSPI_OFFSET_SI              (0xA0000U)

typedef enum Rcm_ModuleClkInitStage_e
{
    Rcm_ModuleClkInitStage_PRE_APLL_SWITCH,
    Rcm_ModuleClkInitStage_POST_APLL_SWITCH,
} Rcm_ModuleClkInitStage;

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */
void SBL_SocEarlyInit();
void SBL_SocLateInit(void);
extern void SBL_moduleClockInit(Rcm_ModuleClkInitStage initStage);

#endif
