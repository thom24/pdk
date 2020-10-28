/**
 * Note: This file was auto-generated by TI PinMux on 10/28/2020.
 *
 * \file   AM64x_pinmux.h
 *
 * \brief  This file contains pad configure register offsets and bit-field 
 *         value macros for different configurations,
 *
 *           BIT[21]		TXDISABLE		disable the pin's output driver
 *           BIT[18]		RXACTIVE		enable the pin's input buffer (typically kept enabled)
 *           BIT[17]		PULLTYPESEL		set the iternal resistor pull direction high or low (if enabled)
 *           BIT[16]		PULLUDEN		internal resistor disable (0 = enabled / 1 = disabled)
 *           BIT[3:0]		MUXMODE			select the desired function on the given pin
 *
 *  \copyright Copyright (CU) 2020 Texas Instruments Incorporated - 
 *             http://www.ti.com/
 */

#ifndef _AM64x_PIN_MUX_H_
#define _AM64x_PIN_MUX_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <ti/board/src/am64x_evm/include/pinmux.h>
#include <ti/csl/csl_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */
#define PIN_MODE(mode)	                (mode)
#define PINMUX_END                      (-1)

/** \brief Active mode configurations */
/** \brief Resistor enable */
#define PIN_PULL_DISABLE                (0x1U << 16U)
/** \brief Pull direction */
#define	PIN_PULL_DIRECTION              (0x1U << 17U)
/** \brief Receiver enable */
#define	PIN_INPUT_ENABLE                (0x1U << 18U)
/** \brief Driver disable */
#define	PIN_OUTPUT_DISABLE              (0x1U << 21U)
/** \brief Wakeup enable */
#define	PIN_WAKEUP_ENABLE               (0x1U << 29U)

/** \brief Pad config register offset in control module */

