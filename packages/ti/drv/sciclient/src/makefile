#
# This file is the makefile for building sciclient .
#
ifeq ($(RULES_MAKE), )
include $(PDK_INSTALL_PATH)/ti/build/Rules.make
else
include $(RULES_MAKE)
endif

MODULE_NAME = sciclient

SRCDIR = src
INCDIR = soc/sysfw/include
PACKAGE_SRCS_COMMON =

# SoC Specific source files
ifeq ($(SOC),$(filter $(SOC), am65xx))
  SCICLIENT_SOCVER = V0
endif
ifeq ($(SOC),$(filter $(SOC), j721e j7200))
  SCICLIENT_SOCVER = V1
endif

SRCDIR += soc/$(SCICLIENT_SOCVER)
INCDIR += soc/$(SCICLIENT_SOCVER)
ifeq ($(BUILD_HS),yes)
GEN_FILE = soc/$(SCICLIENT_SOCVER)/sciclient_defaultBoardcfg_hexhs.h
else
GEN_FILE = soc/$(SCICLIENT_SOCVER)/sciclient_defaultBoardcfg_hex.h
endif

# List all the external components/interfaces, whose interface header files
# need to be included for this component
INCLUDE_EXTERNAL_INTERFACES = pdk osal

# Common source files and CFLAGS across all platforms and cores
ifeq ($(SOC),$(filter $(SOC), am65xx j721e j7200))
# Keep the sciclient.c the first file in this list. This is used to trigger
# the generation of the header files from the firware binary file.
SRCS_COMMON  = sciclient.c
SRCS_COMMON += sciclient_fmwSecureProxyMap.c
SRCS_COMMON += sciclient_pm.c
SRCS_COMMON += sciclient_rm.c 
SRCS_COMMON += sciclient_boardcfg.c
SRCS_COMMON += sciclient_procboot.c
SRCS_COMMON += sciclient_genericMsgs.c
endif

#Use CSL implementation to bypass DMSC
# ifeq ($(SOC),$(filter $(SOC), j721e j7200))
# SRCS_COMMON = sciclient_rm_csl.c sciclient_dummy.c
# endif

#Bypass DMSC for host emulation and Loki build
ifeq ($(BOARD),$(filter $(BOARD), j721e_hostemu j721e_loki))
 SRCS_COMMON = sciclient_rm_csl.c sciclient_dummy.c
endif

CFLAGS_LOCAL_COMMON = $(PDK_CFLAGS)
PACKAGE_SRCS_COMMON += sciclient.h sciclient_component.mk makefile .gitignore
PACKAGE_SRCS_COMMON += include docs tools $(SRCDIR) soc/sciclient_soc_priv.h
PACKAGE_SRCS_COMMON += config_mk.bld package.bld package.xdc package.xs
PACKAGE_SRCS_COMMON += Settings.xdc.xdt sciclientver.h sciclientver.h.xdt

# Selectively package sysfw components
PACKAGE_SRCS_COMMON += soc/sysfw/binaries/system-firmware-public-documentation
PACKAGE_SRCS_COMMON += soc/sysfw/include
PACKAGE_SRCS_COMMON += soc/sysfw/binaries/ti-sci-firmware-am65x-gp.bin
PACKAGE_SRCS_COMMON += soc/sysfw/binaries/ti-sci-firmware-j721e-gp.bin
ifeq ($(BUILD_HS),yes)
PACKAGE_SRCS_COMMON += soc/sysfw/binaries/ti-sci-firmware-am65x-hs-cert.bin
PACKAGE_SRCS_COMMON += soc/sysfw/binaries/ti-sci-firmware-am65x-hs-enc.bin
endif

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

ifeq ($(OS),Windows_NT)
  EXE_EXT=.exe
endif
ifneq ("$(wildcard $(TOOLCHAIN_PATH_GCC)/bin/arm-none-eabi-objcopy$(EXE_EXT))","")
SCICLIENT_OBJCOPY?=$(TOOLCHAIN_PATH_GCC)/bin/arm-none-eabi-objcopy$(EXE_EXT)
endif
ifneq ("$(wildcard $(TOOLCHAIN_PATH_GCC_ARCH64)/bin/aarch64-elf-objcopy$(EXE_EXT))","")
SCICLIENT_OBJCOPY?=$(TOOLCHAIN_PATH_GCC_ARCH64)/bin/aarch64-elf-objcopy$(EXE_EXT)
endif
ifneq ("$(wildcard $(PDK_SBL_AUTO_COMP_PATH)/tools)","")
SBL_TOOLS_PATH=$(PDK_SBL_AUTO_COMP_PATH)/tools
else
SBL_TOOLS_PATH=$(PDK_SBL_COMP_PATH)/tools
endif
export SCICLIENT_OBJCOPY
ifeq ($(OS),Windows_NT)
  SCICLIENT_CERT_GEN=powershell -executionpolicy unrestricted -command $(ROOTDIR)/ti/build/makerules/x509CertificateGen.ps1
  BIN2C_EXE=bin2c.exe
