
/*
 * Copyright (C) 2024 Texas Instruments Incorporated - http://www.ti.com/
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
 *  \file   i2c_extended_testapp.c
 *
 *  \brief  Example application file. This application will read the data
 *          from eeprom and compares it with the known data.
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================= */

#include <stdio.h>
#include <string.h>
#include <ti/csl/soc.h>
#include <ti/csl/csl_i2c.h>
#include <ti/drv/i2c/soc/I2C_soc.h>
#include <ti/drv/uart/UART_stdio.h>
#include <ti/board/board.h>

/* ========================================================================== */
/*                             Macros                                         */
/* ========================================================================== */

#define I2C_APP_TRANSACTION_TIMEOUT                  (40000U)

#define I2C_APP_MAX_FREQ_COUNT                       5
#define I2C_APP_BUFSTAT_INVALID_COMMAND              4

#define I2C_APP_ID_BIT_RATE_INTERRUPT_MODE           0   /* I2C bit rate test in interrupt mode */
#define I2C_APP_ID_BIT_RATE_POLLING_MODE             1   /* I2C bit rate test in polling mode */
#define I2C_APP_ID_BIT_RATE_CALLBACK_MODE            2   /* I2C bit rate test in callback mode */
#define I2C_APP_ID_PROBE_BUS_FREQ                    3   /* I2C probe bus frequency Test in interrupt mode  */
#define I2C_APP_ID_TIMEOUT_INT                       4   /* I2C timeout test in interrupt mode */
#define I2C_APP_ID_NULL_CHECK                        5   /* I2C NULL check*/

#define I2C_APP_EEPROM_INSTANCE                      BOARD_I2C_EEPROM_INSTANCE
#define I2C_APP_EEPROM_ADDR                          BOARD_I2C_EEPROM_ADDR
#define I2C_APP_EEPROM_TEST_ADDR                     0x0001           /* Read and write test start address */
#define I2C_APP_EEPROM_ADDR_SIZE                     2   /* in bytes */
#define I2C_APP_EEPROM_TEST_LENGTH                   10  /* Read (and write) length in bytes */

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* Define the I2C test interface */
typedef struct I2CApp_TestCfg_s
{
    bool     (*testFunc)(void *);
    int32_t  testId;
    bool     dmaMode;
    bool     intrMode;
    bool     cbMode;
    uint32_t timeout;
    size_t   readcount;
    size_t   writecount;
    uint32_t slaveAddress;
    uint8_t  testDesc[80];
} I2CApp_TestCfg;

/* ========================================================================== */
/*                            Functions Declarations                          */
/* ========================================================================== */

static bool I2CApp_boardInitI2C(void);
static bool I2CApp_probeBusFreqTest(void *arg);
static bool I2CApp_nullTest(void *arg);
static bool I2CApp_bitrateTest(void *arg);
static bool I2CApp_bitrateTestTxnInvdParams(void *arg);
static bool I2CApp_bitrateTestFrequency(I2C_BitRate bitRate, I2CApp_TestCfg *test);
static bool I2CApp_compareData(uint8_t *expData, uint8_t *rxData, uint32_t length);
static void I2CApp_initConfig(uint32_t instance, I2CApp_TestCfg *test);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

uint8_t gI2CApp_EepromData[I2C_APP_EEPROM_TEST_LENGTH] = {85, 51, 238, 1, 11, 1, 16, 46, 0, 74};

