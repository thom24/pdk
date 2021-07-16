/*  ============================================================================
 *   Copyright (c) Texas Instruments Incorporated 2021
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


/**
<<<<<<< HEAD
<<<<<<< HEAD
 *   @file  pcie_ssd.c
 *
 *   @brief
 *      This is the PCIe ssd example code.
=======
 *   @file  pcie_sample.c
 *
 *   @brief
 *      This is the PCIe example code.
>>>>>>> cc7d9980b (PDK-9352: Added PCIe SSD examples for AM65x)
=======
 *   @file  pcie_ssd.c
 *
 *   @brief
 *      This is the PCIe ssd example code.
>>>>>>> 3fda8cd5c (Updating as per the review comments)
 *
 */

/**
<<<<<<< HEAD
<<<<<<< HEAD
 * In the PCIe SSD example one IDK is used to test the PCIe SSD driver.
 * As described in the following figure, IDK RC is configured as a Root Complex
 * and SSD EP is configured as End Point.
=======
 * In the PCIe sample example two EVMs are used to test the PCIe driver.
 * As described in the following figure, EVM RC is configured as a Root Complex
 * and EVM EP is configured as End Point.
>>>>>>> cc7d9980b (PDK-9352: Added PCIe SSD examples for AM65x)
=======
 * In the PCIe SSD example one IDK is used to test the PCIe SSD driver.
 * As described in the following figure, IDK RC is configured as a Root Complex
 * and SSD EP is configured as End Point.
>>>>>>> 57bdc7c55 (PDK-9352: PCIE SSD support for mpu core on am65xx)
 *
 *         EVM RC                                          EVM EP
 *   ------------------                             -------------------
 *   |                |                             |                 |
 *   |   Root         |          PCIe Link          |  End Point      |
 *   |   Complex      | <-------------------------->|                 |
 *   |                |                             |                 |
 *   ------------------                             -------------------
 *
 * Once the PCIe link is established, the following sequence of actions will happen:
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> 57bdc7c55 (PDK-9352: PCIE SSD support for mpu core on am65xx)
 *  - IDK RC sends data to SSD EP
 *  - SSD EP waits to receive all the data
 *  - IDK sends command for File IO operations
 *  - SSD performs the corresponding operation and sends the response back to IDK
<<<<<<< HEAD
 *
 */

#include <string.h>
#include <stdio.h>

#include <ti/drv/pcie/example/ssd/src/pcie_ssd.h>
#include <ti/drv/pcie/example/ssd/src/nvme.h>

#include <ti/drv/pcie/soc/pcie_soc.h>

#include <ti/drv/i2c/I2C.h>
#include <ti/drv/i2c/soc/I2C_soc.h>
#include <ti/csl/csl_gpio.h>
#include <ti/board/board.h>
#include <ti/board/src/am65xx_idk/include/board_cfg.h>
#include <ti/board/src/evmKeystone3/include/board_i2c_io_exp.h>

#include <ti/osal/osal.h>
<<<<<<< HEAD
#include <ti/osal/TaskP.h>

#if defined(USE_BIOS)
#ifdef __ARM_ARCH_7A__
#include <ti/sysbios/family/arm/a15/Cache.h>
#include <ti/sysbios/family/arm/a15/Mmu.h>
#endif
#endif

=======
 *  - EVM RC sends data to EVM EP
 *  - EVM EP waits to receive all the data
 *  - EVM EP sends the data back to EVM RC
 *  - EVM RC waits to receive all the data
 *  - EVM RC verifies if the received data matches the sent data and declares test pass or fail.
 *  - EVM EP sends 10 MSI and 10 INTA's to EVM RC (on certain device and core combinations).
=======
>>>>>>> 57bdc7c55 (PDK-9352: PCIE SSD support for mpu core on am65xx)
 *
 */

#include <string.h>
#include <stdio.h>

#include <ti/drv/pcie/example/ssd/src/pcie_ssd.h>
#include <ti/drv/pcie/example/ssd/src/nvme.h>

#include <ti/drv/pcie/soc/pcie_soc.h>

#include <ti/drv/i2c/I2C.h>
#include <ti/drv/i2c/soc/I2C_soc.h>
#include <ti/csl/csl_gpio.h>
#include <ti/board/board.h>
#include <ti/board/src/am65xx_idk/include/board_cfg.h>
#include <ti/board/src/evmKeystone3/include/board_i2c_io_exp.h>

#include <ti/osal/osal.h>
#include <ti/osal/TaskP.h>

#if defined(USE_BIOS)
#ifdef __ARM_ARCH_7A__
#include <ti/sysbios/family/arm/a15/Cache.h>
#include <ti/sysbios/family/arm/a15/Mmu.h>
<<<<<<< HEAD
>>>>>>> cc7d9980b (PDK-9352: Added PCIe SSD examples for AM65x)
=======
#endif
#endif

>>>>>>> 36779907c (updated the makefile and other components to enable freertos)
#if defined(SOC_K2G)
#include <ti/csl/cslr_msmc.h>
#define COHERENT  /* Cache ops unnecessary */
#endif
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> 6deb7e263 (PDK-9352: PCIE SSD Support for am65xx)

=======
#endif
>>>>>>> cc7d9980b (PDK-9352: Added PCIe SSD examples for AM65x)
=======

>>>>>>> 36779907c (updated the makefile and other components to enable freertos)
#if defined(SOC_AM65XX) && defined(BUILD_MPU)
#define COHERENT  /* Cache ops unnecessary */
#endif
#if defined(SOC_AM574x) || defined(SOC_AM572x) || defined(SOC_AM571x) || \
    defined(SOC_K2G) || defined(SOC_AM65XX) || defined(SOC_J721E) || defined(__ARM_ARCH_7A__)
<<<<<<< HEAD
<<<<<<< HEAD
#include <ti/board/board.h>
=======
#include "ti/board/board.h"
>>>>>>> cc7d9980b (PDK-9352: Added PCIe SSD examples for AM65x)
=======
#include <ti/board/board.h>
>>>>>>> ad03eb174 (Updated the files as per previous comments)
#endif

#if !defined(SOC_AM572x) && !defined(SOC_AM571x) && !defined(SOC_AM574x) && !defined(SOC_AM65XX) && !defined(SOC_J721E)
#include <ti/csl/csl_bootcfgAux.h>
#include <ti/csl/csl_xmcAux.h>
#include <ti/csl/csl_serdes_pcie.h>
#include <ti/csl/csl_pscAux.h>
#define PCIE_REV0_HW
#else
<<<<<<< HEAD
<<<<<<< HEAD
#ifdef SOC_AM65XX
#include <ti/drv/pcie/example/sample/am65xx/src/pcie_sample_board.h>
=======
#include "pcie_sample_board.h"
#ifdef SOC_AM65XX
>>>>>>> cc7d9980b (PDK-9352: Added PCIe SSD examples for AM65x)
=======
#ifdef SOC_AM65XX
#include <ti/drv/pcie/example/sample/am65xx/src/pcie_sample_board.h>
>>>>>>> ad03eb174 (Updated the files as per previous comments)
#include <ti/csl/cslr_gic500.h>
#define PCIE_REV2_HW
#else
#ifdef SOC_J721E
<<<<<<< HEAD
<<<<<<< HEAD
#include <ti/drv/pcie/example/sample/j721e/src/pcie_sample_board.h>
=======
>>>>>>> cc7d9980b (PDK-9352: Added PCIe SSD examples for AM65x)
=======
#include <ti/drv/pcie/example/sample/j721e/src/pcie_sample_board.h>
>>>>>>> ad03eb174 (Updated the files as per previous comments)
#include <ti/csl/cslr_gic500.h>
#define PCIE_REV3_HW
#else
#define PCIE_REV1_HW
#endif
#endif
#endif
<<<<<<< HEAD
<<<<<<< HEAD

=======
>>>>>>> cc7d9980b (PDK-9352: Added PCIe SSD examples for AM65x)
=======

>>>>>>> ad03eb174 (Updated the files as per previous comments)
#ifdef _TMS320C6X
#include <ti/csl/csl_cacheAux.h>
#include <ti/csl/csl_chip.h>
#endif
#if defined(SOC_AM65XX) || defined(SOC_J721E)
#include <ti/csl/arch/csl_arch.h>
#endif

#if (defined(_TMS320C6X) || defined (__TI_ARM_V7M4__)) || defined (__TI_ARM_V7R4__)
/* Cache coherence: Align must be a multiple of cache line size (L2=128 bytes, L1=64 bytes) to operate with cache enabled. */
/* Aligning to 256 bytes because the PCIe inbound offset register masks the last 8bits of the buffer address  */
#ifdef SOC_J721E
#pragma DATA_SECTION(dstBuf, ".far:dstBufSec")
#pragma DATA_ALIGN(dstBuf, 0x1000) /* TI way of aligning */
#else
#pragma DATA_SECTION(dstBuf, ".dstBufSec")
#pragma DATA_ALIGN(dstBuf, 256) /* TI way of aligning */
#endif
#endif

<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> f0102448a (PCIE SSD Example code)
/* last element in the buffer is a marker that indicates the buffer status: full/empty */
#ifdef SOC_J721E
#define PCIE_EXAMPLE_MAX_CACHE_LINE_SIZE 0x1000
#else
#define PCIE_EXAMPLE_MAX_CACHE_LINE_SIZE 128
#endif
#define PCIE_EXAMPLE_UINT32_SIZE           4 /* preprocessor #if requires a real constant, not a sizeof() */

<<<<<<< HEAD
=======
>>>>>>> cc7d9980b (PDK-9352: Added PCIe SSD examples for AM65x)
=======
>>>>>>> f0102448a (PCIE SSD Example code)
#define PCIE_EXAMPLE_DSTBUF_BYTES ((PCIE_BUFSIZE_APP + 1) * PCIE_EXAMPLE_UINT32_SIZE)
#define PCIE_EXAMPLE_DSTBUF_REM (PCIE_EXAMPLE_DSTBUF_BYTES % PCIE_EXAMPLE_MAX_CACHE_LINE_SIZE)
#define PCIE_EXAMPLE_DSTBUF_PAD (PCIE_EXAMPLE_DSTBUF_REM ? (PCIE_EXAMPLE_MAX_CACHE_LINE_SIZE - PCIE_EXAMPLE_DSTBUF_REM) : 0)
#define PCIE_EDMA_EXAMPLE_DSTBUF_BYTES (PCIE_EXAMPLE_LINE_SIZE * PCIE_EXAMPLE_UINT32_SIZE)
typedef struct dstBuf_s {
  volatile uint32_t buf[PCIE_BUFSIZE_APP + 1];
  /* Cache coherence: Must pad to cache line size in order to enable cacheability */
#if PCIE_EXAMPLE_DSTBUF_PAD
  uint8_t padding[PCIE_EXAMPLE_DSTBUF_PAD];
#endif
#if defined(EDMA) || defined(UDMA)
  volatile uint32_t edma_buf[PCIE_EXAMPLE_LINE_SIZE];
#endif
#ifdef EDMAPKTBENCH
  edmaPktBenchBuf_t edmaPktBenchBuf;
#endif
} dstBuf_t;
dstBuf_t dstBuf
#if defined(BUILD_MPU) || defined(__ARM_ARCH_7A__)
#ifdef SOC_J721E
__attribute__((aligned(0x1000), section(".bss:dstBufSec"))) /* GCC way of aligning */
#else
__attribute__((aligned(256), section(".bss:dstBufSec"))) /* GCC way of aligning */
#endif
#endif
#if defined(SOC_J721E) && defined(BUILD_MPU)
__attribute__((aligned(0x01000))) /* GCC way of aligning */
#endif
; /* for dstBuf */

#define PCIE_EXAMPLE_BUF_EMPTY 0
#define PCIE_EXAMPLE_BUF_FULL  1

/* Does not need to be aligned (even for cache) since it is only accessed locally */
uint32_t srcBuf[PCIE_BUFSIZE_APP];

/* Global variable timers for throughput */
uint64_t totalDMATime = 0;

#ifdef EDMA
/* This is the data that will be used as a temporary space holder
 * for the data being transfered using DMA.
 *
 * This is done since EDMA cannot send a specific value or token
 * but instead it can send blocks of data.
 * */
 #ifdef _TMS320C6X
#pragma DATA_SECTION(dataContainer, ".testData")
#pragma DATA_ALIGN(dataContainer, PCIE_EXAMPLE_LINE_SIZE)
#endif
UInt32 dataContainer[PCIE_EXAMPLE_LINE_SIZE]
#ifdef __ARM_ARCH_7A__
#ifdef SOC_J721E
__attribute__((aligned(0x1000))) /* GCC way of aligning */
#else
__attribute__((aligned(256))) /* GCC way of aligning */
#endif
#endif
; /* for dstBuf */
#endif

#ifdef _TMS320C6X
extern volatile unsigned int cregister TSCL;
#endif

/* Global config variable that controls
   the PCIe mode. It is global so it can be poked
   from CCS. It should be set either to EP or RC. */
pcieMode_e PcieModeGbl = pcie_RC_MODE;

#ifndef CSL_PSC_PD_PCIEX
#ifndef CSL_PSC_PD_PCIE
#define CSL_PSC_PD_PCIE CSL_PSC_PD_PCIE_0
#endif
#else
#define CSL_PSC_PD_PCIE CSL_PSC_PD_PCIEX
#endif

#ifndef CSL_PSC_LPSC_PCIEX
#ifndef CSL_PSC_LPSC_PCIE
#define CSL_PSC_LPSC_PCIE CSL_PSC_LPSC_PCIE_0
#endif
#else
#define CSL_PSC_LPSC_PCIE CSL_PSC_LPSC_PCIEX
#endif

<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> f0102448a (PCIE SSD Example code)

/* FATFS Include */
#include <ti/fs/fatfs/FATFS.h>
#include <ti/drv/pcie/example/ssd/src/fatfs_pcie_interface.h>
#include <ti/drv/pcie/example/ssd/src/fs_shell_app_utils.h>

#if defined(ENABLE_FATFS)

/* FATFS Variables */

FATFS_DrvFxnTable FATFS_drvFxnTable = {
    /* closeDrvFxn */
    FATFS_PCIE_Disk_Close,
    /* controlDrvFxn */
<<<<<<< HEAD
<<<<<<< HEAD
    /*FATFSPortUSBDiskIoctl*/
=======
    //FATFSPortUSBDiskIoctl,
>>>>>>> f0102448a (PCIE SSD Example code)
=======
    /*FATFSPortUSBDiskIoctl*/
>>>>>>> ad03eb174 (Updated the files as per previous comments)
    NULL,
    /* initDrvFxn */
    FATFS_PCIE_Disk_Initialize,
    /* openDrvFxn */
    FATFS_PCIE_Disk_Open,
    /* writeDrvFxn */
    FATFS_PCIE_Write,
    /* readDrvFxn */
    FATFS_PCIE_Read
};

/**
 *  \brief FATFS configuration structure.
 */
