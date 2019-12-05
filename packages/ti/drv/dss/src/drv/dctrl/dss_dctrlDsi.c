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
 *  \file dss_dctrlApi.c
 *
 *  \brief File containing the DSS Controller driver implementation.
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <ti/drv/dss/dss.h>
#include <ti/drv/dss/src/drv/common/dss_evtMgr.h>
#include <ti/drv/dss/src/drv/dctrl/dss_dctrlDrv.h>
#include <ti/drv/dss/src/drv/dctrl/dss_dctrlIntr.h>
#include <ti/drv/dss/src/drv/dctrl/dss_dctrlPriv.h>
#include <ti/drv/dss/src/csl/dsi/csl_dsi.h>
#include <ti/drv/dss/src/csl/dphy/csl_dphy.h>
#include <dss_soc_priv.h>


/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* TODO: Only RGB24 input and output supported as of now,
   multipled by 3 for RGB24 */
#define DSITX_VID_DATA_TYPE                 (DSITX_VID_DATA_TYPE_RGB_24)
#define DSITX_VID_PIX_MODE                  (DSITX_VID_PIXEL_MODE_RGB_24)
#define BPP                                 (3u)

#define PLL_LOCK_REPEAT_COUNT               (10000u)
#define WAIT_FOR_LANES_ACTIVE_REPEAT_COUNT  (10000u)

/* Base Address of PHY */
#define DPHYTX0_CORE_BASE                   (CSL_DPHY_TX0_BASE)
/* Base Address of DSI Wrapper */
#define DSITX2_WRAP_REGS_BASE               (CSL_DSS_DSI0_DSI_WRAP_MMR_VBUSP_CFG_DSI_WRAP_BASE)


/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

typedef struct
{
    DSITX_Config                cfgDsiTx;
    DSITX_SysReq                sysReqDsiTx;
    DSITX_PhyConfig             phyCfg;
    DSITX_DphyConfig            dphyCfg;
    DSITX_DsiLinkConfig         dsiLinkCfg;
    DSITX_DphyPwrRstConfig      dphyRstCfg;
    DSITX_DataPathConfig        datapathCfg;
    DSITX_PrivateData           privDsiTx;

    DSITX_VideoSize             videoSizeCfg;
    DSITX_VideoModeSettings     videoModeCfg;

    uint32_t                    dphyTxIpDiv;
    uint32_t                    dphyTxOpDiv;
    uint32_t                    dphyTxFbDiv;
    uint32_t                    dphyTxRate;

    CSL_wiz16b8m4cdtRegs_DPHYTX     *dphyRegs;
    CSL_wiz16b8m4cdtRegs_WIZ_CONFIG *dphyWizRegs;
    DSITX_Regs                 *topRegs;

} Dss_DctrlDSIDrvObj;



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

static Dss_DctrlDSIDrvObj gDssDctrlDsiDrvObj;


/* ========================================================================== */
/*                  Internal/Private Function Declarations                    */
/* ========================================================================== */

static void dssDctrlSetDSIInCtrlMod();
static uint32_t dssDctrlInitPhyConfig(Dss_DctrlDSIDrvObj *drvObj,
    DSITX_PrivateData* pD, DSITX_Config* cfg);
static uint32_t dssDctrlSetDphyConfiguration(Dss_DctrlDSIDrvObj *dsiObj,
    DSITX_PrivateData *pD);
static int32_t dssDctrlSetClockConfig(Dss_DctrlDSIDrvObj *dsiObj);
static int32_t dssDctrlWaitForLock(Dss_DctrlDSIDrvObj *dsiObj);
static int32_t dssDctrlSetDphyPowerAndReset(Dss_DctrlDSIDrvObj *dsiObj);
static int32_t dssDctrlInitDsiLinkConfig(Dss_DctrlDSIDrvObj *dsiObj);
static int32_t dssDctrlSetVideoConfig(Dss_DctrlDSIDrvObj *dsiObj,
    const Fvid2_ModeInfo *mInfo);
static int32_t dssDctrlGetVideoModeAndSizeConfig(Dss_DctrlDSIDrvObj *dsiObj);
static int32_t dssDctrlSetVideoModeAndSizeConfig(Dss_DctrlDSIDrvObj *dsiObj);
static void dssDctrlUpdateVideoModeConfig(Dss_DctrlDSIDrvObj *dsiObj,
    const Fvid2_ModeInfo *mInfo);
