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

#include "sbl_soc.h"
#include "sbl_soc_cfg.h"
#include "sbl_log.h"
#include "sbl_profile.h"
#include "sbl_rprc_parse.h"
#include "sbl_err_trap.h"
#include "sbl_rcm.h"
#include <strings.h>
#include <ti/drv/i2c/I2C.h>
#include <ti/drv/i2c/soc/I2C_soc.h>
#include <ti/board/board.h>


#define SBL_NEVER_AUTH_APP         (0x55555555u)
#define SBL_ALWAYS_AUTH_APP        (0xAAAAAAAAu)
#define SBL_SYSFW_MAX_SIZE         (0x42000U)


#define SBL_SYS_STATUS_REG              (0x44234100U)
#define SBL_SYS_STATUS_DEV_TYPE_MASK    (0x0000000FU)
#define SBL_SYS_STATUS_DEV_SUBTYPE_MASK (0x00000F00U)


#define SBL_SYS_STATUS_DEV_TYPE_GP      (0x3U)
#define SBL_SYS_STATUS_DEV_TYPE_TEST    (0x5U)
#define SBL_SYS_STATUS_DEV_SUBTYPE_FS   (0x00000A00U)

sblProfileInfo_t sblProfileLog[MAX_PROFILE_LOG_ENTRIES];
uint32_t sblProfileLogIndx = 0, sblProfileLogOvrFlw = 0;

uint32_t SBL_IsAuthReq(void);

void SBL_init_profile(void)
{
    CSL_armR5PmuEnableAllCntrs(1);    /* Set/clear PMCR E-bit */
    CSL_armR5PmuResetCntrs();         /* Set PMCR P-bit */
    CSL_armR5PmuResetCycleCnt();      /* Set PMCR C-bit */
    CSL_armR5PmuEnableCntr(CSL_ARM_R5_PMU_CYCLE_COUNTER_NUM, 1);     /* Set PMCNTENSET for event */
    CSL_armR5PmuClearCntrOverflowStatus(0x80000007);
}

uint32_t SBL_readPmu(void)
{
    return (CSL_armR5PmuReadCntr(0x1F));
}

#ifdef __cplusplus
#pragma FUNC_EXT_CALLED()
#else
#pragma FUNC_EXT_CALLED(SBL_LogCycleCount)
#endif
uint32_t SBL_LogCycleCount(uint8_t *fxn, uint32_t line)
{
    uint32_t indx = sblProfileLogIndx % MAX_PROFILE_LOG_ENTRIES;
    sblProfileInfo_t *sblProfileLogPtr = &(sblProfileLog[indx]);

    if (sblProfileLogIndx == MAX_PROFILE_LOG_ENTRIES)
    {
        sblProfileLogOvrFlw = 1;
    }

    sblProfileLogPtr->fxn = fxn;
    sblProfileLogPtr->line = line;
    sblProfileLogPtr->cycle_cnt = CSL_armR5PmuReadCntr(0x1F);

    sblProfileLogIndx++;

    return sblProfileLogPtr->cycle_cnt;
}

void SBL_cpuGetFreq(uint32_t coreId, uint64_t *cpu_clk_freq)
{
    *cpu_clk_freq = 250000000;
}


int32_t SBL_procBootAuthAndStart(uint32_t cert_load_addr)
{
    return CSL_PASS;

}

#ifdef __cplusplus
#pragma FUNC_EXT_CALLED()
#pragma FUNC_CANNOT_INLINE()
#else
#pragma FUNC_EXT_CALLED(SBL_printProfileLog)
#pragma FUNC_CANNOT_INLINE(SBL_printProfileLog)
#endif

void SBL_printProfileLog(void)
{
#ifdef SBL_DISPLAY_PROFILE_INFO
    uint64_t mcu_clk_freq;
    uint32_t i = 0, prev_cycle_cnt = 0, cycles_per_usec;
    uint32_t lastlogIndx;

    SBL_ADD_PROFILE_POINT;

    SBL_cpuGetFreq(SBL_CLK_ID_MCU1_CPU0, &mcu_clk_freq);
    cycles_per_usec = (mcu_clk_freq / 1000000);

    SBL_log(SBL_LOG_ERR, "MCU @ %dHz.\n", uint64_to_int32(mcu_clk_freq));
    SBL_log(SBL_LOG_ERR, "cycles per usec  = %d\n", cycles_per_usec);

    lastlogIndx = sblProfileLogIndx;

    if (sblProfileLogOvrFlw)
    {
        i = sblProfileLogIndx;
        prev_cycle_cnt = sblProfileLog[i].cycle_cnt;
        lastlogIndx = MAX_PROFILE_LOG_ENTRIES;
        SBL_log(SBL_LOG_ERR, "Detected overflow, some profile entries might be lost.\n");
        SBL_log(SBL_LOG_ERR, "Rebuild with a larger vlaue of MAX_PROFILE_LOG_ENTRIES ??\n");
    }

    while((i % MAX_PROFILE_LOG_ENTRIES) < lastlogIndx)
    {
        uint32_t cycles_to_us;

        if (sblProfileLog[i].cycle_cnt < prev_cycle_cnt)
        {
            SBL_log(SBL_LOG_MAX,"**");
        }
        else
        {
            SBL_log(SBL_LOG_MAX,"  ");
        }
        cycles_to_us = sblProfileLog[i].cycle_cnt/cycles_per_usec;
        SBL_log(SBL_LOG_ERR,"fxn:%32s\t", sblProfileLog[i].fxn);
        SBL_log(SBL_LOG_ERR,"line:%4d\t", sblProfileLog[i].line);
        SBL_log(SBL_LOG_ERR,"cycle:%10d\t", sblProfileLog[i].cycle_cnt);
        SBL_log(SBL_LOG_ERR,"timestamp:%10dus\n", cycles_to_us);
        prev_cycle_cnt = sblProfileLog[i].cycle_cnt;
        i++;
    }
#else
    if (sblProfileLogOvrFlw)
    {
    }
#endif
}

