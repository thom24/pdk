/******************************************************************************
 * Copyright (c) 2022 Texas Instruments Incorporated - http://www.ti.com
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
 *   \file    board_i2c_io_exp.c
 *
 *   \brief   This file contains the API's for accessing the i2c IO Expander.
 *
 */

#include "board_i2c_io_exp.h"

/**
 *  \brief    Reads the current configuration of direction port.
 *
 *  \param    slaveAddr       [IN]      I2C Slave Address.
 *  \param    ioExpType       [IN]      IO expander type.
 *                                      X_PORT_IOEXP - Total number of ports
 *                                                     in slave device.
 *  \param    portNum         [IN]      Port number of the i2c slave device
 *                                      PORTNUM_X    - Port number of a slave
 *                                                     device.
 *  \param    data            [IN/OUT]  Pointer to the data buffer to store
 *                                      the pin level data of a specified port.
 *
 *  \return   Board_STATUS in case of success or appropriate error code.
 *
 */
Board_STATUS Board_i2cIoExpReadDirPort(uint8_t slaveAddr,
                                       i2cIoExpType_t ioExpType,
                                       i2cIoExpPortNumber_t portNum,
                                       uint8_t *data)
{
    return BOARD_SOK;
}

/**
 *  \brief    Reads the current configuration of output port.
 *
 *  \param    slaveAddr       [IN]      I2C Slave Address.
 *  \param    ioExpType       [IN]      IO expander type.
 *                                      X_PORT_IOEXP - Total number of ports
 *                                                     in slave device.
 *  \param    portNum         [IN]      Port number of the i2c slave device
 *                                      PORTNUM_X    - Port number of a slave
 *                                                     device.
 *  \param    data            [IN/OUT]  Pointer to the data buffer to store
 *                                      the pin level data of a specified port.
 *
 *  \return   Board_STATUS in case of success or appropriate error code.
 *
 */
Board_STATUS Board_i2cIoExpReadOutputPort(uint8_t slaveAddr,
                                          i2cIoExpType_t ioExpType,
                                          i2cIoExpPortNumber_t portNum,
                                          uint8_t *data)
{
    return BOARD_SOK;
}

/**
 *  \brief    Reads the signal level of all the pins of the specified port.
 *
 *  \param    slaveAddr       [IN]      I2C Slave Address.
 *  \param    ioExpType       [IN]      IO expander type.
 *                                      X_PORT_IOEXP - Total number of ports
 *                                                     in slave device.
 *  \param    portNum         [IN]      Port number of the i2c slave device
 *                                      PORTNUM_X    - Port number of a slave
 *                                                     device.
 *  \param    data            [IN/OUT]  Pointer to the data buffer to store
 *                                      the pin level data of a specified port.
 *
 *  \return   Board_STATUS in case of success or appropriate error code.
 *
 */
Board_STATUS Board_i2cIoExpReadInputPort(uint8_t slaveAddr,
                                         i2cIoExpType_t ioExpType,
                                         i2cIoExpPortNumber_t portNum,
                                         uint8_t *data)
{
    return BOARD_SOK;
}

/**
 *  \brief    Sets the direction of all the pins of the specified Port.
 *
 *  \param    slaveAddr       [IN]        I2C Slave Address.
 *  \param    ioExpType       [IN]        IO expander type.
 *                                        X_PORT_IOEXP - Total number of ports
 *                                                       in that slave device.
 *  \param    portNum         [IN]        Port number of the i2c slave device
 *                                        PORTNUM_X    - Port number of a
 *                                                       slave device.
 *  \param    data            [IN]        Register data to be configured.
 *
 *  \return   Board_STATUS in case of success or appropriate error code.
 *
 */
Board_STATUS Board_i2cIoExpSetPortDirection(uint8_t slaveAddr,
                                            i2cIoExpType_t ioExpType,
                                            i2cIoExpPortNumber_t portNum,
                                            uint8_t data)
{
    return BOARD_SOK;
}

/**
 *  \brief    Sets the direction of the specified pin of the specified port.
 *
 *  \param    slaveAddr       [IN]      I2C Slave Address.
 *  \param    ioExpType       [IN]      IO expander type.
 *                                      X_PORT_IOEXP - Total number of ports
 *                                                     in slave device.
 *  \param    portNum         [IN]      Port number of the i2c slave device
 *                                      PORTNUM_X    - Port number of a slave
 *                                                     device.
 *  \param    pinNum          [IN]      Pin with in the specified port of
 *                                      the i2c slave device.
 *                                      PIN_NUM_X    - Pin number.
 *  \param    direction       [IN]      Direction of the pin to be configured.
 *
 *  \return   Board_STATUS in case of success or appropriate error code.
 *
 */