static void dssDctrlUpdateVideoSizeConfig(Dss_DctrlDSIDrvObj *dsiObj,
    const Fvid2_ModeInfo *mInfo);
static int32_t dssDctrlSetVideoModeAndSizeConfig(Dss_DctrlDSIDrvObj *dsiObj);
static int32_t dssDctrlEnableDsiLinkAndPath(Dss_DctrlDSIDrvObj *dsiObj);
static int32_t dssDctrlEnableDsiLink(Dss_DctrlDSIDrvObj *dsiObj);
static int32_t dssDctrlEnableDsiDatapath(Dss_DctrlDSIDrvObj *dsiObj);
static int32_t dssDctrlWaitForLaneReady(Dss_DctrlDSIDrvObj *dsiObj);


/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

void Dss_dctrlDrvInitDSI()
{
    Dss_DctrlDSIDrvObj *dsiObj;

    dsiObj = &gDssDctrlDsiDrvObj;

    Fvid2Utils_memset(&dsiObj->cfgDsiTx, 0, sizeof(DSITX_Config));
    Fvid2Utils_memset(&dsiObj->sysReqDsiTx, 0, sizeof(DSITX_SysReq));
    Fvid2Utils_memset(&dsiObj->privDsiTx, 0, sizeof(DSITX_PrivateData));

    dsiObj->cfgDsiTx.regBase = (DSITX_Regs*) CSL_DSS_DSI0_DSI_TOP_VBUSP_CFG_DSI_0_DSI_BASE;
    dsiObj->privDsiTx.regBase = (DSITX_Regs*) CSL_DSS_DSI0_DSI_TOP_VBUSP_CFG_DSI_0_DSI_BASE;
    dsiObj->topRegs = (DSITX_Regs*) CSL_DSS_DSI0_DSI_TOP_VBUSP_CFG_DSI_0_DSI_BASE;
    dsiObj->dphyRegs = (CSL_wiz16b8m4cdtRegs_DPHYTX *)DPHYTX0_CORE_BASE;
    dsiObj->dphyWizRegs = (CSL_wiz16b8m4cdtRegs_WIZ_CONFIG *)
        (DPHYTX0_CORE_BASE + CSL_WIZ16B8M4CDT_WIZ_CONFIG_MOD_VER);


    dsiObj->dphyTxIpDiv = 0x2;
    dsiObj->dphyTxOpDiv = 0x2;
    dsiObj->dphyTxFbDiv = 0x173;
    dsiObj->dphyTxRate = 0x1CE;
    dsiObj->cfgDsiTx.numOfLanes = 0x2u;
    dsiObj->privDsiTx.numOfLanes = 0x2u;
}

int32_t Dss_dctrlDrvSetDSIParams(Dss_DctrlDrvInfo *drvInfo,
    const Dss_DctrlDsiParams *dsiPrms)
{
    int32_t status;
    Dss_DctrlDSIDrvObj *dsiObj;

    dsiObj = &gDssDctrlDsiDrvObj;

    dsiObj->cfgDsiTx.numOfLanes = dsiPrms->numOfLanes;
    dsiObj->privDsiTx.numOfLanes = dsiPrms->numOfLanes;

    /* Checks to see if the configuration (num of lanes) is valid */
    status = DSITX_Probe(&dsiObj->cfgDsiTx, &dsiObj->sysReqDsiTx);
    if (CDN_EOK == status)
    {
        /* Calculate lane parameters based on the input speed */
        status = FVID2_SOK;
    }
    else
    {
        status = FVID2_EINVALID_PARAMS;
    }

    return (status);
}

