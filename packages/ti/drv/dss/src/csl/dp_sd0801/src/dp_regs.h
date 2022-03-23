/* parasoft-begin-suppress MISRA2012-RULE-1_1_a_c99-2 "C99 - limits, DRV-4754" parasoft-begin-suppress MISRA2012-RULE-1_1_a_c90-2 "C90 - limits, DRV-4754" */
/* parasoft-begin-suppress item MISRA2012-DIR-4_8 "Consider hiding implementation of structure, DRV-3667" */

/**********************************************************************
* Copyright (C) 2012-2022 Cadence Design Systems, Inc.
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
*
* 1. Redistributions of source code must retain the above copyright
* notice, this list of conditions and the following disclaimer.
* 2. Redistributions in binary form must reproduce the above copyright
* notice, this list of conditions and the following disclaimer in the
* documentation and/or other materials provided with the distribution.
* 3. Neither the name of the copyright holder nor the names of its
* contributors may be used to endorse or promote products derived from
* this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
* INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
* CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*
* THIS FILE IS AUTOMATICALLY GENERATED, DO NOT EDIT
*
**********************************************************************/

#ifndef REG_DP_REGS_H_
#define REG_DP_REGS_H_

#include <cps_drv.h>

#include "dp_regs_macros.h"

typedef struct __attribute__((packed)) DP_Regs_s {
    struct {
        volatile char pad__0[0x30A00U];
        volatile uint32_t AUX_CONFIG_p;
        volatile uint32_t AUX_CTRL_p;
        volatile uint32_t AUX_ATBSEL_p;
        volatile uint32_t AUX_TESTMODE_CTL_p;
        volatile uint32_t AUX_TESTMODE_ST_p;
        volatile char pad__1[0xCU];
        volatile uint32_t PHY_RESET_p;
        volatile uint32_t PMA_PLLCLK_EN_p;
        volatile uint32_t PMA_PLLCLK_EN_ACK_p;
        volatile uint32_t PMA_POWER_STATE_REQ_p;
        volatile uint32_t PMA_POWER_STATE_ACK_p;
        volatile uint32_t PMA_CMN_READY_p;
        volatile uint32_t PMA_TX_VMARGIN_p;
        volatile uint32_t PMA_TX_DEEMPH_p;
    } dp_regs;
} DP_Regs;

#endif /* REG_DP_REGS_H_ */

/* parasoft-end-suppress MISRA2012-RULE-1_1_a_c99-2 "C99 - limits, DRV-4754" */
/* parasoft-end-suppress MISRA2012-RULE-1_1_a_c90-2 "C90 - limits, DRV-4754" */
/* parasoft-end-suppress MISRA2012-DIR-4_8 "Consider hiding implementation of structure, DRV-3667" */

