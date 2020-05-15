/*
 * SDL MPU Cfg
 *
 * SafeTI Diagnostics Library module for MPU Configuration
 *
 * Copyright (C) 2018-2019 Texas Instruments Incorporated - http://www.ti.com/
 * ALL RIGHTS RESERVED
 */

#include <sdl_mpuCfg.h>

/* Local functions */
static void SDL_mpuCfg_local(const CSL_ArmR5MpuRegionCfg *pCfg);

static const uint32_t gMemAttr[CSL_ARM_R5_MEM_ATTR_MAX][3U] =
{
/*    TEX[2:0], C,     B bits */
    {   0x0U,   0x0U,  0x0U,}, /* Strongly-ordered.*/
    {   0x0U,   0x0U,  0x1U,}, /* Shareable Device.*/
    {   0x0U,   0x1U,  0x0U,}, /* Outer and Inner write-through, no write-allocate. */
    {   0x0U,   0x1U,  0x1U,}, /* Outer and Inner write-back, no write-allocate. */
    {   0x1U,   0x0U,  0x0U,}, /* Outer and Inner Non-cacheable. */
    {   0x1U,   0x1U,  0x1U,}, /* Outer and Inner write-back, write-allocate.*/
    {   0x2U,   0x0U,  0x0U,}, /* Non-shareable Device.*/
};

static  const CSL_ArmR5MpuRegionCfg   gCfg =
    {
        /* Region X configuration for 32 byte sizes */
        .regionId         = 0U,
        .enable           = 1U,
        .baseAddr         = 0x0U,
        .size             = CSL_ARM_R5_MPU_REGION_SIZE_32B,
        .subRegionEnable  = CSL_ARM_R5_MPU_SUB_REGION_ENABLE_ALL,
        .exeNeverControl  = 1U,
        .accessPermission = CSL_ARM_R5_ACC_PERM_PRIV_USR_RD_WR,
        .shareable        = 0U,
        .cacheable        = 0U,
        .cachePolicy      = 0U,
        .memAttr          = 0U,
    };


void SDL_MPUResolve(uint32_t baseAddr, uint32_t regId)
{
    CSL_ArmR5MpuRegionCfg   cfg = gCfg;

    /* Update the base addr and region ID configured */
    cfg.baseAddr            = baseAddr;
    cfg.regionId            = regId;
    cfg.accessPermission    = CSL_ARM_R5_ACC_PERM_PRIV_USR_RD_WR;

    SDL_mpuCfg_local(&cfg);
    return;
}

void SDL_MPUCfgAddRegion(const SDL_MPU_memConfig_t *pMemConfig)
{
    CSL_ArmR5MpuRegionCfg   cfg = gCfg;
    /* Update the base addr and region ID configured */
    cfg.baseAddr = (uint32_t) (uintptr_t) pMemConfig->pMemLocation;
    cfg.regionId = pMemConfig->mpuRegionId;

    /* Update the access permission for the configured */
    if (pMemConfig->memAccessType == SDL_MPU_DATA_WRITE_ACCESS) {
         cfg.accessPermission = CSL_ARM_R5_ACC_PERM_PRIV_USR_RD;
    }
    else {
         cfg.accessPermission = CSL_ARM_R5_ACC_PERM_NO_ACCESS;
    }

    SDL_mpuCfg_local(&cfg);
    return;
}

/* This is an internal implementation for MPU configuration for a given Region */
static void SDL_mpuCfg_local(const CSL_ArmR5MpuRegionCfg *pCfg)
{
    uint32_t baseAddrRegVal, sizeRegVal, accessCtrlRegVal, tex;

    /* Create the region base Address register value */
    baseAddrRegVal   = ( pCfg->baseAddr &
                                    CSL_ARM_R5_MPU_REGION_BASE_ADDR_MASK);

    /* Create Region Size and Enable register value */
    sizeRegVal       = ( pCfg->enable <<
                                    CSL_ARM_R5_MPU_REGION_SZEN_EN_SHIFT);
    sizeRegVal       |= ( pCfg->size <<
                                    CSL_ARM_R5_MPU_REGION_SZEN_SZ_SHIFT);
    sizeRegVal       |= ( pCfg->subRegionEnable <<
                                    CSL_ARM_R5_MPU_REGION_SZEN_SRD_SHIFT);
    /* Create the access control register value */
    accessCtrlRegVal  = ( pCfg->exeNeverControl <<
                                   CSL_ARM_R5_MPU_REGION_AC_XN_SHIFT);
    accessCtrlRegVal |= ( pCfg->accessPermission <<
                                   CSL_ARM_R5_MPU_REGION_AC_AP_SHIFT);
    accessCtrlRegVal |= ( pCfg->shareable <<
                                   CSL_ARM_R5_MPU_REGION_AC_S_SHIFT);

    /* Below code is creating the accessCtrlReg value for cachable == 0u */
    tex = gMemAttr[pCfg->memAttr][0U];
    accessCtrlRegVal |=  ( tex << CSL_ARM_R5_MPU_REGION_AC_TEX_SHIFT);
    accessCtrlRegVal |=
                         ( gMemAttr[pCfg->memAttr][1U] <<
                           CSL_ARM_R5_MPU_REGION_AC_B_SHIFT);
    accessCtrlRegVal |=
                         ( gMemAttr[pCfg->memAttr][2U] <<
                           CSL_ARM_R5_MPU_REGION_AC_C_SHIFT);

    /* configure MPU region here */
    CSL_armR5MpuCfgRegion(pCfg->regionId,
                          baseAddrRegVal,
                          sizeRegVal,
                          accessCtrlRegVal);
    return;
}

/* Internal implementation to remove the MPU configuration */
void SDL_MPUCfgRemoveRegion(const SDL_MPU_memConfig_t *pMemConfig)
{
    uint32_t baseAddrRegVal = 0U, sizeRegVal = 0U, accessCtrlRegVal = 0U;

    /* Remove the MPU configured for that Region */
    CSL_armR5MpuCfgRegion(pMemConfig->mpuRegionId,
                                  baseAddrRegVal,
                                  sizeRegVal,
                                  accessCtrlRegVal);
    return;
}

