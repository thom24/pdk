/******************************************************************************
* Copyright (c) 2020 Texas Instruments Incorporated - http://www.ti.com
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
*****************************************************************************/

/**
 *
 * \file   csirx_test_tpr12.h
 *
 * \brief  This is the header file for CSI-Rx diagnostic test.
 *
 */

#ifndef _CSIRX_TEST_TPR12_H_
#define _CSIRX_TEST_TPR12_H_

/* Standard Include Files. */
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#include <ti/drv/csirx/soc/csirx_soc_priv.h>
#include <ti/drv/csirx/csirx.h>
#include <ti/csl/cslr_csirx.h>
#include <ti/osal/CacheP.h>

#include <assert.h>
#include <ti/drv/csirx/csirx.h>

#include <ti/csl/soc.h>
#include <ti/osal/osal.h>
#include <ti/osal/DebugP.h>
#include <ti/osal/CycleprofilerP.h>
#include <ti/board/board.h>

#include <ti/drv/uart/UART.h>
#include <ti/drv/uart/UART_stdio.h>

/* ======================================================================== */
/*                           Macros & Typedefs                              */
/* ======================================================================== */

/* should match with CSI TX, assumes complex sample */
//#define PAYLOAD_PATTERN_CHECK             (1)

#define BOARD_DIAG_TEST_NUM_ADC_SAMPLES_PER_CHIRP (256U)
#define BOARD_DIAG_TEST_BYTES_PER_ADC_SAMPLE      (4U) //complex
#define BOARD_DIAG_TEST_NUM_CHIRPS                (1U)
#define BOARD_DIAG_TEST_NUM_FRAMES                (10U)
#define BOARD_DIAG_TEST_NUM_RX                    (4U)

#define BOARD_DIAG_TEST_PATTERN				  (0xFFU)
#define BOARD_DIAG_TEST_CONTEXT               (2U)
#define BOARD_DIAG_TEST_FORMAT                (CSIRX_FORMAT_RAW8)
#define BOARD_DIAG_TEST_VC                    (0U)
#define BOARD_DIAG_TEST_USER_DEFINED_MAPPING  (CSIRX_USER_DEFINED_FORMAT_RAW8)
#define BOARD_DIAG_NUM_INSTANCES			  (2U)

#define BOARD_DIAG_TEST_NUM_BYTES_PER_FRAME \
    ((BOARD_DIAG_TEST_NUM_ADC_SAMPLES_PER_CHIRP * BOARD_DIAG_TEST_BYTES_PER_ADC_SAMPLE * \
	 BOARD_DIAG_TEST_NUM_CHIRPS * BOARD_DIAG_TEST_NUM_RX))

#define BOARD_DIAG_TEST_COMMON_CB_ARG       (0x11112222U)
#define BOARD_DIAG_TEST_COMBINED_EOF_CB_ARG (0x33334444U)

#define BOARD_DIAG_PING_PONG_ALIGNMENT CSL_MAX(CSIRX_PING_PONG_ADDRESS_LINEOFFSET_ALIGNMENT_IN_BYTES, \
                                         CSL_CACHE_L1D_LINESIZE)

#define BOARD_DIAG_PING_OR_PONG_BUF_SIZE_ALIGNED CSL_NEXT_MULTIPLE_OF(BOARD_DIAG_TEST_NUM_BYTES_PER_FRAME, \
                                                                BOARD_DIAG_PING_PONG_ALIGNMENT)


/*! Defines RAM types for choosing ping-pong buffers */
typedef enum BoardDiag_RAMtype_e
{
    BOARD_DIAG_L3RAM,
    BOARD_DIAG_HWARAM,
    BOARD_DIAG_L2RAM
} BoardDiag_RAMtype;

typedef struct Board_DiagConfig_s
{
    CSIRX_DphyConfig_t DPHYcfg;
    CSIRX_ComplexIoConfig_t complexIOcfg;
    CSIRX_CommonConfig_t commonCfg;
    CSIRX_ContextConfig_t contextCfg;
} Board_DiagConfig;

typedef struct BoardDiag_ContextIRQcount_s
{
    volatile uint32_t frameEndCodeDetect;
    uint32_t lineEndCodeDetect;
} BoardDiag_ContextIRQcount;

/*! holds common IRQ counts */
typedef struct BoardDiag_CommonIRQcount_s
{
    uint32_t isOCPerror;
    uint32_t isGenericShortPacketReceive;
    uint32_t isECConeBitShortPacketErrorCorrect;
    uint32_t isECCmoreThanOneBitCannotCorrect;
    uint32_t isComplexIOerror;
    uint32_t isFIFOoverflow;
} BoardDiag_CommonIRQcount;

typedef struct BoardDiag_IRQs_s
{
    CSIRX_ContextIRQ_t context[CSIRX_NUM_CONTEXTS];
    CSIRX_CommonIRQ_t common;
    CSIRX_LanesIRQ_t  complexIOlanes;
} BoardDiag_IRQs;

typedef struct BoardDiag_CallBackCounts_s
{
    uint32_t common;
    uint32_t combinedEOL;
    uint32_t combinedEOF;
    uint32_t EOF0;
    uint32_t EOF1;
    uint32_t SOF0;
    uint32_t SOF1;
    uint32_t contextEOL[CSIRX_NUM_CONTEXTS];
} BoardDaig_CallBackCounts;

typedef struct BoardDaig_State_s
{
	BoardDiag_CommonIRQcount commonIRQcount;
    BoardDiag_IRQs IRQ;
    BoardDaig_CallBackCounts callbackCount;
    bool isReceivedPayloadCorrect;
	BoardDiag_ContextIRQcount contextIRQcounts[CSIRX_NUM_CONTEXTS];
    uint32_t frameId;
    uint32_t lineId;
	uint32_t receivedBuffer;
} BoardDaig_State;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief   This function executes CSI-Rx diagnostic test
 *
 * \return  int8_t
 *             0   - in case of success
 *            -1   - in case of failure
 *
 */
int8_t BoardDiag_CsirxTest(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _CSIRX_TEST_TPR12_H_ */
