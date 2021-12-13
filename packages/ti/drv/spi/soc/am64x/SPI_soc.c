/**
 *  \file   SPI_soc.c
 *
 *  \brief  AM64X device specific hardware attributes.
 *
 */

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

#include <ti/csl/soc/am64x/src/cslr_soc.h>
#include <ti/csl/soc/am64x/src/csl_psilcfg_thread_map.h>
#include <ti/drv/spi/soc/SPI_soc.h>
#include <ti/csl/arch/csl_arch.h>
#include <ti/drv/sciclient/sciclient.h>

#define MCSPI_PER_CNT   (7U)
#define OSPI_PER_CNT    (1U)
#define SPI_PER_CNT     (MCSPI_PER_CNT + OSPI_PER_CNT)

#if defined (BUILD_M4F)
#define MCSPI_RAT_MAP_ADDR   (0x60000000U) /* RAT map address for MCSPI */
#endif

/* SPI configuration structure */
SPI_v1_HWAttrs spiInitCfg[MCSPI_PER_CNT] =
{
    {
#if defined (BUILD_M4F)
        /* McSPI0 on the MCU channel */
        CSL_MCU_MCSPI0_CFG_BASE + MCSPI_RAT_MAP_ADDR, /* baseAddr */
#else
        /* McSPI0 on the Main domain */
        CSL_MCSPI0_CFG_BASE,                /* baseAddr */
#endif
#if defined (BUILD_MPU)
        /* A53 cores on the Main domain */
        CSLR_GICSS0_SPI_MCSPI0_INTR_SPI_0,  /* intNum */
#elif defined (BUILD_MCU)
        /* R5 cores on the Main domain */
        CSLR_R5FSS0_CORE0_INTR_MCSPI0_INTR_SPI_0,
#else
        /* M4 core on the MCU Channel */
        CSLR_MCU_M4FSS0_CORE0_NVIC_MCU_MCSPI0_INTR_SPI_0,
#endif
        0,                                  /* eventId */
        SPI_PINMODE_4_PIN,                  /* pinMode */
        MCSPI_CHANNEL_0,                    /* chNum */
        MCSPI_SINGLE_CH,                    /* chMode */
        true,                               /* enableIntr */
        48000000,                           /* inputClkFreq */
        MCSPI_INITDLY_0,                    /* initDelay */
        MCSPI_RX_TX_FIFO_SIZE,              /* rxTrigLvl */
        MCSPI_RX_TX_FIFO_SIZE,              /* txTrigLvl */
        {
            {
                MCSPI_CS_POL_LOW,                  /* csPolarity */
                MCSPI_DATA_LINE_COMM_MODE_7,       /* dataLineCommMode */
                MCSPI_CH0CONF_TCS0_ZEROCYCLEDLY,   /* tcs */
                MCSPI_TX_RX_MODE,                  /* trMode */
            },
            {
                MCSPI_CS_POL_LOW,
                MCSPI_DATA_LINE_COMM_MODE_7,
                MCSPI_CH0CONF_TCS0_ZEROCYCLEDLY,
                MCSPI_TX_RX_MODE,
            },
            {
                MCSPI_CS_POL_LOW,
                MCSPI_DATA_LINE_COMM_MODE_7,
                MCSPI_CH0CONF_TCS0_ZEROCYCLEDLY,
                MCSPI_TX_RX_MODE,
            },
            {
                MCSPI_CS_POL_LOW,
                MCSPI_DATA_LINE_COMM_MODE_7,
                MCSPI_CH0CONF_TCS0_ZEROCYCLEDLY,
                MCSPI_TX_RX_MODE,
            },
        },
        CSL_PDMA_CH_MAIN0_MCSPI0_CH0_RX,    /* rxDmaEventNumber */
        CSL_PDMA_CH_MAIN0_MCSPI0_CH0_TX,    /* txDmaEventNumber */
        0,                                  /* edmaTxTCC */
        0,                                  /* edmaRxTCC */
        0,                                  /* edmaTxTC */
        0,                                  /* edmaRxTC */
        NULL,                               /* edmaHandle */
        false,                              /* dmaMode */
        NULL                                /* dmaInfo */
    },
    {
#if defined (BUILD_M4F)
        /* McSPI1 on the MCU channel */
        CSL_MCU_MCSPI1_CFG_BASE + MCSPI_RAT_MAP_ADDR,
#else
        /* McSPI1 on the Main domain */
        CSL_MCSPI1_CFG_BASE,
#endif
#if defined (BUILD_MPU)
        CSLR_GICSS0_SPI_MCSPI1_INTR_SPI_0,
#elif defined (BUILD_MCU)
        CSLR_R5FSS0_CORE0_INTR_MCSPI1_INTR_SPI_0,
#else
        CSLR_MCU_M4FSS0_CORE0_NVIC_MCU_MCSPI1_INTR_SPI_0,
#endif
        0,
        SPI_PINMODE_4_PIN,
        MCSPI_CHANNEL_0,
        MCSPI_SINGLE_CH,
        true,
        48000000,
        MCSPI_INITDLY_0,
        MCSPI_RX_TX_FIFO_SIZE,
        MCSPI_RX_TX_FIFO_SIZE,
        {
            {
                MCSPI_CS_POL_LOW,
                MCSPI_DATA_LINE_COMM_MODE_7,
                MCSPI_CH0CONF_TCS0_ZEROCYCLEDLY,
                MCSPI_TX_RX_MODE,
            },
            {
                MCSPI_CS_POL_LOW,
                MCSPI_DATA_LINE_COMM_MODE_7,
                MCSPI_CH0CONF_TCS0_ZEROCYCLEDLY,
                MCSPI_TX_RX_MODE,
            },
            {
                MCSPI_CS_POL_LOW,
                MCSPI_DATA_LINE_COMM_MODE_7,
                MCSPI_CH0CONF_TCS0_ZEROCYCLEDLY,
                MCSPI_TX_RX_MODE,
            },
            {
                MCSPI_CS_POL_LOW,
                MCSPI_DATA_LINE_COMM_MODE_7,
                MCSPI_CH0CONF_TCS0_ZEROCYCLEDLY,
                MCSPI_TX_RX_MODE,
            },
        },
        CSL_PDMA_CH_MAIN0_MCSPI1_CH0_RX,
        CSL_PDMA_CH_MAIN0_MCSPI1_CH0_TX,
        0,
        0,
        0,
        0,
        NULL,
        false,
        NULL
    },
    {
        /* McSPI2 on the Main domain */
        CSL_MCSPI2_CFG_BASE,
#if defined (BUILD_MPU)
        CSLR_GICSS0_SPI_MCSPI2_INTR_SPI_0,
#elif defined (BUILD_MCU)
        CSLR_R5FSS0_CORE0_INTR_MCSPI2_INTR_SPI_0,
#else
        0U,
#endif
        0,
        SPI_PINMODE_4_PIN,
        MCSPI_CHANNEL_0,
        MCSPI_SINGLE_CH,
        true,
        48000000,
        MCSPI_INITDLY_0,
        MCSPI_RX_TX_FIFO_SIZE,
        MCSPI_RX_TX_FIFO_SIZE,
        {
            {
                MCSPI_CS_POL_LOW,
                MCSPI_DATA_LINE_COMM_MODE_7,
                MCSPI_CH0CONF_TCS0_ZEROCYCLEDLY,
                MCSPI_TX_RX_MODE,
            },
            {
                MCSPI_CS_POL_LOW,
                MCSPI_DATA_LINE_COMM_MODE_7,
                MCSPI_CH0CONF_TCS0_ZEROCYCLEDLY,
                MCSPI_TX_RX_MODE,
            },
            {
                MCSPI_CS_POL_LOW,
                MCSPI_DATA_LINE_COMM_MODE_7,
                MCSPI_CH0CONF_TCS0_ZEROCYCLEDLY,
                MCSPI_TX_RX_MODE,
            },
            {
                MCSPI_CS_POL_LOW,
                MCSPI_DATA_LINE_COMM_MODE_7,
                MCSPI_CH0CONF_TCS0_ZEROCYCLEDLY,
                MCSPI_TX_RX_MODE,
            },
        },
        CSL_PDMA_CH_MAIN0_MCSPI2_CH0_RX,
        CSL_PDMA_CH_MAIN0_MCSPI2_CH0_TX,
        0,
        0,
        0,
        0,
        NULL,
        false,
        NULL
    },
    {
        /* McSPI3 on the Main domain */
        CSL_MCSPI3_CFG_BASE,
#if defined (BUILD_MPU)
        CSLR_GICSS0_SPI_MCSPI3_INTR_SPI_0,
#elif defined (BUILD_MCU)
        CSLR_R5FSS0_CORE0_INTR_MCSPI3_INTR_SPI_0,
#else
        0U,
#endif
        0,
        SPI_PINMODE_4_PIN,
        MCSPI_CHANNEL_0,
        MCSPI_SINGLE_CH,
        true,
        48000000,
        MCSPI_INITDLY_0,
        MCSPI_RX_TX_FIFO_SIZE,
        MCSPI_RX_TX_FIFO_SIZE,
        {
            {
                MCSPI_CS_POL_LOW,
                MCSPI_DATA_LINE_COMM_MODE_7,
                MCSPI_CH0CONF_TCS0_ZEROCYCLEDLY,
                MCSPI_TX_RX_MODE,
            },
            {
                MCSPI_CS_POL_LOW,
                MCSPI_DATA_LINE_COMM_MODE_7,
                MCSPI_CH0CONF_TCS0_ZEROCYCLEDLY,
                MCSPI_TX_RX_MODE,
            },
            {
                MCSPI_CS_POL_LOW,
                MCSPI_DATA_LINE_COMM_MODE_7,
                MCSPI_CH0CONF_TCS0_ZEROCYCLEDLY,
                MCSPI_TX_RX_MODE,
            },
            {
                MCSPI_CS_POL_LOW,
                MCSPI_DATA_LINE_COMM_MODE_7,
                MCSPI_CH0CONF_TCS0_ZEROCYCLEDLY,
                MCSPI_TX_RX_MODE,
            },
        },
        CSL_PDMA_CH_MAIN0_MCSPI3_CH0_RX,
        CSL_PDMA_CH_MAIN0_MCSPI3_CH0_TX,
        0,
        0,
        0,
        0,
        NULL,
        false,
        NULL
    },
    {
        /* McSPI4 on the Main domain */
        CSL_MCSPI4_CFG_BASE,
#if defined (BUILD_MPU)
        CSLR_GICSS0_SPI_MCSPI4_INTR_SPI_0,
#elif defined (BUILD_MCU)
        CSLR_R5FSS0_CORE0_INTR_MCSPI4_INTR_SPI_0,
#else
        0U,
#endif
        0,
        SPI_PINMODE_4_PIN,
        MCSPI_CHANNEL_0,
        MCSPI_SINGLE_CH,
        true,
        48000000,
        MCSPI_INITDLY_0,
        MCSPI_RX_TX_FIFO_SIZE,
        MCSPI_RX_TX_FIFO_SIZE,
        {
            {
                MCSPI_CS_POL_LOW,
                MCSPI_DATA_LINE_COMM_MODE_7,
                MCSPI_CH0CONF_TCS0_ZEROCYCLEDLY,
                MCSPI_TX_RX_MODE,
            },
            {
                MCSPI_CS_POL_LOW,
                MCSPI_DATA_LINE_COMM_MODE_7,
                MCSPI_CH0CONF_TCS0_ZEROCYCLEDLY,
                MCSPI_TX_RX_MODE,
            },
            {
                MCSPI_CS_POL_LOW,
                MCSPI_DATA_LINE_COMM_MODE_7,
                MCSPI_CH0CONF_TCS0_ZEROCYCLEDLY,
                MCSPI_TX_RX_MODE,
            },
            {
                MCSPI_CS_POL_LOW,
                MCSPI_DATA_LINE_COMM_MODE_7,
                MCSPI_CH0CONF_TCS0_ZEROCYCLEDLY,
                MCSPI_TX_RX_MODE,
            },
        },
        CSL_PDMA_CH_MAIN1_MCSPI4_CH0_RX,
        CSL_PDMA_CH_MAIN1_MCSPI4_CH0_TX,
        0,
        0,
        0,
        0,
        NULL,
        false,
        NULL
    },
    {
        /* McSPI0 on the MCU Channel */
        CSL_MCU_MCSPI0_CFG_BASE,
#if defined (BUILD_MPU)
        CSLR_GICSS0_SPI_MCU_MCSPI0_INTR_SPI_0,
#elif defined (BUILD_MCU)
        CSLR_R5FSS0_CORE0_INTR_MCU_MCSPI0_INTR_SPI_0,
#else
        CSLR_MCU_M4FSS0_CORE0_NVIC_MCU_MCSPI0_INTR_SPI_0,
#endif
        0,
        SPI_PINMODE_4_PIN,
        MCSPI_CHANNEL_0,
        MCSPI_SINGLE_CH,
        true,
        48000000,
        MCSPI_INITDLY_0,
        MCSPI_RX_TX_FIFO_SIZE,
        MCSPI_RX_TX_FIFO_SIZE,
        {
            {
                MCSPI_CS_POL_LOW,
                MCSPI_DATA_LINE_COMM_MODE_7,
                MCSPI_CH0CONF_TCS0_ZEROCYCLEDLY,
                MCSPI_TX_RX_MODE,
            },
            {
                MCSPI_CS_POL_LOW,
                MCSPI_DATA_LINE_COMM_MODE_7,
                MCSPI_CH0CONF_TCS0_ZEROCYCLEDLY,
                MCSPI_TX_RX_MODE,
            },
            {
                MCSPI_CS_POL_LOW,
                MCSPI_DATA_LINE_COMM_MODE_7,
                MCSPI_CH0CONF_TCS0_ZEROCYCLEDLY,
                MCSPI_TX_RX_MODE,
            },
            {
                MCSPI_CS_POL_LOW,
                MCSPI_DATA_LINE_COMM_MODE_7,
                MCSPI_CH0CONF_TCS0_ZEROCYCLEDLY,
                MCSPI_TX_RX_MODE,
            },
        },
        0,   /* PDMA not supported for peripherals on MCU channel */
        0,
        0,
        0,
        0,
        0,
        NULL,
        false,
        NULL
    },
    {
        /* McSPI1 on the MCU Channel */
        CSL_MCU_MCSPI1_CFG_BASE,
#if defined (BUILD_MPU)
        CSLR_GICSS0_SPI_MCU_MCSPI1_INTR_SPI_0,
#elif defined (BUILD_MCU)
        CSLR_R5FSS0_CORE0_INTR_MCU_MCSPI1_INTR_SPI_0,
#else
        CSLR_MCU_M4FSS0_CORE0_NVIC_MCU_MCSPI1_INTR_SPI_0,
#endif
        0,
        SPI_PINMODE_4_PIN,
        MCSPI_CHANNEL_0,
        MCSPI_SINGLE_CH,
        true,
        48000000,
        MCSPI_INITDLY_0,
        MCSPI_RX_TX_FIFO_SIZE,
        MCSPI_RX_TX_FIFO_SIZE,
        {
            {
                MCSPI_CS_POL_LOW,
                MCSPI_DATA_LINE_COMM_MODE_7,
                MCSPI_CH0CONF_TCS0_ZEROCYCLEDLY,
                MCSPI_TX_RX_MODE,
            },
            {
                MCSPI_CS_POL_LOW,
                MCSPI_DATA_LINE_COMM_MODE_7,
                MCSPI_CH0CONF_TCS0_ZEROCYCLEDLY,
                MCSPI_TX_RX_MODE,
            },
            {
                MCSPI_CS_POL_LOW,
                MCSPI_DATA_LINE_COMM_MODE_7,
                MCSPI_CH0CONF_TCS0_ZEROCYCLEDLY,
                MCSPI_TX_RX_MODE,
            },
            {
                MCSPI_CS_POL_LOW,
                MCSPI_DATA_LINE_COMM_MODE_7,
                MCSPI_CH0CONF_TCS0_ZEROCYCLEDLY,
                MCSPI_TX_RX_MODE,
            },
        },
        0,
        0,
        0,
        0,
        0,
        0,
        NULL,
        false,
        NULL
    }
};


