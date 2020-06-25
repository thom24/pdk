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

#include <ti/csl/soc.h>
#include <ti/drv/esm/soc/ESM_soc.h>

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
