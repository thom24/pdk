/*
 *  Copyright (c) Texas Instruments Incorporated 2018
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
 *  \file udma_soc.h
 *
 *  \brief UDMA Low Level Driver AM64x SOC specific file.
 */

#ifndef UDMA_SOC_H_
#define UDMA_SOC_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

/* None */

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/**
 *  \anchor Udma_InstanceId
 *  \name UDMA Instance ID
 *
 *  UDMA instance ID - BCDMA/PKTDMA
 *
 *  @{
 */
/** \brief BCDMA instance */
#define UDMA_INST_ID_BCDMA_0             (0U)
/** \brief PKTDMA instance */
#define UDMA_INST_ID_PKTDMA_0            (1U)
/** \brief Maximum number of UDMA instance */
#define UDMA_INST_ID_MAX                 (2U)
/* @} */
 
/**
 *  \anchor Udma_SocCfg
 *  \name UDMA SOC Configuration
 *
 *  UDMA Soc Cfg - Flags to indicate the presnce of various SOC specific modules.
 *
 *  @{
 */
/** \brief Flag to indicate UDMAP module is present or not in the SOC*/
#define UDMA_SOC_CFG_UDMAP_PRESENT               (0U)
    
/** \brief Flag to indicate LCDMA module is present or not in the SOC*/
#define UDMA_SOC_CFG_LCDMA_PRESENT               (1U) 
    
/** \brief Flag to indicate Proxy is present or not in the SOC*/
#define UDMA_SOC_CFG_PROXY_PRESENT               (0U) 

/** \brief Flag to indicate Clec is present or not in the SOC*/
#define UDMA_SOC_CFG_CLEC_PRESENT                (0U) 

/** \brief Flag to indicate Normal RA is present or not in the SOC*/
#define UDMA_SOC_CFG_RA_NORMAL_PRESENT           (0U) 

/** \brief Flag to indicate LCDMA RA is present or not in the SOC*/
#define UDMA_SOC_CFG_RA_LCDMA_PRESENT            (1U)

/** \brief Flag to indicate Ring Monitor is present or not in the SOC*/
#define UDMA_SOC_CFG_RING_MON_PRESENT            (0U)

/** \brief Flag to indicate the SOC needs ring reset workaround */
#define UDMA_SOC_CFG_APPLY_RING_WORKAROUND       (0U)
/* @} */

 /**
 *  \anchor Udma_TxChFdepth
 *  \name UDMA Tx Channels FDEPTH
 *
 *  UDMA Tx Ch Fdepth - Fdepth of various types of channels present in the SOC.
 *
 *  @{
 */
/** \brief Tx Ultra High Capacity Channel FDEPTH*/
#define UDMA_TX_UHC_CHANS_FDEPTH         (CSL_DMSS_UDMAP_TX_UHC_CHANS_FDEPTH)
/** \brief Tx High Capacity Channel FDEPTH*/
#define UDMA_TX_HC_CHANS_FDEPTH          (CSL_DMSS_UDMAP_TX_HC_CHANS_FDEPTH)
/** \brief Tx Normal Channel FDEPTH*/
#define UDMA_TX_CHANS_FDEPTH             (CSL_DMSS_UDMAP_TX_CHANS_FDEPTH)
/* @} */

/** \brief Number of UTC instance - No UTC in AM64x */
#define UDMA_NUM_UTC_INSTANCE           (CSL_DMSS_UTC_CNT)

/**
 *  \anchor Udma_CoreId
 *  \name Core ID specific to a SOC
 *
 *  List of all cores present in the SOC.
 *
 *  @{
 */
/*
 * Locally used core ID to define default RM configuration.
 * Not to be used by caller
 */
#define UDMA_CORE_ID_MPU1_0             (0U)
#define UDMA_CORE_ID_MCU2_0             (1U)
#define UDMA_CORE_ID_MCU2_1             (2U)
#define UDMA_CORE_ID_MCU1_0             (3U)
#define UDMA_CORE_ID_MCU1_1             (4U)
#define UDMA_CORE_ID_M4F_0              (5U)
/* Total number of cores */
#define UDMA_NUM_CORE                   (6U)
/* @} */

/**
 *  \anchor Udma_PsilCh
 *  \name PSIL Channels
 *
 *  List of all PSIL channels and the corresponding counts
 *
 *  @{
 */

#define UDMA_PSIL_CH_CPSW2_TX           (CSL_PSILCFG_DMSS_CPSW2_PSILD_THREAD_OFFSET)
#define UDMA_PSIL_CH_SAUL0_TX           (CSL_PSILCFG_DMSS_SAUL0_PSILD_THREAD_OFFSET)

#define UDMA_PSIL_CH_CPSW2_RX           (CSL_PSILCFG_DMSS_CPSW2_PSILS_THREAD_OFFSET)
#define UDMA_PSIL_CH_SAUL0_RX           (CSL_PSILCFG_DMSS_SAUL0_PSILS_THREAD_OFFSET)

