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

/**
 * \brief Macro to formulated device specific inst, port, pin configuration
 */
#define GPIO_DEVICE_CONFIG(INST, PORT, PIN)     (((INST)<<8U) | ((PORT)*8U) + (PIN))


/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/**
 *  @brief  GPIO device specific driver configuration structure
 */
typedef struct GPIO_v2_Config_s {
    /*! Pointer to the board's PinConfig array */
    GPIO_PinConfig *pinConfigs;

    /*! Pointer to the board's callback array */
    GPIO_CallbackFxn *callbacks;

    /*! Number of pin configs defined */
    uint32_t numberOfPinConfigs;

    /*! Number of callbacks defined */
    uint32_t numberOfCallbacks;

} GPIO_v2_Config;

/**
 * \brief       GPIO Hardware Attributes
 *
 * \details     The structure contains the hardware atrributes which are used
 *              to specify the platform specific configurations.
 */
typedef struct GPIO_v2_HwAttrs_t
{
    /**
     * \brief   GPIO registers base address
     */
    uint32_t    gpioBaseAddr;

    /**
     * \brief   Interrupt number associated with the high interrupts
     */
    uint8_t     highInterruptNum;

    /**
     * \brief   Interrupt number associated with the low interrupts
     */
    uint8_t     lowInterruptNum;
}GPIO_v2_HwAttrs;

/**
 * \brief       GPIO driver function pointer table
 */
extern const GPIO_FxnTable GPIO_FxnTable_v2;
/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */
/**
 * \brief   API to get the pointer to the GPIO Hardware Attributes for
 *          given the GPIO Instance. Implemented in SoC specific file.
 *
 * \param   gpioHwAttr      Pointer to hold the GPIO Hardware Attributes data.
 * \param   gpioInst        GPIO Instance
 *
 * \return  status value
 *              0        - Successful
 *              Non Zero - failure
 *
 */
int32_t GPIO_getHwAttr(GPIO_v2_HwAttrs** gpioHwAttr, uint32_t gpioInst);


#ifdef __cplusplus
}
#endif

#endif /* ti_drivers_GPIO__include */
