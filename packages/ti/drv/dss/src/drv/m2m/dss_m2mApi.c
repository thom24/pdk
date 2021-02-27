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
 *  \file dss_dispApi.c
 *
 *  \brief File containing the DSS display driver implementation.
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <ti/drv/dss/dss.h>
#include <ti/drv/dss/src/drv/common/dss_evtMgr.h>
#include <ti/drv/dss/src/drv/disp/dss_dispDrv.h>
#include <ti/drv/dss/src/drv/disp/dss_dispPriv.h>
#include <ti/drv/dss/src/drv/dctrl/dss_dctrlDrv.h>
#include <ti/drv/dss/src/drv/dctrl/dss_dctrlPriv.h>
#include <ti/drv/dss/src/drv/m2m/dss_m2mDrv.h>
#include <dss_soc_priv.h>

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
/**
 *  Below ifdef __cplusplus is added so that C++ build passes without
 *  typecasting. This is because the prototype is build as C type
 *  whereas this file is build as CPP file. Hence we get C++ build error.
 *  Also if typecasting is used, then we get MisraC error Rule 11.1.
 */
#ifdef __cplusplus
extern "C" {
#endif


#ifdef __cplusplus
}
#endif

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */
DssM2MDrv_CommonObj gDssM2mCommonObj;

extern Dss_EvtMgrClientInfo gDss_DispEvtMgrClientInfo[DSS_DISP_EVT_MGR_MAX_CLIENTS];
/* ========================================================================== */
/*                  Internal/Private Function Declarations                    */
/* ========================================================================== */
static int32_t Dss_m2mDrvSetWbPipeDmaCfg(DssM2MDrv_InstObj *instObj,
                                         const CSL_DssWbPipeDmaCfg *dmaCfg);

int32_t Dss_m2mDrvCreateInstQueues(DssM2MDrv_VirtContext *context);

int32_t Dss_m2mDrvDeleteInstQueues(DssM2MDrv_VirtContext *context);
/*
 * Display IOCTLs
 */
static int32_t Dss_m2mDrvIoctlSetWbPipeParams(DssM2MDrv_VirtContext *context,
                                       const Dss_WbPipeCfgParams *pipeCfg);

static int32_t Dss_m2mDrvIoctlSetmFlagParams(DssM2MDrv_VirtContext *context,
                                      const Dss_WbPipeMflagParams *mFlagParams);

static int32_t Dss_m2mDrvIoctlGetCurrentStatus(DssM2MDrv_VirtContext *context,
                                               Dss_WbStatus *status);

static int32_t Dss_m2mDrvIoctlSetWbPipeCsc(DssM2MDrv_VirtContext *context,
                                    const CSL_DssCscCoeff *csc);

static int32_t Dss_m2mDrvIoctlSetDmaCfg(DssM2MDrv_VirtContext *context,
                                        const CSL_DssWbPipeDmaCfg *dmaCfg);

extern int32_t Dss_m2mDrvIoctlSetDssPipeParams(DssM2MDrv_VirtContext *context,
                                               const Dss_DispParams *pipeCfg);

extern int32_t Dss_m2mDrvIoctlSetPiprMflagParams(DssM2MDrv_VirtContext *context,
                                  const Dss_DispPipeMflagParams *mFlagParams);

extern int32_t Dss_m2mDrvIoctlSetPipeCsc(DssM2MDrv_VirtContext *context,
                                         const CSL_DssCscCoeff *csc);

extern int32_t Dss_m2mDrvIoctlSetOverlayParams(DssM2MDrv_VirtContext *context,
                                    const Dss_DctrlOverlayParams *ovlParams);

extern int32_t Dss_m2mDrvIoctlSetLayerParams(DssM2MDrv_VirtContext *context,
                        const Dss_DctrlOverlayLayerParams *ovlLayerParams);

extern int32_t Dss_m2mDrvIoctlSetGlobalParams(DssM2MDrv_VirtContext *context,
                               const Dss_DctrlGlobalDssParams *globalParams);
