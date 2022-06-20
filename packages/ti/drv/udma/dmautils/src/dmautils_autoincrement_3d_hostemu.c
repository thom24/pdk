/*
 *  Copyright (c) Texas Instruments Incorporated 2018
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


/*
 *******************************************************************************
 *
 *  DMA utility functions
 *
 *******************************************************************************
*/

/**
 *****************************************************************************
 * \file  dmautils_autoincrement_3d_hostemu.c
 *
 * \brief*  This file contains host emulation function implementation for dma util functions for
 *  configuring DMA on J7 device for autoincrement 3D usecase.
 *
 *
 *****************************************************************************
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include <string.h>
#include <math.h>
#include <stdint.h>

#include "ti/drv/udma/dmautils/src/dmautils_autoincrement_3d_priv.h"
#include "ti/drv/udma/dmautils/include/dmautils_autoincrement_3d.h"

#if defined(__C7100__) || defined(__C7120__) || defined(__C7504__)
#include <c7x.h>
#endif


/* If its a Loki Build then force the mode to be in hostemulation as Loki doesnt support DRU */
#if defined (LOKI_BUILD)
#define HOST_EMULATION (1U)
#endif

#define DMAUTILS_ALIGN_CEIL(VAL, ALIGN) ((((VAL) + (ALIGN) - 1)/(ALIGN)) * (ALIGN) )


#ifdef HOST_EMULATION
#include "stdlib.h"



void hostEmulation_updateTriggerCount(struct Udma_DrvObj * udmaDrvHandle,
                                                                              volatile uint64_t *pSwTrigReg)
{
  uint32_t i;
  CSL_DRU_t *druRegs = udmaDrvHandle->utcInfo[0].druRegs;
  uintptr_t origPtr = (uintptr_t)pSwTrigReg;
  uintptr_t currPtr;

  for ( i = 0; i < udmaDrvHandle->utcInfo[0].numCh; i++)
  {
    currPtr = (uintptr_t)&druRegs->CHRT[i].CHRT_SWTRIG;
    if ( currPtr == origPtr)
    {
      /* Use reserved space for tracking number of triggers submitted for a given channel */
      druRegs->CHRT[i].Resv_256[0]++;
      break;
    }
  }
}

void hostEmulation_druChSubmitAtomicTr(CSL_DRU_t *pRegs,
                                           uint32_t chId,
                                           void *  vdata)
{
  CSL_UdmapTR * tr = (CSL_UdmapTR *)vdata;
  CSL_UdmapTR * origTransferRecord   = ( CSL_UdmapTR *)&pRegs->CHATOMIC[chId];
  CSL_UdmapTR * nextTransferRecord  = (CSL_UdmapTR *)&pRegs->CHATOMIC[chId].DEBUG[0];
  CSL_UdmapTR * nextTransferRecord1 = (CSL_UdmapTR *)&pRegs->CHATOMIC[chId].DEBUG[1];
  CSL_UdmapTR * nextTransferRecord2 = (CSL_UdmapTR *)&pRegs->CHATOMIC[chId].DEBUG[2];

  /* Use reserved space for tracking number of triggers submitted for a given channel */
  pRegs->CHRT[chId].Resv_256[0] = 0;

  *origTransferRecord  = *tr;
  *nextTransferRecord = *tr;
  *nextTransferRecord1 = *tr;
  *nextTransferRecord2 = *tr;
}


uint64_t hostEmulation_addressUpdate( uint64_t base, int32_t offset, uint64_t addrMask )
{
  uint64_t newAddr;

  newAddr = base + offset;

  if ( addrMask != 0 )
  {
    newAddr = ( base & addrMask ) | ( newAddr & ~addrMask );
  }

  return newAddr;
}

void hostEmulation_circMask( uint32_t cbk0, uint32_t cbk1, uint64_t * circMask0, uint64_t * circMask1  )
{
  uint32_t blockSize0 = cbk0 + 9U; /* power-of-2 of block size in bytes */
  uint32_t blockSize1 = cbk0 + cbk1 + 10U; /* power-of-2 of block size in bytes */

  if ( blockSize1 > 31U )
  {
    blockSize1 = 32U; /* clamp to 4GB maximum size */
  }
  *circMask0 = (~0ULL) << blockSize0;
  *circMask1 = (~0ULL) << blockSize1;
}

