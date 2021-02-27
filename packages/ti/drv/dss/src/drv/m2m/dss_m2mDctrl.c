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
 *  \file dss_m2mDctrl.c
 *
 *  \brief File containing the DSS M2M driver DSS_DCTRL APIs.
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

/* None */

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */
int32_t Dss_m2mDrvIoctlSetOverlayParams(DssM2MDrv_VirtContext *context,
                                        const Dss_DctrlOverlayParams *ovlParams);

int32_t Dss_m2mDrvIoctlSetLayerParams(DssM2MDrv_VirtContext *context,
                        const Dss_DctrlOverlayLayerParams *ovlLayerParams);

int32_t Dss_m2mDrvIoctlSetGlobalParams(DssM2MDrv_VirtContext *context,
                               const Dss_DctrlGlobalDssParams *globalParams);
/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */
extern DssM2MDrv_CommonObj gDssM2mCommonObj;

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */
int32_t Dss_m2mDrvIoctlSetOverlayParams(DssM2MDrv_VirtContext *context,
                                        const Dss_DctrlOverlayParams *ovlParams)
{
    int32_t retVal = FVID2_SOK;

    /* check for parameters */
    if (NULL == ovlParams)
    {
        retVal = FVID2_EBADARGS;
    }
    else
    {
        /* upgrade configurations into context object and re-program HW module
           on buffer submission */
        Fvid2Utils_memcpy(&context->instCfg.dctrlCfg.ovlParams,
                          ovlParams,
                          sizeof(Dss_DctrlOverlayParams));
    }

    return retVal;
}

int32_t Dss_m2mDrvIoctlSetLayerParams(DssM2MDrv_VirtContext *context,
                            const Dss_DctrlOverlayLayerParams *ovlLayerParams)
{
    int32_t retVal = FVID2_SOK;

    /* check for parameters */
    if (NULL == ovlLayerParams)
    {
        retVal = FVID2_EBADARGS;
    }
    else
    {
        /* upgrade configurations into context object and re-program HW module
           on buffer submission */
        Fvid2Utils_memcpy(&context->instCfg.dctrlCfg.ovlLayerParams,
                          ovlLayerParams,
                          sizeof(Dss_DctrlOverlayLayerParams));
    }

    return retVal;
}

int32_t Dss_m2mDrvIoctlSetGlobalParams(DssM2MDrv_VirtContext *context,
                               const Dss_DctrlGlobalDssParams *globalParams)
{
    int32_t retVal = FVID2_SOK;

    /* check for parameters */
    if (NULL == globalParams)
    {
        retVal = FVID2_EBADARGS;
    }
    else
    {
        /* upgrade configurations into context object and re-program HW module
           on buffer submission */
        Fvid2Utils_memcpy(&context->instCfg.dctrlCfg.globalParams,
                          globalParams,
                          sizeof(Dss_DctrlGlobalDssParams));
    }

    return retVal;
}
