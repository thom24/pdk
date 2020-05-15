/*
 * SDTF ECC
 *
 * SafeTI Diagnostics Library Test for ECC module
 *
 * Copyright (C) 2019 Texas Instruments Incorporated - http://www.ti.com/
 * ALL RIGHTS RESERVED
 */

#include <stdint.h>

#include <sdl_ecc.h>

#include "sdtf_common.h"
#include "sdtf_profile.h"
#include "sdtf_ecc.h"
#include "sdtf_test.h"

/* Local functions */
void SDTF_readVIMRAM(uint32_t offset);
int32_t SDTF_ECC_waitDEDTrigger(uint64_t timeCount);

void SDTF_ECC_dummyFunction(void);


#pragma CODE_SECTION (SDTF_ECC_dummyFunction, ".sdtf_ecc_testcodesection");

void SDTF_ECC_dummyFunction(void)
{
    volatile int i;

    i = 10;
    return;
}

/*********************************************************************
 * @fn      SDTF_runECC1BitInjectTest
 *
 * @brief   Execute ECC 1 bit inject test
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runECC1BitInjectTest(void)
{
    SDL_Result result;
    int32_t retVal=0;

    SDL_ECC_InjectErrorConfig_t injectErrorConfig;
    volatile uint32_t testLocationValue;

    SDTF_printf("\n ATCM Single bit error inject: test starting");

    SDTF_profileBegin(SDTF_PROFILE_ONESHOT);
    /* Note the address is relative to start of ram */
    injectErrorConfig.pErrMem = (uint32_t *)(0x00);

    /* Run one shot test for ATCM 1 bit error */
    injectErrorConfig.flipBitMask = 0x10;
    result = SDL_ECC_injectError(SDL_ECC_MEMTYPE_MCU_R5F0_CORE,
                                 SDL_ECC_R5F_MEM_SUBTYPE_ATCM0_BANK0_VECTOR_ID,
                                 SDL_INJECT_ECC_ERROR_FORCING_1BIT_ONCE,
                                 &injectErrorConfig);

    SDTF_profileEnd(SDTF_PROFILE_ONESHOT);

    if (result != SDL_PASS ) {
        SDTF_printf("\n ATCM Single bit error inject at pErrMem 0x%p test failed",
                    injectErrorConfig.pErrMem);
        retVal = -1;
    } else {
        /* Access the memory where injection is expected */
        testLocationValue = injectErrorConfig.pErrMem[0];

        SDTF_printf("\n ATCM Single bit error inject at pErrMem 0x%p: test complete",
                    injectErrorConfig.pErrMem);
        SDTF_printf("\n Cycles taken %u", SDTF_profileDelta(SDTF_PROFILE_ONESHOT));
    }

    return retVal;
}

/*********************************************************************
 * @fn      SDTF_runECC2BitInjectTest
 *
 * @brief   Execute ECC 2 bit Inject test
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runECC2BitInjectTest(void)
{
    SDL_Result result;
    int32_t retVal=0;

    SDL_ECC_InjectErrorConfig_t injectErrorConfig;
    volatile uint32_t testLocationValue;

    SDTF_printf("\n ATCM Double bit error inject: starting");

    SDTF_profileBegin(SDTF_PROFILE_ONESHOT);
    /* Run one shot test for ATCM 2 bit error */
    /* Note the address is relative to start of ram */
    injectErrorConfig.pErrMem = (uint32_t *)(0x200u);

    injectErrorConfig.flipBitMask = 0x101;
    result = SDL_ECC_injectError(SDL_ECC_MEMTYPE_MCU_R5F0_CORE,
                                 SDL_ECC_R5F_MEM_SUBTYPE_ATCM0_BANK0_VECTOR_ID,
                                 SDL_INJECT_ECC_ERROR_FORCING_2BIT_ONCE,
                                 &injectErrorConfig);

    SDTF_profileEnd(SDTF_PROFILE_ONESHOT);
    /* Access the memory where injection is expected */
    testLocationValue = injectErrorConfig.pErrMem[0];

    if (result != SDL_PASS ) {
        SDTF_printf("\n ATCM Double bit error inject: at pErrMem 0x%p: fixed location once test failed",
                    injectErrorConfig.pErrMem);
       retVal = -1;
    } else {
        SDTF_printf("\n ATCM Double bit error inject: pErrMem 0x%p fixed location once test complete",
                    injectErrorConfig.pErrMem);
        SDTF_printf("\n Cycles taken %u", SDTF_profileDelta(SDTF_PROFILE_ONESHOT));
    }

    return retVal;
}


void SDTF_readVIMRAM(uint32_t offset)
{

    uint32_t *vimAddr = (uint32_t *)0x40F82000;
    volatile uint32_t tmpValue;

    tmpValue = vimAddr[offset >> 2u];

}

