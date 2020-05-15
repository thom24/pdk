/*
 * SDTF ECC
 *
 * SafeTI Diagnostics Library Test for ECC Module
 *
 * Copyright (C) 2019 Texas Instruments Incorporated - http://www.ti.com/
 * ALL RIGHTS RESERVED
 */

#ifndef _INCLUDE_SDTF_ECC_H_
#define _INCLUDE_SDTF_ECC_H_

#include <stdint.h>

/*********************************************************************
 * @fn      SDTF_runECC1BitInjectTest
 *
 * @brief   Execute ECC 1 bit inject test
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runECC1BitInjectTest(void);

/*********************************************************************
 * @fn      SDTF_runECC2BitInjectTest
 *
 * @brief   Execute ECC 2 bit Inject test
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runECC2BitInjectTest(void);

/*********************************************************************
 * @fn      SDTF_runECC1BitVIMRAMInjectTest
 *
 * @brief   Execute ECC 1 bit inject test on VIM RAM
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runECC1BitVIMRAMInjectTest(void);

/*********************************************************************
 * @fn      SDTF_runECC2BitVIMRAMInjectTest
 *
 * @brief   Execute ECC 2 bit Inject test with VIM RAM
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runECC2BitVIMRAMInjectTest(void);

/*********************************************************************
 * @fn      SDTF_runECC2BitVIMRAMDEDvector
 *
 * @brief   Execute ECC 2 bit Inject test with VIM RAM,but expect to
 *          trigger VIM DED vector
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runECC2BitVIMRAMDEDvector(void);

/*********************************************************************
 * @fn      SDTF_runECC1BitVIMRAMSelfTest
 *
 * @brief   Execute ECC 1 bit self test on VIM RAM
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runECC1BitVIMRAMSelfTest(void);

/*********************************************************************
 * @fn      SDTF_runECC2BitVIMRAMSelfTest
 *
 * @brief   Execute ECC 2 bit Self test with VIM RAM
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runECC2BitVIMRAMSelfTest(void);

/*********************************************************************
 * @fn      SDTF_runECC1BitSelfTest
 *
 * @brief   Execute ECC 1 bit Self test
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runECC1BitSelfTest(void);

/*********************************************************************
 * @fn      SDTF_runECC2BitSelfTest
 *
 * @brief   Execute ECC 2 bit Self Test
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runECC2BitSelfTest(void);

/*********************************************************************
 * @fn      SDTF_runECC1BitB0TCM0Bank0SelfTest
 *
 * @brief   Execute ECC single bit error test on BTCM0 Bank 0 memory
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runECC1BitB0TCM0Bank0SelfTest(void);

/*********************************************************************
 * @fn      SDTF_runECC2BitB0TCM0Bank0SelfTest
 *
 * @brief   Execute ECC Double bit error test on BTCM0 Bank 0 memory
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runECC2BitB0TCM0Bank0SelfTest(void);

/*********************************************************************
 * @fn      SDTF_runECC1BitB0TCM0Bank0SelfTest
 *
 * @brief   Execute ECC Single bit error test on BTCM0 Bank 1 memory
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runECC1BitB0TCM0Bank1SelfTest(void);

/*********************************************************************
 * @fn      SDTF_runECC2BitB0TCM0Bank0SelfTest
 *
 * @brief   Execute ECC Double bit error test on BTCM0 Bank 1 memory
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runECC2BitB0TCM0Bank1SelfTest(void);

/*********************************************************************
 * @fn      SDTF_runECC1BitB1TCM0Bank0SelfTest
 *
 * @brief   Execute ECC Double bit error test on B1TCM0 Bank 0 memory
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runECC1BitB1TCM0Bank0SelfTest(void);

/*********************************************************************
 * @fn      SDTF_runECC2BitB1TCM0Bank0SelfTest
 *
 * @brief   Execute ECC Double bit error test on B1TCM0 Bank 0 memory
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runECC2BitB1TCM0Bank0SelfTest(void);

/*********************************************************************
 * @fn      SDTF_runECC1BitB1TCM0Bank1SelfTest
 *
 * @brief   Execute ECC Single bit error test on B1TCM0 Bank 1 memory
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runECC1BitB1TCM0Bank1SelfTest(void);

/*********************************************************************
 * @fn      SDTF_runECC2BitB1TCM0Bank1SelfTest
 *
 * @brief   Execute ECC Single bit error test on B1TCM0 Bank 1 memory
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runECC2BitB1TCM0Bank1SelfTest(void);

/*********************************************************************
 *
 * @brief   Indicate DED trigger
 *
 *
 * @return  0 : Success; < 0 for failures
 */
void SDTF_ECC_indicateDEDTrigger(void);


/*********************************************************************
 *
 * @brief   Run Negative tests on ECC module
 *
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_ECC_runNegativeTests(void);

/*********************************************************************
 * @fn      SDTF_runECC2BitCodeInjectTest
 *
 * @brief   Execute ECC 2 bit Inject error or program section
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runECC2BitCodeInjectTest(void);

#endif /*  _INCLUDE_SDTF_ECC_H_ */
