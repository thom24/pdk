/*  ============================================================================
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> f0102448a (PCIE SSD Example code)
 *   Copyright (c) Texas Instruments Incorporated 2010-2019
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
<<<<<<< HEAD
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
=======
 *   Copyright (c) Texas Instruments Incorporated 2021
 * 
 *  Redistribution and use in source and binary forms, with or without 
 *  modification, are permitted provided that the following conditions 
=======
>>>>>>> f0102448a (PCIE SSD Example code)
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
<<<<<<< HEAD
 *    notice, this list of conditions and the following disclaimer in the 
 *    documentation and/or other materials provided with the   
>>>>>>> cc7d9980b (PDK-9352: Added PCIe SSD examples for AM65x)
=======
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
>>>>>>> f0102448a (PCIE SSD Example code)
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
<<<<<<< HEAD
<<<<<<< HEAD
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
=======
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
=======
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
>>>>>>> f0102448a (PCIE SSD Example code)
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
<<<<<<< HEAD
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
>>>>>>> cc7d9980b (PDK-9352: Added PCIe SSD examples for AM65x)
=======
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
>>>>>>> f0102448a (PCIE SSD Example code)
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
*/


<<<<<<< HEAD
<<<<<<< HEAD
/**
 * @file pcie_ssd.h
<<<<<<< HEAD
 *
 * @brief
 *  Holds all the constants and API definitions required by the example
 *  application to run.
 */

#ifndef _PCIE_SSD_H_
#define _PCIE_SSD_H_

#define ENABLE_FATFS
=======
/**  
=======
/**
>>>>>>> f0102448a (PCIE SSD Example code)
 * @file pcie_sample.h
=======
>>>>>>> 3fda8cd5c (Updating as per the review comments)
 *
 * @brief
 *  Holds all the constants and API definitions required by the example
 *  application to run.
 */

<<<<<<< HEAD
#ifndef _PCIE_SAMPLE_H_
#define _PCIE_SAMPLE_H_
>>>>>>> cc7d9980b (PDK-9352: Added PCIe SSD examples for AM65x)
=======
#ifndef _PCIE_SSD_H_
#define _PCIE_SSD_H_
>>>>>>> 3fda8cd5c (Updating as per the review comments)

#define ENABLE_FATFS

/* C Standard library include */
#include <string.h>

<<<<<<< HEAD
<<<<<<< HEAD
#include <stdint.h>
#include <stdio.h>

#include <ti/osal/osal.h>
<<<<<<< HEAD
#include <ti/osal/TaskP.h>

#if defined (USE_BIOS)
#if defined (__aarch64__)
/* XDCtools Header files */
#include <xdc/std.h>
#endif
#endif
=======
/* XDC include */
#include <xdc/std.h>
#include <xdc/cfg/global.h>
#include <xdc/runtime/System.h>
=======
#include <stdint.h>
>>>>>>> 36779907c (updated the makefile and other components to enable freertos)
#include <stdio.h>

#include <ti/osal/osal.h>
#include <ti/osal/TaskP.h>

<<<<<<< HEAD
<<<<<<< HEAD
/* BIOS include */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/family/c64p/EventCombiner.h>
#include <ti/sysbios/family/c64p/Hwi.h>
#include <ti/sysbios/knl/Event.h>
>>>>>>> cc7d9980b (PDK-9352: Added PCIe SSD examples for AM65x)
=======
=======
#if defined (USE_BIOS)
>>>>>>> 57bdc7c55 (PDK-9352: PCIE SSD support for mpu core on am65xx)
#if defined (__aarch64__)
/* XDCtools Header files */
#include <xdc/std.h>
#endif
<<<<<<< HEAD
>>>>>>> 36779907c (updated the makefile and other components to enable freertos)
=======
#endif
>>>>>>> 57bdc7c55 (PDK-9352: PCIE SSD support for mpu core on am65xx)
=======
>>>>>>> 6deb7e263 (PDK-9352: PCIE SSD Support for am65xx)