void SBL_DCacheClean(void *addr, uint32_t size)
{
    uint32_t set = 0, way = 0;
    uint32_t cacheLineSize = CSL_armR5CacheGetDcacheLineSize();
    uintptr_t firstAddr = (uintptr_t ) addr & ~(cacheLineSize -1U);
    uintptr_t lastAddr = (uintptr_t) addr + size;
    uintptr_t num_iterations = (lastAddr - firstAddr) / cacheLineSize;

    if (num_iterations < 64 * 4)
    {
        /* Invalidate by MVA */
        CSL_armR5CacheWbInv((const void *)addr, uint32_to_int32(size));
    }
    else
    {
        /* Invalidating full cache by set and way is more efficient */
        for (set = 0; set < 128; set ++)
        {
            for (way = 0; way < 4; way++)
            {
                CSL_armR5CacheCleanInvalidateDcacheSetWay(set, way);
            }
        }
    }

}

uint32_t SBL_GetCertLen(uint8_t *x509_cert_ptr)
{
    uint32_t cert_len = 0;
    uint8_t *cert_len_ptr = (uint8_t *)&cert_len;

    SBL_ADD_PROFILE_POINT;

    SBL_log(SBL_LOG_MAX,"Searching for X509 certificate ...");
    if ( *x509_cert_ptr != 0x30)
    {
        SBL_log(SBL_LOG_MAX,"not found\r\n");
        SBL_ADD_PROFILE_POINT;
        return 0;
    }

    cert_len = *(x509_cert_ptr + 1);

    /* If you need more than 2 bytes to store the cert length  */
    /* it means that the cert length is greater than 64 Kbytes */
    /* and we do not support it                                */
    if ((cert_len > 0x80) &&
        (cert_len != 0x82))
    {
        SBL_log(SBL_LOG_MAX,"size invalid\r\n");
        SBL_ADD_PROFILE_POINT;
        return 0;
    }

    SBL_log(SBL_LOG_MAX,"found @0x%x, ", x509_cert_ptr);

    if ( cert_len == 0x82)
    {
        *cert_len_ptr = *(x509_cert_ptr + 3);
        *(cert_len_ptr + 1) = *(x509_cert_ptr + 2);

        /* add current offset from start of x509 cert */
        cert_len += 3;
    }
    else
    {
        /* add current offset from start of x509 cert  */
        /* if cert len was obtained from 2nd byte i.e. */
        /* cert size is 127 bytes or less              */
        cert_len += 1;
    }

    /* cert_len now contains the offset of the last byte */
    /* of the cert from the ccert_start. To get the size */
    /* of certificate, add 1                             */
    SBL_log(SBL_LOG_MAX,"size = %d bytes\r\n", cert_len + 1);

    SBL_ADD_PROFILE_POINT;
    return cert_len + 1;
}

uint8_t *SBL_FindSeq(uint8_t *x509_cert_ptr, uint32_t x509_cert_size, uint8_t *seq_oid, uint8_t seq_len)
{
    uint8_t *x509_cert_end = x509_cert_ptr + x509_cert_size - seq_len;

    SBL_ADD_PROFILE_POINT;

    /* searching for the following byte seq in the cert */
    /* seq_id(0x30) seq_len(< 0x80) 0x06 0x09 0x2B...   */
    while (x509_cert_ptr < x509_cert_end)
    {
        if ((*x509_cert_ptr == seq_oid[0]) &&
            (*(x509_cert_ptr + 2) == seq_oid[2]) &&
            (*(x509_cert_ptr - 2) == 0x30))
        {
            if ((bcmp((const void *)x509_cert_ptr, (const void *)seq_oid, seq_len)) == 0)
            {
                SBL_ADD_PROFILE_POINT;
                /* return start boot_seq */
                return (x509_cert_ptr - 2);
            }
        }
        x509_cert_ptr++;
    }

    SBL_ADD_PROFILE_POINT;

    return NULL;
}

uint32_t SBL_GetMsgLen(uint8_t *x509_cert_ptr, uint32_t x509_cert_size)
{
    uint8_t *boot_seq_ptr;
    uint32_t msg_len = 0, boot_seq_len;
    uint8_t *msg_len_ptr = (uint8_t *)&msg_len;
    /* oid encoding of boot_seq extension - 1.3.6.1.4.1.294.1.1 */
    uint8_t boot_seq_oid[] = {0x06, 0x09, 0x2B, 0x06, 0x01, 0x04, 0x01, 0x82, 0x26, 0x01, 0x01};

    SBL_ADD_PROFILE_POINT;

    boot_seq_ptr = SBL_FindSeq(x509_cert_ptr, x509_cert_size, boot_seq_oid, sizeof(boot_seq_oid));
    SBL_log(SBL_LOG_MAX,"Found seq @ 0x%x\r\n", boot_seq_ptr);

    /* length of seq is stored in the byte after the 0x30 seq_id */
    /* length of seq is stored as offset of the last byte of seq */
    /* from current offset. Jump to the end of the boot seq as   */
    /* the length of the message  is the last field of this seq  */
    boot_seq_len = *(++boot_seq_ptr);
    boot_seq_ptr = boot_seq_ptr + boot_seq_len;

    /* The last integer in this sequence is the msg length    */
    /* integers are tagged 0x20, so search backwards for 0x20 */
    /* The msg size can be encoded in 1, 2, 3 or 4 bytes      */
    /* 0x02 0x01 0x##                                         */
    /* 0x02 0x02 0x## 0x##                                    */
    /* 0x02 0x03 0x## 0x## 0x##                               */
    /* 0x02 0x04 0x## 0x## 0x## 0x##                          */
    if ( (*(boot_seq_ptr - 5) == 0x02) &&
         (*(boot_seq_ptr - 4) == 0x04) )
    {
        /* msg length encoded in 4 bytes */
        *msg_len_ptr = *boot_seq_ptr;
        *(msg_len_ptr + 1) = *(boot_seq_ptr - 1);
        *(msg_len_ptr + 2) = *(boot_seq_ptr - 2);
        *(msg_len_ptr + 3) = *(boot_seq_ptr - 3);
    }
    else if ( (*(boot_seq_ptr - 4) == 0x02) &&
         (*(boot_seq_ptr - 3) == 0x03) )
    {
        /* msg length encoded in 3 bytes */
        *msg_len_ptr = *boot_seq_ptr;
        *(msg_len_ptr + 1) = *(boot_seq_ptr - 1);
        *(msg_len_ptr + 2) = *(boot_seq_ptr - 2);
    }
    else if ( (*(boot_seq_ptr - 3) == 0x02) &&
         (*(boot_seq_ptr - 2) == 0x02) )
    {
        /* msg length encoded in 2 bytes */
        *msg_len_ptr = *boot_seq_ptr;
        *(msg_len_ptr + 1) = *(boot_seq_ptr - 1);
    }
    else if ( (*(boot_seq_ptr - 2) == 0x02) &&
         (*(boot_seq_ptr - 1) == 0x01) )
    {
        /* msg length encoded in 1 byte */
        *msg_len_ptr = *boot_seq_ptr;
    }

    SBL_log(SBL_LOG_MAX,"image length = %d bytes\r\n", msg_len);

    SBL_ADD_PROFILE_POINT;

    return msg_len;
}

