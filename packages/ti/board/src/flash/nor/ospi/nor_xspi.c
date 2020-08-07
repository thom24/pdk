/*
 * Copyright (c) 2020, Texas Instruments Incorporated
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

#include <ti/board/src/flash/nor/ospi/nor_xspi.h>
#include <ti/drv/spi/soc/SPI_soc.h>
#include <ti/csl/soc.h>

#if defined (j7200_evm)
/* SPI entry offset is at index 5 of SPI config array */
#define SPI_CONFIG_OFFSET     (5U)
#else
#define SPI_CONFIG_OFFSET     CSL_MCSPI_PER_CNT
#endif

static NOR_HANDLE Nor_xspiOpen(uint32_t norIntf, uint32_t portNum, void *params);
static void Nor_xspiClose(NOR_HANDLE handle);
static NOR_STATUS Nor_xspiRead(NOR_HANDLE handle, uint32_t addr,
                               uint32_t len, uint8_t *buf, uint32_t mode);
static NOR_STATUS Nor_xspiWrite(NOR_HANDLE handle, uint32_t addr,
                                uint32_t len, uint8_t *buf, uint32_t mode);
static NOR_STATUS Nor_xspiErase(NOR_HANDLE handle, int32_t eraseCnt, bool blkErase);

static NOR_STATUS Nor_xspiCmdWrite(SPI_Handle handle, uint8_t *cmdBuf,
                                   uint32_t cmdLen, uint32_t dataLen);
static NOR_STATUS Nor_xspiWaitReady(SPI_Handle handle, uint32_t timeOut);

/* NOR function table for NOR OSPI interface implementation */
const NOR_FxnTable Nor_xspiFxnTable =
{
    &Nor_xspiOpen,
    &Nor_xspiClose,
    &Nor_xspiRead,
    &Nor_xspiWrite,
    &Nor_xspiErase,
};

NOR_Info Nor_xspiInfo =
{
    0,                          /* hwHandle */
    0,                          /* manufacturerId */
    0,                          /* deviceId */
    0,                          /* busWidth */
    NOR_NUM_BLOCKS,            /* blockCnt */
    NOR_NUM_PAGES_PER_BLOCK,   /* pageCnt */
    NOR_PAGE_SIZE,             /* pageSize */
    0,                         /* baseAddr */
    NOR_SECTOR_SIZE            /* sectorSize */
};

static NOR_STATUS Nor_xspiCmdRead(SPI_Handle handle, uint8_t *cmdBuf,
                            uint32_t cmdLen, uint8_t *rxBuf, uint32_t rxLen)
{
    SPI_Transaction  transaction;
    uint32_t         transferType = SPI_TRANSACTION_TYPE_READ;
    bool             ret;    

    /* Update the mode and transfer type with the required values */
    SPI_control(handle, SPI_V0_CMD_SET_CFG_MODE, NULL);
    SPI_control(handle, SPI_V0_CMD_XFER_MODE_RW, (void *)&transferType);

    transaction.txBuf = (void *)cmdBuf;
    transaction.rxBuf = (void *)rxBuf;
    transaction.count = cmdLen + rxLen;

    ret = SPI_transfer(handle, &transaction);
    if (ret == true)
    {
        return NOR_PASS;
    }
	else
    {
        return NOR_FAIL;
    }
}

static NOR_STATUS Nor_xspiCmdWrite(SPI_Handle handle, uint8_t *cmdBuf,
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
        return NOR_PASS;
    }
	else
    {
        return NOR_FAIL;
    }
}

static NOR_STATUS Nor_xspiRegRead(SPI_Handle handle,
                                  uint32_t   regAddr,
                                  uint8_t    *data)
{
    NOR_STATUS retVal;
    uint8_t    cmd[6];

    cmd[0] = NOR_CMD_RDREG;
    cmd[1] = (regAddr >> 24) & 0xFF;
    cmd[2] = (regAddr >> 16) & 0xFF;
    cmd[3] = (regAddr >> 8) & 0xFF;
    cmd[4] = (regAddr) & 0xFF;

    retVal = Nor_xspiCmdRead(handle, cmd, 5, data, 1);
    if(retVal != NOR_PASS)
    {
        return NOR_FAIL;
    }

    return retVal;
}