/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */
/* FVID2 independent functions: Init and deinit*/
int32_t Dss_m2mDrvInit(const Dss_InitParams *initParams)
{
    int32_t retVal = FVID2_SOK;
    DssM2MDrv_CommonObj *m2mObj;
    uint32_t loopCnt = 0U, contextCnt = 0U;
    DssM2MDrv_VirtContext *virtContext;
    DssM2MDrv_InstObj *instObj;
    SemaphoreP_Params semParams;
    const Dss_SocInfo *socInfo;

    m2mObj = &gDssM2mCommonObj;
    GT_assert(DssTrace, (NULL != m2mObj));

    /* Reset full driver object to '0' and set only required parameters */
    Fvid2Utils_memset(m2mObj, 0U, sizeof(DssM2MDrv_CommonObj));
    socInfo = Dss_getSocInfo();
    Fvid2Utils_memcpy(&m2mObj->socInfo,
                      socInfo,
                      sizeof(Dss_SocInfo));

    m2mObj->getTimeStamp = NULL;
    m2mObj->isRegistered = (uint32_t)FALSE;

    m2mObj->lockSem = SemaphoreP_create(1U, &semParams);
    if (m2mObj->lockSem == NULL)
    {
        GT_0trace(DssTrace, GT_ERR, "Instance semaphore create failed!!\r\n");
        retVal = FVID2_EALLOC;
    }

    for (loopCnt = 0U ; loopCnt < DSSM2M_NUM_WB_PIPELINE ; loopCnt++)
    {
        m2mObj->numVirtContUsed[loopCnt] = 0U;
        instObj                          = &m2mObj->instObj[loopCnt];
        instObj->drvInstId               = loopCnt;
        instObj->inUse                   = DSSM2M_DRV_USAGE_STATUS_NOT_USED;
        instObj->commonObjRef            = m2mObj;
        instObj->wbRegs                  = socInfo->wbRegs[loopCnt];
        instObj->pipeId                  = CSL_DSS_VID_PIPE_ID_MAX;
        instObj->overlayId               = CSL_DSS_OVERLAY_ID_MAX;
        /* Allocate instance semaphore */
        SemaphoreP_Params_init(&semParams);
        semParams.mode           = SemaphoreP_Mode_BINARY;
        for (contextCnt = 0U ;
             contextCnt < DSSM2M_NUM_VIRTUAL_CONTEXT ;
             contextCnt++)
        {
            virtContext            = &m2mObj->virtContext[loopCnt][contextCnt];
            virtContext->pipeId    = CSL_DSS_VID_PIPE_ID_MAX;
            virtContext->overlayId = CSL_DSS_OVERLAY_ID_MAX;
            virtContext->contextId = contextCnt;
            virtContext->state     = DSSM2M_DRV_STATE_IDLE;
            virtContext->inUse     = DSSM2M_DRV_USAGE_STATUS_NOT_USED;
            virtContext->instObj   = instObj;
        }
    }

    if (FVID2_SOK != retVal)
    {
        /* Initialize Driver operations */
        Fvid2DrvOps_init(&m2mObj->fvidDrvOps);
        m2mObj->fvidDrvOps.drvId                  = DSS_M2M_DRV_ID;
        m2mObj->fvidDrvOps.createFxn              = &Dss_m2mCreate;
        m2mObj->fvidDrvOps.deleteFxn              = &Dss_m2mDelete;
        m2mObj->fvidDrvOps.controlFxn             = &Dss_m2mControl;
        m2mObj->fvidDrvOps.processRequestFxn      = &Dss_m2mProcessRequest;
        m2mObj->fvidDrvOps.getProcessedRequestFxn = &Dss_m2mGetProcessedRequest;

        retVal = Fvid2_registerDriver(&m2mObj->fvidDrvOps);
        if (FVID2_SOK != retVal)
        {
            GT_0trace(DssTrace, GT_ERR,
                "Registering to FVID2 driver manager failed\r\n");
            /* Un-initialize the internal objects if error occurs: deInit() */
        }
        else
        {
            /* Init successful */
            m2mObj->isRegistered = (uint32_t) TRUE;
        }
    }


    return (retVal);
}

