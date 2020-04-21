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
 * \brief  Board library configurations
 *
 * This file configures the instance numbers, address and gpio reset
 * details of different interfaces of the board.
 *
 */
#ifndef BOARD_CFG_H_
#define BOARD_CFG_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Board ID information */
#define BOARD_INFO_CPU_NAME     "am64x"
#define BOARD_INFO_BOARD_NAME   "am64x_evm"

/* Memory sections */
#define BOARD_DDR_START_ADDR                            (0x80000000U)
#define BOARD_DDR_SIZE                                  (2048 * 1024 * 1024UL)
#define BOARD_DDR_END_ADDR                              (0xFFFFFFFFU)

/* UART LLD instance number for MAIN UART0 port */
#define BOARD_UART0_INSTANCE                            (0U)
/* UART LLD instance number for MCU UART0 port */
#define BOARD_MCU_UART0_INSTANCE                        (7U)

/* UART LLD instance number for primary UART port */
#if defined (BUILD_MPU) || defined (BUILD_MCU)
/* default UART instance for A53 and R5 cores in the Main domain */ 
#define BOARD_UART_INSTANCE                             (BOARD_UART0_INSTANCE)
#else
/* default UART instance for M4 core in the MCU domain */ 
#define BOARD_UART_INSTANCE                             (BOARD_MCU_UART0_INSTANCE)
#endif

/* ICSSG UART instance number */
#define BOARD_ICSSG_UART_INSTANCE                       (4U)

/* I2C instance connected to EEPROM - WKUP I2C0 */
#define BOARD_I2C_EEPROM_INSTANCE                       (0U)

/* I2C instance for BOOT EEPROM */
#define BOARD_I2C_BOOT_EEPROM_INSTANCE                  (0U)

/* I2C address for Boot EEPROM */
#define BOARD_I2C_BOOT_EEPROM_ADDR                      (0x50U)
#define BOARD_I2C_BOOT_EEPROM_ADDR2                     (0x51U)

/* I2C address for Board Id EEPROM */
#define BOARD_I2C_EEPROM_ADDR                           (0x51U)

/* QSPI instance number */
#define BOARD_SPI_NOR_INSTANCE                          (1U)

/* I2C Instance connected to clock generator */
#define BOARD_CLOCK_GENERATOR_INSTANCE                  (0U)
/* I2C slave address of clock generator */
#define BOARD_I2C_CLOCK_GENERATOR1                      (0)   //Can be used later
#define BOARD_I2C_CLOCK_GENERATOR2_ADDR1                (0x76)
#define BOARD_I2C_CLOCK_GENERATOR2_ADDR2                (0x77)
#define BOARD_I2C_QSGMII_CLOCK_GENERATOR                (0x77)
#define BOARD_I2C_PERI_CLOCK_GENERATOR                  (0x6D)

/* OSPI instance connected to OSPI NOR flash */
#define BOARD_OSPI_NOR_INSTANCE                         (0U)

/* I2C instance connected to IO Expander */
#define BOARD_I2C_IOEXP_DEVICE1_INSTANCE                (0U)
#define BOARD_I2C_IOEXP_DEVICE2_INSTANCE                (0U)
#define BOARD_I2C_IOEXP_DEVICE3_INSTANCE                (3U)
#define BOARD_I2C_IOEXP_DEVICE4_INSTANCE                (0U)
#define BOARD_I2C_IOEXP_DEVICE5_INSTANCE                (1U)
#define BOARD_I2C_AUDIO_IOEXP_DEVICE_INSTANCE           (3U)
#define BOARD_I2C_VIDEO_IOEXP_DEVICE_INSTANCE           (0U)

/* I2C IO Expander Slave devices */
#define BOARD_I2C_IOEXP_DEVICE1_ADDR                    (0x20U)
#define BOARD_I2C_IOEXP_DEVICE2_ADDR                    (0x22U)
#define BOARD_I2C_IOEXP_DEVICE3_ADDR                    (0x20U)
#define BOARD_I2C_IOEXP_DEVICE4_ADDR                    (0x20U)
#define BOARD_I2C_IOEXP_DEVICE5_ADDR                    (0x20U)
#define BOARD_I2C_AUDIO_IOEXP_DEVICE_ADDR               (0x21U)
#define BOARD_I2C_VIDEO_IOEXP_DEVICE_ADDR               (0x21U)

