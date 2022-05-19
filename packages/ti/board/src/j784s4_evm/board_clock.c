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

 /** \file board_clock.c
  *
  *  \brief This file contains initialization of wakeup and main PSC
  *  configuration structures and function definitions to get the number
  *  of wakeup and main PSC config exists.
  */

#include "board_clock.h"
#include "board_utils.h"
#include <ti/drv/sciclient/sciclient.h>

/**
 * \brief Disables module clock
 *
 * \return  BOARD_SOK   - Clock disable successful.
 *          BOARD_FAIL  - Clock disable failed.
 *
 */
Board_STATUS Board_moduleClockDisable(uint32_t moduleId)
{
    return BOARD_SOK;
}

/**
 * \brief Enables module clock
 *
 * \return  BOARD_SOK   - Clock enable sucessful.
 *          BOARD_FAIL  - Clock enable failed.
 *
 */
Board_STATUS Board_moduleClockEnable(uint32_t moduleId)
{
    return BOARD_SOK;
}

/**
 * \brief clock Initialization function for MCU domain
 *
 * Enables different power domains and peripheral clocks of the MCU.
 * Some of the power domains and peripherals will be OFF by default.
 * Enabling the power domains is mandatory before accessing using
 * board interfaces connected to those peripherals.
 *
 * \return  BOARD_SOK              - Clock initialization sucessful.
 *          BOARD_INIT_CLOCK_FAIL  - Clock initialization failed.
 *
 */
Board_STATUS Board_moduleClockInitMcu(void)
{
    return BOARD_SOK;
}

/**
 * \brief clock Initialization function for MAIN domain
 *
 * Enables different power domains and peripheral clocks of the SoC.
 * Some of the power domains and peripherals will be OFF by default.
 * Enabling the power domains is mandatory before accessing using
 * board interfaces connected to those peripherals.
 *
 * \return  BOARD_SOK              - Clock initialization successful.
 *          BOARD_INIT_CLOCK_FAIL  - Clock initialization failed.
 *
 */
Board_STATUS Board_moduleClockInitMain(void)
{
    return BOARD_SOK;
}

/**
 * \brief clock de-initialization function for MCU domain
 *
 * Disables different power domains and peripheral clocks of the SoC.
 *
 * \return  BOARD_SOK              - Clock de-initialization successful. 
 *          BOARD_INIT_CLOCK_FAIL  - Clock de-initialization failed.
 *
 */
Board_STATUS Board_moduleClockDeinitMcu(void)
{
    return BOARD_SOK;
}

/**
 * \brief clock de-initialization function for MAIN domain
 *
 * Disables different power domains and peripheral clocks of the SoC.
 *
 * \return  BOARD_SOK              - Clock de-initialization successful.
 *          BOARD_INIT_CLOCK_FAIL  - Clock de-initialization failed.
 *
 */
Board_STATUS Board_moduleClockDeinitMain(void)
{
    return BOARD_SOK;
}
