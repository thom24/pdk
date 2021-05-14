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
 *  \file awr294x/GPIO_soc.h
 *
 *  \brief GPIO Driver SOC specific file for awr294x.
 */

#ifndef AWR294X_GPIO_SOC_H_
#define AWR294X_GPIO_SOC_H_

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
/*! \brief Last Gpio INST */
#define GPIO_INST_MAX                   (GPIO_INST_MCU)
/*! \brief Invalid Gpio INST */
#define GPIO_INST_INVALID               (0xFFU)

/** @}*/ /* end defgroup GPIO_INSTANCE */

/**
 * @brief
 *  This is the maximum number of ports per GPIO instance
 */
#define GPIO_MAX_PORT                   (4U)

#define GPIO_ALL_INST_MAX_PINS          ((GPIO_INST_MAX + 1) * GPIO_MAX_PORT * GPIO_MAX_PINS_PER_PORT)
/**
 * \brief Macro to create index value at compile time.
 * Note: This doesnot have any error checking.
 * The logic should be same as in function GPIO_createIndex
 */
#define GPIO_CREATE_INDEX(INST, PORT, PIN)     (((INST) * (GPIO_MAX_PORT) * (GPIO_MAX_PINS_PER_PORT)) + ((PORT)* (GPIO_MAX_PINS_PER_PORT)) + (PIN))

/**
 * \brief Gpio Index macros for AWR294X.
 */
/* MCU GPIO Port 0 (A) */
#define SOC_AWR294X_GPIO_0        GPIO_CREATE_INDEX (GPIO_INST_MCU, 0, 0)
#define SOC_AWR294X_GPIO_1        GPIO_CREATE_INDEX (GPIO_INST_MCU, 0, 1)
#define SOC_AWR294X_GPIO_2        GPIO_CREATE_INDEX (GPIO_INST_MCU, 0, 2)
#define SOC_AWR294X_GPIO_3        GPIO_CREATE_INDEX (GPIO_INST_MCU, 0, 3)
#define SOC_AWR294X_GPIO_4        GPIO_CREATE_INDEX (GPIO_INST_MCU, 0, 4)
#define SOC_AWR294X_GPIO_5        GPIO_CREATE_INDEX (GPIO_INST_MCU, 0, 5)
#define SOC_AWR294X_GPIO_6        GPIO_CREATE_INDEX (GPIO_INST_MCU, 0, 6)
#define SOC_AWR294X_GPIO_7        GPIO_CREATE_INDEX (GPIO_INST_MCU, 0, 7)

/* MCU GPIO Port 1 (B) */
#define SOC_AWR294X_GPIO_8        GPIO_CREATE_INDEX (GPIO_INST_MCU, 1, 0)
#define SOC_AWR294X_GPIO_9        GPIO_CREATE_INDEX (GPIO_INST_MCU, 1, 1)
#define SOC_AWR294X_GPIO_10       GPIO_CREATE_INDEX (GPIO_INST_MCU, 1, 2)
#define SOC_AWR294X_GPIO_11       GPIO_CREATE_INDEX (GPIO_INST_MCU, 1, 3)
#define SOC_AWR294X_GPIO_12       GPIO_CREATE_INDEX (GPIO_INST_MCU, 1, 4)
#define SOC_AWR294X_GPIO_13       GPIO_CREATE_INDEX (GPIO_INST_MCU, 1, 5)
#define SOC_AWR294X_GPIO_14       GPIO_CREATE_INDEX (GPIO_INST_MCU, 1, 6)
#define SOC_AWR294X_GPIO_15       GPIO_CREATE_INDEX (GPIO_INST_MCU, 1, 7)

/* MCU GPIO Port 2 (C) */
#define SOC_AWR294X_GPIO_16       GPIO_CREATE_INDEX (GPIO_INST_MCU, 2, 0)
#define SOC_AWR294X_GPIO_17       GPIO_CREATE_INDEX (GPIO_INST_MCU, 2, 1)
#define SOC_AWR294X_GPIO_18       GPIO_CREATE_INDEX (GPIO_INST_MCU, 2, 2)
#define SOC_AWR294X_GPIO_19       GPIO_CREATE_INDEX (GPIO_INST_MCU, 2, 3)
#define SOC_AWR294X_GPIO_20       GPIO_CREATE_INDEX (GPIO_INST_MCU, 2, 4)
#define SOC_AWR294X_GPIO_21       GPIO_CREATE_INDEX (GPIO_INST_MCU, 2, 5)
#define SOC_AWR294X_GPIO_22       GPIO_CREATE_INDEX (GPIO_INST_MCU, 2, 6)
#define SOC_AWR294X_GPIO_23       GPIO_CREATE_INDEX (GPIO_INST_MCU, 2, 7)

/* MCU GPIO Port 3 (D) */
#define SOC_AWR294X_GPIO_24       GPIO_CREATE_INDEX (GPIO_INST_MCU, 3, 0)
#define SOC_AWR294X_GPIO_25       GPIO_CREATE_INDEX (GPIO_INST_MCU, 3, 1)
#define SOC_AWR294X_GPIO_26       GPIO_CREATE_INDEX (GPIO_INST_MCU, 3, 2)
#define SOC_AWR294X_GPIO_27       GPIO_CREATE_INDEX (GPIO_INST_MCU, 3, 3)
#define SOC_AWR294X_GPIO_28       GPIO_CREATE_INDEX (GPIO_INST_MCU, 3, 4)
#define SOC_AWR294X_GPIO_29       GPIO_CREATE_INDEX (GPIO_INST_MCU, 3, 5)
#define SOC_AWR294X_GPIO_30       GPIO_CREATE_INDEX (GPIO_INST_MCU, 3, 6)
#define SOC_AWR294X_GPIO_31       GPIO_CREATE_INDEX (GPIO_INST_MCU, 3, 7)


#define SOC_AWR294X_GPIO_MIN      (SOC_AWR294X_GPIO_0)
#define SOC_AWR294X_GPIO_MAX      (SOC_AWR294X_GPIO_31)

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

inline void GPIO_decodeIndex(uint32_t index, uint32_t* inst, uint32_t* port, uint32_t* pin);

/* ========================================================================== */
/*                       Static Function Definitions                          */
/* ========================================================================== */

inline void GPIO_decodeIndex(uint32_t index, uint32_t* inst, uint32_t* port, uint32_t* pin)
{
    uint32_t pinOffset;
    *inst = index / (GPIO_MAX_PORT * GPIO_MAX_PINS_PER_PORT);
    pinOffset = index % (GPIO_MAX_PORT * GPIO_MAX_PINS_PER_PORT);
    *port = pinOffset / GPIO_MAX_PINS_PER_PORT;
    *pin  = pinOffset % GPIO_MAX_PINS_PER_PORT;
}

#ifdef __cplusplus
}
#endif

#endif /* #ifndef AWR294X_GPIO_SOC_H_ */