/* SPI objects */
SPI_v1_Object SpiObjects[MCSPI_PER_CNT];

/* OSPI configuration structure */
OSPI_v0_HwAttrs ospiInitCfg =
{
    /* OSPI0 on the Main domain */
    0,                                  /* Instance Number */
    CSL_FSS0_OSPI0_CTRL_BASE,          /* baseAddr, flash config register baseAddr */
#if defined (BUILD_MPU)
    /* A53 cores on the Main domain */
    CSL_FSS0_DAT_REG0_BASE,            /* dataAddr, OSPI data base address */
#elif defined (BUILD_MCU)
    /* R5 cores on the Main domain */
    CSL_FSS0_DAT_REG1_BASE,
#else
    /* M4 core on the MCU channel */
    CSL_FSS0_DAT_REG1_BASE,
#endif
    OSPI_MODULE_CLOCK,                 /* funcClk, input frequency */
#if defined (BUILD_MPU)
    CSLR_GICSS0_SPI_FSS0_OSPI_0_OSPI_LVL_INTR_0,  /* intrNum, interrupt number */
#elif defined (BUILD_MCU)
    CSLR_R5FSS0_CORE0_INTR_FSS0_OSPI_0_OSPI_LVL_INTR_0,
#else
    0U,  /* TBD, no OSPI0 interrupt routed to M4 core */
#endif
    0,                                 /* Event ID not used for ARM INTC */
    OSPI_OPER_MODE_CFG,                /* operMode */
    CSL_OSPI_CS0,                      /* chipSelect */
    CSL_OSPI_CLK_MODE_0,               /* frmFmt */
    {
        0,                             /* default Chip Select Start of Transfer Delay */
        0,                             /* default Chip Select End of Transfer Delay */
        0,                             /* default Chip Select De-Assert Different Slaves Delay */
        OSPI_DEV_DELAY_CSDA            /* default Chip Select De-Assert Delay */
    },
    256,                               /* device page size is 256 bytes  */
    18,                                /* device block size is 2 ^ 18 = 256K bytes */
    OSPI_XFER_LINES_OCTAL,             /* xferLines */
    (bool)false,                       /* Interrupt mode */
    (bool)true,                        /* Direct Access Controller mode */
    (bool)false,                       /* DMA mode */
    NULL,                              /* dmaInfo */
    (bool)true,                        /* enable PHY */
    0,                                 /* rdDataCapDelay */
    (bool)true,                        /* enable DDR */
    (bool)false,                       /* enable XIP */
    10U,                               /* Chip Select Start Of Transfer delay */
    0,                                 /* Baudrate divider. Set to a non-zero value (2 to 32)
                                          to override default divider settings */
    (bool)false,                       /* enable Cache */
};

