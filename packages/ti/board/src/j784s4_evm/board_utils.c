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
 *  \file   board_utils.c
 *
 *  \brief  Implements multiple board utility functions
 *
 */

#include <stdio.h>
#include <string.h>
#include <ti/csl/arch/csl_arch.h>
#include "board_internal.h"
#include "board_utils.h"
#include "board_cfg.h"

Board_I2cInitCfg_t gBoardI2cInitCfg = {0, BOARD_SOC_DOMAIN_MAIN, 0};
Board_initParams_t gBoardInitParams = {BOARD_UART_INSTANCE, BOARD_UART_SOC_DOMAIN, BOARD_PSC_DEVICE_MODE_NONEXCLUSIVE,
                                       BOARD_MAIN_CLOCK_GROUP_ALL, BOARD_MCU_CLOCK_GROUP_ALL};

/**
 * \brief Board ID read function
 *
 * \param   info     [IN]  Board info structure
 * \param   boardID  [IN]  ID of the board to be detected
 * \n                      BOARD_ID_GESI(0x0) - GESI Board
 * \n                      BOARD_ID_FUSION2(0x1) - Fusion 2 Board
 * \n                      BOARD_ID_SOM(0x2) - SoM Board
 * \n                      BOARD_ID_CP(0x3) - CP Board
 * \n                      BOARD_ID_ENET(0x4) - Quad ENET expansion
 *
 * \return   BOARD_SOK in case of success or appropriate error code.
 *
 */
Board_STATUS Board_getBoardData(Board_IDInfo_v2 *info, uint32_t boardID)
{
    return BOARD_SOK;
}

/**
 * \brief Board detect function
 *
 *  Checks if the board with given 'boardID' is connected to the
 *  base board.
 *
 *  \n Note: Board ID EEPROM should be programmed for this function
 *           to work properly.
 *
 * \param   boardID  [IN]  ID of the board to be detected
 * \n                      BOARD_ID_GESI(0x0) - GESI Board
 * \n                      BOARD_ID_FUSION2(0x1) - Fusion 2 Board
 * \n                      BOARD_ID_SOM(0x2) - SoM Board
 * \n                      BOARD_ID_CP(0x3) - CP Board
 * \n                      BOARD_ID_ENET(0x4) - Quad ENET expansion
 *
 * \return   TRUE if the given board is detected else FALSE.
 *           SoM board will be always connected to the base board.
 *           For SoM boardID return value TRUE indicates dual PMIC
 *           SoM and FALSE indicates alternate PMIC SoM
 *
 */
bool Board_detectBoard(uint32_t boardID)
{
    return BOARD_SOK;
}

/**
 * \brief  Checks for Alpha board revision
 *
 * \param   boardID  [IN]  ID of the board to be detected
 * \n                      BOARD_ID_GESI(0x0) - GESI Board
 * \n                      BOARD_ID_FUSION2(0x1) - Fusion 2 Board
 * \n                      BOARD_ID_SOM(0x2) - SoM Board
 * \n                      BOARD_ID_CP(0x3) - CP Board
 * \n                      BOARD_ID_ENET(0x4) - Quad ENET expansion
 *
 * \return TRUE if board revision is E2, FALSE for all other cases
 */
bool Board_isAlpha(uint32_t boardID)
{
    return FALSE;
}

/**
 *  \brief    Function to detect ENET expansion application card type
 *
 *
 *  \return
 */
int32_t Board_detectEnetCard(void)
{
    return 0;
}

/**
 * \brief Read MAC ID function
 *
 *  This function reads the MAC addresses programmed to the board ID EEPROM
 *  on the boards with Ethernet ports. Exception is for MCU Ethernet port
 *  which is supposed to use MAC ID from SoC MMR registers.
 *
 *  There can be multiple MAC IDs stored in the EEPROM based on the
 *  number of Ethernet ports on the board. Number of MAC IDs copied
 *  to 'macAddrBuf' can be read using 'macAddrCnt' parameters.
 *
 *  Each MAC address will be 6 bytes long. MAC IDs will be copied to buffer
 *  based on 'macBufSize'. If the buffer size is long enough, all the MAC
 *  addresses from EEPROM will be copied to 'macAddrBuf' else fewer MAC
 *  IDs to fit within 'macBufSize'. MAC count for a given board can be
 *  read using Board_readMacAddrCount() function.
 *
 *  \n Note: Board ID EEPROM should be programmed for this function
 *           to work properly.
 *
 * \param  boardID  [IN]  ID of the board to be detected
 * \n                      BOARD_ID_GESI(0x0) - GESI Board
 * \n                      BOARD_ID_FUSION2(0x1) - Fusion 2 Board
 * \n                      BOARD_ID_SOM(0x2) - SoM Board
 * \n                      BOARD_ID_CP(0x3) - CP Board
 * \param  macAddrBuf[OUT] Buffer to write MAC IDs read from EEPROM
 * \param  macBufSize[IN]  Size of the macAddrBuf
 * \param  macAddrCnt[OUT] Number of valid MAC addresses programmed to the EEPROM
 *                         This an optional variable to read the MAC ID count
 *                         filled to the 'macAddrBuf'.
 *                         Pass a valid address to get MAC ID count.
 *
 * \return   BOARD_SOK in case of success or appropriate error code.
 *
 */
