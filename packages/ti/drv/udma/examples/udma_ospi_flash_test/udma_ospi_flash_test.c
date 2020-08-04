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

/**
 *  \file udma_ospi_flash_test.c
 *
 *  \brief UDMA OSPI Flash sample application performs DAC DMA mode ospi write and read 
 *  at 133MHz RCLK and 166MHz RCLK with UDMA 3D transfer using SW trigger method as below
 * 
 *  Loop QxPxN times (icnt3*icnt2*inct1)
 * 
 *      [This following performs OSPI write of icnt0 bytes]
 *      - SW trigger CH 0 (TX Channel) -> Triggers OSPI Write from R5 TCM to OSPI Data Buffer
 *      - wait for CH 0 (TX Channel) icnt0  to complete
 *      - wait for ospi flash to get ready for next write
 * 
 *      [The following performs OSPI read of icnt0 bytes]
 *      - SW trigger CH 1 (RX Channel) -> Triggers OSPI Read from OSPI Data Buffer to R5 TCM 
 *      - Wait for CH 1 (RX Channel) icnt0  to complete
 * 
 *  > Each loop transfers M (icnt0) bytes of data
 *  > R5 TCM and OSPI Data Buffer size is M x N bytes.
 *  > After each N no.of loops (ie, after performing MxN continous bytes tranfer), 
 *    the tranfers restarts from the origin address.
 *    This happens QxP times.
 *    Memory is set to wrap around after MxN bytes of transfer.
 *    This shows how to do tranfer on circular buffer of size MxN bytes
 *
 *  Where,
 *      - M is icnt0 - "appTestObj->numBytes"  
 *          This refers to the no. of bytes transferred per OSPI Read/Write operation.
 *      - N is icnt1 - UDMA_TEST_XFER_CNT 
 *          This parameter can be used to configure the no.of times the transfer operation to be carried out for continous data.
 *      - P is icnt2 = UDMA_TEST_XFER_REPEAT_CNT_0 and
 *        Q is icnt3 = UDMA_TEST_XFER_REPEAT_CNT_1
 *          This parameters can be used to configure the the no.of times to repeat the whole transfer starting from the origin address.
 * 
 *  NOTE: As per the requirments of the specific usecase, N,Q and P can be used interchangeably by making appropiate changes in the TRPD.
 *  For example UDMA_TEST_XFER_CNT can be NxP (icnt1xicnt2) with UDMA_TEST_XFER_REPEAT_CNT as Q.
 *  Or even UDMA_TEST_XFER_CNT can be NxPxQ (icnt1xicnt2xicnt3) with UDMA_TEST_XFER_REPEAT_CNT as 1.
 *  And conversly, UDMA_TEST_XFER_REPEAT_CNT can be NxPxQ (icnt1xicnt2xicnt3) with UDMA_TEST_XFER_CNT as 1.
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdio.h>
#include <ti/csl/soc.h>
#include <ti/csl/hw_types.h>
#include <ti/drv/udma/udma.h>
#include <ti/drv/pm/pmlib.h>
#include <ti/drv/uart/UART.h>
#include <ti/drv/uart/UART_stdio.h>
#include <ti/drv/udma/examples/udma_apputils/udma_apputils.h>
#include <ti/csl/example/ospi/ospi_flash/common/ospi_flash_common.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/*
 * Application test config parameters
 */
/** \brief Maximum Number of bytes to perform OSPI Read/Write per operation (Actual number based on test) */
#define UDMA_TEST_XFER_MAX_NUM_BYTES    (1024U)
/** \brief Number of times to do continous data tranfer */
#define UDMA_TEST_XFER_CNT              (5U)
/** \brief Number of times to repeat whole data tranfer */
#define UDMA_TEST_XFER_REPEAT_CNT_0     (2U)
/** \brief Number of times to repeat whole data tranfer */
#define UDMA_TEST_XFER_REPEAT_CNT_1     (1U)

/*
 * Application other test parameters
 */

/** \brief Total number of bytes to copy and buffer allocation */
#define UDMA_TEST_APP_MAX_TOTAL_NUM_BYTES   (UDMA_TEST_XFER_MAX_NUM_BYTES * UDMA_TEST_XFER_CNT)
/** \brief This ensures every channel memory is aligned */
#define UDMA_TEST_APP_BUF_SIZE_ALIGN        ((UDMA_TEST_APP_MAX_TOTAL_NUM_BYTES + UDMA_CACHELINE_ALIGNMENT) & ~(UDMA_CACHELINE_ALIGNMENT - 1U))
/** \brief Number of times to perform the memcpy operation */
#define UDMA_TEST_APP_LOOP_CNT              (1U)
/** \brief Number of channels - RX channel and TX Channel*/
#define UDMA_TEST_APP_NUM_CH                (2U)

/*
 * Ring parameters
 */
/** \brief Number of ring entries - we can prime this much memcpy operations */
#define UDMA_TEST_APP_RING_ENTRIES      (1U)
/** \brief Size (in bytes) of each ring entry (Size of pointer - 64-bit) */
#define UDMA_TEST_APP_RING_ENTRY_SIZE   (sizeof(uint64_t))
/** \brief Total ring memory */
#define UDMA_TEST_APP_RING_MEM_SIZE     (UDMA_TEST_APP_RING_ENTRIES * \
                                         UDMA_TEST_APP_RING_ENTRY_SIZE)
/** \brief This ensures every channel memory is aligned */
#define UDMA_TEST_APP_RING_MEM_SIZE_ALIGN ((UDMA_TEST_APP_RING_MEM_SIZE + UDMA_CACHELINE_ALIGNMENT) & ~(UDMA_CACHELINE_ALIGNMENT - 1U))
/**
 *  \brief UDMA TR packet descriptor memory.
 *  This contains the CSL_UdmapCppi5TRPD + Padding to sizeof(CSL_UdmapTR15) +
 *  one Type_15 TR (CSL_UdmapTR15) + one TR response of 4 bytes.
 *  Since CSL_UdmapCppi5TRPD is less than CSL_UdmapTR15, size is just two times
 *  CSL_UdmapTR15 for alignment.
 */
#define UDMA_TEST_APP_TRPD_SIZE         ((sizeof(CSL_UdmapTR15) * 2U) + 4U)
/** \brief This ensures every channel memory is aligned */
#define UDMA_TEST_APP_TRPD_SIZE_ALIGN   ((UDMA_TEST_APP_TRPD_SIZE + UDMA_CACHELINE_ALIGNMENT) & ~(UDMA_CACHELINE_ALIGNMENT - 1U))

