/**
 * Note: This file was auto-generated by TI PinMux on 7/30/2019 at 2:27:25 AM.
 *
 * \file   AM65xx_pinmux.h
 *
 * \brief  This file contains pad configure register offsets and bit-field 
 *         value macros for different configurations,
 *
 *           BIT[20:19]		BUFFERCLASS		set the pin's output driver characteristics
 *           BIT[18]		RXACTIVE		enable the pin's input buffer (typically kept enabled)
 *           BIT[17]		PULLTYPESEL		set the iternal resistor pull direction high or low (if enabled)
 *           BIT[16]		PULLUDEN		internal resistor disable (0 = enabled / 1 = disabled)
 *           BIT[3:0]		MUXMODE			select the desired function on the given pin
 *
 *  \copyright Copyright (CU) 2019 Texas Instruments Incorporated - 
 *             http://www.ti.com/
 */

#ifndef _AM65XX_PIN_MUX_H_
#define _AM65XX_PIN_MUX_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <ti/board/src/am65xx_idk/include/pinmux.h>
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
/** \brief Resistor disable */
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
    PIN_GPMC0_AD0              = 0x0000U,
    PIN_GPMC0_AD1              = 0x0004U,
    PIN_GPMC0_AD2              = 0x0008U,
    PIN_GPMC0_AD3              = 0x000CU,
    PIN_GPMC0_AD4              = 0x0010U,
    PIN_GPMC0_AD5              = 0x0014U,
    PIN_GPMC0_AD6              = 0x0018U,
    PIN_GPMC0_AD7              = 0x001CU,
    PIN_GPMC0_AD8              = 0x0020U,
    PIN_GPMC0_AD9              = 0x0024U,
    PIN_GPMC0_AD10             = 0x0028U,
    PIN_GPMC0_AD11             = 0x002CU,
    PIN_GPMC0_AD12             = 0x0030U,
    PIN_GPMC0_AD13             = 0x0034U,
    PIN_GPMC0_AD14             = 0x0038U,
    PIN_GPMC0_AD15             = 0x003CU,
    PIN_GPMC0_CLK              = 0x0040U,
    PIN_GPMC0_ADVN_ALE         = 0x0044U,
    PIN_GPMC0_OEN_REN          = 0x0048U,
    PIN_GPMC0_WEN              = 0x004CU,
    PIN_GPMC0_BE0N_CLE         = 0x0050U,
    PIN_GPMC0_BE1N             = 0x0054U,
    PIN_GPMC0_WAIT0            = 0x0058U,
    PIN_GPMC0_WAIT1            = 0x005CU,
    PIN_GPMC0_WPN              = 0x0060U,
    PIN_GPMC0_DIR              = 0x0064U,
    PIN_GPMC0_CSN0             = 0x0068U,
    PIN_GPMC0_CSN1             = 0x006CU,
    PIN_GPMC0_CSN2             = 0x0070U,
    PIN_GPMC0_CSN3             = 0x0074U,
    PIN_PRG2_PRU0_GPO0         = 0x0078U,
    PIN_PRG2_PRU0_GPO1         = 0x007CU,
    PIN_PRG2_PRU0_GPO2         = 0x0080U,
    PIN_PRG2_PRU0_GPO3         = 0x0084U,
    PIN_PRG2_PRU0_GPO4         = 0x0088U,
    PIN_PRG2_PRU0_GPO5         = 0x008CU,
    PIN_PRG2_PRU0_GPO6         = 0x0090U,
    PIN_PRG2_PRU0_GPO7         = 0x0094U,
    PIN_PRG2_PRU0_GPO8         = 0x0098U,
    PIN_PRG2_PRU0_GPO9         = 0x009CU,
    PIN_PRG2_PRU0_GPO10        = 0x00A0U,
    PIN_PRG2_PRU0_GPO11        = 0x00A4U,
    PIN_PRG2_PRU0_GPO16        = 0x00A8U,
    PIN_PRG2_PRU1_GPO0         = 0x00ACU,
    PIN_PRG2_PRU1_GPO1         = 0x00B0U,
    PIN_PRG2_PRU1_GPO2         = 0x00B4U,
    PIN_PRG2_PRU1_GPO3         = 0x00B8U,
    PIN_PRG2_PRU1_GPO4         = 0x00BCU,
    PIN_PRG2_PRU1_GPO5         = 0x00C0U,
    PIN_PRG2_PRU1_GPO6         = 0x00C4U,
    PIN_PRG2_PRU1_GPO7         = 0x00C8U,
    PIN_PRG2_PRU1_GPO8         = 0x00CCU,
    PIN_PRG2_PRU1_GPO9         = 0x00D0U,
    PIN_PRG2_PRU1_GPO10        = 0x00D4U,
    PIN_PRG2_PRU1_GPO11        = 0x00D8U,
    PIN_PRG2_PRU1_GPO16        = 0x00DCU,
    PIN_PRG1_PRU0_GPO0         = 0x00E0U,
    PIN_PRG1_PRU0_GPO1         = 0x00E4U,
    PIN_PRG1_PRU0_GPO2         = 0x00E8U,
    PIN_PRG1_PRU0_GPO3         = 0x00ECU,
    PIN_PRG1_PRU0_GPO4         = 0x00F0U,
    PIN_PRG1_PRU0_GPO5         = 0x00F4U,
    PIN_PRG1_PRU0_GPO6         = 0x00F8U,
    PIN_PRG1_PRU0_GPO7         = 0x00FCU,
    PIN_PRG1_PRU0_GPO8         = 0x0100U,
    PIN_PRG1_PRU0_GPO9         = 0x0104U,
    PIN_PRG1_PRU0_GPO10        = 0x0108U,
    PIN_PRG1_PRU0_GPO11        = 0x010CU,
    PIN_PRG1_PRU0_GPO12        = 0x0110U,
    PIN_PRG1_PRU0_GPO13        = 0x0114U,
    PIN_PRG1_PRU0_GPO14        = 0x0118U,
    PIN_PRG1_PRU0_GPO15        = 0x011CU,
    PIN_PRG1_PRU0_GPO16        = 0x0120U,
    PIN_PRG1_PRU0_GPO17        = 0x0124U,
    PIN_PRG1_PRU0_GPO18        = 0x0128U,
    PIN_PRG1_PRU0_GPO19        = 0x012CU,
    PIN_PRG1_PRU1_GPO0         = 0x0130U,
    PIN_PRG1_PRU1_GPO1         = 0x0134U,
    PIN_PRG1_PRU1_GPO2         = 0x0138U,
    PIN_PRG1_PRU1_GPO3         = 0x013CU,
    PIN_PRG1_PRU1_GPO4         = 0x0140U,
    PIN_PRG1_PRU1_GPO5         = 0x0144U,
    PIN_PRG1_PRU1_GPO6         = 0x0148U,
    PIN_PRG1_PRU1_GPO7         = 0x014CU,
    PIN_PRG1_PRU1_GPO8         = 0x0150U,
    PIN_PRG1_PRU1_GPO9         = 0x0154U,
    PIN_PRG1_PRU1_GPO10        = 0x0158U,
    PIN_PRG1_PRU1_GPO11        = 0x015CU,
    PIN_PRG1_PRU1_GPO12        = 0x0160U,
    PIN_PRG1_PRU1_GPO13        = 0x0164U,
    PIN_PRG1_PRU1_GPO14        = 0x0168U,
    PIN_PRG1_PRU1_GPO15        = 0x016CU,
    PIN_PRG1_PRU1_GPO16        = 0x0170U,
    PIN_PRG1_PRU1_GPO17        = 0x0174U,
    PIN_PRG1_PRU1_GPO18        = 0x0178U,
    PIN_PRG1_PRU1_GPO19        = 0x017CU,
    PIN_PRG1_MDIO0_MDIO        = 0x0180U,
    PIN_PRG1_MDIO0_MDC         = 0x0184U,
    PIN_MMC0_DAT7              = 0x0188U,
    PIN_MMC0_DAT6              = 0x018CU,
    PIN_MMC0_DAT5              = 0x0190U,
    PIN_MMC0_DAT4              = 0x0194U,
    PIN_MMC0_DAT3              = 0x0198U,
    PIN_MMC0_DAT2              = 0x019CU,
    PIN_MMC0_DAT1              = 0x01A0U,
    PIN_MMC0_DAT0              = 0x01A4U,
    PIN_MMC0_CLK               = 0x01A8U,
    PIN_MMC0_CMD               = 0x01ACU,
    PIN_MMC0_DS                = 0x01B0U,
    PIN_MMC0_SDCD              = 0x01B4U,
    PIN_MMC0_SDWP              = 0x01B8U,
    PIN_SPI0_CS0               = 0x01BCU,
    PIN_SPI0_CS1               = 0x01C0U,
    PIN_SPI0_CLK               = 0x01C4U,
    PIN_SPI0_D0                = 0x01C8U,
    PIN_SPI0_D1                = 0x01CCU,
    PIN_SPI1_CS0               = 0x01D0U,
    PIN_SPI1_CS1               = 0x01D4U,
    PIN_SPI1_CLK               = 0x01D8U,
    PIN_SPI1_D0                = 0x01DCU,
    PIN_SPI1_D1                = 0x01E0U,
    PIN_UART0_RXD              = 0x01E4U,
    PIN_UART0_TXD              = 0x01E8U,
    PIN_UART0_CTSN             = 0x01ECU,
    PIN_UART0_RTSN             = 0x01F0U,
    PIN_PRG0_PRU0_GPO0         = 0x01F4U,
    PIN_PRG0_PRU0_GPO1         = 0x01F8U,
    PIN_PRG0_PRU0_GPO2         = 0x01FCU,
    PIN_PRG0_PRU0_GPO3         = 0x0200U,
    PIN_PRG0_PRU0_GPO4         = 0x0204U,
    PIN_PRG0_PRU0_GPO5         = 0x0208U,
    PIN_PRG0_PRU0_GPO6         = 0x020CU,
    PIN_PRG0_PRU0_GPO7         = 0x0210U,
    PIN_PRG0_PRU0_GPO8         = 0x0214U,
    PIN_PRG0_PRU0_GPO9         = 0x0218U,
    PIN_PRG0_PRU0_GPO10        = 0x021CU,
    PIN_PRG0_PRU0_GPO11        = 0x0220U,
    PIN_PRG0_PRU0_GPO12        = 0x0224U,
    PIN_PRG0_PRU0_GPO13        = 0x0228U,
    PIN_PRG0_PRU0_GPO14        = 0x022CU,
    PIN_PRG0_PRU0_GPO15        = 0x0230U,
    PIN_PRG0_PRU0_GPO16        = 0x0234U,
    PIN_PRG0_PRU0_GPO17        = 0x0238U,
    PIN_PRG0_PRU0_GPO18        = 0x023CU,
    PIN_PRG0_PRU0_GPO19        = 0x0240U,
    PIN_PRG0_PRU1_GPO0         = 0x0244U,
    PIN_PRG0_PRU1_GPO1         = 0x0248U,
    PIN_PRG0_PRU1_GPO2         = 0x024CU,
    PIN_PRG0_PRU1_GPO3         = 0x0250U,
    PIN_PRG0_PRU1_GPO4         = 0x0254U,
    PIN_PRG0_PRU1_GPO5         = 0x0258U,
    PIN_PRG0_PRU1_GPO6         = 0x025CU,
    PIN_PRG0_PRU1_GPO7         = 0x0260U,
    PIN_PRG0_PRU1_GPO8         = 0x0264U,
    PIN_PRG0_PRU1_GPO9         = 0x0268U,
    PIN_PRG0_PRU1_GPO10        = 0x026CU,
    PIN_PRG0_PRU1_GPO11        = 0x0270U,
    PIN_PRG0_PRU1_GPO12        = 0x0274U,
    PIN_PRG0_PRU1_GPO13        = 0x0278U,
    PIN_PRG0_PRU1_GPO14        = 0x027CU,
    PIN_PRG0_PRU1_GPO15        = 0x0280U,
    PIN_PRG0_PRU1_GPO16        = 0x0284U,
    PIN_PRG0_PRU1_GPO17        = 0x0288U,
    PIN_PRG0_PRU1_GPO18        = 0x028CU,
    PIN_PRG0_PRU1_GPO19        = 0x0290U,
    PIN_PRG0_MDIO0_MDIO        = 0x0294U,
    PIN_PRG0_MDIO0_MDC         = 0x0298U,
    PIN_NMIN                   = 0x029CU,
    PIN_RESETZ                 = 0x02A0U,
    PIN_RESETSTATZ             = 0x02A4U,
    PIN_PORZ_OUT               = 0x02A8U,
    PIN_SOC_SAFETY_ERRORN      = 0x02ACU,
    PIN_TDI                    = 0x02B0U,
    PIN_TDO                    = 0x02B4U,
    PIN_TMS                    = 0x02B8U,
    PIN_USB0_DRVVBUS           = 0x02BCU,
    PIN_USB1_DRVVBUS           = 0x02C0U,
    PIN_MMC1_DAT3              = 0x02C4U,
    PIN_MMC1_DAT2              = 0x02C8U,
    PIN_MMC1_DAT1              = 0x02CCU,
    PIN_MMC1_DAT0              = 0x02D0U,
    PIN_MMC1_CLK               = 0x02D4U,
    PIN_MMC1_CMD               = 0x02D8U,
    PIN_MMC1_SDCD              = 0x02DCU,
    PIN_MMC1_SDWP              = 0x02E0U,
    PIN_I2C0_SCL               = 0x02E8U,
    PIN_I2C0_SDA               = 0x02ECU,
    PIN_I2C1_SCL               = 0x02F0U,
    PIN_I2C1_SDA               = 0x02F4U,
    PIN_ECAP0_IN_APWM_OUT      = 0x02F8U,
    PIN_EXT_REFCLK1            = 0x02FCU,
    PIN_TIMER_IO0              = 0x0300U,
    PIN_TIMER_IO1              = 0x0304U,
    PIN_PORZ                   = 0x0308U,
};

