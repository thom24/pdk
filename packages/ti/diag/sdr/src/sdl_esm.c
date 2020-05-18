/*
 * SDR ESM
 *
 * Software Diagnostics Reference module for Error Signaling Module
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
#include <stddef.h>
#include <stdbool.h>

#include <sdl_common.h>
#include <sdl_esm.h>

#include <sdl_esm_soc.h>
#include "sdl_esm_core.h"

#include "sdl_esm_priv.h"

/* Global ESM instance */
static SDR_ESM_Instance_t SDR_ESM_instance;

/* Local Defines */
#define BITS_PER_WORD (32u)
#define GROUP_NUMBER_BIT_SHIFT  (5u)
#define NO_EVENT_VALUE (0xffffu)
#define SDR_ESM_EN_KEY_ENBALE_VAL (0xFU)



/* Local functions  */
static void SDR_ESM_interruptHandler (esmIntrPriorityLvl_t esmIntrPriorityLvlType, uintptr_t arg );
static void SDR_ESM_selfTestCallback(void);
static inline void SDR_ESM_getGroupNumberIndex(uint32_t intSrc, uint32_t *groupNumber,
                                    uint32_t *intIndex);
static void SDR_ESM_processInterruptSource(uintptr_t arg, uint32_t intSrc);

/** ============================================================================
 *
 * \brief   Initializes ESM module for SDR
 *
 * \param   esmInitConfig: Configuration for ESM
 *
 * \return  SDR_RETURN_PASS : Success; SDR_RETURN_FAIL for failures
 */
