/*
*
* Copyright (c) 2021 Texas Instruments Incorporated
*
* All rights reserved not granted herein.
*
* Limited License.
*
* Texas Instruments Incorporated grants a world-wide, royalty-free, non-exclusive
* license under copyrights and patents it now or hereafter owns or controls to make,
* have made, use, import, offer to sell and sell ("Utilize") this software subject to the
* terms herein.  With respect to the foregoing patent license, such license is granted
* solely to the extent that any such patent is necessary to Utilize the software alone.
* The patent license shall not apply to any combinations which include this software,
* other than combinations with devices manufactured by or for TI ("TI Devices").
* No hardware patent is licensed hereunder.
*
* Redistributions must preserve existing copyright notices and reproduce this license
* (including the above copyright notice and the disclaimer and (if applicable) source
* code license limitations below) in the documentation and/or other materials provided
* with the distribution
*
* Redistribution and use in binary form, without modification, are permitted provided
* that the following conditions are met:
*
* *       No reverse engineering, decompilation, or disassembly of this software is
* permitted with respect to any software provided in binary form.
*
* *       any redistribution and use are licensed by TI for use only with TI Devices.
*
* *       Nothing shall obligate TI to provide you with source code for the software
* licensed and provided to you in object code.
*
* If software source code is provided to you, modification and redistribution of the
* source code are permitted provided that the following conditions are met:
*
* *       any redistribution and use of the source code, including any resulting derivative
* works, are licensed by TI for use only with TI Devices.
*
* *       any redistribution and use of any object code compiled from the source code
* and any resulting derivative works, are licensed by TI for use only with TI Devices.
*
* Neither the name of Texas Instruments Incorporated nor the names of its suppliers
*
* may be used to endorse or promote products derived from this software without
* specific prior written permission.
*
* DISCLAIMER.
*
* THIS SOFTWARE IS PROVIDED BY TI AND TI'S LICENSORS "AS IS" AND ANY EXPRESS
* OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL TI AND TI'S LICENSORS BE LIABLE FOR ANY DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
* OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
* OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
* OF THE POSSIBILITY OF SUCH DAMAGE.
*
*/

/*! \file dmsc.h
 *  \brief Offsets from dmsc sub-system base address and register default values.
 *  \author Raja Velamuri velamuri@ti.com
 *  \author Matt Clark matt.clark@ti.com
 *
 *  \defgroup DMSC
 */

#ifndef DMSC_H
#define DMSC_H

#define DMSC_REG_MISMATCH           (0x00000001)
#define DMSC_CSLR_MMR_TEST          (0x00010000)

#define DMSC_PWR_CTRL_OFFSET        (0x00130000)
#define DMSC_DMTIMER0_OFFSET        (0x00133000)
#define DMSC_DMTIMER1_OFFSET        (0x00134000)
#define DMSC_RTIWWDT_OFFSET         (0x00135000)
#define DMSC_RTI_CTRL_OFFSET        (0x00135100)
#define DMSC_RAT_OFFSET             (0x00200000)
#define DMSC_ECCAGGR_OFFSET         (0x00202000)
#define DMSC_SEC_CTRL_OFFSET        (0x00230000)
#define DMSC_DBG_AUTH_OFFSET        (0x00232000)
#define DMSC_SEC_MGR_OFFSET         (0x00234000)
#define DMSC_DMTIMER2_OFFSET        (0x00238000)
#define DMSC_DMTIMER3_OFFSET        (0x00239000)
#define DMSC_AES_OFFSET             (0x0023c000)
#define DMSC_IA_INTR_OFFSET         (0x00300000)
#define DMSC_IA_IMAP_OFFSET         (0x00400000)
#define DMSC_IA_CFG_OFFSET          (0x00410000)

#define DMSC_ROM_OFFSET             (0x00000000)
#define DMSC_ROM_SIZE               (160*1024)   /* 0x0_0000 -> 0x2_7FFF */
#define DMSC_IRAM_OFFSET            (0x00040000)
#define DMSC_IRAM_SIZE              (192*1024)   /* 0x4_0000 -> 0x6_FFFF */
#define DMSC_DRAM_OFFSET            (0x00070000)
#define DMSC_DRAM_SIZE              (80*1024)    /* 0x7_0000 -> 0x8_3FFF */
#define DMSC_DRAM_0_OFFSET          (0x00070000)
#define DMSC_DRAM_0_SIZE            (64*1024)    /* 0x7_0000 -> 0x7_FFFF */
#define DMSC_DRAM_1_OFFSET          (0x00080000)
#define DMSC_DRAM_1_SIZE            (16*1024)    /* 0x8_0000 -> 0x8_3FFF */

