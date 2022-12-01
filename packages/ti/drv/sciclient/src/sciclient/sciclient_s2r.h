/*
 *  Copyright (c) Texas Instruments Incorporated 2023
 *  All rights reserved.
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
 * \file sciclient_s2r.h
 *
 * \brief This file contains the declaration of the function to load and jump to
 * the suspend sequence.
 *
 */
#ifndef SCICLIENT_S2R_H_
#define SCICLIENT_S2R_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

/**
 * Debug messages for suspend can be enabled by uncommenting the following macro
 * and recompiling
 */
// #define DEBUG_SUSPEND
#ifdef DEBUG_SUSPEND
#include <ti/drv/uart/UART_stdio.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#ifdef DEBUG_SUSPEND
#define S2R_debugPrintf UART_printf
#else
#define S2R_debugPrintf(...) {};
#endif

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/**
 *  \brief S2R_goRetention Copy the suspend sequence to SRAM and jump to it.
 *
 *  \param core_resume_addr  BL31 resume address to save.
 */
void S2R_goRetention(uint32_t core_resume_addr);

#ifdef __cplusplus
}
#endif

#endif /* SCICLIENT_S2R_H_ */
