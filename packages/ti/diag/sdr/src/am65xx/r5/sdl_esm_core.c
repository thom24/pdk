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

#include <stdbool.h>
#include <stddef.h>

#include <sdl_esm.h>
#include <sdl_esm_soc.h>
#include <sdl_esm_core.h>
#include <sdl_esm_priv.h>

/** ============================================================================
 *
 * \brief   Handle any event that needs to be handled locally before
 *          reporting to application
 *
 * \param  pInstance: Pointer to ESM instance
 * \param  intSrc: Source interrupt number
 *
 * \return  true: if event handled; false if not handled
 */
bool SDR_ESM_handleIntSrc(const SDR_ESM_Instance_t *pInstance, uint32_t intSrc)
{
    bool handledFlag = ((bool)false);

    /* For each error bit */
    switch (intSrc) {
        case SDR_ESM_MCU_R5_CORE0_SEC_INT:
            if ( pInstance->eccCallBackFunction != ((void *)0u)) {
                /* Call back function for Single bit ECC error */
                pInstance->eccCallBackFunction(SDR_ESM_ECC_PARAM_MCU_CPU0_SEC_ERROR,
                                                    SDR_ESM_ERRORADDR_INVALID);
                handledFlag = ((bool)true);
            }
            break;

        case SDR_ESM_MCU_R5_CORE0_DED_INT:
            if ( pInstance->eccCallBackFunction != ((void *)0u)) {
                /* Call back function for Double bit ECC error */
                pInstance->eccCallBackFunction(SDR_ESM_ECC_PARAM_MCU_CPU0_DED_ERROR,
                                                    SDR_ESM_ERRORADDR_INVALID);
                handledFlag = ((bool)true);
            }

            break;

        case SDR_ESM_MCU_R5_CPU_BUS_CMP_ERR:
            if ( pInstance->CCMCallBackFunction != ((void *)0u)) {
            /* Call back function for CCM compare error */
                pInstance->CCMCallBackFunction(SDR_ESM_CCM_OUTPUT_COMPARE_BLOCK_INT);
                handledFlag = ((bool)true);
            }
            break;


        case SDR_ESM_MCU_R5_INACTIVITY_ERR_INT:
            if ( pInstance->CCMCallBackFunction != ((void *)0u)) {
                /* Call back function for CCM inactivity monitor error */
                pInstance->CCMCallBackFunction(SDR_ESM_CCM_INACTIVITY_MONITOR_INT);
                handledFlag = ((bool)true);
            }
            break;

        case SDR_ESM_MCU_R5_VIM_BUS_CMP_ERR_INT:
            if ( pInstance->CCMCallBackFunction != ((void *)0u)) {
                /* Call back function for CCM VIM compare error */
                pInstance->CCMCallBackFunction(SDR_ESM_CCM_VIM_ERR_INT);
                handledFlag = ((bool)true);
            }
            break;

        case SDR_ESM_MCU_R5_SELFTEST_ERR_INT:
            if ( pInstance->CCMCallBackFunction != ((void *)0u)) {
                /* Call back function for CCM VIM compare error */
                pInstance->CCMCallBackFunction(SDR_ESM_CCM_SELF_TEST_ERR_INT);
                handledFlag = ((bool)true);
            }
            break;

        case SDR_ESM_MCU_R5_CCM_STAT_ERR_INT:
            if ( pInstance->CCMCallBackFunction != ((void *)0u)) {
                /* Call back function for CCM VIM compare error */
                pInstance->CCMCallBackFunction(SDR_ESM_CCM_STAT_ERR_INT);
                handledFlag = ((bool)true);
            }
            break;

        case SDR_ESM_MCU_RTI0_WWD_INT:
            if ( pInstance->WDTCallBackFunction != ((void *)0u)) {
                /* Call back function for Windowed watchdog timer 0 */
                handledFlag = pInstance->WDTCallBackFunction(SDR_ESM_TIMER_ID_0);
            }
            break;

        case SDR_ESM_MCU_RTI1_WWD_INT:
            if ( pInstance->WDTCallBackFunction != ((void *)0u)) {
                /* Call back function for Windowed watchdog timer 1 */
                handledFlag = pInstance->WDTCallBackFunction(SDR_ESM_TIMER_ID_1);
            }
            break;

        default:
            /* No actions for other ESM Events */
            break;
    }
    return handledFlag;
}