/*********************************************************************
 * @fn      SDTF_runECC1BitVIMRAMInjectTest
 *
 * @brief   Execute ECC 1 bit inject test on VIM RAM
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runECC1BitVIMRAMInjectTest(void)
{
    SDL_Result result;
    int32_t retVal=0;

    SDL_ECC_InjectErrorConfig_t injectErrorConfig;
    volatile uint32_t testLocationValue;

    SDTF_printf("\n VIM RAM Single bit error inject: test starting");

    SDTF_profileBegin(SDTF_PROFILE_ONESHOT);
    /* Note the address is Actual error address */
    injectErrorConfig.pErrMem = (uint32_t *)(0x40F82000u);

    /* Run one shot test for ATCM 1 bit error */
    injectErrorConfig.flipBitMask = 0x10;
    result = SDL_ECC_injectError(SDL_ECC_MEMTYPE_MCU_R5F0_CORE,
                                 SDL_ECC_R5F_MEM_SUBTYPE_KS_VIM_RAM_VECTOR_ID,
                                 SDL_INJECT_ECC_ERROR_FORCING_1BIT_ONCE,
                                 &injectErrorConfig);

    SDTF_profileEnd(SDTF_PROFILE_ONESHOT);

    if (result != SDL_PASS ) {
        SDTF_printf("\n VIM RAM Single bit error inject at pErrMem 0x%p test failed",
                    injectErrorConfig.pErrMem);
        retVal = -1;
    } else {
        /* Access the memory where injection is expected */
        testLocationValue = injectErrorConfig.pErrMem[0];

        /* Note this should result in actual error call back */
        SDTF_printf("\n VIM RAM Single bit error inject at pErrMem 0x%p: test complete",
                    injectErrorConfig.pErrMem);
        SDTF_printf("\n Cycles taken %u", SDTF_profileDelta(SDTF_PROFILE_ONESHOT));
    }

    return retVal;
}


/*********************************************************************
 * @fn      SDTF_runECC2BitVIMRAMInjectTest
 *
 * @brief   Execute ECC 2 bit Inject test with VIM RAM
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runECC2BitVIMRAMInjectTest(void)
{
    SDL_Result result;
    int32_t retVal=0;

    SDL_ECC_InjectErrorConfig_t injectErrorConfig;
    volatile uint32_t testLocationValue;

    SDTF_printf("\n VIM RAM Double bit error inject: starting");

    SDTF_profileBegin(SDTF_PROFILE_ONESHOT);
    /* Run one shot test for VIM RAM 2 bit error */
    /* Note the address is actual address */
    injectErrorConfig.pErrMem = (uint32_t *)(0x40F82004u);

    injectErrorConfig.flipBitMask = 0x101;
    result = SDL_ECC_injectError(SDL_ECC_MEMTYPE_MCU_R5F0_CORE,
                                 SDL_ECC_R5F_MEM_SUBTYPE_KS_VIM_RAM_VECTOR_ID,
                                 SDL_INJECT_ECC_ERROR_FORCING_2BIT_ONCE,
                                 &injectErrorConfig);

    SDTF_profileEnd(SDTF_PROFILE_ONESHOT);

    if (result != SDL_PASS ) {
        SDTF_printf("\n VIM RAM Double bit error inject: at pErrMem 0x%p: fixed location once test failed",
                    injectErrorConfig.pErrMem);
       retVal = -1;
    } else {
        /* Access the memory where injection is expected */
        testLocationValue = injectErrorConfig.pErrMem[0];

        SDTF_printf("\n VIM RAM Double bit error inject: pErrMem 0x%p fixed location once test complete",
                    injectErrorConfig.pErrMem);
        SDTF_printf("\n Cycles taken %u", SDTF_profileDelta(SDTF_PROFILE_ONESHOT));
    }

    return retVal;
}

/*********************************************************************
 * @fn      SDTF_runECC2BitVIMRAMDEDvector
 *
 * @brief   Execute ECC 2 bit Inject test with VIM RAM,but expect to
 *          trigger VIM DED vector
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runECC2BitVIMRAMDEDvector(void)
{
    SDL_Result result;
    int32_t retVal=0;

    SDL_ECC_InjectErrorConfig_t injectErrorConfig;
    volatile uint32_t testLocationValue;

    SDTF_printf("\n VIM RAM Double bit error inject: starting");

    SDTF_profileBegin(SDTF_PROFILE_ONESHOT);
    /* Run one shot test for VIM RAM 2 bit error */
    /* This is expected to trigger the DED vector */
    injectErrorConfig.pErrMem = (uint32_t *)(0x40F82098u);

    injectErrorConfig.flipBitMask = 0x101;
    result = SDL_ECC_injectError(SDL_ECC_MEMTYPE_MCU_R5F0_CORE,
                                 SDL_ECC_R5F_MEM_SUBTYPE_KS_VIM_RAM_VECTOR_ID,
                                 SDL_INJECT_ECC_ERROR_FORCING_2BIT_ONCE,
                                 &injectErrorConfig);

    SDTF_profileEnd(SDTF_PROFILE_ONESHOT);

    if (result != SDL_PASS ) {
        SDTF_printf("\n VIM RAM Double bit error inject: at pErrMem 0x%p: fixed location once test failed",
                    injectErrorConfig.pErrMem);
       retVal = -1;
    } else {
        SDTF_printf("\n VIM RAM Double bit error inject: pErrMem 0x%p fixed location once test complete",
                    injectErrorConfig.pErrMem);
        SDTF_printf("\n Cycles taken %u", SDTF_profileDelta(SDTF_PROFILE_ONESHOT));
    }
    SDTF_printf("\n waiting for DED Vector Trigger");
    if (retVal == 0) {
        retVal = SDTF_ECC_waitDEDTrigger(0x100000000ull);
    }
    if (retVal == 0) {
        SDTF_printf("\n DED Vector Trigger: Success");
    } else {
        SDTF_printf("\n DED Vector Trigger: Fail");
    }
    return retVal;
}