int32_t Dss_m2mDrvDeInit(void)
{
    int32_t retVal = FVID2_SOK;
    DssM2MDrv_CommonObj *m2mObj;
    uint32_t loopCnt = 0U, contextCnt = 0U;
    DssM2MDrv_VirtContext *virtContext;
    DssM2MDrv_InstObj *instObj;

    m2mObj = &gDssM2mCommonObj;
    GT_assert(DssTrace, (NULL != m2mObj));

    if (m2mObj->lockSem != NULL)
    {
        /* Delete semaphore */
        (void)SemaphoreP_delete(m2mObj->lockSem);
    }
    for (loopCnt = 0U ; loopCnt < DSSM2M_NUM_WB_PIPELINE ; loopCnt++)
    {
        instObj                          = &m2mObj->instObj[loopCnt];
        instObj->inUse                   = DSSM2M_DRV_USAGE_STATUS_NOT_USED;
        instObj->commonObjRef            = NULL;
        instObj->commRegs                = NULL;
        instObj->pipeRegs                = NULL;
        instObj->ovrRegs                 = NULL;
        instObj->wbRegs                  = NULL;
        for (contextCnt = 0U ;
             contextCnt < DSSM2M_NUM_VIRTUAL_CONTEXT ;
             contextCnt++)
        {
            virtContext            = &m2mObj->virtContext[loopCnt][contextCnt];
            virtContext->state     = DSSM2M_DRV_STATE_UNINIT;
            virtContext->inUse     = DSSM2M_DRV_USAGE_STATUS_NOT_USED;
            virtContext->instObj   = NULL;
        }
    }

    /* unregister driver */
    retVal = Fvid2_unRegisterDriver(&m2mObj->fvidDrvOps);
    if (retVal != FVID2_SOK)
    {
        GT_0trace(DssTrace, GT_ERR,
            "Unregistering from FVID2 driver manager failed\r\n");
    }

    return (retVal);
}

/* FVID2 Functions */
Fdrv_Handle Dss_m2mCreate(uint32_t drvId,
                          uint32_t instId,
                          void *createArgs,
                          void *createStatusArgs,
                          const Fvid2_DrvCbParams *fdmCbParams)
{
    Fdrv_Handle drvHandle = NULL;
    int32_t retVal = FVID2_SOK;
    DssM2MDrv_CommonObj *m2mObj;
    Dss_WbCreateParams *createParams;
    Dss_WbCreateStatus *createStatus;
    DssM2MDrv_VirtContext *virtContext;
    DssM2MDrv_InstObj *instObj;
    uint32_t loopCnt = 0U;

    m2mObj = &gDssM2mCommonObj;
    GT_assert(DssTrace, (NULL != m2mObj));

    /* Check for NULL pointers and invalid arguments */
    if ((NULL == createStatusArgs) ||
        (NULL == fdmCbParams) ||
        (DSS_M2M_DRV_ID != drvId) ||
        (DSS_M2M_WB_PIPELINE_ID_MAX <= instId))
    {
        GT_0trace(DssTrace, GT_ERR, "Invalid arguments\r\n");
        retVal = FVID2_EBADARGS;
    }
    else
    {
        createParams = (Dss_WbCreateParams *)createArgs;
        createStatus = (Dss_WbCreateStatus *)createStatusArgs;
        /* Get free Virtual Context */
        for (loopCnt = 0U ; loopCnt < DSSM2M_NUM_VIRTUAL_CONTEXT ; loopCnt++)
        {
            if (m2mObj->virtContext[createParams->instId][loopCnt].inUse ==
                                DSSM2M_DRV_USAGE_STATUS_NOT_USED)
            {
                break;
            }
        }
        if (DSSM2M_NUM_VIRTUAL_CONTEXT <= loopCnt)
        {
            retVal = FVID2_EALLOC;
            GT_0trace(DssTrace, GT_ERR, "No more instances to allocate...\r\n");
        }
        else
        {
            virtContext = &m2mObj->virtContext[createParams->instId][loopCnt];
            instObj = virtContext->instObj;
            GT_assert(DssTrace, (NULL != instObj->commonObjRef));
            if(NULL != instObj->commonObjRef->lockSem)
            {
                /* Take the instance semaphore */
                (void) SemaphoreP_pend(instObj->commonObjRef->lockSem,
                                       SemaphoreP_WAIT_FOREVER);
            }
            /* Update app provided configurations in the instance object */
            /* Copy create and Call-back parameters */
            Fvid2Utils_memcpy(&virtContext->createParams,
                              createParams,
                              sizeof(Dss_WbCreateParams));
            Fvid2Utils_memcpy(&virtContext->fdmCbParams,
                              fdmCbParams,
                              sizeof(Fvid2_DrvCbParams));
            /* Set status to '0' */
            Fvid2Utils_memset(&virtContext->wbStatus, 0U, sizeof(Dss_WbStatus));
            /* Clear instance configurations,
               these will be later set by IOCTLs */
            Fvid2Utils_memset(&virtContext->instCfg,
                              0U,
                              sizeof(DssM2MDrv_moduleCfg));
            virtContext->pipeId    = createParams->pipeId;
            virtContext->overlayId = createParams->overlayId;
            /* Update WB pipeline DMA configurations */
            Fvid2Utils_memcpy(&virtContext->instCfg.wbPipeCfg.dmaCfg,
                              &createParams->dmaCfg,
                              sizeof(CSL_DssWbPipeDmaCfg));
            /* Configure WB pipeline */
            retVal = Dss_m2mDrvSetWbPipeDmaCfg(instObj,
                                               &createParams->dmaCfg);

            if (FVID2_SOK == retVal)
            {
                virtContext->doneQ = NULL;
                retVal = Fvid2Utils_constructQ(&virtContext->doneLlObj);
                GT_assert(DssTrace, (retVal == FVID2_SOK));
                virtContext->doneQ = &virtContext->doneLlObj;
            }

            if (FVID2_SOK == retVal)
            {
                if ((0U == m2mObj->numVirtContUsed[createParams->instId]) &&
                    (DSSM2M_DRV_USAGE_STATUS_NOT_USED == instObj->inUse))
                {
                    instObj->pipeId    = virtContext->pipeId;
                    instObj->overlayId = virtContext->overlayId;
                    /* Set instance status */
                    Fvid2Utils_memset(&instObj->status,
                                      0U,
                                      sizeof(Dss_WbStatus));
                    /* Create Queues for Buffer Management - Do this for
                       only first create call*/
                    retVal = Dss_m2mDrvCreateInstQueues(virtContext);
                }

                if (FVID2_SOK == retVal)
                {
                    /* Set states of driver object and instance object */
                    virtContext->numBufQed = 0U;
                    virtContext->inUse     = DSSM2M_DRV_USAGE_STATUS_IN_USE;
                    virtContext->state     = DSSM2M_DRV_STATE_CREATED;
                    instObj->inUse         = DSSM2M_DRV_USAGE_STATUS_IN_USE;
                    m2mObj->numVirtContUsed[createParams->instId]++;
                    drvHandle = (Fdrv_Handle)virtContext;
                }
            }

            if(NULL != instObj->commonObjRef->lockSem)
            {
                /* Post the instance semaphore */
                (void) SemaphoreP_post(instObj->commonObjRef->lockSem);
            }
        }
    }

    if (NULL != createStatusArgs)
    {
        createStatus->retVal = retVal;
    }

    return (drvHandle);
}

