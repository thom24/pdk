#
# SDR
#
# Software Diagnostics Reference Library Makefile
#
# Copyright (C) 2019-2020 Texas Instruments Incorporated - http://www.ti.com/
# ALL RIGHTS RESERVED
#

# PDK SDL Library
PDK_SDL_DIR = $(PDK_INSTALL_PATH)/ti/diag/sdr

ifeq ($(RULES_MAKE), )
include $(PDK_INSTALL_PATH)/ti/build/Rules.make
else
include $(RULES_MAKE)
endif

MODULE_NAME = sdr

BUILD_PROFILE ?= release
export BUILD_PROFILE_$(CORE) ?= $(BUILD_PROFILE)

# List all the external components/interfaces, whose interface header files
# need to be included for this component
INCLUDE_EXTERNAL_INTERFACES =

SRCS_COMMON += sdl_esm.c sdl_ecc.c
SRCS_COMMON += sdl_exception.c sdl_mpuCfg.c
SRCS_COMMON += sdl_ecc_r5.c
SRCS_COMMON += sdl_ccm.c sdl_mpu.c
SRCS_COMMON += sdl_esm_core.c
SRCS_COMMON += sdl_crc.c
SRCS_COMMON += sdl_wdt.c
SRCS_COMMON += sdl_wdt_r5.c
SRCS_ASM_COMMON += sdl_ecc_utils.asm
SRCS_ASM_COMMON += sdl_utils.asm

#PACKAGE_SRCS_COMMON = sdl_common.h

# Set specific source directories
SRCDIR += src
ifeq ($(SOC),$(filter $(SOC), am65xx j721e))
SRCDIR += src/$(SOC)
endif

ifeq ($(CORE),$(filter $(CORE), mcu1_0))
SRCDIR += src/am65xx/r5
endif

# Set specific include directories
INCDIR += $(PDK_SDL_DIR)
INCDIR += $(PDK_SDL_DIR)/src

ifeq ($(SOC),$(filter $(SOC), am65xx j721e))
INCDIR += $(PDK_SDL_DIR)/src/$(SOC)
endif

ifeq ($(CORE),$(filter $(CORE), mcu1_0))
ifeq ($(SOC),$(filter $(SOC), am65xx j721e))
INCDIR += $(PDK_SDL_DIR)/src/$(SOC)/r5
endif
endif

ifdef PDK_CSL2_ROOT_PATH
INCDIR += $(PDK_CSL2_ROOT_PATH)/
else
INCDIR += $(PDK_INSTALL_PATH)/
endif

ifeq ($(CORE),$(filter $(CORE), mcu1_0))
ENDIAN = little
FORMAT = ELF
CGT_PATH = $(TOOLCHAIN_PATH_R5)
CGT_ISA = R5
CGT_EXT = r5f
endif

CFLAGS_LOCAL_COMMON +=

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
