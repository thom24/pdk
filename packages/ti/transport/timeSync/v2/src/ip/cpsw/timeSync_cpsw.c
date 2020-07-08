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

/*!
 * \file timeSync_cpsw.c
 *
 * \brief This file contains the implemenation of CPSW specific
 *        funcions for PTP stack
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

/* XDCtools Header files */
#include <xdc/std.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/System.h>
#include <ti/drv/sciclient/sciclient.h>

/* BIOS Header files */
#include <ti/sysbios/BIOS.h>

#include <ti/drv/cpsw/cpsw.h>
#include <ti/drv/cpsw/examples/cpsw_apputils/inc/cpsw_appmemutils_cfg.h>
#include <ti/drv/cpsw/examples/cpsw_apputils/inc/cpsw_appmemutils.h>
#include <ti/drv/cpsw/examples/cpsw_apputils/inc/cpsw_apputils.h>
#include <ti/drv/cpsw/examples/cpsw_apputils/inc/cpsw_appsoc.h>
#include <ti/drv/cpsw/examples/cpsw_apputils/inc/cpswapp_ethutils.h>
#include <ti/drv/cpsw/examples/cpsw_apputils/inc/cpsw_mcm.h>

#include <ti/transport/timeSync/v2/include/timeSync.h>
#include <ti/transport/timeSync/v2/include/timeSync_tools.h>

/* ========================================================================== */
/*                                 Macros                                     */
/* ========================================================================== */

#define TIMESYNC_TX_PKTINFO_COUNT           (64U)

/**
 * @def TIMESYNC_PTP_MSG_ID_OFFSET
 *      PTP message ID offset
 */
#define TIMESYNC_PTP_MSG_ID_OFFSET          (14U)

/**
 * @def TIMESYNC_PTP_SEQ_ID_OFFSET
 *      Offset for sequence identifier (incremented every frame)
 */
#define TIMESYNC_PTP_SEQ_ID_OFFSET          (44U)

/**
 * @def TIMESYNC_ANNEX_F_HEADER_SIZE
 *      Number of bytes in Annex F(bare Ethernet) header
 */
#define TIMESYNC_ANNEX_F_HEADER_SIZE        (14U)

/**
 * @def TIMESYNC_ANNEX_D_ANNEX_F_DIFF
 *      Number of extra bytes for Annex D(IPv4) from Annex F(802.3)
 */
#define TIMESYNC_ANNEX_D_ANNEX_F_DIFF       (28U)

/**
 * @def TIMESYNC_ANNEX_E_ANNEX_F_DIFF
 *      Number of extra bytes for Annex E(IPv6) from Annex F(802.3)
 */
#define TIMESYNC_ANNEX_E_ANNEX_F_DIFF       (48U)

/**
 * @def TIMESYNC_SINGLE_TAG_VLAN_HDR_SIZE
 *      Number of extra bytes for single tagged VLAN
 */
#define TIMESYNC_SINGLE_TAG_VLAN_HDR_SIZE   (4U)

/**
 * @def TIMESYNC_DOUBLE_TAG_VLAN_HDR_SIZE
 *      Number of extra bytes for double tagged VLAN
 */
#define TIMESYNC_DOUBLE_TAG_VLAN_HDR_SIZE   (8U)

/**
 * @def TIMESYNC_PPS_HW_PUSH_NUM
 *      CPTS hardware push instance used for PPS
 */
#define TIMESYNC_PPS_HW_PUSH_NUM            (CPSW_CPTS_HW_PUSH_1)

/**
 * @def TIMESYNC_MILLION_PARTS
 *      Value of million useful in PPM calculations
 */
#define TIMESYNC_PPM_VALUE                  (1000000U)

/* ========================================================================== */
/*                         Structures and Enums                               */
/* ========================================================================== */

typedef struct TimeSync_TxPktInfoEle_s
{
    uint8_t portNum;

    uint8_t frameType;

    uint16_t seqId;
} TimeSync_TxPktInfoEle;

typedef struct TimeSync_TxPktInfo_s
{
    TimeSync_TxPktInfoEle txTsPktInfoArr[TIMESYNC_TX_PKTINFO_COUNT];

    uint32_t rdIdx;

    uint32_t wrIdx;
}TimeSync_TxPktInfo;

typedef struct TimeSync_Obj_s
{
    TimeSync_Config timeSyncConfig;

    Cpsw_Type cpswType;

    Cpsw_Handle hCpsw;

    Udma_DrvHandle hUdmaDrv;

    CpswDma_RxFlowHandle hRxFlow;

    uint32_t rxFlowStartIdx;

    uint32_t rxFlowIdx;

    CpswDma_TxChHandle hTxCh;

    uint32_t txChNum;

    uint32_t coreId;

    uint32_t coreKey;

    CpswMcm_CmdIf hMcmCmdIf;

    CpswDma_PktInfoQ rxFreeQ;

    CpswDma_PktInfoQ rxReadyQ;

    CpswDma_PktInfoQ txFreePktInfoQ;

    TimeSync_TxPktInfo txTsPktInfo;
} TimeSync_Obj;

/* ========================================================================== */
/*                 Internal Function Declarations                             */
/* ========================================================================== */

static int32_t TimeSync_openDma(void);

static void TimeSync_closeDma(void);

static void TimeSync_initTxFreePktQ(void);

static void TimeSync_initRxReadyPktQ(void);

static int32_t TimeSync_setCpswAleClassifier(void);

static int32_t TimeSync_setMacPortConfig(void);

static void TimeSync_setPortTsEventPrms(CpswMacPort_TsEventConfig *tsPortEventCfg);

static uint32_t TimeSync_retrieveTxDonePkts(void);

static uint32_t TimeSync_receiveRxReadyPkts(void);

static void TimeSync_getMsgId(uint8_t *msgId,
                              uint8_t *frame);

static void TimeSync_getSeqId(uint16_t *seqId,
                              uint8_t *frame);

static int32_t TimeSync_configPps(void);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

static TimeSync_Obj gTimeSyncCpswObj;

/**PTP Multicast addresses for comparison*/
static const uint8_t peerDlyMsgMAC[TIMESYNC_MAC_ADDR_LEN] = {0x01, 0x80, 0xc2, 0x00, 0x00, 0x0E};
static const uint8_t nonPeerDlyMsgMAC[TIMESYNC_MAC_ADDR_LEN] = {0x01, 0x1b, 0x19, 0x00, 0x00, 0x00};

static const uint8_t peerDlyMsgIpv4[TIMESYNC_IPV4_ADDR_LEN] = {224U, 0U, 0U, 107U};
static const uint8_t nonPeerDlyMsgIpv4[TIMESYNC_IPV4_ADDR_LEN] = {224U, 0U, 1U, 129U};

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

void TimeSync_rxNotifyFxn(void *appData)
{
    TimeSync_Handle timeSyncHandle = NULL;
    TimeSync_FrameNotifyConfig *frameNotifyCfg = NULL;
    uint32_t rxReadyCnt;
    uint32_t i = 0U;

    if (appData != NULL)
    {
        timeSyncHandle = (TimeSync_Handle)appData;
        frameNotifyCfg = &timeSyncHandle->timeSyncConfig.frameNotifyCfg;

        rxReadyCnt = TimeSync_receiveRxReadyPkts();
        for (i = 0U; i < rxReadyCnt; i++)
        {
            /* Notify PTP Rx callback */
            if (frameNotifyCfg->rxNotifyCb != NULL)
            {
                frameNotifyCfg->rxNotifyCb(frameNotifyCfg->rxNotifyCbArg);
            }
        }
    }
}

void TimeSync_txNotifyFxn(void *appData)
{
    uint32_t pktCount;
    TimeSync_TxPktInfo *pTxTsPktInfo = NULL;
    TimeSync_TxPktInfoEle *pTxTsPktInfoEle = NULL;
    TimeSync_Handle timeSyncHandle = NULL;
    TimeSync_FrameNotifyConfig *frameNotifyCfg = NULL;

    if (appData != NULL)
    {
        timeSyncHandle = (TimeSync_Handle)appData;
        frameNotifyCfg = &timeSyncHandle->timeSyncConfig.frameNotifyCfg;

        pktCount = TimeSync_retrieveTxDonePkts();
        CpswAppUtils_assert(pktCount != 0U);

        pTxTsPktInfo = &timeSyncHandle->txTsPktInfo;
        pTxTsPktInfoEle = &pTxTsPktInfo->txTsPktInfoArr[pTxTsPktInfo->rdIdx++];
        pTxTsPktInfo->rdIdx %= TIMESYNC_TX_PKTINFO_COUNT;

        /* Notify PTP Tx callback */
        if (frameNotifyCfg->txNotifyCb != NULL)
        {
            frameNotifyCfg->txNotifyCb(frameNotifyCfg->txNotifyCbArg,
                                       pTxTsPktInfoEle->portNum,
                                       pTxTsPktInfoEle->frameType,
                                       pTxTsPktInfoEle->seqId);
        }
    }
}

