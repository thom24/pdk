/*
 * SDTF WDT
 *
 * SafeTI Diagnostics Library Test for Watchdog module
 *
 * Copyright (C) 2019 Texas Instruments Incorporated - http://www.ti.com/
 * ALL RIGHTS RESERVED
 */

#ifndef _INCLUDE_SDTF_WDT_H_
#define _INCLUDE_SDTF_WDT_H_

#include <stdint.h>

#define SDTF_RTI_DWWD_TIMEOUT_VALUE     (10000u)
/**< DWWD Time Out Value in ms */

#define SDTF_RTI_CLOCK_SOURCE_32KHZ  (32u)
/**< Clock source */

#define SDTF_RTI_DWWDSIZE (SDL_WDT_WINDOWSIZE_50_PERCENT)

/*********************************************************************
 *
  * \brief  Initialize watchdog timer
 *
 *
 * @return  none
 */

int32_t SDTF_WDT_init(void);

/*********************************************************************
 *
  * \brief  Feed watchdog timer
 *
 *
 * @return  none
 */
void SDTF_WDT_feedWatchdogTimer(void);

/*********************************************************************
 *
 * \brief   Execute Watchdog self test for end violation
 *
 * \param   None
 *
 * \return  0 : Success; < 0 for failures
 */
int32_t SDTF_runWDTSelfTestEndViolation(void);

/*********************************************************************
 *
 * \brief   Execute Watchdog self test for window Violation
 *
 * \param   None
 *
 * \return  0 : Success; < 0 for failures
 */
int32_t SDTF_runWDTSelfTestWindowViolation(void);

/*********************************************************************
 *
 * \brief   Execute Watchdog negative tests
 *
 * \param   None
 *
 * \return  0 : Success; < 0 for failures
 */
int32_t SDTF_runWDTNegativeTests(void);

#endif /*  _INCLUDE_SDTF_WDT_H_ */
