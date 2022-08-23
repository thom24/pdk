/*
 *  Copyright ( C ) 2022 Texas Instruments Incorporated
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
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES ( INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION ) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  ( INCLUDING NEGLIGENCE OR OTHERWISE ) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 *  \file   SafeRTOS_config_r5f.c
 *
 *  \brief The file implements the safertos configs specific for R5F.
 *         This includes various startup routines (for MPU configuration, 
 *         Set DLFO bit, clear all interrupts, etc..) and other hook functions.
 * 
 **/

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

/* SafeRTOS Includes */
#include "SafeRTOS_API.h"
#include "mpuARM.h"

#include <ti/osal/src/nonos/Nonos_config.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

extern __attribute__((section(".startupCode"))) portBaseType xConfigureMPU(void);
void vFiqHandler( void );

/*---------------------------------------------------------------------------*/

/* Hook function handlers targeting the TI PDK libraries. */

/* Timer interrupt handler function. */
void vApplicationInterruptHandlerHook( void );

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/*-----------------------------------------------------------------------------
 * TI PDK variables required by the interrupt handler.
 *---------------------------------------------------------------------------*/

extern uint32_t             gVimBaseAddr;
extern uint8_t              intrMap[ R5_VIM_INTR_NUM ];
extern void                 *argArray[ R5_VIM_INTR_NUM ];
extern IntrFuncPtr          fxnArray[ R5_VIM_INTR_NUM ];
extern uint32_t             intrSrcType[ R5_VIM_INTR_NUM ];

/* ========================================================================== */
/*                          Function Defintions                               */
/* ========================================================================== */

/*-----------------------------------------------------------------------------
 * Public function definitions.
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * Override functions for CSL startup code.
 *---------------------------------------------------------------------------*/

/*****************************************************************************/
/* \brief __MPU_INIT() - __mpu_init() is called in the C/C++ startup routine,*/
/* _c_int00(), and provides a mechanism for tailoring mpu init by device     */
/* prior to calling main().                                                  */
/*                                                                           */
/*****************************************************************************/
/*  Usage notes:
    On entry to this function from boot, R5F must be in System (privileged) mode. */

__attribute__((section(".startupCode"))) void __mpu_init( void )
{
    portBaseType xInitMpuResult;

    /* Override the startup code mpu initialisation. */

    /* Region 0 configuration: whole 4 GB */
    xInitMpuResult = xPortMPUConfigureWholeMemory( 0U,
                                                   mpuREGION_PRIVILEGED_READ_WRITE_USER_READ_WRITE,
                                                   0U );

    if( pdPASS == xInitMpuResult )
    {
        xInitMpuResult = xConfigureMPU();
    }

    /* Configure the regions in the MPU that are common to all tasks. */
    vMPUSetupMPU();
}
/*-------------------------------------------------------------------------*/

/*****************************************************************************/
/* _SYSTEM_PRE_INIT() - _system_pre_init() is called in the C/C++ startup    */
/* routine (_c_int00()) and provides a mechanism for the user to             */
/* insert application specific low level initialization instructions prior   */
/* to calling main().  The return value of _system_pre_init() is used to     */
/* determine whether or not C/C++ global data initialization will be         */
/* performed (return value of 0 to bypass C/C++ auto-initialization).        */
/*                                                                           */
/* PLEASE NOTE THAT BYPASSING THE C/C++ AUTO-INITIALIZATION ROUTINE MAY      */
/* RESULT IN PROGRAM FAILURE.                                                */
/*                                                                           */
/* The version of _system_pre_init() below is skeletal and is provided to    */
/* illustrate the interface and provide default behavior.  To replace this   */
/* version rewrite the routine and include it as part of the current project.*/
/* The linker will include the updated version if it is linked in prior to   */
/* linking with the C/C++ runtime library.                                   */
/*****************************************************************************/
__attribute__((section(".startupCode")))  portBaseType _system_pre_init( void )
{
    return 1;
}
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
/* _SYSTEM_POST_CINIT() - _system_post_cinit() is a hook function called in  */
/* the C/C++ auto-initialization function after cinit() and before pinit().  */
/*                                                                           */
/* The version of _system_post_cinit() below is skeletal and is provided to  */
/* illustrate the interface and provide default behavior.  To replace this   */
/* version rewrite the routine and include it as part of the current project.*/
/* The linker will include the updated version if it is linked in prior to   */
/* linking with the C/C++ runtime library.                                   */
/*****************************************************************************/