static NOR_STATUS Nor_xspiRegWrite(SPI_Handle handle,
                                   uint32_t   regAddr,
                                   uint8_t    data)
{
    NOR_STATUS retVal;
    uint8_t    cmd[6];

    /* Enable flash write */
    cmd[0] = NOR_CMD_WREN;
    retVal = Nor_xspiCmdWrite(handle, cmd, 1, 0);
    if(retVal != NOR_PASS)
    {
        return NOR_FAIL;
    }

    retVal = Nor_xspiWaitReady(handle, NOR_WRR_WRITE_TIMEOUT);
    if(retVal != NOR_PASS)
    {
        return NOR_FAIL;
    }

    cmd[0] = NOR_CMD_WRREG;
    cmd[1] = (regAddr >> 24) & 0xFF;
    cmd[2] = (regAddr >> 16) & 0xFF;
    cmd[3] = (regAddr >> 8) & 0xFF;
    cmd[4] = (regAddr) & 0xFF;
    cmd[5] = data;

    retVal = Nor_xspiCmdWrite(handle, cmd, 5, 1);
    if(retVal != NOR_PASS)
    {
        return NOR_FAIL;
    }

    retVal = Nor_xspiWaitReady(handle, NOR_WRR_WRITE_TIMEOUT);
    if(retVal != NOR_PASS)
    {
        return NOR_FAIL;
    }

    return retVal;
}

static NOR_STATUS Nor_xspiHybridSectCfg(SPI_Handle handle,
                                        uint8_t    enable,
                                        uint32_t   cfgFlag)
{
    NOR_STATUS  retVal;
    uint8_t     regData;

    /* Read configuration register3 */
    retVal = Nor_xspiRegRead(handle, NOR_CFG3_VREG_ADDR, &regData);
    if(retVal != NOR_PASS)
    {
        return NOR_FAIL;
    }

    if(enable == 1)
    {
        /* Enable hybrid sector configuration */
        regData &= ~0x8;
    }
    else
    {
        /* Disable hybrid sector configuration */
        regData |= 0x8;
    }

    /* Write configuration register3 */
    retVal = Nor_xspiRegWrite(handle, NOR_CFG3_VREG_ADDR, regData);
    if(retVal != NOR_PASS)
    {
        return NOR_FAIL;
    }

    return retVal;
}

static NOR_STATUS Nor_xspiReadId(SPI_Handle handle)
{
    NOR_STATUS  retVal;
    uint8_t     idCode[NOR_RDID_NUM_BYTES];
    uint8_t     cmd[6];
    uint32_t    manfID, devID;
    
    cmd[0] = NOR_CMD_RDID;
    cmd[1] = 0;  /* Address Bytes */
    cmd[2] = 0;
    cmd[3] = 0;
    cmd[4] = 0;

    retVal = Nor_xspiCmdRead(handle, cmd, 5, idCode, NOR_RDID_NUM_BYTES);
    if (retVal == NOR_PASS)
    {
        manfID = (uint32_t)idCode[0];
        devID = ((uint32_t)idCode[1] << 8) | ((uint32_t)idCode[2]);
        if ((manfID == NOR_MANF_ID) && (devID == NOR_DEVICE_ID))
        {
            Nor_xspiInfo.manufacturerId = manfID;
            Nor_xspiInfo.deviceId = devID;
        }
        else
        {
            retVal = NOR_FAIL;
        }
    }

    return (retVal);
}