void TimeSync_ppsHwPushNotifyFxn(void *cbArg,
                                 CpswCpts_HwPush hwPushNum)
{
    TimeSync_Handle timeSyncHandle = NULL;
    TimeSync_PpsConfig *pPpsConfig = NULL;

    if (cbArg != NULL)
    {
        timeSyncHandle = (TimeSync_Handle)cbArg;

        if (hwPushNum == TIMESYNC_PPS_HW_PUSH_NUM)
        {
            /* Call PPS callback */
            pPpsConfig = &timeSyncHandle->timeSyncConfig.protoCfg.ppsConfig;
            if (pPpsConfig->ppsNotifyCb != NULL)
            {
                pPpsConfig->ppsNotifyCb(pPpsConfig->ppsNotifyCbArg);
            }
        }
    }
}

TimeSync_Handle TimeSync_open(TimeSync_Config *timeSyncConfig)
{
    int32_t status = TIMESYNC_OK;
    CpswMcm_HandleInfo handleInfo;
    Cpsw_AttachCoreOutArgs attachInfo;

    if (timeSyncConfig != NULL)
    {
        memset(&gTimeSyncCpswObj, 0, sizeof(gTimeSyncCpswObj));

        /*Copy timeSyncConfig to handle */
        gTimeSyncCpswObj.timeSyncConfig = *timeSyncConfig;

        if (timeSyncConfig->socConfig.ipVersion == TIMESYNC_IP_VER_CPSW_2G)
        {
            gTimeSyncCpswObj.cpswType = CPSW_2G;
        }
        else if (timeSyncConfig->socConfig.ipVersion == TIMESYNC_IP_VER_CPSW_9G)
        {
            gTimeSyncCpswObj.cpswType = CPSW_9G;
        }
        else if (timeSyncConfig->socConfig.ipVersion == TIMESYNC_IP_VER_CPSW_5G)
        {
            gTimeSyncCpswObj.cpswType = CPSW_5G;
        }
        else
        {
            status = TIMESYNC_PARAM_INVALID;
        }

        CpswAppUtils_assert(status == TIMESYNC_OK);

        gTimeSyncCpswObj.coreId = CpswAppSoc_getCoreId();

        CpswMcm_getCmdIf(gTimeSyncCpswObj.cpswType, &gTimeSyncCpswObj.hMcmCmdIf);
        CpswAppUtils_assert(gTimeSyncCpswObj.hMcmCmdIf.hMboxCmd != NULL);
        CpswAppUtils_assert(gTimeSyncCpswObj.hMcmCmdIf.hMboxResponse != NULL);

        CpswMcm_acquireHandleInfo(&gTimeSyncCpswObj.hMcmCmdIf,
                                  &handleInfo);
        CpswMcm_coreAttach(&gTimeSyncCpswObj.hMcmCmdIf,
                           gTimeSyncCpswObj.coreId,
                           &attachInfo);

        gTimeSyncCpswObj.hCpsw = handleInfo.hCpsw;
        gTimeSyncCpswObj.hUdmaDrv = handleInfo.hUdmaDrv;
        gTimeSyncCpswObj.coreKey  = attachInfo.coreKey;

        if (gTimeSyncCpswObj.hCpsw == NULL)
        {
            CpswAppUtils_print("Failed to open CPSW\n");
            CpswAppUtils_assert(gTimeSyncCpswObj.hCpsw == NULL);
        }

        if (gTimeSyncCpswObj.hUdmaDrv == NULL)
        {
            CpswAppUtils_print("Failed to Get UDMA Handle\n");
            CpswAppUtils_assert(gTimeSyncCpswObj.hUdmaDrv == NULL);
        }

        if (status == TIMESYNC_OK)
        {
            /* MAC port config */
            status = TimeSync_setMacPortConfig();

            if (status != TIMESYNC_OK)
            {
                CpswAppUtils_print("Failed to configure MAC for timestampping: %d\n", status);
            }
        }

        if (status == TIMESYNC_OK)
        {
            status = TimeSync_openDma();
            if (status != TIMESYNC_OK)
            {
                CpswAppUtils_print("Failed to open CPSW DMA: %d\n", status);
            }
        }

        if (status == TIMESYNC_OK)
        {
            /* Based on network protocol type set classifier */
            status = TimeSync_setCpswAleClassifier();
            if (status != TIMESYNC_OK)
            {
                CpswAppUtils_print("Failed to set Classifier: %d\n", status);
            }
        }

        if (status == TIMESYNC_OK)
        {
            status = TimeSync_configPps();
            if (status != TIMESYNC_OK)
            {
                CpswAppUtils_print("Failed to configure PPS generation: %d\n", status);
            }
        }

        /* If open failed, call close() to release resources */
        if (status != TIMESYNC_OK)
        {
            TimeSync_close(&gTimeSyncCpswObj);
        }
    }
    else
    {
        status = TIMESYNC_PARAM_INVALID;
    }

    return (status == TIMESYNC_OK ? (&gTimeSyncCpswObj) : NULL);
}

int32_t TimeSync_getRxTimestamp(TimeSync_Handle timeSyncHandle,
                                TimeSync_FrameType rxFrameType,
                                uint8_t rxPort,
                                uint16_t seqId,
                                uint32_t *nanoseconds,
                                uint64_t *seconds)
{
    int32_t status = TIMESYNC_OK;
    Cpsw_IoctlPrms prms;
    CpswCpts_LookUpEventInArgs lookupEventInArgs;
    CpswCpts_Event lookupEventOutArgs;


    if (timeSyncHandle != NULL)
    {
        lookupEventInArgs.eventType = CPSW_CPTS_EVENTTYPE_ETH_RECEIVE;
        lookupEventInArgs.hwPushNum = CPSW_CPTS_HW_PUSH_INVALID;
        lookupEventInArgs.portNum = rxPort;
        lookupEventInArgs.seqId = seqId;
        lookupEventInArgs.msgType = (CpswCpts_MsgType)rxFrameType;
        lookupEventInArgs.domain  = 0U;

        if (status == TIMESYNC_OK)
        {
            CPSW_IOCTL_SET_INOUT_ARGS(&prms, &lookupEventInArgs, &lookupEventOutArgs);
            status = Cpsw_ioctl(timeSyncHandle->hCpsw,
                                timeSyncHandle->coreId,
                                CPSW_CPTS_IOCTL_LOOKUP_EVENT,
                                &prms);
            if (status == CPSW_ENOTFOUND)
            {
                lookupEventOutArgs.tsVal = 0U;
            }
            *nanoseconds = (uint32_t)(lookupEventOutArgs.tsVal % (uint64_t)TIMESYNC_SEC_TO_NS);
            *seconds = lookupEventOutArgs.tsVal / (uint64_t)TIMESYNC_SEC_TO_NS;
        }
    }
    else
    {
        status = TIMESYNC_HANDLE_NOT_INITIALIZED;
    }

    return status;
}

