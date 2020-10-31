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
 *  \file   csirx_test_tpr12.c
 *
 *  \brief  csirx diagnostic test file.
 *
 *  Targeted Functionality: Verification of csirx interface by receiving fixed
 *                          pattern data from Radar sensor.
 *
 *  Operation: This test verifies CSIRX interface by receiving the userDefinedMapping
 *          user defined from IWR143 radar sensor.
 *
 *  Supported SoCs: TPR12.
 *
 *  Supported Platforms: tp12_evm.
 */



#include <csirx_test_tpr12.h>

BoardDaig_State gTestState = {0};
volatile bool gFrameReceived = 0;
uint32_t gErrorCode = 0;
uint32_t gFrameCounter = 0;

#define BOARD_DIAG_CSIRX_A_TEST             (1U)
#define BOARD_DIAG_TEST_BUF_INIT_PATTERN    (0xBE)
#define BOARD_DIAG_TEST_PAYLOAD_PATTERN_NUM_BYTES_PER_FRAME (128U)
#define BOARD_DIAG_INIT_PATTERN_SIZE        (BOARD_DIAG_PING_OR_PONG_BUF_SIZE_ALIGNED + 32U)
/* Ping */
#pragma DATA_SECTION(testPingBufL3, ".l3ram");
uint8_t testPingBufL3[BOARD_DIAG_INIT_PATTERN_SIZE]       \
        __attribute__ ((aligned(BOARD_DIAG_PING_PONG_ALIGNMENT)));

#pragma DATA_SECTION(testPingBufHWA, ".hwaram");
uint8_t testPingBufHWA[BOARD_DIAG_INIT_PATTERN_SIZE]       \
        __attribute__ ((aligned(BOARD_DIAG_PING_PONG_ALIGNMENT)));

#pragma DATA_SECTION(testPingBufL2, ".l2ram");
uint8_t testPingBufL2[BOARD_DIAG_INIT_PATTERN_SIZE]       \
        __attribute__ ((aligned(BOARD_DIAG_PING_PONG_ALIGNMENT)));

/* Pong */
#pragma DATA_SECTION(testPongBufL3, ".l3ram");
uint8_t testPongBufL3[BOARD_DIAG_INIT_PATTERN_SIZE]       \
        __attribute__ ((aligned(BOARD_DIAG_PING_PONG_ALIGNMENT)));

#pragma DATA_SECTION(testPongBufHWA, ".hwaram");
uint8_t testPongBufHWA[BOARD_DIAG_INIT_PATTERN_SIZE]       \
        __attribute__ ((aligned(BOARD_DIAG_PING_PONG_ALIGNMENT)));

#pragma DATA_SECTION(testPongBufL2, ".l2ram");
uint8_t testPongBufL2[BOARD_DIAG_INIT_PATTERN_SIZE]       \
        __attribute__ ((aligned(BOARD_DIAG_PING_PONG_ALIGNMENT)));

/**
 *  \brief    Used to read the payload data from the ping-pong buffers
 *            This function comapres the data received from the buffers
 *            to expected value.
 *
 *  \param    handle              [IN]     CSIRX Handler
 *
 *  \return   NULL
 *
 */
void BoardDiag_CheckPayloadReceived(CSIRX_Handle handle)
{
    uint32_t numBytes = 0;
    uint32_t buffer, bufIndx = 0;
    uint8_t *buf;

    gErrorCode = CSIRX_getContextReceivedBuffer(handle, BOARD_DIAG_TEST_CONTEXT, &buffer);
    DebugP_assert(gErrorCode == CSIRX_NO_ERROR);
    buffer = CSL_globToLocAddr(buffer);
    buf = (uint8_t *)buffer;
    CacheP_Inv(buf,BOARD_DIAG_PING_OR_PONG_BUF_SIZE_ALIGNED);
#if defined(PAYLOAD_PATTERN_CHECK)
    for(numBytes = 0; numBytes < BOARD_DIAG_PING_OR_PONG_BUF_SIZE_ALIGNED; numBytes++)
    {
        if(buf[bufIndx++] != BOARD_DIAG_TEST_PATTERN)
        {
            gTestState.isReceivedPayloadCorrect = false;
            DebugP_log1("Frame - %d is invalid\n",
            gTestState.contextIRQcounts[BOARD_DIAG_TEST_CONTEXT].frameEndCodeDetect);
            break;
        }
    }
#else
    numBytes = BOARD_DIAG_PING_OR_PONG_BUF_SIZE_ALIGNED;
    bufIndx = BOARD_DIAG_PING_OR_PONG_BUF_SIZE_ALIGNED;
#endif
    if (numBytes == BOARD_DIAG_PING_OR_PONG_BUF_SIZE_ALIGNED)
    {
        for (; numBytes < (BOARD_DIAG_INIT_PATTERN_SIZE);numBytes++)
        {
            if(buf[bufIndx++] != BOARD_DIAG_TEST_BUF_INIT_PATTERN)
            {
                gTestState.isReceivedPayloadCorrect = false;
                UART_printf("Buffer corruption - %d is invalid\n",
                gTestState.contextIRQcounts[BOARD_DIAG_TEST_CONTEXT].frameEndCodeDetect);
                break;
            }
        }
    }
}

