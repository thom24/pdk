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
 *  \file   spi_eeprom.c
 *
 *  \brief  This file contains spi eeprom data transfer functions.
 *          
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <ti/drv/spi/SPI.h>
#include <ti/drv/spi/soc/SPI_soc.h>
#include <ti/drv/uart/UART_stdio.h>

#include "board.h"
#include "board_cfg.h"
#include "spi_eeprom.h"

/*
 *  \brief  Activate SPI transfer
 *
 *  Command code used with SPI_control(). arg is a pointer to an uint32_t
 *  that contains the enable/disable value to be placed into the SPI register.
 *
 *  Command argument 0: (1: enable, 0: disable)
 */
#define SPI_V0_CMD_XFER_ACTIVATE   (SPI_CMD_RESERVED + 0U)

extern void BOARD_delay(uint32_t usecs);

/**
 *  \brief    This function is used to perform spi command
 *            read
 *
 *  \param    handle  spi handle specific to spi instance
 *			  *cmd    spi command to be read
 * 			  cmdLen  length of the spi command
 *            *rxbuf  pionter to the receive buffer
 *            rxlen   length of the receive buffer
 *
 * \return    bool
 * \n         true  - command read successfully
 * \n         false - command read failed
 * 
 */
static bool spieeprom_cmd_read(SPI_Handle handle,
                               uint8_t *cmd,
                               uint32_t cmdLen,
                               uint8_t *rxbuf,
                               uint32_t rxlen)
{
    SPI_Transaction transaction;     /* SPI transaction structure */
    uint32_t        xferEnable;
    uint32_t        terminateXfer = 0;
    bool            retVal = false;

    /* Enable transfer */
    xferEnable = 1;
    SPI_control(handle, SPI_V0_CMD_XFER_ACTIVATE, (void *)&xferEnable);

    /* If no read data, release CS at the end of write */
    if (rxlen == 0)
		terminateXfer = 1;

    /* Write Command */
    transaction.txBuf = cmd;
    transaction.rxBuf = NULL;
    transaction.count = cmdLen;
    transaction.arg = (void *)&terminateXfer;

    retVal = SPI_transfer(handle, &transaction);
	if (retVal == false)
        return retVal;

    /* Receive the data */
    if (rxlen)
    {
        /* Read Device ID */
        transaction.txBuf = NULL;
        transaction.rxBuf = rxbuf;
        transaction.count = rxlen;
        terminateXfer = 1;
        transaction.arg = (void *)&terminateXfer;

        retVal = SPI_transfer(handle, &transaction);
    }

    /* Disable transfer */
    xferEnable = 0;
    SPI_control(handle, SPI_V0_CMD_XFER_ACTIVATE, (void *)&xferEnable);
    return retVal;
}

/**
 *  \brief    This function is used to perform spi command
 *            write
 *
 *  \param    handle  spi handle specific to spi instance
 *			  *cmd    spi command to write
 * 			  cmdLen  length of the spi command
 *            *txbuf  pionter to the transfer buffer
 *            txlen   length of the transfer buffer
 *
 * \return    bool
 * \n         true  - command written successfully
 * \n         false - command write failed
 * 
 */
static bool spieeprom_cmd_write(SPI_Handle handle,
                                uint8_t *cmd,
                                uint32_t cmdLen,
                                uint8_t *txbuf,
                                uint32_t txlen)
{
    SPI_Transaction transaction;     /* SPI transaction structure */
    uint32_t        xferEnable;
    uint32_t        terminateXfer = 0;
    bool            retVal = false;

    /* Enable transfer */
    xferEnable = 1;
    SPI_control(handle, SPI_V0_CMD_XFER_ACTIVATE, (void *)&xferEnable);

    /* If no read data, release CS at the end of write */
    if (txlen == 0)
		terminateXfer = 1;

    /* Write Command */
    transaction.txBuf = cmd;
    transaction.rxBuf = NULL;
    transaction.count = cmdLen;
    transaction.arg = (void *)&terminateXfer;

    retVal = SPI_transfer(handle, &transaction);
	if (retVal == false)
        return retVal;

    /* write data */
    if (txlen)
    {
        /* Write data */
        transaction.txBuf = txbuf;
        transaction.rxBuf = NULL;
        transaction.count = txlen;
        terminateXfer = 1;
        transaction.arg = (void *)&terminateXfer;

        retVal = SPI_transfer(handle, &transaction);
    }

    /* Disable transfer */
    xferEnable = 0;
    SPI_control(handle, SPI_V0_CMD_XFER_ACTIVATE, (void *)&xferEnable);
    return retVal;
}

/**
 *  \brief    This function is used to perform spi write enable
 *
 *  \param    handle  spi handle specific to spi instance
 *			  
 * \return    bool
 * \n         true  - write enabled successfully
 * \n         false - write enable failed
 * 
 */
static bool spieeprom_write_enable(SPI_Handle handle)
{
    uint8_t         cmd = SPI_EEPROM_CMD_WREN;
    bool            retVal;

    retVal = spieeprom_cmd_read(handle, &cmd, 1, NULL, 0);

    return retVal;
}

/**
 *  \brief    This function is used to write data to eeprom
 *
 *  \param    handle          spi handle specific to spi instance
 *            spiTransaction  pointer to spi transaction struct access
 *            length          length of data buffer
 *            addr            address to be written
 *			  
 * \return    int8_t
 * \n         TEST_PASS  - eeprom write passed
 * \n         TEST_FAIL  - eeprom write failed
 * 
 */
int8_t spiEepromWrite(SPI_Handle handle,
                           SPI_Transaction* spiTransaction,
						   int8_t length,
						   uint32_t addr)
{
    uint8_t         cmd[3];
    bool            retVal;

	/* Send Write Enable command */
    retVal = spieeprom_write_enable(handle);
    if (retVal == false)
        return TEST_FAIL;

    cmd[0]  = SPI_EEPROM_CMD_WRITE;
    cmd[1]  = (uint8_t)(addr >> 8);
    cmd[2]  = (uint8_t)addr;

    retVal = spieeprom_cmd_write(handle, cmd, 3, spiTransaction->txBuf, length);
	if (retVal == false)
	{
        return TEST_FAIL;
	}
	else
	{
		return TEST_PASS;
	}
}

/**
 *  \brief    This function is used to read data from eeprom
 *
 *  \param    handle          spi handle specific to spi instance
 *            spiTransaction  pointer to spi transaction struct access
 *            length          length of data buffer
 *            addr            address to be read
 *			  
 * \return    int8_t
 * \n         TEST_PASS  - eeprom write passed
 * \n         TEST_FAIL  - eeprom write failed
 * 
 */
int8_t spiEepromRead(SPI_Handle handle,
                          SPI_Transaction* spiTransaction,
						  int8_t length,
						  uint32_t addr)
{
    uint8_t       cmd[3];
    bool          retVal;

    /* Initialize the command to be sent serially */
    cmd[0] = SPI_EEPROM_CMD_READ;
    cmd[1] = (uint8_t)(addr >> 8);
    cmd[2] = (uint8_t)addr;

    retVal = spieeprom_cmd_read(handle, cmd, 3, spiTransaction->rxBuf, length);
	if (retVal == false)
	{
		return TEST_FAIL;
	}
	else
	{
		return TEST_PASS;
	}
}
