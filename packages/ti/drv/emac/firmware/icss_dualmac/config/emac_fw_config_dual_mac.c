/*
*  TEXAS INSTRUMENTS TEXT FILE LICENSE
* 
*   Copyright (c) 2020 Texas Instruments Incorporated
* 
*  All rights reserved not granted herein.
*  
*  Limited License.  
* 
*  Texas Instruments Incorporated grants a world-wide, royalty-free, non-exclusive 
*  license under copyrights and patents it now or hereafter owns or controls to 
*  make, have made, use, import, offer to sell and sell ("Utilize") this software 
*  subject to the terms herein.  With respect to the foregoing patent license, 
*  such license is granted  solely to the extent that any such patent is necessary 
*  to Utilize the software alone.  The patent license shall not apply to any 
*  combinations which include this software, other than combinations with devices 
*  manufactured by or for TI (“TI Devices”).  No hardware patent is licensed hereunder.
* 
*  Redistributions must preserve existing copyright notices and reproduce this license 
*  (including the above copyright notice and the disclaimer and (if applicable) source 
*  code license limitations below) in the documentation and/or other materials provided 
*  with the distribution.
*  
*  Redistribution and use in binary form, without modification, are permitted provided 
*  that the following conditions are met:
* 	No reverse engineering, decompilation, or disassembly of this software is 
*   permitted with respect to any software provided in binary form.
* 	Any redistribution and use are licensed by TI for use only with TI Devices.
* 	Nothing shall obligate TI to provide you with source code for the software 
*   licensed and provided to you in object code.
*  
*  If software source code is provided to you, modification and redistribution of the 
*  source code are permitted provided that the following conditions are met:
* 	Any redistribution and use of the source code, including any resulting derivative 
*   works, are licensed by TI for use only with TI Devices.
* 	Any redistribution and use of any object code compiled from the source code
*   and any resulting derivative works, are licensed by TI for use only with TI Devices.
* 
*  Neither the name of Texas Instruments Incorporated nor the names of its suppliers 
*  may be used to endorse or promote products derived from this software without 
*  specific prior written permission.
* 
*  DISCLAIMER.
* 
*  THIS SOFTWARE IS PROVIDED BY TI AND TI’S LICENSORS "AS IS" AND ANY EXPRESS OR IMPLIED 
*  WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY 
*  AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL TI AND TI’S 
*  LICENSORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
*  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE 
*  GOODS OR SERVICES* LOSS OF USE, DATA, OR PROFITS* OR BUSINESS INTERRUPTION) HOWEVER 
*  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
*  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
*  EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/ 

/**
 * @file emac_fw_config_dual_mac.c
 * @brief EMAC firmware configuration file which contain initializtion values for
 *          static configuration parameters for ICSSG Firmware.
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdint.h>
#include <ti/csl/cslr_icss.h>
#include <ti/drv/emac/src/v5/emac_drv_v5.h>
#include <ti/drv/emac/soc/emac_soc_v5.h>
#include <ti/drv/emac/src/v5/emac_utils.h>

#include <ti/drv/emac/firmware/icss_dualmac/config/emac_fw_config_dual_mac.h>
#include <ti/drv/emac/firmware/icss_dualmac/bin_pg2/fw_mem_map.h>

#define EMAC_ICSSG_MAX_HWQ                      ((uint32_t)8U)
#define EMAC_ICSSG_RECYCLE_Q_SLICE0             ((uint32_t)16U)
#define EMAC_ICSSG_RECYCLE_Q_SLICE1             ((uint32_t)17U)
#define EMAC_ICSSG_PORT_HF_Q_SLICE0             ((uint32_t)33U)
#define EMAC_ICSSG_PORT_HF_Q_SLICE1             ((uint32_t)36U)
#define EMAC_ICSSG_PORT_LF_Q_SLICE0             ((uint32_t)33U)
#define EMAC_ICSSG_PORT_LF_Q_SLICE1             ((uint32_t)37U)
#define EMAC_ICSSG_HOST_HF_Q_SLICE0             ((uint32_t)34U)
#define EMAC_ICSSG_HOST_HF_Q_SLICE1             ((uint32_t)38U)
#define EMAC_ICSSG_HOST_LF_Q_SLICE0             ((uint32_t)35U)
#define EMAC_ICSSG_HOST_LF_Q_SLICE1             ((uint32_t)39U)
#define EMAC_ICSSG_HOST_SF_Q_SLICE0             ((uint32_t)40U)
#define EMAC_ICSSG_HOST_SF_Q_SLICE1             ((uint32_t)41U)
#define EMAC_ICSSG_RECYCLE_Q_SLICE0_OFFSET      ((uint32_t)4U*16U)
#define EMAC_ICSSG_RECYCLE_Q_SLICE1off          ((uint32_t)4U*17U)
#define EMAC_ICSSG_PORT_HF_Q_SLICE0off          ((uint32_t)4U*32U)
#define EMAC_ICSSG_PORT_HF_Q_SLICE1off          ((uint32_t)4U*36U)
#define EMAC_ICSSG_PORT_LF_Q_SLICE0_OFFSET      ((uint32_t)4U*33U)
#define EMAC_ICSSG_PORT_LF_Q_SLICE1_OFFSET      ((uint32_t)4U*37U)
#define EMAC_ICSSG_HOST_HF_Q_SLICE0_OFFSET      ((uint32_t)4U*34U)
#define EMAC_ICSSG_HOST_HF_Q_SLICE1_OFFSET      ((uint32_t)4U*38U)
#define EMAC_ICSSG_HOST_LF_Q_SLICE0_OFFSET      ((uint32_t)4U*35U)
#define EMAC_ICSSG_HOST_LF_Q_SLICE1_OFFSET      ((uint32_t)4U*39U)
#define EMAC_ICSSG_HOST_SF_Q_SLICE0_OFFSET      ((uint32_t)4U*40U)
#define EMAC_ICSSG_HOST_SF_Q_SLICE1_OFFSET      ((uint32_t)4U*41U)
#define EMAC_ICSSG_NORMAL_PD_SIZE               ((uint32_t)8U)
#define EMAC_ICSSG_SPECIAL_PD_SIZE              ((uint32_t)20U)
#define EMAC_ICSSG_NORMAL_PD_SIZE_BYTE_COUNT               ((uint32_t)2U)
#define EMAC_ICSSG_SPECIAL_PD_SIZE_BYTE_COUNT              ((uint32_t)5U)

#define EMAC_ICSSG_DUAL_MAC_FW_HOST_QUEUE_SIZE_PG1  ((uint32_t)6144U)       /* ~6.kB per host queue */