/**
 *  \brief    Callback function for common.irq interrupt, generated when
 *            end of frame code and line code detected.
 *
 *  \param    handle              [IN]     CSIRX Handler
 *            arg                 [IN]     CALLBACK function argument
 *            IRQ                 [OUT]    CSIRX common irq
 *
 */
void BoardDiag_commonCallback(CSIRX_Handle handle, uint32_t arg,
                              CSIRX_CommonIRQ_t *IRQ)
{
    uint8_t i;
    uint32_t frameCounter =
    gTestState.contextIRQcounts[BOARD_DIAG_TEST_CONTEXT].frameEndCodeDetect + 1;

    DebugP_assert(handle != NULL);
    DebugP_assert(arg == BOARD_DIAG_TEST_COMMON_CB_ARG);
    gTestState.callbackCount.common++;

    gTestState.IRQ.common = *IRQ;

    /* Counts book-keeping */
    if(IRQ->isOCPerror == true)
    {
        gTestState.commonIRQcount.isOCPerror++;
    }
    if(IRQ->isComplexIOerror == true)
    {
        gTestState.commonIRQcount.isComplexIOerror++;
    }
    if(IRQ->isFIFOoverflow == true)
    {
        gTestState.commonIRQcount.isFIFOoverflow++;
    }

    if(IRQ->isComplexIOerror)
    {
        gErrorCode = CSIRX_getComplexIOlanesIRQ(handle,
                                               &gTestState.IRQ.complexIOlanes);
        if(gErrorCode != CSIRX_NO_ERROR)
        {
            DebugP_log1("Error occured while recieving the frame-%d\n", frameCounter);
        }
        DebugP_assert(gErrorCode == CSIRX_NO_ERROR);

        gErrorCode = CSIRX_clearAllcomplexIOlanesIRQ(handle);
        DebugP_assert(gErrorCode == CSIRX_NO_ERROR);
    }

    for(i = 0; i < CSIRX_NUM_CONTEXTS; i++)
    {
        if(IRQ->isContext[i] == true)
        {
            gErrorCode = CSIRX_getContextIRQ(handle, i,
                                            &gTestState.IRQ.context[i]);
            DebugP_assert(gErrorCode == CSIRX_NO_ERROR);

            if(gTestState.IRQ.context[i].isFrameEndCodeDetect == true)
            {
                gTestState.contextIRQcounts[i].frameEndCodeDetect++;
                  /* Single frame is received */
                gFrameReceived = true;

            }

            if(gTestState.IRQ.context[i].isLineEndCodeDetect == true)
            {
                gTestState.contextIRQcounts[i].lineEndCodeDetect++;
            }

            gErrorCode = CSIRX_clearAllcontextIRQ(handle, i);
            DebugP_assert(gErrorCode == CSIRX_NO_ERROR);
        }
    }
}

void BoardDiag_CheckStateError(bool *isTestPass)
{
    if(gTestState.commonIRQcount.isOCPerror != 0)
    {
        DebugP_log1("OCP error has occured %d number of times \n", gTestState.commonIRQcount.isOCPerror);
        *isTestPass = false;
    }
    if(gTestState.commonIRQcount.isComplexIOerror != 0)
    {
        DebugP_log1("Complex IO error has occured %d number of times \n", gTestState.commonIRQcount.isComplexIOerror);
        *isTestPass = false;
    }
    if(gTestState.commonIRQcount.isFIFOoverflow != 0)
    {
        DebugP_log1("FIFO Overflow error has occured %d number of times \n",gTestState.commonIRQcount.isFIFOoverflow);
        *isTestPass = false;
    }
}

