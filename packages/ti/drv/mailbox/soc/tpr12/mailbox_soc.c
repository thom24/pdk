/*
 *  Copyright (c) Texas Instruments Incorporated 2020
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
 */

/**
 *   \file mailbox_soc.c
 *
 *   \brief The file implements the Platform specific Mailbox Driver Interface
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdint.h>
#include <ti/csl/soc.h>
#include <ti/drv/mailbox/mailbox.h>
#include <ti/drv/mailbox/src/mailbox_internal.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

#if defined (__TI_ARM_V7R4__)
CSL_mboxRegAddr gMboxReg =
{
    .memInit        = (uintptr_t)(CSL_MSS_CTRL_U_BASE + CSL_MSS_CTRL_MSS_MAILBOX_MEM_INIT),
    .memInitDone    = (uintptr_t)(CSL_MSS_CTRL_U_BASE + CSL_MSS_CTRL_MSS_MAILBOX_MEM_INIT_DONE),
    .memInitStatus  = (uintptr_t)(CSL_MSS_CTRL_U_BASE + CSL_MSS_CTRL_MSS_MAILBOX_MEM_INIT_STATUS),
    .mboxWriteDone  = (uintptr_t)(CSL_MSS_CTRL_U_BASE + CSL_MSS_CTRL_MSS_CR5A_MBOX_WRITE_DONE),
    .mboxReadReq    = (uintptr_t)(CSL_MSS_CTRL_U_BASE + CSL_MSS_CTRL_MSS_CR5A_MBOX_READ_REQ),
    .mboxReadDone   = (uintptr_t)(CSL_MSS_CTRL_U_BASE + CSL_MSS_CTRL_MSS_CR5A_MBOX_READ_DONE)
};


/**
 * @brief   This is SOC specific configuration and should *NOT* be modified by the customer.
 *          Communication between MSS and DSS (MSS master).
 */
Mailbox_HwCfg   gMailboxMssDssHwCfg =
{
    .mbxReg                 = &gMboxReg,
    .baseLocalToRemote.data = (uint8_t *)(CSL_DSS_MAILBOX_U_BASE),
    .baseRemoteToLocal.data = (uint8_t *)(CSL_MSS_MBOX_U_BASE),
    .boxFullIntNum          = CSL_MSS_INTR_MSS_CR5A_MBOX_READ_REQ,
    .boxEmptyIntNum         = CSL_MSS_INTR_MSS_CR5A_MBOX_READ_ACK,
    .remoteProcNum          = MAILBOX_INST_DSP
};
#endif

#if defined (_TMS320C6X)
CSL_mboxRegAddr gMboxReg =
{
    .memInit        = (uintptr_t)(CSL_DSS_CTRL_U_BASE + CSL_DSS_CTRL_DSS_MAILBOX_MEMINIT_START),
    .memInitDone    = (uintptr_t)(CSL_DSS_CTRL_U_BASE + CSL_DSS_CTRL_DSS_MAILBOX_MEMINIT_DONE),
    .memInitStatus  = (uintptr_t)(CSL_DSS_CTRL_U_BASE + CSL_DSS_CTRL_DSS_MAILBOX_MEMINIT_STATUS),
    .mboxWriteDone  = (uintptr_t)(CSL_DSS_CTRL_U_BASE + CSL_DSS_CTRL_DSS_DSP_MBOX_WRITE_DONE),
    .mboxReadReq    = (uintptr_t)(CSL_DSS_CTRL_U_BASE + CSL_DSS_CTRL_DSS_DSP_MBOX_READ_REQ),
    .mboxReadDone   = (uintptr_t)(CSL_DSS_CTRL_U_BASE + CSL_DSS_CTRL_DSS_DSP_MBOX_READ_DONE),
};


/**
 * @brief   This is SOC specific configuration and should *NOT* be modified by the customer.
 *          Communication between MSS and DSS (MSS master).
 */
Mailbox_HwCfg   gMailboxDssMssHwCfg =
{
    .mbxReg                 = &gMboxReg,
    .baseLocalToRemote.data = (uint8_t *)(CSL_MSS_MBOX_U_BASE),
    .baseRemoteToLocal.data = (uint8_t *)(CSL_DSS_MAILBOX_U_BASE),
    .boxFullIntNum          = CSL_DSS_INTR_DSS_DSP_MBOX_READ_REQ,
    .boxEmptyIntNum         = CSL_DSS_INTR_DSS_DSP_MBOX_READ_ACK,
    .remoteProcNum          = MAILBOX_INST_MSS_CR5A
};
#endif

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

int32_t Mailbox_validateLocalEndPoint(Mailbox_Instance localEndpoint)
{
    int32_t retVal = MAILBOX_SOK;
    /* Validate local End point based on the Core. */
#if defined (BUILD_MCU1_0)
    if (localEndpoint != MAILBOX_INST_MSS_CR5A)
    {
        retVal = MAILBOX_EINVAL;
    }
#elif defined (BUILD_DSP_1)
    if (localEndpoint != MAILBOX_INST_DSP)
    {
        retVal = MAILBOX_EINVAL;
    }
#else
    /* Currently not supported for other cores. */
    retVal = MAILBOX_EINVAL;
#endif
    return retVal;
}