int32_t TimeSync_getTxTimestamp(TimeSync_Handle timeSyncHandle,
                                TimeSync_FrameType txFrameType,
                                uint8_t txPort,
                                uint16_t seqId,
                                uint32_t *nanoseconds,
                                uint64_t *seconds)
{
    int32_t status = TIMESYNC_OK;
    Cpsw_IoctlPrms prms;
    CpswCpts_LookUpEventInArgs lookupEventInArgs;
    CpswCpts_Event lookupEventOutArgs;

    if (timeSyncHandle != NULL)
    {
        lookupEventInArgs.eventType = CPSW_CPTS_EVENTTYPE_ETH_TRANSMIT;
        lookupEventInArgs.hwPushNum = CPSW_CPTS_HW_PUSH_INVALID;
        lookupEventInArgs.portNum = txPort;
        lookupEventInArgs.seqId = seqId;
        lookupEventInArgs.msgType = (CpswCpts_MsgType)txFrameType;
        lookupEventInArgs.domain  = 0U;

        if (status == TIMESYNC_OK)
        {
            CPSW_IOCTL_SET_INOUT_ARGS(&prms, &lookupEventInArgs, &lookupEventOutArgs);
            status = Cpsw_ioctl(timeSyncHandle->hCpsw,
                                timeSyncHandle->coreId,
                                CPSW_CPTS_IOCTL_LOOKUP_EVENT,
                                &prms);
            if (status == CPSW_ENOTFOUND)
            {
                lookupEventOutArgs.tsVal = 0U;
            }
            *nanoseconds = (uint32_t)(lookupEventOutArgs.tsVal % (uint64_t)TIMESYNC_SEC_TO_NS);
            *seconds = lookupEventOutArgs.tsVal / (uint64_t)TIMESYNC_SEC_TO_NS;
        }
    }
    else
    {
        status = TIMESYNC_HANDLE_NOT_INITIALIZED;
    }

    return status;
}

int32_t TimeSync_adjTimeSlowComp(TimeSync_Handle timeSyncHandle,
                                 int32_t adjOffset,
                                 uint32_t interval)
{
    int32_t status = TIMESYNC_OK;
    Cpsw_IoctlPrms prms;
    CpswCpts_SetTsPpmInArgs setTsPpmInArgs;
    int32_t ppmVal = 0;

    ppmVal = ((double)(adjOffset) / (double)(interval)) * TIMESYNC_PPM_VALUE;

    if (timeSyncHandle != NULL)
    {
        setTsPpmInArgs.tsPpmMode = CPSW_CPTS_PPM_PARTS_PER_MILLION;
        if (ppmVal < 0)
        {
            setTsPpmInArgs.tsPpmDir = CPSW_CPTS_PPM_DIR_DECREASE;
            setTsPpmInArgs.tsPpmVal = (-1 * ppmVal);
        }
        else
        {
            setTsPpmInArgs.tsPpmDir = CPSW_CPTS_PPM_DIR_INCREASE;
            setTsPpmInArgs.tsPpmVal = ppmVal;
        }

        CPSW_IOCTL_SET_IN_ARGS(&prms, &setTsPpmInArgs);
        status = Cpsw_ioctl(timeSyncHandle->hCpsw,
                            timeSyncHandle->coreId,
                            CPSW_CPTS_IOCTL_SET_TS_PPM,
                            &prms);
        CpswAppUtils_assert(status == TIMESYNC_OK);
    }
    else
    {
        status = TIMESYNC_HANDLE_NOT_INITIALIZED;
    }

    return status;
}

int32_t TimeSync_setClockTime(TimeSync_Handle timeSyncHandle,
                              uint32_t nanoseconds,
                              uint64_t seconds)
{
    int32_t status = TIMESYNC_OK;
    Cpsw_IoctlPrms prms;
    uint64_t tsVal = 0U;

    if (timeSyncHandle != NULL)
    {
        tsVal = (uint64_t)(((uint64_t)seconds * (uint64_t)TIMESYNC_SEC_TO_NS) + nanoseconds);
        CPSW_IOCTL_SET_IN_ARGS(&prms, &tsVal);
        status = Cpsw_ioctl(timeSyncHandle->hCpsw,
                            timeSyncHandle->coreId,
                            CPSW_CPTS_IOCTL_LOAD_TIMESTAMP,
                            &prms);
        CpswAppUtils_assert(status == TIMESYNC_OK);
    }
    else
    {
        status = TIMESYNC_HANDLE_NOT_INITIALIZED;
    }

    return status;
}

void TimeSync_getCurrentTime(TimeSync_Handle timeSyncHandle,
                             uint32_t *nanoseconds,
                             uint64_t *seconds)
{
    int32_t status = TIMESYNC_OK;
    Cpsw_IoctlPrms prms;
    uint64_t tsVal = 0U;

    if (timeSyncHandle != NULL)
    {
        /* Software Time stamp Push event */
        CPSW_IOCTL_SET_OUT_ARGS(&prms, &tsVal);
        status = Cpsw_ioctl(timeSyncHandle->hCpsw,
                            timeSyncHandle->coreId,
                            CPSW_CPTS_IOCTL_PUSH_TIMESTAMP_EVENT,
                            &prms);
        CpswAppUtils_assert(status == TIMESYNC_OK);

        *nanoseconds = (uint32_t)(tsVal % (uint64_t)TIMESYNC_SEC_TO_NS);
        *seconds = tsVal / (uint64_t)TIMESYNC_SEC_TO_NS;
    }
    else
    {
        status = TIMESYNC_HANDLE_NOT_INITIALIZED;
        *nanoseconds = 0U;
        *seconds = 0U;
    }
}

int32_t TimeSync_getPtpFrame(TimeSync_Handle timeSyncHandle,
                             uint8_t *frame,
                             uint32_t *size,
                             uint8_t *rxPort)
{
    int32_t status = TIMESYNC_OK;
    CpswDma_PktInfo *pktInfo;
    EthFrame *rxFrame;
    uint32_t rxReadyCnt;

    if (timeSyncHandle != NULL)
    {
        rxReadyCnt = CpswUtils_getQCount(&gTimeSyncCpswObj.rxReadyQ);
        if (rxReadyCnt > 0U)
        {
            /* Consume the received packets and release them */
            pktInfo = (CpswDma_PktInfo *)CpswUtils_deQ(&gTimeSyncCpswObj.rxReadyQ);
            CpswUtils_checkPktState(&pktInfo->pktState,
                                    CPSW_PKTSTATE_MODULE_APP,
                                    CPSW_PKTSTATE_APP_WITH_READYQ,
                                    CPSW_PKTSTATE_APP_WITH_FREEQ);

            rxFrame = (EthFrame *)pktInfo->bufPtr;
            *size = pktInfo->userBufLen;
            memcpy(frame, rxFrame, *size);
            *rxPort = (uint8_t)pktInfo->rxPortNum;

            /* Release the received packet */
            CpswUtils_enQ(&gTimeSyncCpswObj.rxFreeQ, &pktInfo->node);

            CpswAppUtils_validatePacketState(&gTimeSyncCpswObj.rxFreeQ,
                                             CPSW_PKTSTATE_APP_WITH_FREEQ,
                                             CPSW_PKTSTATE_APP_WITH_DRIVER);

            CpswAppUtils_submitRxPackets(gTimeSyncCpswObj.hRxFlow,
                                         &gTimeSyncCpswObj.rxFreeQ);
        }
        else
        {
            *size = 0U;
            status = TIMESYNC_FRAME_NOT_AVAILABLE;
        }
    }
    else
    {
        status = TIMESYNC_HANDLE_NOT_INITIALIZED;
    }

    return status;
}

int32_t TimeSync_sendPtpFrame(TimeSync_Handle timeSyncHandle,
                              uint8_t *frame,
                              uint32_t size,
                              uint8_t txPort)
{
    int32_t status = TIMESYNC_OK;
    CpswDma_PktInfoQ txSubmitQ;
    CpswDma_PktInfo *pktInfo;
    uint8_t *txFrame;
    TimeSync_TxPktInfo *pTxTsPktInfo = NULL;
    TimeSync_TxPktInfoEle *pTxTsPktInfoEle = NULL;

    if (timeSyncHandle != NULL)
    {
        CpswUtils_initQ(&txSubmitQ);
        pktInfo = (CpswDma_PktInfo *)CpswUtils_deQ(&gTimeSyncCpswObj.txFreePktInfoQ);
        if (NULL != pktInfo)
        {
            txFrame = (uint8_t *)pktInfo->bufPtr;
            memcpy(txFrame, frame, size);
            pktInfo->userBufLen = size;
            pktInfo->appPriv = (void *)timeSyncHandle;
            pktInfo->txPortNum = (Cpsw_MacPort)CPSW_NORMALIZE_MACPORT(txPort);

            /* Save tx pkt info to re-use during notify callback */
            pTxTsPktInfo = &timeSyncHandle->txTsPktInfo;
            pTxTsPktInfoEle = &pTxTsPktInfo->txTsPktInfoArr[pTxTsPktInfo->wrIdx++];
            pTxTsPktInfoEle->portNum = txPort;
            TimeSync_getMsgId(&pTxTsPktInfoEle->frameType, frame);
            TimeSync_getSeqId(&pTxTsPktInfoEle->seqId, frame);
            pTxTsPktInfo->wrIdx %= TIMESYNC_TX_PKTINFO_COUNT;

            CpswUtils_checkPktState(&pktInfo->pktState,
                                    CPSW_PKTSTATE_MODULE_APP,
                                    CPSW_PKTSTATE_APP_WITH_FREEQ,
                                    CPSW_PKTSTATE_APP_WITH_DRIVER);
            CpswUtils_enQ(&txSubmitQ, &pktInfo->node);

            if (0U != CpswUtils_getQCount(&txSubmitQ))
            {
                status = CpswAppUtils_submitTxPackets(gTimeSyncCpswObj.hTxCh,
                                                      &txSubmitQ);
            }
        }
    }
    else
    {
        status = TIMESYNC_HANDLE_NOT_INITIALIZED;
    }

    return status;
}

