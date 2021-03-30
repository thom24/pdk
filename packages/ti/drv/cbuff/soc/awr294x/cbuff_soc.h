/*
 *  Copyright (c) Texas Instruments Incorporated 2021
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
 *  \file cbuff_soc.h
 *
 *       CBUFF Low Level Driver SOC specific file.
 *
 */

#ifndef CBUFF_SOC_H_
#define CBUFF_SOC_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#define NUM_RX_CHANNEL                                          (4U)

/* This is the size of the Chirp Parameters (CP) in CBUFF Units */
#define CP_SIZE_CBUFF_UNITS                                     (2U)


/*************************************************************
 * BASE Address used in the Single Chirp Mode to get the
 * Chirp Profile Data
 *************************************************************/
#define EDMA3_DSS_CP0_BASE                                      (0x05020C08U)
#define EDMA3_DSS_CP1_BASE                                      (0x05020C18U)

/*************************************************************
 * BASE Address used in the Multiple Chirp Mode to get the
 * Chirp Profile Data
 *************************************************************/
#define EDMA3_DSS_CHIRP0_BASE                                   (0x05020C48U)
#define EDMA3_DSS_CHIRP1_BASE                                   (0x05020C88U)
#define EDMA3_DSS_CHIRP2_BASE                                   (0x05020CC8U)
#define EDMA3_DSS_CHIRP3_BASE                                   (0x05020D08U)
#define EDMA3_DSS_CHIRP4_BASE                                   (0x05020D48U)
#define EDMA3_DSS_CHIRP5_BASE                                   (0x05020D88U)
#define EDMA3_DSS_CHIRP6_BASE                                   (0x05020DC8U)
#define EDMA3_DSS_CHIRP7_BASE                                   (0x05020E08U)


/**
 * @brief
 *  SOC System Interrupt Listener Function
 */
typedef void (*SOC_SysIntListenerFxn)(uintptr_t arg);

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/**
 * @brief
 *  SOC Interrupt Listener Configuration
 *
 * @details
 *  The structure describes the configuration information which is need to initialize
 *  the system interrupt listener
 */
typedef struct SOC_SysIntListenerCfg_t
{
    /**
     * @brief
     *  System interrupt on which the listener is to be registered
     */
    uint32_t                systemInterrupt;

    /**
     * @brief
     *  Listener Function to be registered
     */
    SOC_SysIntListenerFxn   listenerFxn;

    /**
     * @brief
     *  Optional argument with which the listener function is to be invoked on
     *  an ISR
     */
    uintptr_t               arg;
}SOC_SysIntListenerCfg;

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/**
 * @brief
 *  SOC System Interrupt Listener Handle
 */
typedef void*   SOC_SysIntListenerHandle;

/* ========================================================================== */
/*                       Static Function Definitions                          */
/* ========================================================================== */

/* None */

#ifdef __cplusplus
}
#endif

#endif /* #ifndef CBUFF_SOC_H_ */