/* CSL include */
#include <ti/csl/cslr_device.h>
#include <ti/csl/hw_types.h>
<<<<<<< HEAD
<<<<<<< HEAD

=======
>>>>>>> cc7d9980b (PDK-9352: Added PCIe SSD examples for AM65x)
=======

>>>>>>> 36779907c (updated the makefile and other components to enable freertos)
/* PCIE LLD include */
#include <ti/drv/pcie/pcie.h>

#if !defined(__TI_ARM_V7M4__) && !defined(EDMAPKTBENCH) && !defined(SOC_AM65XX) && !defined(SOC_J721E)
/* Only do EDMA on C6X and A15 */
#define EDMA
#endif

#if defined(SOC_AM65XX)||defined(SOC_J721E)
#define UDMA
#endif

#ifdef EDMA
/* EDMA include */
<<<<<<< HEAD
<<<<<<< HEAD
#include <ti/drv/pcie/example/EDMA/PCIeEDMA.h>
#elif defined(EDMAPKTBENCH)
#include <ti/drv/pcie/example/edmaPktBench/edmaPktBench.h>
=======
#include "../../EDMA/PCIeEDMA.h"
#elif defined(EDMAPKTBENCH)
#include "../../edmaPktBench/edmaPktBench.h"
>>>>>>> cc7d9980b (PDK-9352: Added PCIe SSD examples for AM65x)
=======
#include <ti/drv/pcie/example/EDMA/PCIeEDMA.h>
#elif defined(EDMAPKTBENCH)
#include <ti/drv/pcie/example/edmaPktBench/edmaPktBench.h>
>>>>>>> ad03eb174 (Updated the files as per previous comments)
#endif

#if defined(SOC_J721E)
/* Do gen2 on all other devices -- remove or -U for GEN1 */
#define GEN3
#else
#define GEN2
#endif

<<<<<<< HEAD
<<<<<<< HEAD
#define Console_printf printf
=======
/* Set up printf */
#include <xdc/runtime/System.h>
#define Console_printf System_printf
>>>>>>> cc7d9980b (PDK-9352: Added PCIe SSD examples for AM65x)
=======
#define Console_printf printf
>>>>>>> 36779907c (updated the makefile and other components to enable freertos)

/* Enable the below macro to have prints on the IO Console */
#if !defined(SOC_AM574x) && !defined(SOC_AM572x) && !defined(SOC_AM571x) && \
    !defined(SOC_K2G) && \
    !defined(SOC_AM65XX) && \
    !defined(SOC_J721E) && \
    !defined(__ARM_ARCH_7A__)
/* AM57x, AM65x and K2G always use UART for all cores.  Also all ARM use UART */
#define IO_CONSOLE
#endif

#ifndef IO_CONSOLE
#include <ti/drv/uart/UART_stdio.h>
#define PCIE_logPrintf  UART_printf
#define PCIE_logScanf   UART_scanFmt
#else
#define PCIE_logPrintf  Console_printf
#define PCIE_logScanf   Console_scanf
#endif

/* Size of application buffers */
<<<<<<< HEAD
<<<<<<< HEAD
#define PCIE_BUFSIZE_APP 40
=======
#define PCIE_BUFSIZE_APP 40 
>>>>>>> cc7d9980b (PDK-9352: Added PCIe SSD examples for AM65x)
=======
#define PCIE_BUFSIZE_APP 40
>>>>>>> f0102448a (PCIE SSD Example code)

/* Number of each type of interrupt to send */
#define PCIE_NUM_INTS   10

/* Write pattern */
#define PCIE_WR_PATTERN 0xFACEFACE

<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> 3fda8cd5c (Updating as per the review comments)

#define PCIE_EP_LEGACY_A_IRQ_ENABLE_SET_REG     0x5500188
#define PCIE_EP_LEGACY_B_IRQ_ENABLE_SET_REG     0x5500198
#define PCIE_EP_LEGACY_C_IRQ_ENABLE_SET_REG     0x55001A8
#define PCIE_EP_LEGACY_D_IRQ_ENABLE_SET_REG     0x55001B8

