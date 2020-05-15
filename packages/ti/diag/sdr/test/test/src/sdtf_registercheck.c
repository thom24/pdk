/*
 * SDL TEST
 *
 * SafeTI Diagnostics Library Test
 *
 * Copyright (C) 2018-2019 Texas Instruments Incorporated - http://www.ti.com/
 * ALL RIGHTS RESERVED
 */

#include "sdtf_registercheck.h"

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
                          const SDTF_referenceRegisterSet_t *staticReferenceRegisterSet)
{
    int32_t i;

    for( i=0; i< staticReferenceRegisterSet->numWords; i++) {
        if( (staticReferenceRegisterSet->valuesPtr[i]
             & staticReferenceRegisterSet->maskPtr[i]) !=
            (basePtr[i] & staticReferenceRegisterSet->maskPtr[i])) {
            return -1;
        }
    }
    return 0;
}

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
                            SDTF_shadowRegisterSet_t *shadowRegisterSet )
{
    int32_t i;

    /* Copy register set to shadow copy */
    for( i=0; i< shadowRegisterSet->numWords; i++) {
        shadowRegisterSet->valuesPtr[i] = basePtr[i];
    }
    return;
}

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
                              uint32_t offsetWords, uint32_t numWords)
{
    int32_t i;

    /*Updates selective section of the register set */
    /* Copy register set to shadow copy */
    for( i=0; i< numWords; i++) {
        shadowRegisterSet->valuesPtr[i] = basePtr[offsetWords + i];
    }
    return;
}