SDR_Result SDR_ESM_init (const SDR_ESM_InitConfig_t *esmInitConfig)
{
    SDR_Result result = SDR_PASS;
    int32_t cslRet;
    uint32_t intNum;
    uint32_t i,j;
    uint32_t intStatus;
    esmIntrPriorityLvl_t intrPriorityLvlWr, intrPriorityLvlRd;
    bool enableWr, enableRd;
    uint32_t influence;

    if ( esmInitConfig == ((void *)0u)) {
            result = SDR_FAIL;
    } else {

        /* Record init config in instance */
        SDR_ESM_instance.esmInitConfig = *esmInitConfig;


        /* ESM reset and configure */
        cslRet = ESMReset((uint32_t)SOC_ESM_BASE);

        /* Enable interrupt for all events from init configuration*/
        for(i=((uint32_t)0u); i< SDR_ESM_MAX_EVENT_MAP_NUM_WORDS; i++) {
            for(j=((uint32_t)0u); j< BITS_PER_WORD; j++) {
                intNum = (i*BITS_PER_WORD)+j;
                /*
                 * Clear interrupt status and verifiy if interrupt
                 * status is cleared
                 */
                if (cslRet == CSL_PASS)
                {
                    /* Clear interrupt status, so that we start with clean state */
                    cslRet = ESMClearIntrStatus((uint32_t)SOC_ESM_BASE, intNum);
                }
                if (cslRet == CSL_PASS)
                {
                    cslRet = ESMGetIntrStatus((uint32_t)SOC_ESM_BASE, intNum, &intStatus);
                }
                if (cslRet == CSL_PASS)
                {
                    if (intStatus != ((uint32_t)0U))
                    {
                        cslRet = CSL_EFAIL;
                    }
                }

                /* Depending on the bitmap configuration enable interrupt and set priority level */
                if((esmInitConfig->eventMap[i]&(((uint32_t)1u)<<j)) != 0u) {
                    /* Enable interrupt and verifiy if interrupt status is enabled */
                    if (cslRet == CSL_PASS)
                    {
                        cslRet = ESMEnableIntr(((uint32_t)SOC_ESM_BASE), intNum);
                    }
                    if (cslRet == CSL_PASS)
                    {
                        cslRet = ESMIsEnableIntr((uint32_t)SOC_ESM_BASE, intNum, &intStatus);
                    }
                    if (cslRet == CSL_PASS)
                    {
                        if (intStatus != ((uint32_t)1U))
                        {
                            cslRet = CSL_EFAIL;
                        }
                    }

                    /* Set interrupt priority level and verifiy if priority level is set */
                    if (cslRet == CSL_PASS)
                    {
                        intrPriorityLvlWr = ((esmInitConfig->eventPriorityMap[i]
                                              & (((uint32_t)1u)<<j)) != 0u)?1u:0u;
                        cslRet = ESMSetIntrPriorityLvl((uint32_t)SOC_ESM_BASE,
                                                       intNum,
                                                       intrPriorityLvlWr);
                    }
                    if (cslRet == CSL_PASS)
                    {
                        cslRet = ESMGetIntrPriorityLvl((uint32_t)SOC_ESM_BASE,
                                                       intNum,
                                                       &intrPriorityLvlRd);
                    }
                    if (cslRet == CSL_PASS)
                    {
                        if (intrPriorityLvlWr != intrPriorityLvlRd)
                        {
                            cslRet = CSL_EFAIL;
                        }
                    }
                }
                /*
                 * Depending on the bitmap configuration set error output
                 * and verify if error output is set */
                if (cslRet == CSL_PASS)
                {
                    enableWr = ((esmInitConfig->errorOutputMap[i]
                                 & (((uint32_t)1u)<<j)) != 0u)?(bool)true:(bool)false;
                    cslRet = ESMSetInfluenceOnErrPin((uint32_t)SOC_ESM_BASE,
                                                     intNum,
                                                     enableWr);
                }
                if (cslRet == CSL_PASS)
                {
                    cslRet = ESMGetInfluenceOnErrPin((uint32_t)SOC_ESM_BASE,
                                                     intNum,
                                                     &influence);
                }
                if (cslRet == CSL_PASS)
                {
                    enableRd = (influence != 0u)?(bool)true:(bool)false;
                    if (enableWr != enableRd)
                    {
                        cslRet = CSL_EFAIL;
                    }
                }
                if (cslRet != CSL_PASS)
                {
                    break;
                }
            }
        }

        /* Enable Global interrupt and verify if global interrupt is enabled for ESM */
        if (cslRet == CSL_PASS)
        {
            cslRet = ESMEnableGlobalIntr((uint32_t)SOC_ESM_BASE);
        }
        if (cslRet == CSL_PASS)
        {
            cslRet = ESMGetGlobalIntrEnabledStatus((uint32_t)SOC_ESM_BASE, &intStatus);
        }
        if (cslRet == CSL_PASS)
        {
            if (intStatus != SDR_ESM_EN_KEY_ENBALE_VAL)
            {
                cslRet = CSL_EFAIL;
            }
        }
        if (cslRet != CSL_PASS)
        {
            result = SDR_FAIL;
        }
    }

    return result;
}

/** ============================================================================
 *
 * \brief  Function sets the nERROR pin inactive.
 *
 * \param  None
 *
 * \return  SDR_RETURN_PASS : Success; SDR_RETURN_FAIL for failures
 */
SDR_Result SDR_ESM_resetNError(void)
{
    SDR_Result         result;
    int32_t            cslRet;
    uint32_t           status;

    cslRet = ESMResetErrPin((uint32_t)SOC_ESM_BASE);
    if (cslRet == CSL_PASS)
    {
        cslRet = ESMGetErrPinStatus((uint32_t)SOC_ESM_BASE, &status);
    }
    if (cslRet == CSL_PASS)
    {
        if (status != ((uint32_t)1U))
        {
            cslRet = CSL_EFAIL;
        }
    }
    if (cslRet == CSL_PASS)
    {
        result = SDR_PASS;
    }
    else
    {
        result = SDR_FAIL;
    }
    return result;
}

