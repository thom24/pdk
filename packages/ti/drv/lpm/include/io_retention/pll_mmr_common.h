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

#ifndef _PLL_MMR_COMMON_H_
#define _PLL_MMR_COMMON_H_
#include <ti/drv/lpm/include/io_retention/base_address.h>
#define MMR_PLL0_KICK0_UNLOCK_VAL                (0x68EF3490)
#define MMR_PLL0_KICK1_UNLOCK_VAL                (0xD172BC5A)
#define MMR_PLL0_LOCKKEY0                        (0x00010)
#define MMR_PLL0_LOCKKEY1                        (0x00014)
#define MMR_PLL10_KICK0_UNLOCK_VAL               (0x68EF3490)
#define MMR_PLL10_KICK1_UNLOCK_VAL               (0xD172BC5A)
#define MMR_PLL10_LOCKKEY0                       (0x0A010)
#define MMR_PLL10_LOCKKEY1                       (0x0A014)
#define MMR_PLL12_KICK0_UNLOCK_VAL               (0x68EF3490)
#define MMR_PLL12_KICK1_UNLOCK_VAL               (0xD172BC5A)
#define MMR_PLL12_LOCKKEY0                       (0x0C010)
#define MMR_PLL12_LOCKKEY1                       (0x0C014)
#define MMR_PLL13_KICK0_UNLOCK_VAL               (0x68EF3490)
#define MMR_PLL13_KICK1_UNLOCK_VAL               (0xD172BC5A)
#define MMR_PLL13_LOCKKEY0                       (0x0D010)
#define MMR_PLL13_LOCKKEY1                       (0x0D014)
#define MMR_PLL14_KICK0_UNLOCK_VAL               (0x68EF3490)
#define MMR_PLL14_KICK1_UNLOCK_VAL               (0xD172BC5A)
#define MMR_PLL14_LOCKKEY0                       (0x0E010)
#define MMR_PLL14_LOCKKEY1                       (0x0E014)
#define MMR_PLL15_KICK0_UNLOCK_VAL               (0x68EF3490)
#define MMR_PLL15_KICK1_UNLOCK_VAL               (0xD172BC5A)
#define MMR_PLL15_LOCKKEY0                       (0x0E010)
#define MMR_PLL15_LOCKKEY1                       (0x0E014)
#define MMR_PLL16_KICK0_UNLOCK_VAL               (0x68EF3490)
#define MMR_PLL16_KICK1_UNLOCK_VAL               (0xD172BC5A)
#define MMR_PLL16_LOCKKEY0                       (0x10010)
#define MMR_PLL16_LOCKKEY1                       (0x10014)
#define MMR_PLL17_KICK0_UNLOCK_VAL               (0x68EF3490)
#define MMR_PLL17_KICK1_UNLOCK_VAL               (0xD172BC5A)
#define MMR_PLL17_LOCKKEY0                       (0x11010)
#define MMR_PLL17_LOCKKEY1                       (0x11014)
#define MMR_PLL18_KICK0_UNLOCK_VAL               (0x68EF3490)
#define MMR_PLL18_KICK1_UNLOCK_VAL               (0xD172BC5A)
#define MMR_PLL18_LOCKKEY0                       (0x12010)
#define MMR_PLL18_LOCKKEY1                       (0x12014)
#define MMR_PLL19_KICK0_UNLOCK_VAL               (0x68EF3490)
#define MMR_PLL19_KICK1_UNLOCK_VAL               (0xD172BC5A)
#define MMR_PLL19_LOCKKEY0                       (0x13010)
#define MMR_PLL19_LOCKKEY1                       (0x13014)
#define MMR_PLL1_KICK0_UNLOCK_VAL                (0x68EF3490)
#define MMR_PLL1_KICK1_UNLOCK_VAL                (0xD172BC5A)
#define MMR_PLL1_LOCKKEY0                        (0x01010)
#define MMR_PLL1_LOCKKEY1                        (0x01014)
#define MMR_PLL20_KICK0_UNLOCK_VAL               (0x68EF3490)
#define MMR_PLL20_KICK1_UNLOCK_VAL               (0xD172BC5A)
#define MMR_PLL20_LOCKKEY0                       (0x14010)
#define MMR_PLL20_LOCKKEY1                       (0x14014)
#define MMR_PLL21_KICK0_UNLOCK_VAL               (0x68EF3490)
#define MMR_PLL21_KICK1_UNLOCK_VAL               (0xD172BC5A)
#define MMR_PLL21_LOCKKEY0                       (0x15010)
#define MMR_PLL21_LOCKKEY1                       (0x15014)
#define MMR_PLL22_KICK0_UNLOCK_VAL               (0x68EF3490)
#define MMR_PLL22_KICK1_UNLOCK_VAL               (0xD172BC5A)
#define MMR_PLL22_LOCKKEY0                       (0x16010)
#define MMR_PLL22_LOCKKEY1                       (0x16014)
#define MMR_PLL23_KICK0_UNLOCK_VAL               (0x68EF3490)
#define MMR_PLL23_KICK1_UNLOCK_VAL               (0xD172BC5A)
#define MMR_PLL23_LOCKKEY0                       (0x17010)
#define MMR_PLL23_LOCKKEY1                       (0x17014)
#define MMR_PLL24_KICK0_UNLOCK_VAL               (0x68EF3490)
#define MMR_PLL24_KICK1_UNLOCK_VAL               (0xD172BC5A)
#define MMR_PLL24_LOCKKEY0                       (0x18010)
#define MMR_PLL24_LOCKKEY1                       (0x18014)
#define MMR_PLL25_KICK0_UNLOCK_VAL               (0x68EF3490)
#define MMR_PLL25_KICK1_UNLOCK_VAL               (0xD172BC5A)
#define MMR_PLL25_LOCKKEY0                       (0x18010)
#define MMR_PLL25_LOCKKEY1                       (0x18014)
#define MMR_PLL2_KICK0_UNLOCK_VAL                (0x68EF3490)
#define MMR_PLL2_KICK1_UNLOCK_VAL                (0xD172BC5A)
#define MMR_PLL2_LOCKKEY0                        (0x02010)
#define MMR_PLL2_LOCKKEY1                        (0x02014)
#define MMR_PLL3_KICK0_UNLOCK_VAL                (0x68EF3490)
#define MMR_PLL3_KICK1_UNLOCK_VAL                (0xD172BC5A)
#define MMR_PLL3_LOCKKEY0                        (0x03010)
#define MMR_PLL3_LOCKKEY1                        (0x03014)
#define MMR_PLL4_KICK0_UNLOCK_VAL                (0x68EF3490)
#define MMR_PLL4_KICK1_UNLOCK_VAL                (0xD172BC5A)
#define MMR_PLL4_LOCKKEY0                        (0x04010)
#define MMR_PLL4_LOCKKEY1                        (0x04014)
#define MMR_PLL5_KICK0_UNLOCK_VAL                (0x68EF3490)
#define MMR_PLL5_KICK1_UNLOCK_VAL                (0xD172BC5A)
#define MMR_PLL5_LOCKKEY0                        (0x05010)
#define MMR_PLL5_LOCKKEY1                        (0x05014)
#define MMR_PLL6_KICK0_UNLOCK_VAL                (0x68EF3490)
#define MMR_PLL6_KICK1_UNLOCK_VAL                (0xD172BC5A)
#define MMR_PLL6_LOCKKEY0                        (0x06010)
#define MMR_PLL6_LOCKKEY1                        (0x06014)
#define MMR_PLL7_KICK0_UNLOCK_VAL                (0x68EF3490)
#define MMR_PLL7_KICK1_UNLOCK_VAL                (0xD172BC5A)
#define MMR_PLL7_LOCKKEY0                        (0x07010)
#define MMR_PLL7_LOCKKEY1                        (0x07014)
#define MMR_PLL8_KICK0_UNLOCK_VAL                (0x68EF3490)
#define MMR_PLL8_KICK1_UNLOCK_VAL                (0xD172BC5A)
#define MMR_PLL8_LOCKKEY0                        (0x08010)
#define MMR_PLL8_LOCKKEY1                        (0x08014)
#define MMR_PLL9_KICK0_UNLOCK_VAL                (0x68EF3490)
#define MMR_PLL9_KICK1_UNLOCK_VAL                (0xD172BC5A)
#define MMR_PLL9_LOCKKEY0                        (0x09010)
#define MMR_PLL9_LOCKKEY1                        (0x09014)

