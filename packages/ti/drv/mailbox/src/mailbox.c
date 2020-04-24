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
 *  \file mailbox.c
 *
 *  \brief The file implements the Mailbox Driver.
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdint.h>
#include <string.h>
#include <ti/osal/MemoryP.h>
#include <ti/drv/mailbox/mailbox.h>
#include <ti/drv/mailbox/src/mailbox_internal.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

static void Mailbox_boxFullISR(uintptr_t arg);
static void Mailbox_boxEmptyISR(uintptr_t arg);

//TODO
#define DebugP_log0(x0)
#define DebugP_log1(x0,x1)
#define DebugP_log2(x0,x1,x2)
#define DebugP_log3(x0,x1,x2,x3)
#define DebugP_log4(x0,x1,x2,x3,x4)

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/**
 * @brief
 *  Global Variable for tracking information required by the mailbox driver.
 */
static Mailbox_MCB gMailboxMCB = {0};

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

/*
 *  ======== Mailbox_init ========
 */
int32_t Mailbox_init(Mailbox_initParams *initParam)
{
    int32_t    retVal = MAILBOX_SOK;
    int32_t    i;
    uintptr_t  key;

    if (initParam == NULL)
    {
        retVal = MAILBOX_EINVAL;
    }
    if (retVal == MAILBOX_SOK)
    {
        retVal = Mailbox_validateLocalEndPoint(initParam->localEndpoint);
    }
    if (retVal == MAILBOX_SOK)
    {
        /* Critical Section Protection*/
        key = HwiP_disable();
        if(gMailboxMCB.initFlag == 0)
        {
            /* Initialize global variables */

            /* Set local endpoint which is the same for all instances of the driver in this processor */
            gMailboxMCB.localEndpoint        = initParam->localEndpoint;
            gMailboxMCB.totalInstCnt         = 0U;
            for (i=0; i< MAILBOX_MAX_NUM_REMOTES_ENDPOINTS; i++)
            {
                Mailbox_RemoteCfg_init(&gMailboxMCB.remoteConfig[i]);
            }
            memset((void *)&(gMailboxMCB.errCnt), 0, sizeof(gMailboxMCB.errCnt));
            memset((void *)&(gMailboxMCB.hwiHandles), 0, sizeof(gMailboxMCB.hwiHandles));

            gMailboxMCB.initFlag = 1;
        }
        else
        {
            DebugP_log0("MAILBOX: Mailbox_init error! Mailbox Driver has already been initialized\n");
            retVal = MAILBOX_EINITIALIZED;
        }
        /* Release the critical section: */
        HwiP_restore(key);
    }

    return retVal;
}

/*
 *  ======== Mailbox_open ========
 */
