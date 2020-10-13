/**
 *  \file   main_flash_read_test.c
 *
 *  \brief  Example application main file. This application will write and read
 *          the data to/from nor flash through qspi interface.
 *
 */

/*
 * Copyright (C) 2014-2019 Texas Instruments Incorporated - http://www.ti.com/
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

#include <stdlib.h>

#ifdef USE_BIOS
/* XDCtools Header files */
#include <xdc/std.h>
#include <xdc/cfg/global.h>
#include <xdc/runtime/System.h>
#include <stdio.h>
#include <ti/sysbios/knl/Task.h>

/* BIOS Header files */
#include <ti/sysbios/BIOS.h>
#include <xdc/runtime/Error.h>
#endif /* #ifdef USE_BIOS */

/* TI-RTOS Header files */
#include <ti/csl/soc.h>
#include <ti/drv/spi/SPI.h>
#include <ti/drv/spi/soc/QSPI_v1.h>
#include "SPI_log.h"

/* Flash header file */
#include <ti/drv/spi/test/qspi_flash/src/Flash_S25FL/S25FL.h>

#include <ti/board/board.h>

#ifdef SPI_DMA_ENABLE
#include <ti/osal/CacheP.h>
/* EDMA3 Header files */
#if defined(SOC_TPR12)
#include <ti/drv/edma/edma.h>
#else
#include <ti/sdo/edma3/drv/edma3_drv.h>
#include <ti/sdo/edma3/rm/edma3_rm.h>
#include <ti/sdo/edma3/rm/sample/bios6_edma3_rm_sample.h>
#endif
#endif

/**********************************************************************
 ************************** Macros ************************************
 **********************************************************************/
#define QSPI_PER_CNT            (1U)
#define QSPI_INSTANCE           (1U)

#if defined (SOC_AM335x) || defined (SOC_AM437x)
#define QSPI_OFFSET             (5U)
#endif
#if defined(SOC_AM574x) || defined (SOC_AM572x) || defined (SOC_AM571x)
#define QSPI_OFFSET             (4U)
#endif
#if defined(SOC_TPR12)
#define QSPI_OFFSET             (0U)
#endif

#define QSPI_TEST_LENGTH        (75U)  /* read/write test data size in 32-bit words */

/* Unit test IDs */
#define QSPI_TEST_ID_CMD          0   /* Command mode test */
#define QSPI_TEST_ID_MMAP         1   /* Memory map with DMA disabled mode test */
#define QSPI_TEST_ID_MMAP_DMA     2   /* Memory map with DMA enabled mode test */


/* Define the SPI test interface */
typedef struct QSPI_Tests_s
{
    bool     (*testFunc)(void *);
    int32_t  testId;
    bool     intrMode;
    bool     dmaMode;
    bool     mmapMode;
    char     testDesc[80];
    char     *fileName;
    uint32_t flashWriteOffset;
} QSPI_Tests;

/**********************************************************************
 ************************** Internal functions ************************
 **********************************************************************/

/* Function to generate known data */
static void GeneratePattern(uint8_t *txBuf, uint8_t *rxBuf, uint32_t length);

/* Data compare function */
bool VerifyData(unsigned char *expData,
                unsigned char *rxData,
                unsigned int length);

#if defined(SOC_AM574x) || defined (SOC_AM571x) || defined (SOC_AM572x)
void QSPI_board_crossbarInit(void);
#endif

#ifdef SPI_DMA_ENABLE
#if defined(SOC_TPR12)
static EDMA_Handle QSPIApp_edmaInit(void);
#else
static EDMA3_RM_Handle QSPIApp_edmaInit(void);
#endif
#endif

/**********************************************************************
 ************************** Global Variables **************************
 **********************************************************************/

/* Buffer containing the known data that needs to be written to flash */
#if (defined(_TMS320C6X) || defined (__TI_ARM_V7M4__))
#pragma DATA_ALIGN (txBuf, 128)
uint32_t txBuf[1024];
#pragma DATA_ALIGN (rxBuf, 128)
uint32_t rxBuf[1024];
#else
uint32_t txBuf[1024] __attribute__((aligned(128)));
uint32_t rxBuf[1024] __attribute__((aligned(128)));
#endif

