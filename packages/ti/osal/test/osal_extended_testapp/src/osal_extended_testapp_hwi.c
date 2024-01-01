/*
 * Copyright (C) 2024 Texas Instruments Incorporated - http://www.ti.com/
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

/**
 *  \file   osal_extended_testapp_hwi.c
 *
 *  \brief  OSAL Hwi Sub Module testcases file.
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include "osal_extended_test.h"

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#define OSAL_APP_IRQ_INT_NUM           (28U)
#define OSAL_APP_IRQ_SECONDARY_INT_NUM (29U)

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

volatile uint32_t gOsalAppFlagHwiTest = UFALSE;

/* ========================================================================== */
/*                           Function Declarations                            */
/* ========================================================================== */

/*
 * Description : Test posting of interrupts. 
 */
int32_t OsalApp_hwiCreateAllocOvrflwTest(void);

/*
 * Description : Testing Null check for API Hwi_delete
 */
int32_t OsalApp_hwiNullTest(void);

/* ========================================================================== */
/*                          Internal Function Definitions                     */
/* ========================================================================== */

void OsalApp_hwiIRQ(uintptr_t arg)
{
    gOsalAppFlagHwiTest = UTRUE;
}

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

int32_t OsalApp_hwiNullTest()
{
    int32_t result = osal_OK;

    /* Test the else block of HwiP_Params_init with a NULL */
    HwiP_Params_init(NULL_PTR);

    if(NULL_PTR != HwiP_create(CSL_INVALID_VEC_ID, NULL_PTR, NULL_PTR))
    {
        result = osal_FAILURE;
    }

    if(osal_OK == result)
    {
        if(NULL_PTR != HwiP_createDirect(CSL_INVALID_VEC_ID, NULL_PTR, NULL_PTR))
        {
            result = osal_FAILURE;
        }
    }

    if(osal_OK == result)
    {
        if(HwiP_OK == HwiP_delete(NULL))
        {
            result = osal_FAILURE;
        }
    }

    return result;
}

int32_t OsalApp_hwiCreateAllocOvrflwTest()
{
    HwiP_Params hwiParams;
    HwiP_Handle handle1, handle2;
    int32_t result = osal_OK;

    HwiP_Params_init(&hwiParams);

    /* Create 2 HwiP to register a change in peakHandles */
    handle1 = HwiP_create(OSAL_APP_IRQ_INT_NUM, (HwiP_Fxn)OsalApp_hwiIRQ, &hwiParams);
    handle2 = HwiP_createDirect(OSAL_APP_IRQ_SECONDARY_INT_NUM, (HwiP_DirectFxn)OsalApp_hwiIRQ, &hwiParams);

    /* Second handle is not useful, so delete it */
    if((NULL_PTR == handle1) || (NULL_PTR == handle2) || (HwiP_OK != HwiP_delete(handle2)))
    {
        result = osal_FAILURE;
    }

    if(osal_OK == result)
    {
        HwiP_post(OSAL_APP_IRQ_INT_NUM);
        /* Wait till the interupt is hit */
        while(UFALSE == gOsalAppFlagHwiTest)
        {
            /* Do nothing */
        }
        gOsalAppFlagHwiTest = UFALSE;
        if(HwiP_OK != HwiP_delete(handle1))
        {
            result = osal_FAILURE;
        }
    }

    if(osal_FAILURE == result)
    {
        OSAL_log("\n HwiP create delete test has failed!! \n");
    }
    else
    {
        OSAL_log("\n HwiP create delete test has passed!! \n");
    }

    return result;
}

int32_t OsalApp_hwiTests(void)
{
    int32_t result = osal_OK;

    result += OsalApp_hwiCreateAllocOvrflwTest();
    /*
     * TODO: HwiP_create and HwiP_createdirect NULL tests are broken.
     * result += OsalApp_HwiPNullTest();
     */

    if(osal_OK == result)
    {
        OSAL_log("\n All HwiP tests have Passed!!\n");
    }
    else
    {
        OSAL_log("\n Some or all HwiP tests have Failed!!\n");
    }

    return result;
}