I2CApp_TestCfg gI2cApp_Tests[] =
{
    /*{testFunc,    testID,    dma,    intr,    cbMode,    timeout,    readcount,    writecount,    slaveaddress,    testDesc} */
    {I2CApp_bitrateTest, I2C_APP_ID_BIT_RATE_POLLING_MODE, FALSE, FALSE, FALSE, SemaphoreP_WAIT_FOREVER, I2C_APP_EEPROM_TEST_LENGTH, I2C_APP_EEPROM_ADDR_SIZE, I2C_APP_EEPROM_ADDR, "\r\n I2C bit rate test in polling"},
    {I2CApp_bitrateTestTxnInvdParams, I2C_APP_ID_BIT_RATE_POLLING_MODE, FALSE, FALSE, FALSE, SemaphoreP_WAIT_FOREVER, 0, I2C_APP_EEPROM_ADDR_SIZE, I2C_APP_EEPROM_ADDR, "\r\n I2C bit rate test in polling readCount=0"},
    {I2CApp_bitrateTestTxnInvdParams, I2C_APP_ID_BIT_RATE_POLLING_MODE, FALSE, FALSE, FALSE, SemaphoreP_WAIT_FOREVER, I2C_APP_EEPROM_TEST_LENGTH, 0,I2C_APP_EEPROM_ADDR, "\r\n I2C bit rate test in polling writeCount=0"},
    {I2CApp_bitrateTestTxnInvdParams, I2C_APP_ID_BIT_RATE_POLLING_MODE, FALSE, FALSE, FALSE, SemaphoreP_WAIT_FOREVER, I2C_APP_EEPROM_TEST_LENGTH, I2C_APP_EEPROM_ADDR_SIZE,0x60, "\r\n I2C bit rate test in polling invalid slave"},
    {I2CApp_bitrateTestTxnInvdParams, I2C_APP_ID_BIT_RATE_POLLING_MODE, FALSE, FALSE, FALSE, SemaphoreP_WAIT_FOREVER, I2C_APP_EEPROM_TEST_LENGTH, 0, 0x60, "\r\n I2C bit rate test in polling invalid slave"},

    {I2CApp_bitrateTest, I2C_APP_ID_BIT_RATE_INTERRUPT_MODE, FALSE, TRUE, FALSE, SemaphoreP_WAIT_FOREVER, I2C_APP_EEPROM_TEST_LENGTH, I2C_APP_EEPROM_ADDR_SIZE, I2C_APP_EEPROM_ADDR, "\r\n I2C bit rate test in interrupt mode"},
    {I2CApp_bitrateTestTxnInvdParams, I2C_APP_ID_BIT_RATE_INTERRUPT_MODE, FALSE, TRUE, FALSE, 0, 0, I2C_APP_EEPROM_ADDR_SIZE, I2C_APP_EEPROM_ADDR, "\r\n I2C bit rate test in interrupt mode readCount=0, timeout=0"},
    {I2CApp_bitrateTestTxnInvdParams, I2C_APP_ID_BIT_RATE_INTERRUPT_MODE, FALSE, TRUE, FALSE, SemaphoreP_WAIT_FOREVER, I2C_APP_EEPROM_TEST_LENGTH, 0, I2C_APP_EEPROM_ADDR, "\r\n I2C bit rate test in interrupt mode WriteCount=0"},
    {I2CApp_bitrateTestTxnInvdParams, I2C_APP_ID_BIT_RATE_INTERRUPT_MODE, FALSE, TRUE, FALSE, SemaphoreP_WAIT_FOREVER, I2C_APP_EEPROM_TEST_LENGTH, I2C_APP_EEPROM_ADDR_SIZE, 0x60, "\r\n I2C bit rate test in interrupt mode invalid slave address"},

    {I2CApp_probeBusFreqTest, I2C_APP_ID_PROBE_BUS_FREQ, FALSE, TRUE, FALSE, SemaphoreP_WAIT_FOREVER, I2C_APP_EEPROM_TEST_LENGTH, I2C_APP_EEPROM_ADDR_SIZE, I2C_APP_EEPROM_ADDR, "\r\n I2C probe bus frequency test in interrupt mode"},

    {I2CApp_nullTest, I2C_APP_ID_NULL_CHECK, FALSE, FALSE, FALSE, 1, I2C_APP_EEPROM_TEST_LENGTH, I2C_APP_EEPROM_ADDR_SIZE, I2C_APP_EEPROM_ADDR, "\r\n I2C null check test"},
    {NULL, },
};

/* ========================================================================== */
/*                            Function Definitions                            */
/* ========================================================================== */