#define DMSC_ROM_REGION_0_OFFSET    (0x00000000)
#define DMSC_ROM_REGION_1_OFFSET    (0x00010000)
#define DMSC_ROM_REGION_2_OFFSET    (0x00020000)
#define DMSC_ROM_REGION_3_OFFSET    (0x00030000)

#define DMSC_IRAM_REGION_0_OFFSET   (0x00000000)
#define DMSC_IRAM_REGION_1_OFFSET   (0x00004000)
#define DMSC_IRAM_REGION_2_OFFSET   (0x00008000)
#define DMSC_IRAM_REGION_3_OFFSET   (0x0000c000)
#define DMSC_IRAM_REGION_4_OFFSET   (0x00010000)
#define DMSC_IRAM_REGION_5_OFFSET   (0x00014000)
#define DMSC_IRAM_REGION_6_OFFSET   (0x00018000)
#define DMSC_IRAM_REGION_7_OFFSET   (0x0001c000)
#define DMSC_IRAM_REGION_8_OFFSET   (0x00030000)
#define DMSC_IRAM_REGION_9_OFFSET   (0x00034000)
#define DMSC_IRAM_REGION_10_OFFSET  (0x00038000)
#define DMSC_IRAM_REGION_11_OFFSET  (0x0003c000)

#define DMSC_DRAM_0_REGION_0_OFFSET (0x00000000)
#define DMSC_DRAM_0_REGION_1_OFFSET (0x00004000)
#define DMSC_DRAM_0_REGION_2_OFFSET (0x00008000)
#define DMSC_DRAM_0_REGION_3_OFFSET (0x0000c000)

#define DMSC_DRAM_1_REGION_0_OFFSET (0x00000000)

/* msgmgr offsets */
#define DMSC_MSGMGR_GCFG_OFFSET     (0x003a0000)
#define DMSC_MSGMGR_SCFG_OFFSET     (0x003a2000)
#define DMSC_MSGMGR_QDRAM_OFFSET    (0x003ac000)
#define DMSC_MSGMGR_QRAM_OFFSET     (0x003a8000)
#define DMSC_MSGMGR_LRAM_OFFSET     (0x003a4000)
#define DMSC_MSGMGR_QUEUE_OFFSET    (0x003b0000)
#define DMSC_MSGMGR_QPEEK_OFFSET    (0x003b4000)
#define DMSC_MSGMGR_QDIVERT_OFFSET  (0x003b8000)

/* proxy offsets */
#define DMSC_PROXY_CFG_OFFSET       (0x003f0000)
#define DMSC_PROXY_BUF_OFFSET       (0x003f4000)
#define DMSC_PROXY_TARGET0_OFFSET   (0x00400000)

/* firewall scr */
#define DMSC_FIREWALL_CFG_OFFSET    (0x01000000)
#define DMSC_ISC_CFG_OFFSET         (0x01800000)
#define DMSC_EMU_DBG_CTRL_OFFSET    (0x01900000)
#define DMSC_FIREWALL_EXC_OFFSET    (0x01b00000)
#define DMSC_QOS_CFG_OFFSET         (0x01d00000)
#define DMSC_VIRT_CFG_OFFSET        (0x01f00000)

/* Reset values. */
#define DMSC_CM_PID_RST_VAL         (0x6180020C)
#define DMSC_CM_REV_RST_VAL         (0x66E03903)

#define DMSC_SEC_CM_PID_RST_VAL     (0x6180020A)

#define DMSC_WWRTI_PID_RST_VAL      (0x6180020A)

#define DMSC_RAT_PID_RST_VAL        (0x66802900)

#define DMSC_ECCAGGR_PID_RST_VAL    (0x66A0E200)

/* PWR_CTRL MMR UNLOCK values */
#define DMSC_CM_LOCK0_KICK0_UNLOCK_VALUE        (0x8a6b7cdau)
#define DMSC_CM_LOCK0_KICK1_UNLOCK_VALUE        (0x823caef9u)

/* SEC_CTRL MMR UNLOCK values */
#define DMSC_SEC_CM_LOCK0_KICK0_UNLOCK_VALUE    (0xf457c023u)
#define DMSC_SEC_CM_LOCK0_KICK1_UNLOCK_VALUE    (0x93b0a410u)

/* WWRTI_CTRL MMR UNLOCK values */
#define DMSC_WWRTI_LOCK0_KICK0_UNLOCK_VALUE     (0x4658fc21u)
#define DMSC_WWRTI_LOCK0_KICK1_UNLOCK_VALUE     (0x3ac4f102u)

