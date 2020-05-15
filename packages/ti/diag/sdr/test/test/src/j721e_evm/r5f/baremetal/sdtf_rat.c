/*
 * SDTF RAT
 *
 * Software Diagnostics Library Test for RAT module
 *
 * Copyright (C) 2019-2020 Texas Instruments Incorporated - http://www.ti.com/
 * ALL RIGHTS RESERVED
 */

#include <stdint.h>
#include <ti/csl/csl_rat.h>

#include "sdtf_common.h"
#include "sdtf_profile.h"
#include "sdtf_test.h"
#include "sdtf_rat.h"

/* Choosing unused address space */
#define SDTF_RAT_SELF_TEST_TRANSLATE_BASE  (0x10000000U)

#pragma DATA_SECTION (SDTF_RATTestArray, ".sdtf_rat_testsection");
#pragma DATA_ALIGN (SDTF_RATTestArray, 32);
uint32_t SDTF_RATTestArray[8];

#define SDTF_RAT_REGION_INDEX 0

/*********************************************************************
 *
 * \brief Test RAT module
 *
 *
 * \return 0 if test pass, -1 in case of failure
 */
int32_t SDTF_runtestRAT(void)
{
    int32_t retValue=0;
    CSL_RatTranslationCfgInfo translationCfg;

    SDTF_printf("\n RAT Test: starting");

    translationCfg.translatedAddress = ((uint64_t)&SDTF_RATTestArray);
    translationCfg.sizeInBytes = sizeof(SDTF_RATTestArray);
    translationCfg.baseAddress = (uint32_t)SDTF_RAT_SELF_TEST_TRANSLATE_BASE;

    SDTF_profileBegin(SDTF_PROFILE_ONESHOT);
    retValue = SDTF_RATSelfTest(SDTF_RAT_REGION_INDEX, &translationCfg, &SDTF_RATTestArray[0]);

    SDTF_profileEnd(SDTF_PROFILE_ONESHOT);
    if (retValue != 0 ) {
        SDTF_printf("\n RAT Test failed");
    } else {
        SDTF_printf("\n RAT Test  complete");
        SDTF_printf("\n Cycles taken %u", SDTF_profileDelta(SDTF_PROFILE_ONESHOT));
    }

    return retValue;

}
/*********************************************************************
 *
 * \brief Self test for RAT module
 * Note: This function assumes both the mapped and translated addresses are accessible directly
 * for the self test
 * Note: The values in the region may be overwritten
 *
 * \param regionIndex: RAT region index to be used for the test
 * \param pTranslateCfg: Pointer to translation configuration for
 *                       the RAT mapping
 * \param pTestMemArea: Test memory area for doing the RAT region mapping
 *
 * \return 0 if test pass, -1 in case of failure
 */
/* Note this function assumes both the mapped and translated addresses are accessible directly
 * for the self test
 * Note: The values in the region may be overwritten
 */
int32_t SDTF_RATSelfTest(uint32_t regionIndex, CSL_RatTranslationCfgInfo *pTranslationCfg,
                         uint32_t *pTestMemArea)
{
    int32_t retValue=0;
    int32_t cslStatus;
    bool result;
    CSL_ratRegs *pRatRegs = (CSL_ratRegs *)CSL_MCU_ARMSS_RAT_CFG_BASE;
    uint32_t *pMappedBase;

    /* The assumption is that the tranlated Address is accessible directly as well
     * for Self test to work.
     */
    if ((pTranslationCfg->translatedAddress > (uint64_t)0xffffffffu)
            && (pTranslationCfg->translatedAddress != (uint64_t)pTestMemArea)){
        retValue = -1;
    }

    if (retValue == 0) {
        /* Write a value to RAT region */
        pTestMemArea[0] = 0xbabeface;

        /* Set up RAT translation */
        result = CSL_ratConfigRegionTranslation(pRatRegs, regionIndex, pTranslationCfg);
        if(result == false) {
            retValue = -1;
        }
    }

    if (retValue == 0) {
        /* Verify RAT configuration */
        cslStatus = CSL_ratVerifyConfigRegionTranslation(pRatRegs, regionIndex, pTranslationCfg);
        if (cslStatus != CSL_PASS) {
            retValue = -1;
        }
    }

    if (retValue == 0) {
        /* Now write another value to test area */
        pTestMemArea[1] = 0xaaaa5555;

        pMappedBase = ((uint32_t *)(SDTF_RAT_SELF_TEST_TRANSLATE_BASE));

        /* Check the first written value */
        if (pMappedBase[0] != pTestMemArea[0]) {
            retValue = -1;
        }
    }

    if (retValue == 0) {

        /* Check second value written after RAT configuration */
        if (pMappedBase[1] != pTestMemArea[1]) {
            retValue = -1;
        }
    }

    if (retValue == 0) {

        /* Write a value to mapped region */
        pMappedBase[2] = 0x5555aaaa;
        /* Check third write to mapped area */
        if (pMappedBase[1] != pTestMemArea[1]) {
            retValue = -1;
        }
    }

    /* Disable RAT region */
    CSL_ratDisableRegionTranslation(pRatRegs, regionIndex);

    return retValue;
}
