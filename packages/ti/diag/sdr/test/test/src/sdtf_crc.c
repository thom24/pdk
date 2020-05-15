/*
 * SDTF CRC
 *
 * SafeTI Diagnostics Library Test for CRC module
 *
 * Copyright (C) 2019 Texas Instruments Incorporated - http://www.ti.com/
 * ALL RIGHTS RESERVED
 */

#include <stdint.h>

#include <sdl_crc.h>

#include "sdtf_common.h"
#include "sdtf_profile.h"
#include "sdtf_test.h"
#include "sdtf_crc.h"

/* SDTF CRC local defines */
#define SDTF_CRC_REF_SIGN_HIGH    (0xF2C2E9EEU)  /* Reference high 32-bit CRC signautre value */
#define SDTF_CRC_REF_SIGN_LOW     (0xEBEB19C1U)  /* Reference low 32-bit CRC signautre value */
#define SDTF_CRC_DATA_SIZE        (1843200U)

/* CRC Self Test data */
uint8_t SDTF_crcTestData[SDTF_CRC_DATA_SIZE] __attribute__ ((aligned(128))) __attribute__((section(".bss:extMemCache:ramdisk")));

/*********************************************************************
 * @fn      SDTF_runCRCSelfTest
 *
 * @brief   Execute CRC Self Test
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runCRCSelfTest(void)
{
    SDL_Result           result;
    int32_t              retVal=0;
    SDL_CRC_dataConfig_t crcData;
    uint32_t             i;
    uint32_t  *pCRCData;

    SDTF_printf("\n CRC self test: starting");

    crcData.pCRCData       = (uint32_t *)SDTF_crcTestData;
    crcData.size           = SDTF_CRC_DATA_SIZE;
    crcData.dataBitSize    = SDL_CRC_DATA_32_BIT;
    crcData.crcOpMode      = CRC_OPERATION_MODE_FULLCPU;
    crcData.refCRCValueMSW = SDTF_CRC_REF_SIGN_HIGH;
    crcData.refCRCValueLSW = SDTF_CRC_REF_SIGN_LOW;

    pCRCData = (uint32_t *)crcData.pCRCData;
    for (i = 0; i < (crcData.size / 4U); i++)
    {
        pCRCData[i] = i;
    }

    SDTF_profileBegin(SDTF_PROFILE_ONESHOT);

    result = SDL_CRC_selftest(CRC_CHANNEL_1, &crcData);

    SDTF_profileEnd(SDTF_PROFILE_ONESHOT);

    if (result != SDL_PASS)
    {
        SDTF_printf("\n CRC self test failed");
        retVal = SDL_FAIL;
    }
    else
    {
        SDTF_printf("\n CRC self test complete");
        SDTF_printf("\n Cycles taken %d", SDTF_profileDelta(SDTF_PROFILE_ONESHOT));
    }

    return retVal;
}

int32_t SDTF_runCRCNegativeTest(void)
{
    int32_t              retVal = 0;
    SDL_CRC_dataConfig_t crcData;

    SDTF_printf("\n CRC negative tests: starting");

    /* SDL_CRC_selftest Negative tests with invalid params */
    if (SDL_CRC_selftest(CRC_CHANNEL_1 - 1U, &crcData) != SDL_BADARGS)
    {
        retVal = -1;
    }

    if (retVal == 0)
    {
        if (SDL_CRC_selftest(CRC_CHANNEL_4 + 1U, &crcData) != SDL_BADARGS)
        {
            retVal = -1;
        }
    }

    if (retVal == 0)
    {
        if (SDL_CRC_selftest(CRC_CHANNEL_1, NULL_PTR) != SDL_BADARGS)
        {
            retVal = -1;
        }
    }

    if (retVal == 0)
    {
        crcData.pCRCData = NULL_PTR;
        if (SDL_CRC_selftest(CRC_CHANNEL_1, NULL_PTR) != SDL_BADARGS)
        {
            retVal = -1;
        }
    }

    if (retVal == 0)
    {
        SDTF_printf("\n CRC negative tests: success");
    }
    else
    {
        SDTF_printf("\n CRC negative tests: failed");
    }

    return retVal;
}