int32_t TimeSync_getPpsTs(TimeSync_Handle timeSyncHandle,
                          uint32_t *nanoseconds,
                          uint64_t *seconds)
{
    int32_t status = TIMESYNC_OK;
    Cpsw_IoctlPrms prms;
    CpswCpts_LookUpEventInArgs lookupEventInArgs;
    CpswCpts_Event lookupEventOutArgs;

    if (timeSyncHandle != NULL)
    {
        lookupEventInArgs.eventType = CPSW_CPTS_EVENTTYPE_HW_TS_PUSH;
        lookupEventInArgs.hwPushNum = TIMESYNC_PPS_HW_PUSH_NUM;
        lookupEventInArgs.portNum = 0U;
        lookupEventInArgs.seqId = 0U;
        lookupEventInArgs.domain  = 0U;

        CPSW_IOCTL_SET_INOUT_ARGS(&prms, &lookupEventInArgs, &lookupEventOutArgs);
        status = Cpsw_ioctl(timeSyncHandle->hCpsw,
                            timeSyncHandle->coreId,
                            CPSW_CPTS_IOCTL_LOOKUP_EVENT,
                            &prms);
        if (status == CPSW_ENOTFOUND)
        {
            lookupEventOutArgs.tsVal = 0U;
        }
        *nanoseconds = (uint32_t)(lookupEventOutArgs.tsVal % (uint64_t)TIMESYNC_SEC_TO_NS);
        *seconds = lookupEventOutArgs.tsVal / (uint64_t)TIMESYNC_SEC_TO_NS;
    }
    else
    {
        status = TIMESYNC_HANDLE_NOT_INITIALIZED;
    }

    return status;
}

void TimeSync_recalibratePps(TimeSync_Handle timeSyncHandle)
{
    int32_t status = TIMESYNC_OK;
    Cpsw_IoctlPrms prms;
    CpswCpts_SetFxnGenInArgs setGenFInArgs;
    uint64_t currentTime = 0U;
    uint64_t remainder = 0U;

    if (timeSyncHandle != NULL)
    {
        /*Disable GENF0 used for PPS */
        setGenFInArgs.index  = 0U;
        setGenFInArgs.length = 0U;
        CPSW_IOCTL_SET_IN_ARGS(&prms, &setGenFInArgs);
        status = Cpsw_ioctl(timeSyncHandle->hCpsw,
                            timeSyncHandle->coreId,
                            CPSW_CPTS_IOCTL_SET_GENF,
                            &prms);

        /* Not supported is not an error, so set the status
         * to TIMESYNC_OK */
        if (status == CPSW_ENOTSUPPORTED)
        {
            status = TIMESYNC_OK;
        }

        CpswAppUtils_assert(status == TIMESYNC_OK);

        /* Get current time */
        CPSW_IOCTL_SET_OUT_ARGS(&prms, &currentTime);
        status = Cpsw_ioctl(timeSyncHandle->hCpsw,
                            timeSyncHandle->coreId,
                            CPSW_CPTS_IOCTL_PUSH_TIMESTAMP_EVENT,
                            &prms);
        CpswAppUtils_assert(status == TIMESYNC_OK);

        /*We need to make sure that PPS aligns with the start of second boundary
         * This is done by finding the distance to second boundary and setting GENF compare
         * to that value. An extra second is added as a margin of safety*/
        remainder = currentTime % (uint64_t)TIMESYNC_SEC_TO_NS;
        currentTime += (TIMESYNC_SEC_TO_NS - remainder) + TIMESYNC_SEC_TO_NS;

        /* Configure GENF compare value and re-enable GENF */
        setGenFInArgs.index   = 0U;
        setGenFInArgs.length  = gTimeSyncCpswObj.timeSyncConfig.protoCfg.ppsConfig.ppsPulseIntervalNs;
        setGenFInArgs.compare = currentTime;
        setGenFInArgs.polarityInv = true;
        setGenFInArgs.ppmVal  = 0U;
        setGenFInArgs.ppmDir  = CPSW_CPTS_PPM_DIR_INCREASE;
        setGenFInArgs.ppmMode = CPSW_CPTS_PPM_DISABLE;

        CPSW_IOCTL_SET_IN_ARGS(&prms, &setGenFInArgs);
        status = Cpsw_ioctl(gTimeSyncCpswObj.hCpsw,
                            gTimeSyncCpswObj.coreId,
                            CPSW_CPTS_IOCTL_SET_GENF,
                            &prms);

        /* Not supported is not an error, so set the status
         * to TIMESYNC_OK */
        if (status == CPSW_ENOTSUPPORTED)
        {
            status = TIMESYNC_OK;
        }

        CpswAppUtils_assert(status == TIMESYNC_OK);
    }

}

int8_t TimeSync_isPortLinkUp(TimeSync_Handle timeSyncHandle,
                             uint8_t portNum)
{
    int8_t isLinkUp = FALSE;
    bool isLinkUpFlag = false;

    if (timeSyncHandle != NULL)
    {
        isLinkUpFlag = CpswAppUtils_isPortLinkUp(timeSyncHandle->hCpsw,
                                                 timeSyncHandle->coreId,
                                                 (Cpsw_MacPort)(CPSW_NORMALIZE_MACPORT(portNum)));
    }

    if (isLinkUpFlag == true)
    {
        isLinkUp = TRUE;
    }

    return isLinkUp;
}

int32_t TimeSync_isSingleStepSupported(TimeSync_Handle timeSyncHandle)
{
    return FALSE;
}

void TimeSync_reset(TimeSync_Handle timeSyncHandle)
{
    int32_t status = TIMESYNC_OK;
    CpswDma_PktInfo *pktInfo;
    Cpsw_IoctlPrms prms;

    if (timeSyncHandle != NULL)
    {
        /* Disable Rx & Tx Events */
        status = CpswDma_disableRxEvent(timeSyncHandle->hRxFlow);
        if (status != TIMESYNC_OK)
        {
            CpswAppUtils_print("CpswDma_disableRxEvent() failed: %d\n", status);
        }

        status = CpswDma_disableTxEvent(timeSyncHandle->hTxCh);
        if (status != TIMESYNC_OK)
        {
            CpswAppUtils_print("CpswDma_disableTxEvent() failed: %d\n", status);
        }

        /* Clean the SW queues */
        if (status == TIMESYNC_OK)
        {
            pktInfo = (CpswDma_PktInfo *)CpswUtils_deQ(&gTimeSyncCpswObj.rxReadyQ);
            while (pktInfo != NULL)
            {
                /* Consume the received packets and release them */
                CpswUtils_checkPktState(&pktInfo->pktState,
                                        CPSW_PKTSTATE_MODULE_APP,
                                        CPSW_PKTSTATE_APP_WITH_READYQ,
                                        CPSW_PKTSTATE_APP_WITH_FREEQ);

                CpswUtils_enQ(&gTimeSyncCpswObj.rxFreeQ, &pktInfo->node);

                CpswAppUtils_validatePacketState(&gTimeSyncCpswObj.rxFreeQ,
                                                 CPSW_PKTSTATE_APP_WITH_FREEQ,
                                                 CPSW_PKTSTATE_APP_WITH_DRIVER);

                pktInfo = (CpswDma_PktInfo *)CpswUtils_deQ(&gTimeSyncCpswObj.rxReadyQ);
            }
            CpswAppUtils_submitRxPackets(gTimeSyncCpswObj.hRxFlow,
                                         &gTimeSyncCpswObj.rxFreeQ);

            TimeSync_retrieveTxDonePkts();

            /* Clear Timestamp pools */
            CPSW_IOCTL_SET_NO_ARGS(&prms);
            status = Cpsw_ioctl(timeSyncHandle->hCpsw,
                                timeSyncHandle->coreId,
                                CPSW_CPTS_IOCTL_CLEAR_ALL_EVENTS,
                                &prms);
            if (status != TIMESYNC_OK)
            {
                CpswAppUtils_print("CPSW_CPTS_IOCTL_CLEAR_ALL_EVENTS IOCTL failed: %d\n", status);
            }
        }

        /* Clear local book-keeping of Tx Info */
        if (status == TIMESYNC_OK)
        {
            memset(&timeSyncHandle->txTsPktInfo, 0, sizeof(timeSyncHandle->txTsPktInfo));
        }

        /* Enable Rx & Tx Events */
        if (status == TIMESYNC_OK)
        {
            status = CpswDma_enableRxEvent(timeSyncHandle->hRxFlow);
            if (status != TIMESYNC_OK)
            {
                CpswAppUtils_print("CpswDma_enableRxEvent() failed: %d\n", status);
            }

            status = CpswDma_enableTxEvent(timeSyncHandle->hTxCh);
            if (status != TIMESYNC_OK)
            {
                CpswAppUtils_print("CpswDma_enableTxEvent() failed: %d\n", status);
            }
        }
    }

}