/* Keep all boot media read offsets and read sizes */
/* word aligned throughout the function in case    */
/* boot media cannot handle byte accesses          */
int32_t SBL_VerifyMulticoreImage(void **img_handle,
                                 uint32_t *ImageOffsetPtr,
                                 uint8_t *scratch_mem,
                                 uint32_t scratch_sz)
{
/* X509 Certificate parsing is TBD */
#if 0 
    uint8_t *scratch_mem_ptr;
    uint32_t cert_len = 0, img_len, cert_load_addr;
    int32_t retVal = E_FAIL, auth_retval = E_FAIL;
    uint32_t SBL_GetCertLen(uint8_t *x509_cert_ptr);
    uint32_t SBL_GetMsgLen(uint8_t *x509_cert_ptr, uint32_t x509_cert_size);

    SBL_ADD_PROFILE_POINT;

    if ((fp_readData != NULL) && (fp_seek != NULL))
    {
        uint8_t x509Header[4];
        /* Read first 4 bytes of image to */
        /* determine if it is a x509 img  */
        fp_readData(x509Header, *img_handle, 4);
        fp_seek(*img_handle, *ImageOffsetPtr);
        cert_len = SBL_GetCertLen(x509Header);
    }

    /* Check if cert size is within valid range */
    if ((cert_len > 0x100) &&
        (cert_len < 0x800) &&
        (scratch_mem !=NULL) &&
        (scratch_sz != 0))
    {
        uint8_t unaligned_bytes = cert_len % 4;
        uint8_t pad_align = 4 - unaligned_bytes;

        SBL_log(SBL_LOG_MAX,"SBL reserved memory Found: Start = @ 0x%x, Size = 0x%x\r\n", scratch_mem, scratch_sz);
        /* adjust cert load addr so that the */
        /* msg is always word aligned        */
        scratch_mem_ptr = scratch_mem + pad_align;
        SBL_log(SBL_LOG_MAX,"Copying %d bytes from app to 0x%x\r\n", cert_len + pad_align, scratch_mem_ptr);
        fp_readData(scratch_mem_ptr, *img_handle, cert_len + pad_align);
        fp_seek(*img_handle, *ImageOffsetPtr);
        cert_load_addr = (uint32_t)scratch_mem_ptr;

        img_len = SBL_GetMsgLen(scratch_mem_ptr, cert_len);
        if ((scratch_mem_ptr + cert_len + img_len)  <
            (scratch_mem + scratch_sz))
        {
            fp_seek(*img_handle, (*ImageOffsetPtr + cert_len - unaligned_bytes));
            scratch_mem_ptr += (cert_len - unaligned_bytes);

            img_len += unaligned_bytes;
            unaligned_bytes = img_len % 4;
            pad_align = 4 - unaligned_bytes;
            img_len += pad_align;

            SBL_log(SBL_LOG_MAX,"Copying %d bytes from offset 0x%x to 0x%x...", img_len, *ImageOffsetPtr + cert_len - unaligned_bytes, scratch_mem_ptr);
            fp_readData(scratch_mem_ptr, *img_handle, img_len);
            fp_seek(*img_handle, *ImageOffsetPtr);

            /* Request DMSC to authenticate the image */
            SBL_log(SBL_LOG_MAX,"Cert @ 0x%x ...", cert_load_addr);

            SBL_ADD_PROFILE_POINT;

            retVal = auth_retval = SBL_procBootAuthAndStart(cert_load_addr);
            if (retVal == CSL_PASS)
            {
                SBL_log(SBL_LOG_MAX,"Verify Passed...");
            }
            else
            {
                if (SBL_IsAuthReq() == SBL_NEVER_AUTH_APP)
                {
                    SBL_log(SBL_LOG_MAX,"Ignored on GP...");
                }
                else
                {
                    SBL_log(SBL_LOG_ERR,"Verify Failed!!\n\r");
                    SBL_log(SBL_LOG_ERR,"Boot Halted!!\n\r");
                    SblErrLoop(__FILE__, __LINE__);
                }
            }

            SBL_ADD_PROFILE_POINT;

            /* Image is loaded. RPRC parsing no longer */
            /* neeeds to access the boot media. Update */
            /* caller with image load address          */
            SBL_SetMulticoreImageImgReadfxn((void *)SBL_ReadMem, (void *)SBL_SeekMem);
            *img_handle = (void *)(cert_load_addr+cert_len);
            *ImageOffsetPtr = 0;
            SBL_log(SBL_LOG_MAX,"done\r\n", img_len, scratch_mem_ptr);
            retVal = E_PASS;
        }
    }

    if ((auth_retval != CSL_PASS) &&
        (SBL_IsAuthReq() != SBL_NEVER_AUTH_APP))
    {
        SBL_log(SBL_LOG_ERR,"Is app signed correctly??\n\r");
        SBL_log(SBL_LOG_ERR,"App verification fails!! Boot Halted!!\n\r");
        SblErrLoop(__FILE__, __LINE__);
    }
    SBL_ADD_PROFILE_POINT;

    return retVal;
#else
    return E_PASS;
#endif
}