void hostEmulation_triggerDMA(struct Udma_DrvObj * udmaDrvHandle)
{
  uint32_t chId;
  CSL_DRU_t * druRegs;

#if ( DMA_UTILS_STANDALONE == 0 )
  CSL_UdmapCfg  * udmapRegs;
  CSL_ringacc_cfgRegs * ringAccCfgRegs;

  ringAccCfgRegs  = udmaDrvHandle->raRegs.pCfgRegs;
  udmapRegs        = &udmaDrvHandle->udmapRegs;
#endif

  druRegs             = udmaDrvHandle->utcInfo[0].druRegs;



  for ( chId = 0; chId < 32; chId++)//:TODO: Remove hard coded value of 32
  {
    if ( (druRegs->CHRT[chId].CHRT_SWTRIG & CSL_DRU_CHRT_SWTRIG_GLOBAL_TRIGGER0_MASK) == 1U)
    {
      uint8_t *srcPtr;
      uint8_t *dstPtr;
      uint16_t *srcPtr16;
      uint16_t *dstPtr16;
      uint32_t triggerType;
      uint32_t circDir;
      uint32_t icnt0;
      uint32_t CBK0, CBK1;
      uint32_t AM0, AM1, AM2, AM3;
      uint64_t srcAM0, srcAM1, srcAM2, srcAM3;
      uint64_t dstAM0, dstAM1, dstAM2, dstAM3;
      uint64_t circMask0;
      uint64_t circMask1;
      uint32_t AMODE, ELYPE;
      uint32_t loopCnt1Reset, loopCnt2Reset;
      uint8_t * interimBuffer = NULL;
      uint32_t srcLoopExitCondition = 0;
      uint32_t dstLoopExitCondition = 0;
      uint32_t totalSrcCnt, totalDstCnt;
      /* Clear the sw trigger so that next trigger can happen */


      druRegs->CHRT[chId].Resv_256[0]--;
      /* Use reserved space for tracking number of triggers submitted for a given channel */
      if ( druRegs->CHRT[chId].Resv_256[0] == 0 )
      {
        druRegs->CHRT[chId].CHRT_SWTRIG  = druRegs->CHRT[chId].CHRT_SWTRIG & (uint64_t)(~CSL_DRU_CHRT_SWTRIG_GLOBAL_TRIGGER0_MASK);
      }

      CSL_UdmapTR * origTransferRecord  = (CSL_UdmapTR *)(void *) &druRegs->CHATOMIC[chId];
      CSL_UdmapTR * nextTransferRecord = (CSL_UdmapTR *)(void *) &druRegs->CHATOMIC[chId].DEBUG[0];
      CSL_UdmapTR * nextTransferRecord1 = (CSL_UdmapTR *)(void *) &druRegs->CHATOMIC[chId].DEBUG[1];
      CSL_UdmapTR * nextTransferRecord2 = (CSL_UdmapTR *)(void *) &druRegs->CHATOMIC[chId].DEBUG[2];

      /* Do the actual transfer */

      triggerType = CSL_FEXT(origTransferRecord->flags, UDMAP_TR_FLAGS_TRIGGER0_TYPE);
      AMODE = CSL_FEXT(origTransferRecord->fmtflags, UDMAP_TR_FMTFLAGS_AMODE);

      if ( AMODE == CSL_UDMAP_TR_FMTFLAGS_AMODE_CIRCULAR)
      {
        circDir        = CSL_FEXT(origTransferRecord->fmtflags, UDMAP_TR_FMTFLAGS_DIR);
        CBK0         = CSL_FEXT(origTransferRecord->fmtflags, UDMAP_TR_FMTFLAGS_AMODE_SPECIFIC_CBK0);
        CBK1         = CSL_FEXT(origTransferRecord->fmtflags, UDMAP_TR_FMTFLAGS_AMODE_SPECIFIC_CBK1);
        AM0           = CSL_FEXT(origTransferRecord->fmtflags, UDMAP_TR_FMTFLAGS_AMODE_SPECIFIC_AM0);
        AM1           = CSL_FEXT(origTransferRecord->fmtflags, UDMAP_TR_FMTFLAGS_AMODE_SPECIFIC_AM1);
        AM2           = CSL_FEXT(origTransferRecord->fmtflags, UDMAP_TR_FMTFLAGS_AMODE_SPECIFIC_AM2);
        AM3           = CSL_FEXT(origTransferRecord->fmtflags, UDMAP_TR_FMTFLAGS_AMODE_SPECIFIC_AM3);
        hostEmulation_circMask(CBK0, CBK1, &circMask0, &circMask1);
      }
      else
      {
        /* Linear Addressing */
        circMask0 = (~0ULL);
        circMask1 = (~0ULL);
        AM0 = 0;
        AM1 = 0;
        AM2 = 0;
        AM3 = 0;
        circDir = 0;
      }

      if ( circDir == CSL_UDMAP_TR_FMTFLAGS_DIR_DST_USES_AMODE)
      {
        dstAM0 = (AM0 == 0 ) ? 0 : ( (AM0 == 1) ? circMask0 : circMask1  );/* Circular update */
        srcAM0 = 0;/* Linear Update */

        dstAM1 = (AM1 == 0 ) ? 0 : ( (AM1 == 1) ? circMask0 : circMask1  );/* Circular update */
        srcAM1 = 0;/* Linear Update */

        dstAM2 = (AM2 == 0 ) ? 0 : ( (AM2 == 1) ? circMask0 : circMask1  );/* Circular update */
        srcAM2 = 0;/* Linear Update */

        dstAM3 = (AM3 == 0 ) ? 0 : ( (AM3 == 1) ? circMask0 : circMask1  );/* Circular update */
        srcAM3 = 0;/* Linear Update */
      }
      else
      {
        dstAM0 = 0;/* Linear Update */
        srcAM0 = (AM0 == 0 ) ? 0 : ( (AM0 == 1) ? circMask0 : circMask1  );

        dstAM1 = 0;/* Linear Update */
        srcAM1 = (AM1 == 0 ) ? 0 : ( (AM1 == 1) ? circMask0 : circMask1  );

        dstAM2 = 0;/* Linear Update */
        srcAM2 = (AM2 == 0 ) ? 0 : ( (AM2 == 1) ? circMask0 : circMask1  );

        dstAM3 = 0;/* Linear Update */
        srcAM3 = (AM3 == 0 ) ? 0 : ( (AM3 == 1) ? circMask0 : circMask1  );

      }

      loopCnt1Reset = 0;
      loopCnt2Reset = 0;

      ELYPE = CSL_FEXT(origTransferRecord->fmtflags, UDMAP_TR_FMTFLAGS_ELYPE);
      /* allocate worst case, actual buffer used will depend on trugerType */
      uint32_t interimBufSize = (origTransferRecord->icnt0 * origTransferRecord->icnt1 * origTransferRecord->icnt2 *
                                        origTransferRecord->icnt3);
      if (( ELYPE == CSL_UDMAP_TR_FMTFLAGS_ELYPE_2_1) ||
          ( ELYPE == CSL_UDMAP_TR_FMTFLAGS_ELYPE_1_2))
      {
        interimBufSize *= 2; // As icnt0 is in elements in case of 16 bit to 8 bit conversion
      }
      interimBuffer = (uint8_t *)malloc(interimBufSize);
      dstPtr = interimBuffer;
      dstPtr16 = (uint16_t *)interimBuffer;

      totalSrcCnt = nextTransferRecord->icnt0 * nextTransferRecord->icnt1 * nextTransferRecord->icnt2 * nextTransferRecord->icnt3;
      totalDstCnt = nextTransferRecord->dicnt0 * nextTransferRecord->dicnt1 * nextTransferRecord->dicnt2 * nextTransferRecord->dicnt3;

      srcLoopExitCondition  = ( totalSrcCnt < totalDstCnt )? totalSrcCnt : totalDstCnt;
      dstLoopExitCondition  = srcLoopExitCondition;
      /* Set the minimum value of icnt3 for both src and dst as TR completes whenever anyone of them  is exauhsted */
      /* Transfer source data to a intermediate linear buffer */

      while (1)
      {
        if( ELYPE == CSL_UDMAP_TR_FMTFLAGS_ELYPE_1_2)
        {
          srcPtr = (uint8_t *)nextTransferRecord->addr;
          for (icnt0 = 0; icnt0 < nextTransferRecord->icnt0; icnt0++)
          {
            *dstPtr16 = *srcPtr;
              srcPtr = (uint8_t *)hostEmulation_addressUpdate((uint64_t)srcPtr, 1, srcAM0);
              dstPtr16++;
          }
        }
        else if( ELYPE == CSL_UDMAP_TR_FMTFLAGS_ELYPE_2_1)
        {
          srcPtr16 = (uint16_t *)nextTransferRecord->addr;
          for (icnt0 = 0; icnt0 < nextTransferRecord->icnt0; icnt0++)
          {
            *dstPtr16 = *srcPtr16;
              srcPtr16 = (uint16_t *)hostEmulation_addressUpdate((uint64_t)srcPtr16, 2, srcAM0);
              dstPtr16++;
          }
        }
        else
        {
          srcPtr = (uint8_t *)nextTransferRecord->addr;
  /*        dstPtr = interimBuffer +
                      (origTransferRecord->icnt1 - nextTransferRecord->icnt1) * origTransferRecord->icnt0 +
                      (origTransferRecord->icnt2 - nextTransferRecord->icnt2) * origTransferRecord->icnt0 * origTransferRecord->icnt1;*/

          for (icnt0 = 0; icnt0 < nextTransferRecord->icnt0; icnt0++)
          {
            *dstPtr = *srcPtr;
              srcPtr = (uint8_t *)hostEmulation_addressUpdate((uint64_t)srcPtr, 1, srcAM0);
              dstPtr++;
          }
        }
        nextTransferRecord->icnt1--;
        nextTransferRecord->addr   = hostEmulation_addressUpdate(nextTransferRecord->addr ,  nextTransferRecord->dim1,   srcAM1);

        if ( nextTransferRecord->icnt1 == 0)
        {
          loopCnt1Reset = 1;
          nextTransferRecord->icnt2--;
          nextTransferRecord->icnt1 = origTransferRecord->icnt1;

          nextTransferRecord->addr   = hostEmulation_addressUpdate(nextTransferRecord1->addr ,  nextTransferRecord->dim2,   srcAM2);

          nextTransferRecord1->addr   = nextTransferRecord->addr;
        }

        if ( nextTransferRecord->icnt2 == 0)
        {
          loopCnt2Reset= 1;
          nextTransferRecord->icnt3--;
          nextTransferRecord->icnt2 = origTransferRecord->icnt2;

          nextTransferRecord->addr   = hostEmulation_addressUpdate(nextTransferRecord2->addr,  nextTransferRecord->dim3,   srcAM3);

          nextTransferRecord1->addr   = nextTransferRecord->addr;
          nextTransferRecord2->addr   = nextTransferRecord->addr;
        }

        if ( nextTransferRecord->icnt3 == 0)
        {
          CSL_REG64_FINS(&druRegs->CHRT[chId].CHRT_CTL, DRU_CHRT_CTL_ENABLE, 0);
          break;
        }


        if ( triggerType == CSL_UDMAP_TR_FLAGS_TRIGGER_TYPE_ICNT1_DEC )
        {
          break;
        }
        else if ( triggerType == CSL_UDMAP_TR_FLAGS_TRIGGER_TYPE_ICNT2_DEC )
        {
          if ( loopCnt1Reset == 1)
          {
            nextTransferRecord1->addr   = nextTransferRecord->addr;
            break;
          }
        }
        else if ( triggerType == CSL_UDMAP_TR_FLAGS_TRIGGER_TYPE_ICNT3_DEC )
        {
          if ( loopCnt2Reset == 1)
          {
            nextTransferRecord2->addr   = nextTransferRecord->addr;
            break;
          }
        }
        else
        {
          /*Indicates 4D sync,  Just continue */
        }
        srcLoopExitCondition--;
        if ( srcLoopExitCondition == 0)
        {
          nextTransferRecord->icnt3 = 0;
          CSL_REG64_FINS(&druRegs->CHRT[chId].CHRT_CTL, DRU_CHRT_CTL_ENABLE, 0);
          break;
        }
      }

      loopCnt1Reset = 0;
      loopCnt2Reset = 0;

     srcPtr = interimBuffer;
     srcPtr16 = (uint16_t *)interimBuffer;
      /* Now copy the intermediate data to the destination buffer */
     while (1)
      {
        if( ELYPE == CSL_UDMAP_TR_FMTFLAGS_ELYPE_1_2)
        {
          dstPtr16 = (uint16_t *)nextTransferRecord->daddr;
          for (icnt0 = 0; icnt0 < nextTransferRecord->dicnt0; icnt0++)
          {
            *dstPtr16 = (uint16_t)(*srcPtr16);
            srcPtr16++;
            dstPtr16 = (uint16_t *)hostEmulation_addressUpdate((uint64_t)dstPtr16, 2, dstAM0);
          }
        }
        else if( ELYPE == CSL_UDMAP_TR_FMTFLAGS_ELYPE_2_1)
        {
          dstPtr = (uint8_t *)nextTransferRecord->daddr;
          for (icnt0 = 0; icnt0 < nextTransferRecord->dicnt0; icnt0++)
          {
            *dstPtr = (uint8_t)(*srcPtr16);
              srcPtr16++;
              dstPtr = (uint8_t *)hostEmulation_addressUpdate((uint64_t)dstPtr, 1, dstAM0);
          }
        }
        else
        {
          dstPtr = (uint8_t *)nextTransferRecord->daddr;

          for (icnt0 = 0; icnt0 < nextTransferRecord->dicnt0; icnt0++)
          {
            *dstPtr = *srcPtr;
             *srcPtr++;
              dstPtr = (uint8_t *)hostEmulation_addressUpdate((uint64_t)dstPtr, 1, dstAM0);
          }
        }
        nextTransferRecord->dicnt1--;

        nextTransferRecord->daddr = hostEmulation_addressUpdate(nextTransferRecord->daddr, nextTransferRecord->ddim1, dstAM1);

        if ( nextTransferRecord->dicnt1 == 0)
        {
          loopCnt1Reset = 1;
          nextTransferRecord->dicnt2--;
          nextTransferRecord->dicnt1 = origTransferRecord->dicnt1;

          nextTransferRecord->daddr = hostEmulation_addressUpdate(nextTransferRecord1->daddr, nextTransferRecord->ddim2, dstAM2);
          nextTransferRecord1->daddr = nextTransferRecord->daddr;
        }

        if ( nextTransferRecord->dicnt2 == 0)
        {
          loopCnt2Reset= 1;
          nextTransferRecord->dicnt3--;
          nextTransferRecord->dicnt2 = origTransferRecord->dicnt2;

          nextTransferRecord->daddr = hostEmulation_addressUpdate(nextTransferRecord2->daddr, nextTransferRecord->ddim3, dstAM3);

          nextTransferRecord1->daddr = nextTransferRecord->daddr;
          nextTransferRecord2->daddr = nextTransferRecord->daddr;
        }

        if ( nextTransferRecord->dicnt3 == 0)
        {
          CSL_REG64_FINS(&druRegs->CHRT[chId].CHRT_CTL, DRU_CHRT_CTL_ENABLE, 0);
          break;
        }

        if ( triggerType == CSL_UDMAP_TR_FLAGS_TRIGGER_TYPE_ICNT1_DEC )
        {
          break;
        }
        else if ( triggerType == CSL_UDMAP_TR_FLAGS_TRIGGER_TYPE_ICNT2_DEC )
        {
          if ( loopCnt1Reset == 1)
          {
            break;
          }
        }
        else if ( triggerType == CSL_UDMAP_TR_FLAGS_TRIGGER_TYPE_ICNT3_DEC )
        {
          if ( loopCnt2Reset == 1)
          {
            break;
          }
        }

        dstLoopExitCondition--;
        if ( dstLoopExitCondition == 0)
        {
          nextTransferRecord->dicnt3 = 0;
          CSL_REG64_FINS(&druRegs->CHRT[chId].CHRT_CTL, DRU_CHRT_CTL_ENABLE, 0);
          break;
        }
      }

      if (( nextTransferRecord->icnt3 == 0 ) || ( nextTransferRecord->dicnt3 == 0 ))
      {
#if ( DMA_UTILS_STANDALONE == 0 )
        /* this indicates that TR is received from PSIL */
        if ( (druRegs->CHNRT[chId].CFG & CSL_DRU_CHNRT_CFG_CHAN_TYPE_OWNER_MASK) != 0 )
        {
           CSL_UdmapCppi5TRPD * trDescriptor;
           uint64_t * ringPtr;
           uint64_t currTr;
           uint64_t numTr;
           CSL_UdmapTR           *pTr;
           /* Get Ring Memory Pointer */
          ringPtr = (uint64_t *) ((uint64_t)((uint64_t)ringAccCfgRegs->RING[udmapRegs->txChanCnt + chId].BA_HI << 32) |
                                            ringAccCfgRegs->RING[udmapRegs->txChanCnt + chId].BA_LO);

           trDescriptor = (CSL_UdmapCppi5TRPD *) (*ringPtr);

           numTr = CSL_udmapCppi5GetPktLen(trDescriptor);
           /* Use this field to track the TR, For the target build this would be handled by hardware */
           /* In real hardware this will not be like this it is done just for host emulation*/
           currTr =  druRegs->CHATOMIC[chId].DEBUG[1].NEXT_TR_WORD0_1;

            if ( currTr < numTr)
            {
               currTr++;
               pTr = (CSL_UdmapTR *)( (uint8_t *)trDescriptor + sizeof(CSL_UdmapTR) * currTr);

               /* Update both original and next transfer record by reading the TR from the TR descriptor */
              hostEmulation_druChSubmitAtomicTr(druRegs, chId, (void *) pTr);

               druRegs->CHATOMIC[chId].DEBUG[1].NEXT_TR_WORD0_1 = currTr;

            }
        }
#endif
      }
      if ( interimBuffer != NULL)
      {
        free(interimBuffer);
      }
    }
  }
}
#endif

