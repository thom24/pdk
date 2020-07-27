/*
 * Copyright (C) 2020 Texas Instruments Incorporated - http://www.ti.com/
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of Texas Instruments Incorporated nor the names of
 * its contributors may be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

/** \defgroup DRV_ESM_MODULE ESM Driver
*
*  The ESM driver provides APIs to configure and handle errors coming from the ESM H/W module.
*
*  The ESM header file should be included in an application as follows:
*  @code
*  #include <ti/drv/esm/esm.h>
*  @endcode
*
*  ## Initializing the driver #
*  The ESM Driver is initialized using the #ESM_init.
*  None of the ESM API can be used without invoking
*  this API. <br>
*  **Note**: If you using TI RTOS, then ESM errors are cleared before
*        entering main and this flag can be set to 0. For any other RTOS, check
*        the RTOS implementation or set this flag to 1
*
*  ## Using the driver #
*  Following is a psuedo code for using the ESM driver
*
*  @code
*
  ESM_init(0); //ClearErrors is set to 0 i.e. false

  *  @endcode
*
*  ## Handling Interrupts on MSS#
*  The ESM driver when running on R5F, registers for high priority interrupt to handle the MSS ESM errors.
*  When ESM errors occurs on DSS, the MSS is notified via a Group1 error signal. The ESM driver
*  registers for low priority IRQ to handle these errors.
*

*  ## Handling Interrupts on DSS#
*  The DSS Group2 ESM errors generate an NMI that is first captured by the RTOS NMI exception handler.
*  Hence applications have to populate the NMI exception handler to use the ESM driver's high
*  priority interrupt processing API in the .cfg file.
 *
 *  @code
 *
     Exception.nmiHook           = "&ESM_highpriority_interrupt";

 *  @endcode

*  ## Registering the notifiers #
*  The application can register callback functions for the ESM errors that need additional handling. One
*  such example is the watchdog timeout on the DSS.
*  When the watchdog timeout causes an NMI to occur on DSS, that event in turn generates a group1 ESM error
*  on the MSS. The application can register a callback using the ESM notifier to handle these errors.
*

*  ## Instrumentation #
*  Uses DebugP_log functions for debug messages
*
*  ## Hardware Register Map #
*  The hardware register map used by this driver can be found at csl/src/ip/esm/V0/V0_1/cslr_esm.h
*
*  ============================================================================
*/
/**
\defgroup ESM_DRIVER_EXTERNAL_FUNCTION            ESM Driver External Functions
\ingroup ESM_DRIVER
\brief
*   The section has a list of all the exported API which the applications need to
*   invoke in order to use the driver
*/
/**
\defgroup ESM_DRIVER_EXTERNAL_DATA_STRUCTURE      ESM Driver External Data Structures
\ingroup ESM_DRIVER
\brief
*   The section has a list of all the data structures which are exposed to the application
*/

/**
\defgroup ESM_DRIVER_ERROR_CODE                   ESM Driver Error Codes
\ingroup ESM_DRIVER
\brief
*   The section has a list of all the error codes which are generated by the ESM Driver
*   module
*/

/** \file       drv/esm/esm.h
*
*  \brief      ESM driver interface
*
*/