void I2CApp_testPrintTestDesc(I2CApp_TestCfg *test)
{
    uint8_t        testId[16] = {0, };

    /* Print unit test ID */
    sprintf((char*)testId, "%d", test->testId);
    UART_printf("\n I2C UT %s\n", testId);

    /* Print test description */
    UART_printf("\n %s\n", test->testDesc);
}

int I2C_test()
{
    bool       testResult = TRUE;
    uint32_t   testCount;
    I2CApp_TestCfg *test;

    if(I2CApp_boardInitI2C() == FALSE)
    {
        return(0);
    }

    for(testCount= 0; ; testCount++)
    {
        test = &gI2cApp_Tests[testCount];
        if(test->testFunc == NULL)
        {
            break;
        }
        I2CApp_testPrintTestDesc(test);
        if(test->testFunc((void *)test) == TRUE)
        {
            UART_printf("\r\n %s has passed\r\n", test->testDesc);
        }
        else
        {
            UART_printf("\r\n %s has failed\r\n", test->testDesc);
            testResult = FALSE;
            break;
        }
    }
    if(testResult == TRUE)
    {
        UART_printf("\n All tests have passed. \n");
    }
    else
    {
        UART_printf("\n Some tests have failed. \n");
    }

#if defined LDRA_DYN_COVERAGE_EXIT
    UART_printf("\n LDRA ENTRY \n");
    upload_execution_history();
    UART_printf("\n LDRA EXIT \n");
#endif

    return 0;
}

int main(void)
{
    I2C_test();
    return 0;
}

/* ========================================================================== */
/*                           Internal Function Definitions                    */
/* ========================================================================== */

static bool I2CApp_boardInitI2C(void)
{
    Board_initCfg boardCfg;
    Board_STATUS  boardStatus;
    I2C_HwAttrs   i2cCfg;
    bool          status;

    boardCfg = BOARD_INIT_PINMUX_CONFIG |
               BOARD_INIT_MODULE_CLOCK |
               BOARD_INIT_UART_STDIO;

    boardStatus = Board_init(boardCfg);
    if(boardStatus != BOARD_SOK)
    {
        status = FALSE;
    }
    else
    {
        status = TRUE;
    }
    
    if(status == TRUE)
    {
        I2C_init();
        /* Get the default I2C init configurations */
        I2C_socGetInitCfg(I2C_APP_EEPROM_INSTANCE, &i2cCfg);
        /* No I2C instanced connected to eeprom in main domain, use i2c instance in wakeup domain */
        i2cCfg.baseAddr = CSL_WKUP_I2C0_CFG_BASE;
        /* Pulsar R5 core is on the MCU domain */
        i2cCfg.intNum = CSLR_MCU_R5FSS0_CORE0_INTR_WKUP_I2C0_POINTRPEND_0;
        /* Set the default I2C init configurations */
        I2C_socSetInitCfg(I2C_APP_EEPROM_INSTANCE, &i2cCfg);
    }

    UART_printf("\n I2C Test: Using Instance %d", I2C_APP_EEPROM_INSTANCE);

    return (TRUE);
}

static void I2CApp_initConfig(uint32_t instance, I2CApp_TestCfg *test)
{
    I2C_HwAttrs   i2cCfg;

    /* Get the default I2C init configurations */
    I2C_socGetInitCfg(instance, &i2cCfg);
    i2cCfg.enableIntr = test->intrMode;

    /* Set the SPI init configurations */
    I2C_socSetInitCfg(instance, &i2cCfg);
}

static bool I2CApp_compareData(uint8_t *expData, uint8_t *rxData, uint32_t length)
{
    uint32_t idx = 0U;
    uint32_t match = 1U;
    bool retVal = FALSE;

    for(idx = 0U; ((idx < length) && (0U != match)); idx++)
    {
        if(*expData != *rxData) 
        {
            match = 0U;
        }
        expData++;
        rxData++;
    }

    if(1U == match)
    {
        retVal = TRUE;
    }

    return retVal;
}

