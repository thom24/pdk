/**
 *  \file   MIBSPI_drv.c
 *
 *  \brief  MIBSPI Driver high level APIs.
 *
 *   This file contains the driver APIs for MIBSPI.
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

#include <stdint.h>
#include <stdlib.h>

#include <ti/drv/spi/MIBSPI.h>
#include <ti/drv/spi/SPI.h>
#include <ti/drv/spi/src/SPI_osal.h>
#include <ti/drv/spi/src/v3/mibspi_priv.h>
#include <ti/drv/spi/src/v3/mibspi_utils.h>

/* Externs */

/* Default SPI parameters structure */
const MIBSPI_Params MIBSPI_defaultParams = {
    .spiParams    = 
    {
        .mode               = SPI_SLAVE,
        .dataSize           = 16U,
        .frameFormat        = SPI_POL0_PHA0,
        .transferMode       = SPI_MODE_BLOCKING,
        .transferTimeout    = SPI_WAIT_FOREVER,
        .transferCallbackFxn  = NULL,
        .custom               = (void *) &MIBSPI_defaultParams.mibspiParams,
    },
    .mibspiParams =
    {
        .pinMode            = MIBSPI_PINMODE_4PIN_CS,
        .shiftFormat        = MIBSPI_MSB_FIRST,
        .dmaEnable          = (uint8_t)1U,
        .eccEnable          = (uint8_t)1U,
        .csHold             = (uint8_t)0U,
        .txDummyValue       = (uint16_t)0xFFFFU,
    },
};

/* Default Master params */
const MIBSPI_MasterModeParams MIBSPI_defaultMasterParams =
{
    /* Master Params */
    .bitRate   = 5000000U,            /* bitRate */
    .numSlaves = 1,                   /* numSlaves */
    .t2cDelay  = 0,                   /* t2cDelay */
    .c2tDelay  = 0,                   /* c2tDelay */
    .wDelay    = 0,                   /* wDelay */
    .slaveProf = 
    {
        [0] = 
        /* Slave - 0 */
        {
            .chipSelect = (uint8_t)0U,                    /* CS0 */
            .ramBufLen  = (uint8_t)MIBSPI_RAM_MAX_ELEM,   /* ramBufLen in number of dataSize */
            .dmaReqLine = 0U,
        },
        [1] = 
        /* Slave - 1 - not valid */
        {
            (uint8_t)MIBSPI_CS_NONE,               /* CS_NONE */
            (uint8_t)0U,                    /* ramBufLen in number of dataSize */
            .dmaReqLine = 0U,
        },
        [2] = 
        /* Slave - 2 - not valid */
        {
            (uint8_t)MIBSPI_CS_NONE,        /* CS_NONE */
            (uint8_t)0U,                    /* ramBufLen in number of dataSize */
            .dmaReqLine = 0U,
        }
    }
};

/* Default Slave params */
const MIBSPI_SlaveModeParams MIBSPI_defaultSlaveParams =
{
    /* Slave Params */
    .dmaReqLine  = 0U, /*!< DMA request line to be used for Slave */
    .chipSelect  = 0U, /* CS0 */               /*!< CS0-CS7 signal number from 0 -7 */
};

/*
 *  ======== MIBSPI_close ========
 */
void MIBSPI_close(SPI_Handle handle)
{
    SPI_close(handle);
}

/*
 *  ======== MIBSPI_control ========
 */
int32_t MIBSPI_control(SPI_Handle handle, uint32_t cmd, void *arg)
{
    return SPI_control(handle, cmd, arg);
}


/*
 *  ======== MIBSPI_init ========
 */
void MIBSPI_init(void)
{
    SPI_init();
}

/*
 *  ======== MIBSPI_open ========
 */
SPI_Handle MIBSPI_open(enum MibSpi_InstanceId mibspiInstanceId, MIBSPI_Params *params)
{
    uint32_t idx;
    int32_t spiStatus;
    SPI_Handle hSpi;

    spiStatus = MIBSPI_socGetInstIndex(mibspiInstanceId, &idx);
    if (spiStatus == SPI_STATUS_SUCCESS)
    {
        params->spiParams.custom = &params->mibspiParams;
        hSpi = SPI_open(idx, &params->spiParams);
    }
    else
    {
        hSpi = NULL;
    }
    return hSpi;
}


/*
 *  ======== MIBSPI_Params_init ========
 */
void MIBSPI_Params_init(MIBSPI_Params *params)
{
    if (params != NULL)
    {
        *params = MIBSPI_defaultParams;
        if(params->spiParams.mode == SPI_MASTER)
        {
            /* Set Master Params */
            params->mibspiParams.u.masterParams = MIBSPI_defaultMasterParams;
        }
        else
        {
            /* Set Slave Params */
            params->mibspiParams.u.slaveParams = MIBSPI_defaultSlaveParams;
        }
    }
}

/*
 *  ======== MIBSPI_transfer ========
 */
bool MIBSPI_transfer(SPI_Handle handle, MIBSPI_Transaction *spiTrans)
{
    bool status;

    spiTrans->size = sizeof(MIBSPI_Transaction);
    spiTrans->base.custom = spiTrans;
    status = SPI_transfer(handle, &spiTrans->base);
    return status;
}

/*
 *  ======== MIBSPI_transferCancel ========
 */
void MIBSPI_transferCancel(SPI_Handle handle)
{
    SPI_transferCancel(handle);
}


/*
 *  ======== MIBSPI_getStats ========
 */
int32_t MIBSPI_getStats(SPI_Handle handle, MIBSPI_Stats *ptrStats)
{
    return (SPI_control(handle, MIBSPI_CMD_GET_STATS, ptrStats));

}


/*!
 * \brief Initialize utils module
 *
 * Utils module initialization function. Should be only called from the
 * CPSW top level module.
 *
 * \param pUtilsPrms   [IN] Pointer to the initialization parameters
 */
void MIBSPI_utilsInit(MibspiUtils_Prms *pUtilsPrms)
{
    MibspiUtils_init(pUtilsPrms);
}

/*!
 * \brief De-initialize utils module
 */
void MIBSPI_utilsDeInit(void)
{
    MibspiUtils_deInit();
}