FATFS_HwAttrs FATFS_initCfg[_VOLUMES] =
{
    {
        0U
    },
    {
        1U
    },
    {
        2U
    },
    {
        3U
    }
};

/* FATFS objects */
FATFS_Object FATFS_objects[_VOLUMES];

/**
 *  \brief FATFS configuration structure.
 */
const FATFS_Config FATFS_config[_VOLUMES + 1] = {
    {
        &FATFS_drvFxnTable,
        &FATFS_objects[0],
        &FATFS_initCfg[0]
    },

    {
         &FATFS_drvFxnTable,
         &FATFS_objects[1],
         &FATFS_initCfg[1]
    },

    {
         &FATFS_drvFxnTable,
         &FATFS_objects[2],
         &FATFS_initCfg[2]
    },

    {
         &FATFS_drvFxnTable,
         &FATFS_objects[3],
         &FATFS_initCfg[3]
    },
    {NULL, NULL, NULL}
};

FATFS_Handle fatfsHandle = NULL;

#endif
<<<<<<< HEAD
<<<<<<< HEAD

<<<<<<< HEAD
static void PCIe_Reset(void);

=======
static void PCIe_Reset(void);
>>>>>>> d04ff289b (Added the code to toggle the reset line at startup)
=======

static void PCIe_Reset(void);

>>>>>>> ad03eb174 (Updated the files as per previous comments)
static void PCIe_Reset(void)
{
#if defined(SOC_AM65XX)
    I2C_HwAttrs i2cCfg;

    I2C_socGetInitCfg(0, &i2cCfg);

    i2cCfg.baseAddr = CSL_I2C0_CFG_BASE;
    i2cCfg.enableIntr = 0;

    I2C_socSetInitCfg(0, &i2cCfg);

    Board_i2cIoExpInit();
    /* Setting the pin direction as output */
    Board_i2cIoExpSetPinDirection(BOARD_I2C_IOEXP_DEVICE2_ADDR,
                                  PORTNUM_0,
                                  PIN_NUM_5,
                                  PIN_DIRECTION_OUTPUT);

    /* Pulling the EMMC RST line to low for the reset to happen */
    Board_i2cIoExpPinLevelSet(BOARD_I2C_IOEXP_DEVICE2_ADDR,
                              PORTNUM_0,
                              PIN_NUM_5,
                              GPIO_SIGNAL_LEVEL_LOW);
    /* Wait for short duration before pulling the reset line high
       to initiate eMMC reset */
    Osal_delay(100);

    Board_i2cIoExpPinLevelSet(BOARD_I2C_IOEXP_DEVICE2_ADDR,
                              PORTNUM_0,
                              PIN_NUM_5,
                              GPIO_SIGNAL_LEVEL_HIGH);
#endif
}
<<<<<<< HEAD
<<<<<<< HEAD