#define EMAC_ICSSG_DUAL_MAC_START_BUFFER_POOL_NUM ((uint32_t)8U)


EMAC_ICSSG_DUALMAC_FW_CFG emac_dualmac_cfg_pg1 = {
    {
        EMAC_ICSSG_DUAL_MAC_FW_HOST_QUEUE_SIZE_PG1,EMAC_ICSSG_DUAL_MAC_FW_HOST_QUEUE_SIZE_PG1,EMAC_ICSSG_DUAL_MAC_FW_HOST_QUEUE_SIZE_PG1,EMAC_ICSSG_DUAL_MAC_FW_HOST_QUEUE_SIZE_PG1,EMAC_ICSSG_DUAL_MAC_FW_HOST_QUEUE_SIZE_PG1,EMAC_ICSSG_DUAL_MAC_FW_HOST_QUEUE_SIZE_PG1,EMAC_ICSSG_DUAL_MAC_FW_HOST_QUEUE_SIZE_PG1,EMAC_ICSSG_DUAL_MAC_FW_HOST_QUEUE_SIZE_PG1
    }
};

struct EMAC_ICSSG_BUF_POOL_CFG_s {
    uint32_t poolBase;
    uint32_t poolLen;
} EMAC_ICSSG_BUF_POOL_CFG;