static NOR_STATUS Nor_xspiEnableDDR(SPI_Handle handle)
{
    NOR_STATUS             retVal;
    uint8_t                cmdWren = NOR_CMD_WREN;
    uint32_t               data[6];
    uint32_t               dummyCycles;
    uint32_t               rx_lines;
    OSPI_v0_HwAttrs const *hwAttrs= (OSPI_v0_HwAttrs const *)handle->hwAttrs;

    /* Send Write Enable command */
    retVal = Nor_xspiCmdWrite(handle, &cmdWren, 1, 0);

    /* Enable double transfer rate mode */
    if (retVal == NOR_PASS)
    {
        /* send write VCR command to reg addr 0x0 to set to DDR mode */
        data[0] = (NOR_CMD_WRITE_VCR << 24)         | /* write volatile config reg cmd */
                  (0 << 23)                         | /* read data disable */
                  (7 << 20)                         | /* read 8 data bytes */
                  (1 << 19)                         | /* enable cmd adddr */
                  (2 << 16)                         | /* 3 address bytes */
                  (1 << 15);                          /* write data enable */
        data[1] = 0x00800006;     /* Non-volatile config register address */
        data[2] = 0x43; /* set to Octal DDR in Nonvolatile Config Reg 0x0 */
        SPI_control(handle, SPI_V0_CMD_ENABLE_DDR, (void *)data);

        /* Set opcodes */
        dummyCycles = NOR_OCTAL_READ_DUMMY_CYCLE + 1;
        rx_lines    = hwAttrs->xferLines;
        data[0]     = NOR_CMD_OCTAL_DDR_READ;
        data[1]     = NOR_CMD_OCTAL_PROG;
        data[2]     = NOR_CMD_RDSR;

        /* Update the read opCode, rx lines and read dummy cycles */
        SPI_control(handle, SPI_V0_CMD_RD_DUMMY_CLKS, (void *)&dummyCycles);
        SPI_control(handle, SPI_V0_CMD_SET_XFER_LINES, (void *)&rx_lines);
        SPI_control(handle, SPI_V0_CMD_XFER_OPCODE, (void *)data);

        /* Set the opcodes for dual opcode mode */
        data[0]     = 0;
        data[1]     = 0xFA;
        data[2]     = NOR_CMD_OCTAL_PROG;
        data[3]     = NOR_CMD_OCTAL_DDR_READ;
        data[4]     = 0xF9;
        data[5]     = 0x6;
        SPI_control(handle, SPI_V0_CMD_XFER_OPCODE_EXT, (void *)data);

        dummyCycles = NOR_OCTAL_DDR_CMD_READ_DUMMY_CYCLE;
        SPI_control(handle, SPI_V0_CMD_EXT_RD_DUMMY_CLKS, (void *)&dummyCycles);
    }

    return retVal;
}