#define BOARD_GPIO_IOEXP_SPI_RST_PORT_NUM               (0) //J7ES_TODO: need to update
#define BOARD_GPIO_IOEXP_SPI_RST_PIN_NUM                (0) //J7ES_TODO: need to update

/* OSPI instance number */
#define BOARD_OSPI_INSTANCE                             (0)

#define BOARD_GPIO_IOEXP_OSPI_RST_PORT_NUM              (0) //J7ES_TODO: need to update
#define BOARD_GPIO_IOEXP_OSPI_RST_PIN_NUM               (0) //J7ES_TODO: need to update

/* GPIO port & pin numbers for  MMC reset */
#define GPIO_MMC_SDCD_PORT_NUM                          (0) //J7ES_TODO: need to update
#define GPIO_MMC_SDCD_PIN_NUM                           (0) //J7ES_TODO: need to update

#define BOARD_GPIO_IOEXP_EMMC_RST_PORT_NUM              (0x00) //J7ES_TODO: need to update
#define BOARD_GPIO_IOEXP_EMMC_RST_PIN_NUM               (0x00) //J7ES_TODO: need to update

/* I2C instance for External RTC */
#define BOARD_I2C_EXT_RTC_INSTANCE                      (0U)

/* I2C address for External RTC */
#define BOARD_I2C_EXT_RTC_ADDR                          (0x6FU)

#define BOARD_I2C_TOUCH_INSTANCE                        (0) //J7ES_TODO: need to update

#define BOARD_I2C_TOUCH_SLAVE_ADDR                      (0) //J7ES_TODO: need to update

/* I2C instance Board Presence Circuit */
#define BOARD_PRES_WKUP_I2C_INSTANCE                    (0U) //J7ES_TODO: need to update
/* I2C address Board Presence Circuit */
#define BOARD_PRES_DETECT_SLAVE_ADDR                    (0) //J7ES_TODO: need to update

/* User LED Pin Details */
#define BOARD_I2C_USER_LED_INSTANCE                     (0U)

#define BOARD_USER_LED1                                 (6U)
#define BOARD_USER_LED2                                 (7U)

#define BOARD_ICSS_EMAC_PORT_START                      (0) //J7ES_TODO: need to update
#define BOARD_ICSS_EMAC_PORT_END                        (0) //J7ES_TODO: need to update
#define BOARD_ICSS_EMAC_PORT_MAX                        (2) //J7ES_TODO: need to update
#define BOARD_MCU_EMAC_PORT_MAX                         (0) //J7ES_TODO: need to update
#define BOARD_MCU_ETH_PORT                              (0) //J7ES_TODO: need to update


/* ICSS2 EMAC PHY register address */
#define BOARD_ICSS2_EMAC_PHY0_ADDR                      (0x0) //J7ES_TODO: need to update
#define BOARD_ICSS2_EMAC_PHY1_ADDR                      (0x3u) //J7ES_TODO: need to update


/* PRG2_RGMII_RESETn */
#define BOARD_GPIO_IOEXP_ICSS2_EMAC_RST_PORT_NUM        (0) //J7ES_TODO: need to update
#define BOARD_GPIO_IOEXP_ICSS2_EMAC_RST_PIN_NUM         (0) //J7ES_TODO: need to update

/* PRG2_RGMII_INTn */
#define BOARD_GPIO_ICSS2_EMAC_INT_PORT_NUM              (0) /* WKUP_GPIO0_24  */ //J7ES_TODO: need to update
#define BOARD_GPIO_ICSS2_EMAC_INT_PIN_NUM               (0) //J7ES_TODO: need to update