unsigned int addrValue = 0x000000U;

/* hardware attributes */
extern QSPI_HwAttrs qspiInitCfg[QSPI_PER_CNT];

/* transfer length */
uint32_t transferLength = 0;

#ifdef SPI_DMA_ENABLE
/**
 * \brief      Function to initialize the edma driver and get the handle to the
 *             edma driver;
 */
#if defined (SOC_TPR12)
EDMA_Handle  gEdmaHandle = NULL;

static EDMA_Handle QSPIApp_edmaInit(void)
{
    uint32_t edma3Id = EDMA_DRV_INST_MSS_A;
    EDMA_instanceInfo_t instanceInfo;
    int32_t errorCode;

    gEdmaHandle = EDMA_getHandle(edma3Id, &instanceInfo);
    if(gEdmaHandle == NULL)
    {
        EDMA3CCInitParams 	initParam;

        EDMA3CCInitParams_init(&initParam);
        initParam.initParamSet = TRUE;
        if (EDMA_init(edma3Id, &initParam) != EDMA_NO_ERROR)
        {
            printf("EDMA_init failed \n");
            return(gEdmaHandle);
        }
        /* Open DMA driver instance 0 for SPI test */
        gEdmaHandle = EDMA_open(edma3Id, &errorCode, &instanceInfo);
        if(gEdmaHandle == NULL)
        {
            printf("Open DMA driver failed with error=%d\n \n", errorCode);
            return(gEdmaHandle);
        }
    }

    return(gEdmaHandle);
}
#else
EDMA3_RM_Handle gEdmaHandle = NULL;

static EDMA3_RM_Handle QSPIApp_edmaInit(void)
{
    EDMA3_DRV_Result edmaResult = EDMA3_DRV_E_INVALID_PARAM;
    uint32_t         edma3Id;

    if (gEdmaHandle != NULL)
    {
        return (gEdmaHandle);
    }

    edma3Id = 0;
    gEdmaHandle = (EDMA3_RM_Handle)edma3init(edma3Id, &edmaResult);

    return(gEdmaHandle);
}
#endif
#endif

void QSPI_test_print_test_desc(QSPI_Tests *test)
{
    char        testId[16] = {0, };

    /* Print unit test ID */
    sprintf(testId, "%d", test->testId);
    SPI_log("\r\n QSPI UT %s\r\n", testId);

    /* Print test description */
    SPI_log("\r\n %s\r\n", test->testDesc);
}

/*
 *  ======== QSPI init config ========
 */
static void QSPI_initConfig(uint32_t instance, QSPI_Tests *test)
{
    QSPI_HwAttrs *cfg = &qspiInitCfg[instance];
    bool          intrMode = test->intrMode;
#ifdef SPI_DMA_ENABLE
    bool          dmaMode = test->dmaMode;
#endif

    if (intrMode == true)
    {
        /* polling mode */
        cfg->intrEnable = true;
    }
    else
    {
        /* interrupt mode */
        cfg->intrEnable = false;
    }

#ifdef SPI_DMA_ENABLE
    if (dmaMode == true)
    {
        cfg->edmaHandle = QSPIApp_edmaInit();
        cfg->dmaEnable  = true;
    }
    else
#endif
    {
        cfg->edmaHandle = NULL;
        cfg->dmaEnable  = false;
    }
}

/**
 *  @b Description
 *  @n
 *      Utility function which converts a local GEM L2 memory address
 *      to global memory address.
 *
 *  @param[in]  addr
 *      Local address to be converted
 *
 *  @retval
 *      Computed L2 global Address
 */
#ifdef SPI_DMA_ENABLE
static uintptr_t l2_global_address (uintptr_t addr)
{
#if defined (SOC_TPR12)
    return ((uintptr_t)CSL_locToGlobAddr(addr));
#else
    return addr;
#endif
}
#endif

