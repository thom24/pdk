/*
 *  Copyright (c) Texas Instruments Incorporated 2021
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
 *  \ingroup  DRV_DSS_MODULE
 *  \defgroup DRV_DSS_M2M_MODULE Display Write-back Driver
 *            This is DSS Display Write-back driver documentation
 *
 *  @{
 */

/**
 *  \file dss_m2m.h
 *
 *  \brief DSS Display Write-back Driver interface file.
 */

#ifndef DSS_M2M_H_
#define DSS_M2M_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

/* None */

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */
/*
 *  Display M2M driver IOCTLs.
 */

/**
 *  \ingroup  DRV_DSS_M2M_MODULE
 *  \defgroup DRV_DSS_M2M_IOCTL Display IOCTLs
 *  @{
 *
 * These are display FVID2 APIs that can be called by the application.
 * Display M2M driver supports additional IOCTLs from other modules/drivers
 * and those are follows:
 * 1. IOCTLs from Display Pipe Driver (Refer to \ref DRV_DSS_DISP_IOCTL):
 *      - IOCTL_DSS_DISP_SET_DSS_PARAMS
 *      - IOCTL_DSS_DISP_SET_PIPE_MFLAG_PARAMS
 *      - IOCTL_DSS_DISP_SET_PIPE_CSC_COEFF
 * 2. IOCTLs from Display Controller Driver (Refer to \ref DRV_DSS_DCTRL_IOCTL):
 *      - IOCTL_DSS_DCTRL_SET_OVERLAY_PARAMS
 *      - IOCTL_DSS_DCTRL_SET_LAYER_PARAMS
 *      - IOCTL_DSS_DCTRL_SET_GLOBAL_DSS_PARAMS
 */

/**
 * \brief Command to set DSS display M2M pipeline parameters.
 *
 *  This IOCTL is used to program the DSS display parameters. Refer to structure
 *  #Dss_WbPipeCfgParams for details on DSS parameters.
 *
 * \param cmdArgs       [IN]  Pointer of type #Dss_WbPipeCfgParams
 * \param cmdArgsStatus [OUT] NULL
 *
 * \return  FVID2_SOK if successful, else suitable error code
 */
#define IOCTL_DSS_M2M_SET_PIPE_PARAMS          (DSS_M2M_IOCTL_BASE + 0x0001U)

/**
 * \brief Command to set MFLAG parameters for video pipe.
 *
 *  This IOCTL can be used to set the MFLAG parameters for the given video pipe.
 *
 * \param cmdArgs       [IN]  Pointer of type #Dss_WbPipeMflagParams
 * \param cmdArgsStatus [OUT] NULL
 *
 * \return  FVID2_SOK if successful, else suitable error code
 *
 */
#define IOCTL_DSS_M2M_SET_PIPE_MFLAG_PARAMS   (DSS_M2M_IOCTL_BASE + 0x0002U)

/**
 * \brief Command to get the display status.
 *
 *  This IOCTL can be used to get the display status like number of frames
 *  displayed, repeated, queued. dequeued, etc.
 *  Note: These counters will be reset either at the time of driver create or
 *  while starting the display operation. See respective counter comments for
 *  details.
 *
 * \param cmdArgs       [IN]  Pointer of type #Dss_WbStatus
 * \param cmdArgsStatus [OUT] NULL
 *
 * \return  FVID2_SOK if successful, else suitable error code
 *
 */
#define IOCTL_DSS_M2M_GET_CURRENT_STATUS      (DSS_M2M_IOCTL_BASE + 0x0003U)

/**
 * \brief Command to program the CSC coefficients for Video Pipe.
 *  By default BT 601 coefficients are set.
 *
 * \param cmdArgs       [IN]  Pointer of type #CSL_DssCscCoeff
 * \param cmdArgsStatus [OUT] NULL
 *
 * \return  FVID2_SOK if successful, else suitable error code
 */
#define IOCTL_DSS_M2M_SET_PIPE_CSC_COEFF      (DSS_M2M_IOCTL_BASE + 0x0004U)

/**
 * \brief Command to program the write-back pipe DMA configuration parameters
 *
 * \param cmdArgs       [IN]  Pointer of type #CSL_DssWbPipeDmaCfg
 * \param cmdArgsStatus [OUT] NULL
 *
 * \return  FVID2_SOK if successful, else suitable error code
 */
#define IOCTL_DSS_M2M_SET_PIPE_DMA_CFG       (DSS_M2M_IOCTL_BASE + 0x0005U)

/* @} */

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */
/**
 *  \brief Structure containing video write-back pipe configuration.
 */
typedef struct
{
    CSL_DssWbPipeCfg pipeCfg;
    /**< Configuration for WB pipeline
     *   See \ref CSL_DssWbPipeCfg for details
     *   Note: Only 'CSL_DSS_WB_PIPE_MODE_M2M' mode
     *         supported for "pipeCfg.wbMode" */
} Dss_WbPipeCfgParams;

/**
 *  \brief Structure containing Video write-back Pipe Mflag Information.
 *   This structure is used as an argument to
 *   IOCTL_DSS_M2M_SET_PIPE_MFLAG_PARAMS.
 */