#define UDMA_PSIL_CH_CPSW2_TX_CNT       (CSL_PSILCFG_DMSS_CPSW2_PSILD_THREAD_CNT)
#define UDMA_PSIL_CH_SAUL0_TX_CNT       (CSL_PSILCFG_DMSS_SAUL0_PSILD_THREAD_CNT)

#define UDMA_PSIL_CH_CPSW2_RX_CNT       (CSL_PSILCFG_DMSS_CPSW2_PSILS_THREAD_CNT)
#define UDMA_PSIL_CH_SAUL0_RX_CNT       (CSL_PSILCFG_DMSS_SAUL0_PSILS_THREAD_CNT)

/* @} */


/**
 *  \anchor Udma_PdmaCh
 *  \name PDMA Channels
 *
 *  List of all PDMA channels across dmss domain
 *
 *  @{
 */

/**
 *  \anchor Udma_PdmaChMain0Tx
 *  \name Main0 TX PDMA Channels
 *
 *  List of all Main0 PDMA TX channels
 *
 *  @{
 */

/*
 * PDMA MAIN0 MCSPI TX Channels
 */
#define UDMA_PDMA_CH_MAIN0_MCSPI0_CH0_TX    (CSL_PDMA_CH_MAIN0_MCSPI0_CH0_TX)
#define UDMA_PDMA_CH_MAIN0_MCSPI0_CH1_TX    (CSL_PDMA_CH_MAIN0_MCSPI0_CH1_TX)
#define UDMA_PDMA_CH_MAIN0_MCSPI0_CH2_TX    (CSL_PDMA_CH_MAIN0_MCSPI0_CH2_TX)
#define UDMA_PDMA_CH_MAIN0_MCSPI0_CH3_TX    (CSL_PDMA_CH_MAIN0_MCSPI0_CH3_TX)
#define UDMA_PDMA_CH_MAIN0_MCSPI1_CH0_TX    (CSL_PDMA_CH_MAIN0_MCSPI1_CH0_TX)
#define UDMA_PDMA_CH_MAIN0_MCSPI1_CH1_TX    (CSL_PDMA_CH_MAIN0_MCSPI1_CH1_TX)
#define UDMA_PDMA_CH_MAIN0_MCSPI1_CH2_TX    (CSL_PDMA_CH_MAIN0_MCSPI1_CH2_TX)
#define UDMA_PDMA_CH_MAIN0_MCSPI1_CH3_TX    (CSL_PDMA_CH_MAIN0_MCSPI1_CH3_TX)
#define UDMA_PDMA_CH_MAIN0_MCSPI2_CH0_TX    (CSL_PDMA_CH_MAIN0_MCSPI2_CH0_TX)
#define UDMA_PDMA_CH_MAIN0_MCSPI2_CH1_TX    (CSL_PDMA_CH_MAIN0_MCSPI2_CH1_TX)
#define UDMA_PDMA_CH_MAIN0_MCSPI2_CH2_TX    (CSL_PDMA_CH_MAIN0_MCSPI2_CH2_TX)
#define UDMA_PDMA_CH_MAIN0_MCSPI2_CH3_TX    (CSL_PDMA_CH_MAIN0_MCSPI2_CH3_TX)
#define UDMA_PDMA_CH_MAIN0_MCSPI3_CH0_TX    (CSL_PDMA_CH_MAIN0_MCSPI3_CH0_TX)
#define UDMA_PDMA_CH_MAIN0_MCSPI3_CH1_TX    (CSL_PDMA_CH_MAIN0_MCSPI3_CH1_TX)
#define UDMA_PDMA_CH_MAIN0_MCSPI3_CH2_TX    (CSL_PDMA_CH_MAIN0_MCSPI3_CH2_TX)
#define UDMA_PDMA_CH_MAIN0_MCSPI3_CH3_TX    (CSL_PDMA_CH_MAIN0_MCSPI3_CH3_TX)
/*
 * PDMA MAIN0 UART TX Channels
 */
#define UDMA_PDMA_CH_MAIN0_UART0_TX       (CSL_PDMA_CH_MAIN0_UART0_CH0_TX)
#define UDMA_PDMA_CH_MAIN0_UART1_TX       (CSL_PDMA_CH_MAIN0_UART1_CH0_TX)

/* @} */

/**
 *  \anchor Udma_PdmaChMain0Rx
 *  \name Main0 RX PDMA Channels
 *
 *  List of all Main0 PDMA RX channels
 *
 *  @{
 */

/*
 * PDMA MAIN0 MCSPI RX Channels
 */