extern Mbox_Handle Mailbox_open(Mailbox_openParams *openParam,  int32_t* errCode)
{
    Mailbox_Driver*         mailboxDriver;
    Mbox_Handle             retHandle = NULL;
    SemaphoreP_Params       semParams;
    uintptr_t               key;
    Mailbox_HwCfg*          mailboxHwCfg = NULL;
    int32_t                 retVal = MAILBOX_SOK;
    Mailbox_RemoteCfg*  remoteCfg = NULL;
    uint32_t                i;

    *errCode = 0;

    /* Critical Section Protection*/
    key = HwiP_disable();

    /* Sanity Check: Validate the arguments */
    if((gMailboxMCB.initFlag == 0) || (openParam == NULL))
    {
        DebugP_log0("MAILBOX: Mailbox_open error! Mailbox Driver open failed due to bad input argument.\n");
        /* Error: Invalid argument or usage */
        *errCode = MAILBOX_EINVAL;
        goto exit;
    }

    retVal = Mailbox_validateRemoteEndPoint(gMailboxMCB.localEndpoint, openParam->remoteEndpoint);
    if (retVal != MAILBOX_SOK)
    {
        DebugP_log0("MAILBOX: Mailbox_open error! Mailbox Driver open failed due to bad input argument.\n");
        /* Error: Invalid argument or usage */
        *errCode = MAILBOX_EINVAL;
        goto exit;
    }

    /* Check if the driver for this remote is already opened and get the handle. */
    for (i=0; i<MAILBOX_MAX_NUM_REMOTES_ENDPOINTS; i++)
    {
        if(gMailboxMCB.remoteConfig[i].remoteEndpoint == openParam->remoteEndpoint)
        {
            break;
        }
    }
    if(i == MAILBOX_MAX_NUM_REMOTES_ENDPOINTS)
    {
        /* Driver opened first tie for this remote. */
        for (i=0; i<MAILBOX_MAX_NUM_REMOTES_ENDPOINTS; i++)
        {
            if (gMailboxMCB.remoteConfig[i].remoteEndpoint == MAILBOX_INST_INVALID)
            {
                break;
            }
        }
        if (i == MAILBOX_MAX_NUM_REMOTES_ENDPOINTS)
        {
            /* Error: Invalid argument or usage */
            *errCode = MAILBOX_EINVAL;
            goto exit;
        }
        else
        {
            gMailboxMCB.remoteConfig[i].remoteEndpoint = openParam->remoteEndpoint;
        }
        remoteCfg = &(gMailboxMCB.remoteConfig[i]);
    }
    else
    {
        /* Driver is already opened for this remote. */
        remoteCfg = &(gMailboxMCB.remoteConfig[i]);
    }

    /* Check if this is a valid open of a new instance of the driver.
       check if the remote supports multiple instances of the driver.*/

    /*Check if the channel type is valid*/
    if(openParam->cfg.chType > MAILBOX_CHTYPE_MAX)
    {
        DebugP_log1("MAILBOX: Mailbox_open error! Mailbox Driver open failed with bad Channle type parameter (%d).\n",openParam->cfg.chType);
        /* Error: Invalid configuration */
        *errCode = MAILBOX_EBADCHTYPE;
        goto exit;
    }

    if(openParam->cfg.chType == MAILBOX_CHTYPE_MULTI)
    {
        retVal = Mailbox_isMultiChannelSupported(gMailboxMCB.localEndpoint, openParam->remoteEndpoint);
        if (retVal != MAILBOX_SOK)
        {
            DebugP_log1("MAILBOX: Mailbox_open error! Mailbox Driver open failed with bad Channle type parameter (%d).\n",openParam->cfg.chType);
            *errCode = MAILBOX_EBADCHTYPE;
            goto exit;
        }
    }

    /*Check if channel type is consistent*/
    if(remoteCfg->instCount == 0)
    {
        /*First instance of remote. Set global variable to store channel type that must be used for all channel instances.*/
        remoteCfg->chType = openParam->cfg.chType;
    }
    else
    {
        if((remoteCfg->chType != MAILBOX_CHTYPE_MULTI) || (openParam->cfg.chType != MAILBOX_CHTYPE_MULTI))
        {
            DebugP_log1("MAILBOX: Mailbox_open error! Mailbox Driver open failed with bad Channle type parameter (%d).\n",openParam->cfg.chType);
            DebugP_log0("MAILBOX: When using multiple instances all channels must be of type MAILBOX_CHTYPE_MULTI.\n");
            /* Error: Invalid configuration */
            *errCode = MAILBOX_EBADCHTYPE;
            goto exit;
        }
    }

    /*Check if channel ID is consistent. Note that chId is only used for multi-channel.
      Single-channel does not use header or channel ID.*/
    if(openParam->cfg.chType == MAILBOX_CHTYPE_MULTI)
    {
        /*For multi-channel need to check ID value*/
        /*Check if the ID is valid*/
        if(openParam->cfg.chId > MAILBOX_CH_ID_MAX)
        {
            DebugP_log1("MAILBOX: Mailbox_open error! Mailbox Driver open failed with bad Channle ID parameter (%d).\n",openParam->cfg.chId);
            /* Error: Invalid configuration */
            *errCode = MAILBOX_EBADCHID;
            goto exit;
        }

        /*Check if the ID is already in use*/
        if(remoteCfg->handleArray[openParam->cfg.chId] != NULL)
        {
            DebugP_log1("MAILBOX: Mailbox_open error! Mailbox Driver open failed. Channel ID (%d) is already in use.\n",openParam->cfg.chId);
            /* Error: Invalid configuration */
            *errCode = MAILBOX_EBADCHID;
            goto exit;
        }
    }


    /* Check if configuration is valid */
    if( (openParam->cfg.writeMode        == MAILBOX_MODE_CALLBACK) ||
        (openParam->cfg.opMode           != MAILBOX_OPERATION_MODE_PARTIAL_READ_ALLOWED) ||
        (openParam->cfg.dataTransferMode != MAILBOX_DATA_TRANSFER_MEMCPY) )
    {
        DebugP_log0("MAILBOX: Mailbox_open error! Mailbox Driver open failed with bad configuration.\n");
        /* Error: Invalid configuration */
        *errCode = MAILBOX_EINVALCFG;
        goto exit;
    }

    mailboxHwCfg = Mailbox_getHwCfg (openParam->remoteEndpoint);
    if (mailboxHwCfg == NULL)
    {
        DebugP_log0("MAILBOX: Unable to find hardware cfg params.\n");
        /* Error: Invalid configuration */
        *errCode = MAILBOX_EINVALCFG;
        goto exit;
    }

    /* Allocate memory for the Mailbox Driver */
    mailboxDriver = (Mailbox_Driver *) MemoryP_ctrlAlloc((uint32_t)sizeof(Mailbox_Driver), 0);
    if(mailboxDriver == NULL)
    {
        DebugP_log0("MAILBOX: Mailbox_open error! Mailbox Driver memory allocation failed.\n");
        /* Error: No memory available. */
        *errCode = MAILBOX_ENOMEM;
        goto exit;
    }

    /* Initialize the allocated memory: */
    memset((void *)mailboxDriver, 0, sizeof(Mailbox_Driver));

    /* Populate the driver with initial configuration */
    memcpy((void *)&mailboxDriver->cfg, (const void *)&openParam->cfg, sizeof(Mailbox_Config));

    mailboxDriver->remoteEndpoint = openParam->remoteEndpoint;
    mailboxDriver->localEndpoint  = gMailboxMCB.localEndpoint;

    /* Populate hardware configuration */
    mailboxDriver->hwCfg = mailboxHwCfg;
    remoteCfg->hwCfgPtr  = mailboxHwCfg;

    mailboxDriver->remoteCfgPtr = remoteCfg;

    /* Setup the return handle: */
    retHandle = (Mbox_Handle)mailboxDriver;

    if (gMailboxMCB.totalInstCnt == 0U)
    {
        OsalRegisterIntrParams_t interruptRegParams;

        /* Initialize the mailbox */
        Mailbox_initHw(retHandle);

        /* Register the Interrupt Handler: Every mailbox has 2 interrupts: "mailbox full" and "mailbox empty"*/
        /************** Mailbox full ***********/
        /* Initialize with defaults */
        Osal_RegisterInterrupt_initParams(&interruptRegParams);
    #if defined(_TMS320C6X)
        interruptRegParams.corepacConfig.corepacEventNum=(int32_t)(mailboxDriver->hwCfg)->boxFullIntNum; /* Event going in to CPU */
        interruptRegParams.corepacConfig.intVecNum      = OSAL_REGINT_INTVEC_EVENT_COMBINER; /* Host Interrupt vector */
    #else
        interruptRegParams.corepacConfig.priority = 0x1U;
        interruptRegParams.corepacConfig.intVecNum=(int32_t)(mailboxDriver->hwCfg)->boxFullIntNum; /* Host Interrupt vector */
        interruptRegParams.corepacConfig.corepacEventNum = (int32_t)(mailboxDriver->hwCfg)->boxFullIntNum;
    #endif
        /* Populate the interrupt parameters */
        interruptRegParams.corepacConfig.name=(char *)("MAILBOX_FULL");
        interruptRegParams.corepacConfig.isrRoutine=Mailbox_boxFullISR;
        interruptRegParams.corepacConfig.arg=NULL;
        /* Register interrupts */
        Osal_RegisterInterrupt(&interruptRegParams, &(gMailboxMCB.hwiHandles.mailboxFull));
        /* Debug Message: */
        DebugP_log2 ("MAILBOX: Mailbox Driver Registering Mailbox Full HWI ISR [%p] for Interrupt %d\n",
                      gMailboxMCB.hwiHandles.mailboxFull, (mailboxDriver->hwCfg)->boxFullIntNum);
        Osal_EnableInterrupt(interruptRegParams.corepacConfig.corepacEventNum, interruptRegParams.corepacConfig.intVecNum);

        /************** Mailbox empty ***********/
        /* Initialize with defaults */
        Osal_RegisterInterrupt_initParams(&interruptRegParams);
    #if defined(_TMS320C6X)
        interruptRegParams.corepacConfig.corepacEventNum=(int32_t)(mailboxDriver->hwCfg)->boxEmptyIntNum; /* Event going in to CPU */
        interruptRegParams.corepacConfig.intVecNum      = OSAL_REGINT_INTVEC_EVENT_COMBINER; /* Host Interrupt vector */
    #else
        interruptRegParams.corepacConfig.priority = 0x1U;
        interruptRegParams.corepacConfig.intVecNum=(int32_t)(mailboxDriver->hwCfg)->boxEmptyIntNum; /* Host Interrupt vector */
        interruptRegParams.corepacConfig.corepacEventNum = (int32_t)(mailboxDriver->hwCfg)->boxEmptyIntNum;
    #endif
        /* Populate the interrupt parameters */
        interruptRegParams.corepacConfig.name=(char *)("MAILBOX_EMPTY");
        interruptRegParams.corepacConfig.isrRoutine=Mailbox_boxEmptyISR;
        interruptRegParams.corepacConfig.arg=NULL;
        /* Register interrupts */
        Osal_RegisterInterrupt(&interruptRegParams, &(gMailboxMCB.hwiHandles.mailboxEmpty));
        /* Debug Message: */
        DebugP_log2 ("MAILBOX: Mailbox Driver Registering Mailbox Full to Core HWI ISR [%p] for Interrupt %d\n",
                      gMailboxMCB.hwiHandles.mailboxEmpty, (mailboxDriver->hwCfg)->boxEmptyIntNum);
        Osal_EnableInterrupt(interruptRegParams.corepacConfig.corepacEventNum, interruptRegParams.corepacConfig.intVecNum);
    }

    /* Is the write mode blocking? */
    if(mailboxDriver->cfg.writeMode == MAILBOX_MODE_BLOCKING)
    {
        /* YES: Create a binary semaphore which is used to handle the Blocking operation. */
        SemaphoreP_Params_init(&semParams);
        semParams.mode          = SemaphoreP_Mode_BINARY;
        mailboxDriver->writeSem = SemaphoreP_create(0, &semParams);
    }

    /* Is the read mode blocking? */
    if(mailboxDriver->cfg.readMode == MAILBOX_MODE_BLOCKING)
    {
        /* YES: Create a binary semaphore which is used to handle the Blocking operation. */
        SemaphoreP_Params_init(&semParams);
        semParams.mode         = SemaphoreP_Mode_BINARY;
        mailboxDriver->readSem = SemaphoreP_create(0, &semParams);
    }

    /* Instance of the driver for the specific endpoint is created */
    gMailboxMCB.totalInstCnt++;
    remoteCfg->instCount++;
    if (openParam->cfg.chType == MAILBOX_CHTYPE_SINGLE)
    {
        /* Store the handle of driver in first channel.*/
        remoteCfg->handleArray[0U] = retHandle;
    }
    else
    {
        /* Store handle of driver into hash array */
        remoteCfg->handleArray[openParam->cfg.chId] = retHandle;
    }

    DebugP_log1("MAILBOX: Mailbox Driver (%p) open successful.\n", mailboxDriver);

exit:
    if (*errCode != 0)
    {
        /* Restore partially initialized global driver data. */
        if ((remoteCfg != NULL) && (remoteCfg->instCount == 0))
        {
            /* This is first driver open which resulted in error. */
            Mailbox_RemoteCfg_init(remoteCfg);
        }
    }
    /* Release the critical section: */
    HwiP_restore(key);
    return retHandle;
}