/**********************************************************************
 ******************* SoC Specific Initilization ***********************
 **********************************************************************/
uint32_t SBL_IsAuthReq(void)
{
    uint32_t retVal = SBL_ALWAYS_AUTH_APP;
    uint32_t dev_type;
    uint32_t dev_subtype;

    SBL_ADD_PROFILE_POINT;

    //TODO: Reduce EFUSE to determine device type
//    dev_type = CSL_REG32_RD(SBL_SYS_STATUS_REG) & SBL_SYS_STATUS_DEV_TYPE_MASK;
//    dev_subtype = CSL_REG32_RD(SBL_SYS_STATUS_REG) & SBL_SYS_STATUS_DEV_SUBTYPE_MASK;

    dev_type = SBL_SYS_STATUS_DEV_TYPE_GP;
    dev_subtype = SBL_SYS_STATUS_DEV_SUBTYPE_FS;
    /* No auth possible, if valid SMPK/BMPK is not present */
    if ((dev_subtype == SBL_SYS_STATUS_DEV_SUBTYPE_FS) ||
        (dev_type == SBL_SYS_STATUS_DEV_TYPE_GP) ||
        (dev_type == SBL_SYS_STATUS_DEV_TYPE_TEST))
    {
        retVal = SBL_NEVER_AUTH_APP;
    }

    SBL_ADD_PROFILE_POINT;

    return retVal;
}

void SBL_UARTAInitPwrClk(void)
{
    Rcm_Return retVal;

    /* Setup UART clock source */
    retVal = SBL_RcmSetPeripheralClock(Rcm_PeripheralId_MSS_SCIA, Rcm_PeripheralClockSource_SYS_CLK, SBL_MSS_SCIA_FREQ_HZ);
    DebugP_assert(retVal == Rcm_Return_SUCCESS);
}




void SBL_SocEarlyInit(void)
{
    //SetupLvCmosDriveStrength();


}

void SBL_SocLateInit(void)
{
    //EnableThermalMaxTempAlert();

    //SBL_ADD_PROFILE_POINT;

    //SetupPmicAvs(SBL_OPP_NOM);

    //SBL_ADD_PROFILE_POINT;

}



/**
 * \brief clock Initialization function
 *
 * Enables different power domains and peripheral clocks of the SoC.
 * Some of the power domains and peripherals will be OFF by default.
 * Enabling the power domains is mandatory before accessing using
 * board interfaces connected to those peripherals.
 *
 * \return  BOARD_SOK              - Clock initialization sucessful.
 *          BOARD_INIT_CLOCK_FAIL  - Clock initialization failed.
 *
 */