/** ============================================================================ 
 *
 * \brief   Function sets the nERROR pin active.
 *
 * \param [in]    None
 * \param [out]   None
 *
 * \return    None
 */
void SDR_ESM_setNError(void){
    /* Set Force Error output */
    (void)ESMSetMode(((uint32_t)SOC_ESM_BASE), ESM_OPERATION_MODE_ERROR_FORCE);
}

/** ============================================================================
 *
 * \brief   Function returns the status of nERROR pin
 *
 * \param [in]    None
 * \param [out]   None
 *
 * \return    1 : active; 0 not active
 */
bool SDR_ESM_getNErrorStatus(void)
{
    uint32_t status;
    bool retValue;

    (void)ESMGetErrPinStatus((((uint32_t)SOC_ESM_BASE)), &status);

    if (status == 1u) {
        retValue = (bool)true;
    } else {
        retValue = (bool)false;
    }
    return retValue;
}

/** ============================================================================
 *
 * \brief   Insert ESM error
 *
 * \param1   ESM_error_config: Configuration of ESM error
 *
 * \return  SDR_RETURN_PASS : Success; SDR_RETURN_FAIL for failures
 */
SDR_Result SDR_ESM_errorInsert (const SDR_ESM_ErrorConfig_t *esmErrorConfig)
{
    SDR_Result result = SDR_FAIL;

    if (esmErrorConfig != ((void *)0u)) {
        if ((esmErrorConfig->groupNumber < SDR_ESM_MAX_EVENT_MAP_NUM_WORDS)
         && (esmErrorConfig->bitNumber < BITS_PER_WORD)) {
            /* Insert error */
            (void)ESMSetIntrStatusRAW(((uint32_t)SOC_ESM_BASE),
                                (esmErrorConfig->groupNumber*BITS_PER_WORD)
                                + esmErrorConfig->bitNumber);
            result = SDR_PASS;
        }
    }

    return result;
}

/** ============================================================================
 *
 * \brief   Execute self test of ESM module
 *
 * \param   loopCount: Number of tries to check status in a loop
 *
 * \return  SDR_RETURN_PASS : Success; SDR_RETURN_FAIL for failures
 */
SDR_Result SDR_ESM_selfTest (uint32_t loopCount)
{
    uint32_t timeCount = 0u;
    SDR_Result result = SDR_PASS;

    /* reset error and timout flags */
    SDR_ESM_instance.selfTestFlag = (bool)false;

    /* Insert error for configured self test error number */
    result = SDR_ESM_errorInsert(&SDR_ESM_instance.esmInitConfig.esmErrorConfig);
    if (result == SDR_PASS) {
        do
        {
            timeCount++;
            if ((loopCount != 0u)
                && (timeCount >= loopCount)) {
                break;
            }
        } while(!SDR_ESM_instance.selfTestFlag);

        /* Check expected error occurred or timeout */
        if (!SDR_ESM_instance.selfTestFlag ) {
            /* Timed out */
            result = SDR_FAIL;
        }
    }
    /* reset error and timout flags */
    SDR_ESM_instance.selfTestFlag = (bool)false;

    return result;
}

/** ============================================================================
 *
 * \brief   Call back for ESM test error
 *
 * \param   None
 *
 * \return  None
 */
static void SDR_ESM_selfTestCallback(void)
{
    SDR_ESM_instance.selfTestFlag  = (bool)true;

    return;
}

/** ============================================================================
 *
 * \brief   Get the Group number and Index for the given interrrupt
 *          source
 *
 * \param2  registered argument
 * \param2  Interrupt source
 *
 * \return  None
 */
static inline void SDR_ESM_getGroupNumberIndex(uint32_t intSrc, uint32_t *groupNumber,
                                    uint32_t *intIndex)
{
    *groupNumber = intSrc >> GROUP_NUMBER_BIT_SHIFT;
    *intIndex = intSrc-((*groupNumber) << GROUP_NUMBER_BIT_SHIFT);

    return;
}

