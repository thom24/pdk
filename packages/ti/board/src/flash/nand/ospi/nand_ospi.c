/*
 * Copyright (c) 2021, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include <ti/board/src/flash/nand/ospi/nand_ospi.h>
#include <ti/drv/spi/soc/SPI_soc.h>
#include <ti/csl/soc.h>

#if defined(j721s2_evm)
/* SPI entry offset is at index 5 of SPI config array */
#define SPI_CONFIG_OFFSET     (5U)
#endif

static NAND_HANDLE Nand_ospiOpen(uint32_t nandIntf, uint32_t portNum, void *params);
static void Nand_ospiClose(NAND_HANDLE handle);
static NAND_STATUS Nand_ospiRead(NAND_HANDLE handle, uint32_t addr,
                               uint32_t len, uint8_t *buf);
static NAND_STATUS Nand_ospiWrite(NAND_HANDLE handle, uint32_t addr,
                                uint32_t len, uint8_t *buf);
static NAND_STATUS Nand_ospiErase(NAND_HANDLE handle, int32_t eraseCnt);

static NAND_STATUS Nand_ospiCmdWrite(SPI_Handle handle, uint8_t *cmdBuf,
                                   uint32_t cmdLen, uint32_t dataLen);

static NAND_STATUS Nand_ospiWaitReady(SPI_Handle handle, uint32_t timeOut);

/* NAND function table for NAND OSPI interface implementation */
const NAND_FxnTable Nand_ospiFxnTable =
{
    &Nand_ospiOpen,
    &Nand_ospiClose,
    &Nand_ospiRead,
    &Nand_ospiWrite,
    &Nand_ospiErase,
};

NAND_Info Nand_ospiInfo =
{
    0,                          /* hwHandle */
    0,                          /* manufacturerId */
    0,                          /* deviceId */
    0,                          /* busWidth */
    NAND_NUM_BLOCKS,            /* blockCnt */
    NAND_NUM_PAGES_PER_BLOCK,   /* pageCnt */
    NAND_PAGE_SIZE,             /* pageSize */
    0,                          /* baseAddr */
    NAND_SECTOR_SIZE            /* sectorSize */
};

static bool gPhyEnable;
static bool gDtrEnable;

static NAND_STATUS NAND_ospiCmdRead(SPI_Handle handle, uint8_t *cmdBuf,
                            uint32_t cmdLen, uint8_t *rxBuf, uint32_t rxLen)
{
    SPI_Transaction transaction;
    uint32_t        transferType = SPI_TRANSACTION_TYPE_READ;
    bool            ret;

    /* Update the mode and transfer type with the required values */
    SPI_control(handle, SPI_V0_CMD_SET_CFG_MODE, NULL);
    SPI_control(handle, SPI_V0_CMD_XFER_MODE_RW, (void *)&transferType);
    SPI_control(handle, SPI_V0_CMD_EXT_RD_CMD_LEN, (void *)&cmdLen);

    transaction.txBuf = (void *)cmdBuf;
    transaction.rxBuf = (void *)rxBuf;
    transaction.count = cmdLen + rxLen;

    ret = SPI_transfer(handle, &transaction);
    if (ret == true)
    {
        return NAND_PASS;
    }
	else
    {
        return NAND_FAIL;
    }
}

static NAND_STATUS Nand_ospiReadId(SPI_Handle handle)
{
    NAND_STATUS     retVal;
    uint8_t         idCode[NAND_RDID_NUM_BYTES];
    uint8_t         cmd = NAND_CMD_RDID;
    uint32_t        manfID, devID;
    uint32_t        cmdDummyCycles;
    OSPI_v0_HwAttrs const *hwAttrs= (OSPI_v0_HwAttrs const *)handle->hwAttrs;

    if (hwAttrs->xferLines == OSPI_XFER_LINES_OCTAL)
    {
        cmdDummyCycles = 8;
        SPI_control(handle, SPI_V0_CMD_EXT_RD_DUMMY_CLKS, (void *)&cmdDummyCycles);
    }

    retVal = NAND_ospiCmdRead(handle, &cmd, 1, idCode, NAND_RDID_NUM_BYTES);

    if (hwAttrs->xferLines == OSPI_XFER_LINES_OCTAL)
    {
        cmdDummyCycles = 8;
        SPI_control(handle, SPI_V0_CMD_EXT_RD_DUMMY_CLKS, (void *)&cmdDummyCycles);
    }

    if (retVal == NAND_PASS)
    {
        manfID = (uint32_t)idCode[0];
        devID = ((uint32_t)idCode[1] << 8) | ((uint32_t)idCode[2]);
        if ((manfID == NAND_MANF_ID) && (devID == NAND_DEVICE_ID))
        {
            Nand_ospiInfo.manufacturerId = manfID;
            Nand_ospiInfo.deviceId = devID;
        }
        else
        {
            retVal = NAND_FAIL;
        }
    }

    return (retVal);
}