static NOR_STATUS Nor_xspiEnableSDR(SPI_Handle handle)
{
    NOR_STATUS             retVal;
    uint8_t                cmdWren = NOR_CMD_WREN;
    uint32_t               data[6];
    uint32_t               dummyCycles;
    uint32_t               rx_lines;
    OSPI_v0_HwAttrs const *hwAttrs= (OSPI_v0_HwAttrs const *)handle->hwAttrs;
    CSL_ospi_flash_cfgRegs *regAddr;
    uint32_t               regVal;

    /* Send Write Enable command */
    retVal = Nor_xspiCmdWrite(handle, &cmdWren, 1, 0);

    /* Enable double transfer rate mode */
    if (retVal == NOR_PASS)
    {
        /* send write VCR command to reg addr 0x0 to set to DDR mode */
        data[0] = (NOR_CMD_WRITE_VCR << 24)         | /* write volatile config reg cmd */
                  (0 << 23)                         | /* read data disable */
                  (7 << 20)                         | /* read 8 data bytes */
                  (1 << 19)                         | /* enable cmd adddr */
                  (2 << 16)                         | /* 3 address bytes */
                  (1 << 15);                          /* write data enable */
        data[1] = 0x00800006;     /* Non-volatile config register address */
        data[2] = 0x41U; /* set to Extended SPI mode in Nonvolatile Config Reg 0x0 */
        SPI_control(handle, SPI_V0_CMD_ENABLE_SDR, (void *)data);

        /* Set opcodes */
        dummyCycles = NOR_OCTAL_READ_DUMMY_CYCLE;
        rx_lines    = hwAttrs->xferLines;
        data[0]     = NOR_CMD_OCTAL_READ;
        data[1]     = NOR_CMD_OCTAL_PROG;
        data[2]     = NOR_CMD_RDSR;

        /* Update the read opCode, rx lines and read dummy cycles */
        SPI_control(handle, SPI_V0_CMD_RD_DUMMY_CLKS, (void *)&dummyCycles);
        SPI_control(handle, SPI_V0_CMD_SET_XFER_LINES, (void *)&rx_lines);
        SPI_control(handle, SPI_V0_CMD_XFER_OPCODE, (void *)data);

        /* Set the opcodes for dual opcode mode */
        data[0]     = 0;
        data[1]     = 0xFA;
        data[2]     = NOR_CMD_OCTAL_PROG;
        data[3]     = NOR_CMD_OCTAL_READ;
        data[4]     = 0xF9;
        data[5]     = 0x6;
        SPI_control(handle, SPI_V0_CMD_XFER_OPCODE_EXT, (void *)data);
        
        dummyCycles = NOR_OCTAL_SDR_CMD_READ_DUMMY_CYCLE;
        SPI_control(handle, SPI_V0_CMD_EXT_RD_DUMMY_CLKS, (void *)&dummyCycles);

        /* Flash device requires 4-bit access for command as well in quad mode */
        regAddr = (CSL_ospi_flash_cfgRegs *)(hwAttrs->baseAddr);
        regVal = CSL_REG32_RD(&regAddr->DEV_INSTR_RD_CONFIG_REG);
        regVal |= 0x300;
        CSL_REG32_WR(&regAddr->DEV_INSTR_RD_CONFIG_REG, regVal);
    }

    return NOR_PASS;
}

NOR_HANDLE Nor_xspiOpen(uint32_t norIntf, uint32_t portNum, void *params)
{
    SPI_Params      spiParams;  /* SPI params structure */
    SPI_Handle      hwHandle;  /* SPI handle */
    NOR_HANDLE      norHandle = 0;
    OSPI_v0_HwAttrs ospi_cfg;
    NOR_STATUS      retVal;

    /* Update the default driver config for xSPI operation */
    OSPI_socGetInitCfg(portNum, &ospi_cfg);
    if(ospi_cfg.dacEnable == false)
    {
        ospi_cfg.phyEnable   = false;
        ospi_cfg.baudRateDiv = BOARD_XSPI_BAUDRATE_DIV;
        OSPI_socSetInitCfg(portNum, &ospi_cfg);
    }

    /* Use default SPI config params if no params provided */
    SPI_Params_init(&spiParams);

    hwHandle = (SPI_Handle)SPI_open(portNum + SPI_CONFIG_OFFSET, &spiParams);

    if (hwHandle)
    {
        retVal = NOR_PASS;
        if (retVal == NOR_PASS)
        {
            OSPI_socGetInitCfg(portNum, &ospi_cfg);

            if (ospi_cfg.dtrEnable == true)
            {
                Nor_xspiEnableDDR(hwHandle);
            }
            else
            {
                Nor_xspiEnableSDR(hwHandle);
            }

            if (ospi_cfg.phyEnable == true)
            {
                uint32_t offset = NOR_TUNING_DATA_OFFSET;

                retVal = Nor_spiPhyTune(hwHandle, offset);
                if (retVal == NOR_PASS)
                {
                    retVal = Nor_xspiReadId(hwHandle);
                }

                if (retVal == NOR_PASS)
                {
                    Nor_xspiInfo.hwHandle = (uintptr_t)hwHandle;
                    norHandle = (NOR_HANDLE)(&Nor_xspiInfo);
                }
            }
            else /* ospi_cfg->phyEnable == false */
            {                
                if (Nor_xspiReadId(hwHandle) == NOR_PASS)
                {
                    Nor_xspiInfo.hwHandle = (uintptr_t)hwHandle;
                    norHandle = (NOR_HANDLE)(&Nor_xspiInfo);
                }
            }
        }

        if (norHandle == 0)
        {
            SPI_close(hwHandle);
        }
    }

    if(hwHandle != NULL)
    {
        /* Disable hybrid sector configuration */
        Nor_xspiHybridSectCfg(hwHandle, 0, 0);
    }

    return (norHandle);
}

