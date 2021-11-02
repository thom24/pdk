/*
    Copyright (C)2006 onward WITTENSTEIN aerospace & simulation limited. All rights reserved.

    This file is part of the SafeRTOS product, see projdefs.h for version number
    information.

    SafeRTOS is distributed exclusively by WITTENSTEIN high integrity systems,
    and is subject to the terms of the License granted to your organization,
    including its warranties and limitations on use and distribution. It cannot be
    copied or reproduced in any way except as permitted by the License.

    Licenses authorize use by processor, compiler, business unit, and product.

    WITTENSTEIN high integrity systems is a trading name of WITTENSTEIN
    aerospace & simulation ltd, Registered Office: Brown's Court, Long Ashton
    Business Park, Yanley Lane, Long Ashton, Bristol, BS41 9LB, UK.
    Tel: +44 (0) 1275 395 600, fax: +44 (0) 1275 393 630.
    E-mail: info@HighIntegritySystems.com

    www.HighIntegritySystems.com
*/

#include "SafeRTOS_API.h"
#include "aborts.h"
// #include "ParTest.h"
// #include "FullDemo.h"

/*-----------------------------------------------------------------------------
 * Private Function Declarations.
 *---------------------------------------------------------------------------*/

static void prvAbortDelay( void );


/*-----------------------------------------------------------------------------
 * External Functions (declared in abortsasm.s)
 *---------------------------------------------------------------------------*/

portUInt32Type ulGetDataFaultStatusRegister( void );
portUInt32Type ulGetDataFaultAddressRegister( void );
portUInt32Type ulGetInstructionFaultStatusRegister( void );
portUInt32Type ulGetInstructionFaultAddressRegister( void );


/*-----------------------------------------------------------------------------
 * Function Definitions
 *---------------------------------------------------------------------------*/

void vDataAbort( void )
{
    volatile portUInt32Type ulDataFaultStatusReg;
    volatile portUInt32Type ulDataFaultAddressReg;

    ulDataFaultStatusReg = ulGetDataFaultStatusRegister();
    ulDataFaultAddressReg = ulGetDataFaultAddressRegister();

    for( ;; )
    {
        prvAbortDelay();
    }
}
/*---------------------------------------------------------------------------*/

void vPrefetchAbort( void )
{
    volatile portUInt32Type ulInstrFaultStatusReg;
    volatile portUInt32Type ulInstrFaultAddressReg;

    ulInstrFaultStatusReg = ulGetInstructionFaultStatusRegister();
    ulInstrFaultAddressReg = ulGetInstructionFaultAddressRegister();

    for( ;; )
    {
        prvAbortDelay();
    }
}
/*---------------------------------------------------------------------------*/

void vApplicationAbort( void )
{
    for( ;; )
    {
        prvAbortDelay();
    }
}
/*---------------------------------------------------------------------------*/

void vStartupAbort( void )
{
    for( ;; )
    {
        prvAbortDelay();
    }
}
/*---------------------------------------------------------------------------*/

void vUndefAbort( void )
{
    for( ;; )
    {
        prvAbortDelay();
    }
}
/*---------------------------------------------------------------------------*/

static void prvAbortDelay( void )
{
    const portUnsignedBaseType uxDelayTime = 3500000;
    volatile portUnsignedBaseType uxCount = 0;

    for( uxCount = 0; uxCount < uxDelayTime; uxCount++ )
    {
        ; /* Do nothing. */
    }
}
/*---------------------------------------------------------------------------*/