/*
 * UDMA OSPI Flash test ID definitions
 */
/** \brief OSPI flash test at 133MHz RCLK - Read/Write 16 Bytes */
#define UDMA_OSPI_FLASH_TEST_ID_DAC_DMA_133M_16B     (0U)  
/** \brief OSPI flash test at 166MHz RCLK - Read/Write 16 Bytes */
#define UDMA_OSPI_FLASH_TEST_ID_DAC_DMA_166M_16B     (1U) 
/** \brief OSPI flash test at 133MHz RCLK - Read/Write 32 Bytes */
#define UDMA_OSPI_FLASH_TEST_ID_DAC_DMA_133M_32B     (2U) 
/** \brief OSPI flash test at 166MHz RCLK - Read/Write 32 Bytes */
#define UDMA_OSPI_FLASH_TEST_ID_DAC_DMA_166M_32B     (3U)  
/** \brief OSPI flash test at 133MHz RCLK - Read/Write 64 Bytes */
#define UDMA_OSPI_FLASH_TEST_ID_DAC_DMA_133M_64B     (4U)  
/** \brief OSPI flash test at 166MHz RCLK - Read/Write 64 Bytes */
#define UDMA_OSPI_FLASH_TEST_ID_DAC_DMA_166M_64B     (5U)  
/** \brief OSPI flash test at 133MHz RCLK - Read/Write 128 Bytes */
#define UDMA_OSPI_FLASH_TEST_ID_DAC_DMA_133M_128B    (6U) 
/** \brief OSPI flash test at 166MHz RCLK - Read/Write 128 Bytes */
#define UDMA_OSPI_FLASH_TEST_ID_DAC_DMA_166M_128B    (7U)  
/** \brief OSPI flash test at 133MHz RCLK - Read/Write 256 Bytes */
#define UDMA_OSPI_FLASH_TEST_ID_DAC_DMA_133M_256B    (8U)  
/** \brief OSPI flash test at 166MHz RCLK - Read/Write 256 Bytes */
#define UDMA_OSPI_FLASH_TEST_ID_DAC_DMA_166M_256B    (9U)   
/** \brief OSPI flash test at 133MHz RCLK - Read/Write 512 Bytes */
#define UDMA_OSPI_FLASH_TEST_ID_DAC_DMA_133M_512B    (10U)  
/** \brief OSPI flash test at 166MHz RCLK - Read/Write 512 Bytes */
#define UDMA_OSPI_FLASH_TEST_ID_DAC_DMA_166M_512B    (11U)  
/** \brief OSPI flash test at 133MHz RCLK - Read/Write 1024 Bytes */
#define UDMA_OSPI_FLASH_TEST_ID_DAC_DMA_133M_1024B   (12U) 
/** \brief OSPI flash test at 166MHz RCLK - Read/Write 1024 Bytes */
#define UDMA_OSPI_FLASH_TEST_ID_DAC_DMA_166M_1024B   (13U)
    
/** \brief Get GTC Timer Ticks */
#define App_getGTCTimerTicks() (*((uint64_t *)(CSL_GTC0_GTC_CFG1_BASE + 0x8U)))

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

typedef struct
{
    int32_t                 chIdx;
    struct Udma_ChObj       chObj;
    struct Udma_EventObj    trEventObj;

    Udma_ChHandle           chHandle;
    Udma_EventHandle        trEventHandle;
    Udma_EventPrms          trEventPrms;

    uint32_t                trigger;
    /**< Global0 or Global 1 Trigger - refer \ref CSL_UdmapTrFlagsTrigger. */
    uint32_t                eventSize;
    /**< Refer \ref CSL_UdmapTrFlagsEventSize. */
    uint32_t                triggerType;
    /**< Refer \ref CSL_UdmapTrFlagsTriggerType. */
    uint32_t                eolType;
    /**< Refer \ref CSL_UdmapTrFlagsEol. */
    volatile uint32_t      *pSwTriggerReg;
    uint32_t                triggerMask;

    Udma_DrvHandle          drvHandle;
    SemaphoreP_Handle       transferDoneSem;

    /**< GTC Timer ticks at start of profiling. */
    volatile uint64_t       startTicks[UDMA_TEST_XFER_CNT * UDMA_TEST_XFER_REPEAT_CNT_0 * UDMA_TEST_XFER_REPEAT_CNT_1];
    /**< GTC Timer ticks at stop of profiling. */
    volatile uint64_t       stopTicks[UDMA_TEST_XFER_CNT * UDMA_TEST_XFER_REPEAT_CNT_0 * UDMA_TEST_XFER_REPEAT_CNT_1];
    /**< Measured total no. of GTC Timer ticks. */
    volatile uint64_t       totalTicks[(UDMA_TEST_XFER_CNT * UDMA_TEST_XFER_REPEAT_CNT_0 * UDMA_TEST_XFER_REPEAT_CNT_1) + 1U];
    /**< Elapsed time in nsec. */
    volatile uint64_t       elapsedTime[(UDMA_TEST_XFER_CNT * UDMA_TEST_XFER_REPEAT_CNT_0 * UDMA_TEST_XFER_REPEAT_CNT_1) + 1U];

    uint8_t                 *txRingMem;
#if (UDMA_SOC_CFG_RA_NORMAL_PRESENT == 1)
    uint8_t                 *txCompRingMem;
#endif
    uint8_t                 *trpdMem;

    uint8_t                 *srcBuf;
    uint8_t                 *destBuf;
} App_UdmaChObj;

typedef struct 
{
    int32_t  (*testFunc)(void);
    int32_t  testId;    
    uint32_t clk;
    uint32_t numBytes;
    char     testDesc[80];

} App_UdmaTestObj;

typedef struct
{
    struct Udma_DrvObj      drvObj;
    App_UdmaChObj           appChObj[UDMA_TEST_APP_NUM_CH];
    App_UdmaTestObj         appTestObj;   
    uint32_t                totalNumBytes;
} App_UdmaObj;


/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

static int32_t Udma_ospiFlashTestRun(void);

static int32_t App_ospiFlashTest(App_UdmaObj *appObj);
static int32_t App_udmaOspiFlash(App_UdmaObj *appObj)  __attribute__((section(".udma_critical_fxns")));

static int32_t App_init(App_UdmaObj *appObj);
static int32_t App_deinit(App_UdmaObj *appObj);

static int32_t App_create(App_UdmaObj *appObj);
static int32_t App_delete(App_UdmaObj *appObj);

static void App_udmaTrpdInit(App_UdmaChObj  *appChObj, App_UdmaTestObj *appTestObj) __attribute__((section(".udma_critical_fxns")));