/*
 *  ======== Mailbox_read ========
 */
int32_t Mailbox_read(Mbox_Handle handle, uint8_t *buffer, uint32_t size)
{
    Mailbox_Driver*     driver;
    SemaphoreP_Status   status;
    int32_t             retVal = 0;

    driver = (Mailbox_Driver*)handle;

    /* Sanity Check: Validate the arguments */
    if((size == 0) || (buffer == NULL) || (handle == NULL))
    {
        /* Error: Invalid Arguments */
        DebugP_log3 ("MAILBOX: Mailbox_read Error! Invalid param. Size=%d Buffer=(%p) handle=(%p)\n", size, buffer, handle);
        retVal = MAILBOX_EINVAL;
    }
    else
    {
        /* If the size is bigger than maximum mailbox buffer size, read/copy the max available size */
        if(size + driver->numBytesRead > MAILBOX_DATA_BUFFER_SIZE)
        {
            size = MAILBOX_DATA_BUFFER_SIZE - driver->numBytesRead;
        }

        /* First check if this is the first message because it will only block if this is a first message */
        if(driver->numBytesRead == 0)
        {
            /* If mailbox read mode is "blocking", need to pend on semaphore*/
            if(driver->cfg.readMode == MAILBOX_MODE_BLOCKING)
            {
                /* Pend on semaphore until message arrives in mailbox ("mailbox_full" interrupt is received)*/
                status = SemaphoreP_pend (driver->readSem, driver->cfg.readTimeout);
                if(status == SemaphoreP_TIMEOUT)
                {
                    /* Set error code */
                    retVal = MAILBOX_EREADTIMEDOUT;

                    /* Report the error condition: */
                    DebugP_log2 ("MAILBOX:(%p) Mailbox_read timed out. Number of RX messages = %d.\n",
                                 driver, driver->rxCount);
                }
            }

            /* In polling or callback mode the newMessageFlag indicates that a message was received.
               In blocking mode, a posted semaphore indicates that a new message was received.
               If any of these are true, the driver should read the new message.*/
            if( ((driver->cfg.readMode == MAILBOX_MODE_POLLING)  && (driver->newMessageFlag == MAILBOX_NEW_MESSAGE_RECEIVED)) ||
                ((driver->cfg.readMode == MAILBOX_MODE_CALLBACK) && (driver->newMessageFlag == MAILBOX_NEW_MESSAGE_RECEIVED)) ||
                ((driver->cfg.readMode == MAILBOX_MODE_BLOCKING) && (status != SemaphoreP_TIMEOUT)) )
            {
                driver->newMessageFlag = MAILBOX_NEW_MESSAGE_NOT_RECEIVED;
                /* Increment RX count */
                driver->rxCount++;

                /* Copy data from mailbox buffer into application buffer */
                if(driver->cfg.dataTransferMode == MAILBOX_DATA_TRANSFER_MEMCPY)
                {
                    if(driver->cfg.chType == MAILBOX_CHTYPE_MULTI)
                    {
                        /*Read message. Need to account for internal header*/
                        memcpy((void *)buffer, (void *)((uint8_t *)((driver->hwCfg)->baseRemoteToLocal.data) + driver->numBytesRead + MAILBOX_MULTI_CH_HEADER_SIZE), size);
                    }
                    else
                    {
                        /*Read message.*/
                        memcpy((void *)buffer, (void *)((uint8_t *)((driver->hwCfg)->baseRemoteToLocal.data) + driver->numBytesRead), size);
                    }
                }
                else
                {
                    DebugP_log0("MAILBOX: Mailbox_read Error! Only memcpy dataTransferMode is supported\n");
                    retVal = MAILBOX_EINVALCFG;
                    goto exit;
                }

                /* Set return value */
                retVal = (int32_t)size;

                /* Update number of bytes read for this message*/
                driver->numBytesRead = driver->numBytesRead + size;
            }
        }
        else
        {
            /* This is a subsequent read for a message that is already in the mailbox buffer */
            /* Copy data from mailbox buffer into application buffer */
            if(driver->cfg.dataTransferMode == MAILBOX_DATA_TRANSFER_MEMCPY)
            {
                if(driver->cfg.chType == MAILBOX_CHTYPE_MULTI)
                {
                    /*Read message. Need to account for internal header*/
                    memcpy((void *)buffer, (void *)((uint8_t *)((driver->hwCfg)->baseRemoteToLocal.data) + driver->numBytesRead + MAILBOX_MULTI_CH_HEADER_SIZE), size);
                }
                else
                {
                    /*Read message.*/
                    memcpy((void *)buffer, (void *)((uint8_t *)((driver->hwCfg)->baseRemoteToLocal.data) + driver->numBytesRead), size);
                }
            }
            else
            {
                DebugP_log0("MAILBOX: Mailbox_read Error! Only memcpy dataTransferMode is supported\n");
                retVal = MAILBOX_EINVALCFG;
                goto exit;
            }

            /* Set return value */
            retVal = (int32_t)size;

            /* Update number of bytes read for this message*/
            driver->numBytesRead = driver->numBytesRead + size;
        }
    }

exit:
    return retVal;
}

