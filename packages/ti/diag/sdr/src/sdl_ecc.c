/*
 * SDL ECC
 *
 * Software Diagnostics Library module for ECC
 *
 *  Copyright (c) Texas Instruments Incorporated 2018-2020
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include <string.h>
#include <stdint.h>
#include <sdl_common.h>
#include <sdl_ecc.h>
#include <sdl_esm.h>
#include <sdl_exception.h>


#include <ti/csl/csl_ecc_aggr.h>

#include "sdl_utils.h"
#include "sdl_ecc_utils.h"

#include "sdl_ecc_soc.h"
#include "sdl_ecc_core.h"

#include "sdl_ecc_priv.h"

/* Local defines */
#define SDL_ECC_INVALID_ERROR_SOURCE (0xffffffffu)

#define BITS_PER_WORD (32u)

#define ECC_AGGR_LINE_SIZE (4)

/** ---------------------------------------------------------------------------
 * @brief This enumerator defines the values for ecc self test flag
 * ----------------------------------------------------------------------------
 */
typedef enum {
    SDL_ECC_ERROR_FLAG_NONE=0,
    /**< Flag set during ECC initialization or end of the ECC self test */
    SDL_ECC_ERROR_FLAG_STARTING=1,
    /**< Flag set at the start of the ECC self test */
    SDL_ECC_ERROR_FLAG_TRIGGERED=2,
    /**< Flag set when ECC error happens during the ECC self test */

} SDL_ECC_ErrorFlag;

/** ---------------------------------------------------------------------------
 * @brief This structure defines the elements of ECC software instance
 * ----------------------------------------------------------------------------
 */
typedef struct SDL_ECC_Instance_s
{
   SDL_ECC_InitConfig_t eccInitConfig;
   /**< ecc Initial configuration */
   SDL_ECC_ErrorFlag eccErrorFlag;
   /**< Ecc error triggered flag */
   SDL_ECC_InjectErrorType eccSelfTestErrorType;
   /**< Ecc self type error type in progress */
   uint32_t eccSelfTestRamId;
   /**< Ram id used in self test in progress */
   uint32_t *eccSelfTestAddr;
   /**< Address used in self test in progress */
}  SDL_ECC_Instance_t;

/* Global objects */
static SDL_ECC_Instance_t SDL_ECC_instance[SDL_ECC_AGGREGATOR_MAX_ENTRIES];

extern const SDL_RAMIdEntry_t SDL_ECC_mcuArmssRamIdTable[SDL_PULSAR_CPU_RAM_ID_TABLE_MAX_ENTRIES];

/* Local functions */
static SDL_Result SDL_ECC_getRamId(SDL_ECC_MemType eccMemType, SDL_ECC_MemSubType memSubType,
                           uint32_t *ramIdP);
static SDL_Result SDL_ECC_getAggregatorType(SDL_ECC_MemType eccMemType, SDL_ECC_MemSubType memSubType,
    bool *pIinjectOnly);
static SDL_Result SDL_ECC_memoryRefresh(uint32_t *memAddr, size_t size);
static void SDL_ECC_triggerAccessForEcc(const uint32_t *memoryAccessAddr);

static SDL_Result SDL_ECC_getMemConfig(SDL_ECC_MemType eccMemType, SDL_ECC_MemSubType memSubType,
                               SDL_MemConfig_t *memConfig);
static uint32_t SDL_ECC_getDetectErrorSource (SDL_ECC_InjectErrorType injectErorType);
static SDL_Result SDL_ECC_getBitLocation(uint32_t bitMask,
                            uint32_t startBitLocation,
                            uint32_t *bitLocation);
static void SDL_ECC_handleEccAggrEvent (SDL_ECC_MemType eccMemType, uint32_t errorSrc,
                                       uint32_t errorAddr);
static void SDL_ECC_ESMCallBackFunction (uint32_t errorSrc, uint32_t errorAddr);
static SDL_Result SDL_ECC_checkMemoryType(SDL_ECC_MemType eccMemType, SDL_ECC_MemSubType memSubType);

/** ============================================================================*
 *
 * \brief   Get the Error Source corresponding to the inject error
 *          type
 *
 *   @n  This function is called in ECC callback function to convert the injected
 *       error type to the source of the ECC error
 *
 * \param1  errorType: error Type
 *
 * \return  error Source or SDL_ECC_INVALID_ERROR_SOURCE in case of error
 */
