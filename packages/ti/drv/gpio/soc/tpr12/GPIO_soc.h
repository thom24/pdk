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
 *  \file tpr12/GPIO_soc.h
 *
 *  \brief GPIO Driver SOC specific file for tpr12.
 */

#ifndef TPR12_GPIO_SOC_H_
#define TPR12_GPIO_SOC_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */
/*! @brief Gpio Instance Number */
typedef uint32_t GPIO_Instance;

/** @defgroup GPIO_INSTANCE GPIO driver Instance IDs
\ingroup DRV_GPIO_MODULE
 *
 @{ */

#define GPIO_INST_MIN                   (0U)
/*! \brief MCU GPIO INST */
#define GPIO_INST_MCU                   (GPIO_INST_MIN)
/*! \brief RCSS Gpio INST */
#define GPIO_INST_RCSS                  (1U)
/*! \brief Last Gpio INST */
#define GPIO_INST_MAX                   (GPIO_INST_RCSS)
/*! \brief Invalid Gpio INST */
#define GPIO_INST_INVALID               (0xFFU)

/** @}*/ /* end defgroup GPIO_INSTANCE */

/**
 * @brief
 *  This is the maximum number of ports per GPIO instance
 */
#define GPIO_MAX_PORT                   (8U)

/**
 * \brief Macro to create index value at compile time.
 * Note: This doesnot have any error checking.
 * The logic should be same as in function GPIO_createIndex
 */
#define GPIO_CREATE_INDEX(INST, PORT, PIN)     (((INST)<<8U) | ((PORT)*8U) + (PIN))

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                       Static Function Definitions                          */
/* ========================================================================== */

/* None */

#ifdef __cplusplus
}
#endif

#endif /* #ifndef TPR12_GPIO_SOC_H_ */