/*---------------------------------------------------------------------------*/
/* __TI_default_system_post_cinit indicates that the default                 */
__attribute__((section(".startupCode")))  void _system_post_cinit( void )
{
    osalArch_Config_t cfg;

    cfg.disableIrqOnInit = true;
    osalArch_Init(&cfg);

    uint32_t            loopCnt = 0U, regAddr;
    CSL_ArmR5CPUInfo    info = {0};
    uint32_t            maxIntrs;
    CSL_vimRegs         *pRegs;

    CSL_armR5GetCpuID(&info);

#if defined(CSL_MAIN_DOMAIN_VIM_BASE_ADDR0) && defined(CSL_MAIN_DOMAIN_VIM_BASE_ADDR1)
    if (info.grpId == (uint32_t)CSL_ARM_R5_CLUSTER_GROUP_ID_0)
    {
        /* MCU SS Pulsar R5 SS */
        regAddr = (info.cpuID == CSL_ARM_R5_CPU_ID_0)?
                                 CSL_MCU_DOMAIN_VIM_BASE_ADDR0:
                                 CSL_MCU_DOMAIN_VIM_BASE_ADDR1;

    }
    else
    {
        /* MAIN SS Pulsar R5 SS */
        regAddr = (info.cpuID == CSL_ARM_R5_CPU_ID_0)?
                                 CSL_MAIN_DOMAIN_VIM_BASE_ADDR0:
                                 CSL_MAIN_DOMAIN_VIM_BASE_ADDR1;

    }
#else
    /* MCU SS Pulsar R5 SS */
    regAddr = (info.cpuID == CSL_ARM_R5_CPU_ID_0)?
                             CSL_MCU_DOMAIN_VIM_BASE_ADDR0:
                             CSL_MCU_DOMAIN_VIM_BASE_ADDR1;
#endif
    pRegs       = (CSL_vimRegs *)(uintptr_t) regAddr;
    maxIntrs    = pRegs->INFO;

#if defined (SOC_AM65XX) || defined (SOC_J721E)
    /* Limit the outstanding transactions to 2
     * only for AM65xx and J721e platforms
     * Later SoCs do not have this issue
     */

    /* Set DLFO bit is defined in SafeRTOS package portasm.S. */
    extern void vPortSetDLFOBit( void );

    vPortSetDLFOBit();
#else
    /* Fix for PRSDK-8161
     * For AM64x and J7200, there is no issue of limitting outstanding
     * transactions. R5F core can support full 7 outstanding transactions
     */
#endif

    /* Disable/Clear pending Interrupts in VIM before enabling CPU Interrupts */
    /* This is done to prevent serving any bogus interrupt */
    for (loopCnt = 0U ; loopCnt < maxIntrs; loopCnt++)
    {
        /* Disable interrupt in vim */
        CSL_vimSetIntrEnable((CSL_vimRegs *)(uintptr_t)regAddr,
                                    loopCnt,
                                    false);
        /* Clear interrupt status */
        CSL_vimClrIntrPending((CSL_vimRegs *)(uintptr_t)regAddr,
                                     loopCnt);
    }
}

/*-----------------------------------------------------------------------------
 * Copy of the TI master ISR handler.
 * This function is not compiled as an interrupt handler, so won't change stack
 * or enable interrupts on exit.
 *---------------------------------------------------------------------------*/

void vApplicationInterruptHandlerHook( void )
{
    IntrFuncPtr        fxnPtr;
    volatile uint32_t  intNum;
    volatile uint32_t  dummy;

    /* Read to force prioritization logic to take effect */
    dummy = CSL_vimGetIrqVectorAddress( (CSL_vimRegs *)(uintptr_t)gVimBaseAddr);

    /* Process a pending FIQ interrupt before a pending IRQ interrupt */
    if( ( CSL_vimGetActivePendingIntr( (CSL_vimRegs *)(uintptr_t)gVimBaseAddr, CSL_VIM_INTR_MAP_FIQ, (uint32_t *)&intNum, (uint32_t *)0 ) == 0 )       ||
        ( CSL_vimGetActivePendingIntr( (CSL_vimRegs *)(uintptr_t)gVimBaseAddr, CSL_VIM_INTR_MAP_IRQ, (uint32_t *)&intNum, (uint32_t *)0 ) == 0 ) )
    {
        /* Clear pulse-type interrupt before calling ISR */
        if( intrSrcType[intNum] == (uint32_t)CSL_VIM_INTR_TYPE_PULSE )
        {
            CSL_vimClrIntrPending( (CSL_vimRegs *)(uintptr_t)gVimBaseAddr, intNum );
        }
        /* Call ISR */
        if( fxnArray[intNum] != NULL )
        {
            fxnPtr = fxnArray[intNum];
            fxnPtr(argArray[intNum]);
        }
        /* Clear level-type interrupt after calling ISR */
        if( intrSrcType[intNum] == (uint32_t)CSL_VIM_INTR_TYPE_LEVEL )
        {
            CSL_vimClrIntrPending( (CSL_vimRegs *)(uintptr_t)gVimBaseAddr, intNum );
        }
        /* Acknowledge interrupt servicing */
        CSL_vimAckIntr( (CSL_vimRegs *)(uintptr_t)gVimBaseAddr, (CSL_VimIntrMap)intrMap[intNum] );
    }
}

/*-------------------------------------------------------------------------*/

/* Hardware setup using the TI PDK libraries. */
portBaseType prvSetupHardware( void )
{
    portBaseType xStatus = pdPASS;

    return xStatus;
}

/*-------------------------------------------------------------------------*/

void vFiqHandler( void )
{
    /* FIQ is not supported with SafeRTOS.
     *
     * Force an assert(), then stop here so application writers can catch the error. */
    DebugP_assert((bool)false);
}

/*-------------------------------------------------------------------------*/
