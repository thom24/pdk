/*
 * SDL MPU
 *
 * SafeTI Diagnostics Library module for MPU
 *
 * Copyright (C) 2018-2019 Texas Instruments Incorporated - http://www.ti.com/
 * ALL RIGHTS RESERVED
 */

#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <sdl_common.h>
#include <sdl_mpu.h>
#include <sdl_exception.h>
#include <sdl_mpuCfg.h>

#include <ti/csl/soc.h>

#include "sdl_utils.h"

/** ---------------------------------------------------------------------------
 * @brief This enumerator defines the values for mpu self test flag
 * ----------------------------------------------------------------------------
 */
typedef enum {
    SDL_MPU_SELF_TEST_FLAG_NONE=0,
    SDL_MPU_SELF_TEST_FLAG_STARTING=0x55555555U,
    SDL_MPU_SELF_TEST_FLAG_TRIGGERED=0xAAAAAAAAU,

} SDL_mpuSelfTestFlag;

/** ---------------------------------------------------------------------------
 * @brief This structure defines the data used for tracking the current
 *        self test
 * ----------------------------------------------------------------------------
 */
typedef struct SDL_MpuSelfTest_s
{
    volatile SDL_mpuSelfTestFlag    mpuSelfTestState;
    /**< MPU self test state in progress */
    uint32_t               regId;
    /**< MPU self test memory configuration region ID */
    uint32_t               faultAddr;
    /**< MPU self test fault address */
    SDL_MPU_memAccessType  faultType;
    /**< MPU self test fault type */
}  SDL_MpuSelfTest_t;

/* static Global objects */
static SDL_MpuSelfTest_t gSDL_MPUSelfTest;

/* Global Objects */
/* None */

/* Local functions */
static void  SDL_trigger_fault(const SDL_MPU_memConfig_t *pMemConfig);
bool SDL_MPU_exceptionChkSelfTest(SDL_MPU_memAccessType faultType, uint32_t faultAddr);

static void  SDL_trigger_fault(const SDL_MPU_memConfig_t *pMemConfig)
{
    uint32_t      *pMemLocation = pMemConfig->pMemLocation;
    volatile       uint32_t       dummy = 1U;

    if (pMemConfig->memAccessType == SDL_MPU_DATA_WRITE_ACCESS) {
        /* write to the memory to trigger the exception */
        *pMemLocation = dummy;
    }
    else {
        /* read from the memory to trigger the exception */
        dummy = *pMemLocation;
    }
    return;
}

/* Global Functions */

/** ============================================================================*
 *
 * Design: did_mpu_diagnostics
 * Requirement: REQ_TAG(PRSDK-1665)
 *
 * \brief   MPU SELF TEST Check Handler (NOT AN API for SDL MPU)
 *          corrects any mpu self test faults
 *
 * \return  true : if it is in self test; false otherwise
 */
bool SDL_MPU_exceptionChkSelfTest(SDL_MPU_memAccessType faultType, uint32_t faultAddr)
{
    bool    selfTestFlag = (bool)false;

    if ( (gSDL_MPUSelfTest.mpuSelfTestState == SDL_MPU_SELF_TEST_FLAG_STARTING) &&
         (gSDL_MPUSelfTest.faultAddr        == faultAddr) &&
         (gSDL_MPUSelfTest.faultType        == faultType) )
    {
        /* It is in self test flag */
        selfTestFlag = (bool)true;

        /* indicate self test exception trigger flag */
        gSDL_MPUSelfTest.mpuSelfTestState = SDL_MPU_SELF_TEST_FLAG_TRIGGERED;

        /* Provide the necessary privillage for the MPU exception to resolve */
        SDL_MPUResolve(gSDL_MPUSelfTest.faultAddr, gSDL_MPUSelfTest.regId);
    }
    return (selfTestFlag);
}

/** ============================================================================
 *
 * Design: did_mpu_diagnostics
 * Requirement: REQ_TAG(PRSDK-1665)
 *
 * \brief   Runs self test by injecting a fault and monitor response
 *          Assumes MPU Settings from higher layer is already enabled.
 *
 * \param1  pMemConfig: pointer to memory configuration to be used for exception
 * \param2  loopCount: Loopcount for diagnostics to wait in a loop
 *
 * \return  SDL_PASS : Success; SDL_FAIL for failures
 */
SDL_Result SDL_MPU_selfTest(const SDL_MPU_memConfig_t *pMemConfig,
                            uint32_t             loopCount)
{
    SDL_Result retVal = SDL_FAIL;
    uint32_t   waitForEver = 1U, loopCnt = loopCount;

    /* Reset self test flag */
    gSDL_MPUSelfTest.mpuSelfTestState = SDL_MPU_SELF_TEST_FLAG_NONE;

    if ((pMemConfig != NULL) &&
        (pMemConfig->mpuRegionId < 16u)) {

        /* start self test for MPU */
        gSDL_MPUSelfTest.mpuSelfTestState = SDL_MPU_SELF_TEST_FLAG_STARTING;
        /* Copy the mpu memory configuration */
        gSDL_MPUSelfTest.regId  = pMemConfig->mpuRegionId;
        /* Record the mpu fault addresss */
        gSDL_MPUSelfTest.faultAddr = (uint32_t) (uintptr_t) pMemConfig->pMemLocation;
        /* Record the mpu fault type */
        gSDL_MPUSelfTest.faultType = pMemConfig->memAccessType;

        /* Add the MPU entry for the fault trigger */
        SDL_MPUCfgAddRegion(pMemConfig);

        /* trigger the mpu self test */
        SDL_trigger_fault(pMemConfig);

        /* wait until loopCount (> 0) for exception to trigger */
        if (loopCnt > 0U) {
            while(loopCnt != 0U)  {
                loopCnt--;
                /* If exception is triggered, self test is successful */
                if (gSDL_MPUSelfTest.mpuSelfTestState == SDL_MPU_SELF_TEST_FLAG_TRIGGERED)
                {
                    retVal = SDL_PASS;
                    break;
                }
            }
       } else { /* Wait forever configuration */
            while (waitForEver != 0U)  {
                /* If exception is triggered, self test is successful */
                if (gSDL_MPUSelfTest.mpuSelfTestState == SDL_MPU_SELF_TEST_FLAG_TRIGGERED)
                {
                    retVal = SDL_PASS;
                    break;
                }
           }
       }
       /* Remove the MPU entry for the fault trigger */
       SDL_MPUCfgRemoveRegion(pMemConfig);

       /* Reset self test flag */
       gSDL_MPUSelfTest.mpuSelfTestState = SDL_MPU_SELF_TEST_FLAG_NONE;
    }
    return retVal;
}

/* Nothing past this point */