int32_t Dss_m2mDelete(Fdrv_Handle handle, void *reserved)
{
    int32_t retVal = FVID2_SOK;
    DssM2MDrv_CommonObj *m2mObj;
    DssM2MDrv_VirtContext *virtContext;
    DssM2MDrv_InstObj *instObj;
    DssM2M_DrvQueObj *qObj;

    m2mObj = &gDssM2mCommonObj;
    GT_assert(DssTrace, (NULL == m2mObj));

    if (NULL == handle)
    {
        retVal = FVID2_EBADARGS;
    }
    else
    {
        virtContext = (DssM2MDrv_VirtContext *)handle;
        instObj = virtContext->instObj;

        /* Set states of driver object and instance object */
        virtContext->inUse = DSSM2M_DRV_USAGE_STATUS_NOT_USED;
        virtContext->state = DSSM2M_DRV_STATE_IDLE;
        m2mObj->numVirtContUsed[virtContext->contextId]--;

        if(NULL != virtContext->doneQ)
        {
            /* Free-up all the queued free queue objects */
            do
            {
                qObj = (DssM2M_DrvQueObj *) Fvid2Utils_dequeue(virtContext->doneQ);
            } while (NULL != qObj);

            /* Delete the free Q */
            Fvid2Utils_destructQ(virtContext->doneQ);
            virtContext->doneQ = NULL;
        }
        virtContext->pipeId    = CSL_DSS_VID_PIPE_ID_MAX;
        virtContext->overlayId = CSL_DSS_OVERLAY_ID_MAX;

        /* Delete Queues only on delete call for last opened Virtual context */
        if ((0U == m2mObj->numVirtContUsed[virtContext->contextId]) &&
                    (DSSM2M_DRV_USAGE_STATUS_IN_USE == instObj->inUse))
        {
            instObj->pipeId    = CSL_DSS_VID_PIPE_ID_MAX;
            instObj->overlayId = CSL_DSS_OVERLAY_ID_MAX;
            if(NULL != instObj->commonObjRef->lockSem)
            {
                /* Take the instance semaphore */
                (void) SemaphoreP_pend(instObj->commonObjRef->lockSem,
                                       SemaphoreP_WAIT_FOREVER);
            }

            retVal = Dss_m2mDrvDeleteInstQueues(virtContext);

            if(NULL != instObj->commonObjRef->lockSem)
            {
                /* Post the instance semaphore */
                (void) SemaphoreP_post(instObj->commonObjRef->lockSem);
            }
            instObj->inUse = DSSM2M_DRV_USAGE_STATUS_NOT_USED;
        }
    }

    return retVal;
}