void BoardDiag_combinedEOFcallback(CSIRX_Handle handle, uint32_t arg)
{
    DebugP_assert(handle != NULL);
    DebugP_assert(arg == BOARD_DIAG_TEST_COMBINED_EOF_CB_ARG);
    gTestState.callbackCount.combinedEOF++;
}

Board_DiagConfig testConfig =
{
    /* DDR clock set to 300 MHz */
    .DPHYcfg.ddrClockInHz = 300000000U,
    .DPHYcfg.isClockMissingDetectionEnabled = true,
    .DPHYcfg.triggerEscapeCode[0] = 0x0,
    .DPHYcfg.triggerEscapeCode[1] = 0x0,
    .DPHYcfg.triggerEscapeCode[2] = 0x0,
    .DPHYcfg.triggerEscapeCode[3] = 0x0,

    .complexIOcfg.lanesConfig.dataLane[0].polarity = 0,
    .complexIOcfg.lanesConfig.dataLane[0].position = CSIRX_LANE_POSITION_1,
    .complexIOcfg.lanesConfig.dataLane[1].polarity = 0,
    .complexIOcfg.lanesConfig.dataLane[1].position = CSIRX_LANE_POSITION_2,
    .complexIOcfg.lanesConfig.dataLane[2].polarity = 0,
    .complexIOcfg.lanesConfig.dataLane[2].position = CSIRX_LANE_POSITION_4,
    .complexIOcfg.lanesConfig.dataLane[3].polarity = 0,
    .complexIOcfg.lanesConfig.dataLane[3].position = CSIRX_LANE_POSITION_5,
    .complexIOcfg.lanesConfig.clockLane.polarity = 0,
    .complexIOcfg.lanesConfig.clockLane.position = CSIRX_LANE_POSITION_3,
    .complexIOcfg.lanesIRQ.isAllLanesULPMenter = true,
    .complexIOcfg.lanesIRQ.isAllLanesULPMexit = true,
    .complexIOcfg.lanesIRQ.dataLane[0].isStateTransitionToULPM = true,
    .complexIOcfg.lanesIRQ.dataLane[0].isControlError = true,
    .complexIOcfg.lanesIRQ.dataLane[0].isEscapeEntryError = true,
    .complexIOcfg.lanesIRQ.dataLane[0].isStartOfTransmisionSyncError = true,
    .complexIOcfg.lanesIRQ.dataLane[0].isStartOfTransmisionError = true,
    .complexIOcfg.lanesIRQ.dataLane[1].isStateTransitionToULPM = true,
    .complexIOcfg.lanesIRQ.dataLane[1].isControlError = true,
    .complexIOcfg.lanesIRQ.dataLane[1].isEscapeEntryError = true,
    .complexIOcfg.lanesIRQ.dataLane[1].isStartOfTransmisionSyncError = true,
    .complexIOcfg.lanesIRQ.dataLane[1].isStartOfTransmisionError = true,
    .complexIOcfg.lanesIRQ.dataLane[2].isStateTransitionToULPM = true,
    .complexIOcfg.lanesIRQ.dataLane[2].isControlError = true,
    .complexIOcfg.lanesIRQ.dataLane[2].isEscapeEntryError = true,
    .complexIOcfg.lanesIRQ.dataLane[2].isStartOfTransmisionSyncError = true,
    .complexIOcfg.lanesIRQ.dataLane[2].isStartOfTransmisionError = true,
    .complexIOcfg.lanesIRQ.dataLane[3].isStateTransitionToULPM = true,
    .complexIOcfg.lanesIRQ.dataLane[3].isControlError = true,
    .complexIOcfg.lanesIRQ.dataLane[3].isEscapeEntryError = true,
    .complexIOcfg.lanesIRQ.dataLane[3].isStartOfTransmisionSyncError = true,
    .complexIOcfg.lanesIRQ.dataLane[3].isStartOfTransmisionError = true,
    .complexIOcfg.lanesIRQ.clockLane.isStateTransitionToULPM = true,
    .complexIOcfg.lanesIRQ.clockLane.isControlError = true,
    .complexIOcfg.lanesIRQ.clockLane.isEscapeEntryError = true,
    .complexIOcfg.lanesIRQ.clockLane.isStartOfTransmisionSyncError = true,
    .complexIOcfg.lanesIRQ.clockLane.isStartOfTransmisionError = true,

    .commonCfg.isSoftStoppingOnInterfaceDisable = true,
    .commonCfg.isECCenabled = false,
    .commonCfg.isEXP16SignExtensionEnabled = false,
    .commonCfg.isBurstSizeExpand = false,
    .commonCfg.isNonPostedWrites = true,
    .commonCfg.isOCPautoIdle = true,
    .commonCfg.stopStateFSMtimeoutInNanoSecs = 200000U,
    .commonCfg.burstSize = 8,
    .commonCfg.endianness = CSIRX_ALL_LITTLE_ENDIAN,
    .commonCfg.startOfFrameIRQ0contextId = BOARD_DIAG_TEST_CONTEXT,
    .commonCfg.startOfFrameIRQ1contextId = 0,
    .commonCfg.endOfFrameIRQ0contextId = BOARD_DIAG_TEST_CONTEXT,
    .commonCfg.endOfFrameIRQ1contextId = 0,
    .commonCfg.IRQ.isOCPerror = true,
    .commonCfg.IRQ.isGenericShortPacketReceive = false,
    .commonCfg.IRQ.isECConeBitShortPacketErrorCorrect = false,
    .commonCfg.IRQ.isECCmoreThanOneBitCannotCorrect = false,
    .commonCfg.IRQ.isComplexIOerror = true,
    .commonCfg.IRQ.isFIFOoverflow = true,
    .commonCfg.IRQ.isContext[0] = false,
    .commonCfg.IRQ.isContext[1] = false,
    .commonCfg.IRQ.isContext[2] = false,
    .commonCfg.IRQ.isContext[3] = false,
    .commonCfg.IRQ.isContext[4] = false,
    .commonCfg.IRQ.isContext[5] = false,
    .commonCfg.IRQ.isContext[6] = false,
    .commonCfg.IRQ.isContext[7] = false,
    .commonCfg.IRQcallbacks.common.fxn = BoardDiag_commonCallback,
    .commonCfg.IRQcallbacks.common.arg = BOARD_DIAG_TEST_COMMON_CB_ARG,
    .commonCfg.IRQcallbacks.combinedEndOfLine.fxn = NULL,
    .commonCfg.IRQcallbacks.combinedEndOfLine.arg = 0,
#ifdef BUILD_DSP_1
    .commonCfg.IRQcallbacks.combinedEndOfFrame.fxn = NULL,
    .commonCfg.IRQcallbacks.combinedEndOfFrame.arg = 0,
#else
    .commonCfg.IRQcallbacks.combinedEndOfFrame.fxn =
        BoardDiag_combinedEOFcallback,
    .commonCfg.IRQcallbacks.combinedEndOfFrame.arg =
        BOARD_DIAG_TEST_COMBINED_EOF_CB_ARG,
#endif
    .commonCfg.IRQcallbacks.startOfFrameIRQ0.fxn = NULL,
    .commonCfg.IRQcallbacks.startOfFrameIRQ0.arg = 0,
    .commonCfg.IRQcallbacks.startOfFrameIRQ1.fxn = NULL,
    .commonCfg.IRQcallbacks.startOfFrameIRQ1.arg = 0,
    .commonCfg.IRQcallbacks.endOfFrameIRQ0.fxn = NULL,
    .commonCfg.IRQcallbacks.endOfFrameIRQ0.arg = 0,
    .commonCfg.IRQcallbacks.endOfFrameIRQ1.fxn = NULL,
    .commonCfg.IRQcallbacks.endOfFrameIRQ1.arg = 0,

    .contextCfg.virtualChannelId = BOARD_DIAG_TEST_VC,
    .contextCfg.format = BOARD_DIAG_TEST_FORMAT,
    .contextCfg.userDefinedMapping = BOARD_DIAG_TEST_USER_DEFINED_MAPPING,
    .contextCfg.isByteSwapEnabled = false,
    .contextCfg.isGenericEnabled = false,
    .contextCfg.isTranscodingEnabled = false,
    .contextCfg.transcodeConfig.transcodeFormat =
                                CSIRX_TRANSCODE_FORMAT_NO_TRANSCODE,
    .contextCfg.transcodeConfig.isHorizontalDownscalingBy2Enabled = false,
    .contextCfg.transcodeConfig.crop.horizontalCount = 0,
    .contextCfg.transcodeConfig.crop.horizontalSkip = 0,
    .contextCfg.transcodeConfig.crop.verticalCount = 0,
    .contextCfg.transcodeConfig.crop.verticalSkip = 0,
    .contextCfg.alpha = 0,
    .contextCfg.pingPongConfig.pingPongSwitchMode = CSIRX_PING_PONG_FRAME_SWITCHING,
    .contextCfg.pingPongConfig.numFramesForFrameBasedPingPongSwitching = 1,
    .contextCfg.pingPongConfig.lineOffset =
                            CSIRX_LINEOFFSET_CONTIGUOUS_STORAGE,
    .contextCfg.pingPongConfig.pingAddress = NULL,
    .contextCfg.pingPongConfig.pongAddress = NULL,
    .contextCfg.numFramesToAcquire = CSIRX_NUM_FRAMES_TO_ACQUIRE_INFINITE,
    .contextCfg.IRQ.isNumLines = false,
    .contextCfg.isNumLinesForIRQmoduloWithinFrame = false,
    .contextCfg.numLinesForIRQ = 0,
    .contextCfg.IRQ.isFramesToAcquire = false,
    .contextCfg.IRQ.isPayloadChecksumMismatch = false,
    .contextCfg.IRQ.isFrameStartCodeDetect = true,
    .contextCfg.IRQ.isFrameEndCodeDetect = true,
    .contextCfg.IRQ.isLineStartCodeDetect = false,
    .contextCfg.IRQ.isLineEndCodeDetect = true,
    .contextCfg.IRQ.isECConeBitLongPacketCorrect = false,
    .contextCfg.endOfLineIRQcallback.fxn = NULL,
    .contextCfg.endOfLineIRQcallback.arg = NULL,
    .contextCfg.isEndOfFramePulseEnabled = true,
    .contextCfg.isEndOfLinePulseEnabled = false,
    .contextCfg.isChecksumEnabled = true
};