/*
 *  ======== Mailbox_readFlush ========
 */
int32_t Mailbox_readFlush(Mbox_Handle handle)
{
    Mailbox_Driver*     driver;
    int32_t             retVal = 0;
    uintptr_t           key;

    /* Sanity Check: Validate the arguments */
    if(handle == NULL)
    {
        /* Error: Invalid Arguments */
        DebugP_log0("MAILBOX: Mailbox_readFlush Error! Null handle\n");
        retVal = MAILBOX_EINVAL;
    }
    else
    {
        driver = (Mailbox_Driver*)handle;

        /* If this is multi-channel, make sure mailbox is not being used by another instance */
        if(driver->cfg.chType == MAILBOX_CHTYPE_MULTI)
        {
            /* Critical Section Protection*/
            key = HwiP_disable();
            if(driver->remoteCfgPtr->readChIDInUse != driver->cfg.chId)
            {
                retVal = MAILBOX_ECHINUSE;
                /* Release the critical section: */
                HwiP_restore(key);
                goto exit;
            }
            else
            {
                /* Indicate that channel is no longer in use */
                driver->remoteCfgPtr->readChIDInUse = MAILBOX_UNUSED_CHANNEL_ID;
                /* Release the critical section: */
                HwiP_restore(key);
            }
        }

        driver->readFlushCount++;

        /* Reset number of bytes read*/
        driver->numBytesRead = 0;

        if(driver->hwCfg != NULL)
        {
            /* Send acknowledgement to remote endpoint */
            CSL_Mbox_triggerAckInterrupt(driver->hwCfg->mbxReg, driver->hwCfg->remoteProcNum);
        }
        else
        {
            /* Error: Invalid Arguments */
            DebugP_log0("MAILBOX: Mailbox_readFlush Error! Null hardware configuration.\n");
            retVal = MAILBOX_EINVAL;
        }
    }

exit:
    return retVal;
}

