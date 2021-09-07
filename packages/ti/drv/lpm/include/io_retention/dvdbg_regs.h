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

#ifndef _DVDBG_REGS_H_
#define _DVDBG_REGS_H_

#if 0 
#define	por_err_cnt	(*mkptr(mmr0_cfg_base,MMR_TEST_DEBUG_PORRST0))
#define	por_progress0	(*mkptr(mmr0_cfg_base,MMR_TEST_DEBUG_PORRST1))
#define	por_progress1	(*mkptr(mmr0_cfg_base,MMR_TEST_DEBUG_PORRST2))
#define	por_progress2	(*mkptr(mmr0_cfg_base,MMR_TEST_DEBUG_PORRST3))
#define	por_debug0	(*mkptr(mmr0_cfg_base,MMR_TEST_DEBUG_PORRST4))
#define	por_debug1	(*mkptr(mmr0_cfg_base,MMR_TEST_DEBUG_PORRST5))
#define	por_debug2	(*mkptr(mmr0_cfg_base,MMR_TEST_DEBUG_PORRST6))
#define	por_debug3	(*mkptr(mmr0_cfg_base,MMR_TEST_DEBUG_PORRST7))
#define	rst_err_cnt	(*mkptr(mmr0_cfg_base,MMR_TEST_DEBUG_ALLRST0))
#define	rst_progress0	(*mkptr(mmr0_cfg_base,MMR_TEST_DEBUG_ALLRST1))
#define	rst_progress1	(*mkptr(mmr0_cfg_base,MMR_TEST_DEBUG_ALLRST2))
#define	rst_progress2	(*mkptr(mmr0_cfg_base,MMR_TEST_DEBUG_ALLRST3))
#define	rst_debug0	(*mkptr(mmr0_cfg_base,MMR_TEST_DEBUG_ALLRST4))
#define	rst_debug1	(*mkptr(mmr0_cfg_base,MMR_TEST_DEBUG_ALLRST5))
#define	rst_debug2	(*mkptr(mmr0_cfg_base,MMR_TEST_DEBUG_ALLRST6))
#define	rst_debug3	(*mkptr(mmr0_cfg_base,MMR_TEST_DEBUG_ALLRST7))
#endif 

#include <ti/drv/lpm/include/io_retention/wkup_ctrl_mmr.h>

#define	por_err_cnt		(*mkptr(mmr0_cfg_base,WKUP_CTRL_DV_REG48))
#define	por_progress0	(*mkptr(mmr0_cfg_base,WKUP_CTRL_DV_REG49))
#define	por_progress1	(*mkptr(mmr0_cfg_base,WKUP_CTRL_DV_REG50))
#define	por_progress2	(*mkptr(mmr0_cfg_base,WKUP_CTRL_DV_REG51))
#define	por_debug0		(*mkptr(mmr0_cfg_base,WKUP_CTRL_DV_REG52))
#define	por_debug1		(*mkptr(mmr0_cfg_base,WKUP_CTRL_DV_REG53))
#define	por_debug2		(*mkptr(mmr0_cfg_base,WKUP_CTRL_DV_REG54))
#define	por_debug3		(*mkptr(mmr0_cfg_base,WKUP_CTRL_DV_REG55))
#define	rst_err_cnt		(*mkptr(mmr0_cfg_base,WKUP_CTRL_DV_REG16))
#define	rst_progress0	(*mkptr(mmr0_cfg_base,WKUP_CTRL_DV_REG17))
#define	rst_progress1	(*mkptr(mmr0_cfg_base,WKUP_CTRL_DV_REG18))
#define	rst_progress2	(*mkptr(mmr0_cfg_base,WKUP_CTRL_DV_REG19))
#define	rst_debug0		(*mkptr(mmr0_cfg_base,WKUP_CTRL_DV_REG20))
#define	rst_debug1		(*mkptr(mmr0_cfg_base,WKUP_CTRL_DV_REG21))
#define	rst_debug2		(*mkptr(mmr0_cfg_base,WKUP_CTRL_DV_REG22))
#define	rst_debug3		(*mkptr(mmr0_cfg_base,WKUP_CTRL_DV_REG23))
#endif

#define  err_cnt	rst_err_cnt
#define  progress0      rst_progress0
#define  progress1      rst_progress1
#define  progress2      rst_progress2
#define  debug0		rst_debug0
#define  debug1		rst_debug1
#define  debug2		rst_debug2
#define  debug3		rst_debug3

#define  INIT_ERR_CNT  err_cnt = 0;
