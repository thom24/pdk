#
# This file is the makefile for building cbuff.
#
include $(PDK_INSTALL_PATH)/ti/build/Rules.make

MODULE_NAME = cbuff

SRCDIR = src soc/$(SOC)
INCDIR = . src soc/$(SOC)

# List all the external components/interfaces, whose interface header files
# need to be included for this component
INCLUDE_EXTERNAL_INTERFACES = pdk

# Common source files and CFLAGS across all platforms and cores
SRCS_COMMON += cbuff.c cbuff_lvds.c cbuff_edma.c cbuff_transfer.c
ifeq ($(SOC),$(filter $(SOC), awr294x))
  SRCS_COMMON += cbuff_soc.c
endif
CFLAGS_LOCAL_COMMON = $(PDK_CFLAGS) $(CBUFF_CFLAGS)

PACKAGE_SRCS_COMMON = cbuff.h makefile cbuff_component.mk src soc/cbuff_soc.h soc/$(SOC)

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
