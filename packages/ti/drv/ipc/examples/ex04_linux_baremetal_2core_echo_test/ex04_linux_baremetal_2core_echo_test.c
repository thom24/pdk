/*
 *  Copyright (c) Texas Instruments Incorporated 2020
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
 *  \file ex04_linux_baremetal_2core_echo_test.c
 *
 *  \brief 2-core (Linux-to-Baremetal) IPC echo test application performing
 *  basic echo communication using the baremetal IPC driver
 *  The application after initialization of the IPC lld, waits for messages
 *  from the Linux host core and echos each message received, back to the 
 *  source.
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <ti/osal/HwiP.h>
#include <ti/osal/osal.h>
/* SCI Client */
#include <ti/drv/sciclient/sciclient.h>

#include <ti/drv/ipc/ipc.h>
#include <ti/drv/ipc/ipcver.h>
#include <ti/drv/ipc/examples/common/src/ipc_setup.h>
#include "../common/src/ipc_trace.h"
#include "../common/src/ipctest_baremetal.h"
#ifndef BUILD_MPU1_0
#if defined(SOC_AM65XX)
#include "../common/src/ipc_am65xx_rsctable.h"
#else
#include "../common/src/ipc_rsctable.h"
#endif
#endif

/* Size of message */
#define MSGSIZE  256U
/* Service name to be registered for the end point */
#define SERVICE  "ti.ipc4.ping-pong"
/* End point number to be used for IPC communication */
#define ENDPT1   13U
#define NUMMSGS  10000 /* number of message sent by the sender function */
                       /* Note: The sender function is not active in this example */

uint32_t rpmsgDataSize = RPMSG_DATA_SIZE;

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */
#define TEST_TIMEOUT_FOREVER (~(0U))

/* Number of cores used in the test */
#define CORE_IN_TEST            2

/* Define System_printf to use Ipc_Trace_printf */
#define System_printf Ipc_Trace_printf


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
uint8_t  gCntrlBuf[RPMSG_DATA_SIZE] __attribute__ ((section("ipc_data_buffer"), aligned (8)));
uint8_t  gSysVqBuf[VQ_BUF_SIZE]  __attribute__ ((section ("ipc_data_buffer"), aligned (8)));
uint8_t  gSendBuf[RPMSG_DATA_SIZE * CORE_IN_TEST]  __attribute__ ((section ("ipc_data_buffer"), aligned (8)));
uint8_t  gRspBuf[RPMSG_DATA_SIZE]  __attribute__ ((section ("ipc_data_buffer"), aligned (8)));

uint8_t *pCntrlBuf = gCntrlBuf;
uint8_t *pSendTaskBuf = gSendBuf;
uint8_t *pRecvTaskBuf = gRspBuf;
uint8_t *pSysVqBuf = gSysVqBuf;

volatile uint32_t gMessagesReceived = 0;

#ifdef BUILD_MPU1_0
#define NUM_CHRDEV_SERVICES (1)
#define CHRDEV_PROC_NAME    "a53-mpu-core-0"
char *service_names[NUM_CHRDEV_SERVICES] = {"beta"};
uint32_t selfProcId = IPC_MPU1_0;
uint32_t remoteProc[] =
{
    IPC_MCU1_0
};
#endif

#ifdef BUILD_MCU1_0
uint32_t selfProcId = IPC_MCU1_0;
uint32_t remoteProc[] =
{
    IPC_MPU1_0
};
#endif

/* NOTE: all other cores are not used in this test, but must be built as part of full PDK build */
#if !defined(BUILD_MPU1_0) && !defined(BUILD_MCU1_0)
uint32_t selfProcId = 0;
uint32_t remoteProc[] = {};
#endif

uint32_t *pRemoteProcArray = remoteProc;
volatile uint32_t  gNumRemoteProc = sizeof(remoteProc)/sizeof(uint32_t);