typedef struct
{
    CSL_DssWbPipeMFlagCfg mFlagCfg;
    /**< MFlag configuration parameters for WB pipeline
     *   See \ref CSL_DssWbPipeMFlagCfg for details */
} Dss_WbPipeMflagParams;

/**
 *  \brief Structure containing current status of the display M2M driver.
 *   This structure is used as an argument to IOCTL_DSS_M2M_GET_CURRENT_STATUS.
 */
typedef struct
{
    uint32_t queueCount;
    /**< Counter to keep track of how many requests are queued to the driver.
     *   Note: This counter will be reset at the time of driver create */
    uint32_t dequeueCount;
    /**< Counter to keep track of how many requests are dequeued from the
     *   driver.
     *   Note: This counter will be reset at the time of driver create */
    uint32_t wbFrmCount;
    /**< Counter to keep track of how many frames are wrote back to memory.
     *   Note: This counter will be reset at the time of display start */
    uint32_t underflowCount;
    /**< Counter to keep track of number of underflows
     *   Note: This counter will be reset at the time of driver create */
} Dss_WbStatus;

/**
 *  \brief Structure containing display driver create arguments, used when
 *   calling Fvid2_create().
 */
typedef struct
{
    uint32_t instId;
    /**< DSS M2M/WB pipeline instance ID */
    CSL_DssWbPipeDmaCfg dmaCfg;
    /**< DMA configuration for WB pipeline
     *   See \ref CSL_DssWbPipeDmaCfg for details */
    uint32_t pipeId;
    /**< ID DSS pipeline to use.
     *   See \ref CSL_DssVidPipeId for details */
    uint32_t overlayId;
    /**< ID DSS overlay to use.
     *   See \ref CSL_DssOverlayId for details */
} Dss_WbCreateParams;

/**
 *  \brief DSS M2M driver create status. Returned after calling Fvid2_create().
 */
typedef struct
{
    int32_t retVal;
    /**< Create status, FVID2_SOK on success, else failure. */
} Dss_WbCreateStatus;
/* ========================================================================== */
/*                  Internal/Private Function Declarations                    */
/* ========================================================================== */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */
/**
 *  \brief Dss_WbCreateParams structure init function.
 *
 *  \param  createParams  [IN]Pointer to #Dss_WbCreateParams structure.
 *
 *  \return None
 */
static inline void Dss_m2mCreateParamsInit(Dss_WbCreateParams *createParams);

/**
 *  \brief Dss_WbPipeCfgParams structure init function.
 *
 *  \param  cfgParams     [IN]Pointer to #Dss_WbPipeCfgParams structure.
 *
 *  \return None
 */
static inline void Dss_m2mPipeCfgParamsInit(Dss_WbPipeCfgParams *cfgParams);

/**
 *  \brief Dss_WbPipeMflagParams structure init function.
 *
 *  \param  mFlagParams   [IN]Pointer to #Dss_WbPipeMflagParams structure.
 *
 *  \return None
 */
static inline void Dss_m2mMFlagParamsInit(Dss_WbPipeMflagParams *mFlagParams);

/**
 *  \brief Dss_WbStatus structure init function.
 *
 *  \param  status       [IN]Pointer to #Dss_WbStatus structure.
 *
 *  \return None
 */
static inline void Dss_m2mStatusInit(Dss_WbStatus *status);
/* ========================================================================== */
/*                       Static Function Definitions                          */
/* ========================================================================== */
static inline void Dss_m2mCreateParamsInit(Dss_WbCreateParams *createParams)
{
    if(NULL != createParams)
    {
        createParams->instId    = DSS_M2M_WB_PIPELINE_ID_0;
        createParams->pipeId    = CSL_DSS_VID_PIPE_ID_VID1;
        createParams->overlayId = CSL_DSS_OVERLAY_ID_1;
        CSL_dssWbPipeDmaCfgInit(&(createParams->dmaCfg));
    }
}

static inline void Dss_m2mPipeCfgParamsInit(Dss_WbPipeCfgParams *cfgParams)
{
    if(NULL != cfgParams)
    {
        CSL_dssWbPipeCfgInit(&(cfgParams->pipeCfg));
        /* Set mode to M2M mode */
        cfgParams->pipeCfg.wbMode = CSL_DSS_WB_PIPE_MODE_M2M;
    }
}

static inline void Dss_m2mMFlagParamsInit(Dss_WbPipeMflagParams *mFlagParams)
{
    if(NULL != mFlagParams)
    {
        CSL_dssWbPipeMFlagCfgInit(&(mFlagParams->mFlagCfg));
    }
}

static inline void Dss_m2mStatusInit(Dss_WbStatus *status)
{
    if(NULL != status)
    {
        status->queueCount     = 0U;
        status->dequeueCount   = 0U;
        status->wbFrmCount     = 0U;
        status->underflowCount = 0U;
    }
}


#ifdef __cplusplus
}
#endif

#endif /* #ifndef DSS_M2M_H_ */

/* @} */