/* OSPI objects */
OSPI_v0_Object OspiObjects;

/* SPI configuration structure */
CSL_PUBLIC_CONST SPI_config_list SPI_config = {
    {
        &SPI_FxnTable_v1,
        &SpiObjects[0],
        &spiInitCfg[0]
    },
    {
        &SPI_FxnTable_v1,
        &SpiObjects[1],
        &spiInitCfg[1]
    },
    {
        &SPI_FxnTable_v1,
        &SpiObjects[2],
        &spiInitCfg[2]
    },
    {
        &SPI_FxnTable_v1,
        &SpiObjects[3],
        &spiInitCfg[3]
    },
    {
        &SPI_FxnTable_v1,
        &SpiObjects[4],
        &spiInitCfg[4]
    },
    {
        &SPI_FxnTable_v1,
        &SpiObjects[5],
        &spiInitCfg[5]
    },
    {
        &SPI_FxnTable_v1,
        &SpiObjects[6],
        &spiInitCfg[6]
    },
    {
        &OSPI_FxnTable_v0,
        &OspiObjects,
        &ospiInitCfg
    }
};

/* MCSPI configuration structure */
MCSPI_config_list MCSPI_config = {
    {
        {
            &MCSPI_FxnTable_v1,
            (SPI_Handle)(&(SPI_config[0])),
            0
        },
        {
            &MCSPI_FxnTable_v1,
            (SPI_Handle)(&(SPI_config[0])),
            1
        },
        {
            &MCSPI_FxnTable_v1,
            (SPI_Handle)(&(SPI_config[0])),
            2
        },
        {
            &MCSPI_FxnTable_v1,
            (SPI_Handle)(&(SPI_config[0])),
            3
        },
    },
    {
        {
            &MCSPI_FxnTable_v1,
            (SPI_Handle)(&(SPI_config[1])),
            0
        },
        {
            &MCSPI_FxnTable_v1,
            (SPI_Handle)(&(SPI_config[1])),
            1
        },
        {
            &MCSPI_FxnTable_v1,
            (SPI_Handle)(&(SPI_config[1])),
            2
        },
        {
            &MCSPI_FxnTable_v1,
            (SPI_Handle)(&(SPI_config[1])),
            3
        },
    },
    {
        {
            &MCSPI_FxnTable_v1,
            (SPI_Handle)(&(SPI_config[2])),
            0
        },
        {
            &MCSPI_FxnTable_v1,
            (SPI_Handle)(&(SPI_config[2])),
            1
        },
        {
            &MCSPI_FxnTable_v1,
            (SPI_Handle)(&(SPI_config[2])),
            2
        },
        {
            &MCSPI_FxnTable_v1,
            (SPI_Handle)(&(SPI_config[2])),
            3
        },
    },
    {
        {
            &MCSPI_FxnTable_v1,
            (SPI_Handle)(&(SPI_config[3])),
            0
        },
        {
            &MCSPI_FxnTable_v1,
            (SPI_Handle)(&(SPI_config[3])),
            1
        },
        {
            &MCSPI_FxnTable_v1,
            (SPI_Handle)(&(SPI_config[3])),
            2
        },
        {
            &MCSPI_FxnTable_v1,
            (SPI_Handle)(&(SPI_config[3])),
            3
        },
    },
    {
        {
            &MCSPI_FxnTable_v1,
            (SPI_Handle)(&(SPI_config[4])),
            0
        },
        {
            &MCSPI_FxnTable_v1,
            (SPI_Handle)(&(SPI_config[4])),
            1
        },
        {
            &MCSPI_FxnTable_v1,
            (SPI_Handle)(&(SPI_config[4])),
            2
        },
        {
            &MCSPI_FxnTable_v1,
            (SPI_Handle)(&(SPI_config[4])),
            3
        },
    },
    {
        {
            &MCSPI_FxnTable_v1,
            (SPI_Handle)(&(SPI_config[5])),
            0
        },
        {
            &MCSPI_FxnTable_v1,
            (SPI_Handle)(&(SPI_config[5])),
            1
        },
        {
            &MCSPI_FxnTable_v1,
            (SPI_Handle)(&(SPI_config[5])),
            2
        },
        {
            &MCSPI_FxnTable_v1,
            (SPI_Handle)(&(SPI_config[5])),
            3
        },
    },
    {
        {
            &MCSPI_FxnTable_v1,
            (SPI_Handle)(&(SPI_config[6])),
            0
        },
        {
            &MCSPI_FxnTable_v1,
            (SPI_Handle)(&(SPI_config[6])),
            1
        },
        {
            &MCSPI_FxnTable_v1,
            (SPI_Handle)(&(SPI_config[6])),
            2
        },
        {
            &MCSPI_FxnTable_v1,
            (SPI_Handle)(&(SPI_config[6])),
            3
        },
    }
};