void emac_icssg_dual_mac_fw_config_fxn(uint32_t portNum, EMAC_ICSSG_FW_CFG_PG2 *pIcssgFwCfg)
{
    uint32_t startAddr;
    uint32_t endAddr;
    uint32_t regVal, hwQueueNum, i;
    uint32_t MaxNumNormalPDs = 64, MaxNumSpecialPDs = 16;
    uint32_t pdWord[5], pdAddr;
    memset ((void*)&pdWord, 0, sizeof(pdWord));
    uintptr_t icssgBaseAddr = pIcssgFwCfg->icssgBaseAddr;

    //For reducing IEP latency. Enable OCP clock.
    HW_WR_REG32(icssgBaseAddr + CSL_ICSSCFG_REGS_BASE + CSL_ICSSCFG_IEPCLK, 1U);

    //Delay after IEP Sync Config. Requires minimum 10 ICSS clock cycles before IEP register access
    Osal_delay(100);

    //Core sync will make ICSSG access to MSMC optimal
    HW_WR_REG32(icssgBaseAddr + CSL_ICSSCFG_REGS_BASE +CSL_ICSSCFG_CORE_SYNC_REG, 1U); /* Enable coresync */

    /* Enable IEP0 counter and set default increment as 4 */
    regVal = (0x1U << CSL_ICSS_G_PR1_IEP0_SLV_GLOBAL_CFG_REG_CNT_ENABLE_SHIFT)  |
             (0x4U << CSL_ICSS_G_PR1_IEP0_SLV_GLOBAL_CFG_REG_DEFAULT_INC_SHIFT) |
             (0x4U << CSL_ICSS_G_PR1_IEP0_SLV_GLOBAL_CFG_REG_CMP_INC_SHIFT);
    HW_WR_REG32(icssgBaseAddr + CSL_ICSS_G_PR1_IEP0_SLV_REGS_BASE + CSL_ICSS_G_PR1_IEP0_SLV_GLOBAL_CFG_REG, regVal);

    /*Enable IEP1 counter and set default increment as 4 - Required for RX and TX time stamping*/
    regVal = (0x1U << CSL_ICSS_G_PR1_IEP1_SLV_GLOBAL_CFG_REG_CNT_ENABLE_SHIFT)  |
             (0x4U << CSL_ICSS_G_PR1_IEP1_SLV_GLOBAL_CFG_REG_DEFAULT_INC_SHIFT) |
             (0x4U << CSL_ICSS_G_PR1_IEP1_SLV_GLOBAL_CFG_REG_CMP_INC_SHIFT);
    HW_WR_REG32(icssgBaseAddr + CSL_ICSS_G_PR1_IEP1_SLV_REGS_BASE + CSL_ICSS_G_PR1_IEP1_SLV_GLOBAL_CFG_REG, regVal);

    /* Enable PA_STAT block for diagnostic counters in ICSSG */
    regVal = (uint32_t)(1U << 31U) | 2U;      //Enable stats block, 2 vi 
    HW_WR_REG32(icssgBaseAddr + CSL_ICSS_G_PA_STAT_WRAP_PA_SLV_REGS_BASE + 8U,regVal); 

    HW_WR_REG32(icssgBaseAddr + CSL_ICSSCFG_REGS_BASE + CSL_ICSSCFG_GPCFG0, 0x08000003U); /* GPCFG0 mux sel MII_RT */
    HW_WR_REG32(icssgBaseAddr + CSL_ICSSCFG_REGS_BASE + CSL_ICSSCFG_GPCFG1, 0x08000003U); /* GPCFG1 mux sel MII_RT */

    //Need to be slice specific
    if ((portNum & 1U) == 0)
    {
        //Program constant table pointer for C28 on all 3 PRU cores
        HW_WR_REG32(icssgBaseAddr + CSL_ICSS_G_PR1_PDSP0_IRAM_REGS_BASE + CSL_ICSS_G_PR1_PDSP0_IRAM_CONSTANT_TABLE_PROG_PTR_0, 0x100U);
        HW_WR_REG32(icssgBaseAddr + CSL_ICSS_G_PR1_RTU0_PR1_RTU0_IRAM_REGS_BASE + CSL_ICSS_G_PR1_PDSP0_IRAM_CONSTANT_TABLE_PROG_PTR_0, 0x100U);
        HW_WR_REG32(icssgBaseAddr + CSL_ICSS_G_PR1_PDSP_TX0_IRAM_REGS_BASE + CSL_ICSS_G_PR1_PDSP0_IRAM_CONSTANT_TABLE_PROG_PTR_0, 0x100U);
        /* Program task manager configurations for all 3 PRUs */
        /* RX tasks run on RX_PRU0 */
        /*Configure RXBK1 Size = 20Bytes, RXBK2 Size = 12 Bytes, RXBKn Size = 32 Bytes. */
        HW_WR_REG32(icssgBaseAddr +
                    CSL_ICSS_G_PR1_TASKS_MGR_PRU0_PR1_TASKS_MGR_PRU0_MMR_REGS_BASE +
                    CSL_ICSS_G_PR1_TASKS_MGR_PRU0_PR1_TASKS_MGR_PRU0_MMR_RX_CFG, 
                    (19U << CSL_ICSS_G_PR1_TASKS_MGR_PRU0_PR1_TASKS_MGR_PRU0_MMR_RX_CFG_BK1_SIZE_SHIFT)|
                    (11U << CSL_ICSS_G_PR1_TASKS_MGR_PRU0_PR1_TASKS_MGR_PRU0_MMR_RX_CFG_BK2_SIZE_SHIFT)|
                    (31U << CSL_ICSS_G_PR1_TASKS_MGR_PRU0_PR1_TASKS_MGR_PRU0_MMR_RX_CFG_BKN_SIZE_SHIFT));
        /* RX FIFO WM configuration on RTU0 */ 
        HW_WR_REG32(icssgBaseAddr +
                    CSL_ICSS_G_PR1_TASKS_MGR_RTU0_PR1_TASKS_MGR_RTU0_MMR_REGS_BASE +
                    CSL_ICSS_G_PR1_TASKS_MGR_RTU0_PR1_TASKS_MGR_RTU0_MMR_RX_CFG,
                    (19U << CSL_ICSS_G_PR1_TASKS_MGR_RTU0_PR1_TASKS_MGR_RTU0_MMR_RX_CFG_BK1_SIZE_SHIFT) |
                    (11U << CSL_ICSS_G_PR1_TASKS_MGR_RTU0_PR1_TASKS_MGR_RTU0_MMR_RX_CFG_BK2_SIZE_SHIFT) |
                    (31U << CSL_ICSS_G_PR1_TASKS_MGR_RTU0_PR1_TASKS_MGR_RTU0_MMR_RX_CFG_BKN_SIZE_SHIFT));
        HW_WR_REG32(icssgBaseAddr +
                    CSL_ICSS_G_PR1_TASKS_MGR_PRU_TX0_PR1_TASKS_MGR_PRU_TX0_MMR_REGS_BASE +
                    CSL_ICSS_G_PR1_TASKS_MGR_PRU_TX0_PR1_TASKS_MGR_PRU_TX0_MMR_TX_CFG,
                    30U);
        /* Enable task manager nesting for PRUs and RTUs for TS1 tasks only */
        regVal = 0x1FU;
        HW_WR_REG32(icssgBaseAddr +
                    CSL_ICSS_G_PR1_TASKS_MGR_PRU0_PR1_TASKS_MGR_PRU0_MMR_REGS_BASE +
                    CSL_ICSS_G_PR1_TASKS_MGR_PRU0_PR1_TASKS_MGR_PRU0_MMR_CAP_EN_CFG,
                    regVal);
        HW_WR_REG32(icssgBaseAddr +
                    CSL_ICSS_G_PR1_TASKS_MGR_RTU0_PR1_TASKS_MGR_RTU0_MMR_REGS_BASE +
                    CSL_ICSS_G_PR1_TASKS_MGR_RTU0_PR1_TASKS_MGR_RTU0_MMR_CAP_EN_CFG,
                    regVal);
        HW_WR_REG32(icssgBaseAddr +
                    CSL_ICSS_G_PR1_TASKS_MGR_PRU_TX0_PR1_TASKS_MGR_PRU_TX0_MMR_REGS_BASE +
                    CSL_ICSS_G_PR1_TASKS_MGR_PRU_TX0_PR1_TASKS_MGR_PRU_TX0_MMR_CAP_EN_CFG,
                    regVal);

        for (hwQueueNum = 0; hwQueueNum < EMAC_ICSSG_MAX_HWQ; hwQueueNum++)
        {
            HW_WR_REG32(icssgBaseAddr +
                   CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_REGS_BASE +
                   CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_QUEUE_RESET,
                   hwQueueNum);
        }
        HW_WR_REG32(icssgBaseAddr +
                    CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_REGS_BASE+
                    CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_QUEUE_RESET, EMAC_ICSSG_RECYCLE_Q_SLICE0);
        HW_WR_REG32(icssgBaseAddr +
                    CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_REGS_BASE +
                    CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_QUEUE_RESET,
                    EMAC_ICSSG_PORT_HF_Q_SLICE0);
        HW_WR_REG32(icssgBaseAddr +
                    CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_REGS_BASE +
                    CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_QUEUE_RESET,
                    EMAC_ICSSG_PORT_LF_Q_SLICE0);
        HW_WR_REG32(icssgBaseAddr +
                    CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_REGS_BASE +
                    CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_QUEUE_RESET,
                    EMAC_ICSSG_HOST_HF_Q_SLICE0);
        HW_WR_REG32(icssgBaseAddr +
                    CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_REGS_BASE +
                    CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_QUEUE_RESET,
                    EMAC_ICSSG_HOST_LF_Q_SLICE0);
        HW_WR_REG32(icssgBaseAddr +
                    CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_REGS_BASE +
                    CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_QUEUE_RESET,
                    EMAC_ICSSG_HOST_SF_Q_SLICE0);
        //Now do PDs PORTQ LOW & HI, HOSTQ LOW & HI
        for (i = 0; i < MaxNumNormalPDs; i++)
        {
            pdWord[0] = pdWord[0] & 0xFF00FFFFU; //Clear flags
            pdWord[0] = pdWord[0] | 0x000000U; //Set Pool, Slice ID
            pdAddr = PORT_DESC0_LO + i*EMAC_ICSSG_NORMAL_PD_SIZE;
            //Init PD Word
            emac_hw_mem_write ((icssgBaseAddr + CSL_ICSS_G_RAM_SLV_RAM_REGS_BASE + pdAddr), pdWord, EMAC_ICSSG_NORMAL_PD_SIZE/2);
            //Push to EMAC_ICSSG_PORT_LF_Q_SLICE0_OFFSET
            HW_WR_REG32(icssgBaseAddr +
                        CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_REGS_BASE +
                        CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_QUEUE0 +
                        EMAC_ICSSG_PORT_LF_Q_SLICE0_OFFSET,
                        pdAddr);
            pdWord[0] = pdWord[0] & 0xFF00FFFFU; //Clear flags
            pdWord[0] = pdWord[0] | 0x200000U; //Set Pool, Slice ID
            pdAddr = PORT_DESC0_HI + i*EMAC_ICSSG_NORMAL_PD_SIZE;
            //Init PD Word
            emac_hw_mem_write ((icssgBaseAddr + CSL_ICSS_G_RAM_SLV_RAM_REGS_BASE + pdAddr), pdWord, EMAC_ICSSG_NORMAL_PD_SIZE_BYTE_COUNT);
            //Push to EMAC_ICSSG_PORT_HF_Q_SLICE0off
            HW_WR_REG32(icssgBaseAddr +
                        CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_REGS_BASE +
                        CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_QUEUE0 +
                        EMAC_ICSSG_PORT_HF_Q_SLICE0off,
                        pdAddr);
            pdWord[0] = pdWord[0] & 0xFF00FFFFU; //Clear flags
            pdWord[0] = pdWord[0] | 0x000000U; //Set Pool, Slice ID
            pdAddr = HOST_DESC0_LO + i*EMAC_ICSSG_NORMAL_PD_SIZE;
            //Init PD Word
            emac_hw_mem_write ((icssgBaseAddr + CSL_ICSS_G_RAM_SLV_RAM_REGS_BASE + pdAddr), pdWord, EMAC_ICSSG_NORMAL_PD_SIZE_BYTE_COUNT);
            //Push to EMAC_ICSSG_HOST_LF_Q_SLICE0_OFFSET
            HW_WR_REG32(icssgBaseAddr +
                        CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_REGS_BASE +
                        CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_QUEUE0 +
                        EMAC_ICSSG_HOST_LF_Q_SLICE0_OFFSET,
                        pdAddr);
            pdWord[0] = pdWord[0] & 0xFF00FFFFU; //Clear flags
            pdWord[0] = pdWord[0] | 0x200000U; //Set Pool, Slice ID
            pdAddr = HOST_DESC0_HI + i*EMAC_ICSSG_NORMAL_PD_SIZE;
            //Init PD Word
            emac_hw_mem_write ((icssgBaseAddr + CSL_ICSS_G_RAM_SLV_RAM_REGS_BASE + pdAddr), pdWord, EMAC_ICSSG_NORMAL_PD_SIZE_BYTE_COUNT);
            //Push to EMAC_ICSSG_HOST_HF_Q_SLICE0_OFFSET
            HW_WR_REG32(icssgBaseAddr +
                        CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_REGS_BASE +
                        CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_QUEUE0 +
                        EMAC_ICSSG_HOST_HF_Q_SLICE0_OFFSET,
                        pdAddr);
        }
        //Create special PD pools
        for (i = 0; i < MaxNumSpecialPDs; i++)
        {
            pdWord[0] = pdWord[0] & 0xFF00FFFFU; //Clear flags
            pdWord[0] = pdWord[0] | 0x400000U; //Set Pool, Slice ID
            pdAddr = HOST_SPPD0 + i*EMAC_ICSSG_SPECIAL_PD_SIZE;
            //Init PD Word
            emac_hw_mem_write ((icssgBaseAddr + CSL_ICSS_G_RAM_SLV_RAM_REGS_BASE + pdAddr), pdWord, EMAC_ICSSG_SPECIAL_PD_SIZE_BYTE_COUNT);
            //Push to EMAC_ICSSG_HOST_SF_Q_SLICE0_OFFSET
            HW_WR_REG32(icssgBaseAddr +
                        CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_REGS_BASE +
                        CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_QUEUE0 +
                        EMAC_ICSSG_HOST_SF_Q_SLICE0_OFFSET,
                        pdAddr);
        }
                    //Initialize 24 Buffer Pools per slice
        EMAC_ICSSG_BUF_POOL_CFG.poolBase = pIcssgFwCfg->bufferPoolLowAddr;
        for (i = EMAC_ICSSG_DUAL_MAC_START_BUFFER_POOL_NUM; i < (EMAC_ICSSG_DUAL_MAC_START_BUFFER_POOL_NUM +pIcssgFwCfg->numBufferPool); i++)
        {
            EMAC_ICSSG_BUF_POOL_CFG.poolLen = pIcssgFwCfg->bufferPoolSize[i-EMAC_ICSSG_DUAL_MAC_START_BUFFER_POOL_NUM];
            emac_hw_mem_write ((icssgBaseAddr + CSL_ICSS_G_DRAM0_SLV_RAM_REGS_BASE + BUFFER_POOL_0_ADDR_OFFSET + i*8), &EMAC_ICSSG_BUF_POOL_CFG, EMAC_ICSSG_NORMAL_PD_SIZE_BYTE_COUNT);
            EMAC_ICSSG_BUF_POOL_CFG.poolBase += pIcssgFwCfg->bufferPoolSize[i-EMAC_ICSSG_DUAL_MAC_START_BUFFER_POOL_NUM];
        }
        startAddr = EMAC_ICSSG_BUF_POOL_CFG.poolBase;
        endAddr =startAddr + 0x4000;
        HW_WR_REG32(icssgBaseAddr + CSL_ICSS_G_DRAM0_SLV_RAM_REGS_BASE + HOST_RX_Q_PRE_CONTEXT_OFFSET, startAddr);
        HW_WR_REG32(icssgBaseAddr + CSL_ICSS_G_DRAM0_SLV_RAM_REGS_BASE + HOST_RX_Q_PRE_CONTEXT_OFFSET + 4U, startAddr);
        HW_WR_REG32(icssgBaseAddr + CSL_ICSS_G_DRAM0_SLV_RAM_REGS_BASE + HOST_RX_Q_PRE_CONTEXT_OFFSET + 8U, startAddr);
        HW_WR_REG32(icssgBaseAddr + CSL_ICSS_G_DRAM0_SLV_RAM_REGS_BASE + HOST_RX_Q_PRE_CONTEXT_OFFSET + 12U, endAddr);

        //Configure PSI flow info for firmware
        //SMEM
        //#define PSI_L_REGULAR_FLOW_ID_BASE_OFFSET
        //#define PSI_L_MGMT_FLOW_ID_OFFSET
        //10 flows per slice
        HW_WR_REG32(icssgBaseAddr + CSL_ICSS_G_DRAM0_SLV_RAM_REGS_BASE + PSI_L_REGULAR_FLOW_ID_BASE_OFFSET,
                        ((pIcssgFwCfg->rxMgmtFlowStart) << 16) |
                         (pIcssgFwCfg->rxPktFlowStart));

        HWREGB (icssgBaseAddr + CSL_ICSS_G_DRAM0_SLV_RAM_REGS_BASE + SPL_PKT_DEFAULT_PRIORITY) = 0;
        HWREGB (icssgBaseAddr + CSL_ICSS_G_DRAM0_SLV_RAM_REGS_BASE + QUEUE_NUM_UNTAGGED) = 0x4;
    }
    else
    {
        //Program constant table pointer for C28 on all 3 PRU cores
        HW_WR_REG32(icssgBaseAddr + CSL_ICSS_G_PR1_PDSP1_IRAM_REGS_BASE + CSL_ICSS_G_PR1_PDSP1_IRAM_CONSTANT_TABLE_PROG_PTR_0, 0x100U);
        HW_WR_REG32(icssgBaseAddr + CSL_ICSS_G_PR1_RTU1_PR1_RTU1_IRAM_REGS_BASE + CSL_ICSS_G_PR1_PDSP1_IRAM_CONSTANT_TABLE_PROG_PTR_0, 0x100U);
        HW_WR_REG32(icssgBaseAddr + CSL_ICSS_G_PR1_PDSP_TX1_IRAM_REGS_BASE + CSL_ICSS_G_PR1_PDSP1_IRAM_CONSTANT_TABLE_PROG_PTR_0, 0x100U);
        /* Program task manager configurations for all 3 PRUs */
        /* RX tasks run on RX_PRU1 */
        /*Configure RXBK1 Size = 20Bytes, RXBK2 Size = 12 Bytes, RXBKn Size = 32 Bytes. */
        HW_WR_REG32(icssgBaseAddr + 
                    CSL_ICSS_G_PR1_TASKS_MGR_PRU1_PR1_TASKS_MGR_PRU1_MMR_REGS_BASE +
                    CSL_ICSS_G_PR1_TASKS_MGR_PRU1_PR1_TASKS_MGR_PRU1_MMR_RX_CFG,
                    (19U << CSL_ICSS_G_PR1_TASKS_MGR_PRU1_PR1_TASKS_MGR_PRU1_MMR_RX_CFG_BK1_SIZE_SHIFT) |
                    (11U << CSL_ICSS_G_PR1_TASKS_MGR_PRU1_PR1_TASKS_MGR_PRU1_MMR_RX_CFG_BK2_SIZE_SHIFT) |
                    (31U << CSL_ICSS_G_PR1_TASKS_MGR_PRU1_PR1_TASKS_MGR_PRU1_MMR_RX_CFG_BKN_SIZE_SHIFT));
        /* RX FIFO WM configuration on RTU1 */ 
        HW_WR_REG32(icssgBaseAddr +
                    CSL_ICSS_G_PR1_TASKS_MGR_RTU1_PR1_TASKS_MGR_RTU1_MMR_REGS_BASE +
                    CSL_ICSS_G_PR1_TASKS_MGR_RTU1_PR1_TASKS_MGR_RTU1_MMR_RX_CFG,
                    (19U << CSL_ICSS_G_PR1_TASKS_MGR_RTU1_PR1_TASKS_MGR_RTU1_MMR_RX_CFG_BK1_SIZE_SHIFT) |
                    (11U << CSL_ICSS_G_PR1_TASKS_MGR_RTU1_PR1_TASKS_MGR_RTU1_MMR_RX_CFG_BK2_SIZE_SHIFT) |
                    (31U << CSL_ICSS_G_PR1_TASKS_MGR_RTU1_PR1_TASKS_MGR_RTU1_MMR_RX_CFG_BKN_SIZE_SHIFT));
        //Configure TX WM to zero
        HW_WR_REG32(icssgBaseAddr +
                    CSL_ICSS_G_PR1_TASKS_MGR_PRU_TX1_PR1_TASKS_MGR_PRU_TX1_MMR_REGS_BASE +
                    CSL_ICSS_G_PR1_TASKS_MGR_PRU_TX1_PR1_TASKS_MGR_PRU_TX1_MMR_TX_CFG,
                    30U);
        /*Enable task manager nesting for PRUs and RTUs for TS1 tasks only */
        regVal = 0x1FU;
        HW_WR_REG32(icssgBaseAddr +
                    CSL_ICSS_G_PR1_TASKS_MGR_PRU1_PR1_TASKS_MGR_PRU1_MMR_REGS_BASE +
                    CSL_ICSS_G_PR1_TASKS_MGR_PRU1_PR1_TASKS_MGR_PRU1_MMR_CAP_EN_CFG,
                    regVal);
        HW_WR_REG32(icssgBaseAddr +
                    CSL_ICSS_G_PR1_TASKS_MGR_RTU1_PR1_TASKS_MGR_RTU1_MMR_REGS_BASE +
                    CSL_ICSS_G_PR1_TASKS_MGR_RTU1_PR1_TASKS_MGR_RTU1_MMR_CAP_EN_CFG,
                    regVal);
        HW_WR_REG32(icssgBaseAddr +
                    CSL_ICSS_G_PR1_TASKS_MGR_PRU_TX1_PR1_TASKS_MGR_PRU_TX1_MMR_REGS_BASE +
                    CSL_ICSS_G_PR1_TASKS_MGR_PRU_TX1_PR1_TASKS_MGR_PRU_TX1_MMR_CAP_EN_CFG,
                    regVal);

        for (hwQueueNum = EMAC_ICSSG_MAX_HWQ; hwQueueNum < EMAC_ICSSG_MAX_HWQ*2; hwQueueNum++)
        {
            HW_WR_REG32(icssgBaseAddr +
                        CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_REGS_BASE +
                        CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_QUEUE_RESET,
                        hwQueueNum);
        }
        HW_WR_REG32(icssgBaseAddr +
                    CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_REGS_BASE +
                    CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_QUEUE_RESET,
                    EMAC_ICSSG_RECYCLE_Q_SLICE1);
        HW_WR_REG32(icssgBaseAddr +
                    CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_REGS_BASE +
                    CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_QUEUE_RESET,
                    EMAC_ICSSG_PORT_HF_Q_SLICE1);
       HW_WR_REG32(icssgBaseAddr +
                    CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_REGS_BASE +
                    CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_QUEUE_RESET,
                    EMAC_ICSSG_PORT_LF_Q_SLICE1);
        HW_WR_REG32(icssgBaseAddr +
                    CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_REGS_BASE +
                    CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_QUEUE_RESET,
                    EMAC_ICSSG_HOST_HF_Q_SLICE1);
        HW_WR_REG32(icssgBaseAddr +
                    CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_REGS_BASE +
                    CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_QUEUE_RESET,
                    EMAC_ICSSG_HOST_LF_Q_SLICE1);
        HW_WR_REG32(icssgBaseAddr +
                    CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_REGS_BASE +
                    CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_QUEUE_RESET,
                    EMAC_ICSSG_HOST_SF_Q_SLICE1);
        //Now do PDs PORTQ LOW & HI, HOSTQ LOW & HI
        for (i = 0; i < MaxNumNormalPDs; i++)
        {
            pdWord[0] = pdWord[0] & 0xFF00FFFFU; //Clear flags
            pdWord[0] = pdWord[0] | 0x800000U; //Set Pool, Slice ID
            pdAddr = PORT_DESC1_LO + i*EMAC_ICSSG_NORMAL_PD_SIZE;
            //Init PD Word
            emac_hw_mem_write ((icssgBaseAddr + CSL_ICSS_G_RAM_SLV_RAM_REGS_BASE + pdAddr), pdWord, EMAC_ICSSG_NORMAL_PD_SIZE_BYTE_COUNT);
            //Push to EMAC_ICSSG_PORT_LF_Q_SLICE1_OFFSET
            HW_WR_REG32(icssgBaseAddr +
                        CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_REGS_BASE +
                        CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_QUEUE0 +
                        EMAC_ICSSG_PORT_LF_Q_SLICE1_OFFSET, 
                        pdAddr);

            pdWord[0] = pdWord[0] & 0xFF00FFFFU; //Clear flags
            pdWord[0] = pdWord[0] | 0xA00000U; //Set Pool, Slice ID
            pdAddr = PORT_DESC1_HI + i*EMAC_ICSSG_NORMAL_PD_SIZE;
            //Init PD Word
            emac_hw_mem_write ((icssgBaseAddr + CSL_ICSS_G_RAM_SLV_RAM_REGS_BASE + pdAddr), pdWord, EMAC_ICSSG_NORMAL_PD_SIZE_BYTE_COUNT);
            //Push to EMAC_ICSSG_PORT_HF_Q_SLICE1off
            HW_WR_REG32(icssgBaseAddr +
                        CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_REGS_BASE +
                        CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_QUEUE0 +
                        EMAC_ICSSG_PORT_HF_Q_SLICE1off,
                        pdAddr);

            pdWord[0] = pdWord[0] & 0xFF00FFFFU; //Clear flags
            pdWord[0] = pdWord[0] | 0x800000U; //Set Pool, Slice ID
            pdAddr = HOST_DESC1_LO + i*EMAC_ICSSG_NORMAL_PD_SIZE;
            //Init PD Word
            emac_hw_mem_write ((icssgBaseAddr + CSL_ICSS_G_RAM_SLV_RAM_REGS_BASE + pdAddr), pdWord, EMAC_ICSSG_NORMAL_PD_SIZE_BYTE_COUNT);
            //Push to EMAC_ICSSG_HOST_LF_Q_SLICE1_OFFSET
            HW_WR_REG32(icssgBaseAddr +
                        CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_REGS_BASE +
                        CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_QUEUE0 +
                        EMAC_ICSSG_HOST_LF_Q_SLICE1_OFFSET,
                        pdAddr);

            pdWord[0] = pdWord[0] & 0xFF00FFFFU; //Clear flags
            pdWord[0] = pdWord[0] | 0xA00000U; //Set Pool, Slice ID
            pdAddr = HOST_DESC1_HI + i*EMAC_ICSSG_NORMAL_PD_SIZE;
            //Init PD Word
            emac_hw_mem_write ((icssgBaseAddr + CSL_ICSS_G_RAM_SLV_RAM_REGS_BASE + pdAddr), pdWord, EMAC_ICSSG_NORMAL_PD_SIZE_BYTE_COUNT);
            //Push to EMAC_ICSSG_HOST_HF_Q_SLICE1_OFFSET
            HW_WR_REG32(icssgBaseAddr +
                        CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_REGS_BASE +
                        CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_QUEUE0 + 
                        EMAC_ICSSG_HOST_HF_Q_SLICE1_OFFSET,
                        pdAddr);
        }
        //Create special PD pools
        for (i = 0; i < MaxNumSpecialPDs; i++)
        {
            pdWord[0] = pdWord[0] & 0xFF00FFFFU; //Clear flags
            pdWord[0] = pdWord[0] | 0xC00000U; //Set Pool, Slice ID
            pdAddr = HOST_SPPD1 + i*EMAC_ICSSG_SPECIAL_PD_SIZE;
            //Init PD Word
            emac_hw_mem_write ((icssgBaseAddr + CSL_ICSS_G_RAM_SLV_RAM_REGS_BASE + pdAddr), pdWord, EMAC_ICSSG_SPECIAL_PD_SIZE_BYTE_COUNT);
            //Push to EMAC_ICSSG_HOST_SF_Q_SLICE1_OFFSET
            HW_WR_REG32(icssgBaseAddr +
                        CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_REGS_BASE +
                        CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_QUEUE0 +
                        EMAC_ICSSG_HOST_SF_Q_SLICE1_OFFSET,
                        pdAddr);
        }
                //Initialize 24 Buffer Pools per slice
        EMAC_ICSSG_BUF_POOL_CFG.poolBase = pIcssgFwCfg->bufferPoolLowAddr;
        for (i = EMAC_ICSSG_DUAL_MAC_START_BUFFER_POOL_NUM; i < (EMAC_ICSSG_DUAL_MAC_START_BUFFER_POOL_NUM +pIcssgFwCfg->numBufferPool); i++)
        {
            EMAC_ICSSG_BUF_POOL_CFG.poolLen = pIcssgFwCfg->bufferPoolSize[i-EMAC_ICSSG_DUAL_MAC_START_BUFFER_POOL_NUM];
            emac_hw_mem_write ((icssgBaseAddr + CSL_ICSS_G_DRAM1_SLV_RAM_REGS_BASE + BUFFER_POOL_0_ADDR_OFFSET + i*8), &EMAC_ICSSG_BUF_POOL_CFG, EMAC_ICSSG_NORMAL_PD_SIZE_BYTE_COUNT);
            EMAC_ICSSG_BUF_POOL_CFG.poolBase += pIcssgFwCfg->bufferPoolSize[i-EMAC_ICSSG_DUAL_MAC_START_BUFFER_POOL_NUM];
        }
        startAddr = EMAC_ICSSG_BUF_POOL_CFG.poolBase;
        endAddr =startAddr + 0x4000;
        HW_WR_REG32(icssgBaseAddr + CSL_ICSS_G_DRAM1_SLV_RAM_REGS_BASE + HOST_RX_Q_PRE_CONTEXT_OFFSET, startAddr);
        HW_WR_REG32(icssgBaseAddr + CSL_ICSS_G_DRAM1_SLV_RAM_REGS_BASE + HOST_RX_Q_PRE_CONTEXT_OFFSET + 4U, startAddr);
        HW_WR_REG32(icssgBaseAddr + CSL_ICSS_G_DRAM1_SLV_RAM_REGS_BASE + HOST_RX_Q_PRE_CONTEXT_OFFSET + 8U, startAddr);
        HW_WR_REG32(icssgBaseAddr + CSL_ICSS_G_DRAM1_SLV_RAM_REGS_BASE + HOST_RX_Q_PRE_CONTEXT_OFFSET + 12U, endAddr);

        //Configure PSI flow info for firmware
        //SMEM
        //#define PSI_L_REGULAR_FLOW_ID_BASE_OFFSET
        //#define PSI_L_MGMT_FLOW_ID_OFFSET
        //10 flows per slice

        HW_WR_REG32(icssgBaseAddr + CSL_ICSS_G_DRAM1_SLV_RAM_REGS_BASE + PSI_L_REGULAR_FLOW_ID_BASE_OFFSET,
                        ((pIcssgFwCfg->rxMgmtFlowStart) << 16) |
                         (pIcssgFwCfg->rxPktFlowStart));
        HWREGB (icssgBaseAddr + CSL_ICSS_G_DRAM1_SLV_RAM_REGS_BASE + SPL_PKT_DEFAULT_PRIORITY) = 0;
        HWREGB (icssgBaseAddr + CSL_ICSS_G_DRAM1_SLV_RAM_REGS_BASE + QUEUE_NUM_UNTAGGED) = 0x4;
     }

    //Program RXCFG0/1 and TXCFG0/1
    HW_WR_REG32(icssgBaseAddr + CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_CFG_REGS_BASE+CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_CFG_RXCFG0, 0x213U);
    HW_WR_REG32(icssgBaseAddr + CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_CFG_REGS_BASE+CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_CFG_RXCFG1, 0x21BU);
    HW_WR_REG32(icssgBaseAddr + CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_CFG_REGS_BASE+CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_CFG_TXCFG0, 0x1803U);
    HW_WR_REG32(icssgBaseAddr + CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_CFG_REGS_BASE+CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_CFG_TXCFG1, 0x1903U);
    //Program TX_IPG0/IPG1
    HW_WR_REG32(icssgBaseAddr + CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_CFG_REGS_BASE+CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_CFG_TX_IPG1, 0xBU);
    HW_WR_REG32(icssgBaseAddr + CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_CFG_REGS_BASE+CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_CFG_TX_IPG0, 0xBU);
    //Reset Max preamble count
    HW_WR_REG32(icssgBaseAddr + CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_CFG_REGS_BASE+CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_CFG_RX_PCNT0, 0x1U);
    HW_WR_REG32(icssgBaseAddr + CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_CFG_REGS_BASE+CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_CFG_RX_PCNT1, 0x1U);
    //Init RGMII config for ICSSG : TXL2, TXPRU enable etc
    HW_WR_REG32(icssgBaseAddr +
                CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_REGS_BASE +
                CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_ICSS_G_CFG,
                0x1082FU);
}