void TimeSync_close(TimeSync_Handle timeSyncHandle)
{
    if (timeSyncHandle != NULL)
    {
        /* Close DMA */
        TimeSync_closeDma();

        /* Detach core from MCM */
        CpswMcm_coreDetach(&timeSyncHandle->hMcmCmdIf,
                           timeSyncHandle->coreId,
                           timeSyncHandle->coreKey);
        /* Release handle info */
        CpswMcm_releaseHandleInfo(&timeSyncHandle->hMcmCmdIf);

    }
}

/* ========================================================================== */
/*                        IP-specific function declarations                  */
/* ========================================================================== */

void TimeSync_updateParentAddress(TimeSync_Handle timeSyncHandle,
                                  uint8_t *parentMac)
{
    /* Nothing to be done for CPSW Ethfw */
}

void TimeSync_setRcf(TimeSync_Handle timeSyncHandle,
                     uint32_t rcfValue)
{
    /* Nothing to be done for CPSW Ethfw */
}

void TimeSync_setPathDelay(TimeSync_Handle timeSyncHandle,
                           uint32_t pathDelay,
                           uint8_t portNum)
{
    /* Nothing to be done for CPSW Ethfw */
}

void TimeSync_writeTsSingleStepSync(TimeSync_Handle timeSyncHandle,
                                    uint8_t portNum)
{
    /* Nothing to be done for CPSW Ethfw */
}

/* ========================================================================== */
/*                          Internal Function Definitions                     */
/* ========================================================================== */

static int32_t TimeSync_openDma(void)
{
    int32_t status = TIMESYNC_OK;
    CpswDma_OpenRxFlowPrms cpswRxFlowCfg;
    CpswDma_OpenTxChPrms cpswTxChCfg;

    /* Open the CPSW TX channel  */
    if (status == TIMESYNC_OK)
    {
        CpswDma_initTxChParams(&cpswTxChCfg);

        cpswTxChCfg.hUdmaDrv = gTimeSyncCpswObj.hUdmaDrv;
        cpswTxChCfg.hCbArg = &gTimeSyncCpswObj;
        cpswTxChCfg.notifyCb = TimeSync_txNotifyFxn;

        CpswAppUtils_setCommonTxChPrms(&cpswTxChCfg);

        CpswAppUtils_openTxCh(gTimeSyncCpswObj.hCpsw,
                              gTimeSyncCpswObj.coreKey,
                              gTimeSyncCpswObj.coreId,
                              &gTimeSyncCpswObj.txChNum,
                              &gTimeSyncCpswObj.hTxCh,
                              &cpswTxChCfg);

        TimeSync_initTxFreePktQ();

        if (NULL != gTimeSyncCpswObj.hTxCh)
        {
            status = CpswDma_enableTxEvent(gTimeSyncCpswObj.hTxCh);
            if (TIMESYNC_OK != status)
            {
                /* Free the Ch Num if enable event failed */
                CpswAppUtils_freeTxCh(gTimeSyncCpswObj.hCpsw,
                                      gTimeSyncCpswObj.coreKey,
                                      gTimeSyncCpswObj.coreId,
                                      gTimeSyncCpswObj.txChNum);
                CpswAppUtils_print("CpswDma_enableTxEvent() failed: %d\n", status);
                status = CPSW_EFAIL;
            }
        }
        else
        {
            /* Free the Ch Num if open Tx Ch failed */
            CpswAppUtils_freeTxCh(gTimeSyncCpswObj.hCpsw,
                                  gTimeSyncCpswObj.coreKey,
                                  gTimeSyncCpswObj.coreId,
                                  gTimeSyncCpswObj.txChNum);
            CpswAppUtils_print("CpswAppUtils_openTxCh() failed to open: %d\n",
                               status);
            status = CPSW_EFAIL;
        }
    }

    /* Open the CPSW RX flow  */
    if (status == TIMESYNC_OK)
    {
        CpswDma_initRxFlowParams(&cpswRxFlowCfg);
        cpswRxFlowCfg.notifyCb = TimeSync_rxNotifyFxn;
        cpswRxFlowCfg.hUdmaDrv = gTimeSyncCpswObj.hUdmaDrv;
        cpswRxFlowCfg.hCbArg = &gTimeSyncCpswObj;

        CpswAppUtils_setCommonRxFlowPrms(&cpswRxFlowCfg);

        status = CpswAppUtils_allocRxFlow(gTimeSyncCpswObj.hCpsw,
                                          gTimeSyncCpswObj.coreKey,
                                          gTimeSyncCpswObj.coreId,
                                          &gTimeSyncCpswObj.rxFlowStartIdx,
                                          &gTimeSyncCpswObj.rxFlowIdx);
        if (status == TIMESYNC_OK)
        {
            cpswRxFlowCfg.startIdx = gTimeSyncCpswObj.rxFlowStartIdx;
            cpswRxFlowCfg.flowIdx  = gTimeSyncCpswObj.rxFlowIdx;

            gTimeSyncCpswObj.hRxFlow = CpswDma_openRxFlow(&cpswRxFlowCfg);
            if (NULL == gTimeSyncCpswObj.hRxFlow)
            {
                CpswAppUtils_freeRxFlow(gTimeSyncCpswObj.hCpsw,
                                        gTimeSyncCpswObj.coreKey,
                                        gTimeSyncCpswObj.coreId,
                                        gTimeSyncCpswObj.rxFlowIdx);
                CpswAppUtils_print("CpswDma_openRxFlow() failed to open: %d\n",
                                   status);
            }
            else
            {
                /* Submit all ready RX buffers to DMA.*/
                TimeSync_initRxReadyPktQ();
            }
        }
        else
        {
            CpswAppUtils_print("CpswAppUtils_allocRxFlow() failed to open: %d\n",
                               status);
        }

        /* Close Tx channel if Rx flow open failed */
        if (status != TIMESYNC_OK)
        {
            status = CpswDma_disableTxEvent(gTimeSyncCpswObj.hTxCh);
            CpswAppUtils_freeTxCh(gTimeSyncCpswObj.hCpsw,
                                  gTimeSyncCpswObj.coreKey,
                                  gTimeSyncCpswObj.coreId,
                                  gTimeSyncCpswObj.txChNum);
        }
    }

    return status;
}