int32_t Mailbox_validateRemoteEndPoint(Mailbox_Instance localEndpoint, Mailbox_Instance remoteEndpoint)
{
    int32_t retVal = MAILBOX_SOK;
    if ((remoteEndpoint == localEndpoint) || (remoteEndpoint > MAILBOX_INST_LAST))
    {
        retVal = MAILBOX_EINVAL;
    }
    return retVal;
}

int32_t Mailbox_isMultiChannelSupported(Mailbox_Instance localEndpoint, Mailbox_Instance remoteEndpoint)
{
    int32_t retVal = MAILBOX_EINVAL;
#if defined (BUILD_MCU1_0)
    /* Multichannel supported only between DSS and MSS. */
    if ((localEndpoint == MAILBOX_INST_MSS_CR5A) && (remoteEndpoint == MAILBOX_INST_DSP))
    {
        retVal = MAILBOX_SOK;
    }
#elif defined (BUILD_DSP_1)
    /* Multichannel supported only between DSS and MSS. */
    if ((localEndpoint == MAILBOX_INST_DSP) && (remoteEndpoint == MAILBOX_INST_MSS_CR5A))
    {
        retVal = MAILBOX_EINVAL;
    }
#else
    /* Currently not supported for other cores. */
    retVal = MAILBOX_EINVAL;
#endif
    return retVal;
}

/* TODO: Proc Num itself can be used as Mailbox type. */
int32_t getMailboxTypeFromProcNum(uint32_t procNum, Mailbox_Instance *remoteEndpoint)
{
    int32_t retVal = MAILBOX_SOK;

    switch (procNum)
    {
        case MAILBOX_INST_MSS_CR5A:
            *remoteEndpoint = MAILBOX_INST_MSS_CR5A;
            break;
        case MAILBOX_INST_DSP:
            *remoteEndpoint = MAILBOX_INST_DSP;
            break;
        default:
            retVal = MAILBOX_EINVAL;
    }
    return retVal;
}

int32_t Mailbox_getBoxFullRemoteInst(Mailbox_Instance *remoteEndpoint)
{
    uint32_t regVal;
    int32_t retVal = MAILBOX_EINVAL;
    uint32_t i;

    regVal = CSL_Mbox_getBoxFullIntr(&gMboxReg);
    if (regVal != 0)
    {
        for (i=0; i<MAILBOX_INST_LAST; i++)
        {
            uint32_t bits = (i << 2U);
            if ((regVal & (1 << bits)) != 0U)
            {
                retVal = getMailboxTypeFromProcNum(i, remoteEndpoint);
                break;
            }
        }
    }
    return retVal;
}

int32_t Mailbox_getBoxEmptyRemoteInst(Mailbox_Instance *remoteEndpoint)
{
    uint32_t regVal;
    int32_t retVal = MAILBOX_EINVAL;
    uint32_t i;

    regVal = CSL_Mbox_getBoxEmptyIntr(&gMboxReg);
    if (regVal != 0)
    {
        for (i=0; i<MAILBOX_INST_LAST; i++)
        {
            uint32_t bits = (i << 2U);
            if ((regVal & (1 << bits)) != 0U)
            {
                retVal = getMailboxTypeFromProcNum(i, remoteEndpoint);
                break;
            }
        }
    }
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      The function configures the hardware base addresses and interrupt numbers
 *      It populates the driver with hard coded information derived from the platform
 *      files.
 *
 *  @param[in]  driver handle
 *
 *
 *  @retval
 *      Not applicable
 *
 *  \ingroup MAILBOX_DRIVER_INTERNAL_FUNCTION
 *
 */
Mailbox_HwCfg* Mailbox_getHwCfg(Mailbox_Instance remoteEndpoint)
{
    Mailbox_HwCfg *hwCfg = NULL;
#if defined (__TI_ARM_V7R4__)
    /*This is local endpoint MSS*/
    if(remoteEndpoint == MAILBOX_INST_DSP)
    {
        hwCfg = &gMailboxMssDssHwCfg;
    }
#endif

#if defined (_TMS320C6X)
    /* This is local endpoint DSS */
    if(remoteEndpoint == MAILBOX_INST_MSS_CR5A)
    {
        hwCfg = &gMailboxDssMssHwCfg;
    }
#endif
    return hwCfg;
}

int32_t Mailbox_initHw(Mbox_Handle handle)
{
    Mailbox_Driver*     driver;
    int32_t             retVal = MAILBOX_SOK;

    driver = (Mailbox_Driver*)handle;

    if (handle == NULL)
    {
        retVal = MAILBOX_EINVAL;
    }

    if (retVal == MAILBOX_SOK)
    {
        /* Clear interrupt status register. */
        CSL_Mbox_clearTxAckInterrupt(driver->hwCfg->mbxReg, driver->hwCfg->remoteProcNum);

        /* Init mailbox. */
        CSL_Mbox_memInit(driver->hwCfg->mbxReg);

        while (CSL_Mbox_isMeminitDone(driver->hwCfg->mbxReg) != 0x01)
        {
            /* Wait till init is complete. */
        }
    }
    return retVal;
}