/*********************************************************************
 * @fn      SDTF_runECC1BitVIMRAMSelfTest
 *
 * @brief   Execute ECC 1 bit self test on VIM RAM
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runECC1BitVIMRAMSelfTest(void)
{
    SDL_Result result;
    int32_t retVal=0;

    SDL_ECC_InjectErrorConfig_t injectErrorConfig;
    volatile uint32_t testLocationValue;

    SDTF_printf("\n VIM RAM Single bit error self test: starting");

    SDTF_profileBegin(SDTF_PROFILE_ONESHOT);
    /* Note the address is Actual error address */
    injectErrorConfig.pErrMem = (uint32_t *)(0x40F82000u);

    /* Run one shot test for ATCM 1 bit error */
    injectErrorConfig.flipBitMask = 0x10;
    result = SDL_ECC_selfTest(SDL_ECC_MEMTYPE_MCU_R5F0_CORE,
                              SDL_ECC_R5F_MEM_SUBTYPE_KS_VIM_RAM_VECTOR_ID,
                              SDL_INJECT_ECC_ERROR_FORCING_1BIT_ONCE,
                              &injectErrorConfig,
                              100000);

    SDTF_profileEnd(SDTF_PROFILE_ONESHOT);

    if (result != SDL_PASS ) {
        SDTF_printf("\n VIM RAM Single bit error self test: at pErrMem 0x%p test failed",
                    injectErrorConfig.pErrMem);
        retVal = -1;
    } else {
        /* Access the memory where injection is expected */
        testLocationValue = injectErrorConfig.pErrMem[0];

        /* Note this should result in actual error call back */
        SDTF_printf("\n VIM RAM Single bit error self test: at pErrMem 0x%p: test complete",
                    injectErrorConfig.pErrMem);
        SDTF_printf("\n Cycles taken %u", SDTF_profileDelta(SDTF_PROFILE_ONESHOT));
    }

    return retVal;
}


/*********************************************************************
 * @fn      SDTF_runECC2BitVIMRAMSelfTest
 *
 * @brief   Execute ECC 2 bit Self test with VIM RAM
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runECC2BitVIMRAMSelfTest(void)
{
    SDL_Result result;
    int32_t retVal=0;

    SDL_ECC_InjectErrorConfig_t injectErrorConfig;
    volatile uint32_t testLocationValue;

    SDTF_printf("\n VIM RAM Double bit error self test: starting");

    SDTF_profileBegin(SDTF_PROFILE_ONESHOT);
    /* Run one shot test for VIM RAM 2 bit error */
    /* Note the address is actual address */
    injectErrorConfig.pErrMem = (uint32_t *)(0x40F82004u);

    injectErrorConfig.flipBitMask = 0x101;
    result = SDL_ECC_selfTest(SDL_ECC_MEMTYPE_MCU_R5F0_CORE,
                              SDL_ECC_R5F_MEM_SUBTYPE_KS_VIM_RAM_VECTOR_ID,
                              SDL_INJECT_ECC_ERROR_FORCING_2BIT_ONCE,
                              &injectErrorConfig,
                              100000);

    SDTF_profileEnd(SDTF_PROFILE_ONESHOT);

    if (result != SDL_PASS ) {
        SDTF_printf("\n VIM RAM Double bit error self test: at pErrMem 0x%p: fixed location once test failed",
                    injectErrorConfig.pErrMem);
       retVal = -1;
    } else {
        /* Access the memory where injection is expected */
        testLocationValue = injectErrorConfig.pErrMem[0];

        SDTF_printf("\n VIM RAM Double bit error self test: pErrMem 0x%p fixed location once test complete",
                    injectErrorConfig.pErrMem);
        SDTF_printf("\n Cycles taken %u", SDTF_profileDelta(SDTF_PROFILE_ONESHOT));
    }

    return retVal;
}

