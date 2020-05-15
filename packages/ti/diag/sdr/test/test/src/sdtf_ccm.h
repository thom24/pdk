/*
 * SDTF CCM
 *
 * SafeTI Diagnostics Library Test for CCM Module
 *
 * Copyright (C) 2019 Texas Instruments Incorporated - http://www.ti.com/
 * ALL RIGHTS RESERVED
 */

#ifndef _INCLUDE_SDTF_CCM_H_
#define _INCLUDE_SDTF_CCM_H_

#include <stdint.h>

/*********************************************************************
 * @fn      SDTF_runCCMSelfTest
 *
 * @brief   Execute CCM Self test
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runCCMSelfTest(void);

/*********************************************************************
 * @fn      SDTF_runCCMSelfTestErrorForce
 *
 * @brief   Execute CCM Self test with error force
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runCCMSelfTestErrorForce(void);

/*********************************************************************
 * @fn      SDTF_runCCMInjectError
 *
 * @brief   Execute CCM Inject Error test
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runCCMInjectError(void);

/*********************************************************************
 * @fn      SDTF_runCCMSelftestPolarityInvert
 *
 * @brief   Execute CCM with polarity inversion
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runCCMSelftestPolarityInvert(void);

/*********************************************************************
 * @fn      SDTF_runCCMVIMSelfTest
 *
 * @brief   Execute CCM VIM Self test
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runCCMVIMSelfTest(void);

/*********************************************************************
 * @fn      SDTF_runCCMInactivitySelfTest
 *
 * @brief   Execute CCM inactivity monitor Self test
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runCCMInactivitySelfTest(void);

#endif /*  _INCLUDE_SDTF_CCM_H_ */