=======
>>>>>>> d04ff289b (Added the code to toggle the reset line at startup)
void cache_writeback (void *ptr, int size)
{
#ifdef _TMS320C6X
    uint32_t key;
    /* Disable Interrupts */
    key = _disable_interrupts();

    CACHE_wbL1d (ptr, size, CACHE_FENCE_WAIT);
    CACHE_wbL2  (ptr, size, CACHE_FENCE_WAIT);

    /* Reenable Interrupts. */
    _restore_interrupts(key);
#elif defined(__ARM_ARCH_7A__)
#ifndef COHERENT
    /*  while bios could have been used on c66 that device chose csl */
    CacheP_wb (ptr, size);
#endif
#elif defined(__arch64__) || defined(__TI_ARM_V7R4__)
#ifndef COHERENT
    /*  while bios could have been used on c66 that device chose csl */
    CacheP_wb (ptr, size);
#endif
    CSL_archMemoryFence();
=======
=======

>>>>>>> f0102448a (PCIE SSD Example code)
=======

>>>>>>> ad03eb174 (Updated the files as per previous comments)
void cache_writeback (void *ptr, int size)
{
#ifdef _TMS320C6X
    uint32_t key;
    /* Disable Interrupts */
    key = _disable_interrupts();

    CACHE_wbL1d (ptr, size, CACHE_FENCE_WAIT);
    CACHE_wbL2  (ptr, size, CACHE_FENCE_WAIT);

    /* Reenable Interrupts. */
    _restore_interrupts(key);
#elif defined(__ARM_ARCH_7A__)
#ifndef COHERENT
    /*  while bios could have been used on c66 that device chose csl */
    CacheP_wb (ptr, size);
#endif
#elif defined(__arch64__) || defined(__TI_ARM_V7R4__)
#ifndef COHERENT
    /*  while bios could have been used on c66 that device chose csl */
    CacheP_wb (ptr, size);
#endif
<<<<<<< HEAD
  CSL_archMemoryFence();
>>>>>>> cc7d9980b (PDK-9352: Added PCIe SSD examples for AM65x)
=======
    CSL_archMemoryFence();
>>>>>>> ad03eb174 (Updated the files as per previous comments)
#else
/* #error dont know how to writeback the cache */
#endif
}

/*****************************************************************************
 * Function: Converts a core local L2 address to a global L2 address
 *   Input addr:  L2 address to be converted to global.
 *   return:  uint32_t   Global L2 address
 *****************************************************************************/
uint32_t pcieConvert_CoreLocal2GlobalAddr (uintptr_t  addr)
{
#ifdef _TMS320C6X

<<<<<<< HEAD
<<<<<<< HEAD
    uint32_t coreNum;

    /* Get the core number. */
    coreNum = CSL_chipReadReg(CSL_CHIP_DNUM);

#if defined(SOC_AM572x) || defined(SOC_AM571x) || defined(SOC_AM574x)
    /* Compute the global address. */
    return ((1 << 30) | (coreNum << 24) | (addr & 0x00ffffff));

#else
    /* Compute the global address. */
    return ((1 << 28) | (coreNum << 24) | (addr & 0x00ffffff));
#endif
#else
    return addr;
=======
  uint32_t coreNum;
=======
    uint32_t coreNum;
>>>>>>> ad03eb174 (Updated the files as per previous comments)

    /* Get the core number. */
    coreNum = CSL_chipReadReg(CSL_CHIP_DNUM);

#if defined(SOC_AM572x) || defined(SOC_AM571x) || defined(SOC_AM574x)
    /* Compute the global address. */
    return ((1 << 30) | (coreNum << 24) | (addr & 0x00ffffff));

#else
    /* Compute the global address. */
    return ((1 << 28) | (coreNum << 24) | (addr & 0x00ffffff));
#endif
#else
<<<<<<< HEAD
  return addr;
>>>>>>> cc7d9980b (PDK-9352: Added PCIe SSD examples for AM65x)
=======
    return addr;
>>>>>>> ad03eb174 (Updated the files as per previous comments)
#endif
}

/*****************************************************************************
 * Function: Enable/Disable DBI writes
 ****************************************************************************/
pcieRet_e pcieCfgDbi(Pcie_Handle handle, uint8_t enable, pcieLocation_e loc)
{
<<<<<<< HEAD
<<<<<<< HEAD
    pcieRegisters_t        regs;
    pcieRet_e              retVal;
#if defined(PCIE_REV0_HW) || defined(PCIE_REV2_HW)
    pcieCmdStatusReg_t     cmdStatus;

    memset (&cmdStatus, 0, sizeof(cmdStatus));
    memset (&regs, 0, sizeof(regs));

    regs.cmdStatus = &cmdStatus;
    if ((retVal = Pcie_readRegs (handle, loc, &regs)) != pcie_RET_OK)
    {
        PCIE_logPrintf ("Read CMD STATUS register failed!\n");
        return retVal;
    }
    cmdStatus.dbi = enable;

    if ((retVal = Pcie_writeRegs (handle, loc, &regs)) != pcie_RET_OK)
    {
        PCIE_logPrintf ("SET CMD STATUS register failed!\n");
        return retVal;
    }
#elif defined(PCIE_REV3_HW)
    /* No dbiRo setting needed for J7ES */
    memset (&regs, 0, sizeof(regs));
    retVal = pcie_RET_OK;
#else
    pciePlconfDbiRoWrEnReg_t dbiRo;

    memset (&dbiRo, 0, sizeof(dbiRo));
    memset (&regs, 0, sizeof(regs));

    regs.plconfDbiRoWrEn = &dbiRo;

    if ((retVal = Pcie_writeRegs (handle, loc, &regs)) != pcie_RET_OK)
    {
        PCIE_logPrintf ("SET CMD STATUS register failed!\n");
        return retVal;
    }
#endif
    return retVal;
=======
  pcieRegisters_t        regs;
  pcieRet_e              retVal;
=======
    pcieRegisters_t        regs;
    pcieRet_e              retVal;
>>>>>>> ad03eb174 (Updated the files as per previous comments)
#if defined(PCIE_REV0_HW) || defined(PCIE_REV2_HW)
    pcieCmdStatusReg_t     cmdStatus;

    memset (&cmdStatus, 0, sizeof(cmdStatus));
    memset (&regs, 0, sizeof(regs));

    regs.cmdStatus = &cmdStatus;
    if ((retVal = Pcie_readRegs (handle, loc, &regs)) != pcie_RET_OK)
    {
        PCIE_logPrintf ("Read CMD STATUS register failed!\n");
        return retVal;
    }
    cmdStatus.dbi = enable;

    if ((retVal = Pcie_writeRegs (handle, loc, &regs)) != pcie_RET_OK)
    {
        PCIE_logPrintf ("SET CMD STATUS register failed!\n");
        return retVal;
    }
#elif defined(PCIE_REV3_HW)
    /* No dbiRo setting needed for J7ES */
    memset (&regs, 0, sizeof(regs));
    retVal = pcie_RET_OK;
#else
    pciePlconfDbiRoWrEnReg_t dbiRo;

    memset (&dbiRo, 0, sizeof(dbiRo));
    memset (&regs, 0, sizeof(regs));

    regs.plconfDbiRoWrEn = &dbiRo;

    if ((retVal = Pcie_writeRegs (handle, loc, &regs)) != pcie_RET_OK)
    {
        PCIE_logPrintf ("SET CMD STATUS register failed!\n");
        return retVal;
    }
#endif
<<<<<<< HEAD
  return retVal;
>>>>>>> cc7d9980b (PDK-9352: Added PCIe SSD examples for AM65x)
=======
    return retVal;
>>>>>>> ad03eb174 (Updated the files as per previous comments)
}

/*****************************************************************************
 * Function: Enable/Disable DBI_RO_WR_EN writes
 ****************************************************************************/
pcieRet_e pcieCfgDbiRWE(Pcie_Handle handle, uint8_t enable, pcieLocation_e loc)
{
<<<<<<< HEAD
<<<<<<< HEAD
    pcieRet_e              retVal = pcie_RET_OK;
#if defined(PCIE_REV2_HW)
    pcieRegisters_t        regs;
    pciePlconfDbiRoWrEnReg_t dbiRo;

    memset (&dbiRo, 0, sizeof(dbiRo));
    memset (&regs, 0, sizeof(regs));

    dbiRo.cxDbiRoWrEn = enable;
    regs.plconfDbiRoWrEn = &dbiRo;

    if ((retVal = Pcie_writeRegs (handle, loc, &regs)) != pcie_RET_OK)
    {
        PCIE_logPrintf ("SET MISC_CONTROL register failed!\n");
        return retVal;
    }
#endif
    return retVal;
=======
  pcieRet_e              retVal = pcie_RET_OK;
=======
    pcieRet_e              retVal = pcie_RET_OK;
>>>>>>> ad03eb174 (Updated the files as per previous comments)
#if defined(PCIE_REV2_HW)
    pcieRegisters_t        regs;
    pciePlconfDbiRoWrEnReg_t dbiRo;

    memset (&dbiRo, 0, sizeof(dbiRo));
    memset (&regs, 0, sizeof(regs));

    dbiRo.cxDbiRoWrEn = enable;
    regs.plconfDbiRoWrEn = &dbiRo;

    if ((retVal = Pcie_writeRegs (handle, loc, &regs)) != pcie_RET_OK)
    {
        PCIE_logPrintf ("SET MISC_CONTROL register failed!\n");
        return retVal;
    }
#endif
<<<<<<< HEAD
  return retVal;
>>>>>>> cc7d9980b (PDK-9352: Added PCIe SSD examples for AM65x)
=======
    return retVal;
>>>>>>> ad03eb174 (Updated the files as per previous comments)
}

/*****************************************************************************
 * Function: Utility function a cycle clock
 ****************************************************************************/
static uint32_t readTime32(void)
{
<<<<<<< HEAD
<<<<<<< HEAD
    uint32_t timeVal;

#if defined (_TMS320C6X)
    timeVal = TSCL;
#elif __ARM_ARCH_7A__
    __asm__ __volatile__ ("MRC p15, 0, %0, c9, c13, 0\t\n": "=r"(timeVal));
#else
    /* M4 specific implementation*/
    static uint32_t simuTimer = 0;
    simuTimer++;
    timeVal = simuTimer;
#endif
    return timeVal;
=======
  uint32_t timeVal;
=======
    uint32_t timeVal;
>>>>>>> ad03eb174 (Updated the files as per previous comments)

#if defined (_TMS320C6X)
    timeVal = TSCL;
#elif __ARM_ARCH_7A__
    __asm__ __volatile__ ("MRC p15, 0, %0, c9, c13, 0\t\n": "=r"(timeVal));
#else
    /* M4 specific implementation*/
    static uint32_t simuTimer = 0;
    simuTimer++;
    timeVal = simuTimer;
#endif
<<<<<<< HEAD
  return timeVal;
>>>>>>> cc7d9980b (PDK-9352: Added PCIe SSD examples for AM65x)
=======
    return timeVal;
>>>>>>> ad03eb174 (Updated the files as per previous comments)
}

/*****************************************************************************
 * Function: Utility function to introduce delay
 ****************************************************************************/
void cycleDelay (uint32_t count)
{
<<<<<<< HEAD
<<<<<<< HEAD
    uint32_t start = (uint32_t)readTime32();

    while (((uint32_t)readTime32() - start) < count);
=======
  uint32_t start = (uint32_t)readTime32();

  while (((uint32_t)readTime32() - start) < count);
>>>>>>> cc7d9980b (PDK-9352: Added PCIe SSD examples for AM65x)
=======
    uint32_t start = (uint32_t)readTime32();

    while (((uint32_t)readTime32() - start) < count);
>>>>>>> ad03eb174 (Updated the files as per previous comments)
}

/*****************************************************************************
 * Function: Serdes configuration
 ****************************************************************************/
pcieRet_e pcieSerdesCfg(void)
{
#if !defined(DEVICE_K2K) && !defined(DEVICE_K2H) && !defined(DEVICE_K2E) && !defined(DEVICE_K2L) && \
    !defined(SOC_K2K) && !defined(SOC_K2H) && !defined(SOC_K2L) && !defined(SOC_K2E) && !defined(SOC_K2G) && \
    !defined(SOC_AM572x) && !defined(SOC_AM571x) && !defined(SOC_AM574x) && \
    !defined(SOC_AM65XX) && !defined(SOC_J721E)
<<<<<<< HEAD
<<<<<<< HEAD
    uint16_t cfg;

    /* Provide PLL reference clock to SERDES inside PCIESS
     Program PLL settings and enable PLL from PCIe SERDES.*/
    cfg = 0x01C9; /* value based on PCIe userguide */

    CSL_BootCfgSetPCIEConfigPLL(cfg);
#else /* !DEVICE_K2K && !DEVICE_K2H && !DEVICE_K2E && !DEVICE_K2L */
#if defined(SOC_AM572x) || defined(SOC_AM571x) || defined(SOC_AM574x)

    /*Set PCIE_PERSTn to out of reset state*/
    PlatformPCIE_GPIO_Init();
    PlatformPCIE_PERSTn_Reset(0);

    PlatformPCIESS1ClockEnable();
    PlatformPCIESS2ClockEnable();
    PlatformPCIESS1PllConfig();
    PlatformPCIESSSetPhyMode();

    PlatformPCIESS1SetDll();
    PlatformPCIESS2SetDll();

    PlatformPCIESS1CtrlConfig();
    PlatformPCIESS2CtrlConfig();
    PlatformPCIESS1Reset();
    PlatformPCIESS2Reset();
    PlatformPCIESS1PhyConfig();
    PlatformPCIESS2PhyConfig();
#elif defined(SOC_AM65XX)
#ifdef am65xx_idk
    PlatformPCIESSSerdesConfig(0, 0);
#ifndef PCIE0_SERDES0
    PlatformPCIESSSerdesConfig(1, 0);
#endif
#else
    PlatformPCIESSSerdesConfig(1, 1);
#endif
#elif defined(SOC_J721E)
    PlatformPCIESSSerdesConfig(1, 1);
#else
#ifndef  SIMULATOR_SUPPORT

    uint32_t i;

#if defined(DEVICE_K2E) || defined(SOC_K2E)
    /* Configure 2 lanes of serdes with different config */

    CSL_SERDES_RESULT status1, status2;
    CSL_SERDES_LANE_ENABLE_STATUS lane_retval1 = CSL_SERDES_LANE_ENABLE_NO_ERR;
    CSL_SERDES_LANE_ENABLE_STATUS lane_retval2 = CSL_SERDES_LANE_ENABLE_NO_ERR;
    CSL_SERDES_LANE_ENABLE_PARAMS_T serdes_lane_enable_params1, serdes_lane_enable_params2;

    memset(&serdes_lane_enable_params1, 0, sizeof(serdes_lane_enable_params1));
    memset(&serdes_lane_enable_params2, 0, sizeof(serdes_lane_enable_params2));

    serdes_lane_enable_params1.base_addr = CSL_PCIE_0_SERDES_CFG_REGS;
    serdes_lane_enable_params1.peripheral_base_addr = CSL_PCIE_0_SLV_CFG_REGS;
    serdes_lane_enable_params1.ref_clock = CSL_SERDES_REF_CLOCK_100M;
    serdes_lane_enable_params1.linkrate = CSL_SERDES_LINK_RATE_5G;
    serdes_lane_enable_params1.num_lanes = 1;
    serdes_lane_enable_params1.phy_type = SERDES_PCIe;
    serdes_lane_enable_params1.operating_mode = CSL_SERDES_FUNCTIONAL_MODE;
    serdes_lane_enable_params1.lane_mask = 0x1;

    for(i=0; i< serdes_lane_enable_params1.num_lanes; i++)
    {
        serdes_lane_enable_params1.loopback_mode[i] = CSL_SERDES_LOOPBACK_DISABLED;
        serdes_lane_enable_params1.lane_ctrl_rate[i] = CSL_SERDES_LANE_FULL_RATE; /* GEN2 */
    }

    serdes_lane_enable_params2.base_addr = CSL_PCIE_1_SERDES_CFG_REGS;
    serdes_lane_enable_params2.peripheral_base_addr = CSL_PCIE_1_SLV_CFG_REGS;
    serdes_lane_enable_params2.ref_clock = CSL_SERDES_REF_CLOCK_100M;
    serdes_lane_enable_params2.linkrate = CSL_SERDES_LINK_RATE_5G;
    serdes_lane_enable_params2.num_lanes = 1;
    serdes_lane_enable_params2.phy_type = SERDES_PCIe;
    serdes_lane_enable_params2.operating_mode = CSL_SERDES_FUNCTIONAL_MODE;
    serdes_lane_enable_params2.lane_mask = 0x1;

    for(i=0; i< serdes_lane_enable_params2.num_lanes; i++)
    {
        serdes_lane_enable_params2.loopback_mode[i] = CSL_SERDES_LOOPBACK_DISABLED;
        serdes_lane_enable_params2.lane_ctrl_rate[i] = CSL_SERDES_LANE_FULL_RATE; /* GEN2 */
    }

    /* SB CMU and COMLANE Setup */
    status1 = CSL_PCIeSerdesInit(serdes_lane_enable_params1.base_addr, serdes_lane_enable_params1.ref_clock, serdes_lane_enable_params1.linkrate);

    if (status1 != 0)
        PCIE_logPrintf ("Debug: Invalid PCIE 0 Serdes Init Params\n");

    status2 = CSL_PCIeSerdesInit(serdes_lane_enable_params2.base_addr, serdes_lane_enable_params2.ref_clock, serdes_lane_enable_params2.linkrate);

    if (status2 != 0)
        PCIE_logPrintf ("Debug: Invalid PCIE 1 Serdes Init Params\n");

    /* Common Init Mode */
    /* Iteration Mode needs to be set to Common Init Mode first with a lane_mask value equal to the total number of lanes being configured */
    /* For example, if there are a total of 2 lanes being configured, lane mask needs to be set to 0x3 */
    serdes_lane_enable_params1.iteration_mode = CSL_SERDES_LANE_ENABLE_COMMON_INIT;
    serdes_lane_enable_params1.lane_mask = 0x1;
    lane_retval1 = CSL_SerdesLaneEnable(&serdes_lane_enable_params1);

    /* Lane Init Mode */
    /* Once CSL_SerdesLaneEnable is called with iteration_mode = CSL_SERDES_LANE_ENABLE_COMMON_INIT, the lanes needs to be initialized by setting
    iteration_mode =  CSL_SERDES_LANE_ENABLE_LANE_INIT with the lane_mask equal to the specific lane being configured */
    /* For example, if lane 0 is being configured, lane mask needs to be set to 0x1. if lane 1 is being configured, lane mask needs to be 0x2 etc */
    serdes_lane_enable_params1.iteration_mode = CSL_SERDES_LANE_ENABLE_LANE_INIT;
    for(i=0; i< serdes_lane_enable_params1.num_lanes; i++)
    {
        serdes_lane_enable_params1.lane_mask = 1<<i;
        lane_retval1 = CSL_SerdesLaneEnable(&serdes_lane_enable_params1);
    }

    if (lane_retval1 != 0)
    {
        PCIE_logPrintf ("Invalid Serdes Lane Enable Init\n");
        exit(0);
    }

    /* Common Init Mode */
    /* Iteration Mode needs to be set to Common Init Mode first with a lane_mask value equal to the total number of lanes being configured */
    /* For example, if there are a total of 2 lanes being configured, lane mask needs to be set to 0x3 */
    serdes_lane_enable_params2.iteration_mode = CSL_SERDES_LANE_ENABLE_COMMON_INIT;
    serdes_lane_enable_params2.lane_mask = 0x1;
    lane_retval2 = CSL_SerdesLaneEnable(&serdes_lane_enable_params2);

    /* Lane Init Mode */
    /* Once CSL_SerdesLaneEnable is called with iteration_mode = CSL_SERDES_LANE_ENABLE_COMMON_INIT, the lanes needs to be initialized by setting
    iteration_mode =  CSL_SERDES_LANE_ENABLE_LANE_INIT with the lane_mask equal to the specific lane being configured */
    /* For example, if lane 0 is being configured, lane mask needs to be set to 0x1. if lane 1 is being configured, lane mask needs to be 0x2 etc */
    serdes_lane_enable_params2.iteration_mode = CSL_SERDES_LANE_ENABLE_LANE_INIT;
    for(i=0; i< serdes_lane_enable_params2.num_lanes; i++)
    {
        serdes_lane_enable_params2.lane_mask = 1<<i;
        lane_retval2 = CSL_SerdesLaneEnable(&serdes_lane_enable_params2);
    }

    if (lane_retval2 != 0)
    {
        PCIE_logPrintf ("Invalid Serdes Lane Enable Init\n");
        exit(0);
    }

    PCIE_logPrintf ("Debug: Serdes Setup Successfully\n");
#else
    /* Configure all lane of serdes with common config */
    CSL_SERDES_RESULT status;
    CSL_SERDES_LANE_ENABLE_STATUS lane_retval = CSL_SERDES_LANE_ENABLE_NO_ERR;
    CSL_SERDES_LANE_ENABLE_PARAMS_T serdes_lane_enable_params;

    memset(&serdes_lane_enable_params, 0, sizeof(serdes_lane_enable_params));

#if defined(DEVICE_K2L) || defined(SOC_K2L)
    /* Check CSISC2_3_MUXSEL bit */
    if (CSL_FEXTR(*(volatile uint32_t *)(CSL_BOOT_CFG_REGS + 0x20), 28, 28) != 1)
    {
        PCIE_logPrintf ("PCIe Serdes Mux Not Selected!\n");
        exit(1);
    }

    serdes_lane_enable_params.base_addr = CSL_CSISC2_3_SERDES_CFG_REGS;
    serdes_lane_enable_params.peripheral_base_addr = CSL_PCIE_0_SLV_CFG_REGS;
#elif defined(SOC_K2G)
    serdes_lane_enable_params.base_addr = CSL_PCIE_0_SERDES_CFG_REGS;
    serdes_lane_enable_params.peripheral_base_addr = CSL_PCIE_0_SLV_CFG_REGS;
#else
    serdes_lane_enable_params.base_addr = CSL_PCIE_SERDES_CFG_REGS;
    serdes_lane_enable_params.peripheral_base_addr = CSL_PCIE_SLV_CFG_REGS;
#endif

    serdes_lane_enable_params.ref_clock = CSL_SERDES_REF_CLOCK_100M;
    serdes_lane_enable_params.linkrate = CSL_SERDES_LINK_RATE_5G;
    serdes_lane_enable_params.num_lanes = 1;
    serdes_lane_enable_params.phy_type = SERDES_PCIe;
    serdes_lane_enable_params.operating_mode = CSL_SERDES_FUNCTIONAL_MODE;
    serdes_lane_enable_params.lane_mask = 0x1;
    for(i=0; i< serdes_lane_enable_params.num_lanes; i++)
    {
        serdes_lane_enable_params.loopback_mode[i] = CSL_SERDES_LOOPBACK_DISABLED;
        serdes_lane_enable_params.lane_ctrl_rate[i] = CSL_SERDES_LANE_FULL_RATE; /* GEN2 */
    }

    status = CSL_PCIeSerdesInit(serdes_lane_enable_params.base_addr,
                                serdes_lane_enable_params.ref_clock,
                                serdes_lane_enable_params.linkrate);

    if (status != 0)
    {
        PCIE_logPrintf ("Invalid Serdes Init Params\n");
    }

    /* Common Init Mode */
    /* Iteration Mode needs to be set to Common Init Mode first with a lane_mask value equal to the total number of lanes being configured */
    /* For example, if there are a total of 2 lanes being configured, lane mask needs to be set to 0x3 */
    serdes_lane_enable_params.iteration_mode = CSL_SERDES_LANE_ENABLE_COMMON_INIT;
    serdes_lane_enable_params.lane_mask = 0x1;
    lane_retval = CSL_SerdesLaneEnable(&serdes_lane_enable_params);

    /* Lane Init Mode */
    /* Once CSL_SerdesLaneEnable is called with iteration_mode = CSL_SERDES_LANE_ENABLE_COMMON_INIT, the lanes needs to be initialized by setting
    iteration_mode =  CSL_SERDES_LANE_ENABLE_LANE_INIT with the lane_mask equal to the specific lane being configured */
    /* For example, if lane 0 is being configured, lane mask needs to be set to 0x1. if lane 1 is being configured, lane mask needs to be 0x2 etc */
    serdes_lane_enable_params.iteration_mode = CSL_SERDES_LANE_ENABLE_LANE_INIT;
    for(i=0; i< serdes_lane_enable_params.num_lanes; i++)
    {
        serdes_lane_enable_params.lane_mask = 1<<i;
        lane_retval = CSL_SerdesLaneEnable(&serdes_lane_enable_params);
    }

    if (lane_retval != 0)
    {
        PCIE_logPrintf ("Invalid Serdes Lane Enable Init\n");
        exit(0);
    }

    PCIE_logPrintf ("Debug: Serdes Setup Successfully\n");
#endif
#endif
#endif
#endif
    /*Wait for PLL to lock (3000 CLKIN1 cycles) */
    cycleDelay(10000);

    return pcie_RET_OK;
=======
  uint16_t cfg;
=======
    uint16_t cfg;
>>>>>>> ad03eb174 (Updated the files as per previous comments)

    /* Provide PLL reference clock to SERDES inside PCIESS
     Program PLL settings and enable PLL from PCIe SERDES.*/
    cfg = 0x01C9; /* value based on PCIe userguide */

    CSL_BootCfgSetPCIEConfigPLL(cfg);
#else /* !DEVICE_K2K && !DEVICE_K2H && !DEVICE_K2E && !DEVICE_K2L */
#if defined(SOC_AM572x) || defined(SOC_AM571x) || defined(SOC_AM574x)

    /*Set PCIE_PERSTn to out of reset state*/
    PlatformPCIE_GPIO_Init();
    PlatformPCIE_PERSTn_Reset(0);

    PlatformPCIESS1ClockEnable();
    PlatformPCIESS2ClockEnable();
    PlatformPCIESS1PllConfig();
    PlatformPCIESSSetPhyMode();

    PlatformPCIESS1SetDll();
    PlatformPCIESS2SetDll();

    PlatformPCIESS1CtrlConfig();
    PlatformPCIESS2CtrlConfig();
    PlatformPCIESS1Reset();
    PlatformPCIESS2Reset();
    PlatformPCIESS1PhyConfig();
    PlatformPCIESS2PhyConfig();
#elif defined(SOC_AM65XX)
#ifdef am65xx_idk
    PlatformPCIESSSerdesConfig(0, 0);
#ifndef PCIE0_SERDES0
    PlatformPCIESSSerdesConfig(1, 0);
#endif
#else
    PlatformPCIESSSerdesConfig(1, 1);
#endif
#elif defined(SOC_J721E)
    PlatformPCIESSSerdesConfig(1, 1);
#else
#ifndef  SIMULATOR_SUPPORT

    uint32_t i;

#if defined(DEVICE_K2E) || defined(SOC_K2E)
    /* Configure 2 lanes of serdes with different config */

    CSL_SERDES_RESULT status1, status2;
    CSL_SERDES_LANE_ENABLE_STATUS lane_retval1 = CSL_SERDES_LANE_ENABLE_NO_ERR;
    CSL_SERDES_LANE_ENABLE_STATUS lane_retval2 = CSL_SERDES_LANE_ENABLE_NO_ERR;
    CSL_SERDES_LANE_ENABLE_PARAMS_T serdes_lane_enable_params1, serdes_lane_enable_params2;

    memset(&serdes_lane_enable_params1, 0, sizeof(serdes_lane_enable_params1));
    memset(&serdes_lane_enable_params2, 0, sizeof(serdes_lane_enable_params2));

    serdes_lane_enable_params1.base_addr = CSL_PCIE_0_SERDES_CFG_REGS;
    serdes_lane_enable_params1.peripheral_base_addr = CSL_PCIE_0_SLV_CFG_REGS;
    serdes_lane_enable_params1.ref_clock = CSL_SERDES_REF_CLOCK_100M;
    serdes_lane_enable_params1.linkrate = CSL_SERDES_LINK_RATE_5G;
    serdes_lane_enable_params1.num_lanes = 1;
    serdes_lane_enable_params1.phy_type = SERDES_PCIe;
    serdes_lane_enable_params1.operating_mode = CSL_SERDES_FUNCTIONAL_MODE;
    serdes_lane_enable_params1.lane_mask = 0x1;

    for(i=0; i< serdes_lane_enable_params1.num_lanes; i++)
    {
        serdes_lane_enable_params1.loopback_mode[i] = CSL_SERDES_LOOPBACK_DISABLED;
        serdes_lane_enable_params1.lane_ctrl_rate[i] = CSL_SERDES_LANE_FULL_RATE; /* GEN2 */
    }

    serdes_lane_enable_params2.base_addr = CSL_PCIE_1_SERDES_CFG_REGS;
    serdes_lane_enable_params2.peripheral_base_addr = CSL_PCIE_1_SLV_CFG_REGS;
    serdes_lane_enable_params2.ref_clock = CSL_SERDES_REF_CLOCK_100M;
    serdes_lane_enable_params2.linkrate = CSL_SERDES_LINK_RATE_5G;
    serdes_lane_enable_params2.num_lanes = 1;
    serdes_lane_enable_params2.phy_type = SERDES_PCIe;
    serdes_lane_enable_params2.operating_mode = CSL_SERDES_FUNCTIONAL_MODE;
    serdes_lane_enable_params2.lane_mask = 0x1;

    for(i=0; i< serdes_lane_enable_params2.num_lanes; i++)
    {
        serdes_lane_enable_params2.loopback_mode[i] = CSL_SERDES_LOOPBACK_DISABLED;
        serdes_lane_enable_params2.lane_ctrl_rate[i] = CSL_SERDES_LANE_FULL_RATE; /* GEN2 */
    }

    /* SB CMU and COMLANE Setup */
    status1 = CSL_PCIeSerdesInit(serdes_lane_enable_params1.base_addr, serdes_lane_enable_params1.ref_clock, serdes_lane_enable_params1.linkrate);

    if (status1 != 0)
        PCIE_logPrintf ("Debug: Invalid PCIE 0 Serdes Init Params\n");

    status2 = CSL_PCIeSerdesInit(serdes_lane_enable_params2.base_addr, serdes_lane_enable_params2.ref_clock, serdes_lane_enable_params2.linkrate);

    if (status2 != 0)
        PCIE_logPrintf ("Debug: Invalid PCIE 1 Serdes Init Params\n");

    /* Common Init Mode */
    /* Iteration Mode needs to be set to Common Init Mode first with a lane_mask value equal to the total number of lanes being configured */
    /* For example, if there are a total of 2 lanes being configured, lane mask needs to be set to 0x3 */
    serdes_lane_enable_params1.iteration_mode = CSL_SERDES_LANE_ENABLE_COMMON_INIT;
    serdes_lane_enable_params1.lane_mask = 0x1;
    lane_retval1 = CSL_SerdesLaneEnable(&serdes_lane_enable_params1);

    /* Lane Init Mode */
    /* Once CSL_SerdesLaneEnable is called with iteration_mode = CSL_SERDES_LANE_ENABLE_COMMON_INIT, the lanes needs to be initialized by setting
    iteration_mode =  CSL_SERDES_LANE_ENABLE_LANE_INIT with the lane_mask equal to the specific lane being configured */
    /* For example, if lane 0 is being configured, lane mask needs to be set to 0x1. if lane 1 is being configured, lane mask needs to be 0x2 etc */
    serdes_lane_enable_params1.iteration_mode = CSL_SERDES_LANE_ENABLE_LANE_INIT;
    for(i=0; i< serdes_lane_enable_params1.num_lanes; i++)
    {
        serdes_lane_enable_params1.lane_mask = 1<<i;
        lane_retval1 = CSL_SerdesLaneEnable(&serdes_lane_enable_params1);
    }

    if (lane_retval1 != 0)
    {
        PCIE_logPrintf ("Invalid Serdes Lane Enable Init\n");
        exit(0);
    }

    /* Common Init Mode */
    /* Iteration Mode needs to be set to Common Init Mode first with a lane_mask value equal to the total number of lanes being configured */
    /* For example, if there are a total of 2 lanes being configured, lane mask needs to be set to 0x3 */
    serdes_lane_enable_params2.iteration_mode = CSL_SERDES_LANE_ENABLE_COMMON_INIT;
    serdes_lane_enable_params2.lane_mask = 0x1;
    lane_retval2 = CSL_SerdesLaneEnable(&serdes_lane_enable_params2);

    /* Lane Init Mode */
    /* Once CSL_SerdesLaneEnable is called with iteration_mode = CSL_SERDES_LANE_ENABLE_COMMON_INIT, the lanes needs to be initialized by setting
    iteration_mode =  CSL_SERDES_LANE_ENABLE_LANE_INIT with the lane_mask equal to the specific lane being configured */
    /* For example, if lane 0 is being configured, lane mask needs to be set to 0x1. if lane 1 is being configured, lane mask needs to be 0x2 etc */
    serdes_lane_enable_params2.iteration_mode = CSL_SERDES_LANE_ENABLE_LANE_INIT;
    for(i=0; i< serdes_lane_enable_params2.num_lanes; i++)
    {
        serdes_lane_enable_params2.lane_mask = 1<<i;
        lane_retval2 = CSL_SerdesLaneEnable(&serdes_lane_enable_params2);
    }

    if (lane_retval2 != 0)
    {
        PCIE_logPrintf ("Invalid Serdes Lane Enable Init\n");
        exit(0);
    }

    PCIE_logPrintf ("Debug: Serdes Setup Successfully\n");
#else
    /* Configure all lane of serdes with common config */
    CSL_SERDES_RESULT status;
    CSL_SERDES_LANE_ENABLE_STATUS lane_retval = CSL_SERDES_LANE_ENABLE_NO_ERR;
    CSL_SERDES_LANE_ENABLE_PARAMS_T serdes_lane_enable_params;

    memset(&serdes_lane_enable_params, 0, sizeof(serdes_lane_enable_params));

#if defined(DEVICE_K2L) || defined(SOC_K2L)
    /* Check CSISC2_3_MUXSEL bit */
    if (CSL_FEXTR(*(volatile uint32_t *)(CSL_BOOT_CFG_REGS + 0x20), 28, 28) != 1)
    {
        PCIE_logPrintf ("PCIe Serdes Mux Not Selected!\n");
        exit(1);
    }

    serdes_lane_enable_params.base_addr = CSL_CSISC2_3_SERDES_CFG_REGS;
    serdes_lane_enable_params.peripheral_base_addr = CSL_PCIE_0_SLV_CFG_REGS;
#elif defined(SOC_K2G)
    serdes_lane_enable_params.base_addr = CSL_PCIE_0_SERDES_CFG_REGS;
    serdes_lane_enable_params.peripheral_base_addr = CSL_PCIE_0_SLV_CFG_REGS;
#else
    serdes_lane_enable_params.base_addr = CSL_PCIE_SERDES_CFG_REGS;
    serdes_lane_enable_params.peripheral_base_addr = CSL_PCIE_SLV_CFG_REGS;
#endif

    serdes_lane_enable_params.ref_clock = CSL_SERDES_REF_CLOCK_100M;
    serdes_lane_enable_params.linkrate = CSL_SERDES_LINK_RATE_5G;
    serdes_lane_enable_params.num_lanes = 1;
    serdes_lane_enable_params.phy_type = SERDES_PCIe;
    serdes_lane_enable_params.operating_mode = CSL_SERDES_FUNCTIONAL_MODE;
    serdes_lane_enable_params.lane_mask = 0x1;
    for(i=0; i< serdes_lane_enable_params.num_lanes; i++)
    {
        serdes_lane_enable_params.loopback_mode[i] = CSL_SERDES_LOOPBACK_DISABLED;
        serdes_lane_enable_params.lane_ctrl_rate[i] = CSL_SERDES_LANE_FULL_RATE; /* GEN2 */
    }

    status = CSL_PCIeSerdesInit(serdes_lane_enable_params.base_addr,
                                serdes_lane_enable_params.ref_clock,
                                serdes_lane_enable_params.linkrate);

    if (status != 0)
    {
        PCIE_logPrintf ("Invalid Serdes Init Params\n");
    }

    /* Common Init Mode */
    /* Iteration Mode needs to be set to Common Init Mode first with a lane_mask value equal to the total number of lanes being configured */
    /* For example, if there are a total of 2 lanes being configured, lane mask needs to be set to 0x3 */
    serdes_lane_enable_params.iteration_mode = CSL_SERDES_LANE_ENABLE_COMMON_INIT;
    serdes_lane_enable_params.lane_mask = 0x1;
    lane_retval = CSL_SerdesLaneEnable(&serdes_lane_enable_params);

    /* Lane Init Mode */
    /* Once CSL_SerdesLaneEnable is called with iteration_mode = CSL_SERDES_LANE_ENABLE_COMMON_INIT, the lanes needs to be initialized by setting
    iteration_mode =  CSL_SERDES_LANE_ENABLE_LANE_INIT with the lane_mask equal to the specific lane being configured */
    /* For example, if lane 0 is being configured, lane mask needs to be set to 0x1. if lane 1 is being configured, lane mask needs to be 0x2 etc */
    serdes_lane_enable_params.iteration_mode = CSL_SERDES_LANE_ENABLE_LANE_INIT;
    for(i=0; i< serdes_lane_enable_params.num_lanes; i++)
    {
        serdes_lane_enable_params.lane_mask = 1<<i;
        lane_retval = CSL_SerdesLaneEnable(&serdes_lane_enable_params);
    }

    if (lane_retval != 0)
    {
        PCIE_logPrintf ("Invalid Serdes Lane Enable Init\n");
        exit(0);
    }

    PCIE_logPrintf ("Debug: Serdes Setup Successfully\n");
#endif
#endif
#endif
#endif
    /*Wait for PLL to lock (3000 CLKIN1 cycles) */
    cycleDelay(10000);

<<<<<<< HEAD
  return pcie_RET_OK;
>>>>>>> cc7d9980b (PDK-9352: Added PCIe SSD examples for AM65x)
=======
    return pcie_RET_OK;
>>>>>>> ad03eb174 (Updated the files as per previous comments)
}

/*****************************************************************************
 * Function: Enable/Disable LTSSM (Link Training)
 * This function demonstrates how one can write one binary to use either
 * rev of PCIE
 ****************************************************************************/
pcieRet_e pcieLtssmCtrl(Pcie_Handle handle, uint8_t enable, pcieLocation_e loc)
{
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> ad03eb174 (Updated the files as per previous comments)
    pcieCmdStatusReg_t       cmdStatus;
    pcieTiConfDeviceCmdReg_t deviceCmd;
    pcieRegisters_t          regs;
    pcieRet_e retVal;

    memset (&cmdStatus,    0, sizeof(cmdStatus));
    memset (&deviceCmd,    0, sizeof(deviceCmd));
    memset (&regs,         0, sizeof(regs));

    regs.cmdStatus = &cmdStatus;
    if ((retVal = Pcie_readRegs (handle, pcie_LOCATION_LOCAL, &regs)) != pcie_RET_OK)
<<<<<<< HEAD
    {
        if (retVal == pcie_RET_INV_REG)
        {
            /* The cmdStatus register doesn't exist; try the deviceCmd instead */
            regs.cmdStatus       = NULL;
            regs.tiConfDeviceCmd = &deviceCmd;
            if ((retVal = Pcie_readRegs (handle, pcie_LOCATION_LOCAL, &regs)) != pcie_RET_OK)
            {
                PCIE_logPrintf ("Read CMD STATUS and DEVICE CMD registers failed!\n");
                return retVal;
            }
        }
        else
        {
            PCIE_logPrintf ("Read CMD STATUS register failed!\n");
            return retVal;
        }
    }

    if(enable)
        deviceCmd.ltssmEn = cmdStatus.ltssmEn = 1;
    else
        deviceCmd.ltssmEn = cmdStatus.ltssmEn = 0;

    if ((retVal = Pcie_writeRegs (handle, pcie_LOCATION_LOCAL, &regs)) != pcie_RET_OK)
    {
        PCIE_logPrintf ("SET CMD STATUS register failed!\n");
        return retVal;
    }

    return pcie_RET_OK;
=======
  pcieCmdStatusReg_t       cmdStatus;
  pcieTiConfDeviceCmdReg_t deviceCmd;
  pcieRegisters_t          regs;
  pcieRet_e retVal;

  memset (&cmdStatus,    0, sizeof(cmdStatus));
  memset (&deviceCmd,    0, sizeof(deviceCmd));
  memset (&regs,         0, sizeof(regs));

  regs.cmdStatus = &cmdStatus;
  if ((retVal = Pcie_readRegs (handle, pcie_LOCATION_LOCAL, &regs)) != pcie_RET_OK)
  {
    if (retVal == pcie_RET_INV_REG)
    {
      /* The cmdStatus register doesn't exist; try the deviceCmd instead */
      regs.cmdStatus       = NULL;
      regs.tiConfDeviceCmd = &deviceCmd;
      if ((retVal = Pcie_readRegs (handle, pcie_LOCATION_LOCAL, &regs)) != pcie_RET_OK)
      {
        PCIE_logPrintf ("Read CMD STATUS and DEVICE CMD registers failed!\n");
        return retVal;
      }
    }
    else
=======
>>>>>>> ad03eb174 (Updated the files as per previous comments)
    {
        if (retVal == pcie_RET_INV_REG)
        {
            /* The cmdStatus register doesn't exist; try the deviceCmd instead */
            regs.cmdStatus       = NULL;
            regs.tiConfDeviceCmd = &deviceCmd;
            if ((retVal = Pcie_readRegs (handle, pcie_LOCATION_LOCAL, &regs)) != pcie_RET_OK)
            {
                PCIE_logPrintf ("Read CMD STATUS and DEVICE CMD registers failed!\n");
                return retVal;
            }
        }
        else
        {
            PCIE_logPrintf ("Read CMD STATUS register failed!\n");
            return retVal;
        }
    }

    if(enable)
        deviceCmd.ltssmEn = cmdStatus.ltssmEn = 1;
    else
        deviceCmd.ltssmEn = cmdStatus.ltssmEn = 0;

    if ((retVal = Pcie_writeRegs (handle, pcie_LOCATION_LOCAL, &regs)) != pcie_RET_OK)
    {
        PCIE_logPrintf ("SET CMD STATUS register failed!\n");
        return retVal;
    }

<<<<<<< HEAD
  return pcie_RET_OK;
>>>>>>> cc7d9980b (PDK-9352: Added PCIe SSD examples for AM65x)
=======
    return pcie_RET_OK;
>>>>>>> ad03eb174 (Updated the files as per previous comments)
}

pcieRet_e pcieSetReg2(Pcie_Handle handle, pcieLocation_e loc)
{
    pcieRet_e               retVal;

    pcieRegisters_t         regs;
    pcieRootCtrlCapReg_t    rootCtrlCap;
    pcieVndDevIdReg_t       vndDevId;
    pcieRevIdReg_t          revId;

    memset (&regs,             0, sizeof(regs));
    memset (&rootCtrlCap,      0, sizeof(rootCtrlCap));
    memset (&vndDevId,         0, sizeof(vndDevId));
    memset (&revId,            0, sizeof(revId));

<<<<<<< HEAD
<<<<<<< HEAD
    HW_WR_REG32(PCIE_EP_BAR0_REG, 0x0);
=======
    HW_WR_REG32(0x05501010, 0x0);
>>>>>>> cc7d9980b (PDK-9352: Added PCIe SSD examples for AM65x)
=======
    HW_WR_REG32(PCIE_EP_BAR0_REG, 0x0);
>>>>>>> 3fda8cd5c (Updating as per the review comments)

    memset (&regs, 0, sizeof(regs));
    regs.vndDevId = &vndDevId;
    if ((retVal = Pcie_readRegs (handle, loc, &regs)) != pcie_RET_OK)
    {
        PCIE_logPrintf ("GET vndDevId register failed!\n");
        return retVal;
    }
    vndDevId.devId    = 0xB00C;
    vndDevId.vndId    = 0x104c;
    regs.vndDevId = &vndDevId;

    if ((retVal = Pcie_writeRegs (handle, loc, &regs)) != pcie_RET_OK)
    {
        PCIE_logPrintf ("SET vndDevId register failed!\n");
        return retVal;
    }

    memset (&regs, 0, sizeof(regs));
    regs.revId = &revId;
    if ((retVal = Pcie_readRegs (handle, loc, &regs)) != pcie_RET_OK)
    {
        PCIE_logPrintf ("GET revId register failed!\n");
        return retVal;
    }
    revId.classCode    = 0x00060400;
    regs.revId = &revId;

    if ((retVal = Pcie_writeRegs (handle, loc, &regs)) != pcie_RET_OK)
    {
        PCIE_logPrintf ("SET revId register failed!\n");
        return retVal;
    }

    memset (&regs, 0, sizeof(regs));
    regs.rootCtrlCap = &rootCtrlCap;
    if ((retVal = Pcie_readRegs (handle, loc, &regs)) != pcie_RET_OK)
    {
        PCIE_logPrintf ("GET rootCtrlCap register failed!\n");
        return retVal;
    }
    rootCtrlCap.crsSw    = 1;
    rootCtrlCap.crsSwEn  = 1;
    rootCtrlCap.pmeIntEn = 1;
    regs.rootCtrlCap = &rootCtrlCap;

    if ((retVal = Pcie_writeRegs (handle, loc, &regs)) != pcie_RET_OK)
    {
        PCIE_logPrintf ("SET rootCtrlCap register failed!\n");
        return retVal;
    }
    return retVal;
}

pcieRet_e pcieSetreg(Pcie_Handle handle, pcieLocation_e loc)
{
<<<<<<< HEAD
<<<<<<< HEAD
    pcieRet_e           retVal;
    pcieMsiLo32Reg_t    msiLo32;
    pcieRegisters_t     regs;

    memset (&msiLo32, 0, sizeof(msiLo32));
    memset (&regs, 0, sizeof(regs));
=======
    pcieRet_e               retVal;
    pcieMsiLo32Reg_t        msiLo32;
    pcieRegisters_t         regs;

    memset (&msiLo32, 0, sizeof(msiLo32));
    memset (&regs,    0, sizeof(regs));
>>>>>>> cc7d9980b (PDK-9352: Added PCIe SSD examples for AM65x)
=======
    pcieRet_e           retVal;
    pcieMsiLo32Reg_t    msiLo32;
    pcieRegisters_t     regs;

    memset (&msiLo32, 0, sizeof(msiLo32));
    memset (&regs, 0, sizeof(regs));
>>>>>>> ad03eb174 (Updated the files as per previous comments)

    regs.msiLo32 = &msiLo32;
    if ((retVal = Pcie_readRegs (handle, loc, &regs)) != pcie_RET_OK)
    {
        PCIE_logPrintf ("GET msiLo32 register failed!\n");
        return retVal;
    }
    msiLo32.addr    = 0x00400000;
    regs.msiLo32 = &msiLo32;

    if ((retVal = Pcie_writeRegs (handle, loc, &regs)) != pcie_RET_OK)
    {
        PCIE_logPrintf ("SET GEN2/link cap register failed!\n");
        return retVal;
    }

    return retVal;
}
/*****************************************************************************
 * Function: Configure PCIe in Gen2 mode
 ****************************************************************************/
pcieRet_e pcieSetGen2(Pcie_Handle handle, pcieLocation_e loc)
{
    pcieRet_e               retVal;

    pcieRegisters_t         regs;
    pcieLinkCapReg_t        linkCap;
    pcieLinkCtrl2Reg_t      linkCtrl2;
    pcieGen2Reg_t           gen2;

    uint8_t                 targetGen, dirSpd;

    targetGen = 2;
    dirSpd    = 1;

    memset (&gen2,             0, sizeof(gen2));
    memset (&linkCap,          0, sizeof(linkCap));
    memset (&linkCtrl2,        0, sizeof(linkCtrl2));
    memset (&regs,             0, sizeof(regs));

    regs.linkCap = &linkCap;
    if ((retVal = Pcie_readRegs (handle, pcie_LOCATION_LOCAL, &regs)) != pcie_RET_OK)
    {
        PCIE_logPrintf ("GET linkCap register failed!\n");
        return retVal;
    }
    PCIE_logPrintf ("PowerUP linkCap gen=%d change to %d\n", linkCap.maxLinkSpeed, targetGen);
    linkCap.maxLinkSpeed = targetGen;
    regs.linkCap = &linkCap;
    if ((retVal = Pcie_writeRegs (handle, loc, &regs)) != pcie_RET_OK)
    {
        PCIE_logPrintf ("SET linkCap register failed!\n");
        return retVal;
    }

    memset (&regs, 0, sizeof(regs));
    regs.linkCtrl2 = &linkCtrl2;
    if ((retVal = Pcie_readRegs (handle, pcie_LOCATION_LOCAL, &regs)) != pcie_RET_OK)
    {
        PCIE_logPrintf ("GET linkCtrl2 register failed!\n");
        return retVal;
    }
    PCIE_logPrintf ("PowerUP linkCtrl2 gen=%d change to %d\n", linkCtrl2.tgtSpeed, targetGen);
    linkCtrl2.tgtSpeed = targetGen;
    regs.linkCtrl2 = &linkCtrl2;
    if ((retVal = Pcie_writeRegs (handle, loc, &regs)) != pcie_RET_OK)
    {
        PCIE_logPrintf ("SET linkCtrl2 register failed!\n");
        return retVal;
    }

    /* Setting PL_GEN2 */
    memset (&regs, 0, sizeof(regs));
    regs.gen2 = &gen2;
    gen2.numFts = 0xF0;
    gen2.dirSpd = dirSpd;
    gen2.lnEn   = 2;
    gen2.autoFlipEn = 1;
    regs.gen2 = &gen2;

    if ((retVal = Pcie_writeRegs (handle, loc, &regs)) != pcie_RET_OK)
    {
        PCIE_logPrintf ("SET gen2 register failed!\n");
        return retVal;
    }

    return retVal;
}

/*****************************************************************************
 * Function: Configure PCIe in Root Complex Mode
 ****************************************************************************/
pcieRet_e pcieCfgRC(Pcie_Handle handle)
{
    pcieRet_e retVal;

    pcieObSizeReg_t           obSize;
    pcieType1Bar32bitIdx_t    type1Bar32bitIdx;
    pcieStatusCmdReg_t        statusCmd;
    pcieDevStatCtrlReg_t      devStatCtrl;
    pcieRootErrCmdReg_t       rootErrCmd;
    pcieLinkStatCtrlReg_t     linkStatCtrl;
    pcieMsiCapReg_t           msiCap;
    pcieType1BridgeIntReg_t   type1BridgeInt;
    pciePrefMemReg_t          prefMem;
    pcieType1MemspaceReg_t    type1Memspace;
    pcieType1SecStatReg_t     type1SecStat;
    pcieType1BusNumReg_t      type1BusNum;

    pcieRegisters_t           setRegs;
    pcieRegisters_t           getRegs;

    memset (&obSize,              0, sizeof(obSize));
    memset (&type1Bar32bitIdx,    0, sizeof(type1Bar32bitIdx));
    memset (&statusCmd,           0, sizeof(statusCmd));
<<<<<<< HEAD
<<<<<<< HEAD
    memset (&devStatCtrl,         0, sizeof(devStatCtrl));
=======
    memset (&devStatCtrl,         0, sizeof(devStatCtrl));  
>>>>>>> cc7d9980b (PDK-9352: Added PCIe SSD examples for AM65x)
=======
    memset (&devStatCtrl,         0, sizeof(devStatCtrl));
>>>>>>> f0102448a (PCIE SSD Example code)
    memset (&rootErrCmd,          0, sizeof(rootErrCmd));
    memset (&linkStatCtrl,        0, sizeof(linkStatCtrl));
    memset (&msiCap,              0, sizeof(msiCap));
    memset (&type1BridgeInt,      0, sizeof(type1BridgeInt));
    memset (&prefMem,             0, sizeof(prefMem));
    memset (&type1Memspace,       0, sizeof(type1Memspace));
    memset (&type1SecStat,        0, sizeof(type1SecStat));
    memset (&type1BusNum,         0, sizeof(type1BusNum));

<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> ad03eb174 (Updated the files as per previous comments)
    /*Disable link training*/
    if ((retVal = pcieLtssmCtrl(handle, FALSE, pcie_LOCATION_LOCAL)) != pcie_RET_OK)
    {
        PCIE_logPrintf ("Failed to disable Link Training!\n");
        return retVal;
    }
<<<<<<< HEAD

    /* Enable the IRQ bits */
    HW_WR_REG32(PCIE_EP_LEGACY_A_IRQ_ENABLE_SET_REG, 0x1);
    HW_WR_REG32(PCIE_EP_LEGACY_B_IRQ_ENABLE_SET_REG, 0x1);
    HW_WR_REG32(PCIE_EP_LEGACY_C_IRQ_ENABLE_SET_REG, 0x1);
    HW_WR_REG32(PCIE_EP_LEGACY_D_IRQ_ENABLE_SET_REG, 0x1);
    HW_WR_REG32(PCIE_EP_ERR_IRQ_ENABLE_SET_REG, 0x1F);
=======
  /*Disable link training*/
  if ((retVal = pcieLtssmCtrl(handle, FALSE, pcie_LOCATION_LOCAL)) != pcie_RET_OK)
  {
    PCIE_logPrintf ("Failed to disable Link Training!\n");
    return retVal;
  }
=======
>>>>>>> ad03eb174 (Updated the files as per previous comments)

<<<<<<< HEAD
    HW_WR_REG32(0x05500188, 0x1);
    HW_WR_REG32(0x05500198, 0x1);
    HW_WR_REG32(0x055001A8, 0x1);
    HW_WR_REG32(0x055001B8, 0x1);
    HW_WR_REG32(0x055001C8, 0x1F);
>>>>>>> cc7d9980b (PDK-9352: Added PCIe SSD examples for AM65x)
=======
    /* Enable the IRQ bits */
    HW_WR_REG32(PCIE_EP_LEGACY_A_IRQ_ENABLE_SET_REG, 0x1);
    HW_WR_REG32(PCIE_EP_LEGACY_B_IRQ_ENABLE_SET_REG, 0x1);
    HW_WR_REG32(PCIE_EP_LEGACY_C_IRQ_ENABLE_SET_REG, 0x1);
    HW_WR_REG32(PCIE_EP_LEGACY_D_IRQ_ENABLE_SET_REG, 0x1);
    HW_WR_REG32(PCIE_EP_ERR_IRQ_ENABLE_SET_REG, 0x1F);
>>>>>>> 3fda8cd5c (Updating as per the review comments)

    /* Configure the size of the translation regions */
    memset (&setRegs, 0, sizeof(setRegs));
    memset (&getRegs, 0, sizeof(getRegs));

    if ((retVal = pcieCfgDbiRWE (handle, 1, pcie_LOCATION_LOCAL)) != pcie_RET_OK)
    {
        return retVal;
    }
    /* Set gen2/link cap */
    if ((retVal = pcieSetGen2(handle, pcie_LOCATION_LOCAL)) != pcie_RET_OK)
    {
        PCIE_logPrintf ("pcieSetGen2 failed!\n");
        return retVal;
    }
    if ((retVal = pcieSetReg2(handle, pcie_LOCATION_LOCAL)) != pcie_RET_OK)
    {
        PCIE_logPrintf ("pcieSetReg2 failed!\n");
        return retVal;
    }
    if ((retVal = pcieCfgDbiRWE (handle, 0, pcie_LOCATION_LOCAL)) != pcie_RET_OK)
    {
        return retVal;
    }

    if ((retVal = pcieCfgDbi (handle, 1, pcie_LOCATION_LOCAL)) != pcie_RET_OK)
    {
        return retVal;
    }
    if ((retVal = pcieSetreg(handle, pcie_LOCATION_LOCAL)) != pcie_RET_OK)
    {
        PCIE_logPrintf ("pcieSetreg failed!\n");
        return retVal;
    }
    if ((retVal = pcieCfgDbi (handle, 0, pcie_LOCATION_LOCAL)) != pcie_RET_OK)
    {
        return retVal;
    }

    /* Enable memory access and mastership of the bus */
    memset (&setRegs, 0, sizeof(setRegs));
    memset (&getRegs, 0, sizeof(getRegs));

    getRegs.statusCmd = &statusCmd;
    if ((retVal = Pcie_readRegs (handle, pcie_LOCATION_LOCAL, &getRegs)) != pcie_RET_OK)
    {
        PCIE_logPrintf ("Read statusCmd failed!\n");
        return retVal;
    }
    statusCmd.memSp  = 1;
    statusCmd.busMs  = 1;
    statusCmd.serrEn = 1;
    statusCmd.ioSp   = 1;
    statusCmd.dis    = 1;
    setRegs.statusCmd = &statusCmd;

    if ((retVal = Pcie_writeRegs (handle, pcie_LOCATION_LOCAL, &setRegs)) != pcie_RET_OK)
    {
        PCIE_logPrintf ("SET statusCmd failed!\n");
        return retVal;
    }

    /* Enable Error Reporting */
    memset (&setRegs, 0, sizeof(setRegs));
    memset (&getRegs, 0, sizeof(getRegs));

    getRegs.devStatCtrl = &devStatCtrl;
    if ((retVal = Pcie_readRegs (handle, pcie_LOCATION_LOCAL, &getRegs)) != pcie_RET_OK)
    {
        PCIE_logPrintf ("Read devStatCtrl register failed!\n");
        return retVal;
    }

    devStatCtrl.maxSz = 0;
    devStatCtrl.reqRp = 1;
    devStatCtrl.fatalErRp = 1;
    devStatCtrl.nFatalErRp = 1;
    devStatCtrl.corErRp = 1;
    setRegs.devStatCtrl = &devStatCtrl;

    if ((retVal = Pcie_writeRegs (handle, pcie_LOCATION_LOCAL, &setRegs)) != pcie_RET_OK)
    {
        PCIE_logPrintf ("SET devStatCtrl register failed!\n");
        return retVal;
    }

    /*  */
    memset (&setRegs, 0, sizeof(setRegs));
    memset (&getRegs, 0, sizeof(getRegs));

    getRegs.type1BusNum = &type1BusNum;
    if ((retVal = Pcie_readRegs (handle, pcie_LOCATION_LOCAL, &getRegs)) != pcie_RET_OK)
    {
        PCIE_logPrintf ("Read type1BusNum register failed!\n");
        return retVal;
    }

    type1BusNum.subBusNum = 0xff;
    type1BusNum.secBusNum = 0x01;
    setRegs.type1BusNum = &type1BusNum;

    if ((retVal = Pcie_writeRegs (handle, pcie_LOCATION_LOCAL, &setRegs)) != pcie_RET_OK)
    {
        PCIE_logPrintf ("SET type1BusNum register failed!\n");
        return retVal;
    }

    /*  */
    memset (&setRegs, 0, sizeof(setRegs));
    memset (&getRegs, 0, sizeof(getRegs));

    getRegs.type1SecStat = &type1SecStat;
    if ((retVal = Pcie_readRegs (handle, pcie_LOCATION_LOCAL, &getRegs)) != pcie_RET_OK)
    {
        PCIE_logPrintf ("Read type1SecStat register failed!\n");
        return retVal;
    }

    type1SecStat.IOBase = 0xf;
    setRegs.type1SecStat = &type1SecStat;

    if ((retVal = Pcie_writeRegs (handle, pcie_LOCATION_LOCAL, &setRegs)) != pcie_RET_OK)
    {
        PCIE_logPrintf ("SET type1SecStat register failed!\n");
        return retVal;
    }

    /*  */
    memset (&setRegs, 0, sizeof(setRegs));
    memset (&getRegs, 0, sizeof(getRegs));

    getRegs.type1Memspace = &type1Memspace;
    if ((retVal = Pcie_readRegs (handle, pcie_LOCATION_LOCAL, &getRegs)) != pcie_RET_OK)
    {
        PCIE_logPrintf ("Read type1Memspace register failed!\n");
        return retVal;
    }

    type1Memspace.base = 0x101;
    type1Memspace.limit = 0x101;
    setRegs.type1Memspace = &type1Memspace;

    if ((retVal = Pcie_writeRegs (handle, pcie_LOCATION_LOCAL, &setRegs)) != pcie_RET_OK)
    {
        PCIE_logPrintf ("SET type1Memspace register failed!\n");
        return retVal;
    }

    /*  */
    memset (&setRegs, 0, sizeof(setRegs));
    memset (&getRegs, 0, sizeof(getRegs));

    getRegs.prefMem = &prefMem;
    if ((retVal = Pcie_readRegs (handle, pcie_LOCATION_LOCAL, &getRegs)) != pcie_RET_OK)
    {
        PCIE_logPrintf ("Read prefMem register failed!\n");
        return retVal;
    }

    prefMem.base = 0xfff;
    setRegs.prefMem = &prefMem;

    if ((retVal = Pcie_writeRegs (handle, pcie_LOCATION_LOCAL, &setRegs)) != pcie_RET_OK)
    {
        PCIE_logPrintf ("SET prefMem register failed!\n");
        return retVal;
    }

    /*  */
    memset (&setRegs, 0, sizeof(setRegs));
    memset (&getRegs, 0, sizeof(getRegs));

    getRegs.type1BridgeInt = &type1BridgeInt;
    if ((retVal = Pcie_readRegs (handle, pcie_LOCATION_LOCAL, &getRegs)) != pcie_RET_OK)
    {
        PCIE_logPrintf ("Read type1BridgeInt register failed!\n");
        return retVal;
    }

    type1BridgeInt.serrEn = 1;
    type1BridgeInt.intPin = 0x01;
    type1BridgeInt.intLine = 0x26;
    setRegs.type1BridgeInt = &type1BridgeInt;

    if ((retVal = Pcie_writeRegs (handle, pcie_LOCATION_LOCAL, &setRegs)) != pcie_RET_OK)
    {
        PCIE_logPrintf ("SET type1BridgeInt register failed!\n");
        return retVal;
    }

    /* PCI_MSI_CAP_ID_NEXT_CTRL */
    memset (&setRegs, 0, sizeof(setRegs));
    memset (&getRegs, 0, sizeof(getRegs));

    getRegs.msiCap = &msiCap;
    if ((retVal = Pcie_readRegs (handle, pcie_LOCATION_LOCAL, &getRegs)) != pcie_RET_OK)
    {
        PCIE_logPrintf ("Read msiCap register failed!\n");
        return retVal;
    }

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
    /*msiCap.msiEn     = 1; */
    msiCap.msiEn     = 0;
=======
    msiCap.msiEn     = 1;
>>>>>>> cc7d9980b (PDK-9352: Added PCIe SSD examples for AM65x)
=======
    //msiCap.msiEn     = 1;
=======
    /*msiCap.msiEn     = 1; */
>>>>>>> ad03eb174 (Updated the files as per previous comments)
    msiCap.msiEn     = 0;
>>>>>>> f0102448a (PCIE SSD Example code)
    setRegs.msiCap = &msiCap;

    if ((retVal = Pcie_writeRegs (handle, pcie_LOCATION_LOCAL, &setRegs)) != pcie_RET_OK)
    {
        PCIE_logPrintf ("SET msiCap register failed!\n");
        return retVal;
    }

    /* ROOT_ERR_CMD */
    memset (&setRegs, 0, sizeof(setRegs));
    memset (&getRegs, 0, sizeof(getRegs));

    getRegs.rootErrCmd = &rootErrCmd;
    if ((retVal = Pcie_readRegs (handle, pcie_LOCATION_LOCAL, &getRegs)) != pcie_RET_OK)
    {
        PCIE_logPrintf ("Read rootErrCmd register failed!\n");
        return retVal;
    }

    rootErrCmd.ferrRptEn     = 1;
    rootErrCmd.nferrRptEn    = 1;
    rootErrCmd.cerrRptEn     = 1;
    setRegs.rootErrCmd = &rootErrCmd;

    if ((retVal = Pcie_writeRegs (handle, pcie_LOCATION_LOCAL, &setRegs)) != pcie_RET_OK)
    {
        PCIE_logPrintf ("SET rootErrCmd register failed!\n");
        return retVal;
    }

    /* link control */
    memset (&setRegs, 0, sizeof(setRegs));
    memset (&getRegs, 0, sizeof(getRegs));

    getRegs.linkStatCtrl = &linkStatCtrl;
    if ((retVal = Pcie_readRegs (handle, pcie_LOCATION_LOCAL, &getRegs)) != pcie_RET_OK)
    {
        PCIE_logPrintf ("Read linkStatCtrl register failed!\n");
        return retVal;
    }

    linkStatCtrl.commonClkCfg     = 1;
    setRegs.linkStatCtrl = &linkStatCtrl;

    if ((retVal = Pcie_writeRegs (handle, pcie_LOCATION_LOCAL, &setRegs)) != pcie_RET_OK)
    {
        PCIE_logPrintf ("SET linkStatCtrl register failed!\n");
        return retVal;
    }

    return pcie_RET_OK;
}

/*****************************************************************************
 * Function: Configure PCIe in End Point Mode
 ****************************************************************************/
pcieRet_e pcieCfgEP(Pcie_Handle handle)
{
<<<<<<< HEAD
<<<<<<< HEAD
    pcieRet_e               retVal;
    pcieRegisters_t         getRegs;
    pcieRegisters_t         setRegs;
    pcieVndDevIdReg_t       vndDevId;

    pcieIntPinReg_t         intPin;
    pcieStatusCmdReg_t      statusCmd;
    pcieBarCfg_t            barCfg;
=======
    pcieRet_e               retVal;
    pcieRegisters_t         getRegs;
    pcieRegisters_t         setRegs;
    pcieVndDevIdReg_t       vndDevId;

<<<<<<< HEAD
	pcieIntPinReg_t       	intPin;
	pcieStatusCmdReg_t    	statusCmd;
	pcieBarCfg_t     		barCfg;
>>>>>>> f0102448a (PCIE SSD Example code)
=======
    pcieIntPinReg_t         intPin;
    pcieStatusCmdReg_t      statusCmd;
    pcieBarCfg_t            barCfg;
>>>>>>> ad03eb174 (Updated the files as per previous comments)

    /*reading vendor_id and device_id*/
    memset (&getRegs,   0, sizeof(getRegs));
    memset (&vndDevId,  0, sizeof(vndDevId));

    getRegs.vndDevId = &vndDevId;

    if ((retVal = Pcie_readRegs (handle, pcie_LOCATION_REMOTE, &getRegs)) != pcie_RET_OK)
    {
        PCIE_logPrintf ("Failed to read EP VendorId and DeviceId !\n");
        return retVal;
    }
    PCIE_logPrintf ("Raw 0x%x\nVendor ID 0x%x\nDevice ID 0x%x\n", vndDevId.raw, vndDevId.vndId,vndDevId.devId);

    /* Set EP statusCmd */
    memset (&setRegs,   0, sizeof(setRegs));
    memset (&statusCmd, 0, sizeof(statusCmd));
    memset (&intPin,    0, sizeof(intPin));

    statusCmd.dis    = 1;
    statusCmd.busMs  = 1;
    statusCmd.memSp  = 1;
    setRegs.statusCmd = &statusCmd;

    /* Set EP Interrupt Information */
    intPin.intPin = 1;
    intPin.intLine = 0x26;
    setRegs.intPin = &intPin;

    if ((retVal = Pcie_writeRegs (handle, pcie_LOCATION_REMOTE, &setRegs)) != pcie_RET_OK)
    {
        PCIE_logPrintf ("Set EP statusCmd failed!\n");
        return retVal;
    }

    /* Configure EP */
    barCfg.location = pcie_LOCATION_REMOTE;
    barCfg.mode     = pcie_EP_MODE;
<<<<<<< HEAD
<<<<<<< HEAD
=======
    //barCfg.base     = 0x10100004;
>>>>>>> f0102448a (PCIE SSD Example code)
=======
>>>>>>> ad03eb174 (Updated the files as per previous comments)
    barCfg.base     = EP_CONFIG_BASE;
    barCfg.prefetch = pcie_BAR_NON_PREF;
    barCfg.type     = pcie_BAR_TYPE32;
    barCfg.memSpace = pcie_BAR_MEM_MEM;
    barCfg.idx      = 0;

    if ((retVal = Pcie_cfgBar(handle, &barCfg)) != pcie_RET_OK)
    {
        PCIE_logPrintf ("Failed to configure EP BAR0\n");
        return retVal;
    }

<<<<<<< HEAD
<<<<<<< HEAD
=======
    uint32_t temp;

    temp = HW_RD_REG32(0x10000000);
    PCIE_logPrintf ("temp -  0x%x\n", temp);

    /* PCIE_HEADER */
    HW_WR_REG32(0x10000004, 0x00100406);
    HW_WR_REG32(0x1000003c, 0x00000126);
    HW_WR_REG32(0x10000010, 0x10100004);

    
    cycleDelay(1000);

    /* CONFIGURATION_HEADER */
    HW_WR_REG32(0x10100024, 0x001f001f);
    HW_WR_REG32(0x10100028, 0xc7e80000);
    HW_WR_REG32(0x1010002c, 0x00000008);
    HW_WR_REG32(0x10100030, 0xc8b60000);
    HW_WR_REG32(0x10100034, 0x00000008);
    HW_WR_REG32(0x10101000, 0x0013);
    HW_WR_REG32(0x10101004, 0x0013);
    HW_WR_REG32(0x10101008, 0x006F);
    HW_WR_REG32(0x1010100C, 0x006F);
    HW_WR_REG32(0x10101010, 0x009C);
    HW_WR_REG32(0x10101014, 0x009C);
    HW_WR_REG32(0x10101018, 0x006C);
    HW_WR_REG32(0x1010101C, 0x006C);
    HW_WR_REG32(0x10101020, 0x0010);
    HW_WR_REG32(0x10101024, 0x0010);
    HW_WR_REG32(0x10100014, 0x00460001);
>>>>>>> cc7d9980b (PDK-9352: Added PCIe SSD examples for AM65x)
=======
/*HW_WR_REG32(0x10100024, 0x001f001f);
HW_WR_REG32(0x10100028, 0xc7e80000);
HW_WR_REG32(0x1010002c, 0x00000008);
HW_WR_REG32(0x10100030, 0xc8b60000);
HW_WR_REG32(0x10100034, 0x00000008);
HW_WR_REG32(0x10100014, 0x00460001);*/
>>>>>>> f0102448a (PCIE SSD Example code)
=======
>>>>>>> ad03eb174 (Updated the files as per previous comments)
    return pcie_RET_OK;
}

/*****************************************************************************
 * Function: Configure and enable Outbound Address Translation for rev 1/2
 ****************************************************************************/
pcieRet_e pcieObTransCfg(Pcie_Handle handle, uint32_t obAddrLo, uint32_t obAddrHi, pcieLocation_e loc, pcieMode_e PModeGbl)
{
<<<<<<< HEAD
<<<<<<< HEAD
    uint32_t              resSize;
    pcieAtuRegionParams_t regionParams;
    pcieRet_e             retVal;
=======
    pcieAtuRegionParams_t regionParams;
    pcieRet_e             retVal;
    uint32_t              resSize;
>>>>>>> cc7d9980b (PDK-9352: Added PCIe SSD examples for AM65x)
=======
    uint32_t              resSize;
    pcieAtuRegionParams_t regionParams;
    pcieRet_e             retVal;
>>>>>>> f0102448a (PCIE SSD Example code)

    memset (&regionParams, 0, sizeof(regionParams));

    if ((retVal = Pcie_getMemSpaceReserved (handle, &resSize)) != pcie_RET_OK)
    {
        PCIE_logPrintf ("getMemSpaceReserved failed (%d)\n", (int)retVal);
        return retVal;
    }

<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> f0102448a (PCIE SSD Example code)
    /* Configure OB region for remote configuration access space */
    regionParams.regionDir    = PCIE_ATU_REGION_DIR_OUTBOUND;
    regionParams.tlpType      = PCIE_TLP_TYPE_CFG;
    regionParams.enableRegion = 1;
<<<<<<< HEAD

    regionParams.lowerBaseAddr = PCIE_WINDOW_CFG_BASE + resSize;

    /* only 32 bits needed given data area size */
    regionParams.upperBaseAddr    = 0;
    regionParams.regionWindowSize = PCIE_WINDOW_CFG_MASK;

    regionParams.lowerTargetAddr = 0U;
    regionParams.upperTargetAddr = 0U;

    if ( (retVal = Pcie_atuRegionConfig(handle, loc, (uint32_t) 0U, &regionParams)) != pcie_RET_OK)
    {
        return retVal;
    }

    /* Configure OB region for memory transfer */
=======
    if(PModeGbl  == pcie_RC_MODE)
    {
        //Configure OB region for configuration transfer
        regionParams.regionDir    = PCIE_ATU_REGION_DIR_OUTBOUND;
        regionParams.tlpType      = PCIE_TLP_TYPE_CFG;
        regionParams.enableRegion = 1;
=======
>>>>>>> f0102448a (PCIE SSD Example code)

    regionParams.lowerBaseAddr = PCIE_WINDOW_CFG_BASE + resSize;

    /* only 32 bits needed given data area size */
    regionParams.upperBaseAddr    = 0;
    regionParams.regionWindowSize = PCIE_WINDOW_CFG_MASK;

    regionParams.lowerTargetAddr = 0U;
    regionParams.upperTargetAddr = 0U;

    if ( (retVal = Pcie_atuRegionConfig(handle, loc, (uint32_t) 0U, &regionParams)) != pcie_RET_OK)
    {
        return retVal;
    }

<<<<<<< HEAD
    //Configure OB region for remote memory access space
>>>>>>> cc7d9980b (PDK-9352: Added PCIe SSD examples for AM65x)
=======
    /* Configure OB region for memory transfer */
>>>>>>> f0102448a (PCIE SSD Example code)
    regionParams.regionDir    = PCIE_ATU_REGION_DIR_OUTBOUND;
    regionParams.tlpType      = PCIE_TLP_TYPE_MEM;
    regionParams.enableRegion = 1;

    regionParams.lowerBaseAddr    = PCIE_WINDOW_MEM_BASE + resSize;
<<<<<<< HEAD
<<<<<<< HEAD
    regionParams.upperBaseAddr    = 0; /* only 32 bits needed given data area size */
=======
    regionParams.upperBaseAddr    = 0; //only 32 bits needed given data area size
>>>>>>> cc7d9980b (PDK-9352: Added PCIe SSD examples for AM65x)
=======
    regionParams.upperBaseAddr    = 0; /* only 32 bits needed given data area size */
>>>>>>> f0102448a (PCIE SSD Example code)
    regionParams.regionWindowSize = PCIE_WINDOW_MEM_MASK;

    regionParams.lowerTargetAddr = obAddrLo;
    regionParams.upperTargetAddr = obAddrHi;

<<<<<<< HEAD
<<<<<<< HEAD
    return Pcie_atuRegionConfig(handle, loc, (uint32_t) 1U, &regionParams);
=======
    if ( (retVal = Pcie_atuRegionConfig( handle, loc, (uint32_t) 0U, &regionParams)) != pcie_RET_OK)
    {
        return retVal;
    }
    HW_WR_REG32(0x05506010, 0x17ffffff);

    //Configure OB region for __________ transfer
    regionParams.regionDir    = PCIE_ATU_REGION_DIR_OUTBOUND;
    regionParams.tlpType      = PCIE_TLP_TYPE_IO;
    regionParams.enableRegion = 1;

    regionParams.lowerBaseAddr    = PCIE_WINDOW_MSI_ADDR + resSize;
    regionParams.upperBaseAddr    = 0; // only 32 bits needed given data area size 
    regionParams.regionWindowSize = PCIE_WINDOW_MSI_MASK;

    regionParams.lowerTargetAddr = 0U;
    regionParams.upperTargetAddr = 0U;

    if ( (Pcie_atuRegionConfig( handle, loc, (uint32_t) 2U, &regionParams)) != pcie_RET_OK)
    {
        return retVal;
    }
    HW_WR_REG32(0x05506410, 0x1002ffff);
    return retVal;
>>>>>>> cc7d9980b (PDK-9352: Added PCIe SSD examples for AM65x)
=======
    return Pcie_atuRegionConfig(handle, loc, (uint32_t) 1U, &regionParams);
>>>>>>> f0102448a (PCIE SSD Example code)
}

pcieRet_e pcieIbTransCfg(Pcie_Handle handle, pcieIbTransCfg_t *ibCfg, pcieLocation_e loc)
{
    pcieAtuRegionParams_t   regionParams;
    pcieRet_e               retVal = pcie_RET_OK;

    memset (&regionParams, 0, sizeof(regionParams));

    /* Configure IB region for memory transfer */
    regionParams.regionDir    = PCIE_ATU_REGION_DIR_INBOUND;
    regionParams.tlpType      = PCIE_TLP_TYPE_MEM;
    regionParams.enableRegion = 1;
    regionParams.matchMode    = PCIE_ATU_REGION_MATCH_MODE_ADDR;

    regionParams.lowerBaseAddr    = ibCfg->ibStartAddrLo;
    regionParams.upperBaseAddr    = ibCfg->ibStartAddrHi;
    regionParams.regionWindowSize = PCIE_INBOUND_MASK;

    /* This aligns the buffer to 4K, which needs to be compensated by the application */
    regionParams.lowerTargetAddr = (ibCfg->ibOffsetAddr & ~0xfffU) ;
    regionParams.upperTargetAddr = 0;

    if ( (retVal = Pcie_atuRegionConfig(handle, loc, (uint32_t) 0U, &regionParams)) != pcie_RET_OK)
    {
        return retVal;
    }

    /*Configure IB region for MSI receive */
    regionParams.regionDir    = PCIE_ATU_REGION_DIR_INBOUND;
    regionParams.tlpType      = PCIE_TLP_TYPE_MEM;
    regionParams.enableRegion = 1;
    regionParams.matchMode    = PCIE_ATU_REGION_MATCH_MODE_ADDR;

    regionParams.lowerBaseAddr    = PCIE_PCIE_MSI_BASE;
    regionParams.upperBaseAddr    = 0U;
    regionParams.regionWindowSize = PCIE_WINDOW_MSI_MASK;

    /* Point at GICD_SETSPI_NSR in ARM GIC to directly trigger a SPI */
    regionParams.lowerTargetAddr = ((CSL_GIC0_DISTRIBUTOR_BASE + CSL_GIC500_MSG_SETSPI_NSR) -  PCIE_PCIE_MSI_OFF);
    regionParams.upperTargetAddr = 0;

    retVal = Pcie_atuRegionConfig(handle, loc, (uint32_t) 1U, &regionParams);

    return retVal;
}

/*****************************************************************************
 * Function: Initialize application buffers
 ****************************************************************************/
void pcieInitAppBuf(void)
{
<<<<<<< HEAD
<<<<<<< HEAD
    uint32_t i;

    for (i=0; i<PCIE_BUFSIZE_APP; i++)
    {
    dstBuf.buf[i] = 0;
    srcBuf[i] = i;
    }

    dstBuf.buf[PCIE_BUFSIZE_APP] = PCIE_EXAMPLE_BUF_EMPTY;
    cache_writeback ((void *)dstBuf.buf, PCIE_EXAMPLE_DSTBUF_BYTES);

#if defined(EDMA)||defined(UDMA)
    for (i = 0; i < PCIE_EXAMPLE_LINE_SIZE - 1U; i++)
    {
        dstBuf.edma_buf[i] = 0;
    }
    dstBuf.edma_buf[PCIE_EXAMPLE_LINE_SIZE - 1U] = PCIE_EXAMPLE_BUF_EMPTY;
    cache_writeback ((void *)dstBuf.edma_buf, PCIE_EDMA_EXAMPLE_DSTBUF_BYTES);
=======
  uint32_t i;
=======
    uint32_t i;
>>>>>>> ad03eb174 (Updated the files as per previous comments)

    for (i=0; i<PCIE_BUFSIZE_APP; i++)
    {
    dstBuf.buf[i] = 0;
    srcBuf[i] = i;
    }

    dstBuf.buf[PCIE_BUFSIZE_APP] = PCIE_EXAMPLE_BUF_EMPTY;
    cache_writeback ((void *)dstBuf.buf, PCIE_EXAMPLE_DSTBUF_BYTES);

#if defined(EDMA)||defined(UDMA)
<<<<<<< HEAD
  for (i = 0; i < PCIE_EXAMPLE_LINE_SIZE - 1U; i++)
  {
    dstBuf.edma_buf[i] = 0;
  }
  dstBuf.edma_buf[PCIE_EXAMPLE_LINE_SIZE - 1U] = PCIE_EXAMPLE_BUF_EMPTY;
  cache_writeback ((void *)dstBuf.edma_buf, PCIE_EDMA_EXAMPLE_DSTBUF_BYTES);
>>>>>>> cc7d9980b (PDK-9352: Added PCIe SSD examples for AM65x)
=======
    for (i = 0; i < PCIE_EXAMPLE_LINE_SIZE - 1U; i++)
    {
        dstBuf.edma_buf[i] = 0;
    }
    dstBuf.edma_buf[PCIE_EXAMPLE_LINE_SIZE - 1U] = PCIE_EXAMPLE_BUF_EMPTY;
    cache_writeback ((void *)dstBuf.edma_buf, PCIE_EDMA_EXAMPLE_DSTBUF_BYTES);
>>>>>>> ad03eb174 (Updated the files as per previous comments)
#endif
}

/*****************************************************************************
 * Function: Check LTSSM status and wait for the link to be up
 ****************************************************************************/
void pcieWaitLinkUp(Pcie_Handle handle, pcieLocation_e loc)
{
<<<<<<< HEAD
<<<<<<< HEAD
    pcieRegisters_t  getRegs;

    memset (&getRegs, 0, sizeof(getRegs));

#if defined(PCIE_REV0_HW) || defined(PCIE_REV2_HW)
    pcieDebug0Reg_t            ltssmStateReg;
    getRegs.debug0 =          &ltssmStateReg;
#else
    pcieTiConfDeviceCmdReg_t   ltssmStateReg;
    getRegs.tiConfDeviceCmd = &ltssmStateReg;
#endif

    memset (&ltssmStateReg,  0, sizeof(ltssmStateReg));

    uint8_t ltssmState = 0;

#if defined(SOC_J721E)
    while(ltssmState != (pcie_LTSSM_L0-1)) /* For J721E 0x10 is L0 state */
#else
    while(ltssmState != pcie_LTSSM_L0)
#endif
    {
        cycleDelay(100);
        if (Pcie_readRegs (handle, pcie_LOCATION_LOCAL, &getRegs) != pcie_RET_OK)
        {
            PCIE_logPrintf ("Read LTSSM state failed!\n");
            return;
        }
        ltssmState = ltssmStateReg.ltssmState;
    }
=======
  pcieRegisters_t  getRegs;
=======
    pcieRegisters_t  getRegs;
>>>>>>> ad03eb174 (Updated the files as per previous comments)

    memset (&getRegs, 0, sizeof(getRegs));

#if defined(PCIE_REV0_HW) || defined(PCIE_REV2_HW)
    pcieDebug0Reg_t            ltssmStateReg;
    getRegs.debug0 =          &ltssmStateReg;
#else
    pcieTiConfDeviceCmdReg_t   ltssmStateReg;
    getRegs.tiConfDeviceCmd = &ltssmStateReg;
#endif

    memset (&ltssmStateReg,  0, sizeof(ltssmStateReg));

    uint8_t ltssmState = 0;

#if defined(SOC_J721E)
    while(ltssmState != (pcie_LTSSM_L0-1)) /* For J721E 0x10 is L0 state */
#else
    while(ltssmState != pcie_LTSSM_L0)
#endif
    {
        cycleDelay(100);
        if (Pcie_readRegs (handle, pcie_LOCATION_LOCAL, &getRegs) != pcie_RET_OK)
        {
            PCIE_logPrintf ("Read LTSSM state failed!\n");
            return;
        }
        ltssmState = ltssmStateReg.ltssmState;
    }
<<<<<<< HEAD
    ltssmState = ltssmStateReg.ltssmState;
  }
>>>>>>> cc7d9980b (PDK-9352: Added PCIe SSD examples for AM65x)
=======
>>>>>>> ad03eb174 (Updated the files as per previous comments)
}

void pcieSetLanes (Pcie_Handle handle, pcieLocation_e loc)
{
#ifdef PCIE_REV2_HW
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> ad03eb174 (Updated the files as per previous comments)
    /* Request just 1 lane -- this would work on all HW */
    pcieLnkCtrlReg_t lnkCtrlReg;
    pcieRegisters_t  regs;
    uint8_t          origLanes;

    memset (&regs, 0, sizeof(regs));
    regs.lnkCtrl = &lnkCtrlReg;
    if (Pcie_readRegs (handle, loc, &regs) != pcie_RET_OK)
    {
<<<<<<< HEAD
    PCIE_logPrintf ("Read LnkCtrl register failed!\n");
    exit(1);
    }
    origLanes = lnkCtrlReg.lnkMode;
#ifdef am65xx_idk
#ifdef PCIE0_SERDES0
    lnkCtrlReg.lnkMode = 1; /* bitfield enabling one lane case */
#else
    lnkCtrlReg.lnkMode = 3; /* bitfield enabling both lanes */
#endif
#else
    lnkCtrlReg.lnkMode = 1;
#endif
    if (Pcie_writeRegs (handle, loc, &regs) != pcie_RET_OK)
    {
    PCIE_logPrintf ("Write LnkCtrl register failed!\n");
    exit(1);
    }
    PCIE_logPrintf ("Set lanes from %d to %d\n", (int)origLanes, (int)lnkCtrlReg.lnkMode);
#endif

#ifdef PCIE_REV3_HW
    /* Request just 1 lane -- this would work on all HW */
    pcieLnkCtrlReg_t lnkCtrlReg;
    pcieRegisters_t regs;
    uint8_t origLanes;

    memset (&regs, 0, sizeof(regs));
    regs.lnkCtrl = &lnkCtrlReg;
    if (Pcie_readRegs (handle, loc, &regs) != pcie_RET_OK)
    {
        PCIE_logPrintf ("Read pcieCtrlAddr register failed!\n");
        exit(1);
    }
    origLanes = lnkCtrlReg.lnkMode;
    lnkCtrlReg.lnkMode = 2-1; /* number of lanes-1, 2 lane */
    if (origLanes!=lnkCtrlReg.lnkMode)
    {
        if (Pcie_writeRegs (handle, loc, &regs) != pcie_RET_OK)
        {
            PCIE_logPrintf ("Write pcieCtrlAddr register failed!\n");
            exit(1);
        }
        PCIE_logPrintf ("Set lanes from %d to %d\n", (int)origLanes+1, (int)lnkCtrlReg.lnkMode+1);
    }
#endif
}

void createFolder(const TCHAR* path)
{
    FRESULT fr;

    /*Create folder/directory */
    fr = f_mkdir(path);
    if(fr == FR_OK){
        PCIE_logPrintf("Created Directory %s \n", path);
    }
    else
    {
        PCIE_logPrintf("Failed to create %s \n", path);
    }
}

void pcie (void)
{
    int32_t         deviceNum = 0;
    pcieRet_e       retVal;
    pcieIbTransCfg_t ibCfg;
    Pcie_Handle     handle = NULL;
    NVME_ERR        err;
=======
  /* Request just 1 lane -- this would work on all HW */
  pcieLnkCtrlReg_t lnkCtrlReg;
  pcieRegisters_t  regs;
  uint8_t          origLanes;

  memset (&regs, 0, sizeof(regs));
  regs.lnkCtrl = &lnkCtrlReg;
  if (Pcie_readRegs (handle, loc, &regs) != pcie_RET_OK)
  {
=======
>>>>>>> ad03eb174 (Updated the files as per previous comments)
    PCIE_logPrintf ("Read LnkCtrl register failed!\n");
    exit(1);
    }
    origLanes = lnkCtrlReg.lnkMode;
#ifdef am65xx_idk
#ifdef PCIE0_SERDES0
    lnkCtrlReg.lnkMode = 1; /* bitfield enabling one lane case */
#else
    lnkCtrlReg.lnkMode = 3; /* bitfield enabling both lanes */
#endif
#else
    lnkCtrlReg.lnkMode = 1;
#endif
    if (Pcie_writeRegs (handle, loc, &regs) != pcie_RET_OK)
    {
    PCIE_logPrintf ("Write LnkCtrl register failed!\n");
    exit(1);
    }
    PCIE_logPrintf ("Set lanes from %d to %d\n", (int)origLanes, (int)lnkCtrlReg.lnkMode);
#endif

#ifdef PCIE_REV3_HW
    /* Request just 1 lane -- this would work on all HW */
    pcieLnkCtrlReg_t lnkCtrlReg;
    pcieRegisters_t regs;
    uint8_t origLanes;

    memset (&regs, 0, sizeof(regs));
    regs.lnkCtrl = &lnkCtrlReg;
    if (Pcie_readRegs (handle, loc, &regs) != pcie_RET_OK)
    {
        PCIE_logPrintf ("Read pcieCtrlAddr register failed!\n");
        exit(1);
    }
    origLanes = lnkCtrlReg.lnkMode;
    lnkCtrlReg.lnkMode = 2-1; /* number of lanes-1, 2 lane */
    if (origLanes!=lnkCtrlReg.lnkMode)
    {
        if (Pcie_writeRegs (handle, loc, &regs) != pcie_RET_OK)
        {
            PCIE_logPrintf ("Write pcieCtrlAddr register failed!\n");
            exit(1);
        }
        PCIE_logPrintf ("Set lanes from %d to %d\n", (int)origLanes+1, (int)lnkCtrlReg.lnkMode+1);
    }
#endif
}

void createFolder(const TCHAR* path)
{
    FRESULT fr;

    /*Create folder/directory */
    fr = f_mkdir(path);
    if(fr == FR_OK){
        PCIE_logPrintf("Created Directory %s \n", path);
    }
    else
    {
        PCIE_logPrintf("Failed to create %s \n", path);
    }
}

void pcie (void)
{
    int32_t         deviceNum = 0;
    pcieRet_e       retVal;
    pcieIbTransCfg_t ibCfg;
    Pcie_Handle     handle = NULL;
<<<<<<< HEAD
  void            *pcieBase;
  dstBuf_t        *pciedstBufBase;
>>>>>>> cc7d9980b (PDK-9352: Added PCIe SSD examples for AM65x)
=======
    NVME_ERR        err;
>>>>>>> f0102448a (PCIE SSD Example code)

    /* Get remote buffer out of cache */
    cache_writeback ((void *)&dstBuf, sizeof(dstBuf));

    PCIE_logPrintf ("**********************************************\n");
    PCIE_logPrintf ("*             PCIe Test Start                *\n");
    PCIE_logPrintf ("**********************************************\n\n");

    PCIE_logPrintf ("Version #: 0x%08x;\nstring %s\n\n", (unsigned)Pcie_getVersion(), Pcie_getVersionStr());

    /* Pass device config to LLD */
    if ((retVal = Pcie_init (&pcieInitCfg)) != pcie_RET_OK)
    {
        PCIE_logPrintf ("LLD device configuration failed\n");
        exit(1);
    }

    /* Initialize application buffers */
    pcieInitAppBuf();

    if ((retVal = Pcie_open(deviceNum, &handle)) != pcie_RET_OK)
    {
        PCIE_logPrintf ("Open failed (%d)\n", (int)retVal);
        exit(1);
    }

    /* Configure SERDES*/
    if ((retVal = pcieSerdesCfg()) != pcie_RET_OK)
    {
        PCIE_logPrintf ("PCIe Serdes config failed (%d)\n", (int)retVal);
        exit(1);
    }

    PCIE_logPrintf ("Configuring pcie_RC_MODE.\n");
    /* Set the PCIe mode*/
    if ((retVal = Pcie_setInterfaceMode(handle, pcie_RC_MODE)) != pcie_RET_OK)
    {
        PCIE_logPrintf ("Set PCIe Mode failed (%d)\n", (int)retVal);
        exit(1);
    }

    /* Configure application registers for Root Complex*/
    if ((retVal = pcieCfgRC(handle)) != pcie_RET_OK)
    {
    PCIE_logPrintf ("Failed to configure PCIe in RC mode (%d)\n", (int)retVal);
    exit(1);
    }

<<<<<<< HEAD
<<<<<<< HEAD
    ibCfg.ibBar         = PCIE_BAR_IDX_RC;
=======
    /*ibCfg.ibBar         = PCIE_BAR_IDX_RC;
>>>>>>> cc7d9980b (PDK-9352: Added PCIe SSD examples for AM65x)
=======
    ibCfg.ibBar         = PCIE_BAR_IDX_RC;
>>>>>>> f0102448a (PCIE SSD Example code)
    ibCfg.ibStartAddrLo = PCIE_IB_LO_ADDR_RC;
    ibCfg.ibStartAddrHi = PCIE_IB_HI_ADDR_RC;
    ibCfg.ibOffsetAddr  = (uint32_t)pcieConvert_CoreLocal2GlobalAddr ((uintptr_t)dstBuf.buf);
    ibCfg.region        = PCIE_IB_REGION_RC;

    if ((retVal = pcieIbTransCfg(handle, &ibCfg, pcie_LOCATION_LOCAL)) != pcie_RET_OK)
    {
        PCIE_logPrintf ("Failed to configure Inbound Translation (%d)\n", (int)retVal);
        exit(1);
    }
    else
    {
        PCIE_logPrintf ("Successfully configured Inbound Translation!\n");
<<<<<<< HEAD
<<<<<<< HEAD
    }
=======
    }*/
>>>>>>> cc7d9980b (PDK-9352: Added PCIe SSD examples for AM65x)
=======
    }
>>>>>>> f0102448a (PCIE SSD Example code)

    if ((retVal = pcieObTransCfg (handle, PCIE_OB_LO_ADDR_RC, PCIE_OB_HI_ADDR_RC, pcie_LOCATION_LOCAL, pcie_RC_MODE)) != pcie_RET_OK)
    {
        PCIE_logPrintf ("Failed to configure Outbound Address Translation (%d)\n", (int)retVal);
        exit(1);
    }
    else
    {
        PCIE_logPrintf ("Successfully configured Outbound Translation!\n");
    }
    /* Configure/limit number of lanes */
    pcieSetLanes (handle, pcie_LOCATION_LOCAL);
    PCIE_logPrintf ("Starting link training...\n");

    /*Enable link training*/
    if ((retVal = pcieLtssmCtrl(handle, TRUE, pcie_LOCATION_LOCAL)) != pcie_RET_OK)
    {
        PCIE_logPrintf ("Failed to Enable Link Training! (%d)\n", (int)retVal);
        exit(1);
    }

    pcieWaitLinkUp(handle,pcie_LOCATION_LOCAL);
    PCIE_logPrintf ("Link is up.\n");

    PCIE_logPrintf ("\nConfiguring pcie_EP_MODE.\n");
    /* Configure application registers for End Point*/
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> f0102448a (PCIE SSD Example code)
    if ((retVal = pcieCfgEP(handle)) != pcie_RET_OK)
    {
        PCIE_logPrintf ("Failed to configure BAR registers (%d)\n", (int)retVal);
        exit(1);
    }
<<<<<<< HEAD

    err = nvme_init();
    if(err != NVME_OK)
    {
        PCIE_logPrintf("Failed to initialize nvme\n");
        exit(1);
    }

    /* Initialize the file system. */
#if defined(ENABLE_FATFS)
    FATFS_init();
    PCIE_logPrintf("FATFS INIT Done\n");
    FATFS_open(0U, NULL, &fatfsHandle);
    PCIE_logPrintf("FATFS OPEN DONE\n");
    FSShellAppUtilsProcess();
#endif

#if defined (USE_BIOS)
    OS_stop();
#endif

=======
    NVME_init();
=======
>>>>>>> f0102448a (PCIE SSD Example code)

    err = nvme_init();
    if(err != NVME_OK)
    {
        PCIE_logPrintf("Failed to initialize nvme\n");
        exit(1);
    }

    /* Initialize the file system. */
#if defined(ENABLE_FATFS)
    FATFS_init();
    PCIE_logPrintf("FATFS INIT Done\n");
    FATFS_open(0U, NULL, &fatfsHandle);
    PCIE_logPrintf("FATFS OPEN DONE\n");
    FSShellAppUtilsProcess();
#endif
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
    BIOS_exit(0);
>>>>>>> cc7d9980b (PDK-9352: Added PCIe SSD examples for AM65x)
=======
    OS_stop();
>>>>>>> 36779907c (updated the makefile and other components to enable freertos)
=======

#if defined (USE_BIOS)
    OS_stop();
#endif

>>>>>>> 57bdc7c55 (PDK-9352: PCIE SSD support for mpu core on am65xx)
=======
>>>>>>> 6deb7e263 (PDK-9352: PCIE SSD Support for am65xx)
}

int main()
{
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
#if defined (USE_BIOS)
=======
>>>>>>> 36779907c (updated the makefile and other components to enable freertos)
=======
#if defined (USE_BIOS)
>>>>>>> 57bdc7c55 (PDK-9352: PCIE SSD support for mpu core on am65xx)
    TaskP_Params params;

    OS_init();

    TaskP_Params_init (&params);
    params.stacksize = 36864; /* 32768; */
    TaskP_create((void *)pcie, &params);
<<<<<<< HEAD
=======
    Task_Params params;
    Task_Params_init (&params);
    params.stackSize = 36864; /* 32768; */
<<<<<<< HEAD
    Task_create((Task_FuncPtr) pcie_ssd_test, &params, NULL);
>>>>>>> cc7d9980b (PDK-9352: Added PCIe SSD examples for AM65x)
=======
    Task_create((Task_FuncPtr) pcie, &params, NULL);
>>>>>>> f0102448a (PCIE SSD Example code)
=======
>>>>>>> 36779907c (updated the makefile and other components to enable freertos)

#ifdef __ARM_ARCH_7A__
    /* Add MMU entries for MMR's required for PCIE example */
    Mmu_DescriptorAttrs attrs;
    extern char ti_sysbios_family_arm_a15_Mmu_Module_State_0_secondLevelTableBuf_0__A;
    extern char ti_sysbios_family_arm_a15_Mmu_Module_State_0_secondLevelTableBuf_1__A;
    uint32_t addr0 = (uint32_t)&ti_sysbios_family_arm_a15_Mmu_Module_State_0_secondLevelTableBuf_0__A;
    uint32_t addr1 = (uint32_t)&ti_sysbios_family_arm_a15_Mmu_Module_State_0_secondLevelTableBuf_1__A;

    Mmu_initDescAttrs(&attrs);

    attrs.type = Mmu_DescriptorType_TABLE;
    attrs.shareable = 0;            /* non-shareable */
    attrs.accPerm = 1;              /* read/write at any privelege level */
    attrs.attrIndx = 0;             /* Use MAIR0 Register Byte 3 for */
                                    /* determining the memory attributes */
                                    /* for each MMU entry */


    /* Update the first level table's MMU entry for 0x00000000 with the */
    /* new attributes. */
    Mmu_setFirstLevelDesc((Ptr)0x00000000, (UInt64)addr0, &attrs);
    /* Update the first level table's MMU entry for 0x40000000 with the */
    /* new attributes. */
    Mmu_setFirstLevelDesc((Ptr)0x40000000, (UInt64)addr1, &attrs);
#endif
<<<<<<< HEAD
<<<<<<< HEAD
#endif

=======
>>>>>>> cc7d9980b (PDK-9352: Added PCIe SSD examples for AM65x)
=======
#endif

>>>>>>> 57bdc7c55 (PDK-9352: PCIE SSD support for mpu core on am65xx)
=======
>>>>>>> 6deb7e263 (PDK-9352: PCIE SSD Support for am65xx)
    Board_initCfg boardCfg;
    boardCfg = BOARD_INIT_UNLOCK_MMR
                | BOARD_INIT_UART_STDIO
                | BOARD_INIT_MODULE_CLOCK
                | BOARD_INIT_PINMUX_CONFIG;
    Board_init(boardCfg);
<<<<<<< HEAD
<<<<<<< HEAD

    PCIe_Reset();

#if defined(USE_BIOS)
    OS_start();
#else
    pcie();
#endif

=======
    BIOS_start();
>>>>>>> cc7d9980b (PDK-9352: Added PCIe SSD examples for AM65x)
=======

    PCIe_Reset();

    pcie();

>>>>>>> 36779907c (updated the makefile and other components to enable freertos)
    return 0;
}
