/*
 * SDL TEST
 *
 * SafeTI Diagnostics Library Test
 *
 * Copyright (C) 2018-2019 Texas Instruments Incorporated - http://www.ti.com/
 * ALL RIGHTS RESERVED
 */

#ifndef _INCLUDE_SDTF_REGISTERCHECK_H_
#define _INCLUDE_SDTF_REGISTERCHECK_H_

#include <stdint.h>

typedef struct SDTF_referenceRegisterSet_s {
    uint32_t *valuesPtr;
    uint32_t *maskPtr;
    uint32_t numWords;
} SDTF_referenceRegisterSet_t;

typedef struct SDTF_shadowRegisterSet_s {
    uint32_t *valuesPtr;
    uint32_t *maskPtr;
    uint32_t numWords;
} SDTF_shadowRegisterSet_t;

/******************************************************************************
 * @fn      SDTF_checkRegisterSet
 *
 * @brief   Check register set against static reference set
 *
 * @param   basePtr  Base address of the HW module
 * @param   staticReferenceRegisterSet Structure specifying reference register
 *          set     
 *
 * @return    0 : Success; < 0 for failures
 */
int32_t SDTF_checkRegisterSet(uint32_t *basePtr,
                          const SDTF_referenceRegisterSet_t *staticReferenceRegisterSet);

/******************************************************************************
 * @fn      SDTF_copyToShadowConfig
 *
 * @brief   Copy register set to shadow config
 *
 * @param   basePtr  Base address of the HW module
 * @param   shadowRegisterSet Structure specifying reference register set    
 *
 * @return  None
 */           
void SDTF_copyToShadowConfig(uint32_t *basePtr,
                            SDTF_shadowRegisterSet_t *shadowRegisterSet );

/******************************************************************************
* @fn      SDTF_updateToShadowConfig
*
* @brief   update register set in shadow config
*
* @param   basePtr  Base address of the HW module
* @param   shadowRegisterSet Structure specifying shadow register set
* @param   offsetWords Offset in workds to update from the base address  
* @param   numWords Number of words to update  
*
* @return  None
*/     
void SDTF_updateToShadowConfig(uint32_t *basePtr,
                              SDTF_shadowRegisterSet_t *shadowRegisterSet,
                              uint32_t offsetWords, uint32_t numWords);

#endif /* _INCLUDE_SDTF_REGISTERCHECK_H_ */