static void App_printPerfResults(App_UdmaObj *appObj);

void App_print(const char *str);
static void App_printNum(const char *str, uint32_t num);
int32_t App_setGTCClk(uint32_t moduleId,
                      uint32_t clkId,
                      uint64_t clkRateHz);


static int32_t App_ospiFlashInit(uint32_t clk);
static int32_t App_ospiFlashStart(uint32_t numBytes) __attribute__((section(".udma_critical_fxns")));

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/*
 * UDMA driver and channel objects
 */
App_UdmaObj gUdmaAppObj;

/*
 * UDMA Memories
 */
static uint8_t gTxRingMem[UDMA_TEST_APP_NUM_CH][UDMA_TEST_APP_RING_MEM_SIZE_ALIGN] __attribute__((aligned(UDMA_CACHELINE_ALIGNMENT), section(".udma_buffer_r5_tcm")));
#if (UDMA_SOC_CFG_RA_NORMAL_PRESENT == 1)
static uint8_t gTxCompRingMem[UDMA_TEST_APP_NUM_CH][UDMA_TEST_APP_RING_MEM_SIZE_ALIGN] __attribute__((aligned(UDMA_CACHELINE_ALIGNMENT), section(".udma_buffer_r5_tcm")));
#endif
static uint8_t gUdmaTrpdMem[UDMA_TEST_APP_NUM_CH][UDMA_TEST_APP_TRPD_SIZE_ALIGN] __attribute__((aligned(UDMA_CACHELINE_ALIGNMENT)));

/*
 * Application Buffers
 */
static uint8_t gUdmaTestTxBuf[UDMA_TEST_APP_BUF_SIZE_ALIGN] __attribute__((aligned(UDMA_CACHELINE_ALIGNMENT), section(".udma_buffer_r5_tcm")));
static uint8_t gUdmaTestRxBuf[UDMA_TEST_APP_BUF_SIZE_ALIGN] __attribute__((aligned(UDMA_CACHELINE_ALIGNMENT), section(".udma_buffer_r5_tcm")));
static uint8_t * gUdmaTestOspiFlashDataAddr = (uint8_t *)(OSPI_FLASH_DATA_BASE_ADDR);

/* Global test pass/fail flag */
static volatile int32_t gUdmaAppResult = UDMA_SOK;

/* No.of ticks taken to do a GTC Reg Read operation */
volatile uint64_t getTicksDelay = 0; 

/* UDMA OSPI Flash Tests data structure */
App_UdmaTestObj gUdmaAppTestObj[] =
{
    /* testFunc, testID, clk, numBytes, testDesc */
    {Udma_ospiFlashTestRun, UDMA_OSPI_FLASH_TEST_ID_DAC_DMA_133M_16B,  OSPI_MODULE_CLK_133M, 16U, "\r\n OSPI flash test at 133MHz RCLK - Read/Write 16 Bytes"},
    {Udma_ospiFlashTestRun, UDMA_OSPI_FLASH_TEST_ID_DAC_DMA_166M_16B,  OSPI_MODULE_CLK_166M, 16U, "\r\n OSPI flash test at 166MHz RCLK - Read/Write 16 Bytes"},
    {Udma_ospiFlashTestRun, UDMA_OSPI_FLASH_TEST_ID_DAC_DMA_133M_32B,  OSPI_MODULE_CLK_133M, 32U, "\r\n OSPI flash test at 133MHz RCLK - Read/Write 32 Bytes"},
    {Udma_ospiFlashTestRun, UDMA_OSPI_FLASH_TEST_ID_DAC_DMA_166M_32B,  OSPI_MODULE_CLK_166M, 32U, "\r\n OSPI flash test at 166MHz RCLK - Read/Write 32 Bytes"},
    {Udma_ospiFlashTestRun, UDMA_OSPI_FLASH_TEST_ID_DAC_DMA_133M_64B,  OSPI_MODULE_CLK_133M, 64U, "\r\n OSPI flash test at 133MHz RCLK - Read/Write 64 Bytes"},
    {Udma_ospiFlashTestRun, UDMA_OSPI_FLASH_TEST_ID_DAC_DMA_166M_64B,  OSPI_MODULE_CLK_166M, 64U, "\r\n OSPI flash test at 166MHz RCLK - Read/Write 64 Bytes"},
    {Udma_ospiFlashTestRun, UDMA_OSPI_FLASH_TEST_ID_DAC_DMA_133M_128B,  OSPI_MODULE_CLK_133M, 128U, "\r\n OSPI flash test at 133MHz RCLK - Read/Write 128 Bytes"},
    {Udma_ospiFlashTestRun, UDMA_OSPI_FLASH_TEST_ID_DAC_DMA_166M_128B,  OSPI_MODULE_CLK_166M, 128U, "\r\n OSPI flash test at 166MHz RCLK - Read/Write 128 Bytes"},
    {Udma_ospiFlashTestRun, UDMA_OSPI_FLASH_TEST_ID_DAC_DMA_133M_256B,  OSPI_MODULE_CLK_133M, 256U, "\r\n OSPI flash test at 133MHz RCLK - Read/Write 256 Bytes"},
    {Udma_ospiFlashTestRun, UDMA_OSPI_FLASH_TEST_ID_DAC_DMA_166M_256B,  OSPI_MODULE_CLK_166M, 256U, "\r\n OSPI flash test at 166MHz RCLK - Read/Write 256 Bytes"},
    {Udma_ospiFlashTestRun, UDMA_OSPI_FLASH_TEST_ID_DAC_DMA_133M_512B,  OSPI_MODULE_CLK_133M, 512U, "\r\n OSPI flash test at 133MHz RCLK - Read/Write 512 Bytes"},
    {Udma_ospiFlashTestRun, UDMA_OSPI_FLASH_TEST_ID_DAC_DMA_166M_512B,  OSPI_MODULE_CLK_166M, 512U, "\r\n OSPI flash test at 166MHz RCLK - Read/Write 512 Bytes"},
    {Udma_ospiFlashTestRun, UDMA_OSPI_FLASH_TEST_ID_DAC_DMA_133M_1024B,  OSPI_MODULE_CLK_133M, 1024U, "\r\n OSPI flash test at 133MHz RCLK - Read/Write 1024 Bytes"},
    {Udma_ospiFlashTestRun, UDMA_OSPI_FLASH_TEST_ID_DAC_DMA_166M_1024B,  OSPI_MODULE_CLK_166M, 1024U, "\r\n OSPI flash test at 166MHz RCLK - Read/Write 1024 Bytes"},
    {NULL, }
};

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

