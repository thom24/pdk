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
 *  \file hwa_soc.h
 *
 *  \brief HWA Driver SOC specific file.
 */
 
 /**
 @defgroup HWA_RAM_Size            HWA RAMs Size
 @ingroup DRV_HWA_MODULE
 @brief
 *   The section has a list of RAM sizes in bytes for all HWA RAMs
 */
 
/**
 @defgroup HWA_ADDR_Transfer       Transfer CPU address to HWA address
 @ingroup DRV_HWA_MODULE
 @brief
 *   transfer the CPU address to HWA address 
 */
#ifndef HWA_SOC_H_
#define HWA_SOC_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#ifdef __cplusplus
extern "C" {
#endif
#include <ti/csl/soc/tpr12/src/cslr_soc_baseaddress.h>
#include <ti/csl/soc/tpr12/src/cslr_dss_ctrl.h>

#ifdef BUILD_DSP_1
#include <ti/csl/soc/tpr12/src/cslr_intr_dss.h>
#endif

#ifdef BUILD_MCU
#include <ti/csl/soc/tpr12/src/cslr_intr_mss.h>
#endif

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/*! \brief number of HWA instance */
#define HWA_NUM_INSTANCES            (1U)
/*! \brief number of HWA memory banks */
#define SOC_HWA_NUM_MEM_BANKS           (8U)
/*! \brief number of HWA parameter sets */
#define SOC_HWA_NUM_PARAM_SETS          (64U)
/*! \brief number of HWA MDA channels */
#define SOC_HWA_NUM_DAM_CHANNEL         (32U)
/*! \brief number of HWA memory size in bytes */
#define SOC_HWA_MEM_SIZE                (131072U)
/*! \brief number of csirx IRQs*/
#define SOC_HWA_NUM_CSIRX_IRQS          (20U)


 /** @addtogroup HWA_RAM_Size
 @{ */

#define HWA_RAM_WINDOW_SIZE_IN_BYTES                     8192      /*!<  Window RAM size in bytes. See \ref HWA_configRam */
#define HWA_RAM_VECTORMULTIPLY_SIZE_IN_BYTES             8192      /*!<  internal ram for pre-loaded complex vector used in vector multiplication mode 1 and mode 2, for upto 1024 complex samples */
#define HWA_RAM_LUT_FREQ_DEROTATE_SIZE_IN_BYTES          256       /*!<  stores upto 64 difference frequency de-rotation and starting phase values, each entry is 32 bits wide */
#define HWA_RAM_SHUFFLE_RAM_SIZE_IN_BYTES                512       /*!<  stores 256 element with 14-bits number for shuffled addressing */
#define HWA_RAM_HIST_THRESH_RAM_SIZE_IN_BYTES            8192      /*!<  if CDF count threshold mode is enabled, RAM stores the CFT bin number, CDF count, and histogram count at the bin, which hits the CDF threshold*/
#define HWA_RAM_2DSTAT_ITER_VAL_SIZE_IN_BYTES            4096      /*!<  2-D maximum array, 1024 elements with 24 bits each, for maximum values of each iteration */
#define HWA_RAM_2DSTAT_ITER_IDX_SIZE_IN_BYTES            2048      /*!<  2-D maximum array, 1024 elements with 10 bits each, for maximum locations of each iteration */
#define HWA_RAM_2DSTAT_SMPL_VAL_SIZE_IN_BYTES            1024      /*!<  2-D maximum array, 256 elements with 24 bits each, for maximum values correspoinding to each sample index */
#define HWA_RAM_2DSTAT_SMPL_IDX_SIZE_IN_BYTES            512       /*!<  2-D maximum array, 256 elements with 10 bits each, for maximum locations correspoinding to each sample index */
#define HWA_RAM_HIST_RAM_SIZE_IN_BYTES                   8192      /*!< histogram output corresponding to each sample index */

/** @}*/ /* end addtogroup HWA_RAM_Size */

/** @addtogroup HWA_ADDR_Transfer
 @{ */
 
/*! @brief transfer the CPU address to HWA address */
#define HWADRV_ADDR_TRANSLATE_CPU_TO_HWA(x)  (uint32_t)(((uint32_t)(x) - CSL_DSS_HWA_DMA0_U_BASE) & 0x000FFFFFU)

/** @}*/ /* end addtogroup HWA_ADDR_Transfer  */


/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                       Static Function Definitions                          */
/* ========================================================================== */

/* None */

#ifdef __cplusplus
}
#endif

#endif /* #ifndef HWA_SOC_TOP_H_ */