void Nor_xspiClose(NOR_HANDLE handle)
{
    NOR_Info    *norOspiInfo;
    SPI_Handle   spiHandle;

    if (handle)
    {
        norOspiInfo = (NOR_Info *)handle;
        spiHandle = (SPI_Handle)norOspiInfo->hwHandle;

        if (spiHandle)
        {
            SPI_close(spiHandle);
        }
    }
}

static NOR_STATUS Nor_xspiWaitReady(SPI_Handle handle, uint32_t timeOut)
{
    uint8_t         status;
    uint8_t         cmd[6];

    cmd[0] = NOR_CMD_RDREG;
    cmd[1] = 0;
    cmd[2] = NOR_VREG_OFFSET;
    cmd[3] = 0;
    cmd[4] = 0;

    do
    {
        if (Nor_xspiCmdRead(handle, cmd, 5, &status, 1))
        {
            return NOR_FAIL;
        }
        if ((status & NOR_SR_WIP) == 0)
        {
            break;
        }

        timeOut--;
        if (!timeOut) {
            break;
        }

    } while (1);

    if ((status & NOR_SR_WIP) == 0)
    {
        return NOR_PASS;
    }

    /* Timed out */
    return NOR_FAIL;
}

static SPI_Transaction transaction;
NOR_STATUS Nor_xspiRead(NOR_HANDLE handle, uint32_t addr,
                        uint32_t len, uint8_t *buf, uint32_t mode)
{
    NOR_Info        *norOspiInfo;
    SPI_Handle       spiHandle;
    bool             ret;
    uint32_t         transferType = SPI_TRANSACTION_TYPE_READ;

    if (!handle)
    {
        return NOR_FAIL;
    }

    norOspiInfo = (NOR_Info *)handle;
    if (!norOspiInfo->hwHandle)
    {
        return NOR_FAIL;
    }
    spiHandle = (SPI_Handle)norOspiInfo->hwHandle;

    /* Validate address input */
    if ((addr + len) > NOR_SIZE)
    {
        return NOR_FAIL;
    }
    /* Set transfer mode and read type */
    SPI_control(spiHandle, SPI_V0_CMD_SET_XFER_MODE, NULL);
    SPI_control(spiHandle, SPI_V0_CMD_XFER_MODE_RW, (void *)&transferType);

    transaction.arg   = (void *)(uintptr_t)addr;
    transaction.txBuf = NULL;
    transaction.rxBuf = (void *)buf;
    transaction.count = len;

    ret = SPI_transfer(spiHandle, &transaction);
    if (ret == true)
    {
        return NOR_PASS;
    }
	else
    {
        return NOR_FAIL;
    }
}

