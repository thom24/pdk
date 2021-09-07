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

#ifndef _MMR_COMMOM_H_
#define _MMR_COMMOM_H_
#define MMR_LOCK0_KICK0_UNLOCK_VAL                (0x68EF3490)
#define MMR_LOCK0_KICK1_UNLOCK_VAL                (0xD172BC5A)
#define MMR_LOCK1_KICK0_UNLOCK_VAL                (0x68EF3490)
#define MMR_LOCK1_KICK1_UNLOCK_VAL                (0xD172BC5A)
#define MMR_LOCK2_KICK0_UNLOCK_VAL                (0x68EF3490)
#define MMR_LOCK2_KICK1_UNLOCK_VAL                (0xD172BC5A)
#define MMR_LOCK3_KICK0_UNLOCK_VAL                (0x68EF3490)
#define MMR_LOCK3_KICK1_UNLOCK_VAL                (0xD172BC5A)
#define MMR_LOCK4_KICK0_UNLOCK_VAL                (0x68EF3490)
#define MMR_LOCK4_KICK1_UNLOCK_VAL                (0xD172BC5A)
#define MMR_LOCK6_KICK0_UNLOCK_VAL                (0x68EF3490)
#define MMR_LOCK6_KICK1_UNLOCK_VAL                (0xD172BC5A)
#define MMR_LOCK7_KICK0_UNLOCK_VAL                (0x68EF3490)
#define MMR_LOCK7_KICK1_UNLOCK_VAL                (0xD172BC5A)
#define MMR_CORE_PASS_DONE_SET0                   (0x10000)
#define MMR_CORE_PASS_DONE_CLR0                   (0x10010)
#define MMR_ACTIVE_CORES                          (0x10020)
#define MMR_CORE_DEBUG0                           (0x10030)
#define MMR_CORE_DEBUG1                           (0x10034)
#define MMR_CORE_DEBUG2                           (0x10038)
#define MMR_CORE_DEBUG3                           (0x1003C)
#define MMR_DBG_FEATURE0                          (0x10050)
#define MMR_DBG_FEATURE1                          (0x10054)
#define MMR_DBG_FEATURE2                          (0x10058)
#define MMR_DBG_FEATURE3                          (0x1005C)
#define MMR_TEST_DEBUG_PORRST0                    (0x10080)
#define MMR_TEST_DEBUG_PORRST1                    (0x10084)
#define MMR_TEST_DEBUG_PORRST2                    (0x10088)
#define MMR_TEST_DEBUG_PORRST3                    (0x1008C)
#define MMR_TEST_DEBUG_PORRST4                    (0x10090)
#define MMR_TEST_DEBUG_PORRST5                    (0x10094)
#define MMR_TEST_DEBUG_PORRST6                    (0x10098)
#define MMR_TEST_DEBUG_PORRST7                    (0x1009C)
#define MMR_TEST_DEBUG_ALLRST0                    (0x100A0)
#define MMR_TEST_DEBUG_ALLRST1                    (0x100A4)
#define MMR_TEST_DEBUG_ALLRST2                    (0x100A8)
#define MMR_TEST_DEBUG_ALLRST3                    (0x100AC)
#define MMR_TEST_DEBUG_ALLRST4                    (0x100B0)
#define MMR_TEST_DEBUG_ALLRST5                    (0x100B4)
#define MMR_TEST_DEBUG_ALLRST6                    (0x100B8)
#define MMR_TEST_DEBUG_ALLRST7                    (0x100BC)
#define MMR_LOCK0_KICK0                           (0x01008)
#define MMR_LOCK1_KICK0                           (0x05008)
#define MMR_LOCK2_KICK0                           (0x09008)
#define MMR_LOCK3_KICK0                           (0x0D008)
#define MMR_LOCK4_KICK0                           (0x11008)
#define MMR_LOCK7_KICK0                           (0x1D008)
#define MMR_LOCK0_KICK1                           (0x0100C)
#define MMR_LOCK1_KICK1                           (0x0500C)
#define MMR_LOCK2_KICK1                           (0x0900C)
#define MMR_LOCK3_KICK1                           (0x0D00C)
#define MMR_LOCK4_KICK1                           (0x1100C)
#define MMR_LOCK7_KICK1                           (0x1D00C)
#define MMR_P0_CLAIM0_RDONLY                      (0x01100)
#define MMR_P0_CLAIM0                             (0x03100)
#define MMR_P0_CLAIM1_RDONLY                      (0x01104)
#define MMR_P0_CLAIM1                             (0x03104)
#define MMR_P0_CLAIM2_RDONLY                      (0x01108)
#define MMR_P0_CLAIM2                             (0x03108)
#define MMR_P0_CLAIM3_RDONLY                      (0x0110C)
#define MMR_P0_CLAIM3                             (0x0310C)
#define MMR_P0_CLAIM4_RDONLY                      (0x01110)
#define MMR_P0_CLAIM4                             (0x03110)
#define MMR_P0_CLAIM5_RDONLY                      (0x01114)
#define MMR_P0_CLAIM5                             (0x03114)
#define MMR_P0_CLAIM6_RDONLY                      (0x01118)
#define MMR_P0_CLAIM6                             (0x03118)
#define MMR_P1_CLAIM0_RDONLY                      (0x05100)
#define MMR_P1_CLAIM0                             (0x07100)
#define MMR_P1_CLAIM1_RDONLY                      (0x05104)
#define MMR_P1_CLAIM1                             (0x07104)
#define MMR_P1_CLAIM2_RDONLY                      (0x05108)
#define MMR_P1_CLAIM2                             (0x07108)
#define MMR_P1_CLAIM3_RDONLY                      (0x0510C)
#define MMR_P1_CLAIM3                             (0x0710C)
#define MMR_P1_CLAIM4_RDONLY                      (0x05110)
#define MMR_P1_CLAIM4                             (0x07110)
#define MMR_P1_CLAIM5_RDONLY                      (0x05114)
#define MMR_P1_CLAIM5                             (0x07114)
#define MMR_P1_CLAIM6_RDONLY                      (0x05118)
#define MMR_P1_CLAIM6                             (0x07118)
#define MMR_P1_CLAIM7_RDONLY                      (0x0511C)
#define MMR_P1_CLAIM7                             (0x0711C)
#define MMR_P1_CLAIM8_RDONLY                      (0x05120)
#define MMR_P1_CLAIM8                             (0x07120)
#define MMR_P1_CLAIM9_RDONLY                      (0x05124)
#define MMR_P1_CLAIM9                             (0x07124)
#define MMR_P1_CLAIM10_RDONLY                     (0x05128)
#define MMR_P1_CLAIM10                            (0x07128)
#define MMR_P1_CLAIM11_RDONLY                     (0x0512C)
#define MMR_P1_CLAIM11                            (0x0712C)
#define MMR_P1_CLAIM12_RDONLY                     (0x05130)
#define MMR_P1_CLAIM12                            (0x07130)
#define MMR_P1_CLAIM13_RDONLY                     (0x05134)
#define MMR_P1_CLAIM13                            (0x07134)
#define MMR_P1_CLAIM14_RDONLY                     (0x05138)
#define MMR_P1_CLAIM14                            (0x07138)
#define MMR_P1_CLAIM15_RDONLY                     (0x0513C)
#define MMR_P1_CLAIM15                            (0x0713C)
#define MMR_P1_CLAIM16_RDONLY                     (0x05140)
#define MMR_P1_CLAIM16                            (0x07140)
#define MMR_P1_CLAIM17_RDONLY                     (0x05144)
#define MMR_P1_CLAIM17                            (0x07144)
#define MMR_P1_CLAIM18_RDONLY                     (0x05148)
#define MMR_P1_CLAIM18                            (0x07148)
#define MMR_P2_CLAIM0_RDONLY                      (0x09100)
#define MMR_P2_CLAIM0                             (0x0B100)
#define MMR_P2_CLAIM1_RDONLY                      (0x09104)
#define MMR_P2_CLAIM1                             (0x0B104)
#define MMR_P2_CLAIM2_RDONLY                      (0x09108)
#define MMR_P2_CLAIM2                             (0x0B108)
#define MMR_P2_CLAIM3_RDONLY                      (0x0910C)
#define MMR_P2_CLAIM3                             (0x0B10C)
#define MMR_P3_CLAIM0_RDONLY                      (0x0D100)
#define MMR_P3_CLAIM0                             (0x0F100)
#define MMR_P3_CLAIM1_RDONLY                      (0x0D104)
#define MMR_P3_CLAIM1                             (0x0F104)
#define MMR_P3_CLAIM2_RDONLY                      (0x0D108)
#define MMR_P3_CLAIM2                             (0x0F108)
#define MMR_P3_CLAIM3_RDONLY                      (0x0D10C)
#define MMR_P3_CLAIM3                             (0x0F10C)
#define MMR_P3_CLAIM4_RDONLY                      (0x0D110)
#define MMR_P3_CLAIM4                             (0x0F110)
#define MMR_P3_CLAIM5_RDONLY                      (0x0D114)
#define MMR_P3_CLAIM5                             (0x0F114)
#define MMR_P3_CLAIM6_RDONLY                      (0x0D118)
#define MMR_P3_CLAIM6                             (0x0F118)
#define MMR_P4_CLAIM0_RDONLY                      (0x11100)
#define MMR_P4_CLAIM0                             (0x13100)
#define MMR_P4_CLAIM1_RDONLY                      (0x11104)
#define MMR_P4_CLAIM1                             (0x13104)
#define MMR_P4_CLAIM2_RDONLY                      (0x11108)
#define MMR_P4_CLAIM2                             (0x13108)
#define MMR_P6_CLAIM0_RDONLY                      (0x19100)
#define MMR_P6_CLAIM0                             (0x1B100)
#define MMR_P7_CLAIM0_RDONLY                      (0x1D100)
#define MMR_P7_CLAIM0                             (0x1F100)
#define MMR_P7_CLAIM1_RDONLY                      (0x1D104)
#define MMR_P7_CLAIM1                             (0x1F104)
#define MMR_P7_CLAIM2_RDONLY                      (0x1D108)
#define MMR_P7_CLAIM2                             (0x1F108)
#define MMR_P7_CLAIM3_RDONLY                      (0x1D10C)
#define MMR_P7_CLAIM3                             (0x1F10C)
#define MMR_P7_CLAIM4_RDONLY                      (0x1D110)
#define MMR_P7_CLAIM4                             (0x1F110)
#define MMR_P7_CLAIM5_RDONLY                      (0x1D114)
#define MMR_P7_CLAIM5                             (0x1F114)
#define MMR_P7_CLAIM6_RDONLY                      (0x1D118)
#define MMR_P7_CLAIM6                             (0x1F118)
#define MMR_LOCK6_KICK0                           (0x19008)
#define MMR_LOCK6_KICK1                           (0x1900C)
#define MMR_P6_CLAIM1_RDONLY                      (0x19104)
#define MMR_P6_CLAIM1                             (0x1B104)
#define MMR_P6_CLAIM2_RDONLY                      (0x19108)
#define MMR_P6_CLAIM2                             (0x1B108)
#define MMR_P6_CLAIM3_RDONLY                      (0x1910C)
#define MMR_P6_CLAIM3                             (0x1B10C)

#endif