#define UDMA_PDMA_CH_MAIN0_MCSPI0_CH0_RX    (CSL_PDMA_CH_MAIN0_MCSPI0_CH0_RX)
#define UDMA_PDMA_CH_MAIN0_MCSPI0_CH1_RX    (CSL_PDMA_CH_MAIN0_MCSPI0_CH1_RX)
#define UDMA_PDMA_CH_MAIN0_MCSPI0_CH2_RX    (CSL_PDMA_CH_MAIN0_MCSPI0_CH2_RX)
#define UDMA_PDMA_CH_MAIN0_MCSPI0_CH3_RX    (CSL_PDMA_CH_MAIN0_MCSPI0_CH3_RX)
#define UDMA_PDMA_CH_MAIN0_MCSPI1_CH0_RX    (CSL_PDMA_CH_MAIN0_MCSPI1_CH0_RX)
#define UDMA_PDMA_CH_MAIN0_MCSPI1_CH1_RX    (CSL_PDMA_CH_MAIN0_MCSPI1_CH1_RX)
#define UDMA_PDMA_CH_MAIN0_MCSPI1_CH2_RX    (CSL_PDMA_CH_MAIN0_MCSPI1_CH2_RX)
#define UDMA_PDMA_CH_MAIN0_MCSPI1_CH3_RX    (CSL_PDMA_CH_MAIN0_MCSPI1_CH3_RX)
#define UDMA_PDMA_CH_MAIN0_MCSPI2_CH0_RX    (CSL_PDMA_CH_MAIN0_MCSPI2_CH0_RX)
#define UDMA_PDMA_CH_MAIN0_MCSPI2_CH1_RX    (CSL_PDMA_CH_MAIN0_MCSPI2_CH1_RX)
#define UDMA_PDMA_CH_MAIN0_MCSPI2_CH2_RX    (CSL_PDMA_CH_MAIN0_MCSPI2_CH2_RX)
#define UDMA_PDMA_CH_MAIN0_MCSPI2_CH3_RX    (CSL_PDMA_CH_MAIN0_MCSPI2_CH3_RX)
#define UDMA_PDMA_CH_MAIN0_MCSPI3_CH0_RX    (CSL_PDMA_CH_MAIN0_MCSPI3_CH0_RX)
#define UDMA_PDMA_CH_MAIN0_MCSPI3_CH1_RX    (CSL_PDMA_CH_MAIN0_MCSPI3_CH1_RX)
#define UDMA_PDMA_CH_MAIN0_MCSPI3_CH2_RX    (CSL_PDMA_CH_MAIN0_MCSPI3_CH2_RX)
#define UDMA_PDMA_CH_MAIN0_MCSPI3_CH3_RX    (CSL_PDMA_CH_MAIN0_MCSPI3_CH3_RX)
/*
 * PDMA MAIN0 UART RX Channels
 */
#define UDMA_PDMA_CH_MAIN0_UART0_RX       (CSL_PDMA_CH_MAIN0_UART0_CH0_RX)
#define UDMA_PDMA_CH_MAIN0_UART1_RX       (CSL_PDMA_CH_MAIN0_UART1_CH0_RX)

/* @} */

/**
 *  \anchor Udma_PdmaChMain1Tx
 *  \name Main1 TX PDMA Channels
 *
 *  List of all Main1 PDMA TX channels
 *
 *  @{
 */

/*
 * PDMA MAIN1 MCSPI TX Channels
 */
#define UDMA_PDMA_CH_MAIN1_MCSPI4_CH0_TX    (CSL_PDMA_CH_MAIN1_MCSPI4_CH0_TX)
#define UDMA_PDMA_CH_MAIN1_MCSPI4_CH1_TX    (CSL_PDMA_CH_MAIN1_MCSPI4_CH1_TX)
#define UDMA_PDMA_CH_MAIN1_MCSPI4_CH2_TX    (CSL_PDMA_CH_MAIN1_MCSPI4_CH2_TX)
#define UDMA_PDMA_CH_MAIN1_MCSPI4_CH3_TX    (CSL_PDMA_CH_MAIN1_MCSPI4_CH3_TX)
/*
 * PDMA MAIN1 UART TX Channels
 */
#define UDMA_PDMA_CH_MAIN1_UART2_TX (CSL_PDMA_CH_MAIN1_UART2_CH0_TX)
#define UDMA_PDMA_CH_MAIN1_UART3_TX (CSL_PDMA_CH_MAIN1_UART3_CH0_TX)
#define UDMA_PDMA_CH_MAIN1_UART4_TX (CSL_PDMA_CH_MAIN1_UART4_CH0_TX)
#define UDMA_PDMA_CH_MAIN1_UART5_TX (CSL_PDMA_CH_MAIN1_UART5_CH0_TX)
#define UDMA_PDMA_CH_MAIN1_UART6_TX (CSL_PDMA_CH_MAIN1_UART6_CH0_TX)
/*
 * PDMA MAIN1 MCAN TX Channels
 */