static bool QSPI_test_func (void *arg)
{
    QSPI_Tests        *test = (QSPI_Tests *)arg;
    SPI_Params         spiParams;
    S25FL_Handle       flashHandle;            /* Flash handle */
    uint32_t           blockNumber = 0U;       /* Block number */
    S25FL_Transaction  flashTransaction;       /* Flash transaction structure */
    SPI_Handle         handle;                 /* SPI handle */
    QSPI_HwAttrs      *hwAttrs;                /* QSPI hardware attributes */
    bool               retVal = true;          /* return value */
    uint32_t           rxLines;
    uint32_t           qspiMode;

    QSPI_initConfig(0, test);

    /* Default SPI configuration parameters */
    SPI_Params_init(&spiParams);

    /* Open QSPI driver */
    flashHandle = SF25FL_open(((QSPI_INSTANCE - 1)+(QSPI_OFFSET)), &spiParams);

    /* Extract hardware attributes */
    handle = flashHandle->spiHandle;
    hwAttrs = (QSPI_HwAttrs *)handle->hwAttrs;

    /* Print flash Id */
    FlashPrintId(flashHandle);

    for (rxLines = QSPI_RX_LINES_SINGLE; rxLines < (QSPI_RX_LINES_QUAD + 1); rxLines++)
    {
        if (test->mmapMode)
        {
            qspiMode = QSPI_OPER_MODE_MMAP;
            SPI_control(handle, SPI_V1_CMD_SETMEMMORYMAPMODE, (void *)&qspiMode);
        }
        else
        {
            qspiMode = QSPI_OPER_MODE_CFG;
            SPI_control(handle, SPI_V1_CMD_SETCONFIGMODE, (void *)&qspiMode);
        }

        SPI_control(handle, SPI_V1_CMD_SETRXLINES, (void *)&rxLines);

        /* Erase block, to which data has to be written */
        S25FLFlash_BlockErase(flashHandle, blockNumber);

        /* Set the transfer length in number of 32 bit words */
        transferLength = QSPI_TEST_LENGTH;

        /* Generate the data */
        GeneratePattern((uint8_t *)&txBuf[0], (uint8_t *)&rxBuf[0], transferLength);

#ifdef SPI_DMA_ENABLE
        if (test->dmaMode)
        {
            CacheP_wbInv((void *)rxBuf, (int32_t)sizeof(rxBuf));
            CacheP_wbInv((void *)txBuf, (int32_t)sizeof(txBuf));
        }
#endif

        /* Update transaction parameters */
#ifdef SPI_DMA_ENABLE
        if (test->dmaMode)
        {
            flashTransaction.data    = (uint8_t *)l2_global_address((uintptr_t)&txBuf[0]);
        }
        else
#endif
        {
            flashTransaction.data    = (uint8_t *)&txBuf[0];
        }

        if (test->mmapMode)
        {
            flashTransaction.address = addrValue;
        }
        else
        {
            /*
             * For CFG Mode,flashTransaction.address should be assigned with the
             * address of the variable which contains the flash offset value
             */
            flashTransaction.address = (uint32_t)&addrValue;
        }
        flashTransaction.dataSize    = transferLength * 4;  /* In bytes */

        /* Write buffer to flash */
        retVal = SF25FL_bufferWrite(flashHandle, &flashTransaction);

        if(QSPI_RX_LINES_QUAD == hwAttrs->rxLines)
        {
            S25FLFlash_QuadModeEnable(flashHandle);
        }

        /* Update transaction parameters */
#ifdef SPI_DMA_ENABLE
        if (test->dmaMode)
        {
            flashTransaction.data    = (uint8_t *)l2_global_address((uintptr_t)&rxBuf[0]);
        }
        else
#endif
        {
            flashTransaction.data    = (uint8_t *)&rxBuf[0];
        }

        if (test->mmapMode)
        {
            flashTransaction.address = addrValue;
        }
        else
        {
            /*
             * For CFG Mode,flashTransaction.address should be assigned with the
             * address of the variable which contains the flash offset value
             */
            flashTransaction.address = (uint32_t)&addrValue;
        }
        flashTransaction.dataSize    = transferLength * 4; /* In bytes */

        /* Read data from flash */
        retVal = SF25FL_bufferRead(flashHandle, &flashTransaction);

        /* Verify Data */
        retVal = VerifyData((unsigned char *)&txBuf[0], (unsigned char *)&rxBuf[0],
            transferLength);

        if (retVal == false)
        {
            break;
        }
    }

    SF25FL_close(flashHandle);

    return (retVal);
}