EMAC_PER_PORT_ICSSG_FW_CFG emacFwCfgMmap[EMAC_MAX_ICSS*2] = {
    {
        {0,0,0},
        (struct EMAC_FW_PORT_CFG*)&emac_dualmac_cfg_pg1,
        emac_icssg_dual_mac_fw_config_fxn,
    },
    {
        {0,0,0},
        (struct EMAC_FW_PORT_CFG*)&emac_dualmac_cfg_pg1,
        emac_icssg_dual_mac_fw_config_fxn,
    },
    {
        {0,0,0},
        (struct EMAC_FW_PORT_CFG*)&emac_dualmac_cfg_pg1,
        emac_icssg_dual_mac_fw_config_fxn,
    },    
    {
        {0,0,0},
        (struct EMAC_FW_PORT_CFG*)&emac_dualmac_cfg_pg1,
        emac_icssg_dual_mac_fw_config_fxn,
    },
    {
        {0,0,0},
        (struct EMAC_FW_PORT_CFG*)&emac_dualmac_cfg_pg1,
        emac_icssg_dual_mac_fw_config_fxn,
    },
    {
        {0,0,0},
        (struct EMAC_FW_PORT_CFG*)&emac_dualmac_cfg_pg1,
        emac_icssg_dual_mac_fw_config_fxn
    }
};

