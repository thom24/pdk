/*
 *  Copyright (C) 2017-2018 Texas Instruments Incorporated
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

/**
 *  \file sciclient_rtos_main.c
 *
 *  \brief Implementation of tests for RTOS
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#if defined (__C7100__)
#include <xdc/std.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/System.h>
#include <ti/sysbios/family/c7x/Mmu.h>
#endif

#include <stdint.h>
#include <string.h>
#include <ti/csl/tistdtypes.h>
#include <ti/csl/soc.h>
#include <ti/csl/arch/csl_arch.h>
#include <ti/csl/hw_types.h>
#include <sciclient_appCommon.h>

#include <ti/osal/TaskP.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                         Structures and Enums                               */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                 Internal Function Declarations                             */
/* ========================================================================== */

void GetRevisionTest1(void* arg0, void *arg1);
void GetRevisionTest2(void* arg0, void *arg1);

void appReset(void);

/**
 * \brief   Set the interrupt config for C66x Timer interrupts.
 *
 * \param   None.
 *
 * \return  None
 */
void App_sciclientC66xIntrConfig(void);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

int32_t tsk1Pass = CSL_EFAIL, tsk2Pass = CSL_EFAIL;

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

void appReset(void)
{
}

int main(void)
{
    TaskP_Handle task1,task2;
    TaskP_Params taskParams1,taskParams2;

    App_SciclientC7xPreInit();

    uint32_t    retVal = CSL_PASS;

    TaskP_Params_init(&taskParams1);
    TaskP_Params_init(&taskParams2);
    taskParams1.priority = 14;
    taskParams2.priority = 15;

    App_sciclientConsoleInit();
    App_sciclientC66xIntrConfig();

    task1 = TaskP_create(GetRevisionTest1, &taskParams1);
    if (task1 == NULL)
    {
        App_sciclientPrintf("Task_create() GetRevisionTest1 failed!\n");
        OS_stop();
    }

    task2 = TaskP_create(GetRevisionTest2, &taskParams2);
    if (task2 == NULL)
    {
        App_sciclientPrintf("Task_create() GetRevisionTest2 failed!\n");
        OS_stop();
    }

    /* Start BIOS */
    OS_start();

    return retVal;
}

void GetRevisionTest1(void* arg0, void* arg1)
{
    int32_t status = CSL_PASS;
    volatile uint32_t loopForever = 1U;
    Sciclient_ConfigPrms_t        config =
    {
        SCICLIENT_SERVICE_OPERATION_MODE_POLLED,
        NULL
    };
    struct tisci_msg_version_req request;
    const Sciclient_ReqPrm_t      reqPrm =
    {
        TISCI_MSG_VERSION,
        TISCI_MSG_FLAG_AOP,
        (uint8_t *) &request,
        sizeof(request),
        SCICLIENT_SERVICE_WAIT_FOREVER
    };

    struct tisci_msg_version_resp response;
    Sciclient_RespPrm_t           respPrm =
    {
        0,
        (uint8_t *) &response,
        sizeof (response)
    };

    App_sciclientPrintf("SCIClient RTOS Test App1\n\n");
    status = Sciclient_init(&config);
    if (status == CSL_PASS)
    {
        status = Sciclient_service(&reqPrm, &respPrm);
        if (CSL_PASS == status)
        {
            if (respPrm.flags == TISCI_MSG_FLAG_ACK)
            {
                status = CSL_PASS;
                App_sciclientPrintf(
                                  " DMSC Firmware Version 1 %s\n",
                                  (char *) response.str);
                App_sciclientPrintf(
                                  " Firmware revision 0x%x\n", response.version);
                App_sciclientPrintf(
                                  " ABI revision %d.%d\n", response.abi_major,
                                  response.abi_minor);
            }
            else
            {
                App_sciclientPrintf(
                                  " DMSC Firmware Get Version failed \n");
            }
        }
        else
        {
            App_sciclientPrintf(
                              " DMSC Firmware Get Version failed \n");
        }
    }
    if (status == CSL_PASS)
    {
        status = Sciclient_deinit();
    }
    if (status == CSL_PASS)
    {
        tsk1Pass = CSL_PASS;
    }
    else
    {
        tsk1Pass = CSL_EFAIL;
    }
    if ((tsk1Pass == CSL_PASS) && (tsk2Pass == CSL_PASS))
    {
        App_sciclientPrintf("All tests have passed \n");
    }
    if (tsk1Pass != CSL_PASS)
    {
        App_sciclientPrintf("Some Tests have failed \n");
    }
    while (loopForever) {;}
}

void GetRevisionTest2(void* arg0, void*  arg1)
{
    int32_t status = CSL_PASS;
    volatile uint32_t loopForever = 1U;
    Sciclient_ConfigPrms_t        config =
    {
        SCICLIENT_SERVICE_OPERATION_MODE_POLLED,
        NULL
    };
    struct tisci_msg_version_req request;
    const Sciclient_ReqPrm_t      reqPrm =
    {
        TISCI_MSG_VERSION,
        TISCI_MSG_FLAG_AOP,
        (uint8_t *) &request,
        sizeof(request),
        SCICLIENT_SERVICE_WAIT_FOREVER
    };

    struct tisci_msg_version_resp response;
    Sciclient_RespPrm_t           respPrm =
    {
        0,
        (uint8_t *) &response,
        sizeof (response)
    };

    App_sciclientPrintf("SCIClient RTOS Test App2\n\n");
    status = Sciclient_init(&config);
    if (status == CSL_PASS)
    {
        status = Sciclient_service(&reqPrm, &respPrm);
        if (CSL_PASS == status)
        {
            if (respPrm.flags == TISCI_MSG_FLAG_ACK)
            {
                status = CSL_PASS;
                App_sciclientPrintf(
                                  " DMSC Firmware Version 2 %s\n",
                                  (char *) response.str);
                App_sciclientPrintf(
                                  " Firmware revision 0x%x\n", response.version);
                App_sciclientPrintf(
                                  " ABI revision %d.%d\n", response.abi_major,
                                  response.abi_minor);
                TaskP_sleep(100);
            }
            else
            {
                App_sciclientPrintf(
                                  " DMSC Firmware Get Version failed \n");
            }
        }
        else
        {
            App_sciclientPrintf(
                              " DMSC Firmware Get Version failed \n");
        }
    }
    if (status == CSL_PASS)
    {
        status = Sciclient_deinit();
    }
    if (status == CSL_PASS)
    {
        tsk2Pass = CSL_PASS;
    }
    else
    {
        tsk2Pass = CSL_EFAIL;
    }
    if ((tsk1Pass == CSL_PASS) && (tsk2Pass == CSL_PASS))
    {
        App_sciclientPrintf("All tests have passed \n");
    }
    if (tsk2Pass != CSL_PASS)
    {
        App_sciclientPrintf("Some Tests have failed \n");
    }
    while (loopForever) {;}
}

#if defined(BUILD_MPU) || defined (__C7100__)
extern void Osal_initMmuDefault(void);
void InitMmu(void)
{
    Osal_initMmuDefault();
}
#endif

