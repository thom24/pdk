/*
 * SDTF MPU
 *
 * SafeTI Diagnostics Library Test for MPU Module
 *
 * Copyright (C) 2019 Texas Instruments Incorporated - http://www.ti.com/
 * ALL RIGHTS RESERVED
 */

#ifndef _INCLUDE_SDTF_MPU_H_
#define _INCLUDE_SDTF_MPU_H_

#include <stdint.h>

/*********************************************************************
 * @fn      SDTF_runMPUSelfTest
 *
 * @brief   Execute MPU Self test
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runMPUSelfTest(void);
#endif /*  _INCLUDE_SDTF_MPU_H_ */
