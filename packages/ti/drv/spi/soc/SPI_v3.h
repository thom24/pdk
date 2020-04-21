/*
 * Copyright (c) 2020, Texas Instruments Incorporated
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
/** ============================================================================
 *  @file       SPI_v3.h
 *
 *  @brief      SPI driver implementation for a TPR12 MIBSPI controller.
 *
 *  The SPI header file should be included in an application as follows:
 *  @code
 *  #include <ti/drv/spi/SPI.h>
 *  #include <ti/drv/spi/soc/SPI_v3.h>
 *  @endcode
 *
 *  This SPI driver implementation is designed to operate on a TPR12 SPI
 *
 *  ## SPI data frames #
 *
 *  SPI data frames can be any size from 2-bits to 16-bits. If the dataSize in
 *  ::SPI_Params is greater that 8-bits, then the SPI_v1 driver
 *  implementation will assume that the ::SPI_Transaction txBuf and rxBuf
 *  point to an array of 16-bit uint16_t elements.
 *
 *  dataSize  | buffer element size |
 *  --------  | ------------------- |
 *  2-8 bits  | uint8_t             |
 *  9-16 bits | uint16_t            |
 *
 *  ============================================================================
 */

#ifndef SPI_V3_H
#define SPI_V3_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <ti/csl/src/ip/mcspi/V1/spi.h>
#include <ti/drv/spi/SPI.h>


/*!
 *  @brief    Maximum element in MibSPI RAM. 
 *            For 8bit data size, the maximum transfer size is 64 bytes
 *            For 16bit data size, the maximum transfer size is 128 bytes
 */
#define MIBSPI_RAM_MAX_ELEM          64U

/**
 * @brief
 *  Max number of slaves supported when MibSPI is configured as master
 */
#define MIBSPI_SLAVE_MAX             3U

/**
 * @brief
 *  Max number of DMA REQ lines supported by MIBSPI
 */
#define MIBSPI_DMA_REQLINE_MAX       3U


/**
 * @brief
 *  Maximum CS supported for the device
 */
#define MIBSPI_MAX_CS                       MIBSPI_SLAVE_MAX

/**
 * @brief
 *  CS configuration value when none of the CS is activated
 */
#define MIBSPI_CS_NONE                      0xFFU

/**
 * @brief
 *  Max number of transport group
 */
#define MIBSPI_NUM_TRANS_GROUP              8U

/**
 * @brief
 *  Max number of DMA group
 */
#define MIBSPI_NUM_DMA_GROUP                8U


/**
 *  @defgroup MIBSPI_FEATURE defines
 *  MIBSPI_FEATURE_* macros indicate presence of a particular features in 
 *  SPI instance on the given SoC.
 *  @{
 */
/**
 * @brief MIBSPI Parallel mode operation supported
 */
#define MIBSPI_FEATURE_PARALLEL_MODE       (0x1U << 0)

/**
 * @brief MIBSPI Modulo mode operation supported
 */
#define MIBSPI_FEATURE_MODULO_MODE         (0x1U << 1)

/**
 * @brief MIBSPI SPIENA pin supported
 */
#define MIBSPI_FEATURE_SPIENA_PIN          (0x1U << 2)

/** @} */

/**
 * @brief
 *  SPI Driver DMA request line configuration
 *
 * @details
 *  The structure is used to store the hardware specific configuration for DMA request lines.
 *
 *  \ingroup SPI_DRIVER_INTERNAL_DATA_STRUCTURE
 *
 */
typedef struct MibSpi_DMAReqlineCfg_t
{
    /**
     * @brief   TX DMA Request Line number
     */
    uint8_t     txDmaReqLine;

    /**
     * @brief   RX DMA Request Line number
     */
    uint8_t     rxDmaReqLine;
}MibSpi_DMAReqlineCfg;

/**
 * @brief
 *  SPI Peripheral version info
 *
 * @details
 *  The structure is used to store the hardware version info expected in the 
 *  SPIREV register
 *
 *  \ingroup SPI_DRIVER_INTERNAL_DATA_STRUCTURE
 *
 */