static void TimeSync_closeDma(void)
{
    int32_t status = TIMESYNC_OK;
    CpswDma_PktInfoQ fqPktInfoQ;
    CpswDma_PktInfoQ cqPktInfoQ;

    CpswUtils_initQ(&fqPktInfoQ);
    CpswUtils_initQ(&cqPktInfoQ);

    /* Close RX Flow */
    if (gTimeSyncCpswObj.hRxFlow != NULL)
    {
        status = CpswDma_closeRxFlow(gTimeSyncCpswObj.hRxFlow,
                                     &fqPktInfoQ,
                                     &cqPktInfoQ);
        if (status == TIMESYNC_OK)
        {
            CpswAppUtils_freeRxFlow(gTimeSyncCpswObj.hCpsw,
                                    gTimeSyncCpswObj.coreKey,
                                    gTimeSyncCpswObj.coreId,
                                    gTimeSyncCpswObj.rxFlowIdx);
            CpswAppUtils_freePktInfoQ(&fqPktInfoQ);
            CpswAppUtils_freePktInfoQ(&cqPktInfoQ);
        }
        else
        {
            CpswAppUtils_print("CpswDma_closeRxFlow() failed to open: %d\n",
                               status);
        }

    }

    /* Close TX channel */
    if (gTimeSyncCpswObj.hTxCh != NULL)
    {
        if (status == TIMESYNC_OK)
        {
            CpswUtils_initQ(&fqPktInfoQ);
            CpswUtils_initQ(&cqPktInfoQ);

            CpswAppUtils_closeTxCh(gTimeSyncCpswObj.hCpsw,
                                   gTimeSyncCpswObj.coreKey,
                                   gTimeSyncCpswObj.coreId,
                                   &fqPktInfoQ,
                                   &cqPktInfoQ,
                                   gTimeSyncCpswObj.hTxCh,
                                   gTimeSyncCpswObj.txChNum);
            CpswAppUtils_freePktInfoQ(&fqPktInfoQ);
            CpswAppUtils_freePktInfoQ(&cqPktInfoQ);

            CpswAppUtils_freePktInfoQ(&gTimeSyncCpswObj.rxFreeQ);
            CpswAppUtils_freePktInfoQ(&gTimeSyncCpswObj.txFreePktInfoQ);
        }
    }

}

static void TimeSync_initTxFreePktQ(void)
{
    CpswDma_PktInfo *pPktInfo;
    uint32_t i;

    /* Initialize all queues */
    CpswUtils_initQ(&gTimeSyncCpswObj.txFreePktInfoQ);

    /* Initialize TX EthPkts and queue them to txFreePktInfoQ */
    for (i = 0U; i < CPSW_APPMEMUTILS_NUM_TX_PKTS; i++)
    {
        pPktInfo = CpswAppMemUtils_allocEthPktFxn(&gTimeSyncCpswObj,
                                                  CPSW_APPMEMUTILS_LARGE_POOL_PKT_SIZE,
                                                  UDMA_CACHELINE_ALIGNMENT);
        CpswAppUtils_assert(pPktInfo != NULL);
        CPSW_UTILS_SET_PKT_APP_STATE(&pPktInfo->pktState, CPSW_PKTSTATE_APP_WITH_FREEQ);

        CpswUtils_enQ(&gTimeSyncCpswObj.txFreePktInfoQ, &pPktInfo->node);
    }
}

static void TimeSync_initRxReadyPktQ(void)
{
    CpswDma_PktInfoQ rxReadyQ;
    CpswDma_PktInfo *pPktInfo;
    int32_t status;
    uint32_t i;

    CpswUtils_initQ(&gTimeSyncCpswObj.rxFreeQ);
    CpswUtils_initQ(&gTimeSyncCpswObj.rxReadyQ);
    CpswUtils_initQ(&rxReadyQ);

    for (i = 0U; i < CPSW_APPMEMUTILS_NUM_RX_PKTS; i++)
    {
        pPktInfo = CpswAppMemUtils_allocEthPktFxn(&gTimeSyncCpswObj,
                                                  CPSW_APPMEMUTILS_LARGE_POOL_PKT_SIZE,
                                                  UDMA_CACHELINE_ALIGNMENT);
        CpswAppUtils_assert(pPktInfo != NULL);
        CPSW_UTILS_SET_PKT_APP_STATE(&pPktInfo->pktState, CPSW_PKTSTATE_APP_WITH_FREEQ);
        CpswUtils_enQ(&gTimeSyncCpswObj.rxFreeQ, &pPktInfo->node);
    }

    /* Retrieve any CPSW packets which are ready */
    status = CpswDma_retrieveRxPackets(gTimeSyncCpswObj.hRxFlow, &rxReadyQ);
    CpswAppUtils_assert(status == TIMESYNC_OK);
    /* There should not be any packet with DMA during init */
    CpswAppUtils_assert(CpswUtils_getQCount(&rxReadyQ) == 0U);

    CpswAppUtils_validatePacketState(&gTimeSyncCpswObj.rxFreeQ,
                                     CPSW_PKTSTATE_APP_WITH_FREEQ,
                                     CPSW_PKTSTATE_APP_WITH_DRIVER);

    CpswAppUtils_submitRxPackets(gTimeSyncCpswObj.hRxFlow,
                                 &gTimeSyncCpswObj.rxFreeQ);

    /* Assert here as during init no. of DMA descriptors should be equal to
     * no. of free Ethernet buffers available with app */
    CpswAppUtils_assert(0U == CpswUtils_getQCount(&gTimeSyncCpswObj.rxFreeQ));
}

static int32_t TimeSync_setCpswAleClassifier(void)
{
    int32_t status = TIMESYNC_OK;
    Cpsw_IoctlPrms prms;
    CpswAle_SetPolicerEntryOutArgs setPolicerEntryOutArgs;
    CpswAle_SetPolicerEntryInArgs setPolicerEntryInArgs;

    if (gTimeSyncCpswObj.timeSyncConfig.protoCfg.protocol == TIMESYNC_PROT_IEEE_802_3)
    {
        setPolicerEntryInArgs.policerMatch.policerMatchEnableMask = CPSW_ALE_POLICER_MATCH_MACDST;
        setPolicerEntryInArgs.policerMatch.dstMacAddr.egressPortNum = 0U;
        setPolicerEntryInArgs.policerMatch.dstMacAddr.addr.vlanId = 0U;
        memcpy(&setPolicerEntryInArgs.policerMatch.dstMacAddr.addr.addr[0U],
               &peerDlyMsgMAC[0U],
               CPSW_MAC_ADDR_LEN);

        setPolicerEntryInArgs.threadIdEnable = true;
        setPolicerEntryInArgs.threadId = gTimeSyncCpswObj.rxFlowIdx;

        CPSW_IOCTL_SET_INOUT_ARGS(&prms, &setPolicerEntryInArgs, &setPolicerEntryOutArgs);
        status = Cpsw_ioctl(gTimeSyncCpswObj.hCpsw,
                            gTimeSyncCpswObj.coreId,
                            CPSW_ALE_IOCTL_SET_POLICER,
                            &prms);
        if (status != TIMESYNC_OK)
        {
            CpswAppUtils_print("TimeSync_setCpswAleClassifier() failed CPSW_ALE_IOCTL_SET_POLICER: %d\n",
                               status);
        }

        setPolicerEntryInArgs.policerMatch.policerMatchEnableMask = CPSW_ALE_POLICER_MATCH_MACDST;
        setPolicerEntryInArgs.policerMatch.dstMacAddr.egressPortNum = 0U;
        setPolicerEntryInArgs.policerMatch.dstMacAddr.addr.vlanId = 0U;
        memcpy(&setPolicerEntryInArgs.policerMatch.dstMacAddr.addr.addr[0U],
               &nonPeerDlyMsgMAC[0U],
               CPSW_MAC_ADDR_LEN);

        setPolicerEntryInArgs.threadIdEnable = true;
        setPolicerEntryInArgs.threadId = gTimeSyncCpswObj.rxFlowIdx;

        CPSW_IOCTL_SET_INOUT_ARGS(&prms, &setPolicerEntryInArgs, &setPolicerEntryOutArgs);
        status = Cpsw_ioctl(gTimeSyncCpswObj.hCpsw,
                            gTimeSyncCpswObj.coreId,
                            CPSW_ALE_IOCTL_SET_POLICER,
                            &prms);
        if (status != TIMESYNC_OK)
        {
            CpswAppUtils_print("TimeSync_setCpswAleClassifier() failed CPSW_ALE_IOCTL_SET_POLICER: %d\n",
                               status);
        }
    }
    else if (gTimeSyncCpswObj.timeSyncConfig.protoCfg.protocol == TIMESYNC_PROT_UDP_IPV4)
    {
        setPolicerEntryInArgs.policerMatch.policerMatchEnableMask = CPSW_ALE_POLICER_MATCH_IPDST;
        setPolicerEntryInArgs.policerMatch.dstIp.ipAddrtype = CPSW_ALE_IPADDR_CLASSIFIER_IPV4;
        memcpy(&setPolicerEntryInArgs.policerMatch.dstIp.ipv4.ipv4Addr[0U],
               &peerDlyMsgIpv4[0U],
               CPSW_ALE_IPV4ADDR_NUM_OCTETS);
        setPolicerEntryInArgs.policerMatch.dstIp.ipv4.numLSBIgnoreBits = 0U;

        setPolicerEntryInArgs.threadIdEnable = true;
        setPolicerEntryInArgs.threadId = gTimeSyncCpswObj.rxFlowIdx;

        CPSW_IOCTL_SET_INOUT_ARGS(&prms, &setPolicerEntryInArgs, &setPolicerEntryOutArgs);
        status = Cpsw_ioctl(gTimeSyncCpswObj.hCpsw,
                            gTimeSyncCpswObj.coreId,
                            CPSW_ALE_IOCTL_SET_POLICER,
                            &prms);
        if (status != TIMESYNC_OK)
        {
            CpswAppUtils_print(
                               "TimeSync_setCpswAleClassifier() failed CPSW_ALE_IOCTL_SET_POLICER: %d\n", status);
        }

        setPolicerEntryInArgs.policerMatch.policerMatchEnableMask = CPSW_ALE_POLICER_MATCH_IPDST;
        setPolicerEntryInArgs.policerMatch.dstIp.ipAddrtype = CPSW_ALE_IPADDR_CLASSIFIER_IPV4;
        memcpy(&setPolicerEntryInArgs.policerMatch.dstIp.ipv4.ipv4Addr[0U],
               &nonPeerDlyMsgIpv4[0U],
               CPSW_ALE_IPV4ADDR_NUM_OCTETS);
        setPolicerEntryInArgs.policerMatch.dstIp.ipv4.numLSBIgnoreBits = 0U;

        setPolicerEntryInArgs.threadIdEnable = true;
        setPolicerEntryInArgs.threadId = gTimeSyncCpswObj.rxFlowIdx;

        CPSW_IOCTL_SET_INOUT_ARGS(&prms, &setPolicerEntryInArgs, &setPolicerEntryOutArgs);
        status = Cpsw_ioctl(gTimeSyncCpswObj.hCpsw,
                            gTimeSyncCpswObj.coreId,
                            CPSW_ALE_IOCTL_SET_POLICER,
                            &prms);
        if (status != TIMESYNC_OK)
        {
            CpswAppUtils_print(
                               "TimeSync_setCpswAleClassifier() failed CPSW_ALE_IOCTL_SET_POLICER: %d\n", status);
        }
    }
    else
    {
        status = TIMESYNC_UNSUPPORTED_FORMAT;
    }

    return status;
}