static bool I2CApp_bitrateTestFrequency(I2C_BitRate bitRate, I2CApp_TestCfg *test)
{
    I2C_Params      i2cParams;
    I2C_Handle      handle = NULL;
    I2C_Transaction i2cTransaction;
    uint8_t         txBuf[I2C_APP_EEPROM_TEST_LENGTH + I2C_APP_EEPROM_ADDR_SIZE] = {0x00, };
    uint8_t         rxBuf[I2C_APP_EEPROM_TEST_LENGTH];
    int16_t         status;
    bool            testStatus = TRUE;

    /* Set the I2C EEPROM write/read address */
    txBuf[0] = (I2C_APP_EEPROM_TEST_ADDR >> 8) & 0xff; /* EEPROM memory high address byte */
    txBuf[1] = I2C_APP_EEPROM_TEST_ADDR & 0xff;        /* EEPROM memory low address byte */

    I2CApp_initConfig(I2C_APP_EEPROM_INSTANCE, test);

    I2C_Params_init(&i2cParams);

    /* Set bitRate */
    i2cParams.bitRate = bitRate;
    handle = I2C_open(I2C_APP_EEPROM_INSTANCE, &i2cParams);
    if(handle == NULL)
    {
        testStatus = FALSE;
    }

    memset(rxBuf, 0, I2C_APP_EEPROM_TEST_LENGTH);
    I2C_transactionInit(&i2cTransaction);
    i2cTransaction.slaveAddress = test->slaveAddress;
    i2cTransaction.writeBuf = (uint8_t *)&txBuf[0];
    i2cTransaction.writeCount = test->writecount;
    i2cTransaction.readBuf = (uint8_t *)&rxBuf[0];
    i2cTransaction.readCount = test->readcount;
    i2cTransaction.timeout   = I2C_APP_TRANSACTION_TIMEOUT;
    status = I2C_transfer(handle, &i2cTransaction);

    if(I2C_STS_SUCCESS != status)
    {
        UART_printf("I2C Test: Read Data Transfer failed. \n");
        testStatus = FALSE;
    }
    else
    {
        testStatus = I2CApp_compareData(&gI2CApp_EepromData[0], &rxBuf[0], I2C_APP_EEPROM_TEST_LENGTH);
        if(FALSE == testStatus)
        {
            UART_printf("I2C Test: Data Mismatch. \n");
        }
    }

    if(handle)
    {
        I2C_close(handle);
    }

    return (testStatus);
}