/**
 * \brief  This API gets the SoC level of SPI intial configuration
 *
 * \param  idx       SPI instance index.
 * \param  cfg       Pointer to SPI SOC initial config.
 *
 * \return 0 success: -1: error
 *
 */
int32_t SPI_socGetInitCfg(uint32_t idx, SPI_v1_HWAttrs *cfg)
{
    int32_t ret = 0;

    if (idx < MCSPI_PER_CNT)
    {
        *cfg = spiInitCfg[idx];
    }
    else
    {
        ret = (int32_t)(-1);
    }
    return ret;
}

/**
 * \brief  This API sets the SoC level of SPI intial configuration
 *
 * \param  idx       SPI instance index.
 * \param  cfg       Pointer to SPI SOC initial config.
 *
 * \return           0 success: -1: error
 *
 */
int32_t SPI_socSetInitCfg(uint32_t idx, const SPI_v1_HWAttrs *cfg)
{
    int32_t ret = 0;

    if (idx < MCSPI_PER_CNT)
    {
        spiInitCfg[idx] = *cfg;
    }
    else
    {
        ret = (int32_t)(-1);
    }
    return ret;
}

/**
 * \brief  This API gets the SoC level of OSPI intial configuration
 *
 * \param  idx       OSPI instance index.
 * \param  cfg       Pointer to OSPI SOC initial config.
 *
 * \return 0 success: -1: error
 *
 */