/*
 *  ======== Mailbox_write ========
 */
int32_t Mailbox_write(Mbox_Handle handle, const uint8_t *buffer, uint32_t size)
{
    Mailbox_Driver*     driver;
    SemaphoreP_Status   status;
    int32_t             retVal = 0;
    uint32_t            header = 0;
    uintptr_t           key;

    driver = (Mailbox_Driver*)handle;

    /* Sanity Check: Validate the arguments */
    if((size == 0) || (size > MAILBOX_DATA_BUFFER_SIZE) || (buffer == NULL) || (handle == NULL) || (driver->hwCfg == NULL))
    {
        /* Error: Invalid Arguments */
        DebugP_log4 ("MAILBOX: Mailbox_write Error! Invalid param. Size=%d Buffer=(%p) handle=(%p) hwCfgPtr=(%p)\n", size, buffer, handle, driver->hwCfg);
        retVal = MAILBOX_EINVAL;
        goto exit;
    }

    if(driver->cfg.chType == MAILBOX_CHTYPE_MULTI)
    {
        /* Critical Section Protection*/
        key = HwiP_disable();
        if(driver->remoteCfgPtr->writeChIDInUse == MAILBOX_UNUSED_CHANNEL_ID)
        {
            /* Mark that the TX mailbox is now in use*/
            driver->remoteCfgPtr->writeChIDInUse = driver->cfg.chId;
            /* Release the critical section: */
            HwiP_restore(key);
        }
        else
        {
            /* Error: TX mailbox is being used by another mailbox instance*/
            DebugP_log2 ("MAILBOX: Mailbox_write Error! handle=(%p). Write attempt with TX box in use by channel ID %d\n",driver, driver->remoteCfgPtr->writeChIDInUse);
            retVal = MAILBOX_ECHINUSE;
            /* Release the critical section: */
            HwiP_restore(key);
            goto exit;
        }
    }

    if(driver->txBoxStatus == MAILBOX_TX_BOX_FULL)
    {
        /* Error: TX mailbox is full, can not write new message until acknowledge is received from remote endpoint */
        /* Note that this should take care that the DMA has been completed as well because this flag is cleaned only after
           copy is done */
        DebugP_log1("MAILBOX: Mailbox_write Error! handle=(%p). Write attempt with txBoxStatus == MAILBOX_TX_BOX_FULL\n", handle);
        retVal = MAILBOX_ETXFULL;
        goto exit;
    }

    /* A write operation is starting, need to set TXbox flag to full to block any other write to this instance of mailbox*/
    driver->txBoxStatus = MAILBOX_TX_BOX_FULL;

    /* Copy data from application buffer to mailbox buffer */
    if(driver->cfg.dataTransferMode == MAILBOX_DATA_TRANSFER_MEMCPY)
    {
        if(driver->cfg.chType == MAILBOX_CHTYPE_MULTI)
        {
            /*Write internal header*/
            header = driver->cfg.chId;
            memcpy((void *)(driver->hwCfg)->baseLocalToRemote.data, (void *)(&header), sizeof(header));
            /*Write message. Need to account for internal header size*/
            memcpy((void *)((uint8_t *)((driver->hwCfg)->baseLocalToRemote.data) + MAILBOX_MULTI_CH_HEADER_SIZE), (const void *)buffer, size);
        }
        else
        {
            /*Write message.*/
            memcpy((void *)(driver->hwCfg)->baseLocalToRemote.data, (const void *)buffer, size);
        }

#if defined (__TI_ARM_V7R4__)
        //MEM_BARRIER(); //TODO
#endif
    }
    else
    {
        DebugP_log0("MAILBOX: Mailbox_write Error! Only memcpy dataTransferMode is supported\n");
        retVal = MAILBOX_EINVALCFG;
        goto exit;
    }

    /* Store handle of instance */
    driver->remoteCfgPtr->lastMsgSentHandle = handle;

    /* Trigger "mailbox full" interrupt to remote endpoint*/
    CSL_Mbox_triggerTxInterrupt(driver->hwCfg->mbxReg, driver->hwCfg->remoteProcNum);

    /* Next action depends on the mailbox write mode*/
    if(driver->cfg.writeMode == MAILBOX_MODE_BLOCKING)
    {
        /* Pend on semaphore until acknowledge ("mailbox_empty" interrupt) from remote endpoint is received*/
        status = SemaphoreP_pend (driver->writeSem, driver->cfg.writeTimeout);
        if(status == SemaphoreP_TIMEOUT)
        {
            /* Set error code */
            retVal = MAILBOX_ETXACKTIMEDOUT;

            /* Report the error condition: */
            DebugP_log2 ("MAILBOX:(%p) Write acknowledge timed out. Ack was never received. Number of received TX messages = %d.\n",
                         driver, driver->txCount);
        }
    }

    /* If write is blocking mode and semaphore did not timeout, write succeeded and ack received.
       If write is polling mode and we reached this point, write was done but not sure if ack has been received. */
    if( ((driver->cfg.writeMode == MAILBOX_MODE_BLOCKING) && (status != SemaphoreP_TIMEOUT)) ||
        (driver->cfg.writeMode == MAILBOX_MODE_POLLING) )
    {
        /* Increment TX count */
        driver->txCount++;

        /* Set return value */
        retVal = (int32_t)size;
    }

exit:
    return retVal;
}

