/*
 * SDTF ESM
 *
 * SafeTI Diagnostics Library Test for ESM Module
 *
 * Copyright (C) 2019 Texas Instruments Incorporated - http://www.ti.com/
 * ALL RIGHTS RESERVED
 */

#ifndef _INCLUDE_SDTF_ESM_H_
#define _INCLUDE_SDTF_ESM_H_

#include <stdint.h>

/*********************************************************************
 * @fn      SDTF_runESMSelfTest
 *
 * @brief   Execute ESM Self test
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runESMSelfTest(void);

/*********************************************************************
 * @fn      SDTF_runESMInject
 *
 * @brief   Execute ESM Inject
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runESMInject(void);

/*********************************************************************
 * @fn      SDTF_runESMAPITest
 *
 * @brief   Execute ESM API test
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runESMAPITest(void);

/*********************************************************************
 * @fn      SDTF_runESMNegativeTest
 *
 * @brief   Execute ESM Negative tests
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runESMNegativeTest(void);

#endif /*  _INCLUDE_SDTF_ESM_H_ */