static NAND_STATUS Nand_ospiEnableOctalDDR(SPI_Handle handle)
{
    NAND_STATUS     retVal = NAND_PASS;
    uint8_t         cmdWren = NAND_CMD_WREN;
    uint32_t        data[3];

    /* Send Write Enable command */
    if(Nand_ospiCmdWrite(handle, &cmdWren, 1, 0))
    {
        retVal = NAND_FAIL;
    }

    /* Check BUSY bit of Flash */
    if (Nand_ospiWaitReady(handle, NAND_WRR_WRITE_TIMEOUT))
    {
        retVal = NAND_FAIL;
    }

    /* Enable double transfer rate mode */
    if (retVal == NAND_PASS)
    {
        /* send write VCR command to reg addr 0x0 to set to DDR mode */
        data[0] = (NAND_CMD_WRITE_VCR << 24)        | /* write volatile config reg cmd */
                  (0 << 23)                         | /* read data disable */
                  (7 << 20)                         | /* read 8 data bytes */
                  (1 << 19)                         | /* enable cmd adddr */
                  (2 << 16)                         | /* 3 address bytes */
                  (1 << 15);                          /* write data enable */
        data[1] = 0;     /* Non-volatile config register address */
        data[2] = 0xE7U; /* set to Octal SPI DDR with DS in Nonvolatile Config Reg 0x0 */
        SPI_control(handle, SPI_V0_CMD_ENABLE_DDR, (void *)data);
    }

    return retVal;
}

static NAND_STATUS Nand_ospiEnableOctalSDR(SPI_Handle handle)
{
    NAND_STATUS     retVal = NAND_PASS;
    uint8_t         cmdWren = NAND_CMD_WREN;
    uint32_t        data[3];

    /* Send Write Enable command */
    if(Nand_ospiCmdWrite(handle, &cmdWren, 1, 0))
    {
        retVal = NAND_FAIL;
    }

    /* Check BUSY bit of Flash */
    if (Nand_ospiWaitReady(handle, NAND_WRR_WRITE_TIMEOUT))
    {
        retVal = NAND_FAIL;
    }

    /* Enable single transfer rate mode */
    if (retVal == NAND_PASS)
    {
        /* send write VCR command to reg addr 0x0 to set to SDR mode */
        data[0] = (NAND_CMD_WRITE_VCR << 24)        | /* write volatile config reg cmd */
                  (0 << 23)                         | /* read data disable */
                  (7 << 20)                         | /* read 8 data bytes */
                  (1 << 19)                         | /* enable cmd adddr */
                  (2 << 16)                         | /* 3 address bytes */
                  (1 << 15);                          /* write data enable */
        data[1] = 0;     /* Non-volatile config register address */
        data[2] = 0xDFU; /* set to Octal SPI SDR without DS in Nonvolatile Config Reg 0x0 */
        SPI_control(handle, SPI_V0_CMD_ENABLE_SDR, (void *)data);

    }

    return retVal;
}