/* PRG2_ETH1_LED_LINK */
#define BOARD_GPIO_ICSS2_EMAC_PHY0_LED_LINK_PORT_NUM    (0) /* GPIO1_13 */ //J7ES_TODO: need to update
#define BOARD_GPIO_ICSS2_EMAC_PHY0_LED_LINK_PIN_NUM     (0) //J7ES_TODO: need to update

/* PRG2_ETH2_LED_LINK */
#define BOARD_GPIO_ICSS2_EMAC_PHY1_LED_LINK_PORT_NUM    (0) /* GPIO1_14 */ //J7ES_TODO: need to update
#define BOARD_GPIO_ICSS2_EMAC_PHY1_LED_LINK_PIN_NUM     (0) //J7ES_TODO: need to update

/* GPIO to drive PRG2 LED0 */
#define BOARD_GPIO_ICSS2_EMAC_LED0_PORT_NUM             (0) /* WKUP_GPIO0_13 */ //J7ES_TODO: need to update
#define BOARD_GPIO_ICSS2_EMAC_LED0_PIN_NUM              (0) //J7ES_TODO: need to update

/* GPIO to drive PRG2 LED1 */
#define BOARD_GPIO_ICSS2_EMAC_LED1_PORT_NUM             (0) /* WKUP_GPIO0_0 */ //J7ES_TODO: need to update
#define BOARD_GPIO_ICSS2_EMAC_LED1_PIN_NUM              (0) //J7ES_TODO: need to update

/* GPIO to drive PRG2 LED2 */
#define BOARD_GPIO_ICSS2_EMAC_LED2_PORT_NUM             (0) /* WKUP_GPIO0_1 */ //J7ES_TODO: need to update
#define BOARD_GPIO_ICSS2_EMAC_LED2_PIN_NUM              (0) //J7ES_TODO: need to update

/* GPIO to drive PRG2 LED3 */
#define BOARD_GPIO_ICSS2_EMAC_LED3_PORT_NUM             (0) /* WKUP_GPIO0_27 */ //J7ES_TODO: need to update
#define BOARD_GPIO_ICSS2_EMAC_LED3_PIN_NUM              (0) //J7ES_TODO: need to update

/* MCU EMAC PHY MDIO address */
#define BOARD_MCU_EMAC_PHY_ADDR                         (0U) //J7ES_TODO: need to update

/* MCU EMAC MAX REG DUMP */
#define BOARD_MCU_EMAC_REG_DUMP_MAX                     (0) //J7ES_TODO: need to update

/* MCU EMAC PHY register address definitions for reading strap values */
#define BOARD_MCU_EMAC_STRAP_STS1_ADDR                  (0) //J7ES_TODO: need to update
#define BOARD_MCU_EMAC_STRAP_STS2_ADDR                  (0) //J7ES_TODO: need to update

/* MCU EMAC PHY register address definitions for reading strap values */
#define BOARD_ICSS_EMAC_STRAP_STS1_ADDR                 (0) //J7ES_TODO: need to update
#define BOARD_ICSS_EMAC_STRAP_STS2_ADDR                 (0) //J7ES_TODO: need to update

/* MCU_ETH1_RESETn */
#define BOARD_GPIO_IOEXP_MCU_EMAC_RST_PORT_NUM          (0) //J7ES_TODO: need to update
#define BOARD_GPIO_IOEXP_MCU_EMAC_RST_PIN_NUM           (0) //J7ES_TODO: need to update

/* MCU_ETH1_INTn */
#define BOARD_GPIO_MCU_EMAC_INT_PORT_NUM                (0) /* WKUP_GPIO0_32 */ //J7ES_TODO: need to update
#define BOARD_GPIO_MCU_EMAC_INT_PIN_NUM                 (0) //J7ES_TODO: need to update

/* AUTOMATION HEADER */
#define BOARD_TEST_HEADER_I2C_INSTANCE                  (2U)
#define BOARD_I2C_BOOT_MODE_SW_ADDR                     (0x22U)