#include <string.h>
#include <stdio.h>

#define MEM_SIZE_KB                         (1024)
#define QSPI_DEVICE_BLOCK_SIZE              (64 * MEM_SIZE_KB)
/* Maximum length of each config line */
#define MAX_LINE_LENGTH                     256
#define FREAD_TEST_CONFIG_FILE_PATH         "C:\\delete\\FlashReadConfig.txt"

uint8_t fileLoadBuffer[192 * 1024];
uint8_t fileReadBuffer[192 * 1024];

/**
 * @brief - QSPI_test_flashWrite() - function to do flash SPI
 *
 * @param
 *     handle = pointer to SPI handle
 *     src = byte pointer to source
 *     length = size of source to copy
 *     offset = SPI offset to flash into
 *
 * @return - int32t
 *      0 = Init completed successfully
 *     <0 = Negative value indicate error occurred
 *
 */
int32_t QSPI_test_flashWrite(S25FL_Handle flashHandle, uint8_t *src, uint32_t length,
    uint32_t offset, bool dmaMode, bool mmapMode)
{
    uint32_t startBlockNumber, endBlockNumber;
    uint32_t i;
    S25FL_Transaction flashTransaction;
    int32_t retVal;

    /* Computing the block numbers to be erased  */
    startBlockNumber = (offset / QSPI_DEVICE_BLOCK_SIZE);
    endBlockNumber = (offset + length) /
        QSPI_DEVICE_BLOCK_SIZE;

    for (i = startBlockNumber; i <= endBlockNumber; i++)
    {
        S25FLFlash_BlockErase(flashHandle, i);
    }

#ifdef SPI_DMA_ENABLE
    if (dmaMode)
    {
        CacheP_wbInv((void *)src, length);
    }
#endif

    /* Update transaction parameters */
#ifdef SPI_DMA_ENABLE
    if (dmaMode)
    {
        flashTransaction.data    = (uint8_t *)l2_global_address((uintptr_t)src);
    }
    else
#endif
    {
        flashTransaction.data    = src;
    }

    if (mmapMode)
    {
        flashTransaction.address = offset;
    }
    else
    {
        /*
         * For CFG Mode,flashTransaction.address should be assigned with the
         * address of the variable which contains the flash offset value
         */
        flashTransaction.address = (uint32_t)&offset;
    }
    flashTransaction.dataSize    = length;  /* In bytes */

    /* Write buffer to flash */
    retVal = SF25FL_bufferWrite(flashHandle, &flashTransaction);

    DebugP_assert(retVal == true);
    return !retVal;
}


/**
 * @brief - QSPI_test_flashRead() - function to do flash SPI
 *
 * @param
 *     handle = pointer to SPI handle
 *     dst = byte pointer to destination
 *     length = size of source to copy
 *     offset = SPI offset to flash into
 *
 * @return - int32t
 *      0 = Init completed successfully
 *     <0 = Negative value indicate error occurred
 *
 */
int32_t QSPI_test_flashRead(S25FL_Handle flashHandle, uint8_t *dst, uint32_t length,
    uint32_t offset, bool dmaMode, bool mmapMode, bool enableQuadMode)
{
    S25FL_Transaction flashTransaction;
    int32_t retVal;

    if(enableQuadMode)
    {
        S25FLFlash_QuadModeEnable(flashHandle);
    }

#ifdef SPI_DMA_ENABLE
    if (dmaMode)
    {
        CacheP_wbInv((void *)dst, length);
    }
#endif

        /* Update transaction parameters */
#ifdef SPI_DMA_ENABLE
    if (dmaMode)
    {
        flashTransaction.data    = (uint8_t *)l2_global_address((uintptr_t)dst);
    }
    else
#endif
    {
        flashTransaction.data    = dst;
    }

    if (mmapMode)
    {
        flashTransaction.address = offset;
    }
    else
    {
        /*
         * For CFG Mode,flashTransaction.address should be assigned with the
         * address of the variable which contains the flash offset value
         */
        flashTransaction.address = (uint32_t)&offset;
    }
    flashTransaction.dataSize    = length; /* In bytes */

    /* Read data from flash */
    retVal = SF25FL_bufferRead(flashHandle, &flashTransaction);

    DebugP_assert(retVal == true);
    /* SPI_transfer() returns TRUE if successful
       SBL_qspiFlashRead() needs to return 0 if there's no errors */
    return !retVal;
}

