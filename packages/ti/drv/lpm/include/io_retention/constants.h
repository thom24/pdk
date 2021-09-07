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

#ifndef _CONSTANT_H_
#define _CONSTANT_H_


#include <stdint.h>
#include <stdbool.h>
#include <ti/drv/lpm/include/io_retention/glb_defines.h>

#ifdef SOC_J7200
#define DEVICE_NAME j7vcl_core
#else
#define DEVICE_NAME j7_core
#endif

#define PASS  		1
#define FAIL  		0
#define EPIC_FAIL       2

#define newton_core	(0xFFFF3000)
#define fleming_core	(0xFFFF3001)
#define maxwell_core    (0xFFFF3002)
#define j7_core		(0xFFFF3003) 
#define pascal_core     (0xFFFF3004)
#define j7vcl_core      (0xFFFF3005)

#if DEVICE_NAME == fleming_core
#define C7X_CNUM_START	0
#define M3_CNUM_START	2
#define R5F_CNUM_START	3
#define A53_CNUM_START	9
#elif DEVICE_NAME == newton_core
#define C7X_CNUM_START	0
#define M3_CNUM_START	2
#define R5F_CNUM_START	3
#elif DEVICE_NAME == j7_core
#define M3_CNUM_START	0
#define R5F_CNUM_START	1
#define A72_CNUM_START  10	
#define C7X_CNUM_START	12
#define C66X_CNUM_START	13
#elif DEVICE_NAME == maxwell_core
#define M3_CNUM_START	0
#define R5F_CNUM_START	1
#define A53_CNUM_START	4
#elif DEVICE_NAME == pascal_core
#define M3_CNUM_START   0 /* 1 M3 */
#define R5F_CNUM_START  1 /* 2 pulsars (3 cores each) 6 cores */
#define A53_CNUM_START  7 /* 1 corepac (2 cores each) 2 cores */
#elif DEVICE_NAME == j7vcl_core
#define M3_CNUM_START   0 /* 1 M3 */
#define R5F_CNUM_START  1 /* 2 pulsars (3 cores each) 6 cores */
#define A72_CNUM_START  7 /* 1 corepac (2 cores each) 2 cores */
#define C7X_CNUM_START	-1 /* No c7x but we need this define for testcase reuse */
#define C66X_CNUM_START	-1 /* No c66x but we need this define for testcase reuse */
#endif

#if DEVICE_NAME == fleming_core
    #define CORE_COUNT      13
#elif DEVICE_NAME == newton_core
    #define CORE_COUNT      9
#elif DEVICE_NAME == maxwell_core
    #define CORE_COUNT      8
#elif DEVICE_NAME == j7_core
    #define CORE_COUNT      15
#elif DEVICE_NAME == pascal_core
    #define CORE_COUNT      9
#elif DEVICE_NAME == j7vcl_core
    #define CORE_COUNT      9
#else
    #error "Unknown DEVICE_NAME"  DEVICE_NAME
#endif

#define DDR3 	0
#define DDR4	1
#define LPDDR4 	2

#endif