/* TEST_GPIO1 */
#define BOARD_GPIO_IOEXP_TEST_PORT_NUM                  (0) //J7ES_TODO: need to update
#define BOARD_GPIO_IOEXP_TEST_PIN_NUM                   (0) //J7ES_TODO: need to update

/* Temperature sensor i2c instance */
#define BOARD_TEMP_SENSOR_I2C_INSTANCE                  (2U)

/* Temperature sensor slave device addresses */
#define BOARD_TEMP_SENSOR_I2C_SLAVE_DEVICE1_ADDR        (0x48U)
#define BOARD_TEMP_SENSOR_I2C_SLAVE_DEVICE2_ADDR        (0x49U)

#define BOARD_I2C_CURRENT_MONITOR_INSTANCE              (2U)

/* Instance for interfaces connected to MMCSD */
#define BOARD_MMCSD_SD_INSTANCE                         (1U)
#define BOARD_MMCSD_EMMC_INSTANCE                       (0)

/* Enable NOR flash driver */
#define BOARD_NOR_FLASH_IN //J7ES_TODO: need to update

/* McSPI instance for master and slave test */
#define BOARD_MCSPI_MASTER_INSTANCE                     (1) //J7ES_TODO: need to update
#define BOARD_MCSPI_SLAVE_INSTANCE                      (1) //J7ES_TODO: need to update

/* Maximum possible buffer length */
#define BOARD_EEPROM_MAX_BUFF_LENGTH                    (0) //J7ES_TODO: need to update

/* EEPROM board ID information */
#define BOARD_EEPROM_HEADER_FIELD_SIZE                  (0) //J7ES_TODO: need to update
#define BOARD_EEPROM_TYPE_SIZE                          (0) //J7ES_TODO: need to update
#define BOARD_EEPROM_STRUCT_LENGTH_SIZE                 (0) //J7ES_TODO: need to update
#define BOARD_EEPROM_MAGIC_NUMBER                       (0) //J7ES_TODO: need to update

#define BOARD_BOARD_FIELD_TYPE                          (0) //J7ES_TODO: need to update
#define BOARD_DDR_FIELD_TYPE                            (0) //J7ES_TODO: need to update
#define BOARD_MACINFO_FIELD_TYPE                        (0) //J7ES_TODO: need to update
#define BOARD_ENDLIST                                   (0) //J7ES_TODO: need to update

#define BOARD_EEPROM_HEADER_ADDR                        (0U) //J7ES_TODO: need to update

/* PinMux data to be programmed to configure a pin to be a GPIO */
#define PINMUX_GPIO_CFG                                 (0x00050007U)

typedef enum
{
	APP_CARD_DETECT = 0,
	LCD_BRD_DETECT,
	SERDES_BRD_DETECT,
	HDMI_BRD_DETECT
}boardPresDetect_t;

typedef enum domainType
{
    MAIN_DOMAIN = 0U,
    WKUP_DOMAIN
}domainType_t;


#define BOARD_EEPROM_HEADER_LENGTH                      (4U)
#define BOARD_EEPROM_BOARD_NAME_LENGTH                  (8U)
#define BOARD_EEPROM_VERSION_LENGTH                     (4U)
#define BOARD_EEPROM_SERIAL_NO_LENGTH                   (12U)
#define BOARD_EEPROM_CONFIG_LENGTH                      (32U)

#define BOARD_EEPROM_BOARD_NAME_ADDR                    (BOARD_EEPROM_HEADER_ADDR + BOARD_EEPROM_HEADER_LENGTH)
#define BOARD_EEPROM_VERSION_ADDR                       (BOARD_EEPROM_BOARD_NAME_ADDR + BOARD_EEPROM_BOARD_NAME_LENGTH)
#define BOARD_EEPROM_SERIAL_NO_ADDR                     (BOARD_EEPROM_VERSION_ADDR + BOARD_EEPROM_VERSION_LENGTH)
#define BOARD_EEPROM_CONFIG_ADDR                        (BOARD_EEPROM_SERIAL_NO_ADDR + BOARD_EEPROM_SERIAL_NO_LENGTH)

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif   /* BOARD_CFG_H_ */