#define PCIE_EP_ERR_IRQ_ENABLE_SET_REG          0x55001C8

#define PCIE_EP_BAR0_REG                        0x5501010


/* In this example all addresses are 32bit */
/* Outbound Base Address for PCIe RC */
#define PCIE_OB_LO_ADDR_RC   0x70000000 /* 0x10030000 */
<<<<<<< HEAD
=======
/* In this example all addresses are 32bit */
/* Outbound Base Address for PCIe RC */
<<<<<<< HEAD
#define PCIE_OB_LO_ADDR_RC   0x10030000 //0x70000000
>>>>>>> cc7d9980b (PDK-9352: Added PCIe SSD examples for AM65x)
=======
#define PCIE_OB_LO_ADDR_RC   0x70000000 //0x10030000
>>>>>>> f0102448a (PCIE SSD Example code)
=======
>>>>>>> ad03eb174 (Updated the files as per previous comments)
#define PCIE_OB_HI_ADDR_RC   0

/* Inbound  Base Address for PCIe RC */
#define PCIE_IB_LO_ADDR_RC   0x90000000
#define PCIE_IB_HI_ADDR_RC   0

/* Outbound Base Address for PCIe EP */
#define PCIE_OB_LO_ADDR_EP   PCIE_IB_LO_ADDR_RC
#define PCIE_OB_HI_ADDR_EP   0

/* Inbound  Base Address for PCIe EP */
#define PCIE_IB_LO_ADDR_EP   PCIE_OB_LO_ADDR_RC
#define PCIE_IB_HI_ADDR_EP   0


#define PCIE_WINDOW_START    0x10000000U

/* PCIE address space for MSI */
#define PCIE_PCIE_MSI_BASE   (0x00010000U)
#define PCIE_PCIE_MSI_OFF    (0x00000040U)

/* Data area offset absolute including PCIe base (only used rev 2) */
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
#define PCIE_WINDOW_MEM_BASE (PCIE_WINDOW_START + 0x01000000U)/*(PCIE_WINDOW_START + 0x00030000U)*/
#define PCIE_WINDOW_MEM_MASK 0x00FFFFFFU  /* 0x0000FFFFU */

/* Cfg area offset absolute including PCIe base (only used rev 2) */
/* This MUST agree Pciev1_DeviceCfgBaseAddrs.bases! */
#define PCIE_WINDOW_CFG_BASE (PCIE_WINDOW_START + 0x00010000U) /* (PCIE_WINDOW_START) */
=======
#define PCIE_WINDOW_MEM_BASE (PCIE_WINDOW_START + 0x00030000U )//(PCIE_WINDOW_START + 0x01000000U)
#define PCIE_WINDOW_MEM_MASK 0x0000FFFFU//0x00FFFFFFU

/* Cfg area offset absolute including PCIe base (only used rev 2) */
/* This MUST agree Pciev1_DeviceCfgBaseAddrs.bases! */
#define PCIE_WINDOW_CFG_BASE (PCIE_WINDOW_START )//(PCIE_WINDOW_START + 0x00010000U)
>>>>>>> cc7d9980b (PDK-9352: Added PCIe SSD examples for AM65x)
=======
#define PCIE_WINDOW_MEM_BASE (PCIE_WINDOW_START + 0x01000000U)//(PCIE_WINDOW_START + 0x00030000U)
#define PCIE_WINDOW_MEM_MASK 0x00FFFFFFU//0x0000FFFFU

/* Cfg area offset absolute including PCIe base (only used rev 2) */
/* This MUST agree Pciev1_DeviceCfgBaseAddrs.bases! */
#define PCIE_WINDOW_CFG_BASE (PCIE_WINDOW_START + 0x00010000U)//(PCIE_WINDOW_START)
>>>>>>> f0102448a (PCIE SSD Example code)
=======
#define PCIE_WINDOW_MEM_BASE (PCIE_WINDOW_START + 0x01000000U)/*(PCIE_WINDOW_START + 0x00030000U)*/
#define PCIE_WINDOW_MEM_MASK 0x00FFFFFFU  /* 0x0000FFFFU */

