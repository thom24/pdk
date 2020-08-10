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

#ifndef DRV_GPIO_V2_PRIV_H
#define DRV_GPIO_V2_PRIV_H

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

/* None. */

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */
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

/**
 * \brief   API to parse the GPIO index passed from application.
 *          Get the GPIO instance, port and pin numbers. Get the baseaddress
 *          of the corresponding GPIO module.
 *
 * \param   index           GPIO index
 * \param   baseAddr        GPIO module base address corresponding to index.
 * \param   inst            GPIO module instance corresponding to index.
 * \param   port            GPIO port number corresponding to index.
 * \param   pin             GPIO pin number corresponding to index.
 *
 * \return  status value
 *              0        - Successful
 *              Non Zero - failure
 *
 */
int32_t GPIO_parseIndex(uint32_t index, uint32_t* baseAddr, uint32_t* inst, uint32_t* port, uint32_t* pin);

/**
 * \brief   API to get the pin configuration
 *
 * \param   index           GPIO index
 * \param   pinConfig       pin Config value corresponding to index.
 *
 * \return  status value
 *              0        - Successful
 *              Non Zero - failure
 *
 */
int32_t GPIO_v2_getConfig(uint32_t index, GPIO_PinConfig *pinConfig);

/**
 * \brief   API to set the pin configuration
 *
 * \param   index           GPIO index
 * \param   pinConfig       pin Config value corresponding to index.
 *
 * \return  status value
 *              0        - Successful
 *              Non Zero - failure
 *
 */
int32_t GPIO_v2_setConfig(uint32_t index, GPIO_PinConfig pinConfig);

/**
 * \brief   API to get the callback function
 *
 * \param   index           GPIO index
 * \param   pinConfig       callback function corresponding to index.
 *
 * \return  status value
 *              0        - Successful
 *              Non Zero - failure
 *
 */
int32_t GPIO_v2_getCallback(uint32_t index, GPIO_CallbackFxn *callback);

/**
 * \brief   API to set the callback function
 *
 * \param   index           GPIO index
 * \param   pinConfig       callback function corresponding to index.
 *
 * \return  status value
 *              0        - Successful
 *              Non Zero - failure
 *
 */
int32_t GPIO_v2_setCallback(uint32_t index, GPIO_CallbackFxn callback);

#ifdef __cplusplus
}
#endif

#endif /* ti_drivers_GPIO__include */
