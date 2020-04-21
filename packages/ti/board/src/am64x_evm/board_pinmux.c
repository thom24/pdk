/******************************************************************************
 * Copyright (c) 2019 Texas Instruments Incorporated - http://www.ti.com
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
 *  \brief This file Enables pinmux for the board 
 *
 */

#include <ti/csl/soc.h>				
#include "board_internal.h"
#include "board_pinmux.h"

/**
 *  \brief    This function used to set the specified pinMux
 *            mode for a specified pinMux offset address register.
 *
 *  \param    offset     [IN]       Pad config offset address
 *            mode       [IN]       Pad config mux mode.
 *
 *  \return   int8_t
 *               0 - in case of success
 *              -1 - in case of failure.
 *
 */
void Board_pinMuxSetMode(uint32_t offset, uint32_t mode)
{
    
}

/**
 *  \brief    This function used to set the specified pinMux
 *            mode for a specified pinMux offset address register
 *            of a wkup domain.
 *
 *  \param    offset     [IN]       Pad config offset address
 *            mode       [IN]       Pad config mux mode.
 *
 *  \return   int8_t
 *               0 - in case of success
 *              -1 - in case of failure.
 *
 */
void Board_wkupPinMuxSetMode(uint32_t offset, uint32_t mode)
{

}

/**
 *  \brief    This function used to get the specified pinMux configuration
 *            for a specified pinMux offset address register of a specified
 *            domain.
 *
 *  \param    domainType   [IN]       enum variable to specify the domain
 *            offset       [IN]       Pad config offset address
 *            *dataBuff    [IN/OUT]   Pointer to data buffer.
 *
 *  \return   BOARD_SOK in case of success or appropriate error code
 *
 */
Board_STATUS Board_pinMuxGetCfg(domainType_t domainType, uint32_t offset, uint32_t *dataBuff)
{
    return BOARD_SOK;
}

/**
 *  \brief    This function used to set the specified pinMux configuration
 *            for a specified pinMux offset address register of a specified
 *            domain.
 *
 *  \param    domainType   [IN]       enum variable to specify the domain
 *            offset       [IN]       Pad config offset address
 *            cfgData      [IN]       Pad config pinmux data.
 *
 *  \return   BOARD_SOK in case of success or appropriate error code
 *
 */
Board_STATUS Board_pinMuxSetCfg(domainType_t domainType, uint32_t offset, uint32_t cfgData)
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
