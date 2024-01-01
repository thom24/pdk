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
 *  \file   osal_extended_testapp_heap.c
 *
 *  \brief  OSAL Heap Sub Module testcases file.
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include "osal_extended_test.h"

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#define OSAL_APP_HEAP_SIZE (1024U)

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

static uint8_t gOsalAppHeapPbuf[OSAL_APP_HEAP_SIZE];

/* ========================================================================== */
/*                            Function Declarations                           */
/* ========================================================================== */

/*
 * Description: Test below mentioned APIs :
 *      1. HeapP_Params_init
 *      2. HeapP_create
 *      3. HeapP_alloc
 *      4. HeapP_free
 *      5. HeapP_delete
 */
int32_t OsalApp_heapFreertosAllocTest(void);

/*
 * Description: Testing Null paramter check for the below mentioned APIs :
 *      1. HeapP_create
 *      2. HeapP_alloc
 *      3. HeapP_free
 *      4. HeapP_getHeapStats
 *      5. HeapP_delete
 */
int32_t OsalApp_heapFreertosIsUsedTest(void);

/*
 * Description: Testing null parameter check for HeapP_Params_init API
 */
int32_t OsalApp_heapParamsInitNullTest(void);

/*
 * Description: Testing if HeapP_create can create the maximum number of heaps.
 */
int32_t OsalApp_heapFreertosMaxTest(void);

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

int32_t OsalApp_heapFreertosAllocTest(void)
{
    HeapP_Params      params;
    HeapP_Handle      handle, memAddr;
    HeapP_Status      status;
    HeapP_MemStats    memstats;
    uint32_t          allocSize = 10U, freeSize = 5U;
    int32_t           result = osal_OK;
    
    memset(gOsalAppHeapPbuf, 0x00, sizeof(gOsalAppHeapPbuf));

    HeapP_Params_init(&params);
    params.buf = gOsalAppHeapPbuf;
    params.size = sizeof(gOsalAppHeapPbuf);

    handle = HeapP_create(&params);
    if(NULL_PTR == handle)
    {
        result = osal_FAILURE;
    }
    
    if(osal_OK == result)
    {
        memAddr = HeapP_alloc(handle, allocSize);
        if(NULL_PTR == memAddr)
        {
            result = osal_FAILURE;
        }
    }

    if(osal_OK == result)
    {
        status = HeapP_free(handle, memAddr, freeSize);
        if(HeapP_OK != status)
        {
            result = osal_FAILURE;
        }
    }
    
    if(osal_OK == result)
    {
        status = HeapP_getHeapStats(handle, &memstats);
        if(HeapP_OK != status)
        {
            result = osal_FAILURE;
        }
    }

    if(osal_OK == result)
    {
        status = HeapP_delete(handle);
        if(HeapP_OK != status)
        {
            result = osal_FAILURE;
        }
    }

    if(osal_OK == result)
    {
        OSAL_log("\n HeapP negative tests have passed! \n");
    }
    else
    {
        OSAL_log("\n HeapP negative tests have failed! \n");
    }

    return result;
}