static uint32_t SDL_ECC_getDetectErrorSource (SDL_ECC_InjectErrorType injectErorType)
{
    uint32_t errorSource;

    switch(injectErorType) {
        case SDL_INJECT_ECC_ERROR_FORCING_1BIT_ONCE:
        case SDL_INJECT_ECC_ERROR_FORCING_1BIT_N_ROW_ONCE:
        case SDL_INJECT_ECC_ERROR_FORCING_1BIT_REPEAT:
        case SDL_INJECT_ECC_ERROR_FORCING_1BIT_N_ROW_REPEAT:
            errorSource = SDL_ESM_ECC_PARAM_MCU_CPU0_SEC_ERROR;
            break;


        case SDL_INJECT_ECC_ERROR_FORCING_2BIT_ONCE:
        case SDL_INJECT_ECC_ERROR_FORCING_2BIT_N_ROW_ONCE:
        case SDL_INJECT_ECC_ERROR_FORCING_2BIT_REPEAT:
        case SDL_INJECT_ECC_ERROR_FORCING_2BIT_N_ROW_REPEAT:
            errorSource = SDL_ESM_ECC_PARAM_MCU_CPU0_DED_ERROR;
            break;

        default:
            errorSource = ((uint32_t)SDL_ECC_INVALID_ERROR_SOURCE);
            break;
    }
    return errorSource;
}

/** ============================================================================*
 *
 * \brief   Ecc call back function registered with exception handler
 *
 * \param1  eccMemType: ECC Memory Type
 * \param2  errorSrc: Source of ECC Error
 * \param3  errorAddr: ECC error address
 *
 * \return  None
 */
static void SDL_ECC_handleEccAggrEvent (SDL_ECC_MemType eccMemType, uint32_t errorSrc,
                                       uint32_t errorAddr)
{
    CSL_ecc_aggrRegs *eccAggrRegs;
    uint32_t ramId=0;
    SDL_Result retVal;
    SDL_ECC_MemSubType memSubType;
    uint32_t i;
    bool eventFound = ((bool)false);
    bool eventFound1;
    int32_t cslResult;

    eccAggrRegs = (SDL_ECC_aggrBaseAddressTable[eccMemType]);

    /* Check which Ram Id triggered the error */
    for (i = ((uint32_t)0U);
         i < SDL_ECC_instance[eccMemType].eccInitConfig.numRams;
         i++) {
        /* Get corresponding ram Id */
        memSubType = SDL_ECC_instance[eccMemType].eccInitConfig.pMemSubTypeList[i];
        retVal = SDL_ECC_getRamId(eccMemType, memSubType, &ramId);
        if (retVal != SDL_PASS) {
            /* Unexpected FAILURE: call Assert */
            SDL_assertExternalFunction(SDL_ECC_RAM_ID_NOT_FOUND);
            continue;
        }

        /* Check if this event in triggered, by reading the ECC aggregator status register */
        cslResult = CSL_ecc_aggrIsEccRamIntrPending(eccAggrRegs, ramId, errorSrc, &eventFound1);
        if((cslResult == CSL_PASS)  && (eventFound1)) {
            eventFound = ((bool)true);
            /* Clear the event */
            (void)CSL_ecc_aggrClrEccRamIntrPending(eccAggrRegs, ramId, errorSrc);
            /* If it matches self test set flag */
            if ((errorSrc
                 == SDL_ECC_getDetectErrorSource(SDL_ECC_instance[eccMemType].eccSelfTestErrorType))
                 && (ramId == SDL_ECC_instance[eccMemType].eccSelfTestRamId)) {

                 SDL_ECC_instance[eccMemType].eccErrorFlag = SDL_ECC_ERROR_FLAG_TRIGGERED;
            } else {
                /* Call application callback function to indicate error */
                SDL_ECC_applicationCallbackFunction(errorSrc, errorAddr);
            }
        }
    }

    /* Ack Ecc aggregator error */
    cslResult = CSL_ecc_aggrAckIntr(eccAggrRegs, CSL_ECC_AGGR_INTR_SRC_SINGLE_BIT);
    if ((cslResult != CSL_PASS) || (!eventFound)) {
        /* Unexpected event; Call assert */
        SDL_assertExternalFunction(SDL_ECC_INTERRUPT_WITH_NOEVENT);
    }
    return;
}

/** ============================================================================*
 *
 * \brief   Ecc call back function registered with exception handler
 *          Certain ECC events on CPU memory are reported as Exception events
 *          And this function provides a way to intercept the event to do self test
 *
 * \param1  errorSrc: source of the ECC error reported.
 * \param2  errorAddr: Error address at which the actual error happened
 *
 * \return  None
 */