int32_t OSPI_socGetInitCfg(uint32_t idx, OSPI_v0_HwAttrs *cfg)
{
    int32_t ret = 0;

    if (idx < OSPI_PER_CNT)
    {
        *cfg = ospiInitCfg;
    }
    else
    {
        ret = -1;
    }

    return ret;
}

/**
 * \brief  This API sets the SoC level of OSPI intial configuration
 *
 * \param  idx       OSPI instance index.
 * \param  cfg       Pointer to OSPI SOC initial config.
 *
 * \return           0 success: -1: error
 *
 */
int32_t OSPI_socSetInitCfg(uint32_t idx, const OSPI_v0_HwAttrs *cfg)
{
    int32_t ret = 0;

    if (idx < OSPI_PER_CNT)
    {
        ospiInitCfg = *cfg;
    }
    else
    {
        ret = -1;
    }

    return ret;
}

/**
 * \brief  This API update the default SoC level of configurations
 *         based on the core and domain
 *
 *         ospiInitCfg table configures MCU domain's OSPI instances by
 *         default for R5, OSPI_socInit() is called to
 *         overwrite the defaut configurations with the configurations
 *         of Main domain's OSPI instances if R5 is on the Main domain
 *
 * \param  none
 *
 * \return           0 success: -1: error
 *
 */
