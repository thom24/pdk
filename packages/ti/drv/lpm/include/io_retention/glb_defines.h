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

#ifndef _GLOBAL_DEFINE_H
#define _GLOBAL_DEFINE_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
#endif
#define RD_MEM_8(addr)	         *mkptr8((addr),0)	 
#define WR_MEM_8(addr, data)     *mkptr8((addr),0) = (data)

#define RD_MEM_16(addr)	         *mkptr16((addr),0)	 
#define WR_MEM_16(addr, data)	 *mkptr16((addr),0) = (data)

#define RD_MEM_32(addr)	         *mkptr((addr),0)	 
#define WR_MEM_32(addr, data)	 *mkptr((addr),0) = (data)

#define RD_MEM_64(addr)	         *mkptr64((addr),0)	 
#define WR_MEM_64(addr, data)	 *mkptr64((addr),0) = (data)
 
#define BOOL           bool

#define FLOAT 		float
#define DOUBLE 		double
#define Ptr 		void *
#define UWORD8 		uint8_t
#define WORD8 		int8_t
#define UWORD16 	uint16_t
#define WORD16 		int16_t
#define UWORD32 	uint32_t
#define WORD32 		int32_t
#define UWORD64 	uint64_t
#define WORD64 		int64_t
#define Int 		int32_t
#define Char 		int8_t
#define String 		int8_t * 
#define Uint32 		uint32_t
#define Uint16 		uint16_t
#define Uint8 		uint8_t
#define Int32 		int32_t
#define Int16 		int16_t
#define Int8 		int8_t
#define REG_UWORD8  	volatile uint8_t
#define REG_WORD8   	volatile int8_t
#define REG_UWORD16 	volatile uint16_t
#define REG_WORD16 	volatile int16_t
#define REG_UWORD32 	volatile uint32_t
#define REG_WORD32 	volatile int32_t
#define REG_UWORD64 	volatile uint64_t
#define REG_WORD64 	volatile int64_t
	
#ifdef __cplusplus
}
#endif

#endif	/* _GLOBAL_DEFINE_H */


