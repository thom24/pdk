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

/**
 *  \file   ESM_soc.c
 *
 *  \brief  TPR12 device specific hardware attributes.
 *
 */

/**************************************************************************
 *************************** Include Files ********************************
 **************************************************************************/

#include <ti/csl/soc.h>
#include <ti/drv/esm/soc/ESM_soc.h>

#include <ti/csl/soc/tpr12/src/cslr_mss_ctrl.h>
#include <ti/csl/soc/tpr12/src/cslr_dss_ctrl.h>

/**************************************************************************
 ************************** Global Variables ******************************
 **************************************************************************/

/**
 * @brief   This is TPR12 ESM platform configuration for MSS/DSS Subsystem.
 */
ESM_HwAttrs gESMHwCfgAttrs =
{
#if defined (__TI_ARM_V7R4__)
    (CSL_esmRegs*)CSL_MSS_ESM_U_BASE,
    CSL_MSS_INTR_MSS_ESM_HI,
    CSL_MSS_INTR_MSS_ESM_LO
#elif defined (_TMS320C6X)
    (CSL_esmRegs*)CSL_DSS_ESM_U_BASE,
    0,                                  /* Not used */
    0                                   /* Not used */
#else
#error "TPR12 ESM: unsupported core!~
#endif
};

/**************************************************************************
 ************************* Functions **************************************
 **************************************************************************/
/**
 *  @b Description
 *  @n
 *      This function is used to gate/ungate an ESM error
 *
 *  @param[in]  groupNumber
 *      Group number for the ESM error
 *  @param[in]  errorNumber
 *      ESM error number
 *  @param[in]  gating
 *      Gating of the ESM error: 0 - ungating; 1 - gating
  *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
#define MASK  (0xFU)
#define SHIFT (0x4U)
#define COUNT (0x8U)
int32_t ESM_socConfigErrorGating(uint8_t groupNumber, uint8_t errorNumber, uint8_t gating)
{
    uint32_t regVal;
    uint32_t regIndex;

    if (groupNumber != 2)
    {
        return -1;
    }

#if defined (__TI_ARM_V7R4__)
    CSL_mss_ctrlRegs* ptrCtrlRegs = (CSL_mss_ctrlRegs*)CSL_MSS_CTRL_U_BASE;
#elif defined (_TMS320C6X)
    CSL_mss_ctrlRegs* ptrCtrlRegs = (CSL_mss_ctrlRegs*)CSL_DSS_CTRL_U_BASE;
#else
#error "TPR12 ESM: unsupported core!~
#endif

    regIndex = errorNumber / COUNT;
    switch (regIndex)
    {
        case 0:
            regVal = CSL_REG_RD(&ptrCtrlRegs->ESM_GATING0);
            regVal &= ~(MASK << (SHIFT*(errorNumber % COUNT)));
            if (gating)
            {
                regVal |= (MASK << (SHIFT*(errorNumber % COUNT)));
            }
            CSL_REG_WR(&ptrCtrlRegs->ESM_GATING0, regVal);
            break;
        case 1:
            errorNumber -= COUNT*regIndex;
            regVal = CSL_REG_RD(&ptrCtrlRegs->ESM_GATING1);
            regVal &= ~(MASK << (SHIFT*(errorNumber % COUNT)));
            if (gating)
            {
                regVal |= (MASK << (SHIFT*(errorNumber % COUNT)));
            }
            CSL_REG_WR(&ptrCtrlRegs->ESM_GATING1, regVal);
            break;
         case 2:
            errorNumber -= COUNT*regIndex;
            regVal = CSL_REG_RD(&ptrCtrlRegs->ESM_GATING2);
            regVal &= ~(MASK << (SHIFT*(errorNumber % COUNT)));
            if (gating)
            {
                regVal |= (MASK << (SHIFT*(errorNumber % COUNT)));
            }
            CSL_REG_WR(&ptrCtrlRegs->ESM_GATING2, regVal);
            break;
          case 3:
            errorNumber -= COUNT*regIndex;
            regVal = CSL_REG_RD(&ptrCtrlRegs->ESM_GATING3);
            regVal &= ~(MASK << (SHIFT*(errorNumber % COUNT)));
            if (gating)
            {
                regVal |= (MASK << (SHIFT*(errorNumber % COUNT)));
            }
            CSL_REG_WR(&ptrCtrlRegs->ESM_GATING3, regVal);
            break;
          default:
            return -1;
    }

    return 0;
}

