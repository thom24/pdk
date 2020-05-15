/*
 * SDL TEST
 *
 * SafeTI Diagnostics Library Test
 *
 * Copyright (C) 2018-2019 Texas Instruments Incorporated - http://www.ti.com/
 * ALL RIGHTS RESERVED
 */

#ifndef _INCLUDE_SDTF_TEST_H_
#define _INCLUDE_SDTF_TEST_H_

#include <stdint.h>


#define SDTF_PROFILE_ONESHOT 0
#define SDTF_PROFILE_PERIODIC 1

#define SDTF_PROFILE_CLOCK_FREQUENCY (400000u)

extern volatile uint32_t SDTF_allTasksEnded;

typedef struct SDTF_periodic_config_s {
    uint32_t timerPeriodInms;
} SDTF_periodic_config_t;

/*********************************************************************
 * @fn      SDTF_oneShotTestAllModules
 *
  * @brief   Executes all one shot tests
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */

int32_t SDTF_oneShotTestAllModules(void);

/*********************************************************************
 * @fn      SDTF_configPeriodicTests
 *
 * @brief   Execute all the configured periodic tests
 *
 * @param   periodicConfig - configuration structure for periodic tests
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_configPeriodicTests(SDTF_periodic_config_t periodic_config);

/*********************************************************************
 * @fn      SDTF_triggerPeriodicTests
 *
 * @brief   Trigger the periodic tests
 *
 * @param   None
*
* @return  None
*/
void SDTF_triggerPeriodicTests(uintptr_t arg);

/*********************************************************************
 * @fn      SDTF_runPeriodicTests
 *
 * @brief   Execute all the configured periodic tests
 *
 * @param   None
*
* @return  0 : Success; < 0 for failures
*/
int32_t SDTF_runPeriodicTests(void);

/*********************************************************************
 * @fn      SDTF_runInteractiveTests
 *
 * @brief   Execute interactive tests
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runInteractiveTests(void);

#endif /*  _INCLUDE_SDTF_TEST_H_ */