size_t  QSPI_freadProgress(void *ptr, size_t size, size_t nmemb, FILE *fp)
{
    uint32_t chunkCount;
    size_t chunkSize = (nmemb / 100);
    size_t totalReadSize;
    size_t chunkReadSize;
    uint8_t *fileLoadBuffer = ptr;

    totalReadSize = 0;
    SPI_log("\r\n File Read Progress:\n");
    if (chunkSize > 0)
    {
        for (chunkCount = 0; chunkCount < 100; chunkCount++)
        {
            chunkReadSize = fread(fileLoadBuffer, size, chunkSize, fp);
            if (chunkReadSize != chunkSize)
            {
                break;
            }
            totalReadSize += chunkSize;
            fileLoadBuffer += (size * chunkSize);
            if ((chunkCount % 5) == 0)
            {
                SPI_log("%d%% \r\n", chunkCount);
            }
        }
    }
    if ((nmemb - totalReadSize) > 0)
    {
        chunkReadSize = fread(fileLoadBuffer, size, (nmemb - totalReadSize), fp);
        totalReadSize += chunkReadSize;
    }
    return totalReadSize;
}


int32_t QSPI_test_readInputFile(S25FL_Handle flashHandle, QSPI_Tests *test, bool enableQuadMode)
{
    FILE *fp;
    FILE *binPtr;
    char line[MAX_LINE_LENGTH];
    char fileName[MAX_LINE_LENGTH];
    int32_t offsetAddr = 0U;
    uint32_t len, i;
    int32_t ret;
    const char *s = FREAD_TEST_CONFIG_FILE_PATH;
    uint32_t readlen;

    fp = fopen(s, "r");
    if (fp == NULL)
    {
        SPI_log("\r\n Error opening file %s\r\n", s);
        return -1;
    }

    memset(line, 0, MAX_LINE_LENGTH);

    while (fgets(line, MAX_LINE_LENGTH, fp) != 0)
    {
        if (sscanf(line,"%s %x", fileName, &offsetAddr) < 0)
        {
            SPI_log("Error parsing config line -\n");
            SPI_log("\t Make sure each line is in the format: [filename] [address]\n");
            fclose(fp);
            return -2;
        }
        else
        {
            SPI_log("Parsed config line, received parameters: filename = %s, address = 0x%x\n", fileName, offsetAddr);
            binPtr = fopen(fileName, "rb");
            if (binPtr == NULL)
            {
                SPI_log("\tUnable to open file to load: %s\n", fileName);
                fclose(fp);
                return -3;
            }
            fseek(binPtr, 0, SEEK_END);
            len = ftell(binPtr);
            fseek(binPtr, 0, SEEK_SET);
            if (len == 0)
            {
                SPI_log("\tUnable to read size of file %s\n", fileName);
                fclose(binPtr);
                fclose(fp);
                return -4;
            }
            else
            {
                SPI_log("\tSize of %s is 0x%x\n", fileName, len);
            }
            if (len > sizeof(fileLoadBuffer))
            {
                SPI_log("\tFile size exceeds max size  %d\n", sizeof(fileLoadBuffer));
                fclose(binPtr);
                fclose(fp);
                return -4;
            }
            SPI_log("\tLoading binary to memory ...\n");
            readlen = QSPI_freadProgress(fileLoadBuffer, 1, len, binPtr);
            DebugP_assert(readlen == len);
            SPI_log("\tFinished loading binary to memory!\n");
            if (ret = QSPI_test_flashWrite(flashHandle, fileLoadBuffer, len, offsetAddr, test->dmaMode, test->mmapMode))
            {
                SPI_log("\tError flashing memory! Error code %d\n", ret);
                return -5;
            }
            else
            {
                SPI_log("\tFlashed %s to offset 0x%x!\n", fileName, offsetAddr);
            }
            if (ret = QSPI_test_flashRead(flashHandle, fileReadBuffer, len, offsetAddr, test->dmaMode, test->mmapMode, enableQuadMode))
            {
                SPI_log("\tError reading memory at addr 0x%x\n", offsetAddr);
                return -6;
            }
            else
            {
                SPI_log("\tRead flash memory at 0x%x, checking flashed content...\n", offsetAddr);
            }
            for (i = 0; i<len; i+=4)
            {
                if ( (*(uint32_t *) (fileLoadBuffer + i)) != (*(uint32_t *) (fileReadBuffer + i)) )
                {
                    SPI_log("\t\tMismatched data at offset 0x%x, expected = 0x%08x, read = 0x%08x\n",
                        i, (*(uint32_t *) (fileLoadBuffer + i)), (*(uint32_t *) (fileReadBuffer + i)));
                    ret = -7;
                }
            }
            if (ret == -7)
            {
                SPI_log("\tVerifying flashed data failed!\n");
                //return ret;
            }
            else
            {
                SPI_log("\tVerified flash data equal expected data!\n");
            }
            fclose(binPtr);

        }
    }
    fclose(fp);

    return 0;
}