/* SEC_CTRL MMR warm and cold reset codes */
#define DMSC_SEC_CM_SOC_COLD_RESET_CODE_VALUE   (0x72594681u)
#define DMSC_SEC_CM_SOC_WARM_RESET_CODE_VALUE   (0x41324052u)

/* SEC_CTRL MMR SEC_ROM_ADDR TIE-OFF values */
#define DMSC_SEC_CM_ROM_START_ADDR_TIEOFF_VALUE (0x00018000u)
#define DMSC_SEC_CM_ROM_END_ADDR_TIEOFF_VALUE   (0x00027FBFu)

#define RAT_EXCEPTION_PEND_SET                  (0x840U)
#define RAT_EXCEPTION_PEND_CLEAR                (0x844U)
#define RAT_EXCEPTION_ENABLE_SET                (0x848U)
#define RAT_EXCEPTION_ENABLE_CLEAR              (0x84CU)

#define NVIC_RAT_EXCEPTION_INTR                 11
#define NVIC_ECC_CORRECTABLE_INTR               12
#define NVIC_ECC_UNCORRECTABLE_INTR             13
#define NVIC_DEBUG_FORCEACTIVE_INTR             26

/* based on resolution of Jacinto7_J7.BTS_Jacinto7_J7_rtl.669 */
#define NUM_DMSC_ECC_INTR                       46

/* DMSC top-level interrupt port to NVIC port translation. */
#define DMSC_INT_0              NVIC_IN_176
#define DMSC_INT_1              NVIC_IN_177
#define DMSC_INT_2              NVIC_IN_178
#define DMSC_INT_3              NVIC_IN_179
#define DMSC_INT_4              NVIC_IN_180
#define DMSC_INT_5              NVIC_IN_181
#define DMSC_INT_6              NVIC_IN_182
#define DMSC_INT_7              NVIC_IN_183
#define DMSC_INT_8              NVIC_IN_184
#define DMSC_INT_9              NVIC_IN_185
#define DMSC_INT_10             NVIC_IN_186
#define DMSC_INT_11             NVIC_IN_187
#define DMSC_INT_12             NVIC_IN_188
#define DMSC_INT_13             NVIC_IN_189
#define DMSC_INT_14             NVIC_IN_190
#define DMSC_INT_15             NVIC_IN_191
#define DMSC_INT_16             NVIC_IN_192
#define DMSC_INT_17             NVIC_IN_193
#define DMSC_INT_18             NVIC_IN_194
#define DMSC_INT_19             NVIC_IN_195
#define DMSC_INT_20             NVIC_IN_196
#define DMSC_INT_21             NVIC_IN_197
#define DMSC_INT_22             NVIC_IN_198
#define DMSC_INT_23             NVIC_IN_199
#define DMSC_INT_24             NVIC_IN_200
#define DMSC_INT_25             NVIC_IN_201
#define DMSC_INT_26             NVIC_IN_202
#define DMSC_INT_27             NVIC_IN_203
#define DMSC_INT_28             NVIC_IN_204
#define DMSC_INT_29             NVIC_IN_205
#define DMSC_INT_30             NVIC_IN_206
#define DMSC_INT_31             NVIC_IN_207
#define DMSC_INT_32             NVIC_IN_208
#define DMSC_INT_33             NVIC_IN_209
#define DMSC_INT_34             NVIC_IN_210
#define DMSC_INT_35             NVIC_IN_211
#define DMSC_INT_36             NVIC_IN_212
#define DMSC_INT_37             NVIC_IN_213
#define DMSC_INT_38             NVIC_IN_214
#define DMSC_INT_39             NVIC_IN_215
#define DMSC_INT_40             NVIC_IN_216
#define DMSC_INT_41             NVIC_IN_217
#define DMSC_INT_42             NVIC_IN_218
#define DMSC_INT_43             NVIC_IN_219
#define DMSC_INT_44             NVIC_IN_220
#define DMSC_INT_45             NVIC_IN_221
#define DMSC_INT_46             NVIC_IN_222
#define DMSC_INT_47             NVIC_IN_223
#define DMSC_INT_48             NVIC_IN_224
#define DMSC_INT_49             NVIC_IN_225
#define DMSC_INT_50             NVIC_IN_226
#define DMSC_INT_51             NVIC_IN_227
#define DMSC_INT_52             NVIC_IN_228
#define DMSC_INT_53             NVIC_IN_229
#define DMSC_INT_54             NVIC_IN_230
#define DMSC_INT_55             NVIC_IN_231
#define DMSC_INT_56             NVIC_IN_232
#define DMSC_INT_57             NVIC_IN_233
#define DMSC_INT_58             NVIC_IN_234
#define DMSC_INT_59             NVIC_IN_235
#define DMSC_INT_60             NVIC_IN_236
#define DMSC_INT_61             NVIC_IN_237
#define DMSC_INT_62             NVIC_IN_238
#define DMSC_INT_63             NVIC_IN_239