/*
 *  ======== Mailbox_close ========
 */
int32_t Mailbox_close(Mbox_Handle handle)
{
    int32_t            retVal = 0;
    Mailbox_Driver*    driver;
    uintptr_t          key;
    int32_t            interruptNum;
    Mailbox_RemoteCfg* remoteCfg;

    /* Get the mailbox driver */
    driver = (Mailbox_Driver*)handle;

    if(handle == NULL)
    {
        /* Error: Invalid Arguments */
        DebugP_log0("MAILBOX: Mailbox_close Error! Null handle");
        retVal = MAILBOX_EINVAL;
    }

    if (retVal == 0)
    {
        remoteCfg = driver->remoteCfgPtr;
        /* Critical Section Protection:
         * close needs to be protected against multiple threads */
        key = HwiP_disable();
        /* Reset the array for this instance of the driver */
        remoteCfg->handleArray[driver->cfg.chId] = NULL;

        remoteCfg->instCount--;
        gMailboxMCB.totalInstCnt--;

        /*Unregister ISRs if there are no more instances */
        if(gMailboxMCB.totalInstCnt == 0)
        {
            /* Was the Full HWI registered?  */
            if(gMailboxMCB.hwiHandles.mailboxFull != NULL)
            {
            #if defined (_TMS320C6X)
                interruptNum = OSAL_REGINT_INTVEC_EVENT_COMBINER;
            #else
                interruptNum = (int32_t)(driver->hwCfg)->boxFullIntNum;
            #endif
                Osal_DisableInterrupt((int32_t)(driver->hwCfg)->boxFullIntNum, interruptNum);

                /* YES: Delete and unregister the interrupt handler. */
                Osal_DeleteInterrupt(gMailboxMCB.hwiHandles.mailboxFull, (int32_t)(driver->hwCfg)->boxFullIntNum);
                gMailboxMCB.hwiHandles.mailboxFull = NULL;
                gMailboxMCB.errCnt.mailboxFull = 0;
            }

            /* Was the Empty HWI registered?  */
            if(gMailboxMCB.hwiHandles.mailboxEmpty != NULL)
            {
            #if defined (_TMS320C6X)
                interruptNum = OSAL_REGINT_INTVEC_EVENT_COMBINER;
            #else
                interruptNum = (int32_t)(driver->hwCfg)->boxEmptyIntNum;
            #endif
                Osal_DisableInterrupt((int32_t)(driver->hwCfg)->boxEmptyIntNum, interruptNum);

                /* YES: Delete and unregister the interrupt handler. */
                Osal_DeleteInterrupt(gMailboxMCB.hwiHandles.mailboxEmpty, (int32_t)(driver->hwCfg)->boxEmptyIntNum);
                gMailboxMCB.hwiHandles.mailboxEmpty = NULL;
                gMailboxMCB.errCnt.mailboxEmpty = 0;
            }
        }
        /* Release the critical section: */
        HwiP_restore(key);

        /* Was the driver operating in Write Blocking mode? */
        if(driver->writeSem)
        {
            /* YES: Delete the write semaphore */
            SemaphoreP_delete(driver->writeSem);
        }

        /* Was the driver operating in Read Blocking mode? */
        if(driver->readSem)
        {
            /* YES: Delete the read semaphore */
            SemaphoreP_delete(driver->readSem);
        }

        /* Cleanup the memory: */
        MemoryP_ctrlFree(driver, (uint32_t)sizeof(Mailbox_Driver));
    }

    return retVal;
}