static bool QSPI_test_func_file_write (void *arg)
{
    QSPI_Tests        *test = (QSPI_Tests *)arg;
    SPI_Params         spiParams;
    S25FL_Handle       flashHandle;            /* Flash handle */
    SPI_Handle         handle;                 /* SPI handle */
    QSPI_HwAttrs      *hwAttrs;                /* QSPI hardware attributes */
    bool               retVal = true;          /* return value */
    uint32_t           rxLines;
    uint32_t           qspiMode;

    QSPI_initConfig(0, test);

    /* Default SPI configuration parameters */
    SPI_Params_init(&spiParams);

    /* Open QSPI driver */
    flashHandle = SF25FL_open(((QSPI_INSTANCE - 1)+(QSPI_OFFSET)), &spiParams);

    /* Extract hardware attributes */
    handle = flashHandle->spiHandle;
    hwAttrs = (QSPI_HwAttrs *)handle->hwAttrs;

    /* Print flash Id */
    FlashPrintId(flashHandle);

    rxLines = QSPI_RX_LINES_QUAD;
    if (test->mmapMode)
    {
        qspiMode = QSPI_OPER_MODE_MMAP;
        SPI_control(handle, SPI_V1_CMD_SETMEMMORYMAPMODE, (void *)&qspiMode);
    }
    else
    {
        qspiMode = QSPI_OPER_MODE_CFG;
        SPI_control(handle, SPI_V1_CMD_SETCONFIGMODE, (void *)&qspiMode);
    }

    SPI_control(handle, SPI_V1_CMD_SETRXLINES, (void *)&rxLines);

    retVal = QSPI_test_readInputFile(flashHandle, test,(QSPI_RX_LINES_QUAD == hwAttrs->rxLines));
    SF25FL_close(flashHandle);

    return (retVal);
}

QSPI_Tests qspi_tests[] =
{
    /* testFunc      testID                 intrMode dmaMode mmapMode testDesc */
    {QSPI_test_func, QSPI_TEST_ID_CMD,      true,    false,  false,  "\r\n QSPI flash command mode test", },
    {QSPI_test_func, QSPI_TEST_ID_MMAP,     false,   false,  true,   "\r\n QSPI flash memory map mode test", },
#ifdef SPI_DMA_ENABLE
    {QSPI_test_func, QSPI_TEST_ID_MMAP_DMA, false,   true,   true,   "\r\n QSPI flash memory map mode with DMA enabled test", },
#endif
    {QSPI_test_func_file_write, QSPI_TEST_ID_MMAP_DMA, false,   false,   true,   "\r\n QSPI file write to flash and verify in mmap mode test", },
    {NULL, },
};

/*
 *  ======== test function ========
 */