enum pinMainOffsets
{
	PIN_ADC0_AIN0		 = 0x02B0,
	PIN_ADC0_AIN1		 = 0x02B4,
	PIN_ADC0_AIN2		 = 0x02B8,
	PIN_ADC0_AIN3		 = 0x02BC,
	PIN_ADC0_AIN4		 = 0x02C0,
	PIN_ADC0_AIN5		 = 0x02C4,
	PIN_ADC0_AIN6		 = 0x02C8,
	PIN_ADC0_AIN7		 = 0x02CC,
	PIN_PRG1_PRU0_GPO18		 = 0x0100,
	PIN_PRG1_PRU0_GPO7		 = 0x00D4,
	PIN_PRG1_PRU0_GPO19		 = 0x0104,
	PIN_PRG1_PRU0_GPO17		 = 0x00FC,
	PIN_GPMC0_AD0		 = 0x003C,
	PIN_GPMC0_AD1		 = 0x0040,
	PIN_GPMC0_AD2		 = 0x0044,
	PIN_GPMC0_AD3		 = 0x0048,
	PIN_GPMC0_AD4		 = 0x004C,
	PIN_GPMC0_AD5		 = 0x0050,
	PIN_GPMC0_AD6		 = 0x0054,
	PIN_GPMC0_AD7		 = 0x0058,
	PIN_GPMC0_AD11		 = 0x0068,
	PIN_GPMC0_AD12		 = 0x006C,
	PIN_GPMC0_AD13		 = 0x0070,
	PIN_GPMC0_ADVN_ALE		 = 0x0084,
	PIN_GPMC0_OEN_REN		 = 0x0088,
	PIN_GPMC0_WEN		 = 0x008C,
	PIN_GPMC0_BE0N_CLE		 = 0x0090,
	PIN_GPMC0_WAIT0		 = 0x0098,
	PIN_GPMC0_WAIT1		 = 0x009C,
	PIN_GPMC0_WPN		 = 0x00A0,
	PIN_GPMC0_CSN0		 = 0x00A8,
	PIN_PRG0_PRU0_GPO18		 = 0x01A8,
	PIN_PRG0_PRU0_GPO19		 = 0x01AC,
	PIN_SPI1_CS0		 = 0x021C,
	PIN_SPI1_D1		 = 0x022C,
	PIN_GPMC0_AD8		 = 0x005C,
	PIN_GPMC0_AD9		 = 0x0060,
	PIN_GPMC0_AD10		 = 0x0064,
	PIN_GPMC0_BE1N		 = 0x0094,
	PIN_GPMC0_AD14		 = 0x0074,
	PIN_GPMC0_AD15		 = 0x0078,
	PIN_OSPI0_CSN1		 = 0x0030,
	PIN_OSPI0_CSN2		 = 0x0034,
	PIN_OSPI0_CSN3		 = 0x0038,
	PIN_SPI0_CS1		 = 0x020C,
	PIN_EXTINTN		 = 0x0278,
	PIN_MMC1_SDWP		 = 0x029C,
	PIN_PRG1_PRU1_GPO8		 = 0x0128,
	PIN_PRG1_PRU1_GPO5		 = 0x011C,
	PIN_PRG1_PRU0_GPO5		 = 0x00CC,
	PIN_GPMC0_CSN1		 = 0x00AC,
	PIN_GPMC0_DIR		 = 0x00A4,
	PIN_I2C0_SCL		 = 0x0260,
	PIN_I2C0_SDA		 = 0x0264,
	PIN_I2C1_SCL		 = 0x0268,
	PIN_I2C1_SDA		 = 0x026C,
	PIN_GPMC0_CSN2		 = 0x00B0,
	PIN_GPMC0_CSN3		 = 0x00B4,
	PIN_MCAN0_RX		 = 0x0254,
	PIN_MCAN0_TX		 = 0x0250,
	PIN_MCAN1_RX		 = 0x025C,
	PIN_MCAN1_TX		 = 0x0258,
	PIN_PRG0_PRU1_GPO19		 = 0x01FC,
	PIN_PRG0_PRU1_GPO18		 = 0x01F8,
	PIN_MMC1_CMD		 = 0x0294,
	PIN_MMC1_CLK		 = 0x028C,
	PIN_MMC1_DAT0		 = 0x0288,
	PIN_MMC1_DAT1		 = 0x0284,
	PIN_MMC1_DAT2		 = 0x0280,
	PIN_MMC1_DAT3		 = 0x027C,
	PIN_MMC1_SDCD		 = 0x0298,
	PIN_OSPI0_CLK		 = 0x0000,
	PIN_OSPI0_CSN0		 = 0x002C,
	PIN_OSPI0_D0		 = 0x000C,
	PIN_OSPI0_D1		 = 0x0010,
	PIN_OSPI0_D2		 = 0x0014,
	PIN_OSPI0_D3		 = 0x0018,
	PIN_OSPI0_D4		 = 0x001C,
	PIN_OSPI0_D5		 = 0x0020,
	PIN_OSPI0_D6		 = 0x0024,
	PIN_OSPI0_D7		 = 0x0028,
	PIN_OSPI0_DQS		 = 0x0008,
	PIN_PRG0_MDIO0_MDC		 = 0x0204,
	PIN_PRG0_MDIO0_MDIO		 = 0x0200,
	PIN_PRG0_PRU0_GPO0		 = 0x0160,
	PIN_PRG0_PRU0_GPO1		 = 0x0164,
	PIN_PRG0_PRU0_GPO11		 = 0x018C,
	PIN_PRG0_PRU0_GPO12		 = 0x0190,
	PIN_PRG0_PRU0_GPO13		 = 0x0194,
	PIN_PRG0_PRU0_GPO14		 = 0x0198,
	PIN_PRG0_PRU0_GPO15		 = 0x019C,
	PIN_PRG0_PRU0_GPO16		 = 0x01A0,
	PIN_PRG0_PRU0_GPO17		 = 0x01A4,
	PIN_PRG0_PRU0_GPO2		 = 0x0168,
	PIN_PRG0_PRU0_GPO3		 = 0x016C,
	PIN_PRG0_PRU0_GPO4		 = 0x0170,
	PIN_PRG0_PRU0_GPO5		 = 0x0174,
	PIN_PRG0_PRU0_GPO6		 = 0x0178,
	PIN_PRG0_PRU0_GPO7		 = 0x017C,
	PIN_PRG0_PRU0_GPO8		 = 0x0180,
	PIN_PRG0_PRU1_GPO0		 = 0x01B0,
	PIN_PRG0_PRU1_GPO1		 = 0x01B4,
	PIN_PRG0_PRU1_GPO11		 = 0x01DC,
	PIN_PRG0_PRU1_GPO12		 = 0x01E0,
	PIN_PRG0_PRU1_GPO13		 = 0x01E4,
	PIN_PRG0_PRU1_GPO14		 = 0x01E8,
	PIN_PRG0_PRU1_GPO15		 = 0x01EC,
	PIN_PRG0_PRU1_GPO16		 = 0x01F0,
	PIN_PRG0_PRU1_GPO2		 = 0x01B8,
	PIN_PRG0_PRU1_GPO3		 = 0x01BC,
	PIN_PRG0_PRU1_GPO4		 = 0x01C0,
	PIN_PRG0_PRU1_GPO5		 = 0x01C4,
	PIN_PRG0_PRU1_GPO6		 = 0x01C8,
	PIN_PRG0_PRU1_GPO8		 = 0x01D0,
	PIN_PRG1_MDIO0_MDC		 = 0x015C,
	PIN_PRG1_MDIO0_MDIO		 = 0x0158,
	PIN_PRG1_PRU1_GPO18		 = 0x0150,
	PIN_PRG1_PRU1_GPO19		 = 0x0154,
	PIN_PRG1_PRU0_GPO0		 = 0x00B8,
	PIN_PRG1_PRU0_GPO1		 = 0x00BC,
	PIN_PRG1_PRU0_GPO2		 = 0x00C0,
	PIN_PRG1_PRU0_GPO3		 = 0x00C4,
	PIN_PRG1_PRU0_GPO6		 = 0x00D0,
	PIN_PRG1_PRU0_GPO4		 = 0x00C8,
	PIN_PRG1_PRU0_GPO11		 = 0x00E4,
	PIN_PRG1_PRU0_GPO12		 = 0x00E8,
	PIN_PRG1_PRU0_GPO13		 = 0x00EC,
	PIN_PRG1_PRU0_GPO14		 = 0x00F0,
	PIN_PRG1_PRU0_GPO16		 = 0x00F8,
	PIN_PRG1_PRU0_GPO15		 = 0x00F4,
	PIN_PRG1_PRU1_GPO0		 = 0x0108,
	PIN_PRG1_PRU1_GPO1		 = 0x010C,
	PIN_PRG1_PRU1_GPO2		 = 0x0110,
	PIN_PRG1_PRU1_GPO3		 = 0x0114,
	PIN_PRG1_PRU1_GPO6		 = 0x0120,
	PIN_PRG1_PRU1_GPO4		 = 0x0118,
	PIN_PRG1_PRU1_GPO11		 = 0x0134,
	PIN_PRG1_PRU1_GPO12		 = 0x0138,
	PIN_PRG1_PRU1_GPO13		 = 0x013C,
	PIN_PRG1_PRU1_GPO14		 = 0x0140,
	PIN_PRG1_PRU1_GPO16		 = 0x0148,
	PIN_PRG1_PRU1_GPO15		 = 0x0144,
	PIN_PRG0_PRU1_GPO7		 = 0x01CC,
	PIN_PRG0_PRU1_GPO9		 = 0x01D4,
	PIN_PRG0_PRU1_GPO10		 = 0x01D8,
	PIN_PRG0_PRU1_GPO17		 = 0x01F4,
	PIN_PRG0_PRU0_GPO10		 = 0x0188,
	PIN_PRG0_PRU0_GPO9		 = 0x0184,
	PIN_PRG1_PRU1_GPO7		 = 0x0124,
	PIN_PRG1_PRU1_GPO9		 = 0x012C,
	PIN_PRG1_PRU1_GPO10		 = 0x0130,
	PIN_PRG1_PRU1_GPO17		 = 0x014C,
	PIN_PRG1_PRU0_GPO10		 = 0x00E0,
	PIN_PRG1_PRU0_GPO9		 = 0x00DC,
	PIN_SPI0_CLK		 = 0x0210,
	PIN_SPI0_CS0		 = 0x0208,
	PIN_SPI0_D0		 = 0x0214,
	PIN_SPI0_D1		 = 0x0218,
	PIN_SPI1_CLK		 = 0x0224,
	PIN_SPI1_CS1		 = 0x0220,
	PIN_SPI1_D0		 = 0x0228,
	PIN_EXT_REFCLK1		 = 0x0274,
	PIN_GPMC0_CLK		 = 0x007C,
	PIN_PORZ_OUT		 = 0x02AC,
	PIN_RESETSTATZ		 = 0x02A4,
	PIN_ECAP0_IN_APWM_OUT		 = 0x0270,
	PIN_UART0_CTSN		 = 0x0238,
	PIN_UART0_RTSN		 = 0x023C,
	PIN_UART0_RXD		 = 0x0230,
	PIN_UART0_TXD		 = 0x0234,
	PIN_UART1_RXD		 = 0x0240,
	PIN_UART1_TXD		 = 0x0244,
	PIN_UART1_CTSN		 = 0x0248,
	PIN_UART1_RTSN		 = 0x024C,
};