/*
 * This "Task" waits for Linux vdev ready, and late create the vrings
 *
 */
#if !defined(BUILD_MPU1_0) && defined(A72_LINUX_OS) && defined(A72_LINUX_OS_IPC_ATTACH)
void rpmsg_vdevMonitorFxn(void)
{
    int32_t status;

    /* Wait for Linux VDev ready... */
    while(!Ipc_isRemoteReady(IPC_MPU1_0))
    {
//       Task_sleep(10);
    }

    /* Create the VRing now ... */
    status = Ipc_lateVirtioCreate(IPC_MPU1_0);
    if(status != IPC_SOK)
    {
        System_printf("%s: Ipc_lateVirtioCreate failed\n", __func__);
        return;
    }

    status = RPMessage_lateInit(IPC_MPU1_0);
    if(status != IPC_SOK)
    {
        System_printf("%s: RPMessage_lateInit failed\n", __func__);
        return;
    }

    status = RPMessage_announce(IPC_MPU1_0, RecvEndPt, SERVICE);
    if(status != IPC_SOK)
    {
        System_printf("rpmsg_vdevMonitorFxn: RPMessage_announce() failed\n");
    }
}
#endif /* !defined(BUILD_MPU1_0) && defined(A72_LINUX_OS) && defined(A72_LINUX_OS_IPC_ATTACH)*/

bool g_exitRespTsk = 0;

/*
 * This function can be used to stop the
 * responder task.
 */
void rpmsg_exit_responseTask()
{
    g_exitRespTsk = 1;
}

/*
 * This function waits for a "ping" message from any processor
 * then replies with a "pong" message.
 */
void rpmsg_responderFxn(uintptr_t arg0, uintptr_t arg1)
{
    RPMessage_Handle handle;
    RPMessage_Params params;
    uint32_t		 myEndPt = 0;
    uint32_t		 remoteEndPt;
    uint32_t		 remoteProcId;
    uint16_t		 len;
    int32_t		     n;
    int32_t		     status = 0;
    void		     *buf;
    uint32_t         t;
    uint32_t         lastNumMessagesReceived = 0;
    uint32_t         emptyReceiveCalls = 0;

    uint32_t            bufSize = rpmsgDataSize;
    char                str[MSGSIZE];

    buf = pRecvTaskBuf;
    if(buf == NULL)
    {
        System_printf("RecvTask: buffer allocation failed\n");
        return;
    }

    RPMessageParams_init(&params);

    params.requestedEndpt = ENDPT1;

    params.buf = buf;
    params.bufSize = bufSize;

    handle = RPMessage_create(&params, &myEndPt);
    if(!handle)
    {
        System_printf("RecvTask: Failed to create endpoint\n");
        return;
    }
    for (t = 0; t < gNumRemoteProc; t++)
    {
        Ipc_mailboxEnableNewMsgInt(selfProcId, t);
    }
#if !defined(BUILD_MPU1_0) && defined(A72_LINUX_OS) && defined(A72_LINUX_OS_IPC_ATTACH)
    RecvEndPt = myEndPt;
#endif

    status = RPMessage_announce(RPMESSAGE_ALL, myEndPt, SERVICE);
    if(status != IPC_SOK)
    {
        System_printf("RecvTask: RPMessage_announce() failed\n");
        return;
    }

    while(!g_exitRespTsk)
    {
        /* Wait for messages to show up */
        while(gMessagesReceived == lastNumMessagesReceived);
        /* NOTE: The following function may need to be replaced by a blocking
          function RPMessage_recv in later implementations */
        status = RPMessage_recvNb(handle,
                                  (Ptr)str, &len, &remoteEndPt,
                                  &remoteProcId);
        if(status != IPC_SOK)
        {
#ifdef DEBUG_PRINT
            System_printf("RecvTask: failed with code %d\n", status);
#endif
            emptyReceiveCalls++;
        }
        else
        {
            lastNumMessagesReceived++;

            /* NULL terminated string */
            str[len] = '\0';
#ifdef DEBUG_PRINT
            System_printf("RecvTask: Revcvd msg \"%s\" len %d from %s\n",
                    str, len, Ipc_mpGetName(remoteProcId));
#endif
        }
        if(status == IPC_SOK)
        {
            status = sscanf(str, "ping %d", &n);
            if(status == 1)
            {
                memset(str, 0, MSGSIZE);
                len = snprintf(str, 255, "pong %d", n);
                if(len > 255)
                {
                    System_printf("RecvTask: snprintf failed, len %d\n", len);
                    len = 255;
                }
                str[len++] = '\0';
            }
            else
            {
                /* If this is not ping/pong message, just print the message */
                System_printf("%s <--> %s : %s recvd : %d:%d:%d\n",
                              Ipc_mpGetSelfName(),
                              Ipc_mpGetName(remoteProcId),
                              str,
                              gMessagesReceived,
                              lastNumMessagesReceived,
                              emptyReceiveCalls);
            }
    #ifdef DEBUG_PRINT
            System_printf("RecvTask: Sending msg \"%s\" len %d from %s to %s\n",
                          str, len, Ipc_mpGetSelfName(),
                          Ipc_mpGetName(remoteProcId));
    #endif
            status = RPMessage_send(handle, remoteProcId, remoteEndPt, myEndPt, str, len);
            if (status != IPC_SOK)
            {
                System_printf("RecvTask: Sending msg \"%s\" len %d from %s to %s failed!!!\n",
                    str, len, Ipc_mpGetSelfName(),
                    Ipc_mpGetName(remoteProcId));
            }
        }
    }

    System_printf("%s responder task exiting ...\n",
                    Ipc_mpGetSelfName());
}

