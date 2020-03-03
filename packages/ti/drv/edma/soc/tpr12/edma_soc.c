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
 *   @file  edma_soc.c
 *
 *   @brief
 *      edma soc specific configuration.
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdint.h>
#include <ti/drv/edma/edma.h>
#include <ti/drv/edma/soc/edma_soc_priv.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */
#ifdef BUILD_DSP_1
#define EDMA_DSS_CC0_INTAGG_MASK_REG_ADDRESS    (CSL_DSS_CTRL_U_BASE + CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_MASK)
#define EDMA_DSS_CC0_INTAGG_STATUS_REG_ADDRESS  (CSL_DSS_CTRL_U_BASE + CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_STATUS)
#define EDMA_DSS_CC0_ERRAGG_MASK_REG_ADDRESS    (CSL_DSS_CTRL_U_BASE + CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_MASK)
#define EDMA_DSS_CC0_ERRAGG_STATUS_REG_ADDRESS  (CSL_DSS_CTRL_U_BASE + CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_STATUS)

#define EDMA_DSS_CC1_INTAGG_MASK_REG_ADDRESS    (CSL_DSS_CTRL_U_BASE + CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_MASK)
#define EDMA_DSS_CC1_INTAGG_STATUS_REG_ADDRESS  (CSL_DSS_CTRL_U_BASE + CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_STATUS)
#define EDMA_DSS_CC1_ERRAGG_MASK_REG_ADDRESS    (CSL_DSS_CTRL_U_BASE + CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_MASK)
#define EDMA_DSS_CC1_ERRAGG_STATUS_REG_ADDRESS  (CSL_DSS_CTRL_U_BASE + CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_STATUS)

#define EDMA_DSS_CC2_INTAGG_MASK_REG_ADDRESS    (CSL_DSS_CTRL_U_BASE + CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_MASK)
#define EDMA_DSS_CC2_INTAGG_STATUS_REG_ADDRESS  (CSL_DSS_CTRL_U_BASE + CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_STATUS)
#define EDMA_DSS_CC2_ERRAGG_MASK_REG_ADDRESS    (CSL_DSS_CTRL_U_BASE + CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_MASK)
#define EDMA_DSS_CC2_ERRAGG_STATUS_REG_ADDRESS  (CSL_DSS_CTRL_U_BASE + CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_STATUS)

#define EDMA_DSS_CC3_INTAGG_MASK_REG_ADDRESS    (CSL_RCSS_CTRL_U_BASE + CSL_RCSS_CTRL_RCSS_TPCC_A_INTAGG_MASK)
#define EDMA_DSS_CC3_INTAGG_STATUS_REG_ADDRESS  (CSL_RCSS_CTRL_U_BASE + CSL_RCSS_CTRL_RCSS_TPCC_A_INTAGG_STATUS)
#define EDMA_DSS_CC3_ERRAGG_MASK_REG_ADDRESS    (CSL_RCSS_CTRL_U_BASE + CSL_RCSS_CTRL_RCSS_TPCC_A_ERRAGG_MASK)
#define EDMA_DSS_CC3_ERRAGG_STATUS_REG_ADDRESS  (CSL_RCSS_CTRL_U_BASE + CSL_RCSS_CTRL_RCSS_TPCC_A_ERRAGG_STATUS)
#endif

#ifdef BUILD_MCU
#define EDMA_MSS_CC0_INTAGG_MASK_REG_ADDRESS    (CSL_MSS_CTRL_U_BASE + CSL_MSS_CTRL_MSS_TPCC_A_INTAGG_MASK)
#define EDMA_MSS_CC0_INTAGG_STATUS_REG_ADDRESS  (CSL_MSS_CTRL_U_BASE + CSL_MSS_CTRL_MSS_TPCC_A_INTAGG_STATUS)
#define EDMA_MSS_CC0_ERRAGG_MASK_REG_ADDRESS    (CSL_MSS_CTRL_U_BASE + CSL_MSS_CTRL_MSS_TPCC_A_ERRAGG_MASK)
#define EDMA_MSS_CC0_ERRAGG_STATUS_REG_ADDRESS  (CSL_MSS_CTRL_U_BASE + CSL_MSS_CTRL_MSS_TPCC_A_ERRAGG_STATUS)