int32_t Dss_m2mControl(Fdrv_Handle handle,
                       uint32_t cmd,
                       void *cmdArgs,
                       void *cmdStatusArgs)
{
    int32_t retVal = FVID2_SOK;
    DssM2MDrv_VirtContext *virtContext;
    Fvid2_TimeStampParams *tsParams;

    /* Check for NULL pointers */
    if (handle == NULL)
    {
        GT_0trace(DssTrace, GT_ERR, "Invalid argument!!\r\n");
        retVal = FVID2_EBADARGS;
    }
    else
    {
        virtContext = (DssM2MDrv_VirtContext *) handle;
        switch (cmd)
        {
            case FVID2_START:
            break;
            case FVID2_STOP:
            break;
            case IOCTL_DSS_M2M_SET_PIPE_PARAMS:
                retVal = Dss_m2mDrvIoctlSetWbPipeParams(virtContext,
                                    (const Dss_WbPipeCfgParams *)cmdArgs);
            break;
            case IOCTL_DSS_M2M_SET_PIPE_MFLAG_PARAMS:
                retVal = Dss_m2mDrvIoctlSetmFlagParams(virtContext,
                                    (const Dss_WbPipeMflagParams *)cmdArgs);
            break;
            case IOCTL_DSS_M2M_GET_CURRENT_STATUS:
                retVal = Dss_m2mDrvIoctlGetCurrentStatus(virtContext,
                                    (Dss_WbStatus *)cmdArgs);
            break;
            case IOCTL_DSS_M2M_SET_PIPE_CSC_COEFF:
                retVal = Dss_m2mDrvIoctlSetWbPipeCsc(virtContext,
                                    (const CSL_DssCscCoeff *)cmdArgs);
            break;
            case IOCTL_DSS_M2M_SET_PIPE_DMA_CFG:
                retVal = Dss_m2mDrvIoctlSetDmaCfg(virtContext,
                                    (const CSL_DssWbPipeDmaCfg *)cmdArgs);
            break;
            case FVID2_REGISTER_TIMESTAMP_FXN:
                tsParams = (Fvid2_TimeStampParams *)cmdArgs;
                gDssM2mCommonObj.getTimeStamp = tsParams->timeStampFxn;
            break;
            case IOCTL_DSS_DISP_SET_DSS_PARAMS:
                retVal = Dss_m2mDrvIoctlSetDssPipeParams(virtContext,
                                    (const Dss_DispParams *)cmdArgs);
            break;
            case IOCTL_DSS_DISP_SET_PIPE_MFLAG_PARAMS:
                retVal = Dss_m2mDrvIoctlSetPiprMflagParams(virtContext,
                                    (const Dss_DispPipeMflagParams *)cmdArgs);
            break;
            case IOCTL_DSS_DISP_SET_PIPE_CSC_COEFF:
                retVal = Dss_m2mDrvIoctlSetPipeCsc(virtContext,
                                    (const CSL_DssCscCoeff *)cmdArgs);
            break;
            case IOCTL_DSS_DCTRL_SET_PATH:
            break;
            case IOCTL_DSS_DCTRL_CLEAR_PATH:
            break;
            case IOCTL_DSS_DCTRL_SET_OVERLAY_PARAMS:
                retVal = Dss_m2mDrvIoctlSetOverlayParams(virtContext,
                                    (const Dss_DctrlOverlayParams *)cmdArgs);
            break;
            case IOCTL_DSS_DCTRL_SET_LAYER_PARAMS:
                retVal = Dss_m2mDrvIoctlSetLayerParams(virtContext,
                                (const Dss_DctrlOverlayLayerParams *)cmdArgs);
            break;
            case IOCTL_DSS_DCTRL_SET_GLOBAL_DSS_PARAMS:
                retVal = Dss_m2mDrvIoctlSetGlobalParams(virtContext,
                                (const Dss_DctrlGlobalDssParams *)cmdArgs);
            break;
            default:
            {
                GT_0trace(DssTrace, GT_ERR,
                            "UNSUPPORTED_CMD: IOCTL not supported\r\n");
                retVal = FVID2_EUNSUPPORTED_CMD;
            }
			break;
        }
    }

    return retVal;
}
/* ========================================================================== */
/*                       Static Function Definitions                          */
/* ========================================================================== */
static int32_t Dss_m2mDrvSetWbPipeDmaCfg(DssM2MDrv_InstObj *instObj,
                                         const CSL_DssWbPipeDmaCfg *dmaCfg)
{
    int32_t retVal = FVID2_SOK;

    if (NULL == instObj->wbRegs)
    {
        retVal = FVID2_EFAIL;
    }
    else
    {
        /* Configure HW module */
        CSL_dssWbPipeSetDmaConfig(instObj->wbRegs, dmaCfg);
        /* Update programmed configurations in the instance object */
        Fvid2Utils_memcpy(&instObj->progCfg.wbPipeCfg.dmaCfg,
                          dmaCfg,
                          sizeof(CSL_DssWbPipeDmaCfg));
    }

    return retVal;
}