/**
 * \brief Gets the EMAC dual mac firmware configuration
 *
 *  @param[in]  port_num      port number
 *  @param[in]  pFwCfg  Address of pointer to be set to firmware configuration structure
 *
 * \return 0 success: -1: error
 *
 */
int32_t emacGetDualMacFwConfig(uint32_t portNum, EMAC_PER_PORT_ICSSG_FW_CFG **pFwCfg)
{
    int32_t ret = -1;
    if ((pFwCfg != NULL) && (portNum < (EMAC_MAX_ICSS*2)))
    {
        *pFwCfg = &(emacFwCfgMmap[portNum]);
        ret = 0;
    }
    return ret;
}

/**
 * \brief Sets the EMAC dual mac firmware configuration
 *
 *  @param[in]  port_num      port number
 *  @param[in]  pFwCfg  Pointer to  firmware configuration to be set
 *
 * \return 0 success: -1: error
 *
 */
int32_t emacSetDualMacFwConfig(uint32_t portNum, const EMAC_PER_PORT_ICSSG_FW_CFG *pFwCfg)
{
    int32_t ret = -1;
    if ((pFwCfg != NULL) && (portNum < (EMAC_MAX_ICSS*2)))
    {
         memcpy(&(emacFwCfgMmap[portNum]),pFwCfg, sizeof(EMAC_PER_PORT_ICSSG_FW_CFG));
        ret = 0;
    }
    return ret;
}

