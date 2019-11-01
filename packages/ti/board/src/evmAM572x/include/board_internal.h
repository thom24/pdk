/* ========================================================================== */
/**
 *  @file   board_internal.h
 *
 *  @brief  Private definitions for the Board Library.
 *
 *  ============================================================================
 */
/* --COPYRIGHT--,BSD
 * Copyright (c) 2011-2017, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/

#ifndef BOARD_INTERNAL_H_
#define BOARD_INTERNAL_H_

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
 * Include Files                                                             *
 *****************************************************************************/
#include <ti/csl/csl_types.h>
#include <ti/csl/cslr_device.h>

#include <ti/drv/i2c/I2C.h>
#include <ti/drv/i2c/soc/I2C_soc.h>

#include <ti/drv/spi/SPI.h>
#include <ti/drv/spi/soc/SPI_soc.h>

#include <ti/drv/uart/UART.h>
#include <ti/drv/uart/UART_stdio.h>

#include <ti/board/board.h>

/*****************************************************************************
 * Internal Objects                                                          *
 *****************************************************************************/
extern const I2C_Config I2C_config[];
extern const SPI_Config SPI_config[];

typedef struct 
{
    I2C_Handle i2cHandle;
    SPI_Handle spiHandle;
} Board_gblObj;
extern Board_gblObj Board_obj;

/*****************************************************************************
 * Macros                                                                    *
 *****************************************************************************/

/* Macro values defining the different OPP Configuration supported. */
#define OPP_NOM                  (0)
#define OPP_OD                   (1)
#define OPP_HIGH                 (2)


/*****************************************************************************
 * Function Prototypes                                                       *
 *****************************************************************************/

Board_STATUS Board_unlockMMR();
Board_STATUS Board_PLLInit(Uint32 opp);
Board_STATUS Board_moduleClockInit();
Board_STATUS Board_DDR3Init();
Board_STATUS Board_watchdogDisable(); 
Board_STATUS Board_pinmuxConfig();
Board_STATUS Board_uartStdioInit();
Board_STATUS Board_internalInitI2C();

#ifdef __cplusplus
}
#endif

#endif