/*
 * This function sends a message and waits for a response
 * and repeats the cycle for predetermined number of messages.
 * NOTE: In this example this sender function is not active.
 */
void rpmsg_senderFxn(uintptr_t arg0, uintptr_t arg1)
{
    RPMessage_Handle    handle;
    RPMessage_Params    params;
    uint32_t            myEndPt = 0;
    uint32_t            remoteEndPt;
    uint32_t            remoteProcId;
    uint16_t            dstProc;
    uint16_t            len;
    int32_t             i;
    int32_t             status = 0;
    char                buf[256];
    uint8_t            *buf1;

    uint32_t            cntPing = 0;
    uint32_t            cntPong = 0;

    buf1 = &pSendTaskBuf[rpmsgDataSize * arg1];
    dstProc = arg0;

    /* Create the endpoint for receiving. */
    RPMessageParams_init(&params);
    params.numBufs = 2;
    params.buf = buf1;
    params.bufSize = rpmsgDataSize;
    handle = RPMessage_create(&params, &myEndPt);
    if(!handle)
    {
        System_printf("SendTas %d: Failed to create message endpoint\n",
                dstProc);
        return;
    }

    status = RPMessage_getRemoteEndPt(dstProc, SERVICE, &remoteProcId,
            &remoteEndPt, TEST_TIMEOUT_FOREVER);
    if(dstProc != remoteProcId)
    {
        System_printf("SendTask%d: RPMessage_getRemoteEndPt() malfunctioned, status %d\n",
                dstProc, status);
        return;
    }

    for (i = 0; i < NUMMSGS; i++)
    {
        /* Send data to remote endPt: */
        memset(buf, 0, 256);
        len = snprintf(buf, 255, "ping %d", i);
        if(len > 255)
        {
            System_printf("SendTask%d: snprintf failed, len %d\n", dstProc, len);
            len = 255;
        }
        buf[len++] = '\0';

#ifdef DEBUG_PRINT
        System_printf("SendTask%d: Sending \"%s\" from %s to %s...\n", dstProc,
                      buf, Ipc_mpGetSelfName(),
                      Ipc_mpGetName(dstProc));
#endif
        /* Increase the Ping Counter */
        cntPing++;

        status = RPMessage_send(handle, dstProc, ENDPT1, myEndPt, (Ptr)buf, len);
        if (status != IPC_SOK)
        {
            System_printf("SendTask%d: RPMessage_send Failed Msg-> \"%s\" from %s to %s...\n",
                dstProc,
                buf, Ipc_mpGetSelfName(),
                Ipc_mpGetName(dstProc));
            break;
        }

        /* wait a for a response message: */
        /* NOTE: The following function may need to be replaced by a blocking
          function RPMessage_recv in later implementations */
        status = RPMessage_recvNb(handle, (Ptr)buf, &len, &remoteEndPt,
                                  &remoteProcId);

        if(status != IPC_SOK)
        {
            System_printf("SendTask%d: RPMessage_recv failed with code %d\n",
                    dstProc, status);
            break;
        }

        /* Make it NULL terminated string */
        if(len >= MSGSIZE)
        {
            buf[MSGSIZE-1] = '\0';
        }
        else
        {
            buf[len] = '\0';
        }
#ifdef DEBUG_PRINT
        System_printf("SendTask%d: Received \"%s\" len %d from %s endPt %d \n",
                dstProc, buf, len, Ipc_mpGetName(remoteProcId),
                remoteEndPt);
#endif
        cntPong++;
        if((i+1)%50 == 0)
        {
#ifdef DEBUG_PRINT
            System_printf("%s <--> %s, ping/pong iteration %d ...\n",
                          Ipc_mpGetSelfName(), Ipc_mpGetName(dstProc), i);
#endif
        }
    }

    System_printf("%s <--> %s, Ping- %d, pong - %d completed\n",
            Ipc_mpGetSelfName(),
            Ipc_mpGetName(dstProc),
            cntPing, cntPong);

    /* Delete the RPMesg object now */
    RPMessage_delete(&handle);
}