int32_t Dss_m2mDrvCreateInstQueues(DssM2MDrv_VirtContext *context)
{
    int32_t retVal = FVID2_SOK;
    DssM2M_DrvBufManObj *bmObj;
    uint32_t qCnt = 0U;
    DssM2M_DrvQueObj *qObj;
    DssM2MDrv_InstObj *instObj;

    instObj = context->instObj;
    bmObj = &instObj->bmObj;

    bmObj->freeQ = NULL;
    bmObj->reqQ = NULL;
    bmObj->currQ = NULL;

    /* Create Queues */
    retVal = Fvid2Utils_constructQ(&bmObj->freeLlObj);
    GT_assert(DssTrace, (retVal == FVID2_SOK));
    bmObj->freeQ = &bmObj->freeLlObj;

    retVal = Fvid2Utils_constructQ(&bmObj->reqLlObj);
    GT_assert(DssTrace, (retVal == FVID2_SOK));
    bmObj->reqQ = &bmObj->reqLlObj;

    retVal = Fvid2Utils_constructQ(&bmObj->curLlObj);
    GT_assert(DssTrace, (retVal == FVID2_SOK));
    bmObj->currQ = &bmObj->curLlObj;

    if(FVID2_SOK == retVal)
    {
        for (qCnt = 0U ;
             qCnt < (DSS_DEF_QUEUE_LEN_PER_M2M_DRV_INST * DSSM2M_NUM_VIRTUAL_CONTEXT) ;
             qCnt++)
        {
            qObj              = &bmObj->m2mQObj[qCnt];
            qObj->virtContext = context;
            qObj->inFrm       = NULL;
            qObj->outFrm      = NULL;
            Fvid2Utils_queue(bmObj->freeQ, &qObj->qElem, qObj);
        }
    }

    return retVal;
}