typedef struct MibSpi_VersionInfo_t
{
    uint32_t scheme;
    uint32_t functionality;
    uint32_t rtl;
    uint32_t major;
    uint32_t custom;
    uint32_t minor;
} MibSpi_VersionInfo;


/**
 * @brief
 *  SPI Driver HW configuration
 *
 * @details
 *  The structure is used to store the hardware specific configuration which is
 *  passed to SPI driver instance
 *
 *  \ingroup SPI_DRIVER_INTERNAL_DATA_STRUCTURE
 *
 */
typedef struct MibSpi_HwCfg_t
{
    /**
     * @brief   Unique id used to identify the MIBSPI instance
     */
    enum MibSpi_InstanceId mibspiInstId;

    /**
     * @brief   Base address of the MibSpi register address space to be used.
     */
    CSL_mss_spiRegs *      ptrSpiRegBase;

    /**
     * @brief   Base address of the MibSpi ram address space to be used.
     */
    CSL_mibspiRam  *       ptrMibSpiRam;

    /**
     * @brief   SPI clock source frequency in Hz
     *             It will be used to calculate prescaler for Master mode
     */
    uint32_t                clockSrcFreq;

    /**
     * @brief   Interrupt Number for INT0
     */
    uint32_t                interrupt0Num;

    /**
     * @brief   Interrupt Number for INT1
     */
    uint32_t                interrupt1Num;

    /**
     * @brief   EDMA Channel Controller Id
     */
    uint32_t                edmaCCId;

    /**
     * @brief Size of MIBSPI RAM in this SoC for this SPI instance
     */
    uint32_t                mibspiRamSize;

    /**
     * @brief Number of transfer groups for this SPI instance
     */
    uint32_t                numTransferGroups;

    /**
     * @brief Number of chip select pins supported for this SPI instance
     */
    uint32_t                numCsPins;

    /**
     * @brief Number of parallel mode pins supported for this SPI instance
     *
     * If MIBSPI_FEATURE_PARALLEL_MODE is not enabled for this SPI instance
     * this value is dont care and will be set to 0
     */
    uint32_t                numParallelModePins;
    /**
     * @brief Optional feature that are supported for this SPI instance
     *        This is a bitmap comprising of defines @ref MIBSPI_FEATURE
     *        Bit being set indicates presence of the features for the SPI
     *        instance
     */
    uint32_t                featureBitMap;

    /**
     * @brief Number of independent DMA request lines supported by MIBSPI
     */
    uint32_t                numDmaReqLines;
    /**
     * @brief   SPI DMA reqline definition
     */
    MibSpi_DMAReqlineCfg    dmaReqlineCfg[MIBSPI_DMA_REQLINE_MAX];
    /**
     * @brief   SPIREV info 
     */
    MibSpi_VersionInfo      versionInfo;
} MibSpi_HwCfg;

typedef MibSpi_HwCfg SPI_v3_HWAttrs;

typedef struct MibSpi_dmaXferAddrInfo_s
{
    uintptr_t saddr;
    uintptr_t daddr;
} MibSpi_dmaXferAddrInfo_t;

typedef struct MibSpi_dmaXferSizeInfo_s
{
    uint16_t elemSize;
    uint16_t elemCnt;
    uint16_t frameCnt;
} MibSpi_dmaXferSizeInfo_t;

typedef struct MibSpi_dmaXferInfo_s
{
    MibSpi_dmaXferAddrInfo_t tx;
    MibSpi_dmaXferAddrInfo_t rx;
    MibSpi_dmaXferSizeInfo_t size;
    uint32_t  dmaReqLine;
    bool     isMibspiRamXfer;
} MibSpi_dmaXferInfo_t;



int32_t MIBSPI_dmaConfig(SPI_Handle handle);
int32_t MIBSPI_dmaTransfer(SPI_Handle handle, MibSpi_dmaXferInfo_t *xferInfo);
int32_t MIBSPI_dmaFreeChannel(const SPI_Handle handle);
void MIBSPI_dmaDoneCb(SPI_Handle spiHandle);

/**
 * @brief   This is the SPI Driver registered function table
 */
extern const SPI_FxnTable SPI_FxnTable_v3;

#ifdef __cplusplus
}
#endif

#endif /* _SPI_V3_H */