static int32_t TimeSync_setMacPortConfig(void)
{
    int32_t status = TIMESYNC_OK;
    Cpsw_IoctlPrms prms;
    CpswMacPort_EnableTsEventInArgs enableTsEventInArgs;
    uint8_t i = 0U;
    uint32_t maxMacPorts = 0U;

    maxMacPorts = Cpsw_getMacPortMax(gTimeSyncCpswObj.cpswType);
    TimeSync_setPortTsEventPrms(&enableTsEventInArgs.portCfg);

    for (i = 0U; i < maxMacPorts; i++)
    {
        if (TIMESYNC_IS_BIT_SET(gTimeSyncCpswObj.timeSyncConfig.protoCfg.portMask, i))
        {
            enableTsEventInArgs.portNum = (Cpsw_MacPort)(CPSW_NORMALIZE_MACPORT(i));
            CPSW_IOCTL_SET_IN_ARGS(&prms, &enableTsEventInArgs);
            status = Cpsw_ioctl(gTimeSyncCpswObj.hCpsw,
                                gTimeSyncCpswObj.coreId,
                                CPSW_MACPORT_IOCTL_ENABLE_CPTS_EVENT,
                                &prms);
            CpswAppUtils_assert(status == TIMESYNC_OK);
        }
    }

    return status;
}

static void TimeSync_setPortTsEventPrms(CpswMacPort_TsEventConfig *tsPortEventCfg)
{
    tsPortEventCfg->commonPortIpConfig.enableTTLNonzero = true;
    tsPortEventCfg->commonPortIpConfig.enableTsIp107 = false;
    tsPortEventCfg->commonPortIpConfig.enableTsIp129 = false;
    tsPortEventCfg->commonPortIpConfig.enableTsIp130 = false;
    tsPortEventCfg->commonPortIpConfig.enableTsIp131 = false;
    tsPortEventCfg->commonPortIpConfig.enableTsIp132 = false;
    tsPortEventCfg->commonPortIpConfig.enableTsPort319 = true;
    tsPortEventCfg->commonPortIpConfig.enableTsPort320 = true;
    tsPortEventCfg->commonPortIpConfig.enableUnicast = false;
    tsPortEventCfg->domainOffset = 4U;
    tsPortEventCfg->enableLType2 = false;
    tsPortEventCfg->enableRxAnnexD = true;
    tsPortEventCfg->enableRxAnnexE = true;
    tsPortEventCfg->enableRxAnnexF = true;
    tsPortEventCfg->enableTxAnnexD = true;
    tsPortEventCfg->enableTxAnnexE = true;
    tsPortEventCfg->enableTxAnnexF = true;
    tsPortEventCfg->enableTxHostTs = true;
    tsPortEventCfg->mcastType = 0U;
    tsPortEventCfg->messageType = 0xFFFFU;
    tsPortEventCfg->seqIdOffset = 30U;
    tsPortEventCfg->rxVlanType  = CPSW_MACPORT_VLAN_TYPE_NONE;
    tsPortEventCfg->txVlanType  = CPSW_MACPORT_VLAN_TYPE_NONE;
    tsPortEventCfg->vlanLType1  = 0U;
    tsPortEventCfg->vlanLType2  = 0U;

    if (gTimeSyncCpswObj.timeSyncConfig.protoCfg.vlanCfg.vlanType == TIMESYNC_VLAN_TYPE_SINGLE_TAG)
    {
        tsPortEventCfg->rxVlanType = CPSW_MACPORT_VLAN_TYPE_SINGLE_TAG;
        tsPortEventCfg->txVlanType = CPSW_MACPORT_VLAN_TYPE_SINGLE_TAG;
        tsPortEventCfg->vlanLType1 = gTimeSyncCpswObj.timeSyncConfig.protoCfg.vlanCfg.iVlanTag;
        tsPortEventCfg->vlanLType2 = 0U;
    }

    if (gTimeSyncCpswObj.timeSyncConfig.protoCfg.vlanCfg.vlanType == TIMESYNC_VLAN_TYPE_DOUBLE_TAG)
    {
        tsPortEventCfg->rxVlanType = CPSW_MACPORT_VLAN_TYPE_STACKED_TAGS;
        tsPortEventCfg->txVlanType = CPSW_MACPORT_VLAN_TYPE_STACKED_TAGS;
        tsPortEventCfg->vlanLType1 = gTimeSyncCpswObj.timeSyncConfig.protoCfg.vlanCfg.iVlanTag;
        tsPortEventCfg->vlanLType2 = gTimeSyncCpswObj.timeSyncConfig.protoCfg.vlanCfg.oVlanTag;
    }
}