static void SDL_ECC_callBackFunction (uint32_t errorSrc, uint32_t errorAddr)
{
    SDL_ECC_MemType eccMemType;


    /* Check Error Source and call acknowledge appropriate interrupt */
    switch (errorSrc) {
        case SDL_ESM_ECC_PARAM_MCU_CPU0_SEC_ERROR:
        case SDL_ESM_ECC_PARAM_MCU_CPU0_DED_ERROR:
        case SDL_ESM_ECC_PARAM_MCU_CPU1_SEC_ERROR:
        case SDL_ESM_ECC_PARAM_MCU_CPU1_DED_ERROR:
            /* Get corresponding Memory type */
            switch(errorSrc)
            {
                default:
                case SDL_ESM_ECC_PARAM_MCU_CPU0_SEC_ERROR:
                case SDL_ESM_ECC_PARAM_MCU_CPU0_DED_ERROR:
                    eccMemType = SDL_ECC_MEMTYPE_MCU_R5F0_CORE;
                    break;

                case SDL_ESM_ECC_PARAM_MCU_CPU1_SEC_ERROR:
                case SDL_ESM_ECC_PARAM_MCU_CPU1_DED_ERROR:
                    eccMemType = SDL_ECC_MEMTYPE_MCU_R5F1_CORE;
                    break;

            }

            /* If self test in progress, indicate triggered flag */
            if ((errorSrc
                 == SDL_ECC_getDetectErrorSource(SDL_ECC_instance[eccMemType].eccSelfTestErrorType))
                && ((errorAddr == ((uint32_t)SDL_ESM_ERRORADDR_INVALID))
                || (((uintptr_t)errorAddr)
                    == ((uintptr_t)SDL_ECC_instance[eccMemType].eccSelfTestAddr)))
                && (SDL_ECC_instance[eccMemType].eccErrorFlag == SDL_ECC_ERROR_FLAG_STARTING)) {
                SDL_ECC_instance[eccMemType].eccErrorFlag = SDL_ECC_ERROR_FLAG_TRIGGERED;
            } else {
                /* Execute call back */
                SDL_ECC_applicationCallbackFunction(errorSrc, errorAddr);
            }

            break;

        default:
            break;
    }


}


/** ============================================================================*
 *
 * \brief   Ecc call back function registered with ESM handler
 *
 * \param1  errorSrc: Error source
 *
 * \return  None
 */
static void SDL_ECC_ESMCallBackFunction (uint32_t errorSrc, uint32_t errorAddr)
{
    SDL_ECC_MemType eccMemType;


    /* Check Error Source and call acknowledge appropriate interrupt */
    switch (errorSrc) {
        case SDL_ESM_ECC_PARAM_MCU_CPU0_SEC_ERROR:
        case SDL_ESM_ECC_PARAM_MCU_CPU0_DED_ERROR:
        case SDL_ESM_ECC_PARAM_MCU_CPU1_SEC_ERROR:
        case SDL_ESM_ECC_PARAM_MCU_CPU1_DED_ERROR:
            /* Get corresponding Memory type */
            switch(errorSrc)
            {
                default:
                case SDL_ESM_ECC_PARAM_MCU_CPU0_SEC_ERROR:
                case SDL_ESM_ECC_PARAM_MCU_CPU0_DED_ERROR:
                    eccMemType = SDL_ECC_MEMTYPE_MCU_R5F0_CORE;
                    break;

                case SDL_ESM_ECC_PARAM_MCU_CPU1_SEC_ERROR:
                case SDL_ESM_ECC_PARAM_MCU_CPU1_DED_ERROR:
                    eccMemType = SDL_ECC_MEMTYPE_MCU_R5F1_CORE;
                    break;

            }

            /* Handle ECC Aggregator event */
            SDL_ECC_handleEccAggrEvent(eccMemType, errorSrc, errorAddr);

            break;

        default:
            break;
    }


}
/** ============================================================================*
 *
 * Requirement: REQ_TAG(PRSDK-2577) REQ_TAG(PRSDK-1671) REQ_TAG(PRSDK-1705) REQ_TAG(PRSDK-1706) REQ_TAG(PRSDK-3240)
 * Design: did_software_diagnostics_reference did_ecc_diagnostics did_ecc_vim_ram
 *
 * \brief   Initializes ECC module for ECC detection
 *
 * \param1 eccAggrInstNumber Instance number of ECC aggregator
 * \param2 pECCInitConfig     Pointer to Ecc init configuration
 *
 * \return  SDL_PASS : Success; SDL_FAIL for failures
 *          NOTE: On failure the ECC peripheral registers may be initialized
 *          partially.
 */