#ifndef ESM_DRV_H_
#define ESM_DRV_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
    extern "C" {
#endif

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/** \brief ESM driver error base */
#define ESM_ERRNO_BASE               (-3000)

/** \defgroup ESM_DRIVER_ERROR_CODE    ESM Driver Error Codes
\ingroup DRV_ESM_MODULE
 *
 @{ */

/**
 * \brief   Error Code: Invalid argument
 */
#define ESM_EINVAL                 (ESM_ERRNO_BASE-1)

/**
 * \brief   Error Code: Operation cannot be done as ESM_init is not done.
 */
#define ESM_ENOINIT                (ESM_ERRNO_BASE-2)

/**
 * \brief   Error Code: Operation cannot be done as ESM_init is already done and re-init is not permitted
 */
#define ESM_EREINIT                (ESM_ERRNO_BASE-3)

/**
 * \brief   Error Code: Out of memory
 */
#define ESM_ENOMEM                 (ESM_ERRNO_BASE-4)

/** @}*/

/** \defgroup ESM_DRIVER_EXTERNAL_DATA_STRUCTURE    ESM Driver External Data Structures
\ingroup DRV_ESM_MODULE
 @{ */


/*!
 *  \brief      ESM module handle returned by the ESM_init() API call.
 */
typedef void* ESM_Handle;

/**
 * \brief
 * Callback function which is invoked by the ESM module if a notify function is registered
 * using the ESM_registerNotifier() API.
 *
 *  \param[in]  arg
 *      Argument passed back when the Notify function is invoked.
 *
 *  @retval
 *      Not applicable
 */
typedef void (*ESM_CallBack)(void* arg);

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/*!
 * \brief
 *  Parameters used to register the ESM notify function to handle
 *  Group1 and Group2 errors. The notify function will be invoked post
 *  the ESM FIQ interrupt handler processing.
 */
typedef struct ESM_NotifyParams_t
{
    /*! Group number to which the following error number belongs. */
    uint32_t            groupNumber;

    /*! Error number for which the notify function is registered. */
    uint32_t            errorNumber;

    /*! Set the interrupt priority level to high. Applicable to Group 1 errors only. */
    uint32_t            setIntrPriorityLvlHigh;

    /*! Enable failure influence on ERROR pin. Applicable to Group 1 errors only. */
    uint32_t            enableInfluenceOnErrPin;

    /*! Argument passed back when the Notify function is invoked. */
    void*               arg;

    /*! Notify function called by the ESM driver. */
    ESM_CallBack        notify;
} ESM_NotifyParams;

/** @}*/

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/** \defgroup ESM_DRIVER_EXTERNAL_FUNCTION   ESM Driver External Functions
\ingroup DRV_ESM_MODULE
 *
 @{ */

/**
*  @b Description
*  @n
*      The function initializes the ESM driver.
*
*    \param[in] bClearErrors: boolean value to indicate if old ESM pending errors should be cleared or not <br>
*                             value = 0: do not clear <br>
*                             value = 1: clear all ESM group errors <br>
*                             hint: If you are using TI RTOS, then ESM errors are cleared before
*                                   entering main and this flag can be set to 0. <br> For any other RTOS, check the
*                                   the RTOS implementation or set this flag to 1
*
*    \retval
*        Success -   Handle to the ESM Driver
*    \retval
*        Error   -   NULL
*
*/
extern ESM_Handle ESM_init(uint8_t bClearErrors);

/**
*  @b Description
*  @n
*      The function closes the ESM driver.
*
*    \param[in] handle: Handle to the ESM Driver
*
*
*    \retval
*        Success - Value > 0
*    \retval
*        Error   - Value < 0
*
*/
extern int32_t ESM_close(ESM_Handle handle);

/**
*  @b Description
*  @n
*    The function registers the notifers the ESM module will call back if error interrupt is detected.
*
*   \param[in] handle: Handle to the ESM Driver.
*
*   \param[in] params: Notifier error number and callback function.
*
*   \param[out]  errCode
*      Error code populated on error.
*
*    \retval
*        Success -   Notifier index. Used when deregistering the notifier.
*    \retval
*        Error   -   ESM Error code
*
*/
extern int32_t ESM_registerNotifier(ESM_Handle handle, ESM_NotifyParams* params, int32_t* errCode);

/**
*  @b Description
*  @n
*    The function deregisters the ESM notifers.
*
*   \param[in] handle: Handle to the ESM Driver.
*
*   \param[in] notifyIndex: Notifier index returned when the notifier was registered.
*
*   \param[out]  errCode
*      Error code populated on error.
*
*    \retval
*        Success -   0
*    \retval
*        Error   -   ESM Error code
*
*/
extern int32_t ESM_deregisterNotifier(ESM_Handle handle, int32_t notifyIndex, int32_t* errCode);
/** @}*/


#ifdef __cplusplus
}
#endif

#endif /* ESM_DRV_H_ */