static NAND_STATUS Nand_ospiEnableSingleSDR(SPI_Handle handle)
{
    NAND_STATUS     retVal = NAND_PASS;
    uint8_t         cmdWren = NAND_CMD_WREN;
    uint32_t        data[3];

    /* Send Write Enable command */
    if(Nand_ospiCmdWrite(handle, &cmdWren, 1, 0))
    {
        retVal = NAND_FAIL;
    }

    /* Check BUSY bit of Flash */
    if (Nand_ospiWaitReady(handle, NAND_WRR_WRITE_TIMEOUT))
    {
        retVal = NAND_FAIL;
    }

    /* Enable single transfer rate mode */
    if (retVal == NAND_PASS)
    {
        /* send write VCR command to reg addr 0x0 to set to SDR mode */
        data[0] = (NAND_CMD_WRITE_VCR << 24)        | /* write volatile config reg cmd */
                  (0 << 23)                         | /* read data disable */
                  (7 << 20)                         | /* read 8 data bytes */
                  (1 << 19)                         | /* enable cmd adddr */
                  (2 << 16)                         | /* 3 address bytes */
                  (1 << 15);                          /* write data enable */
        data[1] = 0;     /* Non-volatile config register address */
        data[2] = 0xFFU; /* set to Single SPI SDR with DS in Nonvolatile Config Reg 0x0 */
        SPI_control(handle, SPI_V0_CMD_ENABLE_SDR, (void *)data);

    }

    return retVal;
}

static NAND_STATUS Nand_ospiResetMemory(SPI_Handle handle)
{
    NAND_STATUS     retVal = NAND_PASS;
    uint8_t         cmd;

    /* Send Reset Enable command */
    cmd = NAND_CMD_RSTEN;
    retVal = Nand_ospiCmdWrite(handle, &cmd, 1, 0);

    if (retVal == NAND_PASS)
    {
        /* Send Reset Device Memory command */
        cmd = NAND_CMD_RST_MEM;
        retVal = Nand_ospiCmdWrite(handle, &cmd, 1, 0);
    }

    return (retVal);
}

static NAND_STATUS Nand_ospiSetDummyCycle(SPI_Handle handle, uint32_t dummyCycle)
{
    NAND_STATUS     retVal = NAND_PASS;
    uint8_t         cmdWren = NAND_CMD_WREN;
    uint32_t        data[3];

    /* Send Write Enable command */
    if(Nand_ospiCmdWrite(handle, &cmdWren, 1, 0))
    {
        retVal = NAND_FAIL;
    }

    /* Check BUSY bit of Flash */
    if (Nand_ospiWaitReady(handle, NAND_WRR_WRITE_TIMEOUT))
    {
        retVal = NAND_FAIL;
    }

    /* send the dummy cycle value to reg addr 0x1 */
    if (retVal == NAND_PASS)
    {
        data[0] = (NAND_CMD_WRITE_VCR << 24)        | /* write volatile config reg cmd */
                  (0 << 23)                         | /* read data disable */
                  (7 << 20)                         | /* read 8 data bytes */
                  (1 << 19)                         | /* enable cmd adddr */
                  (2 << 16)                         | /* 3 address bytes */
                  (1 << 15);                          /* write data enable */
        data[1] = 0x00000001;                         /* Dummy cycle config register address */
        data[2] = dummyCycle;                         /* Dummy cycle # */
        SPI_control(handle, SPI_V0_CMD_CFG_DUMMY_CYCLE, (void *)data);
    }

    return retVal;
}

static NAND_STATUS Nand_ospiDisableWriteProtection(SPI_Handle handle)
{
    NAND_STATUS     retVal = NAND_PASS;
    uint32_t        data[3];
    uint32_t        retry = 10;
    volatile uint32_t delay = 1000;
    uint32_t        idleFlag = FALSE;
    OSPI_v0_HwAttrs const *hwAttrs = (OSPI_v0_HwAttrs const *)handle->hwAttrs;
    const CSL_ospi_flash_cfgRegs *pRegs = (const CSL_ospi_flash_cfgRegs *)hwAttrs->baseAddr;

    data[0] = (NAND_CMD_WRITE_STATUS << 24)   | /* write volatile config reg cmd */
            (0 << 23)                         | /* read data disable */
            (0 << 20)                         | /* read 0 data bytes */
            (1 << 19)                         | /* enable cmd adddr */
            (0 << 16)                         | /* 1 address bytes */
            (1 << 15)                         | /* write data enable */
            (0 << 12);                          /* write 1 data bytes */
    data[1] = 0x000000A0;                       /* status register 1 address */
    data[2] = 0x00000000;                       /* Value to disable all protection */

    CSL_ospiFlashStig((const CSL_ospi_flash_cfgRegs *)(hwAttrs->baseAddr), data[0], data[1], data[2]);

    while (idleFlag == FALSE)
    {
        idleFlag = CSL_ospiIsIdle(pRegs);
    }

    /* Start to execute flash read/write command */
    CSL_ospiFlashExecCmd(pRegs);

    while (retry != 0U)
    {
        /* Check the command execution status */
        if (CSL_ospiFlashExecCmdComplete(pRegs) == TRUE)
        {
            break;
        }
        while(delay--);
        delay = 1000;
        retry--;
    }

    if (retry == 0U)
    {
        retVal = (int32_t)(-1);
    }

    idleFlag = FALSE;
    while (idleFlag == FALSE)
    {
        idleFlag = CSL_ospiIsIdle(pRegs);
    }

    return retVal;
}