int32_t Dss_dctrlDrvEnableVideoDSI(Dss_DctrlDrvInfo *drvInfo,
    const Fvid2_ModeInfo *mInfo, uint32_t hsyncPolarity, uint32_t vsyncPolarity)
{
    int32_t status;
    Dss_DctrlDSIDrvObj *dsiObj;

    dsiObj = &gDssDctrlDsiDrvObj;

    dssDctrlSetDSIInCtrlMod();

    /* Disables/clears interrupts, sets default status */
    status = DSITX_Init(&dsiObj->privDsiTx, &dsiObj->cfgDsiTx);

    if (CDN_EOK == status)
    {
        /* Initialize PHY configuration, number of lanes, continuous clock etc */
        status = dssDctrlInitPhyConfig(dsiObj,
            &dsiObj->privDsiTx, &dsiObj->cfgDsiTx);
    }

    if (CDN_EOK == status)
    {
        status = dssDctrlSetDphyConfiguration(dsiObj, &dsiObj->privDsiTx);
    }

    if (CDN_EOK == status)
    {
        status = dssDctrlSetClockConfig(dsiObj);
    }

    if (CDN_EOK == status)
    {
        status = dssDctrlWaitForLock(dsiObj);
    }

    if (CDN_EOK == status)
    {
        status = dssDctrlSetDphyPowerAndReset(dsiObj);
    }

    if (CDN_EOK == status)
    {
        status = dssDctrlInitDsiLinkConfig(dsiObj);
    }

    if (CDN_EOK == status)
    {
        DSITX_Start(&dsiObj->privDsiTx);
    }

    if (CDN_EOK == status)
    {
        status = dssDctrlSetVideoConfig(dsiObj, mInfo);
    }

    if (CDN_EOK == status)
    {
        status = dssDctrlEnableDsiLinkAndPath(dsiObj);
    }

    if (CDN_EOK == status)
    {
        status = dssDctrlWaitForLaneReady(dsiObj);
    }

    if (CDN_EOK == status)
    {
        status = FVID2_SOK;
    }
    else
    {
        status = FVID2_EINVALID_PARAMS;
    }
    return (status);
}


/* ========================================================================== */
/*                  Internal/Private Function Definitions                     */
/* ========================================================================== */

static void dssDctrlSetDSIInCtrlMod()
{
    /*
     * XXX push this things to some CSL kind of functions
     */

    /* Enable DPI0, no register overlays for Wrapper */
    CSL_REG32_WR(CSL_DSS_DSI0_DSI_WRAP_MMR_VBUSP_CFG_DSI_WRAP_BASE +
        CSL_DSI_WRAP_DPI_CONTROL, 1);

    /* Assumes the MMRs are unlocked for the 1st partition */

    /* Select DSITX as the source for DPHYTX */
    CSL_REG32_WR(CSL_CTRL_MMR0_CFG0_BASE +
        CSL_MAIN_CTRL_MMR_CFG0_DPHY_TX0_CTRL, 0x0);
}

static uint32_t dssDctrlInitPhyConfig(Dss_DctrlDSIDrvObj *drvObj,
    DSITX_PrivateData* pD, DSITX_Config* cfg)
{
    uint32_t status = CDN_EOK;

    status = DSITX_GetPhyConfig(pD, &drvObj->phyCfg);
    if (CDN_EOK == status)
    {
        drvObj->phyCfg.lane2Enabled =  (((cfg->numOfLanes) >  (1U)) ? (bool)(1U) : (bool)(0U));
        drvObj->phyCfg.lane3Enabled =  (((cfg->numOfLanes) >  (2U)) ? (bool)(1U) : (bool)(0U));
        drvObj->phyCfg.lane4Enabled =  (((cfg->numOfLanes) >  (3U)) ? (bool)(1U) : (bool)(0U));
        drvObj->phyCfg.laneClkContinous = TRUE;
        drvObj->phyCfg.waitBurstTime = 15; /* TODO: How to calculate waitBurstTime */
        status = DSITX_SetPhyConfig(pD, &drvObj->phyCfg);
    }
    return (status);
}

static uint32_t dssDctrlSetDphyConfiguration(Dss_DctrlDSIDrvObj *dsiObj,
    DSITX_PrivateData *pD)
{
    int32_t status;

    status = DSITX_GetDphyConfig(pD, &dsiObj->dphyCfg);
    if (CDN_EOK == status)
    {
        /* TODO: How to calculate these parameters */
        dsiObj->dphyCfg.clkDivisionRatio = 0xB;
        dsiObj->dphyCfg.hstxTimeout = 0xAFFF;
        dsiObj->dphyCfg.lprxTimeout = 0x3FFFF;
        dsiObj->dphyCfg.clkLaneUlpTimeout = 0x105;
        dsiObj->dphyCfg.dataLaneUlpTimeout = 0x1d5;

        status = DSITX_SetDphyConfig(pD, &dsiObj->dphyCfg);
    }

    return status;
}