#define EDMA_MSS_CC1_INTAGG_MASK_REG_ADDRESS    (CSL_MSS_CTRL_U_BASE + CSL_MSS_CTRL_MSS_TPCC_B_INTAGG_MASK)
#define EDMA_MSS_CC1_INTAGG_STATUS_REG_ADDRESS  (CSL_MSS_CTRL_U_BASE + CSL_MSS_CTRL_MSS_TPCC_B_INTAGG_STATUS)
#define EDMA_MSS_CC1_ERRAGG_MASK_REG_ADDRESS    (CSL_MSS_CTRL_U_BASE + CSL_MSS_CTRL_MSS_TPCC_B_ERRAGG_MASK)
#define EDMA_MSS_CC1_ERRAGG_STATUS_REG_ADDRESS  (CSL_MSS_CTRL_U_BASE + CSL_MSS_CTRL_MSS_TPCC_B_ERRAGG_STATUS)

#define EDMA_MSS_CC2_INTAGG_MASK_REG_ADDRESS    (CSL_DSS_CTRL_U_BASE + CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_MASK)
#define EDMA_MSS_CC2_INTAGG_STATUS_REG_ADDRESS  (CSL_DSS_CTRL_U_BASE + CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_STATUS)
#define EDMA_MSS_CC2_ERRAGG_MASK_REG_ADDRESS    (CSL_DSS_CTRL_U_BASE + CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_MASK)
#define EDMA_MSS_CC2_ERRAGG_STATUS_REG_ADDRESS  (CSL_DSS_CTRL_U_BASE + CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_STATUS)

#define EDMA_MSS_CC3_INTAGG_MASK_REG_ADDRESS    (CSL_DSS_CTRL_U_BASE + CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_MASK)
#define EDMA_MSS_CC3_INTAGG_STATUS_REG_ADDRESS  (CSL_DSS_CTRL_U_BASE + CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_STATUS)
#define EDMA_MSS_CC3_ERRAGG_MASK_REG_ADDRESS    (CSL_DSS_CTRL_U_BASE + CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_MASK)
#define EDMA_MSS_CC3_ERRAGG_STATUS_REG_ADDRESS  (CSL_DSS_CTRL_U_BASE + CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_STATUS)

#define EDMA_MSS_CC4_INTAGG_MASK_REG_ADDRESS    (CSL_DSS_CTRL_U_BASE + CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_MASK)
#define EDMA_MSS_CC4_INTAGG_STATUS_REG_ADDRESS  (CSL_DSS_CTRL_U_BASE + CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_STATUS)
#define EDMA_MSS_CC4_ERRAGG_MASK_REG_ADDRESS    (CSL_DSS_CTRL_U_BASE + CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_MASK)
#define EDMA_MSS_CC4_ERRAGG_STATUS_REG_ADDRESS  (CSL_DSS_CTRL_U_BASE + CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_STATUS)

#define EDMA_MSS_CC5_INTAGG_MASK_REG_ADDRESS    (CSL_RCSS_CTRL_U_BASE + CSL_RCSS_CTRL_RCSS_TPCC_A_INTAGG_MASK)
#define EDMA_MSS_CC5_INTAGG_STATUS_REG_ADDRESS  (CSL_RCSS_CTRL_U_BASE + CSL_RCSS_CTRL_RCSS_TPCC_A_INTAGG_STATUS)
#define EDMA_MSS_CC5_ERRAGG_MASK_REG_ADDRESS    (CSL_RCSS_CTRL_U_BASE + CSL_RCSS_CTRL_RCSS_TPCC_A_ERRAGG_MASK)
#define EDMA_MSS_CC5_ERRAGG_STATUS_REG_ADDRESS  (CSL_RCSS_CTRL_U_BASE + CSL_RCSS_CTRL_RCSS_TPCC_A_ERRAGG_STATUS)
#endif

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/* None. */

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