static void Nand_ospiSetOpcode(SPI_Handle handle)
{
    uint32_t               data[6];
    uint32_t               rdDummyCycles = 0;
    uint32_t               cmdDummyCycles = 0;
    uint32_t               readCmd;
    uint32_t               progCmd;
    uint32_t               rx_lines;
    OSPI_v0_HwAttrs const *hwAttrs= (OSPI_v0_HwAttrs const *)handle->hwAttrs;

    rx_lines = hwAttrs->xferLines;
    if (rx_lines == OSPI_XFER_LINES_OCTAL)
    {
        if (hwAttrs->dacEnable)
        {
            rdDummyCycles = NAND_OCTAL_READ_DUMMY_CYCLE;
        }
        else
        {
            rdDummyCycles = NAND_OCTAL_READ_DUMMY_CYCLE_INDAC;
        }

        if (gDtrEnable == true)
        {
            cmdDummyCycles  = NAND_OCTAL_DDR_CMD_READ_DUMMY_CYCLE;
            readCmd         = NAND_CMD_FAST_READ_DDR;
            progCmd         = NAND_CMD_PAGE_PROG;
        }
        else
        {
            cmdDummyCycles  = NAND_OCTAL_SDR_CMD_READ_DUMMY_CYCLE;
            readCmd         = NAND_CMD_OCTAL_IO_FAST_RD;
            progCmd         = NAND_CMD_EXT_OCTAL_FAST_PROG;
        }
    }
    else
    {
        /* Set to legacy SPI mode 1-1-1 if not Octal mode */
        rdDummyCycles   = NAND_SINGLE_READ_DUMMY_CYCLE;
        cmdDummyCycles  = NAND_SINGLE_CMD_READ_DUMMY_CYCLE;
        readCmd         = NAND_CMD_READ;
        progCmd         = NAND_CMD_PAGE_PROG;
    }

    data[0] = readCmd;
    data[1] = progCmd;
    data[2] = NAND_CMD_RDSR;
    data[3] = NAND_SR3_ADDR;

    /* Update the read opCode, rx lines and read dummy cycles */
    SPI_control(handle, SPI_V0_CMD_RD_DUMMY_CLKS, (void *)&rdDummyCycles);
    SPI_control(handle, SPI_V0_CMD_SET_XFER_LINES, (void *)&rx_lines);
    SPI_control(handle, SPI_V0_CMD_XFER_OPCODE, (void *)data);
    SPI_control(handle, SPI_V0_CMD_EXT_RD_DUMMY_CLKS, (void *)&cmdDummyCycles);

    /* Set device size cofigurations */
    CSL_ospiSetDevSize((const CSL_ospi_flash_cfgRegs *)(hwAttrs->baseAddr),
                        1,
                        256,
                        16);

    if (rx_lines == OSPI_XFER_LINES_OCTAL)
    {
        /* Set the opcodes for dual opcode mode */
        data[0] = 0x00;
        data[1] = 0xFA;
        data[2] = readCmd;
        data[3] = progCmd;
        data[4] = 0xF9;
        data[5] = 0x06;
        SPI_control(handle, SPI_V0_CMD_XFER_OPCODE_EXT, (void *)data);
    }
    else
    {
        CSL_ospiSetDualByteOpcodeMode((const CSL_ospi_flash_cfgRegs *)(hwAttrs->baseAddr), FALSE);
    }

    return;
}