else
  SCICLIENT_CERT_GEN=$(ROOTDIR)/ti/build/makerules/x509CertificateGen.sh
  BIN2C_EXE=bin2c.out
endif
BIN2C_GEN=$(ROOTDIR)/ti/drv/sciclient/tools/bin2c/$(BIN2C_EXE)
ifeq ($(BUILD_HS),yes)
SCICLIENT_CERT_KEY=$(ROOTDIR)/ti/build/makerules/k3_dev_mpk.pem
else
SCICLIENT_CERT_KEY=$(ROOTDIR)/ti/build/makerules/rom_degenerateKey.pem
endif
MAKE=gcc

src/sciclient_boardcfg.c : $(GEN_FILE)

$(GEN_FILE): soc/$(SCICLIENT_SOCVER)/sciclient_defaultBoardcfg.c soc/$(SCICLIENT_SOCVER)/sciclient_defaultBoardcfg_pm.c soc/$(SCICLIENT_SOCVER)/sciclient_defaultBoardcfg_rm.c soc/$(SCICLIENT_SOCVER)/sciclient_defaultBoardcfg_security.c
	echo "Build Board Configuration Obj Files"
	$(MKDIR) -p $(OBJDIR)
	$(CC) $(_CFLAGS) $(INCLUDES) $(CFLAGS_DIROPTS) $(COMPILEMODE) soc/$(SCICLIENT_SOCVER)/sciclient_defaultBoardcfg.c
	$(CC) $(_CFLAGS) $(INCLUDES) $(CFLAGS_DIROPTS) $(COMPILEMODE) soc/$(SCICLIENT_SOCVER)/sciclient_defaultBoardcfg_pm.c
	$(CC) $(_CFLAGS) $(INCLUDES) $(CFLAGS_DIROPTS) $(COMPILEMODE) soc/$(SCICLIENT_SOCVER)/sciclient_defaultBoardcfg_rm.c
	$(CC) $(_CFLAGS) $(INCLUDES) $(CFLAGS_DIROPTS) $(COMPILEMODE) soc/$(SCICLIENT_SOCVER)/sciclient_defaultBoardcfg_security.c
	echo "Create Binary" 
	$(SCICLIENT_OBJCOPY) -S -O binary $(OBJDIR)/sciclient_defaultBoardcfg.$(OBJEXT) $(OBJDIR)/sciclient_defaultBoardcfg.bin.unsigned
	$(SCICLIENT_OBJCOPY) -S -O binary $(OBJDIR)/sciclient_defaultBoardcfg_pm.$(OBJEXT) $(OBJDIR)/sciclient_defaultBoardcfg_pm.bin.unsigned
	$(SCICLIENT_OBJCOPY) -S -O binary $(OBJDIR)/sciclient_defaultBoardcfg_rm.$(OBJEXT) $(OBJDIR)/sciclient_defaultBoardcfg_rm.bin.unsigned
	$(SCICLIENT_OBJCOPY) -S -O binary $(OBJDIR)/sciclient_defaultBoardcfg_security.$(OBJEXT) $(OBJDIR)/sciclient_defaultBoardcfg_security.bin.unsigned
ifeq ($(OS),Windows_NT)
	$(CHMOD) a+x $(BIN2C_GEN)
endif
ifeq ($(BUILD_HS),yes)
ifneq ($(OS),Windows_NT)
	$(CHMOD) a+x $(SCICLIENT_CERT_GEN)