/*
 *  ======== Mailbox_getStats ========
 */
int32_t Mailbox_getStats(Mbox_Handle handle, Mailbox_Stats * stats)
{
    int32_t             retVal = 0;
    Mailbox_Driver*     driver;

    /* Get the mailbox driver */
    driver = (Mailbox_Driver*)handle;

    if(handle == NULL)
    {
        /* Error: Invalid Arguments */
        DebugP_log0("MAILBOX: Mailbox_getStats Error! Null handle");
        retVal = MAILBOX_EINVAL;
    }
    else if(stats == NULL)
    {
        /* Error: Invalid Arguments */
        DebugP_log0("MAILBOX: Mailbox_getStats Error! Null pointer");
        retVal = MAILBOX_EINVAL;
    }
    else
    {
        stats->txBoxStatus         = (uint32_t) driver->txBoxStatus;
        stats->txCount             = driver->txCount;
        stats->rxCount             = driver->rxCount;
        stats->newMessageFlag      = (uint32_t) driver->newMessageFlag;
        stats->numBytesRead        = driver->numBytesRead;
        stats->boxFullIsrCount     = driver->boxFullIsrCount;
        stats->boxEmptyIsrCount    = driver->boxEmptyIsrCount;
        stats->readFlushCount      = driver->readFlushCount;
        stats->boxEmptyIsrErrCount = gMailboxMCB.errCnt.mailboxEmpty;
        stats->boxFullIsrErrCount  = gMailboxMCB.errCnt.mailboxFull;
    }

    return retVal;
}


/**
 *  @b Description
 *  @n
 *      The function is the registered ISR for the "mailbox full" interrupt.
 *
 *  @param[in]  driver
 *      Driver handle
 *
 *  @retval
 *      Not applicable
 *
 *  \ingroup MAILBOX_DRIVER_INTERNAL_FUNCTION
 */
static void Mailbox_boxFullISRProcessing(Mailbox_Driver* driver)
{
    if(driver != NULL)
    {
        driver->boxFullIsrCount++;

        /* set flag to inform that new message received */
        driver->newMessageFlag = MAILBOX_NEW_MESSAGE_RECEIVED;

        /* Action depends on the read mode */
        if(driver->cfg.readMode == MAILBOX_MODE_POLLING)
        {
            /* Do nothing. Flag set above is enough */
        }
        else if(driver->cfg.readMode == MAILBOX_MODE_BLOCKING)
        {
            /* Post semaphore */
            if(driver->readSem)
            {
                /* Post the semaphore to unblock calling thread. */
                SemaphoreP_postFromISR(driver->readSem);
            }
        }
        else /* Call back read mode */
        {
            /* Check if call back function is implemented */
            if(driver->cfg.readCallback != NULL)
            {
                (*(driver->cfg.readCallback))(driver, driver->remoteEndpoint);
            }
        }

        /* Clear the status register */
        if(driver->hwCfg != NULL)
        {
            CSL_Mbox_clearBoxFullInterrupt(driver->hwCfg->mbxReg, driver->hwCfg->remoteProcNum);
        }
    }
}

/**
 *  @b Description
 *  @n
 *      The function is the registered ISR for the "mailbox full" interrupt.
 *
 *  @param[in]  arg
 *      Argument which is registered with the OS while registering
 *      the ISR
 *
 *  @retval
 *      Not applicable
 *
 *  \ingroup MAILBOX_DRIVER_INTERNAL_FUNCTION
 */
