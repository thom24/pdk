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
#include <ti/drv/lpm/include/rat_functions.h>
#include <ti/drv/lpm/include/rat.h>

#if (defined R5FCORE)
/* R5 MMU table mark 0xC000_0000 as memroy region so sometime we can run into cache/write butter hitting issues. So we need another region where it marked as device */

  /* New Region ID 0 : map R5F: 0xFFF0_0000 : 0xFFF0_07FF ->    0x0000_0000 :    0x0000_07FF (2kB)    */
  /* New Region ID 1 : map R5F: 0xFFF0_0800 : 0xFFF0_0BFF ->    0x0020_0000 :    0x0020_03FF (1kB)    */
  /* New Region ID 2 : map R5F: 0xFFF0_0C00 : 0xFFF0_0DFF ->    0x4028_0000 :    0x4028_01FF (512B)   */
  /* New Region ID 3 : map R5F: 0xFC00_0000 : 0xFC07_FFFF -> 0x4F_0200_0000 : 0x4F_0207_FFFF (512kB)  */
  /* New Region ID 4 map R5F: TCM_EXTEND0 : 0x0001_0000 : 0x0001_FFFF => tcm_extend_map0              */
  /* New Region ID 5 map R5F: TCM_EXTEND1 : 0x0002_0000 : 0x0002_FFFF => tcm_extend_map1              */
  /* New Region ID 6 map R5F: 0xF000_0000 : 0xF7FF_FFFF -> 0x08_8000_0000 : 0x08_8000_0000 (128M)     */

   #if (defined USE_MSMC_REGION_FOR_R5_CC_RAT)
	/* RAT PROGRAMMING */
	/* New Region ID  8 : map R5:0x6800_0000:0x6BFF_FFFF -> 0x4D_2000_0000: 0x4D_23FF_FFFF (64M) */
	/* New Region ID  9 : map R5:0x6C00_0000:0x6C7F_FFFF -> 0x4D_1000_0000: 0x4D_107F_FFFF (8M)  */
	/* New Region ID 10 : map R5:0x6C80_0000:0x6CFF_FFFF -> 0x4D_0000_0000: 0x4D_007F_FFFF (8M)  */
        #define CC_CFG_RAT_PGM  set_rat_region(rat_cfg_base, 8,0x4D,0x68000000,0x20000000,26); \
                                set_rat_region(rat_cfg_base, 9,0x4D,0x6C000000,0x10000000,23); \
                                set_rat_region(rat_cfg_base,10,0x4D,0x6C800000,0x00000000,23);

        #define CC_RAT_BASE_ADDR 0x60000000

   #else
	/* RAT PROGRAMMING  */
	/* New Region ID  8 : map R5:0xC800_0000:0xCBFF_FFFF -> 0x4D_2000_0000: 0x4D_23FF_FFFF (64M) */
	/* New Region ID  9 : map R5:0xCC00_0000:0xCC7F_FFFF -> 0x4D_1000_0000: 0x4D_107F_FFFF (8M)  */
	/* New Region ID 10 : map R5:0xCC80_0000:0xCCFF_FFFF -> 0x4D_0000_0000: 0x4D_007F_FFFF (8M)  */
        #define CC_CFG_RAT_PGM set_rat_region(rat_cfg_base, 8,0x4D,0xC8000000,0x20000000,26); \
                               set_rat_region(rat_cfg_base, 9,0x4D,0xCC000000,0x10000000,23); \
                               set_rat_region(rat_cfg_base,10,0x4D,0xCC800000,0x00000000,23);



        #define CC_RAT_BASE_ADDR 0xC0000000
    #endif
#endif