#define INTRTR_CFG_MAIN_DOMAIN_MBX_CLST0_USR1_OUT_INT_NO  (17U)
#define INTRTR_CFG_START_LEVEL_INT_NUMBER \
            (CSL_MCU0_INTR_MAIN2MCU_LVL_INTR0_OUTL_0)
#define CDD_IPC_CORE_MPU1_0            (0u)
#define APP_SCICLIENT_TIMEOUT          (SCICLIENT_SERVICE_WAIT_FOREVER)
/*
 * This function calls the Ipc lld ISR handler to service the interrupt
 */
void IpcTestBaremetalIrqMbxFromMpu_10(void)
{
    Ipc_newMessageIsr(CDD_IPC_CORE_MPU1_0);
    return;
}
/*
 * This function needs to be registered as an interrupt
 * handler for IPC mailbox events
 */
void IpcTestBaremetalAppMsgFromMpu10Isr(uintptr_t notUsed)
{
    /* Invoke MPU 10 Isr handler */
    IpcTestBaremetalIrqMbxFromMpu_10();
}
/*
 * This function is the callback function the ipc lld library calls when a
 * message is received.
 */
static void IpcTestBaremetalNewMsgCb(uint32_t srcEndPt, uint32_t procId)
{
    /* Add code here to take action on any incoming messages */
    gMessagesReceived++;
    return;
}

/*
 * This function registers the interrupt handlers
 * NOTE: This code may change and may be abstracted into the lld code 
 *       or a seperate osal file in a future release.
 */