SDL_Result SDL_ECC_init (SDL_ECC_MemType eccMemType,
                         const SDL_ECC_InitConfig_t *pECCInitConfig)
{
    CSL_ecc_aggrRegs *eccAggrRegs;
    uint32_t         numMemRegions;
    SDL_Result retVal = SDL_PASS;
    uint32_t ramId;
    uint32_t i;
    int32_t cslResult;
    SDL_ECC_MemSubType memSubType;
    bool injectOnlyFlag;

    if (pECCInitConfig == NULL) {
        retVal = SDL_BADARGS;
    }

    if (retVal == SDL_PASS) {

        eccAggrRegs = (SDL_ECC_aggrBaseAddressTable[eccMemType]);

        /* Disable all interrupts to start clean */
        (void)CSL_ecc_aggrDisableAllIntrs(eccAggrRegs);

        /* Get the number of RAMs */
        cslResult = CSL_ecc_aggrGetNumRams(eccAggrRegs, &numMemRegions);
        if ((cslResult != CSL_PASS) || (numMemRegions == 0U)) {
            retVal = SDL_FAIL;
        }
    }

    if (retVal == SDL_PASS) {
        /* Record the Init configuration */
        SDL_ECC_instance[eccMemType].eccInitConfig = *pECCInitConfig;

        /* Enable the single bit ECC interrupts */
        /* Note: The following statement enables interrupts for all RAMs */
        cslResult = CSL_ecc_aggrEnableIntrs(eccAggrRegs,
                                CSL_ECC_AGGR_INTR_SRC_SINGLE_BIT);
        if (cslResult != CSL_PASS) {
            retVal = SDL_FAIL;
        }
    }

    if (retVal == SDL_PASS) {
            /* Enable the Double bit ECC Interrupts */
            cslResult = CSL_ecc_aggrEnableIntrs(eccAggrRegs,
                                CSL_ECC_AGGR_INTR_SRC_DOUBLE_BIT);
            if (cslResult != CSL_PASS) {
                retVal = SDL_FAIL;
            }
    }
    if (retVal == SDL_PASS) {

        /* Register error interrupt call back function */
        /* No need to check return value , as it is just based on null check*/
        (void)SDL_ESM_registerECCHandler(&SDL_ECC_ESMCallBackFunction);
        /* currently as this is not reported by ESM, register with
           exception handler */
        SDL_EXCEPTION_registerECCHandler(&SDL_ECC_callBackFunction);

        /* Enable ECC */
        for ( i = ((uint32_t)0U); i < pECCInitConfig->numRams; i++) {

            /* Get memory Sub type to be configured */
             memSubType = pECCInitConfig->pMemSubTypeList[i];

             /* Get the corresponding ram Id */
             retVal = SDL_ECC_getRamId(eccMemType, memSubType, &ramId);
             if (retVal == SDL_PASS) {

                 /* Get the corresponding ram Id */
                 retVal = SDL_ECC_getAggregatorType(eccMemType, memSubType, &injectOnlyFlag);
             }
             if (retVal == SDL_PASS) {
                 if (injectOnlyFlag) {
                     /* Call CSL APIs to enable ECC, specific to the module */
                     retVal = SDL_ECC_configECCRam(ramId);
                     if (retVal != SDL_PASS) {
                         retVal = SDL_FAIL;
                     }
                 } else {

                     /* Enables ECC, ecc checkmreg, rmw parity errors */
                     cslResult = CSL_ecc_aggrConfigEccRam(eccAggrRegs,
                                              ramId, (bool)true, (bool)true, (bool)true);
                     if (cslResult != CSL_PASS) {
                         retVal = SDL_FAIL;
                     }
                 }
             }
             if (retVal != SDL_PASS) {
                 break;
             }
        }

        /* Initialize object for self test */
        SDL_ECC_instance[eccMemType].eccErrorFlag = SDL_ECC_ERROR_FLAG_NONE;
        SDL_ECC_instance[eccMemType].eccSelfTestErrorType = SDL_INJECT_ECC_NO_ERROR;
        SDL_ECC_instance[eccMemType].eccSelfTestRamId = ((uint32_t)0u);
        SDL_ECC_instance[eccMemType].eccSelfTestAddr = NULL;
    }

    return retVal;
}

/** ============================================================================*
 *
 * \brief   Refresh memory to make sure ECC is generated
 *
 *
 * \param1  memAddr: Address to refresh ( Need to aligned to 32 bit)
 * \param2  size: Size of memory region to refresh
 *                Should be multiple of 4
 *
 * \return  SDL_PASS : Success; SDL_FAIL for failures

 */
static SDL_Result SDL_ECC_memoryRefresh(uint32_t *memAddr, size_t size)
{
    uint32_t i;
    SDL_Result result = SDL_PASS;
    volatile uint32_t tmpValue;

    /* Simply read and copy back data */
    for (i = ((uint32_t)0u); i < (size>>2u); i++) {
        tmpValue = memAddr[i];
        memAddr[i] = tmpValue;
    }
    return result;
}

/** ============================================================================
 *
 * Requirement: REQ_TAG(PRSDK-2577) REQ_TAG(PRSDK-1709)
 * Design: did_software_diagnostics_reference did_ecc_diagnostics
 *
 * \brief   Initializes Memory to be ready for ECC error detection.
 *          Assumes ECC is already enabled.
 *
 * \param  eccMemType ECC memory type
 * \param  memSubType: Memory subtype
 *
 * \return  SDL_PASS : Success; SDL_FAIL for failures
 */
SDL_Result SDL_ECC_initMemory (SDL_ECC_MemType eccMemType,
                               SDL_ECC_MemSubType memSubType)
{
    SDL_Result result=SDL_PASS;
    SDL_MemConfig_t memConfig;

    /* Get memory configuration */
    result = SDL_ECC_getMemConfig(eccMemType, memSubType, &memConfig);
    if (result == SDL_PASS) {

        /* Initialize only if readable */
        if (memConfig.readable) {
            /* Initialised the whole memory so that ECC is updated */
            result = SDL_ECC_memoryRefresh((uint32_t *)memConfig.memStartAddr,
                                       (size_t)memConfig.size);
        }
    }

    return result;
}