int32_t Dss_m2mDrvDeleteInstQueues(DssM2MDrv_VirtContext *context)
{
    int32_t retVal = FVID2_SOK;
    DssM2M_DrvBufManObj *bmObj;
    DssM2M_DrvQueObj *qObj;
    DssM2MDrv_InstObj *instObj;

    instObj = context->instObj;
    bmObj = &instObj->bmObj;

    if(NULL != bmObj->freeQ)
    {
        /* Free-up all the queued free queue objects */
        do
        {
            qObj = (DssM2M_DrvQueObj *) Fvid2Utils_dequeue(bmObj->freeQ);
        } while (NULL != qObj);

        /* Delete the free Q */
        Fvid2Utils_destructQ(bmObj->freeQ);
        bmObj->freeQ = NULL;
    }

    if(NULL != bmObj->reqQ)
    {
        /* Free-up all the queued free queue objects */
        do
        {
            qObj = (DssM2M_DrvQueObj *) Fvid2Utils_dequeue(bmObj->reqQ);
        } while (NULL != qObj);

        /* Delete the free Q */
        Fvid2Utils_destructQ(bmObj->reqQ);
        bmObj->reqQ = NULL;
    }

    if(NULL != bmObj->currQ)
    {
        /* Free-up all the queued free queue objects */
        do
        {
            qObj = (DssM2M_DrvQueObj *) Fvid2Utils_dequeue(bmObj->currQ);
        } while (NULL != qObj);

        /* Delete the free Q */
        Fvid2Utils_destructQ(bmObj->currQ);
        bmObj->currQ = NULL;
    }

    return retVal;
}

static int32_t Dss_m2mDrvIoctlSetWbPipeParams(DssM2MDrv_VirtContext *context,
                                       const Dss_WbPipeCfgParams *pipeCfg)
{
    int32_t retVal = FVID2_SOK;

    /* check for parameters */
    if (NULL == pipeCfg)
    {
        retVal = FVID2_EBADARGS;
    }
    else
    {
        /* Only copy provided configurations to channel/virtual context object.
           This will be programmed to HW module when buffers are submitted
           through 'Fvid2_processRequest()' */
           Fvid2Utils_memcpy(&context->instCfg.wbPipeCfg.cfgParams,
                             pipeCfg,
                             sizeof(Dss_WbPipeCfgParams));
    }

    return retVal;
}

static int32_t Dss_m2mDrvIoctlSetmFlagParams(DssM2MDrv_VirtContext *context,
                                      const Dss_WbPipeMflagParams *mFlagParams)
{
    int32_t retVal = FVID2_SOK;

    /* check for parameters */
    if (NULL == mFlagParams)
    {
        retVal = FVID2_EBADARGS;
    }
    else
    {
        /* Only copy provided configurations to channel/virtual context object.
           This will be programmed to HW module when buffers are submitted
           through 'Fvid2_processRequest()' */
           Fvid2Utils_memcpy(&context->instCfg.wbPipeCfg.mFlagParams,
                             mFlagParams,
                             sizeof(Dss_WbPipeMflagParams));
    }

    return retVal;
}

static int32_t Dss_m2mDrvIoctlGetCurrentStatus(DssM2MDrv_VirtContext *context,
                                               Dss_WbStatus *status)
{
    int32_t retVal = FVID2_SOK;


    /* check for parameters */
    if (NULL == status)
    {
        retVal = FVID2_EBADARGS;
    }
    else
    {
        /* Only copy provided configurations to channel/virtual context object.
           This will be programmed to HW module when buffers are submitted
           through 'Fvid2_processRequest()' */
        Fvid2Utils_memcpy(status,
                          &context->wbStatus,
                          sizeof(Dss_WbStatus));
    }

    return retVal;
}

static int32_t Dss_m2mDrvIoctlSetWbPipeCsc(DssM2MDrv_VirtContext *context,
                                    const CSL_DssCscCoeff *csc)
{
    int32_t retVal = FVID2_SOK;

    /* check for parameters */
    if (NULL == csc)
    {
        retVal = FVID2_EBADARGS;
    }
    else
    {
        /* Only copy provided configurations to channel/virtual context object.
           This will be programmed to HW module when buffers are submitted
           through 'Fvid2_processRequest()' */
        Fvid2Utils_memcpy(&context->instCfg.wbPipeCfg.cscCoeff,
                          csc,
                          sizeof(CSL_DssCscCoeff));
    }

    return retVal;
}

static int32_t Dss_m2mDrvIoctlSetDmaCfg(DssM2MDrv_VirtContext *context,
                                        const CSL_DssWbPipeDmaCfg *dmaCfg)
{
    int32_t retVal = FVID2_SOK;

    /* check for parameters */
    if (NULL == dmaCfg)
    {
        retVal = FVID2_EBADARGS;
    }
    else
    {
        /* upgrade configurations into context object and re-program HW module
           on buffer submission */
        Fvid2Utils_memcpy(&context->instCfg.wbPipeCfg.dmaCfg,
                          dmaCfg,
                          sizeof(CSL_DssWbPipeDmaCfg));
    }

    return retVal;
}