int32_t IpctestConfigureInterruptHandlers(void)
{
    OsalRegisterIntrParams_t    intrPrms;
    OsalInterruptRetCode_e      osalRetVal;
    HwiP_Handle hwiHandle;
    struct tisci_msg_rm_irq_set_req     rmIrqReq;
    struct tisci_msg_rm_irq_set_resp    rmIrqResp;
    int32_t retVal;

    rmIrqReq.valid_params           = TISCI_MSG_VALUE_RM_DST_ID_VALID;
    rmIrqReq.valid_params          |= TISCI_MSG_VALUE_RM_DST_HOST_IRQ_VALID;
    rmIrqReq.src_id                 = TISCI_DEV_NAVSS0_MAILBOX0_CLUSTER0;
    rmIrqReq.global_event           = 0U;
    rmIrqReq.src_index              = 1U; /* 0 for User 0, 1 for user 1... */
    rmIrqReq.dst_id                 = TISCI_DEV_MCU_ARMSS0_CPU0;
    rmIrqReq.dst_host_irq           =
                        (INTRTR_CFG_MAIN_DOMAIN_MBX_CLST0_USR1_OUT_INT_NO +
                         INTRTR_CFG_START_LEVEL_INT_NUMBER);

    rmIrqReq.ia_id                  = 0U;
    rmIrqReq.vint                   = 0U;
    rmIrqReq.vint_status_bit_index  = 0U;
    rmIrqReq.secondary_host         = TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST;
    retVal = Sciclient_rmIrqSet(
                 &rmIrqReq, &rmIrqResp, APP_SCICLIENT_TIMEOUT);
    if(CSL_PASS != retVal)
    {
        System_printf(": Error in SciClient Interrupt Params Configuration!!!\n");
        return -1;
    }

    /* Interrupt hook up */
    Osal_RegisterInterrupt_initParams(&intrPrms);
    intrPrms.corepacConfig.arg          = (uintptr_t)NULL;
    intrPrms.corepacConfig.isrRoutine   = &IpcTestBaremetalAppMsgFromMpu10Isr;
    intrPrms.corepacConfig.priority     = 1U;
    intrPrms.corepacConfig.corepacEventNum = 0U;
    intrPrms.corepacConfig.intVecNum    =
        (INTRTR_CFG_MAIN_DOMAIN_MBX_CLST0_USR1_OUT_INT_NO +
         INTRTR_CFG_START_LEVEL_INT_NUMBER);

    osalRetVal = Osal_RegisterInterrupt(&intrPrms, &hwiHandle);

    if(OSAL_INT_SUCCESS != osalRetVal)
    {
        System_printf( ": Error Could not register ISR to receive"
                         " from MCU 1 1 !!!\n");
        return -1;
    }
    Osal_EnableInterrupt(0, intrPrms.corepacConfig.intVecNum);
    return 0;

}

/*
 * This is the main test function which initializes and runs the Sender and 
 * responder functions.
 * NOTE: Sender function is not actually active in this example.
 *       Only the receiver function does a echo back of incoming messages
 */