int32_t OsalApp_heapFreertosIsUsedTest(void)
{
    HeapP_Params      params;
    HeapP_Handle      handle;
    HeapP_Status      status;
    HeapP_MemStats    memstats;
    void              *allocAddr = NULL_PTR;
    uint32_t          allocSize = 10U, freeSize = 5U;
    int32_t           result = osal_OK;

    memset(gOsalAppHeapPbuf, 0xFF, sizeof(gOsalAppHeapPbuf));
    HeapP_Params_init(&params);

    params.buf = gOsalAppHeapPbuf;
    params.size = sizeof(gOsalAppHeapPbuf);

    handle = HeapP_create(&params);
    if(NULL_PTR == handle)
    {
        result = osal_FAILURE;
    }

    /* Here handleAddr is used to get the memory location of the handle
    * we are corrupting the content of the handle and passing in a corrupt handle to the driver
    * to test negative condition for HeapP_alloc API
    */
    uint32_t *handleAddr = (uint32_t *)handle, temp;
    temp = (*handleAddr);
    (*handleAddr) = 0U;

    if(osal_OK == result)
    {
        status = HeapP_free(handle, allocAddr, freeSize);
        if(HeapP_OK == status)
        {
            result = osal_FAILURE;
        }
    }

    if(osal_OK == result)
    {
        allocAddr = HeapP_alloc(handle, allocSize);
        if(NULL_PTR != allocAddr)
        {
            result = osal_FAILURE;
        }
    }

    if(osal_OK == result)
    {
        status = HeapP_free(handle, allocAddr, freeSize);
        if(HeapP_OK == status)
        {
            result = osal_FAILURE;
        }
    }

    if(osal_OK == result)
    {
        status = HeapP_getHeapStats(handle, &memstats);
        if(HeapP_OK == status)
        {
            result = osal_FAILURE;
        }
    }
  
    if(osal_OK == result)
    {
        status = HeapP_delete(handle);
        if(HeapP_OK == status)
        {
            result = osal_FAILURE;
        }
    }

    /* Restore the value in the handle and delete the created Heap. */
    *handleAddr = temp;
    if(osal_OK == result)
    {
        status = HeapP_delete(handle);
        if(HeapP_OK != status)
        {
            result = osal_FAILURE;
        }
    }

    if(osal_OK == result)
    {
        OSAL_log("\n HeapP test for used parameter passed! \n");
    }
    else
    {
        OSAL_log("\n HeapP test for used parameter failed!! \n");
    }

    return result;
}

int32_t OsalApp_heapParamsInitNullTest(void)
{
    HeapP_Params *params = (HeapP_Params*)NULL_PTR;

    HeapP_Params_init(params);
    OSAL_log("\n HeapP_Params_init Nullcheck done!! \n");

    return osal_OK;
}

int32_t OsalApp_heapFreertosMaxTest(void)
{
    HeapP_Params      params;
    HeapP_Handle      handle, handles[OSAL_FREERTOS_CONFIGNUM_HEAP];
    HeapP_Status      status;
    uint32_t          heapInd;
    int32_t           result = osal_OK;
    
    memset(gOsalAppHeapPbuf, 0x00, sizeof(gOsalAppHeapPbuf));
    HeapP_Params_init(&params);
    
    params.buf = gOsalAppHeapPbuf;
    params.size = sizeof(gOsalAppHeapPbuf);
    
    /* Testing Maximum Heap creation */
    for(heapInd = 0U; heapInd < OSAL_FREERTOS_CONFIGNUM_HEAP; heapInd++)
    {
        handle = HeapP_create(&params);
        handles[heapInd] = handle;
        if(NULL == handle)
        {
            result = osal_FAILURE;
            break;
        }
    }

    if(osal_OK == result)
    {
        if(NULL != HeapP_create(&params))
        {
            result = osal_FAILURE;
        }
    }
    
    if(osal_OK == result)
    {
        for(heapInd = 0U; heapInd < OSAL_FREERTOS_CONFIGNUM_HEAP; heapInd++)
        {
            status = HeapP_delete(handles[heapInd]);
            if(HeapP_OK != status)
            {
                result = osal_FAILURE;
                break;
            }
        }
    }
    
    if(osal_OK != result)
    {
        OSAL_log("\n HeapP Freertos Max create test has failed! \n");
    }
    else
    {
        OSAL_log("\n HeapP Freertos Max create Test Passed!! \n");
    }

    return result;
}

int32_t OsalApp_heapFreertosTest(void)
{
    int32_t result = osal_OK;
    
    result += OsalApp_heapFreertosAllocTest();
    result += OsalApp_heapFreertosIsUsedTest();
    result += OsalApp_heapParamsInitNullTest();
    result += OsalApp_heapFreertosMaxTest();

    if(osal_OK != result)
    {
        OSAL_log("\n Some or all HeapP tests have failed! \n");
    }
    else
    {
        OSAL_log("\n All HeapP tests have Passed!! \n");
    }

    return result;
}