NOR_STATUS Nor_xspiWrite(NOR_HANDLE handle, uint32_t addr, uint32_t len,
                         uint8_t *buf, uint32_t mode)
{
    NOR_Info        *norOspiInfo;
    SPI_Handle       spiHandle;
    bool             ret;
    uint32_t         byteAddr;
    uint32_t         wrSize = len;
    uint32_t         chunkLen;
    uint32_t         actual;
    uint32_t         transferType = SPI_TRANSACTION_TYPE_WRITE;
    OSPI_v0_HwAttrs *hwAttrs;
    uint8_t         cmdWren[2];

    if (!handle)
    {
        return NOR_FAIL;
    }

    norOspiInfo = (NOR_Info *)handle;
    if (!norOspiInfo->hwHandle)
    {
        return NOR_FAIL;
    }

    /* Validate address input */
    if ((addr + len) > NOR_SIZE)
    {
        return NOR_FAIL;
    }

    spiHandle = (SPI_Handle)norOspiInfo->hwHandle;
    hwAttrs = (OSPI_v0_HwAttrs *)spiHandle->hwAttrs;

    cmdWren[0]  = NOR_CMD_WREN;

    if (hwAttrs->dacEnable )
    {
        /* direct access transfer mode */
        if ((hwAttrs->dmaEnable) && (hwAttrs->phyEnable))
        {
            wrSize = 16U;
        }
    }
    else
    {
        /* indirect access transfer mode */
        wrSize = NOR_PAGE_SIZE;
    }
    byteAddr = addr & (wrSize - 1);

    for (actual = 0; actual < len; actual += chunkLen)
    {
        if (Nor_xspiCmdWrite(spiHandle, cmdWren, 1, 0))
        {
            return NOR_FAIL;
        }

        if (Nor_xspiWaitReady(spiHandle, NOR_WRR_WRITE_TIMEOUT))
        {
            return NOR_FAIL;
        }

        /* Set the transfer mode, write op code and tx lines */
        SPI_control(spiHandle, SPI_V0_CMD_SET_XFER_MODE, NULL);
        SPI_control(spiHandle, SPI_V0_CMD_XFER_MODE_RW, (void *)&transferType);

        /* Send Page Program command */
        chunkLen = ((len - actual) < (wrSize - byteAddr) ?
                    (len - actual) : (wrSize - byteAddr));

        transaction.arg   = (void *)(uintptr_t)addr;
        transaction.txBuf = (void *)(buf + actual);
        transaction.rxBuf = NULL;
        transaction.count = chunkLen;

        ret = SPI_transfer(spiHandle, &transaction);
        if (ret == false)
        {
            return NOR_FAIL;
        }

        addr += chunkLen;
        byteAddr = 0;

        /* Wait till the write operation completes */
        if (Nor_xspiWaitReady(spiHandle, NOR_PAGE_PROG_TIMEOUT))
        {
            return NOR_FAIL;
        }
    }

    return NOR_PASS;
}

NOR_STATUS Nor_xspiErase(NOR_HANDLE handle, int32_t erLoc, bool blkErase)
{
    uint8_t         cmd[10];
    uint8_t         wrenStatus = 0;
    uint32_t        cmdLen;
    uint32_t        address = 0;
    uint8_t         cmdWren[2];
    NOR_Info       *norOspiInfo;
    SPI_Handle      spiHandle;
    
    wrenStatus = wrenStatus;

    if (!handle)
    {
        return NOR_FAIL;
    }
    
    cmdWren[0]  = NOR_CMD_WREN;

    norOspiInfo = (NOR_Info *)handle;
    if (!norOspiInfo->hwHandle)
    {
        return NOR_FAIL;
    }
    spiHandle = (SPI_Handle)norOspiInfo->hwHandle;

    if (erLoc == NOR_BE_SECTOR_NUM)
    {
        cmd[0] = NOR_CMD_BULK_ERASE;
        cmdLen = 1;
    }
    else
    {
        if (erLoc >= NOR_NUM_BLOCKS)
        {
            return NOR_FAIL;
        }

        address = erLoc * NOR_BLOCK_SIZE;
        cmd[0]  = NOR_CMD_BLOCK_ERASE;

        cmd[1] = (address >> 24) & 0xff; /* 4 address bytes */
        cmd[2] = (address >> 16) & 0xff;
        cmd[3] = (address >>  8) & 0xff;
        cmd[4] = (address >>  0) & 0xff;
        cmdLen = 5;
    }

    if (Nor_xspiWaitReady(spiHandle, NOR_WRR_WRITE_TIMEOUT))
    {
    	return NOR_FAIL;
    }

    if (Nor_xspiCmdWrite(spiHandle, cmdWren, 1, 0))
    {
    	return NOR_FAIL;
    }

    if (Nor_xspiWaitReady(spiHandle, NOR_WRR_WRITE_TIMEOUT))
    {
    	return NOR_FAIL;
    }

    if (Nor_xspiCmdWrite(spiHandle, cmd, cmdLen, 0))
    {
    	return NOR_FAIL;
    }

    if (Nor_xspiWaitReady(spiHandle, NOR_BULK_ERASE_TIMEOUT))
    {
    	return NOR_FAIL;
    }

    return NOR_PASS;
}