static int32_t dssDctrlSetClockConfig(Dss_DctrlDSIDrvObj *dsiObj)
{
    /* Set DPHY Wrapper PSM clock */
    /*
    ** The signal must be driven with a value such that the internal psm frequency of the divided psm clock is 1 MHz.PSM clock is 20MHz, so dividing by 20 which is ** 14 in hex
    */
    CSL_FINSR(*(volatile uint32_t *)(
        DPHYTX0_CORE_BASE + CSL_WIZ16B8M4CDT_WIZ_CONFIG_PSM_FREQ), 7, 0, 0x14);

    /* PWM control divider */
    CSL_FINSR(*(volatile uint32_t *)(
        DPHYTX0_CORE_BASE + CSL_WIZ16B8M4CDT_DPHYTX_CMN0_CMN_DIG_TBIT10),
        9, 0, 0x00E /*0xE4*/);
    CSL_FINSR(*(volatile uint32_t *)(
        DPHYTX0_CORE_BASE + CSL_WIZ16B8M4CDT_DPHYTX_CMN0_CMN_DIG_TBIT10),
        19, 10, 0x1FF /*0xC*/);

    /* pllcnt start value */
    CSL_FINSR(*(volatile uint32_t *)(
        DPHYTX0_CORE_BASE + CSL_WIZ16B8M4CDT_DPHYTX_CMN0_CMN_DIG_TBIT7),
        15, 0, 0xC8);

    /* pllcnt lock threshold value */
    CSL_FINSR(*(volatile uint32_t *)(
        DPHYTX0_CORE_BASE + CSL_WIZ16B8M4CDT_DPHYTX_CMN0_CMN_DIG_TBIT7),
        31, 16, 0x4);

    /* Enable CMN startup state machine */
    CSL_FINSR(*(volatile uint32_t *)(
        DPHYTX0_CORE_BASE + CSL_WIZ16B8M4CDT_DPHYTX_CMN0_CMN_DIG_TBIT2),
        0, 0, 0x1);

    /* TX mode enable */
    CSL_FINSR(*(volatile uint32_t *)(
        DPHYTX0_CORE_BASE + CSL_WIZ16B8M4CDT_DPHYTX_CMN0_CMN_DIG_TBIT2),
        9, 9, 0x1);

    /* Lane Reset */
    CSL_FINSR(*(volatile uint32_t *)(
        DPHYTX0_CORE_BASE + CSL_WIZ16B8M4CDT_WIZ_CONFIG_RST_CTRL),
        31, 31, 0x1);

    /* PLL Programming */
    CSL_FINSR(*(volatile uint32_t *)(
        DPHYTX0_CORE_BASE + CSL_WIZ16B8M4CDT_WIZ_CONFIG_PLL_CTRL),
        4, 0, dsiObj->dphyTxIpDiv);
    CSL_FINSR(*(volatile uint32_t *)(
        DPHYTX0_CORE_BASE + CSL_WIZ16B8M4CDT_WIZ_CONFIG_PLL_CTRL),
        13, 8, dsiObj->dphyTxOpDiv);
    CSL_FINSR(*(volatile uint32_t *)(
        DPHYTX0_CORE_BASE + CSL_WIZ16B8M4CDT_WIZ_CONFIG_PLL_CTRL),
        25, 16, dsiObj->dphyTxFbDiv);

    CSL_FINSR(*(volatile uint32_t *)(
        DPHYTX0_CORE_BASE + CSL_WIZ16B8M4CDT_WIZ_CONFIG_PLL_CTRL),
        28, 28, 0x0);

    /* PHY Band ctrl - Data Rate */
    CSL_FINSR(*(volatile uint32_t *)(
        DPHYTX0_CORE_BASE + CSL_WIZ16B8M4CDT_DPHYTX_PCS_TX_DIG_TBIT0),
        4, 0, dsiObj->dphyTxRate);
    CSL_FINSR(*(volatile uint32_t *)(
        DPHYTX0_CORE_BASE + CSL_WIZ16B8M4CDT_DPHYTX_PCS_TX_DIG_TBIT0),
        9, 5, dsiObj->dphyTxRate);

    return (CDN_EOK);
}