/** ============================================================================
 *
 * \brief   Does access to trigger ECC
 *
 * \param  pMemoryAccessAddr: Memory Access Address
 *
 * @return  None

 */
static void SDL_ECC_triggerAccessForEcc(const uint32_t *pMemoryAccessAddr)
{
    volatile uint32_t testLocationValue;


   /* Read value to trigger ECC error injection */
   testLocationValue = *(pMemoryAccessAddr);

}

/** ============================================================================
 *
 * Requirement: REQ_TAG(PRSDK-2577) REQ_TAG(PRSDK-1709) REQ_TAG(PRSDK-1671) REQ_TAG(PRSDK-1705) REQ_TAG(PRSDK-1706) REQ_TAG(PRSDK-3240)
 * Design: did_software_diagnostics_reference did_ecc_diagnostics did_ecc_vim_ram
 *
 * \brief   Runs self test by injecting and error and monitor response
 *          Assumes ECC is already enabled.
 *
 * \param1  eccMemType ECC memory type
 * \param2  memSubType: Memory subtype
 * \param3  errorType: ECC Self test type
 * \param4  pECCErrorConfig: Memory configuration self test area
 * \param4  selfTestTimeOut: Number of retries before time out
 *
 * \return  SDL_PASS : Success; SDL_FAIL for failures
 */
SDL_Result SDL_ECC_selfTest(SDL_ECC_MemType eccMemType,
                                  SDL_ECC_MemSubType memSubType,
                                  SDL_ECC_InjectErrorType errorType,
                                  const SDL_ECC_InjectErrorConfig_t *pECCErrorConfig,
                                  uint32_t selfTestTimeOut)
{
    SDL_Result retVal = SDL_PASS;
    uint32_t timeCount = 0;
    uint32_t testLocationPreserve;
    uint32_t ramId;
    uint32_t retVal2;
    uint32_t *testLocationAddress;
    SDL_MemConfig_t memConfig;

    if (pECCErrorConfig == NULL) {
        retVal = SDL_BADARGS;
    }

    if (retVal == SDL_PASS) {
        /* Configure error configuration based on Test type */
        /* Get Ram Id */
        retVal = SDL_ECC_getRamId(eccMemType, memSubType,
                                 &ramId);
    }

    if (retVal == SDL_PASS) {

        /* Get memory configuration */
        retVal = SDL_ECC_getMemConfig(eccMemType, memSubType, &memConfig);
    }

    if (retVal == SDL_PASS) {

        /* Get actual location address for the memory */
        testLocationAddress = pECCErrorConfig->pErrMem;

        if (memConfig.readable) {
            /* Store test location value */
            testLocationPreserve = *(testLocationAddress);
        }

        /* Set self test error flag */
        SDL_ECC_instance[eccMemType].eccErrorFlag = SDL_ECC_ERROR_FLAG_STARTING;
        SDL_ECC_instance[eccMemType].eccSelfTestErrorType = errorType;
        SDL_ECC_instance[eccMemType].eccSelfTestRamId = ramId;
        SDL_ECC_instance[eccMemType].eccSelfTestAddr = testLocationAddress;

        /* Enable PMU to monitor event about to be triggerred */
        SDL_ECC_enableECCEventCheck(eccMemType, memSubType, errorType);

        /* Inject error */
        retVal = SDL_ECC_injectError(eccMemType,
                                    memSubType, errorType,
                                    pECCErrorConfig);
    }

    if (retVal == SDL_PASS) {

        if (memConfig.readable) {
            /* Trigger access for ECC error injection to complete */
            SDL_ECC_triggerAccessForEcc(testLocationAddress);
        }

        /* Wait for error to take effect */
        while((SDL_ECC_instance[eccMemType].eccErrorFlag != SDL_ECC_ERROR_FLAG_TRIGGERED)
              && (timeCount < selfTestTimeOut)) {

            /* In cases there are no interrupts for the ECC event poll directly */
            retVal2 = SDL_ECC_pollErrorEvent(eccMemType, memSubType, errorType);
            if (retVal2 == SDL_ECC_EVENT_FOUND) {
                SDL_ECC_instance[eccMemType].eccErrorFlag = SDL_ECC_ERROR_FLAG_TRIGGERED;
            }

            /* Increment timeout counter */
            timeCount++;
        }

        /* Check expected error occurred or timeout */
        if ((SDL_ECC_instance[eccMemType].eccErrorFlag != SDL_ECC_ERROR_FLAG_TRIGGERED)) {
            retVal = SDL_FAIL;
        } else {

            if (memConfig.readable) {
                /* correct error injected if not autocorrect by hardware */
                *(testLocationAddress) = testLocationPreserve;
            }
        }

    }
    /* Disable any ECC event check */
    SDL_ECC_disableECCEventCheck(eccMemType, errorType);

    /* Reset self test error flag */
    SDL_ECC_instance[eccMemType].eccErrorFlag = SDL_ECC_ERROR_FLAG_NONE;

    return retVal;
}

