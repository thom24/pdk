/*
 * SDL TEST
 *
 * SafeTI Diagnostics Library Test
 *
 * Copyright (C) 2018-2019 Texas Instruments Incorporated - http://www.ti.com/
 * ALL RIGHTS RESERVED
 */

#ifndef _INCLUDE_SDTF_INJECTERR_H_
#define _INCLUDE_SDTF_INJECTERR_H_

#include <stdint.h>
#include "sdtf_errortype.h"

#define SDTF_RAM_TEST_1BIT 0x00100000
#define SDTF_RAM_TEST_2BIT 0x00100100

/*******************************************************************************
 * @fn      SDTF_injectError
 *
 * @brief   Routine to inject error
 *
 * @param   errorType  Type of error to introduce
 *
 * @return    0 : Success; < 0 for failures
 */
int SDTF_injectError(SDTF_errorType_t errorType);

#endif /*  _INCLUDE_SDTF_INJECTERR_H_ */