Board_STATUS Board_i2cIoExpSetPinDirection(uint8_t slaveAddr,
                                           i2cIoExpType_t ioExpType,
                                           i2cIoExpPortNumber_t portNum,
                                           i2cIoExpPinNumber_t pinNum,
                                           i2cIoExpPinDirection_t direction)
{
    return BOARD_SOK;
}

/**
 *  \brief    Sets the signal level of all the pins of the specified port.
 *
 *  \param    slaveAddr       [IN]      I2C Slave Address.
 *  \param    ioExpType       [IN]      IO expander type.
 *                                      X_PORT_IOEXP - Total number of ports
 *                                                     in slave device.
 *  \param    portNum         [IN]      Port number of the i2c slave device
 *                                      PORTNUM_X    - Port number of a slave
 *                                                     device.
 *  \param    data            [IN]      Signal level data of the pins to be
 *                                      configured.
 *
 *  \return   Board_STATUS in case of success or appropriate error code.
 *
 */
Board_STATUS Board_i2cIoExpWritePort(uint8_t slaveAddr,
                                     i2cIoExpType_t ioExpType,
                                     i2cIoExpPortNumber_t portNum,
                                     uint8_t data)
{
    return BOARD_SOK;
}

/**
 *  \brief    Sets the signal level of the specified pin of the specified port.
 *
 *  \param    slaveAddr       [IN]      I2C Slave Address.
 *  \param    ioExpType       [IN]      IO expander type.
 *                                      X_PORT_IOEXP - Total number of ports
 *                                                     in slave device.
 *  \param    portNum         [IN]      Port number of the i2c slave device
 *                                      PORTNUM_X    - Port number of a slave
 *                                                     device.
 *  \param    pinNum          [IN]      Pin with in the specified port of
 *                                      the i2c slave device.
 *                                      PIN_NUM_X    - Pin number.
 *  \param    signalLevel     [IN]      Signal level data of the pin to be
 *                                      configured.
 *
 *  \return   Board_STATUS in case of success or appropriate error code.
 *
 */
Board_STATUS Board_i2cIoExpPinLevelSet(uint8_t slaveAddr,
                                       i2cIoExpType_t ioExpType,
                                       i2cIoExpPortNumber_t portNum,
                                       i2cIoExpPinNumber_t pinNum,
                                       i2cIoExpSignalLevel_t signalLevel)
{
    return BOARD_SOK;
}

/**
 *  \brief    Reads the signal level of specified pin of the specified port.
 *
 *  \param    slaveAddr       [IN]      I2C Slave Address.
 *  \param    ioExpType       [IN]      IO expander type.
 *                                      X_PORT_IOEXP - Total number of ports
 *                                                     in slave device.
 *  \param    portNum         [IN]      Port number of the i2c slave device.
 *                                      PORTNUM_X    - Port number of a slave
 *                                                     device.
 *  \param    pinNum          [IN]      Pin number of the specified port.
 *                                      PIN_NUM_X    - Pin number.
 *  \param    signalLevel     [IN/OUT]  Data buffer to store specified pin
 *                                      level of a specified port.
 *
 *  \return   Board_STATUS in case of success or appropriate error code.
 *
 */
Board_STATUS Board_i2cIoExpPinLevelGet(uint8_t slaveAddr,
                                       i2cIoExpType_t ioExpType,
                                       i2cIoExpPortNumber_t portNum,
                                       i2cIoExpPinNumber_t pinNum,
                                       uint8_t *signalLevel)
{
    return BOARD_SOK;
}

/**
 *  \brief    Initializes the i2c instance connected to the i2c IO Expander.
 *
 *  \return   BOARD_SOK in case of success or appropriate error code.
 *
 */
Board_STATUS Board_i2cIoExpInit(void)
{
    return BOARD_SOK;
}

/**
 *  \brief    de-initializes the i2c instance connected to the i2c IO Expander.
 *
 */
void Board_i2cIoExpDeInit(void)
{

}