NAND_HANDLE Nand_ospiOpen(uint32_t nandIntf, uint32_t portNum, void *params)
{
    SPI_Params      spiParams;      /* SPI params structure */
    SPI_Handle      hwHandle;       /* SPI handle */
    NAND_HANDLE     nandHandle = 0;
    OSPI_v0_HwAttrs ospiCfg;
    NAND_STATUS     retVal;
    uint32_t        data;

    /* Get the OSPI SoC configurations */
    OSPI_socGetInitCfg(portNum, &ospiCfg);

    /* Save the DTR enable flag */
    gDtrEnable = ospiCfg.dtrEnable;

    /* Reset the PHY tunning configuration data when enabled */
    data = *(uint32_t *)params;
    if (data != 0)
    {
        /* TO DO - Implement PHY tuning */
        /*
        Nand_spiPhyTuneReset(gDtrEnable);
        */
    }

    /* Save the PHY enable flag */
    gPhyEnable = ospiCfg.phyEnable;
    if (gPhyEnable == (bool)true)
    {
        /*
         * phyEnable is turned on only for DAC read,
         * it turned off for open/erase/write operation
         */
        ospiCfg.phyEnable = false;
        OSPI_socSetInitCfg(portNum, &ospiCfg);
    }

    /* Use default SPI config params if no params provided */
    SPI_Params_init(&spiParams);
    hwHandle = (SPI_Handle)SPI_open(portNum + SPI_CONFIG_OFFSET, &spiParams);
    if (hwHandle)
    {
        retVal = NAND_PASS;
        if (retVal == NAND_PASS)
        {
            OSPI_v0_HwAttrs *hwAttrs = (OSPI_v0_HwAttrs *)hwHandle->hwAttrs;
            CSL_ospiSetDualByteOpcodeMode((const CSL_ospi_flash_cfgRegs *)(hwAttrs->baseAddr), FALSE);
            if (ospiCfg.xferLines == OSPI_XFER_LINES_OCTAL)
            {
                /* Disable write protection */
                Nand_ospiDisableWriteProtection(hwHandle);
                /* Set read dummy cycles to the flash device */
                Nand_ospiSetDummyCycle(hwHandle, 0x08);
                /* Enable DDR or SDR mode for Octal lines */
                if (gDtrEnable == (bool)true)
                {
                    Nand_ospiEnableOctalDDR(hwHandle);
                }
                else
                {
                    Nand_ospiEnableOctalSDR(hwHandle);
                }
            }
            else
            {
                /* Reset device memory for all the other lines */
                Nand_ospiResetMemory(hwHandle);
                /* Disable write protection */
                Nand_ospiDisableWriteProtection(hwHandle);
                /* Set read dummy cycles to the flash device */
                Nand_ospiSetDummyCycle(hwHandle, 0x08);
                /* Enable Single SDR mode */
                Nand_ospiEnableSingleSDR(hwHandle);
            }
            
            /* Set read/write opcode and read dummy cycles */
            Nand_ospiSetOpcode(hwHandle);

            /* Read the flash ID to ensure correct config */
            if (Nand_ospiReadId(hwHandle) == NAND_PASS)
            {
                Nand_ospiInfo.hwHandle = (uintptr_t)hwHandle;
                nandHandle = (NAND_HANDLE)(&Nand_ospiInfo);
            }
        }

        if (nandHandle == 0)
        {
            SPI_close(hwHandle);
        }
    }

    return (nandHandle);
}

void Nand_ospiClose(NAND_HANDLE handle)
{
    NAND_Info    *nandOspiInfo;
    SPI_Handle   spiHandle;

    if (handle)
    {
        nandOspiInfo = (NAND_Info *)handle;
        spiHandle = (SPI_Handle)nandOspiInfo->hwHandle;

        if (spiHandle)
        {
            SPI_close(spiHandle);
        }
    }
}

static NAND_STATUS Nand_ospiCmdWrite(SPI_Handle handle, uint8_t *cmdBuf,
                                        uint32_t cmdLen, uint32_t dataLen)
{
    SPI_Transaction  transaction;
    uint32_t         transferType = SPI_TRANSACTION_TYPE_WRITE;
    bool             ret;

    /* Update the mode and transfer type with the required values */
    SPI_control(handle, SPI_V0_CMD_SET_CFG_MODE, NULL);
    SPI_control(handle, SPI_V0_CMD_XFER_MODE_RW, (void *)&transferType);

    transaction.txBuf = (void *)cmdBuf; /* Buffer includes command and write data */
    transaction.count = cmdLen + dataLen;
    transaction.rxBuf = NULL;
    transaction.arg = (void *)(uintptr_t)dataLen;

    ret = SPI_transfer(handle, &transaction);
    if (ret == true)
    {
        return NAND_PASS;
    }
	else
    {
        return NAND_FAIL;
    }
}