static int32_t dssDctrlWaitForLock(Dss_DctrlDSIDrvObj *dsiObj)
{
    uint32_t    value;
    uint32_t    timeout;

    /* First, wait for PLL to get locked */
    timeout = 0;
    do
    {
        value = *(volatile uint32_t *)(
            DPHYTX0_CORE_BASE + CSL_WIZ16B8M4CDT_WIZ_CONFIG_PLL_CTRL) &
            0x80000000;

        if (timeout >= PLL_LOCK_REPEAT_COUNT)
        {
            break;
        }
        else
        {
            timeout ++;
        }
    } while(value != 0x80000000);

    /* Wait for CMN to be ready */
    timeout = 0;
    do
    {
        value = *(volatile uint32_t *)(
            DPHYTX0_CORE_BASE + CSL_WIZ16B8M4CDT_WIZ_CONFIG_STATUS) &
            0x80000000;
        if (timeout >= PLL_LOCK_REPEAT_COUNT)
        {
            break;
        }
        else
        {
            timeout ++;
        }
    } while(value != 0x80000000);

    return (CDN_EOK);
}

static int32_t dssDctrlSetDphyPowerAndReset(Dss_DctrlDSIDrvObj *dsiObj)
{
    int32_t status, cnt;

    /* DSI - D-PHY release c_rstb and d_rstb */
    Fvid2Utils_memset(&dsiObj->dphyRstCfg, 0, sizeof(DSITX_DphyPwrRstConfig));

    dsiObj->dphyRstCfg.dphyCRstb = 0x1;

    for (cnt = 0u; cnt < dsiObj->cfgDsiTx.numOfLanes; cnt ++)
    {
        dsiObj->cfgDsiTx.numOfLanes |= (1 << cnt);
    }

    status = DSITX_SetDphyPwrAndRstCtrl(
        &dsiObj->privDsiTx, &dsiObj->dphyRstCfg);

    return (status);
}

static int32_t dssDctrlInitDsiLinkConfig(Dss_DctrlDSIDrvObj *dsiObj)
{
    uint32_t cnt;
    uint32_t status = CDN_EOK;

    status = DSITX_GetDsiLinkConfig(&dsiObj->privDsiTx, &dsiObj->dsiLinkCfg);
    if (CDN_EOK == status)
    {
        dsiObj->dsiLinkCfg.pllEnabled = TRUE;
        dsiObj->dsiLinkCfg.clkLaneEnabled = TRUE;

        for (cnt = 0U; cnt < (dsiObj->privDsiTx.numOfLanes); cnt ++)
        {
            dsiObj->dsiLinkCfg.datLaneEnabled[cnt] = TRUE;
        }

        status = DSITX_SetDsiLinkConfig(&dsiObj->privDsiTx,
            &dsiObj->dsiLinkCfg);
    }

    return (status);
}

static int32_t dssDctrlSetVideoConfig(Dss_DctrlDSIDrvObj *dsiObj,
    const Fvid2_ModeInfo *mInfo)
{
    int32_t status;

    status = dssDctrlGetVideoModeAndSizeConfig(dsiObj);

    if (CDN_EOK == status)
    {
        dssDctrlUpdateVideoModeConfig(dsiObj, mInfo);
        dssDctrlUpdateVideoSizeConfig(dsiObj, mInfo);
    }

    if (CDN_EOK == status)
    {
        status = dssDctrlSetVideoModeAndSizeConfig(dsiObj);
    }

    return (status);
}

static int32_t dssDctrlGetVideoModeAndSizeConfig(Dss_DctrlDSIDrvObj *dsiObj)
{
    int32_t status;

    status = DSITX_GetVideoSize(&dsiObj->privDsiTx, &dsiObj->videoSizeCfg);

    if (CDN_EOK == status)
    {
        status = DSITX_GetVideoMode(&dsiObj->privDsiTx, &dsiObj->videoModeCfg);
    }

    return (status);
}

static int32_t dssDctrlSetVideoModeAndSizeConfig(Dss_DctrlDSIDrvObj *dsiObj)
{
    int32_t status;

    status = DSITX_SetVideoSize(&dsiObj->privDsiTx, &dsiObj->videoSizeCfg);

    if (CDN_EOK == status)
    {
        status = DSITX_SetVideoMode(&dsiObj->privDsiTx, &dsiObj->videoModeCfg);
    }

    return (status);
}