enum pinWkupOffsets
{
    PIN_MCU_OSPI0_CLK          = 0x0000U,
    PIN_MCU_OSPI0_LBCLKO       = 0x0004U,
    PIN_MCU_OSPI0_DQS          = 0x0008U,
    PIN_MCU_OSPI0_D0           = 0x000CU,
    PIN_MCU_OSPI0_D1           = 0x0010U,
    PIN_MCU_OSPI0_D2           = 0x0014U,
    PIN_MCU_OSPI0_D3           = 0x0018U,
    PIN_MCU_OSPI0_D4           = 0x001CU,
    PIN_MCU_OSPI0_D5           = 0x0020U,
    PIN_MCU_OSPI0_D6           = 0x0024U,
    PIN_MCU_OSPI0_D7           = 0x0028U,
    PIN_MCU_OSPI0_CSN0         = 0x002CU,
    PIN_MCU_OSPI0_CSN1         = 0x0030U,
    PIN_MCU_OSPI1_CLK          = 0x0034U,
    PIN_MCU_OSPI1_LBCLKO       = 0x0038U,
    PIN_MCU_OSPI1_DQS          = 0x003CU,
    PIN_MCU_OSPI1_D0           = 0x0040U,
    PIN_MCU_OSPI1_D1           = 0x0044U,
    PIN_MCU_OSPI1_D2           = 0x0048U,
    PIN_MCU_OSPI1_D3           = 0x004CU,
    PIN_MCU_OSPI1_CSN0         = 0x0050U,
    PIN_MCU_OSPI1_CSN1         = 0x0054U,
    PIN_MCU_RGMII1_TX_CTL      = 0x0058U,
    PIN_MCU_RGMII1_RX_CTL      = 0x005CU,
    PIN_MCU_RGMII1_TD3         = 0x0060U,
    PIN_MCU_RGMII1_TD2         = 0x0064U,
    PIN_MCU_RGMII1_TD1         = 0x0068U,
    PIN_MCU_RGMII1_TD0         = 0x006CU,
    PIN_MCU_RGMII1_TXC         = 0x0070U,
    PIN_MCU_RGMII1_RXC         = 0x0074U,
    PIN_MCU_RGMII1_RD3         = 0x0078U,
    PIN_MCU_RGMII1_RD2         = 0x007CU,
    PIN_MCU_RGMII1_RD1         = 0x0080U,
    PIN_MCU_RGMII1_RD0         = 0x0084U,
    PIN_MCU_MDIO0_MDIO         = 0x0088U,
    PIN_MCU_MDIO0_MDC          = 0x008CU,
    PIN_MCU_SPI0_CLK           = 0x0090U,
    PIN_MCU_SPI0_D0            = 0x0094U,
    PIN_MCU_SPI0_D1            = 0x0098U,
    PIN_MCU_SPI0_CS0           = 0x009CU,
    PIN_WKUP_UART0_RXD         = 0x00A0U,
    PIN_WKUP_UART0_TXD         = 0x00A4U,
    PIN_MCU_MCAN0_TX           = 0x00A8U,
    PIN_MCU_MCAN0_RX           = 0x00ACU,
    PIN_WKUP_GPIO0_0           = 0x00B0U,
    PIN_WKUP_GPIO0_1           = 0x00B4U,
    PIN_WKUP_GPIO0_2           = 0x00B8U,
    PIN_WKUP_GPIO0_3           = 0x00BCU,
    PIN_WKUP_GPIO0_4           = 0x00C0U,
    PIN_WKUP_GPIO0_5           = 0x00C4U,
    PIN_WKUP_GPIO0_6           = 0x00C8U,
    PIN_WKUP_GPIO0_7           = 0x00CCU,
    PIN_WKUP_GPIO0_8           = 0x00D0U,
    PIN_WKUP_GPIO0_9           = 0x00D4U,
    PIN_WKUP_GPIO0_10          = 0x00D8U,
    PIN_WKUP_GPIO0_11          = 0x00DCU,
    PIN_WKUP_I2C0_SCL          = 0x00E0U,
    PIN_WKUP_I2C0_SDA          = 0x00E4U,
    PIN_MCU_I2C0_SCL           = 0x00E8U,
    PIN_MCU_I2C0_SDA           = 0x00ECU,
    PIN_PMIC_POWER_EN1         = 0x00F0U,
    PIN_MCU_SAFETY_ERRORN      = 0x00F4U,
    PIN_MCU_RESETZ             = 0x00F8U,
    PIN_MCU_RESETSTATZ         = 0x00FCU,
    PIN_MCU_PORZ_OUT           = 0x0100U,
    PIN_TCK                    = 0x0104U,
    PIN_TRSTN                  = 0x0108U,
    PIN_EMU0                   = 0x010CU,
    PIN_EMU1                   = 0x0110U,
    PIN_PMIC_POWER_EN0         = 0x0114U,
};


/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/** \brief Pinmux configuration data for the board. Auto-generated from 
           Pinmux tool. */
extern pinmuxBoardCfg_t gAM65xxMainPinmuxData[];
extern pinmuxBoardCfg_t gAM65xxWkupPinmuxData[];

extern pinmuxPerCfg_t gSystem0PinCfgPG1[];
extern pinmuxModuleCfg_t gSystemPinCfg[];
extern pinmuxPerCfg_t gWkup_system0PinCfgPG1[];
extern pinmuxModuleCfg_t gWkup_systemPinCfg[];

extern uint32_t Board_isSocPG1(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* _AM65XX_PIN_MUX_H_ */