int32_t OSPI_socInit(void)
{
    int32_t ret = 0;
#if defined (BUILD_MCU)
    CSL_ArmR5CPUInfo r5CpuInfo;

    CSL_armR5GetCpuID(&r5CpuInfo);

    if (r5CpuInfo.grpId == (uint32_t)CSL_ARM_R5_CLUSTER_GROUP_ID_0)
    {
        /* Configure the MCU SS OSPI instances for MCU SS Pulsar R5 . R5FSS'n'_CORE'n' */
        /*  ********* OSPI-0 . intrNum = R5FSS'n'_CORE'n' **************** */
        ospiInitCfg.intrNum = CSLR_R5FSS0_CORE0_INTR_FSS0_OSPI_0_OSPI_LVL_INTR_0;
    }
    else
    {
        ret = -1;
    }
#endif
    return (ret);
}

/**
 * \brief  This function will configure the interrupt path to the destination CPU
 *         using DMSC firmware via sciclient. if setIntrPath is set to TRUE,
 *         a path is set, else the interrupt path is released
 *
 * \param  hwAttrs_ptr Pointer to hardware attributes
 * \param  setIntrPath Set or release interrupt
 *
 * \return           0 success: -1: error
 *
 */
int32_t OSPI_configSocIntrPath(void *hwAttrs_ptr, bool setIntrPath)
{
    int32_t ret = 0;
    /* Routing of interrupts not required for this SOC */
    return(ret);
}