/* Below are definitions used by pll programming in the flow */
#define CFG_OFFSET                                (0x008)
#define LOCKKEY0_OFFSET                           (0x010)
#define LOCKKEY1_OFFSET                           (0x014)
#define CTRL_OFFSET	                              (0x020)
#define STAT_OFFSET                               (0x024)
#define FCTRL0_OFFSET                             (0x030)
#define FCTRL1_OFFSET                             (0x034)
#define PLLDIV_OFFSET                          	  (0x038)
#define PLLSSCTRL_OFFSET                          (0x040)
#define PLLSSSPRD_OFFSET                          (0x044)
#define CAL_OFFSET                                (0x60)
#define CAL_STAT_OFFSET                           (0x64)
#define HSDIVCTRL0_OFFSET                         (0x80)
#define HSDIVCTRL1_OFFSET                         (0x84)
#define HSDIVCTRL2_OFFSET						  (0x88)
#define HSDIVCTRL3_OFFSET						  (0x8C)
#define HSDIVCTRL4_OFFSET						  (0x90)
#define HSDIVCTRL5_OFFSET						  (0x94)
#define HSDIVCTRL6_OFFSET						  (0x98)
#define HSDIVCTRL7_OFFSET						  (0x9C)
#define HSDIVCTRL8_OFFSET						  (0xA0)
#define HSDIVSTAT0_OFFSET                         (0xC0)
#define HSDIVSTAT1_OFFSET                         (0xC4)
#define HSDIVSTAT2_OFFSET                         (0xC8)
#define HSDIVSTAT3_OFFSET                         (0xCC)
#define HSDIVSTAT4_OFFSET                         (0xD0)
#define HSDIVSTAT5_OFFSET                         (0xD4)
#define HSDIVSTAT6_OFFSET                         (0xD8)
#define HSDIVSTAT7_OFFSET                         (0xDC)
#define HSDIVSTAT8_OFFSET                         (0xE0)
#define TESTCTRL_OFFSET							  (0x50)

