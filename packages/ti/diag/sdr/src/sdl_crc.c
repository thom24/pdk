/*
 * SDL CRC
 *
 * Software Diagnostics Library module for CRC
 *
 *  Copyright (c) Texas Instruments Incorporated 2019-2020
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
 *
 */

#include <ti/csl/soc.h>
#include <ti/csl/hw_types.h>

#include <string.h>
#include <stdint.h>
#include <sdl_common.h>
#include <sdl_crc.h>

/* Local defines */

/**
 * @brief CRC-64 polynomial: x^64 + x^4 + x^3 + x + 1
 */
#define SDL_CRC_POLYNOMIAL   (0x1BULL)

/**
 * @brief This structure defines initial configuration of CRC
 */
typedef struct SDL_CrcInitMCB_s
{
   uint32_t baseAddr;
   /**< CRC instance base address */
   uint32_t wdPreload;
   /**< Watch dog preload time */
   uint32_t blkCompPreload;
   /**< block complete preload time */
}  SDL_CrcInitMCB_t;

/* Global objects */
static SDL_CrcInitMCB_t sdlCrcInitMCB;

/* Local functions */
static uint32_t SDL_CRC_getPatternSize(SDL_CRC_dataBitSize dataBitSize);
static uint32_t SDL_CRC_getPatternSize(SDL_CRC_dataBitSize dataBitSize)
{
    uint32_t patSize;

    if (dataBitSize == SDL_CRC_DATA_8_BIT)
    {
        patSize = (uint32_t)(1U);
    }
    else if (dataBitSize == SDL_CRC_DATA_16_BIT)
    {
        patSize = (uint32_t)(2U);
    }
    else
    {
        patSize = (uint32_t)(4U);
    }
    return (patSize);
}

/* Global functions */

void SDL_CRC_init (void)
{
    sdlCrcInitMCB.baseAddr       = CSL_MCU_NAVSS0_MCRC_BASE;
    sdlCrcInitMCB.wdPreload      = (uint32_t)(0U);
    sdlCrcInitMCB.blkCompPreload = (uint32_t)(0U);
}

SDL_Result SDL_CRC_selftest (uint32_t crcChannel, const SDL_CRC_dataConfig_t *pDataConfig)
{
    SDL_Result            result = SDL_FAIL;
    int32_t               status;
    crcSignatureRegAddr_t sigRegAddr;
    uint32_t              patSize;
    uint32_t              secCnt;
    crcSignature_t        sectSignVal;
    uint32_t              i;

    if ((crcChannel < CRC_CHANNEL_1)            ||
        (crcChannel > CRC_CHANNEL_4)            ||
        (pDataConfig == (NULL_PTR))             ||
        (pDataConfig->pCRCData == (NULL_PTR)))
    {
        result = SDL_BADARGS;
    }
    else
    {
        /* Initialize the CRC channel */
        status = CRCInitialize(sdlCrcInitMCB.baseAddr,
                               crcChannel,
                               sdlCrcInitMCB.wdPreload,
                               sdlCrcInitMCB.blkCompPreload);

        if (status == CSL_PASS)
        {
            /* Get the CRC signature register address for the CRC channel */
            status = CRCGetPSASigRegAddr(sdlCrcInitMCB.baseAddr,
                                         crcChannel,
                                         &sigRegAddr);
        }

        if (status == CSL_PASS)
        {
            /* Reset the channel to clear the CRC signature to zero */
            status = CRCChannelReset(sdlCrcInitMCB.baseAddr,
                                     crcChannel);
        }

        if (status == CSL_PASS)
        {
            /* Configure the operation mode and pattern/sector count for the channel */
            patSize = SDL_CRC_getPatternSize(pDataConfig->dataBitSize);
            secCnt  = (uint32_t)(1U);
            status = CRCConfigure(sdlCrcInitMCB.baseAddr,
                                  crcChannel,
                                  pDataConfig->size / patSize,
                                  secCnt,
                                  pDataConfig->crcOpMode);
        }

        if (status == CSL_PASS)
        {
            /* Write CRC data */
            if (pDataConfig->dataBitSize == SDL_CRC_DATA_8_BIT)
            {
                uint8_t *pData = (uint8_t *)(pDataConfig->pCRCData);
                for (i = (uint32_t)(0U); i < pDataConfig->size; i++)
                {
                    HW_WR_REG8(sigRegAddr.regL, pData[i]);
                }
            }
            else if (pDataConfig->dataBitSize == SDL_CRC_DATA_16_BIT)
            {
                uint16_t *pData = (uint16_t *)(pDataConfig->pCRCData);
                for (i = (uint32_t)(0U); i < (pDataConfig->size / 2U); i++)
                {
                    HW_WR_REG16(sigRegAddr.regL, pData[i]);
                }
            }
            else
            {
                uint32_t *pData = (uint32_t *)(pDataConfig->pCRCData);
                for (i = (uint32_t)(0U); i < (pDataConfig->size / 4U); i++)
                {
                    HW_WR_REG32(sigRegAddr.regL, pData[i]);
                }
            }

            /* Get the CRC signature value */
            status = CRCGetPSASectorSig(sdlCrcInitMCB.baseAddr,
                                        crcChannel,
                                        &sectSignVal);
        }

        if (status == CSL_PASS)
        {
            /*
             * Check if the generated CRC signature value
             * matches with the reference signaure vaule
             */
            if ((sectSignVal.regH == pDataConfig->refCRCValueMSW) &&
                (sectSignVal.regL == pDataConfig->refCRCValueLSW))
            {
                result = SDL_PASS;
            }
        }
    }

    return (result);
}