#ifdef USE_BIOS
void spi_test(UArg arg0, UArg arg1)
#else
void spi_test()
#endif
{
    uint32_t    i;
    QSPI_Tests *test;
    bool        testFail = false;

    /* Init SPI driver */
    SPI_init();

    for (i = 0; ; i++)
    {
        test = &qspi_tests[i];
        if (test->testFunc == NULL)
            break;

        QSPI_test_print_test_desc(test);
        if (test->testFunc((void *)test) == true)
        {
            SPI_log("\r\n %s have passed\r\n", test->testDesc);
        }
        else
        {
            SPI_log("\r\n %s have failed\r\n", test->testDesc);
            testFail = true;
            break;
        }
    }

    if(true == testFail)
    {
        SPI_log("\n All tests have failed. \n");
    }
    else
    {
        SPI_log("\n All tests have passed. \n");
    }

    while(1);
}

/*
 *  ======== main ========
 */
int main(void)
{
    /* Call board init functions */
    Board_initCfg boardCfg;

#if defined (SOC_AM335x) || defined (SOC_AM437x) || defined (SOC_TPR12)
#ifdef USE_BIOS
    Task_Handle task;
    Error_Block eb;

    Error_init(&eb);
    task = Task_create(spi_test, NULL, &eb);
    if (task == NULL) {
        System_printf("Task_create() failed!\n");
        BIOS_exit(0);
    }
#endif
#endif

    boardCfg = BOARD_INIT_PINMUX_CONFIG |
        BOARD_INIT_MODULE_CLOCK |
        BOARD_INIT_UART_STDIO;
    Board_init(boardCfg);

#if defined (SOC_AM571x) || defined (SOC_AM572x) || defined(SOC_AM574x)
    QSPI_board_crossbarInit();
#endif

#ifdef USE_BIOS
    /* Start BIOS */
    BIOS_start();
#else
    spi_test();
#endif

    return (0);
}

/*
 *  ======== Board_initQSPI ========
 */
#if defined (SOC_AM571x) || defined (SOC_AM572x) || defined(SOC_AM574x)
void QSPI_board_crossbarInit(void) {
#ifndef __ARM_ARCH_7A__
    CSL_XbarIrqCpuId                   cpu;
    uint32_t                           cpuEvent;
    uint32_t                           xbarIndex;
#endif
#ifdef C66X
    /* Configure xbar connect for MCSPI3: DSP_IRQ_38 mapped to MCSPI3 intr */
    cpu = CSL_XBAR_IRQ_CPU_ID_DSP1;
    cpuEvent = 38;
    xbarIndex = cpuEvent - 31;

    /* Configure xbar */
    CSL_xbarIrqConfigure (cpu, xbarIndex, CSL_XBAR_QSPI_IRQ);

#elif __ARM_ARCH_7A__
    /* Configure xbar connect for QSPI: MPU_IRQ_35 mapped to QSPI intr */
    *(unsigned int*)0x4A002A80 = (unsigned int)(0x0157001D);

#else

    /* Configure xbar connect for MCSPI3: DSP_IRQ_38 mapped to MCSPI3 intr */
    cpu = CSL_XBAR_IRQ_CPU_ID_IPU1;
    cpuEvent = 62;
    xbarIndex = cpuEvent - 22;

    /* Configure xbar */
    CSL_xbarIrqConfigure (cpu, xbarIndex, CSL_XBAR_QSPI_IRQ);
#endif

}
#endif

/*
 *  ======== CompareData ========
 */
bool VerifyData(unsigned char *expData,
                unsigned char *rxData,
                unsigned int length)
{
    unsigned int idx = 0;
    unsigned int match = 1;
    bool retVal = false;
    unsigned int lenInBytes = length * 4;

    for(idx = 0; ((idx < lenInBytes) && (match != 0)); idx++)
    {
        if(*expData != *rxData) match = 0;
        expData++;
        rxData++;
    }

    if(match == 1) retVal = true;

    return retVal;
}

/*
 *  ======== GeneratePattern ========
 */
static void GeneratePattern(uint8_t *txBuf, uint8_t *rxBuf, uint32_t length)
{
    unsigned int idx;
    unsigned int lenInBytes = length * 4;

    for(idx = 0; idx < lenInBytes; idx++)
    {
        txBuf[idx] = (uint8_t)idx;
        rxBuf[idx] = (uint8_t)0U;
    }
}