Board_STATUS Board_readMacAddr(uint32_t boardID,
                               uint8_t  *macAddrBuf,
                               uint32_t macBufSize,
                               uint32_t *macAddrCnt)
{
    return BOARD_SOK;
}

/**
 * \brief Read MAC ID count
 *
 *  This function reads the number of MAC addresses programmed to
 *  board ID EEPROM on the boards with Ethernet ports. Exception is
 *  for MCU Ethernet port which is supposed to use MAC ID from SoC
 *  MMR registers. Each MAC address programmed to EEPROM is 6 bytes long.
 *
 *  \n Note: Board ID EEPROM should be programmed for this function
 *           to work properly.
 *
 * \param  boardID  [IN]  ID of the board to be detected
 * \n                      BOARD_ID_GESI(0x0) - GESI Board
 * \n                      BOARD_ID_FUSION2(0x1) - Fusion 2 Board
 * \n                      BOARD_ID_SOM(0x2) - SoM Board
 * \n                      BOARD_ID_CP(0x3) - CP Board
 * \n                      BOARD_ID_ENET(0x4) - Quad ENET expansion
 * \param  macAddrCnt[OUT] Number of valid MAC addresses programmed to the EEPROM
 *
 * \return   BOARD_SOK in case of success or appropriate error code.
 *
 */
Board_STATUS Board_readMacAddrCount(uint32_t boardID,
                                    uint32_t *macAddrCnt)
{
    return BOARD_SOK;
}

/**
 * \brief Function to configure I2C configurations used by board
 *
 * This function is used to set the I2C controller instance and
 * SoC domain used by the board module for IO expander and board
 * ID info read.
 *
 * Usage:
 * Call Board_setI2cInitConfig to set the I2C configurations
 * Call IO expander Init or Board ID info read/write
 *
 *  \return   BOARD_SOK in case of success or appropriate error code.
 *
 */
Board_STATUS Board_setI2cInitConfig(Board_I2cInitCfg_t *i2cCfg)
{
    return BOARD_SOK;
}

/**
 * \brief Function to get board init params
 *
 *  This function shall be used to know the current board init
 *  parameters and update them if needed using the function Board_setInitParams.
 *
 * \param   initParams  [IN]  Board init params structure
 *
 * \return   BOARD_SOK in case of success or appropriate error code.
 *
 */
Board_STATUS Board_getInitParams(Board_initParams_t *initParams)
{
    return BOARD_SOK;
}

/**
 * \brief Function to configure board init parameters
 *
 *  Board init params includes the parameters used by Board_init
 *  function for different operations. Default init parameters
 *  used by Board_init can be updated using this function.
 *  All the default params can be overwritten by calling this function
 *  or some of can be changed by reading the existing init parameters
 *  using Board_getInitParams function.
 *
 * Usage:
 * Call Board_getInitParams to get the default board init parameters
 * Update the parameters as needed
 * Call Board_setInitParams to update the default board init parameters
 *
 * \param   initCfg  [IN]  Board Init config structure
 *
 * \return   BOARD_SOK in case of success or appropriate error code.
 *
 */
Board_STATUS Board_setInitParams(Board_initParams_t *initParams)
{
    return BOARD_SOK;
}

/**
 * \brief Function to get the SoC domain
 *
 *  This function returns the domain of the SoC core on which
 *  it is executing.
 *
 * \return   SoC domain of the core.
 *
 */
uint32_t Board_getSocDomain(void)
{
    uint32_t socDomain = BOARD_SOC_DOMAIN_MAIN;

    return socDomain;
}

/**
 *  \brief  Sets RAT configuration
 *
 *  MAIN padconfig registers are not directly accessible for C66x core
 *  which requires RAT configuration for the access.
 *
 *  \return   None
 */
void Board_setRATCfg(void)
{

}

/**
 *  \brief  Restores RAT configuration
 *
 *  \return   None
 */
void Board_restoreRATCfg(void)
{

}

/**
 *  \brief    Function to generate delay in micro seconds
 *
 *  This function takes the delay parameters in usecs but the generated
 *  delay will be in multiples of msecs due to the osal function which
 *  generates delay in msecs. Delay parameter passed will be converted to
 *  msecs and fractional value will be adjusted to nearest msecs value.
 *  Minimum delay generated by this function is 1 msec.
 *  Function parameter is kept in usecs to match with existing
 *  platforms which has delay function for usecs.
 *
 *  \param    usecs [IN]  Specifies the time to delay in micro seconds.
 *
 */
void BOARD_delay(uint32_t usecs)
{

}
