/*
 * SDTF ESM
 *
 * SafeTI Diagnostics Library Test for ESM module
 *
 * Copyright (C) 2019 Texas Instruments Incorporated - http://www.ti.com/
 * ALL RIGHTS RESERVED
 */

#include <stdint.h>

#include <sdl_esm.h>

#include "sdtf_common.h"
#include "sdtf_profile.h"
#include "sdtf_esm.h"
#include "sdtf_test.h"

/*********************************************************************
 * @fn      SDTF_runESMSelfTest
 *
 * @brief   Execute ESM Self test
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runESMSelfTest(void)
{
    SDL_Result result;
    int32_t retVal=0;

    SDTF_printf("\n ESM self test: starting");
    SDTF_profileBegin(SDTF_PROFILE_ONESHOT);
    /* Run esm test 1*/
    result = SDL_ESM_selfTest(1000);
    SDTF_profileEnd(SDTF_PROFILE_ONESHOT);
    if (result != SDL_PASS ) {
        SDTF_printf("\n ESM self test  failed");
        retVal = -1;
    } else {
        SDTF_printf("\n ESM self test  passed");
        SDTF_printf("\n Cycles taken %u", SDTF_profileDelta(SDTF_PROFILE_ONESHOT));
    }

    return retVal;
}

/*********************************************************************
 * @fn      SDTF_runESMInject
 *
 * @brief   Execute ESM Inject
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runESMInject(void)
{
    SDL_Result result;
    int32_t retVal=0;

    SDL_ESM_ErrorConfig_t esmErrorConfig;

    esmErrorConfig.groupNumber = 1;
    esmErrorConfig.bitNumber = 4;

    SDTF_printf("\n ESM inject: test starting");

    SDTF_profileBegin(SDTF_PROFILE_ONESHOT);
    /* Run esm test 2*/
    result = SDL_ESM_errorInsert(&esmErrorConfig);
    SDTF_profileEnd(SDTF_PROFILE_ONESHOT);
    if (result != SDL_PASS ) {
        SDTF_printf("\n ESM inject test  failed");
        retVal = -1;
    } else {
        SDTF_printf("\n ESM inject test  Done");
        SDTF_printf("\n Cycles taken %u", SDTF_profileDelta(SDTF_PROFILE_ONESHOT));
    }

    return retVal;
}

/*********************************************************************
 * @fn      SDTF_runESMAPITest
 *
 * @brief   Execute ESM API test, covers APIs not covered by functional test
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runESMAPITest(void)
{
    int32_t retVal=0;
    volatile bool errorStatus;

    SDTF_printf("\n ESM API test: starting");

    /* Run Set N Error */
    SDL_ESM_setNError();

    /* Get error status to check */
    errorStatus = SDL_ESM_getNErrorStatus();
    if (errorStatus) {
        retVal = -1;
    }

    if (retVal == 0) {
        /* Run Reset N Error */
        SDL_ESM_resetNError();
        /* Get error status to check */
        errorStatus = SDL_ESM_getNErrorStatus();
        if (!errorStatus) {
            retVal = -2;
        }
    }

    if (retVal == 0) {
        SDTF_printf("\n ESM API test  passed");
    } else {
        SDTF_printf("\n ESM API test  failed");
    }

    return retVal;
}

/*********************************************************************
 * @fn      SDTF_runESMNegativeTest
 *
 * @brief   Execute ESM Negative tests
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runESMNegativeTest(void)
{
    SDL_Result result;
    int32_t retVal=0;
    volatile bool errorStatus;

    SDTF_printf("\n ESM Negative  test: starting");

    result = SDL_ESM_errorInsert(NULL);

    if ( result == SDL_PASS)
    {
        retVal = -1;
    }

    if (retVal != 0 ) {
        SDTF_printf("\n ESM Negative test failed");
    } else {
        SDTF_printf("\n ESM Negative test passed");
    }

    return retVal;
}