/*********************************************************************
 * @fn      SDTF_runECC1BitSelfTest
 *
 * @brief   Execute ECC 1 bit Self test
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runECC1BitSelfTest(void)
{
    SDL_Result result;
    int32_t retVal=0;

    SDL_ECC_InjectErrorConfig_t injectErrorConfig;

    SDTF_printf("\n ATCM Single bit error self test: starting");

    SDTF_profileBegin(SDTF_PROFILE_ONESHOT);
    /* Note the address is relative to start of ram */
    injectErrorConfig.pErrMem = (uint32_t *)(0x100);

    /* Run one shot test for ATCM 1 bit error */
    injectErrorConfig.flipBitMask = 0x10;
    result = SDL_ECC_selfTest(SDL_ECC_MEMTYPE_MCU_R5F0_CORE,
                              SDL_ECC_R5F_MEM_SUBTYPE_ATCM0_BANK0_VECTOR_ID,
                              SDL_INJECT_ECC_ERROR_FORCING_1BIT_ONCE,
                              &injectErrorConfig,
                              100000);

    SDTF_profileEnd(SDTF_PROFILE_ONESHOT);

    if (result != SDL_PASS ) {
         SDTF_printf("\n ATCM Single bit error self test at pErrMem 0x%p test failed",
                     injectErrorConfig.pErrMem);
        retVal = -1;
    } else {

        SDTF_printf("\n ATCM Single bit error self test at pErrMem 0x%p: test complete",
                    injectErrorConfig.pErrMem);
        SDTF_printf("\n Cycles taken %u", SDTF_profileDelta(SDTF_PROFILE_ONESHOT));
    }

    return retVal;
}

/*********************************************************************
 * @fn      SDTF_runECC2BitSelfTest
 *
 * @brief   Execute ECC 2 bit Self Test
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runECC2BitSelfTest(void)
{
    SDL_Result result;
    int32_t retVal=0;

    SDL_ECC_InjectErrorConfig_t injectErrorConfig;

    SDTF_printf("\n ATCM Double bit error self test: starting");

    SDTF_profileBegin(SDTF_PROFILE_ONESHOT);
    /* Run one shot test for ATCM 2 bit error */
    /* Note the address is relative to start of ram */
    injectErrorConfig.pErrMem = (uint32_t *)(0x400u);

    injectErrorConfig.flipBitMask = 0x101;
    result = SDL_ECC_selfTest(SDL_ECC_MEMTYPE_MCU_R5F0_CORE,
                              SDL_ECC_R5F_MEM_SUBTYPE_ATCM0_BANK0_VECTOR_ID,
                              SDL_INJECT_ECC_ERROR_FORCING_2BIT_ONCE,
                              &injectErrorConfig,
                              1000);

    SDTF_profileEnd(SDTF_PROFILE_ONESHOT);

    if (result != SDL_PASS ) {
        SDTF_printf("\n ATCM Double bit error self test: at pErrMem 0x%p: fixed location once test failed",
                    injectErrorConfig.pErrMem);
        retVal = -1;
    } else {
        SDTF_printf("\n ATCM Double bit error self test: pErrMem 0x%p fixed location once test complete",
                    injectErrorConfig.pErrMem);
        SDTF_printf("\n Cycles taken %u", SDTF_profileDelta(SDTF_PROFILE_ONESHOT));
    }

    return retVal;
}

/*********************************************************************
 * @fn      SDTF_runECC1BitB0TCM0Bank0SelfTest
 *
 * @brief   Execute ECC single bit error test on BTCM0 Bank 0 memory
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runECC1BitB0TCM0Bank0SelfTest(void)
{
    SDL_Result result;
    int32_t retVal=0;

    SDL_ECC_InjectErrorConfig_t injectErrorConfig;

    SDTF_printf("\n B0TCM0 Bank0 Single bit error self test: starting");

    SDTF_profileBegin(SDTF_PROFILE_ONESHOT);
    /* Note the address is relative to start of ram */
    injectErrorConfig.pErrMem = (uint32_t *)(0x41010100u);

    /* Run one shot test for BTCM0 1 bit error */
    injectErrorConfig.flipBitMask = 0x20;
    result = SDL_ECC_selfTest(SDL_ECC_MEMTYPE_MCU_R5F0_CORE,
                              SDL_ECC_R5F_MEM_SUBTYPE_B0TCM0_BANK0_VECTOR_ID,
                              SDL_INJECT_ECC_ERROR_FORCING_1BIT_ONCE,
                              &injectErrorConfig,
                              100000);

    SDTF_profileEnd(SDTF_PROFILE_ONESHOT);

    if (result != SDL_PASS ) {
        SDTF_printf("\n B0TCM0 Bank0 Single bit error self test at pErrMem 0x%p test failed",
                    injectErrorConfig.pErrMem);
       retVal = -1;
    } else {
        SDTF_printf("\n B0TCM0 Bank0 Single bit error self test at pErrMem 0x%p: test complete",
                    injectErrorConfig.pErrMem);
        SDTF_printf("\n Cycles taken %u", SDTF_profileDelta(SDTF_PROFILE_ONESHOT));
    }

    return retVal;
}