static NAND_STATUS Nand_ospiWaitReady(SPI_Handle handle, uint32_t timeOut)
{
    uint8_t         status;
    uint8_t         cmd[3];
    uint32_t        cmdDummyCycles = 0;
    OSPI_v0_HwAttrs const *hwAttrs= (OSPI_v0_HwAttrs const *)handle->hwAttrs;

    if(CSL_ospiGetDualByteOpcodeMode((const CSL_ospi_flash_cfgRegs *)(hwAttrs->baseAddr)))
    {
        cmd[0] = NAND_CMD_RDSR;
        cmd[1] = NAND_SR3_ADDR;  /* Address Bytes */
        cmd[2] = 0x00;

        cmdDummyCycles = 7;
        SPI_control(handle, SPI_V0_CMD_EXT_RD_DUMMY_CLKS, (void *)&cmdDummyCycles);

        do
        {
            if (NAND_ospiCmdRead(handle, cmd, 3, &status, 1))
            {
                return NAND_FAIL;
            }
            if ((status & NAND_SR_WIP) == 0)
            {
                break;
            }

            timeOut--;
            if (!timeOut) {
                status = 0;
                break;
            }

        } while (1);
        cmdDummyCycles = 8;
        SPI_control(handle, SPI_V0_CMD_EXT_RD_DUMMY_CLKS, (void *)&cmdDummyCycles);
    }
    else
    {
        cmd[0] = NAND_CMD_RDSR;
        cmd[1] = NAND_SR3_ADDR;  /* Address Bytes */

        cmdDummyCycles = 0;
        SPI_control(handle, SPI_V0_CMD_EXT_RD_DUMMY_CLKS, (void *)&cmdDummyCycles);

        do
        {
            if (NAND_ospiCmdRead(handle, cmd, 2, &status, 1))
            {
                return NAND_FAIL;
            }
            if ((status & NAND_SR_WIP) == 0)
            {
                break;
            }

            timeOut--;
            if (!timeOut) {
                break;
            }

        } while (1);
        cmdDummyCycles = 8;
        SPI_control(handle, SPI_V0_CMD_EXT_RD_DUMMY_CLKS, (void *)&cmdDummyCycles);
    }

    if ((status & NAND_SR_WIP) == 0)
    {
        return NAND_PASS;
    }

    /* Timed out */
    return NAND_FAIL;
}

static SPI_Transaction transaction;
NAND_STATUS Nand_ospiRead(NAND_HANDLE handle, uint32_t addr, uint32_t len, uint8_t *buf)
{
    NAND_Info        *nandOspiInfo;
    SPI_Handle       spiHandle;
    uint32_t         rdAddr;
    uint32_t         pageAddr;
    uint32_t         colmAddr;
    bool             ret = TRUE;
    uint32_t         transferType = SPI_TRANSACTION_TYPE_READ;
    OSPI_v0_HwAttrs *hwAttrs;
    uint8_t          pageReadCmd[4];
    uint32_t         pageReadCmdLen = 4;

    if (!handle)
    {
        return NAND_FAIL;
    }

    nandOspiInfo = (NAND_Info *)handle;
    if (!nandOspiInfo->hwHandle)
    {
        return NAND_FAIL;
    }
    spiHandle = (SPI_Handle)nandOspiInfo->hwHandle;
    hwAttrs = (OSPI_v0_HwAttrs *)spiHandle->hwAttrs;

    if (gPhyEnable == (bool)true)
    {
        /* TO DO - Implement PHY tuning */
        /*
        if (Nand_spiPhyTune(spiHandle, NAND_TUNING_DATA_OFFSET) == NAND_FAIL)
           return NAND_FAIL;
        */
    }
    /* Validate address input */
    if ((addr + len) > NAND_SIZE)
    {
        return NAND_FAIL;
    }

    for(rdAddr = addr; rdAddr < (addr+len); rdAddr += NAND_PAGE_SIZE)
    {
        /* Split the page and column addresses */
        pageAddr = rdAddr / NAND_PAGE_SIZE;
        colmAddr = rdAddr % NAND_PAGE_SIZE;

        if (hwAttrs->xferLines == OSPI_XFER_LINES_OCTAL)
        {
            /* Send the page read command */
            pageReadCmd[0] = NAND_CMD_PAGE_READ;
            pageReadCmd[1] = (pageAddr >>  8) & 0xff;    /* page address 2 bytes */
            pageReadCmd[2] = (pageAddr >>  0) & 0xff;
            pageReadCmdLen = 3;
            Nand_ospiCmdWrite(spiHandle, pageReadCmd, pageReadCmdLen, 0);
        }
        else
        {
            /* Send the page read command */
            pageReadCmd[0] = NAND_CMD_PAGE_READ;
            pageReadCmd[1] = 0x00;                       /* Dummy Byte */
            pageReadCmd[2] = (pageAddr >>  8) & 0xff;    /* page address 2 bytes */
            pageReadCmd[3] = (pageAddr >>  0) & 0xff;
            pageReadCmdLen = 4;
            Nand_ospiCmdWrite(spiHandle, pageReadCmd, pageReadCmdLen, 0);
        }

        /* Check BUSY bit of Flash */
        if (Nand_ospiWaitReady(spiHandle, NAND_WRR_WRITE_TIMEOUT))
        {
            return NAND_FAIL;
        }

        /* Set transfer mode and read type */
        SPI_control(spiHandle, SPI_V0_CMD_SET_XFER_MODE, NULL);
        SPI_control(spiHandle, SPI_V0_CMD_XFER_MODE_RW, (void *)&transferType);

        transaction.arg   = (void *)(uintptr_t)colmAddr;
        transaction.txBuf = NULL;
        transaction.rxBuf = (void *)(buf+rdAddr-addr);
        transaction.count = NAND_PAGE_SIZE;

        ret = SPI_transfer(spiHandle, &transaction);
        if (ret == false)
        {
            return NAND_FAIL;
        }
    }
    return NAND_PASS;
}