enum pinWkupOffsets
{
	PIN_MCU_SPI1_CS0		 = 0x0014,
	PIN_MCU_I2C0_SCL		 = 0x0048,
	PIN_MCU_I2C0_SDA		 = 0x004C,
	PIN_MCU_I2C1_SCL		 = 0x0050,
	PIN_MCU_I2C1_SDA		 = 0x0054,
	PIN_MCU_SPI0_CLK		 = 0x0008,
	PIN_MCU_SPI0_CS0		 = 0x0000,
	PIN_MCU_SPI0_CS1		 = 0x0004,
	PIN_MCU_SPI0_D0		 = 0x000C,
	PIN_MCU_SPI0_D1		 = 0x0010,
	PIN_MCU_SPI1_CLK		 = 0x001C,
	PIN_MCU_SPI1_CS1		 = 0x0018,
	PIN_MCU_SPI1_D0		 = 0x0020,
	PIN_MCU_SPI1_D1		 = 0x0024,
	PIN_MCU_PORZ		 = 0x005C,
	PIN_MCU_RESETSTATZ		 = 0x0060,
	PIN_MCU_RESETZ		 = 0x0058,
	PIN_MCU_SAFETY_ERRORN		 = 0x0064,
	PIN_MCU_UART0_CTSN		 = 0x0030,
	PIN_MCU_UART0_RTSN		 = 0x0034,
	PIN_MCU_UART0_RXD		 = 0x0028,
	PIN_MCU_UART0_TXD		 = 0x002C,
	PIN_MCU_UART1_CTSN		 = 0x0040,
	PIN_MCU_UART1_RTSN		 = 0x0044,
	PIN_MCU_UART1_RXD		 = 0x0038,
	PIN_MCU_UART1_TXD		 = 0x003C,
};

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/** \brief Pinmux configuration data for the board. Auto-generated from 
           Pinmux tool. */
extern pinmuxBoardCfg_t gAM64x_MainPinmuxData[];
extern pinmuxBoardCfg_t gAM64x_WkupPinmuxData[];

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* _AM64x_PIN_MUX_H_ */