/** ============================================================================
 *
 * \brief   Get bit location in word.
 *
 * \param1  bitMask: Input bitmask
 * \param2  startBitLocation: Bit location to start from
 * \param3  pPbitLocation: Pointer to Next location of 1b found
 *
 * \return  SDL_PASS : Success; SDL_FAIL for failures
 */
static SDL_Result SDL_ECC_getBitLocation(uint32_t bitMask,
                                         uint32_t startBitLocation,
                                         uint32_t *pPbitLocation)
{
    SDL_Result result = SDL_PASS;
    uint32_t bitCount;

    if (startBitLocation >= BITS_PER_WORD) {
        result = SDL_FAIL;
    } else {
        /* Find first bit error for single bit */
         for (bitCount=startBitLocation; bitCount < BITS_PER_WORD; bitCount++) {
             if ((bitMask
                 & (((uint32_t)1u) << bitCount)) != 0u  ) {
                 *pPbitLocation = bitCount;
                 break;
             }
         }
         if ( bitCount >= BITS_PER_WORD) {
             result = SDL_FAIL;
         }
    }
    return result;
}

/** ============================================================================
 *
 * Requirement: REQ_TAG(PRSDK-2577) REQ_TAG(PRSDK-1709) REQ_TAG(PRSDK-1705) REQ_TAG(PRSDK-1706) REQ_TAG(PRSDK-3240)
 * Design: did_software_diagnostics_reference did_ecc_diagnostics did_ecc_vim_ram
 *
 * \brief   Injects ECC error at specified location
 *          Assumes ECC is already enabled.
 *
 * \param1  memType: Memory type for self test
 * \param2  memSubType: Memory subtype
 * \param3  errorType: ECC error type
 * \param4  pECCErrorConfig: Pointer to Error configuration
 *
 * \return  SDL_PASS : Success; SDL_FAIL for failures
 */