#define UDMA_PDMA_CH_MAIN1_MCAN0_CH0_TX (CSL_PDMA_CH_MAIN1_MCAN0_CH0_TX)
#define UDMA_PDMA_CH_MAIN1_MCAN0_CH1_TX (CSL_PDMA_CH_MAIN1_MCAN0_CH1_TX)
#define UDMA_PDMA_CH_MAIN1_MCAN0_CH2_TX (CSL_PDMA_CH_MAIN1_MCAN0_CH2_TX)
#define UDMA_PDMA_CH_MAIN1_MCAN1_CH0_TX (CSL_PDMA_CH_MAIN1_MCAN1_CH0_TX)
#define UDMA_PDMA_CH_MAIN1_MCAN1_CH1_TX (CSL_PDMA_CH_MAIN1_MCAN1_CH1_TX)
#define UDMA_PDMA_CH_MAIN1_MCAN1_CH2_TX (CSL_PDMA_CH_MAIN1_MCAN1_CH2_TX)

/* @} */

/**
 *  \anchor Udma_PdmaChMain1Rx
 *  \name Main1 RX PDMA Channels
 *
 *  List of all Main1 PDMA RX channels
 *
 *  @{
 */

/*
 * PDMA MAIN1 MCSPI RX Channels
 */
#define UDMA_PDMA_CH_MAIN1_MCSPI4_CH0_RX    (CSL_PDMA_CH_MAIN1_MCSPI4_CH0_RX)
#define UDMA_PDMA_CH_MAIN1_MCSPI4_CH1_RX    (CSL_PDMA_CH_MAIN1_MCSPI4_CH1_RX)
#define UDMA_PDMA_CH_MAIN1_MCSPI4_CH2_RX    (CSL_PDMA_CH_MAIN1_MCSPI4_CH2_RX)
#define UDMA_PDMA_CH_MAIN1_MCSPI4_CH3_RX    (CSL_PDMA_CH_MAIN1_MCSPI4_CH3_RX)
/*
 * PDMA MAIN1 UART RX Channels
 */
#define UDMA_PDMA_CH_MAIN1_UART2_RX (CSL_PDMA_CH_MAIN1_UART2_CH0_RX)
#define UDMA_PDMA_CH_MAIN1_UART3_RX (CSL_PDMA_CH_MAIN1_UART3_CH0_RX)
#define UDMA_PDMA_CH_MAIN1_UART4_RX (CSL_PDMA_CH_MAIN1_UART4_CH0_RX)
#define UDMA_PDMA_CH_MAIN1_UART5_RX (CSL_PDMA_CH_MAIN1_UART5_CH0_RX)
#define UDMA_PDMA_CH_MAIN1_UART6_RX (CSL_PDMA_CH_MAIN1_UART6_CH0_RX)
/*
 * PDMA MAIN1 MCAN RX Channels
 */
#define UDMA_PDMA_CH_MAIN1_MCAN0_CH0_RX (CSL_PDMA_CH_MAIN1_MCAN0_CH0_RX)
#define UDMA_PDMA_CH_MAIN1_MCAN0_CH1_RX (CSL_PDMA_CH_MAIN1_MCAN0_CH1_RX)
#define UDMA_PDMA_CH_MAIN1_MCAN0_CH2_RX (CSL_PDMA_CH_MAIN1_MCAN0_CH2_RX)
#define UDMA_PDMA_CH_MAIN1_MCAN1_CH0_RX (CSL_PDMA_CH_MAIN1_MCAN1_CH0_RX)
#define UDMA_PDMA_CH_MAIN1_MCAN1_CH1_RX (CSL_PDMA_CH_MAIN1_MCAN1_CH1_RX)
#define UDMA_PDMA_CH_MAIN1_MCAN1_CH2_RX (CSL_PDMA_CH_MAIN1_MCAN1_CH2_RX)
/*
 * PDMA MAIN1 ADC RX Channels
 */
#define UDMA_PDMA_CH_MAIN1_ADC0_CH0_RX  (CSL_PDMA_CH_MAIN1_ADC0_CH0_RX)
#define UDMA_PDMA_CH_MAIN1_ADC0_CH1_RX  (CSL_PDMA_CH_MAIN1_ADC0_CH1_RX)

/* @} */

/* @} */

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/**
 *  \brief Returns the core ID
 *
 *  \return Core ID \ref Udma_CoreId
 */
uint32_t Udma_getCoreId(void);

/**
 *  \brief Returns TRUE if the memory is cache coherent
 *
 *  \return TRUE/FALSE
 */
uint32_t Udma_isCacheCoherent(void);

/* ========================================================================== */
/*                       Static Function Definitions                          */
/* ========================================================================== */

/* None */

#ifdef __cplusplus
}
#endif

#endif /* #ifndef UDMA_SOC_H_ */
