/******************************************************************************
 * Copyright (c) 2020 Texas Instruments Incorporated - http://www.ti.com
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
 *
 *****************************************************************************/

/**
 *  \file   board_control.c
 *
 *  \brief  Implements multiple board control functions
 *
 */

#include "board_control.h"

/**
 * \brief   Configures IO expander pin level
 *
 * \param   *cfg    structure to configure IO expander
 *
 * \return  Board_SOK in case of success or appropriate error code.
 *
 */
static Board_STATUS Board_setIoExpPinOutput(Board_IoExpCfg_t *cfg)
{
    Board_I2cInitCfg_t i2cCfg;
    Board_STATUS status;

    i2cCfg.i2cInst    = cfg->i2cInst;
    i2cCfg.socDomain  = cfg->socDomain;
    i2cCfg.enableIntr = cfg->enableIntr;
    Board_setI2cInitConfig(&i2cCfg);

    status = Board_i2cIoExpInit();
    if(status == BOARD_SOK)
    {
        /* Setting the pin direction as output */
        status = Board_i2cIoExpSetPinDirection(cfg->slaveAddr,
                                               cfg->ioExpType,
                                               cfg->portNum,
                                               cfg->pinNum,
                                               PIN_DIRECTION_OUTPUT);
        BOARD_delay(1000);
        /* Pulling the hdmi power pin to low */
        status |= Board_i2cIoExpPinLevelSet(cfg->slaveAddr,
                                            cfg->ioExpType,
                                            cfg->portNum,
                                            cfg->pinNum,
                                            cfg->signalLevel);
        BOARD_delay(1000);

        Board_i2cIoExpDeInit();
    }

    return status;
}

/**
 * \brief   Configures CPSW5G mux to high
 *
 * \return  Board_SOK in case of success or appropriate error code.
 *
 */
static Board_STATUS Board_setCpsw5GMdioMux(void)
{
    Board_IoExpCfg_t ioExpCfg;
    Board_STATUS status;

    ioExpCfg.i2cInst     = BOARD_I2C_IOEXP_DEVICE1_INSTANCE;
    ioExpCfg.socDomain   = BOARD_SOC_DOMAIN_MAIN;
    ioExpCfg.slaveAddr   = BOARD_I2C_IOEXP_DEVICE1_ADDR;
    ioExpCfg.enableIntr  = false;
    ioExpCfg.ioExpType   = TWO_PORT_IOEXP;
    ioExpCfg.portNum     = PORTNUM_1;
    ioExpCfg.pinNum      = PIN_NUM_5;
    ioExpCfg.signalLevel = GPIO_SIGNAL_LEVEL_HIGH;

    status = Board_setIoExpPinOutput(&ioExpCfg);
    if(status != BOARD_SOK)
    {
        return status;
    }

    ioExpCfg.pinNum = PIN_NUM_6;
    ioExpCfg.signalLevel = GPIO_SIGNAL_LEVEL_HIGH;
    status = Board_setIoExpPinOutput(&ioExpCfg);

    return status;
}

/**
 * \brief   Configures data path for GPMC pins
 *
 * \return  Board_SOK in case of success or appropriate error code.
 *
 */
static Board_STATUS Board_setGpmcDataMux(void)
{
    Board_IoExpCfg_t ioExpCfg;
    Board_STATUS status;

    ioExpCfg.i2cInst     = BOARD_I2C_IOEXP_DEVICE2_INSTANCE;
    ioExpCfg.socDomain   = BOARD_SOC_DOMAIN_MAIN;
    ioExpCfg.slaveAddr   = BOARD_I2C_IOEXP_DEVICE2_ADDR;
    ioExpCfg.enableIntr  = false;
    ioExpCfg.ioExpType   = THREE_PORT_IOEXP;
    ioExpCfg.portNum     = PORTNUM_1;
    ioExpCfg.pinNum      = PIN_NUM_1;
    ioExpCfg.signalLevel = GPIO_SIGNAL_LEVEL_HIGH;

    status = Board_setIoExpPinOutput(&ioExpCfg);
    if(status != BOARD_SOK)
    {
        return status;
    }

    ioExpCfg.pinNum = PIN_NUM_2;
    ioExpCfg.signalLevel = GPIO_SIGNAL_LEVEL_LOW;
    status = Board_setIoExpPinOutput(&ioExpCfg);

    return status;
}

/**
 * \brief Board control function
 *
 * \param   cmd  [IN]  Board control command
 * \param   arg  [IN]  Control command argument. 
 *                     Changes based on the command
 *
 * \return   TRUE if the given board is detected else 0.
 *           SoM board will be always connected to the base board.
 *           For SoM boardID return value TRUE indicates dual PMIC
 *           SoM and FALSE indicates alternate PMIC SoM
 *
 */
Board_STATUS Board_control(uint32_t cmd, void *arg)
{
    Board_STATUS status;

    switch (cmd)
    {
        case BOARD_CTRL_CMD_SET_IO_EXP_PIN_OUT:
            status = Board_setIoExpPinOutput((Board_IoExpCfg_t *)arg);
            break;

        case BOARD_CTRL_CMD_SET_CPSW5G_MDIO_MUX:
            status = Board_setCpsw5GMdioMux();
            break;

        case BOARD_CTRL_CMD_SET_RMII_DATA_MUX:
            status = Board_setGpmcDataMux();
            break;

        default:
            status = BOARD_INVALID_PARAM;
            break;
    }

    return status;
}