/*Deskew specific offsets */
#define DESKEW_DIV_OFFSET						  (0x038)
#define DESKEW_TEST_CTRL_OFFSET					  (0x050)
#define DESKEW_CAL_CTRL_OFFSET					  (0x060)
#define DESKEW_CAL_STAT_OFFSET					  (0x064)

/* MCU MMR controlled PLLs */
#define MCU_PLL0_OFFSET                           (0x0000)
#define MCU_PLL1_OFFSET                           (0x1000)
#define MCU_PLL2_OFFSET							  (0x2000)
/* MAIN MMR controlled PLLs */
#define MAIN_PLL0_OFFSET                           (0x0000)
#define MAIN_PLL1_OFFSET                           (0x1000)
#define MAIN_PLL2_OFFSET                           (0x2000)
#define MAIN_PLL3_OFFSET                           (0x3000)
#define MAIN_PLL4_OFFSET                           (0x4000)
#define MAIN_PLL5_OFFSET                           (0x5000)
#define MAIN_PLL6_OFFSET                           (0x6000)
#define MAIN_PLL7_OFFSET                           (0x7000)
#define MAIN_PLL8_OFFSET                           (0x8000)
#define MAIN_PLL9_OFFSET                           (0x9000)
#define MAIN_PLL10_OFFSET                           (0xA000)
#define MAIN_PLL12_OFFSET                           (0xC000)
#define MAIN_PLL13_OFFSET                           (0xD000)
#define MAIN_PLL14_OFFSET                           (0xE000)
#define MAIN_PLL15_OFFSET							(0xF000)
#define MAIN_PLL16_OFFSET                           (0x10000)
#define MAIN_PLL17_OFFSET                           (0x11000)
#define MAIN_PLL18_OFFSET                           (0x12000)
#define MAIN_PLL19_OFFSET                           (0x13000)
#define MAIN_PLL20_OFFSET                           (0x14000)
#define MAIN_PLL21_OFFSET                           (0x15000)
#define MAIN_PLL22_OFFSET                           (0x16000)
#define MAIN_PLL23_OFFSET                           (0x17000)
#define MAIN_PLL24_OFFSET                           (0x18000)
#define MAIN_PLL25_OFFSET                           (0x19000)

