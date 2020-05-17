/*
 * SDTF ESM
 *
 * Software Diagnostics Library Test for ESM module
 *
 *  Copyright (c) Texas Instruments Incorporated 2019-2020
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
 *
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