SDL_Result SDL_ECC_injectError(SDL_ECC_MemType eccMemType,
                              SDL_ECC_MemSubType memSubType,
                              SDL_ECC_InjectErrorType errorType,
                              const SDL_ECC_InjectErrorConfig_t *pECCErrorConfig)
{
    uint32_t regValue;
    volatile uint32_t regValue2;
    uint32_t firstBitLocation, secondBitLocation;
    uint32_t errAddrOffset;
    CSL_ecc_aggrRegs *eccAggrRegs;
    uint32_t ramId;
    SDL_Result retVal = SDL_PASS;
    int32_t cslRetval;
    SDL_MemConfig_t memConfig;

    if (pECCErrorConfig == NULL) {
        retVal = SDL_BADARGS;
    }

    if (retVal == SDL_PASS) {

        eccAggrRegs = (SDL_ECC_aggrBaseAddressTable[eccMemType]);

        /* Get Ram Id */
        retVal = SDL_ECC_getRamId(eccMemType, memSubType,
                                 &ramId);
    }

    if (retVal == SDL_PASS) {
        /* Get memory configuration */
        retVal = SDL_ECC_getMemConfig(eccMemType, memSubType, &memConfig);
    }

    if (retVal == SDL_PASS) {
        if ( ((uintptr_t)pECCErrorConfig->pErrMem) < memConfig.memStartAddr) {
            retVal = SDL_FAIL;
        } else {
            /* Calculate error offset */
            errAddrOffset =  ((uintptr_t)pECCErrorConfig->pErrMem - memConfig.memStartAddr)
                            / (memConfig.stride);
        }
     }

    if (retVal == SDL_PASS) {
        /* Set error Address */
        cslRetval = CSL_ecc_aggrWriteEccRamErrCtrlReg(eccAggrRegs,
                                                      ramId, 0u,
                                                      errAddrOffset);
        if (cslRetval != CSL_PASS) {
            retVal = SDL_FAIL;
        }
    }

    if (retVal == SDL_PASS) {

        /* Read ECC Ram Control Register */
        cslRetval = CSL_ecc_aggrReadEccRamCtrlReg(eccAggrRegs,
                                                 ramId, &regValue);
        if (cslRetval != CSL_PASS) {
            retVal = SDL_FAIL;
        }
    }

    if (retVal == SDL_PASS) {

        switch (errorType) {
             case SDL_INJECT_ECC_ERROR_FORCING_1BIT_ONCE:
             case SDL_INJECT_ECC_ERROR_FORCING_1BIT_REPEAT:
             case SDL_INJECT_ECC_ERROR_FORCING_1BIT_N_ROW_ONCE:
             case SDL_INJECT_ECC_ERROR_FORCING_1BIT_N_ROW_REPEAT:
                 /* Get bit location */
                 retVal = SDL_ECC_getBitLocation(pECCErrorConfig->flipBitMask,
                                                 0u, &firstBitLocation);
                 if (retVal != SDL_PASS) {
                     break;
                 }

                 /* Write bit error configuration for single bit */
                 cslRetval = CSL_ecc_aggrWriteEccRamErrCtrlReg(eccAggrRegs,
                                                   ramId, 1, firstBitLocation);
                 if (cslRetval != CSL_PASS) {
                     retVal = SDL_FAIL;
                     break;
                 }
                 if ((errorType == SDL_INJECT_ECC_ERROR_FORCING_1BIT_ONCE) ||
                     (errorType == SDL_INJECT_ECC_ERROR_FORCING_1BIT_N_ROW_ONCE)){
                     /* Configure settings for inject error once  */
                     regValue |= CSL_ECC_RAM_CTRL_ERROR_ONCE_MASK;
                 } else {
                     /* Configure settings for inject error repeat */
                     regValue = (regValue & (~CSL_ECC_RAM_CTRL_ERROR_ONCE_MASK));
                 }
                 if ((errorType == SDL_INJECT_ECC_ERROR_FORCING_1BIT_ONCE) ||
                     (errorType == SDL_INJECT_ECC_ERROR_FORCING_1BIT_REPEAT)){
                     /* Configure settings for single bit error, specific row */
                     regValue = (regValue
                                 & (~(CSL_ECC_RAM_CTRL_FORCE_N_ROW_MASK | CSL_ECC_RAM_CTRL_FORCE_DED_MASK)))
                                | CSL_ECC_RAM_CTRL_FORCE_SEC_MASK;
                 } else {
                     /* Configure settings for Single bit error N Row */
                     regValue = (regValue
                                 & (~CSL_ECC_RAM_CTRL_FORCE_DED_MASK))
                                | CSL_ECC_RAM_CTRL_FORCE_SEC_MASK
                                | CSL_ECC_RAM_CTRL_FORCE_N_ROW_MASK;
                 }
                 break;

             case SDL_INJECT_ECC_ERROR_FORCING_2BIT_ONCE:
             case SDL_INJECT_ECC_ERROR_FORCING_2BIT_REPEAT:
             case SDL_INJECT_ECC_ERROR_FORCING_2BIT_N_ROW_ONCE:
             case SDL_INJECT_ECC_ERROR_FORCING_2BIT_N_ROW_REPEAT:
                 /* Get bit location */
                 retVal = SDL_ECC_getBitLocation(pECCErrorConfig->flipBitMask,
                                             0, &firstBitLocation);
                 if (retVal != SDL_PASS) {
                     break;
                 }

                 /* Get Second bit location */
                 retVal = SDL_ECC_getBitLocation(pECCErrorConfig->flipBitMask,
                                             firstBitLocation+1u, &secondBitLocation);
                 if (retVal != SDL_PASS) {
                     break;
                 }

                 /* Record double bit error position */
                 regValue2 = firstBitLocation | (secondBitLocation << 16);

                 /* Set bit error configuration settings to register */
                 cslRetval = CSL_ecc_aggrWriteEccRamErrCtrlReg(eccAggrRegs,
                                           ramId, 1, regValue2);
                 if (cslRetval != CSL_PASS) {
                     retVal = SDL_FAIL;
                     break;
                 }
                 if ((errorType == SDL_INJECT_ECC_ERROR_FORCING_2BIT_ONCE) ||
                         (errorType == SDL_INJECT_ECC_ERROR_FORCING_2BIT_N_ROW_ONCE)){
                     /* Configure settings for Double bit error */
                     regValue |= CSL_ECC_RAM_CTRL_ERROR_ONCE_MASK;
                 } else {
                     /* Configure settings for single bit error */
                     regValue = (regValue & (~CSL_ECC_RAM_CTRL_ERROR_ONCE_MASK));
                 }
                 if ((errorType == SDL_INJECT_ECC_ERROR_FORCING_2BIT_ONCE) ||
                     (errorType == SDL_INJECT_ECC_ERROR_FORCING_2BIT_REPEAT)) {
                     /* Configure settings for double bit error, specific row */
                     regValue = (regValue
                                 & (~(CSL_ECC_RAM_CTRL_FORCE_N_ROW_MASK+CSL_ECC_RAM_CTRL_FORCE_SEC_MASK)))
                                | CSL_ECC_RAM_CTRL_FORCE_DED_MASK;
                 } else {
                     /* Configure settings for Double bit error N Row*/
                     regValue = (regValue
                                 & (~CSL_ECC_RAM_CTRL_FORCE_SEC_MASK))
                                | CSL_ECC_RAM_CTRL_FORCE_DED_MASK
                                | CSL_ECC_RAM_CTRL_FORCE_N_ROW_MASK;
                 }
                 break;

             default:
                 break;
         }
    }

    if (retVal == SDL_PASS) {
        /* Write bit error configuration to register */
        cslRetval = CSL_ecc_aggrWriteEccRamCtrlReg(eccAggrRegs,
                                                   ramId, regValue);
        if (cslRetval != CSL_PASS) {
            retVal = SDL_FAIL;
        }
    }

    if (retVal == SDL_PASS) {
        /* Just read back ctrl register to confirm write */
        /* NOTE: The read value may not be same as what is written as some fields
         * in the register are not writable or can self clear
         */
        cslRetval = CSL_ecc_aggrReadEccRamCtrlReg(eccAggrRegs,
                                            ramId,
                                            (uint32_t *)(&regValue2));
        if (cslRetval != CSL_PASS) {
            retVal = SDL_FAIL;
        }
    }

    return retVal;
}

