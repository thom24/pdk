/******************************************************************************
* Copyright (c) 2020 Texas Instruments Incorporated - http://www.ti.com
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
*****************************************************************************/

/**
 * \file   exp_header_tpr12.c
 *
 * \brief  HD connector test file.
 *
 *  Targeted Functionality: Verification of FE HD connector control 
 *                          lines as gpio through spi interface for 
 *                            handshake mechanism.
 *
 *  Operation: This application toggles the control signal as gpio 
 *               lines connected to HD connector FE1 and FE2 which 
 *               are connected to externak IWR1443 radar sensor module.
 *
 *  Supported SoCs: TPR12.
 *
 *  Supported Platforms: tpr12_evm.
 *
 */

#include "exp_header_tpr12.h"

/**
 *  \brief    This function used to receive acknowledgment from slave 
 *            device.
 *
 *  \param    MIBSPI_Handle  handle [IN]  MIBSPI_Handle
 *
 */
void BoardDiag_spiReceiveAck(MIBSPI_Handle handle)
{
    MIBSPI_Transaction transaction;
    uint8_t rxBuf = NULL;

    memset(&transaction, 0, sizeof(transaction));

    transaction.txBuf = NULL;
    transaction.rxBuf = &rxBuf;
    transaction.count = 1;
    transaction.slaveIndex = 0;

    while(rxBuf != BOARD_DIAG_SPI_SLAVE_ACK)
    {
        MIBSPI_transfer(handle, &transaction);
    }
}

/**
 *  \brief    This function used to send data to slave
 *            device.
 *
 *  \param    handle            [IN]     MIBSPI Handler
 *            txBuf             [IN]     Data buffer
 *            count             [IN]     Data count
 *
 */
void BoardDiag_spiTransferData(MIBSPI_Handle handle, uint8_t txBuf,
                               uint8_t count)
{
    MIBSPI_Transaction transaction;

    memset(&transaction, 0, sizeof(transaction));

    transaction.txBuf = &txBuf;
    transaction.rxBuf = NULL;
    transaction.count = count;
    transaction.slaveIndex = 0;
    MIBSPI_transfer(handle, &transaction);
}

/**
 *  \brief    The function performs Expansion header verification test.
 *
 *  \return   int8_t
 *               0 - in case of success
 *              -1 - in case of failure.
 *
 */
int8_t BoardDiag_expansionHeaderTest(void)
{
    MIBSPI_Params     params;
    MIBSPI_Handle     handle;
    MIBSPI_UtilsPrms utilsPrms;

    enum MibSpi_InstanceId mibspiInstance[BOARD_DIAG_NUM_MIBSPI_INSTANCES] =
                          {MIBSPI_INST_ID_RCSS_SPIA, MIBSPI_INST_ID_RCSS_SPIB};
    uint16_t pinMuxOffset[BOARD_DIAG_TOTAL_NUM_GPIO_LINES] = 
    {BOARD_DIAG_PAD_CS_OFFSET_ADDRESS, BOARD_DIAG_PAD_CY_OFFSET_ADDRESS,
     BOARD_DIAG_PAD_CX_OFFSET_ADDRESS, BOARD_DIAG_PAD_CZ_OFFSET_ADDRESS};

    uint8_t txBuf, spiInst, gpioPinNum, gpioPinIndex, toggleFreq;

    for(gpioPinNum = 0; gpioPinNum < BOARD_DIAG_TOTAL_NUM_GPIO_LINES;
        gpioPinNum++)
    {
        Board_pinMuxSetMode(pinMuxOffset[gpioPinNum],
                            BOARD_DIAG_GPIO_MUX_MODE);
    }

    /* Start toggling gpio control lines */
    GPIO_init();

    /* Initialize the SPI */
    memset(&utilsPrms, 0, sizeof(utilsPrms));

    MIBSPI_init(&utilsPrms);

    /* Setup the default SPI Parameters */
    MIBSPI_Params_init(&params);

    /* Disble DMA */
    params.dmaEnable = 0;
    params.dmaHandle = NULL;

    /* Set SPI in master mode */
    params.mode = MIBSPI_MASTER;

    for(spiInst = 0; spiInst < BOARD_DIAG_NUM_MIBSPI_INSTANCES; spiInst++)
    {
        /* MIBSPIA for HD-connector-1 and MIBSPIB for HD-connector-2 */
        UART_printf("Controlling gpio signals from HD connector - %d\n",
                    spiInst);

        /* Open the SPI Instance for MibSpi */
        handle = MIBSPI_open(mibspiInstance[spiInst], &params);
        if(handle == NULL)
        {
            UART_printf("Failed to open MIBPSI instance - %d\n", spiInst);
            return -1;
        }

        /* Send START command to indicate the start of transaction */
        txBuf = BOARD_DIAG_SPI_MASTER_START_COMMAND;

        /* Transfer start Command */
        BoardDiag_spiTransferData(handle, txBuf, 1);

        /* Wait till it receives the acknowledgment from slave device */
        BoardDiag_spiReceiveAck(handle);

        for(gpioPinNum = 0; gpioPinNum < BOARD_DIAG_MAX_GPIO_LINES_PER_SPI_INST;
            gpioPinNum++)
        {
            for(toggleFreq = 0; toggleFreq < BOARD_DIAG_GPIO_TOGGLE_FREQUENCY;
                toggleFreq++)
            {
                if(spiInst == 0)
                {
                    gpioPinIndex = gpioPinNum;
                }
                else
                {
                    gpioPinIndex = gpioPinNum +
                                    BOARD_DIAG_MAX_GPIO_LINES_PER_SPI_INST;
                }
                UART_printf("Making gpio line - %d as high\n", gpioPinNum);
                GPIO_write(gpioPinIndex, BOARD_DIAG_GPIO_PIN_HIGH);

                /* Wait till it receives the acknowledgment from slave device */
                UART_printf("waiting for acknowledgment from the slave");

                BoardDiag_spiReceiveAck(handle);

                UART_printf("Received acknowledgment from the slave "
                            "successfully\n\n");

                UART_printf("Making gpio line - %d as low\n", gpioPinNum);
                GPIO_write(gpioPinIndex, BOARD_DIAG_GPIO_PIN_LOW);

                /* Wait till it receives the acknowledgment from slave device */
                UART_printf("waiting for acknowledgment from the slave");

                BoardDiag_spiReceiveAck(handle);

                UART_printf("Received acknowledgment from the slave successfully\n\n");
            }
        }
    }

    return 0;
}

/**
 * \brief  main function
 *
 *  This function performs board intializations and calls expansion
 *  header test
 *
 * \return  int
 *              0  - in case of success
 *             -1  - in case of failure
 */
int main(void)
{
    uint32_t status;
    Board_initCfg boardCfg;

    boardCfg = BOARD_INIT_MODULE_CLOCK |
               BOARD_INIT_PINMUX_CONFIG |
               BOARD_INIT_UART_STDIO;

    status = Board_init(boardCfg);
    if (status != BOARD_SOK)
    {
        return -1;
    }

    return BoardDiag_expansionHeaderTest();
}