/* Sunder to fix this and remove the comment rramesh@ti.com */
#define DMSC_SEC_IN_0           NVIC_IN_47
#define DMSC_SEC_IN_1           NVIC_IN_47
#define DMSC_SEC_IN_2           NVIC_IN_47
#define DMSC_SEC_IN_3           NVIC_IN_47
#define DMSC_SEC_IN_4           NVIC_IN_47
#define DMSC_SEC_IN_5           NVIC_IN_47
#define DMSC_SEC_IN_6           NVIC_IN_47
#define DMSC_SEC_IN_7           NVIC_IN_47
#define DMSC_SEC_IN_8           NVIC_IN_47
#define DMSC_SEC_IN_9           NVIC_IN_47
#define DMSC_SEC_IN_10          NVIC_IN_47
#define DMSC_SEC_IN_11          NVIC_IN_47
#define DMSC_SEC_IN_12          NVIC_IN_47
#define DMSC_SEC_IN_13          NVIC_IN_47
#define DMSC_SEC_IN_14          NVIC_IN_47
#define DMSC_SEC_IN_15          NVIC_IN_47
#define DMSC_SEC_IN_16          NVIC_IN_47
#define DMSC_SEC_IN_17          NVIC_IN_47
#define DMSC_SEC_IN_18          NVIC_IN_47
#define DMSC_SEC_IN_19          NVIC_IN_47
#define DMSC_SEC_IN_20          NVIC_IN_47
#define DMSC_SEC_IN_21          NVIC_IN_47
#define DMSC_SEC_IN_22          NVIC_IN_47
#define DMSC_SEC_IN_23          NVIC_IN_47
#define DMSC_SEC_IN_24          NVIC_IN_47
#define DMSC_SEC_IN_25          NVIC_IN_47
#define DMSC_SEC_IN_26          NVIC_IN_47
#define DMSC_SEC_IN_27          NVIC_IN_47
#define DMSC_SEC_IN_28          NVIC_IN_47
#define DMSC_SEC_IN_29          NVIC_IN_47
#define DMSC_SEC_IN_30          NVIC_IN_47
#define DMSC_SEC_IN_31          NVIC_IN_47
#define DMSC_SEC_IN_32          NVIC_IN_47
#define DMSC_SEC_IN_33          NVIC_IN_47
#define DMSC_SEC_IN_34          NVIC_IN_47
#define DMSC_SEC_IN_35          NVIC_IN_47
#define DMSC_SEC_IN_36          NVIC_IN_47
#define DMSC_SEC_IN_37          NVIC_IN_47
#define DMSC_SEC_IN_38          NVIC_IN_47
#define DMSC_SEC_IN_39          NVIC_IN_47
#define DMSC_SEC_IN_40          NVIC_IN_47
#define DMSC_SEC_IN_41          NVIC_IN_47
#define DMSC_SEC_IN_42          NVIC_IN_47
#define DMSC_SEC_IN_43          NVIC_IN_47
#define DMSC_SEC_IN_44          NVIC_IN_47
#define DMSC_SEC_IN_45          NVIC_IN_47
#define DMSC_SEC_IN_46          NVIC_IN_47
#define DMSC_SEC_IN_47          NVIC_IN_47
#define DMSC_SEC_IN_48          NVIC_IN_47
#define DMSC_SEC_IN_49          NVIC_IN_47
#define DMSC_SEC_IN_50          NVIC_IN_47
#define DMSC_SEC_IN_51          NVIC_IN_47
#define DMSC_SEC_IN_52          NVIC_IN_47
#define DMSC_SEC_IN_53          NVIC_IN_47
#define DMSC_SEC_IN_54          NVIC_IN_47
#define DMSC_SEC_IN_55          NVIC_IN_47
#define DMSC_SEC_IN_56          NVIC_IN_47
#define DMSC_SEC_IN_57          NVIC_IN_47
#define DMSC_SEC_IN_58          NVIC_IN_47
#define DMSC_SEC_IN_59          NVIC_IN_47
#define DMSC_SEC_IN_60          NVIC_IN_47
#define DMSC_SEC_IN_61          NVIC_IN_47
#define DMSC_SEC_IN_62          NVIC_IN_47




#endif
