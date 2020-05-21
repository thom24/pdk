/**
 *  \file   SPI_soc.c
 *
 *  \brief  DRA72x device specific hardware attributes.
 *
 */

/*
 * Copyright (C) 2016 - 2020 Texas Instruments Incorporated - http://www.ti.com/
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

#include <ti/csl/soc.h>
#include <ti/drv/spi/soc/SPI_soc.h>

#define CSL_QSPI_PER_CNT    1U

/* SPI configuration structure */
QSPI_HwAttrs qspiInitCfg[CSL_QSPI_PER_CNT] =
{
    {
        CSL_MSS_QSPI_U_BASE,      /* register baseAddr */
        CSL_EXT_FLASH_U_BASE,     /* memMappedBaseAddr */
        QSPI_MODULE_CLK_80M,      /* Input frequency   */
        CSL_MSS_INTR_MSS_QSPI_INT,/* Interrupt number  */
        CSL_MSS_INTR_MSS_QSPI_INT,/* Event id          */
        QSPI_OPER_MODE_MMAP,         /* operMode   */
        0U,                          /* chipSelect */
        QSPI_CS_POL_ACTIVE_LOW,      /* csPol      */
        QSPI_CLK_MODE_0,             /* frmFmt     */
        QSPI_DATA_DELAY_0,           /* dataDelay  */
        QSPI_RX_LINES_QUAD,          /* rxLines    */
        true,                        /* intrEnable */
        false,                       /* dmaEnable  */
        NULL,                        /* edmaHandle */
        0,                           /* edmaTcc    */
        EDMA_MSS_TPCC_A_EVT_FREE_0,  /* edmaChId   */
        4U                           /* clkDiv     */
    }
};


/* QSPI objects */
QSPI_v1_Object QspiObjects[CSL_QSPI_PER_CNT];


/* SPI configuration structure */
CSL_PUBLIC_CONST SPI_config_list SPI_config = {
    {
        &QSPI_FxnTable_v1,
        &QspiObjects[0],
        &qspiInitCfg[0]
    },
    /* "pad to full predefined length of array" */
    {NULL, NULL, NULL},
    {NULL, NULL, NULL},
    {NULL, NULL, NULL},
    {NULL, NULL, NULL},
    {NULL, NULL, NULL},
    {NULL, NULL, NULL},
    {NULL, NULL, NULL}
};

/**
 * \brief  This API gets the SoC level of QSPI intial configuration
 *
 * \param  index     QSPI instance index.
 * \param  cfg       Pointer to QSPI SOC initial config.
 *
 * \return 0 success: -1: error
 *
 */
int32_t QSPI_socGetInitCfg(uint32_t index, QSPI_HwAttrs *cfg)
{
    int32_t ret = 0;

    if (index < CSL_QSPI_PER_CNT)
    {
        *cfg = qspiInitCfg[index];
    }
    else
    {
        ret = -1;
    }

    return ret;
}

/**
 * \brief  This API sets the SoC level of QSPI intial configuration
 *
 * \param  index     QSPI instance index.
 * \param  cfg       Pointer to QSPI SOC initial config.
 *
 * \return           0 success: -1: error
 *
 */
int32_t QSPI_socSetInitCfg(uint32_t index, const QSPI_HwAttrs *cfg)
{
    int32_t ret = 0;

    if (index < CSL_QSPI_PER_CNT)
    {
        qspiInitCfg[index] = *cfg;
    }
    else
    {
        ret = -1;
    }

    return ret;
}
