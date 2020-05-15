/*
 * SDTF RAT
 *
 * SafeTI Diagnostics Library Test for RAT module
 *
 * Copyright (C) 2019 Texas Instruments Incorporated - http://www.ti.com/
 * ALL RIGHTS RESERVED
 */

#ifndef _INCLUDE_SDTF_RAT_H_
#define _INCLUDE_SDTF_RAT_H_

#include <stdint.h>
#include <ti/csl/csl_rat.h>

/*********************************************************************
 *
 * \brief Test RAT module
 *
 *
 * \return 0 if test pass, -1 in case of failure
 */
int32_t SDTF_runtestRAT(void);

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
int32_t SDTF_RATSelfTest(uint32_t regionIndex, CSL_RatTranslationCfgInfo *pTranslationCfg,
                         uint32_t *pTestMemArea);

#endif /*  _INCLUDE_SDTF_RAT_H_ */