/*********************************************************************
 * @fn      SDTF_runECC2BitB0TCM0Bank0SelfTest
 *
 * @brief   Execute ECC Double bit error test on BTCM0 Bank 0 memory
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runECC2BitB0TCM0Bank0SelfTest(void)
{
    SDL_Result result;
    int32_t retVal=0;

    SDL_ECC_InjectErrorConfig_t injectErrorConfig;

    SDTF_printf("\n B0TCM0 Bank0 Double bit error self test: starting");

    SDTF_profileBegin(SDTF_PROFILE_ONESHOT);
    /* Run one shot test for BTCM0 2 bit error */
    /* Note the address is relative to start of ram */
    injectErrorConfig.pErrMem = (uint32_t *)(0x41010200u);

    injectErrorConfig.flipBitMask = 0x201;
    result = SDL_ECC_selfTest(SDL_ECC_MEMTYPE_MCU_R5F0_CORE,
                              SDL_ECC_R5F_MEM_SUBTYPE_B0TCM0_BANK0_VECTOR_ID,
                              SDL_INJECT_ECC_ERROR_FORCING_2BIT_ONCE,
                              &injectErrorConfig,
                              1000);

    SDTF_profileEnd(SDTF_PROFILE_ONESHOT);

    if (result != SDL_PASS ) {
        SDTF_printf("\n B0TCM0 Bank0 Double bit error self test: at pErrMem 0x%p: fixed location once test failed",
                    injectErrorConfig.pErrMem);
       retVal = -1;
    } else {
        SDTF_printf("\n B0TCM0 Bank0 Double bit error self test: pErrMem 0x%p fixed location once test complete",
                    injectErrorConfig.pErrMem);
        SDTF_printf("\n Cycles taken %u", SDTF_profileDelta(SDTF_PROFILE_ONESHOT));
    }

    return retVal;
}

/*********************************************************************
 * @fn      SDTF_runECC1BitB0TCM0Bank0SelfTest
 *
 * @brief   Execute ECC Single bit error test on BTCM0 Bank 1 memory
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runECC1BitB0TCM0Bank1SelfTest(void)
{
    SDL_Result result;
    int32_t retVal=0;

    SDL_ECC_InjectErrorConfig_t injectErrorConfig;

    SDTF_printf("\n B0TCM0 Bank1 Single bit error self test: starting");

    SDTF_profileBegin(SDTF_PROFILE_ONESHOT);
    /* Note the address is relative to start of ram */
    injectErrorConfig.pErrMem = (uint32_t *)(0x41010104u);

    /* Run one shot test for BTCM0 1 bit error */
    injectErrorConfig.flipBitMask = 0x20;
    result = SDL_ECC_selfTest(SDL_ECC_MEMTYPE_MCU_R5F0_CORE,
                              SDL_ECC_R5F_MEM_SUBTYPE_B0TCM0_BANK1_VECTOR_ID,
                              SDL_INJECT_ECC_ERROR_FORCING_1BIT_ONCE,
                              &injectErrorConfig,
                              100000);

    SDTF_profileEnd(SDTF_PROFILE_ONESHOT);

    if (result != SDL_PASS ) {
        SDTF_printf("\n B0TCM0 Bank1 Single bit error self test at pErrMem 0x%p test failed",
                    injectErrorConfig.pErrMem);
        retVal = -1;
    } else {
        SDTF_printf("\n B0TCM0 Bank1 Single bit error self test at pErrMem 0x%p: test complete",
                    injectErrorConfig.pErrMem);
        SDTF_printf("\n Cycles taken %u", SDTF_profileDelta(SDTF_PROFILE_ONESHOT));
    }

    return retVal;
}

/*********************************************************************
 * @fn      SDTF_runECC2BitB0TCM0Bank0SelfTest
 *
 * @brief   Execute ECC Double bit error test on BTCM0 Bank 1 memory
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runECC2BitB0TCM0Bank1SelfTest(void)
{
    SDL_Result result;
    int32_t retVal=0;

    SDL_ECC_InjectErrorConfig_t injectErrorConfig;

    SDTF_printf("\n B0TCM0 Bank1 Double bit error self test: starting");

    SDTF_profileBegin(SDTF_PROFILE_ONESHOT);
    /* Run one shot test for BTCM0 2 bit error */
    /* Note the address is relative to start of ram */
    injectErrorConfig.pErrMem = (uint32_t *)(0x41010204u);

    injectErrorConfig.flipBitMask = 0x201;
    result = SDL_ECC_selfTest(SDL_ECC_MEMTYPE_MCU_R5F0_CORE,
                              SDL_ECC_R5F_MEM_SUBTYPE_B0TCM0_BANK1_VECTOR_ID,
                              SDL_INJECT_ECC_ERROR_FORCING_2BIT_ONCE,
                              &injectErrorConfig,
                              1000);

    SDTF_profileEnd(SDTF_PROFILE_ONESHOT);

    if (result != SDL_PASS ) {
        SDTF_printf("\n B0TCM0 Bank1 Double bit error self test: at pErrMem 0x%p: fixed location once test failed",
                    injectErrorConfig.pErrMem);
        retVal = -1;
    } else {
        SDTF_printf("\n B0TCM0 Bank1 Double bit error self test: pErrMem 0x%p fixed location once test complete",
                    injectErrorConfig.pErrMem);
        SDTF_printf("\n Cycles taken %u", SDTF_profileDelta(SDTF_PROFILE_ONESHOT));
    }

    return retVal;
}

