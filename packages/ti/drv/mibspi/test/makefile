# Makefile for SPI master/slave test app
include $(PDK_INSTALL_PATH)/ti/build/Rules.make

ifeq ($(IS_BAREMETAL),yes)
BUILD_OS_TYPE = baremetal
CFLAGS_OS_DEFINES =
LNKFLAGS_LOCAL_mpu1_0  += --entry Entry
COMP_LIST_COMMON =  csl_init osal_nonos
EXTERNAL_INTERFACES =
XDC_CFG_FILE_$(CORE) =
else
BUILD_OS_TYPE = tirtos
CFLAGS_OS_DEFINES = -DUSE_BIOS
EXTERNAL_INTERFACES = bios xdc
COMP_LIST_COMMON    = osal_tirtos

ifeq ($(SOC),$(filter $(SOC), tpr12))
XDC_CFG_FILE_$(CORE) = ./tpr12/mss.cfg
endif

endif

SRCDIR = . src
INCDIR = . src

ifeq ($(BUILD_OS_TYPE), tirtos)
MIBSPI_OS_TESTPREFIX=
else
MIBSPI_OS_TESTPREFIX=_Baremetal
endif


# Common source files across all platforms and cores
SRCS_COMMON += main_mibspi_test.c mibspi_test_common.c

# List all the external components/interfaces, whose interface header files
# need to be included for this component
INCLUDE_EXTERNAL_INTERFACES = pdk $(EXTERNAL_INTERFACES)

# DMA enabled build
ifeq ($(DMA), enable)

#Name of the directory created under packages/ti/binary/
APP_NAME = MIBSPI$(MIBSPI_OS_TESTPREFIX)_MasterSlave_Dma_TestApp
# Name of the binary if different from the default (APP_NAME)_$(BOARD_$(CORE)_<build_profile>
LOCAL_APP_NAME =  MIBSPI$(MIBSPI_OS_TESTPREFIX)_MasterSlave_Dma_$(BOARD)_$(CORE)TestApp

COMP_LIST_COMMON += mibspi_dma edma
CFLAGS_SPI_DMA    = -DMIBSPI_DMA_ENABLE
else

#Name of the directory created under packages/ti/binary/
APP_NAME = MIBSPI$(MIBSPI_OS_TESTPREFIX)_MasterSlave_TestApp

# Name of the binary if different fmake clrom the default (APP_NAME)_$(BOARD_$(CORE)_<build_profile>
LOCAL_APP_NAME =  MIBSPI$(MIBSPI_OS_TESTPREFIX)_MasterSlave_$(BOARD)_$(CORE)TestApp


COMP_LIST_COMMON += mibspi
CFLAGS_SPI_DMA    =

endif

# Master running on mcu1_0 core, use the common linker command file in build
#CFLAGS_SPI_MS     = -DMIBSPI_MASTER_TASK
ifeq ($(SOC),$(filter $(SOC), tpr12))
# MIBSPI uses local linker command file to avoid code/data memory conflict with master application
ifeq ($(IS_BAREMETAL),yes)
EXTERNAL_LNKCMD_FILE_LOCAL =  ./tpr12/mss_spi_linker.cmd
else
EXTERNAL_LNKCMD_FILE_LOCAL =  ./tpr12/mss_spi_linker.cmd
endif
endif


# List all the components required by the application
COMP_LIST_COMMON  += csl board
# tpr12 build
ifeq ($(BOARD),$(filter $(BOARD), tpr12_evm))
#CFLAGS_SPI_UT    = -DMIBSPI_MULT_CHANNEL
endif

PACKAGE_SRCS_COMMON = ../test

CFLAGS_LOCAL_COMMON = $(PDK_CFLAGS) $(CFLAGS_SPI_UT) $(CFLAGS_SPI_DMA) $(CFLAGS_SPI_MS) $(CFLAGS_OS_DEFINES)

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
