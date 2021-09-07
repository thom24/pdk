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

#ifndef _IO_H_
#define _IO_H_

/* Defines IO DINUMs and reg_info struct */
#include <ti/drv/lpm/include/io_retention/dev_info.h>
#include <ti/drv/lpm/include/io_retention/io_instance.h>

/* Declares (main|wkup)_ctrl_mmr_io_padconfig_reg and register macros */
#include <ti/drv/lpm/include/io_retention/wkup_ctrl_mmr.h>
#include <ti/drv/lpm/include/io_retention/main_ctrl_mmr.h>
#include <ti/drv/lpm/include/io_retention/io_padconfig_mmr.h>

/* Array size utilities */
#define IO_ARR_SIZE(arr,type) sizeof(arr)/sizeof(type)

#define CNT_WKUP_PADCFG IO_ARR_SIZE(wkup_ctrl_mmr_io_padconfig_reg,struct reg_info)
#define CNT_MAIN_PADCFG IO_ARR_SIZE(main_ctrl_mmr_io_padconfig_reg,struct reg_info)

/* MMRs */
#define WKUP_CTRL_MMR_BASE mmr0_cfg_base
#define MAIN_CTRL_MMR_BASE mmr2_cfg_base

/* Padconfig translation macros */
#define IO_WKUP_PADCFG2OFFSET(i) mmr0_cfg_base+i*0x04
#define IO_MAIN_PADCFG2OFFSET(i) mmr2_cfg_base+i*0x04
#define IO_WKUP_INDEX2OFFSET(i) wkup_ctrl_mmr_io_padconfig_reg[i].offset
#define IO_MAIN_INDEX2OFFSET(i) main_ctrl_mmr_io_padconfig_reg[i].offset
#define IO_WKUP_OFFSET2PADCFG(i) (i-WKUP_CTRL_PADCONFIG0)/0x04
#define IO_MAIN_OFFSET2PADCFG(i) (i-MAIN_CTRL_PADCONFIG0)/0x04
#define IO_WKUP_INDEX2PADCFG(i) IO_WKUP_OFFSET2PADCFG(IO_WKUP_INDEX2OFFSET(i))
#define IO_MAIN_INDEX2PADCFG(i) IO_MAIN_OFFSET2PADCFG(IO_MAIN_INDEX2OFFSET(i))

/* Various padconfig definitions */
#define WKUP_GPIO0_PADCONFIG_MMR      48
#define WKUP_GPIO1_PADCONFIG_MMR      49
#define WKUP_GPIO2_PADCONFIG_MMR      50
#define WKUP_GPIO3_PADCONFIG_MMR      51
#define WKUP_GPIO12_PADCONFIG_MMR     60
#define WKUP_GPIO13_PADCONFIG_MMR     61
#define WKUP_GPIO14_PADCONFIG_MMR     62
#define WKUP_GPIO15_PADCONFIG_MMR     63
#define MCU_SAFETY_ERR_PADCONFIG_MMR  69
#define MCU_RESETZ_PADCONFIG_MMR      70
#define MCU_RESETSTATZ_PADCONFIG_MMR  71
/*#define MCU_PORZ_OUT_PADCONFIG_MMR    71 */
#define TRSTN_PADCONFIG_MMR           74
#define EMU0_PADCONFIG_MMR            75
#define EMU1_PADCONFIG_MMR            76

#define EXTINTN_PADCONFIG_MMR          0
#define SOC_SAFETY_ERR_PADCONFIG_MMR  68
#define MAIN_RESETZ_PADCONFIG_MMR     93
#define MAIN_PORZ_PADCONFIG_MMR       94
#define RESETSTATZ_PADCONFIG_MMR      66

#define SOC_ERROR_PADCFG MAIN_CTRL_PADCONFIG68
#define MCU_ERROR_PADCFG WKUP_CTRL_PADCONFIG69

/*#define RESETZ_REQ_PADCONFIG_MMR     158 */
/*#define PORZ_PADCONFIG_MMR           159 */

/* my error codes to use in testcase assertions */
typedef enum {
  IO_FINAL_DATA_MISMATCH  = 0xf0000100u,  /*/< data comparison failed at the end of the testcase. */
  IO_CSLR_MMR_TEST        = 0xf1000000u,  /*/< generic cslr_mmr_test failure. See mmr_test failure codes for real reason. */
  IO_REG_MISMATCH         = 0xf3000000u,  /*/< register mismatch error in mmr tests */
  IO_INVALID_INSTANCE     = 0xf3000100u,  /*/< invalid instance requested in instance_select function */
} io_fail_code_t;


#endif