/*********************************************************************
 * @fn      SDTF_runECC1BitB1TCM0Bank0SelfTest
 *
 * @brief   Execute ECC Double bit error test on B1TCM0 Bank 0 memory
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runECC1BitB1TCM0Bank0SelfTest(void)
{
    SDL_Result result;
    int32_t retVal=0;

    SDL_ECC_InjectErrorConfig_t injectErrorConfig;

    SDTF_printf("\n B1TCM0 Bank0 Single bit error self test: starting");

    SDTF_profileBegin(SDTF_PROFILE_ONESHOT);
     /* Note the address is relative to start of ram */
     injectErrorConfig.pErrMem = (uint32_t *)(0x41010108u);

     /* Run one shot test for BTCM0 1 bit error */
     injectErrorConfig.flipBitMask = 0x20;
     result = SDL_ECC_selfTest(SDL_ECC_MEMTYPE_MCU_R5F0_CORE,
                               SDL_ECC_R5F_MEM_SUBTYPE_B1TCM0_BANK0_VECTOR_ID,
                               SDL_INJECT_ECC_ERROR_FORCING_1BIT_ONCE,
                               &injectErrorConfig,
                               100000);

     SDTF_profileEnd(SDTF_PROFILE_ONESHOT);

     if (result != SDL_PASS ) {
         SDTF_printf("\n B1TCM0 Bank0 Single bit error self test at pErrMem 0x%p test failed",
                     injectErrorConfig.pErrMem);
        retVal = -1;
    } else {
        SDTF_printf("\n B1TCM0 Bank0 Single bit error self test at pErrMem 0x%p: test complete",
                    injectErrorConfig.pErrMem);
        SDTF_printf("\n Cycles taken %u", SDTF_profileDelta(SDTF_PROFILE_ONESHOT));
    }

    return retVal;
}

/*********************************************************************
 * @fn      SDTF_runECC2BitB1TCM0Bank0SelfTest
 *
 * @brief   Execute ECC Double bit error test on B1TCM0 Bank 0 memory
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runECC2BitB1TCM0Bank0SelfTest(void)
{
    SDL_Result result;
    int32_t retVal=0;

    SDL_ECC_InjectErrorConfig_t injectErrorConfig;

    SDTF_printf("\n B1TCM0 Bank0 Double bit error self test: starting");

    SDTF_profileBegin(SDTF_PROFILE_ONESHOT);
    /* Run one shot test for BTCM0 2 bit error */
    /* Note the address is relative to start of ram */
    injectErrorConfig.pErrMem = (uint32_t *)(0x41010208u);

    injectErrorConfig.flipBitMask = 0x201;
    result = SDL_ECC_selfTest(SDL_ECC_MEMTYPE_MCU_R5F0_CORE,
                              SDL_ECC_R5F_MEM_SUBTYPE_B1TCM0_BANK0_VECTOR_ID,
                              SDL_INJECT_ECC_ERROR_FORCING_2BIT_ONCE,
                              &injectErrorConfig,
                              1000);

    SDTF_profileEnd(SDTF_PROFILE_ONESHOT);

    if (result != SDL_PASS ) {
        SDTF_printf("\n B1TCM0 Bank0 Double bit error self test: at pErrMem 0x%p: fixed location once test failed",
                    injectErrorConfig.pErrMem);
       retVal = -1;
    } else {
        SDTF_printf("\n B1TCM0 Bank0 Double bit error self test: pErrMem 0x%p fixed location once test complete",
                    injectErrorConfig.pErrMem);
        SDTF_printf("\n Cycles taken %u", SDTF_profileDelta(SDTF_PROFILE_ONESHOT));
    }

    return retVal;
}