static uint32_t TimeSync_retrieveTxDonePkts(void)
{
    CpswDma_PktInfoQ txFreeQ;
    CpswDma_PktInfo *pktInfo;
    int32_t status;
    uint32_t txFreeQCnt = 0U;

    CpswUtils_initQ(&txFreeQ);

    /* Retrieve any CPSW packets that may be free now */
    status = CpswDma_retrieveTxDonePackets(gTimeSyncCpswObj.hTxCh, &txFreeQ);
    if (status == TIMESYNC_OK)
    {
        txFreeQCnt = CpswUtils_getQCount(&txFreeQ);

        pktInfo = (CpswDma_PktInfo *)CpswUtils_deQ(&txFreeQ);
        while (NULL != pktInfo)
        {
            CpswUtils_checkPktState(&pktInfo->pktState,
                                    CPSW_PKTSTATE_MODULE_APP,
                                    CPSW_PKTSTATE_APP_WITH_DRIVER,
                                    CPSW_PKTSTATE_APP_WITH_FREEQ);

            CpswUtils_enQ(&gTimeSyncCpswObj.txFreePktInfoQ, &pktInfo->node);
            pktInfo = (CpswDma_PktInfo *)CpswUtils_deQ(&txFreeQ);
        }
    }
    else
    {
        CpswAppUtils_print("CpswDma_retrieveTxDonePackets() failed to retrieve pkts: %d\n",
                           status);
    }

    return txFreeQCnt;
}

static uint32_t TimeSync_receiveRxReadyPkts(void)
{
    CpswDma_PktInfoQ rxReadyQ;
    CpswDma_PktInfo *pktInfo;
    int32_t status;
    uint32_t rxReadyCnt = 0U;

    CpswUtils_initQ(&rxReadyQ);

    /* Retrieve any CPSW packets which are ready */
    status = CpswDma_retrieveRxPackets(gTimeSyncCpswObj.hRxFlow, &rxReadyQ);
    rxReadyCnt = CpswUtils_getQCount(&rxReadyQ);
    if (status == TIMESYNC_OK)
    {
        /* Queue the received packet to rxReadyQ and pass new ones from rxFreeQ */
        pktInfo = (CpswDma_PktInfo *)CpswUtils_deQ(&rxReadyQ);
        while (pktInfo != NULL)
        {
            CpswUtils_checkPktState(&pktInfo->pktState,
                                    CPSW_PKTSTATE_MODULE_APP,
                                    CPSW_PKTSTATE_APP_WITH_DRIVER,
                                    CPSW_PKTSTATE_APP_WITH_READYQ);

            CpswUtils_enQ(&gTimeSyncCpswObj.rxReadyQ, &pktInfo->node);
            pktInfo = (CpswDma_PktInfo *)CpswUtils_deQ(&rxReadyQ);
        }
    }
    else
    {
        CpswAppUtils_print("TimeSync_receiveRxReadyPkts() failed to retrieve pkts: %d\n",
                           status);
    }

    return rxReadyCnt;
}

static void TimeSync_getMsgId(uint8_t *msgId,
                              uint8_t *frame)
{
    uint32_t offset = TIMESYNC_PTP_MSG_ID_OFFSET;

    if (gTimeSyncCpswObj.timeSyncConfig.protoCfg.protocol == TIMESYNC_PROT_UDP_IPV4)
    {
        offset += TIMESYNC_ANNEX_D_ANNEX_F_DIFF;
    }

    if (gTimeSyncCpswObj.timeSyncConfig.protoCfg.protocol == TIMESYNC_PROT_UDP_IPV6)
    {
        offset += TIMESYNC_ANNEX_E_ANNEX_F_DIFF;
    }

    if (gTimeSyncCpswObj.timeSyncConfig.protoCfg.vlanCfg.vlanType == TIMESYNC_VLAN_TYPE_SINGLE_TAG)
    {
        offset += TIMESYNC_SINGLE_TAG_VLAN_HDR_SIZE;
    }

    if (gTimeSyncCpswObj.timeSyncConfig.protoCfg.vlanCfg.vlanType == TIMESYNC_VLAN_TYPE_DOUBLE_TAG)
    {
        offset += TIMESYNC_DOUBLE_TAG_VLAN_HDR_SIZE;
    }

    *msgId = *(frame + offset);
    *msgId = *msgId & 0xF;
}

static void TimeSync_getSeqId(uint16_t *seqId,
                              uint8_t *frame)
{
    uint32_t offset = TIMESYNC_PTP_SEQ_ID_OFFSET;

    if (gTimeSyncCpswObj.timeSyncConfig.protoCfg.protocol == TIMESYNC_PROT_UDP_IPV4)
    {
        offset += TIMESYNC_ANNEX_D_ANNEX_F_DIFF;
    }

    if (gTimeSyncCpswObj.timeSyncConfig.protoCfg.protocol == TIMESYNC_PROT_UDP_IPV6)
    {
        offset += TIMESYNC_ANNEX_E_ANNEX_F_DIFF;
    }

    if (gTimeSyncCpswObj.timeSyncConfig.protoCfg.vlanCfg.vlanType == TIMESYNC_VLAN_TYPE_SINGLE_TAG)
    {
        offset += TIMESYNC_SINGLE_TAG_VLAN_HDR_SIZE;
    }

    if (gTimeSyncCpswObj.timeSyncConfig.protoCfg.vlanCfg.vlanType == TIMESYNC_VLAN_TYPE_DOUBLE_TAG)
    {
        offset += TIMESYNC_DOUBLE_TAG_VLAN_HDR_SIZE;
    }

    *seqId = *(uint16_t *)(frame + offset);
    *seqId = ntohs(*seqId);
}

static int32_t TimeSync_configPps(void)
{
    int32_t status = TIMESYNC_OK;
    Cpsw_IoctlPrms prms;
    CpswCpts_RegisterHwPushCbInArgs hwPushCbInArgs;
    CpswCpts_SetFxnGenInArgs setGenFInArgs;
    uint64_t tsVal = 0U;

    /* Configure Time Sync Router to route GENF0 signal to hardware push 1 */
    status = CpswAppUtils_setTimeSyncRouter(gTimeSyncCpswObj.cpswType,
                                            CSLR_TIMESYNC_INTRTR0_IN_CPSW0_CPTS_GENF0_0,
                                            CSLR_TIMESYNC_INTRTR0_OUTL_CPSW0_CPTS_HW1_PUSH_0);
    CpswAppUtils_assert(status == TIMESYNC_OK);

    /* Register hardware push 1 callback */
    hwPushCbInArgs.hwPushNum = TIMESYNC_PPS_HW_PUSH_NUM;
    hwPushCbInArgs.pHwPushNotifyCb = TimeSync_ppsHwPushNotifyFxn;
    hwPushCbInArgs.pHwPushNotifyCbArg = (void *)&gTimeSyncCpswObj;
    CPSW_IOCTL_SET_IN_ARGS(&prms, &hwPushCbInArgs);
    status = Cpsw_ioctl(gTimeSyncCpswObj.hCpsw,
                        gTimeSyncCpswObj.coreId,
                        CPSW_CPTS_IOCTL_REGISTER_HW_PUSH_CALLBACK,
                        &prms);
    CpswAppUtils_assert(status == TIMESYNC_OK);

    /* Reset CPTS timer to zero, to ensure GENF compare value hits on first run */
    tsVal = 0U;
    CPSW_IOCTL_SET_IN_ARGS(&prms, &tsVal);
    status = Cpsw_ioctl(gTimeSyncCpswObj.hCpsw,
                        gTimeSyncCpswObj.coreId,
                        CPSW_CPTS_IOCTL_LOAD_TIMESTAMP,
                        &prms);
    CpswAppUtils_assert(status == TIMESYNC_OK);

    /* Configure GENF0 to generate pulse signal */
    setGenFInArgs.index  = 0U;
    setGenFInArgs.length = gTimeSyncCpswObj.timeSyncConfig.protoCfg.ppsConfig.ppsPulseIntervalNs;

    /* Add one second to compare value to ensure GENF compare happens
     *  and PPS gets generated then after */
    setGenFInArgs.compare = gTimeSyncCpswObj.timeSyncConfig.protoCfg.ppsConfig.ppsPulseIntervalNs +
                            TIMESYNC_SEC_TO_NS;
    setGenFInArgs.polarityInv = true;
    setGenFInArgs.ppmVal  = 0U;
    setGenFInArgs.ppmDir  = CPSW_CPTS_PPM_DIR_INCREASE;
    setGenFInArgs.ppmMode = CPSW_CPTS_PPM_DISABLE;

    CPSW_IOCTL_SET_IN_ARGS(&prms, &setGenFInArgs);
    status = Cpsw_ioctl(gTimeSyncCpswObj.hCpsw,
                        gTimeSyncCpswObj.coreId,
                        CPSW_CPTS_IOCTL_SET_GENF,
                        &prms);
    CpswAppUtils_assert(status == TIMESYNC_OK);

    return status;
}