NAND_STATUS Nand_ospiWrite(NAND_HANDLE handle, uint32_t addr, uint32_t len, uint8_t *buf)
{
    NAND_Info        *nandOspiInfo;
    SPI_Handle       spiHandle;
    bool             ret;
    uint32_t         byteAddr;
    uint32_t         pageAddr;
    uint32_t         colmAddr;
    uint32_t         wrSize = len;
    uint32_t         chunkLen;
    uint32_t         actual;
    uint32_t         transferType = SPI_TRANSACTION_TYPE_WRITE;
    OSPI_v0_HwAttrs *hwAttrs;
    uint8_t          progExecuteCmd[4];
    uint32_t         progExecuteCmdLen = 4;
    uint8_t          cmdWren = NAND_CMD_WREN;

    if (!handle)
    {
        return NAND_FAIL;
    }

    nandOspiInfo = (NAND_Info *)handle;
    if (!nandOspiInfo->hwHandle)
    {
        return NAND_FAIL;
    }

    /* Validate address input */
    if ((addr + len) > NAND_SIZE)
    {
        return NAND_FAIL;
    }

    spiHandle = (SPI_Handle)nandOspiInfo->hwHandle;
    hwAttrs = (OSPI_v0_HwAttrs *)spiHandle->hwAttrs;

    wrSize = NAND_PAGE_SIZE;
    byteAddr = addr & (wrSize - 1);

    for (actual = 0; actual < len; actual += chunkLen)
    {
        /* Send Write Enable command */
        if(Nand_ospiCmdWrite(spiHandle, &cmdWren, 1, 0))
        {
            return NAND_FAIL;
        }

        /* Check BUSY bit of Flash */
        if (Nand_ospiWaitReady(spiHandle, NAND_WRR_WRITE_TIMEOUT))
        {
            return NAND_FAIL;
        }

        /* Set the transfer mode, write op code and tx lines */
        SPI_control(spiHandle, SPI_V0_CMD_SET_XFER_MODE, NULL);
        SPI_control(spiHandle, SPI_V0_CMD_XFER_MODE_RW, (void *)&transferType);

        /* Send Page Program command */
        chunkLen = ((len - actual) < (wrSize - byteAddr) ?
                    (len - actual) : (wrSize - byteAddr));

        /* Split the page and column addresses */
        pageAddr = addr / NAND_PAGE_SIZE;
        colmAddr = addr % NAND_PAGE_SIZE;

        transaction.arg   = (void *)(uintptr_t)colmAddr;
        transaction.txBuf = (void *)(buf + actual);
        transaction.rxBuf = NULL;
        transaction.count = chunkLen;

        ret = SPI_transfer(spiHandle, &transaction);
        if (ret == false)
        {
            return NAND_FAIL;
        }

        /* Check BUSY bit of Flash */
        if (Nand_ospiWaitReady(spiHandle, NAND_WRR_WRITE_TIMEOUT))
        {
            return NAND_FAIL;
        }

        if (hwAttrs->xferLines == OSPI_XFER_LINES_OCTAL)
        {
            progExecuteCmd[0] = NAND_CMD_PAGE_PROG_EXECUTE;
            progExecuteCmd[1] = (pageAddr >>  8) & 0xff;    /* page address 2 bytes */
            progExecuteCmd[2] = (pageAddr >>  0) & 0xff;
            progExecuteCmdLen = 3;
            Nand_ospiCmdWrite(spiHandle, progExecuteCmd, progExecuteCmdLen, 0);
        }
        else
        {
            progExecuteCmd[0] = NAND_CMD_PAGE_PROG_EXECUTE;
            progExecuteCmd[1] = 0x00;                       /* Dummy Byte */
            progExecuteCmd[2] = (pageAddr >>  8) & 0xff;    /* page address 2 bytes */
            progExecuteCmd[3] = (pageAddr >>  0) & 0xff;
            progExecuteCmdLen = 4;
            Nand_ospiCmdWrite(spiHandle, progExecuteCmd, progExecuteCmdLen, 0);
        }
        addr += chunkLen;
        byteAddr = 0;

        /* Wait till the write operation completes */
        if (Nand_ospiWaitReady(spiHandle, NAND_WRR_WRITE_TIMEOUT))
        {
            return NAND_FAIL;
        }
    }

    return NAND_PASS;
}