/**
 *  \brief    Initialize the ping pong buffers to reset values
 *  \param    pingBuf   [OUT]    Ping buffer
 *            pongBuf   [OUT]    Pong buffer
 *            sizeBuf   [OUT]    Size of the buffer
 *
 * \retval
 *      none
 */
void BoardDiag_InitBuf(uint32_t pingBuf, uint32_t pongBuf, uint32_t sizeBuf)
{
    /* initialize ping/pong bufs to known failing pattern */
    memset((void *)pingBuf, BOARD_DIAG_TEST_BUF_INIT_PATTERN, sizeBuf);
    CacheP_wbInv((void *)pingBuf, sizeBuf);
    memset((void *)pongBuf, BOARD_DIAG_TEST_BUF_INIT_PATTERN, sizeBuf);
    CacheP_wbInv((void *)pongBuf, sizeBuf);
}

/**
 *  \brief    This function initializes test state variable.
 */
void BoardDiag_TestInit(void)
{
    memset(&gTestState, 0, sizeof(gTestState));

    gTestState.isReceivedPayloadCorrect = true;

}

/**
 *  \brief    Gets test buffer address from a bunch of input parameters
 *  \param    BoardDiag_RAMtype   [IN]    Type of buffer RAM
 *            buf                 [OUT]   Pointer to where the buffer address is returned
 *            isPing              [IN]    true if ping buffer else pong buffer
 *
 *
 *  \retval
 *      none
 */