/*********************************************************************
 * @fn      SDTF_runECC1BitB1TCM0Bank1SelfTest
 *
 * @brief   Execute ECC Single bit error test on B1TCM0 Bank 1 memory
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runECC1BitB1TCM0Bank1SelfTest(void)
{
    SDL_Result result;
    int32_t retVal=0;

    SDL_ECC_InjectErrorConfig_t injectErrorConfig;

    SDTF_printf("\n B1TCM0 Bank1 Single bit error self test: starting");

    SDTF_profileBegin(SDTF_PROFILE_ONESHOT);
    /* Note the address is relative to start of ram */
    injectErrorConfig.pErrMem = (uint32_t *)(0x4101010cu);

    /* Run one shot test for BTCM0 1 bit error */
    injectErrorConfig.flipBitMask = 0x20;
    result = SDL_ECC_selfTest(SDL_ECC_MEMTYPE_MCU_R5F0_CORE,
                              SDL_ECC_R5F_MEM_SUBTYPE_B1TCM0_BANK1_VECTOR_ID,
                              SDL_INJECT_ECC_ERROR_FORCING_1BIT_ONCE,
                              &injectErrorConfig,
                              100000);

    SDTF_profileEnd(SDTF_PROFILE_ONESHOT);

    if (result != SDL_PASS ) {
        SDTF_printf("\n B1TCM0 Bank1 Single bit error self test at pErrMem 0x%p test failed",
                    injectErrorConfig.pErrMem);
       retVal = -1;
    } else {
        SDTF_printf("\n B1TCM0 Bank1 Single bit error self test at pErrMem 0x%p: test complete",
                    injectErrorConfig.pErrMem);
        SDTF_printf("\n Cycles taken %u", SDTF_profileDelta(SDTF_PROFILE_ONESHOT));
    }

    return retVal;
}

/*********************************************************************
 * @fn      SDTF_runECC2BitB1TCM0Bank1SelfTest
 *
 * @brief   Execute ECC Single bit error test on B1TCM0 Bank 1 memory
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runECC2BitB1TCM0Bank1SelfTest(void)
{
    SDL_Result result;
    int32_t retVal=0;

    SDL_ECC_InjectErrorConfig_t injectErrorConfig;

    SDTF_printf("\n B1TCM0 Bank1 Double bit error self test: starting");

    SDTF_profileBegin(SDTF_PROFILE_ONESHOT);
    /* Run one shot test for BTCM0 2 bit error */
    /* Note the address is relative to start of ram */
    injectErrorConfig.pErrMem = (uint32_t *)(0x4101020cu);

    injectErrorConfig.flipBitMask = 0x201;
    result = SDL_ECC_selfTest(SDL_ECC_MEMTYPE_MCU_R5F0_CORE,
                              SDL_ECC_R5F_MEM_SUBTYPE_B1TCM0_BANK1_VECTOR_ID,
                              SDL_INJECT_ECC_ERROR_FORCING_2BIT_ONCE,
                              &injectErrorConfig,
                              1000);

    SDTF_profileEnd(SDTF_PROFILE_ONESHOT);

    if (result != SDL_PASS ) {
        SDTF_printf("\n B1TCM0 Bank1 Double bit error self test: at pErrMem 0x%p: fixed location once test failed",
                    injectErrorConfig.pErrMem);
        retVal = -1;
    } else {
        SDTF_printf("\n B1TCM0 Bank1 Double bit error self test: pErrMem 0x%p fixed location once test complete",
                    injectErrorConfig.pErrMem);
        SDTF_printf("\n Cycles taken %u", SDTF_profileDelta(SDTF_PROFILE_ONESHOT));
    }

    return retVal;
}
static bool SDTF_ECC_DEDTriggerFlag = false;

/*********************************************************************
 *
 * @brief   indicate DED trigger
 *
 *
 * @return  0 : Success; < 0 for failures
 */
void SDTF_ECC_indicateDEDTrigger(void)
{
    SDTF_ECC_DEDTriggerFlag = true;
}


int32_t SDTF_ECC_waitDEDTrigger(uint64_t timeCount)
{
    int32_t retVal = 0;
    uint32_t startTimeStamp, curTimeStamp, lastTimeStamp, deltaTime;
    uint64_t elapsedTime = 0u;

    startTimeStamp = SDL_getTime();
    lastTimeStamp = startTimeStamp;
    while(SDTF_ECC_DEDTriggerFlag != true) {
        curTimeStamp = SDL_getTime();
        deltaTime = curTimeStamp-lastTimeStamp;
        lastTimeStamp = curTimeStamp;
        elapsedTime += deltaTime;
        if (elapsedTime > timeCount) {
            break;
        }
    }

    if (SDTF_ECC_DEDTriggerFlag == true ) {
        SDTF_ECC_DEDTriggerFlag = false;
    } else {
        retVal = -1;
    }
    return retVal;
}

