/*
 * SDL TEST
 *
 * SafeTI Diagnostics Library Test
 *
 * Copyright (C) 2018-2019 Texas Instruments Incorporated - http://www.ti.com/
 * ALL RIGHTS RESERVED
 */

#include <string.h>
#include "sdtf_profile.h"

#define SDTF_MAX_PROFIlE_ENTRIES 32

typedef struct SDTF_profileEntry_s {
    uint32_t profileStart;
    uint32_t profileEnd;
} SDTF_profileEntry_t;

SDTF_profileEntry_t SDTF_profileEntry[SDTF_MAX_PROFIlE_ENTRIES];

/*********************************************************************
* @fn      SDTF_profileInit
*
* @brief   Profile point init routine
*
* @param   maxNumProfiles  Max Number of profile points
*
* @return    0 : Success; < 0 for failures
*/
int32_t SDTF_profileInit(uint32_t maxNumProfiles)
{

    memset(SDTF_profileEntry, 0, sizeof(SDTF_profileEntry));

    SDTF_profileTimerInit();

    return 0;
}


/*********************************************************************
* @fn      SDTF_profileBegin
*
* @brief   Profile point begin routine
*
* @param   profileNumber  Profile number
*
* @return  None
*/
void SDTF_profileBegin(uint32_t profileNumber)
{
    SDTF_profileEntry[profileNumber].profileStart = SDTF_profileTimerRead();
}

/*********************************************************************
* @fn      SDTF_profileEnd
*
* @brief   Profile point end routine
*
* @param   profileNumber  Profile number
*
* @return  None
*/
void SDTF_profileEnd(uint32_t profileNumber)
{
    SDTF_profileEntry[profileNumber].profileEnd = SDTF_profileTimerRead();
}

/*********************************************************************
* @fn      SDTF_profileDelta
*
* @brief   calculate delta time
*
* @param   profileNumber  Profile number
*
* @return  Time delta value for profile number
*/
uint32_t SDTF_profileDelta(uint32_t profileNumber)
{
    return(SDTF_profileEntry[profileNumber].profileEnd-
           SDTF_profileEntry[profileNumber].profileStart);
}
