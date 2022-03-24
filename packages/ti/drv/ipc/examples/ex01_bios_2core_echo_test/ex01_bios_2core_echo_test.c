/*
 *  Copyright (c) Texas Instruments Incorporated 2018
 *  All rights reserved.
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
 *  \file ex01_bios_2core_echo_test.c
 *
 *  \brief Two-core (BIOS-to-BIOS) IPC echo test application performing basic echo
 *  communication using the IPC driver
 *  
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdio.h>
#include <stdint.h>

#include <ti/drv/ipc/examples/common/src/ipc_setup.h>
#include <ti/drv/ipc/ipc.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */


/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */


/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

#define CORE_IN_TEST            2

/*
 * In the cfg file of R5F, C66x, default heap is 48K which is not
 * enough for 9 task_stack, so creating task_stack on global.
 * C7x cfg has 256k default heap, so no need to put task_stack on global
 */
#if !defined(BUILD_C7X)

uint8_t  g_taskStackBuf[(CORE_IN_TEST+2)*IPC_TASK_STACKSIZE];

#else

/* IMPORTANT NOTE: For C7x,
 * - stack size and stack ptr MUST be 8KB aligned
 * - AND min stack size MUST be 16KB
 * - AND stack assigned for task context is "size - 8KB"
*       - 8KB chunk for the stack area is used for interrupt handling in this task context
*/
uint8_t g_taskStackBuf[(CORE_IN_TEST+2)*IPC_TASK_STACKSIZE]
__attribute__ ((section(".bss:taskStackSection")))
__attribute__ ((aligned(8192)))
    ;
#endif

uint8_t  gCntrlBuf[RPMSG_DATA_SIZE] __attribute__ ((section("ipc_data_buffer"), aligned (8)));
uint8_t  sysVqBuf[VQ_BUF_SIZE]  __attribute__ ((section ("ipc_data_buffer"), aligned (8)));
uint8_t  g_sendBuf[RPMSG_DATA_SIZE * CORE_IN_TEST]  __attribute__ ((section ("ipc_data_buffer"), aligned (8)));
uint8_t  g_rspBuf[RPMSG_DATA_SIZE]  __attribute__ ((section ("ipc_data_buffer"), aligned (8)));

uint8_t *pCntrlBuf = gCntrlBuf;
uint8_t *pTaskBuf = g_taskStackBuf;
uint8_t *pSendTaskBuf = g_sendBuf;
uint8_t *pRecvTaskBuf = g_rspBuf;
uint8_t *pSysVqBuf = sysVqBuf;

/* am65x can only use MCU1_0 and MCU1_1 for this 2-core IPC echo test */
/* j7200 & j721s2 use MCU1_0 and MCU2_0 for this 2-core IPC echo test */
/* am64x uses MCU1_0 and MCU2_0 for this 2-core IPC echo test */

#ifdef BUILD_MCU1_0
uint32_t selfProcId = IPC_MCU1_0;
uint32_t remoteProc[] =
{
#if defined (SOC_AM65XX)
    IPC_MCU1_1
#endif
#if defined (SOC_J7200) || defined (SOC_AM64X) || defined (SOC_J721S2)
    IPC_MCU2_0
#endif
};
#endif

#ifdef BUILD_MCU1_1
uint32_t selfProcId = IPC_MCU1_1;
uint32_t remoteProc[] =
{
#if defined (SOC_AM65XX)
    IPC_MCU1_0
#endif
};
#endif

/* For j721e, we use MCU2_0 and C66X_1 for this 2-core IPC echo test */
/* j7200 & j721s2 use MCU1_0 and MCU2_0 for this 2-core IPC echo test */

#ifdef BUILD_MCU2_0
uint32_t selfProcId = IPC_MCU2_0;
uint32_t remoteProc[] =
{
#if defined (SOC_J721E)
    IPC_C66X_1
#endif
#if defined (SOC_J7200) || defined (SOC_AM64X) || defined (SOC_J721S2)
    IPC_MCU1_0
#endif
};
#endif

#ifdef BUILD_C66X_1
uint32_t selfProcId = IPC_C66X_1;
uint32_t remoteProc[] =
{
    IPC_MCU2_0
};
#endif

/* NOTE: all other cores are not used in this test, but must be built as part of full PDK build */ 
#if !defined(BUILD_MCU1_0) && !defined(BUILD_MCU1_1) && !defined(BUILD_MCU2_0) && !defined(BUILD_C66X_1)
uint32_t selfProcId = 0;
uint32_t remoteProc[] = {};
#endif

uint32_t *pRemoteProcArray = remoteProc;
uint32_t  gNumRemoteProc = sizeof(remoteProc)/sizeof(uint32_t);