#if (defined M3CORE)
	/* RAT PROGRAMMING (Cfg programmed 0 to 9) */
        /* New Region ID 0 : map M3:0x8000_0000:0xbfff_ffff ->    0x0000_0000:   0x3fff_ffff (1024M) */
        /* New Region ID 1 : map M3:0x7000_0000:0x77ff_ffff ->    0x4000_0000:   0x47ff_ffff (128M) */
        /* New Region ID 2 : map M3:0xC000_0000:0xC7ff_ffff ->    0x4800_0000:   0x4fff_ffff (128M) */
        /* New Region ID 3 : map M3:0x7C00_0000:0x7Dff_ffff ->    0x5000_0000:   0x51ff_ffff (32M) */
        /* New Region ID 4 : map M3:0x7E00_0000:0x7Fff_ffff ->    0x5800_0000:   0x59ff_ffff (32M) */
        /* New Region ID 5 : map M3:0x6000_0000:0x63ff_ffff ->    0x8000_0000:   0x87ff_ffff (128M) */
        /* New Region ID 6 : map M3:0x6800_0000:0x6fff_ffff -> 0x08_8000_0000:0x08_87ff_ffff (128M) */
        /* New Region ID 7 : map M3:0x7800_0000:0x79ff_ffff ->    0x7000_0000:   0x71ff_ffff (32M) */
        /* New Region ID 8 : map M3:0x7A00_0000:0x7Bff_ffff -> 0x4f_0200_0000:0x4f_3fff_ffff (64M) */
        /* New Region ID 9 : map M3:0xD000_0000:0xD3ff_ffff ->    0x6c00_0000:   0x6fff_ffff (64M) */
	/* New Region ID 10: map M3:0xC800_0000:0xCBFF_FFFF -> 0x4D_2000_0000:0x4D_23FF_FFFF (64M) */
	/* New Region ID 11: map M3:0xCC00_0000:0xCC7F_FFFF -> 0x4D_1000_0000:0x4D_107F_FFFF ( 8M) */
	/* New Region ID 12: map M3:0xCC80_0000:0xCCFF_FFFF -> 0x4D_0000_0000:0x4D_007F_FFFF ( 8M) */

        #define CC_CFG_RAT_PGM set_rat_region(rat_cfg_base,10, 0x4D, 0xC8000000, 0x20000000, 26); \
                               set_rat_region(rat_cfg_base,11, 0x4D, 0xCC000000, 0x10000000, 23); \
                               set_rat_region(rat_cfg_base,12, 0x4D, 0xCC800000, 0x00000000, 23);

        #define CC_RAT_BASE_ADDR 0xC0000000
#endif



#ifdef M3CORE
	/*! \brief base2core macro for m3 core.
	 *  \sa base2core
	 */