/**
 * \brief  This API update the default SoC level of configurations
 *         based on the core and domain
 *
 *         spiInitCfg table configures MCSPI instances by
 *         default for MCU R5 0, MCSPI_socInit() is called to
 *         overwrite the defaut configurations
 *
 * \param  none
 *
 * \return           0 success: -1: error
 *
 */
int32_t MCSPI_socInit(void)
{
    int32_t ret = 0;
#if defined(BUILD_MCU)
    CSL_ArmR5CPUInfo r5CpuInfo;

    CSL_armR5GetCpuID(&r5CpuInfo);

    if (r5CpuInfo.grpId == (uint32_t)CSL_ARM_R5_CLUSTER_GROUP_ID_0)             /* MCU R5 */
    {
        /* No change required */
    }
    else if (r5CpuInfo.grpId == (uint32_t)CSL_ARM_R5_CLUSTER_GROUP_ID_1)         /* MAIN R5 SS0 */
    {
        if(r5CpuInfo.cpuID == 0U)                                           /* Main R5 -SS0 - CPU0 */
        {
            spiInitCfg[0].intNum = CSLR_R5FSS0_CORE0_INTR_MCU_MCSPI0_INTR_SPI_0;
            spiInitCfg[1].intNum = CSLR_R5FSS0_CORE0_INTR_MCU_MCSPI1_INTR_SPI_0;
            spiInitCfg[3].intNum = CSLR_R5FSS0_CORE0_INTR_MCSPI3_INTR_SPI_0;
            spiInitCfg[4].intNum = CSLR_R5FSS0_CORE0_INTR_MCSPI4_INTR_SPI_0;
        }
        else                                                                /* Main R5 -SS0 - CPU1*/
        {
            spiInitCfg[0].intNum = CSLR_R5FSS0_CORE1_INTR_MCU_MCSPI0_INTR_SPI_0;
            spiInitCfg[1].intNum = CSLR_R5FSS0_CORE1_INTR_MCU_MCSPI1_INTR_SPI_0;
            spiInitCfg[3].intNum = CSLR_R5FSS0_CORE1_INTR_MCSPI3_INTR_SPI_0;
            spiInitCfg[4].intNum = CSLR_R5FSS0_CORE1_INTR_MCSPI4_INTR_SPI_0;
        }
    }
    else if (r5CpuInfo.grpId == (uint32_t)CSL_ARM_R5_CLUSTER_GROUP_ID_2)         /* MAIN R5 SS1 */
    {
        if(r5CpuInfo.cpuID == 0U)                                           /* Main R5 -SS1 - CPU0 */
        {
            spiInitCfg[0].intNum = CSLR_R5FSS1_CORE0_INTR_MCU_MCSPI0_INTR_SPI_0;
            spiInitCfg[1].intNum = CSLR_R5FSS1_CORE0_INTR_MCU_MCSPI1_INTR_SPI_0;
            spiInitCfg[3].intNum = CSLR_R5FSS1_CORE0_INTR_MCSPI3_INTR_SPI_0;
            spiInitCfg[4].intNum = CSLR_R5FSS1_CORE0_INTR_MCSPI4_INTR_SPI_0;
        }
        else                                                                /* Main R5 -SS1 - CPU1*/
        {
            spiInitCfg[0].intNum = CSLR_R5FSS1_CORE1_INTR_MCU_MCSPI0_INTR_SPI_0;
            spiInitCfg[1].intNum = CSLR_R5FSS1_CORE1_INTR_MCU_MCSPI1_INTR_SPI_0;
            spiInitCfg[3].intNum = CSLR_R5FSS1_CORE1_INTR_MCSPI3_INTR_SPI_0;
            spiInitCfg[4].intNum = CSLR_R5FSS1_CORE1_INTR_MCSPI4_INTR_SPI_0;
        }
    }
    else
    {
        ret = -1;
    }
#endif
    return (ret);
}

/**
 * \brief  This function will configure the interrupt path to the destination CPU
 *         using DMSC firmware via sciclient. if setIntrPath is set to TRUE,
 *         a path is set, else the interrupt path is released
 *
 * \param  instance    MCSPI Instance
 * \param  hwAttrs_ptr Pointer to hardware attributes
 * \param  setIntrPath Set or release interrupt
 *
 * \return           0 success: -1: error
 *
 */
int32_t MCSPI_configSocIntrPath(uint32_t instance, void *hwAttrs_ptr, bool setIntrPath)
{
    int32_t ret = 0;
    /* Routing of interrupts not required for this SOC */
    return(ret);
}
