/*
 * SDTF MPU
 *
 * SafeTI Diagnostics Library Test for MPU module
 *
 * Copyright (C) 2019 Texas Instruments Incorporated - http://www.ti.com/
 * ALL RIGHTS RESERVED
 */

#include <stdint.h>

#include <sdl_mpu.h>

#include "sdtf_common.h"
#include "sdtf_profile.h"
#include "sdtf_mpu.h"
#include "sdtf_test.h"

/* SDTF MPU SelfTest local defines */

#define SDTF_MPU_SELF_TEST_MEM_BASE  (0x41C7EFE0U)
#define SDTF_MPU_SELF_TEST_MEM_SIZE  (32U)

/*********************************************************************
 * @fn      SDTF_runMPUSelfTest
 *
 * @brief   Execute MPU Self test
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runMPUSelfTest(void)
{
    SDL_Result result;
    SDL_MPU_memConfig_t  triggerFaultMemRd, triggerFaultMemWr;
    uint32_t             loopCnt = 10U;
    int32_t              retVal=0;

    /* Initialize the MPU for the test */
    triggerFaultMemWr.pMemLocation  = (uint32_t *) SDTF_MPU_SELF_TEST_MEM_BASE;
    triggerFaultMemWr.memAccessType = SDL_MPU_DATA_WRITE_ACCESS;
    triggerFaultMemWr.mpuRegionId   = 8;

    triggerFaultMemRd.pMemLocation  = (uint32_t *) SDTF_MPU_SELF_TEST_MEM_BASE;
    triggerFaultMemRd.memAccessType = SDL_MPU_DATA_READ_ACCESS;
    triggerFaultMemRd.mpuRegionId   = 8;

    /* Initialize the memory for triggering MPU exception */
    SDTF_printf("\n MPU self test for Mem Write Access Fault: starting");
    SDTF_profileBegin(SDTF_PROFILE_ONESHOT);

    result = SDL_MPU_selfTest(&triggerFaultMemWr, loopCnt);

    SDTF_profileEnd(SDTF_PROFILE_ONESHOT);

    if (result != SDL_PASS ) {
        SDTF_printf("\n MPU self test for Mem Write Access Fault failed");
        retVal = -1;
    } else {
        SDTF_printf("\n MPU self test for Mem Write Access Fault complete");
        SDTF_printf("\n Cycles taken %u", SDTF_profileDelta(SDTF_PROFILE_ONESHOT));
    }

    SDTF_printf("\n MPU self test for Mem Read Access Fault: starting");
    SDTF_profileBegin(SDTF_PROFILE_ONESHOT);

    result = SDL_MPU_selfTest(&triggerFaultMemRd, loopCnt);

    SDTF_profileEnd(SDTF_PROFILE_ONESHOT);

    if (result != SDL_PASS ) {
        SDTF_printf("\n MPU self test for Mem Read Access Fault failed");
        retVal = -1;
    } else {
        SDTF_printf("\n MPU self test for Mem Read Access Fault complete");
        SDTF_printf("\n Cycles taken %u", SDTF_profileDelta(SDTF_PROFILE_ONESHOT));
    }


    SDTF_printf("\n MPU self test for Mem Write Access Fault (Wait Forever): starting");
    SDTF_profileBegin(SDTF_PROFILE_ONESHOT);

    result = SDL_MPU_selfTest(&triggerFaultMemWr, 0U);

    SDTF_profileEnd(SDTF_PROFILE_ONESHOT);

    if (result != SDL_PASS ) {
        SDTF_printf("\n MPU self test for Mem Write Access Fault (Wait Forever) failed");
        retVal = -1;
    } else {
        SDTF_printf("\n MPU self test for Mem Write Access Fault  (Wait Forever) complete");
        SDTF_printf("\n Cycles taken %u", SDTF_profileDelta(SDTF_PROFILE_ONESHOT));
    }

    SDTF_printf("\n MPU self test for Mem Read Access Fault  (Wait Forever): starting");
    SDTF_profileBegin(SDTF_PROFILE_ONESHOT);

    result = SDL_MPU_selfTest(&triggerFaultMemRd, 0U);

    SDTF_profileEnd(SDTF_PROFILE_ONESHOT);

    if (result != SDL_PASS ) {
        SDTF_printf("\n MPU self test for Mem Read Access Fault (Wait Forever) failed");
        retVal = -1;
    } else {
        SDTF_printf("\n MPU self test for Mem Read Access Fault  (Wait Forever) complete");
        SDTF_printf("\n Cycles taken %u", SDTF_profileDelta(SDTF_PROFILE_ONESHOT));
    }

    return retVal;
}