#ifdef BUILD_DSP_1
/*! @brief EDMA hardware attributes global. */
const EDMA_hwAttrs_t gEdmaHwAttrs[EDMA_NUM_CC] = {
    {
        .CCbaseAddress      = EDMA_DSS_CC0_BASE_ADDRESS,
        .CCcompletionInterruptsAggregatorMaskRegAddress   = EDMA_DSS_CC0_INTAGG_MASK_REG_ADDRESS,
        .CCcompletionInterruptsAggregatorStatusRegAddress = EDMA_DSS_CC0_INTAGG_STATUS_REG_ADDRESS,
        .CCerrorInterruptsAggregatorMaskRegAddress        = EDMA_DSS_CC0_ERRAGG_MASK_REG_ADDRESS,
        .CCerrorInterruptsAggregatorStatusRegAddress      = EDMA_DSS_CC0_ERRAGG_STATUS_REG_ADDRESS,
        .TCbaseAddress[0]   = EDMA_DSS_CC0_TC0_BASE_ADDRESS,
        .TCbaseAddress[1]   = EDMA_DSS_CC0_TC1_BASE_ADDRESS,
        .numEventQueues     = EDMA_DSS_NUM_TC_IN_CC0,
        .numParamSets       = EDMA_DSS_NUM_PARAM_SETS_CC0,
        .isChannelMapExist = false,
        .transferCompletionInterruptNum         = EDMA_DSS_CC0_TRANSFER_COMPLETE_INTR_ID,
        .errorInterruptNum                      = EDMA_DSS_CC0_AGGREGATED_ERRROR_INTR_ID,
        .transferControllerErrorInterruptNum[0] = EDMA_DSS_CC0_AGGREGATED_ERRROR_INTR_ID,
        .transferControllerErrorInterruptNum[1] = EDMA_DSS_CC0_AGGREGATED_ERRROR_INTR_ID
    },
    {
        .CCbaseAddress      = EDMA_DSS_CC1_BASE_ADDRESS,
        .CCcompletionInterruptsAggregatorMaskRegAddress   = EDMA_DSS_CC1_INTAGG_MASK_REG_ADDRESS,
        .CCcompletionInterruptsAggregatorStatusRegAddress = EDMA_DSS_CC1_INTAGG_STATUS_REG_ADDRESS,
        .CCerrorInterruptsAggregatorMaskRegAddress        = EDMA_DSS_CC1_ERRAGG_MASK_REG_ADDRESS,
        .CCerrorInterruptsAggregatorStatusRegAddress      = EDMA_DSS_CC1_ERRAGG_STATUS_REG_ADDRESS,
        .TCbaseAddress[0]   = EDMA_DSS_CC1_TC0_BASE_ADDRESS,
        .TCbaseAddress[1]   = EDMA_DSS_CC1_TC1_BASE_ADDRESS,
        .numEventQueues     = EDMA_DSS_NUM_TC_IN_CC1,
        .numParamSets       = EDMA_DSS_NUM_PARAM_SETS_CC1,
        .isChannelMapExist = false,
        .transferCompletionInterruptNum = EDMA_DSS_CC1_TRANSFER_COMPLETE_INTR_ID,
        .errorInterruptNum                      = EDMA_DSS_CC1_AGGREGATED_ERRROR_INTR_ID, //EDMA_INTERRUPT_NOT_CONNECTED_ID,
        .transferControllerErrorInterruptNum[0] = EDMA_DSS_CC1_AGGREGATED_ERRROR_INTR_ID, //EDMA_INTERRUPT_NOT_CONNECTED_ID,
        .transferControllerErrorInterruptNum[1] = EDMA_DSS_CC1_AGGREGATED_ERRROR_INTR_ID //EDMA_INTERRUPT_NOT_CONNECTED_ID,
    },
    {
        .CCbaseAddress      = EDMA_DSS_CC2_BASE_ADDRESS,
        .CCcompletionInterruptsAggregatorMaskRegAddress   = EDMA_DSS_CC2_INTAGG_MASK_REG_ADDRESS,
        .CCcompletionInterruptsAggregatorStatusRegAddress = EDMA_DSS_CC2_INTAGG_STATUS_REG_ADDRESS,
        .CCerrorInterruptsAggregatorMaskRegAddress        = EDMA_DSS_CC2_ERRAGG_MASK_REG_ADDRESS,
        .CCerrorInterruptsAggregatorStatusRegAddress      = EDMA_DSS_CC2_ERRAGG_STATUS_REG_ADDRESS,
        .TCbaseAddress[0]   = EDMA_DSS_CC2_TC0_BASE_ADDRESS,
        .TCbaseAddress[1]   = EDMA_DSS_CC2_TC1_BASE_ADDRESS,
        .numEventQueues     = EDMA_DSS_NUM_TC_IN_CC2,
        .numParamSets       = EDMA_DSS_NUM_PARAM_SETS_CC2,
        .isChannelMapExist = false,
        .transferCompletionInterruptNum = EDMA_DSS_CC2_TRANSFER_COMPLETE_INTR_ID,
        .errorInterruptNum                      = EDMA_DSS_CC2_AGGREGATED_ERRROR_INTR_ID, //EDMA_INTERRUPT_NOT_CONNECTED_ID,
        .transferControllerErrorInterruptNum[0] = EDMA_DSS_CC2_AGGREGATED_ERRROR_INTR_ID, //EDMA_INTERRUPT_NOT_CONNECTED_ID,
        .transferControllerErrorInterruptNum[1] = EDMA_DSS_CC2_AGGREGATED_ERRROR_INTR_ID  //EDMA_INTERRUPT_NOT_CONNECTED_ID
    },
    {
        .CCbaseAddress      = EDMA_DSS_CC3_BASE_ADDRESS,
        .CCcompletionInterruptsAggregatorMaskRegAddress   = EDMA_DSS_CC3_INTAGG_MASK_REG_ADDRESS,
        .CCcompletionInterruptsAggregatorStatusRegAddress = EDMA_DSS_CC3_INTAGG_STATUS_REG_ADDRESS,
        .CCerrorInterruptsAggregatorMaskRegAddress        = EDMA_DSS_CC3_ERRAGG_MASK_REG_ADDRESS,
        .CCerrorInterruptsAggregatorStatusRegAddress      = EDMA_DSS_CC3_ERRAGG_STATUS_REG_ADDRESS,
        .TCbaseAddress[0]   = EDMA_DSS_CC3_TC0_BASE_ADDRESS,
        .TCbaseAddress[1]   = EDMA_DSS_CC3_TC1_BASE_ADDRESS,
        .numEventQueues     = EDMA_DSS_NUM_TC_IN_CC3,
        .numParamSets       = EDMA_DSS_NUM_PARAM_SETS_CC3,
        .isChannelMapExist = false,
        .transferCompletionInterruptNum = EDMA_DSS_CC3_TRANSFER_COMPLETE_INTR_ID,
        .errorInterruptNum                      = EDMA_DSS_CC3_AGGREGATED_ERRROR_INTR_ID, //EDMA_INTERRUPT_NOT_CONNECTED_ID,
        .transferControllerErrorInterruptNum[0] = EDMA_DSS_CC3_AGGREGATED_ERRROR_INTR_ID, //EDMA_INTERRUPT_NOT_CONNECTED_ID,
        .transferControllerErrorInterruptNum[1] = EDMA_DSS_CC3_AGGREGATED_ERRROR_INTR_ID  //EDMA_INTERRUPT_NOT_CONNECTED_ID
    }
};
#endif