/*
 * UDMA OSPI Flash test
 */
int32_t Udma_ospiFlashTest(void)
{
    int32_t          retVal;
    uint32_t         i;
    App_UdmaTestObj *test;
    App_UdmaObj     *appObj = &gUdmaAppObj;

    for (i = 0; ; i++)
    {
        test = &gUdmaAppTestObj[i];
        if (test->testFunc == NULL)
        {
            break;
        }

        appObj->appTestObj = *test;
        appObj->totalNumBytes = test->numBytes * UDMA_TEST_XFER_CNT;
        retVal = test->testFunc();
        if(UDMA_SOK == retVal)
        {
            App_print(test->testDesc);
            App_print(" have passed\r\n");
        }
        else
        {
            App_print(test->testDesc);
            App_print(" have failed\r\n");
            break;
        }
    }

    if(UDMA_SOK != retVal)
    {
        App_print("\n Some tests have failed. \n");
    }
    else
    {
        App_print("\n All tests have passed. \n");
    }

    App_print("\n Done\n");

    return (0);
}

/*
 * UDMA OSPI Flash test run
 */
static int32_t Udma_ospiFlashTestRun(void)
{
    int32_t         retVal;
    App_UdmaObj    *appObj = &gUdmaAppObj;

    retVal = App_init(appObj);
    if(UDMA_SOK != retVal)
    {
        App_print("\n [Error] UDMA App init failed!!\n");
    }

    App_print("UDMA OSPI Flash application started...\n");

    if(UDMA_SOK == retVal)
    {
        retVal = App_create(appObj);
        if(UDMA_SOK != retVal)
        {
            App_print("\n [Error] UDMA App create failed!!\n");
        }
    }

    if(UDMA_SOK == retVal)
    {
        retVal = App_ospiFlashTest(appObj);
        if(UDMA_SOK != retVal)
        {
            App_print("\n [Error] UDMA App OSPI Flash test failed!!\n");
        }
    }

    retVal += App_delete(appObj);
    if(UDMA_SOK != retVal)
    {
        App_print("\n [Error] UDMA App delete failed!!\n");
    }

    retVal += App_deinit(appObj);
    if(UDMA_SOK != retVal)
    {
        App_print("\n [Error] UDMA App deinit failed!!\n");
    }

    return (0);
}

static int32_t App_ospiFlashTest(App_UdmaObj *appObj)
{
    int32_t         retVal = UDMA_SOK;
    uint32_t        loopCnt = 0U;
    uint32_t        i;
    uint8_t        *rxBuf;

    while(loopCnt < UDMA_TEST_APP_LOOP_CNT)
    {
        /* Reset RX buffer */
        rxBuf  = &gUdmaTestRxBuf[0U];
        for(i = 0U; i < appObj->totalNumBytes; i++)
        {
            rxBuf[i] = 0U;
        }
        /* Writeback RX buffer */
        Udma_appUtilsCacheWb(rxBuf, appObj->totalNumBytes);

        /* Perform UDMA memcpy */
        retVal = App_udmaOspiFlash(appObj);
        if(UDMA_SOK != retVal)
        {
            break;
        }
        else
        {
            /* Print performance results for OSPI Flash in DAC DMA mode */
            App_printPerfResults(appObj);
        }
    
        loopCnt++;
    }

    return (retVal);
}

