# Makefile for Watchdog test app for TI RTOS
include $(PDK_INSTALL_PATH)/ti/build/Rules.make

BUILD_OS_TYPE = tirtos
EXTERNAL_INTERFACES = bios xdc
COMP_LIST_COMMON    = $(PDK_COMMON_TIRTOS_COMP)

ifeq ($(SOC),$(filter $(SOC), tpr12))
# Enable XDC build for application by providing XDC CFG File per core
XDC_CFG_FILE_$(CORE) = $(PDK_INSTALL_PATH)/ti/build/$(SOC)/sysbios_$(ISA).cfg
endif

SRCDIR = .
INCDIR =

# Common source files across all platforms and cores
SRCS_mcu1_0 += main_mss.c
SRCS_c66xdsp_1 += main_dss.c

# List all the external components/interfaces, whose interface header files
# need to be included for this component
INCLUDE_EXTERNAL_INTERFACES = pdk $(EXTERNAL_INTERFACES)

APP_NAME = watchdog_testapp

# List all the specific components required by the application
COMP_LIST_COMMON += watchdog esm

PACKAGE_SRCS_COMMON = .
CFLAGS_LOCAL_COMMON = $(PDK_CFLAGS)

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
