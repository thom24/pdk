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
 *  \file   spi_eeprom_test.c
 *
 *  \brief  This file contains spi eeprom test functions.
 *          
 */

 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <ti/csl/soc.h>
#include <ti/drv/spi/SPI.h>
#include <ti/drv/spi/soc/SPI_soc.h>
#include <ti/drv/uart/UART_stdio.h>

#include "board.h"
#include "board_cfg.h"

/* Platform test return codes */
#define TEST_PASS     (0)
#define TEST_FAIL     (-1)
#define CSL_SPI_CNT   (1U)

/*SPI data buffer length in bytes*/
#define BOARD_DIAG_SPI_TEST_BUF_LENGTH             (64U)

/* SPI EEPROM TEST DELAY in usec */
#define BOARD_DIAG_SPI_EEPROM_TEST_DELAY           (100000U)

/* SPI EEPROM CLOCK FREQUENCY in Hz */
#define BOARD_DIAG_SPI_EEPROM_CLOCK_FREQUENCY      (5000000U)

/* SPI CLOCK FREQUENCY in Hz */
#define BOARD_DIAG_SPI_CLOCK_FREQUENCY             (1000000U)

extern const SPI_Config SPI_config[];
extern void BOARD_delay(uint32_t usecs);
extern int8_t spiEepromWrite(SPI_Handle handle,
                           SPI_Transaction* spiTransaction,
						   int8_t length,
						   uint32_t addr);

extern int8_t spiEepromRead(SPI_Handle handle,
						  SPI_Transaction* spiTransaction,
						  int8_t length,
						  uint32_t addr);
						  
int8_t eepromData[BOARD_DIAG_SPI_TEST_BUF_LENGTH] = {0x01, 0x02, 0x03, 0x04, 0x05,
                                                     0x06, 0x07, 0x08, 0x09, 0x0A,
											         0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
											         0x10, 0x11, 0x12, 0x13, 0x14,
											         0x15, 0x16, 0x17, 0x18, 0x19,
											         0x1A, 0x1B, 0x1C, 0x1D, 0x1E,
											         0x1F, 0x20, 0x21, 0x22, 0x23,
											         0x24, 0x25, 0x26, 0x27, 0x28,
											         0x29, 0x2A, 0x2B, 0x2C, 0x2D,
											         0x2E, 0x2F, 0x30, 0x31, 0x32,
											         0x33, 0x34, 0x35, 0x36, 0x37,
											         0x38, 0x39, 0x3A, 0x3B, 0x3C,
											         0x3D, 0x3E, 0x3F, 0x40};

int8_t txbuf[BOARD_DIAG_SPI_TEST_BUF_LENGTH];
int8_t rxbuf[BOARD_DIAG_SPI_TEST_BUF_LENGTH];												

/**
 *  \brief    This function compares data between two buffers
 *
 *  \param    *expData  pointer to the reference buffer
 *            *rxData   pointer to the buffer to be compared
 *            length    length of the buffer
 *
 * \return
 * \n         TEST_PASS  - Test Passed
 * \n         TEST_FAIL  - Test Failed
 */											
static int8_t CompareData(int8_t *expData, 
							   int8_t *rxData, 
							   uint16_t length)
{
    uint32_t idx = 0;
    uint32_t match = 1;

    for(idx = 0; ((idx < length) && (match != 0)); idx++)
    {
        if(*expData != *rxData) match = 0;
        expData++;
        rxData++;
    }

    if(match == 1)
	{
		return TEST_PASS;
	}

    return TEST_FAIL;
}

/**
 *  \brief    This function writes and reads data in eeprom
 *
 *  \param    spi_instance spi instance on which device is connected
 *            
 *
 * \return
 * \n         TEST_PASS  - Test Passed
 * \n         TEST_FAIL  - Test Failed
 */
