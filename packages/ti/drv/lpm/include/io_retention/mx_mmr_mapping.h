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

#ifndef _MX_MMR_MAPPING_H
#define _MX_MMR_MAPPING_H
#include <ti/drv/lpm/include/io_retention/mmr_common.h>
#define WKUP_CTRL_CORE_PASS_DONE_SET0             WKUP_CTRL_DV_REG0_SET 
#define WKUP_CTRL_CORE_PASS_DONE_CLR0             WKUP_CTRL_DV_REG0_CLR
#define WKUP_CTRL_ACTIVE_CORES                    WKUP_CTRL_DV_REG32 
#define WKUP_CTRL_DBG_FEATURE0                    WKUP_CTRL_DV_REG33
#define WKUP_CTRL_DBG_FEATURE1                    WKUP_CTRL_DV_REG34 
#define WKUP_CTRL_DBG_FEATURE2                    WKUP_CTRL_DV_REG35 
#define WKUP_CTRL_DBG_FEATURE3                    WKUP_CTRL_DV_REG36 
#define WKUP_CTRL_CORE_DEBUG0                     WKUP_CTRL_DV_REG0 
#define WKUP_CTRL_CORE_DEBUG1                     WKUP_CTRL_DV_REG38 
#define WKUP_CTRL_CORE_DEBUG2                     WKUP_CTRL_DV_REG39 
#define WKUP_CTRL_CORE_DEBUG3                     WKUP_CTRL_DV_REG40 
#endif 