static bool I2CApp_bitrateTestTxnInvdParams(void *arg)
{
    I2C_Params          i2cParams;
    I2C_Handle          handle = NULL;
    I2C_Transaction     i2cTransaction;
    uint8_t             txBuf[I2C_APP_EEPROM_TEST_LENGTH + I2C_APP_EEPROM_ADDR_SIZE] = {0x00, };
    uint8_t             rxBuf[I2C_APP_EEPROM_TEST_LENGTH];
    int16_t             status;
    bool                testStatus = TRUE;
    I2CApp_TestCfg        *test = (I2CApp_TestCfg *)arg;
    I2C_HwAttrs const   *hwAttrs = NULL;
    uint32_t            retVal;

    /* Set the I2C EEPROM write/read address */
    txBuf[0] = (I2C_APP_EEPROM_TEST_ADDR >> 8) & 0xff; /* EEPROM memory high address byte */
    txBuf[1] = I2C_APP_EEPROM_TEST_ADDR & 0xff;        /* EEPROM memory low address byte */

    I2CApp_initConfig(I2C_APP_EEPROM_INSTANCE, test);

    I2C_Params_init(&i2cParams);

    /* Set bitRate */
    i2cParams.bitRate = I2C_100kHz;
    handle = I2C_open(I2C_APP_EEPROM_INSTANCE, &i2cParams);
    if(handle == NULL)
    {
        testStatus = FALSE;
    }
    /* EEPROM write disabled on K2, need copy data */
    memset(rxBuf, 0, I2C_APP_EEPROM_TEST_LENGTH);
    I2C_transactionInit(&i2cTransaction);
    i2cTransaction.slaveAddress = test->slaveAddress;
    i2cTransaction.writeBuf = (uint8_t *)&txBuf[0];
    i2cTransaction.writeCount = test->writecount;
    i2cTransaction.readBuf = (uint8_t *)&rxBuf[0];
    i2cTransaction.readCount = test->readcount;
    i2cTransaction.timeout   = test->timeout;
    status = I2C_transfer(handle, &i2cTransaction);

    /* check master is busy */
    retVal = I2CMasterBusy(hwAttrs->baseAddr);
    if(0U != retVal)
    {
        UART_printf(": MASTER BUS BUSY :%d \n",retVal);
        testStatus = FALSE;
    }

    /* check for invalid salve if NACK bit SET */
    retVal = I2CMasterErr(hwAttrs->baseAddr);
    if(0x02U == retVal)
    {
        UART_printf("I2C Test: NACK.\n");
    }

    /* check Arbitration bit */
    retVal = I2CMasterIntStatusEx(hwAttrs->baseAddr, 0x01);
    if(0U != retVal)
    {
        UART_printf("I2C Test: Arbitration LOST.\n");
        testStatus = FALSE;
    }

    /* check the buffer status */
    retVal = I2CBufferStatus(hwAttrs->baseAddr, I2C_RX_BUFFER_STATUS);
    if(0U != retVal)
    {
        UART_printf("I2C Test:RX buffer Status :%d \n", retVal);
        testStatus = FALSE;
    }

    /* check the buffer by passing invalid FLag */
    retVal = I2CBufferStatus(hwAttrs->baseAddr, I2C_APP_BUFSTAT_INVALID_COMMAND);
    if(0U != retVal)
    {
        UART_printf("I2C Test: Invalid case :%d \n", retVal);
        testStatus = FALSE;
    }

    /* clear and check the interrupt status */
    I2CMasterIntRawStatusClearEx(hwAttrs->baseAddr, 0U);
    retVal = I2CMasterIntRawStatus(hwAttrs->baseAddr);
    if(0U != retVal)
    {
        UART_printf("I2C Test: Interrupt Not cleared :%d \n", retVal);
        testStatus = FALSE;
    }

    if(I2C_STS_SUCCESS != status)
    {
        UART_printf("I2C Test: Read Data Transfer failed. \n");
        testStatus = FALSE;
        if(test->slaveAddress != I2C_APP_EEPROM_ADDR)
        {
            UART_printf("I2C Test: Invalid slave Test: PASS \n");
            testStatus = TRUE;
        }
    }
    else
    {
        if(test->readcount == 0)
        {
            for(int i=0; i < I2C_APP_EEPROM_TEST_LENGTH; i++)
            {
                if(0U != rxBuf[i])
                {
                    UART_printf("I2C Test: Data Mismatch \n");
                    testStatus = FALSE;
                }
           }
        }
        else
        {
            testStatus = I2CApp_compareData(&gI2CApp_EepromData[0], &rxBuf[0], I2C_APP_EEPROM_TEST_LENGTH);
            if(FALSE == testStatus)
            {
                UART_printf("I2C Test: Data Mismatch \n");
            }
        }
    }

    if(handle)
    {
        I2C_close(handle);
    }

    return (testStatus);
}

static bool I2CApp_bitrateTest(void *arg)
{
    bool       testResult = TRUE;
    uint32_t   i;

    for(i = 0; i < I2C_APP_MAX_FREQ_COUNT; i++)
    {
        testResult = I2CApp_bitrateTestFrequency((I2C_BitRate)i, (I2CApp_TestCfg *)arg);
        if(testResult == FALSE)
        {
            break;
        }
    }

    return (testResult);
}

static bool I2CApp_nullTest(void *arg)
{
    bool    testResult = TRUE;
    I2C_Transaction i2cTransaction;
    uint32_t busFrequency = I2C_400kHz;

    /*null check for i2c_drv.c file*/
    I2C_init();
    I2C_Params_init(NULL);
    I2C_open(I2C_APP_EEPROM_INSTANCE, NULL);
    I2C_transfer(NULL, &i2cTransaction);
    I2C_control(NULL, I2C_CMD_SET_BUS_FREQUENCY, &busFrequency);
    I2C_close(NULL);

    return (testResult);
}