/* PLL MMR BASE ADDRESS */
#define MAIN_PLL0_BASE  (pllmmr1_cfg_base + MAIN_PLL0_OFFSET)
#define MAIN_PLL1_BASE  (pllmmr1_cfg_base + MAIN_PLL1_OFFSET)
#define MAIN_PLL2_BASE  (pllmmr1_cfg_base + MAIN_PLL2_OFFSET)
#define MAIN_PLL3_BASE  (pllmmr1_cfg_base + MAIN_PLL3_OFFSET)
#define MAIN_PLL4_BASE  (pllmmr1_cfg_base + MAIN_PLL4_OFFSET)
#define MAIN_PLL5_BASE  (pllmmr1_cfg_base + MAIN_PLL5_OFFSET)
#define MAIN_PLL6_BASE  (pllmmr1_cfg_base + MAIN_PLL6_OFFSET)
#define MAIN_PLL7_BASE  (pllmmr1_cfg_base + MAIN_PLL7_OFFSET)
#define MAIN_PLL8_BASE  (pllmmr1_cfg_base + MAIN_PLL8_OFFSET)
#define MAIN_PLL9_BASE  (pllmmr1_cfg_base + MAIN_PLL9_OFFSET)
#define MAIN_PLL10_BASE  (pllmmr1_cfg_base + MAIN_PLL10_OFFSET)
#define MAIN_PLL12_BASE  (pllmmr1_cfg_base + MAIN_PLL12_OFFSET)
#define MAIN_PLL13_BASE  (pllmmr1_cfg_base + MAIN_PLL13_OFFSET)
#define MAIN_PLL14_BASE  (pllmmr1_cfg_base + MAIN_PLL14_OFFSET)
#define MAIN_PLL15_BASE  (pllmmr1_cfg_base + MAIN_PLL15_OFFSET)
#define MAIN_PLL16_BASE  (pllmmr1_cfg_base + MAIN_PLL16_OFFSET)
#define MAIN_PLL17_BASE  (pllmmr1_cfg_base + MAIN_PLL17_OFFSET)
#define MAIN_PLL18_BASE  (pllmmr1_cfg_base + MAIN_PLL18_OFFSET)
#define MAIN_PLL19_BASE  (pllmmr1_cfg_base + MAIN_PLL19_OFFSET)
#define MAIN_PLL20_BASE  (pllmmr1_cfg_base + MAIN_PLL20_OFFSET)
#define MAIN_PLL21_BASE  (pllmmr1_cfg_base + MAIN_PLL21_OFFSET)
#define MAIN_PLL22_BASE  (pllmmr1_cfg_base + MAIN_PLL22_OFFSET)
#define MAIN_PLL23_BASE  (pllmmr1_cfg_base + MAIN_PLL23_OFFSET)
#define MAIN_PLL24_BASE  (pllmmr1_cfg_base + MAIN_PLL24_OFFSET)
#define MAIN_PLL25_BASE  (pllmmr1_cfg_base + MAIN_PLL25_OFFSET)
#define MCU_PLL0_BASE   (pllmmr0_cfg_base + MCU_PLL0_OFFSET) 
#define MCU_PLL1_BASE   (pllmmr0_cfg_base + MCU_PLL1_OFFSET)
#define MCU_PLL2_BASE   (pllmmr0_cfg_base + MCU_PLL2_OFFSET)
                                                                               

/* Combined PLL MMR field values for both PLLM and PLLLJM */
#define NUM_PLLS 12
#define NUM_MCU_PLLS 3
#define NUM_MAIN_PLLS 9


#endif