NAND_STATUS Nand_ospiErase(NAND_HANDLE handle, int32_t erLoc)
{
    uint8_t         cmd[4];
    uint32_t        cmdLen;
    uint32_t        address = 0;
    uint32_t        pageAddr = 0;
    uint8_t         cmdWren  = NAND_CMD_WREN;
    NAND_Info       *nandOspiInfo;
    SPI_Handle      spiHandle;
    OSPI_v0_HwAttrs *hwAttrs;

    if (!handle)
    {
        return NAND_FAIL;
    }

    nandOspiInfo = (NAND_Info *)handle;
    if (!nandOspiInfo->hwHandle)
    {
        return NAND_FAIL;
    }
    spiHandle = (SPI_Handle)nandOspiInfo->hwHandle;
    hwAttrs = (OSPI_v0_HwAttrs *)spiHandle->hwAttrs;

    if (erLoc >= NAND_NUM_BLOCKS)
    {
        return NAND_FAIL;
    }
    address = erLoc * NAND_BLOCK_SIZE;

    /* Get the page addresses */
    pageAddr = address / NAND_PAGE_SIZE;

    if (hwAttrs->xferLines == OSPI_XFER_LINES_OCTAL)
    {
        cmd[0]  = NAND_CMD_BLOCK_ERASE;
        cmd[1] = (pageAddr >> 8) & 0xff; /* page address 2 bytes */
        cmd[2] = (pageAddr >> 0) & 0xff;
        cmdLen = 3;
    }
    else
    {
        cmd[0]  = NAND_CMD_BLOCK_ERASE;
        cmd[1]  = 0x00;                  /* Dummy Byte */
        cmd[2] = (pageAddr >> 8) & 0xff; /* page address 2 bytes */
        cmd[3] = (pageAddr >> 0) & 0xff;
        cmdLen = 4;
    }

    if (Nand_ospiCmdWrite(spiHandle, &cmdWren, 1, 0))
    {
    	return NAND_FAIL;
    }

    if (Nand_ospiWaitReady(spiHandle, NAND_WRR_WRITE_TIMEOUT))
    {
    	return NAND_FAIL;
    }

    if (Nand_ospiCmdWrite(spiHandle, cmd, cmdLen, 0))
    {
    	return NAND_FAIL;
    }

    if (Nand_ospiWaitReady(spiHandle, NAND_BULK_ERASE_TIMEOUT))
    {
    	return NAND_FAIL;
    }

    return NAND_PASS;
}
