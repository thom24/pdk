#
# This file is the makefile for building SafeRTOS Task Switch Example App
#
ifeq ($(RULES_MAKE), )
include $(PDK_INSTALL_PATH)/ti/build/Rules.make
else
include $(RULES_MAKE)
endif


APP_NAME = safertos_test_task_switch
BUILD_OS_TYPE=safertos
CFLAGS_TEST_DEFINES =


SRCDIR = .
INCDIR += .

# List all the external components/interfaces, whose interface header files
#  need to be included for this component
INCLUDE_EXTERNAL_INTERFACES = pdk
INCLUDE_EXTERNAL_INTERFACES += safertos
ifeq ($(BUILD_OS_TYPE), safertos)
COMP_LIST_COMMON = $(PDK_COMMON_BAREMETAL_COMP)
COMP_LIST_COMMON += safertos
else
# List all the components required by the application
endif

ifeq ($(ISA),$(filter $(ISA), r5f))
  EXTERNAL_LNKCMD_FILE_LOCAL = linker_core_r5.lds
endif

# Common source files and CFLAGS across all platforms and cores
PACKAGE_SRCS_COMMON = .
SRCS_COMMON += main.c task_switch.c SafeRTOSConfig.c aborts.c

SRCS_ASM_COMMON += utilsasm.asm

CFLAGS_LOCAL_COMMON = $(PDK_CFLAGS) $(CFLAGS_TEST_DEFINES)

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
