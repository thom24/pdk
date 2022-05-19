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
 *  \file board_pinmux.c
 *
 *  \brief This file enables pinmux for the board 
 *
 */

#include <ti/csl/soc.h>
#include "board_internal.h"
#include "board_pinmux.h"

/**
 *  \brief  Sets pinmux mode for a pin in main domain
 *
 *  Only pinmux mode is updated by this function. Other values of
 *  padconfig register remains unchanged after this function call.
 *
 *  \param    offset [IN]    Pad config offset of the pin
 *  \param    mode   [IN]    Pad config mux mode.
 *
 *  \return   None
 */
void Board_pinMuxSetMode(uint32_t offset, uint32_t mode)
{

}

/**
 *  \brief  Sets pinmux mode for a pin in wake-up domain
 *
 *  Only pinmux mode is updated by this function. Other values of
 *  padconfig register remains unchanged after this function call.
 *
 *  \param    offset [IN]    Pad config offset of the pin
 *  \param    mode   [IN]    Pad config mux mode.
 *
 *  \return   None
 */
void Board_pinMuxSetModeWkup(uint32_t offset, uint32_t mode)
{

}

/**
 *  \brief Sets padconfig register of a pin at given offset
 *
 *  Configures whole padconfig register of the pin at given offset
 *  with the value in 'muxData'.
 *
 *  \param   domain  [IN]  SoC domain for pinmux
 *  \n                      BOARD_SOC_DOMAIN_MAIN - Main domain
 *  \n                      BOARD_SOC_DOMAIN_WKUP - Wakeup domain
 *
 *  \param   offset  [IN]  Pad config offset of the pin
 *  \param   muxData [IN]  Value to be written to padconfig register
 *
 *  \return   BOARD_SOK in case of success or appropriate error code
 *
 */
Board_STATUS Board_pinmuxSetReg(uint8_t  domain,
                                uint32_t offset,
                                uint32_t muxData)
{
    return BOARD_SOK;
}

/**
 *  \brief Gets padconfig register of a pin at given offset
 *
 *  \param   domain  [IN]  SoC domain for pinmux
 *  \n                      BOARD_SOC_DOMAIN_MAIN - Main domain
 *  \n                      BOARD_SOC_DOMAIN_WKUP - Wakeup domain
 *  \param   offset  [IN]  Pad config offset of the pin
 *  \param   muxData [OUT] Value of padconfig register
 *
 *  \return   BOARD_SOK in case of success or appropriate error code
 *
 */
Board_STATUS Board_pinmuxGetReg(uint8_t  domain,
                                uint32_t offset,
                                uint32_t *muxData)
{
    return BOARD_SOK;
}

/**
 *  \brief Sets the board pinmux configuration.
 *
 *  There are multiple addon cards that can connected to baseboard and
 *  multiple addon cards can be connected to one expansion connector.
 *  Pinmux configured through Board_init will be set to a default
 *  combination of the boards which can be changed using this function.
 *
 *  \n Usage:
 *  \n - Call Board_pinmuxGetCfg to get default pinmux config
 *  \n - Call Board_pinmuxSetCfg to change pinmux config
 *  \n - Call Board_init with pinmux flag to apply the updated pinmux config
 *
 *  \param   pinmuxCfg [IN]  Pinmux configurations
 *
 *  \return  BOARD_SOK in case of success or appropriate error code
 *
 */
Board_STATUS Board_pinmuxSetCfg(Board_PinmuxConfig_t *pinmuxCfg)
{
    return BOARD_SOK;
}

/**
 *  \brief Gets the board pinmux configuration.
 *
 *  There are multiple addon cards that can connected to baseboard and
 *  multiple addon cards can be connected to one expansion connector.
 *  Pinmux configured through Board_init will be set to a default
 *  combination of the boards which can be read using this function.
 *
 *  \param   pinmuxCfg [IN]  Pinmux configurations
 *
 *  \return  BOARD_SOK in case of success or appropriate error code
 *
 */
Board_STATUS Board_pinmuxGetCfg(Board_PinmuxConfig_t *pinmuxCfg)
{
    return BOARD_SOK;
}

/**
 * \brief  Board pinmuxing update function
 *
 * Provides the option to configure/update the pinmux.
 * This function can be used to change the pinmux set by
 * Board_init by default.
 *
 * \param   pinmuxData [IN]  Pinmux data structure
 * \param   domain     [IN]  SoC domain for pinmux
 *  \n                        BOARD_SOC_DOMAIN_MAIN - Main domain
 *  \n                        BOARD_SOC_DOMAIN_WKUP - Wakeup domain
 *
 * \return  BOARD_SOK in case of success or appropriate error code
 *
 */
Board_STATUS Board_pinmuxUpdate (pinmuxBoardCfg_t *pinmuxData,
                                 uint32_t domain)
{
    return BOARD_SOK;
}

/**
 * \brief  Board pinmuxing enable function
 *
 * Enables pinmux for the board interfaces. Pin mux is done based
 * on the default/primary functionality of the board. Any pins shared by
 * multiple interfaces need to be reconfigured to access the secondary
 * functionality.
 *
 * \param   void
 *
 * \return  BOARD_SOK in case of success or appropriate error code
 *
 */
Board_STATUS Board_pinmuxConfig (void)
{
    return BOARD_SOK;
}

/**
 * \brief  Board pinmuxing enable function for main domain
 *
 * Enables pinmux for the board interfaces connected to main domain.
 * Pin mux is done based on the default/primary functionality of the board.
 * Any pins shared by multiple interfaces need to be reconfigured to access
 * the secondary functionality.
 *
 * \param   void
 *
 * \return  BOARD_SOK in case of success or appropriate error code
 *
 */
Board_STATUS Board_pinmuxConfigMain (void)
{
    return BOARD_SOK;
}

/**
 * \brief  Board pinmuxing enable function for wakeup/mcu domain
 *
 * Enables pinmux for the board interfaces connected to wakeup/mcu domain.
 * Pin mux is done based on the default/primary functionality of the board.
 * Any pins shared by multiple interfaces need to be reconfigured to access
 * the secondary functionality.
 *
 * \param   void
 *
 * \return  BOARD_SOK in case of success or appropriate error code
 *
 */
Board_STATUS Board_pinmuxConfigWkup (void)
{
    return BOARD_SOK;
}

/**
 * \brief  Board UART Tx pinmux config function
 *
 * Enables pinmux for UART Tx lines
 *
 * \param   void
 *
 * \return  None
 *
 */
void Board_uartTxPinmuxConfig(void)
{

}
