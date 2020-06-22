/******************************************************************************
 * Copyright (c) 2020 Texas Instruments Incorporated - http://www.ti.com
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *****************************************************************************/
 
/**
 * \file   board_pll.h
 *
 * \brief  Board PLL configurations header file
 *
 * This file includes the structures, enums and register offsets
 * for PLL configurations
 *
 */

#ifndef BOARD_PLL_H
#define BOARD_PLL_H

#include "board_internal.h"
#include "board_pll.h"
#include "ti/csl/soc.h"
#include <ti/csl/hw_types.h>

#ifdef __cplusplus
extern "C" {
#endif

//Global definitions
#define DEBUG 	0 //turn on this for debugging messages

#define M3_MCU_OFFSET 		(0x20000000U)
#define M3_MAIN_OFFSET 		(0x80000000U)

#define CONTROL 			(0x20U)

#define KICK0_UNLOCK 	    (0x68EF3490U)
#define KICK1_UNLOCK 	    (0xD172BC5AU)
#define KICK_LOCK		    (0)

#define DDR_PLL_INDEX		(12U)

typedef struct Board_PllClkCfg_s
{
    uint32_t tisciDevID;
    uint32_t tisciClkID;
    uint64_t clkRate;
    
} Board_PllClkCfg_t;

/* END OF FILE */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif  /* BOARD_PLL_H */