/** ============================================================================
 *
 * \brief   Process Interrupt source and call callback function
 *
 * \param2  registered argument
 * \param2  Interrupt source
 *
 * \return  None
 */
static void SDR_ESM_processInterruptSource(uintptr_t arg, uint32_t intSrc)
{

    uint32_t groupNumber, intIndex;
    bool handledFlag;

    if (intSrc != NO_EVENT_VALUE) {
        if (intSrc >= (BITS_PER_WORD*SDR_ESM_MAX_EVENT_MAP_NUM_WORDS)) {
            /* Unexpected error */
            /* Call assert function */
            SDR_assertExternalFunction(SDR_ESM_INT_SRC_OUT_OF_BOUNDS);
        } else {
            SDR_ESM_getGroupNumberIndex(intSrc, &groupNumber, &intIndex);
            if((SDR_ESM_instance.esmInitConfig.eventMap[groupNumber]
               & (((uint32_t)1u)<<intIndex)) != 0u) {

                /* Check if this is due to self test */
                if((groupNumber
                    == SDR_ESM_instance.esmInitConfig.esmErrorConfig.groupNumber) &&
                   (intIndex ==  SDR_ESM_instance.esmInitConfig.esmErrorConfig.bitNumber)){
                    SDR_ESM_selfTestCallback();
                } else {
                    handledFlag = SDR_ESM_handleIntSrc(&SDR_ESM_instance, intSrc);
                    if (!handledFlag) {
                        SDR_ESM_applicationCallbackFunction(arg, groupNumber,
                                                            intIndex,
                                                            intSrc);
                    }
                }
                /* Clear this error */
                (void)ESMClearIntrStatus(((uint32_t)SOC_ESM_BASE), intSrc);
            }
        }
    }
    return;
}

/** ============================================================================
 *
 * \brief   Interrupt handler for Hi ESM interrupt
 *
 * \param1  esmIntrPriorityLvlType: Interrupt priority level
 * \param2  arg: Argument to callback function
 *
 * \return  None
 */
static void SDR_ESM_interruptHandler (esmIntrPriorityLvl_t esmIntrPriorityLvlType,
                                           uintptr_t arg )
{
    uint32_t intSrc1, intSrc2;
    esmGroupIntrStatus_t localEsmGroupIntrStatus;

    /* Check on the highest priority event and handle it */
    do {
        (void)ESMGetGroupIntrStatus(((uint32_t)SOC_ESM_BASE), (uint32_t)esmIntrPriorityLvlType,
                              &localEsmGroupIntrStatus);
        intSrc1 = localEsmGroupIntrStatus.highestPendPlsIntNum;
        SDR_ESM_processInterruptSource(arg, intSrc1);
        intSrc2 = localEsmGroupIntrStatus.highestPendLvlIntNum;
        SDR_ESM_processInterruptSource(arg, intSrc2);
    } while ((intSrc1 != (uint32_t)(NO_EVENT_VALUE)) || (intSrc2 != (uint32_t)(NO_EVENT_VALUE)));

    return;
}

/** ============================================================================
 *
 * \brief   Esm Hi Interrupt Handler
 *
 * \param1  arg: argument for handler
 *
 * \return  None
 */
void SDR_ESM_hiInterruptHandler (uintptr_t arg)
{

    /* Call common Interrupt handler */
     SDR_ESM_interruptHandler(ESM_INTR_PRIORITY_LEVEL_HIGH, arg);

    /* Write end of interrupt */
    (void)ESMWriteEOI(((uint32_t)SOC_ESM_BASE), (uint32_t)ESM_INTR_TYPE_HIGH_PRIO_ERROR);

    return ;
}

/** ============================================================================
 *
 * \brief   Esm Lo Interrupt Handler
 *
 * \param1  arg: argument for handler
 *
 * \return  None
 */
