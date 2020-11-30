# Makefile for CANFD test app
include $(PDK_INSTALL_PATH)/ti/build/Rules.make

ifeq ($(IS_BAREMETAL),yes)
BUILD_OS_TYPE = baremetal
CFLAGS_OS_DEFINES =
LNKFLAGS_LOCAL_mpu1_0  += --entry Entry
COMP_LIST_COMMON = $(PDK_COMMON_BAREMETAL_COMP)
EXTERNAL_INTERFACES =
XDC_CFG_FILE_$(CORE) =
else
BUILD_OS_TYPE = tirtos
CFLAGS_OS_DEFINES = -DUSE_BIOS
EXTERNAL_INTERFACES = bios xdc
COMP_LIST_COMMON = $(PDK_COMMON_TIRTOS_COMP)

ifeq ($(SOC),$(filter $(SOC), tpr12))
ifeq ($(CORE),$(filter $(CORE), mcu1_0))
  XDC_CFG_FILE_$(CORE) = ./tpr12/mss.cfg
endif
endif

endif

SRCDIR = . src
INCDIR = . src

ifeq ($(SOC),$(filter $(SOC), tpr12))
SRCDIR += $(SOC)
INCDIR += $(SOC)
endif

ifeq ($(BUILD_OS_TYPE), tirtos)
CANFD_OS_TESTPREFIX=
else
CANFD_OS_TESTPREFIX=_Baremetal
endif


# Common source files across all platforms and cores
SRCS_COMMON += main_canfd_test.c


# List all the external components/interfaces, whose interface header files
# need to be included for this component
INCLUDE_EXTERNAL_INTERFACES = pdk $(EXTERNAL_INTERFACES)

#Name of the directory created under packages/ti/binary/
APP_NAME = canfd$(CANFD_OS_TESTPREFIX)_test

# Name of the binary if different fmake clrom the default (APP_NAME)_$(BOARD_$(CORE)_<build_profile>
LOCAL_APP_NAME =  canfd$(CANFD_OS_TESTPREFIX)_$(BOARD)_$(CORE)_test


COMP_LIST_COMMON += canfd esm

# Master running on mcu1_0 core, use the common linker command file in build
ifeq ($(SOC),$(filter $(SOC), tpr12))
    ifeq ($(CORE),$(filter $(CORE), mcu1_0))
        EXTERNAL_LNKCMD_FILE_LOCAL =  ./tpr12/mss_canfd_linker.cmd
    endif
endif

# tpr12 build

PACKAGE_SRCS_COMMON = ../test

CFLAGS_LOCAL_COMMON = $(PDK_CFLAGS) $(CFLAGS_OS_DEFINES)

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