/* ==========================================*/
int32_t Ipc_echo_test(void)
{
    uint32_t          t;
    uint32_t          numProc = gNumRemoteProc;
    Ipc_VirtIoParams  vqParam;
    Ipc_InitPrms      initPrms;

    /* Step1 : Initialize the multiproc */
    if (IPC_SOK == Ipc_mpSetConfig(selfProcId, numProc, pRemoteProcArray))
    {
        System_printf("IPC_echo_test (core : %s) .....\r\n%s\r\n",
                Ipc_mpGetSelfName(), IPC_DRV_VERSION_STR);

        initPrms.instId = 0U;
        initPrms.osalPrms.disableAllIntr = &IpcTestBaremetalCriticalSectionIntEnter;
        initPrms.osalPrms.restoreAllIntr = &IpcTestBaremetalCriticalSectionIntExit;

        initPrms.osalPrms.createHIsr     = &IpcTestBaremetalHIsrCreate;
        initPrms.osalPrms.deleteHIsr     = &IpcTestBaremetalHIsrDelete;
        initPrms.osalPrms.postHIsr       = &IpcTestBaremetalHIsrPost;
        initPrms.osalPrms.createHIsrGate = &IpcTestBaremetalHIsrGateCreate;
        initPrms.osalPrms.deleteHIsrGate = &IpcTestBaremetalHIsrGateDelete;
        initPrms.osalPrms.lockHIsrGate   = &IpcTestBaremetalHIsrGateEnter;
        initPrms.osalPrms.unLockHIsrGate = &IpcTestBaremetalHIsrGateExit;

        initPrms.osalPrms.createMutex    = (Ipc_OsalMutexCreateFxn) NULL_PTR;
        initPrms.osalPrms.deleteMutex    = (Ipc_OsalMutexDeleteFxn) NULL_PTR;
        initPrms.osalPrms.lockMutex      = (Ipc_OsalMutexLockFxn) NULL_PTR;
        initPrms.osalPrms.unlockMutex    = (Ipc_OsalMutexUnlockFxn) NULL_PTR;

        initPrms.newMsgFxn = &IpcTestBaremetalNewMsgCb;

        if (IPC_SOK != Ipc_init(&initPrms))
        {
            return -1;
        }
    }
#ifdef DEBUG_PRINT
    System_printf("Required Local memory for Virtio_Object = %d\r\n",
                  numProc * Ipc_getVqObjMemoryRequiredPerCore());
#endif
#if !defined(BUILD_MPU1_0) && defined(A72_LINUX_OS)
    /* If A72 remote core is running Linux OS, then
     * load resource table
     */
    Ipc_loadResourceTable((void*)&ti_ipc_remoteproc_ResourceTable);

#if !defined(A72_LINUX_OS_IPC_ATTACH)
    /* Wait for Linux VDev ready... */
    for(t = 0; t < numProc; t++)
    {
        while(!Ipc_isRemoteReady(pRemoteProcArray[t]))
        {
            // Task_sleep(100);
        }
    }
    System_printf("Linux VDEV ready now .....\n");
#endif
#endif

    /* Step2 : Initialize Virtio */
    vqParam.vqObjBaseAddr = (void*)pSysVqBuf;
    vqParam.vqBufSize     = numProc * Ipc_getVqObjMemoryRequiredPerCore();
    vqParam.vringBaseAddr = (void*)VRING_BASE_ADDRESS;
    vqParam.vringBufSize  = IPC_VRING_BUFFER_SIZE;
    vqParam.timeoutCnt    = 100;  /* Wait for counts */
    Ipc_initVirtIO(&vqParam);

    /* Step 3: Initialize RPMessage */
    RPMessage_Params cntrlParam;

#ifdef DEBUG_PRINT
    System_printf("Required Local memory for RPMessage Object = %d\n",
                  RPMessage_getObjMemRequired());
#endif

    /* Initialize the param */
    RPMessageParams_init(&cntrlParam);

    /* Set memory for HeapMemory for control task */
    cntrlParam.buf         = pCntrlBuf;
    cntrlParam.bufSize     = rpmsgDataSize;
    cntrlParam.stackBuffer = NULL;
    cntrlParam.stackSize   = 0U;
    RPMessage_init(&cntrlParam);

    /* Step 4: Setup IPC interrupt handling for baremetal */
    if(IpctestConfigureInterruptHandlers() != 0)
    {
        System_printf("IpctestConfigureInterruptHandlers Failed!!!\n");
        return -1;
    }

#if !defined(BUILD_MPU1_0) && defined(A72_LINUX_OS) && defined(A72_LINUX_OS_IPC_ATTACH)
    rpmsg_vdevMonitorFxn();
#endif

    for (t = 0; t < numProc; t++)
    {
#if !defined(BUILD_MPU1_0) && defined(A72_LINUX_OS)
        /* Linux does not have a responder func running */
        if(pRemoteProcArray[t] == IPC_MPU1_0)
            continue;
#endif
        rpmsg_senderFxn(pRemoteProcArray[t], t);
    }

    /* run responder function to receive and reply messages back */
    rpmsg_responderFxn(0,0);

    return 1;
}
