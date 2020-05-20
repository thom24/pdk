/*
 * Copyright (c) 2019, Texas Instruments Incorporated
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
  *  ======== hwa_tpr12.c ========
  */

#include <stdint.h>

#include <ti/osal/osal.h>
#include <ti/drv/hwa/hwa.h>
#include <ti/drv/hwa/src/V1/hwa_internal.h>
#include <ti/drv/hwa/soc/hwa_soc.h>



#define NUM_HWA_PARAMSETS_PER_INSTANCE          SOC_HWA_NUM_PARAM_SETS
#define NUM_HWA_DMACHANNELS_PER_INSTANCE        SOC_HWA_NUM_DAM_CHANNEL

/* storage for HWA driver object handles */
HWA_Driver *gHWADriverPtr[HWA_NUM_INSTANCES] = { NULL };

HWA_RAMAttrs gHWARAMCfg[HWA_NUM_RAMS] = {
    { SOC_TPR12_HWA_WINDOM_RAM_ADDRESS , HWA_RAM_WINDOW_SIZE_IN_BYTES },
    { SOC_TPR12_HWA_MULT_RAM_ADDRESS , HWA_RAM_VECTORMULTIPLY_SIZE_IN_BYTES },
    { SOC_TRP12_HWA_DEROT_RAM_ADDRESS , HWA_RAM_LUT_FREQ_DEROTATE_SIZE_IN_BYTES } ,
    { SOC_TPR12_HWA_SHUFFLE_RAM_ADDRESS ,HWA_RAM_SHUFFLE_RAM_SIZE_IN_BYTES } ,
    { SOC_TPR12_HWA_HIST_THRESH_RAM_ADDRESS , HWA_RAM_HIST_THRESH_RAM_SIZE_IN_BYTES } ,
    { SOC_TPR12_HWA_2DSTAT_ITER_VAL_RAM_ADDRESS , HWA_RAM_2DSTAT_ITER_VAL_SIZE_IN_BYTES } ,
    { SOC_TPR12_HWA_2DSTAT_ITER_IDX_RAM_ADDRESS , HWA_RAM_2DSTAT_ITER_IDX_SIZE_IN_BYTES },
    { SOC_TPR12_HWA_2DSTAT_SMPL_VAL_RAM_ADDRESS , HWA_RAM_2DSTAT_SMPL_VAL_SIZE_IN_BYTES } ,
    { SOC_TPR12_HWA_2DSTAT_SMPL_IDX_RAM_ADDRESS , HWA_RAM_2DSTAT_SMPL_IDX_SIZE_IN_BYTES } ,
    { SOC_TPR12_HWA_HIST_RAM_ADDRESS , HWA_RAM_HIST_RAM_SIZE_IN_BYTES }

};
/* TPR12 DSS/MSS specific HWA hardware attributes */
HWA_HWAttrs gHWAHwCfg[HWA_NUM_INSTANCES] =
{
    /* HWA1 Hardware configuration:
     */
    {
        0U,
        SOC_TPR12_HWA_COMMON_BASE_ADDRESS,    /* HWA base address for control/common register */
        SOC_TPR12_HWA_PARAM_BASE_ADDRESS,     /* HWA base address for the paramset */
        SOC_TPR12_HWA_WINDOM_RAM_ADDRESS,     /* HWA base address for window ram */
        SOC_TPR12_DSS_DSSREG_BASE_ADDRESS,    /* DSS_CTRL base address */
        NUM_HWA_PARAMSETS_PER_INSTANCE,       /* number of HWA paramsets */
#ifdef BUILD_DSP_1
        SOC_TPR12_DSS_INTC_EVENT1_HW_ACC_PARAM_DONE,    /* intNumParamSet HWA Peripheral's interrupt vector 1 for individual paramset completion */
        SOC_TPR12_DSS_INTC_EVENT2_HW_ACC_PARAM_DONE,    /* intNumParamSet HWA Peripheral's interrupt vector 2 for individual paramset completion */
        SOC_TPR12_DSS_INTC_EVENT_HW_ACC_DONE,           /* intNumDone HWA Peripheral's interrupt vector for completion of all programmed paramset */
        SOC_TPR12_DSS_INTC_EVENT_HW_ACC_ALT_DONE,       /* intNumDone HWA Peripheral's interrupt vector for completion of all programmed paramset in ALT thread */
#else //BUILD_MCU
        SOC_TPR12_MSS_INTC_EVENT1_HW_ACC_PARAM_DONE,    /* intNumParamSet HWA Peripheral's interrupt vector 1 for individual paramset completion */
        SOC_TPR12_MSS_INTC_EVENT2_HW_ACC_PARAM_DONE,    /* intNumParamSet HWA Peripheral's interrupt vector 2 for individual paramset completion */
        SOC_TPR12_MSS_INTC_EVENT_HW_ACC_DONE,           /* intNumDone HWA Peripheral's interrupt vector for completion of all programmed paramset */
        SOC_TPR12_MSS_INTC_EVENT_HW_ACC_ALT_DONE,       /* intNumDone HWA Peripheral's interrupt vector for completion of all programmed paramset in ALT thread  */
#endif
        NUM_HWA_DMACHANNELS_PER_INSTANCE,          /* number of DMA channels available for HWA */
        SOC_TPR12_HWA_MEM0_BASE_ADDRESS,             /* HWA Accelerator processing memory base address */
        SOC_HWA_MEM_SIZE,                                /* HWA Accelerator processing memory size in bytes */
        true
    }
};


