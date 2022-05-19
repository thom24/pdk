/******************************************************************************
 * Copyright (c) 2022 Texas Instruments Incorporated - http://www.ti.com
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
 *  \file board_info.c
 *
 *  \brief This file contains the functions to read/write board info data 
 *
 */

#include "board_utils.h"
#include "board_internal.h"
#include "board_cfg.h"
#include <stdio.h>
#include <string.h>

extern Board_I2cInitCfg_t gBoardI2cInitCfg;

/**
 *  @brief  This function is not supported by this platform.
 *
 *  Function implementation for build backward compatibilty.
 *  Always returns 'BOARD_UNSUPPORTED_FEATURE'
 *
 */
Board_STATUS Board_getIDInfo(Board_IDInfo *info)
{
    return BOARD_UNSUPPORTED_FEATURE;
}

/**
 *  @brief      Get board information.
 *  
 *  This function requires the information of I2C instance and domain
 *  to which board ID EEPROM is connected. This need to be set using
 *  Board_setI2cInitConfig() before calling this function.
 *
 *  @param[out] Board_STATUS
 *    Returns status on API call
 *  @param[out] info
 *    This structure will have board information on return
 *  @param[in] slaveAddress
 *    I2C slave address of EEPROM to be read
 *
 */
Board_STATUS Board_getIDInfo_v2(Board_IDInfo_v2 *info, uint8_t slaveAddress)
{
    return BOARD_SOK;
}

/**
 *  @brief  This function is not supported by this platform.
 *
 *  Function implementation for build backward compatibilty.
 *  Always returns 'BOARD_UNSUPPORTED_FEATURE'
 *
 */
Board_STATUS Board_writeIDInfo(Board_IDInfo *info)
{
    return BOARD_UNSUPPORTED_FEATURE;
}

/**
 *  @brief  Write board id contents to specific EEPROM.
 *
 *  This function requires the information of I2C instance and domain
 *  to which board ID EEPROM is connected. This need to be set using
 *  Board_setI2cInitConfig() before calling this function.
 *
 *  @param[out] Board_STATUS
 *    Returns status on API call
 * @param[out] info
 *    Structure contain board id contents to write
 *  @param[in] slaveAddress
 *    Address of eeprom
 *
 */
Board_STATUS Board_writeIDInfo_v2(Board_IDInfo_v2 *info, uint8_t slaveAddress)
{
    return BOARD_SOK;
}
