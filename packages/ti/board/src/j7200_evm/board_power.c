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
 *  \file   board_power.c
 *
 *  \brief  Implements functions to control the power on the board.
 *
 */

#include <ti/csl/cslr_device.h>

#include <ti/drv/i2c/I2C.h>
#include <ti/drv/i2c/soc/I2C_soc.h>
#include <ti/osal/osal.h>
#include <ti/board/board.h>
#include <ti/board/src/devices/common/common.h>
#include "board_internal.h"
#include "board_power.h"
#include "board_utils.h"

/**
 *  \brief    Function to configure SD card voltage control gpio configuration.
 *
 *  \param    gpioValue [IN] GPIO pin value.
 *            1 for GPIO pin high
 *            0 for GPIO pin low
 *
 *  \return   BOARD_SOK in case of success or appropriate error code
 *
 */
static void Board_sdVoltageCtrlGpioCfg(uint8_t gpioValue)
{
    uint32_t regVal;

    /* Setting the GPIO direction to output */
    regVal = HW_RD_REG32(CSL_GPIO0_BASE + 0x38);
    regVal &= ~(0x01 << (BOARD_SDIO_1V8_EN_PIN_NUM % 32));
    HW_WR_REG32((CSL_GPIO0_BASE + 0x38), regVal);

    /* Setting the GPIO value */
    regVal = HW_RD_REG32(CSL_GPIO0_BASE + 0x40);

    if(gpioValue == 0)
    {
        regVal &= ~(0x01 << (BOARD_SDIO_1V8_EN_PIN_NUM % 32));
        HW_WR_REG32((CSL_GPIO0_BASE + 0x40), regVal);
    }
    else
    {
        regVal |= (gpioValue << (BOARD_SDIO_1V8_EN_PIN_NUM % 32));
        HW_WR_REG32((CSL_GPIO0_BASE + 0x40), regVal);
    }
}

/**
 *  \brief    Function to configure SD card voltage.
 *
 *  \param vsel [IN] SD voltage selection. 0 for 1.8v, 1 for 3.3v
 *
 *  \return   BOARD_SOK in case of success or appropriate error code
 *
 */
Board_STATUS Board_pmSdVoltageCtrl(uint8_t vsel)
{
    Board_sdVoltageCtrlGpioCfg(vsel);

    return BOARD_SOK;
}