static bool I2CApp_probeBusFreqTest(void *arg)
{
    I2C_Handle      handle;
    I2C_Params      i2cParams;
    uint32_t        busFrequency;
    int16_t         transferStatus;
    I2C_Transaction i2cTransaction;
    uint32_t        slaveAddress;
    int32_t         controlStatus;
    bool            testStatus = TRUE;
    uint8_t         txBuf[I2C_APP_EEPROM_TEST_LENGTH + I2C_APP_EEPROM_ADDR_SIZE] = {0x00, };
    uint8_t         rxBuf[I2C_APP_EEPROM_TEST_LENGTH];
    uint32_t        delayValue;
    I2CApp_TestCfg      *test = (I2CApp_TestCfg *)arg;

    I2CApp_initConfig(I2C_APP_EEPROM_INSTANCE, test);

    I2C_Params_init(&i2cParams);
    handle = I2C_open(I2C_APP_EEPROM_INSTANCE, &i2cParams);
    if(handle == NULL)
    {
        testStatus = FALSE;
    }

    /* Set the I2C EEPROM write/read address */
    txBuf[0] = (I2C_APP_EEPROM_TEST_ADDR >> 8) & 0xff; /* EEPROM memory high address byte */
    txBuf[1] = I2C_APP_EEPROM_TEST_ADDR & 0xff;        /* EEPROM memory low address byte */


    /* Test Runtime Configuration of Bus Frequency */

    /* Test runtime configuration of 400 kHz */
    busFrequency = I2C_400kHz;
    I2C_control(handle, I2C_CMD_SET_BUS_FREQUENCY, &busFrequency);

    memset(rxBuf, 0, I2C_APP_EEPROM_TEST_LENGTH);
    I2C_transactionInit(&i2cTransaction);
    i2cTransaction.slaveAddress = test->slaveAddress;
    i2cTransaction.writeBuf = (uint8_t *)&txBuf[0];
    i2cTransaction.writeCount = test->writecount;
    i2cTransaction.readBuf = (uint8_t *)&rxBuf[0];
    i2cTransaction.readCount = test->readcount;
    i2cTransaction.timeout   = I2C_APP_TRANSACTION_TIMEOUT;
    transferStatus = I2C_transfer(handle, &i2cTransaction);

    if(I2C_STS_SUCCESS != transferStatus)
    {
        UART_printf("\n I2C Test: Dynamic configuration of bus Freq failed. \n");
        testStatus = FALSE;
    }
    else
    {
        testStatus = I2CApp_compareData(&gI2CApp_EepromData[0], &rxBuf[0], I2C_APP_EEPROM_TEST_LENGTH);
    }

    if(TRUE == testStatus)
    {
        /* Test runtime configuration of 100 kHz */
        busFrequency = I2C_100kHz;
        I2C_control(handle, I2C_CMD_SET_BUS_FREQUENCY, &busFrequency);

        memset(rxBuf, 0, I2C_APP_EEPROM_TEST_LENGTH);
        transferStatus = I2C_transfer(handle, &i2cTransaction);

        if(I2C_STS_SUCCESS != transferStatus)
        {
            UART_printf("\n I2C Test: Dynamic configuration of bus Freq failed. \n");
            testStatus = FALSE;
        }
        else 
        {
            testStatus = I2CApp_compareData(&gI2CApp_EepromData[0], &rxBuf[0], I2C_APP_EEPROM_TEST_LENGTH);
        }
    }

    if(TRUE == testStatus)
    {
        /* Test runtime configuration default value */
        busFrequency = I2C_1P0Mhz;
        I2C_control(handle, I2C_CMD_SET_BUS_FREQUENCY, &busFrequency);

        memset(rxBuf, 0, I2C_APP_EEPROM_TEST_LENGTH);
        transferStatus = I2C_transfer(handle, &i2cTransaction);

        if(I2C_STS_SUCCESS != transferStatus)
        {
            UART_printf("\n I2C Test: Dynamic configuration of bus Freq failed. \n");
            testStatus = FALSE;
        }
        else{
            testStatus = I2CApp_compareData(&gI2CApp_EepromData[0], &rxBuf[0], I2C_APP_EEPROM_TEST_LENGTH);
        }
    }

    /* Test Probe functionality */

    if(TRUE == testStatus)
    {
        /* Probe test with valid slave address */
        slaveAddress = I2C_APP_EEPROM_ADDR;
        controlStatus = I2C_control(handle, I2C_CMD_PROBE, &slaveAddress);

        if(I2C_STATUS_SUCCESS == controlStatus)
        {
            testStatus = TRUE;
        }
        else
        {
            testStatus = FALSE;
            UART_printf("\n I2C Test: Probe test failed. \n");
        }
    }

    /* Test Invalid CMD */

    if(TRUE == testStatus)
    {
        /* Probe test with valid slave address */
        slaveAddress = test->slaveAddress;
        controlStatus = I2C_control(handle, 0x04, &slaveAddress);

        if(I2C_STATUS_SUCCESS == controlStatus)
        {
            UART_printf("\n I2C Test: VALID CMD... \n");
            testStatus = FALSE;
        }
        else
        {
            testStatus = TRUE;
        }
    }

    if(TRUE == testStatus)
    {
        /* Probe test with invalid slave address */
        slaveAddress = 0x70U;
        controlStatus = I2C_control(handle, I2C_CMD_PROBE, &slaveAddress);

        if(I2C_STATUS_ERROR == controlStatus)
        {
            UART_printf("\n I2C Test: Probe test passed. \n");
        }
        else
        {
            UART_printf("\n I2C Test: Probe test failed. \n");
            testStatus = FALSE;
        }
    }

    if(TRUE == testStatus)
    {
        /* Test bus recovery functionality */
        delayValue = 2000U;
        controlStatus = I2C_control(handle, I2C_CMD_RECOVER_BUS, &delayValue);

        if(I2C_STATUS_SUCCESS == controlStatus)
        {
            memset(rxBuf, 0, I2C_APP_EEPROM_TEST_LENGTH);
            transferStatus = I2C_transfer(handle, &i2cTransaction);

            if(I2C_STS_SUCCESS != transferStatus)
            {
                UART_printf("\n I2C Test: Bus recovery test failed. \n");
                testStatus = FALSE;
            }
            else
            {
                testStatus = I2CApp_compareData(&gI2CApp_EepromData[0], &rxBuf[0], I2C_APP_EEPROM_TEST_LENGTH);
            }
        }
        else
        {
            testStatus = FALSE;
        }
    }

    if(TRUE == testStatus)
    {
        /* Test bus recovery functionality with passing timeout valve to zero*/
        delayValue = 2000U;
        controlStatus = I2C_control(handle, I2C_CMD_RECOVER_BUS, &delayValue);

        if(I2C_STATUS_SUCCESS == controlStatus)
        {
            memset(rxBuf, 0, I2C_APP_EEPROM_TEST_LENGTH);
            I2C_transactionInit(&i2cTransaction);
            i2cTransaction.slaveAddress = test->slaveAddress;
            i2cTransaction.writeBuf = (uint8_t *)&txBuf[0];
            i2cTransaction.writeCount = test->writecount;
            i2cTransaction.readBuf = (uint8_t *)&rxBuf[0];
            i2cTransaction.readCount = test->readcount;
            i2cTransaction.timeout   = 0;
            transferStatus = I2C_transfer(handle, &i2cTransaction);

            if(I2C_STS_SUCCESS != transferStatus)
            {
                UART_printf("\n I2C Test: Bus recovery test failed. \n");
                testStatus = FALSE;
            }
            else
            {
                testStatus = I2CApp_compareData(&gI2CApp_EepromData[0], &rxBuf[0], I2C_APP_EEPROM_TEST_LENGTH);
            }
        }
        else
        {
            testStatus = FALSE;
        }
    }

    if(handle)
    {
        I2C_close(handle);
    }
    return testStatus;
}