static void dssDctrlUpdateVideoModeConfig(Dss_DctrlDSIDrvObj *dsiObj,
    const Fvid2_ModeInfo *mInfo)
{
    uint32_t horzTotal;

    /* TODO: Only RGB24 input and output supported as of now */
    dsiObj->videoModeCfg.header = DSITX_VID_DATA_TYPE;
    dsiObj->videoModeCfg.vidPixelMode = DSITX_VID_PIX_MODE;
    dsiObj->videoModeCfg.syncPulseActive = TRUE;
    dsiObj->videoModeCfg.syncPulseHorizontal = TRUE;
    dsiObj->videoModeCfg.blkLineMode = DSITX_VID_BLK_MODE_BLANKING_PACKET;
    dsiObj->videoModeCfg.blkEolMode = DSITX_VID_BLK_MODE_BLANKING_PACKET;
    /* TODO: How to calculate this params */
    dsiObj->videoModeCfg.regWakeupTime = 0x1A8u; // any non-zero value, value from other tests
    dsiObj->videoModeCfg.ignoreMissVsync = TRUE;

    horzTotal = mInfo->width + mInfo->hFrontPorch + mInfo->hBackPorch + mInfo->hSyncLen;

    dsiObj->videoModeCfg.regLineDuration =
        ((horzTotal * BPP) / dsiObj->cfgDsiTx.numOfLanes) -
        ((mInfo->hSyncLen * BPP) / dsiObj->cfgDsiTx.numOfLanes) -
        (4u / dsiObj->cfgDsiTx.numOfLanes);
}

static void dssDctrlUpdateVideoSizeConfig(Dss_DctrlDSIDrvObj *dsiObj,
    const Fvid2_ModeInfo *mInfo)
{
    uint32_t horzTotal;

    horzTotal = mInfo->width + mInfo->hFrontPorch + mInfo->hBackPorch + mInfo->hSyncLen;

    dsiObj->videoSizeCfg.vact = mInfo->height;
    dsiObj->videoSizeCfg.vfp = 1;
    dsiObj->videoSizeCfg.vbp = mInfo->vBackPorch;
    dsiObj->videoSizeCfg.vsa = mInfo->vSyncLen;
    dsiObj->videoSizeCfg.hsa = (mInfo->hSyncLen * BPP) - 14;
    dsiObj->videoSizeCfg.hbp = (mInfo->hBackPorch * BPP) - 12;
    dsiObj->videoSizeCfg.rgb = mInfo->width * BPP;
    dsiObj->videoSizeCfg.hfp = (mInfo->hFrontPorch * BPP) - 6;
    dsiObj->videoSizeCfg.blkLinePulsePacket = (horzTotal * BPP) - 20 - dsiObj->videoSizeCfg.hsa;
}

static int32_t dssDctrlEnableDsiLinkAndPath(Dss_DctrlDSIDrvObj *dsiObj)
{
    int32_t status;

    status = dssDctrlEnableDsiLink(dsiObj);
    if (CDN_EOK == status)
    {
        status = dssDctrlEnableDsiDatapath(dsiObj);
    }

    return (status);
}


static int32_t dssDctrlEnableDsiLink(Dss_DctrlDSIDrvObj *dsiObj)
{
    int32_t status;

    status = DSITX_GetDsiLinkConfig(&dsiObj->privDsiTx, &dsiObj->dsiLinkCfg);
    if (CDN_EOK == status)
    {
        dsiObj->dsiLinkCfg.if1Enabled = FALSE;
        dsiObj->dsiLinkCfg.if2Enabled = TRUE;
        dsiObj->dsiLinkCfg.if3Enabled = FALSE;
        status = DSITX_SetDsiLinkConfig(&dsiObj->privDsiTx, &dsiObj->dsiLinkCfg);
    }

    return (status);
}

static int32_t dssDctrlEnableDsiDatapath(Dss_DctrlDSIDrvObj *dsiObj)
{
    int32_t status;

    status = DSITX_GetDataPathConfig(&dsiObj->privDsiTx, &dsiObj->datapathCfg);
    if (CDN_EOK == status)
    {
        dsiObj->datapathCfg.linkEnabled = 1;
        dsiObj->datapathCfg.videoIfSelect = DSITX_VID_IF_SELECT_IF2;
        dsiObj->datapathCfg.videoStreamGenEnabled = true;
        status = DSITX_SetDataPathConfig(&dsiObj->privDsiTx, &dsiObj->datapathCfg);
    }

    if (CDN_EOK == status)
    {
        status = DSITX_WaitForPllLock(&dsiObj->privDsiTx, PLL_LOCK_REPEAT_COUNT);
    }

    if (CDN_EOK == status)
    {
        status = DSITX_CheckLanesState(&dsiObj->privDsiTx,
            WAIT_FOR_LANES_ACTIVE_REPEAT_COUNT);
    }

    return (status);
}

