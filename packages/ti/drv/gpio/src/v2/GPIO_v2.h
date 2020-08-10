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

#ifndef DRV_GPIO_V2_H
#define DRV_GPIO_V2_H

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdint.h>
#include <ti/drv/gpio/GPIO.h>
#include <ti/csl/csl_gpio.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/**
 * \brief
 *  Only Ports A, B, C and D are capable of interrupts.
 *  This is a system limit on the GPIO IP Block.
 */
#define GPIO_MAX_INTERRUPT_PORT         4U

/**
 * \brief
 *  This is the maximum number of pins per GPIO port
 *  This is a system limit on the GPIO IP Block.
 */
#define GPIO_MAX_PINS_PER_PORT          8U

/**
 * \brief
 *  This is the maximum number of interrupts which can be supported.
 *  Only Ports A, B, C and D are capable of interrupts. Also since
 *  each port can have a maximum of 8 pins this is the maximum number
 *  of interrupts which can be supported
 *
 *  This is a system limit on the GPIO IP Block.
 */
#define GPIO_MAX_INTERRUPT              (GPIO_MAX_INTERRUPT_PORT * GPIO_MAX_PINS_PER_PORT)

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */
/**
 * \brief Pin configuration for each of the GPIO pins.
 */
typedef struct GPIO_v2_PinConfig_s {
    /* GPIO pin index number. as defined in soc specific header file. */
    uint32_t            pinIndex;
    /* GPIO pin configuration settings. */
    GPIO_PinConfig      pinConfig;
    /* Callback function pointer for the GPIO pin. */
    GPIO_CallbackFxn    callback;
}GPIO_v2_PinConfig;


/**
 * \brief configuration structure to initialize the GPIO pins.
 *         This is used for updating the configuration for multiple GPIO pins by
 *         calling the GPIO_v2_updateConfig API.
 */
typedef struct GPIO_v2_Config_s {
    /* Pointer to pin configuration array. */
    GPIO_v2_PinConfig  *pinConfigs;
    /* Number of elements in the pin config array. */
    uint32_t            numPinConfig;
}GPIO_v2_Config;

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/**
 * \brief   API to update the pin configuration. Updates configuration for
 *          multiple pins at once. Can be used to initialize default pin values
 *          for all pins used in the board.
 *          Alternatively GPIO_setConfigFxn and GPIO_setCallbackFxn APIs can be
 *          called for individual pins.
 *
 * \param   gpioV2Config    GPIO configuration structure to update.
 *
 * \return  status value
 *              0        - Successful
 *              Non Zero - failure
 *
 */
int32_t GPIO_v2_updateConfig(GPIO_v2_Config *gpioV2Config);

#ifdef __cplusplus
}
#endif

#endif /* ti_drivers_GPIO__include */
