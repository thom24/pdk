#
# This file is the makefile for building LPM library.
#
ifeq ($(RULES_MAKE), )
include $(PDK_INSTALL_PATH)/ti/build/Rules.make
else
include $(RULES_MAKE)
endif

MODULE_NAME = lpm

include $(PDK_INSTALL_PATH)/ti/drv/spi/spi_component.mk
PDK_SBL_COMP_PATH = $(PDK_INSTALL_PATH)/ti/boot/sbl

SRCDIR = src soc/$(SOC)
INCDIR = include soc/$(SOC)

INCDIR += $(PDK_SBL_COMP_PATH)
INCDIR += $(PDK_SBL_COMP_PATH)/soc
INCDIR += $(PDK_SBL_COMP_PATH)/board/src
INCDIR += $(PDK_SBL_COMP_PATH)/src/rprc
INCDIR += $(PDK_SBL_COMP_PATH)/src/ospi
INCDIR += $(PDK_SBL_COMP_PATH)/src/mmcsd
INCDIR += $(PDK_SBL_COMP_PATH)/soc/k3

INCDIR += $(PDK_INSTALL_PATH)
INCDIR += $(PDK_INSTALL_PATH)/ti/board
ifeq ($(SOC),$(filter $(SOC), j7200))
INCDIR += $(PDK_INSTALL_PATH)/ti/board/src/j7200_evm/include
else
INCDIR += $(PDK_INSTALL_PATH)/ti/board/src/j721e_evm/include
endif
INCDIR += $(PDK_INSTALL_PATH)/ti/board/src/flash/include
INCDIR += $(PDK_INSTALL_PATH)/ti/board/src/flash/nor

# List all the external components/interfaces, whose interface header files
# need to be included for this component
INCLUDE_EXTERNAL_INTERFACES = pdk
INCLUDE_INTERNAL_INTERFACES = board sbl_lib_cust spi pmic ipc
COMP_LIST_COMMON = 

PACKAGE_SRCS_COMMON = lpm.h makefile .gitignore $(SRCDIR)
PACKAGE_SRCS_COMMON += include/lpm_boot.h
PACKAGE_SRCS_COMMON += include/lpm_pmic.h
PACKAGE_SRCS_COMMON += include/lpm_ipc.h
PACKAGE_SRCS_COMMON += include/print_utils.h
PACKAGE_SRCS_COMMON += soc/$(SOC)/boot_core_defs.h
PACKAGE_SRCS_COMMON += config_mk.bld package.bld package.xdc package.xs
PACKAGE_SRCS_COMMON += Settings.xdc.xdt

# Common source files and CFLAGS across all platforms and cores
SRCS_COMMON += lpm_boot.c
SRCS_COMMON += lpm_pmic.c
SRCS_COMMON += lpm_ipc.c
SRCS_COMMON += print_utils.c
SRCS_COMMON += soc/$(SOC)/boot_core_defs.c

INCDIR += $(PDK_INSTALL_PATH)/ti/drv/pmic
INCDIR += $(PDK_INSTALL_PATH)/ti/drv/pmic/include
INCDIR += $(PDK_INSTALL_PATH)/ti/drv/pmic/test/common
INCDIR += $(PDK_INSTALL_PATH)/ti/drv/pmic/src
INCDIR += $(PDK_INSTALL_PATH)/ti/drv/pmic/src/cfg/tps6594x
INCDIR += $(PDK_INSTALL_PATH)/ti/drv/pmic/src/cfg/lp8764x
INCDIR += $(PDK_INSTALL_PATH)/ti/drv/pmic/include/cfg/tps6594x
INCDIR += $(PDK_INSTALL_PATH)/ti/drv/pmic/include/cfg/lp8764x
SRCDIR += $(PDK_INSTALL_PATH)/ti/drv/pmic/src/cfg/tps6594x
SRCDIR += $(PDK_INSTALL_PATH)/ti/drv/pmic/src/cfg/lp8764x
SRCDIR += $(PDK_INSTALL_PATH)/ti/drv/pmic/src
SRCDIR += $(PDK_INSTALL_PATH)/ti/drv/pmic/test/common
SRCDIR += $(PDK_INSTALL_PATH)/ti/build/unit-test/Unity/src
SRCDIR += $(PDK_INSTALL_PATH)/ti/build/unit-test/config
INCDIR += $(PDK_INSTALL_PATH)/ti/build/unit-test/Unity/src
INCDIR += $(PDK_INSTALL_PATH)/ti/build/unit-test/config
SRCS_COMMON += unity_config.c unity.c
SRCS_COMMON += pmic_ut_common.c pmic_fsm.c pmic_irq_status.c pmic_io.c pmic_core.c pmic_esm.c pmic_rtc.c pmic_gpio.c pmic_gpio_lp8764x.c pmic_gpio_tps6594x.c pmic_irq_lp8764x.c pmic_irq_tps6594x.c

CFLAGS_LOCAL_COMMON = $(PDK_CFLAGS) -DUNITY_INCLUDE_CONFIG_V2_H -DLINUX_OS -DA72_LINUX_OS -DA72_LINUX_OS_IPC_ATTACH -DSBL_LOG_LEVEL=3 -DBOOT_OSPI -DMPU1_HLOS_BOOT_ENABLED

# Core/SoC/platform specific source files and CFLAGS
# Example:
#   SRCS_<core/SoC/platform-name> =
#   CFLAGS_LOCAL_<core/SoC/platform-name> =

# Include common make files
ifeq ($(MAKERULEDIR), )
#Makerule path not defined, define this and assume relative path from ROOTDIR
  MAKERULEDIR := $(ROOTDIR)/ti/build/makerules
  export MAKERULEDIR
endif
include $(MAKERULEDIR)/common.mk

# OBJs and libraries are built by using rule defined in rules_<target>.mk
#     and need not be explicitly specified here

# Nothing beyond this point