static int32_t dssDctrlWaitForLaneReady(Dss_DctrlDSIDrvObj *dsiObj)
{
    int32_t status = CDN_EOK, done = FALSE;
    uint32_t timeout, laneCnt;
    uint32_t numOfLanes = dsiObj->cfgDsiTx.numOfLanes;
    uint32_t laneStatus, readData;
    uint32_t reg[] = {
        DPHYTX0_CORE_BASE + CSL_WIZ16B8M4CDT_DPHYTX_ISO_PHY_ISO_CL_CNTRL_L,
        DPHYTX0_CORE_BASE + CSL_WIZ16B8M4CDT_DPHYTX_ISO_PHY_ISO_DL_CTRL_L0,
        DPHYTX0_CORE_BASE + CSL_WIZ16B8M4CDT_DPHYTX_ISO_PHY_ISO_DL_CTRL_L1,
        DPHYTX0_CORE_BASE + CSL_WIZ16B8M4CDT_DPHYTX_ISO_LDD_PHY_ISO_DL_CTRL_L2,
        DPHYTX0_CORE_BASE + CSL_WIZ16B8M4CDT_DPHYTX_ISO_LDD_PHY_ISO_DL_CTRL_L3
    };
    uint32_t mask[] = {
        CSL_WIZ16B8M4CDT_DPHYTX_ISO_PHY_ISO_CL_CNTRL_L_ISO_LANE_READY_CL_L_MASK,
        CSL_WIZ16B8M4CDT_DPHYTX_ISO_PHY_ISO_DL_CTRL_L0_ISO_LANE_READY_DL_L_0_MASK,
        CSL_WIZ16B8M4CDT_DPHYTX_ISO_PHY_ISO_DL_CTRL_L1_ISO_LANE_READY_DL_L_1_MASK,
        CSL_WIZ16B8M4CDT_DPHYTX_ISO_LDD_PHY_ISO_DL_CTRL_L2_ISO_LANE_READY_DL_L_2_MASK,
        CSL_WIZ16B8M4CDT_DPHYTX_ISO_LDD_PHY_ISO_DL_CTRL_L3_ISO_LANE_READY_DL_L_3_MASK
    };
    uint32_t shift[] = {
        CSL_WIZ16B8M4CDT_DPHYTX_ISO_PHY_ISO_CL_CNTRL_L_ISO_LANE_READY_CL_L_SHIFT,
        CSL_WIZ16B8M4CDT_DPHYTX_ISO_PHY_ISO_DL_CTRL_L0_ISO_LANE_READY_DL_L_0_SHIFT,
        CSL_WIZ16B8M4CDT_DPHYTX_ISO_PHY_ISO_DL_CTRL_L1_ISO_LANE_READY_DL_L_1_SHIFT,
        CSL_WIZ16B8M4CDT_DPHYTX_ISO_LDD_PHY_ISO_DL_CTRL_L2_ISO_LANE_READY_DL_L_2_SHIFT,
        CSL_WIZ16B8M4CDT_DPHYTX_ISO_LDD_PHY_ISO_DL_CTRL_L3_ISO_LANE_READY_DL_L_3_SHIFT
    };

    timeout = 0;
    do
    {
        laneStatus = (CPS_REG_READ(reg[0]) & mask[0]) >> shift[0];
        for (laneCnt = 1u; laneCnt <= numOfLanes; laneCnt ++)
        {
            laneStatus = laneStatus &
                ((CPS_REG_READ(reg[laneCnt]) & mask[laneCnt]) >>
                shift[laneCnt]);
        }

        if (1u == laneStatus)
        {
            done = TRUE;
        }

        if (WAIT_FOR_LANES_ACTIVE_REPEAT_COUNT == timeout)
        {
            status = CDN_ETIMEDOUT;
            done = TRUE;
        }
        else
        {
            timeout ++;
        }
    } while (FALSE == done);

    if (CDN_EOK == status)
    {
        done = FALSE;
        timeout = 0;
        /* DSI HS Ready Check */
        do
        {
            readData = CSL_REG32_RD (
                &dsiObj->cfgDsiTx.regBase->mctl_lane_sts);
            if ((readData & 0x40000) == 0x40000)
            {
                done = TRUE;
            }

            if (WAIT_FOR_LANES_ACTIVE_REPEAT_COUNT == timeout)
            {
                status = CDN_ETIMEDOUT;
                done = TRUE;
            }
            else
            {
                timeout ++;
            }
        } while (FALSE == done);
    }

    return (status);
}