/** ============================================================================
 *
 * \brief   Get Ram Id for given memory and memory subtype
 *
 * \param1  eccMemType: Memory type for self test
 * \param2  memSubType: Memory subtype for self test
 * \param3  pRAMId: pointer to return Ram Id
 *
 * @return  SDL_PASS : Success; SDL_FAIL for failures
 */
static SDL_Result SDL_ECC_getRamId(SDL_ECC_MemType eccMemType, SDL_ECC_MemSubType memSubType,
    uint32_t *pRAMId)
{
    SDL_Result retVal = SDL_PASS;

    retVal = SDL_ECC_checkMemoryType(eccMemType, memSubType);
    if (retVal == SDL_PASS) {
        if (eccMemType == SDL_ECC_MEMTYPE_MCU_R5F0_CORE) {

            /* Get ram Id from table */
            *pRAMId = SDL_ECC_mcuArmssRamIdTable[memSubType].RAMId;

            /* To support other ecc aggregator instances,
             * need to add switch cases here
             */
        } else {
            retVal = SDL_FAIL;
        }
    }
    return retVal;
}

/** ============================================================================
 *
 * \brief   Check ECC check valid memory and memory subtype
 *
 * \param1  eccMemType: Memory type for self test
 * \param2  memSubType: Memory subtype for self test
 *
 * @return  SDL_PASS : Success; SDL_FAIL for failures
 */
static SDL_Result SDL_ECC_checkMemoryType(SDL_ECC_MemType eccMemType, SDL_ECC_MemSubType memSubType)
{
    SDL_Result retVal = SDL_PASS;

    if (eccMemType == SDL_ECC_MEMTYPE_MCU_R5F0_CORE) {
        /* Do bound check */
        if (((uint32_t)memSubType)
                >= ((uint32_t)(sizeof(SDL_ECC_mcuArmssRamIdTable)/sizeof(SDL_ECC_mcuArmssRamIdTable[0])))) {
            retVal = SDL_FAIL;
        }

        /* To support other ecc aggregator instances,
         * need to add switch cases here
         */
    } else {
        retVal = SDL_FAIL;
    }
    return retVal;
}

/** ============================================================================
 *
 * \brief   Get ECC aggregator type for given memory and memory subtype
 *
 * \param1  eccMemType: Memory type for self test
 * \param2  memSubType: Memory subtype for self test
 * \param3  pInjectOnly: pointer to variable indicating ECC aggregator
 *                       inject only
 *
 * @return  SDL_PASS : Success; SDL_FAIL for failures
 */
static SDL_Result SDL_ECC_getAggregatorType(SDL_ECC_MemType eccMemType, SDL_ECC_MemSubType memSubType,
    bool *pIinjectOnly)
{
    SDL_Result retVal = SDL_PASS;

    retVal = SDL_ECC_checkMemoryType(eccMemType, memSubType);
    if (retVal == SDL_PASS) {
        if (eccMemType == SDL_ECC_MEMTYPE_MCU_R5F0_CORE) {
            /* Get ram Id from table */
            *pIinjectOnly = SDL_ECC_mcuArmssRamIdTable[memSubType].aggregatorTypeInjectOnly;
        } else {
            retVal = SDL_FAIL;
        }
    }
    return retVal;
}
/** ============================================================================
 *
 * \brief   Get Ram Id for given memory and memory subtype
 *
 * \param1  eccMemType: Memory type for self test
 * \param2  memSubType: Memory subtype for self test
 * \param3  pMemConfig: pointer to return memory configuration
 *
 * \return  SDL_PASS : Success; SDL_FAIL for failures
 */
static SDL_Result SDL_ECC_getMemConfig(SDL_ECC_MemType eccMemType, SDL_ECC_MemSubType memSubType,
                              SDL_MemConfig_t *pMemConfig)
{
    SDL_Result retVal = SDL_PASS;

    retVal = SDL_ECC_checkMemoryType(eccMemType, memSubType);

    if (retVal == SDL_PASS) {
        if (eccMemType == SDL_ECC_MEMTYPE_MCU_R5F0_CORE) {
            /* Get memory configuration from table */
            *pMemConfig = SDL_ECC_mcuArmssMemEntries[memSubType];
        } else {
            retVal = SDL_FAIL;
        }
    }

   return retVal;
}
