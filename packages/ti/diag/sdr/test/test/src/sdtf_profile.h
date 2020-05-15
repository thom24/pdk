/*
 * SDL TEST
 *
 * SafeTI Diagnostics Library Test
 *
 * Copyright (C) 2018-2019 Texas Instruments Incorporated - http://www.ti.com/
 * ALL RIGHTS RESERVED
 */

#ifndef _INCLUDE_SDTF_PROFILE_H_
#define _INCLUDE_SDTF_PROFILE_H_

#include <stdint.h>

/*********************************************************************
* @fn      SDTF_profileInit
*
* @brief   Profile point init routine
*
* @param   maxNumProfiles  Max Number of profile points
*
* @return    0 : Success; < 0 for failures
*/
int32_t SDTF_profileInit(uint32_t maxNumProfiles);

/*********************************************************************
* @fn      SDTF_profileBegin
*
* @brief   Profile point begin routine
*
* @param   profileNumber  Profile number
*
* @return  None
*/
void SDTF_profileBegin(uint32_t profileNumber);

/*********************************************************************
* @fn      SDTF_profileEnd
*
* @brief   Profile point end routine
*
* @param   profileNumber  Profile number
*
* @return  None
*/    
void SDTF_profileEnd(uint32_t profileNumber);

/*********************************************************************
* @fn      SDTF_profileDelta
*
* @brief   calculate delta time
*
* @param   profileNumber  Profile number
*
* @return  Time delta value for profile number
*/
uint32_t SDTF_profileDelta(uint32_t profileNumber);

extern void SDTF_profileTimerInit(void);
extern inline uint32_t SDTF_profileTimerRead(void);

#endif /* _INCLUDE_SDTF_PROFILE_H_ */