#undef base2core /* Undef Config defintion */
#define base2core(base)							\
((uintptr_t)  (((((uint64_t)(base)) & 0xFE000000u) == 0x44000000u)                             ? ((((uint64_t)(base)) < 0x44100000u) ? (((uint64_t)(base)) & 0xfffffu) : ((uint64_t)(base))) : /* internal DMSC, IRAM DRAM and peripherals */ \
	       (((uint64_t)(base))  <  0x40000000u)                                            ?  (((uint64_t)(base)) + 0x80000000u) : /* for psram1, psram2 */ \
	       (((uint64_t)(base))  <  0x48000000u)                                            ?  (((uint64_t)(base)) | 0x30000000u) : /* for ocmsram */ \
	       ((((uint64_t)(base)) >=   0x50000000u) && (((uint64_t)(base)) <   0x52000000u)) ?  (((uint64_t)(base)) + 0x2C000000u) : /* for fss1 */ \
	       ((((uint64_t)(base)) >=   0x58000000u) && (((uint64_t)(base)) <   0x5A000000u)) ?  (((uint64_t)(base)) + 0x26000000u) : /* for fss2 */ \
	       ((((uint64_t)(base)) >=   0x70000000u) && (((uint64_t)(base)) <   0x74000000u)) ?  (((uint64_t)(base)) | 0x08000000u) : /* for msmc sram */ \
	       (((uint64_t)(base))  <    0x50000000u)                                          ?  (((uint64_t)(base)) + 0x78000000u) : \
	       ((((uint64_t)(base)) &    0xFC000000u) == 0x6C000000u)                          ?  ((((uint64_t)(base)) - 0x6C000000u) + 0xD0000000u) : \
	       ((((uint64_t)(base)) >=   0x80000000u) && (((uint64_t)(base)) <   0x88000000u)) ?  ((((uint64_t)(base)) | 0x60000000u) & ~0x80000000u) : /* ddr_lo */ \
	       ((((uint64_t)(base)) >=  0x880000000u) && (((uint64_t)(base)) <  0x888000000u)) ?  ((((uint64_t)(base)) | 0x68000000u) & ~0x800000000u) : /* ddr_hi */ \
	       ((((uint64_t)(base)) >= 0x4F02000000u) && (((uint64_t)(base)) < 0x4F04000000u)) ?  ((((uint64_t)(base)) & 0x01FFFFFFu ) + 0x7A000000u) : /* main_ocmsram */ \
	       ((((uint64_t)(base)) >= 0x4D20000000u) && (((uint64_t)(base)) < 0x4D24000000u)) ?  (((uint64_t)(base)) + 0x08000000 + CC_RAT_BASE_ADDR - 0x4D20000000) : \
	       ((((uint64_t)(base)) >= 0x4D10000000u) && (((uint64_t)(base)) < 0x4D10800000u)) ?  (((uint64_t)(base)) + 0x0C000000 + CC_RAT_BASE_ADDR - 0x4D10000000) : \
	       ((((uint64_t)(base)) >= 0x4D00000000u) && (((uint64_t)(base)) < 0x4D00800000u)) ?  (((uint64_t)(base)) + 0x0C800000 + CC_RAT_BASE_ADDR - 0x4D00000000) : \
	       ((uint64_t)(base))))

#endif

#ifdef R5FCORE		
#undef base2core

#define base2core(base)							\
  ((uintptr_t)(((((uint64_t)(base)) >=   0x00000000u) && (((uint64_t)(base)) <   0x00000800u)) ? ((((uint64_t)(base)) & 0x000007ffu) + 0xfff00000u) : \
	       ((((uint64_t)(base)) >=   0x00200000u) && (((uint64_t)(base)) <   0x00200400u)) ? ((((uint64_t)(base)) & 0x000003ffu) + 0xfff00800u) : \
	       ((((uint64_t)(base)) >=   0x40280000u) && (((uint64_t)(base)) <   0x40280200u)) ? ((((uint64_t)(base)) & 0x000001ffu) + 0xfff00c00u) : \
	       ((((uint64_t)(base)) >= 0x4F02000000u) && (((uint64_t)(base)) < 0x4F02000000u)) ? ((((uint64_t)(base)) & 0x0007ffffu) + 0xfc000000u) : \
	       ((((uint64_t)(base)) >=  0x880000000u) && (((uint64_t)(base)) <  0x888000000u)) ? ((((uint64_t)(base)) & 0x07ffffffu) + 0xf0000000u) : \
	       ((((uint64_t)(base)) >= 0x4D20000000u) && (((uint64_t)(base)) < 0x4D24000000u)) ?  (((uint64_t)(base)) + 0x08000000 +  CC_RAT_BASE_ADDR - 0x4D20000000) : \
	       ((((uint64_t)(base)) >= 0x4D10000000u) && (((uint64_t)(base)) < 0x4D10800000u)) ?  (((uint64_t)(base)) + 0x0C000000 +  CC_RAT_BASE_ADDR - 0x4D10000000) : \
	       ((((uint64_t)(base)) >= 0x4D00000000u) && (((uint64_t)(base)) < 0x4D00800000u)) ?  (((uint64_t)(base)) + 0x0C800000 +  CC_RAT_BASE_ADDR - 0x4D00000000) : \
	       ((uint64_t)(base))))
  
#endif