void BoardDiag_getBuf(BoardDiag_RAMtype bufRAMtype, uint32_t *buf, bool isPing)
{
    if(isPing == true)
    {
        switch(bufRAMtype)
        {
        case BOARD_DIAG_L3RAM:
            *buf = (uint32_t) &testPingBufL3;
            break;
        case BOARD_DIAG_HWARAM:
            *buf = (uint32_t) &testPingBufHWA;
            break;
        case BOARD_DIAG_L2RAM:
            *buf = (uint32_t) &testPingBufL2;
            break;
        }
    }
    else
    {
        switch(bufRAMtype)
        {
        case BOARD_DIAG_L3RAM:
            *buf = (uint32_t) &testPongBufL3;
            break;
        case BOARD_DIAG_HWARAM:
            *buf = (uint32_t) &testPongBufHWA;
            break;
        case BOARD_DIAG_L2RAM:
            *buf = (uint32_t) &testPongBufL2;
            break;
        }
    }
}

/**
 *  \brief    The function performs the CSI-Rx Diagnostic
 *            test.
 *
 *  \return   int8_t
 *               0 - in case of success
 *              -1 - in case of failure.
 *
 */
bool BoardDiag_CsirxTestRun(uint8_t instanceId)
{
    CSIRX_Handle         handle;
    int32_t errorCode;
    uint32_t pingBuf, pongBuf;
    CSIRX_InstanceInfo_t instanceInfo;
    bool isTestPass = true;
    CSL_rcss_rcmRegs *rcss_rcm = (CSL_rcss_rcmRegs *)CSL_RCSS_RCM_U_BASE;
    volatile bool isComplexIOresetDone, isForceRxModeDeasserted;
    volatile uint32_t numComplexIOresetDonePolls, numComplexIOPowerStatusPolls,
             numForceRxModeDeassertedPolls;
    volatile uint8_t isComplexIOpowerStatus;
    volatile bool isForceRxModeOnComplexIOdeasserted;

    /* get ping-pong buffer addresses based on the RAM type and context */
    BoardDiag_getBuf(BOARD_DIAG_HWARAM, &pingBuf, true);
    BoardDiag_getBuf(BOARD_DIAG_HWARAM, &pongBuf, false);

    BoardDiag_InitBuf(pingBuf, pongBuf, BOARD_DIAG_INIT_PATTERN_SIZE);
    /* initialize the ping-pong buffers */
    BoardDiag_TestInit();

    CSL_FINS(rcss_rcm->RCSS_CSI2A_RST_CTRL, RCSS_RCM_RCSS_CSI2A_RST_CTRL_RCSS_CSI2A_RST_CTRL_ASSERT, 0x7);

    CSL_FINS(rcss_rcm->RCSS_CSI2A_RST_CTRL, RCSS_RCM_RCSS_CSI2A_RST_CTRL_RCSS_CSI2A_RST_CTRL_ASSERT, 0U);


    /* Initialize CSIRX */
    errorCode = CSIRX_init();
    if(errorCode != CSIRX_NO_ERROR)
    {
        DebugP_log1("CSIRX_init failed with errorCode = %d\n", errorCode);
        isTestPass = false;
        return isTestPass;
    }

    /* Open the CSI Instance */
    handle = CSIRX_open(instanceId, NULL, &errorCode, &instanceInfo);
    if(handle == NULL)
    {
        if(errorCode == CSIRX_E_INVALID__INSTANCE_ID)
        {
            printf("Csirx Instance not supported\n");
        }
        else
        {
            printf("Unable to open the csirx Instance, erorCode = %d\n", errorCode);
        }
        isTestPass = false;
        return isTestPass;
    }
    DebugP_log3("Instance opened, Revision = %d.%d, Number of "
            "Contexts = %d\n", instanceInfo.majorRevisionId,
            instanceInfo.minorRevisionId,
            instanceInfo.numContexts);

    /* reset csi */
    errorCode = CSIRX_reset(handle);
    if(errorCode != CSIRX_NO_ERROR)
    {
        DebugP_log1("CSIRX_reset failed, errorCode = %d\n", errorCode);
        isTestPass = false;
        return isTestPass;
    }
    /* config complex IO - lanes and IRQ */
    errorCode = CSIRX_configComplexIO(handle, &testConfig.complexIOcfg);
    if(errorCode != CSIRX_NO_ERROR)
    {
        DebugP_log1("CSIRX_configComplexIO failed, errorCode = %d\n",
                    errorCode);
        isTestPass = false;
        return isTestPass;
    }

    /* deassert complex IO reset */
    errorCode = CSIRX_deassertComplexIOreset(handle);
    if(errorCode != CSIRX_NO_ERROR)
    {
        DebugP_log1("CSIRX_deassertComplexIOreset failed, errorCode = %d\n",
                    errorCode);
        isTestPass = false;
        return isTestPass;
    }

    /* config DPHY */
    errorCode = CSIRX_configDPHY(handle, &testConfig.DPHYcfg);
    if(errorCode != CSIRX_NO_ERROR)
    {
        DebugP_log1("CSIRX_configDPHY failed, errorCode = %d\n", errorCode);
        isTestPass = false;
        return isTestPass;
    }

    errorCode = CSIRX_setComplexIOpowerCommand(handle, 1);
    if(errorCode != CSIRX_NO_ERROR)
    {
        DebugP_log1("CSIRX_setComplexIOpowerCommand failed, errorCode = %d\n",
                    errorCode);
        isTestPass = false;
        return isTestPass;
    }

    numComplexIOPowerStatusPolls = 0;
    do
    {
        errorCode = CSIRX_getComplexIOpowerStatus(handle,
                                            (uint8_t*)&isComplexIOpowerStatus);
        if(errorCode != CSIRX_NO_ERROR)
        {
            DebugP_log1("CSIRX_getComplexIOpowerStatus failed, errorCode = "
            " %d\n", errorCode);
            isTestPass = false;
        return isTestPass;
        }
        if (isComplexIOpowerStatus == 0)
        {
            Osal_delay(1);
        }
        numComplexIOPowerStatusPolls++;
    } while((isComplexIOpowerStatus == 0));
    
    /* config common */
    testConfig.commonCfg.IRQ.isContext[BOARD_DIAG_TEST_CONTEXT] = true,
    errorCode = CSIRX_configCommon(handle, &testConfig.commonCfg);
    if(errorCode != CSIRX_NO_ERROR)
    {
        DebugP_log1("CSIRX_configCommon failed, errorCode = %d\n", errorCode);
        isTestPass = false;
        return isTestPass;
    }

    /* config contexts */
    /* assign ping pong address */
    testConfig.contextCfg.pingPongConfig.pingAddress =
            (uint32_t) CSL_locToGlobAddr(pingBuf);
    testConfig.contextCfg.pingPongConfig.pongAddress =
            (uint32_t) CSL_locToGlobAddr(pongBuf);

    errorCode = CSIRX_configContext(handle, BOARD_DIAG_TEST_CONTEXT,
                                &testConfig.contextCfg);
    if(errorCode != CSIRX_NO_ERROR)
    {
        DebugP_log1("CSIRX_configContext failed, errorCode = %d\n", errorCode);
        
        isTestPass = false;
        return isTestPass;
    }

    /* enable context */
    errorCode = CSIRX_enableContext(handle, BOARD_DIAG_TEST_CONTEXT);
    if(errorCode != CSIRX_NO_ERROR)
    {
        DebugP_log1("CSIRX_enableContext failed, errorCode = %d\n", errorCode);
        isTestPass = false;
        return isTestPass;
    }

    /* enable interface */
    errorCode = CSIRX_enableInterface(handle);
    if(errorCode != CSIRX_NO_ERROR)
    {
        DebugP_log1("CSIRX_enableInterface failed, errorCode = %d\n",
                    errorCode);
        isTestPass = false;
        return isTestPass;
    }

    /* Wait until complex IO reset complete */
    numComplexIOresetDonePolls = 0;
    do
    {
        errorCode = CSIRX_isComplexIOresetDone(handle,
                                            (bool *)&isComplexIOresetDone);
        if(errorCode != CSIRX_NO_ERROR)
        {
            DebugP_log1("CSIRX_isComplexIOresetDone failed, errorCode = "
            " %d\n", errorCode);
            isTestPass = false;
            return isTestPass;
        }
        if (isComplexIOresetDone == false)
        {
            /* NOTE: This delay should be much smaller than frame time, default BIOS tick = 1 ms */
            Osal_delay(1);
        }
        numComplexIOresetDonePolls++;
    }while((isComplexIOresetDone == false));

    if(isComplexIOresetDone == false)
    {
        DebugP_log0("CSIRX_isComplexIOresetDone attempts exceeded\n");
        isTestPass = false;
        return isTestPass;
    }

    /*Wait csirx receive the data */
    while(gFrameCounter !=
          BOARD_DIAG_TEST_NUM_FRAMES)
    {
        if(gFrameReceived)
        {
            /* TODO: Added global variable counter to check the while loop is not optimized */
            gFrameCounter++;
            BoardDiag_CheckPayloadReceived(handle);
            DebugP_log1("Frame - %d received\n" ,
            gTestState.contextIRQcounts[BOARD_DIAG_TEST_CONTEXT].frameEndCodeDetect);
            gFrameReceived = false;

            /* Test is considered as failed even if any one of the frame
               received is invalid */
            if(isTestPass != false)
            {
                isTestPass = isTestPass & gTestState.isReceivedPayloadCorrect;
            }
        }
        Osal_delay(1);
    }

    if(gFrameCounter != BOARD_DIAG_TEST_NUM_FRAMES)
    {
        DebugP_log0("Number of frames recieved does not match\n");
        isTestPass = false;
    }

    BoardDiag_CheckStateError(&isTestPass);

    /* disable context */
    errorCode = CSIRX_disableContext(handle, BOARD_DIAG_TEST_CONTEXT);
    if(errorCode != CSIRX_NO_ERROR)
    {
        DebugP_log1("CSIRX_disableContext failed,errorCode = %d\n", errorCode);
        isTestPass = false;
        return isTestPass;
    }

    /* disable interface */
    errorCode = CSIRX_disableInterface(handle);
    if(errorCode != CSIRX_NO_ERROR)
    {
        DebugP_log1("CSIRX_disableInterface failed, errorCode = %d\n",
                    errorCode);
        isTestPass = false;
        return isTestPass;
    }

    /* close instance */
    errorCode = CSIRX_close(handle);
    if(errorCode != CSIRX_NO_ERROR)
    {
        DebugP_log1("CSIRX_close failed, errorCode = %d\n", errorCode);
        isTestPass = false;
        return isTestPass;
    }

    return(isTestPass);
}

