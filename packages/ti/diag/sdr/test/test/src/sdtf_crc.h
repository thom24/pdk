/*
 * SDTF CRC
 *
 * SafeTI Diagnostics Library Test for CRC Module
 *
 * Copyright (C) 2019 Texas Instruments Incorporated - http://www.ti.com/
 * ALL RIGHTS RESERVED
 */

#ifndef _INCLUDE_SDTF_CRC_H_
#define _INCLUDE_SDTF_CRC_H_

#include <stdint.h>

/*********************************************************************
 * @fn      SDTF_runCRCSelfTest
 *
 * @brief   Execute CRC Self Test
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runCRCSelfTest(void);

/*********************************************************************
 * @fn      SDTF_runCRCNegativeTest
 *
 * @brief   Run Negative tests on CRC module
 *
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runCRCNegativeTest(void);

#endif /*  _INCLUDE_SDTF_CRC_H_ */