static int32_t App_udmaOspiFlash(App_UdmaObj *appObj)
{
    int32_t         retVal = UDMA_SOK;
    uint32_t       *pTrResp, trRespStatus;
    uint8_t        *trpdMem;
    uint64_t        pDesc = 0;
    int32_t         chIdx;
    uint32_t        i;
    App_UdmaTestObj *appTestObj = &appObj->appTestObj;
    App_UdmaChObj  *appChObj;
    Udma_ChHandle   chHandle;
    uint32_t        triggerCnt, tCnt;
    uint8_t        *rxBuf, *txBuf;
    const CSL_ospi_flash_cfgRegs *baseAddr = (const CSL_ospi_flash_cfgRegs *)(OSPI_FLASH_CONFIG_REG_BASE_ADDR);
    
    rxBuf  = &gUdmaTestRxBuf[0U];
    txBuf  = &gUdmaTestTxBuf[0U];

    retVal = App_ospiFlashStart(appObj->totalNumBytes);

    if(UDMA_SOK == retVal)
    {
        for(chIdx = 0U; chIdx < UDMA_TEST_APP_NUM_CH; chIdx++)
        {
            appChObj = &appObj->appChObj[chIdx];
            chHandle = appChObj->chHandle;

            /* Get SW trigger register for easy access */
            appChObj->triggerMask = ((uint32_t)1U << (appChObj->trigger - 1U));
            appChObj->pSwTriggerReg = (volatile uint32_t *) Udma_chGetSwTriggerRegister(chHandle);
            if(NULL == appChObj->pSwTriggerReg)
            {
                App_print("\n [Error] Channel trigger register get failed!!\n");
                break;
            }

            /* Submit TRPD to channels */
            App_udmaTrpdInit(appChObj, appTestObj);
            retVal = Udma_ringQueueRaw(
                        Udma_chGetFqRingHandle(chHandle),
                        (uint64_t) Udma_appVirtToPhyFxn(appChObj->trpdMem, 0U, NULL));
            if(UDMA_SOK != retVal)
            {
                App_print("\n [Error] Channel queue failed!!\n");
                break;
            }
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Set number of times to trigger based on event size */
        triggerCnt = UDMA_TEST_XFER_CNT * UDMA_TEST_XFER_REPEAT_CNT_0 * UDMA_TEST_XFER_REPEAT_CNT_1;
        for(tCnt = 0U; tCnt < triggerCnt; tCnt++)
        {
             /**********************************
             * OSPI Write "appTestObj->numBytes" 
             ***********************************/
            appChObj = &appObj->appChObj[0U]; /* TX Channel */

            appChObj->startTicks[tCnt] = App_getGTCTimerTicks();
            /* Set TX channel trigger and wait for completion */
            CSL_REG32_WR(appChObj->pSwTriggerReg, appChObj->triggerMask);

            while(1U)
            {
                volatile uint64_t   intrStatusReg;
                intrStatusReg = CSL_REG64_RD(appChObj->trEventPrms.intrStatusReg);
                if(intrStatusReg & appChObj->trEventPrms.intrMask)
                {
                    /* Clear interrupt */
                    CSL_REG64_WR(appChObj->trEventPrms.intrClearReg, appChObj->trEventPrms.intrMask);
                    break;
                }
            }

            /* Wait device to be ready after write operation */
            uint32_t timeOutVal = OSPI_FLASH_WRITE_TIMEOUT;
            uint32_t retry = OSPI_FLASH_WRITE_TIMEOUT;
            volatile uint32_t delay = OSPI_FLASH_CHECK_IDLE_DELAY;
            uint8_t  status = 0xff;
            uint32_t regVal;
            while (timeOutVal != 0U)
            {
                (void)CSL_ospiCmdRead(baseAddr, OSPI_FLASH_CMD_RDSR, 1U);
                while(!CSL_ospiIsIdle(baseAddr));
                CSL_ospiFlashExecCmd(baseAddr);
                while(retry != 0U)
                {
                    if(CSL_ospiFlashExecCmdComplete(baseAddr) == TRUE)
                    {
                        break;
                    }
                    while (delay > 0U)
                    {  
                        delay = delay - 1U;
                    }
                    retry--;
                }
                while(!CSL_ospiIsIdle(baseAddr));
                regVal = CSL_REG32_RD(&baseAddr->FLASH_RD_DATA_LOWER_REG);
                (void)memcpy((void *)&status, (void *)(&regVal), 1U);
                if ((status & 1U) == 0U)
                {
                    break;
                }
                timeOutVal--;
                delay = OSPI_FLASH_CHECK_IDLE_DELAY;
                while (delay > 0U)
                {  
                    delay = delay - 1U;
                }
            }
            appChObj->stopTicks[tCnt] = App_getGTCTimerTicks();


            /**********************************
             * OSPI Read "appTestObj->numBytes"  
             **********************************/
            appChObj = &appObj->appChObj[1U]; /* RX Channel */

            appChObj->startTicks[tCnt] = App_getGTCTimerTicks();
            /* Set RX channel trigger and wait for completion */
            CSL_REG32_WR(appChObj->pSwTriggerReg, appChObj->triggerMask);

            while(1U)
            {
                volatile uint64_t   intrStatusReg;
                intrStatusReg = CSL_REG64_RD(appChObj->trEventPrms.intrStatusReg);
                if(intrStatusReg & appChObj->trEventPrms.intrMask)
                {
                    /* Clear interrupt */
                    CSL_REG64_WR(appChObj->trEventPrms.intrClearReg, appChObj->trEventPrms.intrMask);
                    break;
                }
            }
            appChObj->stopTicks[tCnt] = App_getGTCTimerTicks();

            /* NOTE: Depending on the use-case applications can implement the above reatime loop in the folllowing order also
             * 
             * Read(feedback);
             * results = Process(feedback);
             * Write(results);
             * 
             */
        }
    }

    if(UDMA_SOK == retVal)
    {
        for(chIdx = 0U; chIdx < UDMA_TEST_APP_NUM_CH; chIdx++)
        {
            appChObj = &appObj->appChObj[chIdx];
            chHandle = appChObj->chHandle;
            trpdMem  = appChObj->trpdMem;

            /* wait for response to be received in completion queue */
            while(1)
            {
                retVal =
                    Udma_ringDequeueRaw(Udma_chGetCqRingHandle(chHandle), &pDesc);
                if(UDMA_SOK == retVal)
                {
                    break;
                }
            }

            /*
             * Sanity check
             */
            /* Check returned descriptor pointer */
            if(((uint64_t) Udma_appPhyToVirtFxn(pDesc, 0U, NULL)) != ((uint64_t) trpdMem))
            {
                App_print("\n [Error] TR descriptor pointer returned doesn't "
                       "match the submitted address!!\n");
                retVal = UDMA_EFAIL;
            }

            if(UDMA_SOK == retVal)
            {
                /* Invalidate cache */
                Udma_appUtilsCacheInv(trpdMem, UDMA_TEST_APP_TRPD_SIZE_ALIGN);

                /* check TR response status */
                pTrResp = (uint32_t *) (trpdMem + (sizeof(CSL_UdmapTR15) * 2U));
                trRespStatus = CSL_FEXT(*pTrResp, UDMAP_TR_RESPONSE_STATUS_TYPE);
                if(trRespStatus != CSL_UDMAP_TR_RESPONSE_STATUS_COMPLETE)
                {
                    App_print("\n [Error] TR Response not completed!!\n");
                    retVal = UDMA_EFAIL;
                }
            }

            if(UDMA_SOK != retVal)
            {
                break;
            }
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Compare data */
        /* Invalidate RX buffer */
        Udma_appUtilsCacheInv(rxBuf, appObj->totalNumBytes);
        for(i = 0U; i < appObj->totalNumBytes; i++)
        {
            if(rxBuf[i] != txBuf[i])
            {
                App_printNum("\n [Error] Data mismatch at idx %d", i);
                retVal = UDMA_EFAIL;
                break;
            }
        }
    }

    return (retVal);
}

static int32_t App_init(App_UdmaObj *appObj)
{
    int32_t         retVal;
    Udma_InitPrms   initPrms;
    uint32_t        instId;
    int32_t         chIdx;
    App_UdmaChObj  *appChObj;
    App_UdmaTestObj *appTestObj = &appObj->appTestObj;
    Udma_DrvHandle  drvHandle = &appObj->drvObj;
    uint32_t        i;
    uint8_t        *srcBuf;

#if defined (SOC_AM64X)
    /* Use Block Copy DMA for AM64x */
    instId = UDMA_INST_ID_BCDMA_0;
#else
    /* Use MCU NAVSS for MCU domain cores. Rest all cores uses Main NAVSS */
#if defined (BUILD_MCU1_0) || defined (BUILD_MCU1_1)
    instId = UDMA_INST_ID_MCU_0;
#else
    instId = UDMA_INST_ID_MAIN_0;
#endif
#endif
    /* UDMA driver init */
    UdmaInitPrms_init(instId, &initPrms);
    initPrms.virtToPhyFxn   = &Udma_appVirtToPhyFxn;
    initPrms.phyToVirtFxn   = &Udma_appPhyToVirtFxn;
    initPrms.printFxn       = &App_print;
    retVal = Udma_init(drvHandle, &initPrms);
    if(UDMA_SOK != retVal)
    {
        App_print("\n [Error] UDMA init failed!!\n");
    }

    /* Init channel parameters */
    for(chIdx = 0U; chIdx < UDMA_TEST_APP_NUM_CH; chIdx++)
    {
        appChObj                    = &appObj->appChObj[chIdx];
        appChObj->chIdx             = chIdx;
        appChObj->chHandle          = &appChObj->chObj;
        appChObj->drvHandle         = drvHandle;
        appChObj->transferDoneSem   = NULL;
        appChObj->trEventHandle     = NULL;
        appChObj->trigger           = CSL_UDMAP_TR_FLAGS_TRIGGER_GLOBAL0;
        appChObj->eventSize         = CSL_UDMAP_TR_FLAGS_EVENT_SIZE_ICNT1_DEC;
        appChObj->triggerType       = CSL_UDMAP_TR_FLAGS_TRIGGER_TYPE_ICNT1_DEC;
        appChObj->eolType           = CSL_UDMAP_TR_FLAGS_EOL_ICNT0; 
        appChObj->txRingMem         = &gTxRingMem[chIdx][0U];
#if (UDMA_SOC_CFG_RA_NORMAL_PRESENT == 1)
        appChObj->txCompRingMem     = &gTxCompRingMem[chIdx][0U];
#endif
        appChObj->trpdMem           = &gUdmaTrpdMem[chIdx][0U];
        if(0U == chIdx) /* TX Channel */
        {   
            appChObj->srcBuf        = &gUdmaTestTxBuf[0U];
            appChObj->destBuf       = gUdmaTestOspiFlashDataAddr;
            /* Init buffers */
            srcBuf = appChObj->srcBuf;
            for(i = 0U; i < appObj->totalNumBytes; i++)
            {
                srcBuf[i] = i;
            }
            /* Writeback source buffer */
            Udma_appUtilsCacheWb(appChObj->srcBuf, appObj->totalNumBytes);                
        }
        else /* RX Channel */
        {     
            appChObj->srcBuf        = gUdmaTestOspiFlashDataAddr;
            appChObj->destBuf       = &gUdmaTestRxBuf[0U];
        }
        
        memset((void *)&appChObj->startTicks, 0, sizeof(appChObj->startTicks));
        memset((void *)&appChObj->stopTicks, 0, sizeof(appChObj->stopTicks));
        memset((void *)&appChObj->totalTicks, 0, sizeof(appChObj->totalTicks));
        memset((void *)&appChObj->elapsedTime, 0, sizeof(appChObj->elapsedTime));
    }

    if(UDMA_SOK == retVal)
    {
       retVal = App_ospiFlashInit(appTestObj->clk);
    }

    return (retVal);
}


static int32_t App_deinit(App_UdmaObj *appObj)
{
    int32_t         retVal;
    Udma_DrvHandle  drvHandle = &appObj->drvObj;

    OspiFlash_ospiClose(FALSE);

    retVal = Udma_deinit(drvHandle);
    if(UDMA_SOK != retVal)
    {
        App_print("\n [Error] UDMA deinit failed!!\n");
    }

    return (retVal);
}
static int32_t App_create(App_UdmaObj *appObj)
{
    int32_t             retVal = UDMA_SOK;
    uint32_t            chType;
    Udma_ChPrms         chPrms;
    Udma_ChTxPrms       txPrms;
    Udma_ChRxPrms       rxPrms;
    Udma_EventHandle    eventHandle;
    SemaphoreP_Params   semPrms;
    int32_t             chIdx;
    App_UdmaChObj      *appChObj;
    Udma_ChHandle       chHandle;
    Udma_DrvHandle      drvHandle = &appObj->drvObj;

    for(chIdx = 0U; chIdx < UDMA_TEST_APP_NUM_CH; chIdx++)
    {
        appChObj = &appObj->appChObj[chIdx];
        chHandle = appChObj->chHandle;

        SemaphoreP_Params_init(&semPrms);
        appChObj->transferDoneSem = SemaphoreP_create(0, &semPrms);
        if(NULL == appChObj->transferDoneSem)
        {
            App_print("\n [Error] Sem create failed!!\n");
            retVal = UDMA_EFAIL;
        }

        if(UDMA_SOK == retVal)
        {
            /* Init channel parameters */
            chType = UDMA_CH_TYPE_TR_BLK_COPY;
            UdmaChPrms_init(&chPrms, chType);
            chPrms.fqRingPrms.ringMem       = appChObj->txRingMem;
            chPrms.fqRingPrms.ringMemSize   = UDMA_TEST_APP_RING_MEM_SIZE;
            chPrms.fqRingPrms.elemCnt       = UDMA_TEST_APP_RING_ENTRIES;
#if (UDMA_SOC_CFG_RA_NORMAL_PRESENT == 1)
            chPrms.cqRingPrms.ringMem       = appChObj->txCompRingMem;
            chPrms.cqRingPrms.ringMemSize   = UDMA_TEST_APP_RING_MEM_SIZE;
            chPrms.cqRingPrms.elemCnt       = UDMA_TEST_APP_RING_ENTRIES;
#endif

            /* Open channel for block copy */
            retVal = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
            if(UDMA_SOK != retVal)
            {
                App_print("\n [Error] UDMA channel open failed!!\n");
            }
        }

        if(UDMA_SOK == retVal)
        {
            /* Config TX channel */
            UdmaChTxPrms_init(&txPrms, chType);
            retVal = Udma_chConfigTx(chHandle, &txPrms);
            if(UDMA_SOK != retVal)
            {
                App_print("\n [Error] UDMA TX channel config failed!!\n");
            }
        }

        if(UDMA_SOK == retVal)
        {
            /* Config RX channel - which is implicitly paired to TX channel in
             * block copy mode */
            UdmaChRxPrms_init(&rxPrms, chType);
            retVal = Udma_chConfigRx(chHandle, &rxPrms);
            if(UDMA_SOK != retVal)
            {
                App_print("\n [Error] UDMA RX channel config failed!!\n");
            }
        }

        if(UDMA_SOK == retVal)
        {
            /* Register TR event */
            eventHandle = &appChObj->trEventObj;
            UdmaEventPrms_init(&appChObj->trEventPrms);
            appChObj->trEventPrms.eventType         = UDMA_EVENT_TYPE_TR;
            appChObj->trEventPrms.eventMode         = UDMA_EVENT_MODE_SHARED;
            appChObj->trEventPrms.chHandle          = chHandle;
            appChObj->trEventPrms.masterEventHandle = NULL;
            appChObj->trEventPrms.eventCb           = NULL;
            appChObj->trEventPrms.appData           = appChObj;
            retVal = Udma_eventRegister(drvHandle, eventHandle, &appChObj->trEventPrms);
            if(UDMA_SOK != retVal)
            {
                App_print("\n [Error] UDMA TR event register failed!!\n");
            }
            else
            {
                appChObj->trEventHandle = eventHandle;
            }
        }

        if(UDMA_SOK == retVal)
        {
            /* Channel enable */
            retVal = Udma_chEnable(chHandle);
            if(UDMA_SOK != retVal)
            {
                App_print("\n [Error] UDMA channel enable failed!!\n");
            }
        }

        if(UDMA_SOK != retVal)
        {
            break;
        }
    }

    return (retVal);
}

static int32_t App_delete(App_UdmaObj *appObj)
{
    int32_t         retVal, tempRetVal;
    uint64_t        pDesc;
    int32_t         chIdx;
    App_UdmaChObj  *appChObj;
    Udma_ChHandle   chHandle;

    for(chIdx = 0U; chIdx < UDMA_TEST_APP_NUM_CH; chIdx++)
    {
        appChObj = &appObj->appChObj[chIdx];
        chHandle = appChObj->chHandle;

        retVal = Udma_chDisable(chHandle, UDMA_DEFAULT_CH_DISABLE_TIMEOUT);
        if(UDMA_SOK != retVal)
        {
            App_print("\n [Error] UDMA channel disable failed!!\n");
        }

        /* Flush any pending request from the free queue */
        while(1)
        {
            tempRetVal = Udma_ringFlushRaw(
                             Udma_chGetFqRingHandle(chHandle), &pDesc);
            if(UDMA_ETIMEOUT == tempRetVal)
            {
                break;
            }
        }
    }

    for(chIdx = UDMA_TEST_APP_NUM_CH - 1U; chIdx >=0 ; chIdx--)
    {
        appChObj = &appObj->appChObj[chIdx];
        chHandle = appChObj->chHandle;

        /* Unregister all events */
        if(NULL != appChObj->trEventHandle)
        {
            retVal += Udma_eventUnRegister(appChObj->trEventHandle);
            if(UDMA_SOK != retVal)
            {
                App_print("\n [Error] UDMA event unregister failed!!\n");
            }
            appChObj->trEventHandle = NULL;
        }

        retVal += Udma_chClose(chHandle);
        if(UDMA_SOK != retVal)
        {
            App_print("\n [Error] UDMA channel close failed!!\n");
        }

        if(appChObj->transferDoneSem != NULL)
        {
            SemaphoreP_delete(appChObj->transferDoneSem);
            appChObj->transferDoneSem = NULL;
        }
    }

    return (retVal);
}

static void App_udmaTrpdInit(App_UdmaChObj  *appChObj, App_UdmaTestObj *appTestObj)
{    
    CSL_UdmapCppi5TRPD *pTrpd = (CSL_UdmapCppi5TRPD *) appChObj->trpdMem;
    CSL_UdmapTR15 *pTr = (CSL_UdmapTR15 *)(appChObj->trpdMem + sizeof(CSL_UdmapTR15));
    uint32_t *pTrResp = (uint32_t *) (appChObj->trpdMem + (sizeof(CSL_UdmapTR15) * 2U));
    uint32_t cqRingNum = Udma_chGetCqRingNum(appChObj->chHandle);

    /* Make TRPD */
    UdmaUtils_makeTrpd(pTrpd, UDMA_TR_TYPE_15, 1U, cqRingNum);

    /* Setup TR */
    pTr->flags  = CSL_FMK(UDMAP_TR_FLAGS_TYPE, CSL_UDMAP_TR_FLAGS_TYPE_4D_BLOCK_MOVE_REPACKING_INDIRECTION);
    pTr->flags |= CSL_FMK(UDMAP_TR_FLAGS_STATIC, 0U);
    pTr->flags |= CSL_FMK(UDMAP_TR_FLAGS_EOL, appChObj->eolType);
    pTr->flags |= CSL_FMK(UDMAP_TR_FLAGS_EVENT_SIZE, appChObj->eventSize);
    pTr->flags |= CSL_FMK(UDMAP_TR_FLAGS_TRIGGER0, appChObj->trigger);
    pTr->flags |= CSL_FMK(UDMAP_TR_FLAGS_TRIGGER1, CSL_UDMAP_TR_FLAGS_TRIGGER_NONE);
    pTr->flags |= CSL_FMK(UDMAP_TR_FLAGS_TRIGGER0_TYPE, appChObj->triggerType);
    pTr->flags |= CSL_FMK(UDMAP_TR_FLAGS_TRIGGER1_TYPE, CSL_UDMAP_TR_FLAGS_TRIGGER_TYPE_ALL);
    pTr->flags |= CSL_FMK(UDMAP_TR_FLAGS_CMD_ID, 0x25U);
    pTr->flags |= CSL_FMK(UDMAP_TR_FLAGS_SA_INDIRECT, 0U);
    pTr->flags |= CSL_FMK(UDMAP_TR_FLAGS_DA_INDIRECT, 0U);
    pTr->flags |= CSL_FMK(UDMAP_TR_FLAGS_EOP, 1U);
    
    pTr->icnt0    = appTestObj->numBytes;
    pTr->icnt1    = UDMA_TEST_XFER_CNT;
    pTr->icnt2    = UDMA_TEST_XFER_REPEAT_CNT_0;
    pTr->icnt3    = UDMA_TEST_XFER_REPEAT_CNT_1;
    pTr->addr     = (uint64_t) Udma_appVirtToPhyFxn(appChObj->srcBuf, 0U, NULL);
    pTr->fmtflags = 0x00000000U;        /* Linear addressing, 1 byte per elem.
                                           Replace with CSL-FL API */

    pTr->dicnt0   = appTestObj->numBytes;
    pTr->dicnt1   = UDMA_TEST_XFER_CNT;
    pTr->dicnt2   = UDMA_TEST_XFER_REPEAT_CNT_0;
    pTr->dicnt3   = UDMA_TEST_XFER_REPEAT_CNT_1;
    pTr->daddr    = (uint64_t) Udma_appVirtToPhyFxn(appChObj->destBuf, 0U, NULL);

    pTr->dim1     = pTr->icnt0; /* Use 0U when icnt1 is also used for repeating transfer starting from origin */
    pTr->dim2     = 0U; /* Use (pTr->icnt0 * pTr->icnt1) when icnt2 is also used for transferring continous data */
    pTr->dim3     = 0U; /* Use (pTr->icnt0 * pTr->icnt1 * pTr->icnt2) when icnt3 is also used for transferring continous data */
    pTr->ddim1    = pTr->dicnt0; /* Use 0U when dicnt1 is also used for repeating transfer starting from origin */
    pTr->ddim2    = 0U; /* Use (pTr->dicnt0 * pTr->dicnt1) when dicnt2 is also used for transferring continous data */
    pTr->ddim3    = 0U; /* Use (pTr->dicnt0 * pTr->dicnt1 * pTr->dicnt2) when dicnt3 is also used for continous data transfer */

    /* Clear TR response memory */
    *pTrResp = 0xFFFFFFFFU;

    /* Writeback cache */
    Udma_appUtilsCacheWb(appChObj->trpdMem, UDMA_TEST_APP_TRPD_SIZE_ALIGN);

    return;
}

static void App_printPerfResults(App_UdmaObj *appObj)
{    
    App_UdmaChObj  *appChObj;
    App_UdmaTestObj *appTestObj = &appObj->appTestObj;
    uint32_t        tCnt;
    uint32_t        triggerCnt = UDMA_TEST_XFER_CNT * UDMA_TEST_XFER_REPEAT_CNT_0 * UDMA_TEST_XFER_REPEAT_CNT_1;;

    appChObj = &appObj->appChObj[0U]; /* TX Channel */
    for(tCnt = 0U; tCnt < triggerCnt; tCnt++)
    {
        appChObj->totalTicks[tCnt] = appChObj->stopTicks[tCnt] - appChObj->startTicks[tCnt]  - getTicksDelay;
        appChObj->totalTicks[triggerCnt] += appChObj->totalTicks[tCnt];
        appChObj->elapsedTime[tCnt] = (appChObj->totalTicks[tCnt]*1000000000U)/(uint64_t)OSPI_FLASH_GTC_CLK_FREQ;

        App_printNum("\n OSPI Write %d", appTestObj->numBytes);
        App_printNum(" bytes in %d", (uint32_t)appChObj->elapsedTime[tCnt]);
        App_print("ns.");
    }

    appChObj = &appObj->appChObj[1U]; /* RX Channel */
    for(tCnt = 0U; tCnt < triggerCnt; tCnt++)
    {
        appChObj->totalTicks[tCnt] = appChObj->stopTicks[tCnt] - appChObj->startTicks[tCnt]  - getTicksDelay;
        appChObj->totalTicks[triggerCnt] += appChObj->totalTicks[tCnt];
        appChObj->elapsedTime[tCnt] = (appChObj->totalTicks[tCnt]*1000000000U)/(uint64_t)OSPI_FLASH_GTC_CLK_FREQ;

        App_printNum("\n OSPI Read %d", appTestObj->numBytes);
        App_printNum(" bytes in %d", (uint32_t)appChObj->elapsedTime[tCnt]);
        App_print("ns.");
    }

    /* Print average time */
    appChObj = &appObj->appChObj[0U]; /* TX Channel */
    appChObj->totalTicks[triggerCnt] = appChObj->totalTicks[triggerCnt]/triggerCnt;
    appChObj->elapsedTime[triggerCnt] = (appChObj->totalTicks[triggerCnt]*1000000000U)/(uint64_t)OSPI_FLASH_GTC_CLK_FREQ;
    App_printNum("\n\n Average time for OSPI Write %d", appTestObj->numBytes);
    App_printNum(" bytes = %d", (uint32_t)appChObj->elapsedTime[triggerCnt]);
    App_print("ns.");

    appChObj = &appObj->appChObj[1U]; /* RX Channel */
    appChObj->totalTicks[triggerCnt] = appChObj->totalTicks[triggerCnt]/triggerCnt;
    appChObj->elapsedTime[triggerCnt] = (appChObj->totalTicks[triggerCnt]*1000000000U)/(uint64_t)OSPI_FLASH_GTC_CLK_FREQ;
    App_printNum("\n\n Average time for OSPI Read %d", appTestObj->numBytes);
    App_printNum(" bytes = %d", (uint32_t)appChObj->elapsedTime[triggerCnt]);
    App_print("ns. \n\n");

    return;
}

void App_print(const char *str)
{
    UART_printf("%s", str);
    if(TRUE == Udma_appIsPrintSupported())
    {
        printf("%s", str);
    }

    return;
}

static void App_printNum(const char *str, uint32_t num)
{
    static char printBuf[200U];

    snprintf(printBuf, 200U, str, num);
    UART_printf("%s", printBuf);

    if(TRUE == Udma_appIsPrintSupported())
    {
        printf("%s", printBuf);
    }

    return;
}

int32_t App_setGTCClk(uint32_t moduleId,
                      uint32_t clkId,
                      uint64_t clkRateHz)
{
    int32_t retVal;
    uint64_t currClkFreqHz;

    retVal = PMLIBClkRateGet(moduleId, clkId, &currClkFreqHz);
    if ((retVal == CSL_PASS) &&
        (currClkFreqHz != clkRateHz))
    {
        retVal = PMLIBClkRateSet(moduleId, clkId, clkRateHz);
    }

    /* Enable GTC */
    HW_WR_REG32(CSL_GTC0_GTC_CFG1_BASE + 0x0U, 0x1);

    /* Measure and store the time spent to do a getTime operation */
    getTicksDelay = App_getGTCTimerTicks();
    getTicksDelay = App_getGTCTimerTicks() - getTicksDelay;
    OSPI_FLASH_log("\n Time taken to read GTC Timer ticks = %d ns (%d ticks) ",
                   (uint32_t)((getTicksDelay*1000000000U)/(uint64_t)OSPI_FLASH_GTC_CLK_FREQ), 
                   (uint32_t)getTicksDelay);

    return (retVal);
}

static int32_t App_ospiFlashInit(uint32_t clk)
{
    int32_t status = UDMA_SOK;

    status += OspiFlash_ospiConfigClk(clk);

    status += OspiFlash_ospiOpen(OSPI_FLASH_WRITE_TIMEOUT, OSPI_FLASH_CHECK_IDLE_DELAY, FALSE);

    status += OspiFlash_ospiEnableDDR(FALSE);

    status += OspiFlash_ospiSetOpcode();

    status += OspiFlash_ospiConfigPHY(clk, FALSE);

    return (status);    
}

static int32_t App_ospiFlashStart(uint32_t numBytes)
{
    int32_t retVal = UDMA_SOK;
    const CSL_ospi_flash_cfgRegs *baseAddr = (const CSL_ospi_flash_cfgRegs *)(OSPI_FLASH_CONFIG_REG_BASE_ADDR);

    retVal = OspiFlash_ospiEraseBlk(numBytes, FALSE);

    OspiFlash_ospiXferIntrInit(FALSE);

    /* Enable PHY pipeline mode */
    CSL_ospiPipelinePhyEnable(baseAddr, TRUE);

    return (retVal);
}