void SDR_ESM_loInterruptHandler (uintptr_t arg)
{

    /* Call common Interrupt handler */
    SDR_ESM_interruptHandler(ESM_INTR_PRIORITY_LEVEL_LOW, arg);

    /* Write end of interrupt */
    (void)ESMWriteEOI(((uint32_t)SOC_ESM_BASE), (uint32_t)ESM_INTR_TYPE_LOW_PRIO_ERROR);

    return;
}

/** ============================================================================
 *
 * \brief   Esm Config Interrupt Handler
 *
 * \param1  arg: argument for handler
 *
 * \return  None
 */
void SDR_ESM_configInterruptHandler(uintptr_t arg)
{
    SDR_ESM_applicationCallbackFunction(arg, 0, 0, 0);

    /* Write end of interrupt */
    (void)ESMWriteEOI(((uint32_t)SOC_ESM_BASE), (uint32_t)ESM_INTR_TYPE_CONFIG_ERROR);

    return;
}

/** ============================================================================
 *
 * \brief   Register handler for ECC with ESM
 *
 * \param1  eccCallBackFunctionPtr: Call back function to register
 *
 * \return  SDR_RETURN_PASS : Success; SDR_RETURN_FAIL for failures
 */
SDR_Result SDR_ESM_registerECCHandler(SDR_ESM_ECCCallback_t eccCallBackFunctionPtr)
{
    SDR_Result result = SDR_PASS;

    /* Store call back function */
    SDR_ESM_instance.eccCallBackFunction = eccCallBackFunctionPtr;

    return result;
}


/** ============================================================================
 *
 * \brief   Register callback function for CCM errrors
 *
 * \param   CCMCallBackFunctionPtr: Callback function pointer
 *
 * \return  SDR_PASS : Success; SDR_FAIL for failures
 */
SDR_Result SDR_ESM_registerCCMHandler(SDR_ESM_CCMCallback_t CCMCallBackFunctionPtr)
{
    SDR_Result result = SDR_PASS;

    /* Store call back function */
    SDR_ESM_instance.CCMCallBackFunction = CCMCallBackFunctionPtr;

    return result;
}

/** ============================================================================
 *
 * \brief   Register callback function for WDT events
 *
 * \param   WDTCallBackFunctionPtr: Callback function pointer
 *
 * \return  SDR_PASS : Success; SDR_FAIL for failures
 */
SDR_Result SDR_ESM_registerWDTHandler(SDR_ESM_WDTCallback_t WDTCallBackFunctionPtr)
{
    SDR_Result result = SDR_PASS;

    /* Store call back function */
    SDR_ESM_instance.WDTCallBackFunction = WDTCallBackFunctionPtr;

    return result;
}

/** ============================================================================
 *
 * \brief   De-Register callback function for WDT events
 *
 *
 * \return  SDR_PASS : Success; SDR_FAIL for failures
 */
void SDR_ESM_deRegisterWDTHandler(void)
{
    /* Store call back function */
    SDR_ESM_instance.WDTCallBackFunction = NULL;

    return;
}

/** ============================================================================
 *
 * \brief   Esm get Interrupt Number corresponding to the
 *          input interrupt type
 *
 * \param   Interrupt type
 *
 * \return  Interrupt Number or SDR_ESM_INTNUMBER_INVALID error
 */
uint32_t SDR_ESM_getIntNumber(SDR_ESM_IntType esmIntType)
{
    uint32_t intNum = SDR_ESM_INTNUMBER_INVALID;

    switch(esmIntType)
    {
        case SDR_ESM_INT_TYPE_HI:
            intNum = SDR_ESM_HI_INTNO;
            break;

        case SDR_ESM_INT_TYPE_CFG:
            intNum = SDR_ESM_CFG_INTNO;
            break;

        case SDR_ESM_INT_TYPE_LO:
        default:
            intNum = SDR_ESM_LO_INTNO;
            break;
    }

    return intNum;
}