/**
 *  \brief    The function performs the CSIRX Diagnostic
 *            test.
 *
 *  \return   int8_t
 *               0 - in case of success
 *              -1 - in case of failure.
 *
 */
int8_t BoardDiag_CsirxTest()
{
    uint8_t result;
    /* TPR12_TODO: Update this to menu based after initial testing */
#if defined (BOARD_DIAG_CSIRX_A_TEST)
    uint8_t instanceId = CSIRX_INST_ID_FIRST;
#else
    uint8_t instanceId = CSIRX_INST_ID_LAST;
#endif
    char instName[25];

    UART_printf("\n**********************************************\n");
    UART_printf  ("*                CSI-Rx Test                 *\n");
    UART_printf  ("**********************************************\n");

    CSIRX_getInstanceName(instanceId, &instName[0], sizeof(instName));

    UART_printf("Receiving data from CSIRX instance #%d: %s\n", instanceId,
                 instName);

    result = BoardDiag_CsirxTestRun(instanceId);
    if(result != true)
    {
        UART_printf("Failed to receive data from CSIRX instance #%d\n",
                    instanceId);
        return -1;
    }
    else
    {
        UART_printf("CSIRX diag test passed\n");
    }
    UART_printf("csirx test finished\n");

    return 0;

}

/**
 *  \brief   CSIRX Diagnostic test main function
 *
 *  \return  int - CSIRX Diagnostic test status.
 *             0 - in case of success
 *            -1 - in case of failure.
 *
 */
int main (void)
{
    Board_STATUS status;
    Board_initCfg boardCfg;
    int8_t ret;

#ifdef PDK_RAW_BOOT
    boardCfg = BOARD_INIT_MODULE_CLOCK |
               BOARD_INIT_PINMUX_CONFIG |
               BOARD_INIT_UART_STDIO;
#else
    boardCfg = BOARD_INIT_PINMUX_CONFIG | BOARD_INIT_UART_STDIO | BOARD_INIT_MODULE_CLOCK;
#endif

    status = Board_init(boardCfg);
    if(status != BOARD_SOK)
    {
        return -1;
    }

    ret = BoardDiag_CsirxTest();
    if(ret != 0)
    {
        UART_printf("\nCSIRX Test Failed\n");
        return -1;
    }
    else
    {
        UART_printf("\nCSIRX Test Passed\n");
    }

    return 0;
}
