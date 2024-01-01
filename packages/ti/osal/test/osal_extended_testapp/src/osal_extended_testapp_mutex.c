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
 *  \file   osal_extended_testapp_mutex.c
 *
 *  \brief  OSAL Mutex Sub Module testcases file.
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include "osal_extended_test.h"

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                           Function Declarations                            */
/* ========================================================================== */

/*
 * Description: Test Null parameter checks for MutexP_create API
 */
int32_t OsalApp_mutexNullCheckTest(void);

#if defined(BARE_METAL)
/*
 * Description: Test MutexP nonos APIs by calling in this function
 */
int32_t OsalApp_mutexGeneralTest(void);
#endif

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

#if defined(BARE_METAL)
int32_t OsalApp_mutexGeneralTest(void)
{
    MutexP_Object         mutexObj;
    MutexP_Handle         mutexHandle;
    int32_t               result = osal_OK;

    mutexHandle = MutexP_create(NULL_PTR);
    if(NULL_PTR != mutexHandle)
    {
        result = osal_FAILURE;
    }

    if(osal_OK == result)
    {
        mutexHandle = MutexP_create(&mutexObj);
        if(NULL_PTR == mutexHandle)
        {
            result = osal_FAILURE;
        }
    }

    if(osal_OK == result)
    {
        if(MutexP_OK != MutexP_lock(mutexHandle, MutexP_WAIT_FOREVER))
        {
            result = osal_FAILURE;
        }
        else
        {
            if(MutexP_OK != MutexP_unlock(mutexHandle))
            {
                result = osal_FAILURE;
            }
        }
    }

    if(osal_OK == result)
    {
        if(MutexP_OK != MutexP_delete(mutexHandle))
        {
            result = osal_FAILURE;
        }
    }
    
    if(osal_OK == result)
    {
        OSAL_log("\n MutexP Test has passed!! \n");
    }
    else
    {
        OSAL_log("\n MutexP Test has failed!! \n");
    }

    return result;
}
#endif

int32_t OsalApp_mutexNullCheckTest(void)
{
    MutexP_Object         *mutexObj = (MutexP_Object *)NULL;
    MutexP_Handle         mutexHandle;
    MutexP_Status         status;
    int32_t               result = osal_OK;

    mutexHandle = MutexP_create(mutexObj);
    if(NULL != mutexHandle)
    {
       result = osal_FAILURE;
    }
    if(osal_OK == result)
    {
        status = MutexP_lock(mutexHandle, MutexP_WAIT_FOREVER);
        if(MutexP_OK == status)
        {
            result = osal_FAILURE;
        }
    }

    if(osal_OK == result)
    {
        status = MutexP_unlock(mutexHandle);
        if(MutexP_OK == status)
        {
            result = osal_FAILURE;
        }
    }

    if(osal_OK == result)
    {
        status = MutexP_delete(mutexHandle);
        if(MutexP_OK == status)   
        {
            result = osal_FAILURE;
        }
    }

    return result;
}

int32_t OsalApp_mutexTests(void)
{
    int32_t result = osal_OK;

#if defined(BARE_METAL)
    result += OsalApp_mutexGeneralTest();
#else
    result += OsalApp_mutexNullCheckTest();
#endif

    if(osal_OK != result)
    {
        OSAL_log("\n All MutexP tests have failed! \n");
    }
    else
    {
        OSAL_log("\n All MutexP tests have passed! \n");
    }

    return result;
}

