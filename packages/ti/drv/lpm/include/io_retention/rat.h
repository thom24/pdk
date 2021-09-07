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

#include <ti/drv/lpm/include/io_retention/constants.h>
#include <ti/drv/lpm/include/io_retention/dev_info.h>
#include <ti/drv/lpm/include/io_retention/base_address.h>


#ifndef IP_RAT
#define IP_RAT

#define IP_RAT_REGION_SIZE	0x10
#define IP_RAT_NUM_REGIONS	  16

#define IP_RAT_PID            0x0
#define IP_RAT_R0_CTRL        0x20
#define IP_RAT_R0_BASE        0x24
#define IP_RAT_R0_LOWER       0x28
#define IP_RAT_R0_UPPER       0x2C
#define IP_RAT_R1_CTRL        0x30
#define IP_RAT_R1_BASE        0x34
#define IP_RAT_R1_LOWER       0x38
#define IP_RAT_R1_UPPER       0x3C
#define IP_RAT_R2_CTRL        0x40
#define IP_RAT_R2_BASE        0x44
#define IP_RAT_R2_LOWER       0x48
#define IP_RAT_R2_UPPER       0x4C
#define IP_RAT_R3_CTRL        0x50
#define IP_RAT_R3_BASE        0x54
#define IP_RAT_R3_LOWER       0x58
#define IP_RAT_R3_UPPER       0x5C
#define IP_RAT_R4_CTRL        0x60
#define IP_RAT_R4_BASE        0x64
#define IP_RAT_R4_LOWER       0x68
#define IP_RAT_R4_UPPER       0x6C
#define IP_RAT_R5_CTRL        0x70
#define IP_RAT_R5_BASE        0x74
#define IP_RAT_R5_LOWER       0x78
#define IP_RAT_R5_UPPER       0x7C
#define IP_RAT_R6_CTRL        0x80
#define IP_RAT_R6_BASE        0x84
#define IP_RAT_R6_LOWER       0x88
#define IP_RAT_R6_UPPER       0x8C
#define IP_RAT_R7_CTRL        0x90
#define IP_RAT_R7_BASE        0x94
#define IP_RAT_R7_LOWER       0x98
#define IP_RAT_R7_UPPER       0x9C
#define IP_RAT_R8_CTRL        0xa0
#define IP_RAT_R8_BASE        0xa4
#define IP_RAT_R8_LOWER       0xa8
#define IP_RAT_R8_UPPER       0xaC
#define IP_RAT_R9_CTRL        0xb0
#define IP_RAT_R9_BASE        0xb4
#define IP_RAT_R9_LOWER       0xb8
#define IP_RAT_R9_UPPER       0xbC
#define IP_RAT_R10_CTRL       0xc0
#define IP_RAT_R10_BASE       0xc4
#define IP_RAT_R10_LOWER      0xc8
#define IP_RAT_R10_UPPER      0xcC
#define IP_RAT_R11_CTRL       0xd0
#define IP_RAT_R11_BASE       0xd4
#define IP_RAT_R11_LOWER      0xd8
#define IP_RAT_R11_UPPER      0xdC
#define IP_RAT_R12_CTRL       0xe0
#define IP_RAT_R12_BASE       0xe4
#define IP_RAT_R12_LOWER      0xe8
#define IP_RAT_R12_UPPER      0xeC
#define IP_RAT_R13_CTRL       0xf0
#define IP_RAT_R13_BASE       0xf4
#define IP_RAT_R13_LOWER      0xf8
#define IP_RAT_R13_UPPER      0xfC
#define IP_RAT_R14_CTRL       0x100
#define IP_RAT_R14_BASE       0x104
#define IP_RAT_R14_LOWER      0x108
#define IP_RAT_R14_UPPER      0x10C
#define IP_RAT_R15_CTRL       0x110
#define IP_RAT_R15_BASE       0x114
#define IP_RAT_R15_LOWER      0x118
#define IP_RAT_R15_UPPER      0x11C


#define IP_RAT_EXC_EN         0x848
#define IP_RAT_EXC_SET        0x840
#define IP_RAT_EXC_CLR        0x844


/* This code is not allowed for config as DDNUM is not valid define there. rramesh@ti.com */
#ifndef CONFIG_SRC
    #if (defined M3CORE)
       #include <dmsc.h>
       #define rat_cfg_base (dmsc0_ssr0_base + DMSC_RAT_OFFSET)
    #elif (defined R5FCORE)
       #include <pulsar.h>
       /* MCU and Main R5s */
       /* FIX: We should use define and not hardcoded value */
       #if ((DDNUM == 0) || (DDNUM == 1) || (DDNUM == 2)) 
           #define rat_cfg_base 0x40f90000
       #else 
           #define rat_cfg_base 0x0ff90000
       #endif
    #elif (defined C66XCORE)
       #include <c66x.h>
       #define rat_cfg_base c66_local_rat_cfg_base
    #endif
#endif

#endif