void SBL_moduleClockInit(void)
{
    Rcm_Return retVal;

    /* Generate 96 MHz CSIRX Control Clock */
    //HW_WR_REG32(CSL_MSS_TOPRCM_U_BASE+CSIRX_CLK_SRC_SEL, 0x222);
    //HW_WR_REG32(CSL_MSS_TOPRCM_U_BASE+CSIRX_DIV_VAL, 0x000);
    retVal = SBL_RcmSetPeripheralClock(Rcm_PeripheralId_CSIRX, Rcm_PeripheralClockSource_DPLL_PER_HSDIV0_CLKOUT2, SBL_CSIRX_FREQ_HZ);
    DebugP_assert(retVal == Rcm_Return_SUCCESS);

    //HW_WR_REG32(CSL_MSS_RCM_U_BASE+MSS_MCANA_CLK_DIV_VAL, 0x444);  //MSS_MCANA_CLK_DIV_VAL_CLKDIVR   = 0x444; 400/5
    //HW_WR_REG32(CSL_MSS_RCM_U_BASE+MSS_MCANA_CLK_SRC_SEL, 0x444);  //MSS_MCANA_CLK_SRC_SEL_CLKSRCSEL = 0x222;
    retVal = SBL_RcmSetPeripheralClock(Rcm_PeripheralId_MSS_MCANA, Rcm_PeripheralClockSource_DPLL_CORE_HSDIV0_CLKOUT2, SBL_MCANA_FREQ_HZ);
    DebugP_assert(retVal == Rcm_Return_SUCCESS);

    //HW_WR_REG32(CSL_MSS_RCM_U_BASE+MSS_MCANB_CLK_DIV_VAL, 0x444);  //MSS_MCANB_CLK_DIV_VAL_CLKDIVR   = 0x444; 
    //HW_WR_REG32(CSL_MSS_RCM_U_BASE+MSS_MCANB_CLK_SRC_SEL, 0x444);  //MSS_MCANB_CLK_SRC_SEL_CLKSRCSEL = 0x222;
    retVal = SBL_RcmSetPeripheralClock(Rcm_PeripheralId_MSS_MCANB, Rcm_PeripheralClockSource_DPLL_CORE_HSDIV0_CLKOUT2, SBL_MCANB_FREQ_HZ);
    DebugP_assert(retVal == Rcm_Return_SUCCESS);

    //HW_WR_REG32(CSL_MSS_RCM_U_BASE+MSS_RTIA_CLK_DIV_VAL, 0x000);  //MSS_RTIA_CLK_DIV_VAL_CLKDIVR   = 0x444; 200/1
    //HW_WR_REG32(CSL_MSS_RCM_U_BASE+MSS_RTIA_CLK_SRC_SEL, 0x222);  //MSS_RTIA_CLK_SRC_SEL_CLKSRCSEL = 0x222;
    retVal = SBL_RcmSetPeripheralClock(Rcm_PeripheralId_MSS_RTIA, Rcm_PeripheralClockSource_SYS_CLK, SBL_MSS_RTIA_FREQ_HZ);
    DebugP_assert(retVal == Rcm_Return_SUCCESS);

    //HW_WR_REG32(CSL_MSS_RCM_U_BASE+MSS_RTIB_CLK_DIV_VAL, 0x000);  //MSS_RTIB_CLK_DIV_VAL_CLKDIVR   = 0x444;
    //HW_WR_REG32(CSL_MSS_RCM_U_BASE+MSS_RTIB_CLK_SRC_SEL, 0x222);  //MSS_RTIB_CLK_SRC_SEL_CLKSRCSEL = 0x222;
    retVal = SBL_RcmSetPeripheralClock(Rcm_PeripheralId_MSS_RTIB, Rcm_PeripheralClockSource_SYS_CLK, SBL_MSS_RTIB_FREQ_HZ);
    DebugP_assert(retVal == Rcm_Return_SUCCESS);

    //HW_WR_REG32(CSL_MSS_RCM_U_BASE+MSS_RTIC_CLK_DIV_VAL, 0x000);  //MSS_RTIC_CLK_DIV_VAL_CLKDIVR   = 0x444;
    //HW_WR_REG32(CSL_MSS_RCM_U_BASE+MSS_RTIC_CLK_SRC_SEL, 0x222);  //MSS_RTIC_CLK_SRC_SEL_CLKSRCSEL = 0x222;
    retVal = SBL_RcmSetPeripheralClock(Rcm_PeripheralId_MSS_RTIC, Rcm_PeripheralClockSource_SYS_CLK, SBL_MSS_RTIC_FREQ_HZ);
    DebugP_assert(retVal == Rcm_Return_SUCCESS);

    //HW_WR_REG32(CSL_MSS_RCM_U_BASE+MSS_WDT_CLK_DIV_VAL, 0x000);  //MSS_WDT_CLK_DIV_VAL_CLKDIVR   = 0x444;
    //HW_WR_REG32(CSL_MSS_RCM_U_BASE+MSS_WDT_CLK_SRC_SEL, 0x222);  //MSS_WDT_CLK_SRC_SEL_CLKSRCSEL = 0x222;
    retVal = SBL_RcmSetPeripheralClock(Rcm_PeripheralId_MSS_WDT, Rcm_PeripheralClockSource_SYS_CLK, SBL_MSS_WDT_FREQ_HZ);
    DebugP_assert(retVal == Rcm_Return_SUCCESS);

    //HW_WR_REG32(CSL_MSS_RCM_U_BASE+MSS_QSPI_CLK_DIV_VAL, 0x444);  //MSS_QSPI_CLK_DIV_VAL_CLKDIVR   = 0x444; 400/5
    //HW_WR_REG32(CSL_MSS_RCM_U_BASE+MSS_QSPI_CLK_SRC_SEL, 0x444);  //MSS_QSPI_CLK_SRC_SEL_CLKSRCSEL = 0x444;
    retVal = SBL_RcmSetPeripheralClock(Rcm_PeripheralId_MSS_QSPI, Rcm_PeripheralClockSource_DPLL_CORE_HSDIV0_CLKOUT2, SBL_QSPI_FREQ_HZ);
    DebugP_assert(retVal == Rcm_Return_SUCCESS);

    //HW_WR_REG32(CSL_MSS_RCM_U_BASE+MSS_SPIA_CLK_DIV_VAL, 0x000);  //MSS_SPIA_CLK_DIV_VAL_CLKDIVR = 0x000;   200/1
    //HW_WR_REG32(CSL_MSS_RCM_U_BASE+MSS_SPIA_CLK_SRC_SEL, 0x222);  //MSS_SPIA_CLK_SRC_SEL_CLKSRCSEL = 0x222; 
    retVal = SBL_RcmSetPeripheralClock(Rcm_PeripheralId_MSS_SPIA, Rcm_PeripheralClockSource_SYS_CLK, SBL_MSS_SPIA_FREQ_HZ);
    DebugP_assert(retVal == Rcm_Return_SUCCESS);

    //HW_WR_REG32(CSL_MSS_RCM_U_BASE+MSS_SPIB_CLK_SRC_SEL, 0x222);  //MSS_SPIB_CLK_SRC_SEL_CLKSRCSEL = 0x222;
    //HW_WR_REG32(CSL_MSS_RCM_U_BASE+MSS_SPIB_CLK_DIV_VAL, 0x000);  //MSS_SPIB_CLK_DIV_VAL_CLKDIVR = 0x000;
    retVal = SBL_RcmSetPeripheralClock(Rcm_PeripheralId_MSS_SPIB, Rcm_PeripheralClockSource_SYS_CLK, SBL_MSS_SPIB_FREQ_HZ);
    DebugP_assert(retVal == Rcm_Return_SUCCESS);

    //HW_WR_REG32(CSL_MSS_RCM_U_BASE+MSS_I2C_CLK_DIV_VAL, 0x000);  //MSS_I2C_CLK_DIV_VAL_CLKDIVR   = 0x000; 200/1
    //HW_WR_REG32(CSL_MSS_RCM_U_BASE+MSS_I2C_CLK_SRC_SEL, 0x222);  //MSS_I2C_CLK_SRC_SEL_CLKSRCSEL = 0x222;
    retVal = SBL_RcmSetPeripheralClock(Rcm_PeripheralId_MSS_I2C, Rcm_PeripheralClockSource_SYS_CLK, SBL_MSS_I2C_FREQ_HZ);
    DebugP_assert(retVal == Rcm_Return_SUCCESS);

    //HW_WR_REG32(CSL_MSS_RCM_U_BASE+MSS_SCIA_CLK_DIV_VAL, 0x000);  //MSS_SCIA_CLK_DIV_VAL_CLKDIVR = 0x000; 200/1
    //HW_WR_REG32(CSL_MSS_RCM_U_BASE+MSS_SCIA_CLK_SRC_SEL, 0x222);  //MSS_SCIA_CLK_SRC_SEL_CLKSRCSEL = 0x222;
    retVal = SBL_RcmSetPeripheralClock(Rcm_PeripheralId_MSS_SCIA, Rcm_PeripheralClockSource_SYS_CLK, SBL_MSS_SCIA_FREQ_HZ);
    DebugP_assert(retVal == Rcm_Return_SUCCESS);

    //HW_WR_REG32(CSL_MSS_RCM_U_BASE+MSS_SCIB_CLK_DIV_VAL, 0x000);  //MSS_SCIB_CLK_DIV_VAL_CLKDIVR = 0x000;
    //HW_WR_REG32(CSL_MSS_RCM_U_BASE+MSS_SCIB_CLK_SRC_SEL, 0x222);  //MSS_SCIB_CLK_SRC_SEL_CLKSRCSEL = 0x222;
    retVal = SBL_RcmSetPeripheralClock(Rcm_PeripheralId_MSS_SCIB, Rcm_PeripheralClockSource_SYS_CLK, SBL_MSS_SCIB_FREQ_HZ);
    DebugP_assert(retVal == Rcm_Return_SUCCESS);

    //HW_WR_REG32(CSL_MSS_RCM_U_BASE+MSS_CPSW_CLK_DIV_VAL, 0x111);  //MSS_CPSW_CLK_DIV_VAL_CLKDIVR   = 0x000; 400/2
    //HW_WR_REG32(CSL_MSS_RCM_U_BASE+MSS_CPSW_CLK_SRC_SEL, 0x444);  //MSS_CPSW_CLK_SRC_SEL_CLKSRCSEL = 0x222;
    retVal = SBL_RcmSetPeripheralClock(Rcm_PeripheralId_MSS_CPSW, Rcm_PeripheralClockSource_DPLL_CORE_HSDIV0_CLKOUT2, SBL_CPSW_FREQ_HZ);
    DebugP_assert(retVal == Rcm_Return_SUCCESS);

    //HW_WR_REG32(CSL_MSS_RCM_U_BASE+MSS_CPTS_CLK_DIV_VAL, 0x111);  //MSS_CPTS_CLK_DIV_VAL_CLKDIVR   = 0x000; 400/2
    //HW_WR_REG32(CSL_MSS_RCM_U_BASE+MSS_CPTS_CLK_SRC_SEL, 0x444);  //MSS_CPTS_CLK_SRC_SEL_CLKSRCSEL = 0x222;
    retVal = SBL_RcmSetPeripheralClock(Rcm_PeripheralId_MSS_CPTS, Rcm_PeripheralClockSource_DPLL_CORE_HSDIV0_CLKOUT2, SBL_CPTS_FREQ_HZ);
    DebugP_assert(retVal == Rcm_Return_SUCCESS);

    /* add more modules, add DSP and RCSS */
    //HW_WR_REG32(CSL_DSS_RCM_U_BASE+DSS_RTIA_CLK_DIV_VAL, 0x000);  //DSS_RTIA_CLK_DIV_VAL_CLKDIVR   = 0x444; 200/1
    //HW_WR_REG32(CSL_DSS_RCM_U_BASE+DSS_RTIA_CLK_SRC_SEL, 0x222);  //DSS_RTIA_CLK_SRC_SEL_CLKSRCSEL = 0x222;
    retVal = SBL_RcmSetPeripheralClock(Rcm_PeripheralId_DSS_RTIA, Rcm_PeripheralClockSource_SYS_CLK, SBL_DSS_RTIA_FREQ_HZ);
    DebugP_assert(retVal == Rcm_Return_SUCCESS);

    //HW_WR_REG32(CSL_DSS_RCM_U_BASE+DSS_RTIB_CLK_DIV_VAL, 0x000);  //DSS_RTIB_CLK_DIV_VAL_CLKDIVR   = 0x444;
    //HW_WR_REG32(CSL_DSS_RCM_U_BASE+DSS_RTIB_CLK_SRC_SEL, 0x222);  //DSS_RTIB_CLK_SRC_SEL_CLKSRCSEL = 0x222;
    retVal = SBL_RcmSetPeripheralClock(Rcm_PeripheralId_DSS_RTIB, Rcm_PeripheralClockSource_SYS_CLK, SBL_DSS_RTIB_FREQ_HZ);
    DebugP_assert(retVal == Rcm_Return_SUCCESS);

    //HW_WR_REG32(CSL_DSS_RCM_U_BASE+DSS_WDT_CLK_DIV_VAL, 0x000);  //DSS_WDT_CLK_DIV_VAL_CLKDIVR   = 0x444;
    //HW_WR_REG32(CSL_DSS_RCM_U_BASE+DSS_WDT_CLK_SRC_SEL, 0x222);  //DSS_WDT_CLK_SRC_SEL_CLKSRCSEL = 0x222;
    retVal = SBL_RcmSetPeripheralClock(Rcm_PeripheralId_DSS_WDT, Rcm_PeripheralClockSource_SYS_CLK, SBL_DSS_WDT_FREQ_HZ);
    DebugP_assert(retVal == Rcm_Return_SUCCESS);

    //HW_WR_REG32(CSL_DSS_RCM_U_BASE+DSS_SCIA_CLK_DIV_VAL, 0x000);  //DSS_SCIA_CLK_DIV_VAL_CLKDIVR = 0x000; 200/1
    //HW_WR_REG32(CSL_DSS_RCM_U_BASE+DSS_SCIA_CLK_SRC_SEL, 0x222);  //DSS_SCIA_CLK_SRC_SEL_CLKSRCSEL = 0x222;
    retVal = SBL_RcmSetPeripheralClock(Rcm_PeripheralId_DSS_SCIA, Rcm_PeripheralClockSource_SYS_CLK, SBL_DSS_SCIA_FREQ_HZ);
    DebugP_assert(retVal == Rcm_Return_SUCCESS);

    //HW_WR_REG32(CSL_RCSS_RCM_U_BASE+RCSS_SPIA_CLK_DIV_VAL, 0x000);  //RCSS_SPIA_CLK_DIV_VAL_CLKDIVR = 0x000; 200/1
    //HW_WR_REG32(CSL_RCSS_RCM_U_BASE+RCSS_SPIA_CLK_SRC_SEL, 0x222);  //RCSS_SPIA_CLK_SRC_SEL_CLKSRCSEL = 0x222;
    retVal = SBL_RcmSetPeripheralClock(Rcm_PeripheralId_RCSS_SCIA, Rcm_PeripheralClockSource_SYS_CLK, SBL_RCSS_SCIA_FREQ_HZ);
    DebugP_assert(retVal == Rcm_Return_SUCCESS);

    //HW_WR_REG32(CSL_RCSS_RCM_U_BASE+RCSS_SPIA_CLK_DIV_VAL, 0x000);  //RCSS_SPIA_CLK_DIV_VAL_CLKDIVR = 0x000; 200/1
    //HW_WR_REG32(CSL_RCSS_RCM_U_BASE+RCSS_SPIA_CLK_SRC_SEL, 0x222);  //RCSS_SPIA_CLK_SRC_SEL_CLKSRCSEL = 0x222;
    retVal = SBL_RcmSetPeripheralClock(Rcm_PeripheralId_RCSS_SPIA, Rcm_PeripheralClockSource_SYS_CLK, SBL_RCSS_SPIA_FREQ_HZ);
    DebugP_assert(retVal == Rcm_Return_SUCCESS);

    //HW_WR_REG32(CSL_RCSS_RCM_U_BASE+RCSS_SPIB_CLK_DIV_VAL, 0x000);  //RCSS_SPIB_CLK_DIV_VAL_CLKDIVR = 0x000; 200/1
    //HW_WR_REG32(CSL_RCSS_RCM_U_BASE+RCSS_SPIB_CLK_SRC_SEL, 0x222);  //RCSS_SPIB_CLK_SRC_SEL_CLKSRCSEL = 0x222;
    retVal = SBL_RcmSetPeripheralClock(Rcm_PeripheralId_RCSS_SPIB, Rcm_PeripheralClockSource_SYS_CLK, SBL_RCSS_SPIB_FREQ_HZ);
    DebugP_assert(retVal == Rcm_Return_SUCCESS);

    //HW_WR_REG32(CSL_RCSS_RCM_U_BASE+RCSS_I2CA_CLK_DIV_VAL, 0x000);  //RCSS_I2CA_CLK_DIV_VAL_CLKDIVR = 0x000; 200/1
    //HW_WR_REG32(CSL_RCSS_RCM_U_BASE+RCSS_I2CA_CLK_SRC_SEL, 0x222);  //RCSS_I2CA_CLK_SRC_SEL_CLKSRCSEL = 0x222;
    retVal = SBL_RcmSetPeripheralClock(Rcm_PeripheralId_RCSS_I2CA, Rcm_PeripheralClockSource_SYS_CLK, SBL_RCSS_I2CA_FREQ_HZ);
    DebugP_assert(retVal == Rcm_Return_SUCCESS);

    //HW_WR_REG32(CSL_RCSS_RCM_U_BASE+RCSS_I2CB_CLK_DIV_VAL, 0x000);  //RCSS_I2CB_CLK_DIV_VAL_CLKDIVR = 0x000; 200/1
    //HW_WR_REG32(CSL_RCSS_RCM_U_BASE+RCSS_I2CB_CLK_SRC_SEL, 0x222);  //RCSS_I2CB_CLK_SRC_SEL_CLKSRCSEL = 0x222;
    retVal = SBL_RcmSetPeripheralClock(Rcm_PeripheralId_RCSS_I2CB, Rcm_PeripheralClockSource_SYS_CLK, SBL_RCSS_I2CB_FREQ_HZ);
    DebugP_assert(retVal == Rcm_Return_SUCCESS);

    //HW_WR_REG32(CSL_RCSS_RCM_U_BASE+RCSS_ATL_CLK_DIV_VAL, 0x111);   //RCSS_ATL_CLK_DIV_VAL_CLKDIVR = 0x000; 240/2
    //HW_WR_REG32(CSL_RCSS_RCM_U_BASE+RCSS_ATL_CLK_SRC_SEL, 0x666);   //RCSS_ATL_CLK_SRC_SEL_CLKSRCSEL = 0x222;
    retVal = SBL_RcmSetPeripheralClock(Rcm_PeripheralId_RCSS_ATL, Rcm_PeripheralClockSource_DPLL_PER_HSDIV0_CLKOUT3, SBL_RCSS_ATL_FREQ_HZ);
    DebugP_assert(retVal == Rcm_Return_SUCCESS);

    //HW_WR_REG32(CSL_RCSS_RCM_U_BASE+RCSS_MCASPA_AUX_CLK_DIV_VAL, 0x111);  //RCSS_MCASPA_AUX_CLK_DIV_VAL_CLKDIVR = 0x111; 192/2
    //HW_WR_REG32(CSL_RCSS_RCM_U_BASE+RCSS_MCASPA_AUX_CLK_SRC_SEL, 0x222);  //RCSS_MCASPA_AUX_CLK_SRC_SEL_CLKSRCSEL = 0x222;
    retVal = SBL_RcmSetPeripheralClock(Rcm_PeripheralId_RCSS_MCASPA_AUX, Rcm_PeripheralClockSource_DPLL_PER_HSDIV0_CLKOUT1 , SBL_RCSS_MCASPA_AUX_FREQ_HZ);
    DebugP_assert(retVal == Rcm_Return_SUCCESS);

    //HW_WR_REG32(CSL_RCSS_RCM_U_BASE+RCSS_MCASPB_AUX_CLK_DIV_VAL, 0x111);  //RCSS_MCASPB_AUX_CLK_DIV_VAL_CLKDIVR = 0x111; 192/2
    //HW_WR_REG32(CSL_RCSS_RCM_U_BASE+RCSS_MCASPB_AUX_CLK_SRC_SEL, 0x222);  //RCSS_MCASPB_AUX_CLK_SRC_SEL_CLKSRCSEL = 0x222;
    retVal = SBL_RcmSetPeripheralClock(Rcm_PeripheralId_RCSS_MCASPB_AUX, Rcm_PeripheralClockSource_DPLL_PER_HSDIV0_CLKOUT1 , SBL_RCSS_MCASPB_AUX_FREQ_HZ);
    DebugP_assert(retVal == Rcm_Return_SUCCESS);

    //HW_WR_REG32(CSL_RCSS_RCM_U_BASE+RCSS_MCASPC_AUX_CLK_DIV_VAL, 0x111);  //RCSS_MCASPB_AUX_CLK_DIV_VAL_CLKDIVR = 0x111; 192/2
    //HW_WR_REG32(CSL_RCSS_RCM_U_BASE+RCSS_MCASPC_AUX_CLK_SRC_SEL, 0x222);  //RCSS_MCASPB_AUX_CLK_SRC_SEL_CLKSRCSEL = 0x222;
    retVal = SBL_RcmSetPeripheralClock(Rcm_PeripheralId_RCSS_MCASPC_AUX, Rcm_PeripheralClockSource_DPLL_PER_HSDIV0_CLKOUT1 , SBL_RCSS_MCASPC_AUX_FREQ_HZ);
    DebugP_assert(retVal == Rcm_Return_SUCCESS);

    /* TODO: Rcm Library support for Rcm_PeripheralClockSource_RCSS_ATL_CLKOUT0 clock source */
    //HW_WR_REG32(CSL_RCSS_RCM_U_BASE+RCSS_MCASPA_REF0_CLK_DIV_VAL, 0x111);  //RCSS_MCASPA_REF0_CLK_DIV_VAL_CLKDIVR = 0x111; 200/2
    //HW_WR_REG32(CSL_RCSS_RCM_U_BASE+RCSS_MCASPA_REF0_CLK_SRC_SEL, 0x111);  //RCSS_MCASPA_REF0_CLK_SRC_SEL_CLKSRCSEL = 0x111;
    //SBL_RcmSetPeripheralClock(Rcm_PeripheralId_RCSS_MCASPA_REF0, Rcm_PeripheralClockSource_RCSS_ATL_CLKOUT0 , SBL_RCSS_MCASPA_REF0_FREQ_HZ);

    //HW_WR_REG32(CSL_RCSS_RCM_U_BASE+RCSS_MCASPB_REF0_CLK_DIV_VAL, 0x111);  //RCSS_MCASPB_REF0_CLK_DIV_VAL_CLKDIVR = 0x111; 200/2
    //HW_WR_REG32(CSL_RCSS_RCM_U_BASE+RCSS_MCASPB_REF0_CLK_SRC_SEL, 0x111);  //RCSS_MCASPB_REF0_CLK_SRC_SEL_CLKSRCSEL = 0x111;

    //HW_WR_REG32(CSL_RCSS_RCM_U_BASE+RCSS_MCASPC_REF0_CLK_DIV_VAL, 0x111);  //RCSS_MCASPB_REF0_CLK_DIV_VAL_CLKDIVR = 0x111; 200/2
    //HW_WR_REG32(CSL_RCSS_RCM_U_BASE+RCSS_MCASPC_REF0_CLK_SRC_SEL, 0x111);  //RCSS_MCASPB_REF0_CLK_SRC_SEL_CLKSRCSEL = 0x111;

    //HW_WR_REG32(CSL_RCSS_RCM_U_BASE+RCSS_MCASPA_REF1_CLK_DIV_VAL, 0x111);  //RCSS_MCASPA_REF1_CLK_DIV_VAL_CLKDIVR = 0x111; 200/2
    //HW_WR_REG32(CSL_RCSS_RCM_U_BASE+RCSS_MCASPA_REF1_CLK_SRC_SEL, 0x111);  //RCSS_MCASPA_REF1_CLK_SRC_SEL_CLKSRCSEL = 0x111;

    //HW_WR_REG32(CSL_RCSS_RCM_U_BASE+RCSS_MCASPB_REF1_CLK_DIV_VAL, 0x111);  //RCSS_MCASPB_REF1_CLK_DIV_VAL_CLKDIVR = 0x111; 200/2
    //HW_WR_REG32(CSL_RCSS_RCM_U_BASE+RCSS_MCASPB_REF1_CLK_SRC_SEL, 0x111);  //RCSS_MCASPB_REF1_CLK_SRC_SEL_CLKSRCSEL = 0x111;

    //HW_WR_REG32(CSL_RCSS_RCM_U_BASE+RCSS_MCASPC_REF1_CLK_DIV_VAL, 0x111);  //RCSS_MCASPB_REF1_CLK_DIV_VAL_CLKDIVR = 0x111; 200/2
    //HW_WR_REG32(CSL_RCSS_RCM_U_BASE+RCSS_MCASPC_REF1_CLK_SRC_SEL, 0x111);  //RCSS_MCASPB_REF1_CLK_SRC_SEL_CLKSRCSEL = 0x111;

}

#ifdef __cplusplus
#pragma RETAIN
#pragma LOCATION(SBL_SCRATCH_MEM_START);
#pragma DATA_SECTION(".sblScratch");
#else
#pragma RETAIN(gSblScratch);
#pragma LOCATION(gSblScratch,SBL_SCRATCH_MEM_START);
#pragma DATA_SECTION(gSblScratch, ".sblScratch");
#endif
uint8_t gSblScratch[SBL_SCRATCH_MEM_SIZE];