/**
 * \brief Gets the application provided EMAC dual mac firmware configuration
 *
 *  @param[in]  port_num      port number
 *  @param[in]  pFwCfg  Address of pointer to be set to application provided firmware configuration structure
 *
 * \return 0 success: -1: error
 *
 */
int32_t emacGetDualMacFwAppInitCfg(uint32_t portNum, EMAC_FW_APP_CONFIG **pFwAppCfg)
{
    int32_t ret = -1;
    if ((pFwAppCfg != NULL) && (portNum < (EMAC_MAX_ICSS*2)))
    {
        *pFwAppCfg = &(emacFwCfgMmap[portNum].fwAppCfg);
        ret = 0;
    }
    return ret;
}

/**
 * \brief Sets the application provided EMAC dual mac firmware configuration
 *
 *  @param[in]  port_num      port number
 *  @param[in]  pFwCfg  Pointer to  application provided firmware configuration to be set
 *
 * \return 0 success: -1: error
 *
 */
int32_t emacSetDualMacFwAppInitCfg(uint32_t portNum, EMAC_FW_APP_CONFIG *pFwAppCfg)
{
   int32_t ret = -1;

    if ((pFwAppCfg != NULL) && (portNum < (EMAC_MAX_ICSS*2)))
    {
        emacFwCfgMmap[portNum].fwAppCfg.txPortQueueHighAddr = pFwAppCfg->txPortQueueHighAddr;
        emacFwCfgMmap[portNum].fwAppCfg.txPortQueueLowAddr = pFwAppCfg->txPortQueueLowAddr;
        emacFwCfgMmap[portNum].fwAppCfg.bufferPoolHighAddr = pFwAppCfg->bufferPoolHighAddr;
        emacFwCfgMmap[portNum].fwAppCfg.bufferPoolLowAddr = pFwAppCfg->bufferPoolLowAddr;
        ret = 0;
     }
     return ret;
}




