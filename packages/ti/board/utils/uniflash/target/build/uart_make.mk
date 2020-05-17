# Copyright (C) 2019 Texas Instruments Incorporated - http://www.ti.com/
#
#
#  Redistribution and use in source and binary forms, with or without
#  modification, are permitted provided that the following conditions
#  are met:
#
#    Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
#
#    Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the
#    distribution.
#
#    Neither the name of Texas Instruments Incorporated nor the names of
#    its contributors may be used to endorse or promote products derived
#    from this software without specific prior written permission.
#
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
#  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
#  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
#  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
#  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
#  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
#  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
#  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
#  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
#  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
#  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#
# Macro definitions referenced below
#

#
# This file is the makefile for building CSL uart app.
#
include $(PDK_INSTALL_PATH)/ti/build/Rules.make

# Board diag parameters which can be controlled outside this makefile
UNIFLASH ?= uart
PROFILE ?= release
TESTMODE ?= FUNCTIONAL_TEST
MODENAME ?=
BOARD_UTILS_CFLAGS ?=

HS_SUFFIX=
ifeq ($(BUILD_HS), yes)
HS_SUFFIX=_hs
endif

APP_NAME ?= board_utils_$(UNIFLASH)_flash_programmer$(HS_SUFFIX)

BUILD_PROFILE_$(CORE) = $(PROFILE)

BUILD_OS_TYPE = baremetal
LNKFLAGS_LOCAL_mpu1_0  += --entry Entry
EXTERNAL_INTERFACES =
XDC_CFG_FILE_mpu1_0 =
XDC_CFG_FILE_mcu1_0 =

# Name of the binary if different from the default (APP_NAME)_$(BOARD_$(CORE)_<build_profile>
LOCAL_APP_NAME =  $(UNIFLASH)_$(BOARD)_flash_programmer

SRCDIR = ../src ./$(BOARD)
INCDIR = ../../../../../board ../src ../include ../../../../src/$(BOARD)/include ../../../../src/$(BOARD) ../../../../src/flash ../../../../../csl

ifeq ($(BOARD), $(filter $(BOARD), j721e_evm am65xx_evm am65xx_idk))
  SRCDIR += ../soc/k3 ../src/ospi ../src/qspi ../src/hyperflash ../src/emmc ../../../../diag/common/$(SOC)
  INCDIR += ../soc/k3 ../src/ospi ../src/qspi ../src/hyperflash ../src/emmc ../../../../diag/common/$(SOC)
  ifeq ($(BOARD), $(filter $(BOARD), am65xx_evm am65xx_idk))
    SRCDIR += ../../../../diag/common/am65xx
    INCDIR += ../../../../diag/common/am65xx
  endif
endif

# List all the external components/interfaces, whose interface header files
# need to be included for this component
INCLUDE_EXTERNAL_INTERFACES = pdk

# List all the components required by the application
COMP_LIST_COMMON = board csl csl_init osal_nonos uart i2c
ifeq ($(BOARD), $(filter $(BOARD), j721e_evm am65xx_evm am65xx_idk))
COMP_LIST_COMMON += spi_dma udma gpio mmcsd
endif
COMP_LIST_COMMON += sciclient$(HS_SUFFIX)

# Common source files and CFLAGS across all platforms and cores
PACKAGE_SRCS_COMMON = ../../board ../../build ../../include ../../src ../../soc/soc.h
PACKAGE_SRCS_COMMON += ../../../host
PACKAGE_SRCS_COMMON += ../../../../board_utils_component.mk
ifeq ($(BOARD), $(filter $(BOARD), j721e_evm am65xx_evm am65xx_idk))
PACKAGE_SRCS_COMMON += ../../soc/k3
endif


SRCS_COMMON = uart_main.c xmodem.c soc.c

ifeq ($(BOARD), $(filter $(BOARD), j721e_evm))
SRCS_COMMON += ospi.c emmc.c qspi.c hyperflash.c
EXTERNAL_LNKCMD_FILE_LOCAL = ../soc/k3/linker_j7.cmd
endif
ifeq ($(BOARD), $(filter $(BOARD), am65xx_evm am65xx_idk))
SRCS_COMMON += ospi.c emmc.c
EXTERNAL_LNKCMD_FILE_LOCAL = ../soc/k3/linker.cmd
endif

ifeq ($(NOJTAG), yes)
BOARD_UTILS_CFLAGS += UFP_DISABLE_JTAG_LOAD
endif

CFLAGS_LOCAL_COMMON = $(PDK_CFLAGS) $(BOARD_UTILS_CFLAGS) -DSOC_$(SOC) -D$(BOARD)
ifeq ($(BOARD), $(filter $(BOARD), am65xx_evm am65xx_idk))
CFLAGS_LOCAL_COMMON += -DSOC_AM65XX
endif

ifeq ($(BUILD_HS), yes)
CFLAGS_LOCAL_COMMON += -DBUILD_HS
else
CFLAGS_LOCAL_COMMON += -DSPI_DMA_ENABLE
endif

SBL_OBJ_COPY := $(TOOLCHAIN_PATH_GCC_ARCH64)/bin/$(GCC_ARCH64_BIN_PREFIX)-objcopy
export SBL_OBJ_COPY

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