static int8_t spi_eeprom_access_test(uint8_t spi_instance)
{
	uint32_t index;
	uint32_t addr;
	SPI_Params spiParams;
    SPI_Handle handle = NULL;
    SPI_Transaction spiTransaction;
	int8_t length;
    int8_t status;
	
	for (index=0; index<CSL_SPI_CNT; index++)
    {
        ((SPI_v1_HWAttrs *)SPI_config[index].hwAttrs)->enableIntr = false;
    }
	
    SPI_init();

	/* Default SPI configuration parameters */
    SPI_Params_init(&spiParams);
	spiParams.bitRate = BOARD_DIAG_SPI_EEPROM_CLOCK_FREQUENCY;
	spiParams.frameFormat  = SPI_POL0_PHA1;
	spiParams.transferMode = (SPI_TransferMode)SPI_OPER_MODE_BLOCKING;

    handle = SPI_open(spi_instance, &spiParams);
	if(handle == NULL)
	{
		UART_printf("SPI Handle open failed\n");
		return(TEST_FAIL);
	}

    /* populating buffers with data */
	for(index=0; index<BOARD_DIAG_SPI_TEST_BUF_LENGTH; index++)
	{
		txbuf[index] = eepromData[index];
		rxbuf[index] = 0U;
	}
	
	/* Write to EEPROM */
	UART_printf("Writing data to EEPROM\n");
	addr = 0;
	length = BOARD_DIAG_SPI_TEST_BUF_LENGTH;
    spiTransaction.txBuf = txbuf;
	spiTransaction.rxBuf = rxbuf;
    status = spiEepromWrite(handle, &spiTransaction,length,addr);
	BOARD_delay(BOARD_DIAG_SPI_EEPROM_TEST_DELAY);

	/* Read from EEPROM */
	UART_printf("Reading data from EEPROM\n");
	length = BOARD_DIAG_SPI_TEST_BUF_LENGTH;
    spiTransaction.txBuf = NULL;
    spiTransaction.rxBuf = rxbuf;
    status = spiEepromRead(handle, &spiTransaction,length,addr);

    SPI_close(handle);

    status = CompareData(&eepromData[0], &rxbuf[0], BOARD_DIAG_SPI_TEST_BUF_LENGTH);
	if(status == TEST_PASS) 
	{
		UART_printf("Data Matched\n");
	}
	else
	{
		UART_printf("Data Mismatch!\n");
	}
    return status;
}

/**
 *  \brief    This function is used to perform soc level 
 *            initialisation of spi
 *
 *  \param    spi_instance  instance which has to be initialised
 *
 * \return    void
 * 
 */
static void soc_initspi(uint8_t spi_instance)
{
	SPI_v1_HWAttrs spi_cfg;
	
	/* Get the default SPI init configurations */
    SPI_socGetInitCfg(spi_instance, &spi_cfg);
	spi_cfg.chNum = 0;

    /* Set the default SPI init configurations */
    SPI_socSetInitCfg(spi_instance, &spi_cfg);
}

/**
 *  \brief    This function is used to perform ioexp spi eeprom 
 *            read/write test
 *
 *  \param    void
 *
 * \return
 * \n         TEST_PASS  - Test Passed
 * \n         TEST_FAIL  - Test Failed
 */
static int8_t run_ioexp_spi_eeprom_test(void)
{
	int8_t testStatus;
	
	soc_initspi(BOARD_EEPROM_SPI0_PORT);
	UART_printf("\nRunning SPI1 EEPROM access Test\n\n");
	
	testStatus = spi_eeprom_access_test(BOARD_EEPROM_SPI0_PORT);
    if(testStatus != TEST_PASS)
    {
   	    UART_printf("\nSPI1 EEPROM access Test Failed!!\n");
   		return (testStatus);
   	}
   	else
   	{
		UART_printf("\nSPI1 EEPROM access Test Passed!\n");
	}
	return (testStatus);
}

/**
 * \brief This function performs ioexp spi eeprom test
 *
 * \param void
 *
 * \return
 * \n      TEST_PASS  - Test Passed
 * \n      TEST_FAIL  - Test Failed
 *
 */
static int8_t ioexp_spi_eeprom_test(void)
{
	int8_t status = 0;

	UART_printf  ("*************************\n"); 
	UART_printf  ("*    SPI EEPROM Test    *\n");
	UART_printf  ("*************************\n");
	
	status = run_ioexp_spi_eeprom_test();
	if (status != 0)
	{
		UART_printf("\nSPI EEPROM Test FAILED!\n");
	}
	else
	{
		UART_printf("\nSPI EEPROM Test PASSED!\n");
	}

	UART_printf("\nSPI EEPROM tests completed\n");
	return status;
}

/*
 *  ======== main ========
 */
int main(void)
{
    Board_initCfg boardCfg;
#ifdef PDK_RAW_BOOT
    boardCfg = BOARD_INIT_PINMUX_CONFIG | 
        BOARD_INIT_UART_STDIO;
#else
    boardCfg = BOARD_INIT_UART_STDIO;
#endif
    Board_init(boardCfg);

	return ioexp_spi_eeprom_test();
}
