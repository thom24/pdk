/*
 * SDTF CCM
 *
 * SafeTI Diagnostics Library Test for CCM module
 *
 * Copyright (C) 2019 Texas Instruments Incorporated - http://www.ti.com/
 * ALL RIGHTS RESERVED
 */

#include <stdint.h>

#include <sdl_ccm.h>

#include "sdtf_common.h"
#include "sdtf_profile.h"
#include "sdtf_ccm.h"
#include "sdtf_test.h"

/*********************************************************************
 * @fn      SDTF_runCCMSelfTest
 *
 * @brief   Execute CCM Self test
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runCCMSelfTest(void)
{
    SDL_Result result;
    int32_t retVal=0;

    SDL_CCM_ErrorStatus_t errorStatus;

    SDTF_printf("\n CCM self test: starting");

    SDTF_profileBegin(SDTF_PROFILE_ONESHOT);

    result = SDL_CCM_selfTest(SDL_CCM_MONITOR_TYPE_OUTPUT_COMPARE_BLOCK,
                              SDL_CCM_SELFTEST_TYPE_NORMAL,
                              10000000,
                              &errorStatus);

    SDTF_profileEnd(SDTF_PROFILE_ONESHOT);

    if (result != SDL_PASS ) {
        SDTF_printf("\n CCM self test failed");
        SDTF_printf("\n Error status cmp error %d, self test error %d, Self test error type %d",
                    errorStatus.compareErrorFlag,
                    errorStatus.selfTestErrorFlag,
                    errorStatus.selfTestErrorTypeFlag);
        retVal = -1;
    } else {
        SDTF_printf("\n CCM Self Test complete");
        SDTF_printf("\n Cycles taken %u", SDTF_profileDelta(SDTF_PROFILE_ONESHOT));
    }

    return retVal;
}


/*********************************************************************
 * @fn      SDTF_runCCMSelfTestErrorForce
 *
 * @brief   Execute CCM Self test with error force
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runCCMSelfTestErrorForce(void)
{
    SDL_Result result;
    int32_t retVal=0;

    SDL_CCM_ErrorStatus_t errorStatus;

    SDTF_printf("\n CCM self test with error forcing: starting");

    SDTF_profileBegin(SDTF_PROFILE_ONESHOT);

    result = SDL_CCM_selfTest(SDL_CCM_MONITOR_TYPE_OUTPUT_COMPARE_BLOCK,
                              SDL_CCM_SELFTEST_TYPE_ERR_FORCING,
                              10000000,
                              &errorStatus);

    SDTF_profileEnd(SDTF_PROFILE_ONESHOT);

    if (result != SDL_PASS ) {
        SDTF_printf("\n CCM self test with error forcing failed");
        SDTF_printf("\n Error status cmp error %d, self test error %d, Self test error type %d",
                    errorStatus.compareErrorFlag,
                    errorStatus.selfTestErrorFlag,
                    errorStatus.selfTestErrorTypeFlag);
        retVal = -1;
    } else {
        SDTF_printf("\n CCM Self Test with error forcing complete");
        SDTF_printf("\n Cycles taken %u", SDTF_profileDelta(SDTF_PROFILE_ONESHOT));
    }

    return retVal;
}

/*********************************************************************
 * @fn      SDTF_runCCMInjectError
 *
 * @brief   Execute CCM Inject Error test
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runCCMInjectError(void)
{
    SDL_Result result;
    int32_t retVal=0;

    SDTF_printf("\n CCM inject  error: test starting");

    SDTF_profileBegin(SDTF_PROFILE_ONESHOT);

    result = SDL_CCM_injectError(SDL_CCM_MONITOR_TYPE_OUTPUT_COMPARE_BLOCK);

    SDTF_profileEnd(SDTF_PROFILE_ONESHOT);

    if (result != SDL_PASS ) {
        SDTF_printf("\n CCM inject failed");
       retVal = -1;
    } else {
        SDTF_printf("\n CCM inject Test complete");
        SDTF_printf("\n Cycles taken %u", SDTF_profileDelta(SDTF_PROFILE_ONESHOT));
    }

    return retVal;
}


/*********************************************************************
 * @fn      SDTF_runCCMSelftestPolarityInvert
 *
 * @brief   Execute CCM with polarity inversion
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runCCMSelftestPolarityInvert(void)
{
    SDL_Result result;
    int32_t retVal=0;

    SDTF_printf("\n CCM polarity invert self test: starting");

    SDTF_profileBegin(SDTF_PROFILE_ONESHOT);

    result = SDL_CCM_selfTestPolarityInvert(0xff, 100000);

    SDTF_profileEnd(SDTF_PROFILE_ONESHOT);

    if (result != SDL_PASS ) {
        SDTF_printf("\n CCM polarity invert self test failed");
       retVal = -1;
    } else {
        SDTF_printf("\n CCM polarity invert self test complete");
        SDTF_printf("\n Cycles taken %u", SDTF_profileDelta(SDTF_PROFILE_ONESHOT));
    }

    return retVal;
}

/*********************************************************************
 * @fn      SDTF_runCCMVIMSelfTest
 *
 * @brief   Execute CCM VIM Self test
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runCCMVIMSelfTest(void)
{
    SDL_Result result;
    int32_t retVal=0;

    SDL_CCM_ErrorStatus_t errorStatus;

    SDTF_printf("\n CCM VIM self test: starting");

    SDTF_profileBegin(SDTF_PROFILE_ONESHOT);

    result = SDL_CCM_selfTest(SDL_CCM_MONITOR_TYPE_VIM,
                              SDL_CCM_SELFTEST_TYPE_NORMAL,
                              10000000,
                              &errorStatus);

    SDTF_profileEnd(SDTF_PROFILE_ONESHOT);

    if (result != SDL_PASS ) {
        SDTF_printf("\n CCM VIM self test failed");
        SDTF_printf("\n Error status cmp error %d, self test error %d, Self test error type %d",
                    errorStatus.compareErrorFlag,
                    errorStatus.selfTestErrorFlag,
                    errorStatus.selfTestErrorTypeFlag);
        retVal = -1;
    } else {
        SDTF_printf("\n CCM VIM Self Test complete");
        SDTF_printf("\n Cycles taken %u", SDTF_profileDelta(SDTF_PROFILE_ONESHOT));
    }

    return retVal;
}

/*********************************************************************
 * @fn      SDTF_runCCMInactivitySelfTest
 *
 * @brief   Execute CCM inactivity monitor Self test
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runCCMInactivitySelfTest(void)
{
    SDL_Result result;
    int32_t retVal=0;

    SDL_CCM_ErrorStatus_t errorStatus;

    SDTF_printf("\n CCM inactivity monitor self test: starting");

    SDTF_profileBegin(SDTF_PROFILE_ONESHOT);

    result = SDL_CCM_selfTest(SDL_CCM_MONITOR_TYPE_INACTIVITY_MONITOR,
                              SDL_CCM_SELFTEST_TYPE_NORMAL,
                              10000000,
                              &errorStatus);

    SDTF_profileEnd(SDTF_PROFILE_ONESHOT);

    if (result != SDL_PASS ) {
        SDTF_printf("\n CCM inactivity monitor self test failed");
        SDTF_printf("\n Error status cmp error %d, self test error %d, Self test error type %d",
                    errorStatus.compareErrorFlag,
                    errorStatus.selfTestErrorFlag,
                    errorStatus.selfTestErrorTypeFlag);
        retVal = -1;
    } else {
        SDTF_printf("\n CCM inactivity monitor Self Test complete");
        SDTF_printf("\n Cycles taken %u", SDTF_profileDelta(SDTF_PROFILE_ONESHOT));
    }

    return retVal;
}