static void Mailbox_boxFullISR(uintptr_t arg)
{
    uint32_t         header;
    uint8_t          id = MAILBOX_UNUSED_CHANNEL_ID;
    Mailbox_Instance     remoteEndpoint;
    int32_t          retVal;
    Mailbox_RemoteCfg *remoteCfg;
    uint32_t            i;

    retVal = Mailbox_getBoxFullRemoteInst(&remoteEndpoint);

    if (retVal == MAILBOX_SOK)
    {
        /* Get the remote Cfg */
        for (i=0; i<MAILBOX_MAX_NUM_REMOTES_ENDPOINTS; i++)
        {
            if (gMailboxMCB.remoteConfig[i].remoteEndpoint == remoteEndpoint)
            {
                break;
            }
        }
        if (i != MAILBOX_MAX_NUM_REMOTES_ENDPOINTS)
        {
            remoteCfg = &gMailboxMCB.remoteConfig[i];
            if (remoteCfg->chType == MAILBOX_CHTYPE_SINGLE)
            {
                Mailbox_boxFullISRProcessing((Mailbox_Driver*) remoteCfg->handleArray[0]);
            }
            else
            {
                /* Get the Channel Id from the header. */
                /* First need to find the ID in the received message*/
                if(remoteCfg->hwCfgPtr != NULL)
                {
                    memcpy((void *)&header, (void *)(remoteCfg->hwCfgPtr->baseRemoteToLocal.data), sizeof(header));
                    id = (uint8_t)(header & 0x7U);
                }

                if(id > MAILBOX_CH_ID_MAX)
                {
                    /*error*/
                    gMailboxMCB.errCnt.mailboxFull++;
                }
                else
                {
                    Mailbox_boxFullISRProcessing((Mailbox_Driver*) remoteCfg->handleArray[id]);
                }
            }
        }
        else
        {
            /* TODO: Sphurious interrupt, Not registerded remote proc. Disable it. */
            gMailboxMCB.errCnt.mailboxFull++;
        }
    }
    return;
}

/**
 *  @b Description
 *  @n
 *      The function is the registered ISR for the "mailbox empty" interrupt.
 *      This interrupt indicates that an acknowledge for the last write operation was received.
 *
 *  @param[in]  driver
 *      Driver handle.
 *
 *  @retval
 *      Not applicable
 *
 *  \ingroup MAILBOX_DRIVER_INTERNAL_FUNCTION
 */
static void Mailbox_boxEmptyISRProcessing(Mailbox_Driver* driver)
{
    if(driver != NULL)
    {
        /* Acknowldedgement from remote endpoint has been received */
        /* Local endpoint clears the "mailbox empty" interrupt */
        if(driver->hwCfg != NULL)
        {
            CSL_Mbox_clearTxAckInterrupt(driver->hwCfg->mbxReg, driver->hwCfg->remoteProcNum);
        }

        driver->boxEmptyIsrCount++;

        /* update txBox status flag */
        driver->txBoxStatus = MAILBOX_TX_BOX_EMPTY;

        /* update TX box multichannel status */
        if(driver->cfg.chType == MAILBOX_CHTYPE_MULTI)
        {
            driver->remoteCfgPtr->writeChIDInUse = MAILBOX_UNUSED_CHANNEL_ID;
        }

        /* Action depends on the write mode */
        if(driver->cfg.writeMode == MAILBOX_MODE_BLOCKING)
        {
            if(driver->writeSem)
            {
                /* Post the semaphore to unblock calling thread. */
                SemaphoreP_postFromISR(driver->writeSem);
            }
        }
    }
}

/**
 *  @b Description
 *  @n
 *      The function is the registered ISR for the "mailbox empty" interrupt.
 *      This interrupt indicates that an acknowledge for the last write operation was received.
 *
 *  @param[in]  arg
 *      Argument which is registered with the OS while registering
 *      the ISR
 *
 *  @retval
 *      Not applicable
 *
 *  \ingroup MAILBOX_DRIVER_INTERNAL_FUNCTION
 */
static void Mailbox_boxEmptyISR(uintptr_t arg)
{
    Mailbox_Instance     remoteEndpoint;
    int32_t          retVal;
    Mailbox_RemoteCfg *remoteCfg;
    uint32_t            i;

    retVal = Mailbox_getBoxEmptyRemoteInst(&remoteEndpoint);

    if (retVal == MAILBOX_SOK)
    {
        /* Get the remote Cfg */
        for (i=0; i<MAILBOX_MAX_NUM_REMOTES_ENDPOINTS; i++)
        {
            if (gMailboxMCB.remoteConfig[i].remoteEndpoint == remoteEndpoint)
            {
                break;
            }
        }
        if (i != MAILBOX_MAX_NUM_REMOTES_ENDPOINTS)
        {
            remoteCfg = &gMailboxMCB.remoteConfig[i];
            if (remoteCfg->lastMsgSentHandle != NULL)
            {
                Mailbox_boxEmptyISRProcessing((Mailbox_Driver*) remoteCfg->lastMsgSentHandle);
            }
            else
            {
                /* TODO: Sphurious interrupt, clear it. */
                gMailboxMCB.errCnt.mailboxEmpty++;
            }
        }
        else
        {
            /* TODO: Sphurious interrupt, Not registerded remote proc. Disable it. */
            gMailboxMCB.errCnt.mailboxFull++;
        }
    }
    return;
}