/*********************************************************************
 *
 * @brief   Run Negative tests on ECC module
 *
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_ECC_runNegativeTests(void)
{
    SDL_Result result;
    int32_t retVal = 0;
    SDL_ECC_InjectErrorConfig_t injectErrorConfig;

    SDTF_printf("\n ECC negative tests: starting");

    /* Negative tests with invalid mem type 33U */
    result = SDL_ECC_initMemory(33U,
                                0U);
    if (result == SDL_PASS) {
        retVal = -1;
    }

    if (retVal == 0) {
        /* Negative tests with invalid mem subtype 50U */
        result = SDL_ECC_initMemory(SDL_ECC_MEMTYPE_MCU_R5F0_CORE,
                                    50U);
        if (result == SDL_PASS) {
            retVal = -1;
        }
    }

    if (retVal == 0) {
        /* Negative tests with invalid mem subtype 50U */
        result = SDL_ECC_selfTest(SDL_ECC_MEMTYPE_MCU_R5F0_CORE,
                                  SDL_ECC_R5F_MEM_SUBTYPE_ATCM0_BANK0_VECTOR_ID,
                                  SDL_INJECT_ECC_ERROR_FORCING_1BIT_ONCE,
                                  NULL,
                                  1000);
        if (result != SDL_BADARGS) {
            retVal = -1;
        }
    }

    if (retVal == 0) {
        /* Negative tests with invalid mem subtype 50U */
        result = SDL_ECC_injectError(SDL_ECC_MEMTYPE_MCU_R5F0_CORE,
                                  SDL_ECC_R5F_MEM_SUBTYPE_ATCM0_BANK0_VECTOR_ID,
                                  SDL_INJECT_ECC_ERROR_FORCING_1BIT_ONCE,
                                  NULL
                                  );
        if (result != SDL_BADARGS) {
            retVal = -1;
        }
    }


    if (retVal == 0) {
        /* Pass invalid error configuration. No bits set */
        injectErrorConfig.flipBitMask = 0x00;
        /* Set Error address */
        injectErrorConfig.pErrMem = (uint32_t *)(0x100);
        /* Negative tests with invalid mem subtype 50U */
        result = SDL_ECC_selfTest(SDL_ECC_MEMTYPE_MCU_R5F0_CORE,
                                  SDL_ECC_R5F_MEM_SUBTYPE_ATCM0_BANK0_VECTOR_ID,
                                  SDL_INJECT_ECC_ERROR_FORCING_1BIT_ONCE,
                                  &injectErrorConfig,
                                  1000);
        if (result == SDL_PASS) {
            retVal = -1;
        }
    }

    if (retVal == 0) {
        /* Pass invalid error configuration. No bits set */
        injectErrorConfig.flipBitMask = 0x80000000;
        /* Set Error address */
        injectErrorConfig.pErrMem = (uint32_t *)(0x100);
        /* Negative tests with invalid mem subtype 50U */
        result = SDL_ECC_selfTest(SDL_ECC_MEMTYPE_MCU_R5F0_CORE,
                                  SDL_ECC_R5F_MEM_SUBTYPE_ATCM0_BANK0_VECTOR_ID,
                                  SDL_INJECT_ECC_ERROR_FORCING_2BIT_ONCE,
                                  &injectErrorConfig,
                                  1000);
        if (result == SDL_PASS) {
            retVal = -1;
        }
    }

    if ( retVal == 0) {
        SDTF_printf("\n ECC negative tests: success");
    } else {
        SDTF_printf("\n ECC negative tests: failed");
    }

    return retVal;
}

/*********************************************************************
 * @fn      SDTF_runECC2BitCodeInjectTest
 *
 * @brief   Execute ECC 2 bit Inject error on program section
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runECC2BitCodeInjectTest(void)
{
    SDL_Result result;
    int32_t retVal=0;

    SDL_ECC_InjectErrorConfig_t injectErrorConfig;
    volatile uint32_t testLocationValue;

    SDTF_printf("\n ATCM Double bit Program section error inject: starting");

    SDTF_profileBegin(SDTF_PROFILE_ONESHOT);
    /* Error injection on program memory */
    injectErrorConfig.pErrMem = (uint32_t *)(&SDTF_ECC_dummyFunction);

    injectErrorConfig.flipBitMask = 0x101;

    result = SDL_ECC_injectError(SDL_ECC_MEMTYPE_MCU_R5F0_CORE,
                                 SDL_ECC_R5F_MEM_SUBTYPE_ATCM0_BANK0_VECTOR_ID,
                                 SDL_INJECT_ECC_ERROR_FORCING_2BIT_ONCE,
                                 &injectErrorConfig);

    SDTF_profileEnd(SDTF_PROFILE_ONESHOT);

    /* Call progrem for the ECC error to take effect */
    SDTF_ECC_dummyFunction();

    if (result != SDL_PASS ) {
        SDTF_printf("\n ATCM Double bit Program section error inject: at pErrMem 0x%p: fixed location once test failed",
                    injectErrorConfig.pErrMem);
       retVal = -1;
    } else {
        SDTF_printf("\n ATCM Double bit Program section error inject: pErrMem 0x%p fixed location once test complete",
                    injectErrorConfig.pErrMem);
        SDTF_printf("\n Cycles taken %u", SDTF_profileDelta(SDTF_PROFILE_ONESHOT));
    }

    return retVal;
}

