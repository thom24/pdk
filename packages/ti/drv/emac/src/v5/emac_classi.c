/**
 *  \file   emac_classi.c
 *
 *  \brief  EMAC driver interface to ICSSG hardware queues
 *
 *  This file contains internal functions to push/pop from ICSSG hardware queues
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

#include <ti/csl/soc.h>
#include <ti/csl/cslr_icss.h>
#include <ti/drv/emac/src/v5/emac_drv_v5.h>

struct ft1_t {
    volatile uint32_t DA0;            /* Filter1_0_DA0_Pru0 */
    volatile uint32_t DA1;            /* Filter1_0_DA1_Pru0 */
    volatile uint32_t DA_MASK0;       /* Filter1_0_DA0MaskPru0 */
    volatile uint32_t DA_MASK1;       /* Filter1_0_DA1MaskPru0 */
};

struct ft3_t {
    struct {
        volatile uint32_t START;          /* Filter3_0_StartPru0 */
        volatile uint32_t START_AUTO;     /* Filter3_0_StartAutoPru0 */
        volatile uint32_t START_LEN;      /* Filter3_0_StartoffsetPru0 */
        volatile uint32_t JMP_OFFSET;     /* Filter3_0_JmpoffsetPru0 */
        volatile uint32_t LEN;            /* Filter3_0_LenPru0 */
        volatile uint32_t CFG;            /* Filter3_0_ConfigPru0 */
        volatile uint32_t T;              /* Filter3_0_TPru0 */
        volatile uint32_t T_MASK;         /* Filter3_0_T_MaskPru0 */
    } A[16];
    struct {
        volatile uint32_t FT3_0_P0;             /* Filter3_0P0Pru0 */
        volatile uint32_t FT3_0_P1;             /* Filter3_0P1Pru0 */
        volatile uint32_t FT3_0_P_MASK0;        /* Filter3P_Mask0Pru0 */
        volatile uint32_t FT3_0_P_MASK1;        /* Filter3P_Mask1Pru0 */
    } B[16];
};

struct icssg_classi {
    volatile uint32_t FT1_START_LEN;        /* Filter1StartandLengthPru0 */
    volatile uint32_t FT1_CFG;              /* Filter1ConfigPru0 */
    struct ft1_t      FT1[8];
    struct ft3_t      FT3;
    volatile uint32_t FT_RX_PTR;            /* FilterByteCountPru0 */
    struct {
        volatile uint32_t AND_EN;     /* rx_class0_and_enablesPru0 */
        volatile uint32_t OR_EN;      /* rx_class0_or_enablesPru0 */
    } RX_CLASS[16];
    volatile uint32_t RX_CLASS_CFG1;        /* rx_class_cfg1Pru0 */
    volatile uint32_t RX_CLASS_CFG2;        /* rx_class_cfg2Pru0 */
    volatile uint32_t RX_CLASS_GATES[16];      /* rx_class_gate_cfgPru0 */
    volatile uint32_t RX_GREEN;             /* RX_greenPru0 */
    volatile uint32_t SA_HASH;              /* SAHashPru0 */
    volatile uint32_t CONN_HASH;            /* ConnectionHashPru0 */
    volatile uint32_t CONN_HASH_START;      /* ConnectionHashStartPru0 */
    volatile uint32_t RX_RATE_CFG[8];         /* RX_RateConfiguration0Pru0 */
    volatile uint32_t RX_RATE_SRC_SEL0;     /* RX_RateSourceSelect0Pru0 */
    volatile uint32_t RX_RATE_SRC_SEL1;     /* RX_RateSourceSelect1Pru0 */
    struct {
        volatile uint32_t CFG1;       /* TX_RateConfiguration1_0Pru0 */
        volatile uint32_t CFG2;       /* TX_RateConfiguration2_0Pru0 */
    } TX_RATE[8];
}; 

static struct icssg_classi *icssg_classi_ptr[EMAC_MAX_ICSSG_INSTANCES][2] =
{   
    {
        (struct icssg_classi *) (CSL_PRU_ICSSG0_DRAM0_SLV_RAM_BASE + CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_REGS_BASE
                                        + CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_FT1_START_LEN_PRU0),
        (struct icssg_classi *) (CSL_PRU_ICSSG0_DRAM0_SLV_RAM_BASE + CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_REGS_BASE
                                        + CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_FT1_START_LEN_PRU1)
    },
    {
        (struct icssg_classi *) (CSL_PRU_ICSSG1_DRAM0_SLV_RAM_BASE + CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_REGS_BASE
                                        + CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_FT1_START_LEN_PRU0),
        (struct icssg_classi *) (CSL_PRU_ICSSG1_DRAM0_SLV_RAM_BASE + CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_REGS_BASE
                                        + CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_FT1_START_LEN_PRU1)
    },
#if defined (SOC_AM65XX)
    {
        (struct icssg_classi *) (CSL_PRU_ICSSG2_DRAM0_SLV_RAM_BASE + CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_REGS_BASE
                                 + CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_FT1_START_LEN_PRU0),
        (struct icssg_classi *) (CSL_PRU_ICSSG2_DRAM0_SLV_RAM_BASE + CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_REGS_BASE
                                 + CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_FT1_START_LEN_PRU1)
    }
#endif
};



void classi_rx_limiter_example (void)
{
    struct icssg_classi *cl_prt = icssg_classi_ptr[0][0]; // icssg_0 port 0

    cl_prt->FT1_START_LEN =         0x00060000;
    cl_prt->FT1_CFG =               0x00005555;
    // setup classifiers
    cl_prt->RX_CLASS[8].AND_EN =    0x00000000;
    cl_prt->RX_CLASS[8].OR_EN =     0x20000000; // BC 
    cl_prt->RX_CLASS[9].AND_EN =    0x00000000;
    cl_prt->RX_CLASS[9].OR_EN =     0x20000000; // BC 
  
    cl_prt->RX_CLASS[10].AND_EN =   0x40000000; // MC
    cl_prt->RX_CLASS[10].OR_EN =    0x20000000; // for !BC
    cl_prt->RX_CLASS[11].AND_EN =   0x40000000; // MC
    cl_prt->RX_CLASS[11].OR_EN =    0x20000000; // for !BC

    cl_prt->RX_CLASS_GATES[ 8] =    0x00000050;
    cl_prt->RX_CLASS_GATES[ 9] =    0x00000010; // rate_limit 0 
    cl_prt->RX_CLASS_GATES[10] =    0x00000050;
    cl_prt->RX_CLASS_GATES[11] =    0x00000011; // rate_limit 1

    cl_prt->RX_CLASS_CFG1 =         (2 << (10 * 2)) | (2 << (11 * 2)); // OR AND AND OR for classi 10 and classi 11
    cl_prt->RX_CLASS_CFG2 =         ((1 << 10) | (1 << 11)) << 16 ;    // invert OR for classi 10 and classi 11

    cl_prt->RX_RATE_CFG[0] =    (50 * 32768) / 250;  // 50 Mbps for BC
    cl_prt->RX_RATE_CFG[1] =    (100 * 32768) / 250; // 100 Mbps for MC
    cl_prt->RX_RATE_SRC_SEL0 =  41 | 43 << 8 ; // select rx_class_raw8 and rx_class_raw10
}


