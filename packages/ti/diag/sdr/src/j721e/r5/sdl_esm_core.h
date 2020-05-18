/*
 * SDR ESM
 *
 * Software Diagnostics Reference module for Error Signaling Module
 *
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
 *
 */

#ifndef INCLUDE_SDR_ESM_CORE_H_
#define INCLUDE_SDR_ESM_CORE_H_

#include <ti/csl/csl_esm.h>

/* Enumerate Interrupt number for the different esm interrupts */
#define SDR_ESM_HI_INTNO  CSLR_MCU_R5FSS0_CORE0_INTR_MCU_ESM0_ESM_INT_HI_LVL_0
#define SDR_ESM_LO_INTNO  CSLR_MCU_R5FSS0_CORE0_INTR_MCU_ESM0_ESM_INT_LOW_LVL_0
#define SDR_ESM_CFG_INTNO CSLR_MCU_R5FSS0_CORE0_INTR_MCU_ESM0_ESM_INT_CFG_LVL_0

/* Enumerate ESM events for R5F core handled by SDR */
#define SDR_ESM_MCU_R5_CORE0_SEC_INT           CSLR_MCU_ESM0_ESM_LVL_EVENT_MCU_R5FSS0_CORE0_ECC_CORRECTED_LEVEL_0
#define SDR_ESM_MCU_R5_CORE0_DED_INT           CSLR_MCU_ESM0_ESM_LVL_EVENT_MCU_R5FSS0_CORE0_ECC_UNCORRECTED_LEVEL_0
#define SDR_ESM_MCU_R5_CORE1_SEC_INT           CSL_ESM1_INTR_MCU0_CPU1_ECC_CORRECTED_LEVEL
#define SDR_ESM_MCU_R5_CORE1_DED_INT           CSL_ESM1_INTR_MCU0_CPU1_ECC_UNCORRECTED_LEVEL

#define SDR_ESM_MCU_CPU_MISCOMPARE             CSL_ESM1_INTR_MCU0_CPU_MISCOMPARE_LEVEL
#define SDR_ESM_MCU_RTI0_WWD_INT               CSLR_MCU_ESM0_ESM_LVL_EVENT_MCU_RTI0_INTR_WWD_0
#define SDR_ESM_MCU_RTI1_WWD_INT               CSLR_MCU_ESM0_ESM_LVL_EVENT_MCU_RTI1_INTR_WWD_0
#define SDR_ESM_MAIN_ESM_ERROR_INT             CSLR_MCU_ESM0_ESM_LVL_EVENT_GLUELOGIC_ESM_MAIN_ERR_GLUE_ERR_I_N_0
#define SDR_ESM_MCU_R5_SELFTEST_ERR_INT        CSLR_MCU_ESM0_ESM_PLS_EVENT0_MCU_R5FSS0_SELFTEST_ERR_PULSE_0
#define SDR_ESM_MCU_R5_CPU_BUS_CMP_ERR         CSLR_MCU_ESM0_ESM_PLS_EVENT0_MCU_R5FSS0_COMPARE_ERR_PULSE_0
#define SDR_ESM_MCU_R5_INACTIVITY_ERR_INT      CSLR_MCU_ESM0_ESM_PLS_EVENT0_MCU_R5FSS0_BUS_MONITOR_ERR_PULSE_0
#define SDR_ESM_MCU_R5_VIM_BUS_CMP_ERR_INT     CSLR_MCU_ESM0_ESM_PLS_EVENT0_MCU_R5FSS0_VIM_COMPARE_ERR_PULSE_0
#define SDR_ESM_MCU_R5_CCM_STAT_ERR_INT        CSLR_MCU_ESM0_ESM_PLS_EVENT0_MCU_R5FSS0_CCM_COMPARE_STAT_PULSE_INTR_0

#endif /* INCLUDE_SDR_ESM_CORE_H_ */