/* Cfg area offset absolute including PCIe base (only used rev 2) */
/* This MUST agree Pciev1_DeviceCfgBaseAddrs.bases! */
#define PCIE_WINDOW_CFG_BASE (PCIE_WINDOW_START + 0x00010000U) /* (PCIE_WINDOW_START) */
>>>>>>> ad03eb174 (Updated the files as per previous comments)
#define PCIE_WINDOW_CFG_MASK 0x0000FFFFU

/* MSI address in PCIE data window */
#define PCIE_WINDOW_MSI_ADDR (PCIE_WINDOW_START + 0x00020000U)
#define PCIE_WINDOW_MSI_MASK 0x0000FFFFU

/* SPI number (a block of reserved ARM GIC SPIs) to use for MSI) */
/* For AM65xx, use the reserved SPI 268 - 279 */
#define PCIE_SPI_BASE        (300) /* 268+32 = 300 */
#define PCIE_WINDOW_MSI_DATA (PCIE_SPI_BASE)

/* Inbound limit */
#define PCIE_INBOUND_MASK    0x0FFFFFFFU

/* BAR mask */
#define PCIE_BAR_MASK         0x0FFFFFFF

/* BAR Index PCie*/
#define PCIE_BAR_IDX_RC        1
#define PCIE_BAR_IDX_EP        1


/* PCIe Regions used in the example */
#define PCIE_IB_REGION_RC   0
#define PCIE_OB_REGION_RC   0
#define PCIE_IB_REGION_EP   0
#define PCIE_OB_REGION_EP   0

<<<<<<< HEAD
<<<<<<< HEAD

=======
>>>>>>> cc7d9980b (PDK-9352: Added PCIe SSD examples for AM65x)
=======

>>>>>>> f0102448a (PCIE SSD Example code)
#ifdef EDMA
/* Synchronized token passing between two endpoints using EDMA IO */
#define PCIE_EXAMPLE_DMA_RC
#endif

#ifdef EDMA
/* Asynchronous block transfers using EDMA */
#define PCIE_EXAMPLE_DMA_EP
#endif

#if defined(EDMA)
/* 64KB EDMA transfer */
<<<<<<< HEAD
<<<<<<< HEAD
#define PCIE_EXAMPLE_LINE_SIZE      16384
#elif defined(UDMA)
/* 32KB UDMA transfer */
#define PCIE_EXAMPLE_LINE_SIZE      8192
=======
#define PCIE_EXAMPLE_LINE_SIZE      16384 
#elif defined(UDMA)
/* 32KB UDMA transfer */
#define PCIE_EXAMPLE_LINE_SIZE      8192 
>>>>>>> cc7d9980b (PDK-9352: Added PCIe SSD examples for AM65x)
=======
#define PCIE_EXAMPLE_LINE_SIZE      16384
#elif defined(UDMA)
/* 32KB UDMA transfer */
#define PCIE_EXAMPLE_LINE_SIZE      8192
>>>>>>> f0102448a (PCIE SSD Example code)
#endif

#ifdef EDMA
/*****************************************************************************
 * Choose the type of EDMA transfer (Current options are "DMA" and "QDMA")
 *****************************************************************************/
#define EDMA_TYPE 0 /* DMA */
/* #define EDMA_TYPE 1 //QDMA */

#endif

<<<<<<< HEAD
<<<<<<< HEAD
#endif

=======
/* last element in the buffer is a marker that indicates the buffer status: full/empty */
#ifdef SOC_J721E
#define PCIE_EXAMPLE_MAX_CACHE_LINE_SIZE 0x1000
#else
#define PCIE_EXAMPLE_MAX_CACHE_LINE_SIZE 128
=======
>>>>>>> f0102448a (PCIE SSD Example code)
#endif

<<<<<<< HEAD

#endif
>>>>>>> cc7d9980b (PDK-9352: Added PCIe SSD examples for AM65x)
=======
>>>>>>> f0102448a (PCIE SSD Example code)
