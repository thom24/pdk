#
# This file is the makefile for building Keywriter image
# that is loaded by R5 ROM.
#
include $(PDK_INSTALL_PATH)/ti/build/Rules.make

APP_NAME = keywriter_img
LOCAL_APP_NAME = keywriter_img_$(SOC)
BUILD_OS_TYPE = baremetal

KEYWRITER_APP_DIR = $(PDK_SBL_COMP_PATH)/example/k3MulticoreApp/keywriter
SRCDIR      += $(KEYWRITER_APP_DIR)

INCDIR    += $(PDK_INSTALL_PATH)/ti/board/src/$(BOARD)/include
INCDIR    += $(PDK_INSTALL_PATH)/ti/csl

PACKAGE_SRCS_COMMON = .

# List all the external components/interfaces, whose interface header files
#  need to be included for this component
INCLUDE_EXTERNAL_INTERFACES = pdk

# List all the components required by the application
COMP_LIST_COMMON = board uart osal_nonos csl csl_init sciclient_direct i2c gpio rm_pm_hal

# Adjust KEYWRITER_LOG_LEVEL to switch on/off the R5 side logs
# No logs      0
# Maximum logs 1
KEYWRITER_CFLAGS = -DKEYWRITER_LOG_LEVEL=1

# Comment the below line to turn of M3 logs
KEYWRITER_CFLAGS += -DKEYWRITER_ENABLE_M3_LOGS

CFLAGS_LOCAL_COMMON = $(PDK_CFLAGS) $(KEYWRITER_CFLAGS)

SRCS_COMMON += main.c
SRCS_ASM_COMMON += init.asm

EXTERNAL_LNKCMD_FILE_LOCAL = $(PDK_SBL_COMP_PATH)/example/k3MulticoreApp/keywriter/linker.lds

R5_STARTUP_MODE := SPLIT_MODE
export R5_STARTUP_MODE

SYSFW_KEYWRITER_SUFFIX := $(SOC)
SCI_CLIENT_DIR	:= $(PDK_INSTALL_PATH)/ti/drv/sciclient
SCI_CLIENT_TOOLS_BIN2C_DIR	:= $(SCI_CLIENT_DIR)/tools/bin2c

ifeq ($(OS),Windows_NT)
  BIN2C_EXE := $(SCI_CLIENT_TOOLS_BIN2C_DIR)/bin2c.exe
else
  BIN2C_EXE := $(SCI_CLIENT_TOOLS_BIN2C_DIR)/bin2c.out
endif

ifeq ($(SOC),$(filter $(SOC), j721e))
  SYSFW_SOC_DIR := V1
endif

SIGNED_SYSFW_BIN := $(KEYWRITER_APP_DIR)/tifs_bin/ti-fs-keywriter-$(SYSFW_KEYWRITER_SUFFIX).bin

# Include common make files
ifeq ($(MAKERULEDIR), )
#Makerule path not defined, define this and assume relative path from ROOTDIR
  MAKERULEDIR := $(ROOTDIR)/ti/build/makerules
  export MAKERULEDIR
endif
include $(MAKERULEDIR)/common.mk

main.c: ti-fs-keywriter.h

ti-fs-keywriter.h: $(BIN2C_EXE) $(SIGNED_SYSFW_BIN)
	# USAGE: bin2c <binary file name> <c file name> <array name>
	$(BIN2C_EXE) $(SIGNED_SYSFW_BIN) ti-fs-keywriter.h TIFS_KEYWRITER_BIN > $(KEYWRITER_APP_DIR)/ti-fs-keywriter.h

$(BIN2C_EXE):
	gcc $(SCI_CLIENT_TOOLS_BIN2C_DIR)/bin2c.c -o $(BIN2C_EXE)


# OBJs and libraries are built by using rule defined in rules_<target>.mk
#     and need not be explicitly specified here

# Nothing beyond this point