endif
	$(SCICLIENT_CERT_GEN) -b $(OBJDIR)/sciclient_defaultBoardcfg.bin.unsigned -o $(OBJDIR)/sciclient_defaultBoardcfg.bin.signed -k $(SCICLIENT_CERT_KEY) -f 1
	$(SCICLIENT_CERT_GEN) -b $(OBJDIR)/sciclient_defaultBoardcfg_pm.bin.unsigned -o $(OBJDIR)/sciclient_defaultBoardcfg_pm.bin.signed -k $(SCICLIENT_CERT_KEY) -f 1
	$(SCICLIENT_CERT_GEN) -b $(OBJDIR)/sciclient_defaultBoardcfg_rm.bin.unsigned -o $(OBJDIR)/sciclient_defaultBoardcfg_rm.bin.signed -k $(SCICLIENT_CERT_KEY) -f 1
	$(SCICLIENT_CERT_GEN) -b $(OBJDIR)/sciclient_defaultBoardcfg_security.bin.unsigned -o $(OBJDIR)/sciclient_defaultBoardcfg_security.bin.signed -k $(SCICLIENT_CERT_KEY) -f 1
	$(BIN2C_GEN) $(OBJDIR)/sciclient_defaultBoardcfg.bin.signed sciclient_defaultBoardcfg_hexhs.h SCICLIENT_BOARDCFG > $(ROOTDIR)/ti/drv/sciclient/soc/$(SCICLIENT_SOCVER)/sciclient_defaultBoardcfg_hexhs.h
	$(BIN2C_GEN) $(OBJDIR)/sciclient_defaultBoardcfg_pm.bin.signed sciclient_defaultBoardcfg_pm_hexhs.h SCICLIENT_PM_BOARDCFG > $(ROOTDIR)/ti/drv/sciclient/soc/$(SCICLIENT_SOCVER)/sciclient_defaultBoardcfg_pm_hexhs.h
	$(BIN2C_GEN) $(OBJDIR)/sciclient_defaultBoardcfg_rm.bin.signed sciclient_defaultBoardcfg_rm_hexhs.h SCICLIENT_RM_BOARDCFG > $(ROOTDIR)/ti/drv/sciclient/soc/$(SCICLIENT_SOCVER)/sciclient_defaultBoardcfg_rm_hexhs.h
	$(BIN2C_GEN) $(OBJDIR)/sciclient_defaultBoardcfg_security.bin.signed sciclient_defaultBoardcfg_security_hexhs.h SCICLIENT_SECURITY_BOARDCFG > $(ROOTDIR)/ti/drv/sciclient/soc/$(SCICLIENT_SOCVER)/sciclient_defaultBoardcfg_security_hexhs.h
else
	$(CP) $(OBJDIR)/sciclient_defaultBoardcfg.bin.unsigned $(OBJDIR)/sciclient_defaultBoardcfg.bin.signed
	$(CP) $(OBJDIR)/sciclient_defaultBoardcfg_pm.bin.unsigned $(OBJDIR)/sciclient_defaultBoardcfg_pm.bin.signed
	$(CP) $(OBJDIR)/sciclient_defaultBoardcfg_rm.bin.unsigned $(OBJDIR)/sciclient_defaultBoardcfg_rm.bin.signed
	$(CP) $(OBJDIR)/sciclient_defaultBoardcfg_security.bin.unsigned $(OBJDIR)/sciclient_defaultBoardcfg_security.bin.signed
	$(BIN2C_GEN) $(OBJDIR)/sciclient_defaultBoardcfg.bin.signed sciclient_defaultBoardcfg_hex.h SCICLIENT_BOARDCFG > $(ROOTDIR)/ti/drv/sciclient/soc/$(SCICLIENT_SOCVER)/sciclient_defaultBoardcfg_hex.h
	$(BIN2C_GEN) $(OBJDIR)/sciclient_defaultBoardcfg_pm.bin.signed sciclient_defaultBoardcfg_pm_hex.h SCICLIENT_PM_BOARDCFG > $(ROOTDIR)/ti/drv/sciclient/soc/$(SCICLIENT_SOCVER)/sciclient_defaultBoardcfg_pm_hex.h
	$(BIN2C_GEN) $(OBJDIR)/sciclient_defaultBoardcfg_rm.bin.signed sciclient_defaultBoardcfg_rm_hex.h SCICLIENT_RM_BOARDCFG > $(ROOTDIR)/ti/drv/sciclient/soc/$(SCICLIENT_SOCVER)/sciclient_defaultBoardcfg_rm_hex.h
	$(BIN2C_GEN) $(OBJDIR)/sciclient_defaultBoardcfg_security.bin.signed sciclient_defaultBoardcfg_security_hex.h SCICLIENT_SECURITY_BOARDCFG > $(ROOTDIR)/ti/drv/sciclient/soc/$(SCICLIENT_SOCVER)/sciclient_defaultBoardcfg_security_hex.h
endif

# OBJs and libraries are built by using rule defined in rules_<target>.mk
#     and need not be explicitly specified here

# Nothing beyond this point
