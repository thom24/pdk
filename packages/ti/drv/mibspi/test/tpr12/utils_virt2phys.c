/*
 *   @file  utils_virt2phys.c
 *
 *   @brief
 *      Utility to convert CPU address to System Address
 *
 *  \par
 *  NOTE:
 *      (C) Copyright 2020 Texas Instruments, Inc.
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

/**************************************************************************
 *************************** Include Files ********************************
 **************************************************************************/
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>


/**************************************************************************
 *************************** Local Definitions *********************************
 **************************************************************************/
typedef enum Utils_mmapMemId_types
{
    UTILSMMAP_MEMID_CR5A_TCMA_RAM,
    UTILSMMAP_MEMID_CR5A_TCMB_RAM,
    UTILSMMAP_MEMID_DSS_L2,
    UTILSMMAP_MEMID_DSS_L1D,
    UTILSMMAP_MEMID_MSS_L2,
    UTILSMMAP_MEMID_UNKNOWN
}Utils_mmapMemId_e;

typedef struct Utils_mmapSegmentEntry_s
{
    uint32_t baseAddress;
    uint32_t length;
    Utils_mmapMemId_e memId;
} Utils_mmapSegmentEntry;

#ifdef BUILD_MCU
static const Utils_mmapSegmentEntry gMemSegmentTblVirtCR5[] = 
{
    {
        .baseAddress = 0x00000000,
        .length      = 0x00008000,
        .memId       = UTILSMMAP_MEMID_CR5A_TCMA_RAM,
    },
    {
        .baseAddress = 0x00080000,
        .length      = (16 * 1024U),
        .memId       = UTILSMMAP_MEMID_CR5A_TCMB_RAM,
    },
    {
        .baseAddress = 0x10200000,
        .length      = (960 * 1024U),
        .memId       = UTILSMMAP_MEMID_MSS_L2,
    },
};
#endif

#ifdef BUILD_DSP
static const Utils_mmapSegmentEntry gMemSegmentTblVirtDSP[] = 
{
    {
        .baseAddress = 0x00F00000,
        .length      = (32 * 1024),
        .memId       = UTILSMMAP_MEMID_DSS_L1D,
    },
    {
        .baseAddress = 0x00800000,
        .length      = (384 * 1024U),
        .memId       = UTILSMMAP_MEMID_DSS_L2,
    },
};
#endif


static const uint32_t gSysMemAddress[] = 
{
    [UTILSMMAP_MEMID_CR5A_TCMA_RAM] = 0xC1000000,
    [UTILSMMAP_MEMID_CR5A_TCMB_RAM] = 0xC1800000,
    [UTILSMMAP_MEMID_DSS_L2]        = 0x80800000,
    [UTILSMMAP_MEMID_DSS_L1D]       = 0x80F00000,
    [UTILSMMAP_MEMID_MSS_L2]        = 0xC0200000,
};


#define UTILSMMAP_ARRAYSIZE(x)  (sizeof(x)/sizeof(x[0]))

static Utils_mmapMemId_e UtilsMmap_getMemId(uint32_t addr, const Utils_mmapSegmentEntry *memTbl, uint32_t numEntries, uint32_t *addrOffset)
{
    uint_fast8_t i;
    Utils_mmapMemId_e memId;

    for (i = 0; i < numEntries; i++)
    {
        if ((addr >= memTbl[i].baseAddress) && (addr < (memTbl[i].baseAddress + memTbl[i].length)))
        {
            break;
        }
    }
    if (i < numEntries)
    {
        memId = memTbl[i].memId;
        *addrOffset = (addr - memTbl[i].baseAddress);
    }
    else
    {
        memId = UTILSMMAP_MEMID_UNKNOWN;
    }
    return memId;
}

static void UtilsMmap_getVirtAddrTblInfo(Utils_mmapSegmentEntry const **tblBase, uint32_t *tblLen)
{
#ifdef BUILD_MCU1_0
    *tblBase = &gMemSegmentTblVirtCR5[0];
    *tblLen = UTILSMMAP_ARRAYSIZE(gMemSegmentTblVirtCR5);
#elif BUILD_DSP
    *tblBase = &gMemSegmentTblVirtDSP[0];
    *tblLen = UTILSMMAP_ARRAYSIZE(gMemSegmentTblVirtDSP);
#else
    #error "Unknown build target"
#endif

}


uint32_t UtilsMmap_virt2Phys(const void *virtAddr)
{
    Utils_mmapMemId_e memId;
    Utils_mmapSegmentEntry const *tblBase; 
    uint32_t tblLen;
    uint32_t physAddr;
    uint32_t addrOffset;

    UtilsMmap_getVirtAddrTblInfo(&tblBase, &tblLen);
    memId = UtilsMmap_getMemId((uint32_t)virtAddr, tblBase, tblLen, &addrOffset);
    if (UTILSMMAP_MEMID_UNKNOWN == memId)
    {
        physAddr = (uint32_t)virtAddr;
    }
    else
    {
        physAddr = gSysMemAddress[memId] + addrOffset;
    }
    return physAddr;
}