#ifdef BUILD_MCU
/*! @brief EDMA hardware attributes global. */
const EDMA_hwAttrs_t gEdmaHwAttrs[EDMA_NUM_CC] = {
    {
        .CCbaseAddress      = EDMA_MSS_CC0_BASE_ADDRESS,
        .CCcompletionInterruptsAggregatorMaskRegAddress   = EDMA_MSS_CC0_INTAGG_MASK_REG_ADDRESS,
        .CCcompletionInterruptsAggregatorStatusRegAddress = EDMA_MSS_CC0_INTAGG_STATUS_REG_ADDRESS,
        .CCerrorInterruptsAggregatorMaskRegAddress        = EDMA_MSS_CC0_ERRAGG_MASK_REG_ADDRESS,
        .CCerrorInterruptsAggregatorStatusRegAddress      = EDMA_MSS_CC0_ERRAGG_STATUS_REG_ADDRESS,
        .TCbaseAddress[0]   = EDMA_MSS_CC0_TC0_BASE_ADDRESS,
        .TCbaseAddress[1]   = EDMA_MSS_CC0_TC1_BASE_ADDRESS,
        .numEventQueues     = EDMA_MSS_NUM_TC_IN_CC0,
        .numParamSets       = EDMA_MSS_NUM_PARAM_SETS_CC0,
        .isChannelMapExist = false,
        .transferCompletionInterruptNum = EDMA_MSS_CC0_TRANSFER_COMPLETE_INTR_ID,
        //TODO_TPR12: enable when interrupts are assigned in future RTL version
        .errorInterruptNum                      = EDMA_INTERRUPT_NOT_CONNECTED_ID, //EDMA_MSS_CC0_AGGREGATED_ERRROR_INTR_ID,
        .transferControllerErrorInterruptNum[0] = EDMA_INTERRUPT_NOT_CONNECTED_ID, //EDMA_MSS_CC0_AGGREGATED_ERRROR_INTR_ID,
        .transferControllerErrorInterruptNum[1] = EDMA_INTERRUPT_NOT_CONNECTED_ID, //EDMA_MSS_CC0_AGGREGATED_ERRROR_INTR_ID
    },
    {
        .CCbaseAddress      = EDMA_MSS_CC1_BASE_ADDRESS,
        .CCcompletionInterruptsAggregatorMaskRegAddress   = EDMA_MSS_CC1_INTAGG_MASK_REG_ADDRESS,
        .CCcompletionInterruptsAggregatorStatusRegAddress = EDMA_MSS_CC1_INTAGG_STATUS_REG_ADDRESS,
        .CCerrorInterruptsAggregatorMaskRegAddress        = EDMA_MSS_CC1_ERRAGG_MASK_REG_ADDRESS,
        .CCerrorInterruptsAggregatorStatusRegAddress      = EDMA_MSS_CC1_ERRAGG_STATUS_REG_ADDRESS,
        .TCbaseAddress[0]   = EDMA_MSS_CC1_TC0_BASE_ADDRESS,
        .numEventQueues     = EDMA_MSS_NUM_TC_IN_CC1,
        .numParamSets       = EDMA_MSS_NUM_PARAM_SETS_CC1,
        .isChannelMapExist = false,
        .transferCompletionInterruptNum = EDMA_MSS_CC1_TRANSFER_COMPLETE_INTR_ID,
        //TODO_TPR12: enable when interrupts are assigned in future RTL version
        .errorInterruptNum                      = EDMA_INTERRUPT_NOT_CONNECTED_ID, //EDMA_MSS_CC1_AGGREGATED_ERRROR_INTR_ID,
        .transferControllerErrorInterruptNum[0] = EDMA_INTERRUPT_NOT_CONNECTED_ID, //EDMA_MSS_CC1_AGGREGATED_ERRROR_INTR_ID,
    },
    {
        .CCbaseAddress      = EDMA_MSS_CC2_BASE_ADDRESS,
        .CCcompletionInterruptsAggregatorMaskRegAddress   = EDMA_MSS_CC2_INTAGG_MASK_REG_ADDRESS,
        .CCcompletionInterruptsAggregatorStatusRegAddress = EDMA_MSS_CC2_INTAGG_STATUS_REG_ADDRESS,
        .CCerrorInterruptsAggregatorMaskRegAddress        = EDMA_MSS_CC2_ERRAGG_MASK_REG_ADDRESS,
        .CCerrorInterruptsAggregatorStatusRegAddress      = EDMA_MSS_CC2_ERRAGG_STATUS_REG_ADDRESS,
        .TCbaseAddress[0]   = EDMA_MSS_CC2_TC0_BASE_ADDRESS,
        .TCbaseAddress[1]   = EDMA_MSS_CC2_TC1_BASE_ADDRESS,
        .numEventQueues     = EDMA_MSS_NUM_TC_IN_CC2,
        .numParamSets       = EDMA_MSS_NUM_PARAM_SETS_CC2,
        .isChannelMapExist = false,
        .transferCompletionInterruptNum = EDMA_MSS_CC2_TRANSFER_COMPLETE_INTR_ID,
        .errorInterruptNum                      = EDMA_MSS_CC2_AGGREGATED_ERRROR_INTR_ID, //EDMA_INTERRUPT_NOT_CONNECTED_ID,
        .transferControllerErrorInterruptNum[0] = EDMA_MSS_CC2_AGGREGATED_ERRROR_INTR_ID, //EDMA_INTERRUPT_NOT_CONNECTED_ID,
        .transferControllerErrorInterruptNum[1] = EDMA_MSS_CC2_AGGREGATED_ERRROR_INTR_ID  //EDMA_INTERRUPT_NOT_CONNECTED_ID,
    },
    {
        .CCbaseAddress      = EDMA_MSS_CC3_BASE_ADDRESS,
        .CCcompletionInterruptsAggregatorMaskRegAddress   = EDMA_MSS_CC3_INTAGG_MASK_REG_ADDRESS,
        .CCcompletionInterruptsAggregatorStatusRegAddress = EDMA_MSS_CC3_INTAGG_STATUS_REG_ADDRESS,
        .CCerrorInterruptsAggregatorMaskRegAddress        = EDMA_MSS_CC3_ERRAGG_MASK_REG_ADDRESS,
        .CCerrorInterruptsAggregatorStatusRegAddress      = EDMA_MSS_CC3_ERRAGG_STATUS_REG_ADDRESS,
        .TCbaseAddress[0]   = EDMA_MSS_CC3_TC0_BASE_ADDRESS,
        .TCbaseAddress[1]   = EDMA_MSS_CC3_TC1_BASE_ADDRESS,
        .numEventQueues     = EDMA_MSS_NUM_TC_IN_CC3,
        .numParamSets       = EDMA_MSS_NUM_PARAM_SETS_CC3,
        .isChannelMapExist = false,
        .transferCompletionInterruptNum = EDMA_MSS_CC3_TRANSFER_COMPLETE_INTR_ID,
        .errorInterruptNum                      = EDMA_MSS_CC3_AGGREGATED_ERRROR_INTR_ID, //EDMA_INTERRUPT_NOT_CONNECTED_ID,
        .transferControllerErrorInterruptNum[0] = EDMA_MSS_CC3_AGGREGATED_ERRROR_INTR_ID, //EDMA_INTERRUPT_NOT_CONNECTED_ID,
        .transferControllerErrorInterruptNum[1] = EDMA_MSS_CC3_AGGREGATED_ERRROR_INTR_ID  //EDMA_INTERRUPT_NOT_CONNECTED_ID,
    },
    {
        .CCbaseAddress      = EDMA_MSS_CC4_BASE_ADDRESS,
        .CCcompletionInterruptsAggregatorMaskRegAddress   = EDMA_MSS_CC4_INTAGG_MASK_REG_ADDRESS,
        .CCcompletionInterruptsAggregatorStatusRegAddress = EDMA_MSS_CC4_INTAGG_STATUS_REG_ADDRESS,
        .CCerrorInterruptsAggregatorMaskRegAddress        = EDMA_MSS_CC4_ERRAGG_MASK_REG_ADDRESS,
        .CCerrorInterruptsAggregatorStatusRegAddress      = EDMA_MSS_CC4_ERRAGG_STATUS_REG_ADDRESS,
        .TCbaseAddress[0]   = EDMA_MSS_CC4_TC0_BASE_ADDRESS,
        .TCbaseAddress[1]   = EDMA_MSS_CC4_TC1_BASE_ADDRESS,
        .numEventQueues     = EDMA_MSS_NUM_TC_IN_CC4,
        .numParamSets       = EDMA_MSS_NUM_PARAM_SETS_CC4,
        .isChannelMapExist = false,
        .transferCompletionInterruptNum = EDMA_MSS_CC4_TRANSFER_COMPLETE_INTR_ID,
        .errorInterruptNum                      = EDMA_MSS_CC4_AGGREGATED_ERRROR_INTR_ID, //EDMA_INTERRUPT_NOT_CONNECTED_ID,
        .transferControllerErrorInterruptNum[0] = EDMA_MSS_CC4_AGGREGATED_ERRROR_INTR_ID, //EDMA_INTERRUPT_NOT_CONNECTED_ID,
        .transferControllerErrorInterruptNum[1] = EDMA_MSS_CC4_AGGREGATED_ERRROR_INTR_ID  //EDMA_INTERRUPT_NOT_CONNECTED_ID,
    },
    {
        .CCbaseAddress      = EDMA_MSS_CC5_BASE_ADDRESS,
        .CCcompletionInterruptsAggregatorMaskRegAddress   = EDMA_MSS_CC5_INTAGG_MASK_REG_ADDRESS,
        .CCcompletionInterruptsAggregatorStatusRegAddress = EDMA_MSS_CC5_INTAGG_STATUS_REG_ADDRESS,
        .CCerrorInterruptsAggregatorMaskRegAddress        = EDMA_MSS_CC5_ERRAGG_MASK_REG_ADDRESS,
        .CCerrorInterruptsAggregatorStatusRegAddress      = EDMA_MSS_CC5_ERRAGG_STATUS_REG_ADDRESS,
        .TCbaseAddress[0]   = EDMA_MSS_CC5_TC0_BASE_ADDRESS,
        .TCbaseAddress[1]   = EDMA_MSS_CC5_TC1_BASE_ADDRESS,
        .numEventQueues     = EDMA_MSS_NUM_TC_IN_CC5,
        .numParamSets       = EDMA_MSS_NUM_PARAM_SETS_CC5,
        .isChannelMapExist = false,
        .transferCompletionInterruptNum = EDMA_MSS_CC5_TRANSFER_COMPLETE_INTR_ID,
        .errorInterruptNum                      = EDMA_MSS_CC5_AGGREGATED_ERRROR_INTR_ID, //EDMA_INTERRUPT_NOT_CONNECTED_ID,
        .transferControllerErrorInterruptNum[0] = EDMA_MSS_CC5_AGGREGATED_ERRROR_INTR_ID, //EDMA_INTERRUPT_NOT_CONNECTED_ID,
        .transferControllerErrorInterruptNum[1] = EDMA_MSS_CC5_AGGREGATED_ERRROR_INTR_ID  //EDMA_INTERRUPT_NOT_CONNECTED_ID
    }
};

#endif

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

/* None. */
