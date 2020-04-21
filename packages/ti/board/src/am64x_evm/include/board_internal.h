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

#include <ti/drv/uart/UART.h>
#include <ti/drv/uart/UART_stdio.h>
#include <ti/drv/uart/soc/UART_soc.h>

#include <ti/board/board.h>
#include <ti/csl/tistdtypes.h>
#include <stdio.h>
#include <stdbool.h>

#define MODE_PIN_MASK                        (0xFU)
#define PINMUX_BIT_MASK                      (0xFFF8FFF0U)
#define GPIO_PIN_MUX_CFG                     (0x50007U)

/* MAIN CTRL base address + offset to beginning of PAD CONFIG  section */
#define MAIN_PMUX_CTRL	                     (0)  //J7ES_TODO: Need to update

/* WKUP CTRL base address + offset to beginning of PAD CONFIG section */
#define WKUP_PMUX_CTRL	                     (0)  //J7ES_TODO: Need to update

/*****************************************************************************
 * Internal Objects                                                          *
 *****************************************************************************/
extern const I2C_Config I2C_config[];

typedef struct
{
    I2C_Handle i2cHandle;
} Board_gblObj;

extern Board_gblObj Board_obj;

/****************************************************************************/

#define KICK0_UNLOCK_VAL                     (0x68EF3490U)
#define KICK1_UNLOCK_VAL                     (0xD172BC5AU)

/* The below macro are for temporary use only, Once the CSL macros are 
 *  added these can be removed */ 

#define MAIN_PSC_ADDR_OFFSET                 (0x80000000U)  //J7ES_TODO: Need to update
#define WAKEUP_PSC_ADDR_OFFSET               (0x20000000U)  //J7ES_TODO: Need to update

/*****************************************************************************
 * Function Prototypes                                                       *
 *****************************************************************************/

/**
 *
 * \brief  Board pinmuxing enable function
 *
 * Enables pinmux for the Maxwell idk board interfaces. Pin mux is done based on the
 * default/primary functionality of the board. Any pins shared by multiple
 * interfaces need to be reconfigured to access the secondary functionality.
 *
 * \return  BOARD_SOK in case of success or appropriate error code
 *
 */
Board_STATUS Board_pinmuxConfig(void);

/**
 *
 * \brief  Board PLL initialization function
 *
 *  Configures different PLL controller modules. This enables all the PLL
 *  controllers on the SoC with default configurations.
 *
 * \return BOARD_SOK in case of success or appropriate error code
 */
Board_STATUS Board_PLLInit(uint32_t modId, uint32_t clkId, uint64_t clkRate);

/**
 *
 * \brief DDR4 Initialization function
 *
 * Initializes the DDR timing parameters. Sets the DDR timing parameters
 * based in the DDR PLL controller configuration done by the board library.
 * Any changes to DDR PLL requires change to DDR timing.
 *
 * \return  BOARD_SOK in case of success or appropriate error code
 *
 */
Board_STATUS Board_DDRInit(void);

/**
 *
 * \brief clock Initialization function
 *
 * Enables different power domains and peripheral clocks of the SoC.
 * Some of the power domains and peripherals will be off by default.
 * Enabling the power domains is mandatory before accessing using
 * board interfaces connected to those peripherals.
 *
 * \return  BOARD_SOK in case of success or appropriate error code
 *
 */
Board_STATUS Board_moduleClockInit(void);

/**
 * \brief  Board specific configurations for Gigabit Ethernet PHYs
 *
 * This function takes care of configuring the internal delays for gigabit
 * Ethernet PHY. 2.25ns delay is configured for Rx, and .25ns delay is
 * configured for Tx
 *
 * \return  none
 */
Board_STATUS Board_mcuEthConfig(void);

/**
 * \brief  Board specific configurations for ICSS EMAC Ethernet PHYs
 *
 *  This function takes care of making several board level configurations
 *  required for ICSS EMAC PHY as listed below.
 *   - Disabling internal SoC pull up/down for the pins used for strapping
 *   - Setting the GPIOs for PHY reset, routing ICSS signals to PHYs,
 *     PHY interrupt lines.
 *   - Resetting the PHYs for proper address latching
 *   - MDIO initialization
 *   - MDIO configuration for setting MII mode
 *
 * \return  none
 */
Board_STATUS Board_icssEthConfig(void);

/**
 * \brief   This function initializes the default UART instance for use for
 *          console operations.
 *
 * \return  Board_STATUS in case of success or appropriate error code.
 *
 */
Board_STATUS Board_uartStdioInit(void);

/**
 * \brief   This function initializes the i2c instance connected to the
 *          board Id EEPROM.
 * This function disables the interrupt mode as the Board i2c instance
 * doesn't require interrupt mode and restores back original at the end.
 *
 * \param   i2cInst  [IN]  i2c instance connected to board Id EEPROM and
 *                         IO expander device.
 *
 * \return  Board_STATUS in case of success or appropriate error code.
 *
 */
Board_STATUS Board_internalInitI2C(uint8_t i2cInst);

/**
 * \brief board detect test
 *
 * This function used to check whether the specified board exists.
 *
 * \param   detectBoard  [IN]  enum used to send the name of the
 *                             board going to be detected.
 *
 * \return   bool
 *                    true   - In case of specified board detected
 *                    false  - In case of specified board not detected.
 *
 */
bool Board_detectBoard(boardPresDetect_t detectBoard);

/**
 * \brief  Unlocks MMR registers
 *
 * \return  Board_STATUS
 */
Board_STATUS Board_unlockMMR(void);

/**
 *  \brief Serdes configurations
 *
 *  The function detects the personality boards connected and configures the
 *  respective module.
 *
 *  \return   BOARD_SOK in case of success or appropriate error code
 *
 */
Board_STATUS Board_serdesCfg(void);

/**
 *
 * \brief  Board PLL initialization function
 *
 *  Configures different PLL controller modules. This enables all the PLL
 *  controllers on the SoC with default configurations.
 *
 * \return BOARD_SOK in case of success or appropriate error code
 */
Board_STATUS Board_PLLInitAll(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* BOARD_INTERNAL_H_ */
