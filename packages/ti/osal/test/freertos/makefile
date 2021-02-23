#
# This file is the makefile for building CSL timer app.
#
ifeq ($(RULES_MAKE), )
include $(PDK_INSTALL_PATH)/ti/build/Rules.make
else
include $(RULES_MAKE)
endif

#Name of the directory created under packages/ti/binary/
APP_NAME = OSAL_freertos_TestApp
# Name of the binary if different from the default (APP_NAME)_$(BOARD_$(CORE)_<build_profile>
LOCAL_APP_NAME =  OSAL_freertos_$(BOARD)_$(CORE)TestApp
BUILD_OS_TYPE = freertos

SRCDIR = ../src
INCDIR = ../src
INCDIR +=  ${FREERTOS_KERNEL_INSTALL_PATH}/FreeRTOS-Kernel/include
INCDIR += $(PDK_FREERTOS_COMP_PATH)/freertos/portable/TI_CGT/$(ISA)
INCDIR += $(PDK_FREERTOS_COMP_PATH)/freertos/config/$(SOC)/$(ISA)
INCDIR += $(PDK_FREERTOS_COMP_PATH)/freertos/dpl/common

# List all the external components/interfaces, whose interface header files
#  need to be included for this component
INCLUDE_EXTERNAL_INTERFACES = pdk

# List all the components required by the application
COMP_LIST_COMMON = $(PDK_COMMON_FREERTOS_COMP)

# Common source files and CFLAGS across all platforms and cores
PACKAGE_SRCS_COMMON = . ../src
CFLAGS_LOCAL_COMMON = $(PDK_CFLAGS) -DFREERTOS

ifeq ($(MANUAL_CACHE_TEST),YES)
  CFLAGS_LOCAL_COMMON += -DMANUAL_CACHE_TEST
endif

ifeq ($(SOC),$(filter $(SOC), am65xx j721e j7200 am64x tpr12 awr294x))
  CFLAGS_LOCAL_COMMON += -DENABLE_TIMER_TEST
endif

SRCS_COMMON += main_osal_test.c

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
